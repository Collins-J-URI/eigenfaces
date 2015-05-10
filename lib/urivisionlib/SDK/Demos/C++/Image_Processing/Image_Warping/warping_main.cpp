/*  NAME:
        warping_main.cpp
 
    DESCRIPTION:
        demonstration application for ImageWarper_GL classes
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|	This simple application demonstrates the use of ImageWarper classes  to |
|   apply geometric transformations to an image.                            |
|   This demo uses the OpenGL implementation of warping (ImageWarper_GL),   |
|   which in fact at the time of this writing is the sole implementation in |
|   the library, but any other future implementation could be used instead, |
|   the only difference that OpenGL places restrictions (on some platforms) |
|   on the dimensions of its texture maps.                                  |
|                                                                           |
|	The input image is displayed in the left subwindow, with the original   |
|   warping grid overlaid on it.                                            |
|   The right subwindow displays the warped image with the current warping  |
|   grid overlaid on it.  The user can warp the image by clicking one of    |
|   the warping grid's nodes.  The active node changes from blue to red.    |
|   The user can then click to where this point should be moved (within     |
|   the image).                                                             |
|                                                                           |
|	Author:	jean-yves herve',                                               |
|                 3D Group for Interactive Visualization,                   |
|                 Department of Computer Science and Statistics	            |
|		          University of Rhode Island					            |	
+--------------------------------------------------------------------------*/

//#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
//
#include "ImageReader.h"
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_RGBa_F.h"
#include "ImageWarper_GL.h"
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
#define	kNbRowsSubwindows	1
#define	kNbColsSubwindows	2
#define kNbSubwindows       kNbRowsSubwindows*kNbColsSubwindows
#define	kNbImages			kNbSubwindows
//
#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	900//640

//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
//
void initImagesAndWarper(void);
void setupWarper(void);
void applyWarp(void);
void resetGrid(void);
void deleteGrids(void);
//
void myDisplayMain(void);
void myDisplay0(void);
void myDisplay1(void);
void precalcCircle(void);
//
void myReshape(int w, int h);
void setupCamera(int w, int h);
//
void myMenuMain(int value);
void myMenu0(int value);
void myMenu1(int value);
//
void myColorSubmenu(int value);
void myGridSizeMenu(int value);
void myMouse(int button, int buttonState, int ix ,int iy);
void myKeyboard(unsigned char c, int x, int y);
void myInit(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
//  GUI stuff
//
const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1;

const int   QUIT_MENU = 0;
const int   RESET_MENU = 1;
const int   SEPARATOR = 1000;

const int   FOUR_BY_FOUR_GRID = 404;
const int   FOUR_BY_SIX_GRID = 406;
const int   FIVE_BY_FIVE_GRID = 505;
const int   SIX_BY_FOUR_GRID = 604;
const int   SIX_BY_SIX_GRID = 606;

#define	kNbBckgndCols		5
//
GLfloat	kBackgroundColor[kNbBckgndCols][4] =	{	{0.f, 0.f, 0.f, 1.f},	//	black
                                                    {1.f, 0.f, 0.f, 1.f},	//	red
                                                    {0.f, 1.f, 0.f, 1.f},	//	green
                                                    {1.f, 1.f, 0.f, 1.f},	//	yellow
                                                    {0.f, 1.f, 1.f, 1.f}	//	cyan
                                             };
char *kBckgndColStr[] =	{
							"Black",
							"Red",
							"Green" ,
							"Yellow",
							"Cyan"
						};


RasterImage *gImgIn = NULL,
            *gImgOut = NULL;
int			gImgWidth,
            gImgHeight;
int			gMainWindow,
            gSubwindow[kNbSubwindows];
bool		gInitDone = false;
GLfloat     **gCirclePt;
const int   gNbCirclePts = 12;
const GLfloat   gGridPtColor[4] = {0.f, 0.f, 1.f, 1.f},
                gActivePtColor[4] = {1.f, 0.f, 0.f, 1.f};
int         gSelectedPtRow = -1,
            gSelectedPtCol = -1;
            
ImageWarper_GL	*gWarper = NULL;
InnerWarpingMode gInnerMode = INNER_FREE;
BorderWarpingMode gBorderMode = BORDER_FIXED;
int     gGridRows = 5,
        gGridCols = 5;
float	***gGridPoint = NULL,
        ***gInitGridPoint = NULL;

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


void initImagesAndWarper(void)
{
    //-------------------------------------------------------------
    //	We read an image from a file
    //-------------------------------------------------------------
    const string filePath = color_dir + "bottles_256x256.tga";
//    const string filePath = color_dir + "mandrill_256x256.tga";
//    const string filePath = color_dir + "warpedMandrill_256x256.tga";
    ImageReader	myReader(const_cast<char *>(filePath.c_str()));
    RasterImage *tempImage = myReader.getFrame(true);
    if (tempImage ==NULL)
    {
        cout << "Error encountered while reading image file " << filePath << endl;
        cleanupAndQuit(-1);
    }
    const BaseRasterType imgType = tempImage->getBaseRasterType();
    if (imgType != kRGBa32Raster)
    {
        cout << "This demo only takes RGBA32 color images" << endl;
        cleanupAndQuit(-1);
    }
    
    //-------------------------------------------------------------
    //  Create the warper
    //-------------------------------------------------------------
    //  Initially the warper has fixed border nodes and free-moving
    //  inner nodes
    gInnerMode = INNER_FREE;
    gBorderMode = BORDER_FIXED;
    gWarper = new ImageWarper_GL(gInnerMode, gBorderMode);
    
    //  we find out which image dimensions are recommended by the warper (closest
    //  to the input image's dimensions)
    int imgHeight = tempImage->getHeight(),
        imgWidth = tempImage->getWidth();
    int loWidth, hiWidth, loHeight, hiHeight;
    gWarper->getImageSuitableDimensions(imgWidth, imgHeight, &loWidth, &loHeight, 
                                        &hiWidth, &hiHeight);

    //  for this demo I don't mind chopping a few pixels off the right and bottom
    //  edges of my image, so if the "low" values are at least 90% of the input
    //  I keep them
    if (10*loWidth >= 9*imgWidth)
        imgWidth = loWidth;
    else
        imgWidth = hiWidth;
    if (10*loHeight >= 9*imgHeight)
        imgHeight = loHeight;
    else
        imgHeight = loHeight;
    
	//  Now we can copy (part of) the temporary image into the input image	
	ImageRect myRect(0, 0, imgWidth, imgHeight),
	          *trueRect = ImageRect::intersection(&myRect, tempImage->getValidRect());
	gImgIn = new RasterImage_RGBa(imgHeight, imgWidth);
	gImgOut = new RasterImage_RGBa(imgHeight, imgWidth);
	gImgIn->setValidRect(&myRect);
	gImgOut->setValidRect(&myRect);
	tempImage->writeInto(gImgIn, trueRect);
	delete trueRect;
}


void setupWarper(void)
{	
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    gWarper->setImages(gImgIn, gImgOut);
    gWarper->setGrid(gGridRows, gGridCols);
    
    //  ask the warper for a grid of points to work with
    //  We only need gGridPoint to perform the warp, but we also get a copy to store
    //  the initial configuration, to use for our display.
    gGridPoint = gWarper->newWarpingGrid();
    gInitGridPoint = gWarper->newWarpingGrid();
}

void applyWarp(void)
{
//    glutSetWindow(gSubwindow[UPPER_LEFT]);
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
	gWarper->warp(gGridPoint);
}

void resetGrid(void) 
{
    for (int i=0; i<gGridRows; i++)
        for (int j=0; j<gGridCols; j++)
        {
            gGridPoint[i][j][0] = gInitGridPoint[i][j][0];
            gGridPoint[i][j][1] = gInitGridPoint[i][j][1];
        }

    applyWarp();
    myDisplayMain();
}

void deleteGrids(void)
{
    //  if we already previously set a grid, we need to delete it
    if (gGridPoint != NULL)
    {
        for (int i=0; i<gGridRows; i++)
        {
            for (int j=0; j<gGridCols; j++)
            {
                delete []gGridPoint[i][j];
                delete []gInitGridPoint[i][j];
            }
            delete []gGridPoint[i];
            delete []gInitGridPoint[i];
        }
        delete []gGridPoint;    
        delete []gInitGridPoint;    
    }
    
    gGridPoint = gInitGridPoint = NULL;
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
}


void myDisplay0(void)
{
    glutSetWindow(gSubwindow[UPPER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
    {
        gImgIn->render();

        //  then overlay the warping grid
        glColor4f(0.f, 0.f, 0.f, 1.f);
        for (int i=0; i<gGridRows; i++)
        {
            glBegin(GL_LINE_STRIP);
                for (int j=0; j<gGridCols; j++)
                    glVertex2f(gInitGridPoint[i][j][0], gImgHeight - gInitGridPoint[i][j][1]);
            glEnd();
         }
        for (int j=0; j<gGridCols; j++)
        {
            glBegin(GL_LINE_STRIP);
                for (int i=0; i<gGridRows; i++)
                    glVertex2f(gInitGridPoint[i][j][0], gImgHeight - gInitGridPoint[i][j][1]);
            glEnd();
        }
        
        //  and the grid points as circles
        for (int i=0; i<gGridRows; i++) 
        {
            for (int j=0; j<gGridCols; j++)
            {
                glPushMatrix();
                glTranslatef(gInitGridPoint[i][j][0], gImgHeight - gInitGridPoint[i][j][1], 0.f);
                if ((i!=gSelectedPtRow) || (j!=gSelectedPtCol))
                    glColor4f(gGridPtColor[0], gGridPtColor[1], gGridPtColor[2], gGridPtColor[3]);
                else
                    glColor4f(gActivePtColor[0], gActivePtColor[1], gActivePtColor[2], gActivePtColor[3]);
                
                glScalef(4.f, 4.f, 1.f);
                glBegin(GL_POLYGON);
                    for (int k=0; k<gNbCirclePts; k++)
                        glVertex2fv(gCirclePt[k]);
                glEnd();
                glPopMatrix();
            }
        }   
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
        //  render the warped image
        gImgOut->render();
        
        //  then overlay the warping grid
        glColor4f(0.f, 0.f, 0.f, 1.f);
        for (int i=0; i<gGridRows; i++)
        {
            glBegin(GL_LINE_STRIP);
                for (int j=0; j<gGridCols; j++)
                    glVertex2f(gGridPoint[i][j][0], gImgHeight - gGridPoint[i][j][1]);
            glEnd();
         }
        for (int j=0; j<gGridCols; j++)
        {
            glBegin(GL_LINE_STRIP);
                for (int i=0; i<gGridRows; i++)
                    glVertex2f(gGridPoint[i][j][0], gImgHeight - gGridPoint[i][j][1]);
            glEnd();
        }
        //  and the grid points as circles
        for (int i=0; i<gGridRows; i++) 
        {
            for (int j=0; j<gGridCols; j++)
            {
                glPushMatrix();
                glTranslatef(gGridPoint[i][j][0], gImgHeight - gGridPoint[i][j][1], 0.f);
                if ((i!=gSelectedPtRow) || (j!=gSelectedPtCol))
                    glColor4f(gGridPtColor[0], gGridPtColor[1], gGridPtColor[2], gGridPtColor[3]);
                else
                    glColor4f(gActivePtColor[0], gActivePtColor[1], gActivePtColor[2], gActivePtColor[3]);
                
                glScalef(4.f, 4.f, 1.f);
                glBegin(GL_POLYGON);
                    for (int k=0; k<gNbCirclePts; k++)
                        glVertex2fv(gCirclePt[k]);
                glEnd();
                glPopMatrix();
            }
        } 
        
//        glColor4f(1.f, 1.f, 1.f, 1.f);  
    }
            
    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

//   Precalculates the coordinates of points on a circle, to later display
//  grid points
void precalcCircle(void)
{
    gCirclePt = new GLfloat*[gNbCirclePts];
    for (int k=0; k<gNbCirclePts; k++) 
    {
        gCirclePt[k] = new GLfloat[2];
        float theta = 2*PI_F*k/gNbCirclePts;
        gCirclePt[k][0] = cosf(theta);
        gCirclePt[k][1] = sinf(theta);
    }

}

#if 0
#pragma mark -
#endif

void myReshape(int w, int h)
{
    if (gInitDone)
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


//  we only recourd mouse clicks in the right subwindow
void myMouse(int button, int buttonState, int ix ,int iy)
{
    if ((button == GLUT_LEFT_BUTTON) && (buttonState == GLUT_UP))
    {
        //  if no point was previously selected, this mouse click is used to
        //  select a new point
        if ((gSelectedPtRow == -1) && (gSelectedPtCol == -1))
        {
            //  did the user click on one of the active points?
            bool  pointClicked = false;
            gSelectedPtRow = gSelectedPtCol = -1;
            for (int i=1; (i<gGridRows-1) && !pointClicked; i++)
            {
                for (int j=1; (j<gGridCols-1) && !pointClicked; j++)
                {
                    float   dx = ix - gGridPoint[i][j][0],
                            dy = iy - gGridPoint[i][j][1],
                            dist2 = dx*dx + dy*dy;
                    if (dist2 < 25)
                    {
                        pointClicked = true;
                        gSelectedPtRow = i;
                        gSelectedPtCol = j;
                    }
                }
            }
        }
        //  otherwise, the location of the mouse click gives the new coordinates of
        //  the grid point
        else
        {
            float   tempX = gGridPoint[gSelectedPtRow][gSelectedPtCol][0],
                    tempY = gGridPoint[gSelectedPtRow][gSelectedPtCol][1];
            gGridPoint[gSelectedPtRow][gSelectedPtCol][0] = ix;
            gGridPoint[gSelectedPtRow][gSelectedPtCol][1] = iy;
            
            //  if this is a valid displacement, proceed
            if (gWarper->isValidDisplacement(gGridPoint))
            {
                //  Note that the warper performs again the above validity test.  We only do it
                //  it so that we can restore the grid to a correct state if the displacement
                //  was not valid
                applyWarp();
            }
            //  otherwise restore grid point values and ignore
            else
            {
                gGridPoint[gSelectedPtRow][gSelectedPtCol][0] = tempX;
                gGridPoint[gSelectedPtRow][gSelectedPtCol][1] = tempY;
            }
            
            //  in any case, deselect the point
            gSelectedPtRow = gSelectedPtCol = -1;
        }
    }
     
    myDisplayMain();
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

        case RESET_MENU:
            resetGrid();
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

void myGridSizeMenu(int value)
{
    switch (value)
    {
        case FOUR_BY_FOUR_GRID:
            if ((gGridRows != 4) || (gGridCols != 4))
                deleteGrids();
            gGridRows = 4;
            gGridCols = 4;
            setupWarper();
            break;
    
        case FOUR_BY_SIX_GRID:
            if ((gGridRows != 4) || (gGridCols != 6))
                deleteGrids();
            gGridRows = 4;
            gGridCols = 6;
            setupWarper();
            break;
    
        case FIVE_BY_FIVE_GRID:
            if ((gGridRows != 5) || (gGridCols != 5))
                deleteGrids();
            gGridRows = 5;
            gGridCols = 5;
            setupWarper();
            break;
    
        case SIX_BY_FOUR_GRID:
            if ((gGridRows != 6) || (gGridCols != 4))
                deleteGrids();
            gGridRows = 6;
            gGridCols = 4;
            setupWarper();
            break;
    
        case SIX_BY_SIX_GRID:
            if ((gGridRows != 6) || (gGridCols != 6))
                deleteGrids();
            gGridRows = 6;
            gGridCols = 6;
            setupWarper();
            break;
    
        default:
            break;
    }
}

void myInit(void)
{
    // Create Menus
    int menuMain, menu[kNbSubwindows], colorSubmenu, 
        gridSizeMenu /*, innerGridMoveMenu, borderGridMoveMenu*/;

    //	Set a general menu for the main window
    glutSetWindow(gMainWindow);
    glClearColor(0.f, 0.f, 0.f, 1.f);

    //	Submenu for background color selection choices
    colorSubmenu = glutCreateMenu(myColorSubmenu);
    for (int i=0; i<kNbBckgndCols; i++)
        glutAddMenuEntry(kBckgndColStr[i], i);

    // Main menu that the submenus are connected to
    glutSetWindow(gMainWindow);
    menuMain = glutCreateMenu(myMenuMain);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //---------------------------------------
    //  Left subwindow
    //---------------------------------------
    glutSetWindow(gSubwindow[UPPER_LEFT]);
    gridSizeMenu = glutCreateMenu(myGridSizeMenu);
    glutAddMenuEntry("4x4", FOUR_BY_FOUR_GRID);
    glutAddMenuEntry("4x6", FOUR_BY_SIX_GRID);
    glutAddMenuEntry("5x5", FOUR_BY_FOUR_GRID);
    glutAddMenuEntry("6x4", SIX_BY_FOUR_GRID);
    glutAddMenuEntry("6x6", SIX_BY_SIX_GRID);
    //
    menu[UPPER_LEFT] = glutCreateMenu(myMenu0);
    glutAddMenuEntry("Reset Images", RESET_MENU);
    glutAddSubMenu("Grid dimensions", gridSizeMenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //---------------------------------------
    //  Right subwindow
    //---------------------------------------
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    glutMouseFunc(myMouse);
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddMenuEntry("-", SEPARATOR);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gMainWindow);

    gInitDone = true;
}


int main(int argc, char** argv)
{
    GLfloat	*backColor;
    void	(*displayFuncTable[kNbSubwindows])() = {myDisplay0, myDisplay1};

    try {
        glutInit(&argc, argv);

        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

        initializeQTEnvironment();

        //  It's a bit of a hack.  Right now I use the window's back buffer for my warper so I need
        //  to have a window already created when I activate the warper.  I resize the window
        //  later when I know the image's dimensions
        glutInitWindowSize(kDefaultWidth, kDefaultHeight);
        gMainWindow = glutCreateWindow("Background Remover Demo");

        //	I load my image and initialize my warper
        initImagesAndWarper();

        gImgHeight = gImgIn->getHeight();
        gImgWidth = (gImgIn->getBytesPerRow())/4;

        //	I inform all drawable objects of the dimensions of their subwindow
        DrawableObject2D::setDefaultWindowDimensions(gImgWidth, gImgHeight);

        //	I initialize my window's dimensions
        int winHeight = kNbRowsSubwindows * (gImgHeight + kBorder) + kBorder;
        int winWidth = kNbColsSubwindows * (gImgWidth + kBorder) + kBorder;
        glutSetWindow(gMainWindow);
        glutReshapeWindow(winWidth, winHeight);

        glutKeyboardFunc(myKeyboard);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);

        glClearColor(0.f, 0.f, 0.f, 1.f);

        int	winID = 0;
        glutSetWindow(gMainWindow);
        for (int i=0; i<kNbRowsSubwindows; i++)
            for (int j=0; j<kNbColsSubwindows; j++)
            {
                gSubwindow[winID] = glutCreateSubWindow(gMainWindow,
                                                        j*(gImgWidth + kBorder) + kBorder,
                                                        i*(gImgHeight + kBorder) + kBorder,
                                                        gImgWidth, gImgHeight);

                //	I don't need to do this.  I just set a different "clear" color for
                //	each subwindow.
                backColor = kBackgroundColor[winID];
                glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

                //	This *has* to be defined:  a "display" callback function for each subwindow
                glutDisplayFunc(displayFuncTable[winID++]);

                glutKeyboardFunc(myKeyboard);

                //	This calls maps "world" coordinates to "screen" coordinates.
                //	In this particular case I make them coincide.
                setupCamera(gImgWidth, gImgHeight);
                //
                glutSetWindow(gMainWindow);
            }

        myInit();
        precalcCircle();

        //  set up the warper
        setupWarper();
        
        //  apply a first warping into the output image
        applyWarp();
        myDisplayMain();

        glutMainLoop();

        cleanupAndQuit(0);
        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
