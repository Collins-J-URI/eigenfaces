/*  NAME:
        ImageGradient_Gaussian_I.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageGradient_Gaussian_I class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ImageGradient_Gaussian_I.h"

using namespace uriVL;


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ImageGradient_Gaussian_I::ImageGradient_Gaussian_I(double theScale)
		try	:	ImageGradient(2*(((int ) (5*theScale))/2) + 1, true),
				//
				scale_(theScale),
				ig_(NULL),
				igD_(NULL),
				iDenum_(1),				
				iDevDenum_(1)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient_Gaussian_I constructor not implemented.");

    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian Gradient operator is 0.8");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Gaussian_I constructor");
	throw e;
}



ImageGradient_Gaussian_I::ImageGradient_Gaussian_I(double theScale, const ImageRect*  theRect)
		try	:	ImageGradient(2*(((int ) (5*theScale))/2) + 1, false),
				//
				scale_(theScale),
				ig_(NULL),
				igD_(NULL),
				iDenum_(1),			
				iDevDenum_(1)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient_Gaussian_I constructor not implemented.");

    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a G Gradient filter is 0.8");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Gaussian_I constructor");
	throw e;
}



ImageGradient_Gaussian_I::ImageGradient_Gaussian_I(int theWidth)
		try	:	ImageGradient(theWidth, false),
				//
				scale_(0.2L*getWidth()),
				ig_(NULL),
				igD_(NULL),
				iDenum_(1),			
				iDevDenum_(1)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient_Gaussian_I constructor not implemented.");

    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest width admissible for a G gradient filter is 3");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Gaussian_I constructor");
	throw e;
}


			
ImageGradient_Gaussian_I::ImageGradient_Gaussian_I(int  theWidth, const ImageRect*  theRect)
		try	:	ImageGradient(theWidth, theRect, false),
				//
				scale_(0.2L*getWidth()),
				ig_(NULL),
				igD_(NULL),
				iDenum_(1),			
				iDevDenum_(1)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient_Gaussian_I constructor not implemented.");

    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest width admissible for a G Gradient filter is 3");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Gaussian_I constructor");
	throw e;
}


ImageGradient_Gaussian_I::ImageGradient_Gaussian_I(const ImageGradient_Gaussian_I& theObj)
		try	:	ImageGradient(theObj),
				//
				scale_(theObj.scale_),
				ig_(NULL),
				igD_(NULL),
				iDenum_(1),			
				iDevDenum_(1)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient_Gaussian_I copy constructor not implemented.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Gaussian_I constructor");
	throw e;
}


ImageGradient_Gaussian_I::~ImageGradient_Gaussian_I(void)
{
    delete []ig_;
    delete []igD_;
}


const ImageGradient_Gaussian_I& ImageGradient_Gaussian_I::operator = (const ImageGradient_Gaussian_I& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient_Gaussian_I copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters
//------------------------------------------------------
#endif


float ImageGradient_Gaussian_I::getScale(void) const
{
    return static_cast<float>(scale_);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


//VectorField* ImageGradient_Gaussian_I::applyTo(RasterImage* imgIn)
//{
//    FAIL_CONDITION(	imgIn == NULL,
//    				kNullRasterImageError,
//                    "null RasterImage passed as input to ImageGradient_Gaussian_I::applyInto");
//	return applyTo(imgIn, imgIn->getValidRect());
//}
//
//VectorField* ImageGradient_Gaussian_I::applyTo(RasterImage* imgIn, const ImageRect* destRect)
//{
//    FAIL_CONDITION(	imgIn == NULL,
//    				kNullRasterImageError,
//                    "null RasterImage passed as input to ImageGradient_Gaussian_I::applyInto");
//
//    FAIL_CONDITION( destRect == NULL, 
//                    kNullRectangleError, 
//                    "NULL ImageRect parameter in ImageGradient_Gaussian_I::applyInto");
//
//    FAIL_CONDITION(	true,
//    				kFunctionNotImplemented,
//    				"not implemented yet");
//
//	VectorField	*vectOut = NULL;
//	
//	return vectOut;
//}
//
//void ImageGradient_Gaussian_I::applyInto(RasterImage* imgIn, VectorField* vectOut)
//{
//    FAIL_CONDITION(	imgIn == NULL,
//    				kNullRasterImageError,
//    				"NULL input raster image passed as parameter");
//    applyInto(imgIn, imgIn->getValidRect(), vectOut);
//}
//
//void ImageGradient_Gaussian_I::applyInto(RasterImage* imgIn, const ImageRect* destRect, 
//                                        VectorField* vectOut)
//{
//    FAIL_CONDITION(	imgIn == NULL,
//    				kNullRasterImageError,
//    				"NULL input raster image passed as parameter");
//
//    FAIL_CONDITION( destRect == NULL, 
//                    kNullRectangleError, 
//                    "NULL ImageRect parameter in VectorField_F::copyInto");
//
//    FAIL_CONDITION(	vectOut == NULL,
//    				kNullVectorFieldError,
//    				"NULL output vector field passed as parameter");
//
//    FAIL_CONDITION(	true,
//    				kFunctionNotImplemented,
//    				"not implemented yet");
//
//    vectOut->setValidRect(destRect);
//	vectOut->setRasterAsReference();
//	vectOut->forceRasterUpdate();
//}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Initialization
//------------------------------------------------------
#endif


void ImageGradient_Gaussian_I::initializeFilter_(void)
{
    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

//    const int  m = getWidth()/2;
//    const double	sigma2Scale = 1.L / (2.L * scale_ * scale_),
//					piSigma = 1.L / (sqrt(2*PI) * scale_),
//					piSigma2 = piSigma/scale_;
//
//    ig_ = new int [m+1];
//    FAIL_CONDITION( ig_ == NULL,
//                    kFilterAllocationError,
//                    "filter allocation failed in ImageGradient_Gaussian_I::initializeFilter_");
//    igD_ = new int [m+1];
//    FAIL_CONDITION( igD_ == NULL,
//                    kFilterAllocationError,
//                    "filter allocation failed in ImageGradient_Gaussian_I::initializeFilter_");
//
}