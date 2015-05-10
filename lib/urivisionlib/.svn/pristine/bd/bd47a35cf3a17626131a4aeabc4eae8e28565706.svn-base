/*  NAME:
        ColorGaussianFilter.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorGaussianFilter class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColorGaussianFilter.h"

using namespace uriVL;


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ColorGaussianFilter::ColorGaussianFilter(double theScale, bool isIntOperator)
		try	:	ImageOperator(isIntOperator, kSaturatedPositive),
				//
				boundRect_(NULL),
				tempRaster_(NULL),
				scale_(theScale)			
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a color Gaussian filter is 0.8");

    const int	m = ((int) (5*theScale))/2;
    const int width = 2*m+1;

    setRect(-m, -m, width, width);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter constructor");
	throw e;
}


ColorGaussianFilter::ColorGaussianFilter(int theWidth, bool isIntOperator)
		try	:	ImageOperator(-theWidth/2, -theWidth/2, 
							  2*(theWidth/2) + 1, 2*(theWidth/2) + 1, isIntOperator, 
							  kSaturatedPositive),
				boundRect_(NULL),
				tempRaster_(NULL),
				scale_(0.2L*(2*(theWidth/2) + 1))
{
    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest theWidth admissible for a color Gaussian filter is 3");

}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter constructor");
	throw e;
}

ColorGaussianFilter::ColorGaussianFilter(double theScale, const ImageRect* theRect, bool isIntOperator)
		try	:	ImageOperator(isIntOperator, kSaturatedPositive),
				//
				boundRect_(new ImageRect(theRect)),
				tempRaster_(new RasterImage_RGBa_F(theRect)),
				scale_(theScale)
{
    if (theScale < 0.8f)
	{
		delete boundRect_;
		delete tempRaster_;
		
		FAIL_CONDITION( true,
						kFilterAllocationError,
						"The smallest theWidth admissible for a color Gaussian filter is 3");
	}
	
    int	m = ((int) (5*theScale))/2;
    int width = 2*m+1;

	try {
		setRect(-m, -m, width, width);
    }
	catch (ErrorReport& e) {
		e.appendToMessage("called by ColorGaussianFilter constructor");
		throw e;
	}
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter constructor");
	throw e;
}


ColorGaussianFilter::ColorGaussianFilter(int theWidth, const ImageRect* theRect, bool isIntOperator)
		try	:	ImageOperator(-theWidth/2, -theWidth/2, 
							  2*(theWidth/2) + 1, 2*(theWidth/2) + 1),
				boundRect_(new ImageRect(theRect)),
				tempRaster_(new RasterImage_RGBa_F(theRect)),
				scale_(0.2L*(2*(theWidth/2) + 1))
{

}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter constructor");
	throw e;
}


ColorGaussianFilter::ColorGaussianFilter(const ColorGaussianFilter& theObj)
			:	ImageOperator(theObj),
				//
				boundRect_(NULL),
				tempRaster_(NULL),
				scale_(0)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorGaussianFilter copy constructor not implemented.");
}



ColorGaussianFilter::~ColorGaussianFilter(void)
{
    if (boundRect_ != NULL)
        delete boundRect_;
    if (tempRaster_ != NULL)
        delete tempRaster_;

}

const ColorGaussianFilter& ColorGaussianFilter::operator = (const ColorGaussianFilter& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorGaussianFilter copy operator not implemented.");

	return *this;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark public getters
//------------------------------------------------------
#endif

int ColorGaussianFilter::getWidth(void) const
{
    return getRect()->getWidth();
}

int ColorGaussianFilter::getHeight(void) const
{
    return getRect()->getHeight();
}

float ColorGaussianFilter::getScale(void) const
{
    return static_cast<float>(scale_);
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Temp raster
//------------------------------------------------------
#endif

bool ColorGaussianFilter::allocateTempRaster_(const ImageRect* theRect)
{	
	bool newTempRaster = (tempRaster_ == NULL);
    if (newTempRaster) {
    	tempRaster_ = new RasterImage_RGBa_F(theRect);
	}
	//	Check if the user is a bonehead
    else if (!tempRaster_->contains(theRect)) {
		tempRaster_->setBoundRect(theRect);
		newTempRaster = true;
	}
	
	return newTempRaster;
}

void ColorGaussianFilter::freeTempRaster_(void)
{
	if (tempRaster_ != NULL) {
		delete tempRaster_;
		tempRaster_ = NULL;
	}
}

float* const* ColorGaussianFilter::getShiftedTempRedF2D_(ReadWriteStatus rw)
{
	return tempRaster_->getShiftedRedF2D(R_W_ACCESS);
}


float* const* ColorGaussianFilter::getShiftedTempGreenF2D_(ReadWriteStatus rw)
{
	return tempRaster_->getShiftedGreenF2D(R_W_ACCESS);
}


float* const* ColorGaussianFilter::getShiftedTempBlueF2D_(ReadWriteStatus rw)
{
	return tempRaster_->getShiftedBlueF2D(R_W_ACCESS);
}




