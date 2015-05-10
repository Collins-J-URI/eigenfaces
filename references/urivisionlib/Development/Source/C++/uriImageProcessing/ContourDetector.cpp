/*  NAME:
        ContourDetector.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ContourDetector class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ContourDetector.h"

using namespace uriVL;

ContourDetector::ContourDetector(void)
	:	FeatureDetector()
{
}

ContourDetector::ContourDetector(const ImageRect* rect)
	:	FeatureDetector(rect)
{
}



ContourDetector::ContourDetector(const ContourDetector& theObj)
	:	FeatureDetector()
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ContourDetector copy constructor not implemented.");
}



const ContourDetector& ContourDetector::operator = (const ContourDetector& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ContourDetector copy operator not implemented.");

	return *this;
}
