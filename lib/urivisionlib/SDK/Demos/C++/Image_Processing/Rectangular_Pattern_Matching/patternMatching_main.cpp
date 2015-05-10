/*  NAME:
        patternMatching_main.cpp
 
    DESCRIPTION:
        demonstration application for PatternMatcher2D classes
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|	This project demonstrates the use of a number of  PatternMatcher2D      |
|   classes.  In particular, its showcases partial pattern matchers that    |
|   deal with occlusion of the pattern by an object of the scene.           |
|   The partial matcher computes matching scores for different parts of     |
|   the pattern (here, 4 quadrants).  The results of the matching for the   |
|   different parts are then analyzed to decide whether there is occlusion. |
|																			|
|	Version	1.2 [05/03/2006]												|
|	Version	1.3 [03/25/2007]    simple cleanup								|
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|	
|																			|
|   o The upper-left subwindow shows the reference image (gray-level or     |
|       color, the user can right-click to select which).  Within this      |
|       image, the user can select a rectangular region that will be used   |
|       as our pattern to find a match for.                                 | 
|	o The upper-right subwindow shows the rectangular region selected in    |
|       the upper-left quadrant                                             |
|	o The lower-left subwindow shows a noisy copy of the original image     |
|       (the noise is selected/applied by the user) in which we will seek   |
|       a best match for the blob pattern selected.                         |
|       Middle click and drag fills in the selected rectangle with random   |
|       pixel values (for occlusion).  In the same menu, the user can       |
|       select the pattern matching algorithm to use for subsequent         |
|       matches.   The user can select a search region by mouse click and   |
|       drag (remider: this is the search region for the upper left corner  |
|       of the pattern).                                                    |
|	o The lower-right subwindow shows the template pattern overlaid on the  |
|                   noisy image at the location of the best match           |
+--------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <typeinfo>
#include <list>
#include <cstdlib>
#include <cmath>
//
#if	__profile__
	#include <Profiler.h>
#endif
//
//	uriVisionLib headers
//
#include "ImageReader.h"
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "PatternMatcher2D_corr.h"
#include "PatternMatcher2D_partCorr.h"
#include "PatternMatcher2D_scaledDiff.h"
#include "PatternMatcher2D_medianDiff.h"
#include "Pattern2DMatchRecord_part.h"
#include "Pattern2D.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------
//	Hack definition of input image files names
//----------------------------------------------
#if	URIVL_OS_MACOS
	const string color_dir = "../../../../../../../../Data_Samples/Color/";
#elif URIVL_OS_WIN32 || URIVL_OS_UNIX
	const string color_dir = "../../../../../../Data_Samples/Color/";
#else
    #error "OS not supported"
#endif

const string filePath = color_dir + "mandrill_256x256.tga";


//----------------------------------------------
//	Some GUI constants
//----------------------------------------------
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

const int   MENU_SEPARATOR = -1,
            QUIT_MENU = 0,
            //
            COLOR_INPUT_MENU = 100,
            GRAY_INPUT_MENU = 110,
            //
            GAUSSIAN_NOISE_MENU = 10,
            SALT_AND_PEPPER_NOISE_MENU = 20,
            RESET_IMAGE_MENU = 30;
            
const int   MATCHER_BASE = 50,
            CORRELATION_MATCHER = 50,
            SCALED_DIFF_MATCHER = 51,
            MEDIAN_DIFF_MATCHER = 52,
            PARTIAL_CORR_MATCHER = 53;


//----------------------------------------------
//	Function prototypes
//----------------------------------------------
void initializeQTEnvironment(void);
void cleanupAndQuit(int exitCode);
//
void loadImage(void);
void applySaltAndPepperNoise(void);
void applyAdditiveNoise(void);
void performMatching(void);
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
void myMenu0(int value);
void myMenu1(int value);
void myMenu2(int value);
void myMenu3(int value);
void myColorSubmenu(int value);
//
void myMouse0(int button, int buttonState, int ix ,int iy);
void myMouse1(int button, int buttonState, int ix ,int iy);
void myMouse2(int button, int buttonState, int ix ,int iy);
void myMouse3(int button, int buttonState, int ix ,int iy);
void handleMouseDrag(int buttonState, int ix, int iy, int winIndex);
void handleMiddleDrag(int buttonState, int ix, int iy, int winIndex);
void myMouseMotion(int ix ,int iy);
bool withinBounds(int ix, int iy);
void myKeyboard(unsigned char c, int x, int y);
void myInit(void);


//----------------------------------------------
//	File-level global variables
//----------------------------------------------
//
int		        gWinWidth,
                gWinHeight,
                gImageWidth = kDefaultWidth,
                gImageHeight = kDefaultHeight;

int			    gMainWindow,
                gSubwindow[kNbSubwindows];

bool			gInitDone = false;

RasterImage    *gGrayImage = NULL;
RasterImage    *gColorImage = NULL;
RasterImage    *gInputImage = NULL;	
RasterImage    *gSearchImage = NULL;	
RasterImage    **gDisplayImage = NULL;	//	an array of references to Image objects
ImageRect      *gSearchRect = new ImageRect(0, 0, 0, 0);
Pattern2D      *gSelectedPat = NULL;
ImagePoint     *gSelectedOrig = new ImagePoint(0, 0);
ImagePoint     *gBestMatch;
bool           gMatchDone = false;

const int	kMinSearchDim = 11;

bool		gMouseDown = false;
int		    gDragWin;
int			gStartDragX,
            gStartDragY;
int         gMatchingMode = CORRELATION_MATCHER;
//
//	For menu handling demo
//
#define	kNbBckgndCols		5
//
GLfloat	kBackgroundColor[kNbBckgndCols][4] =	{	{1.f, 0.f, 0.f, 1.f},	//	red
                                                    {0.f, 0.f, 0.f, 1.f},	//	black
                                                    {0.f, 1.f, 0.f, 1.f},	//	green
                                                    {1.f, 1.f, 0.f, 1.f},	//	yellow
                                                    {0.f, 1.f, 1.f, 1.f}	//	cyan
                                                 };
char	*kBckgndColStr[] =	{
                                "Red",
                                "Green" ,
                                "Yellow",
                                "Black",
                                "Cyan"
                            };

const float ADDITIVE_NOISE_RANGE = 20.f;
const float S_N_P_NOISE_RANGE = 255.f;
const int   NB_S_N_P_PIXELS = 300;

const float ADDITIVE_NOISE_SCALE = (2*ADDITIVE_NOISE_RANGE)/(RAND_MAX + 1.f);
const float S_N_P_NOISE_SCALE = S_N_P_NOISE_RANGE/(RAND_MAX + 1.f);
const float UNIT_SCALE = 1.f/(RAND_MAX + 1.f);


//=============================================================================
//	Function Implementations
//=============================================================================

void initializeQTEnvironment(void)
{
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
}

void cleanupAndQuit(int exitCode)
{
    #if URIVL_USE_QUICKTIME
        // terminate the QuickTime Media Layer
        ExitMovies();

        #if URIVL_OS_WIN32
            TerminateQTML();
        #endif

    	#if __profile__
    		ProfilerDump("\pProfilerData");
    		ProfilerTerm();
    	#endif
    #endif

    std::exit(exitCode);
}

#if (defined(__MWERKS__))
#pragma mark -
#endif


void loadImage(void)
{
    const string colorFilePath = color_dir + "mandrill_256x256.tga";
    ImageReader	myReader(const_cast<char*>(colorFilePath.c_str()));

    //-------------------------------------------------------------
    //	We read the color input images from a file
    //-------------------------------------------------------------
    gColorImage = myReader.getFrame(true);
    if (gColorImage ==NULL)
    {
        cout << "Error encountered while reading image file " << colorFilePath << endl;
        cleanupAndQuit(-1);
    }
    gImageWidth = gColorImage->getWidth();
    gImageHeight = gColorImage->getHeight();
    //
    //  The gray image is a gray-level copy of the color image
    gGrayImage = new RasterImage_gray(gColorImage);
        
    //-------------------------------------------------------------
    //	Initially the search image is a copy of the input image
    //-------------------------------------------------------------
    gInputImage = gColorImage;
    gSearchImage = new RasterImage_RGBa(gColorImage);

    //-------------------------------------------------------------
    //	We allocate the array of display images
    //-------------------------------------------------------------
    gDisplayImage = new RasterImage*[kNbImages];
    
    gDisplayImage[UPPER_LEFT] = gInputImage;
    gDisplayImage[UPPER_RIGHT] = NULL;
    gDisplayImage[LOWER_LEFT] = gSearchImage;
    gDisplayImage[LOWER_RIGHT] = gSearchImage;
    
    gBestMatch = new ImagePoint();
}


void applyAdditiveNoise(void)
{
    const ImageRect *rect = gSearchImage->getValidRect();
    const int   iLow = rect->getTop(),
                iHigh = rect->getBottom(),
                jLow = rect->getLeft(),
                jHigh = rect->getRight();

    switch (gSearchImage->getBaseRasterType())
    {
        case kGrayRaster:
            {
                unsigned char *const* gray = gSearchImage->getShiftedRaster2D(R_W_ACCESS);
                for (int i=iLow; i<=iHigh; i++)
                   for (int j=jLow; j<=jHigh; j++) 
                   {
                      float   v = gray[i][j] + (rand() - 0.5f)*ADDITIVE_NOISE_SCALE;
                      
                      if (v >=0)
                            if (v <256)
                               gray[i][j] = static_cast<unsigned char>(v);
                            else
                               gray[i][j] = static_cast<unsigned char>(255);
                      else
                            gray[i][j] = static_cast<unsigned char>(0);
    
                   }            
               }
            break;
            
        case kRGBa32Raster:
            {
                unsigned char *const* rgba = gSearchImage->getShiftedRaster2D(R_W_ACCESS);
                for (int i=iLow; i<=iHigh; i++)
                   for (int j=jLow; j<=jHigh; j++) 
                      for (int k=0; k<3; k++)
                      {
                            float   v = rgba[i][4*j+k] + (rand() - 0.5f)*ADDITIVE_NOISE_SCALE;
                            
                            if (v >=0)
                               if (v <256)
                                  rgba[i][4*j+k] = static_cast<unsigned char>(v);
                               else
                                  rgba[i][4*j+k] = static_cast<unsigned char>(255);
                            else
                               rgba[i][4*j+k] = static_cast<unsigned char>(0);
    
                      }            
            }
            break;

        default:
            break;
    }
}

void applySaltAndPepperNoise(void)
{
    const ImageRect *rect = gSearchImage->getValidRect();
    const int   iLow = rect->getTop(),
                iHigh = rect->getBottom(),
                jLow = rect->getLeft(),
                jHigh = rect->getRight();

    switch (gSearchImage->getBaseRasterType())
    {
        case kGrayRaster:
            {       
                unsigned char *const* gray = gSearchImage->getShiftedRaster2D(R_W_ACCESS);
                
                for (int l=0; l<NB_S_N_P_PIXELS; l++)
                {
                   int i = (int) (iLow + (iHigh-iLow)*rand()*UNIT_SCALE);
                   int j = (int) (jLow + (jHigh-jLow)*rand()*UNIT_SCALE);
                   gray[i][j] = static_cast<unsigned char>(S_N_P_NOISE_SCALE * rand());
                }
            }
            break;
            
        case kRGBa32Raster:
            {
                unsigned char *const* rgba = gSearchImage->getShiftedRaster2D(R_W_ACCESS);
   
                for (int l=0; l<NB_S_N_P_PIXELS; l++)
                {
                   int i = static_cast<int>(iLow + (iHigh-iLow)*rand()*UNIT_SCALE);
                   int j = static_cast<int>(jLow + (jHigh-jLow)*rand()*UNIT_SCALE);
                   for (int k=0; k<3; k++)
                      rgba[i][4*j+k] = static_cast<unsigned char>(S_N_P_NOISE_SCALE * rand());
                }
            }
            break;

        default:
            break;
    }
}

#if (defined(__MWERKS__))
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
    glutSetWindow(gSubwindow[0]);

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
    glutSetWindow(gSubwindow[1]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gSelectedPat!= NULL)
    {
        gSelectedPat->render(gSelectedOrig);
    }

    glutSwapBuffers();
}

void myDisplay2(void)
{

    glutSetWindow(gSubwindow[2]);

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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    if (gDisplayImage[LOWER_RIGHT]!= NULL)
        gDisplayImage[LOWER_RIGHT]->render();

    if (gMatchDone)
        gSelectedPat->render(gBestMatch);
    
    glutSwapBuffers();
    glutSetWindow(gMainWindow);
}

#if (defined(__MWERKS__))
#pragma mark -
#endif

void myReshape(int w, int h)
{
    if (gDisplayImage != NULL)
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


#if (defined(__MWERKS__))
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

void myMouse0(int button, int buttonState, int ix ,int iy)
{
    if (gDisplayImage[UPPER_LEFT] != NULL)
    {
        glutSetWindow(gSubwindow[UPPER_LEFT]);

        if (button == GLUT_LEFT_BUTTON)
            handleMouseDrag(buttonState, ix, iy, UPPER_LEFT);
    }
}

//	Mouse event handling function for subwindow 1.
void myMouse1(int button, int buttonState, int ix ,int iy)
{
}

//	Mouse event handling function for subwindow 2.
void myMouse2(int button, int buttonState, int ix ,int iy)
{
    if (gDisplayImage[LOWER_LEFT] != NULL)
    {
        glutSetWindow(gSubwindow[LOWER_LEFT]);

        if (button == GLUT_LEFT_BUTTON)
            handleMouseDrag(buttonState, ix, iy, LOWER_LEFT);
        if (button == GLUT_MIDDLE_BUTTON)
            handleMiddleDrag(buttonState, ix, iy, LOWER_LEFT);
    }
}


//	Mouse event handling function for subwindow 3.
void myMouse3(int button, int buttonState, int ix ,int iy)
{
}


bool withinBounds(int ix, int iy)
{
    return ((ix>=0) && (ix<gImageWidth) &&
            (iy>=0) && (iy<gImageHeight));
}

void myMouseMotion(int ix ,int iy)
{
    if (gMouseDown)
    {
        glutSetWindow(gSubwindow[gDragWin]);

        glClear(GL_COLOR_BUFFER_BIT);

        if (gDisplayImage[gDragWin]!= NULL)
        {
            gDisplayImage[gDragWin]->render();
        }

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


void handleMouseDrag(int buttonState, int ix, int iy, int winIndex)
{
    if (buttonState == GLUT_DOWN)
    {
        //  If the mouse button was not already pressed down, we start recording
        if (!gMouseDown)
        {
            gMouseDown = true;
            gDragWin = winIndex;
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
                //  If the selection was done in the upper-left quadrant, we now have a
                //  pattern 2D to look for and match
                if (winIndex == UPPER_LEFT)
                {
                    ImageRect *patRect = new ImageRect(theLeft, theTop, theWidth, theHeight);
                    gSelectedOrig->setCoordinates(theLeft, theTop);
                    
                    if (gSelectedPat != NULL)
                        delete gSelectedPat;
                    gSelectedPat = new Pattern2D(patRect, gInputImage);
                    gSelectedPat->setFrameColor(0.f, 0.f, 1.f);
                    gSelectedPat->setDrawValidDataFrame(true);
                    delete patRect;
                }
                //  If the selection was donw in the lower-left quadrant, then the user has
                //  selected a search region and we should try to do a match.
                //  The selected region must be larger than the pattern, though...
                if ((winIndex == LOWER_LEFT) && (gSelectedPat != NULL) &&
                    (theWidth>gSelectedPat->getWidth()) && (theHeight>gSelectedPat->getHeight()))
                {
                    gSearchRect->setRect(theLeft, theTop, theWidth - gSelectedPat->getWidth(), 
                                         theHeight - gSelectedPat->getHeight());
                    performMatching();
                }
            }
        }
        gMouseDown = false;
        gDragWin = -1;

        myDisplayMain();
    }
}

void handleMiddleDrag(int buttonState, int ix, int iy, int winIndex)
{
    if (buttonState == GLUT_DOWN)
    {
        //  If the mouse button was not already pressed down, we start recording
        if (!gMouseDown)
        {
            gMouseDown = true;
            gDragWin = winIndex;
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
            const int   iLow = MIN(iy, gStartDragY),
                        iHigh = MAX(iy, gStartDragY),
                        jLow = MIN(ix, gStartDragX),
                        jHigh = MAX(ix, gStartDragX);

            //  fill in the selected region with random noise
            switch (gSearchImage->getBaseRasterType())
            {
                case kGrayRaster:
                    {
                        unsigned char *const* gray = gSearchImage->getShiftedRaster2D(R_W_ACCESS);
                        for (int i=iLow; i<=iHigh; i++)
                              for (int j=jLow; j<=jHigh; j++) 
                                 gray[i][j] = static_cast<unsigned char>(
                                     S_N_P_NOISE_SCALE * rand());
                    }
                    break;
                    
                case kRGBa32Raster:
                    {
                        unsigned char *const* rgba = gSearchImage->getShiftedRaster2D(R_W_ACCESS);
                        for (int i=iLow; i<=iHigh; i++)
                              for (int j=jLow; j<=jHigh; j++) 
                                 for (int k=0; k<3; k++)
                                    rgba[i][4*j+k] = static_cast<unsigned char>(
                                        S_N_P_NOISE_SCALE * rand());
                    }
                    break;

                default:
                    break;

            }

        }
        gMouseDown = false;
        gDragWin = -1;

        myDisplayMain();
    }
}


#if (defined(__MWERKS__))
#pragma mark -
#endif

void performMatching(void)
{
    ImagePoint              *matchPt;
    
	#if __profile__
		ProfilerSetStatus(true);
	#endif

    switch(gMatchingMode)
    {
        case CORRELATION_MATCHER:
            {
                PatternMatcher2D_corr   *corrMatcher = new PatternMatcher2D_corr();
                Pattern2DMatchRecord    *corrMatchRec = new Pattern2DMatchRecord();
#if __profile__
for(int i=0; i<10; i++)
#endif
                corrMatcher->match(gSelectedPat, gSearchRect, gSearchImage, corrMatchRec);
                matchPt = corrMatchRec->getBestMatch();
                gBestMatch->setCoordinates(matchPt->getX(), matchPt->getY());
                cout << "Correlation matcher:" << endl << "====================" << endl ;
                cout << "\tBest match:\t\t<" << matchPt->getX() << ", " << matchPt->getY() << 
                      ">, score = " << corrMatchRec->getScore() << endl << endl;
                gMatchDone = true;
                delete corrMatcher;
                delete corrMatchRec;
            }
            break;

        case SCALED_DIFF_MATCHER:
            {
                PatternMatcher2D_scaledDiff *scaledDiffMatcher = new PatternMatcher2D_scaledDiff();
                Pattern2DMatchRecord        *scaledDiffMatchRec = new Pattern2DMatchRecord();
#if __profile__
for(int i=0; i<10; i++)
#endif
                scaledDiffMatcher->match(gSelectedPat, gSearchRect, gSearchImage, 
                    scaledDiffMatchRec);
                matchPt = scaledDiffMatchRec->getBestMatch();
                gBestMatch->setCoordinates(matchPt->getX(), matchPt->getY());
                cout << "Scaled difference matcher:" << endl 
                     << "==========================" << endl ;
                cout << "\tBest match:\t\t<" << matchPt->getX() << ", " << matchPt->getY() << 
                     ">, score = " << scaledDiffMatchRec->getScore() << endl << endl;
                gMatchDone = true;
                delete scaledDiffMatcher;
                delete scaledDiffMatchRec;
            }
            break;

        case MEDIAN_DIFF_MATCHER:
            {
                PatternMatcher2D_medianDiff *medianDiffMatcher = new PatternMatcher2D_medianDiff();
                Pattern2DMatchRecord        *medianDiffMatchRec = new Pattern2DMatchRecord();
#if __profile__
for(int i=0; i<10; i++)
#endif
                medianDiffMatcher->match(gSelectedPat, gSearchRect, gSearchImage, 
                    medianDiffMatchRec);
                matchPt = medianDiffMatchRec->getBestMatch();
                gBestMatch->setCoordinates(matchPt->getX(), matchPt->getY());
                cout << "Median difference matcher:" << endl 
                      << "==========================" << endl ;
                cout << "\tBest match:\t\t<" << matchPt->getX() << ", " << matchPt->getY() << 
                         ">, score = " << medianDiffMatchRec->getScore() << endl << endl;
                gMatchDone = true;
                delete medianDiffMatcher;
                delete medianDiffMatchRec;
            }
            break;

        case PARTIAL_CORR_MATCHER:
            {
                PatternMatcher2D_partCorr *partCorrMatcher = new PatternMatcher2D_partCorr();
                Pattern2DMatchRecord_part *partCorrMatchRec = new Pattern2DMatchRecord_part();
#if __profile__
for(int i=0; i<10; i++)
#endif
                partCorrMatcher->match(gSelectedPat, gSearchRect, gSearchImage, partCorrMatchRec);
                matchPt = static_cast<Pattern2DMatchRecord*>(partCorrMatchRec)->getBestMatch();
                gBestMatch->setCoordinates(matchPt->getX(), matchPt->getY());
   
                cout << "Partial correlation matcher:" << endl 
                   << "============================" << endl ;
                cout << "\tGlobal match:\t<" << matchPt->getX() << ", " << matchPt->getY() << 
                      ">, score = " 
                   << static_cast<Pattern2DMatchRecord*>(partCorrMatchRec)->getScore() << endl;
                //
                Pattern2DMatchRecord  *quadMatch;
                quadMatch = partCorrMatchRec->getMatchRecord(UPPER_LEFT_QUAD);
                cout << "\tUpper-left:\t\t<" << quadMatch->getBestMatch()->getX() << 
                      ", " << quadMatch->getBestMatch()->getY() << 
                      ">, score = " << quadMatch->getScore() << endl;
                quadMatch = partCorrMatchRec->getMatchRecord(UPPER_RIGHT_QUAD);
                cout << "\tUpper-right:\t\t<" << quadMatch->getBestMatch()->getX() << 
                      ", " << quadMatch->getBestMatch()->getY() << 
                      ">, score = " << quadMatch->getScore() << endl;
                quadMatch = partCorrMatchRec->getMatchRecord(LOWER_LEFT_QUAD);
                cout << "\tLower-left:\t\t<" << quadMatch->getBestMatch()->getX() << 
                      ", " << quadMatch->getBestMatch()->getY() << 
                      ">, score = " << quadMatch->getScore() << endl;
                quadMatch = partCorrMatchRec->getMatchRecord(LOWER_RIGHT_QUAD);
                cout << "\tLower-right:\t\t<" << quadMatch->getBestMatch()->getX() << 
                      ", " << quadMatch->getBestMatch()->getY() << 
                      ">, score = " << quadMatch->getScore() << endl << endl;
   
                gMatchDone = true;
                delete partCorrMatcher;
                delete partCorrMatchRec;
            }
            break;
            
        default:
            //  do nothing
            break;
     }

	#if __profile__
		ProfilerSetStatus(false);
	#endif
}

#if (defined(__MWERKS__))
#pragma mark -
#endif

void myMenu0(int value)
{
    switch (value)
    {
        case QUIT_MENU:
            cleanupAndQuit(0);
            break;
        
        case COLOR_INPUT_MENU:
            delete gSelectedPat;
            gSelectedPat = NULL;
            gMatchDone = false;
            gDisplayImage[UPPER_LEFT] = gInputImage = gColorImage;
            delete gSearchImage;
            gSearchImage = new RasterImage_RGBa(gInputImage);
            break;
            
            
        case GRAY_INPUT_MENU:
            delete gSelectedPat;
            gSelectedPat = NULL;
            gMatchDone = false;
            gDisplayImage[UPPER_LEFT] = gInputImage = gGrayImage;
            delete gSearchImage;
            gSearchImage = new RasterImage_gray(gInputImage);
            break;
            
        default:	// This is not happening.  This is not happening. This is not...
            cout << "Why/how did this happen?" << endl;
            break;

    }
    
    myDisplayMain();
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

        case GAUSSIAN_NOISE_MENU:
            applyAdditiveNoise();
            myDisplayMain();
            break;

        case SALT_AND_PEPPER_NOISE_MENU:
            applySaltAndPepperNoise();
            myDisplayMain();
            break;

        case RESET_IMAGE_MENU:
            gInputImage->copyInto(gSearchImage);
            gMatchDone = false;
            myDisplayMain();
            break;

        case CORRELATION_MATCHER:
        case SCALED_DIFF_MATCHER:
        case MEDIAN_DIFF_MATCHER:
        case PARTIAL_CORR_MATCHER:
            gMatchingMode = value;
            if (gMatchDone)
                performMatching();
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

        glutSetWindow(gMainWindow);
        glutPostRedisplay();
    }
}


void myInit(void)
{
    // Create Menus
    int menu[kNbSubwindows], colorSubmenu;

    //	Set a general menu for the main window
    glutSetWindow(gMainWindow);

    //	Submenu for background color selection choices
    colorSubmenu = glutCreateMenu(myColorSubmenu);
    for (int i=0; i<kNbBckgndCols; i++)
        glutAddMenuEntry(kBckgndColStr[i], i);

    //------------------------------------
    //  menus for upper-left quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[UPPER_LEFT]);
    menu[UPPER_LEFT] = glutCreateMenu(myMenu0);
    glutAddMenuEntry("Quit", QUIT_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddMenuEntry("Color input image", COLOR_INPUT_MENU);
    glutAddMenuEntry("Gray-level input image", GRAY_INPUT_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMouseFunc(myMouse0);
    glutMotionFunc(myMouseMotion);

    //------------------------------------
    //  menus for upper-right quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[UPPER_RIGHT]);
    menu[UPPER_RIGHT] = glutCreateMenu(myMenu1);
    glutAddMenuEntry("Quit", QUIT_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //------------------------------------
    //  menus for lower-left quadrant
    //------------------------------------
    glutSetWindow(gSubwindow[LOWER_LEFT]);
    menu[LOWER_LEFT] = glutCreateMenu(myMenu2);
    glutAddMenuEntry("Quit", QUIT_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddMenuEntry("Additive noise", GAUSSIAN_NOISE_MENU);
    glutAddMenuEntry("Salt and pepper noise", SALT_AND_PEPPER_NOISE_MENU);
    glutAddMenuEntry("reset image", RESET_IMAGE_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddMenuEntry("Correlation matcher", CORRELATION_MATCHER);
    glutAddMenuEntry("Scaled centered difference matcher", SCALED_DIFF_MATCHER);
    glutAddMenuEntry("Scaled median difference matcher", MEDIAN_DIFF_MATCHER);
    glutAddMenuEntry("Partial correlation matcher", PARTIAL_CORR_MATCHER);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMouseFunc(myMouse2);
    glutMotionFunc(myMouseMotion);

    //------------------------------------
    //  menus for lower-right quadrant
    //------------------------------------
    //
    glutSetWindow(gSubwindow[LOWER_RIGHT]);
    menu[LOWER_RIGHT] = glutCreateMenu(myMenu3);
    glutAddMenuEntry("Quit", QUIT_MENU);
    //
    glutAddMenuEntry("-", MENU_SEPARATOR);
    //
    glutAddSubMenu("Frame color", colorSubmenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSetWindow(gMainWindow);

    gInitDone = true;
}


int main(int argc, char** argv)
{
	#if __profile__
		OSErr						errProfiler = noErr;
		ProfilerCollectionMethod	method = collectDetailed;
		ProfilerTimeBase			timeBase = bestTimeBase;
		short						numFunctions = 300,
									stackDepth = 150;
	#endif

    int		winID;
    GLfloat	*backColor;
    void	(*displayFuncTable[kNbSubwindows])() = {myDisplay0, myDisplay1,
                                     myDisplay2, myDisplay3};

    try {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        initializeQTEnvironment();

        loadImage();

        //	I initialize my window's dimensions at the default values
        gWinHeight = kNbRowsSubwindows*(gImageHeight + kBorder) + kBorder;
        gWinWidth = kNbColsSubwindows*(gImageWidth + kBorder) + kBorder;

        glutInitWindowSize(gWinWidth,gWinHeight);
        gMainWindow = glutCreateWindow("Pattern Matching Demo");

        glutKeyboardFunc(myKeyboard);
        glutDisplayFunc(myDisplayMain);
        glutReshapeFunc(myReshape);

        glClearColor(0.0, 1.0, 1.0, 1.0);

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

                glutKeyboardFunc(myKeyboard);

                //	This calls maps "world" coordinates to "screen" coordinates.
                //	In this particular case I make them coincide.
                setupCamera(gImageWidth, gImageHeight);
                //
                glutSetWindow(gMainWindow);
            }

        //  Inform drawable objects of the dimensions of my rendering windows
        DrawableObject2D::setDefaultWindowDimensions(gImageWidth, gImageHeight);


    	#if __profile__
    		ProfilerInit(method, timeBase, numFunctions, stackDepth);
    		if (errProfiler != noErr)
    			return 1;
    		ProfilerSetStatus(false);
    	#endif

        myInit();

RasterImage_RGBa *testImg = new RasterImage_RGBa(100, 100);
RasterImage *testImg2 = testImg;
string  s1 = typeid(*testImg).name();
string  s2 = typeid(*testImg2).name();
cout << "With images: true type is --> " << s1 << endl;
cout << "             polymorphism type is still --> " << s2 << endl;


Pattern2DMatchRecord_part *partCorrMatchRec = new Pattern2DMatchRecord_part();
Pattern2DMatchRecord *partCorrMatchRec2 = partCorrMatchRec;
string  s3 = typeid(*partCorrMatchRec).name();
string  s4 = typeid(*partCorrMatchRec2).name();
cout << "With Pattern2DMatchRecord_part: true type is --> " << s3 << endl;
cout << "                                polymorphism type is now --> " << s4 << endl;


        glutMainLoop();
        cleanupAndQuit(0);

        return 0;
    }
    catch (uriVL::ErrorReport & e) {
        cerr << "Error: caught ErrorReport: " << e.getMessage() << endl;
    }
}
