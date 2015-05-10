/*  NAME:
        videoNormalFlow_main.cpp
 
    DESCRIPTION:
        NormalFlowCalculator's application on video.
 
    COPYRIGHT:
        uriVisionLib Copyright  2004, uriVisionLib Developers.
 
 
 
        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2 of the License, or (at your option) any later version.
 
        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Lesser General Public License for more details.
 
        You should have received a copy of the GNU Lesser General Public
        License along with this library; if not, write to the Free Software
        Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/

/*--------------------------------------------------------------------------+
|	videoNormalFlow_main.cpp	application that demonstrates the use of    |
|                               the NormalFlowCalculator class              |
|																			|
|	Authors:	Meng Wan and jean-yves herve', 3D Group,                    |
|                           Department of Computer Science and Statistics	|
|								University of Rhode Island					|	
|																			|
|	Version	1.0 [05/31/2005]    jyh: Q&D test version	                    |
|	Version	1.1 [07/11/2005]	MW: rewritten with new BackgroundRemover	|
|	Version 1.2 [04/14/2007]	jyh: changed to work with new Movieplayer   |
|                                    simplified processing & display        |
|																			|
|	The upper-left quadrant shows the current frame						    |
|	The upper-right quadrant shows the normal flow		                    |
|	The lower-left quadrant shows the result of applying a high-pass filter |
|       (thresholding) to the normal flow.  The user is initialized         |
|       in initThresholds.                                                  |
|	The lower-right quadrant shows the result of a "find all blobs"			|
|		operation (minimum blob size is hard-coded in initThresholds)	    |
+--------------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
//
//	uriVisionLib headers
//
#include "ErrorReport.h"
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray_F.h"
#include "VectorField_F.h"
#include "ImageGradient_Gaussian_F.h"
#include "GrayThresholdLabeler.h"
#include "MoviePlayer.h"
#include "BlobFinder.h"
#include "NormalFlowCalculator.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------
//	Hack definition of movie files names
//----------------------------------------------
#if	URIVL_OS_MACOS
	const string movie_dir = "../../../../../../../../Data_Samples/Movies/";
#elif URIVL_OS_WIN32 || URIVL_OS_UNIX
	const string movie_dir = "../../../../../../Data_Samples/Movies/";
#else
    #error "OS not supported"
#endif

//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	2
#define	kNbColsSubwindows	2
//
#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640

//  names for my subwindows
const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1,
            LOWER_LEFT = 2,
            LOWER_RIGHT = 3;

//-----------------------------------------------------------------
//		Function prototypes
//-----------------------------------------------------------------

void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);

void setupOrthoCam(int w, int h);
void myIdle(void);
void myIdleDone(void);

void myDisplayMain(void);
void myDisplay0(void);
void myDisplay1(void);
void myDisplay2(void);
void myDisplay3(void);

void myReshape(int w, int h);
void myMouse(int button, int state, int ix , int iy);
void myKeyboard(unsigned char c, int x, int y);

void createRasterImageObj(void);
void initThresholds(void);
void glSetupAndRun(void);
int main(int argc, char** argv);



//-----------------------------------------------------------------
//  	Globals to be read from a configuration file (later)
//-----------------------------------------------------------------
int 	        gWinWidth,
                gWinHeight;
int				gMainWindow,
                gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];

bool	        stepByStep = false;
long	        gFrameIndex = 0;

NormalFlowCalculator *gNormFlowCalc = NULL;
RasterImage_RGBa	*gCurrentImage = NULL;
RasterImage_RGBa	*gPreviousImage = NULL;
RasterImage_RGBa    *gThresholdedImg = NULL;
RasterImage_gray_F  *gNormalFlow = NULL;
MoviePlayer			*gMyPlayer = NULL;

GrayThresholdLabeler	*gThresholder = NULL;
ColorSegParams		*gGrayParams = NULL;
LabelRaster			*gLabel = NULL;
BlobList 			*gBlobList = NULL;
BlobFinder			*gBlobFinder = NULL;
long				gBlobMinSize = 20;
char 				*gTarget = new char[1];

//-----------------------------------------------------------------
//		For menu handling demo
//-----------------------------------------------------------------
#define	kNbBckgndCols		6
//
GLfloat	kBackgroundColor[kNbBckgndCols][4] = {	{0.f, 0.f, 0.f, 1.f}, 	//	black
                                                {1.f, 0.f, 0.f, 1.f}, 	//	red
                                                {0.f, 1.f, 0.f, 1.f}, 	//	green
                                                {1.f, 1.f, 0.f, 1.f}, 	//	yellow
                                                {0.f, 1.f, 1.f, 1.f}, 	//	cyan
                                                {.1f, .1f, .1f, 1.f}
                                             };
#define	kBlackBackground		0
#define	kGrayBackground			5

//----------------------------------------------------------------------------
//		initializeQTEnvironment
//
// 		function initialize QuickTime Media Layer and QuickTime
//----------------------------------------------------------------------------
void initializeQTEnvironment(void)
{ 
	#if URIVL_USE_QUICKTIME
    OSErr	myErr = noErr;

    #if URIVL_OS_WIN32
        myErr = InitializeQTML(kQTMLNoIdleEvents);
    #else
        #if URIVL_OS_MACOS
            long	myAttrs;
            myErr = Gestalt(gestaltQuickTime, &myAttrs);
        #endif
    #endif

    FAIL_CONDITION(	myErr != noErr,
                    kQuickTimeNotInstalled,
                    "QuickTime is not installed on this computer");

    myErr = EnterMovies();
    FAIL_CONDITION(	myErr != noErr,
                    kEnterMoviesError,
                    "Problems with EnterMovies, returning errors (exit)");
	#endif
}


void cleanupAndQuit(int exitCode)
{
	#if URIVL_USE_QUICKTIME
    // terminate the QuickTime Media Layer
    ExitMovies();
    #endif

    #if URIVL_OS_WIN32
        TerminateQTML();
    #endif

    std::exit(exitCode);
}

//============================================================================================
//============================================================================================
#if 0
#pragma mark -
#endif


//---------------------------------------------------------------------------------
//		myIdle
//
//		OpenGL, called when idle by glut
//		contains playing the movie, updating the raster image,
//		and detecting the lines in the image
//---------------------------------------------------------------------------------
void myIdle(void)
{
    if (!gMyPlayer->isFinished())
    {
        //  Swap the images: What was the current image is now the previous image
        RasterImage_RGBa* bufImage = gPreviousImage;
        gPreviousImage	= gCurrentImage;
        gCurrentImage	= bufImage;
        
        gMyPlayer->getFrame(gCurrentImage);

        gFrameIndex++;

        if(gFrameIndex > 1)
        {
            //  compute normal flow
            gNormFlowCalc->applyInto(gCurrentImage, gPreviousImage, gNormalFlow);

            //-------------------------------------------------
            //	  threshold & blob_filling
            //-------------------------------------------------
            
            //	These two calls are only needed to get something to display as a background
            //	under the blobs
            gThresholder->applyInto(gNormalFlow, gGrayParams, gThresholdedImg);
            
            //	Apply gray-level thresholding and then detect sufficiently large blobs
            //	of high normal flow (presumably motion contours).
            gThresholder->applyInto(gNormalFlow, gGrayParams, gLabel, true);

            gBlobFinder->findAllBlobs(gLabel, gGrayParams->nbModes, gTarget,
                                      gBlobMinSize, gBlobList);

            for (int n=0; n<gGrayParams->nbModes ; n++)
            {
                if (!gBlobList[n].empty())
                {
                    BlobList::iterator iter;
                    for (iter = gBlobList[n].begin(); iter != gBlobList[n].end(); iter++)
                        (*iter)->setColor(1.f, 0.f, 0.f);

                }
            }
        }
    }
    else
        glutIdleFunc(myIdleDone);

    glutPostRedisplay();

    if (stepByStep)
        glutIdleFunc(myIdleDone);

}

//---------------------------------------------------------------------------------
//		myIdleDone
//
//		Called when the video sequence is over
//---------------------------------------------------------------------------------
void myIdleDone(void)
{
    glutPostRedisplay();
}



//---------------------------------------------------------------------------------
//		myMainReshape
//
//		OpenGL, used to reshape the main display window, calls the three other
//		display reshape methods
//---------------------------------------------------------------------------------

void myReshape(int w, int h)
{
    if ((w != gWinWidth) || (h != gWinHeight))
        glutReshapeWindow(gWinWidth, gWinHeight);

    glutPostRedisplay();
}


#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------------
//	myDisplay() functions
//---------------------------------------------------------------------------------


void myDisplayMain(void)
{
    glutSetWindow(gMainWindow);

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();

    myDisplay0();
    myDisplay1();
    myDisplay2();
    myDisplay3();
}


void myDisplay0(void)
{
    glutSetWindow(gSubwindow[UPPER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gFrameIndex > 0)
    {
        gCurrentImage->render();
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}


void myDisplay1(void)
{
    glutSetWindow(gSubwindow[UPPER_RIGHT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gFrameIndex > 1)
    {
        gNormalFlow->render();
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay2(void)
{
    glutSetWindow(gSubwindow[LOWER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gFrameIndex > 1)
    {
        gThresholdedImg->render();
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gFrameIndex > 1)
    {
        gCurrentImage->render();

        for (int k=0; k<gGrayParams->nbModes ; k++)
            if (!gBlobList[k].empty())
            {
                BlobList::iterator iter;
                for (iter = gBlobList[k].begin(); iter != gBlobList[k].end(); iter++)
                    (*iter)->render(true);

            }
    }
    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

#if 0
#pragma mark -
#endif



//---------------------------------------------------------------------------------
//		myKeyboard
//
//		glut, used for key controls during the execution of the program
//---------------------------------------------------------------------------------
void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 'q':
            cleanupAndQuit(0);
            break;

        case 'a':
            stepByStep = false;
            glutIdleFunc(myIdle);
            break;

        case 's':
            glutIdleFunc(NULL);
            break;


        default:
            break;

    }

    glutPostRedisplay();
}


//---------------------------------------------------------------------------------
//		myMouse
//
//		glut, used for mouse controls during the execution of the program
//---------------------------------------------------------------------------------


void myMouse(int button, int state, int ix , int iy)
{
    // stop start the playing of the images/movie in the display
    // if the left mouse button is clicked, do something
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        stepByStep = true;
        myIdle();
    }

    //	...and then refresh the display
    glutPostRedisplay();
}


#if 0
#pragma mark -
#endif


//---------------------------------------------------------------------------------
//		setupOrthoCam
//
//		This function maps world coordinates to screen coordinates.  This is where
//		the projection mode would be defined.  Here I only want to have a 2D
//		rectangle where to draw 2D stuff on top of my image, so I define my camera
//		as an orthographic camera, and I set the world coordinates to coincide with
//		the pixel coordinates (this is what the gluOrtho2D call does).
//---------------------------------------------------------------------------------
void setupOrthoCam(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(	0.0f, (GLfloat) w, 0.0f, (GLfloat) h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
}


#if 0
#pragma mark -
#endif

void initThresholds(void)
{
    gGrayParams = new ColorSegParams;
    gGrayParams->nbModes = 1;
    gGrayParams->colorMode = false;
    gGrayParams->colorReg = NULL;
    gGrayParams->grayReg = new GrayRegion[gGrayParams->nbModes];
    gGrayParams->fillMode = kOrigColorFill;

    gGrayParams->grayReg[0].labelStr =  "difference";
    gGrayParams->grayReg[0].vMin = 10;
    gGrayParams->grayReg[0].vMax = 255;
    gGrayParams->grayReg[0].fillColor[0] = 0;
    gGrayParams->grayReg[0].fillColor[1] = (unsigned char) 255;
    gGrayParams->grayReg[0].fillColor[2] = (unsigned char) 0;
    gGrayParams->grayReg[0].label = (char) 1;		//  must begin from n>0

    gTarget[0] = gGrayParams->grayReg[0].label;
}

//-----------------------------------------------------------------
//		createRasterImageObj
//
//		create the raster image objects and setup the rows,cols,pixels
//-----------------------------------------------------------------
void createRasterImageObj(void)
{
    const ImageRect *myRect = gMyPlayer->getRect();
    const int	nbRows = myRect->getHeight();
    const int	nbCols = myRect->getWidth();

    gCurrentImage = new RasterImage_RGBa(nbRows, nbCols);
    FAIL_CONDITION(	gCurrentImage == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");

    gPreviousImage = new RasterImage_RGBa(nbRows, nbCols);
    FAIL_CONDITION(	gPreviousImage == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");

    gNormalFlow = new RasterImage_gray_F(nbRows, nbCols);
    FAIL_CONDITION(	gNormalFlow == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");

    gThresholdedImg = new RasterImage_RGBa(nbRows, nbCols);
    FAIL_CONDITION(	gThresholdedImg == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");

    gLabel = new LabelRaster(nbRows, nbCols);
    FAIL_CONDITION(	gLabel == NULL,
                    kRasterImageError,
                    "Failed to allocate LabelRaster");

    gBlobList = new BlobList[gGrayParams->nbModes];
    gThresholder = new GrayThresholdLabeler(nbCols, nbRows);
    gBlobFinder = new BlobFinder();
}


//----------------------------------------------------------------------------
//  	glSetupAndRun
//
//		create the raster and line detector objects
//		setup the glut stuff
//		called from either cameraMain or movieMain
//----------------------------------------------------------------------------
void glSetupAndRun(void)
{
    int		winID;
    GLfloat	*backColor;
    void	(*displayFuncTable[kNbRowsSubwindows*kNbColsSubwindows+1])() =
                                        {myDisplay0, myDisplay1, myDisplay2, myDisplay3};

    initThresholds( );
    createRasterImageObj( );

    //	I initialize my window's dimensions at the default values
    int	imgHeight = gCurrentImage->getHeight();
    int	imgWidth = gCurrentImage->getWidth();

    //	I initialize my window's dimensions
    gWinHeight = kNbRowsSubwindows * (imgHeight + kBorder) + kBorder;
    gWinWidth = kNbColsSubwindows * (imgWidth + kBorder) + kBorder;

    glutInitWindowSize(gWinWidth, gWinHeight);
    gMainWindow = glutCreateWindow("Normal Flow Computation Demo");
    backColor = kBackgroundColor[kBlackBackground];

    glutDisplayFunc(myDisplayMain);
    setupOrthoCam(imgWidth, imgHeight);
    glutReshapeFunc(myReshape);
    glutIdleFunc(myIdle);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);

    winID = 0;
    glutSetWindow(gMainWindow);
    for (int i=0; i<kNbRowsSubwindows; i++)
        for (int j=0; j<kNbColsSubwindows; j++)
        {
            gSubwindow[winID] = glutCreateSubWindow(gMainWindow,
                                                    j*(imgWidth + kBorder) + kBorder,
                                                    i*(imgHeight + kBorder) + kBorder,
                                                    imgWidth, imgHeight);

            //	I don't need to do this.  I just set a different "clear" color for
            //	each subwindow.
            backColor = kBackgroundColor[kGrayBackground];
            glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

            //	This *has* to be defined:  a "display" callback function for each subwindow
            glutDisplayFunc(displayFuncTable[winID++]);
            glutMouseFunc(myMouse);
            glutKeyboardFunc(myKeyboard);

            //	This calls maps "world" coordinates to "screen" coordinates.
            //	In this particular case I make them coincide.
            setupOrthoCam(imgWidth, imgHeight);
            //
            glutSetWindow(gMainWindow);
        }

    glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);
    DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

    glutMainLoop();
}

//----------------------------------------------------------------------------
//  	Main
//----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    try {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

        initializeQTEnvironment( );

        gMyPlayer = new MoviePlayer();
        const string moviePath = movie_dir + "TFGreen1.mov";
        gMyPlayer->setSource(const_cast<char*>(moviePath.c_str()));

        ImageRect *myRect = const_cast<ImageRect*>(gMyPlayer->getRect());
        gNormFlowCalc = new NormalFlowCalculator(myRect, 2.f, 2.f);
        glSetupAndRun( );

        //  Normally this call should never be placed since the only way to leave the
        //  glut loop is to quit
        cleanupAndQuit(0);

        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
