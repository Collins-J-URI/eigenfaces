/*  NAME:
        GaussianFilter.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib GaussianFilter class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "RasterImage.h"
#include "GaussianFilter.h"

using namespace uriVL;


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


GaussianFilter::GaussianFilter(double theScale, bool isIntOperator)
		try	:	ImageOperator(isIntOperator, kSaturatedPositive),
				//
				boundRect_(NULL),
				tempRaster_(NULL),
				scale_(theScale)			
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian filter is 0.8");

    const int	m = ((int) (5*theScale))/2;
    const int width = 2*m+1;

    setRect(-m, -m, width, width);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter constructor");
	throw e;
}

GaussianFilter::GaussianFilter(int theWidth, bool isIntOperator)
		try	:	ImageOperator(-theWidth/2, -theWidth/2, 
							  2*(theWidth/2) + 1, 2*(theWidth/2) + 1, isIntOperator, 
							  kSaturatedPositive),
				boundRect_(NULL),
				tempRaster_(NULL),
				scale_(0.2L*(2*(theWidth/2) + 1))
{
    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest theWidth admissible for a G filter is 3");

}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter constructor");
	throw e;
}


GaussianFilter::GaussianFilter(double theScale, const ImageRect* theRect, bool isIntOperator)
		try	:	ImageOperator(isIntOperator, kSaturatedPositive),
				//
				boundRect_(new ImageRect(theRect)),
				tempRaster_(new RasterImage_gray_F(theRect)),
				scale_(theScale)
{
    if (theScale < 0.8f)
	{
		delete boundRect_;
		delete tempRaster_;
		
		FAIL_CONDITION( true,
						kFilterAllocationError,
						"The smallest scale admissible for a G filter is 0.8");
	}
	
    int	m = ((int) (5*theScale))/2;
    int width = 2*m+1;

	try {
		setRect(-m, -m, width, width);
    }
	catch (ErrorReport& e) {
		e.appendToMessage("called by GaussianFilter constructor");
		throw e;
	}
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter constructor");
	throw e;
}

GaussianFilter::GaussianFilter(int theWidth, const ImageRect* theRect, bool isIntOperator)
		try	:	ImageOperator(-theWidth/2, -theWidth/2, 
							  2*(theWidth/2) + 1, 2*(theWidth/2) + 1, isIntOperator, kSaturatedPositive),
				boundRect_(new ImageRect(theRect)),
				tempRaster_(new RasterImage_gray_F(theRect)),
				scale_(0.2L*(2*(theWidth/2) + 1))
{

}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter constructor");
	throw e;
}


GaussianFilter::GaussianFilter(const GaussianFilter& theObj)
			:	ImageOperator(theObj),
				//
				boundRect_(NULL),
				tempRaster_(NULL),
				scale_(0)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "GaussianFilter copy constructor not implemented.");
}



GaussianFilter::~GaussianFilter(void)
{
    if (boundRect_ != NULL)
        delete boundRect_;
    if (tempRaster_ != NULL)
        delete tempRaster_;

}

const GaussianFilter& GaussianFilter::operator = (const GaussianFilter& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "GaussianFilter copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters
//------------------------------------------------------
#endif


float GaussianFilter::getScale(void) const
{
    return static_cast<float>(scale_);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Temp raster
//------------------------------------------------------
#endif

bool GaussianFilter::allocateTempRaster_(const ImageRect* theRect)
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

void GaussianFilter::freeTempRaster_(void)
{
	if (tempRaster_ != NULL) {
		delete tempRaster_;
		tempRaster_ = NULL;
	}
}

float* const* GaussianFilter::getShiftedTempGrayF2D_(ReadWriteStatus rw)
{
	return tempRaster_->getShiftedGrayF2D(R_W_ACCESS);
}


