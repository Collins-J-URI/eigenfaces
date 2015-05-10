/*  NAME:
        main_movieVideoHybrid.cpp

    DESCRIPTION:
        demonstration application for movie playing & frame grabbing operations

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|	This demo brings nothing new that the Play_Movie and Video_Grabbing		|
|	applications had not shown before, except for the fact that it allows	|
|	the user to specify statically, via precompiler statement, what type of	|
|	video source to select and whether to display it in one window or in	|
|	four separate quadrants.												|
|																			|
|	v. 1.0	June 21, 2010.	jyh, 3D Group, URI								|
|																			|
+--------------------------------------------------------------------------*/


#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>

#if URIVL_OS_UNIX
	#include <quicktime/lqt.h>
#endif

//
//	uriVisionLib headers
//
#include "ErrorReport.h"
#include "RasterImage.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray.h"
#include "FrameGrabber.h"
#include "MoviePlayer.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;


//============================================================================
//	Hack definition of movie files names
//============================================================================
const string movie_dir = MOVIE_DIR_PATH;


//============================================================================
//	Define video source configurations
//============================================================================

//  Select the type of image to read into.  One only!!
#define READ_IN_RGBA_IMAGE  1
#define READ_IN_RGB_IMAGE   0
#define READ_IN_GRAY_IMAGE  0

//-----------------------------------------------------
//  Select the type of input.  One only!!
//-----------------------------------------------------
#define DEMO_LIVE_INPUT			1
#define DEMO_MOVIE_INPUT		0

//-----------------------------------------------------
//  Select the type of output.  One only!!
//-----------------------------------------------------
#define DEMO_ONE_PANEL_OUTPUT	1
#define DEMO_FOUR_PANEL_OUTPUT	0

//-----------------------------------------------------
//	Verification of the settings
//-----------------------------------------------------

#if ( (READ_IN_RGBA_IMAGE && READ_IN_RGB_IMAGE) || \
      (READ_IN_RGBA_IMAGE && READ_IN_GRAY_IMAGE) || \
      (READ_IN_RGB_IMAGE && READ_IN_GRAY_IMAGE) )
    error --> pick only one kind of image
#endif
#if !(READ_IN_RGBA_IMAGE || READ_IN_RGB_IMAGE || READ_IN_GRAY_IMAGE)
    error --> pick one kind of image
#endif
//
#if (DEMO_LIVE_INPUT && DEMO_MOVIE_INPUT)
	error --> pick only one type of input
#endif
#if !(DEMO_LIVE_INPUT || DEMO_MOVIE_INPUT)
	error --> pick one type of input
#endif
//
#if (DEMO_ONE_PANEL_OUTPUT && DEMO_FOUR_PANEL_OUTPUT)
	error --> pick only one type of input
#endif
#if !(DEMO_ONE_PANEL_OUTPUT || DEMO_FOUR_PANEL_OUTPUT)
	error --> pick one type of input
#endif



//============================================================================
//	Some GUI constants
//============================================================================

enum Quadrant {	UPPER_LEFT = 0,
				UPPER_RIGHT = 1,
				LOWER_LEFT = 2,
				LOWER_RIGHT = 3
};
//	In the future we should be able to get the "native" dimensions from the
//	sequence grabber.  Right now we have to set the dimensions ourselves
//	(this is a project-driven library)

const int	IMG_WIDTH = 640;
const int	IMG_HEIGHT = 480;

//============================================================================
//		Function prototypes
//============================================================================

void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);

void myDisplay(void);
//
#if DEMO_FOUR_PANEL_OUTPUT
	void myDisplay0(void);
	void myDisplay1(void);
	void myDisplay2(void);
	void myDisplay3(void);
#endif

void setupOrthoCam(int w, int h);
void myIdle(void);
void myIdleDone(void);

void myReshape(int w, int h);
void myKeyboard(unsigned char c, int x, int y);
void myMouse(int button, int state, int ix , int iy);

void initializeApplication(void);
void initGlut(void);
int main(int argc, char** argv);


//============================================================================
//	File-level global variables
//============================================================================

long				gFrameIndex = 0;
const ImageRect*	gInputRect = NULL;
bool				gInitDone = false;

#if READ_IN_RGBA_IMAGE
		RasterImage_RGBa	*gInputImage = NULL;
#else
	#if READ_IN_RGB_IMAGE
		RasterImage_RGB	    *gInputImage = NULL;
	#else
		RasterImage_gray	*gInputImage = NULL;
	#endif
#endif

#if DEMO_LIVE_INPUT
	FrameGrabber    *gMyGrabber = NULL;
#else
	MoviePlayer     *gMyPlayer = NULL;
#endif
//
VideoSource* gMySource = NULL;

//-------------------------------
//	Display
//-------------------------------

int     gWinWidth,
		gWinHeight;
int		gMainWindow;

#if DEMO_FOUR_PANEL_OUTPUT

	const int kNbRowsSubwindows = 2;
	const int kNbColsSubwindows	 = 2;

    int gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];

	const int	kBorder=  8;
	
	#if DEMO_LIVE_INPUT
		const float	SCALE_FACTOR = 0.8f;
	#else
//		const float	SCALE_FACTOR = 0.7f;
		const float	SCALE_FACTOR = 0.8f;
	#endif
	
#else
	const float	SCALE_FACTOR = 1.0f;

	const int kNbRowsSubwindows = 1;
	const int kNbColsSubwindows	 = 1;

#endif


//============================================================================================
//============================================================================================
#if 0
#pragma mark - QuickTime Setup
#endif

//----------------------------------------------------------------------------
//		initializeQTEnvironment
//
// 		function initialize QuickTime Media Layer and QuickTime
//----------------------------------------------------------------------------
void initializeQTEnvironment(void)
{
	#if URIVL_USE_QUICKTIME
		printf("using quicktime\n");
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
	 #else
	 	//do nothing?
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
	
    std::exit(exitCode);
}

//============================================================================================
//============================================================================================
#if 0
#pragma mark - Callback functions
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
    gMySource->getFrame(gInputImage);

	gFrameIndex++;
	cout << "frame " << gFrameIndex << endl;

	//  do something cool

    glutPostRedisplay();
}


//---------------------------------------------------------------------------------
//		myIdleDone
//
//		Called when the video sequnce is over
//---------------------------------------------------------------------------------
void myIdleDone(void)
{
    glutPostRedisplay();
}


//---------------------------------------------------------------------------------
//		myDisplay
//
//		OpenGL, call the three other display methods
//---------------------------------------------------------------------------------
void myDisplay(void)
{
    glutSetWindow(gMainWindow);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	#if DEMO_FOUR_PANEL_OUTPUT
		myDisplay0();
		myDisplay1();
		myDisplay2();
		myDisplay3();
	#else
		if (gFrameIndex>0)
		{
			gInputImage->render( );
		}
	#endif

    glutSwapBuffers();
}

#if DEMO_FOUR_PANEL_OUTPUT
void myDisplay0(void)
{
    glutSetWindow(gSubwindow[UPPER_LEFT]);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gInitDone)
    {
//		gInputImage->setRenderingScale(SCALE_FACTOR);
		if ((gFrameIndex>0) && (gInputImage != NULL)) {
			gInputImage->render(true);
		}
    }

	glutSwapBuffers();
    glutSetWindow(gMainWindow);
}

void myDisplay1(void)
{
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gInitDone)
    {
//		gInputImage->setRenderingScale(SCALE_FACTOR);
		if ((gFrameIndex>0) && (gInputImage != NULL)) {
			gInputImage->render(true);
		}
    }

	glutSwapBuffers();
    glutSetWindow(gMainWindow);
}

void myDisplay2(void)
{
    glutSetWindow(gSubwindow[LOWER_LEFT]);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gInitDone)
    {
//		gInputImage->setRenderingScale(SCALE_FACTOR);
		if ((gFrameIndex>0) && (gInputImage != NULL)) {
			gInputImage->render(true);
		}
    }

	glutSwapBuffers();
    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gInitDone)
    {
//		gInputImage->setRenderingScale(SCALE_FACTOR);
		if ((gFrameIndex>0) && (gInputImage != NULL)) {
			gInputImage->render(true);
		}
    }

	glutSwapBuffers();
    glutSetWindow(gMainWindow);
}
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


//---------------------------------------------------------------------------------
//		myReshape
//---------------------------------------------------------------------------------

void myReshape(int w, int h)
{
    if ((w != gWinWidth) || (h != gWinHeight))
        glutReshapeWindow(gWinWidth, gWinHeight);

    glutPostRedisplay();
}



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
	}
}


#if 0
#pragma mark -
#endif


//-----------------------------------------------------------------
//		initializeApplication
//
//	Create frame grabber and raster image objects
//-----------------------------------------------------------------
void initializeApplication(void)
{

	#if DEMO_LIVE_INPUT
		gInputRect = new ImageRect(0, 0, IMG_WIDTH, IMG_HEIGHT);
		gMyGrabber = new FrameGrabber(gInputRect);
		gMyGrabber->setSource();
		gMySource = gMyGrabber;
		
	#else
        gMyPlayer = new MoviePlayer();
        gMyPlayer->setSource();
		gMySource = gMyPlayer;

		gInputRect = gMyPlayer->getRect();
	#endif

	#if READ_IN_RGBA_IMAGE
		gInputImage = new RasterImage_RGBa(gInputRect);
	#elif READ_IN_RGB_IMAGE
		gInputImage = new RasterImage_RGB(gInputRect);
	#else READ_IN_GRAY_IMAGE
		gInputImage = new RasterImage_gray(gInputRect);
	#endif
	gInputImage->setValidRect(gInputRect);
	gInitDone = true;
}


//----------------------------------------------------------------------------
//  	Main
//----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    try {

		initializeQTEnvironment();
		
		initializeApplication();

		//	I initialize my window's dimensions
		#if DEMO_FOUR_PANEL_OUTPUT
			int imgHeight = static_cast<int>(SCALE_FACTOR * gInputRect->getHeight()),
				imgWidth = static_cast<int>(SCALE_FACTOR * gInputRect->getWidth());
			DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

			gWinHeight = 2 * imgHeight + 3 * kBorder;
			gWinWidth = 2 * imgWidth + 3 * kBorder;

		#else
			gWinHeight = gInputRect->getHeight();
			gWinWidth = gInputRect->getWidth();
			DrawableObject2D::setDefaultWindowDimensions(gWinWidth, gWinHeight);
		#endif
		
		//	Create the Main Window and set up its callback functions
		glutInitWindowSize(gWinWidth, gWinHeight);
		gMainWindow = glutCreateWindow("Live/Canned Video Hybrid Demo");
		glClearColor(0.1f, 0.3f, 0.3f, 1.f);
		glutDisplayFunc(myDisplay);
		setupOrthoCam(gWinWidth, gWinHeight);
		glutReshapeFunc(myReshape);
		glutIdleFunc(myIdle);
		glutMouseFunc(myMouse);
		glutKeyboardFunc(myKeyboard);
				
		//	Then (if needed) create the 4 quadrant subwindows
		#if DEMO_FOUR_PANEL_OUTPUT
			void	(*displayFuncTable[])() = {myDisplay0, myDisplay1,
											   myDisplay2, myDisplay3};
			int winID = 0;
			glutSetWindow(gMainWindow);
			for (int i=0; i<kNbRowsSubwindows; i++)
				for (int j=0; j<kNbColsSubwindows; j++)
				{
					gSubwindow[winID] = glutCreateSubWindow(gMainWindow,
															j*(imgWidth + kBorder) + kBorder,
															i*(imgHeight + kBorder) + kBorder,
															imgWidth, imgHeight);

					glClearColor(0.f, 0.f, 0.f, 1.f);

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
		
		#endif


		#if DEMO_LIVE_INPUT
			gMyGrabber->start();
		#endif

		glutMainLoop();

		#if DEMO_LIVE_INPUT
			delete gInputRect;
		#endif

		cleanupAndQuit(0);
		return 0;
	}
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
