/*  NAME:
        edgePixels.cpp
 
    DESCRIPTION:
        demonstration application for edge detection classes
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|	This simple application demonstrates the use of the components of       |
|   an edge detector: Gaussian, gradient, gradient of Gaussian, Laplacian,  |
|   Laplacian of Gaussian, zero crossings, nonmaximum suppression,          |
|   hysteresis thresholding.  In other words, the components that make up   |
|   the Marr-Hildreth and Canny (and other) edge detectors.  Complete edge  |
|   detector classes are (going to be) implemented in the library, so this  |
|    one is really for people who want to roll their own operator.          |
|                                                                           |
|	The original image is displayed in the upper-left subwindow.            |
|   The upper-right quadrant shows the image to which has been applied a    |
|       Gaussian filter os cale selectable through a right-click menu       |
|   The lower-left quadrant shows the result of applying to the U-R image   |
|       a Laplacian Sobel operator or LoG + zero-crossing detector          |
|   The lower-right quadrant show the effect of applying to the U-L image   |
|       a gradient of Gaussian operator or GoG + nonmaximum supressor +     |
|       hysteresis thresholding (the latter still being worked on)          |
|                                                                           |
|	Author:	jean-yves herve',                                               |
|                 3D Group for Interactive Visualization,                   |
|                 Department of Computer Science and Statistics	            |
|		          University of Rhode Island					            |	
+--------------------------------------------------------------------------*/

#include <fstream>
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
#include "ImageLaplacian_Gaussian_F.h"
#include "GaussianFilter_F.h"
#include "ImageGradient_Sobel.h"
#include "ImageLaplacian_Sobel.h"
#include "ZeroCrossingDetector.h"
#include "NonMaximumSuppressor.h"
#include "HistogramCalculator.h"
#include "LabelRaster.h"
#include "HysteresisLabeler.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
//
#if	URIVL_OS_MACOS
	const string color_dir = "../../../../../../../../Data_Samples/Color/";
	const string gray_dir = "../../../../../../../../Data_Samples/Gray/";
#elif URIVL_OS_WIN32 || URIVL_OS_UNIX
	const string color_dir = "../../../../../../Data_Samples/Color/";
	const string gray_dir = "../../../../../../Data_Samples/Gray/";
#else
    #error "OS not supported"
#endif

const string filePath = gray_dir + "Lena_256x256.tga";

//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640

#define	kNbRowsSubwindows	2
#define	kNbColsSubwindows	2
#define	kNbSubwindows       kNbRowsSubwindows*kNbColsSubwindows
#define	kNbImages			kNbSubwindows

//  names for my subwindows
const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1,
            LOWER_LEFT = 2,
            LOWER_RIGHT = 3;

//  Common menu items
const int QUIT_APPL = 0;
const int NO_FILTER = 1;
const int SEPARATOR = 1000;
const int ORIGINAL_IMAGE = 51;
//
//  Menus items for lower left subwindow
const int LAPLACIAN_SOBEL_FORM_1_MENU = 31;
const int LAPLACIAN_SOBEL_FORM_2_MENU = 32;
const int LAPLACIAN_SOBEL_FORM_3_MENU = 33;
const int LAPLACIAN_OF_GAUSSIAN_5x5 = 5;
const int LAPLACIAN_OF_GAUSSIAN_7x7 = 7;
const int LAPLACIAN_OF_GAUSSIAN_9x9 = 9;
const int LAPLACIAN_OF_GAUSSIAN_11x11 = 11;
const int LAPLACIAN_OF_GAUSSIAN_13x13 = 13;
const int LAPLACIAN_OF_GAUSSIAN_21x21 = 21;
//
const int TOGGLE_ZERO_CROSSINGS = 50;
const int TOGGLE_COLORED_LAPLACIAN = 55;
//
//  Menus items for lower right subwindow
const int GoG_3x3 = 3;
const int GoG_5x5 = 5;
const int GoG_7x7 = 7;
const int GoG_9x9 = 9;
const int GoG_11x11 = 11;
const int GoG_13x13 = 13;
const int GoG_21x21 = 21;
//
const int GoG = 61;
const int NONMAX_SUPPR_GoG = 62;
const int HYST_THRESH_GoG = 63;
//
const int NORM_GoG = 52;
const int NONMAX_SUPPR_NoGoG = 53;
const int HYST_THRESH_NoGoG = 54;
//
const int COARSER_VECT_RESOLUTION = 71;
const int FINER_VECT_RESOLUTION = 72;


//----------------------------------------------------------
//  Constants for definition of window background color
//----------------------------------------------------------
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


//  I don't understand why TGA files that are read the right side up on
//  the Mac are upside down on the PC, or maybe it's the other way around
//  (hundreds of billions of cockroaches can't be wrong)
#ifdef	MAC_VERSION
#define UPSIDE_DOWN     false
#else
#define UPSIDE_DOWN     true
#endif

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


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int			    gWinWidth,
                gWinHeight,
                gImageWidth = kDefaultWidth,
                gImageHeight = kDefaultHeight,
                //
                gCurrentFieldRes = 30;

int				gMainWindow,
                gSubwindow[kNbSubwindows];

bool			    gInitDone = false,
                       gDrawStuff = false,
                                    gDrawVectField = false;

unsigned char	    gImageTreshold = 100;

RasterImage         *gMyImage = NULL;
RasterImage_gray    *gImgGray = NULL;

RasterImage_gray_F	*gSmoothedImg = NULL;
VectorField_F       *gMyGradient = NULL;
VectorField_F       *gNonMaxSupprGrad = NULL;
RasterImage_gray_F  *gGradientNorm = NULL;
RasterImage_gray	*gZeroCrossImg = NULL;
RasterImage_gray_F  *gNonMaxSupprGradNorm = NULL;
RasterImage_gray    *gHistoThresholdedImg = NULL;

RasterImage_gray_F	*gLplcImg = NULL;
RasterImage_gray_F	*gLplcImgSobel = NULL;
RasterImage_gray_F	*gLplcImgGauss = NULL;
RasterImage_gray	*gCannyRas = NULL;
RasterImage_RGBa	*gColoredLaplacian = NULL;
LabelRaster			*gLabel = NULL;

bool                gradComputed = false;
bool				gDisplayZeroCrossings = false;
bool				gDisplayColoredLaplacian = false;

RasterImage         *gImageDisplayed[kNbSubwindows] = {NULL, NULL, NULL, NULL};
VectorField         *gVFieldDisplayed[kNbSubwindows] = {NULL, NULL, NULL, NULL};



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
    //-------------------------------------------------------------
    //	We read from a file the image to process
    //-------------------------------------------------------------
    ImageReader	myReader(const_cast<char*>(filePath.c_str()));

    gMyImage = myReader.getFrame(true);

    if (gMyImage ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath << endl;
        cleanupAndQuit(-1);
    }

    //	we convert the image to gray level
    gImgGray = new RasterImage_gray(gMyImage);

    //	initially the "smoothed" gray level image is just a copy of gImgGray
    gSmoothedImg = new RasterImage_gray_F(gImgGray);

    //	The other images and vector fields are just allocated at the same dimensions
    //	as the input image
    const ImageRect	*rect = gMyImage->getBoundRect();
    gLplcImgSobel = new RasterImage_gray_F(rect);
    gLplcImgGauss = new RasterImage_gray_F(rect);
    gMyGradient = new VectorField_F(rect);
    gNonMaxSupprGrad = new VectorField_F(rect);
    gGradientNorm = new RasterImage_gray_F(rect);
    gZeroCrossImg = new RasterImage_gray(rect);
    gColoredLaplacian = new RasterImage_RGBa(rect);
	gCannyRas = new RasterImage_gray(rect);
	gLabel = new LabelRaster(rect);

    gImageDisplayed[UPPER_LEFT] = gMyImage;
    gImageDisplayed[UPPER_RIGHT] = gImgGray;
    gImageDisplayed[LOWER_LEFT] = gImgGray;
    gImageDisplayed[LOWER_RIGHT] = gImgGray;
    //
    gVFieldDisplayed[LOWER_RIGHT] = gMyGradient;
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
        gImageDisplayed[UPPER_LEFT]->render(true);
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
        gImageDisplayed[UPPER_RIGHT]->render(true);
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
        gImageDisplayed[LOWER_LEFT]->render(true);
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
        gImageDisplayed[LOWER_RIGHT]->render(true);

        if (gradComputed)
            gVFieldDisplayed[LOWER_RIGHT]->render(true);

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
        case QUIT_APPL:
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
        case QUIT_APPL:
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
        case QUIT_APPL:
            cleanupAndQuit(0);
            break;

        case NO_FILTER:
            gImageDisplayed[UPPER_RIGHT] = gImgGray;
            gImgGray->copyInto(gSmoothedImg);
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        case 3:
        case 5:
        case 7:
        case 9:
        case 11:
        case 13:
        case 21:
            //  apply the Gaussian filter to the image
            GaussianFilter_F    *gauss;
            gauss = new GaussianFilter_F((int) value);
            gauss->applyInto(gImgGray, gSmoothedImg);
            gImageDisplayed[UPPER_RIGHT] = gSmoothedImg;
            delete gauss;
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
        case QUIT_APPL:
            cleanupAndQuit(0);
            break;

        case LAPLACIAN_SOBEL_FORM_1_MENU:
        case LAPLACIAN_SOBEL_FORM_2_MENU:
        case LAPLACIAN_SOBEL_FORM_3_MENU:
            ImageLaplacian_Sobel *laplaceSobel;
            if (value == LAPLACIAN_SOBEL_FORM_1_MENU)
                laplaceSobel = new ImageLaplacian_Sobel(LAPLACIAN_SOBEL_FORM_1);
            else if (value == LAPLACIAN_SOBEL_FORM_2_MENU)
                laplaceSobel = new ImageLaplacian_Sobel(LAPLACIAN_SOBEL_FORM_2);
            else	//if (value == LAPLACIAN_SOBEL_FORM_3_MENU)
                laplaceSobel = new ImageLaplacian_Sobel(LAPLACIAN_SOBEL_FORM_3);

            laplaceSobel->applyInto(gSmoothedImg, gLplcImgSobel);
            gLplcImg = gLplcImgSobel;
            delete laplaceSobel;
            ZeroCrossingDetector::applyInto_st(gLplcImgSobel, gZeroCrossImg);
            unsigned char colorChoice[3][3];
            colorChoice[0][0] = 0;
            colorChoice[0][1] = 255;
            colorChoice[0][2] = 0;
            colorChoice[1][0] = 0;
            colorChoice[1][1] = 0;
            colorChoice[1][2] = 255;
            colorChoice[2][0] = 255;
            colorChoice[2][1] = 0;
            colorChoice[2][2] = 0;
              
            ZeroCrossingDetector::showSignAsColor(gLplcImgSobel, gColoredLaplacian, colorChoice);
            if (gDisplayZeroCrossings)
                gImageDisplayed[LOWER_LEFT] = gZeroCrossImg;
            else
                gImageDisplayed[LOWER_LEFT] = gLplcImg;

           glutSetWindow(gMainWindow);
           glutPostRedisplay();
           break;

        case LAPLACIAN_OF_GAUSSIAN_5x5:
        case LAPLACIAN_OF_GAUSSIAN_7x7:
        case LAPLACIAN_OF_GAUSSIAN_9x9:
        case LAPLACIAN_OF_GAUSSIAN_11x11:
        case LAPLACIAN_OF_GAUSSIAN_13x13:
        case LAPLACIAN_OF_GAUSSIAN_21x21:
            //  apply the Laplacian filter to the image
            ImageLaplacian_Gaussian_F *laplaceGauss;
            laplaceGauss = new ImageLaplacian_Gaussian_F((int) value);
            laplaceGauss->applyInto(gSmoothedImg, gLplcImgGauss);
            gLplcImg = gLplcImgGauss;
            delete laplaceGauss;

            ZeroCrossingDetector::applyInto_st(gLplcImgGauss, gZeroCrossImg);
            unsigned char colorChoice2[3][3];

            colorChoice2[0][0] = 0;
            colorChoice2[0][1] = 255;
            colorChoice2[0][2] = 0;
            colorChoice2[1][0] = 0;
            colorChoice2[1][1] = 0;
            colorChoice2[1][2] = 255;
            colorChoice2[2][0] = 255;
            colorChoice2[2][1] = 0;
            colorChoice2[2][2] = 0;

            ZeroCrossingDetector::showSignAsColor(gLplcImgGauss, gColoredLaplacian, colorChoice2);

            if (gDisplayZeroCrossings)
                gImageDisplayed[LOWER_LEFT] = gZeroCrossImg;
            else
                gImageDisplayed[LOWER_LEFT] = gLplcImg;

            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        //	Toggle zero crossing display
        case TOGGLE_ZERO_CROSSINGS:
            gDisplayZeroCrossings = !gDisplayZeroCrossings;
            gDisplayColoredLaplacian = false;
            if (gDisplayZeroCrossings)
                gImageDisplayed[LOWER_LEFT] = gZeroCrossImg;
            else
                gImageDisplayed[LOWER_LEFT] = gLplcImg;
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;


        //	Toggle colored Laplacian display
        case TOGGLE_COLORED_LAPLACIAN:
            gDisplayColoredLaplacian = !gDisplayColoredLaplacian;
            gDisplayZeroCrossings = false;
            if (gDisplayColoredLaplacian)
                gImageDisplayed[LOWER_LEFT] = gColoredLaplacian;
            else
                gImageDisplayed[LOWER_LEFT] = gLplcImg;
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
        case QUIT_APPL:
            cleanupAndQuit(0);
            break;

        //---------------------------------------------------------------
        //  "operator" menu selections
        //---------------------------------------------------------------

        //  Computation of gradient of gaussian for gray-level image
        case GoG_3x3:
        case GoG_5x5:
        case GoG_7x7:
        case GoG_9x9:
        case GoG_11x11:
        case GoG_13x13:
        case GoG_21x21:
		{
            //  apply the gradient filter to the image

            ImageGradient_Gaussian_F    *grad;
            grad = new ImageGradient_Gaussian_F((int) value);
            grad->applyInto(gImgGray, gMyGradient);
            delete grad;
            gMyGradient->setDrawValidDataFrame(true);
            gMyGradient->setFrameColor(0.f, 0.f, 1.f);
            gradComputed = true;
            //
            //  also get the gradient's norm as a rasterimage
            gMyGradient->getNormAsRasterImage(gGradientNorm);
            //
            //  apply a nonmaximum suppression
            NonMaximumSuppressor::applyInto_st(gMyGradient, gNonMaxSupprGrad);
            gNonMaxSupprGradNorm = gNonMaxSupprGrad->getNormAsRasterImage(R_W_ACCESS);
            //
            //  And then apply hysteresis thresholding
            HistogramCalculator *histCalc;
            histCalc = new HistogramCalculator();
            histCalc->computeHistogram(gImgGray);
			
			HysteresisLabeler::applyInto(500,100,gNonMaxSupprGrad,gLabel);
			
			unsigned char *const*canny = gCannyRas->getGray2D(R_W_ACCESS);
			char const*const* label = gLabel->getLabel2D();
			
			for (int i = 0; i < gImageHeight; i++)
			{
				for (int j = 0; j < gImageWidth; j++)
				{
					canny[i][j] = label[i][j] == 0 ? 0 : (unsigned char) 255; 
				}
			}
			
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
		}break;

        //---------------------------------------------------------------
        //  Selection of background image (below gradient vector field)
        //---------------------------------------------------------------
        case ORIGINAL_IMAGE:
            gImageDisplayed[LOWER_RIGHT] = gMyImage;
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        case NORM_GoG:
            if (gradComputed)
            {
                gImageDisplayed[LOWER_RIGHT] = gGradientNorm;

                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        case NONMAX_SUPPR_NoGoG:
            if (gradComputed)
            {
                gImageDisplayed[LOWER_RIGHT] = gNonMaxSupprGradNorm;

                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        case HYST_THRESH_NoGoG:
            if (gradComputed)
            {
				gImageDisplayed[LOWER_RIGHT] = gCannyRas;
                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;


        //---------------------------------------------------------------
        //  Selection of gradient vector field to display
        //---------------------------------------------------------------
        case GoG:
            if (gradComputed)
            {
                gVFieldDisplayed[LOWER_RIGHT] = gMyGradient;

                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        case NONMAX_SUPPR_GoG:
            if (gradComputed)
            {
                gVFieldDisplayed[LOWER_RIGHT] = gNonMaxSupprGrad;

                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        case HYST_THRESH_GoG:
            if (gradComputed)
            {
                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        //	coarser vector field resolution
        case COARSER_VECT_RESOLUTION:
            if (gradComputed)
            {
                if (gCurrentFieldRes < 60)
                    gCurrentFieldRes += 5;
                gVFieldDisplayed[LOWER_RIGHT]->setVectResolution(gCurrentFieldRes);
                gVFieldDisplayed[LOWER_RIGHT]->forceRasterUpdate();
                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        //	finer vector field resolution
        case FINER_VECT_RESOLUTION:
            if (gradComputed)
            {
                if (gCurrentFieldRes > 10)
                    gCurrentFieldRes -= 5;
                gVFieldDisplayed[LOWER_RIGHT]->setVectResolution(gCurrentFieldRes);
                gVFieldDisplayed[LOWER_RIGHT]->forceRasterUpdate();
                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
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

        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}


void myInit(void)
{
    // Create Menus
    int menuMain, menu[4], colorSubmenu, gaussMenu, laplacianGaussianMenu,
    laplacianSobelMenu, gradientMenu, gradBackgroundMenu, gradVecDisplayMenu;

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
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //	Setup menus for upper-left subwindow
    glutSetWindow(gSubwindow[UPPER_LEFT]);
    //
    menu[UPPER_LEFT] = glutCreateMenu(myMenu0);
    glutAddMenuEntry("Quit", QUIT_APPL);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //	Setup menus for upper-right subwindow
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    //
    gaussMenu = glutCreateMenu(myMenu1);
    glutAddMenuEntry("3x3 Gaussian", 3);
    glutAddMenuEntry("5x5 Gaussian", 5);
    glutAddMenuEntry("7x7 Gaussian", 7);
    glutAddMenuEntry("9x9 Gaussian", 9);
    glutAddMenuEntry("11x11 Gaussian", 11);
    glutAddMenuEntry("13x13 Gaussian", 13);
    glutAddMenuEntry("21x21 Gaussian", 21);
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_APPL);
    glutAddMenuEntry("No Filter", NO_FILTER);
    glutAddSubMenu("Apply Filter", gaussMenu);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //	Setup menus for lower-left subwindow
    glutSetWindow(gSubwindow[LOWER_LEFT]);
    //
    laplacianSobelMenu = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Form 1", LAPLACIAN_SOBEL_FORM_1_MENU);
    glutAddMenuEntry("Form 2", LAPLACIAN_SOBEL_FORM_2_MENU);
    glutAddMenuEntry("Form 3", LAPLACIAN_SOBEL_FORM_3_MENU);
    laplacianGaussianMenu = glutCreateMenu(myMenu2);
    glutAddMenuEntry("5x5 LoG", LAPLACIAN_OF_GAUSSIAN_5x5);
    glutAddMenuEntry("7x7 LoG", LAPLACIAN_OF_GAUSSIAN_7x7);
    glutAddMenuEntry("9x9 LoG", LAPLACIAN_OF_GAUSSIAN_9x9);
    glutAddMenuEntry("11x11 LoG", LAPLACIAN_OF_GAUSSIAN_11x11);
    glutAddMenuEntry("13x13 LoG", LAPLACIAN_OF_GAUSSIAN_13x13);
    glutAddMenuEntry("21x21 LoG", LAPLACIAN_OF_GAUSSIAN_21x21);
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddSubMenu("Laplacian Sobel", laplacianSobelMenu);
    glutAddSubMenu("Laplacian of Gaussian", laplacianGaussianMenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Toggle zero crossings", TOGGLE_ZERO_CROSSINGS);
    glutAddMenuEntry("Toggle colored Laplacian", TOGGLE_COLORED_LAPLACIAN);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Quit", QUIT_APPL);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //	Setup menus for lower-right subwindow
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    //
    gradientMenu = glutCreateMenu(myMenu3);
    glutAddMenuEntry("3x3 GoG", GoG_3x3);
    glutAddMenuEntry("5x5 GoG", GoG_5x5);
    glutAddMenuEntry("7x7 GoG", GoG_7x7);
    glutAddMenuEntry("9x9 GoG", GoG_9x9);
    glutAddMenuEntry("11x11 GoG", GoG_11x11);
    glutAddMenuEntry("13x13 GoG", GoG_13x13);
    glutAddMenuEntry("21x21 GoG", GoG_21x21);
    //
    gradVecDisplayMenu = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Gradient of Gaussian", GoG);
    glutAddMenuEntry("Nonmaximum suppressed GoG", NONMAX_SUPPR_GoG);
    glutAddMenuEntry("Hysteresis thresholded GoG", HYST_THRESH_GoG);
    //
    gradBackgroundMenu = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Original image", ORIGINAL_IMAGE);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Norm of GoG", NORM_GoG);
    glutAddMenuEntry("Nonmaximum suppressed NoGoG", NONMAX_SUPPR_NoGoG);
    glutAddMenuEntry("Hysteresis thresholded NoGoG", HYST_THRESH_NoGoG);
    //
    menu[LOWER_RIGHT] = glutCreateMenu(myMenu3);
    glutAddSubMenu("Apply gradient of Gaussian", gradientMenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddSubMenu("Vector field displayed", gradVecDisplayMenu);
    glutAddMenuEntry("Coarser resolution", COARSER_VECT_RESOLUTION);
    glutAddMenuEntry("Finer resolution", FINER_VECT_RESOLUTION);
    glutAddSubMenu("Select background image", gradBackgroundMenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Quit", QUIT_APPL);
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

        int	imgHeight = gMyImage->getHeight(),
            imgWidth = gMyImage->getWidth();

        //	I inform all drawable objects of the dimensions of their subwindow
        DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

        //	I initialize my window's dimensions
        gWinHeight = kNbRowsSubwindows * (imgHeight + kBorder) + kBorder;
        gWinWidth = kNbColsSubwindows * (imgWidth + kBorder) + kBorder;

        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Edge Pixels Detection Demo");

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
        myDisplayMain();

        glutMainLoop();

        cleanupAndQuit(0);

        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
