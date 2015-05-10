/*
	NAME:
        main_backgroundUpdate.cpp
 
    DESCRIPTION:
       BackgroundRemover_Dynamic_MW's application on video.
 
    COPYRIGHT:
        uriVisionLib Copyright © 2005, uriVisionLib Developers.
 
 
 
        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2 of the License, or (at your option) any later version.
 
        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Lesser General Public License for more details.
 
        You should have received a copy of the GNU Lesser General Public
        License along with this library; if not, write to the Free Software
        Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
/**
 *	select indoor1 or scene movie file under movie/ directory
 */


#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <glut.h>
//
#if URIVL_OS_MACOS
#include <QuickTimeComponents.h>
#include <ImageCompression.h>
#endif
//
#include "comFramework.h"
//
//	uriVisionLib headers
//
#include "ErrorReport.h"
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray_F.h"
#include "VectorField_F.h"
#include "MoviePlayer.h"
#include "ImageReader.h"
#include "BackgroundRemoverTester.h"

using namespace std;
using namespace uriVL;
//using namespace uriVL;

//  Select the type of image to read into.  One only!!
#define READ_IN_RGBA_IMAGE  1
#define READ_IN_RGB_IMAGE   0
#define READ_IN_GRAY_IMAGE  0
#if ( (READ_IN_RGBA_IMAGE && READ_IN_RGB_IMAGE) || (READ_IN_RGBA_IMAGE && READ_IN_GRAY_IMAGE) \
|| (READ_IN_RGB_IMAGE && READ_IN_GRAY_IMAGE) )
error --> pick only one kind of image
#endif
#if !(READ_IN_RGBA_IMAGE || READ_IN_RGB_IMAGE || READ_IN_GRAY_IMAGE)
error --> pick one kind of image
#endif


//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
#define	kNbRowsSubwindows	2
#define	kNbColsSubwindows	2
//
#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640

#define	kBorder				8
#define	kDefaultHeight		256
#define	kDefaultWidth		256
#define kMaxWindowWidth		900
#define	kMaxWindowHeight	640

//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
//
#ifdef	MAC_VERSION
#define kRootPath		":"
#define kPathSeparator	":"
#define kUpRoot			"::"
#define kUpNext			":"
#else	//	Win32 or Unix version
#define kRootPath		"./"
#define kPathSeparator	"/"
#define kUpRoot			"../"
#define kUpNext			"../"
#endif
//
//
#ifndef	PI
#define	PI	3.14159265
#endif


//-----------------------------------------------------------------
//		Function prototypes
//-----------------------------------------------------------------

void initializeQTEnvironment(void);
void cleanupQT(void);

void setupOrthoCam(int w, int h);
void myIdle(void);
void myIdleDone(void);

void myDisplayMain(void);

void myDisplay0(void);
void myDisplay1(void);
void myDisplay2(void);
void myDisplay3(void);

void myReshape(int w, int h);
void myMenu(int value);
void myMouse(int button, int state, int ix , int iy);
void myKeyboard(unsigned char c, int x, int y);

void createRasterImageObj(void);
void glSetupAndRun(void);
int main(int argc, char** argv);



//-----------------------------------------------------------------
//  	Globals to be read from a configuration file (later)
//-----------------------------------------------------------------
int	        gWinWidth,
gWinHeight;

int				gMainWindow,
gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];


bool	        stepByStep = false;
long	        gFrameIndex = 0;
ImageRect		*gMyRect = NULL;

#if READ_IN_RGBA_IMAGE
RasterImage_RGBa	*gMyImage = NULL;
RasterImage_RGBa	*gFirstBackground = NULL;
RasterImage_RGBa	*gCurrBackground = NULL;
#else
#if READ_IN_RGB_IMAGE
RasterImage_RGB	    *gMyImage = NULL;
RasterImage_RGB	    *gFirstBackground = NULL;
RasterImage_RGB	    *gCurrBackground = NULL;
#else
RasterImage_gray	*gMyImage = NULL;
RasterImage_gray	*gFirstBackground = NULL;
RasterImage_gray	*gCurrBackground = NULL;
#endif
#endif

#ifdef	MAC_VERSION
#define	filePath	":::::Data_Samples:Movies:backgroundScene.tga"
#else
#define	filePath	"../../../../Data_Samples/Movies/backgroundScene.tga"
#endif
ImageReader			*myReader;
//RasterImage			*gFirstBackground;
//RasterImage			*gCurrBackground;


MoviePlayer			*gMyPlayer = NULL;
/*
BlobFinder			*gBlobFinder = NULL;
long				gBlobMinSize = 10;
float				gMetricX = 0,
					gMetricY = 0;
BlobTrackingState	*gInitState = NULL,
					*gCurrentState = NULL; 
BlobTracker2D		*gBlobTracker = NULL;
ImagePoint_F		*gCurrentPos = NULL;
*/
//BackgroundRemover_Dynamic_MW	*gRemover	= NULL;
BackgroundRemoverTester		*gTester	= NULL;
int							**gDiffData	= NULL;
int							***gDiffData3D = NULL;
RasterImage_gray			*gDiffImg	= NULL;
int		gXUnit = 0,
		gYUnit = 0,
		gDimX = 0,
		gDimY = 0,
		gNbData = 0;
double	gSum = 0.L,
		gTotalIntensity = 0.L;
float	gMean = 0.f,
		gSumSquare = 0.f,
		gRootMeanSquare = 0.f,
		gNormDiff = 0.f,
		gCoveredPercent = 0.f;
int		gThreshold = 0;		

float	*gCoveredPercNoSmooth;

float	**gRatio = NULL;		

list<float>	gMeanList;
list<float> gRMSList;

ofstream	gFrameIndexOutput("frameIndex.txt", ios::app);
ofstream	gCoveredPercentOutput("coveredPercent.txt", ios::app);
ofstream	gNormDiffOutput("normDiff.txt", ios::app);
ofstream	gRatioOutput("intensityRatio.txt", ios::app);
ofstream	gThresholdOutput("imgThreshold.txt", ios::app);
ofstream	gMeanOutput("meanData.txt", ios::app);
ofstream	gRMSOutput("RootMeanSquareData.txt", ios::app);

#define	TESTCASE 22

//#define DEBUG

//		Platform-specific
//-----------------------------------------------------------------
#if URIVL_OS_MACOS
int 			gAppResFile = kInvalidFileRefNum;	// file reference number
FSSpec			gAppFSSpec;							// file specification
Str255			gAppName;							// name of this application
#endif

//-----------------------------------------------------------------
//		For menu handling demo
//-----------------------------------------------------------------
#define	kNbBckgndCols		6
//
GLfloat	kBackgroundColor[kNbBckgndCols][4] = {	{0.f, 0.f, 0.f, 1.f}, 	//	black
        {1.f, 0.f, 0.f, 1.f}, 	//	red
        {0.f, 1.f, 0.f, 1.f}, 	//	green
        {1.f, 1.f, 0.f, 1.f}, 	//	yellow
        {0.f, 1.f, 1.f, 1.f}, 	//	cyan
        {.1f, .1f, .1f, 1.f}
                                             };
bool	gInitDone = false;
#define	kBlackBackground		0
#define	kGrayBackground			5


//=============================================================================
//=============================================================================


//----------------------------------------------------------------------------
//		initializeQTEnvironment
//
// 		function initialize QuickTime Media Layer and QuickTime
//----------------------------------------------------------------------------
void initializeQTEnvironment(void)
{
    OSErr	myErr = noErr;

#if URIVL_OS_WIN32
    myErr = InitializeQTML(kQTMLNoIdleEvents);
#else
#if URIVL_OS_MACOS
    long	myAttrs;
    myErr = Gestalt(gestaltQuickTime, &myAttrs);
#endif
#endif

    FAIL_CONDITION(	myErr != noErr,
                    kQuickTimeNotInstalled,
                    "QuickTime is not installed on this computer");

    myErr = EnterMovies();
    FAIL_CONDITION(	myErr != noErr,
                    kEnterMoviesError,
                    "Problems with EnterMovies, returning errors (exit)");
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


//============================================================================================
//============================================================================================
#if 0
#pragma mark -
#endif


//---------------------------------------------------------------------------------
//		myIdle
//
//		OpenGL, called when idle by glut
//		contains playing the movie, updating the raster image,
//		and detecting the lines in the image
//---------------------------------------------------------------------------------
void myIdle(void)
{
    static int	frameNum=0;
    OSErr			anErr = noErr;
    OSType			mediaType = VideoMediaType;
    TimeValue		nextTime = -1;

    nextTime = gMyPlayer->getNextTime();

    if (nextTime > -1)
    {
        gMyPlayer->getFrame(gMyImage);

		if(gFrameIndex == 4)
		{
			switch (TESTCASE)
			{
				//	case 1,2(no objects) read 1st bkgd from the first frame of video
				case 0:
				case 1:
				case 21:		
				case 22:
					#if READ_IN_RGBA_IMAGE
					gFirstBackground = new RasterImage_RGBa(gMyImage->getBoundRect());
					#else
					#if READ_IN_RGB_IMAGE
					gFirstBackground = new RasterImage_RGB(gMyImage->getBoundRect());
					#else
					gFirstBackground = new RasterImage_gray(gMyImage->getBoundRect());
					#endif
					#endif

					gMyImage->copyInto(gFirstBackground);
					gTester = new BackgroundRemoverTester(0, gFirstBackground);
					
					break;
					
				//	case 3,4(with objects) read 1st bkgd from a image file
				case 3:
				case 4:
					gFirstBackground = (RasterImage_RGBa *)myReader->getFrame(true);

					break;

				default:	// this should not happen
					break;
			}					
		}

        gFrameIndex++;
		
		if(TESTCASE == 4)
		{
			gYUnit = gTester->getYUnit();
			gXUnit = gTester->getXUnit();
		}
		
		if(gFrameIndex > 4)
		{
			switch (TESTCASE)
			{
				case 0:
					int i,j;

					gTester->test(gMyImage, INTENSITY_RATIO);
				    
					for(i=0; i<gYUnit; i++)
					{
						for(j=0; j<gXUnit-1; j++){
							gRatio = gTester->getRatio();
							gRatioOutput << gRatio[i][j] << " ";
						}
						gRatio = gTester->getRatio();
						gRatioOutput << gRatio[i][j] << endl;
					}
					
					gRatioOutput << "New Frame=============New Frame" << endl;
					
					break;
				
				case 1:
					gTester->test(gMyImage, THRESHOLD_NO_OBJECT);

					if(gFrameIndex > 5)
					{
						gThreshold = gTester->getRightThreshold();
						gDiffImg = gTester->getDiffImg();

						#if READ_IN_RGBA_IMAGE
						gCurrBackground = (RasterImage_RGBa *) gTester->getBackground();
						#else
						#if READ_IN_RGB_IMAGE
						gCurrBackground = (RasterImage_RGB *) gTester->getBackground();
						#else
						gCurrBackground = (RasterImage_gray *) gTester->getBackground();
						#endif
						#endif

				      	gThresholdOutput << gThreshold << endl;
			        	cout << "testing1..." << endl;
		        	}
					
					break;
					
				case 21:
					gTester->test(gMyImage, PERCENTAGE_NO_SMOOTHING, 163);

					if(gFrameIndex > 5)
					{
			        	gDiffData3D	= gTester->getDiffData3D();
				
						#if READ_IN_RGBA_IMAGE
						gCurrBackground = (RasterImage_RGBa *) gTester->getBackground();
						#else
						#if READ_IN_RGB_IMAGE
						gCurrBackground = (RasterImage_RGB *) gTester->getBackground();
						#else
						gCurrBackground = (RasterImage_gray *) gTester->getBackground();
						#endif
						#endif

			        	//	process gDiffData
			        	int i, j, index;
			        	
			        	if(gDimX == 0 && gDimY == 0)		// only initialize once, same eachtime
			        	{
			        		gDimX = gTester->getDimX();
			        		gDimY = gTester->getDimY();
			        		gNbData = gDimX * gDimY;
			        	}
			        	
			        	gCoveredPercNoSmooth = gTester->getCoveredPercNoSmooth();
			        	gTotalIntensity = gTester->getTotalIntensity();
			        	for(index=0; index<gTester->getNbIncreaseRect(); index++)
			        	{
				        	gSum = 0.0;
				        	for(i=0; i<gDimY; i++)
				        		for(j=0; j<gDimX; j++)
				        			gSum += gDiffData3D[index][i][j];
				        	
				        	//	covered percentage
				        	gCoveredPercent = gCoveredPercNoSmooth[index];
				        	gCoveredPercentOutput << gCoveredPercent << endl;
				        	
				        	//	normalized difference
				        	gNormDiff = gSum/gTotalIntensity;
				        	gNormDiffOutput << gNormDiff << endl;
				        	cout << "gSum: " << gSum << endl;
				        	cout << "gTotalIntensity: " << gTotalIntensity << endl;
				        	cout << "gNormDiff: " << gNormDiff << endl;
				        }
					}
					
					break;
					
				case 22:
					gTester->test(gMyImage, PERCENTAGE_NO_OBJECT, 0);

					if(gFrameIndex > 5)
					{
			        	gDiffData	= gTester->getDiffData();
						gDiffImg	= gTester->getDiffImg();
				
						#if READ_IN_RGBA_IMAGE
						gCurrBackground = (RasterImage_RGBa *) gTester->getBackground();
						#else
						#if READ_IN_RGB_IMAGE
						gCurrBackground = (RasterImage_RGB *) gTester->getBackground();
						#else
						gCurrBackground = (RasterImage_gray *) gTester->getBackground();
						#endif
						#endif

			        	//	process gDiffData
			        	int i, j;
			        	
			        	if(gDimX == 0 && gDimY == 0)		// only initialize once, same eachtime
			        	{
			        		gDimX = gTester->getDimX();
			        		gDimY = gTester->getDimY();
			        		gNbData = gDimX * gDimY;
			        	}
			        	
			        	gSum = 0.0;
			        	for(i=0; i<gDimY; i++)
			        		for(j=0; j<gDimX; j++)
			        			gSum += gDiffData[i][j];
			        	
			        	//	frame count ouput
			        	gFrameIndexOutput << gFrameIndex << endl;
			        	
			        	//	covered percentage
			        	gCoveredPercent = gTester->getCoveredPercent();
			        	gCoveredPercentOutput << gCoveredPercent << endl;
			        	
			        	//	normalized difference
			        	gNormDiff = gSum/(gTester->getTotalIntensity());
			        	gNormDiffOutput << gNormDiff << endl;
			        	
			        	//	mean difference
			        	gMean = gSum / gNbData;
			        	//cout << "gNbData: " << gNbData << endl;
			        	//cout << "gSum: " << gSum << endl;
			        	cout << "gMean: " << gMean << endl;
			        	
			        	//	square mean difference
			        	gSumSquare = 0.f;
			        	for(i=0; i<gDimY; i++)
			        		for(j=0; j<gDimX; j++)
			        			gSumSquare += gDiffData[i][j] * gDiffData[i][j];
			        	gRootMeanSquare = sqrt(gSumSquare / gNbData);
			        	
			        	//	output data to 2 separate files
			        	//	data separated by line for later processing in excel
			        	gMeanOutput << gMean << endl;
			        	gRMSOutput << gRootMeanSquare << endl;
						if(gCurrBackground == NULL)
							cout << "gCurrBackground == NULL" << endl;
					}
					
					break;
					
				case 3:
					gTester->test(gMyImage, ALL_OBJECT);
					//	in progress
					
					break;
					
				case 4:
					gTester->test(gMyImage, MISS_SOME_OBJECT);
					//	in progress
					
					break;

				default:	// this should not happen
					break;
			}
		}	
    }
    else
        glutIdleFunc(myIdleDone);

    glutPostRedisplay();

    if (stepByStep)
        glutIdleFunc(myIdleDone);
}

//---------------------------------------------------------------------------------
//		myIdleDone
//
//		Called when the video sequence is over
//---------------------------------------------------------------------------------
void myIdleDone(void)
{
    glutPostRedisplay();
}



//---------------------------------------------------------------------------------
//		myMainReshape
//
//		OpenGL, used to reshape the main display window, calls the three other
//		display reshape methods
//---------------------------------------------------------------------------------

void myReshape(int w, int h)
{
    if ((w != gWinWidth) || (h != gWinHeight))
        glutReshapeWindow(gWinWidth, gWinHeight);

    glutPostRedisplay();
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
    glutSetWindow(gSubwindow[0]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

	if(TESTCASE != 21 && gInitDone)
    {
		gMyImage->render();
	}

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}


void myDisplay1(void)
{
    glutSetWindow(gSubwindow[1]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(TESTCASE != 0 && TESTCASE != 21
		&& gInitDone && gFrameIndex > 5)
    {
		gDiffImg->render();
	}
    
    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay2(void)
{
    glutSetWindow(gSubwindow[2]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(TESTCASE != 21 && gInitDone && gFrameIndex > 5)
	{
	 	if(TESTCASE == 0)
	 		(gTester->getFittedRatioAsRasterImage())->render();
	 	else
	   		gCurrBackground->render();
	}

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(TESTCASE != 21 && gInitDone && gFrameIndex > 5)
	{
	    if(TESTCASE == 0)
	    	(gTester->getRatioAsRasterImage())->render();
		else
	        gFirstBackground->render();
	}

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

#if 0
#pragma mark -
#endif


//---------------------------------------------------------------------------------
//		myKeyboard
//
//		glut, used for key controls during the execution of the program
//---------------------------------------------------------------------------------
void myKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 'q':
            cleanupQT();
            std::exit(0);
            break;

        case 'a':
            glutIdleFunc(myIdle);
            break;

        case 's':
            glutIdleFunc(NULL);
            break;


        default:
            break;

    }

    //    glutPostRedisplay();
}


//---------------------------------------------------------------------------------
//		myMouse
//
//		glut, used for mouse controls during the execution of the program
//---------------------------------------------------------------------------------


void myMouse(int button, int state, int ix , int iy)
{
    // stop start the playing of the images/movie in the display
    // if the left mouse button is clicked, do something
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        stepByStep = true;
        myIdle();
    }

    if (state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON)
    {
        glutIdleFunc(NULL);
    }

    if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
    {
        cleanupQT();
        std::exit(0);
    }

    //	...and then refresh the display
    glutPostRedisplay();
}


//---------------------------------------------------------------------------------
//		myMenu
//
//		glut, sets up menus
//---------------------------------------------------------------------------------
void myMenu(int value)
{
    switch (value)
    {
            //	0 --> Exit/Quit
        case 0:
            std::exit(0);
            break;

            //	1 --> Next image
        case 1:
            break;

            //	2 --> Previous image
        case 2:
            break;

            //	10-14 --> image selection
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            break;

    }

    glutPostRedisplay();
}


#if 0
#pragma mark -
#endif


//---------------------------------------------------------------------------------
//		setupOrthoCam
//
//		This function maps world coordinates to screen coordinates.  This is where
//		the projection mode would be defined.  Here I only want to have a 2D
//		rectangle where to draw 2D stuff on top of my image, so I define my camera
//		as an orthographic camera, and I set the world coordinates to coincide with
//		the pixel coordinates (this is what the gluOrtho2D call does).
//---------------------------------------------------------------------------------
void setupOrthoCam(int w, int h)
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

//-----------------------------------------------------------------
//		createRasterImageObj
//
//		create the raster image objects and setup the rows,cols,pixels
//-----------------------------------------------------------------
void createRasterImageObj(void)
{
	myReader = new ImageReader(filePath);
	/*
	gFirstBackground = myReader->getFrame(true);
	*/
	
    gMyPlayer = new MoviePlayer();
    gMyPlayer->setSource();
    gMyRect = gMyPlayer->getRect();

    int	nbRows = gMyRect->getHeight();
    int	nbCols = gMyRect->getWidth();


#if READ_IN_RGBA_IMAGE
    gMyImage = new RasterImage_RGBa(nbRows, nbCols);
#else
#if READ_IN_RGB_IMAGE
    gMyImage = new RasterImage_RGB(nbRows, nbCols);
#else
    gMyImage = new RasterImage_gray(nbRows, nbCols);
#endif
#endif

    FAIL_CONDITION(	gMyImage == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");
//    FAIL_CONDITION(	gBackgroundImage == NULL,
//                    kRasterImageError,
//                    "Failed to allocate RasterImage");

    //--------------
    //	Create BackgroundRemoverTester
    //--------------
	//gTester = new BackgroundRemoverTester(5, gFirstBackground);
}

//----------------------------------------------------------------------------
//  	glSetupAndRun
//
//		create the raster and line detector objects
//		setup the glut stuff
//		called from either cameraMain or movieMain
//----------------------------------------------------------------------------
void glSetupAndRun(void)
{

    int		winID;
    GLfloat	*backColor;
    void	(*displayFuncTable[kNbRowsSubwindows*kNbColsSubwindows+1])() =
        {myDisplayMain, myDisplay0, myDisplay1, myDisplay2, myDisplay3};

    createRasterImageObj( );

    //	I initialize my window's dimensions at the default values
    int	imgHeight = gMyImage->getHeight();
    int	imgWidth = gMyImage->getWidth();

    //	I initialize my window's dimensions
    gWinHeight = 2 * imgHeight + 3 * kBorder;
  gWinWidth = 2 * imgWidth + 3 * kBorder;

    glutInitWindowSize(gWinWidth, gWinHeight);
    gMainWindow = glutCreateWindow("Movie Processing Demo");
    backColor = kBackgroundColor[kBlackBackground];
    glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

    glutDisplayFunc(myDisplayMain);
    setupOrthoCam(imgWidth, imgHeight);
    glutReshapeFunc(myReshape);
    glutIdleFunc(myIdle);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);

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
            backColor = kBackgroundColor[kGrayBackground];
            glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);

            //	This *has* to be defined:  a "display" callback function for each subwindow
            glutDisplayFunc(displayFuncTable[winID++]);
            glutMouseFunc(myMouse);

            //	This calls maps "world" coordinates to "screen" coordinates.
            //	In this particular case I make them coincide.
            setupOrthoCam(imgWidth, imgHeight);
            //
            glutSetWindow(gMainWindow);
        }


    DrawableObject2D::setDefaultWindowDimensions(imgWidth, imgHeight);

	
//   
//       grayImg = new RasterImage_gray(gCurrentImage->getBoundRect());
//    grad = new ImageGradient_Gaussian_F(2.0);
//    gradient = new VectorField_F(gCurrentImage->getBoundRect());
//       gNormGrad = new RasterImage_gray_F(gCurrentImage->getBoundRect());
//       gThresholdedImg = new RasterImage_RGBa(gCurrentImage->getBoundRect());
//   

    gInitDone = true;

    glutMainLoop();
}

//----------------------------------------------------------------------------
//  	Main
//----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

double  **Amat = new double*[5];
for (int i=0; i<5; i++)
    Amat[i] = new double[3];
double *bVect = new double[5];
Amat[0][0] = 1;
Amat[0][1] = 1;
Amat[0][2] = 1;
Amat[1][0] = 1;
Amat[1][1] = 0;
Amat[1][2] = 0;
Amat[2][0] = 0;
Amat[2][1] = 1;
Amat[2][2] = 0;
Amat[3][0] = 0;
Amat[3][1] = 0;
Amat[3][2] = 1;
Amat[4][0] = 2;
Amat[4][1] = 0;
Amat[4][2] = 0;

bVect[0] = 6;        
bVect[1] = 3;        
bVect[2] = 2;        
bVect[3] = 1;        
bVect[4] = 6;        

Matrix  *A = new Matrix(5, 3, Amat);
ColumnVector    *b = new ColumnVector(5, bVect);

LinearSolver_QR *solver = new LinearSolver_QR();
Matrix  *sol = solver->solve(A, b);

    initializeQTEnvironment( );

    glSetupAndRun( );

    cleanupQT( );

    return 0;
}
