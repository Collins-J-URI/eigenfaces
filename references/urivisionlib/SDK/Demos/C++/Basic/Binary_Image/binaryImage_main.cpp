/*--------------------------------------------------------------------------+
|	binaryImage_main.cpp	A simple demo of the RasterImage_binary class	|
|																			|
|	Author:	jean-yves herve', Department of Computer Science and Statistics	|
|								University of Rhode Island					|	
|																			|
|	Version	1.0 [08/31/2005]												|
|																			|
|	o upper-left quadrant: original (gray-level) image                      |
|   o upper-right quadrant: a binary copy of that image (QuickDraw set the  |
|     threshold at 127-128)                                                 |
|   o lower-left quadrant: binary copy of an attenuated version of the      |
|       input image (intensity * 0.8)                                       |
|   o lower-right quadrant: modified version of the U-R binary image (added |
|       1 diagonal line)                                                    |
+--------------------------------------------------------------------------*/

#include <iostream>
//
#include "ImageReader.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray.h"
#include "RasterImage_binary.h"
#include "LabelRaster.h"
#include "GrayThresholdLabeler.h"
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
const string output_dir = OUTPUT_DIR_PATH;

//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	2
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
//
void loadImage(void);
//
void myReshape(int w, int h);
void setupCamera(int w, int h);
//
void myDisplayMain(void);
void myDisplay0(void);
void myDisplay1(void);
void myDisplay2(void);
void myDisplay3(void);
//
void myMenuMain(int value);
void myMenu0(int value);
void myMenu1(int value);
void myMenu2(int value);
void myMenu3(int value);
void myColorSubmenu(int value);
//
void myKeyboard(unsigned char c, int x, int y);
//
void myInit(void);
void myIdle(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int         gWinWidth,
            gWinHeight,
            gImageWidth = kDefaultWidth,
            gImageHeight = kDefaultHeight;

int			gMainWindow,
            gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];

bool		gInitDone = false;

RasterImage	**gMyImage = NULL;	//	an array of references to Image objects

const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1,
            LOWER_LEFT = 2,
            LOWER_RIGHT = 3;

const int   QUIT_MENU = 0;

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

void loadImage(void)
{
    gMyImage = new RasterImage*[kNbImages];

    //-------------------------------------------------------------
    //	We read Image 0 from a file
    //-------------------------------------------------------------
    const string filePath = gray_dir + "Lena_256x256.tga";
    ImageReader	myReader(const_cast<char*>(filePath.c_str()));
    gMyImage[UPPER_LEFT] = myReader.getFrame(true);
    if (gMyImage[UPPER_LEFT] ==NULL)
    {
        cout << "Error encountered while reading image file " << const_cast<char*>(filePath.c_str()) << endl;
        std::exit(-1);
    }

    gImageWidth = gMyImage[UPPER_LEFT]->getWidth();
    gImageHeight = gMyImage[UPPER_LEFT]->getHeight();
    
    //-------------------------------------------------------------
    //	Image 1 is a binary version of Image 0
    //-------------------------------------------------------------
    // Here the transformation [0-255] --> [0-1] is done by the CopyBits function
    // of QuickTime/QuickDraw.  It simply puts a threshold level at 127.  Below is 0,
    //  above is 1. 
    gMyImage[UPPER_RIGHT] = new RasterImage_binary(gMyImage[UPPER_LEFT]);
    

    //-------------------------------------------------------------
    //	Image 3 is Image 1 to which I add 1 diagonal line
    //-------------------------------------------------------------
    gMyImage[LOWER_RIGHT] = new RasterImage_binary(gMyImage[UPPER_RIGHT]);
	

    //-------------------------------------------------------------
    //	Image 2 is created out of an attenuated copy of Image 0
    //-------------------------------------------------------------
    RasterImage_gray    *tempGray = new RasterImage_gray(gMyImage[UPPER_LEFT]);
    const unsigned char *const* grayIn = gMyImage[UPPER_LEFT]->getShiftedRaster2D();
    unsigned char *const* grayOut = tempGray->getShiftedRaster2D(R_W_ACCESS);
    
    //  ... and I proceedd to applying the 0.8 attenuation
    const ImageRect *rect = gMyImage[UPPER_LEFT]->getValidRect();
    const int   iLow = rect->getTop(),
                iHigh = rect->getBottom(),
                jLow = rect->getLeft(),
                jHigh = rect->getRight();
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
            grayOut[i][j] = static_cast<unsigned char>(0.8 * grayIn[i][j]);
    //  Same coment as for Image 1.  The thresholding is done by QuickTime
    gMyImage[LOWER_LEFT] = new RasterImage_binary(tempGray);
    delete tempGray;

    //  Write a black cross on the image
    //--------------------------------------
	grayOut = ((RasterImage_binary *) gMyImage[LOWER_RIGHT])->getGrayRasterImage()->getShiftedGray2D(R_W_ACCESS);
    //  UL - LR diagonal
    for (int i=iLow, j=jLow; (i<=iHigh) && (j<=jHigh); i++, j++) {
        //  any non-zero value will count as 1 when binarized
        grayOut[i][j] = static_cast<unsigned char>(0);
        
        if (i>iLow)
            grayOut[i-1][j] = static_cast<unsigned char>(0);
        if (i>iLow+1)
            grayOut[i-2][j] = static_cast<unsigned char>(0);
        if (i<iHigh)
            grayOut[i+1][j] = static_cast<unsigned char>(0);
        if (i<iHigh-1)
            grayOut[i+2][j] = static_cast<unsigned char>(0);
    }
    //  UR - LL diagonal
    for (int i=iLow, j=jHigh; (i<=iHigh) && (j>=jLow); i++, j--) {
        //  any non-zero value will count as 1 when binarized
        grayOut[i][j] = static_cast<unsigned char>(0);
        if (i>iLow)
            grayOut[i-1][j] = static_cast<unsigned char>(0);
        if (i>iLow+1)
            grayOut[i-2][j] = static_cast<unsigned char>(0);
        if (i<iHigh)
            grayOut[i+1][j] = static_cast<unsigned char>(0);
        if (i<iHigh-1)
            grayOut[i+2][j] = static_cast<unsigned char>(0);
     }
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

    if (gMyImage[UPPER_LEFT]!= NULL)
        gMyImage[UPPER_LEFT]->render();

    glutSwapBuffers();
}

void myDisplay1(void)
{
    glutSetWindow(gSubwindow[UPPER_RIGHT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gMyImage[UPPER_RIGHT]!= NULL)
        gMyImage[UPPER_RIGHT]->render();

    glutSwapBuffers();
}

void myDisplay2(void)
{

    glutSetWindow(gSubwindow[LOWER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gMyImage[LOWER_LEFT]!= NULL)
        gMyImage[LOWER_LEFT]->render();

    glutSwapBuffers();
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[LOWER_RIGHT]);

    glClear(GL_COLOR_BUFFER_BIT);

    if (gMyImage[LOWER_RIGHT]!= NULL)
        gMyImage[LOWER_RIGHT]->render();

    glutSwapBuffers();
}

#if 0
#pragma mark -
#endif

void myReshape(int w, int h)
{
    setupCamera(w, h);

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
            //	0 --> Exit/Quit
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

        glutPostRedisplay();
    }
}


void myIdle(void)
{
	if (gInitDone) 
	{
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
    glutAddSubMenu("Color", colorSubmenu);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    menu[1] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[LOWER_LEFT]);
    menu[2] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    menu[3] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gMainWindow);

    gInitDone = true;
}


int main(int argc, char** argv)
{
    int		winID;
    GLfloat	*backColor;
    void	(*displayFuncTable[kNbSubwindows])() = {  myDisplay0, myDisplay1, 
                                                      myDisplay2, myDisplay3};
    try {
		glutInit(&argc, argv);

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

		initializeQTEnvironment();

		loadImage();

		//	I initialize my window's dimensions at the default values
		gWinHeight = kNbRowsSubwindows*(gImageHeight + kBorder) + kBorder;
		gWinWidth = kNbColsSubwindows*(gImageWidth + kBorder) + kBorder;

		glutInitWindowSize(gWinWidth,gWinHeight);
		gMainWindow = glutCreateWindow("Binary Image Application Demo");

		glutKeyboardFunc(myKeyboard);
		glutDisplayFunc(myDisplayMain);
		glutIdleFunc(myIdle);

		glClearColor(0.0, 0.0, 1.0, 1.0);

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

				glutKeyboardFunc(myKeyboard);

				//	This calls maps "world" coordinates to "screen" coordinates.
				//	In this particular case I make them coincide.
				setupCamera(gImageWidth, gImageHeight);

				//	This *has* to be defined:  a "display" callback function for each subwindow
				glutDisplayFunc(displayFuncTable[winID++]);

				glutSetWindow(gMainWindow);
			}

		myInit();

		//  and I inform drawable objects of the image's dimensions (in case they want to get 
		//  rendered "shifted")
		DrawableObject2D::setDefaultWindowDimensions(gImageWidth, gImageHeight);
		
		myDisplayMain();

		glutMainLoop();

		cleanupAndQuit(0);
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }

    return 0;
}
