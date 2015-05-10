/*  NAME:
        ImageGradient.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageGradient class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ImageGradient.h"

using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ImageGradient::ImageGradient(int theWidth, bool isIntOperator)
		try	:	VectorOperator(-theWidth/2, -theWidth/2, 
							  2*(theWidth/2) + 1, 2*(theWidth/2) + 1, isIntOperator),
				boundRect_(NULL),
				tempRaster_(NULL)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient constructor");
	throw e;
}



ImageGradient::ImageGradient(int theWidth, const ImageRect* theRect, bool isIntOperator)
		try	:	VectorOperator(-theWidth/2, -theWidth/2, 
							  2*(theWidth/2) + 1, 2*(theWidth/2) + 1, isIntOperator),
				boundRect_(new ImageRect(theRect)),
				tempRaster_(new RasterImage_gray_F(theRect))
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient constructor");
	throw e;
}


ImageGradient::ImageGradient(const ImageGradient& theObj)
			:	VectorOperator(theObj),
				//
				boundRect_(NULL),
				tempRaster_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient copy constructor not implemented.");
}


ImageGradient::~ImageGradient(void)
{
   if (boundRect_ != NULL)
        delete boundRect_;
    if (tempRaster_ != NULL)
        delete tempRaster_;
}

const ImageGradient& ImageGradient::operator = (const ImageGradient& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Temp raster
//------------------------------------------------------
#endif

bool ImageGradient::allocateTempRaster_(const ImageRect* theRect)
{	
	bool newTempRaster = (tempRaster_ == NULL);
    if (newTempRaster) {
    	tempRaster_ = new RasterImage_gray_F(theRect);
	}
	//	Check if the user is a bonehead
    else if (!tempRaster_->contains(theRect)) {
		tempRaster_->setBoundRect(theRect);
		newTempRaster = true;
	}
	
	return newTempRaster;
}

void ImageGradient::freeTempRaster_(void)
{
	if (tempRaster_ != NULL) {
		delete tempRaster_;
		tempRaster_ = NULL;
	}
}

float* const* ImageGradient::getShiftedTempGrayF2D_(ReadWriteStatus rw)
{
	return tempRaster_->getShiftedGrayF2D(R_W_ACCESS);
}


