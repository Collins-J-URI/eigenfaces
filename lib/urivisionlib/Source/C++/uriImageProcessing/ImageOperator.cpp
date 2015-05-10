/*  NAME:
        ImageOperator.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageOperator class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ImageOperator.h"

using namespace std;
using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ImageOperator::ImageOperator(bool isIntOperator, ConversionFromFloatMode mode)
			:	rect_(NULL),
				isIntegerOperator_(isIntOperator),
				floatConversionMode_(mode)
{
}


ImageOperator::ImageOperator(const ImageRect* theRect, bool isIntOperator, ConversionFromFloatMode mode)
		try	:	rect_(new ImageRect(theRect)),
				isIntegerOperator_(isIntOperator),
				floatConversionMode_(mode)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageOperator constructor");
	throw e;
}


ImageOperator::ImageOperator(int theLeft, int theTop, int theWidth, int theHeight,
							 bool isIntOperator, ConversionFromFloatMode mode)
		try	:	rect_(new ImageRect(theLeft, theTop, theWidth, theHeight)),
				isIntegerOperator_(isIntOperator),
				floatConversionMode_(mode)
{
    FAIL_CONDITION( (theLeft > theTop) || (theWidth < 0) || (theHeight < 0),
                    kInvalidParameterError,
                    "Invalid dimensions passed to ImageOperator constructor.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageOperator constructor");
	throw e;
}


ImageOperator::ImageOperator(const ImageOperator& theObj)
			:	rect_(NULL),
				isIntegerOperator_(true),
				floatConversionMode_(theObj.floatConversionMode_)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageOperator copy constructor not implemented.");
}


ImageOperator::~ImageOperator(void)
{
	delete rect_;
}


const ImageOperator& ImageOperator::operator = (const ImageOperator& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageOperator copy operator not implemented.");

	return *this;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters and setters
//------------------------------------------------------
#endif



bool ImageOperator::isIntegerOperator(void) const
{
    return isIntegerOperator_;
}

bool ImageOperator::isFloatOperator(void) const
{
    return !isIntegerOperator_;
}

void ImageOperator::setFloatConversionMode(ConversionFromFloatMode theMode)
{
    floatConversionMode_ = theMode;
}

ConversionFromFloatMode ImageOperator::getFloatConversionMode(void) const
{
    return floatConversionMode_;
}


void ImageOperator::setRect(int theLeft, int theTop, int theWidth, int theHeight)
{
	FAIL_CONDITION( (theWidth < 0) || (theHeight < 0),
					kImageProcessingError,
					"Error in the computation of MedianFilter_F::applyTo");

	if (rect_ == NULL)
			rect_ = new ImageRect(theLeft, theTop, theWidth, theHeight);
	else
		rect_->setRect(theLeft, theTop, theWidth, theHeight);
		
}

void ImageOperator::setRect(const ImageRect* theRect)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageOperator copy operator not implemented.");
}

const ImageRect* ImageOperator::getRect(void) const
{
	return rect_;
}

int ImageOperator::getWidth(void) const
{
    return rect_->getWidth();
}

int ImageOperator::getHeight(void) const
{
    return rect_->getHeight();
}

