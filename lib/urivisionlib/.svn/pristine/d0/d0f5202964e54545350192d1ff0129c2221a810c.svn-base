/*  NAME:
        RectifiedStereoDisparity.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RectifiedStereoDisparity class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "RectifiedStereoDisparity.h"

#error Work in progress: do not use this class yet

using namespace uriVL;

RectifiedStereoDisparity::RectifiedStereoDisparity(float d, float conf, bool l2r)
    :   Disparity(),
        StereoDisparity(),
        Disparity_F(),
		StereoDisparity_F(),
		RectifiedStereoDisparity()
{
	fd_ = d;
	confidence_ = conf;
	isLeftToRight_ = l2r;
}

RectifiedStereoDisparity::RectifiedStereoDisparity(RectifiedStereoDisparity *disp)
	:	StereoDisparity(disp)
{
	d_ = disp->d_;
}

RectifiedStereoDisparity::~RectifiedStereoDisparity_F(void)
{
}

float RectifiedStereoDisparity::getDisparity1DF(void)
{
	return fd_;
}

void RectifiedStereoDisparity::setDisparity(float d)
{
    fd_ = d;
    d_ = (int) (d + 0.5f);
	disparity_->setCoordinates(d_, 0);
	disparityF_->setCoordinates(d, 0);
}


