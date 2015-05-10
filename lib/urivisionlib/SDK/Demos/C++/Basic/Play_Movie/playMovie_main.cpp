/*  NAME:
        playMovie_main.cpp
 
    DESCRIPTION:
        demonstration application playing a QT movie into a RasterImage object
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/



/*--------------------------------------------------------------------------+
|  Play Movie demo application                                              | 
|																			|
|  This simple program demonstrate the use of a RasterImage object as a     |
|  destination for a QuickTime movie being played.                          |
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|
|																			|
|	Ported to Linux by: Ian Lonngren										|
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
#include "MoviePlayer.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;


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

//============================================================================
//	Hack definition of movie files names
//============================================================================
const string movie_dir = MOVIE_DIR_PATH;


//============================================================================
//	Some GUI constants
//============================================================================

#define	kNbRowsSubwindows	1
#define	kNbColsSubwindows	1
//
#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640

//============================================================================
//		Function prototypes
//============================================================================

void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);

void myDisplay(void);
void setupOrthoCam(int w, int h);
void myIdle(void);
void myIdleDone(void);

void myReshape(int w, int h);
void myMouse(int button, int state, int ix , int iy);
void myKeyboard(unsigned char c, int x, int y);

void createRasterImageObj(void);
void glSetupAndRun(void);
int main(int argc, char** argv);



//============================================================================
//	File-level globals to be read from a configuration file (later)
//============================================================================

bool        stepByStep = false;
long        gFrameIndex = 0;

int         gWinWidth,
            gWinHeight;

#if READ_IN_RGBA_IMAGE
    RasterImage_RGBa	*gMyImage = NULL;
#else
#if READ_IN_RGB_IMAGE
    RasterImage_RGB	    *gMyImage = NULL;
#else
    RasterImage_gray	*gMyImage = NULL;
#endif
#endif

MoviePlayer     *gMyPlayer = NULL;


//-----------------------------------------------------------------
//		For menu handling demo
//-----------------------------------------------------------------
#define	kNbBckgndCols		6
//
GLfloat	kBackgroundColor[4] = {0.f, 0.f, 0.f, 1.f}; 	//	black

bool	gInitDone = false;


#if 0
#pragma mark -
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
    const ImageRect *myRect = gMyPlayer->getRect();
    int	nbRows = myRect->getHeight();
    int	nbCols = myRect->getWidth();

    #if READ_IN_RGBA_IMAGE
        gMyImage = new RasterImage_RGBa(nbRows, nbCols);
    #elif READ_IN_RGB_IMAGE
        gMyImage = new RasterImage_RGB(nbRows, nbCols);
    #else
        gMyImage = new RasterImage_gray(nbRows, nbCols);
    #endif

    FAIL_CONDITION(	gMyImage == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");

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

        //  do something cool
    }
    else 
	{
		//	do this to stop playing when we reach movie's end
        glutIdleFunc(myIdleDone);
		
		//	do that to go back to the beginning of the movie instead
		//	gFrameIndex = 0;
		//	gMyPlayer->goToBeginning();
	}
	
    glutPostRedisplay();

    if (stepByStep)
        glutIdleFunc(myIdleDone);

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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gFrameIndex>0)
    {
        gMyImage->render( );
    }
	
    glutSwapBuffers();
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

		//	's' pauses the movie playing
        case 's':
            glutIdleFunc(NULL);
            break;

		//	'a' resumes the movie playing
        case 'a':
            stepByStep = false;
            glutIdleFunc(myIdle);
            break;

		//	'r' restarts the movie at beginning
        case 'r':
			gMyPlayer->goToBeginning();
            glutIdleFunc(myIdle);
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



//----------------------------------------------------------------------------
//  	glSetupAndRun
//
//		create the raster and line detector objects
//		setup the glut stuff
//		called from either cameraMain or movieMain
//----------------------------------------------------------------------------
void glSetupAndRun(void)
{
    createRasterImageObj();

    //	I initialize my window's dimensions at the default values
    int     imgHeight = gMyImage->getHeight(),
            imgWidth = gMyImage->getWidth();

    //	I initialize my window's dimensions
    gWinHeight = imgHeight;
    gWinWidth = imgWidth;
    DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

    glutInitWindowSize(gWinWidth, gWinHeight);
    glutCreateWindow("Movie Playing in a RasterImage Demo");

    glClearColor(kBackgroundColor[0], kBackgroundColor[1], kBackgroundColor[2], kBackgroundColor[3]);
    glutDisplayFunc(myDisplay);
    setupOrthoCam(imgWidth, imgHeight);
    glutReshapeFunc(myReshape);
    glutIdleFunc(myIdle);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);

    //  Enter infinite glut loop
    glutMainLoop();
}


#if 0
#pragma mark -
#endif

//----------------------------------------------------------------------------
//  	Main
//----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    try {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		
        initializeQTEnvironment();

        gMyPlayer = new MoviePlayer();

string outPath = "../../../out.txt"; //movie_dir + "out.txt";
ofstream outFile(outPath.c_str());
outFile << "test" << endl;
outFile.close();

		//	QuickTime for Mac and Windows includes a rudimentary GUI file selector
		#if URIVL_USE_QUICKTIME
			gMyPlayer->setSource();
		//	at this point, for any other implementation, a file path is the only way
        #else
//			const string moviePath = movie_dir + "TFGreen1.mov";
			const string moviePath = "/SDKs/uriVisionLib/SDK/Data_Samples/Movies/TFGreen1.mov";
//			const string moviePath = "/Volumes/Development/SDKs/uriVisionLib/SDK/Data_Samples/Movies/TFGreen1.mov";
          
			gMyPlayer->setSource(moviePath.c_str());
		#endif
		
        glSetupAndRun();
        
        //  Normally this call should never be placed since the only way to leave the
        //  glut loop is to quit
        cleanupAndQuit(0);

        return 0;
    }
    catch (uriVL::ErrorReport & e) 
	{
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
