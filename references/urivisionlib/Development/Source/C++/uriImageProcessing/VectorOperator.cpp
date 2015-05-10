/*  NAME:
        VectorOperator.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib VectorOperator class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "VectorOperator.h"

using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif
VectorOperator::VectorOperator(bool isIntOperator)
			:	rect_(NULL),
				isIntegerOperator_(isIntOperator)
{
}


VectorOperator::VectorOperator(const ImageRect* theRect, bool isIntOperator)
		try	:	rect_(new ImageRect(theRect)),
				isIntegerOperator_(isIntOperator)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by VectorOperator constructor");
	throw e;
}


VectorOperator::VectorOperator(int theLeft, int theTop, int theWidth, int theHeight,
							 bool isIntOperator)
		try	:	rect_(new ImageRect(theLeft, theTop, theWidth, theHeight)),
				isIntegerOperator_(isIntOperator)
{
    FAIL_CONDITION( (theLeft > theTop) || (theWidth < 0) || (theHeight < 0),
                    kInvalidParameterError,
                    "Invalid dimensions passed to VectorOperator constructor.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by VectorOperator constructor");
	throw e;
}


VectorOperator::VectorOperator(const VectorOperator& theObj)
			:	rect_(NULL),
				isIntegerOperator_(true)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "VectorOperator copy constructor not implemented.");
}


VectorOperator::~VectorOperator(void)
{
	delete rect_;
}


const VectorOperator& VectorOperator::operator = (const VectorOperator& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "VectorOperator copy operator not implemented.");

	return *this;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters and setters
//------------------------------------------------------
#endif



bool VectorOperator::isIntegerOperator(void) const
{
    return isIntegerOperator_;
}

bool VectorOperator::isFloatOperator(void) const
{
    return !isIntegerOperator_;
}

void VectorOperator::setRect(int theLeft, int theTop, int theWidth, int theHeight)
{
	FAIL_CONDITION( (theWidth < 0) || (theHeight < 0),
					kImageProcessingError,
					"Error in the computation of MedianFilter_F::applyTo");

	if (rect_ == NULL)
			rect_ = new ImageRect(theLeft, theTop, theWidth, theHeight);
	else
		rect_->setRect(theLeft, theTop, theWidth, theHeight);
		
}

void VectorOperator::setRect(const ImageRect* theRect)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "VectorOperator copy operator not implemented.");
}

const ImageRect* VectorOperator::getRect(void) const
{
	return rect_;
}

int VectorOperator::getWidth(void) const
{
    return rect_->getWidth();
}

int VectorOperator::getHeight(void) const
{
    return rect_->getHeight();
}

