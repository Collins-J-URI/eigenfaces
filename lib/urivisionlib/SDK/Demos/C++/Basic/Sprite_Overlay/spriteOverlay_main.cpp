/*  NAME:
        spriteOverlay_main.cpp
 
    DESCRIPTION:
        demonstration application for the writeInto and overlay functions of
        the RasterImage class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|   This demo application shows how to use the overlay functions of the     |
|   RasterImage class.                                                      |
|   o The upper-left subwindow shows a color image                          |
|   o The upper-right subvwindow shows a gray-level image                   |
|   o The lower-left subwindow shows the result of overlaying small images  |
|       (gray-level or color) on the upper-left image                       |
|   o The lower-right subwindow shows the result of overlaying small images |
|       (gray-level or color) on the upper-right image                      |
|																			|
|	The user can move the "sprite" images by use of the arrow keys when in  |
|   "relative" overlay mode (an origin is specified for the upper-left      |
|   corner of the sprite's valid data rectangle.                            |
|   The user can switch from "absolute" (the sprite's valid data rectangle  |
|   determine the location in the image to  "relative" display modes by     |
|   hitting the space bar.                                                  |
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|	
+--------------------------------------------------------------------------*/

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
//
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"
#include "ImageReader.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;

const string color_dir = COLOR_DIR_PATH;
const string gray_dir = GRAY_DIR_PATH;

const string filePath0 = color_dir + "bottles_256x256.tga";
const string filePath1 = gray_dir + "Lena_256x256.tga";
const string filePath2 = color_dir + "umbrellas_480x400.tga";
const string filePath3 = gray_dir + "camera_256x256.tga";


//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	2
#define	kNbColsSubwindows	2
#define	kNbSubwindows	    kNbRowsSubwindows*kNbColsSubwindows
//
#define kNbImages           4
#define kNbSprites          4
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
//
void loadImages(void);
void applyOverlays(void);
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
void mySpecialKeys(int c, int x, int y);
void myKeyboard(unsigned char c, int x, int y);
void cleanupAndQuit(void);
void myInit(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
short			gWinWidth,
                gWinHeight,
                gImageWidth = kDefaultWidth,
                gImageHeight = kDefaultHeight;

int				gMainWindow,
                gSubwindow[kNbSubwindows];

bool			gInitDone = false;

RasterImage		**gInputImage = NULL;	//	an array of references to Image objects
RasterImage		**gDisplayImage = NULL;	//	an array of references to Image objects
RasterImage     **gSprite = NULL;       //  the sprites we will ovewrlay on an image
ImagePoint      **gSpriteOrigin;
int             gActiveSprite = 0;
bool            gSpriteUseOrigin = false;


const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1,
            LOWER_LEFT = 2,
            LOWER_RIGHT = 3;


const int   QUIT_MENU = 0;

const int   SPRITE_DISPLCT = 5;


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


void loadImages(void)
{
    ImageReader myReader;
    
    gInputImage = new RasterImage*[kNbImages];

    //-------------------------------------------------------------
    //	We read Images 0-2 from a file
    //-------------------------------------------------------------
    myReader.setSource(const_cast<char*>(filePath0.c_str()));
    gInputImage[0] = myReader.getFrame(true);
    if (gInputImage[0] ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath0 << endl;
        cleanupAndQuit(-1);
    }

    myReader.setSource(const_cast<char*>(filePath1.c_str()));
    gInputImage[1] = myReader.getFrame(false);
    if (gInputImage[1] ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath1 << endl;
        cleanupAndQuit(-1);
    }

    myReader.setSource(const_cast<char*>(filePath2.c_str()));
    gInputImage[2] = myReader.getFrame(true);
    if (gInputImage[2] ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath2 << endl;
        cleanupAndQuit(-1);
    }

    myReader.setSource(const_cast<char*>(filePath3.c_str()));
    gInputImage[3] = myReader.getFrame(true);
    if (gInputImage[3] ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath3 << endl;
        cleanupAndQuit(-1);
    }

    gImageWidth = gInputImage[0]->getWidth();
    gImageHeight = gInputImage[0]->getHeight();
    
    //-------------------------------------------------------------
    //	We allocate the array of display images
    //-------------------------------------------------------------
    gDisplayImage = new RasterImage*[kNbSubwindows];
    // 
    gDisplayImage[UPPER_LEFT] = gInputImage[0];
    gDisplayImage[UPPER_RIGHT] = gInputImage[1];
    gDisplayImage[LOWER_LEFT] = new RasterImage_RGBa(gImageHeight, gImageWidth);
    gDisplayImage[LOWER_RIGHT] = new RasterImage_gray(gImageHeight, gImageWidth);
    
    //-------------------------------------------------------------
    //  We "grab" the sprites and assign them origins 
    //-------------------------------------------------------------
    gSprite = new RasterImage*[kNbSprites];
    gSpriteOrigin = new ImagePoint*[kNbSprites];
    //
    ImageRect   rect(50, 70, 100, 60);
    gSprite[0] = gInputImage[2]->grab(&rect);
    gSpriteOrigin[0] = new ImagePoint(120, 30);

    rect.setRect(180, 200, 50, 50);
    gSprite[1] = gInputImage[2]->grab(&rect);
    gSpriteOrigin[1] = new ImagePoint(90, 150);

    rect.setRect(100, 40, 60, 40);
    gSprite[2] = gInputImage[3]->grab(&rect);
    gSpriteOrigin[2] = new ImagePoint(30, 120);

    rect.setRect(20, 175, 30, 70);
    gSprite[3] = gInputImage[3]->grab(&rect);
    gSpriteOrigin[3] = new ImagePoint(60, 60);    

    applyOverlays();
}


void applyOverlays(void)
{
    //  In the lower-left quadrant we display image 0  + overlays
    gInputImage[0]->copyInto(gDisplayImage[LOWER_LEFT]);
    //
    //  we do the overlays by using the function writeInto

    for (int i=0; i<kNbSprites; i++)
        if (gSpriteUseOrigin)
            gSprite[i]->writeInto(gDisplayImage[LOWER_LEFT], gSpriteOrigin[i]);
        else
            gSprite[i]->writeInto(gDisplayImage[LOWER_LEFT]);
        

    //  In the lower-right quadrant we display image 1  + overlays
    gInputImage[1]->copyInto(gDisplayImage[LOWER_RIGHT]);
    //
    //  we do the overlays by using the function overlay

    for (int i=0; i<kNbSprites; i++)
        if (gSpriteUseOrigin)
            gDisplayImage[LOWER_RIGHT]->overlay(gSprite[i], gSpriteOrigin[i]);
        else
            gDisplayImage[LOWER_RIGHT]->overlay(gSprite[i]);
    
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

    if (gDisplayImage[UPPER_LEFT]!= NULL)
    {
        gDisplayImage[UPPER_LEFT]->render(true);
    }

    glutSwapBuffers();
}

void myDisplay1(void)
{
    glutSetWindow(gSubwindow[UPPER_RIGHT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gDisplayImage[UPPER_RIGHT]!= NULL)
    {
        gDisplayImage[UPPER_RIGHT]->render(true);
    }

    glutSwapBuffers();
}

void myDisplay2(void)
{

    glutSetWindow(gSubwindow[LOWER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gDisplayImage[LOWER_LEFT]!= NULL)
    {
        gDisplayImage[LOWER_LEFT]->render(false);
    }

    glutSwapBuffers();
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT);

    if (gDisplayImage[LOWER_RIGHT]!= NULL)
    {
        gDisplayImage[LOWER_RIGHT]->render(true);
    }

    glutSwapBuffers();
}
#if 0
#pragma mark -
#endif

void myReshape(int w, int h)
{
    glutSetWindow(gMainWindow);
    setupCamera(w, h);

    //  and I inform drawable object of these dimensions (in case they want to get rendered
    //  "shifted")
    DrawableObject2D::setDefaultWindowDimensions(gImageWidth, gImageHeight);

    glutPostRedisplay();
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


void mySpecialKeys(int c, int x, int y)
{
    switch (c)
    {
        case GLUT_KEY_LEFT:
            gSpriteOrigin[gActiveSprite]->shiftX(-SPRITE_DISPLCT);
            applyOverlays();
            break;
            
        case GLUT_KEY_RIGHT:
            gSpriteOrigin[gActiveSprite]->shiftX(+SPRITE_DISPLCT);
            applyOverlays();
            break;

        case GLUT_KEY_UP:
            gSpriteOrigin[gActiveSprite]->shiftY(-SPRITE_DISPLCT);
            applyOverlays();
            break;

        case GLUT_KEY_DOWN:
            gSpriteOrigin[gActiveSprite]->shiftY(+SPRITE_DISPLCT);
            applyOverlays();
            break;

        default:
            break;

    }

    glutSetWindow(gMainWindow);
    glutPostRedisplay();
}

void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 'q':
            cleanupAndQuit(0);
            break;

        case '[':
            gActiveSprite = (gActiveSprite + kNbSprites - 1) % kNbSprites;
            break;
                        
        case ']':
            gActiveSprite = (gActiveSprite + 1) % kNbSprites;
            break;
            
        case ' ':
            gSpriteUseOrigin = !gSpriteUseOrigin;
            applyOverlays();
            break;
                        
        default:
            break;

    }
    glutSetWindow(gMainWindow);
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
    int menuMain, menu[4], colorSubmenu;

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
    menu[UPPER_LEFT] = glutCreateMenu(myMenu0);
    glutAddSubMenu("Color", colorSubmenu);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[LOWER_LEFT]);
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    menu[LOWER_RIGHT] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gMainWindow);

    gInitDone = true;
}


int main(int argc, char** argv)
{
    int		winID;
    GLfloat	*backColor;
    void	(*displayFuncTable[5])() = {myDisplayMain, myDisplay0, myDisplay1, 
                                        myDisplay2, myDisplay3};

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    initializeQTEnvironment();

    loadImages();

    //	I initialize my window's dimensions at the default values
    gWinWidth = kNbColsSubwindows*(gImageWidth + kBorder) + kBorder;
    gWinHeight = kNbRowsSubwindows*(gImageHeight + kBorder) + kBorder;

    glutInitWindowSize(gWinWidth,gWinHeight);
    gMainWindow = glutCreateWindow("Sprite Overlay Demo");

    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKeys);
    glutDisplayFunc(myDisplayMain);
    glutReshapeFunc(myReshape);

    glClearColor(0.0, 0.0, 0.0, 1.0);

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

            //  Here I use the same keyboard function for my 4 subwindows, but I could
            //  set up different ones.
            glutSpecialFunc(mySpecialKeys);
            glutKeyboardFunc(myKeyboard);

            //	This calls maps "world" coordinates to "screen" coordinates.
            //	In this particular case I make them coincide.
            setupCamera(gImageWidth, gImageHeight);
            //
            glutSetWindow(gMainWindow);
        }

    //  Initializes QuickTime and loads the image
    myInit();

    //  Enter infinite glut loop
    glutMainLoop();

    //  Normally this code should never be placed since the only way to leave the
    //  glut loop is to quit
    cleanupAndQuit(0);

    return 0;
}
