/*  NAME:
        main_imageHistogram.cpp

    DESCRIPTION:
        demonstration application for the HistogramCalculator class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|   This simple application demonstrates how to get and plat the histogram  |
|   of a RasterImage_gray object.                                           |
|   The input image is displayed in the left subwindow                      |
|   Initially the histogram of that image is displayed in the right half.   |
|   A right-click drop menu in the right half allows the user to select     |
|       - the render mode (LIN_LIN or LIN_LOG) of the histogram             |
|       - whether or not to display the axes                                |
|       - whether or not to display a title for the histogram.              |
|           Actually, this is not implemented yet because I am having 2nd   |
|           thoughts about the pertinence of this functionality which       |
|           forces me to use glut within the library (something that I      |
|           have been trying to avoid)                                      |
|       - the color of the histogram                                        |
|       - the color of the histogram's background                           |
|       - the color of the histogram' axes                                  |
|       - the color of the histogram' title                                 |
|                                                                           |
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|
|																			|
+--------------------------------------------------------------------------*/

#include <iostream>
//
#include "ImageReader.h"
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "ImageHistogram.h"
#include "HistogramCalculator.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
const string color_dir = COLOR_DIR_PATH;
const string gray_dir = GRAY_DIR_PATH;
const string output_dir = OUTPUT_DIR_PATH;


//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	1
#define	kNbColsSubwindows	2
#define	kNbSubwindows		kNbRowsSubwindows*kNbColsSubwindows
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
//
void myDisplayMain(void);
void myDisplay0(void);
void myDisplay1(void);
//
void myReshape(int w, int h);
void settupCamera(int w, int h);
//
void myMouse(int button, int state, int ix ,int iy);
void myKeyboard(unsigned char c, int x, int y);
void myMouseMotion(int ix ,int iy);
bool withinBounds(int ix, int iy);
//
void myMenuMain(int value);
void myMenu0(int value);
void myMenu1(int value);
//
void myFrameColorSubmenu(int value);
void myHistogramColorSubmenu(int value);
void myHistogramAxesColorSubmenu(int value);
void myHistogramTitleColorSubmenu(int value);
void myHistogramBackgroundColorSubmenu(int value);
void myHistogramRenderModeSubmenu(int value);
//
void myInit(void);

//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int			    gWinWidth,
                gWinHeight,
                gImageWidth = kDefaultWidth,
                gImageHeight = kDefaultHeight;

int				gMainWindow,
                gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];

bool			gInitDone = false;
bool            gDisplayAxes = false;
bool            gDisplayLabels = false;

RasterImage         *gMyImage = NULL;
RasterImage_gray    *gGrayImage = NULL;
ImageHistogram      *gGrayHist = NULL;

const int	kMinSearchDim = 20;
bool		gMouseDown = false;
int		    gDragWin;
int			gStartDragX,
            gStartDragY;

const int   UPPER_LEFT = 0,
            UPPER_RIGHT = 1;

const int   QUIT_MENU = 0,
            SEPARATOR = 1000,
            RESET_MENU = 1,
            TOGGLE_DISPLAY_AXES_MENU = 2,
            TOGGLE_DISPLAY_TITLE_MENU = 3;



//
//	For menu handling demo
//
#define	kNbColorChoices		5
//
GLfloat	kColorChoice[kNbColorChoices][4] =	{	{0.f, 0.f, 0.f, 1.f},	//	black
                                                    {1.f, 0.f, 0.f, 1.f},	//	red
                                                    {0.f, 1.f, 0.f, 1.f},	//	green
                                                    {1.f, 1.f, 0.f, 1.f},	//	yellow
                                                    {0.f, 1.f, 1.f, 1.f}	//	cyan
                                                                                         };
string kColStr[] =	{
                                "Black",
                                "Red",
                                "Green" ,
                                "Yellow",
                                "Cyan"
                            };


#define kNbRenderModes      2
//
HistogramRenderMode kRenderMode[kNbRenderModes] = {
                                                    LIN_LIN,
                                                    LIN_LOG
                                                  };

char    *kRenderModeStr[kNbRenderModes] = {
                                            "LIN_LIN",
                                            "LIN_LOG"
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
    const string grayFilePath = gray_dir + "Lena_256x256.tga";

    //-------------------------------------------------------------
    //	We read the image from a file
    //-------------------------------------------------------------
    ImageReader myReader(const_cast<char*>(grayFilePath.c_str()));
    gGrayImage = static_cast<RasterImage_gray*>(myReader.getFrame(true));
    if (gGrayImage ==NULL)
    {
        cout << "Error encountered while reading image file " << grayFilePath << endl;
		cleanupAndQuit(-1);
    }
    gMyImage = gGrayImage;
    
    gGrayHist = new ImageHistogram(gGrayImage);
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
}


void myDisplay0(void)
{
    glutSetWindow(gSubwindow[UPPER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
    {
        gMyImage->render();
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
        gGrayHist->render();
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
void settupCamera(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(	0.0f, (GLfloat) w, 0.0f, (GLfloat) h);

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


void myMouse(int button, int buttonState, int ix ,int iy)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (buttonState == GLUT_DOWN)
        {
            //  If the mouse button was not already pressed down, we start recording
            if (!gMouseDown)
            {
                gMouseDown = true;
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
                    ImageRect myRect(theLeft, theTop, theWidth, theHeight);
                    gGrayHist->setImage(gGrayImage, &myRect);
                }
            }
            gMouseDown = false;
            gDragWin = -1;

            myDisplayMain();
        }
    }
}


void myMouseMotion(int ix ,int iy)
{
    if (gMouseDown)
    {
        glutSetWindow(gSubwindow[UPPER_LEFT]);

        glClear(GL_COLOR_BUFFER_BIT);

        if (gMyImage!= NULL)
            gMyImage->render();

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
    return ((ix>=0) && (ix<gMyImage->getWidth()) &&
            (iy>=0) && (iy<gMyImage->getHeight()));
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

        case RESET_MENU:
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

        case TOGGLE_DISPLAY_AXES_MENU:
            gDisplayAxes = !gDisplayAxes;
            gGrayHist->setDrawAxes(gDisplayAxes);
            glutPostRedisplay();
            break;

        case TOGGLE_DISPLAY_TITLE_MENU:
            gDisplayLabels = !gDisplayLabels;
            gGrayHist->setDrawTitle(gDisplayAxes);
            glutPostRedisplay();
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why is this happening?" << endl;
            break;

    }
}

void myFrameColorSubmenu(int colorIndex)
{
    if ((colorIndex >= 0) && (colorIndex < kNbColorChoices))
    {
        GLfloat		*backColor;

        glutSetWindow(gMainWindow);

        backColor = kColorChoice[colorIndex];
        glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}

void myHistogramColorSubmenu(int colorIndex)
{
    if ((colorIndex >= 0) && (colorIndex < kNbColorChoices))
    {
        GLfloat		*histColor = kColorChoice[colorIndex];
        gGrayHist->setColor(histColor[0], histColor[1], histColor[2]);

        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}

void myHistogramAxesColorSubmenu(int colorIndex)
{
    if ((colorIndex >= 0) && (colorIndex < kNbColorChoices))
    {
        GLfloat		*histColor = kColorChoice[colorIndex];
        gGrayHist->setAxesColor(histColor[0], histColor[1], histColor[2]);

        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}

void myHistogramTitleColorSubmenu(int colorIndex)
{
    if ((colorIndex >= 0) && (colorIndex < kNbColorChoices))
    {
        GLfloat		*histColor = kColorChoice[colorIndex];
        gGrayHist->setTitleColor(histColor[0], histColor[1], histColor[2]);

        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}

void myHistogramBackgroundColorSubmenu(int colorIndex)
{
    if ((colorIndex >= 0) && (colorIndex < kNbColorChoices))
    {
        GLfloat		*histColor = kColorChoice[colorIndex];
        gGrayHist->setBackgroundColor(histColor[0], histColor[1], histColor[2]);

        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}

void myHistogramRenderModeSubmenu(int modeIndex)
{
    if ((modeIndex >= 0) && (modeIndex < kNbRenderModes))
    {
        gGrayHist->setRenderMode(kRenderMode[modeIndex]);

        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}


#if 0
#pragma mark -
#endif


void myInit(void)
{
    // Create Menus
    int menuMain, menu[kNbSubwindows], frameColorSubmenu, histColorSubmenu, 
        histRenderModeSubmenu, histAxesColorSubmenu, histBackgroundColorSubmenu,
        histTitleColorSubmenu;

    //	Set a general menu for the main window
    glutSetWindow(gMainWindow);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    //	Submenu for background color selection choices
    frameColorSubmenu = glutCreateMenu(myFrameColorSubmenu);
    for (int i=0; i<kNbColorChoices; i++)
        glutAddMenuEntry(kColStr[i].c_str(), i);

    // Main menu that the submenus are connected to
    glutSetWindow(gMainWindow);
    menuMain = glutCreateMenu(myMenuMain);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------------------------------------------------
    //  Left subwindow: select gray-color, frame color, quit
    //------------------------------------------------------------------------------
    glutSetWindow(gSubwindow[UPPER_LEFT]);
    glutMotionFunc(myMouseMotion);
    glutMouseFunc(myMouse);
    menu[UPPER_LEFT] = glutCreateMenu(myMenu0);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddSubMenu("Frame color", frameColorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------------------------------------------------
    //  right subwindow: select histogram display modes & color, frame color, quit
    //------------------------------------------------------------------------------
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    //
    //	Submenu for histogram color selection choices
    histColorSubmenu = glutCreateMenu(myHistogramColorSubmenu);
    for (int i=0; i<kNbColorChoices; i++)
        glutAddMenuEntry(kColStr[i].c_str(), i);
    //
    //	Submenu for histogram axes color selection choices
    histAxesColorSubmenu = glutCreateMenu(myHistogramAxesColorSubmenu);
    for (int i=0; i<kNbColorChoices; i++)
        glutAddMenuEntry(kColStr[i].c_str(), i);
    //
    //	Submenu for histogram background color selection choices
    histBackgroundColorSubmenu = glutCreateMenu(myHistogramBackgroundColorSubmenu);
    for (int i=0; i<kNbColorChoices; i++)
        glutAddMenuEntry(kColStr[i].c_str(), i);
    //
    //	Submenu for histogram title color selection choices
    histTitleColorSubmenu = glutCreateMenu(myHistogramTitleColorSubmenu);
    for (int i=0; i<kNbColorChoices; i++)
        glutAddMenuEntry(kColStr[i].c_str(), i);
    //
    //  Submenu for histogram render mode selection 
    histRenderModeSubmenu = glutCreateMenu(myHistogramRenderModeSubmenu);
    for (int i=0; i<kNbRenderModes; i++)
        glutAddMenuEntry(kRenderModeStr[i], i);
    //   
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    //
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Toggle display axes", TOGGLE_DISPLAY_AXES_MENU);
    glutAddMenuEntry("Toggle display title", TOGGLE_DISPLAY_TITLE_MENU);
    glutAddSubMenu("Histogram render mode", histRenderModeSubmenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddSubMenu("Histogram color", histColorSubmenu);
    glutAddSubMenu("Histogram background color", histBackgroundColorSubmenu);
    glutAddSubMenu("Histogram axes color", histAxesColorSubmenu);
    glutAddSubMenu("Histogram title color", histTitleColorSubmenu);
    glutAddSubMenu("Histogram rendering mode", histRenderModeSubmenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddSubMenu("Frame color", frameColorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gMainWindow);

    gInitDone = true;
}


int main(int argc, char** argv)
{
    int		winID;
    GLfloat	*backColor;
    void	(*displayFuncTable[kNbSubwindows])() = {myDisplay0, myDisplay1};

	try 
	{
        //	initialization code.  You will always need this
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        initializeQTEnvironment();

        //	I load my image
        loadImage();

        int	imgHeight = gMyImage->getHeight(),
            imgWidth = gMyImage->getWidth();

        //	I initialize my window's dimensions
        gWinHeight = kNbRowsSubwindows * imgHeight + (kNbRowsSubwindows + 1) * kBorder;
        gWinWidth = kNbColsSubwindows * imgWidth + (kNbColsSubwindows + 1) * kBorder;
        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Image Histogram Demo");
        glClearColor(0.0, 0.0, 0.0, 1.0);

        //  and I inform drawable object of these dimensions (in case they want to get rendered
        //  "shifted")
        DrawableObject2D::setDefaultWindowDimensions(gImageWidth, gImageHeight);

        //	in all applications with a GUI you will need to define some
        //	of these callback functions
        glutKeyboardFunc(myKeyboard);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);
        glutMouseFunc(myMouse);

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
                backColor = kColorChoice[winID];
                glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

                //	This *has* to be defined:  a "display" callback function for each subwindow
                glutDisplayFunc(displayFuncTable[winID++]);
                glutKeyboardFunc(myKeyboard);

                //	This calls maps "world" coordinates to "screen" coordinates.
                //	In this particular case I make them coincide.
                settupCamera(imgWidth, imgHeight);
                //
                glutSetWindow(gMainWindow);
            }

        myInit();
        glutSetWindow(gMainWindow);
        glutPostRedisplay();

		//  Enter infinite glut loop
		glutMainLoop();

		//  Normally this call should never be placed since the only way to leave the
		//  glut loop is to quit
		cleanupAndQuit(0);
	}
	catch (uriVL::ErrorReport & e) 
	{
		cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
		return 1;
	}

	return 0;
}
