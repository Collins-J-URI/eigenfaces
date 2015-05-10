/*  NAME:
        RectifiedStereoMatcher_corr.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RectifiedStereoMatcher_corr class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "RectifiedStereoMatcher_corr.h"
#include "PatternMatcher2D_corr.h"
#include "ImageWriter.h"

//#error Work in progress: do not use this class yet

using namespace uriVL;

#define	SCORE_THRESHOLD		0.8f

RectifiedStereoMatcher_corr::RectifiedStereoMatcher_corr(BaseRasterType rasterType, int nbRowsS,
        								int nbColsS, int verticalPaddingS, int nbRowsM,
        								int nbColsM, int verticalPaddingM, int nbRowsL,
        								int nbColsL, int verticalPaddingL)
	:	RectifiedStereoMatcher()
{
	//	make sure that dimensions are odd numbers
	sWidth_ = 2*(nbColsS/2) +1;
	sHeight_ = 2*(nbRowsS/2) +1;

	sVerticalPadding_ = verticalPaddingS;	
	
	//	create the pattern we will use for the matching
	sPat_ = new Pattern2D(rasterType, sHeight_, sWidth_);
	sGrabRect_ = new ImageRect(0, 0, sWidth_, sHeight_);
	matcher_ = new PatternMatcher2D_corr();

	//	make sure that dimensions are odd numbers
	mWidth_ = 2*(nbColsM/2) +1;
	mHeight_ = 2*(nbRowsM/2) +1;

	mVerticalPadding_ = verticalPaddingM;	
	
	//	create the pattern we will use for the matching
	mPat_ = new Pattern2D(rasterType, mHeight_, mWidth_);
	mGrabRect_ = new ImageRect(0, 0, mWidth_, mHeight_);
	mMatcher_ = new PatternMatcher2D_corr();

	//	make sure that dimensions are odd numbers
	lWidth_ = 2*(nbColsL/2) +1;
	lHeight_ = 2*(nbRowsL/2) +1;

	lVerticalPadding_ = verticalPaddingL;	
	
	//	create the pattern we will use for the matching
	lPat_ = new Pattern2D(rasterType, lHeight_, lWidth_);
	lGrabRect_ = new ImageRect(0, 0, lWidth_, lHeight_);
	lMatcher_ = new PatternMatcher2D_corr();


	patMatchRecord_ = new Pattern2DMatchRecord();
	
	isSubpixelMatcher_ = false;
}

RectifiedStereoMatcher_corr::~RectifiedStereoMatcher_corr(void)
{
	delete sGrabRect_;
	delete sPat_;
	delete matcher_;
	delete mGrabRect_;
	delete mPat_;
	delete mMatcher_;
	delete lGrabRect_;
	delete lPat_;
	delete lMatcher_;
	delete patMatchRecord_;
}

int RectifiedStereoMatcher_corr::getSWidth(void)
{
	return sWidth_;
}
int RectifiedStereoMatcher_corr::getSHeight(void)
{
	return sHeight_;
}
int RectifiedStereoMatcher_corr::getMWidth(void)
{
	return mWidth_;
}
int RectifiedStereoMatcher_corr::getMHeight(void)
{
	return mHeight_;
}
int RectifiedStereoMatcher_corr::getLWidth(void)
{
	return lWidth_;
}
int RectifiedStereoMatcher_corr::getLHeight(void)
{
	return lHeight_;
}
#if 0
#pragma mark -
#endif
//------------------------------------------------------
//	private matching functions
//------------------------------------------------------


void RectifiedStereoMatcher_corr::match_(StereoPair *stereoPair, ImagePoint* pt, 
										 StereoImageID l_or_r, StereoMatchRecord *matchRec,
										 ImageRect* searchRect)
{
	FAIL_CONDITION( !stereoPair->isRectified(),
                    kInvalidParameterError,
                    "RectifiedStereoMatcher objects only accepts rrectified stereo pairs");

	//	get access to the 2 images's raster
	RasterImage	*leftImage = stereoPair->getLeftImage(),
				*rightImage = stereoPair->getRightImage();
	RasterImage	*patImage;
    const int   ptX = pt->getX(),
                ptY = pt->getY();
	
    //  because pattern matching uses the upper-left corner of the pattern as origin, we need
    //  to translate our point by the pattern's half dimensions
	sGrabRect_->setOrigin(ptX - sWidth_/2, ptY - sHeight_/2);
	
	//  find a match in the left image
	if (l_or_r == LEFT_IMAGE) 
	{
		const ImageRect* imgRect = leftImage->getValidRect();
		
		//	grab into pat_ the part of the left image we want to find a match for on the right
		patImage = sPat_->getPattern();
		patImage->setValidRect(0, 0, sWidth_, sHeight_);
		ImagePoint	origin(0, 0);
		leftImage->writeInto(patImage, sGrabRect_, &origin);
		
		matcher_->match(sPat_, searchRect, rightImage, patMatchRecord_);
		if (patMatchRecord_->getScore() < SCORE_THRESHOLD)
		{
		    //  because pattern matching uses the upper-left corner of the pattern as origin, we need
		    //  to translate our point by the pattern's half dimensions
			mGrabRect_->setOrigin(ptX - mWidth_/2, ptY - mHeight_/2);

			//	grab into pat_ the part of the left image we want to find a match for on the right
			patImage = mPat_->getPattern();
			patImage->setValidRect(0, 0, mWidth_, mHeight_);
			ImagePoint	origin(0, 0);
			leftImage->writeInto(patImage, mGrabRect_, &origin);

			computeSearchRects_(pt, imgRect, mWidth_, mHeight_, mVerticalPadding_, l_or_r, searchRect);
			mMatcher_->match(mPat_, searchRect, rightImage, patMatchRecord_);

			if (patMatchRecord_->getScore() < SCORE_THRESHOLD) 
			{
			    //  because pattern matching uses the upper-left corner of the pattern as origin, we need
			    //  to translate our point by the pattern's half dimensions
				lGrabRect_->setOrigin(ptX - lWidth_/2, ptY - lHeight_/2);

				//	grab into pat_ the part of the left image we want to find a match for on the right
				patImage = lPat_->getPattern();
				patImage->setValidRect(0, 0, lWidth_, lHeight_);
				ImagePoint	origin(0, 0);
				leftImage->writeInto(patImage, lGrabRect_, &origin);

				computeSearchRects_(pt, imgRect, lWidth_, lHeight_, lVerticalPadding_, l_or_r, searchRect);
				lMatcher_->match(lPat_, searchRect, rightImage, patMatchRecord_);
				
				//  don't forget to translate back the matching point found by hal/f the dimensions of the
				//  matching window!
				ImagePoint  *bestMatch = patMatchRecord_->getBestMatch();
				matchRec->setBestMatch(bestMatch->getX() + lWidth_/2, bestMatch->getY() + lHeight_/2);
				matchRec->setScore(patMatchRecord_->getScore());

cout << "left, large match: " <<  patMatchRecord_->getScore() << endl;
			}
			else
			{
				//  don't forget to translate back the matching point found by hal/f the dimensions of the
				//  matching window!
				ImagePoint  *bestMatch = patMatchRecord_->getBestMatch();
				matchRec->setBestMatch(bestMatch->getX() + mWidth_/2, bestMatch->getY() + mHeight_/2);
				matchRec->setScore(patMatchRecord_->getScore());

cout << "left, medium match: " <<  patMatchRecord_->getScore() << endl;
			}
		}
		else 
		{
			//  don't forget to translate back the matching point found by hal/f the dimensions of the
			//  matching window!
			ImagePoint  *bestMatch = patMatchRecord_->getBestMatch();
			matchRec->setBestMatch(bestMatch->getX() + sWidth_/2, bestMatch->getY() + sHeight_/2);
			matchRec->setScore(patMatchRecord_->getScore());

cout << "left, small match: " <<  patMatchRecord_->getScore() << endl;
		}
	}
	
	//  find a match in the right image
	else
	{
		const ImageRect* imgRect = rightImage->getValidRect();
		
		//	grab into pat_ the part of the left image we want to find a match for on the right
		patImage = sPat_->getPattern();
		patImage->setValidRect(0, 0, sWidth_, sHeight_);
		ImagePoint	origin(0, 0);
		rightImage->writeInto(patImage, sGrabRect_, &origin);
		
		matcher_->match(sPat_, searchRect, leftImage, patMatchRecord_);
		if (patMatchRecord_->getScore() < SCORE_THRESHOLD)
		{
		    //  because pattern matching uses the upper-left corner of the pattern as origin, we need
		    //  to translate our point by the pattern's half dimensions
			mGrabRect_->setOrigin(ptX - mWidth_/2, ptY - mHeight_/2);

			//	grab into pat_ the part of the left image we want to find a match for on the right
			patImage = mPat_->getPattern();
			patImage->setValidRect(0, 0, mWidth_, mHeight_);
			ImagePoint	origin(0, 0);
			rightImage->writeInto(patImage, mGrabRect_, &origin);

			computeSearchRects_(pt, imgRect, mWidth_, mHeight_, mVerticalPadding_, l_or_r, searchRect);
			mMatcher_->match(mPat_, searchRect, leftImage, patMatchRecord_);

			if (patMatchRecord_->getScore() < SCORE_THRESHOLD) 
			{
			    //  because pattern matching uses the upper-left corner of the pattern as origin, we need
			    //  to translate our point by the pattern's half dimensions
				lGrabRect_->setOrigin(ptX - lWidth_/2, ptY - lHeight_/2);

				//	grab into pat_ the part of the left image we want to find a match for on the right
				patImage = lPat_->getPattern();
				patImage->setValidRect(0, 0, lWidth_, lHeight_);
				ImagePoint	origin(0, 0);
				rightImage->writeInto(patImage, lGrabRect_, &origin);

				computeSearchRects_(pt, imgRect, lWidth_, lHeight_, lVerticalPadding_, l_or_r, searchRect);
				lMatcher_->match(lPat_, searchRect, leftImage, patMatchRecord_);
				
				//  don't forget to translate back the matching point found by hal/f the dimensions of the
				//  matching window!
				ImagePoint  *bestMatch = patMatchRecord_->getBestMatch();
				matchRec->setBestMatch(bestMatch->getX() + lWidth_/2, bestMatch->getY() + lHeight_/2);
				matchRec->setScore(patMatchRecord_->getScore());

cout << "right, large match: " <<  patMatchRecord_->getScore() << endl;
			}
			else
			{
				//  don't forget to translate back the matching point found by hal/f the dimensions of the
				//  matching window!
				ImagePoint  *bestMatch = patMatchRecord_->getBestMatch();
				matchRec->setBestMatch(bestMatch->getX() + mWidth_/2, bestMatch->getY() + mHeight_/2);
				matchRec->setScore(patMatchRecord_->getScore());

cout << "right, medium match: " <<  patMatchRecord_->getScore() << endl;
			}
		}
		else 
		{
			//  don't forget to translate back the matching point found by hal/f the dimensions of the
			//  matching window!
			ImagePoint  *bestMatch = patMatchRecord_->getBestMatch();
			matchRec->setBestMatch(bestMatch->getX() + sWidth_/2, bestMatch->getY() + sHeight_/2);
			matchRec->setScore(patMatchRecord_->getScore());

cout << "right, small match: " <<  patMatchRecord_->getScore() << endl;
		}
	}
	
	
}


void RectifiedStereoMatcher_corr::computeSearchRect_(ImagePoint* pt, ImageRect* imgRect,
													 const StereoImageID l_or_r,  ImageRect* rect)
{
	computeSearchRects_(pt, imgRect, sWidth_, sHeight_, sVerticalPadding_, l_or_r, rect);
}


void RectifiedStereoMatcher_corr::computeSearchRects_(ImagePoint* pt, ImageRect* imgRect, 
													  const int theWidth, const int theHeight,
													  const int verticalPadding,
													  const StereoImageID l_or_r, ImageRect* rect)
{
    const int   iLow = imgRect->getTop(),
                iHigh = imgRect->getBottom(),
                jLow = imgRect->getLeft(),
                jHigh = imgRect->getRight(),
                ptX = pt->getX(),
                ptY = pt->getY();
                //  because pattern matching uses the upper-left corner of the pattern as
                //  origin, we need to translate our point by the pattern's half dimensions

    int     iMin, iMax, jMin, jMax,
            shiftedPtX,
            shiftedPtY;

	//---------------------------------------------------
	//	Small matcher
	//---------------------------------------------------
    shiftedPtX = ptX-theWidth/2,
    shiftedPtY = ptY-theHeight/2;
    iMin = shiftedPtY - verticalPadding;
    iMax = shiftedPtY + verticalPadding;
    if (iMin < iLow)
        iMin = iLow;
    if (iMax > iHigh)
        iMax = iHigh;

	jMin = shiftedPtX - maxDisparity_;
	if (jMin < jLow)
		jMin = jLow;
	jMax = pt->getX()+theWidth/2 + maxDisparity_;
	if (jMax > jHigh)
		jMax= jHigh;
		
	//  find a match in the left image
	if (l_or_r == LEFT_IMAGE)
	{   
	    //  the search rectangle on the right horizontally 
	    //      o starts from the image's left border
	    //      o ends to the left pt's x (infinite disp.), shifted by pat's half width
		rect->setRect(jMin, iMin, shiftedPtX-jMin+1, iMax-iMin + 1);
	}

	//  find a match in the right image
	else
	{
	    //  the search rectangle on the left horizontally 
	    //      o starts from the right pt's x (infinite disp.), shifted by pat's half width
	    //      o ends at the image's right border - width
		rect->setRect(shiftedPtX, iMin, jMax-theWidth-shiftedPtX+1, iMax-iMin + 1);
    }
}
