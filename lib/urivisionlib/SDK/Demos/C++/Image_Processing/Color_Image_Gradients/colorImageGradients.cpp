/*  NAME:
        colorImageGradient.cpp
 
    DESCRIPTION:
        demonstration application for ColorImageGradient classes
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|	This simple application demonstrates the use of the ColorImageGradient  |
|   classes.                                                                |
|																			|
|  The UL quadrant shows the original image                                 |
|  the UR quadrant shows a gray-level copy of the original image            |
|  the LL corner shows the results of applying a Grad of Gaussian filter    |
|      (after a menu selection)                                             |
|  the LR corner shows the results of applying a Sobel Gradient mask        |
|      (after a menu selection)                                             |
|  The color gradient is shown as red needles, the "gray" gradient as blue  |
|   needles.  The user can select one of the two modes of color gradient    |
|   calculation (average of the 3 components or largest gradient            |
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|	
+--------------------------------------------------------------------------*/

#include <fstream>
#include <string>
#include <iostream>
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
#include "ColorImageGradient_Gaussian_F.h"
#include "ImageGradient_Sobel.h"
#include "ColorImageGradient_Sobel.h"
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

const int   SOBEL = 1;
const int   GAUSSIAN = 0;

const int   QUIT_MENU = 0;
const int   RESET_IMAGES_MENU = 1;
//
const int   COARSER_RES_MENU = 101;
const int   FINER_RES_MENU = 102;
//
const int   COLOR_IMAGE_AS_BKGND_MENU = 50;
const int   GRAY_IMAGE_AS_BKGND_MENU = 51;
const int   COLOR_NORM_AS_BKGND_MENU = 52;
const int   GRAY_NORM_AS_BKGND_MENU = 53;
//
const int   GRAD_AVE_OF_COLOR_COMPONENTS_MENU = 60;
const int   GRAD_LARGEST_COLOR_COMPONENT_MENU = 61;
//
const int   APPLY_SOBEL_MENU = 3;


#define	kNbBckgndCols		5
//
GLfloat	kBackgroundColor[kNbBckgndCols][4] =	{	{0.f, 0.f, 0.f, 1.f},	//	black
                                                    {1.f, 0.f, 0.f, 1.f},	//	red
                                                    {0.f, 1.f, 0.f, 1.f},	//	green
                                                    {1.f, 1.f, 0.f, 1.f},	//	yellow
                                                    {0.f, 1.f, 1.f, 1.f}	//	cyan
                                             };
string kBckgndColStr[] =	{
                                "Black",
                                "Red",
                                "Green" ,
                                "Yellow",
                                "Cyan"
                            };

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
void cleanupAndQuit(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int	    gWinWidth,
        gWinHeight,
        gImageWidth = kDefaultWidth,
        gImageHeight = kDefaultHeight,
        //
        gCurrentFieldRes[2] = {30, 30};

int		gMainWindow,
        gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];

bool	gInitDone = false,
        gDrawStuff = false,
        gDrawVectField = false;

unsigned char	    gImageTreshold = 100;

RasterImage         **gMyImage = NULL;	//	an array of references to Image objects
RasterImage_gray    **gColorGradientNorm = NULL;
RasterImage_gray    **gGrayGradientNorm = NULL;
RasterImage         **gBackgroundImage = NULL;
VectorField         **gMyColorGradient = NULL;
VectorField         **gMyGrayGradient = NULL;
bool                gradComputed[2] = {false, false};
bool                gradNormIsBackground[2] = {false, false};
ColorGradientMode   gColorGradeMode[2] =  {GRAD_AVE_OF_COLOR_COMPONENTS, GRAD_AVE_OF_COLOR_COMPONENTS};
int                 gLastGaussianWidth = 3;

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
    const string filePath = color_dir + "mandrill_256x256.tga";
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
    gBackgroundImage[GAUSSIAN] = gMyImage[UPPER_LEFT];
    gBackgroundImage[SOBEL] = gMyImage[UPPER_LEFT];

    gMyColorGradient = new VectorField*[2];
    gMyColorGradient[GAUSSIAN] = new VectorField_F(gMyImage[UPPER_LEFT]->getBoundRect());
    gMyColorGradient[SOBEL] = new VectorField_I(gMyImage[UPPER_LEFT]->getBoundRect());

    gMyGrayGradient = new VectorField*[2];
    gMyGrayGradient[GAUSSIAN] = new VectorField_F(gMyImage[UPPER_LEFT]->getBoundRect());
    gMyGrayGradient[SOBEL] = new VectorField_I(gMyImage[UPPER_LEFT]->getBoundRect());

    gColorGradientNorm = new RasterImage_gray*[2];
    gColorGradientNorm[GAUSSIAN] = gColorGradientNorm[SOBEL] = NULL;

    gGrayGradientNorm = new RasterImage_gray*[2];
    gGrayGradientNorm[GAUSSIAN] = gGrayGradientNorm[SOBEL] = NULL;
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
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gInitDone)
    {
        gMyImage[UPPER_LEFT]->render(-2.f);
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
        gMyImage[UPPER_RIGHT]->render();
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
        gBackgroundImage[GAUSSIAN]->render(true);

    if (gradComputed[GAUSSIAN])
    {
        gMyGrayGradient[GAUSSIAN]->render(true);
        gMyColorGradient[GAUSSIAN]->render(true);
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
        gBackgroundImage[SOBEL]->render(true);

    if (gradComputed[SOBEL])
    {
        gMyGrayGradient[SOBEL]->render(true);
        gMyColorGradient[SOBEL]->render(true);
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
        setupCamera(w, h);

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
    glOrtho(0.0f, (GLfloat) w, 0.0f, (GLfloat) h, -1.f, 3.f);

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
        //	0 --> Exit/Quit
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
				//  apply the color gradient filter to the image
				ColorImageGradient_Gaussian_F	colorGrad(static_cast<int>(value));
				colorGrad.setColorGradientMode(gColorGradeMode[GAUSSIAN]);
				colorGrad.applyInto(gMyImage[UPPER_LEFT], gMyColorGradient[GAUSSIAN]);
				gMyColorGradient[GAUSSIAN]->forceRasterUpdate();
				gMyColorGradient[GAUSSIAN]->setDrawValidDataFrame(true);
				gMyColorGradient[GAUSSIAN]->setFrameColor(0.f, 0.f, 1.f);
				gMyColorGradient[GAUSSIAN]->setColor(0.f, 1.f, 1.f);
				//  also get the gradient's norm as a rasterimage
				//  The first time around, return a new RI_g_F, after that just write into
				//  the existing raster image
				if (gColorGradientNorm[GAUSSIAN] == NULL)
					gColorGradientNorm[GAUSSIAN] = new RasterImage_gray_F(gMyImage[UPPER_RIGHT]->getBoundRect());
				gMyColorGradient[GAUSSIAN]->getNormAsRasterImage(gColorGradientNorm[GAUSSIAN]);
				//
				//  apply the gray gradient filter to the image
				ImageGradient_Gaussian_F	grayGrad(static_cast<int>(value));
				grayGrad.applyInto(gMyImage[UPPER_RIGHT], gMyGrayGradient[GAUSSIAN]);
				gMyGrayGradient[GAUSSIAN]->forceRasterUpdate();
				gMyGrayGradient[GAUSSIAN]->setDrawValidDataFrame(true);
				gMyGrayGradient[GAUSSIAN]->setFrameColor(0.f, 0.f, 1.f);
				gMyGrayGradient[GAUSSIAN]->setColor(0.f, 1.f, 1.f);
				//  also get the gradient's norm as a rasterimage
				//  The first time around, return a new RI_g_F, after that just write into
				//  the existing raster image
				if (gGrayGradientNorm[GAUSSIAN] == NULL)
					gGrayGradientNorm[GAUSSIAN] = new RasterImage_gray_F(gMyImage[UPPER_RIGHT]->getBoundRect());
				gMyGrayGradient[GAUSSIAN]->getNormAsRasterImage(gGrayGradientNorm[GAUSSIAN]);
				//
				gradComputed[GAUSSIAN] = true;
				gLastGaussianWidth = value;
            }
			break;

        case COLOR_IMAGE_AS_BKGND_MENU:
            gBackgroundImage[GAUSSIAN] = gMyImage[UPPER_LEFT];
            break;
            
        case GRAY_IMAGE_AS_BKGND_MENU:
            gBackgroundImage[GAUSSIAN] = gMyImage[UPPER_RIGHT];
            break;
            
        case COLOR_NORM_AS_BKGND_MENU:
            if (gradComputed[GAUSSIAN])
                gBackgroundImage[GAUSSIAN] = gColorGradientNorm[GAUSSIAN];
            break;

        case GRAY_NORM_AS_BKGND_MENU:
            if (gradComputed[GAUSSIAN])
                gBackgroundImage[GAUSSIAN] = gGrayGradientNorm[GAUSSIAN];
            break;

        case GRAD_AVE_OF_COLOR_COMPONENTS_MENU:
            if (gradComputed[GAUSSIAN])
            {
                gColorGradeMode[GAUSSIAN] = GRAD_AVE_OF_COLOR_COMPONENTS;
                //  and force a recalculation of the gradient
                myMenu2(gLastGaussianWidth);
            }
            break;
            
        case GRAD_LARGEST_COLOR_COMPONENT_MENU:
            if (gradComputed[GAUSSIAN])
            {
                gColorGradeMode[GAUSSIAN] = GRAD_LARGEST_COLOR_COMPONENT;
                //  and force a recalculation of the gradient
                myMenu2(gLastGaussianWidth);
            }
            break;
            
        //	coarser resolution
        case COARSER_RES_MENU:
            if (gradComputed[GAUSSIAN])
            {
                if (gCurrentFieldRes[GAUSSIAN] < 60)
                    gCurrentFieldRes[GAUSSIAN] += 5;
                gMyColorGradient[GAUSSIAN]->setVectResolution(gCurrentFieldRes[GAUSSIAN]);
                gMyColorGradient[GAUSSIAN]->forceRasterUpdate();
                gMyGrayGradient[GAUSSIAN]->setVectResolution(gCurrentFieldRes[GAUSSIAN]);
                gMyGrayGradient[GAUSSIAN]->forceRasterUpdate();
            }
            break;

        //	finer resolution
        case FINER_RES_MENU:
            if (gradComputed[GAUSSIAN])
            {
                if (gCurrentFieldRes[GAUSSIAN] > 10)
                    gCurrentFieldRes[GAUSSIAN] -= 5;
                gMyColorGradient[GAUSSIAN]->setVectResolution(gCurrentFieldRes[GAUSSIAN]);
                gMyColorGradient[GAUSSIAN]->forceRasterUpdate();
                gMyGrayGradient[GAUSSIAN]->setVectResolution(gCurrentFieldRes[GAUSSIAN]);
                gMyGrayGradient[GAUSSIAN]->forceRasterUpdate();
            }
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }

	myDisplayMain();
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
				ColorImageGradient_Sobel colorGrad;
				colorGrad.setColorGradientMode(gColorGradeMode[SOBEL]);
				colorGrad.applyInto(gMyImage[UPPER_LEFT], gMyColorGradient[SOBEL]);
				gMyColorGradient[SOBEL]->forceRasterUpdate();
				gMyColorGradient[SOBEL]->setDrawValidDataFrame(true);
				gMyColorGradient[SOBEL]->setFrameColor(0.f, 0.f, 1.f);
				gMyColorGradient[SOBEL]->setColor(0.f, 1.f, 1.f);
				//  also get the gradient's norm as a rasterimage
				//  The first time around, return a new RI_g_F, after that just write into
				//  the existing raster image
				if (gColorGradientNorm[SOBEL] == NULL)
					gColorGradientNorm[SOBEL] = new RasterImage_gray_F(gMyImage[UPPER_RIGHT]->getBoundRect());
				gMyColorGradient[SOBEL]->getNormAsRasterImage(gColorGradientNorm[SOBEL]);
				//
				//  apply the gray gradient filter to the image
				ImageGradient_Sobel grayGrad;
				grayGrad.applyInto(gMyImage[UPPER_RIGHT], gMyGrayGradient[SOBEL]);
				gMyGrayGradient[SOBEL]->forceRasterUpdate();
				gMyGrayGradient[SOBEL]->setDrawValidDataFrame(true);
				gMyGrayGradient[SOBEL]->setFrameColor(0.f, 0.f, 1.f);
				gMyGrayGradient[SOBEL]->setColor(0.f, 1.f, 1.f);
				//  also get the gradient's norm as a rasterimage
				//  The first time around, return a new RI_g_F, after that just write into
				//  the existing raster image
				if (gGrayGradientNorm[SOBEL] == NULL)
					gGrayGradientNorm[SOBEL] = new RasterImage_gray_F(gMyImage[UPPER_RIGHT]->getBoundRect());
				gMyGrayGradient[SOBEL]->getNormAsRasterImage(gGrayGradientNorm[SOBEL]);
				//
				gradComputed[SOBEL] = true;
            }
			break;

        case COLOR_IMAGE_AS_BKGND_MENU:
            gBackgroundImage[SOBEL] = gMyImage[UPPER_LEFT];
            break;
            
        case GRAY_IMAGE_AS_BKGND_MENU:
            gBackgroundImage[SOBEL] = gMyImage[UPPER_RIGHT];
            break;
            
        case COLOR_NORM_AS_BKGND_MENU:
            if (gradComputed[SOBEL])
                gBackgroundImage[SOBEL] = gColorGradientNorm[SOBEL];
            break;

        case GRAY_NORM_AS_BKGND_MENU:
            if (gradComputed[SOBEL])
                gBackgroundImage[SOBEL] = gGrayGradientNorm[SOBEL];
            break;

        case GRAD_AVE_OF_COLOR_COMPONENTS_MENU:
            if (gradComputed[SOBEL])
            {
                gColorGradeMode[SOBEL] = GRAD_AVE_OF_COLOR_COMPONENTS;
                //  force a recalculation of the gradient
                myMenu3(APPLY_SOBEL_MENU);
            }
            break;
            
        case GRAD_LARGEST_COLOR_COMPONENT_MENU:
            if (gradComputed[SOBEL])
            {
                gColorGradeMode[SOBEL] = GRAD_LARGEST_COLOR_COMPONENT;
                //  force a recalculation of the gradient
                myMenu3(APPLY_SOBEL_MENU);
            }
            break;
            

        //	coarser resolution
        case COARSER_RES_MENU:
            if (gradComputed[SOBEL])
            {
                if (gCurrentFieldRes[SOBEL] < 60)
                    gCurrentFieldRes[SOBEL] += 5;
                gMyColorGradient[SOBEL]->setVectResolution(gCurrentFieldRes[SOBEL]);
                gMyColorGradient[SOBEL]->forceRasterUpdate();
                gMyGrayGradient[SOBEL]->setVectResolution(gCurrentFieldRes[SOBEL]);
                gMyGrayGradient[SOBEL]->forceRasterUpdate();
            }
            break;

        //	finer resolution
        case FINER_RES_MENU:
            if (gradComputed[SOBEL])
            {
                if (gCurrentFieldRes[SOBEL] > 10)
                    gCurrentFieldRes[SOBEL] -= 5;
                gMyColorGradient[SOBEL]->setVectResolution(gCurrentFieldRes[SOBEL]);
                gMyColorGradient[SOBEL]->forceRasterUpdate();
                gMyGrayGradient[SOBEL]->setVectResolution(gCurrentFieldRes[SOBEL]);
                gMyGrayGradient[SOBEL]->forceRasterUpdate();
            }
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
	
	myDisplayMain();
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
    int menuMain, menu[kNbSubwindows], colorSubmenu, gaussMenu, 
        gaussBckgndMenu, sobelBckgndMenu,
        gaussColorGradModeMenu, sobelColrGradModeMenu;

    //	Set a general menu for the main window
    glutSetWindow(gMainWindow);
    glClearColor(0.0, 0.0, 0.0, 1.0);

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
    //
    gaussMenu = glutCreateMenu(myMenu2);
    glutAddMenuEntry("3x3 gradient of Gaussian", 3);
    glutAddMenuEntry("5x5 gradient of Gaussian", 5);
    glutAddMenuEntry("7x7 gradient of Gaussian", 7);
    glutAddMenuEntry("9x9 gradient of Gaussian", 9);
    glutAddMenuEntry("11x11 gradient of Gaussian", 11);
    glutAddMenuEntry("21x21 gradient of Gaussian", 21);
    //
    gaussBckgndMenu = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Color image", COLOR_IMAGE_AS_BKGND_MENU);
    glutAddMenuEntry("Gray image", GRAY_IMAGE_AS_BKGND_MENU);
    glutAddMenuEntry("Norm of color gradient", COLOR_NORM_AS_BKGND_MENU);
    glutAddMenuEntry("Norm of gray gradient", GRAY_NORM_AS_BKGND_MENU);
    //
    gaussColorGradModeMenu = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Average of color component gradients", GRAD_AVE_OF_COLOR_COMPONENTS_MENU);
    glutAddMenuEntry("Largest color component gradients", GRAD_LARGEST_COLOR_COMPONENT_MENU);
    //
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Apply Filter", gaussMenu);
    glutAddMenuEntry("Coarser resolution", COARSER_RES_MENU);
    glutAddMenuEntry("Finer resolution", FINER_RES_MENU);
    glutAddSubMenu("Select background", gaussBckgndMenu);
    glutAddSubMenu("Color gradient mode", gaussColorGradModeMenu);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for lower-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    //
    sobelBckgndMenu = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Color image", COLOR_IMAGE_AS_BKGND_MENU);
    glutAddMenuEntry("Gray image", GRAY_IMAGE_AS_BKGND_MENU);
    glutAddMenuEntry("Norm of color gradient", COLOR_NORM_AS_BKGND_MENU);
    glutAddMenuEntry("Norm of gray gradient", GRAY_NORM_AS_BKGND_MENU);
    //
    sobelColrGradModeMenu = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Average of color component gradients", GRAD_AVE_OF_COLOR_COMPONENTS_MENU);
    glutAddMenuEntry("Largest color component gradients", GRAD_LARGEST_COLOR_COMPONENT_MENU);
    //
    menu[LOWER_RIGHT] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("Apply Sobel image gradient", APPLY_SOBEL_MENU);
    glutAddMenuEntry("Coarser resolution", COARSER_RES_MENU);
    glutAddMenuEntry("Finer resolution", FINER_RES_MENU);
    glutAddSubMenu("Select background", sobelBckgndMenu);
    glutAddSubMenu("Color gradient mode", sobelColrGradModeMenu);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gMainWindow);

    gInitDone = true;
}



int main(int argc, char** argv)
{
    int		winID;
    GLfloat	*backColor;
    void	(*displayFuncTable[kNbSubwindows])() = {myDisplay0, myDisplay1, 
                                                    myDisplay2, myDisplay3};

    try {
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
        gMainWindow = glutCreateWindow("Color Image Gradients Demo");

        glutKeyboardFunc(myKeyboard);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);

        glClearColor(0.0, 0.0, 0.0, 1.0);

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
        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
