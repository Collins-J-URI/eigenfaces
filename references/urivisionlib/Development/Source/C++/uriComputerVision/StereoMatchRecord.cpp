/*  NAME:
        StereoMatchRecord.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib StereoMatchRecord class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "StereoMatchRecord.h"

using namespace std;
using namespace uriVL;


StereoMatchRecord::StereoMatchRecord(void)
		:	bestMatch_(new ImagePoint(0, 0)),
			score_(0.f),
			isSubpixelMatch_(false),
			isLeftMatch_(false),
			isRectifiedStereoMatch_(false)
{
}

StereoMatchRecord::StereoMatchRecord(StereoMatchRecord *disp)
		:	bestMatch_(new ImagePoint(disp->bestMatch_)),
			score_(disp->score_),
			isSubpixelMatch_(disp->isSubpixelMatch_),
			isLeftMatch_(disp->isLeftMatch_),
			isRectifiedStereoMatch_(disp->isRectifiedStereoMatch_)
{
}

StereoMatchRecord::StereoMatchRecord(ImagePoint* bestMatch, float score, bool isLeft)
		:	bestMatch_(new ImagePoint(bestMatch)),
			score_(score),
			isSubpixelMatch_(false),
			isLeftMatch_(isLeft),
			isRectifiedStereoMatch_(false)
{
}

StereoMatchRecord::StereoMatchRecord(int x, int y, float score, bool isLeft)
		:	bestMatch_(new ImagePoint(x, y)),
			score_(score),
			isSubpixelMatch_(false),
			isLeftMatch_(isLeft),
			isRectifiedStereoMatch_(false)
{
}


StereoMatchRecord::StereoMatchRecord(const StereoMatchRecord& obj)
		:	bestMatch_(NULL),
			score_(0.f),
			isSubpixelMatch_(false),
			isLeftMatch_(false),
			isRectifiedStereoMatch_(false)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"StereoMatchRecord copy constructor not implemented");
}


StereoMatchRecord::~StereoMatchRecord(void)
{
	delete bestMatch_;
}

const StereoMatchRecord& StereoMatchRecord::operator = (const StereoMatchRecord& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"StereoMatchRecord copy operator not implemented");
	return *this;
}



//----------------------------------------------------------
//  public funcs
//----------------------------------------------------------

float StereoMatchRecord::getScore(void)
{
    return score_;
    
}

ImagePoint* StereoMatchRecord::getBestMatch(void)
{
    return bestMatch_;
}

void StereoMatchRecord::setScore(float score)
{
    score_ = score;
    
}

void StereoMatchRecord::setBestMatch(int x, int y)
{
    if (bestMatch_ == NULL)
        bestMatch_ = new ImagePoint(x, y);
    else
        bestMatch_->setCoordinates(x, y);
        
}

void StereoMatchRecord::setBestMatch(ImagePoint* bestMatch)
{
    FAIL_CONDITION( bestMatch == NULL,
                    kNullImagePointError,
                    "null ImagePoint passed to StereoMatchRecord::setBestMatch");
                    
    if (bestMatch_ == NULL)
        bestMatch_ = new ImagePoint(bestMatch);
    else
        bestMatch_->setCoordinates(bestMatch->getX(), bestMatch->getY());
        
}

bool StereoMatchRecord::isSubpixelMatch(void)
{
    return isSubpixelMatch_;
}

void StereoMatchRecord::setIsSubpixelMatch(bool isSubpixel)
{
    isSubpixelMatch_ = isSubpixel;
}

bool StereoMatchRecord::isRectifiedStereoMatch(void)
{
    return isRectifiedStereoMatch_;
}

void StereoMatchRecord::setIsRectifiedStereoMatch(bool isRect)
{
    isRectifiedStereoMatch_ = isRect;
}

bool StereoMatchRecord::isLeftMatch(void)
{
    return isLeftMatch_;
}




//----------------------------------------------------------
//  protected funcs
//----------------------------------------------------------

