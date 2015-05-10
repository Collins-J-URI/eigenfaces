/*  NAME:
        RectifiedStereoMatcher.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RectifiedStereoMatcher class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include    "RectifiedStereoMatcher.h"
using namespace uriVL;

//#error Work in progress: do not use this class yet

RectifiedStereoMatcher::RectifiedStereoMatcher(void)
        :	StereoMatcher()
{
}

RectifiedStereoMatcher::RectifiedStereoMatcher(RectifiedStereoPair *stereoPair)
        :	StereoMatcher(stereoPair)
{
}


RectifiedStereoMatcher::~RectifiedStereoMatcher(void)
{
}

void RectifiedStereoMatcher::setStereoPair(StereoPair *stereoPair)
{
	FAIL_CONDITION( stereoPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to StereoMatcher::setStereoPair");
	FAIL_CONDITION( !stereoPair->isRectified(),
                    kInvalidParameterError,
                    "RectifiedStereoMatcher::setStereoPair only accepts rrectified stereo pairs");
    
	stereoPair_ = stereoPair;
}


void RectifiedStereoMatcher::setMaxDisparity(int maxDisparity)
{
	maxDisparity_ = maxDisparity;
}



#if 0
#pragma mark -
#endif

//-------------------------------------------------------
//	Vector Field matching functions
//-------------------------------------------------------

/*
void RectifiedStereoMatcher::match(StereoPair *imgPair, StereoImageID l_or_r, 
            					VectorField *matchOut, ConfidenceRecord *conf) 
{
	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(!imgPair->isRectified(),
					kStereoMatchingError,
					"RectifiedStereoMatcher requires a recrtified stereo pair");
	FAIL_CONDITION(conf == NULL,
                    kNullParameterError,
                    "NULL parameter passed to RectifiedStereoMatcher::match");
	FAIL_CONDITION(	matchOut == NULL,
                    kNullVectorFieldError,
					"NULL VectorField passed as parameter to RectifiedStereoMatcher::match");
	ImageRect	*imgRect = imgPair->getValidRect(l_or_r);
    FAIL_CONDITION( imgRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in RectifiedStereoMatcher::match");
	match_((RectifiedStereoPair *) imgPair, l_or_r, imgRect, matchOut, conf);
}


void RectifiedStereoMatcher::match(RectifiedStereoPair *imgPair, StereoImageID l_or_r, 
            					VectorField *matchOut, ConfidenceRecord *conf)
{
	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to RectifiedStereoMatcher::match");
	
	FAIL_CONDITION(	matchOut == NULL,
                    kNullVectorFieldError,
					"NULL VectorField passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(	conf == NULL,
                    kNullParameterError,
                    "NULL parameter passed to RectifiedStereoMatcher::match");
	ImageRect* matchRect = imgPair->getValidRect(l_or_r);
	match_(imgPair, l_or_r, matchRect, matchOut, conf);
}

//-------

void RectifiedStereoMatcher::match(StereoPair *imgPair, StereoImageID l_or_r, ImageRect* rect, 
            					VectorField *matchOut, ConfidenceRecord *conf)
{
	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(!imgPair->isRectified(),
					kStereoMatchingError,
					"RectifiedStereoMatcher requires a recrtified stereo pair");
	FAIL_CONDITION( rect == NULL,
                    kNullRectangleError,
                    "NULL rectangle passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(	matchOut == NULL,
                    kNullVectorFieldError,
					"NULL VectorField passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(conf == NULL,
                    kNullParameterError,
                    "NULL parameter passed to RectifiedStereoMatcher::match");

	match_((RectifiedStereoPair *) imgPair, l_or_r, rect, matchOut, conf);
}            					

void RectifiedStereoMatcher::match(RectifiedStereoPair *imgPair, StereoImageID l_or_r, ImageRect* rect, 
            					VectorField *matchOut, ConfidenceRecord *conf)
{
	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION( rect == NULL,
                    kNullRectangleError,
                    "NULL rectangle passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(	matchOut == NULL,
                    kNullVectorFieldError,
                    "NULL VectorField passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(	conf == NULL,
                    kNullParameterError,
                    "NULL parameter passed to RectifiedStereoMatcher::match");
	match_(imgPair, l_or_r, rect, matchOut, conf);
}            					

//--------


VectorField *RectifiedStereoMatcher::match(StereoPair *imgPair, StereoImageID l_or_r, 
            							ConfidenceRecord *conf)
{
	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
   					"NULL stereo pair passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(	conf == NULL,
                    kNullParameterError,
                    "NULL parameter passed to RectifiedStereoMatcher::match");
	FAIL_CONDITION(!imgPair->isRectified(),
					kStereoMatchingError,
					"RectifiedStereoMatcher requires a recrtified stereo pair");

	ImageRect* matchRect = imgPair->getValidRect(l_or_r);
	return match((RectifiedStereoPair *) imgPair, l_or_r, matchRect, conf);
}            							

VectorField *RectifiedStereoMatcher::match(RectifiedStereoPair *imgPair, StereoImageID l_or_r, 
            							ConfidenceRecord *conf)
{
	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
   					"NULL stereo pair passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(	conf == NULL,
                    kNullParameterError,
                    "NULL parameter passed to RectifiedStereoMatcher::match");

	ImageRect* matchRect = imgPair->getValidRect(l_or_r);
	return match(imgPair, l_or_r, matchRect, conf);
}            							

 							
void RectifiedStereoMatcher::match(RectifiedStereoPair *imgPair, StereoImageID l_or_r, 
            			RasterImage_gray* disparityMap, ConfidenceRecord *conf)
{

	FAIL_CONDITION(	imgPair == NULL,
                    kNullParameterError,
                    "NULL stereo pair passed as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION( disparityMap == NULL,
                    kNullRasterImageError,
                    "null disparityMap as parameter to RectifiedStereoMatcher::match");
	FAIL_CONDITION(	conf == NULL,
                    kNullParameterError,
                    "NULL parameter passed to RectifiedStereoMatcher::match");
	ImageRect	*imgRect = imgPair->getValidRect(l_or_r);
    FAIL_CONDITION( imgRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in RectifiedStereoMatcher::match");

	match_(imgPair, l_or_r, imgRect, disparityMap, conf);
}

*/

