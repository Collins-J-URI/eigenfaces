/*  NAME:
        ImageLaplacian_Gaussian_I.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImagePoint class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ImageLaplacian_Gaussian_I.h"
#include "RasterImage_gray_F.h"

using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif

ImageLaplacian_Gaussian_I::ImageLaplacian_Gaussian_I(double theScale)
		try	:	ImageLaplacian(2*(((int ) (5*theScale))/2) + 1, 2*(((int ) (5*theScale))/2) + 1, true),
				//
				scale_(theScale),
				iL_(NULL),
				iDenum_(0)
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian Laplacian operator is 0.8");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageLaplacian_Gaussian_I constructor");
	throw e;
}



ImageLaplacian_Gaussian_I::ImageLaplacian_Gaussian_I(int theWidth)
		try	:	ImageLaplacian(theWidth, theWidth, true),
				//
				scale_(0.2L*getWidth()),
				iL_(NULL),
				iDenum_(0)		
{
    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageLaplacian_Gaussian_I constructor");
	throw e;
}
			
ImageLaplacian_Gaussian_I::ImageLaplacian_Gaussian_I(const ImageLaplacian_Gaussian_I& theObj)
		try	:	ImageLaplacian(theObj),
				//
				scale_(0.L),
				iL_(NULL),
				iDenum_(0)		
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageLaplacian_Gaussian_I copy constructor not implemented.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageLaplacian_Gaussian_I constructor");
	throw e;
}



ImageLaplacian_Gaussian_I::~ImageLaplacian_Gaussian_I(void)
{
	if (iL_ != NULL)
		delete []iL_;

}


const ImageLaplacian_Gaussian_I& ImageLaplacian_Gaussian_I::operator = (const ImageLaplacian_Gaussian_I& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageLaplacian_Gaussian_I copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters
//------------------------------------------------------
#endif


float ImageLaplacian_Gaussian_I::getScale(void) const
{
    return static_cast<float>(scale_);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif



RasterImage* ImageLaplacian_Gaussian_I::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    return applyTo(imgIn, imgIn->getValidRect());
}

RasterImage* ImageLaplacian_Gaussian_I::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

	ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
	usableRect->removeAllAround(1);
	ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle");

    RasterImage	*imgOut;
    if (imgIn->hasFloatRaster() && imgIn->floatRasterIsReference())
    	imgOut = new RasterImage_gray_F(destRect);
    else	
    	imgOut = new RasterImage_gray(destRect);

    applyInto(imgIn, theRect, imgOut);

	delete destRect;
	delete usableRect;

	return imgOut;
}

void ImageLaplacian_Gaussian_I::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}

void ImageLaplacian_Gaussian_I::applyInto(const RasterImage* imgIn, const ImageRect* theRect,
                                            RasterImage* imgOut)
{
        FAIL_CONDITION(	true,
        				kFunctionNotImplemented,
        				"not implemented yet");
}

#if 0
#pragma mark -
#endif


void ImageLaplacian_Gaussian_I::initializeFilter_(void)
{
}


