/*  NAME:
        movieProcessing.cpp
 
    DESCRIPTION:
        demonstration application for IP operation on video data provided
        by a movie file.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|  Video processing demo application                                        | 
|																			|
|  This simple program demonstrate the use of a RasterImage object as a     |
|  destination for a movie being played and processed.                      |
|																			|
|   o The upper-left quadrant displays the video's current frame            |
|   o The upper-right quadrant displays a the gradient of the current frame |
|       overlaid on a gray-level copy of that frame                         |
|   o The lower-left quadrant displays either color blobs found (when       |
|       SEEK_COLOR_TARGETS is set to 1) or the norm of the gradient         |
|       (otherwise)                                                         |
|   o The lower-right quadrant displays a saturated color copy of the       |
|       current frame (not particularly useful, but add a a pretty note of  |
|       color to the display).                                              |
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|	
+--------------------------------------------------------------------------*/

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
//
//	uriVisionLib headers
//
#include "ErrorReport.h"
#include "RasterImage.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_HSV.h"
#include "RasterImage_gray_F.h"
#include "VectorField_F.h"
#include "ImageGradient_Gaussian_F.h"
#include "ColorThresholdLabeler.h"
#include "MoviePlayer.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;

//--------------------------------------------------------------------
//  If set to 1, the lower-left quadrant will display colored
//  blobs found (if an appropriate movie file was selected).  
//  Otherwise, the norm of the gradient is displayed
//--------------------------------------------------------------------
#define SEEK_COLOR_TARGETS  1

//--------------------------------------------------------------------

//  Select the type of image to read into.  One only!!
#define READ_IN_RGBA_IMAGE  1
#define READ_IN_RGB_IMAGE   0
#define READ_IN_GRAY_IMAGE  0
#if ( (READ_IN_RGBA_IMAGE && READ_IN_RGB_IMAGE) || \
      (READ_IN_RGBA_IMAGE && READ_IN_GRAY_IMAGE) || \
      (READ_IN_RGB_IMAGE && READ_IN_GRAY_IMAGE) )
    error --> pick only one kind of image
#endif
#if !(READ_IN_RGBA_IMAGE || READ_IN_RGB_IMAGE || READ_IN_GRAY_IMAGE)
    error --> pick one kind of image
#endif


//----------------------------------------------
//	Hack definition of movie files names
//----------------------------------------------
const string movie_dir = MOVIE_DIR_PATH;


//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	2
#define	kNbColsSubwindows	2
#define	kNbSubwindows       kNbRowsSubwindows*kNbColsSubwindows
#define	kNbImages			kNbSubwindows
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
void glSetupAndRun(void);
#if SEEK_COLOR_TARGETS
	void initThresholds(void);
#endif



//-----------------------------------------------------------------
//  	Globals to be read from a configuration file (later)
//-----------------------------------------------------------------
int	            gCurrentImg = 0, 	//	index of image used for display
                gWinWidth,
                gWinHeight;

int				gMainWindow,
                gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];

bool	        stepByStep = false;
long	        gFrameIndex = 0;

RasterImage_HSV	    *gMyImage = NULL;

#if SEEK_COLOR_TARGETS
// WHY???    RasterImage_HSV		*gMyImage2 = NULL;
    RasterImage_RGBa		*gMyImage2 = NULL;
	ColorSegParams		*gColorParams;
#else
    RasterImage_gray_F  *gNormGrad = NULL;
#endif

RasterImage_gray    *grayImg = NULL;
RasterImage_HSV     *gSaturatedImg = NULL;
VectorField_F       *gradient = NULL;
ImageGradient_Gaussian_F    *grad = NULL;

MoviePlayer			*gMyPlayer = NULL;

ColorThresholdLabeler	*gThresholder = NULL;


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
bool	gInitDone = false;
#define	kBlackBackground		0
#define	kGrayBackground			5


//=============================================================================
//=============================================================================


//----------------------------------------------------------------------------
//		initializeQTEnvironment
//
// 		function initialize QuickTime Media Layer and QuickTime
//----------------------------------------------------------------------------
void initializeQTEnvironment(void)
{
    #if URIVL_USE_QUICKTIME
        #if	URIVL_OS_WIN32
            OSErr	anErr = noErr;
            anErr = InitializeQTML(kQTMLNoIdleEvents);

            if (anErr != noErr)
            {
                cerr <<"\pQuickTime is not installed on this computer";
                ExitToShell();
            }


            anErr = EnterMovies();
            if(anErr != noErr)
            {
                cerr <<"\pProblems with EnterMovies, returning errors (exit)";
                ExitToShell();
            }
        #endif
    #endif
}

void cleanupAndQuit(int exitCode)
{
    #if URIVL_USE_QUICKTIME
        // terminate the QuickTime Media Layer
        ExitMovies();

        #if URIVL_OS_WIN32
            TerminateQTML();
        #endif
    #endif

    exit(exitCode);
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
        gMyPlayer->getFrame(gMyImage);

        gFrameIndex++;

        gMyImage->copyInto(grayImg);
        gMyImage->copyInto(gSaturatedImg);
        gSaturatedImg->forceConversionToSaturatedRGBa();
        grad->applyInto(grayImg, gradient);
        
        #if SEEK_COLOR_TARGETS
		    gThresholder->applyInto(gMyImage, gColorParams, gMyImage2);
        #else
            gradient->getNormAsRasterImage(gNormGrad);
        #endif
		
        //  Init is only completely done once I have have processed the first frame
        gInitDone = true;
    }
    else
        glutIdleFunc(myIdleDone);

    glutSetWindow(gMainWindow);
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
    glutSetWindow(gMainWindow);
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

    glutSetWindow(gMainWindow);
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

    if (gInitDone)
    {
        gMyImage->render();
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

    if (gInitDone)
    {
        grayImg->render();
        gradient->render(true);
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

    if (gInitDone)
    {
        #if SEEK_COLOR_TARGETS
            gMyImage2->render();
        #else
            gNormGrad->render();
        #endif
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gInitDone)
    {
        gSaturatedImg->render();
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

    //    glutPostRedisplay();
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

    if (state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON)
    {
        glutIdleFunc(NULL);
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

#if SEEK_COLOR_TARGETS
void initThresholds(void)
{
    gColorParams = new ColorSegParams;
    gColorParams->nbModes = 2;
    gColorParams->colorMode = true;
    gColorParams->grayReg = NULL;
    gColorParams->colorReg = new ColorRegion[gColorParams->nbModes];

	gColorParams->colorReg[0].labelStr = "orange";
    gColorParams->colorReg[0].hueRef = 0;
    gColorParams->colorReg[0].deltaHue = 1;
    gColorParams->colorReg[0].satMin = 10;
    gColorParams->colorReg[0].satMax = 255;
    gColorParams->colorReg[0].valMin = 10;
    gColorParams->colorReg[0].valMax = 255;
    gColorParams->colorReg[0].fillColor[0] = (unsigned char) 255;
    gColorParams->colorReg[0].fillColor[1] = (unsigned char) 127;
    gColorParams->colorReg[0].fillColor[2] = (unsigned char) 0;
	gColorParams->colorReg[0].label = (char) 2;

	gColorParams->colorReg[1].labelStr = "greenish";
	
    gColorParams->colorReg[1].hueRef = 120;
    gColorParams->colorReg[1].deltaHue = 20;
    gColorParams->colorReg[1].satMin = 20;
    gColorParams->colorReg[1].satMax = 255;
    gColorParams->colorReg[1].valMin = 30;
    gColorParams->colorReg[1].valMax = 255;
    gColorParams->colorReg[1].fillColor[0] = (unsigned char) 0;
    gColorParams->colorReg[1].fillColor[1] = (unsigned char) 255;
    gColorParams->colorReg[1].fillColor[2] = (unsigned char) 0;
    gColorParams->colorReg[1].label = (char) 1;

    gColorParams->fillMode = kOrigGrayFill;
	gColorParams->noMatchVal = 7;
    gThresholder = new ColorThresholdLabeler();
}
#endif

//-----------------------------------------------------------------
//		createRasterImageObj
//
//		create the raster image objects and setup the rows,cols,pixels
//-----------------------------------------------------------------
void createRasterImageObj(void)
{
    const ImageRect *myRect = gMyPlayer->getRect();
    int	nbRows = myRect->getHeight();
    int	nbCols = myRect->getWidth();

    //  The purpose of this application is to demonstrate how to apply to a
    //  video sequences a series of IP operation, including color thresholding,
    //  so I had better make this a RasterImage_HSV to begin with.  Depending
    //  on the type of operatoin you want to do, make the image you will be reading
    //  into a RasterImage_gray, a RasterImage_RGBa, etc.
    gMyImage = new RasterImage_HSV(nbRows, nbCols);
    FAIL_CONDITION(	gMyImage == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");

	#if SEEK_COLOR_TARGETS
//		gMyImage2 = new RasterImage_HSV(nbRows, nbCols);
		gMyImage2 = new RasterImage_RGBa(nbRows, nbCols);
		FAIL_CONDITION(	gMyImage2 == NULL,
						kRasterImageError,
						"Failed to allocate RasterImage");
	#endif
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
    void	(*displayFuncTable[kNbSubwindows])() = {myDisplay0, myDisplay1,
                                                    myDisplay2, myDisplay3};

    createRasterImageObj( );

	#if SEEK_COLOR_TARGETS
		initThresholds( );
	#endif
	
    //	I initialize my window's dimensions at the default values
    int	imgHeight = gMyImage->getHeight();
    int	imgWidth = gMyImage->getWidth();

    //	I initialize my window's dimensions
    gWinHeight = kNbRowsSubwindows * (imgHeight + kBorder) + kBorder;
    gWinWidth = kNbColsSubwindows * (imgWidth + kBorder) + kBorder;

    glutInitWindowSize(gWinWidth, gWinHeight);
    gMainWindow = glutCreateWindow("Movie Processing Demo");
    backColor = kBackgroundColor[kBlackBackground];
    glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

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

    DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

    //--------------
    //	Create rasters for Image processing
    //--------------
    grayImg = new RasterImage_gray(gMyImage->getBoundRect());

    grad = new ImageGradient_Gaussian_F(2.0);
    gradient = new VectorField_F(gMyImage->getBoundRect());
    gradient->setVectResolution(20);
	gradient->setColor(1.f, 0.f, 0.f);
    gSaturatedImg = new RasterImage_HSV(gMyImage->getBoundRect());
	//
	#if !SEEK_COLOR_TARGETS
		gNormGrad = new RasterImage_gray_F(gMyImage->getBoundRect());
	#endif
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

        //  Set the source using a file path
//        gMyPlayer->setSource(const_cast<char*>(moviePath.c_str()));
        //  set the source using the GUI
        gMyPlayer->setSource();

        glSetupAndRun( );

        glutMainLoop();

        //  Normally this call should never be placed since the only way to leave the
        //  glut loop is to quit
        cleanupAndQuit(0);

        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
