/*  NAME:
        videoGrabbing_main.cpp

    DESCRIPTION:
        demonstration application for frame grabbing operations

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|	This application shows how to create and start a FrameGrabber object	|
|	and obtain frames from this grabber.									|
|	Because a FrameGrabber is a VideoSource, just like an ImageReader or	|
|	a MoviePlayer, all you have to do once your FrameGRabber is properly	|
|	set up is called getFrame.  Once you have your RasterImage, it does not |
|	matter much which VideoSource you got it from, so you can apply any	of	|
|	the Image Processing or Computer Vision classes downstream from that	|
|	point																	|
|																			|
|	v. 1.0	May 10, 2008.	jyh, 3D Group, URI								|
|																			|
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
#include "RasterImage_gray.h"
#include "FrameGrabber.h"

using namespace std;
using namespace uriVL;


//============================================================================
//	Define video source configurations
//============================================================================

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
//	Some GUI constants
//============================================================================

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
void setupOrthoCam(int w, int h);
void myIdle(void);

void myReshape(int w, int h);
void myKeyboard(unsigned char c, int x, int y);

void initializeApplication(void);
void initGlut(void);
int main(int argc, char** argv);


//============================================================================
//	File-level global variables
//============================================================================

long			gFrameIndex = 0;
ImageRect		*gMyRect = NULL;
FrameGrabber    *gMyGrabber = NULL;
bool			gInitDone = false;

#if READ_IN_RGBA_IMAGE
		RasterImage_RGBa	*gInputImage = NULL;
#else
	#if READ_IN_RGB_IMAGE
		RasterImage_RGB	    *gInputImage = NULL;
	#else
		RasterImage_gray	*gInputImage = NULL;
	#endif
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
    gMyGrabber->getFrame(gInputImage);

	gFrameIndex++;
cout << "frame " << gFrameIndex << endl;

        //  do something cool

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
        gInputImage->render( );
    }


    glutSwapBuffers();
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


//---------------------------------------------------------------------------------
//		myReshape
//---------------------------------------------------------------------------------

void myReshape(int w, int h)
{
	if ((w != IMG_WIDTH) || (h != IMG_HEIGHT))
		glutReshapeWindow(IMG_WIDTH, IMG_HEIGHT);

	setupOrthoCam(w, h);
	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    DrawableObject2D::setDefaultWindowDimensions(w, h);
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
		
	ImageRect imgRect(0, 0, IMG_WIDTH, IMG_HEIGHT);

	#if READ_IN_RGBA_IMAGE
		gInputImage = new RasterImage_RGBa(&imgRect);
	#elif READ_IN_RGB_IMAGE
		gInputImage = new RasterImage_RGB(&imgRect);
	#else READ_IN_GRAY_IMAGE
		gInputImage = new RasterImage_gray(&imgRect);
	#endif
	gInputImage->setValidRect(&imgRect);
	
	gMyGrabber = new FrameGrabber(&imgRect);
	gMyGrabber->setSource();
}


void initGlut(void)
{
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glutDisplayFunc(myDisplay);
		setupOrthoCam(IMG_WIDTH, IMG_HEIGHT);
		glutReshapeFunc(myReshape);
		glutIdleFunc(myIdle);
		glutKeyboardFunc(myKeyboard);
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
		
		glutInitWindowSize(IMG_WIDTH, IMG_HEIGHT);
		glutCreateWindow("Video Capture Demo");
		initGlut();
		initializeApplication();
		
		gMyGrabber->start();

		glutMainLoop();

		cleanupAndQuit(0);
		return 0;
	}
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
