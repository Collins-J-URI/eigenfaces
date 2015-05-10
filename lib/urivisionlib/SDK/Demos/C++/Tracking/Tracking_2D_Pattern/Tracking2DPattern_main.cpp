/*  NAME:
        Tracking2D_main.cpp
 
    DESCRIPTION:
       Tracker2D's application on video.
 
    COPYRIGHT:
        uriVisionLib Copyright © 2005, uriVisionLib Developers.
 
 
 
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

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <glut.h>
//
#if URIVL_OS_MACOS
#include <QuickTimeComponents.h>
#include <ImageCompression.h>
#endif
//
#include "ComFramework.h"
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
#include "MoviePlayer.h"
#include "ImageReader.h"
#include "PatternTracker2DController.h"

using namespace std;
using namespace uriVL;

//  Select the type of image to read into.  One only!!
#define READ_IN_RGBA_IMAGE  1
#define READ_IN_RGB_IMAGE   0
#define READ_IN_GRAY_IMAGE  0
#if ( (READ_IN_RGBA_IMAGE && READ_IN_RGB_IMAGE) || (READ_IN_RGBA_IMAGE && READ_IN_GRAY_IMAGE) \
|| (READ_IN_RGB_IMAGE && READ_IN_GRAY_IMAGE) )
error --> pick only one kind of image
#endif
#if !(READ_IN_RGBA_IMAGE || READ_IN_RGB_IMAGE || READ_IN_GRAY_IMAGE)
error --> pick one kind of image
#endif

// 11 15 22 25 movie1

//	movie2 (2 man)
//#define FIRST_FRAME_INDEX    166
//#define FIRST_FRAME_INDEX    135
//#define FIRST_FRAME_INDEX    38
//#define FIRST_FRAME_INDEX    31
//#define FIRST_FRAME_INDEX    20
//#define FIRST_FRAME_INDEX    1

//  movie2 (car & 1 man)
//#define FIRST_FRAME_INDEX    1310
//#define FIRST_FRAME_INDEX    1080
//#define FIRST_FRAME_INDEX    840
//#define FIRST_FRAME_INDEX    780
//#define FIRST_FRAME_INDEX    1172

//#define FIRST_FRAME_INDEX    1390
//#define FIRST_FRAME_INDEX    1112
//#define FIRST_FRAME_INDEX    1050
//#define FIRST_FRAME_INDEX    830


//	library1
//#define FIRST_FRAME_INDEX    1196
//#define FIRST_FRAME_INDEX    1116
//#define FIRST_FRAME_INDEX    108	//780//810
//#define FIRST_FRAME_INDEX    105	//780//810
//#define FIRST_FRAME_INDEX    530
//#define FIRST_FRAME_INDEX    1342

//	shortLib
#define FIRST_FRAME_INDEX	1

//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
const int   kNbRowsSubwindows = 2,
            kNbColsSubwindows = 2,
            kNbSubwindows = kNbRowsSubwindows * kNbColsSubwindows;

const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1,
            LOWER_LEFT = 2,
            LOWER_RIGHT = 3;

#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640

#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640

//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
//
#ifdef	MAC_VERSION
#define kRootPath		":"
#define kPathSeparator	":"
#define kUpRoot			"::"
#define kUpNext			":"
#else	//	Win32 or Unix version
#define kRootPath		"./"
#define kPathSeparator	"/"
#define kUpRoot			"../"
#define kUpNext			"../"
#endif
//
//
#ifndef	PI
#define	PI	3.14159265
#endif


//-----------------------------------------------------------------
//		Function prototypes
//-----------------------------------------------------------------

void initializeQTEnvironment(void);
void cleanupQT(void);

void setupOrthoCam(int w, int h);
void myIdle(void);
void myIdleDone(void);

void myDisplayMain(void);

void myDisplay0(void);
void myDisplay1(void);
void myDisplay2(void);
void myDisplay3(void);
void printBlobs(void);

void myReshape(int w, int h);
void myMenu(int value);
void myMouse(int button, int state, int ix , int iy);
void myKeyboard(unsigned char c, int x, int y);

void createRasterImageObj(void);
void glSetupAndRun(void);
int main(int argc, char** argv);



//-----------------------------------------------------------------
//  	Globals to be read from a configuration file (later)
//-----------------------------------------------------------------
int	    gWinWidth,
        gWinHeight;

GLuint  gMainWindow,
        gSubwindow[kNbSubwindows];

bool	        stepByStep = false;
long	        gFrameIndex = 0;
long            gLastPrintedFrame = -1;
ImageRect		*gMyRect = NULL;

#if READ_IN_RGBA_IMAGE
RasterImage_RGBa	*gMyImage = NULL;
#else
#if READ_IN_RGB_IMAGE
RasterImage_RGB	    *gMyImage = NULL;
#else
RasterImage_gray	*gMyImage = NULL;
#endif
#endif

#ifdef	MAC_VERSION
#define	filePath	":::::Data_Samples:Movies:trackingS02.tga"//background.tga"
#else
#define	filePath	"../../../../Data_Samples/Movies/trackingS02.tga"//Scene.tga"
#endif
ImageReader			*myReader;
RasterImage			*gBackgroundImage,
					*gEstimatedBackImg;
MoviePlayer			*gMyPlayer		= NULL;
ImagePoint_F		**gCurrentPos	= NULL;
PatternTracker2DList	*gTrackerList = NULL;
PatternTracker2DController	*gController = NULL;
ImagePoint			*gNewObjPoint	= NULL;
VectorField_F		*gMyField		= NULL;

const float RENDER_SCALE = 5.f;

#define DEBUG

//		Platform-specific
//-----------------------------------------------------------------
#if URIVL_OS_MACOS
int 			gAppResFile = kInvalidFileRefNum;	// file reference number
FSSpec			gAppFSSpec;							// file specification
Str255			gAppName;							// name of this application
#endif

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
}


void cleanupQT(void)
{
    // terminate the QuickTime Media Layer
    ExitMovies();

#if URIVL_OS_WIN32
    TerminateQTML();
#endif
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
    static int	frameNum=0;
    OSErr			anErr = noErr;
    OSType			mediaType = VideoMediaType;
    TimeValue		nextTime = -1;

    nextTime = gMyPlayer->getNextTime();

    if (nextTime > -1)
    {
        gMyPlayer->getFrame(gMyImage);

        gFrameIndex++;
		
cout << "Frame " << gFrameIndex << endl;

		///*
		// only for ISORA PAPER, which reads first bkgd img from video
		if (gFrameIndex == 1) //530)//780)
			gController = new PatternTracker2DController(gMyImage);
		//*/
		
        //  apply PatternTracker2DController to render the trace of the moving objects
		if (gFrameIndex > FIRST_FRAME_INDEX)
		{
			gController->action(gMyImage);
			gTrackerList = gController->getCurrTrackerList();
			gEstimatedBackImg = gController->getCurrentBackground();
	        
	        #ifdef DEBUG
	        cout << "gTrackerList->size() = " << gTrackerList->size() << endl;
//if (gTrackerList->size() > 0)
//    printBlobs();
   
	        /*
	        PatternTracker2DList::iterator iter = gTrackerList->begin();
	        for( ; iter != gTrackerList->end(); iter++)
	        	cout << "\n xi = " << (*iter)->getCurrState()->getPos()->getX() << endl;
	        cout << "new frame !" << endl;
	        */
	        #endif
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


///*	for ICPR paper
void myDisplay0(void)
{
    glutSetWindow(gSubwindow[UPPER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone && gFrameIndex > FIRST_FRAME_INDEX)
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

	RasterImage_gray	*diffImage = NULL;
	
    if (gInitDone && gFrameIndex > FIRST_FRAME_INDEX)
    {
    	if (gTrackerList->size() >= 1)
    	{
	    	PatternTracker2DList::iterator iter = gTrackerList->begin();
	    	diffImage = (*iter)->getDifferenceImage();
    	}
    	if (diffImage == NULL)
			diffImage = (gController->getDifferImg());
		
		diffImage->render();
    }
    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay2(void)
{
    int	imgHeight = gMyImage->getHeight();

    glutSetWindow(gSubwindow[LOWER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     if (gInitDone && gFrameIndex > FIRST_FRAME_INDEX)
    {
		list<PatternTracker2D *> *currTrackerList = gController->getCurrTrackerList();
		list<PatternTracker2D *>::iterator	iter = currTrackerList->begin(),
											endIter = currTrackerList->end();

		for ( ; iter!=endIter; iter++)
		{
			Pattern2D *pat = (*iter)->getCurrPattern();
			TrackingState *currState = (*iter)->getCurrState();
			pat->setContourColor(1.f, 1.f, 0.f);
			pat->setDrawContour(true);
		    pat->setDrawValidDataFrame(true);
		    pat->setFrameColor(1.f, 1.f, 0.f);
		    ImagePoint origin((int)(currState->getPos()->getX()), 
		    				  (int)(currState->getPos()->getY()));
            pat->setRenderingScale(RENDER_SCALE, RENDER_SCALE);
			pat->render(-1.f, false);
            pat->setRenderingScale(1, 1);
		}
	}

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gInitDone && gFrameIndex > FIRST_FRAME_INDEX)
    {
    	//	display the tracked object on current background
		(gController->getUpperBackground())->render();
		
		//	display bounding rectangle
		list<PatternTracker2D *> *currTrackerList = gController->getCurrTrackerList();
		list<PatternTracker2D *>::iterator	iter = currTrackerList->begin(),
											endIter = currTrackerList->end();
		
		TrackingState *currState = NULL;
		Contour_I	*ct = NULL;
		
		for ( ; iter!=endIter; iter++)
		{
			currState = (*iter)->getCurrState();
			
			ct = (*iter)->getCurrPattern()->getBlob()->getContour();
			ct->setColor(1.f, 1.f, 0.f);
		    ct->setDrawValidDataFrame(true);
		    ct->setFrameColor(1.f, 1.f, 0.f);
		    ImagePoint origin((int)(currState->getPos()->getX()), 
		    				  (int)(currState->getPos()->getY()));

            ct->setRenderingScale(1, 1);
			ct->render(-1.f, true, &origin);
		}
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void printBlobs(void)
{
    if (gFrameIndex != gLastPrintedFrame)
    {
    	list<PatternTracker2D *> *currTrackerList = gController->getCurrTrackerList();
    	list<PatternTracker2D *>::iterator	iter = currTrackerList->begin(),
    										endIter = currTrackerList->end();

        //  For each pattern currently tracked
    	for ( ; iter!=endIter; iter++)
    	{
    		Pattern2D *pat = (*iter)->getCurrPattern();
            LineSegDeque *segDeque = pat->getBlob()->getSegDeque();
            
            //  print info about that pattern's blob
            cout << "\t----------------------------------"<<endl;
            deque< list<HorizontalSegment*> *>::iterator lineIter, lastLine = segDeque->end();
            for (lineIter = segDeque->begin(); lineIter != lastLine; lineIter++)
            {
                cout << "\t\ty = " << (*((*lineIter)->begin()))->y << ": ";
                list<HorizontalSegment*>::iterator  segIter, lastSeg = (*lineIter)->end();
                for (segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
                {
                    HorizontalSegment   *theSeg;
                    theSeg = (*segIter);
                    cout << "<" << theSeg->leftX << ", " << theSeg->rightX << ">  ";
                }
                cout << endl << flush;
            }
            cout << "\t----------------------------------"<<endl;
    	}

    	gLastPrintedFrame = gFrameIndex;
    }
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
            cleanupQT();
            std::exit(0);
            break;

        case 'a':
            stepByStep = false;
            glutIdleFunc(myIdle);
            break;

        case 's':
            glutIdleFunc(myIdleDone);
            break;

        case 'b':
            printBlobs();
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


//---------------------------------------------------------------------------------
//		myMenu
//
//		glut, sets up menus
//---------------------------------------------------------------------------------
void myMenu(int value)
{
    switch (value)
    {
            //	0 --> Exit/Quit
        case 0:
            std::exit(0);
            break;

            //	1 --> Next image
        case 1:
            break;

            //	2 --> Previous image
        case 2:
            break;

            //	10-14 --> image selection
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            break;

    }

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

//-----------------------------------------------------------------
//		createRasterImageObj
//
//		create the raster image objects and setup the rows,cols,pixels
//-----------------------------------------------------------------
void createRasterImageObj(void)
{
	/* ONLY not for ISORA PAPER
	myReader = new ImageReader(filePath);
	gBackgroundImage = myReader->getFrame(true);
	gController = new PatternTracker2DController(gBackgroundImage);
	*/

    gMyPlayer = new MoviePlayer();
    gMyPlayer->setSource();
    gMyRect = gMyPlayer->getRect();

    int	nbRows = gMyRect->getHeight();
    int	nbCols = gMyRect->getWidth();

	gNewObjPoint = new ImagePoint(0, 0);

#if READ_IN_RGBA_IMAGE
    gMyImage = new RasterImage_RGBa(nbRows, nbCols);
    //gMyImage = new RasterImage_HSV(nbRows, nbCols);
#else
#if READ_IN_RGB_IMAGE
    gMyImage = new RasterImage_RGB(nbRows, nbCols);
#else
    gMyImage = new RasterImage_gray(nbRows, nbCols);
#endif
#endif

    FAIL_CONDITION(	gMyImage == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");
    /* ONLY not for ISORA PAPER
    FAIL_CONDITION(	gBackgroundImage == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");
	*/
	
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
    void	(*displayFuncTable[kNbSubwindows])() =
                                {myDisplay0, myDisplay1, myDisplay2, myDisplay3};

    createRasterImageObj( );

    //	I initialize my window's dimensions at the default values
    int	imgHeight = gMyImage->getHeight();//320;
    int	imgWidth = gMyImage->getWidth();//240;

    //	I initialize my window's dimensions
    gWinHeight = 2 * imgHeight + 3 * kBorder;
    gWinWidth = 2 * imgWidth + 3 * kBorder;

    glutInitWindowSize(gWinWidth, gWinHeight);
    gMainWindow = glutCreateWindow("2D Pattern Tracking");
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
            glutDisplayFunc(displayFuncTable[winID]);
            glutMouseFunc(myMouse);
            glutKeyboardFunc(myKeyboard);
            winID++;

            //	This calls maps "world" coordinates to "screen" coordinates.
            //	In this particular case I make them coincide.
            setupOrthoCam(imgWidth, imgHeight);
            //
            glutSetWindow(gMainWindow);
        }


    DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

	
//   
//       grayImg = new RasterImage_gray(gCurrentImage->getBoundRect());
//    grad = new ImageGradient_Gaussian_F(2.0);
//    gradient = new VectorField_F(gCurrentImage->getBoundRect());
//       gNormGrad = new RasterImage_gray_F(gCurrentImage->getBoundRect());
//       gThresholdedImg = new RasterImage_RGBa(gCurrentImage->getBoundRect());
//   

    gInitDone = true;


    glutMainLoop();
}

//----------------------------------------------------------------------------
//  	Main
//----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    initializeQTEnvironment( );

    glSetupAndRun( );

    cleanupQT( );

    return 0;
}
