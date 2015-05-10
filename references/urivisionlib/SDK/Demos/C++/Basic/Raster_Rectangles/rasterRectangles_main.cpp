/*  NAME:
        raster rectangles.cpp
 
    DESCRIPTION:
        demonstration application for the bounding and valid data rectangles
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

/*--------------------------------------------------------------------------+
|	This small demo program illustrates the difference between the bounding |
|   rectangle and the valid data rectangle, the latter being always a       |
|   subpart (maybe totality) of the former.                                 |
|   The demo shows how the shift of the bounding and valid data rectangles  |
|   is affected by a scaling operation.                                     |
|   The '.' and ',' keys (located at '>' and '<') scale down and up the     |
|   rendering of the objects.                                               |
|   If the user hits the space bar, then the display is switched to the     |
|   "absolute" mode, with the valid data rectangle set at a location set by |
|   the user.  That location is initially (0, 0) and can be modified by     |
|   using the arrow keys.  When in that mode, I assigned the same origin to |
|   raster image and the vector field (not because it has to be so, but     |
|   because I did not ferel it was worth the extra effort to assign them    |
|   different origins)                                                      |
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
#define	kNbModes				5


//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
void CreateImageAndField(void);
void myDisplay(void);
void myReshape(int w, int h);
void myMenu(int value);
void myColorSubmenu(int value);
void myKeyboard(unsigned char c, int x, int y);
void mySpecialKeys(int c, int x, int y);
void myInit(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
int 			gCurrentImg = 0;	//	index of image used for display
//
RasterImage		*gMyImage = NULL;
VectorField_F	*gMyField = NULL;
ImagePoint      gOrigin(0, 0);

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


bool	gInitDone = false;
bool    gUseOrigin = false;

int 	gScaleIndex = 10;	//	10 --> scale of 1.0


const float S_N_P_NOISE_RANGE = 255.f;
const float S_N_P_NOISE_SCALE = S_N_P_NOISE_RANGE/(RAND_MAX + 1.f);

const int   OBJECT_DISPLCT = 1;


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

#if 0
#pragma mark -
#endif

void CreateImageAndField(void)
{
    //-------------------------------------------------------------
    //	the image is created in a rectangle [100, 399]x[50, 249]
    //-------------------------------------------------------------
    int		nbRows = 200,
            nbCols = 300;
    ImageRect	myRect(100, 50, nbCols, nbRows);

    gMyImage = new RasterImage_RGBa(&myRect);
    //  I want the image's bounding rectangle and valid data rectangle to be displayed.
    gMyImage->setDrawFrame(true);
    gMyImage->setFrameColor(0.f, 0.f, 1.f);
    gMyImage->setDrawValidDataFrame(true);

    //	We are not going to initialize the full image (just to show how this is
    //	done and how to set properly the valid data rectangle.  Of course, it is
    //	possible to initialize the full image, as we did in Simple IO).
    int	padRow1 = 10,
        padRow2 = 30,
        padCol1 = 50,
        padCol2 = 130;

    //	We initialize the image using the "shifted" base raster
    unsigned char *const*rgba = gMyImage->getShiftedRaster2D(R_W_ACCESS);

    //  First I fill the entire raster with bogus data
    int	iLow  = myRect.getTop(),
		iHigh  = myRect.getBottom(),
		jLow  = myRect.getLeft(),
		jHigh  = myRect.getRight();

    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            rgba[i][4*j] = static_cast<unsigned char>(S_N_P_NOISE_SCALE * rand() / 4);
            rgba[i][4*j+1] = static_cast<unsigned char>(S_N_P_NOISE_SCALE * rand() / 2);
            rgba[i][4*j+2] = static_cast<unsigned char>(S_N_P_NOISE_SCALE * rand() / 2);
            //
            rgba[i][4*j+3] = static_cast<unsigned char>(255);
        }        
    }
    
    //	Only part of the raster contains valid data
    iLow  += padRow1;
    iHigh -= padRow2;
    jLow  += padCol1;
    jHigh -= padCol2;

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            rgba[i][4*j] = static_cast<unsigned char>((i-iLow-nbCols/2.)*(i-iLow-nbCols/2.)/120. + 20);
            rgba[i][4*j+1] = static_cast<unsigned char>((j-jLow)/4 + 30);
            rgba[i][4*j+2] = static_cast<unsigned char>(127*(cos(0.06*(i-iLow))*sin(0.1*(j-jLow))+0.5));
            rgba[i][4*j+3] = static_cast<unsigned char>(255);
        }

    //	we set the valid data rectangle to reflect the values that were put into
    //	the image.
    gMyImage->setValidRect(jLow, iLow, jHigh-jLow+1, iHigh-iLow+1);



    //-------------------------------------------------------------
    //	the vector field is created in a rectangle [150, 449]x[100, 349]
    //-------------------------------------------------------------
    nbRows = 250;
    nbCols = 400;
    //
    padRow1 = 10;
    padRow2 = 30;
    padCol1 = 30;
    padCol2 = 70;
    //
    myRect.setRect(120, 100, nbCols, nbRows);
    gMyField = new VectorField_F(&myRect);
    float	**rx = static_cast<float **>(gMyField->getShiftedRasterX2D(R_W_ACCESS));
    float	**ry = static_cast<float **>(gMyField->getShiftedRasterY2D(R_W_ACCESS));

    //  only part of the vector field contains valid data
    //	Only part of the raster contains valid data
    iLow  = myRect.getTop()+3*padRow1;
    iHigh  = myRect.getBottom()-2*padRow2;
    jLow  = myRect.getLeft()+2*padCol1;
    jHigh  = myRect.getRight()-padCol2;

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            rx[i][j] = sinf(j/30.f)*cosf(i/20.f);
            ry[i][j] = sinf(j/50.f);
        }
    //	Since we initialized the vector field "by hand" we know what the valid data
    //	rectangle is.  If we don't set it and the application works in "data safe" mode
    //	the valid data rect will be initialized empty by the constructor and the vector
    //	field would never be rendered.
    gMyField->setValidRect(jLow, iLow, jHigh-jLow+1, iHigh-iLow+1);

    //  Since we have directly modified the content of the raster, we need to notify
    //  the object that it needs to recalculate its arrows/needles the next time
    //  it renders.
    gMyField->forceRasterUpdate();

    //  Finally, we want the field to display its bounding frame
    gMyField->setDrawFrame(true);
    gMyField->setDrawValidDataFrame(true);
    gMyField->setFrameColor(0.f, 0.f, 1.f);
    gMyField->setColor(1.f, 0.f, 0.f);
}

#if 0
#pragma mark -
#endif


//---------------------------------------------------------------------------------
//	myDisplay()
//		See pdf file for explanations on the different drawing modes.
//---------------------------------------------------------------------------------

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2i(0, 0);

    if (gInitDone)
    {
        if (gUseOrigin)
        {
            gMyImage->render(&gOrigin);

            gMyField->render(&gOrigin);
        }
        else
        {
            gMyImage->render(true);

            gMyField->render(true);
        }
    }

    glutSwapBuffers();
}

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

    //  Inform the VectorField and RasterImage classes that the window has been resized.
    //  I only need to do this because I want to do "shifted" rendering.
    //  Furthermore, if multiple windows are used, I could notify individual images
    //  or fields of the dimensions of their window.
    RasterImage::setDefaultWindowDimensions(w, h);
    VectorField::setDefaultWindowDimensions(w, h);
}

#if 0
#pragma mark -
#endif

void myKeyboard(unsigned char c, int x, int y)
{
    float	theScale;

    switch (c)
    {
        case 'q':
            cleanupAndQuit(0);
            break;

        //	, and . (for < and >) decrease and increase the scale
        case ',':
            if (gScaleIndex>1)
            {
                gMyImage->setRenderingScale(--gScaleIndex/10.f);
                gMyField->setRenderingScale(gScaleIndex/10.f);
            }
            theScale = gScaleIndex/10.f;
            break;
        case '.':
            if (gScaleIndex<20)
            {
                gMyImage->setRenderingScale(++gScaleIndex/10.f);
                gMyField->setRenderingScale(gScaleIndex/10.f);
            }
            theScale = gScaleIndex/10.f;
            break;

        case 's':
            cout << "image index = " << gCurrentImg << "\tscale index = " << gScaleIndex <<
            "\tscale = " << gMyImage->getRenderingScaleX() << endl;
            break;

		//	Toggles on-off "display shifted" mode
        case ' ':
            gUseOrigin = ! gUseOrigin;
            break;
            
        default:
            break;

    }

    glutPostRedisplay();
}


void mySpecialKeys(int c, int x, int y)
{
    switch (c)
    {
        case GLUT_KEY_LEFT:
            gOrigin.shiftX(-OBJECT_DISPLCT);
            break;
            
        case GLUT_KEY_RIGHT:
            gOrigin.shiftX(+OBJECT_DISPLCT);
            break;

        case GLUT_KEY_UP:
            gOrigin.shiftY(-OBJECT_DISPLCT);
            break;

        case GLUT_KEY_DOWN:
            gOrigin.shiftY(+OBJECT_DISPLCT);
            break;

        default:
            break;

    }

    glutPostRedisplay();
}


void myMenu(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
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
        backColor = kBackgroundColor[colorIndex];
        glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

        glutPostRedisplay();
    }
}


void myInit(void)
{
    initializeQTEnvironment();

    CreateImageAndField();

    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Create Menus
    int menu, colorSubmenu;

    //	Submenu for background color selection choices
    colorSubmenu = glutCreateMenu(myColorSubmenu);
    for (int i=0; i<kNbBckgndCols; i++)
        glutAddMenuEntry(kBckgndColStr[i].c_str(), i);

    // Main menu that the submenus are connected to
    menu = glutCreateMenu(myMenu);
    glutAddMenuEntry("Quit", QUIT_MENU);
    glutAddSubMenu("Color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    gInitDone = true;
}


#if 0
#pragma mark -
#endif

int main(int argc, char** argv)
{
    int     myW = 800, myH = 600;

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(myW, myH);
    glutCreateWindow("Bounding and Valid Data Rectangles Demo");

    //  This is important for all drawable objects that must be displayed
    //  in "shifted" mode.  Here there is only one winodw, so alll objects
    //  use the same dimensions.  We could also set the window dimensions
    //  for a specific drawable object.
    DrawableObject2D::setDefaultWindowDimensions(myW, myH);

    //  defines glut callback functions that will interrupt the glut loop
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKeys);

    //  Initializes QuickTime and loads the images
    myInit();

    //  Enter infinite glut loop
    glutMainLoop();

    //  Normally this code should never be placed since the only way to leave the
    //  glut loop is to quit
    cleanupAndQuit(0);

    return 0;
}
