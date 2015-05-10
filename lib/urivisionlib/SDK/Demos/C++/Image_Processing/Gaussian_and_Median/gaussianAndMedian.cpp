/*  NAME:
        gaussianAndMedian.cpp
 
    DESCRIPTION:
        demonstration application for GaussianFilter classes
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

//  o The upper-left quadrant shows the input image
//  o The upper-right quadrant shows a gray-level copy of the input image
//  o the lower-left quadrant shows the effect of applying a Gaussian
//      filter to the gray-level image (right-click menu selection)
//  o the lower-right quadrant shows the effect of applying a Median
//      filter to the gray-level image (right-click menu selection)
//
//  Note [12/10/05]:    The implementation of the MedianFilter class is 
//     still embarassingly inefficient.  This is just a quick hack that
//     gets the basic functionality, and then I moved on to another
//     needed-right-now class and never got around to finishing this one.
//
//  Versions 1.0 & 1.1      jyh, 3DG [sometime in the distant past]
//  Version 1.2 [12/10/05]  jyh, 3DG
//        cleanup and uniformization with other demos

#include <iostream>
#include <cstdlib>
#include <cmath>
//
#include "ImageReader.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "MedianFilter_F.h"
#include "GaussianFilter_F.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
//
const string color_dir = COLOR_DIR_PATH;
const string gray_dir = GRAY_DIR_PATH;

const string filePath = color_dir + "mandrill_256x256.tga";


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

const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1,
            LOWER_LEFT = 2,
            LOWER_RIGHT = 3;

const int   QUIT_MENU = 0;
const int   RESET_IMAGES_MENU = 1;


//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
void loadImage(void);
void resetImages(void);
//
void myDisplayMain(void);
void myDisplay0(void);
void myDisplay1(void);
void myDisplay2(void);
void myDisplay3(void);
//
void myReshape(int w, int h);
void setupCamera(int w, int h);
//
void myMenuMain(int value);
void myMenu0(int value);
void myMenu1(int value);
void myMenu2(int value);
void myMenu3(int value);
//
void myColorSubmenu(int value);
void myKeyboard(unsigned char c, int x, int y);
void myInit(void);
void myIdle(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int         gWinWidth,
            gWinHeight,
            //
            gCurrentFieldRes = 30;

int			gMainWindow,
            gSubwindow[kNbSubwindows];

bool		gInitDone = false,
            gDrawStuff = false,
            gDrawVectField = false;

unsigned char	gImageTreshold = 100;

RasterImage     **gMyImage = NULL;	//	an array of references to Image objects



//
//	For menu handling demo
//
#define	kNbBckgndCols		5
//
GLfloat	kBackgroundColor[kNbBckgndCols][4] =	{	{0.f, 0.f, 0.f, 1.f},	//	black
                                                    {1.f, 0.f, 0.f, 1.f},	//	red
                                                    {0.f, 1.f, 0.f, 1.f},	//	green
                                                    {1.f, 1.f, 0.f, 1.f},	//	yellow
                                                    {0.f, 1.f, 1.f, 1.f}	//	cyan
                                             };
string	kBckgndColStr[] =	{
                                "Black",
                                "Red",
                                "Green" ,
                                "Yellow",
                                "Cyan"
                            };


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


void loadImage(void)
{
    gMyImage = new RasterImage*[kNbImages];

    //-------------------------------------------------------------
    //	We read Image 0 from a file
    //-------------------------------------------------------------
    ImageReader	myReader(const_cast<char*>(filePath.c_str()));
    gMyImage[UPPER_LEFT] = myReader.getFrame(true);
    if (gMyImage[UPPER_LEFT] ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath << endl;
        cleanupAndQuit(-1);
    }

//    int   m0 = gMyImage[UPPER_LEFT]->getHeight(),
//          n0 = gMyImage[UPPER_LEFT]->getWidth();

    //	Image 1 is just Image 0 converted to gray level
    gMyImage[UPPER_RIGHT] = new RasterImage_gray(gMyImage[UPPER_LEFT]);

    //	Images 2 and 3 are created with as gray_F images with the same bounding rectangle
    //  as image 0.  Initialized with the same values as Image 1
    gMyImage[LOWER_LEFT] = new RasterImage_gray_F(gMyImage[UPPER_RIGHT]);
    gMyImage[LOWER_LEFT]->setDrawValidDataFrame(false);
    gMyImage[LOWER_LEFT]->setFrameColor(0.f, 1.f, 1.f);
    gMyImage[LOWER_RIGHT] = new RasterImage_gray_F(gMyImage[UPPER_RIGHT]);
    gMyImage[LOWER_RIGHT]->setDrawValidDataFrame(false);
    gMyImage[LOWER_RIGHT]->setFrameColor(0.f, 1.f, 1.f);
}

void resetImages(void)
{
    gMyImage[UPPER_RIGHT]->copyInto(gMyImage[LOWER_LEFT]);
    gMyImage[UPPER_RIGHT]->copyInto(gMyImage[LOWER_RIGHT]);
    gMyImage[LOWER_LEFT]->setDrawValidDataFrame(false);
    gMyImage[LOWER_RIGHT]->setDrawValidDataFrame(false);
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
        gMyImage[UPPER_LEFT]->render();
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay1(void)
{
    glutSetWindow(gSubwindow[UPPER_RIGHT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
    {
        gMyImage[UPPER_RIGHT]->render();
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay2(void)
{
    glutSetWindow(gSubwindow[LOWER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
    {
        gMyImage[LOWER_LEFT]->render(true);
    }
    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
    {
        gMyImage[LOWER_RIGHT]->render(true);
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

#if 0
#pragma mark -
#endif

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
}

//	This function maps world coordinates to screen coordinates.  This is where
//	the projection mode would be defined.  Here I only want to have a 2D
//	rectangle where to draw 2D stuff on top of my image, so I define my camera
//	as an orthographic camera, and I set the world coordinates to coincide with
//	the pixel coordinates (this is what the gluOrtho2D call does).
void setupCamera(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(	0.0f, (GLfloat) w, 0.0f, (GLfloat) h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
}


void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 'q':
            cleanupAndQuit(0);
            break;

        default:
            break;

    }
}


void myMenuMain(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
}

void myMenu0(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case RESET_IMAGES_MENU:
            resetImages();
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
}

void myMenu1(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case RESET_IMAGES_MENU:
            resetImages();
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why is this happening?" << endl;
            break;

    }
}

void myMenu2(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case 3:
        case 5:
        case 7:
        case 9:
        case 11:
        case 13:
        case 21:
            GaussianFilter_F    *gauss;
            gauss = new GaussianFilter_F((int) value);
            gauss->applyInto(gMyImage[UPPER_RIGHT], gMyImage[LOWER_LEFT]);
            gMyImage[LOWER_LEFT]->setDrawValidDataFrame(true);
            delete gauss;
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
}


void myMenu3(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case 3:
        case 5:
        case 7:
        case 9:
        case 11:
        case 13:
            MedianFilter_F    *median;
            median = new MedianFilter_F((int) value);
            median->applyInto(gMyImage[UPPER_RIGHT], gMyImage[LOWER_RIGHT]);
            gMyImage[LOWER_RIGHT]->setDrawValidDataFrame(true);
            delete median;
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
}


void myColorSubmenu(int colorIndex)
{
    GLfloat		*backColor;

    if ((colorIndex >= 0) && (colorIndex < kNbBckgndCols))
    {
        glutSetWindow(gMainWindow);

        backColor = kBackgroundColor[colorIndex];
        glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

        myDisplay0();
        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}


void myInit(void)
{
    // Create Menus
    int menuMain, menu[kNbSubwindows], colorSubmenu, gaussMenu, medianMenu;

    //	Set a general menu for the main window
    glutSetWindow(gMainWindow);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    //	Submenu for background color selection choices
    colorSubmenu = glutCreateMenu(myColorSubmenu);
    for (int i=0; i<kNbBckgndCols; i++)
        glutAddMenuEntry(kBckgndColStr[i].c_str(), i);

    // Main menu that the submenus are connected to
    glutSetWindow(gMainWindow);
    menuMain = glutCreateMenu(myMenuMain);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for upper-left quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[UPPER_LEFT]);
    menu[UPPER_LEFT] = glutCreateMenu(myMenu0);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("Reset Images", RESET_IMAGES_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for upper-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("Reset Images", RESET_IMAGES_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for lower-left quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_LEFT]);
    //
    gaussMenu = glutCreateMenu(myMenu2);
    glutAddMenuEntry("3x3 Gaussian", 3);
    glutAddMenuEntry("5x5 Gaussian", 5);
    glutAddMenuEntry("7x7 Gaussian", 7);
    glutAddMenuEntry("9x9 Gaussian", 9);
    glutAddMenuEntry("11x11 Gaussian", 11);
    glutAddMenuEntry("13x13 Gaussian", 13);
    glutAddMenuEntry("21x21 Gaussian", 21);
    //
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Apply Filter", gaussMenu);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for lower-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    //
    medianMenu = glutCreateMenu(myMenu3);
    glutAddMenuEntry("3x3 median", 3);
    glutAddMenuEntry("5x5 median", 5);
    glutAddMenuEntry("7x7 median", 7);
    glutAddMenuEntry("9x9 median", 9);
    glutAddMenuEntry("11x11 median", 11);
    glutAddMenuEntry("13x13 median", 13);
    //
    menu[LOWER_RIGHT] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Apply Filter", medianMenu);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gMainWindow);

    gInitDone = true;
}

void myIdle(void)
{
	if (gInitDone) 
	{
		glutSetWindow(gMainWindow);
	    glutPostRedisplay();
	}
}



int main(int argc, char** argv)
{
    int		winID;
    GLfloat	*backColor;
    void	(*displayFuncTable[4])() = {myDisplay0, myDisplay1, 
                                        myDisplay2, myDisplay3};
    try 
	{
        glutInit(&argc, argv);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

        initializeQTEnvironment();

        //	I load my images
        loadImage();

        int	imgHeight = gMyImage[UPPER_LEFT]->getHeight(),
            imgWidth = gMyImage[UPPER_LEFT]->getWidth();

        //	I inform all drawable objects of the dimensions of their subwindow
        DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

        //	I initialize my window's dimensions
        gWinHeight = kNbRowsSubwindows * (imgHeight + kBorder) + kBorder;
        gWinWidth = kNbColsSubwindows * (imgWidth + kBorder) + kBorder;

        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Gaussian and Median Demo");

        glutKeyboardFunc(myKeyboard);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);
		glutIdleFunc(myIdle);

        glClearColor(0.f, 0.f, 0.f, 1.f);

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
                backColor = kBackgroundColor[winID];
                glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

                //	This *has* to be defined:  a "display" callback function for each subwindow
                glutDisplayFunc(displayFuncTable[winID++]);

                glutKeyboardFunc(myKeyboard);

                //	This calls maps "world" coordinates to "screen" coordinates.
                //	In this particular case I make them coincide.
                setupCamera(imgWidth, imgHeight);
                //
                glutSetWindow(gMainWindow);
            }

        myInit();
        glutSetWindow(gMainWindow);
        myDisplayMain();

        glutMainLoop();

        cleanupAndQuit(0);
    }
    catch (uriVL::ErrorReport & e) 
	{
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }

    return 0;
}
