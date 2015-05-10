/*  NAME:
        StereoMatcher.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib StereoMatcher class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include    "StereoMatcher.h"

using namespace uriVL;

StereoMatcher::StereoMatcher(void)
		:	matcher_(NULL),
			stereoPair_(NULL),
			searchRect_(new ImageRect(0, 0, 0, 0)),
			matchRecord_(new StereoMatchRecord()),
			isSubpixelMatcher_(false)
{
}

StereoMatcher::StereoMatcher(const StereoPair *stereoPair)
		:	matcher_(NULL),
			stereoPair_(stereoPair_),
			searchRect_(new ImageRect(0, 0, 0, 0)),
			matchRecord_(new StereoMatchRecord()),
			isSubpixelMatcher_(false)
{
}

StereoMatcher::StereoMatcher(const StereoPair& obj)
		:	matcher_(NULL),
			stereoPair_(NULL),
			searchRect_(NULL),
			matchRecord_(NULL),
			isSubpixelMatcher_(false)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"StereoPair copy constructor not implemented");
}

StereoMatcher::~StereoMatcher(void)
{    
    delete matchRecord_;
    delete searchRect_;
}


const StereoMatcher& StereoMatcher::operator = (const StereoMatcher& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"StereoMatcher copy operator not implemented");
	return *this;
}
#if 0
#pragma mark -
#endif
//------------------------------------------------------
//	point matching functions
//------------------------------------------------------
void StereoMatcher::setStereoPair(StereoPair *stereoPair)
{
	FAIL_CONDITION( stereoPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to StereoMatcher::setStereoPair");
	stereoPair_ = stereoPair;
}


bool StereoMatcher::isSubpixelMatcher(void)
{
	return isSubpixelMatcher_;
}

#if 0
#pragma mark -
#endif
//------------------------------------------------------
//	point matching functions
//------------------------------------------------------

StereoMatchRecord *StereoMatcher::match(StereoPair *stereoPair, ImagePoint* pt, StereoImageID l_or_r, 
						         	    const ImageRect* searchRect)
{
	match(stereoPair, pt, l_or_r, matchRecord_, searchRect);
	return new StereoMatchRecord(matchRecord_);
}

StereoMatchRecord *StereoMatcher::match(ImagePoint* pt, StereoImageID l_or_r,  
                                        const ImageRect* searchRect)
{
	match(stereoPair_, pt, l_or_r, matchRecord_, searchRect);
	return new StereoMatchRecord(matchRecord_);
}

void StereoMatcher::match(ImagePoint* pt, StereoImageID l_or_r, 
				          StereoMatchRecord *matchRec, const ImageRect* searchRect)
{
	match(stereoPair_, pt, l_or_r, matchRec, searchRect);    
}



void StereoMatcher::match(StereoPair *stereoPair, ImagePoint* pt, StereoImageID l_or_r, 
				StereoMatchRecord *matchRec, const ImageRect* searchRect)
{
	FAIL_CONDITION( stereoPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to StereoMatcher::match");
	FAIL_CONDITION( pt == NULL,
                    kNullImagePointError,
                    "NULL ImagePoint passed as parameter to StereoMatcher::match");
	FAIL_CONDITION( matchRec == NULL,
                    kNullParameterError,
                    "NULL StereoMatchRecord passed as parameter to StereoMatcher::match");

    ImageRect   *theRect;
    if (searchRect == NULL)
        theRect = new ImageRect(stereoPair->getValidRect(l_or_r));
    else
        theRect=  new ImageRect(searchRect);
        
	computeSearchRect_(pt, stereoPair->getValidRect(l_or_r), l_or_r, theRect);
	
	if ((theRect->getWidth()>0) && (theRect->getHeight()>0))
    	match_(stereoPair, pt, l_or_r, matchRec, theRect);

	//  if the search rect is empty (the point is outside of the matchable area for
	//  the algorithm used) we simply set confidence to 0
    else
    {
        matchRec->setBestMatch(pt->getX(), pt->getY());
        matchRec->setScore(0);
    }

    delete theRect;
}


StereoDisparity *StereoMatcher::disparity(StereoPair *stereoPair, ImagePoint* pt, 
                                          StereoImageID l_or_r, const ImageRect* searchRect)
{
	match(stereoPair, pt, l_or_r, matchRecord_, searchRect);
	ImagePoint  *bestMatch = matchRecord_->getBestMatch();
	return new StereoDisparity( bestMatch->getX() - pt->getX(),
	                            bestMatch->getY() - pt->getY(),
	                            matchRecord_->getScore(),
	                            l_or_r);
}

StereoDisparity *StereoMatcher::disparity(ImagePoint* pt, StereoImageID l_or_r,  
                                          const ImageRect* searchRect)
{
	match(stereoPair_, pt, l_or_r, matchRecord_, searchRect);
	ImagePoint  *bestMatch = matchRecord_->getBestMatch();
	return new StereoDisparity( bestMatch->getX() - pt->getX(),
	                            bestMatch->getY() - pt->getY(),
	                            matchRecord_->getScore(),
	                            l_or_r);
}

void StereoMatcher::disparity(ImagePoint* pt, StereoImageID l_or_r, 
				          StereoDisparity *disp, const ImageRect* searchRect)
{
	FAIL_CONDITION( disp == NULL,
                    kNullImagePointError,
                    "NULL ImagePoint passed as parameter to StereoMatcher::match");
	match(stereoPair_, pt, l_or_r, matchRecord_, searchRect);
	ImagePoint  *bestMatch = matchRecord_->getBestMatch();
    disp->setDisparity(bestMatch->getX() - pt->getX(),
	                   bestMatch->getY() - pt->getY());
	disp->setScore(matchRecord_->getScore());
	disp->setIsSubpixelDisparity(false);                   
}



void StereoMatcher::disparity(StereoPair *stereoPair, ImagePoint* pt, StereoImageID l_or_r, 
				              StereoDisparity *disp, const ImageRect* searchRect)
{
	FAIL_CONDITION( disp == NULL,
                    kNullImagePointError,
                    "NULL ImagePoint passed as parameter to StereoMatcher::match");
	match(stereoPair, pt, l_or_r, matchRecord_, searchRect);
	ImagePoint  *bestMatch = matchRecord_->getBestMatch();
    disp->setDisparity(bestMatch->getX() - pt->getX(),
	                   bestMatch->getY() - pt->getY());
	disp->setScore(matchRecord_->getScore());
	disp->setIsSubpixelDisparity(false);                   
}

#if 0
#pragma mark -
#endif
//------------------------------------------------------
//	vector field matching functions
//------------------------------------------------------

/*
VectorField *StereoMatcher::match(StereoPair *imgPair, StereoImageID l_or_r, 
							ConfidenceRecord *conf)
{
	//	test imgPair not null
	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to StereoMatcher::match");
	FAIL_CONDITION(conf == NULL,
                    kNullParameterError,
                    "NULL parameter passed to StereoMatcher::match");
	ImageRect* matchRect = imgPair->getValidRect(l_or_r);
	return match(imgPair, l_or_r, matchRect, conf);
}
							
VectorField *StereoMatcher::match(StereoPair *imgPair, StereoImageID l_or_r, 
							ImageRect* rect, ConfidenceRecord *conf)
{
	//	test imgPair not null
	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to StereoMatcher::match");
    FAIL_CONDITION( rect == NULL,
                    kNullRectangleError,
                    "NULL rectangle passed as parameter to StereoMatcher::match");
	FAIL_CONDITION(conf == NULL,
                    kNullParameterError,
                    "NULL confidence parameter passed to StereoMatcher::match");
	ImageRect* validRect = imgPair->getValidRect(l_or_r);
	ImageRect* matchRect = ImageRect::intersection(validRect, rect);
    FAIL_CONDITION( matchRect == NULL,
                    kNullRectangleError,
                    "invalid match rectangle in StereoMatcher::match");

	VectorField	*dispField;
	if (isSubpixelMatcher())
		dispField = new VectorField_F(matchRect);
	else
		dispField = new VectorField_I(matchRect);
	
	//	call some match function
	match_(imgPair, l_or_r, matchRect, dispField, conf);
	delete matchRect;
	return dispField;
}

void StereoMatcher::match(StereoPair *imgPair, StereoImageID l_or_r, 
					VectorField *matchOut, ConfidenceRecord *conf)
{
	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to StereoMatcher::match");
	FAIL_CONDITION(	matchOut == NULL,
                    kNullVectorFieldError,
                    "NULL vector field passed as parameter to StereoMatcher::match");
	FAIL_CONDITION(conf == NULL,
                    kNullParameterError,
                    "NULL confidence parameter passed to StereoMatcher::match");

	ImageRect	*searchRect;
	RasterImage	*img;
	
	if (l_or_r == LEFT_IMAGE) 
		img = imgPair->getLeftImage();
	else
		img = imgPair->getRightImage();
	
	searchRect = img->getValidRect();
	
	match_(imgPair, l_or_r, searchRect, matchOut, conf);
}

*/
