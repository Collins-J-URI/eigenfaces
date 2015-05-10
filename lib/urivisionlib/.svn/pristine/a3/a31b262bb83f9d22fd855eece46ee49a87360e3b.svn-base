/*  NAME:
        HistogramCalculator.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib HistogramCalculator class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "HistogramCalculator.h"

using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


HistogramCalculator::HistogramCalculator(void)
{
    hist_ = NULL;
}

HistogramCalculator::~HistogramCalculator(void)
{
    delete []hist_;
}


#if 0
#pragma mark -
#endif

void HistogramCalculator::computeHistogram(RasterImage_gray* img)
{
    //  if we already were storing an histogram, delete it.
    delete hist_;

//    hist_ = new Histogram(img);
}

ColorSegParams *HistogramCalculator::getHysteresisFilterParams(unsigned char threshRatio)
{
    ColorSegParams  *myParams = new ColorSegParams;
    myParams->colorMode = false;
    myParams->colorReg = NULL;
    myParams->nbModes = 1;
    myParams->grayReg = new GrayRegion[1];
    myParams->fillMode = kBlackFill;

    //  determined the min and max thresholding values
    unsigned char   vMin=0, threshMin=0, threshMax=255;
    FAIL_CONDITION(	true,
                    kFunctionNotImplemented,
                    "not implemented yet");

    threshMin = static_cast<unsigned char>((vMin + threshMax) / 2);

    (myParams->grayReg[0]).labelStr = "edge";
    (myParams->grayReg[0]).vMin = threshMin;
    (myParams->grayReg[0]).vMax = threshMax;
    (myParams->grayReg[0]).label = static_cast<char>(1);
    (myParams->grayReg[0]).fillColor[0] =
        (myParams->grayReg[0]).fillColor[1] =
            (myParams->grayReg[0]).fillColor[2] = static_cast<unsigned char>(255);
    return myParams;
}


#if 0
#pragma mark -
#endif

Histogram *HistogramCalculator::getHistogram(RasterImage_gray* img)
{
/*
    Histogram   *histOut = new Histogram(img);

    getHistogram(img, histOut);

    return histOut;
*/
    return NULL;
}


void HistogramCalculator::getHistogram(RasterImage_gray* img, Histogram *histOut)
{
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "attempt to compute the histogram of a null RasterImage");
    FAIL_CONDITION(	histOut == NULL,
                    kNullParameterError,
                    "attempt to compute the histogram of an image into a null Histogram");

//    histOut->setImage(img);
}


Histogram *HistogramCalculator::getHistogram(RasterImage_gray* img, const float* backgroundColor,
        const float* histogramColor, const float* axesColor,
        const float* labelColor, const float* titleColor)
{
/*
    Histogram   *histOut = new Histogram(img);

    getHistogram(img, backgroundColor, histogramColor, axesColor, labelColor, titleColor,
                 histOut);

    return histOut;
*/
    return NULL;
}



void HistogramCalculator::getHistogram(RasterImage_gray* img, const float* backgroundColor,
                                       const float* histogramColor, const float* axesColor,
                                       const float* labelColor, const float* titleColor,
                                       Histogram *histOut)
{
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "attempt to compute the histogram of a null RasterImage");
    FAIL_CONDITION(	histOut == NULL,
                    kNullParameterError,
                    "attempt to compute the histogram of an image into a null Histogram");

/*
    histOut->setImage(img);

    histOut->setBackgroundColor(backgroundColor[0], backgroundColor[1], backgroundColor[2]);
    histOut->setColor(histogramColor[0], histogramColor[1], histogramColor[2]);
    histOut->setAxesColor(axesColor[0], axesColor[1], axesColor[2]);
    histOut->setLabelColor(labelColor[0], labelColor[1], labelColor[2]);
    histOut->setTitleColor(titleColor[0], titleColor[1], titleColor[2]);
*/
}



RasterImage_RGBa* HistogramCalculator::getHistogramAsRasterImage(RasterImage_gray* img)
{
    FAIL_CONDITION(	true,
                    kFunctionNotImplemented,
                    "not implemented yet");

    return NULL;
}


void HistogramCalculator::getHistogramAsRasterImage(RasterImage_gray* img,
        RasterImage_RGBa* imgOut)
{
    FAIL_CONDITION(	true,
                    kFunctionNotImplemented,
                    "not implemented yet");

}



RasterImage_RGBa* HistogramCalculator::getHistogramAsRasterImage(RasterImage_gray* img,
                                                               const float* backgroundColor,
                                                               const float* histogramColor,
                                                               const float* axesColor)
{
    FAIL_CONDITION(	true,
                    kFunctionNotImplemented,
                    "not implemented yet");

    return NULL;
}

void HistogramCalculator::getHistogramAsRasterImage(RasterImage_gray* img,
                                                   const float* backgroundColor,
                                                   const float* histogramColor,
                                                   const float* axesColor,
                                                   RasterImage_RGBa* imgOut)
{
    FAIL_CONDITION(	true,
                    kFunctionNotImplemented,
                    "not implemented yet");

}




RasterImage_RGBa* HistogramCalculator::getHistogramAsLabeledRasterImage(RasterImage_gray* img)
{
    float   backgroundColor[4] = {1.f, 1.f, 1.f, 1.f},
                                 histogramColor[4] = {0.f, 0.f, 0.f, 1.f};
    float   *axesColor = histogramColor,
                         *labelColor = histogramColor,
                                       *titleColor = histogramColor;

    return getHistogramAsLabeledRasterImage(img, backgroundColor, histogramColor, axesColor,
                                            labelColor, titleColor);
}



RasterImage_RGBa* HistogramCalculator::getHistogramAsLabeledRasterImage(RasterImage_gray* img,
                                                                        const float* backgroundColor,
                                                                        const float* histogramColor,
                                                                        const float* axesColor,
                                                                        const float* labelColor,
                                                                        const float* titleColor)
{
    FAIL_CONDITION(	true,
                    kFunctionNotImplemented,
                    "not implemented yet");

    return NULL;
}

