/*  NAME:
        main_filling.cpp
 
    DESCRIPTION:
        demonstration application for the BlobFiller class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|	mainFilling.cpp		A simple demo of the BlobFiller class.  The code	|
|						is adapted from that of the Thresholder demo, the	|
|						main difference being that instead of labelling		|
|						individual points, we detect blobs that have one of	|
|						the colors sought.									|
|																			|
|	Author:	Fang Han and jean-yves herve', 3D Group,                        |
|                           Department of Computer Science and Statistics	|
|								University of Rhode Island					|	
|																			|
|	Version	1.0 [06/12/2003]												|
|	Version	1.1 [06/22/2003]	Rewritten to conform to Labeler classes.	|
|	Version 1.2 [11/26/2004]	Rewritten for uriVisionLib overhaul			|
|   Version 1.3 [01/27/2006]    Meng Wan & jyh, added blob contour          |
|                                   detection                               |
|																			|
|	The upper-left subwindow shows the original image						|
|	The upper-right subwindow shows the results of a color thresholding		|
|		operation (with saturated colors for matched areas and a gray-level	|
|		copy of the image elsewhere											|
|	The lower-left subwindow shows the result of a "find all blobs"			|
|		operation (the right-click menus allows to change blob min size)	|
|	The lower-right subwindow tries to find a blob around the location of	|
|		a mouse click.  The target color and min blob size are selectable	|
|		through a right-click menu entry									|
+--------------------------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <list>
#include <cstdlib>
#include <cmath>
//
//	uriVisionLib headers
//
#include "ImageReader.h"
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "BlobFinder.h"
#include "ColorThresholdLabeler.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
const string color_dir = COLOR_DIR_PATH;

const string filePath = color_dir + "mandrill_256x256.tga";
//const string filePath = color_dir + "blobTest.jpg";
//const string filePath = color_dir + "blobTest3.tga";


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

const int   MENU_SEPARATOR = -1,
            QUIT_MENU = 0,
            //
            INCREASE_THRESH_MENU = 2,
            DECREASE_THRESH_MENU = 3,
            //
            TOGGLE_SHOW_BCKGND_IMAGE_MENU = 4,
            TOGGLE_SHOW_BLOBS_MENU = 5,
            TOGGLE_SHOW_BLOB_CONTOURS_MENU = 6;
            
//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
//
void loadImage(void);
void initThresholds(void);
void detectAllBlobs(void);
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
void myLabelSubmenu(int value);
void myColorSubmenu(int value);
//
void myMouse3(int button, int buttonState, int ix ,int iy);
void myKeyboard(unsigned char c, int x, int y);
void myInit(void);
void myIdle(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int		                gWinWidth,
                        gWinHeight,
                        gImageWidth = kDefaultWidth,
                        gImageHeight = kDefaultHeight;

int			            gMainWindow,
                        gSubwindow[kNbSubwindows];

bool				    gInitDone = false;

FillingMode			    gModeWin[kNbSubwindows] = {kIdentityFill, kBlackFill, kWhiteFill, 
                                                    kOrigGrayFill};

RasterImage			    **gMyImage = NULL;	//	an array of references to Image objects
LabelRaster			    *gLabel2, *gLabel3;
BlobList 			    *gBlobList2, gBlobList3;
ColorThresholdLabeler	*gThresholder;
ColorSegParams		    *gColorParams;
BlobFinder			    *gBlobFinder = new BlobFinder();
char				    gTargetIndex = (char) -1;
long				    gBlobMinSize = 20;
bool                    gShowBackgndImage = true,
                        gShowBlobs = true,
                        gShowBlobContours = false;

//
//	For menu handling demo
//
#define	kNbBckgndCols		5
//
GLfloat	kBackgroundColor[kNbBckgndCols][4] =	{	{1.f, 0.f, 0.f, 1.f},	//	red
                                                    {0.f, 1.f, 0.f, 1.f},	//	green
                                                    {0.f, 0.f, 0.f, 1.f},	//	black
                                                    {1.f, 1.f, 0.f, 1.f},	//	yellow
                                                    {0.f, 1.f, 1.f, 1.f}	//	cyan
                                                 };
string	kBckgndColStr[] =	{
                                "Red",
                                "Green" ,
                                "Black",
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
    ImageReader	myReader(const_cast<char*>(filePath.c_str()));

    gMyImage = new RasterImage *[kNbImages];

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

    gThresholder = new ColorThresholdLabeler();
    
    //	select a black background
    gColorParams->fillMode = kOrigGrayFill;
    gMyImage[UPPER_RIGHT] = new RasterImage_RGBa(gImageHeight, gImageWidth);
    gThresholder->applyInto(gMyImage[UPPER_LEFT], (char *) gColorParams, gMyImage[UPPER_RIGHT]);

    gMyImage[LOWER_LEFT] = new RasterImage_gray(gMyImage[UPPER_LEFT]);
    gLabel2 = gThresholder->applyTo(gMyImage[UPPER_LEFT], gColorParams);
    //gLabel2 = gThresholder->applyTo(gMyImage[UPPER_LEFT], gColorParams, true);
    gBlobList2 = new BlobList[gColorParams->nbModes];
    detectAllBlobs();

    gLabel3 = gThresholder->applyTo(gMyImage[UPPER_LEFT], gColorParams);
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
}


void detectAllBlobs(void)
{
    //	clear the lists of blobs for each target color
    for (int k=0; k<gColorParams->nbModes; k++)
    {
        BlobList::iterator	iter;
        for (iter=gBlobList2[k].begin(); iter!=gBlobList2[k].end(); iter++)
            //	delete the blob allocated
            delete (*iter);

        gBlobList2[k].clear();
    }
	delete []gBlobList2;
	
    //  apply color thresholding
    gThresholder->applyInto(gMyImage[UPPER_LEFT], gColorParams, gLabel2);
    //gThresholder->applyInto(gMyImage[UPPER_LEFT], gColorParams, gLabel2, true);
    char	*target = new char[gColorParams->nbModes];
    for (int m=0; m<gColorParams->nbModes; m++)
        target[m] = gColorParams->colorReg[m].label;

    //  Detect the blobs and assign to each one of them a filling color to render them
    //gBlobFinder->findAllBlobs(gLabel2, gColorParams->nbModes, target, gBlobMinSize, gBlobList2);
//    gBlobFinder->findAllBlobs(gLabel2, gColorParams->nbModes,
//    						  target, gBlobMinSize, gBlobList2, true);
    gBlobList2 = gBlobFinder->findAllBlobs(gLabel2, gColorParams->nbModes,
    						  target, gBlobMinSize, true);
//    						  target, gBlobMinSize, false);
    for (int m1=0; m1<gColorParams->nbModes ; m1++)
        if (!gBlobList2[m1].empty())
        {
            BlobList::iterator iter, endList = gBlobList2[m1].end();
            for (iter = gBlobList2[m1].begin(); iter != endList; iter++)
            {
                (*iter)->setColor((gColorParams->colorReg[m1]).fillColor);
                (*iter)->setContourColor(1.f, 1.f, 1.f);
            }

        }
    delete []target;
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

    glutSetWindow(gMainWindow);
}


void myDisplay0(void)
{
    glutSetWindow(gSubwindow[UPPER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gMyImage[UPPER_LEFT]!= NULL)
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

    if (gMyImage[UPPER_RIGHT]!= NULL)
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

    if (gShowBackgndImage && (gMyImage[LOWER_LEFT]!= NULL))
        gMyImage[LOWER_LEFT]->render();

    for (int k=0; k<gColorParams->nbModes ; k++)
        if (!gBlobList2[k].empty())
        {
            BlobList::iterator iter;
            for (iter = gBlobList2[k].begin(); iter != gBlobList2[k].end(); iter++)
                if (gShowBlobs)
                    (*iter)->render(true);

        }

    glutSwapBuffers();
    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT);

    if (gMyImage[UPPER_LEFT]!= NULL)
        gMyImage[UPPER_LEFT]->render();

    if (!gBlobList3.empty())
    {
        BlobList::iterator iter;
        for (iter = gBlobList3.begin(); iter != gBlobList3.end(); iter++)
            (*iter)->render(true);

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

    glutPostRedisplay();
}

//	Mouse event handling function for subwindow 3.
void myMouse3(int button, int buttonState, int ix ,int iy)
{
    if (gMyImage[0] != NULL)
    {
        if(buttonState == GLUT_UP && button == GLUT_LEFT_BUTTON)
        {
            if (gTargetIndex >= 0)
            {
                ImagePoint	*pos = new ImagePoint(ix, iy);
                ImageRect	*theRect = new ImageRect(ix-5, iy-5, 11, 11);
                Blob	*myBlob = gBlobFinder->findBlobAbout(gLabel3, pos,
                               gColorParams->colorReg[gTargetIndex].label,
                               gBlobMinSize, theRect);
                if (myBlob != NULL)
                {
                    //	add the block to the list
                    gBlobList3.push_back(myBlob);

                    //	I want to display a frame around my new blob
                    myBlob->setDrawFrame(true);

                    //	I want my blob drawn in the saturated color of my color seg params
                    myBlob->setColor(gColorParams->colorReg[gTargetIndex].fillColor);
                }
                delete pos;
                delete theRect;

                myDisplayMain();
            }
        }

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
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case INCREASE_THRESH_MENU:
            gBlobMinSize += 5;
            detectAllBlobs();
            myDisplayMain();
            break;

        case DECREASE_THRESH_MENU:
            if (gBlobMinSize >= 10)
            {
                gBlobMinSize -= 5;
                detectAllBlobs();
                myDisplayMain();
            }
            break;

        case TOGGLE_SHOW_BCKGND_IMAGE_MENU:
            gShowBackgndImage = !gShowBackgndImage;
            myDisplayMain();
            break;

        case TOGGLE_SHOW_BLOBS_MENU:
            gShowBlobs = !gShowBlobs;
            myDisplayMain();
            break;

        case TOGGLE_SHOW_BLOB_CONTOURS_MENU:
            gShowBlobContours = !gShowBlobContours;
            for (int k=0; k<gColorParams->nbModes ; k++)
                if (!gBlobList2[k].empty())
                {
                    BlobList::iterator iter;
                    for (iter = gBlobList2[k].begin(); iter != gBlobList2[k].end(); iter++)
        				(*iter)->setDrawContour(gShowBlobContours);

                }
            myDisplayMain();
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

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;
    }
}

void myLabelSubmenu(int targetIndex)
{
    gTargetIndex = targetIndex;
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
    int i;
    // Create Menus
    int menuMain, menu[kNbSubwindows], colorSubmenu, labelSubmenu;

    //	Set a general menu for the main window
    glutSetWindow(gMainWindow);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    //	Submenu for background color selection choices
    colorSubmenu = glutCreateMenu(myColorSubmenu);
    for (i=0; i<kNbBckgndCols; i++)
        glutAddMenuEntry(kBckgndColStr[i].c_str(), i);

    //------------------------------------
    // Main menu for the window
    //------------------------------------
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
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for upper-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for lower-left quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_LEFT]);
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddSubMenu("Frame color", colorSubmenu);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddMenuEntry("Raise blob size threshold", INCREASE_THRESH_MENU);
    glutAddMenuEntry("Lower blob size threshold", DECREASE_THRESH_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddMenuEntry("Toggle show background image", TOGGLE_SHOW_BCKGND_IMAGE_MENU);
    glutAddMenuEntry("Toggle show blobs", TOGGLE_SHOW_BLOBS_MENU);
    glutAddMenuEntry("Toggle show blob contours", TOGGLE_SHOW_BLOB_CONTOURS_MENU);
    //
    glutAttachMenu(GLUT_RIGHT_BUTTON);


    //------------------------------------
    //  menus for lower-right quadrant
    //------------------------------------
    //	Submenu for target color selection choices
    labelSubmenu = glutCreateMenu(myLabelSubmenu);
    for (i=0; i<gColorParams->nbModes; i++)
    {
        string	menuStr = " target color = " + gColorParams->colorReg[i].labelStr;
        const	char *labelStr = menuStr.c_str();
        glutAddMenuEntry(labelStr, i);
    }
    //
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    menu[LOWER_RIGHT] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddSubMenu("Target", labelSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMouseFunc(myMouse3);

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

        initThresholds();
        loadImage();

        //	I initialize my window's dimensions at the default values
        gWinHeight = kNbRowsSubwindows*(gImageHeight + kBorder) + kBorder;
        gWinWidth = kNbColsSubwindows*(gImageWidth + kBorder) + kBorder;

        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Blob Filling Demo");

        glutKeyboardFunc(myKeyboard);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);
		glutIdleFunc(myIdle);

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);

        winID = 0;
        glutSetWindow(gMainWindow);
        for (int i=0; i<kNbRowsSubwindows; i++)
            for (int j=0; j<kNbColsSubwindows; j++)
            {
                gSubwindow[winID] = glutCreateSubWindow(
                                        gMainWindow,
                                        j*(gImageWidth + kBorder) + kBorder,
                                        i*(gImageHeight + kBorder) + kBorder,
                                        gImageWidth,
                                        gImageHeight);

                //	I don't need to do this.  I just set a different "clear" color for
                //	each subwindow.
                backColor = kBackgroundColor[winID];
                glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

                //	This *has* to be defined:  a "display" callback function for each subwindow
                glutDisplayFunc(displayFuncTable[winID++]);

                glutKeyboardFunc(myKeyboard);

                //	This calls maps "world" coordinates to "screen" coordinates.
                //	In this particular case I make them coincide.
                setupCamera(gImageWidth, gImageHeight);
                //
                glutSetWindow(gMainWindow);
            }

        //  Inform drawable objects of the dimensions of my rendering windows
        DrawableObject2D::setDefaultWindowDimensions(gImageWidth, gImageHeight);

        myInit();

        glutMainLoop();
        cleanupAndQuit(0);
    }
    catch (uriVL::ErrorReport & e)
	{
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }

	return 0;
}
