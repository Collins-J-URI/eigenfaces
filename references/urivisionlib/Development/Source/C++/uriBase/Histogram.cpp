/*  NAME:
        Histogram.cpp

    DESCRIPTION:
        implementation of the uriVisionLib Histogram class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "ErrorReport.h"
#include "Histogram.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constants that I do not know where to define
//----------------------------------------------------------
const float PAD_FACTOR = 0.1f;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


Histogram::Histogram(void)
		:	drawAxes_(false),
			drawTitle_(false),
			axesColor_(NULL),
			backgroundColor_(NULL),
			titleColor_(NULL),
			title_(NULL),
			//
			dataType_(NO_TYPE_HIST),
			//
			ucRangeMin_(static_cast<unsigned char>(HIST_DEFAULT_RANGE_MIN)),
			ucRangeMax_(static_cast<unsigned char>(HIST_DEFAULT_RANGE_MAX)),
			iRangeMin_(HIST_DEFAULT_RANGE_MIN),
			iRangeMax_(HIST_DEFAULT_RANGE_MAX),
			flRangeMin_(HIST_DEFAULT_RANGE_MIN),
			flRangeMax_(HIST_DEFAULT_RANGE_MAX),
			//
			histVals_(HIST_DEFAULT_NB_VALS),
			hist_(NULL),
			nbPts_(0),
			//
			renderMode_(LIN_LIN),
			isRenderReady_(false),
			vertPad_(0),
			horPad_(0),
			vertScale_(0),
			horScale_(0),
			renderWidth_(0),
			renderHeight_(0)
{
}

Histogram::Histogram(const float* x, int n, int nbVals, float rangeMin,
                     float rangeMax)
		:	drawAxes_(false),
			drawTitle_(false),
			axesColor_(NULL),
			backgroundColor_(NULL),
			titleColor_(NULL),
			title_(NULL),
			//
			dataType_(FLOAT_HIST),
			//
			ucRangeMin_(static_cast<unsigned char>(HIST_DEFAULT_RANGE_MIN)),
			ucRangeMax_(static_cast<unsigned char>(HIST_DEFAULT_RANGE_MAX)),
			iRangeMin_(HIST_DEFAULT_RANGE_MIN),
			iRangeMax_(HIST_DEFAULT_RANGE_MAX),
			flRangeMin_(rangeMin),
			flRangeMax_(rangeMax),
			//
			histVals_(HIST_DEFAULT_NB_VALS),
			hist_(NULL),
			nbPts_(0),
			//
			renderMode_(LIN_LIN),
			isRenderReady_(false),
			vertPad_(0),
			horPad_(0),
			vertScale_(0),
			horScale_(0),
			renderWidth_(0),
			renderHeight_(0)
{
     FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not open for business yet");
}

/*Histogram::Histogram(const int* x, int n, int nbVals, int rangeMin,
                     int rangeMax)
{
    hist_ = NULL;
    drawAxes_ = drawTitle_ = false;
    color_ = axesColor_ = backgroundColor_ = titleColor_ = NULL;
    //
    dataType_ = INT_HIST;
    renderMode_ = LIN_LIN;
    isRenderReady_ = false;
    //
    iRangeMin_ = rangeMin;
    iRangeMax_ = rangeMax;
    
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not open for business yet");
}
*/
Histogram::Histogram(const unsigned char* x, int n, int nbVals, 
                     unsigned char rangeMin, unsigned char rangeMax)
		:	drawAxes_(false),
			drawTitle_(false),
			axesColor_(NULL),
			backgroundColor_(NULL),
			titleColor_(NULL),
			title_(NULL),
			//
			dataType_(UCHAR_HIST),
			//
			ucRangeMin_(rangeMin),
			ucRangeMax_(rangeMax),
			iRangeMin_(HIST_DEFAULT_RANGE_MIN),
			iRangeMax_(HIST_DEFAULT_RANGE_MAX),
			flRangeMin_(HIST_DEFAULT_RANGE_MIN),
			flRangeMax_(HIST_DEFAULT_RANGE_MAX),
			//
			histVals_(HIST_DEFAULT_NB_VALS),
			hist_(NULL),
			nbPts_(0),
			//
			renderMode_(LIN_LIN),
			isRenderReady_(false),
			vertPad_(0),
			horPad_(0),
			vertScale_(0),
			horScale_(0),
			renderWidth_(0),
			renderHeight_(0)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not open for business yet");
}


Histogram::Histogram(const Histogram& hist)
		:	drawAxes_(false),
			drawTitle_(false),
			axesColor_(NULL),
			backgroundColor_(NULL),
			titleColor_(NULL),
			title_(NULL),
			//
			dataType_(NO_TYPE_HIST),
			//
			ucRangeMin_(static_cast<unsigned char>(HIST_DEFAULT_RANGE_MIN)),
			ucRangeMax_(static_cast<unsigned char>(HIST_DEFAULT_RANGE_MAX)),
			iRangeMin_(HIST_DEFAULT_RANGE_MIN),
			iRangeMax_(HIST_DEFAULT_RANGE_MAX),
			flRangeMin_(HIST_DEFAULT_RANGE_MIN),
			flRangeMax_(HIST_DEFAULT_RANGE_MAX),
			//
			histVals_(HIST_DEFAULT_NB_VALS),
			hist_(NULL),
			nbPts_(0),
			//
			renderMode_(LIN_LIN),
			isRenderReady_(false),
			vertPad_(0),
			horPad_(0),
			vertScale_(0),
			horScale_(0),
			renderWidth_(0),
			renderHeight_(0)
{
     FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Histogram copy constructor not open for business yet");
}



Histogram::~Histogram(void)
{
    delete []hist_;
    
    if (backgroundColor_!=NULL)
        delete []backgroundColor_;
    if (titleColor_!=NULL)
        delete []titleColor_;
    if (axesColor_!=NULL)
        delete []axesColor_;

}


const Histogram& Histogram::operator = (const Histogram& hist)
{
     FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Histogram copy operator not open for business yet");
	return *this;
}


#if 0
#pragma mark -
#endif

const long *Histogram::getHistogram(void) const
{
    return hist_;
}


#if 0
#pragma mark -
#endif

void Histogram::setRenderMode(HistogramRenderMode mode)
{
    if (mode != renderMode_)
    {
        renderMode_ = mode;
        isRenderReady_ = false;
    }
}

void Histogram::setDrawAxes(bool drawAxes)
{
    drawAxes_ = drawAxes;
}


void Histogram::setDrawTitle(bool drawTitle)
{
    drawTitle_ = drawTitle;
}


void Histogram::setBackgroundColor(float theRed, float theGreen, float theBlue) 
{
    if (backgroundColor_ == NULL)
        backgroundColor_ = new float[3];

    backgroundColor_[0] = theRed;
    backgroundColor_[1] = theGreen;
    backgroundColor_[2] = theBlue;
}

void Histogram::setTitleColor(float theRed, float theGreen, float theBlue) 
{
    if (titleColor_ == NULL)
        titleColor_ = new float[3];

    titleColor_[0] = theRed;
    titleColor_[1] = theGreen;
    titleColor_[2] = theBlue;
}

void Histogram::setAxesColor(float theRed, float theGreen, float theBlue)
{
    if (axesColor_ == NULL)
        axesColor_ = new float[3];

    axesColor_[0] = theRed;
    axesColor_[1] = theGreen;
    axesColor_[2] = theBlue;
}

void Histogram::setWindowDimensions(int theWidth, int theHeight)
{
    DrawableObject2D::setWindowDimensions(theWidth, theHeight);

    //  forces a recomputation of rendering parameters
    isRenderReady_ = false;
}


#if 0
#pragma mark -
#endif

void Histogram::rotate(float angle, const ImagePoint* rotCenter)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


void Histogram::applyTransformation(const Transformation2D* theTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}



#if 0
#pragma mark -
#endif


void Histogram::render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
						  float theZ, bool shifted, const ImagePoint* origin) const
{
    if (!isRenderReady_)
        computeRenderingParameters_();
        
    //----------------------------------------------------------------------
    //  Actual drawing
    //----------------------------------------------------------------------
    glPushMatrix();
    glTranslatef(scaledLeft + horPad_, scaledTop + vertPad_, 0.f);
    
    //  If we set a background color, then use it.
    if (backgroundColor_ != NULL)
    {
        glColor3fv(backgroundColor_);
        glBegin(GL_POLYGON);
            glVertex2i(0, 0);
            glVertex2i(renderWidth_, 0);
            glVertex2i(renderWidth_, renderHeight_);
            glVertex2i(0, renderHeight_);
        glEnd();
    }
    
    //  Now apply the scale so that the histogram will fill the space
    glPushMatrix();
    glScalef(horScale_, vertScale_, 1.f);
        
    switch (renderMode_)
    {
        case LIN_LIN:            

            glColor4fv(getColor());
            //
            for (int k=0; k<histVals_; k++)
            {
                glBegin(GL_POLYGON);
                    glVertex2f(k, 0);
                    glVertex2f(k+1, 0);
                    glVertex2f(k+1, hist_[k]);
                    glVertex2f(k, hist_[k]);            
                glEnd();
            }
            break;
                
        case LIN_LOG:

            glColor4fv(getColor());
            //
            for (int k=0; k<histVals_; k++)
            {
                const float logHist = logf(1.f + hist_[k]);
                glBegin(GL_POLYGON);
                    glVertex2f(k, 0);
                    glVertex2f(k+1, 0);
                    glVertex2f(k+1, logHist);
                    glVertex2f(k, logHist);            
                glEnd();
            }
            break;
            
        case LIN_POLAR:
        case LOG_POLAR:
            break;
            
    }
    
    //  draw the axes, if needed
    glPopMatrix();
    if (drawAxes_)
    {
        if (axesColor_ != NULL)
            glColor3fv(axesColor_);
        else
            glColor4f(0.f, 0.f, 0.f, 1.f);

        glBegin(GL_LINE_STRIP);
            glVertex2i(0, renderHeight_);
            glVertex2i(0, 0);
            glVertex2i(renderWidth_, 0);
        glEnd();
    }

    glPopMatrix();
}

void Histogram::computeRenderingParameters_(void) const
{
    //----------------------------------------------------------------------
    //  Determine the width and height dimensions for the rendering.
    //----------------------------------------------------------------------
    const int	winHeight = getWindowHeight(), 
				winWidth = getWindowWidth();

    //  use only part of the window for rendering
    renderHeight_ = static_cast<int>((1.f-PAD_FACTOR) * winHeight), 
    renderWidth_ = static_cast<int>((1.f-PAD_FACTOR) * winWidth);
    //  the unused part is horizontal and vertical "padding"
    horPad_ = (winWidth - renderWidth_) / 2;
    vertPad_ = (winHeight - renderHeight_) / 2;
    
    //  we are going to need the max value in the histogram to compute rendering scales
    long histMax = -1;
    for (int k=0; k<histVals_; k++)
        if (hist_[k] > histMax)
            histMax = hist_[k];

    //----------------------------------------------------------------------
    //  Determine the horizontal and vertical rendering scales 
    //----------------------------------------------------------------------
    switch (renderMode_)
    {
        case LIN_LIN:
            horScale_ = (1.f * renderWidth_) / histVals_;
            vertScale_ = (1.f * renderHeight_) / histMax;
            break;

        case LIN_LOG:
            horScale_ = (1.f * renderWidth_) / histVals_;
            vertScale_ = (1.f * renderHeight_) / logf(1.f+histMax);
            break;
        
            
        default:
            FAIL_CONDITION( true,
                            kFunctionNotImplemented,
                            "Render mode not open for business yet");
                       
    }
    
    isRenderReady_ = true;
}


void Histogram::computeHistogram_(void)
{    
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not open for business yet");
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Must implement these (useless) functions
//------------------------------------------------------
#endif


void Histogram::computeBoundingRect_(ImageRect& rect) const
{
	rect.setRect(horPad_, vertPad_, renderWidth_, renderHeight_);
}

void Histogram::computeValidRect_(ImageRect& rect) const
{
	rect.setRect(horPad_, vertPad_, renderWidth_, renderHeight_);
}

void Histogram::computeCentroid_(ImagePoint_F& cent) const
{
	cent.setCoordinates(horPad_ + 0.5f*renderWidth_, vertPad_ + 0.5f*renderHeight_);
}

void Histogram::computeCenterOfMass_(ImagePoint_F& cent) const
{
	cent.setCoordinates(horPad_ + 0.5f*renderWidth_, vertPad_ + 0.5f*renderHeight_);
}


#if 0
#pragma mark -
#endif


void Histogram::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
}

void Histogram::setValidRect_(int x, int y, int theWidth, int theHeight)
{
}


