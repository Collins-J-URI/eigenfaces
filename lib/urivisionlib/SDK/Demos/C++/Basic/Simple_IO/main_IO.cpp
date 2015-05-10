/*  NAME:
        simpleIO.cpp

    DESCRIPTION:
        demonstration application for basic image I/O operations

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|	This small demo program is recycled from code I wrote for the computer	|
|	graphics course to show how to do texture mapping.						|
|	In the original program, the images were read from files or simply		|
|	generated to be mapped as textures on a graphic object.					|
|	In this revised version, they are now displayed in a glut window.		|
|																			|
|	You can cycle through the windows with the '[' and ']' keys.			|
|																			|
|	Note:  If you built this demo using QuickTime (rather than FreeImage	|
|	of the "hack reader/writer," a file selector window will pop up.  It	|
|	waits for you to open an image file (jpeg, gif, etc.).  This is only	|
|	the case with the QuickTime version, because QuickTime for windows		|
|	include a very primitive GUI library, so I exploit this in the demo		|
|	to show how to use a file selecter.  All the other images are read		|
|	using a file path (the way it's done with the non-QuickTime builds of	|
|	this demo).																|
|																			|
|	The important point to keep in mind is that in C/C++ our image is		|
|	a *1D* array of unsigned char.  If you want to manipulate your image	|
|	as a 2D array, you will have to create arrays for your row pointers.	|
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|
+--------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
//
#include "ImageReader.h"
#include "ImageWriter.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;            //  base raster image classes

//----------------------------------------------
//	Hack definition of window padding
//----------------------------------------------
#if URIVL_OS_UNIX
    const int   X_PAD = 0,
                Y_PAD = 1;
#else
    const int   X_PAD = 0,
                Y_PAD = 0;
#endif

//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
const int	kNbImages = 9;
//
const string color_dir = COLOR_DIR_PATH;
const string gray_dir = GRAY_DIR_PATH;
const string output_dir = OUTPUT_DIR_PATH;

//================================================================================
//	Note that all my input is from tga files.  This is simply
//	out of laziness because that's the only format supported
//	by the 3 current ImageReader implementations:
//	If you defined
//		- URIVL_USE_FREEIMAGE	 pretty much any image format
//		- URIVL_USE_QT			 same, minus PPM/PGM/PBM
//		- URIVL_USE_HACK_READER  only uncompressed tga, PPM, and PPM/PGM/PBM
//================================================================================
//const string kNameFile2 = color_dir + "umbrellas_480x400.tga";
const string kNameFile2 = color_dir + "mandrill_256x256.jpg";
const string kNameFile3 = color_dir + "umbrellas_480x400.png";
const string kNameFile4 = gray_dir + "Lena_320x320.png";
//const string kNameFile4 = gray_dir + "barcode.tga";

//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
void loadImages(void);
void myDisplay(void);
void myReshape(int w, int h);
void myMenu(int value);
void myMouse(int button, int state, int ix ,int iy);
void myKeyboard(unsigned char c, int x, int y);
void myInit(void);
void myIdle(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
int gCurrentImg = 0;	//	index of image used for display
//
RasterImage** gMyImage = NULL;	//	an array of pointers to Image objects

bool gInitDone = false;

const int   QUIT_MENU = 0,
            NEXT_IMAGE_MENU = 1,
            PREVIOUS_IMAGE_MENU = 2;


//=============================================================================
//	Function Implementations
//=============================================================================

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


void loadImages(void)
{
    long	idx;
    int	    n;

    gMyImage = new RasterImage*[kNbImages];


    //-------------------------------------------------------------
    //	Image 0 is created by computation --> fill the array with RGB values
    //-------------------------------------------------------------
    n = 256;
    //
    unsigned char	*tempBuf = new unsigned char[n * n * 3];

    if (tempBuf !=NULL)
    {
        for (int i=0; i<n; i++)
        {
            idx = 3 * n * i;

            for (int j=0; j<n; j++)
            {
                tempBuf[idx++] = static_cast<unsigned char>(i);
                tempBuf[idx++] = static_cast<unsigned char>(255-j);
                tempBuf[idx++] = static_cast<unsigned char>(0);
            }
        }
        gMyImage[0] = new RasterImage_RGB(n, n, tempBuf);
        delete tempBuf;
    }
    else
    {
        cout << "Generation of image 0 failed.";
        cleanupAndQuit(-1);
    }


    //-------------------------------------------------------------
    //	Image 1 is created by computation --> fill the array with RGBA values
    //  This code fragment demonstrate how to get direct access to the image's
    //  raster for initialization and other IP calculations
    //-------------------------------------------------------------
    n = 512;
    //
    //  the raster image is created at the desired dimensions
    gMyImage[1] = new RasterImage_RGBa(n, n);
    if (gMyImage[1] != NULL)
    {
        //  we get a reference to the image's base raster
        //  Here, we use the version of the getRaster family of functions that return a
        //  read-write reference
        unsigned char *const* rgba = gMyImage[1]->getRaster2D(R_W_ACCESS);

        //  just to play nice, we get the image's dimensions, although
        //  they should be both equal to n
        int   nbRows = gMyImage[1]->getHeight(),
              nbCols = gMyImage[1]->getWidth();
        //  from this point on
        for (int i=0; i<nbRows; i++)
            for (int j=0, l=0; j<nbCols; j++, l+=4)
            {
                rgba[i][l] = static_cast<unsigned char>(i/4);
                rgba[i][l+1] = static_cast<unsigned char>(j/4);
                rgba[i][l+2] = static_cast<unsigned char>(floor(127*(cos(0.06*i)*sin(0.1*j)+0.5)));
                rgba[i][l+3] = static_cast<unsigned char>(255);
            }

        //	By default, the RasterImage class adopts the "safe" mode for the definition
        //	of the valid data rectangle.  Since image1 was just allocated not initialized,
        //	its valid data rectangle was located at the upper left corner of the bounding
        //	rectangle, with width and height 0.  We must now set the correct values.
        gMyImage[1]->setValidRect(0, 0, nbCols, nbRows);
    }
    else
    {
        cout << "Generation of image 1 failed.";
        cleanupAndQuit(-1);
    }


    //-------------------------------------------------------------
    //	Image 2 is created by computation --> fill the array with Gray values
    //-------------------------------------------------------------
    n = 256;
    //
    gMyImage[2] = new RasterImage_gray(n, n);
    if (gMyImage[2] !=NULL)
    {
        //  Here again, we use the version of the getRaster family of functions that return a
        //  read-write reference
        unsigned char *const* gray = gMyImage[2]->getRaster2D(R_W_ACCESS);

        for (int i=0; i<n; i++)
            for (int j=0; j<n; j++)
                gray[i][j] = static_cast<unsigned char>(i>j ? i : j);

    }
    else
    {
        cout << "Generation of image 3 failed.";
        cleanupAndQuit(-1);
    }

    //-------------------------------------------------------------
    //	Image 3 is read from a TGA file
    //-------------------------------------------------------------
    //  I can create a reader by passing a file path
    ImageReader* myReader;
    myReader = new ImageReader(kNameFile2.c_str());

    gMyImage[3] = myReader->getFrame(true);
    delete myReader;

	if (gMyImage[3] ==NULL)
    {
        cout << "Loading of image 3 failed.";
        cleanupAndQuit(-1);
    }


    //-------------------------------------------------------------
    //	Image 4 is read from an image file
    //-------------------------------------------------------------
    //  I can also create the reader w/o passing any file info
    myReader = new ImageReader( );
 
	//  and later-on specify a file path or select the file to read
    //  through a GUI dialog

	//	File selection through GUI seems broken in Windows and is not implemented for Linux
	#if URIVL_OS_MACOS && URIVL_USE_QUICKTIME
		gMyImage[4] = ImageReader::readImage();
	#else
		myReader->setSource(const_cast<char*>(kNameFile3.c_str()));
		gMyImage[4] = myReader->getFrame(false);
	#endif
    if (gMyImage[4] ==NULL)
    {
        cout << "Generation of image 4 failed.";
        cleanupAndQuit(-1);
    }


    //-------------------------------------------------------------
    //	Image 5 is read from a file (gray-level image)
    //-------------------------------------------------------------
    //  I can also reuse the same ImageReader and simply set a new source
	gMyImage[5] = ImageReader::readImage((kNameFile4.c_str()));
    if (gMyImage[5] ==NULL)
    {
        cout << "Generation of image 5 failed.";
        cleanupAndQuit(-1);
    }

    //-------------------------------------------------------------
    //	Image 6 is an RGBa copy of a gray-level image, using directly the
    //  copyInto function.
    //-------------------------------------------------------------
    //  Let's say that I want to reuse a RasterImage allocated earlier.
    //  This one has bogus dimensions
    gMyImage[6] = new RasterImage_RGBa(100, 250);
    if (gMyImage[6] ==NULL)
    {
        cout << "Generation of image 6 failed.";
        cleanupAndQuit(-1);
    }
    //  since the destination of the copy does not have the same dimensions
    //  as the source, its base raster will get reallocated.
    gMyImage[5]->copyInto(gMyImage[6]);

    //-------------------------------------------------------------
    //	Image 7 is a gray-level copy of an RGBa  image
	//	Note that this copy also forces a resizing
    //-------------------------------------------------------------
    n = 256;
    gMyImage[7] = new RasterImage_gray(n, n);
    if (gMyImage[7] == NULL)
    {
        cout << "Generation of image 7 failed.";
        cleanupAndQuit(-1);
    }
	//	Unless image3 is also a 256x256 image, this will force a resizing of 
	//	image7's raster and rectangles
    gMyImage[3]->copyInto(gMyImage[7]);

    //-------------------------------------------------------------
    //	Image 8 is a gray-level copy of an RGBa  image, initialized by the constructor
    //-------------------------------------------------------------
    gMyImage[8] = new RasterImage_gray(gMyImage[4]);
    if (gMyImage[8] == NULL)
    {
        cout << "Generation of image 8 failed.";
        cleanupAndQuit(-1);
    }

    delete myReader;

	//================================================================================
	//	Re-enable this code if you want to write images to files
	//	Note that all my output goes to tga files.  This is simply
	//	out of laziness because that's the only format supported
	//	by the 3 current ImageWriter implementations:
	//	If you defined
	//		- URIVL_USE_FREEIMAGE	 pretty much any image format
	//		- URIVL_USE_QT			 same, minus PPM/PGM/PBM
	//		- URIVL_USE_HACK_READER  only uncompressed tga, PPM, and PPM/PGM/PBM
	//================================================================================
    //	Tests of image output
//    ImageWriter		*myWriter;
//
//    //  Similarly, I can create an ImageWriter with a destination specified by a path
//    myWriter = new ImageWriter((output_dir + "im0.jpg").c_str());
//    myWriter->outputFrame(gMyImage[0]);
//    delete myWriter;
//
//    //  or w/o any argument and set the destination later
//    myWriter = new ImageWriter( );
//    myWriter->setDestination((output_dir + "im0.jpg").c_str());
//    myWriter->outputFrame(gMyImage[0]);
//
//    //  and I can reuse the same ImageWriter with a new destination
//    myWriter->setDestination((output_dir + "im5.tga").c_str());
//    myWriter->outputFrame(gMyImage[5]);
//
//    //  and I can reuse the same ImageWriter with a new destination
//    myWriter->setDestination((output_dir + "im3.tga").c_str());
//    myWriter->outputFrame(gMyImage[4]);
//
//    myWriter->setDestination((output_dir + "im4.tga").c_str());
//    //  I can choose to write the image upside down
//    myWriter->outputFrame(gMyImage[4], true);
//    delete myWriter;
}



//---------------------------------------------------------------------------------
//	myDisplay()
//		Simply renders the "current" image in the window (using openGL functions)
//---------------------------------------------------------------------------------

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
        gMyImage[gCurrentImg]->render( );

    glutSwapBuffers();
}


void myReshape(int w, int h)
{
    if (gMyImage != NULL)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(	0.0f, (GLfloat) w, 0.0f, (GLfloat) h);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);

    //  and I inform drawable object of these dimensions (in case they want to
    //  get rendered "shifted")
    DrawableObject2D::setDefaultWindowDimensions(w, h);
}


void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 'q':
            cleanupAndQuit(0);
            break;

            //	[ and ] let me cycle through the images
        case '[':
            gCurrentImg = (gCurrentImg+kNbImages-1) % kNbImages;
            glutReshapeWindow(	gMyImage[gCurrentImg]->getWidth() + X_PAD,
                               gMyImage[gCurrentImg]->getHeight() + Y_PAD);
            break;
        case ']':
            gCurrentImg = (gCurrentImg + 1) % kNbImages;
            glutReshapeWindow(	gMyImage[gCurrentImg]->getWidth() + X_PAD,
                               gMyImage[gCurrentImg]->getHeight() + Y_PAD);
            break;

        default:
            break;

    }
    glutPostRedisplay();
}


void myMouse(int button, int state, int ix ,int iy)
{
    // if the left mouse button is clicked, do something
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        gCurrentImg = (gCurrentImg+kNbImages-1) % kNbImages;
        glutReshapeWindow(	gMyImage[gCurrentImg]->getWidth() + X_PAD,
                           gMyImage[gCurrentImg]->getHeight() + Y_PAD);

    }


    //	...and then refresh the display
    glutPostRedisplay();
}



void myMenu(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case NEXT_IMAGE_MENU:
            gCurrentImg = (gCurrentImg + 1) % kNbImages;
            glutReshapeWindow(	gMyImage[gCurrentImg]->getWidth() + X_PAD,
                               gMyImage[gCurrentImg]->getHeight() + Y_PAD);
            break;

        case PREVIOUS_IMAGE_MENU:
            gCurrentImg = (gCurrentImg+kNbImages-1) % kNbImages;
            glutReshapeWindow(	gMyImage[gCurrentImg]->getWidth() + X_PAD,
                               gMyImage[gCurrentImg]->getHeight() + Y_PAD);
            break;

        //	10-14 --> image selection
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            gCurrentImg = value - 10;
            glutReshapeWindow(	gMyImage[gCurrentImg]->getWidth() + X_PAD,
                               gMyImage[gCurrentImg]->getHeight() + Y_PAD);
            break;

    }

//    glutPostRedisplay();
}

void myInit(void)
{
    initializeQTEnvironment();

    loadImages();

    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Create Menus
    int menu, imageSubmenu;

    //	Submenu for direct image selection
    //	This is to show that I can use the same function to handle all submenus
    //	as long as they have unique indexes.  Still, it is probably a good idea
    //	to break down menu handling into several manageable-size functions.
    imageSubmenu = glutCreateMenu(myMenu);
    glutAddMenuEntry("0", 10);
    glutAddMenuEntry("1", 11);
    glutAddMenuEntry("2", 12);
    glutAddMenuEntry("3", 13);
    glutAddMenuEntry("4", 14);

    // Main menu that the submenus are connected to
    menu = glutCreateMenu(myMenu);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Image", imageSubmenu);
    glutAddMenuEntry("Next Image", NEXT_IMAGE_MENU);
    glutAddMenuEntry("Previous Image", PREVIOUS_IMAGE_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    gInitDone = true;
}

void myIdle(void)
{
	if (gInitDone) 
	{
	    glutPostRedisplay();
	}
}



int main(int argc, char** argv)
{
    try {
        glutInit(&argc, argv);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowSize(100,100);
        glutCreateWindow("Image IO & Display Demo");

        //  defines glut callback functions that will interrupt the glut loop
        glutDisplayFunc(myDisplay);
        glutReshapeFunc(myReshape);
        glutMouseFunc(myMouse);
        glutKeyboardFunc(myKeyboard);
		glutIdleFunc(myIdle);

        //  Initializes QuickTime and loads the images
        myInit();

        //  Having read the images, I can now resize my window to the appropriate dimensions
        glutReshapeWindow(gMyImage[0]->getWidth() + X_PAD,
                          gMyImage[0]->getHeight() + Y_PAD);

        //  Enter infinite glut loop
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
