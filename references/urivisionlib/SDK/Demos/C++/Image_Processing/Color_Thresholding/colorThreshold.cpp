/*  NAME:
        colorThresholding.cpp
 
    DESCRIPTION:
        demonstration application for the ColorThresholdLabeler class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|	The original image is displayed in the upper-left subwindow.            |
|   The upper-right subwindow show a "pure hue" (saturated colors) version  |
|   of the input image.                                                     |
|   The other two subwindows show the thresholded image for different       |
|   selections of filling background and search rectangles.					|
|		o A right-click (ctrl-click on the Mac) in any subwindow will bring	|
|			up a popup menu													|
|		o A middle-click (option-click on the Mac) in any subwindow (except	|
|			the upper-left subwindow) will cycle through filling modes.		|
|		o A Click-and-drag in any subwindow will select a search rectangle	|
|			for the thresholding.											|
|																			|
|	Authors:	Fang Han and jean-yves herve',                              |
|                             3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|	
+--------------------------------------------------------------------------*/

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
//
#include "ImageReader.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_HSV.h"
#include "LabelRaster.h"
#include "ColorThresholdLabeler.h"
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

const int   QUIT_MENU = 0,
            TOGGLE_FILL_MODE_MENU = 1;

//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
void loadImage(void);                                        
void initThresholds(void);
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
void myColorSubmenu(int value);
//
void cycleFillMode(FillingMode *theMode);
void myMouse2(int button, int buttonState, int ix ,int iy);
void myMouse3(int button, int buttonState, int ix ,int iy);
void handleMouseDrag(int buttonState, int ix, int iy, int winIndex);
void myMouseMotion(int ix ,int iy);
bool withinBounds(int ix, int iy);
void myKeyboard(unsigned char c, int x, int y);
void myInit(void);
void myIdle(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int			gWinWidth,
            gWinHeight,
            gImageWidth = kDefaultWidth,
            gImageHeight = kDefaultHeight;

int			gMainWindow,
            gSubwindow[kNbSubwindows];

bool		gInitDone = false;

FillingMode	gModeWin[kNbSubwindows] = {kIdentityFill, kBlackFill, kWhiteFill, kOrigGrayFill};

RasterImage** gMyImage = NULL;	//	an array of references to Image objects
ColorThresholdLabeler* gThresholder = new ColorThresholdLabeler();
ColorSegParams* gColorParams;

const int	kMinSearchDim = 20;

bool		gMouseDown = false;
int		    gDragWin;
int			gStartDragX,
            gStartDragY;

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
string kBckgndColStr[] =	{
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



#if 0
#pragma mark -
#endif

void loadImage(void)
{
    const string filePath = color_dir + "mandrill_256x256.tga";
    ImageReader	myReader(const_cast<char*>(filePath.c_str()));

    gMyImage = new RasterImage*[kNbImages];

    //-------------------------------------------------------------
    //	We read the input image from a file
    //-------------------------------------------------------------
    gMyImage[UPPER_LEFT] = myReader.getFrame(true);
    if (gMyImage[UPPER_LEFT] ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath << endl;
        cleanupAndQuit(-1);
    }

    gImageWidth = gMyImage[UPPER_LEFT]->getWidth();
    gImageHeight = gMyImage[UPPER_LEFT]->getHeight();

    //-------------------------------------------------------------
    //  Image 1 is a "saturated" version of Image 0
    //  (obtained courtesy of the RasterImage_HSV class)
    //-------------------------------------------------------------
    gMyImage[UPPER_RIGHT] = new RasterImage_HSV(gMyImage[UPPER_LEFT]);
    ((RasterImage_HSV *) gMyImage[UPPER_RIGHT])->forceConversionToSaturatedRGBa();

    //-------------------------------------------------------------
    //  Image 2 is a regular copy of Image 0
    //  (will get thresholded within user rectangle selection)
    //-------------------------------------------------------------
	gMyImage[LOWER_LEFT] = new RasterImage_RGBa(gMyImage[UPPER_LEFT]);
}


void initThresholds(void)
{
    gColorParams = new ColorSegParams;
    gColorParams->nbModes = 6;
    gColorParams->colorMode = true;
    gColorParams->grayReg = NULL;
    gColorParams->colorReg = new ColorRegion[gColorParams->nbModes];

    gColorParams->colorReg[0].labelStr =  "magentaish";
    gColorParams->colorReg[0].hueRef = 300;
    gColorParams->colorReg[0].deltaHue = 30;
    gColorParams->colorReg[0].satMin = 20;
    gColorParams->colorReg[0].satMax = 100;
    gColorParams->colorReg[0].valMin = 0;
    gColorParams->colorReg[0].valMax = 255;
    gColorParams->colorReg[0].fillColor[0] = (unsigned char) 255;
    gColorParams->colorReg[0].fillColor[1] = 0;
    gColorParams->colorReg[0].fillColor[2] = (unsigned char) 255;
    gColorParams->colorReg[0].label = (char) 1;

    gColorParams->colorReg[1].labelStr = "red";
    gColorParams->colorReg[1].hueRef = 0;
    gColorParams->colorReg[1].deltaHue = 15;
    gColorParams->colorReg[1].satMin = 20;
    gColorParams->colorReg[1].satMax = 100;
    gColorParams->colorReg[1].valMin = 0;
    gColorParams->colorReg[1].valMax = 255;
    gColorParams->colorReg[1].fillColor[0] = (unsigned char) 255;
    gColorParams->colorReg[1].fillColor[1] = 0;
    gColorParams->colorReg[1].fillColor[2] = 0;
    gColorParams->colorReg[1].label = (char) 2;

    gColorParams->colorReg[2].labelStr = "greenish";
    gColorParams->colorReg[2].hueRef = 110;
    gColorParams->colorReg[2].deltaHue = 15;
    gColorParams->colorReg[2].satMin = 40;
    gColorParams->colorReg[2].satMax = 100;
    gColorParams->colorReg[2].valMin = 0;
    gColorParams->colorReg[2].valMax = 255;
    gColorParams->colorReg[2].fillColor[0] = 0;
    gColorParams->colorReg[2].fillColor[1] = (unsigned char) 255;
    gColorParams->colorReg[2].fillColor[2] = 0;
    gColorParams->colorReg[2].label = (char) 3;

    gColorParams->colorReg[3].labelStr = "yellowish";
    gColorParams->colorReg[3].hueRef = 55;
    gColorParams->colorReg[3].deltaHue = 20;
    gColorParams->colorReg[3].satMin = 20;
    gColorParams->colorReg[3].satMax = 100;
    gColorParams->colorReg[3].valMin = 0;
    gColorParams->colorReg[3].valMax = 255;
    gColorParams->colorReg[3].fillColor[0] = (unsigned char) 255;
    gColorParams->colorReg[3].fillColor[1] = (unsigned char) 255;
    gColorParams->colorReg[3].fillColor[2] = 0;
    gColorParams->colorReg[3].label = (char) 4;

    gColorParams->colorReg[4].labelStr = "blueish";
    gColorParams->colorReg[4].hueRef = 240;
    gColorParams->colorReg[4].deltaHue = 20;
    gColorParams->colorReg[4].satMin = 20;
    gColorParams->colorReg[4].satMax = 100;
    gColorParams->colorReg[4].valMin = 0;
    gColorParams->colorReg[4].valMax = 255;
    gColorParams->colorReg[4].fillColor[0] = 0;
    gColorParams->colorReg[4].fillColor[1] = 0;
    gColorParams->colorReg[4].fillColor[2] = (unsigned char) 255;
    gColorParams->colorReg[4].label = (char) 5;

    gColorParams->colorReg[5].labelStr = "cianish";
    gColorParams->colorReg[5].hueRef = 185;
    gColorParams->colorReg[5].deltaHue = 25;
    gColorParams->colorReg[5].satMin = 20;
    gColorParams->colorReg[5].satMax = 100;
    gColorParams->colorReg[5].valMin = 0;
    gColorParams->colorReg[5].valMax = 255;
    gColorParams->colorReg[5].fillColor[0] = 0;
    gColorParams->colorReg[5].fillColor[1] = (unsigned char) 255;
    gColorParams->colorReg[5].fillColor[2] = (unsigned char) 255;
    gColorParams->colorReg[5].label = (char) 6;

    //-----------------------------------------------------------------
    //	Here I wanted to show different ways to use the thresholder
    //-----------------------------------------------------------------
	
	//v1. Produces a new LabelRaster object that contains the results of the thresholding
	//		This mode is mostly useful for blob finding, so check out the Blob Filling demo
	//		to see what to do of that LabelRasterObject
    //LabelRaster* myRaster = gThresholder->applyTo(gMyImage[UPPER_LEFT], gColorParams);

	//v2. Write the results into the LabelRaster object passed as parameter.
	//		same comment as above
	//gThresholder->applyTo(gMyImage[UPPER_LEFT], gColorParams, myRaster, true);

    //	Use the version of the gThresholder that returns a new LabelRaster object;
    //	select a black background
    gColorParams->fillMode = kBlackFill;

    //	Use the version of the gThresholder that uses the first RasterImage parameter as
    //	input and the second one as output
    gMyImage[LOWER_RIGHT] = new RasterImage_RGBa(gMyImage[UPPER_LEFT]);
    gColorParams->fillMode = kOrigGrayFill;
    gThresholder->applyInto(gMyImage[UPPER_LEFT],  gColorParams, gMyImage[LOWER_RIGHT]);
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
    glClearColor(0, 0, 0, 1);
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

    if (gMyImage[UPPER_LEFT]!= NULL)
    {
        gMyImage[UPPER_LEFT]->render();
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

    if (gMyImage[UPPER_RIGHT]!= NULL)
    {
        gMyImage[UPPER_RIGHT]->render();
    }

    glutSwapBuffers();
}

void myDisplay2(void)
{
    glutSetWindow(gSubwindow[LOWER_LEFT]);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gMyImage[LOWER_LEFT]!= NULL)
    {
        gMyImage[LOWER_LEFT]->render();
    }

    glutSwapBuffers();
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gMyImage[LOWER_RIGHT]!= NULL)
    {
        gMyImage[LOWER_RIGHT]->render();
    }

    glutSwapBuffers();
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
    glOrtho(0.0f, (GLfloat) w, 0.0f, (GLfloat) h, -1, 3);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
}


#if 0
#pragma mark -
#endif


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

void cycleFillMode(FillingMode *theMode)
{
    switch (*theMode)
    {
        case kIdentityFill:
            *theMode = kOrigColorFill;
            break;
        case kOrigColorFill:
            *theMode = kBlackFill;
            break;
        case kBlackFill:
            *theMode = kWhiteFill;
            break;
        case kWhiteFill:
            *theMode = kMediumGrayFill;
            break;
        case kMediumGrayFill:
            *theMode = kOrigGrayFill;
            break;
        case kOrigGrayFill:
            *theMode = kIdentityFill;
            break;
        default:
            break;
    }
}

//	Mouse event handling function for subwindow 2.

void myMouse2(int button, int buttonState, int ix ,int iy)
{
    if (gMyImage[UPPER_LEFT] != NULL)
    {
        glutSetWindow(gSubwindow[LOWER_LEFT]);

        if(buttonState == GLUT_UP && button == GLUT_MIDDLE_BUTTON)
        {
            cycleFillMode(gModeWin+LOWER_LEFT);

            gColorParams->fillMode = gModeWin[LOWER_LEFT];

            gThresholder->applyInto(gMyImage[UPPER_LEFT], gColorParams, gMyImage[LOWER_LEFT]);
            //  Here I *know* that the part of gMyImage[LOWER_LEFT] that was not selected
            //  still contains valid data, so I force the valid data rectangle to
            //  the full frame
            gMyImage[LOWER_LEFT]->setValidRect(gMyImage[LOWER_LEFT]->getBoundRect());
            myDisplay2();
        }
        else if (button == GLUT_LEFT_BUTTON)
            handleMouseDrag(buttonState, ix, iy, LOWER_LEFT);

    }
}

//	Mouse event handling function for subwindow 3.
void myMouse3(int button, int buttonState, int ix ,int iy)
{
    if (gMyImage[UPPER_LEFT] != NULL)
    {
        glutSetWindow(gSubwindow[LOWER_RIGHT]);

        if(buttonState == GLUT_UP && button == GLUT_MIDDLE_BUTTON)
        {
            cycleFillMode(gModeWin+LOWER_RIGHT);

            gColorParams->fillMode = gModeWin[LOWER_RIGHT];
            gThresholder->applyInto(gMyImage[UPPER_LEFT], gColorParams, gMyImage[LOWER_RIGHT]);
            //  Here I *know* that the part of gMyImage[LOWER_RIGHT] that was not selected
            //  still contains valid data, so I force the valid data rectangle to
            //  the full frame
            gMyImage[LOWER_RIGHT]->setValidRect(gMyImage[LOWER_RIGHT]->getBoundRect());
            myDisplay3();
        }
        else if (button == GLUT_LEFT_BUTTON)
            handleMouseDrag(buttonState, ix, iy, LOWER_RIGHT);

    }
}

void handleMouseDrag(int buttonState, int ix, int iy, int winIndex)
{
    ImageRect		*myRect;
    FillingMode     localMode;

    if (buttonState == GLUT_DOWN)
    {
        //  If the mouse button was not already pressed down, we start recording
        if (!gMouseDown)
        {
            gMouseDown = true;
            gDragWin = winIndex;
            gStartDragX = ix;
            gStartDragY = iy;
        }
    }
    else	//	buttonState == GLUT_UP
    {
        //	check that the mouse had been dragged from within the same subwindow and
        //	defines a suffficiently large search region.
        if (gMouseDown && withinBounds(ix, iy))
        {
            int theLeft = MIN(ix, gStartDragX),
                theTop = MIN(iy, gStartDragY),
                theWidth = MAX(ix, gStartDragX) - theLeft + 1,
                theHeight = MAX(iy, gStartDragY) - theTop + 1;

            if ((theWidth >= kMinSearchDim) && (theHeight >= kMinSearchDim))
            {
                myRect = new ImageRect(	theLeft, theTop, theWidth, theHeight);

                //	We pick for the selected rectangle a filling mode that difffers from that of
                //	the rest of the subwindow.
                switch (gModeWin[winIndex])
                {
                    case kIdentityFill:
                        localMode = kWhiteFill;
                        break;
                    case kOrigColorFill:
                        localMode = kBlackFill;
                        break;
                    case kBlackFill:
                        localMode = kOrigGrayFill;
                        break;
                    case kWhiteFill:
                        localMode = kOrigGrayFill;
                        break;
                    case kMediumGrayFill:
                        localMode = kOrigColorFill;
                        break;
                    case kOrigGrayFill:
                        localMode = kWhiteFill;
                        break;
                    default:
                        break;
                }

                gColorParams->fillMode = localMode;
                gThresholder->applyInto(gMyImage[UPPER_LEFT], gColorParams, myRect,
                                        (RasterImage_RGBa *) gMyImage[winIndex]);
                //  Here I *know* that the part of gMyImage[winIndex] that was not selected
                //  still contains valid data, so I force the valid data rectangle to
                //  the full frame
                gMyImage[winIndex]->setValidRect(gMyImage[winIndex]->getBoundRect());
                delete myRect;
            }
        }
        gMouseDown = false;
        gDragWin = -1;

        myDisplayMain();
    }
}


void myMouseMotion(int ix ,int iy)
{
    if (gMouseDown)
    {
        glutSetWindow(gSubwindow[gDragWin]);

        glClear(GL_COLOR_BUFFER_BIT);

        if (gMyImage[gDragWin]!= NULL)
        {
            gMyImage[gDragWin]->render();
        }

        if (withinBounds(ix, iy))
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2i(gStartDragX, gImageHeight - 1 -gStartDragY);
            glVertex2i(gStartDragX, gImageHeight - iy - 1);
            glVertex2i(ix, gImageHeight - iy - 1);
            glVertex2i(ix, gImageHeight - 1 - gStartDragY);
            glEnd();
        }

        glutSwapBuffers();
    }
}

bool withinBounds(int ix, int iy)
{
    return ((ix>=0) && (ix<gMyImage[UPPER_LEFT]->getWidth()) &&
            (iy>=0) && (iy<gMyImage[UPPER_LEFT]->getHeight()));
}

#if 0
#pragma mark -
#endif

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
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case TOGGLE_FILL_MODE_MENU:
            cycleFillMode(gModeWin+2);
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

        case TOGGLE_FILL_MODE_MENU:
            cycleFillMode(gModeWin+2);
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
        glutPostRedisplay();
    }
}



void myInit(void)
{
    // Create Menus
    int menuMain, menu[kNbSubwindows], colorSubmenu;

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
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for upper-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMotionFunc(myMouseMotion);

    //------------------------------------
    //  menus for lower-left quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_LEFT]);
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("Toggle fill mode", TOGGLE_FILL_MODE_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMouseFunc(myMouse2);
    glutMotionFunc(myMouseMotion);

    //------------------------------------
    //  menus for lower-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    menu[LOWER_RIGHT] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("Toggle fill mode", TOGGLE_FILL_MODE_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMouseFunc(myMouse3);
    glutMotionFunc(myMouseMotion);

    glutSetWindow(gMainWindow);

    initThresholds();
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

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

        initializeQTEnvironment();

        //	I load my image
        loadImage();

        int	imgHeight = gMyImage[UPPER_LEFT]->getHeight(),
            imgWidth = gMyImage[UPPER_LEFT]->getWidth();

        //	I inform all drawable objects of the dimensions of their subwindow
        DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

        //	I initialize my window's dimensions
        gWinHeight = kNbRowsSubwindows * (imgHeight + kBorder) + kBorder;
        gWinWidth = kNbColsSubwindows * (imgWidth + kBorder) + kBorder;

        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Color Thresholding Demo");

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
        initThresholds();

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
