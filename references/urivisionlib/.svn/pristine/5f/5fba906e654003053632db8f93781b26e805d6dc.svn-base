/*  NAME:
        MedianFilter.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib MedianFilter class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "MedianFilter.h"

using namespace uriVL;

MedianFilter::MedianFilter(void)
			:	ImageOperator()
{
}


MedianFilter::MedianFilter(int theWidth, int theHeight, bool isIntOperator, ConversionFromFloatMode mode)
		try	:	ImageOperator(-(theWidth-1)/2, -(theHeight-1)/2, theWidth, theHeight, 
							  isIntOperator, mode)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by MedianFilter constructor");
	throw e;
}

MedianFilter::MedianFilter(const ImageRect* theRect, bool isIntOperator, ConversionFromFloatMode mode)
		try	:	ImageOperator(theRect, isIntOperator, mode)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by MedianFilter constructor");
	throw e;
}


MedianFilter::~MedianFilter(void)
{}
