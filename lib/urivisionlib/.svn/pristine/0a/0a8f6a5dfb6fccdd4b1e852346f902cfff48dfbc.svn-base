/*  NAME:
        GaussianFilter_I.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib GaussianFilter_I class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "GaussianFilter_I.h"
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


GaussianFilter_I::GaussianFilter_I(double theScale)
		try	:	GaussianFilter(theScale, true),
				//
				ig_(NULL),
				iDenum_(1)				
{
    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter_I constructor");
	throw e;
}


GaussianFilter_I::GaussianFilter_I(int theWidth)
		try	:	GaussianFilter(theWidth, true),
				//
				ig_(NULL),
				iDenum_(1)				
{
    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter_I constructor");
	throw e;
}

			
GaussianFilter_I::GaussianFilter_I(double theScale, const ImageRect* theRect)
		try	:	GaussianFilter(theScale, theRect, true),
				//
				ig_(NULL),
				iDenum_(1)				
{
    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter_I constructor");
	throw e;
}


GaussianFilter_I::GaussianFilter_I(int theWidth, const ImageRect* theRect)
		try	:	GaussianFilter(theWidth, theRect, true),
				//
				ig_(NULL),
				iDenum_(1)				
{
    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter_I constructor");
	throw e;
}

			
GaussianFilter_I::GaussianFilter_I(const GaussianFilter_I& theObj)
			:	GaussianFilter(theObj),
				//
				ig_(NULL),
				iDenum_(1)						
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "GaussianFilter_I copy constructor not implemented.");
}



GaussianFilter_I::~GaussianFilter_I(void)
{
	delete []ig_;
}


const GaussianFilter_I& GaussianFilter_I::operator = (const GaussianFilter_I& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "GaussianFilter_I copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif



RasterImage* GaussianFilter_I::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to GaussianFilter_I::applyTo");

    return applyTo(imgIn, imgIn->getValidRect());
}

RasterImage* GaussianFilter_I::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to GaussianFilter_I::applyTo");
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in GaussianFilter_I::applyTo");

	ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle");

	RasterImage* imgOut = NULL;
	if (imgIn->hasFloatRaster())
	{
	    switch (imgIn->getBaseRasterType())
	    {
	        case kGrayRaster:
	            imgOut = new RasterImage_gray_F(destRect);
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
    }
    else
	{
	    switch (imgIn->getBaseRasterType())
	    {
	        case kGrayRaster:
	            imgOut = new RasterImage_gray(destRect);
	            break;
	            
	        case kRGB24Raster:
	            imgOut = new RasterImage_RGB(destRect);
	            break;
	            
	        case kRGBa32Raster:
	            imgOut = new RasterImage_RGBa(destRect);
	            break;
	            
	        default:
	            FAIL_CONDITION(	true,
	            				kFunctionNotImplemented,
	            				"image type not suported by Gaussian filter yet");
	            break;
	            
	    }
    }
	
    applyInto(imgIn, theRect, imgOut);

	delete destRect;
	delete usableRect;
	
	return imgOut;
}

void GaussianFilter_I::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");
    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}

void GaussianFilter_I::applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut)
{
	//----------------------------------------------------------------
	//	STEP 0:		Test for null or invalid type parameters
	//----------------------------------------------------------------
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in GaussianFilter_I::applyInto");

    FAIL_CONDITION(	imgOut == NULL,
    				kNullRasterImageError,
    				"NULL output raster image passed as parameter");

    //  so far I only accept a RasterImage_gray or RasterImage_gray_F as a source
	FAIL_CONDITION( imgIn->getBaseRasterType() != kGrayRaster,
	                kInvalidRasterType,
	                "GaussianFilter_F::applyInto only accepts RasterImage_gray images as destination");

    //  so far I only accept a RasterImage_gray or RasterImage_gray_F as a destination
	FAIL_CONDITION( imgOut->getBaseRasterType() != kGrayRaster,
	                kInvalidRasterType,
	                "GaussianFilter_F::applyInto only accepts RasterImage_gray images as destination");


	//----------------------------------------------------------------
	//	STEP 1:		Test for rectangle intersection
	//----------------------------------------------------------------
	//  determine the output rectangle that the input image can produce.
	//		Here, because we apply a filter of width&height (2m+1), we
	//		"lose m all around".
	const int	m = getWidth() / 2;
	const ImageRect* inRect = imgIn->getValidRect();
    ImageRect* outRect = new ImageRect(inRect->getLeft() + m, inRect->getTop() + m,
										inRect->getWidth() - 2*m, 
										inRect->getHeight() - 2*m);
	//	... and determine its intersection with the user-specified output rectangle
	ImageRect* destRect = ImageRect::intersection(theRect, outRect);
	delete outRect;
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle");
    //  if the output image does not contain the destination rect, it must be resized.
    if (!outRect->contains(destRect))
    	imgOut->setBoundRect(destRect);
	

	//----------------------------------------------------------------
	//	STEP 2:		Perform the calculations
	//----------------------------------------------------------------
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

	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
	imgOut->setValidRect(destRect);
	imgOut->requestRasterUpdate();    
	//	imgOut->setFloatRasterAsReference();

	delete destRect;
}

#if 0
#pragma mark -
#endif

void GaussianFilter_I::applyInto_3x3_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void GaussianFilter_I::applyInto_5x5_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void GaussianFilter_I::applyInto_7x7_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void GaussianFilter_I::applyInto_9x9_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void GaussianFilter_I::applyInto_11x11_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

void GaussianFilter_I::applyInto_13x13_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}


void GaussianFilter_I::applyInto_(const RasterImage* imgIn, const ImageRect* theRect,
                                          RasterImage* imgOut)
{
    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}

#if 0
#pragma mark -
#endif


void GaussianFilter_I::initializeFilter_(void)
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
