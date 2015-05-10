/*  NAME:
        scaling.cpp
 
    DESCRIPTION:
        demonstration application for basic image scaling operations
 
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
|	The important point to keep in mind is that in C/C++ our image is		|
|	a *1D* array of unsigned char.  If you want to manipulate your image	|
|	as a 2D array, you will have to create arrays for your row pointers.	|
|																			|
|	Note that the image is displayed without scaling: the user needs to		|
|	resize the window manually too see an image larger than the window.		|
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|	
|																			|
+--------------------------------------------------------------------------*/

#include <iostream>
#include <cmath>
//
#include "ImageReader.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;

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
#define	kNbImages				5
//
const string color_dir = COLOR_DIR_PATH;
const string gray_dir = GRAY_DIR_PATH;

const string kNameFile2 = color_dir + "umbrellas_480x400.tga";
const string kNameFile3 = color_dir + "a-port-en-bessin_300x400.tga";
const string kNameFile4 = gray_dir + "barcode.tga";

//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
//
void loadImages(void);
//
void myDisplay(void);
void myReshape(int w, int h);
void setupCamera(int w, int h);
//
void myMenu(int value);
void myMouse(int button, int state, int ix ,int iy);
void myKeyboard(unsigned char c, int x, int y);
int myInit(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
int			gCurrentImg = 0;	//	index of image used for display
//
RasterImage		**gMyImage = NULL;	//	an array of references to Image objects

bool	gInitDone = false;

int	gScaleIndex = 10;	//	10 --> scale of 1.0

const int   QUIT_MENU = 0,
            NEXT_IMAGE_MENU = 1,
            PREVIOUS_IMAGE_MENU = 2;

#if 0
#pragma mark -
#endif

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

    std::exit(exitCode);
}

#if 0
#pragma mark -
#endif

void loadImages(void)
{
    long	idx;
    int	n;
    unsigned char	*tempBuf;
    ImageReader		*myReader;

    gMyImage = new RasterImage *[kNbImages];

    //-------------------------------------------------------------
    //	Image 0 is created by computation --> fill the array with RGB values
    //-------------------------------------------------------------
    n = 256;
    //
    tempBuf = new unsigned char[n * n * 3];

    if (tempBuf !=NULL)
    {
        for (int i=0; i<n; i++)
        {
            idx = 3 * n * i;

            for (int j=0; j<n; j++)
            {
                tempBuf[idx++] = (unsigned char) i;
                tempBuf[idx++] = (unsigned char) 255-j;
                tempBuf[idx++] = (unsigned char) 0;
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
    //	Image 1 is created by computation --> fill the array with RGB values
    //-------------------------------------------------------------
    n = 256;
    //  I create an image with n rows and 2n columns
    //
    tempBuf = new unsigned char[2 * n * n * 3];

    if (tempBuf !=NULL)
    {
        for (int i=0; i<n; i++)
        {
            idx = 6 * n * i;

            for (int j=0; j<2*n; j++)
            {

                tempBuf[idx++] = (unsigned char) 0;
                tempBuf[idx++] = (unsigned char) 255-j/2;
                tempBuf[idx++] = (unsigned char) 0;

            }
        }
        gMyImage[1] = new RasterImage_RGB(n, 2*n, tempBuf);
        delete tempBuf;
    }
    else
    {
        cout << "Generation of image 0 failed.";
        cleanupAndQuit(-1);
    }

    //-------------------------------------------------------------
    //	Image 2 is read from a color image file
    //-------------------------------------------------------------
    myReader = new ImageReader(kNameFile2.c_str());
    gMyImage[2] = myReader->getFrame(true);
    delete myReader;

    if (gMyImage[2] ==NULL)
    {
        cout << "Generation of image 2 failed.";
        cleanupAndQuit(-1);
    }

    //-------------------------------------------------------------
    //	Image 3 is read from a color image file
    //-------------------------------------------------------------
    myReader = new ImageReader(kNameFile3.c_str());
    gMyImage[3] = myReader->getFrame(true);
    delete myReader;

    if (gMyImage[3] ==NULL)
    {
        cout << "Generation of image 3 failed.";
        cleanupAndQuit(-1);
    }


    //-------------------------------------------------------------
    //	Image 4 is read from a gray-level image file
    //-------------------------------------------------------------
    myReader = new ImageReader(kNameFile4.c_str());
    gMyImage[4] = myReader->getFrame(false);
    delete myReader;

    if (gMyImage[4] ==NULL)
    {
        cout << "Generation of image 4 failed.";
        cleanupAndQuit(-1);
    }

}

#if 0
#pragma mark -
#endif


//---------------------------------------------------------------------------------
//	myDisplay()
//		See pdf file for explanations on the different drawing modes.
//---------------------------------------------------------------------------------

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
        gMyImage[gCurrentImg]->render();
    
    glutSwapBuffers();
}

void myReshape(int w, int h)
{
    setupCamera(w, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);

    //  and I inform drawable object of these dimensions (in case they want to get rendered
    //  "shifted")
    DrawableObject2D::setDefaultWindowDimensions(w, h);
}

void setupCamera(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(	0.0f, (GLfloat) w, 0.0f, (GLfloat) h);
}

#if 0
#pragma mark -
#endif

void myKeyboard(unsigned char c, int x, int y)
{
    float	theScale;

    switch (c)
    {
        case 'q':
            cleanupAndQuit(0);
            break;

            //	[ and ] let me cycle through the images
        case '[':
            gCurrentImg = (gCurrentImg+kNbImages-1) % kNbImages;
            gScaleIndex = static_cast<int>(floorf(10.f*gMyImage[gCurrentImg]->getRenderingScaleX()
                                            + 0.01f));
            theScale = gScaleIndex/10.f;
            glutReshapeWindow(static_cast<int>(roundf(theScale*gMyImage[gCurrentImg]->getWidth())) + X_PAD,
                              static_cast<int>(roundf(theScale*gMyImage[gCurrentImg]->getHeight())) + Y_PAD
                             );
            break;
        case ']':
            gCurrentImg = (gCurrentImg + 1) % kNbImages;
            gScaleIndex = static_cast<int>(floorf(10.f*gMyImage[gCurrentImg]->getRenderingScaleX()
                                            + 0.01f));
            theScale = gScaleIndex/10.f;
            glutReshapeWindow(static_cast<int>(roundf(theScale*gMyImage[gCurrentImg]->getWidth())) + X_PAD,
                              static_cast<int>(roundf(theScale*gMyImage[gCurrentImg]->getHeight())) + Y_PAD
                             );
            break;

            //	, and . (for < and >) decrease and increase the scale
        case ',':
            if (gScaleIndex>1)
                gMyImage[gCurrentImg]->setRenderingScale(--gScaleIndex/10.f);
            theScale = gScaleIndex/10.f;
            glutReshapeWindow(static_cast<int>(roundf(theScale*gMyImage[gCurrentImg]->getWidth())) + X_PAD,
                              static_cast<int>(roundf(theScale*gMyImage[gCurrentImg]->getHeight())) + Y_PAD
                             );
            break;
        case '.':
            if (gScaleIndex<20)
                gMyImage[gCurrentImg]->setRenderingScale(++gScaleIndex/10.f);
            theScale = gScaleIndex/10.f;
            glutReshapeWindow(static_cast<int>(roundf(theScale*gMyImage[gCurrentImg]->getWidth()))+ X_PAD,
                             static_cast<int>(roundf(theScale*gMyImage[gCurrentImg]->getHeight())) + Y_PAD
                             );
            break;

        case 's':
            cout << "image index = " << gCurrentImg << "\tscale index = " << gScaleIndex <<
            "\tscale = " << gMyImage[gCurrentImg]->getRenderingScaleX() << endl;
        default:
            break;

    }

    glutPostRedisplay();
}


void myMouse(int button, int state, int ix ,int iy)
{
    float  theScale;

    // if the left mouse button is clicked, do something
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        gCurrentImg = (gCurrentImg+kNbImages-1) % kNbImages;
        gScaleIndex = static_cast<int>(floorf(10.f*gMyImage[gCurrentImg]->getRenderingScaleX()
                                        + 0.01f));
        theScale = gScaleIndex/10.f;
        glutReshapeWindow(static_cast<int>(floorf(theScale*gMyImage[gCurrentImg]->getWidth())) + X_PAD,
                          static_cast<int>(floorf(theScale*gMyImage[gCurrentImg]->getHeight())) + Y_PAD
                         );

    }

    //	...and then refresh the display
    glutPostRedisplay();
}



void myMenu(int value)
{
    float	theScale;

    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case NEXT_IMAGE_MENU:
            gCurrentImg = (gCurrentImg + 1) % kNbImages;
            gScaleIndex = static_cast<int>(floorf(10.f*gMyImage[gCurrentImg]->getRenderingScaleX()
                                            + 0.01f));
            theScale = gScaleIndex/10.f;
            glutReshapeWindow(static_cast<int>(floorf(theScale*gMyImage[gCurrentImg]->getWidth())) + X_PAD,
                              static_cast<int>(floorf(theScale*gMyImage[gCurrentImg]->getHeight())) + Y_PAD
                             );
            break;

        case PREVIOUS_IMAGE_MENU:
            gCurrentImg = (gCurrentImg+kNbImages-1) % kNbImages;
            gScaleIndex = static_cast<int>(floorf(10.f*gMyImage[gCurrentImg]->getRenderingScaleX()
                                            + 0.01f));
            theScale = gScaleIndex/10.f;
            glutReshapeWindow(static_cast<int>(floorf(theScale*gMyImage[gCurrentImg]->getWidth())) + X_PAD,
                              static_cast<int>(floorf(theScale*gMyImage[gCurrentImg]->getHeight())) + Y_PAD
                             );
            break;

        //	10-14 --> image selection
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            gCurrentImg = value - 10;
            gScaleIndex = static_cast<int>(floorf(10.f*gMyImage[gCurrentImg]->getRenderingScaleX()
                                            + 0.01f));
            float	theScale = gScaleIndex/10.f;
            glutReshapeWindow(static_cast<int>(floorf(theScale*gMyImage[gCurrentImg]->getWidth())) + X_PAD,
                              static_cast<int>(floorf(theScale*gMyImage[gCurrentImg]->getHeight())) + Y_PAD
                             );
            break;

    }

    glutPostRedisplay();
}


int myInit(void)
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

    return false;
}


int main(int argc, char** argv)
{
    try {
        glutInit(&argc, argv);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowSize(100,100);
        glutCreateWindow("Image Scaling Demo");

        //  defines glut callback functions that will interrupt the glut loop
        glutDisplayFunc(myDisplay);
        glutReshapeFunc(myReshape);
        glutMouseFunc(myMouse);
        glutKeyboardFunc(myKeyboard);

        //  Initializes QuickTime and loads the images
        myInit();

        //  Having read the images, I can now resize my window to the appropriate dimensions
        glutReshapeWindow(gMyImage[0]->getWidth() + X_PAD,
                          gMyImage[0]->getHeight() + Y_PAD);

        //  Enter infinite glut loop
        glutMainLoop();

        //  Normally this code should never be placed since the only way to leave the
        //  glut loop is to quit
        cleanupAndQuit(0);

        return 0;
   }
   catch (uriVL::ErrorReport & e) {
      cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
   }
}
