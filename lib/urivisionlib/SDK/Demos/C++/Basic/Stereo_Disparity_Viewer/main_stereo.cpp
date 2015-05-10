/*  NAME:
        main_stereo.cpp

    DESCRIPTION:
        demonstration application for basic depth map classes

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|	The upper-left subwindow displays the left image.  The upper-right      |
|   subwindow displayes the right image or the stereo disparity or depth as |
|   a gray level image (I just picked arbitrary values for the focal length |
|   and depth).  The bottom subwindow displays depth as a user-controlable  |
|   3D mesh.                                                                |
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|
+--------------------------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
//
#include "ImageReader.h"
#include "RasterImage_gray.h"
#include "DepthMap.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
//
const string stereo_dir = STEREO_DIR_PATH;

const string leftImagePath = stereo_dir + "pentagon_L.tga";
const string righttImagePath = stereo_dir + "pentagon_R.tga";
const string disparityImagePath = stereo_dir + "pentagon_d.tga";



//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	2
#define	kNbColsSubwindows	2
#define	kNbSubwindows		kNbRowsSubwindows*kNbColsSubwindows
#define	kNbImages			3
//
#define	kBorder				8
#define	kDefaultHeight		120//256
#define	kDefaultWidth		120//256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640


#ifndef	PI
#define	PI	3.14159265
#endif
//
#define RAD_TO_DEG		57.29577951308232	//	180 / PI
#define DEG_TO_RAD		 0.017453292519943	//	PI / 180

//----------------------------------------------
//	I define my perspective camera
//----------------------------------------------
//  Usually I define my camera using information such as the chip
//  dimensions, so that my virtual camera closely resembles a real
//  one.  Here, the camera is a pinhole w/o any CCD information, so
//  I must use the window's dimensions and a reset fov angle
//  to define the camera
const float FOV_Y = 40;

//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
//
void loadImages(void);
void computeDepth(void);
//
void myDisplayMain(void);
void myDisplay0(void);
void myDisplay1(void);
void myDisplayDepth(void);
void setMaterial(GLfloat ambRed, GLfloat ambGreen, GLfloat ambBlue, GLfloat difRed,
                 GLfloat difGreen, GLfloat difBlue, GLfloat specRed, GLfloat specGreen,
                 GLfloat specBlue, GLfloat shine);
//
void myReshape(int w, int h);
void setupOrthoCam(int w, int h);
void setupProjCam(int w, int h);
//
void myMenu0(int value);
void myMenu1(int value);
void myDepthMenu(int value);
//
void myColorSubmenu(int value);
void myUpperRightSubmenu(int value);
void myRenderSubmenu(int value);
void myMouse(int button, int state, int ix ,int iy);
void myDepthMouse(int button, int state, int ix ,int iy);
void myDepthMotion(int ix ,int iy);
void myKeyboard(unsigned char c, int x, int y);
int myInit(void);
void myIdle(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int			gWinWidth,
            gWinHeight,
            gImageWidth = kDefaultWidth,
            gImageHeight = kDefaultHeight,
            gDepthWinWidth,
            gDepthWinHeight;
            
ResolutionLevel gMeshResolution = kMediumResolution;

const float	gNearZ = 1,			        //	Position of the clipping planes along the camera's
            gFarZ = 120.0;		        //		optical axis (Z axis)

const float MAX_DEPTH = 80.f;

int			gMainWindow,
            gSubwindow[2],
            gDepthWindow;

int			gStartx,
            gStarty;

bool		gInitDone = false,
            gDisplayDisparity = false,
            gFilledFaceNormals = true,
            gFilledVertexNormals = false,
            gMoving = false;

float		gCurrentQuat[4],
            gLastQuat[4];

GLfloat		gTx = 0.f,
            gTy = 0.f,
            gTz = -70.0f,	//	the depth map's origin in camera's reference frame
            gAngle = 20,   /* in degrees */
            gAngle2 = 30;   /* in degrees */

RasterImage		    *gMyLeftImage = NULL;
RasterImage		    *gMyRightImage = NULL;
RasterImage_gray    *gMyDisparity = NULL;
RasterImage_gray_F  *gMyDepth = NULL;
RasterImage		    *gUpperRightDisplay = NULL;	//	image displayed in upper-right window
DepthMap		    *gMyDenseMap = NULL;

const int	UPPER_LEFT = 0,
            UPPER_RIGHT = 1;

const int   QUIT_MENU = 0;
//
const int   WIREFRAME_MENU = 100,
            FILLED_FACES_MENU = 101,
            FILLED_VERTEX_MENU = 102;
//
const int   COARSER_RESOLUTION_MENU = 104,
            FINER_RESOLUTION_MENU = 105,
            RED_COLOR_MENU = 106,
            GREEN_COLOR_MENU = 107,
            YELLOW_COLOR_MENU = 108;
//
const int   RIGHT_IMAGE_MENU = 200,
            DISPARITY_MAP_MENU = 201,
            DEPTH_MAP_MENU = 202;

//
//	For menu handling demo
//
#define	kNbBckgndCols		6
//
GLfloat	kBackgroundColor[kNbBckgndCols][4] =	{	{0.f, 0.f, 0.f, 1.f},	//	black
                                                    {1.f, 0.f, 0.f, 1.f},	//	red
                                                    {.2f, .2f, .3f, 1.f},	//	gray
                                                    {0.f, 1.f, 0.f, 1.f},	//	green
                                                    {1.f, 1.f, 0.f, 1.f},	//	yellow
                                                    {0.f, 1.f, 1.f, 1.f}	//	cyan
                                                                                         };
string kBckgndColStr[] =	{
                                "Black",
                                "Red",
                                "Gray",
                                "Green" ,
                                "Yellow",
                                "Cyan"
                            };


//=============================================================================
//	QT stuff
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


//=============================================================================
//	Computation Functions
//=============================================================================

void loadImages(void)
{
    ImageReader		myReader;

    //-------------------------------------------------------------
    //	We read the left image, right image, and disparity map
    //-------------------------------------------------------------
    myReader.setSource(const_cast<char*>(leftImagePath.c_str()));
    gMyLeftImage = myReader.getFrame(false);	//	left image

    myReader.setSource(const_cast<char*>(righttImagePath.c_str()));
    gMyRightImage = myReader.getFrame(false);	//	right image

    //	Normally, I should be able to read the disparity map directly into my
    //	DepthMap's raster.  Then I could render the map as a gray-level image
    //	in one subwindow and draw it as a mesh in an other.  However, at this
    //	point, the "readInto" component of the ImageReader class has not been
    //	implemented, so I had to separate the two operations.
    myReader.setSource(const_cast<char*>(disparityImagePath.c_str()));
    gMyDisparity = static_cast<RasterImage_gray*>(myReader.getFrame(false));	//	disparity map

    //	initially we display the right image in the upper-right subwindow
    gUpperRightDisplay = gMyRightImage;
}

//---------------------------------------------------------------------------------
//	Create two range (depth) maps from the disparity map stored in gMyDisparity:
//		- a dense depth map computed at each image point
//		- a sparse depth map computed at a number of random points.
//---------------------------------------------------------------------------------
void computeDepth(void)
{
    //------------------
    //	Step 1: create the IntrinsicParameters object
    //------------------
    //	I know next to nothing about the cameras that took these images, so
    //	I made up all the data.  I create an ideal pinhole camera
    int	nbRows = gMyDisparity->getHeight(),
                   nbCols = gMyDisparity->getWidth();
    IntrinsicParameters_pinhole	*camParams;
    //  Since this is a "pinhole" camera we are dealing with here, not a real one, I need
    //  to compute my focal length in "pixel" units
    float   focPix = 0.5f * gMyLeftImage->getHeight() / tan(DEG_TO_RAD*FOV_Y/2);
    camParams = new IntrinsicParameters_pinhole(nbCols, nbRows, focPix, focPix);

    //------------------
    //	Step 2: compute depth at each pixel
    //------------------
    //	I don't know the geometry of the stereo pair either, only that they are supposed
    //	to form a parallel pair.  So I make up a baseline.
    //float	baseline = 1.f;

    //  Here we break the process into a series of baby steps.  I create first a
    //  "depth" raster image that will store the depth computed from the disparity
    //  Note that my calculation of "depth" uses completely made up coefficients
    gMyDepth = new RasterImage_gray_F(nbRows, nbCols);
    ImageRect *validRect = new ImageRect(0, 0, nbCols, nbRows);
    float *const* depth = gMyDepth->getShiftedGrayF2D(R_W_ACCESS);
    const unsigned char	*const* disp = gMyDisparity->getShiftedGray2D();
    for (int i=0; i<nbRows; i++)
        for (int j=0; j<nbCols; j++)
        {
            depth[i][j] = 75.f * (1.f + 1.f / (2.f + disp[i][j]));
            if (depth[i][j] > MAX_DEPTH)
                depth[i][j] = MAX_DEPTH;
        }
    gMyDepth->setValidRect(validRect);
    gMyDepth->setFloatConversionMode(kRangeFloat);


    //------------------
    //	Step 3: create the dense DepthMap object
    //------------------
    gMyDenseMap = new DepthMap(gMyDepth, camParams, 1.f);
    gMyDenseMap->setFloatConversionMode(kRangeFloat);
    gMyDenseMap->setResolution(gMeshResolution);
    gMyDenseMap->setWireframe(true);
    gMyDenseMap->setMaterial(0.8f, 0.1f, 0.1f, .5f, .5f, .5f, 0.3f, .3f, .3f, .5f);
}

#if 0
#pragma mark -
#endif
//=============================================================================
//	Display functions for the different subwindows
//=============================================================================

void myDisplayMain(void)
{
    glutSetWindow(gMainWindow);

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);

    myDisplay0();
    myDisplay1();
    myDisplayDepth();

    glutSwapBuffers();
}


void myDisplay0(void)
{
    glutSetWindow(gSubwindow[UPPER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gInitDone)
    {
        gMyLeftImage->render();
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
        gUpperRightDisplay->render();

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplayDepth(void)
{
    glutSetWindow(gDepthWindow);

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (gMyDenseMap != NULL)
    {
        glPushMatrix();
        glTranslatef(gTx, gTy, gTz);

        glRotatef(gAngle2, 1.0, 0.0, 0.0);
        glRotatef(gAngle, 0.0, 1.0, 0.0);

		glScalef(1.f, 1.f, 2.f);
        gMyDenseMap->draw();
        glPopMatrix();

    }

    glutSwapBuffers();
    glutSetWindow(gMainWindow);
}


#if 0
#pragma mark -
#endif
//=============================================================================
//	Event Handling functions
//=============================================================================

void myIdle(void)
{
    glutPostRedisplay();
}

void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 'q':
            cleanupAndQuit(0);
            break;

            //	, and . (below < and >) change the mesh's resolution
        case '.':
            switch (gMeshResolution)
            {
                case kVeryHighResolution:
                case kFineResolution:
                    gMeshResolution = kVeryHighResolution;
                    break;
                case kMediumResolution:
                    gMeshResolution = kFineResolution;
                    break;
                case kCoarseResolution:
                    gMeshResolution = kMediumResolution;
                    break;
                case kVeryLowResolution:
                    gMeshResolution = kCoarseResolution;
                    break;
            }
            gMyDenseMap->setResolution(gMeshResolution);
            break;

        case ',':
            switch (gMeshResolution)
            {
                case kVeryHighResolution:
                    gMeshResolution = kFineResolution;
                    break;
                case kFineResolution:
                    gMeshResolution = kMediumResolution;
                    break;
                case kMediumResolution:
                    gMeshResolution = kCoarseResolution;
                    break;
                case kCoarseResolution:
                case kVeryLowResolution:
                    gMeshResolution = kVeryLowResolution;
                    break;
            }
            gMyDenseMap->setResolution(gMeshResolution);
            break;

        default:
            break;

    }

    glutPostRedisplay();
}


void myMouse(int button, int state, int ix ,int iy)
{}

//	Mouse event handling function for the depth map subwindow.

void myDepthMouse(int button, int state, int ix ,int iy)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        gMoving = true;
        gStartx = ix;
        gStarty = iy;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        gMoving = false;
}

//	"Active" mouse motion event handling function for subwindow 3.
void myDepthMotion(int ix ,int iy)
{
    if (gMoving)
    {
        gAngle += (ix - gStartx);
        gAngle2 += (iy - gStarty);
        gStartx = ix;
        gStarty = iy;
        glutPostRedisplay();
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

void myDepthMenu(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;

        case FINER_RESOLUTION_MENU:
            switch (gMeshResolution)
            {
                case kVeryHighResolution:
                case kFineResolution:
                    gMeshResolution = kVeryHighResolution;
                    break;
                case kMediumResolution:
                    gMeshResolution = kFineResolution;
                    break;
                case kCoarseResolution:
                    gMeshResolution = kMediumResolution;
                    break;
                case kVeryLowResolution:
                    gMeshResolution = kCoarseResolution;
                    break;
            }
            gMyDenseMap->setResolution(gMeshResolution);
            break;

        case COARSER_RESOLUTION_MENU:
            switch (gMeshResolution)
            {
                case kVeryHighResolution:
                    gMeshResolution = kFineResolution;
                    break;
                case kFineResolution:
                    gMeshResolution = kMediumResolution;
                    break;
                case kMediumResolution:
                    gMeshResolution = kCoarseResolution;
                    break;
                case kCoarseResolution:
                case kVeryLowResolution:
                    gMeshResolution = kVeryLowResolution;
                    break;
            }
            gMyDenseMap->setResolution(gMeshResolution);
            break;

        case RED_COLOR_MENU:
            gMyDenseMap->setMaterial(0.8f, 0.1f, 0.1f, .5f, .2f, .2f, 0.3f, 0.f, 0.f, .5f);
            break;

        case GREEN_COLOR_MENU:
            gMyDenseMap->setMaterial(0.1f, 0.7f, 0.1f, .2f, .5f, .2f, 0.f, .3f, 0.f, .5f);
            break;

        case YELLOW_COLOR_MENU:
            gMyDenseMap->setMaterial(0.8f, 0.8f, 0.1f, .5f, .5f, .2f, .3f, .3f, 0.f, .5f);
            break;

        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
}
void myRenderSubmenu(int value)
{
    switch (value)
    {
        case WIREFRAME_MENU:
            if (gMyDenseMap!=NULL)
                gMyDenseMap->setWireframe(true);
            break;

        case FILLED_FACES_MENU:
        case FILLED_VERTEX_MENU:
			gFilledFaceNormals = !gFilledFaceNormals;
            if (gMyDenseMap!=NULL)
                gMyDenseMap->setWireframe(false);

/*
			gFilledVertexNormals = !gFilledVertexNormals;
			if (gCurrentDepthMap!=NULL)
				gCurrentDepthMap->setVertexNormals(gFilledVertexNormals);
*/
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




void myUpperRightSubmenu(int urDisplayIndex)
{
    switch (urDisplayIndex)
    {
        case RIGHT_IMAGE_MENU:
            gUpperRightDisplay = gMyRightImage;
            break;

        case DISPARITY_MAP_MENU:
            gUpperRightDisplay = gMyDisparity;
            break;

        case DEPTH_MAP_MENU:
            gUpperRightDisplay = gMyDenseMap;
            break;

        default:
            break;

    }

    glutPostRedisplay();
}

#if 0
#pragma mark -
#endif
//=============================================================================
//	Various Settup functions
//=============================================================================


void myReshape(int w, int h)
{}

//	This function maps world coordinates to screen coordinates.  This is where
//	the projection mode would be defined.  Here I only want to have a 2D
//	rectangle where to draw 2D stuff on top of my image, so I define my camera
//	as an orthographic camera, and I set the world coordinates to coincide with
//	the pixel coordinates (this is what the gluOrtho2D call does).

void setupOrthoCam(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(	0.0f, (GLfloat) w, 0.0f, (GLfloat) h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
}

//	This function maps world coordinates to screen coordinates.  This is where
//	the projection mode would be defined.  Here I only want to have a 2D
//	rectangle where to draw 2D stuff on top of my image, so I define my camera
//	as an orthographic camera, and I set the world coordinates to coincide with
//	the pixel coordinates (this is what the gluOrtho2D call does).
void setupProjCam(int w, int h)
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    float   fov_xy = (1.0f * gDepthWinWidth) / gDepthWinHeight;

    gluPerspective(FOV_Y, fov_xy, gNearZ, gFarZ);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glViewport(0, 0, w, h);
}


int myInit(void)
{
    initializeQTEnvironment();

    // Create Menus
    int menu[3], colorSubmenu, renderSubmenu,
    upperRightSubmenu;

    //	Set a general menu for the main window
    glutSetWindow(gMainWindow);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    //	Submenu for background color selection choices
    colorSubmenu = glutCreateMenu(myColorSubmenu);
    for (int i=0; i<kNbBckgndCols; i++)
        glutAddMenuEntry(kBckgndColStr[i].c_str(), i);


    glutSetWindow(gSubwindow[UPPER_LEFT]);
    menu[0] = glutCreateMenu(myMenu0);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    upperRightSubmenu = glutCreateMenu(myUpperRightSubmenu);
    glutAddMenuEntry("Right Image", RIGHT_IMAGE_MENU);
    glutAddMenuEntry("Disparity Map", DISPARITY_MAP_MENU);
    glutAddMenuEntry("Depth Map", DEPTH_MAP_MENU);
    menu[1] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Display Selection", upperRightSubmenu);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gDepthWindow);
    renderSubmenu = glutCreateMenu(myRenderSubmenu);
    glutAddMenuEntry("Wireframe", WIREFRAME_MENU);
    glutAddMenuEntry("Filled, face normals", FILLED_FACES_MENU);
    glutAddMenuEntry("Filled, vertex normals", FILLED_VERTEX_MENU);
    menu[2] = glutCreateMenu(myDepthMenu);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAddSubMenu("Rendering", renderSubmenu);
    glutAddMenuEntry("Coarser resolution", COARSER_RESOLUTION_MENU);
    glutAddMenuEntry("Finer resolution", FINER_RESOLUTION_MENU);
    glutAddMenuEntry("Color: red", RED_COLOR_MENU);
    glutAddMenuEntry("Color: green", GREEN_COLOR_MENU);
    glutAddMenuEntry("Color: yellow", YELLOW_COLOR_MENU);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gMainWindow);

    computeDepth();
    gInitDone = true;

    return false;
}


int main(int argc, char** argv)
{
    try {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    	
    	initializeQTEnvironment();
        
    	//	I load my images
        loadImages();

        int	imgWidth = gMyLeftImage->getHeight(),
            imgHeight = gMyLeftImage->getWidth();
        //  and I inform drawable object of these dimensions (in case they want to get rendered
        //  "shifted")
        DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

        //	I initialize my window's dimensions
        gWinHeight = 5 * imgHeight / 2 + 3 * kBorder;
        gWinWidth = 2 * imgWidth + 3 * kBorder;

        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Depth Map Demo");

        glutKeyboardFunc(myKeyboard);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);
        glutIdleFunc(myIdle);
        glutMouseFunc(myMouse);

        glClearColor(0.0, 0.0, 0.0, 1.0);

        glutSetWindow(gMainWindow);

        GLfloat	*backColor;
        //--------------
        //	Upper-left subwindow will display the left image
        //--------------
        gSubwindow[UPPER_LEFT] = glutCreateSubWindow(gMainWindow, kBorder, kBorder,
                                            imgWidth, imgHeight);
        //	I don't need to do this.  I just set a different "clear" color for
        //	each subwindow.
        backColor = kBackgroundColor[0];
        glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);
        //	This *has* to be defined:  a "display" callback function for each subwindow
        glutDisplayFunc(myDisplay0);
        glutKeyboardFunc(myKeyboard);
        setupOrthoCam(imgWidth, imgHeight);
        glutSetWindow(gMainWindow);


        //--------------
        //	Upper-right subwindow will display the right image or the disparity map
        //--------------
        gSubwindow[UPPER_RIGHT] = glutCreateSubWindow(gMainWindow, imgWidth + 2*kBorder, kBorder,
                                            imgWidth, imgHeight);
        //	I don't need to do this.  I just set a different "clear" color for
        //	each subwindow.
        backColor = kBackgroundColor[1];
        glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);
        //	This *has* to be defined:  a "display" callback function for each subwindow
        glutDisplayFunc(myDisplay1);
        glutKeyboardFunc(myKeyboard);
        setupOrthoCam(imgWidth, imgHeight);
        glutSetWindow(gMainWindow);


        //--------------
        //	The depth subwindow will diplay the depth map, interactively.
        //--------------
        gDepthWinWidth = 2*imgWidth + kBorder;
        gDepthWinHeight = 3*imgHeight/2;
        gDepthWindow = glutCreateSubWindow(gMainWindow, kBorder, imgHeight + 2*kBorder,
                                           gDepthWinWidth, gDepthWinHeight);
        //	I don't need to do this.  I just set a different "clear" color for
        //	each subwindow.
        backColor = kBackgroundColor[2];
        glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);
        //	This *has* to be defined:  a "display" callback function for each subwindow
        glutDisplayFunc(myDisplayDepth);
        //	handle mouse events and "active" mouse motion events in this subwindow.
        glutMouseFunc(myDepthMouse);
        glutMotionFunc(myDepthMotion);
        glutKeyboardFunc(myKeyboard);
        //
        setupProjCam(gDepthWinWidth, gDepthWinHeight);
        glutSetWindow(gMainWindow);

        myInit();

        glutMainLoop();

        cleanupAndQuit(0);

        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
