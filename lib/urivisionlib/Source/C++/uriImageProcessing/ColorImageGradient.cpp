/*  NAME:
        ColorImageGradient.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorImageGradient class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColorImageGradient.h"

using namespace uriVL;


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ColorImageGradient::ColorImageGradient(int theWidth, bool isIntOperator,
										ColorGradientMode theMode)
		try	:	VectorOperator(-theWidth/2, -theWidth/2, 
							  2*(theWidth/2) + 1, 2*(theWidth/2) + 1, isIntOperator),
				//
				colorGradientMode_(theMode),
				scale_(0.2f * (2*(theWidth/2) + 1)),
				boundRect_(NULL),
				tempRaster_(NULL),
				altTempRaster_(NULL)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorImageGradient constructor");
	throw e;
}



ColorImageGradient::ColorImageGradient(int theWidth, const ImageRect* theRect, bool isIntOperator, 
										ColorGradientMode theMode)
		try	:	VectorOperator(-theWidth/2, -theWidth/2, 
							  2*(theWidth/2) + 1, 2*(theWidth/2) + 1, isIntOperator),
				colorGradientMode_(theMode),
				scale_(0.2f * (2*(theWidth/2) + 1)),
				boundRect_(new ImageRect(theRect)),
				tempRaster_(new RasterImage_RGBa_F(theRect)),
				altTempRaster_(new RasterImage_RGBa_F(theRect))
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorImageGradient constructor");
	throw e;
}


ColorImageGradient::ColorImageGradient(const ColorImageGradient& theObj)
			:	VectorOperator(),
				//
				colorGradientMode_(GRAD_AVE_OF_COLOR_COMPONENTS),
				scale_(0.f),
				boundRect_(NULL),
				tempRaster_(NULL),
				altTempRaster_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorImageGradient copy constructor not implemented.");
}


ColorImageGradient::~ColorImageGradient(void)
{
   if (boundRect_ != NULL)
        delete boundRect_;
    if (tempRaster_ != NULL)
        delete tempRaster_;
    if (altTempRaster_ != NULL)
        delete altTempRaster_;

}

const ColorImageGradient& ColorImageGradient::operator = (const ColorImageGradient& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorImageGradient copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark public getters
//------------------------------------------------------
#endif

int ColorImageGradient::getWidth(void) const
{
    return getRect()->getWidth();
}

int ColorImageGradient::getHeight(void) const
{
    return getRect()->getHeight();
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Other methods
//------------------------------------------------------
#endif

void ColorImageGradient::setColorGradientMode(ColorGradientMode colorGradientMode)
{
    colorGradientMode_ = colorGradientMode;
}

ColorGradientMode ColorImageGradient::getColorGradientMode(void) const
{
	return colorGradientMode_;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Temp raster
//------------------------------------------------------
#endif

bool ColorImageGradient::allocateTempRasters_(const ImageRect* theRect)
{	
	bool newTempRaster = (tempRaster_ == NULL);
    if (newTempRaster) {
    	tempRaster_ = new RasterImage_RGBa_F(theRect);
    	altTempRaster_ = new RasterImage_RGBa_F(theRect);
	}
	//	Check if the user is a bonehead
    else if (!tempRaster_->contains(theRect)) {
		tempRaster_->setBoundRect(theRect);
		altTempRaster_->setBoundRect(theRect);
		newTempRaster = true;
	}
	
	return newTempRaster;
}

void ColorImageGradient::freeTempRasters_(void)
{
	if (tempRaster_ != NULL) {
		delete tempRaster_;
		tempRaster_ = NULL;
	}
	if (altTempRaster_ != NULL) {
		delete altTempRaster_;
		altTempRaster_ = NULL;
	}
}

float* const* ColorImageGradient::getShiftedTempRedF2D_(ReadWriteStatus rw)
{
	return tempRaster_->getShiftedRedF2D(R_W_ACCESS);
}


float* const* ColorImageGradient::getShiftedTempGreenF2D_(ReadWriteStatus rw)
{
	return tempRaster_->getShiftedGreenF2D(R_W_ACCESS);
}


float* const* ColorImageGradient::getShiftedTempBlueF2D_(ReadWriteStatus rw)
{
	return tempRaster_->getShiftedBlueF2D(R_W_ACCESS);
}

float* const* ColorImageGradient::getShiftedAltTempRedF2D_(ReadWriteStatus rw)
{
	return altTempRaster_->getShiftedRedF2D(R_W_ACCESS);
}


float* const* ColorImageGradient::getShiftedAltTempGreenF2D_(ReadWriteStatus rw)
{
	return altTempRaster_->getShiftedGreenF2D(R_W_ACCESS);
}


float* const* ColorImageGradient::getShiftedAltTempBlueF2D_(ReadWriteStatus rw)
{
	return altTempRaster_->getShiftedBlueF2D(R_W_ACCESS);
}




