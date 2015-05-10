/*  NAME:
        StereoDisparity.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib StereoDisparity class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "StereoDisparity_F.h"

using namespace uriVL;

StereoDisparity_F::StereoDisparity_F(void) 
	:	StereoDisparity(),
	    Disparity_F()
	    
{
}

StereoDisparity_F::StereoDisparity_F(ImagePoint_F *disp, float conf, bool l2r)
	: StereoDisparity(l2r),
	  Disparity_F(disp, conf)
{
}

StereoDisparity_F::~StereoDisparity_F(void)
{
}
