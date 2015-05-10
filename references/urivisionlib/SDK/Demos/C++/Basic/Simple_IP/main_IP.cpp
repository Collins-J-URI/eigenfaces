/*  NAME:
        simpleIP.cpp

    DESCRIPTION:
        demonstration application for basic image processing operations

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/



/*--------------------------------------------------------------------------+
|	This simple application demonstrates how to access the base raster of a |
|  RasterImage object to perform simple Image Processing operations.        |
|                                                                           |
|  The original image and three examples of IP output are displayed in the  |
|  four quadrants (subwindows) of the application window.                   |
|                                                                           |
|  Additionally, the application also demonstrates the overlay of vector    |
|  graphics on top of a rendered RasterImage, as well as the use of the     |
|  VectorField class.                                                       |
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|
|																			|
+--------------------------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <cmath>
//
#include "ImageReader.h"
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "VectorField_F.h"
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

const string filePath = color_dir + "mandrill_256x256.jpg";


//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	2
#define	kNbColsSubwindows	2
#define	kNbSubwindows	    kNbRowsSubwindows*kNbColsSubwindows
#define	kNbImages			kNbSubwindows
//
#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640


#ifndef	PI
#define	PI	3.14159265
#endif

//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
void loadImage(void);
void resetImages(void);
void simpleImageTransform(void);
//
void drawOneCircle(void);
void fillOneDisk(void);
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
//
void myColorSubmenu(int value);
void myVectFieldSubmenu(int value);
void myMouse(int button, int state, int ix ,int iy);
void myMouse3(int button, int state, int ix ,int iy);
void myKeyboard(unsigned char c, int x, int y);
void myInit(void);
void myIdle(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int	    gWinWidth,
        gWinHeight,
        gImageWidth = kDefaultWidth,
        gImageHeight = kDefaultHeight,
        //
        gCurrentFieldRes = 30;

int		gMainWindow,
        gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];

bool	gInitDone = false,
        gDrawStuff = false,
        gDrawVectField = false;

unsigned char	gImageTreshold = 100;

RasterImage     **gMyImage = NULL;	//	an array of references to Image objects
VectorField_F	*gVectField = NULL;

const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1,
            LOWER_LEFT = 2,
            LOWER_RIGHT = 3;

const int   QUIT_MENU = 0,
            RESET_IMAGES_MENU = 1,
            NEIGHBOR_AVERAGE_MENU = 2,
            TOGGLE_VECTOR_ON_OFF_MENU = 3,
            COARSER_RESOLUTION_MENU = 4,
            FINER_RESOLUTION_MENU = 5,
            RED_COLOR_MENU = 6,
            GREEN_COLOR_MENU = 7,
            BLUE_COLOR_MENU = 8,
            MIRROR_MENU = 9,
            TOGGLE_DRAW_STUFF__ON_OFF_MENU = 10;

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

#if 0
#pragma mark -
#endif
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
    //	We read the input image from a file
    //-------------------------------------------------------------
    ImageReader	myReader(filePath.c_str());
    gMyImage[UPPER_LEFT] = myReader.getFrame(true);

    if (gMyImage[UPPER_LEFT] ==NULL)
    {
        cout << "Error encountered while reading image file " << const_cast<char*>(filePath.c_str()) << endl;
        std::exit(-1);
    }

    int   m0 = gMyImage[UPPER_LEFT]->getHeight(),
          n0 = gMyImage[UPPER_LEFT]->getWidth();

    //-------------------------------------------------------------
    //	Make copies into images 1-2-3
    //-------------------------------------------------------------
    //	Image 1 is just Image 0 converted to gray level
    gMyImage[UPPER_RIGHT] = new RasterImage_gray(gMyImage[0]);

    //	Images 2 and 3 are created with all pixels set to 0
    gMyImage[LOWER_LEFT] = new RasterImage_gray(m0, n0);
    gMyImage[LOWER_RIGHT] = new RasterImage_gray(m0, n0);
    //	then we fill them, using the values from image1

    simpleImageTransform();


    //	Just to show how to use the vectorial image type
    float	*rx, *ry;
    long	rowBase;

    rx = new float[m0*n0];
    ry = new float[m0*n0];

    rowBase = 0;
    for (int i=0; i<m0; i++)
    {
        for (int j=0; j<n0; j++)
        {
            rx[rowBase] = sinf(j/30.f)*cosf(i/20.f);
            ry[rowBase++] = sinf(j/50.f);
        }
    }
    gVectField = new VectorField_F(m0, n0, rx, ry);
    gVectField->setVectResolution(gCurrentFieldRes);
    delete  []rx;
    delete  []ry;
}

void resetImages(void)
{
    gMyImage[0]->copyInto(gMyImage[1]);
    simpleImageTransform();
}


//	Copies into image 2 the inverse of image 1 and into image 3 the thresholded image 1
void simpleImageTransform(void)
{
    int   nbRows = gMyImage[1]->getHeight(),
          nbCols = gMyImage[1]->getWidth();
    
    //  Image1 will only be read, not modified, so we can use the version of the getRaster
    //  family of functions that return a read-only reference
    const unsigned char *const* rs1 = gMyImage[1]->getRaster2D();
 
    //  we are going to modify the rasters of images 2 and 3, so we use the version of the
    //  getRaster family of functions that return a read-write reference
    unsigned char *const* rs2 = gMyImage[2]->getRaster2D(R_W_ACCESS),
				  *const* rs3 = gMyImage[3]->getRaster2D(R_W_ACCESS);

    for (int i=0; i<nbRows; i++)
        for (int j=0; j<nbCols; j++)
        {
            //	Image 2 is Image 1 inverted
            rs2[i][j] = static_cast<unsigned char>(255 - rs1[i][j]);

            //	Image 3 is a thresholded version of Image 1
            if (rs1[i][j] > gImageTreshold)
                rs3[i][j] = rs1[i][j];
            else
                rs3[i][j] = 0;
        }
}


#if 0
#pragma mark -
#endif


//---------------------------------------------------------------------------------
//	myDisplay() functions
//---------------------------------------------------------------------------------

//	draws a unit circle
void drawOneCircle(void)
{
    glBegin(GL_LINE_LOOP);
    //	in a real application, I would pre-allocate my array of vertices'
    //	coordinates and only do the drawing in this function.
    for (int i=0; i<=15; i++)
        glVertex2d(cos(2.*i*PI/16), sin(2.*i*PI/16));
    glEnd();
}

void fillOneDisk(void)
{
    glBegin(GL_POLYGON);
    //	in a real application, I would pre-allocate my array of vertices'
    //	coordinates and only do the drawing in this function.
    for (int i=0; i<=15; i++)
        glVertex2d(cos(2.*i*PI/16), sin(2.*i*PI/16));
    glEnd();
}

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

    if (gInitDone)
    {
        gMyImage[UPPER_LEFT]->render();
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
        gMyImage[UPPER_RIGHT]->render();
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay2(void)
{
    int m2 = gMyImage[1]->getHeight(),
        n2 = gMyImage[1]->getWidth();

    glutSetWindow(gSubwindow[LOWER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
    {
        gMyImage[LOWER_LEFT]->render();
    }


    if (gDrawVectField)
        gVectField->render();

    else
        if (gDrawStuff)
        {
            //	draws a red "diamond" shaped polygon that connects the middle points
            //	of the sides of subwindow 2
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_LINE_LOOP);
            glVertex2i(n2/2, 0);
            glVertex2i(n2, m2/2);
            glVertex2i(n2/2, m2);
            glVertex2i(0, m2/2);
            glEnd();

            //	draws a multicolored square of side 100 in the lower left corner
            glBegin(GL_LINE_LOOP);
            glColor3f(1.0, 0.0, 0.0);
            glVertex2i(5, 5);
            glColor3f(0.0, 1.0, 0.0);
            glVertex2i(105, 5);
            glColor3f(0.0, 0.0, 1.0);
            glVertex2i(105, 105);
            glColor3f(1.0, 1.0, 0.0);
            glVertex2i(5, 105);
            glEnd();

            //	draws a blue circle of radius 40 at the center of the image
            glPushMatrix();	//	save the current state
            glTranslatef(n2/2.f, m2/2.f, 0.f);
            glScalef(40.f, 40.f, 1.0f);
            glColor3f(0.0, 0.0, 1.0);
            drawOneCircle();
            glPopMatrix();	//	restore the state that had been pushed

            //	draws a tilted (angle 30 degrees) dark green ellipse with a magenta border
            //	in the upper right corner of the window.
            glPushMatrix();	//	save the current state
            glTranslatef(n2-60.f, m2-60.f, 0.f);
            glRotatef(30.f, 0.f, 0.f, 1.f);
            glScalef(50.f, 30.f, 1.0f);
            glColor3f(0.0f, 0.6f, 0.0f);
            fillOneDisk();
            glColor3f(0.8f, 0.0f, 0.8f);
            drawOneCircle();
            glPopMatrix();	//	restore the state that had been pushed
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
        gMyImage[LOWER_RIGHT]->render();
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

            //	[ and ] do something to the images
        case '[':
            std::exit(0);
            break;
        case ']':
            break;

        default:
            break;

    }
}


void myMouse(int button, int state, int ix ,int iy)
{}

//	Mouse event handling function for subwindow 3.

void myMouse3(int button, int state, int ix ,int iy)
{
//     float   x, y;  // unused variables. -CJC

    // if the left mouse button is clicked, do something
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        if (gImageTreshold < static_cast<unsigned char>(250))
            gImageTreshold += static_cast<unsigned char>(5);
        else
            gImageTreshold = static_cast<unsigned char>(255);

        simpleImageTransform();
        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }

    // if the middle mouse button is clicked, do something
    if(state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON)
    {
        if (gImageTreshold > static_cast<unsigned char>(5))
            gImageTreshold -= static_cast<unsigned char>(5);
        else
            gImageTreshold = static_cast<unsigned char>(0);

        simpleImageTransform();
        glutSetWindow(gMainWindow);
        glutPostRedisplay();
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

        case RESET_IMAGES_MENU:
            resetImages();
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
}

void myMenu1(int menuVal)
{
    switch (menuVal)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case NEIGHBOR_AVERAGE_MENU: {
            //	This is not the best place to do it.  I just wanted to show what
            //  could be done with these rasters
            const int	nbRows = gMyImage[1]->getHeight(),
						nbCols = gMyImage[1]->getWidth();

            long	nbPixels /* , rowBase  unused var. -CJC */;

            nbPixels = nbRows * nbCols;

            //  Image1 will be modified, so we must use the version of the getRaster
            //  family of functions that return a read-write reference
            unsigned char *const* rs1 = gMyImage[1]->getRaster2D(R_W_ACCESS);
            unsigned char **tempBuf = new unsigned char*[nbRows];

            for (int k=0; k<nbRows; k++)
                tempBuf[k] = new unsigned char[nbCols];

            for (int i=0; i<nbRows; i++)
            {
                for (int j=0; j<nbCols; j++)
                {
                    int val = rs1[i][j];
                    if (j>0)
                        val += rs1[i][j-1];
                    if (j<nbCols-1)
                        val += rs1[i][j+1];
                    if (i>0)
                        val += rs1[i-1][j];
                    if (i<nbRows-1)
                        val += rs1[i+1][j];

                    tempBuf[i][j] = static_cast<unsigned char>(val/ 5);
                }
            }
            for (int i=0; i<nbRows; i++)
            {
                for (int j=0; j<nbCols; j++)
                    rs1[i][j] = tempBuf[i][j];
            }

            simpleImageTransform();
            delete []tempBuf;
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            }
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why is this happening?" << endl;
            break;

    }
}

void myMenu2(int menuVal)
{
    switch (menuVal)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        //	Does some image Mirroring
        //	This is not the best place to do it.  I just wanted to show what
        //  could be done
        case MIRROR_MENU: {
            const int	nbRows = gMyImage[2]->getHeight(),
						nbCols = gMyImage[2]->getWidth();

            long	nbPixels  /* , rowBase  unused var. -CJC */;

            nbPixels = nbRows * nbCols;

            //  Image2 will be modified, so we must use the version of the getRaster
            //  family of functions that return a read-write reference
            unsigned char *const* rs2 = gMyImage[2]->getRaster2D(R_W_ACCESS);
            for (int i=0; i<nbRows/2; i++)
            {
                for (int j=0; j<nbCols; j++)
                {
                    unsigned char tmp = rs2[i][j];
                    rs2[i][j] = rs2[nbRows-i-1][j];
                    rs2[nbRows-i-1][j] = tmp;
                }
            }
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            }
            break;

        //	Toggles on-off whether GL stuff should be drawn on top of the image
        case TOGGLE_DRAW_STUFF__ON_OFF_MENU:
            gDrawStuff = !gDrawStuff;
            if (gDrawStuff)
                gDrawVectField = false;
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
}

void myVectFieldSubmenu(int vSelect)
{
    switch (vSelect)
    {
        //	Toggles on-off vector field rendering.  When vector field rendering is on, the
        //	other GL junk is not drawn.
        case TOGGLE_VECTOR_ON_OFF_MENU:
            gDrawVectField = !gDrawVectField;
            if (gDrawVectField)
                gDrawStuff = false;
            glutSetWindow(gMainWindow);
            glutPostRedisplay();
            break;

        case COARSER_RESOLUTION_MENU:
            if (gDrawVectField)
            {
                if (gCurrentFieldRes < 60)
                    gCurrentFieldRes += 5;
                gVectField->setVectResolution(gCurrentFieldRes);
                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        case FINER_RESOLUTION_MENU:
            if (gDrawVectField)
            {
                if (gCurrentFieldRes > 10)
                    gCurrentFieldRes -= 5;
                gVectField->setVectResolution(gCurrentFieldRes);
                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        //	vector field is drawn in red
        case RED_COLOR_MENU:
            if (gDrawVectField)
            {
                gVectField->setColor(1.f, 0.f, 0.f);
                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        //	vector field is drawn in green
        case GREEN_COLOR_MENU:
            if (gDrawVectField)
            {
                gVectField->setColor(0.f, 1.f, 0.f);
                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        //	vector field is drawn in blue
        case BLUE_COLOR_MENU:
            if (gDrawVectField)
            {
                gVectField->setColor(0.f, 0.f, 1.f);
                glutSetWindow(gMainWindow);
                glutPostRedisplay();
            }
            break;

        default:
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
        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}


void myInit(void)
{
    // Create Menus
    int menuMain, menu[4], colorSubmenu, vectFieldMenu;

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
    glutAddMenuEntry("Reset Images", RESET_IMAGES_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("Neighbor average", NEIGHBOR_AVERAGE_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[LOWER_LEFT]);
    vectFieldMenu = glutCreateMenu(myVectFieldSubmenu);
    glutAddMenuEntry("Toggle on-off", TOGGLE_VECTOR_ON_OFF_MENU);
    glutAddMenuEntry("Coarser resolution", COARSER_RESOLUTION_MENU);
    glutAddMenuEntry("Finer resolution", FINER_RESOLUTION_MENU);
    glutAddMenuEntry("Color: red", RED_COLOR_MENU);
    glutAddMenuEntry("Color: green", GREEN_COLOR_MENU);
    glutAddMenuEntry("Color: blue", BLUE_COLOR_MENU);
    //
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddMenuEntry("Mirror", MIRROR_MENU);
    glutAddMenuEntry("Toggle draw stuff", TOGGLE_DRAW_STUFF__ON_OFF_MENU);
    glutAddSubMenu("Vector field", vectFieldMenu);
    glutAddSubMenu("Color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[LOWER_RIGHT]);
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
	try {
		int		winID;
		GLfloat	*backColor;
		void	(*displayFuncTable[kNbSubwindows])() = {myDisplay0, myDisplay1, 
														myDisplay2, myDisplay3};

		//	initialization code.  You will always need this
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
		initializeQTEnvironment();

		//	I load my images
		loadImage();

		int imgHeight = gMyImage[0]->getHeight(),
			imgWidth = gMyImage[0]->getWidth();
		//  and I inform drawable object of these dimensions (in case they want to get rendered
		//  "shifted")
		DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

		//	I initialize my window's dimensions
		gWinHeight = 2 * imgHeight + 3 * kBorder;
		gWinWidth = 2 * imgWidth + 3 * kBorder;
		glutInitWindowSize(gWinWidth,gWinHeight);
		gMainWindow = glutCreateWindow("Image Processing Demo");
		glClearColor(0.0, 0.0, 0.0, 1.0);


		//	in all applications with a GUI you will need to define some
		//	of these callback functions
		glutKeyboardFunc(myKeyboard);
		glutDisplayFunc(myDisplayMain);
		glutReshapeFunc(myReshape);
		glutMouseFunc(myMouse);
		glutIdleFunc(myIdle);

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
				backColor = kBackgroundColor[winID+1];
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

		//  Initializes QuickTime, the menus, and loads the images
		myInit();
		glutSetWindow(gMainWindow);
		myDisplayMain();

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
