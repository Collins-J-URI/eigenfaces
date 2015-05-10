/*  NAME:
        patternCreation_main.cpp
 
    DESCRIPTION:
        demonstration application for the Pattern2D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|	patternCreation_main.cpp                                                |
|   This project demonstrates how to create a blob pattern by grabbing into |
|   a raster image the pixels that correspond to a blob priorly detected in |
|   the image									                            |
|																			|
|	Author:	jean-yves herve', Department of Computer Science and Statistics	|
|								University of Rhode Island					|	
|																			|
|	Version	1.0 [01/25/2006]												|
|																			|
|	The upper-left subwindow shows the original image						|
|	The upper-right subwindow shows the results of a color thresholding		|
|		operation (with saturated colors for matched areas and a gray-level	|
|		copy of the image elsewhere											|
|	The lower-left subwindow shows the result of a "find all blobs"			|
|		operation (the right-click menus allows to change blob min size)	|
|	The lower-right subwindow shows the Pattern2D objects defined at the    |
|       location of the blobs           									|
+--------------------------------------------------------------------------*/

#include <iostream>
#include <list>
//
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
#include "ErrorReport.h"
#include "Pattern2D.h"

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
            INCREASE_THRESH_MENU = 2,
            DECREASE_THRESH_MENU = 3,
            TOGGLE_DRAW_FRAME_MENU = 4,
            TOGGLE_DRAW_CONTOUR_MENU = 5;
            

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
void myColorSubmenu(int value);
//
void myKeyboard(unsigned char c, int x, int y);
void myInit(void);


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
LabelRaster			    *gLabel;
ColorThresholdLabeler	*gThresholder = new ColorThresholdLabeler();
ColorSegParams		    *gColorParams;
BlobFinder			    *gBlobFinder = new BlobFinder();
long				    gBlobMinSize = 20;

BlobList 			    *gBlobList;
Pattern2DList           gPatList;
ImagePointList          gOriginList;
bool                    gDrawBoundFrames = false;
bool                    gDrawContours = false;

//
//	For menu handling demo
//
#define	kNbBckgndCols		5
//
GLfloat	kBackgroundColor[kNbBckgndCols][4] =	{	{1.f, 0.f, 0.f, 1.f},	//	red
                                                    {0.f, 1.f, 0.f, 1.f},	//	green
                                                    {1.f, 1.f, 0.f, 1.f},	//	yellow
                                                    {0.f, 0.f, 0.f, 1.f},	//	black
                                                    {0.f, 1.f, 1.f, 1.f}	//	cyan
                                                 };
char	*kBckgndColStr[] =	{
                                "Red",
                                "Green" ,
                                "Yellow",
                                "Black",
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
    gMyImage = new RasterImage *[kNbImages];

    //-------------------------------------------------------------
    //	We read the image from a file
    //-------------------------------------------------------------
    const string filePath = color_dir + "mandrill_256x256.tga";
    ImageReader	myReader(const_cast<char*>(filePath.c_str()));
    gMyImage[UPPER_LEFT] = myReader.getFrame(true);
    if (gMyImage[UPPER_LEFT] ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath << endl;
        std::exit(-1);
    }
    gImageWidth = gMyImage[UPPER_LEFT]->getWidth();
    gImageHeight = gMyImage[UPPER_LEFT]->getHeight();

    //-------------------------------------------------------------
    //  Image Processing
    //-------------------------------------------------------------
    //  Next we apply color thresholding to the input image to produce the upper-right
    //  image.   The color regions are stored in gColorParams and were initialized in
    //  initThresholds(), called from the main jusy before this function.
    //  We select a black background for no-match filling
    gMyImage[UPPER_RIGHT] = new RasterImage_RGBa(gImageHeight, gImageWidth);
    gColorParams->fillMode = kOrigGrayFill;
    gThresholder->applyInto(gMyImage[UPPER_LEFT], (char *) gColorParams, gMyImage[UPPER_RIGHT]);

    //  I want to display my color blobs over a gray-level version of the input image
    gMyImage[LOWER_LEFT] = new RasterImage_gray(gMyImage[UPPER_LEFT]);

    gLabel = new LabelRaster(gImageHeight, gImageWidth);
    
    //  Detect all color-based blobs and create new patterns associated with these blobs.
    //  I need a list of blobs for each of the color regions sought
    gBlobList = new BlobList[gColorParams->nbModes];
    detectAllBlobs();
}


void initThresholds(void)
{
    //  We define the color regions used for the thresholding
    //   
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
    //-------------------------------------------------------
    //  1. Clear all old lists and label rasters
    //-------------------------------------------------------
    //	clear the lists of blobs for each target color
    for (int k=0; k<gColorParams->nbModes; k++)
    {
        BlobList::iterator	blobIter;
        for (blobIter=gBlobList[k].begin(); blobIter!=gBlobList[k].end(); blobIter++)
            //	delete the blob allocated
            delete (*blobIter);

        gBlobList[k].clear();
    }

    //  Also clear the list of patterns
    Pattern2DList::iterator patIter;
    for (patIter=gPatList.begin(); patIter!=gPatList.end(); patIter++)
        //	delete the pattern allocated
        delete (*patIter);
    gPatList.clear();
    
    //  Finally, clear the list of blobs' origins
    ImagePointList::iterator origIter;
    for (origIter=gOriginList.begin(); origIter!=gOriginList.end(); origIter++)
        //	delete the pattern allocated
        delete (*origIter);
    gOriginList.clear();
        
    //-------------------------------------------------------
    //  2. Detect the blobs for each color region
    //-------------------------------------------------------
    char	*target = new char[gColorParams->nbModes];
    for (int m=0; m<gColorParams->nbModes; m++)
        target[m] = gColorParams->colorReg[m].label;

    //  apply color thresholding and store the results in a LabelRaster
    gThresholder->applyInto(gMyImage[UPPER_LEFT], gColorParams, gLabel, true);
        
    //  Detect the blobs and assign to each one of them a filling color to render them
    //  also, create the corresponding patterns (and store their origin in a list)
    gBlobFinder->findAllBlobs(gLabel, gColorParams->nbModes, target, gBlobMinSize, gBlobList);
    for (int m=0; m<gColorParams->nbModes ; m++)
        if (!gBlobList[m].empty())
        {
            BlobList::iterator blobIter;
            for (blobIter = gBlobList[m].begin(); blobIter != gBlobList[m].end(); blobIter++)
            {
                (*blobIter)->setColor((gColorParams->colorReg[m]).fillColor);
                
                //  Create the pattern defined by the blob on the original image
                Pattern2D   *myPat = new Pattern2D(*blobIter, gMyImage[UPPER_LEFT]);
                gPatList.push_back(myPat);
                
                //  store the blob's origin, to display the pattern at that location
                const ImageRect   *blobRect = (*blobIter)->getValidRect();
                ImagePoint  *orig = new ImagePoint(blobRect->getLeft(), blobRect->getTop());
                gOriginList.push_back(orig);
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

    if (gMyImage[UPPER_LEFT]!= NULL)
    {
        gMyImage[UPPER_LEFT]->render();
    }

    glutSwapBuffers();
}

void myDisplay1(void)
{
    glutSetWindow(gSubwindow[UPPER_RIGHT]);

    if (gMyImage[UPPER_RIGHT]!= NULL)
    {
        gMyImage[UPPER_RIGHT]->render();
    }

    glutSwapBuffers();
}

void myDisplay2(void)
{
    glutSetWindow(gSubwindow[LOWER_LEFT]);

    if (gMyImage[LOWER_LEFT]!= NULL)
        gMyImage[LOWER_LEFT]->render();

    //  render the color blobs over the image.  Note that blobs still use abolute image
    //  coordinates, so there is no need to provide an origin for their rendering
    for (int k=0; k<gColorParams->nbModes ; k++)
        if (!gBlobList[k].empty())
        {
            BlobList::iterator blobIter;
            for (blobIter = gBlobList[k].begin(); blobIter != gBlobList[k].end(); blobIter++)
                (*blobIter)->render(true);
        }

    glutSwapBuffers();
    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT);

    //  Render the list of patterns.  Because a pattern only uses relative coordinates 
    //  (typically, the upper-left corner of a pattern is at (0, 0)), we must provide an
    //  origin for the rendering.
    if (!gPatList.empty())
    {
        Pattern2DList::iterator     patIter;
        ImagePointList::iterator    origIter;
        for (patIter = gPatList.begin(), origIter = gOriginList.begin(); 
                                         patIter != gPatList.end(); patIter++, origIter++)
            (*patIter)->render(*origIter);
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

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
}

void myMenu3(int value)
{
    Pattern2DList::iterator patIter, endList = gPatList.end();

    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case TOGGLE_DRAW_FRAME_MENU:
            gDrawBoundFrames = !gDrawBoundFrames;
            //
            if (gDrawBoundFrames)
                for (patIter = gPatList.begin(); patIter != endList; patIter++)
                {
                    (*patIter)->setDrawFrame(true);
                    (*patIter)->setFrameColor(0.f, 0.f, 1.f);
                }    
            else                                
                for (patIter = gPatList.begin(); patIter != endList; patIter++)
                    (*patIter)->setDrawFrame(false);

            myDisplayMain();
            break;
            
        case TOGGLE_DRAW_CONTOUR_MENU:
            gDrawContours = !gDrawContours;
            //
            if (gDrawContours)
                for (patIter = gPatList.begin(); patIter != endList; patIter++)
                {
                    (*patIter)->setDrawContour(true);
                    (*patIter)->setContourColor(1.f, 0.f, 0.f);
                }    
            else                                
                for (patIter = gPatList.begin(); patIter != endList; patIter++)
                    (*patIter)->setDrawContour(false);

            myDisplayMain();
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
#if 0
#pragma mark -
#endif


void myInit(void)
{
    int i;
    // Create Menus
    int menuMain, menu[kNbSubwindows], colorSubmenu, labelSubmenu;

    //	Set a general menu for the main window
    glutSetWindow(gMainWindow);

    //	Submenu for background color selection choices
    colorSubmenu = glutCreateMenu(myColorSubmenu);
    for (i=0; i<kNbBckgndCols; i++)
        glutAddMenuEntry(kBckgndColStr[i], i);

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
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("Raise blob size threshold", INCREASE_THRESH_MENU);
    glutAddMenuEntry("Lower blob size threshold", DECREASE_THRESH_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


    //------------------------------------
    //  menus for lower-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    menu[LOWER_RIGHT] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("Toggle draw pattern frames", TOGGLE_DRAW_FRAME_MENU);
    glutAddMenuEntry("Toggle draw pattern contours", TOGGLE_DRAW_CONTOUR_MENU);
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

        initThresholds();
        loadImage();

        //	I initialize my window's dimensions at the default values
        gWinHeight = kNbRowsSubwindows*(gImageHeight + kBorder) + kBorder;
        gWinWidth = kNbColsSubwindows*(gImageWidth + kBorder) + kBorder;

        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Blob Pattern Creation Demo");

        glutKeyboardFunc(myKeyboard);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);

        glClearColor(0.0, 1.0, 1.0, 1.0);

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

        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
