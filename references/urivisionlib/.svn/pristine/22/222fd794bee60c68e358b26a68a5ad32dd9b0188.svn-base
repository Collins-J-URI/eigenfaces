/*  NAME:
        ColorGaussianFilter_I.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorGaussianFilter_I class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "ColorGaussianFilter_I.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGBa_F.h"

using namespace std;
using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ColorGaussianFilter_I::ColorGaussianFilter_I(double theScale)
		try	:	ColorGaussianFilter(theScale, false),
				//
				ig_(NULL),
				iDenum_(1)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian filter is 0.8");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_I constructor");
	throw e;
}


ColorGaussianFilter_I::ColorGaussianFilter_I(int theWidth)
		try	:	ColorGaussianFilter(theWidth, false),
				//
				ig_(NULL),
				iDenum_(1)				
{
    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest width admissible for a Gaussian filter is 3");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_I constructor");
	throw e;
}
			
ColorGaussianFilter_I::ColorGaussianFilter_I(double theScale, const ImageRect* theRect)
		try	:	ColorGaussianFilter(theScale, theRect, false),
				//
				ig_(NULL),
				iDenum_(1)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian filter is 0.8");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_I constructor");
	throw e;
}

ColorGaussianFilter_I::ColorGaussianFilter_I(int theWidth, const ImageRect* theRect)
		try	:	ColorGaussianFilter(theWidth, theRect, false),
				//
				ig_(NULL),
				iDenum_(1)				
{
    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_I constructor");
	throw e;
}
			

ColorGaussianFilter_I::ColorGaussianFilter_I(const ColorGaussianFilter_I& theObj)
		try	:	ColorGaussianFilter(theObj),
				//
				ig_(NULL),
				iDenum_(1)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorGaussianFilter_I copy constructor not implemented.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_I constructor");
	throw e;
}


ColorGaussianFilter_I::~ColorGaussianFilter_I(void)
{
	delete [] ig_;
}


const ColorGaussianFilter_I& ColorGaussianFilter_I::operator = (const ColorGaussianFilter_I& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorGaussianFilter_I copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


RasterImage* ColorGaussianFilter_I::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    return applyTo(imgIn, imgIn->getValidRect());
}

RasterImage* ColorGaussianFilter_I::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to ColorGaussianFilter_I::applyTo");
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in ColorGaussianFilter_I::applyTo");

	ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle in ColorGaussianFilter_F::applyTo");
	
	RasterImage	*imgOut = NULL;
    switch (imgIn->getBaseRasterType())
    {
        case kGrayRaster:
            FAIL_CONDITION(	true,
            				kFunctionNotImplemented,
            				"image type not suported by color Gaussian filter yet");
            break;
            
        case kRGB24Raster:
            FAIL_CONDITION(	true,
            				kFunctionNotImplemented,
            				"image type not suported by color Gaussian filter yet");
            break;
            
        case kRGBa32Raster:
            imgOut = new RasterImage_RGBa_F(destRect);
            break;
            
        default:
            FAIL_CONDITION(	true,
            				kFunctionNotImplemented,
            				"image type not suported by Gaussian filter yet");
            break;
            
    }
    
    applyInto(imgIn, theRect, imgOut);

	delete destRect;
	delete usableRect;
	
	return imgOut;
}

void ColorGaussianFilter_I::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to ColorGaussianFilter_I::applyInto");
    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}

void ColorGaussianFilter_I::applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut)
{
	//----------------------------------------------------------------
	//	STEP 0:		Test for null or invalid type parameters
	//----------------------------------------------------------------
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to ColorGaussianFilter_I::applyInto");

    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in ColorGaussianFilter_I::applyInto");

    FAIL_CONDITION(	imgOut == NULL,
    				kNullRasterImageError,
    				"NULL output raster image passed as parameter to ColorGaussianFilter_I::applyInto");

    //  so far I only accept a RasterImage_RGBa or RasterImage_RGBa_F as a source
	FAIL_CONDITION( imgOut->getBaseRasterType() != kRGBa32Raster,
	                kInvalidRasterType,
	                "ColorGaussianFilter_F::applyInto only accepts RasterImage_RGBa or RasterImage_RGBa_F images as source");

    //  so far I only accept a RasterImage_RGBa or RasterImage_RGBa_F as a destination    
	FAIL_CONDITION( imgOut->getBaseRasterType() != kRGBa32Raster,
	                kInvalidRasterType,
	                "ColorGaussianFilter_F::applyInto only accepts RasterImage_RGBa or RasterImage_RGBa_F images as destination");


	//----------------------------------------------------------------
	//	STEP 1:		Test for rectangle intersection
	//----------------------------------------------------------------
	//  determine the output rectangle that the input image can produce.
	//		Here, because we apply a filter of width&height (2m+1), we
	//		"lose m all around".
	ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	//	... and determine its intersection with the user-specified output rectangle
	ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
	delete usableRect;
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle in ColorGaussianFilter_I::applyInto");
    //  if the output image does not contain the destination rect, it must be resized.
    if (!imgOut->contains(destRect))
    	imgOut->setBoundRect(destRect);
	

	//----------------------------------------------------------------
	//	STEP 2:		Perform the calculations
	//----------------------------------------------------------------
    //	allocate a 2D raster for intermediate calculations if needed
    bool newTempRaster = allocateTempRaster_(imgIn->getValidRect());

    switch (getWidth())
    {
        case 3:
            applyInto_3x3_(imgIn, destRect, imgOut);
            break;

        case 5:
            applyInto_5x5_(imgIn, destRect, imgOut);
            break;

        case 7:
            applyInto_7x7_(imgIn, destRect, imgOut);
            break;

        case 9:
            applyInto_9x9_(imgIn, destRect, imgOut);
            break;

        case 11:
            applyInto_11x11_(imgIn, destRect, imgOut);
            break;

        case 13:
            applyInto_13x13_(imgIn, destRect, imgOut);
            break;

        default:
            applyInto_(imgIn, destRect, imgOut);
            break;
    }

	//----------------------------------------------------------------
	//	STEP 4:		Set rectangle
	//----------------------------------------------------------------
	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
	imgOut->setValidRect(destRect);
	if (imgOut->hasFloatRaster())
		(static_cast<RasterImage_RGBa_F*>(imgOut))->setFloatConversionMode(getFloatConversionMode());

	//----------------------------------------------------------------
	//	STEP 5:		Free temporary storage (if needed)
	//----------------------------------------------------------------
//	if (localRGBa)
//	    delete rgbaImg;
	delete destRect;
    if (newTempRaster)
		freeTempRaster_();

}

#if 0
#pragma mark -
#endif

void ColorGaussianFilter_I::applyInto_3x3_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
// unused variables... -CJC
//     int   iLow = theRect->getTop(),
//             iHigh = theRect->getBottom(),
//             jLow = theRect->getLeft(),
//             jHigh = theRect->getRight();

    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void ColorGaussianFilter_I::applyInto_5x5_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
// unused variables... -CJC
//     int   iLow = theRect->getTop(),
//             iHigh = theRect->getBottom(),
//             jLow = theRect->getLeft(),
//             jHigh = theRect->getRight();

    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void ColorGaussianFilter_I::applyInto_7x7_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
// unused variables... -CJC
//     int   iLow = theRect->getTop(),
//             iHigh = theRect->getBottom(),
//             jLow = theRect->getLeft(),
//             jHigh = theRect->getRight();

    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void ColorGaussianFilter_I::applyInto_9x9_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
// unused variables... -CJC
//     int   iLow = theRect->getTop(),
//             iHigh = theRect->getBottom(),
//             jLow = theRect->getLeft(),
//             jHigh = theRect->getRight();

    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void ColorGaussianFilter_I::applyInto_11x11_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
// unused variables... -CJC
//     int   iLow = theRect->getTop(),
//             iHigh = theRect->getBottom(),
//             jLow = theRect->getLeft(),
//             jHigh = theRect->getRight();

    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void ColorGaussianFilter_I::applyInto_13x13_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
// unused variables... -CJC
//     int   iLow = theRect->getTop(),
//             iHigh = theRect->getBottom(),
//             jLow = theRect->getLeft(),
//             jHigh = theRect->getRight();

    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}


void ColorGaussianFilter_I::applyInto_(const RasterImage* imgIn, const ImageRect* theRect,
                                          RasterImage* imgOut)
{
// unused variables... -CJC
//     int   iLow = theRect->getTop(),
//             iHigh = theRect->getBottom(),
//             jLow = theRect->getLeft(),
//             jHigh = theRect->getRight();

    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

#if 0
#pragma mark -
#endif


void ColorGaussianFilter_I::initializeFilter_(void)
{
    const int   m = getWidth()/2;
	const double scale = getScale();
    const double sigma2Scale = 1.L / (2.L * scale * scale);

    ig_ = new int[m+1];
    FAIL_CONDITION( ig_ == NULL,
                    kFilterAllocationError,
                    "filter allocation failed");
    
    //	the scaling denominator is the inverse of the smallest value of the
    //	"floating point filter"
    iDenum_ = static_cast<int>((sqrt(2*PI)*scale) / exp(-m*m*sigma2Scale));
    
    for (int r=0; r<=m; r++)
    	ig_[r] = static_cast<int>(exp((m*m - -r*r)*sigma2Scale));

}
