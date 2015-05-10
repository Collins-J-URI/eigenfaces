/*  NAME:
        StereoPair.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib StereoPair class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "StereoPair.h"

using namespace uriVL;

StereoPair::StereoPair(bool isRect)
		:	    leftCamera_(NULL),
				rightCamera_(NULL),
				leftImg_(NULL),
				rightImg_(NULL),
				transf_(NULL),
				isRectified_(isRect)
{
}


StereoPair::StereoPair(RasterImage* leftImg, RasterImage* rightImg, 
                        RigidTransformation3D *transf, bool isRect)
		:	    leftCamera_(NULL),
				rightCamera_(NULL),
				leftImg_(leftImg),
				rightImg_(rightImg),
				transf_(transf),
				isRectified_(isRect)
{
    FAIL_CONDITION( true,
                    kComputerVisionError,
                    "function not implemented yet"); 
    //  The cameras are assumed to be orthographic
}

StereoPair::StereoPair(Camera *leftCamera, Camera *rightCamera, bool isRect)
		:	    leftCamera_(leftCamera),
				rightCamera_(rightCamera),
				leftImg_(NULL),
				rightImg_(NULL),
				transf_(NULL),
				isRectified_(isRect)
{
    FAIL_CONDITION( true,
                    kComputerVisionError,
                    "function not implemented yet"); 
}

StereoPair::StereoPair(RasterImage* leftImg, RasterImage* rightImg,  
						 Camera *leftCamera, Camera *rightCamera, bool isRect)
		:	    leftCamera_(leftCamera),
				rightCamera_(rightCamera),
				leftImg_(leftImg),
				rightImg_(rightImg),
				transf_(NULL),
				isRectified_(isRect)
{
    FAIL_CONDITION( true,
                    kComputerVisionError,
                    "function not implemented yet"); 
}
			
StereoPair::StereoPair(const StereoPair& obj)
		:	    leftCamera_(NULL),
				rightCamera_(NULL),
				leftImg_(NULL),
				rightImg_(NULL),
				transf_(NULL),
				isRectified_(false)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"StereoPair copy constructor not implemented");
}


StereoPair::~StereoPair(void)
{
}

const StereoPair& StereoPair::operator = (const StereoPair& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"StereoPair copy operator not implemented");
	return *this;
}


void StereoPair::setImages(RasterImage* leftImg, RasterImage* rightImg)
{
 	 FAIL_CONDITION( leftImg == NULL,
                    kNullRasterImageError,
                    "null left RasterImage as parameter in StereoPair::setImages");
	 FAIL_CONDITION( rightImg == NULL,
                    kNullRasterImageError,
                    "null right RasterImage as parameter in StereoPair::setImages");
     leftImg_ = leftImg;
     rightImg_ = rightImg;
   
}

RasterImage* StereoPair::getLeftImage(void)
{
	return leftImg_;
}

RasterImage* StereoPair::getRightImage(void)
{
	return rightImg_;
}

RasterImage* StereoPair::getImage(StereoImageID l_or_r)
{
	if (l_or_r == LEFT_IMAGE)
		return leftImg_;
	else
		return rightImg_;

}

const ImageRect* StereoPair::getValidRect(StereoImageID l_or_r)
{
	if (l_or_r == LEFT_IMAGE)
		return leftImg_->getValidRect();
	else
		return rightImg_->getValidRect();

}

Camera *StereoPair::getLeftCamera(void)
{
	return leftCamera_;
}
Camera *StereoPair::getRightCamera(void)
{
	return rightCamera_;
}

bool StereoPair::isRectified(void)
{
	return isRectified_;
}
