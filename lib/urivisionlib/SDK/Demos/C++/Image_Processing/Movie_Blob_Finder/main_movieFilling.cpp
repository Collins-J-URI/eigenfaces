/*  NAME:
        main_movieFilling.cpp
 
    DESCRIPTION:
        demonstration application for the BlobFiller class
 
    COPYRIGHT:
        (c) 2010,	3D Group for Interactive Visualization
                    University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|	main_videoFilling.cpp	A simple combination of the VideoGrabbing and	|
|							BlobFilling demos.								|
|																			|
|	Author:	jean-yves herve',   3D Group for Interactive Visualization,     |
|                           Department of Computer Science and Statistics	|
|								University of Rhode Island					|	
|																			|
|	Version	1.0 [02/06/2010]												|
|																			|
|	The upper-left quadrant shows the original image						|
|	The upper-right subwindow shows the results of a color thresholding		|
|		operation (with saturated colors for matched areas and a gray-level	|
|		copy of the image elsewhere											|
|	The lower-left quadrant shows the result of a "find all blobs"			|
|		operation (the right-click menus allows to change blob min size),	|
|		with the blobs displayed over a gray-level copy of the image.		|
|	The lower-right quadrant shows the blobs found, over a black background	|
|																			|
|	The user can increase or decrease the min blob size by using the keys	|
|		',' [for decrease] and '.' [for increase].							|
+--------------------------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <list>
#include <cstdlib>
#include <cmath>
//
//	uriVisionLib headers
//
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "BlobFinder.h"
#include "ColorThresholdLabeler.h"
#include "MoviePlayer.h"
//
#include "Demos/dataFolderPath.h"

using namespace std;
using namespace uriVL;

//============================================================================
//	Define video source configurations
//============================================================================

//  Select the type of image to read into.  One only!!
#define READ_IN_RGBA_IMAGE  1
#define READ_IN_RGB_IMAGE   0
#if ( READ_IN_RGBA_IMAGE && READ_IN_RGB_IMAGE )
    error --> pick only one kind of image
#endif
#if !(READ_IN_RGBA_IMAGE || READ_IN_RGB_IMAGE)
    error --> pick one kind of image
#endif


//----------------------------------------------
//	Hack definition of movie files names
//----------------------------------------------
const string movie_dir = MOVIE_DIR_PATH;

//============================================================================
//	Some GUI constants
//============================================================================

//	In the future we should be able to get the "native" dimensions from the
//	sequence grabber.  Right now we have to set the dimensions ourselves
//	(this is a project-driven library)

const int	IMG_WIDTH = 640;
const int	IMG_HEIGHT = 480;

const int MIN_BLOB_SIZE = 50;
const int MAX_BLOB_SIZE = 300;

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
const float RENDER_SCALE = 1.0;//0.75f;

            
//============================================================================
//		Function prototypes
//============================================================================
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
//
void initThresholds(void);
void detectAllBlobs(void);
//
void myIdle(void);
void myDisplayMain(void);
void myDisplay0(void);
void myDisplay1(void);
void myDisplay2(void);
void myDisplay3(void);
void displayTextualInfo(void);
//
void myKeyboard(unsigned char c, int x, int y);
//
void myReshape(int w, int h);
void setupCamera(int w, int h);
//
void initializeApplication(void);


//============================================================================
//	File-level global variables
//============================================================================

int		gWinWidth,
		gWinHeight,
		gImageWidth = static_cast<int>(RENDER_SCALE*IMG_WIDTH),
		gImageHeight = static_cast<int>(RENDER_SCALE*IMG_HEIGHT);

int		gMainWindow,
		gSubwindow[kNbSubwindows];

bool	gInitDone = false;

FillingMode	gModeWin[kNbSubwindows] = {kIdentityFill, kBlackFill, kWhiteFill, 
										kOrigGrayFill};

long			gFrameIndex = 0;
ImageRect*		gMyRect = NULL;
MoviePlayer*	gMyPlayer = NULL;

#if READ_IN_RGBA_IMAGE
		RasterImage_RGBa	*gInputImage = NULL;
#else
	#if READ_IN_RGB_IMAGE
		RasterImage_RGB	    *gInputImage = NULL;
	#else
		RasterImage_gray	*gInputImage = NULL;
	#endif
#endif


RasterImage*			gThresholdedImage = NULL;
LabelRaster*			gLabel = NULL;
BlobList* 			    gBlobList = NULL;
ColorThresholdLabeler*	gThresholder = NULL;
ColorSegParams*		    gColorParams = NULL;
BlobFinder*			    gBlobFinder = new BlobFinder();
char*				    gTarget;
long				    gBlobMinSize = 220;

#define DISPLAY_FONT    GLUT_BITMAP_HELVETICA_12
const int FONT_HEIGHT = 12;
const int TEXT_PADDING = 2;
const float kTextColor[3] = {1.f, 1.f, 1.f},
            kTextBackgroundColor[3] = {0.0f, 0.2f, 0.0f};

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



void initThresholds(void)
{
    gThresholder = new ColorThresholdLabeler();
    
    gColorParams = new ColorSegParams;
    gColorParams->fillMode = kOrigGrayFill;
    gColorParams->nbModes = 2;
    gColorParams->colorMode = true;
    gColorParams->grayReg = NULL;
    gColorParams->colorReg = new ColorRegion[gColorParams->nbModes];

	gColorParams->colorReg[0].labelStr =  "magentaish";
    gColorParams->colorReg[0].hueRef = 320;
    gColorParams->colorReg[0].deltaHue = 40;
    gColorParams->colorReg[0].satMin = 40;
    gColorParams->colorReg[0].satMax = 100;
    gColorParams->colorReg[0].valMin = 40;
    gColorParams->colorReg[0].valMax = 255;
    gColorParams->colorReg[0].fillColor[0] = static_cast<unsigned char>(255);
    gColorParams->colorReg[0].fillColor[1] = static_cast<unsigned char>(0);
    gColorParams->colorReg[0].fillColor[2] = static_cast<unsigned char>(255);
    gColorParams->colorReg[0].label = static_cast<char>(1);

    gColorParams->colorReg[1].labelStr = "cianish";
    gColorParams->colorReg[1].hueRef = 200;
    gColorParams->colorReg[1].deltaHue = 25;
    gColorParams->colorReg[1].satMin = 60;
    gColorParams->colorReg[1].satMax = 100;
    gColorParams->colorReg[1].valMin = 40;
    gColorParams->colorReg[1].valMax = 255;
    gColorParams->colorReg[1].fillColor[0] = static_cast<unsigned char>(0);
    gColorParams->colorReg[1].fillColor[1] = static_cast<unsigned char>(255);
    gColorParams->colorReg[1].fillColor[2] = static_cast<unsigned char>(255);
    gColorParams->colorReg[1].label = static_cast<char>(2);

	gTarget = new char[gColorParams->nbModes];
	for (int i=0; i<gColorParams->nbModes; i++)
		gTarget[i] = gColorParams->colorReg[i].label;
		
    gBlobList = new BlobList[gColorParams->nbModes];
	
}


void detectAllBlobs(void)
{
    gThresholder->applyInto(gInputImage, gColorParams, gLabel);

    //	clear the lists of blobs for each target color
    for (int k=0; k<gColorParams->nbModes; k++)
    {
        if (!gBlobList[k].empty())
		{
			BlobList::iterator	iter;
			for (iter=gBlobList[k].begin(); iter!=gBlobList[k].end(); iter++)
				//	delete the blob allocated
				delete (*iter);

			gBlobList[k].clear();
		}
    }

    gBlobList = gBlobFinder->findAllBlobs(gLabel, gColorParams->nbModes,
    						  gTarget, gBlobMinSize, false);
    for (int k=0; k<gColorParams->nbModes ; k++)
	{
        if (!gBlobList[k].empty())
        {
            BlobList::iterator iter, endList = gBlobList[k].end();
            for (iter = gBlobList[k].begin(); iter != endList; iter++)
            {
                (*iter)->setColor((gColorParams->colorReg[k]).fillColor);
                (*iter)->setContourColor(1.f, 1.f, 1.f);
				(*iter)->setRenderingScale(RENDER_SCALE);
            }
        }
	}
}

#if 0
#pragma mark -
#endif


//---------------------------------------------------------------------------------
//	Callback functions
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//		myIdle
//
//		OpenGL, called when idle by glut
//		contains playing the movie, updating the raster image,
//		and detecting the lines in the image
//---------------------------------------------------------------------------------
void myIdle(void)
{
	if (gInitDone) {
		if (!gMyPlayer->isFinished())
		{
			gMyPlayer->getFrame(gInputImage);
		}
		
		gFrameIndex++;

		gColorParams->fillMode = kOrigGrayFill;

		gThresholder->applyInto(gInputImage, (char *) gColorParams, gThresholdedImage);

		detectAllBlobs();
	}
	
    glutPostRedisplay();
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
		if (gInputImage!= NULL)
		{
			gInputImage->render();
		}
	}
    glutSwapBuffers();
}

void myDisplay1(void)
{
    glutSetWindow(gSubwindow[UPPER_RIGHT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

	if (gInitDone)
	{
		if (gThresholdedImage != NULL)
		{
			gThresholdedImage->render();
		}
	}
	
    glutSwapBuffers();
}

void myDisplay2(void)
{
    glutSetWindow(gSubwindow[LOWER_LEFT]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

	if (gInitDone)
	{
		if (gInputImage!= NULL)
			gInputImage->render();

		for (int k=0; k<gColorParams->nbModes ; k++)
		{
			if (!gBlobList[k].empty())
			{
				BlobList::iterator iter;
				for (iter = gBlobList[k].begin(); iter != gBlobList[k].end(); iter++)
					(*iter)->render(true);
			}
		}
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
		for (int k=0; k<gColorParams->nbModes ; k++)
		{
			if (!gBlobList[k].empty())
			{
				BlobList::iterator iter;
				for (iter = gBlobList[k].begin(); iter != gBlobList[k].end(); iter++)
					(*iter)->render(true);
			}
		}
	}

	displayTextualInfo();
	
    glutSwapBuffers();
    glutSetWindow(gMainWindow);
}



void displayTextualInfo(void)
{
    //-----------------------------------------------
    //  0.  get current material properties
    //-----------------------------------------------
    float oldAmb[4], oldDif[4], oldSpec[4], oldShiny;
    glGetMaterialfv(GL_FRONT, GL_AMBIENT, oldAmb);
    glGetMaterialfv(GL_FRONT, GL_DIFFUSE, oldDif);
    glGetMaterialfv(GL_FRONT, GL_SPECULAR, oldSpec);
    glGetMaterialfv(GL_FRONT, GL_SHININESS, &oldShiny);

    glPushMatrix();

    //-----------------------------------------------
    //  1.  Build the string to display
    //-----------------------------------------------
	char infoStr[256];
	sprintf(infoStr, "Blob threshold: %d ", gBlobMinSize);
    int infoLn = strlen(infoStr);
                          
    //-----------------------------------------------
    //  2.  Determine the string's length (in pixels)
    //-----------------------------------------------
    int textWidth = 0;
    for (int k=0; k<infoLn; k++)
        textWidth += glutBitmapWidth(DISPLAY_FONT, infoStr[k]);
    //  add a few pixels of padding
    textWidth += 2*TEXT_PADDING;

    //-----------------------------------------------
    //  3.  Clear background rectangle
    //-----------------------------------------------
//    setMaterial(kTextBackgroundColor[0], kTextBackgroundColor[1], kTextBackgroundColor[2]);
	glColor3f(kTextBackgroundColor[0], kTextBackgroundColor[1], kTextBackgroundColor[2]);
    glBegin(GL_POLYGON);
        glVertex2i(0, gImageHeight);
        glVertex2i(gImageWidth, gImageHeight);
        glVertex2i(gImageWidth, gImageHeight - FONT_HEIGHT - 4);
        glVertex2i(0, gImageHeight - FONT_HEIGHT - 4);
    glEnd();
	glTranslatef(0.f, 0.f, 1.f);
    
    //-----------------------------------------------
    //  4.  Draw the string
    //-----------------------------------------------    
//    setMaterial(kTextColor[0], kTextColor[1], kTextColor[2]);
    glColor3f(kTextColor[0], kTextColor[1], kTextColor[2]);
    int xPos = TEXT_PADDING,
        yPos = gImageHeight - FONT_HEIGHT;
    for (int k=0, i=0; k<infoLn; k++)
    {
        glRasterPos2i(xPos, yPos);
        glutBitmapCharacter(DISPLAY_FONT, infoStr[k]);
        xPos += glutBitmapWidth(DISPLAY_FONT, infoStr[k]);
    }

    //-----------------------------------------------
    //  5.  Restore old material properties
    //-----------------------------------------------
	glMaterialfv(GL_FRONT, GL_AMBIENT, oldAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, oldDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, oldSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, oldShiny);  
    
    //-----------------------------------------------
    //  6.  Restore reference frame
    //-----------------------------------------------
    glPopMatrix();
}

#if 0
#pragma mark -
#endif

void myReshape(int w, int h)
{
    if (gInputImage != NULL)
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

		case ',':
			gBlobMinSize -= 5;
			if (gBlobMinSize < MIN_BLOB_SIZE)
				gBlobMinSize = MIN_BLOB_SIZE;
			break;
			
		case '.':
			gBlobMinSize += 5;
			if (gBlobMinSize > MAX_BLOB_SIZE)
				gBlobMinSize = MAX_BLOB_SIZE;
			break;
			
        default:
            break;

    }

    glutPostRedisplay();
}


void initializeApplication(void)
{
	gMyPlayer = new MoviePlayer();
	gMyPlayer->setSource();

    const ImageRect *myRect = gMyPlayer->getRect();
    int	nbRows = myRect->getHeight();
    int	nbCols = myRect->getWidth();

	ImageRect imgRect(0, 0, nbCols-1, nbRows-1);

	#if READ_IN_RGBA_IMAGE
		gInputImage = new RasterImage_RGBa(&imgRect);
	#elif READ_IN_RGB_IMAGE
		gInputImage = new RasterImage_RGB(&imgRect);
	#endif
	
	gThresholdedImage = new RasterImage_RGBa(&imgRect);
	gInputImage->setRenderingScale(RENDER_SCALE);
	gThresholdedImage->setRenderingScale(RENDER_SCALE);
	
	gLabel = new LabelRaster(&imgRect);
	
	gInputImage->setValidRect(&imgRect);
	initThresholds();	
}


int main(int argc, char** argv)
{
    int		winID;
    void	(*displayFuncTable[kNbSubwindows])() = {myDisplay0, myDisplay1,
                                     myDisplay2, myDisplay3};


    try {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        initializeQTEnvironment();

        //	I initialize my window's dimensions at the default values
        gWinHeight = kNbRowsSubwindows*(gImageHeight + kBorder) + kBorder;
        gWinWidth = kNbColsSubwindows*(gImageWidth + kBorder) + kBorder;

        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Blob Filling Demo");

		glutIdleFunc(myIdle);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);
        glutKeyboardFunc(myKeyboard);

        winID = 0;
        glutSetWindow(gMainWindow);
		glClearColor(0.1, 0.1, 0.1, 1.);
		
        for (int i=0; i<kNbRowsSubwindows; i++)
            for (int j=0; j<kNbColsSubwindows; j++)
            {
                gSubwindow[winID] = glutCreateSubWindow(
                                        gMainWindow,
                                        j*(gImageWidth + kBorder) + kBorder,
                                        i*(gImageHeight + kBorder) + kBorder,
                                        gImageWidth,
                                        gImageHeight);

				glClearColor(0.0, 0.0, 0.0, 1.0);

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

		initializeApplication();
		
		gInitDone = true;

        glutMainLoop();
        cleanupAndQuit(0);

        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
