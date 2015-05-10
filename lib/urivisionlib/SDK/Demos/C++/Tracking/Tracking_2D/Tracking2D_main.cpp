/*  NAME:
        Tracking2D_main.cpp
 
    DESCRIPTION:
       Tracker2D's application on video.
 
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
#include "BlobTracker2D.h"

using namespace std;
using namespace uriVL;

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
short	        gWinWidth,
gWinHeight;

int				gMainWindow,
gSubwindow[kNbRowsSubwindows*kNbColsSubwindows];


bool	        stepByStep = false;
long	        gFrameIndex = 0;
ImageRect		*gMyRect = NULL;

#if READ_IN_RGBA_IMAGE
RasterImage_RGBa	*gMyImage = NULL;
#else
#if READ_IN_RGB_IMAGE
RasterImage_RGB	    *gMyImage = NULL;
#else
RasterImage_gray	*gMyImage = NULL;
#endif
#endif

#ifdef	MAC_VERSION
#define	filePath	":::::Data_Samples:Color:mandrill_256x256.tga"
#else
#define	filePath	"../../../../Data_Samples/background.tga"
#endif
ImageReader			*myReader;
RasterImage			*gBackgroundImage;


MoviePlayer			*gMyPlayer = NULL;
BlobFinder			*gBlobFinder = NULL;
long				gBlobMinSize = 10;
float				gMetricX = 0,
					gMetricY = 0;
BlobTrackingState	*gInitState = NULL,
					*gCurrentState = NULL; 
BlobTracker2D		*gBlobTracker = NULL;
ImagePoint_F		*gCurrentPos = NULL;

//#define DEBUG

//		Platform-specific
//-----------------------------------------------------------------
#if URIVL_OS_MACOS
short 			gAppResFile = kInvalidFileRefNum;	// file reference number
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


void cleanupQT(void)
{
    // terminate the QuickTime Media Layer
    ExitMovies();

#if URIVL_OS_WIN32
    TerminateQTML();
#endif
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
    static short	frameNum=0;
    OSErr			anErr = noErr;
    OSType			mediaType = VideoMediaType;
    TimeValue		nextTime = -1;

    nextTime = gMyPlayer->getNextTime();

    if (nextTime > -1)
    {
        gMyPlayer->getFrame(gMyImage);

        gFrameIndex++;

        //  apply Tracker2D to render the trace of the moving object
        gCurrentState = (BlobTrackingState*) gBlobTracker->track(gMyImage);	//	downcast to a BlobTrackingState
        gCurrentPos = gCurrentState->getPos();
        
        #ifdef DEBUG
        	cout << "\n x:" << gCurrentPos->getX() << endl;
        #endif
              
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

    if (gInitDone)
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

    if (gInitDone)
    {
		gBackgroundImage->render();
    }
    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay2(void)
{
    short	imgHeight = gMyImage->getHeight();

    glutSetWindow(gSubwindow[2]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gInitDone)
    {
        gBackgroundImage->render();

	    glLoadIdentity();
	    glTranslatef(0.f, imgHeight, -0.5);
	    glColor3f(1., 0, 0);
	    glScalef(1, -1, 1.f);

		glTranslatef(gCurrentPos->getX(), gCurrentPos->getY(), 0);
		const short nbPts = 12;
		glBegin(GL_POLYGON);
			for (short i=0; i<12; i++)
				glVertex3d(3*cos(2*PI*i/nbPts), 3*sin(2*PI*i/nbPts), 0);
		glEnd();
    }

    glutSwapBuffers();

    glutSetWindow(gMainWindow);
}

void myDisplay3(void)
{
    glutSetWindow(gSubwindow[3]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gInitDone)
    {
        gMyImage->render();

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
	gBackgroundImage = myReader->getFrame(true);

    gMyPlayer = new MoviePlayer();
    gMyPlayer->setSource();
    gMyRect = gMyPlayer->getRect();

    short	nbRows = gMyRect->getHeight();
    short	nbCols = gMyRect->getWidth();


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
    FAIL_CONDITION(	gBackgroundImage == NULL,
                    kRasterImageError,
                    "Failed to allocate RasterImage");

    //--------------
    //	Create tracker
    //--------------
	gInitState = new BlobTrackingState(253, 25, gBlobMinSize);
	gBlobFinder = new BlobFinder();
	gBlobTracker = new BlobTracker2D(gBlobFinder,gMetricX,
									 gMetricY,gInitState,gBackgroundImage);
	
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
    short	imgHeight = 320;//gMyImage->getHeight();
    short	imgWidth = 240;//gMyImage->getWidth();

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

    initializeQTEnvironment( );

    glSetupAndRun( );

    cleanupQT( );

    return 0;
}


/* Use following BlobTracker2D   .cpp & .h to make this DEMO run

//  BlobTracker2D.h

#ifndef BLOB_TRACKER_2D_H
#define BLOB_TRACKER_2D_H

#include "Tracker2D.h"
#include "BlobTrackingState.h"
#include "BackgroundRemover_Static.h"
#include "GrayThresholdLabeler.h"
#include "BlobFinder.h"
#include "ImagePoint.h"
#include "ImagePoint_F.h"

namespace uriTracking
{
	using namespace std;
	using namespace uriVL;
	using namespace uriVL;

	class URIVL_EXPORT BlobTracker2D : public Tracker2D
	{
		public:
		
			BlobTracker2D(FeatureDetector *detector,
						  float metricX,
						  float metricY,
						  BlobTrackingState *initState,
						  RasterImage* bkgdImg,
						  bool savePred = false,
						  bool saveState = false);
										  
			~BlobTracker2D(void);
			
			
			TrackingState *track(RasterImage *img);

			TrackingState *track(RasterImage *img,
								 RasterImage *bkgdImg);
			
			TrackingStateList getPredRecord(void);
			
			TrackingStateList getStateRecord(void);
			
			
			
		protected:
		
			GrayThresholdLabeler	*thresholder_;
			
			ColorSegParams			*colorParams_;
			
			RasterImage				*backgroundImg_;
			
			RasterImage				*currImg_;

			BackgroundRemover_Static	*remover_;
	};
}

#endif		//  BLOB_TRACKER_2D_H


----------------------------------------------------------------------------------------


//  BlobTracker2D.cpp

#include "BlobTracker2D.h"
#include <iostream>

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructors
//----------------------------------------------------------

BlobTracker2D::BlobTracker2D(FeatureDetector *detector,
							 float metricX,
							 float metricY,
							 BlobTrackingState *initState,
							 RasterImage *bkgdImg,
							 bool savePred, 
							 bool saveState )
	: Tracker2D(detector, metricX, metricY, initState, savePred, saveState)
{
	thresholder_	= new GrayThresholdLabeler();
	colorParams_	= new ColorSegParams;
	remover_		= new BackgroundRemover_Static();
	backgroundImg_	= bkgdImg;
	currImg_		= NULL;
	
	colorParams_->fillMode	= kOrigColorFill;
	colorParams_->nbModes	= 1;
	colorParams_->colorMode	= false;
	colorParams_->colorReg	= NULL;
	colorParams_->grayReg	= new GrayRegion[colorParams_->nbModes];
    colorParams_->grayReg[0].labelStr =  "medium gray";
    colorParams_->grayReg[0].vMin = 5;
    colorParams_->grayReg[0].vMax = 255;
    colorParams_->grayReg[0].fillColor[0] = 0;
    colorParams_->grayReg[0].fillColor[1] = (unsigned char) 255;
    colorParams_->grayReg[0].fillColor[2] = (unsigned char) 0;
    colorParams_->grayReg[0].label = 1;
	remover_->setBackground(backgroundImg_);

}

BlobTracker2D::~BlobTracker2D(void)
{
	delete thresholder_;
	delete colorParams_;
	delete remover_;
	if(currImg_ != NULL)
		delete currImg_;
}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

TrackingState* BlobTracker2D::track(RasterImage* img)
{
	ImageRect	*theRect = ImageRect::intersection(img->getBoundRect(), backgroundImg_->getBoundRect());
	
	float oldX = currentState_->getPos()->getX();
	float oldY = currentState_->getPos()->getY();	
	
	float predX = oldX+metricX_,
		  predY = oldY+metricY_;
	if(predX > theRect->getRight())
		predX = theRect->getRight();
	else if(predX < theRect->getLeft())
		predX = theRect->getRight();
	if(predY > theRect->getBottom())
		predY = theRect->getBottom();
	else if(predY < theRect->getTop())
		predY = theRect->getTop();

	//	prediction 
	if(savePred_ == true){				//  save to predRecord_
		BlobTrackingState *predState = new BlobTrackingState(predX, predY,
											((BlobTrackingState *)currentState_)->getSize());
		predRecord_.push_back(predState);
	}
	ImagePoint *predPos = new ImagePoint((short) predX, (short) predY);
	//	end of prediction


	//	track based on prediction  
	currImg_ = img;
	RasterImage_gray	*differImg	= new RasterImage_gray(theRect);
	long	sizeRange = 8,
			temp = ((BlobTrackingState *)currentState_)->getSize() - sizeRange,
			blobMinSize = temp > 0 ? temp : (temp+sizeRange);

	remover_->removeBackground(currImg_, differImg, MAX_DIFFERENCE);
	LabelRaster	*label	= thresholder_->applyTo(differImg, colorParams_);
	Blob		*blob	= ((BlobFinder*) detector_)->findBlobAbout(label, predPos, 
											 colorParams_->grayReg[0].label,
											 blobMinSize, theRect);
	//  end of track based on prediction 
	
	
	//	real case 
	//  update currentState_, metricX_, metricY_
	currentState_->setPos(blob->getCentroid());
	metricX_ = currentState_->getPos()->getX() - oldX;
	metricY_ = currentState_->getPos()->getY() - oldY;
	((BlobTrackingState *) currentState_)->setSize(blob->getNbPixels());
	if(saveState_ == true){					//  save to stateRecord_
		BlobTrackingState *realState = new BlobTrackingState(currentState_->getPos()->getX(),
										 		currentState_->getPos()->getY(),
										 		((BlobTrackingState *)currentState_)->getSize());
		stateRecord_.push_back(realState);
	}
	//	end of real case	
	
	delete differImg;
	delete predPos;
	delete label;
	delete blob;
	
	return currentState_;
}


TrackingStateList BlobTracker2D::getPredRecord(void)
{
	Tracker2D::getPredRecord();
}

TrackingStateList BlobTracker2D::getStateRecord(void)
{
	Tracker2D::getStateRecord();
}

//----------------------------------------------------------
//  private and protected functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif


*/
