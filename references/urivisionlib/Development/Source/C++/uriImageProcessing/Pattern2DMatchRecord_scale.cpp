/*  NAME:
        Pattern2DMatchRecord_scale.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Pattern2DMatchRecord_scale class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Pattern2DMatchRecord_scale.h"

using namespace std;
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

Pattern2DMatchRecord_scale::Pattern2DMatchRecord_scale(const ImagePoint* bestMatch, float score, 
                                                       const ScaleParameter *scaleParam)
		:   Pattern2DMatchRecord(bestMatch, score)
{
	scaleParam_ = new ScaleParameter;
	scaleParam_->scaleFactor = scaleParam->scaleFactor;
}

Pattern2DMatchRecord_scale::~Pattern2DMatchRecord_scale(void)
{
	delete scaleParam_;
}

