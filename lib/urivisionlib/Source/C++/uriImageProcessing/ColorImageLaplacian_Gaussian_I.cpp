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

#error Work in progress: do not use this class yet

using namespace uriVL;

ImageLaplacian_Gaussian_I::ImageLaplacian_Gaussian_I(double theScale)
	:	ImageLaplacian( )
{
	scale_ = (float)theScale;
	
	int	m = ((int) (5*theScale))/2;
	width_ = 2*m + 1;
	
	rect_ = new ImageRect(-m, -m, width_, width_);
	isIntegerOperator_ = true;
    initializeFilter_( );
}

ImageLaplacian_Gaussian_I::ImageLaplacian_Gaussian_I(int theWidth)
	:	ImageLaplacian( )
{
	scale_ = 0.2L*theWidth;

    int   m = theWidth/2;
	width_ = 2*m+1;
	
	rect_ = new ImageRect(-m, -m, width_, width_);
	isIntegerOperator_ = false;
    initializeFilter_( );
}
			
ImageLaplacian_Gaussian_I::~ImageLaplacian_Gaussian_I(void)
{
}

Box::Box(const Box& theBox)
		:	GraphicComponent3D(),
			//
			vertex_(NULL),
			normal_(NULL),
			closedBox_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Box copy constructor not implemented.");
}



const Box& Box::operator = (const Box& theBox)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Box copy operator not implemented.");

	return *this;
}

RasterImage* ImageLaplacian_Gaussian_I::applyTo(RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    return applyTo(imgIn, imgIn->getValidRect());
}

RasterImage* ImageLaplacian_Gaussian_I::applyTo(RasterImage* imgIn, ImageRect* theRect)
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

void ImageLaplacian_Gaussian_I::applyInto(RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}

void ImageLaplacian_Gaussian_I::applyInto(RasterImage* imgIn, ImageRect* theRect,
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


