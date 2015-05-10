/*  NAME:
        ColorMedianFilter.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorMedianFilter class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColorMedianFilter.h"

using namespace uriVL;

ColorMedianFilter::ColorMedianFilter(void)
			:	ImageOperator()
{
}


ColorMedianFilter::ColorMedianFilter(int theWidth, int theHeight, bool isIntOperator, ConversionFromFloatMode mode)
		try	:	ImageOperator(-(theWidth-1)/2, -(theHeight-1)/2, theWidth, theHeight, 
							  isIntOperator, mode)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorMedianFilter constructor");
	throw e;
}

ColorMedianFilter::ColorMedianFilter(const ImageRect* theRect, bool isIntOperator, ConversionFromFloatMode mode)
		try	:	ImageOperator(theRect, isIntOperator, mode)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorMedianFilter constructor");
	throw e;
}



ColorMedianFilter::~ColorMedianFilter(void)
{}
