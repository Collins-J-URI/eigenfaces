/*  NAME:
        imageDifference_main.cpp
 
    DESCRIPTION:
        demonstration application for ImageArithmetics class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

//  This project demonstrates the use of the BackgroundRemover classes (here
//  used to compute image differences).	
//      o the upper left and upper right subwindows show the input images
//      o the lower-left and lower right subwindows shows the difference image 
//          calculated using one of the menu-selectable algorithms:
//              - "AVERAGE_DIFFERENCE" 
//                  difference image is the average of (absolute values of) red, green,
//                  and blue differences
//              - "MAX_DIFFERENCE"
//              - "HUE_DIFFERENCE"
//  Other difference algorithms to be implemented later
//          function (after setting the first image as static background)																		|
//
//  Note:
//  Difference between images was initially implemented in the BackgroundRemover class
//  hierarchy, more specifically in the BackgroundRemover_static class.  As of 
//  2007-04-16, it has been moved into the new ImageArithmetics class.
//
//  Version	1.0 [06/12/2004]    Fang Han, 3DG
//                  Initial BR implementation and difference demo
//  Version 1.1 [09/15/2005]    Meng Wan & jyh, 3DG
//  			    Transition to new BR class hierarchy
//  Version 1.2 [12/08/2005]    jyh, 3DG
//  			    Code cleanup and lower subwindow differentiation
//  Version 1.3 [11/02/2006]    jyh, 3DG
//                  Revised data file paths, fixed code for XCode & CodeBlocks
//                  Removed #if code differenciation and added menu-based interface
//  Version 1.4 [03/20/2007]    jyh, 3DG
//                  Revised data file paths and added exeception handling code
//                  (part of "generic" overhaul)
//  Version 1.5 [04/16/2007]    jyh, 3DG
//                  modified the demo to work with new ImageArithmetics class instead
//                  of BackgroundRemover


#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
//
#include "ImageReader.h"
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "ImageArithmetics.h"
//
#include "Demos/dataFolderPath.h"


using namespace std;
using namespace uriVL;


//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
const string color_dir = COLOR_DIR_PATH;
const string gray_dir = GRAY_DIR_PATH;

const string filePath0 = color_dir + "car1.tga";
const string filePath1 = color_dir + "car2.tga";

//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	2
#define	kNbColsSubwindows	2
#define kNbSubwindows       kNbRowsSubwindows*kNbColsSubwindows
#define	kNbImages			kNbSubwindows
//
#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640

//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
void loadImage(void);
void computeImageDifferences(void);
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
void myMouse(int button, int state, int ix ,int iy);
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

RasterImage** gMyImage = NULL;	//	an array of references to Image objects

RasterImage* gDiffImgHue = NULL;
RasterImage* gDiffImgAvg = NULL;
RasterImage* gDiffImgMax = NULL;


//---------------------------
//  Interface elements
//---------------------------
const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1,
            LOWER_LEFT = 2,
            LOWER_RIGHT = 3;

const int   QUIT_MENU = 0;
const int   SEPARATOR = -1;
const int   AVERAGE_DIFF_MENU = 10,
            MAX_DIFF_MENU = 11,
            HUE_DIFF_MENU = 12;

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


void loadImage(void)
{
    gMyImage = new RasterImage*[kNbImages];

    //-------------------------------------------------------------
    //	We read Image 0 from a file
    //-------------------------------------------------------------
    ImageReader		*myReader = new ImageReader(const_cast<char*>(filePath0.c_str()));

    gMyImage[UPPER_LEFT] = myReader->getFrame(true);
    if (gMyImage[UPPER_LEFT] == NULL)
    {
        cout << "Error encountered while reading image file " << filePath0 << endl;
        cleanupAndQuit(-1);
    }

    //-------------------------------------------------------------
    //	We read Image 1 from a file
    //-------------------------------------------------------------
    myReader->setSource(const_cast<char*>(filePath1.c_str()));

    gMyImage[UPPER_RIGHT] = myReader->getFrame(true);
    if (gMyImage[UPPER_RIGHT] == NULL)
    {
        cout << "Error encountered while reading image file " << filePath1 << endl;
        cleanupAndQuit(-1);
    }
	
	gMyImage[LOWER_LEFT] = gMyImage[LOWER_RIGHT] = NULL;
}


void computeImageDifferences(void)
{
    //  Difference between images was implemented in the BackgroundRemover class
    //  hierarchy, more specifically in the BackgroundRemover_static class.
    //  We view background removal as a "computer vision" operation, mostly because
    //  we are more interested in dynamic, adaptive background removal techniques,
    //  which is why basic image difference ended up in the uriComputerVision namespace.

    // difference of "current" and "background" images using HUE_DIFFERENCE
    gDiffImgHue = ImageArithmetics::difference(gMyImage[UPPER_LEFT],
                                                gMyImage[UPPER_RIGHT],
                                                HUE_DIFFERENCE);

    //  Difference of "current" and "background" images using AVERAGE_DIFFERENCE
    gDiffImgAvg = ImageArithmetics::difference(gMyImage[UPPER_LEFT], gMyImage[UPPER_RIGHT],
                                               AVERAGE_DIFFERENCE);
    
    //  Difference of "current" and "background" images using MAX_DIFFERENCE
    //  First, I can keep reusing the same background with new images.
    //  Here I show that we can also use a pre-allocated output image and
    //  pass it for the difference function to write into.
    gDiffImgMax = new RasterImage_gray( gMyImage[UPPER_LEFT]->getHeight(),
                                        gMyImage[UPPER_LEFT]->getWidth());
    ImageArithmetics::difference(gMyImage[UPPER_LEFT],  gMyImage[UPPER_RIGHT],  
                                 (RasterImage_gray *) gDiffImgMax,
                                 MAX_DIFFERENCE);
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

    if (gMyImage[UPPER_LEFT] != NULL)
		gMyImage[UPPER_LEFT]->render();

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay1(void)
{
    glutSetWindow(gSubwindow[UPPER_RIGHT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gMyImage[UPPER_RIGHT] != NULL)
        gMyImage[UPPER_RIGHT]->render();

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay2(void)
{

    glutSetWindow(gSubwindow[LOWER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gMyImage[LOWER_LEFT] != NULL)
		gMyImage[LOWER_LEFT]->render();

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT);

    if (gMyImage[LOWER_RIGHT] != NULL)
		gMyImage[LOWER_RIGHT]->render();

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

        case AVERAGE_DIFF_MENU:
            gMyImage[LOWER_LEFT] =gDiffImgAvg ;
            break;
            
        case MAX_DIFF_MENU:
            gMyImage[LOWER_LEFT] = gDiffImgMax;
            break;
            
        case HUE_DIFF_MENU:
            gMyImage[LOWER_LEFT] = gDiffImgHue;
            break;
            
        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }

    glutPostRedisplay();
}

void myMenu3(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case AVERAGE_DIFF_MENU:
            gMyImage[LOWER_RIGHT] = gDiffImgAvg ;
            break;
            
        case MAX_DIFF_MENU:
            gMyImage[LOWER_RIGHT] = gDiffImgMax;
            break;
            
        case HUE_DIFF_MENU:
            gMyImage[LOWER_RIGHT] = gDiffImgHue;
            break;
            
        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }

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
    int menuMain, menu[kNbSubwindows], colorSubmenu;

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

    glutSetWindow(gSubwindow[UPPER_LEFT]);
    menu[0] = glutCreateMenu(myMenu0);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("Reset Images", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    menu[1] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[LOWER_LEFT]);
    menu[2] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Average difference", AVERAGE_DIFF_MENU);
    glutAddMenuEntry("Max difference", MAX_DIFF_MENU);
    glutAddMenuEntry("Hue difference", HUE_DIFF_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    menu[3] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Average difference", AVERAGE_DIFF_MENU);
    glutAddMenuEntry("Max difference", MAX_DIFF_MENU);
    glutAddMenuEntry("Hue difference", HUE_DIFF_MENU);
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
   try 
   {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);


        initializeQTEnvironment();

        //	I load my images and compute differences
        loadImage();
        computeImageDifferences();
    	int	imgHeight = gMyImage[UPPER_LEFT]->getHeight(),
    	    imgWidth = gMyImage[UPPER_LEFT]->getWidth();
    	
        //	I inform all drawable objects of the dimensions of their subwindow
        DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

        //	I initialize my window's dimensions
        gWinHeight = kNbRowsSubwindows * (imgHeight + kBorder) + kBorder;
        gWinWidth = kNbColsSubwindows * (imgWidth + kBorder) + kBorder;

        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Background Remover Demo");

        GLfloat	*backColor;
        void	(*displayFuncTable[kNbSubwindows])() = {myDisplay0, myDisplay1, 
                                                        myDisplay2, myDisplay3};
        glutKeyboardFunc(myKeyboard);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);
		glutIdleFunc(myIdle);

        int winID = 0;
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
    			glutKeyboardFunc(myKeyboard);

                //	This *has* to be defined:  a "display" callback function for each subwindow
                glutDisplayFunc(displayFuncTable[winID++]);

                //	This calls maps "world" coordinates to "screen" coordinates.
                //	In this particular case I make them coincide.
                setupCamera(imgWidth, imgHeight);
                //
                glutSetWindow(gMainWindow);
            }

        myInit();
        glutSetWindow(gMainWindow);
        glClearColor(0.f, 1.f, 1.f, 1.f);
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
