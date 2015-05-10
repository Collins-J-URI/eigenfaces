/*  NAME:
        ImageHistogram.cpp

    DESCRIPTION:
        implementation of the uriVisionLib ImageHistogram class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ErrorReport.h"
#include "ImageHistogram.h"

using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


ImageHistogram::ImageHistogram(void)
    :   Histogram()
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Constructor not open for business yet");
}

ImageHistogram::ImageHistogram(RasterImage* img, const ImageRect* rect)
    :   Histogram()
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Constructor not open for business yet");
}


ImageHistogram::ImageHistogram(RasterImage_gray* img, const ImageRect* rect, int nbVals,
                               unsigned char rangeMin, unsigned char rangeMax)
    :   Histogram()
{
    FAIL_CONDITION( img==NULL,
                    kNullParameterError,
                    "Null rast image passed to ImageHistogram constructor");

    //  determine the rectangle over which to perform the calculations
    const ImageRect   *theRect;
    bool        localRect = false;
    if (rect !=NULL) 
    {
        theRect = ImageRect::intersection(rect, img->getValidRect());
        FAIL_CONDITION( theRect==NULL,
                        kNullRectangleError,
                        "Computation rectangle not within image valid data bounds in ImageHistogram constructor");
        localRect = true;
    }
    else
        theRect = img->getValidRect();

    //  Allocate the histogram and store its parameters
    dataType_ = UCHAR_HIST;
    hist_ = new long[nbVals];
    histVals_ = nbVals;
    ucRangeMin_ = rangeMin;
    ucRangeMax_ = rangeMax;

    //  We have two forms of the function: the "fast" form uses the default 256 values over the
    //  interval [0, 256[, that is no scaling nor shifting.  The "regular" function does the
    //  scaling and shifting calculations
    if ((nbVals==HIST_DEFAULT_NB_VALS) && (rangeMin==HIST_DEFAULT_RANGE_MIN_UC) &&
        (rangeMax==HIST_DEFAULT_RANGE_MAX_UC))
        computeFastHistogram_(img, theRect);
    else
        computeHistogram_(img, theRect);
    
    if (localRect)
        delete theRect;
        
}

ImageHistogram::ImageHistogram(RasterImage_gray_F *img, const ImageRect* rect, int nbVals,
                               float rangeMin, float rangeMax)
    :   Histogram()
{
    FAIL_CONDITION( img==NULL,
                    kNullParameterError,
                    "Null rast image passed to ImageHistogram constructor");

    //  determine the rectangle over which to perform the calculations
    const ImageRect   *theRect;
    bool        localRect = false;
    if (rect !=NULL) 
    {
        theRect = ImageRect::intersection(rect, img->getValidRect());
        FAIL_CONDITION( theRect==NULL,
                        kNullRectangleError,
                        "Computation rectangle not within image valid data bounds in ImageHistogram constructor");
        localRect = true;
    }
    else
        theRect = img->getValidRect();

    //  Allocate the histogram and store its parameters
    dataType_ = FLOAT_HIST;
    hist_ = new long[nbVals];
    histVals_ = nbVals;
    flRangeMin_ = rangeMin;
    flRangeMax_ = rangeMax;
    
    //  There is no point having two forms of the computation function since there will be scaling,
    //  shifting, and rounding no matter what we do.
    computeFloatHistogram_(img, theRect);
    
    if (localRect)
        delete theRect;
        
}


ImageHistogram::~ImageHistogram(void)
{
}


#if 0
#pragma mark -
#endif

void ImageHistogram::setImage(RasterImage* img, const ImageRect* rect)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Constructor not open for business yet");
}


void ImageHistogram::setImage(RasterImage_gray* img, const ImageRect* rect, int nbVals,
                              unsigned char rangeMin, unsigned char rangeMax)
{
    FAIL_CONDITION( img==NULL,
                    kNullParameterError,
                    "Null rast image passed to ImageHistogram constructor");

    //  determine the rectangle over which to perform the calculations
    const ImageRect   *theRect;
    bool        localRect = false;
    if (rect !=NULL) 
    {
        theRect = ImageRect::intersection(rect, img->getValidRect());
        FAIL_CONDITION( theRect==NULL,
                        kNullRectangleError,
                        "Computation rectangle not within image valid data bounds in ImageHistogram constructor");
        localRect = true;
    }
    else
        theRect = img->getValidRect();

    dataType_ = UCHAR_HIST;

    //  If needed re-allocate the histogram and store its parameters
    if (nbVals != histVals_)
    {
        delete []hist_;
        hist_ = new long[nbVals];
        histVals_ = nbVals;
    }
    ucRangeMin_ = rangeMin;
    ucRangeMax_ = rangeMax;
    isRenderReady_ = false;
    
    //  We have two forms of the function: the "fast" form uses the default 256 values over the
    //  interval [0, 256[, that is no scaling nor shifting.  The "regular" function does the
    //  scaling and shifting calculations
    if ((nbVals==HIST_DEFAULT_NB_VALS) && (rangeMin==HIST_DEFAULT_RANGE_MIN_UC) &&
        (rangeMax==HIST_DEFAULT_RANGE_MAX_UC))
        computeFastHistogram_(img, theRect);
    else
        computeHistogram_(img, theRect);
    
    if (localRect)
        delete theRect;
        
}

void ImageHistogram::setImage(RasterImage_gray_F *img, const ImageRect* rect, int nbVals,
                              float rangeMin, float rangeMax)
{
    FAIL_CONDITION( img==NULL,
                    kNullParameterError,
                    "Null rast image passed to ImageHistogram constructor");

    //  determine the rectangle over which to perform the calculations
    const ImageRect   *theRect;
    bool        localRect = false;
    if (rect !=NULL) 
    {
        theRect = ImageRect::intersection(rect, img->getValidRect());
        FAIL_CONDITION( theRect==NULL,
                        kNullRectangleError,
                        "Computation rectangle not within image valid data bounds in ImageHistogram constructor");
        localRect = true;
    }
    else
        theRect = img->getValidRect();

    dataType_ = FLOAT_HIST;

    //  If needed re-allocate the histogram and store its parameters
    if (nbVals != histVals_)
    {
        delete []hist_;
        hist_ = new long[nbVals];
        histVals_ = nbVals;
    }
    flRangeMin_ = rangeMin;
    flRangeMax_ = rangeMax;
    isRenderReady_ = false;
    
    //  There is no point having two forms of the computation function since there will be scaling,
    //  shifting, and rounding no matter what we do.
    computeFloatHistogram_(img, theRect);
    
    if (localRect)
        delete theRect;
        
}


#if 0
#pragma mark -
#endif

void ImageHistogram::computeFastHistogram_(RasterImage_gray* img, const ImageRect* rect)
{
    const int   iLow = rect->getTop(),
                iHigh = rect->getBottom(),
                jLow = rect->getLeft(),
                jHigh = rect->getRight();
    const unsigned char* const* grayIn = img->getShiftedGray2D();

    //  clear the histogram
    for (int k=0; k<histVals_; k++)
        hist_[k] = 0L;
            
    //  and do the counting
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
            hist_[grayIn[i][j]]++;
            
}

void ImageHistogram::computeHistogram_(RasterImage_gray* img, const ImageRect* rect)
{
    const int   iLow = rect->getTop(),
                iHigh = rect->getBottom(),
                jLow = rect->getLeft(),
                jHigh = rect->getRight();
    const float scale = (1.f * histVals_) / (ucRangeMax_ - ucRangeMin_);
    const float shift = -scale*ucRangeMin_;
    const unsigned char* const* grayIn = img->getShiftedGray2D();

    //  clear the histogram
    for (int k=0; k<histVals_; k++)
        hist_[k] = 0L;
        
    //  If we have to compte a histogram for a large number of pixels (I arbitrarily put the bar 
    //  at 4 times 256), then we are better  off precomputing a lookup table
    const long nbPixels = (iHigh-iLow+1)*(jHigh-jLow+1);
    if (nbPixels > 1024)
    {
        int   lut[256];
        for (int l=0; l<256; l++)
            lut[l] = static_cast<int>(shift + scale*l);

        //  and do the counting
        for (int i=iLow; i<=iHigh; i++)
            for (int j=jLow; j<=jHigh; j++)
                hist_[lut[grayIn[i][j]]]++;

    }
    //  for the histogram of a small region, we perform the conversion for each pixel
    else
    {
        //  and do the counting
        for (int i=iLow; i<=iHigh; i++)
            for (int j=jLow; j<=jHigh; j++)
                hist_[static_cast<int>(shift + scale*grayIn[i][j])]++;
     }           
}

void ImageHistogram::computeFloatHistogram_(RasterImage_gray_F *img, const ImageRect* rect)
{
    const int   iLow = rect->getTop(),
                iHigh = rect->getBottom(),
                jLow = rect->getLeft(),
                jHigh = rect->getRight();
    const float scale = (1.f * histVals_) / (ucRangeMax_ - ucRangeMin_);
    const float shift = -scale*ucRangeMin_;
    const float* const* grayIn = img->getShiftedGrayF2D();

    //  clear the histogram
    for (int k=0; k<histVals_; k++)
        hist_[k] = 0;
    
    //  and do the counting
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
            hist_[static_cast<int>(shift + scale*grayIn[i][j])]++;

}


void ImageHistogram::setRenderMode(HistogramRenderMode mode)
{
    //  only accept LIN_LIN or LIN_LOG mode
    if ((mode==LIN_LIN) || (mode==LIN_LOG))
        Histogram::setRenderMode(mode);
        
}


