/*  NAME:
        imageGradient.cpp
 
    DESCRIPTION:
        demonstration application for ImageGradient classes
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|	This simple application demonstrates the use of the ImageGradient       |
|   classes.                                                                |
|																			|
|  The UL quadrant shows the original image                                 |
|  the UR quadrant shows a gray-level copy of the original image            |
|  the LL corner shows the results of applying a Grad of Gaussian filter    |
|      (after a menu selection)                                             |
|  the LR corner shows the results of applying a Sobel Gradient mask        |
|      (after a menu selection)                                             |
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|	
+--------------------------------------------------------------------------*/


#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
//
#include "ImageReader.h"
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "VectorField_I.h"
#include "VectorField_F.h"
#include "ImageGradient_Gaussian_F.h"
#include "ImageGradient_Sobel.h"
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

const string filePath = gray_dir + "Lena_256x256.tga";


//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	2
#define	kNbColsSubwindows	2
#define	kNbSubwindows       kNbRowsSubwindows*kNbColsSubwindows
#define	kNbImages			2
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

const int   GAUSSIAN = 0;
const int   SOBEL = 1;

const int   QUIT_MENU = 0;
const int   SEPARATOR = 1000;
const int   RESET_IMAGES_MENU = 1;
//
const int   COARSER_RES_MENU = 101;
const int   FINER_RES_MENU = 102;
//
const int   NORM_AS_BKGND_MENU = 50;
//
const int   GRAD_AVE_OF_COLOR_COMPONENTS_MENU = 60;
const int   GRAD_LARGEST_COLOR_COMPONENT_MENU = 61;
//
const int   APPLY_SOBEL_MENU = 3;


//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
void loadImage(void);
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
short			    gWinWidth,
                    gWinHeight,
                    gImageWidth = kDefaultWidth,
                    gImageHeight = kDefaultHeight,
                    //
                    gCurrentFieldRes[2] = {30, 30};

int				    gMainWindow,
                    gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];

bool			    gInitDone = false,
                    gDrawStuff = false,
                    gDrawVectField = false;

unsigned char	    gImageTreshold = 100;

RasterImage**		gMyImage = NULL;	//	an array of references to Image objects
RasterImage_gray**	gGradientNorm = NULL;
RasterImage**		gBackgroundImage = NULL;
VectorField**		gMyGradient = NULL;
bool				gradComputed[2] = {false, false};
bool				gradNormIsBackground[2] = {false, false};


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
    ImageReader	myReader(const_cast<char*>(filePath.c_str()));

    gMyImage = new RasterImage*[kNbImages];

    //-------------------------------------------------------------
    //	We read Image 0 from a file
    //-------------------------------------------------------------
    gMyImage[UPPER_LEFT] = myReader.getFrame(true);
    if (gMyImage[UPPER_LEFT] ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath << endl;
        cleanupAndQuit(-1);
    }

    //	Image 1 is just Image 0 converted to gray level
    gMyImage[UPPER_RIGHT] = new RasterImage_gray(gMyImage[UPPER_LEFT]);

    gBackgroundImage = new RasterImage*[2];
    gBackgroundImage[GAUSSIAN] = gMyImage[UPPER_RIGHT];
    gBackgroundImage[SOBEL] = gMyImage[UPPER_RIGHT];

    gMyGradient = new VectorField*[2];
    gMyGradient[GAUSSIAN] = new VectorField_F(gMyImage[UPPER_RIGHT]->getBoundRect());
    gMyGradient[SOBEL] = new VectorField_I(gMyImage[UPPER_RIGHT]->getBoundRect());

    gGradientNorm = new RasterImage_gray*[2];
    gGradientNorm[GAUSSIAN] = gGradientNorm[SOBEL] = NULL;
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
}

void myDisplay2(void)
{
    glutSetWindow(gSubwindow[LOWER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
        gBackgroundImage[GAUSSIAN]->render(true);

    if (gradComputed[GAUSSIAN])
        gMyGradient[GAUSSIAN]->render(true);

    glutSwapBuffers();
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
        gBackgroundImage[SOBEL]->render(true);

    if (gradComputed[SOBEL])
        gMyGradient[SOBEL]->render(true);

    glutSwapBuffers();
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

        // This is not happening.  This is not happening. This is not...
        default:	
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

        //  Gaussian filter width selection
        case 3:
        case 5:
        case 7:
        case 9:
        case 11:
        case 21: {
				//  apply the gradient filter to the image
				ImageGradient_Gaussian_F grad(static_cast<int>(value));
				grad.applyInto(gMyImage[UPPER_RIGHT], gMyGradient[GAUSSIAN]);
				gMyGradient[GAUSSIAN]->forceRasterUpdate();
				gMyGradient[GAUSSIAN]->setDrawValidDataFrame(true);
				gMyGradient[GAUSSIAN]->setFrameColor(0.f, 0.f, 1.f);
				gMyGradient[GAUSSIAN]->setColor(0.f, 1.f, 1.f);
				gradComputed[GAUSSIAN] = true;
				//  also get the gradient's norm as a rasterimage
				//  The first time around, return a new RI_g_F, after that just write into
				//  the existing raster image
				if (gGradientNorm[GAUSSIAN] == NULL)
					gGradientNorm[GAUSSIAN] = new RasterImage_gray_F(gMyImage[UPPER_RIGHT]->getBoundRect());
				gMyGradient[GAUSSIAN]->getNormAsRasterImage(gGradientNorm[GAUSSIAN]);
            }
			break;

        case NORM_AS_BKGND_MENU:
            if (gradComputed[GAUSSIAN])
            {
                if (gradNormIsBackground[GAUSSIAN])
                {
                    gBackgroundImage[GAUSSIAN] = gMyImage[UPPER_RIGHT];
                    gradNormIsBackground[GAUSSIAN] = false;
                }
                else
                {
                    gBackgroundImage[GAUSSIAN] = gGradientNorm[0];
                    gradNormIsBackground[GAUSSIAN] = true;
                }
            }
            break;

        //	coarser resolution
        case COARSER_RES_MENU:
            if (gradComputed[GAUSSIAN])
            {
                if (gCurrentFieldRes[GAUSSIAN] < 60)
                    gCurrentFieldRes[GAUSSIAN] += 5;
                gMyGradient[GAUSSIAN]->setVectResolution(gCurrentFieldRes[GAUSSIAN]);
                gMyGradient[GAUSSIAN]->forceRasterUpdate();
            }
            break;

        //	finer resolution
        case FINER_RES_MENU:
            if (gradComputed[GAUSSIAN])
            {
                if (gCurrentFieldRes[GAUSSIAN] > 10)
                    gCurrentFieldRes[GAUSSIAN] -= 5;
                gMyGradient[GAUSSIAN]->setVectResolution(gCurrentFieldRes[GAUSSIAN]);
                gMyGradient[GAUSSIAN]->forceRasterUpdate();
            }
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }

	glutSetWindow(gMainWindow);
	glutPostRedisplay();
}



void myMenu3(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case APPLY_SOBEL_MENU: {
				//  apply the gradient filter to the image
				ImageGradient_Sobel grad;
				grad.applyInto(gMyImage[UPPER_RIGHT], gMyGradient[SOBEL]);
				gMyGradient[SOBEL]->forceRasterUpdate();
				gMyGradient[SOBEL]->setDrawValidDataFrame(true);
				gMyGradient[SOBEL]->setFrameColor(0.f, 0.f, 1.f);
				gMyGradient[SOBEL]->setColor(0.f, 1.f, 1.f);
				gradComputed[SOBEL] = true;

				//  also get the gradient's norm as a rasterimage
				//  The first time around, return a new RI_g_F, after that just write into
				//  the existing raster image
				if (gGradientNorm[SOBEL] == NULL)
					gGradientNorm[SOBEL] = new RasterImage_gray_F(gMyImage[UPPER_RIGHT]->getBoundRect());
				gMyGradient[SOBEL]->getNormAsRasterImage(gGradientNorm[SOBEL]);
            }
			break;

        case NORM_AS_BKGND_MENU:
            if (gradComputed[SOBEL])
                if (gradNormIsBackground[SOBEL])
                {
                    gBackgroundImage[SOBEL] = gMyImage[UPPER_RIGHT];
                    gradNormIsBackground[SOBEL] = false;
                }
                else
                {
                    gBackgroundImage[SOBEL] = gGradientNorm[1];
                    gradNormIsBackground[SOBEL] = true;
                }
            break;

        //	coarser resolution
        case COARSER_RES_MENU:
            if (gradComputed[SOBEL])
            {
                if (gCurrentFieldRes[SOBEL] < 60)
                    gCurrentFieldRes[SOBEL] += 5;
                gMyGradient[SOBEL]->setVectResolution(gCurrentFieldRes[SOBEL]);
                gMyGradient[SOBEL]->forceRasterUpdate();
            }
            break;

        //	finer resolution
        case FINER_RES_MENU:
            if (gradComputed[SOBEL])
            {
                if (gCurrentFieldRes[SOBEL] > 10)
                    gCurrentFieldRes[SOBEL] -= 5;
                gMyGradient[SOBEL]->setVectResolution(gCurrentFieldRes[SOBEL]);
                gMyGradient[SOBEL]->forceRasterUpdate();
            }
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }

	glutSetWindow(gMainWindow);
	glutPostRedisplay();
}


void myColorSubmenu(int colorIndex)
{
    GLfloat		*backColor;

    if ((colorIndex >= 0) && (colorIndex < kNbBckgndCols))
    {
        glutSetWindow(gMainWindow);

        backColor = kBackgroundColor[colorIndex];
        glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}


void myInit(void)
{
    // Create Menus
    int menuMain, menu[kNbSubwindows], colorSubmenu, gaussMenu;

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
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for upper-left quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[UPPER_LEFT]);
    menu[UPPER_LEFT] = glutCreateMenu(myMenu0);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for upper-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for lower-left quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_LEFT]);
    gaussMenu = glutCreateMenu(myMenu2);
    glutAddMenuEntry("3x3 gradient of Gaussian", 3);
    glutAddMenuEntry("5x5 gradient of Gaussian", 5);
    glutAddMenuEntry("7x7 gradient of Gaussian", 7);
    glutAddMenuEntry("9x9 gradient of Gaussian", 9);
    glutAddMenuEntry("11x11 gradient of Gaussian", 11);
    glutAddMenuEntry("21x21 gradient of Gaussian", 21);
    //
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddSubMenu("Apply Filter", gaussMenu);
    glutAddMenuEntry("Coarser resolution", COARSER_RES_MENU);
    glutAddMenuEntry("Finer resolution", FINER_RES_MENU);
    glutAddMenuEntry("Toggle image-gradient's norm as background", NORM_AS_BKGND_MENU);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for lower-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    menu[LOWER_RIGHT] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Apply Sobel image gradient", APPLY_SOBEL_MENU);
    glutAddMenuEntry("Coarser resolution", COARSER_RES_MENU);
    glutAddMenuEntry("Finer resolution", FINER_RES_MENU);
    glutAddMenuEntry("Toggle image-gradient's norm as background", NORM_AS_BKGND_MENU);
    glutAddMenuEntry("-", SEPARATOR);
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
    void	(*displayFuncTable[kNbSubwindows])() = {myDisplay0, myDisplay1, 
                                                    myDisplay2, myDisplay3};

   try 
   {
        glutInit(&argc, argv);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

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
        gMainWindow = glutCreateWindow("Image Gradients Demo");

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
        glutPostRedisplay();

        glutMainLoop();

        cleanupAndQuit(0);
    }
    catch (uriVL::ErrorReport & e) 
	{
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
        
	return 0;
}
