/*  NAME:
        PatternMatcher2D_scaledDiff.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib PatternMatcher2D_scaledDiff class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "PatternMatcher2D_scaledDiff.h"

using namespace std;
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

PatternMatcher2D_scaledDiff::PatternMatcher2D_scaledDiff(void)
    :   PatternMatcher2D( )
{
    isSubpixelMatcher_ = false;
    
    grayPat_ = grayImg_ = NULL;
    rgbaPat_ = rgbaImg_ = NULL;
    patBlob_ = NULL;
    patMask_ = NULL;
    patWidth_ = patHeight_ = -1;
    patRasterType_ = kUndefRasterType;
    baseMatcherType_ = PATTERN_MATCHER_2D_CORR;
}


PatternMatcher2D_scaledDiff::~PatternMatcher2D_scaledDiff(void)
{
    deleteRasters_();
}

#if 0
#pragma mark -
#endif

void PatternMatcher2D_scaledDiff::setPattern(const Pattern2D* pat)
{
    FAIL_CONDITION(	pat == NULL,
                    kNullParameterError,
                    "NULL pattern sent as parameter to PatternMatcher2D_scaledDiff::setPattern");
    setPattern(pat->getPattern(), pat->getBlob());
}

void PatternMatcher2D_scaledDiff::setPattern(const RasterImage* patImg, const Blob* patBlob)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL image sent as parameter to PatternMatcher2D_scaledDiff::setPattern");
    
    BaseRasterType patRasterType = patImg->getBaseRasterType();
    FAIL_CONDITION(	(patRasterType != kGrayRaster) && (patRasterType != kRGBa32Raster),
                    kInvalidParameterError,
                    "PatternMatcher2D_scaledDiff::setPattern only takes gray and RGBa images");

    //  if needed, allocate new temporary storage raster for the pattern and image
    allocateRasters_(patImg, patBlob);
    patBlob_ = patBlob;
    patImg_ = patImg;
}


#if 0
#pragma mark -
#endif

//----------------------------------------------------------
//  protected matching functions
//----------------------------------------------------------

//  For the RGBa correlation, I simply use the R, G, B components as independent
//  variables in the summation
void PatternMatcher2D_scaledDiff::match_(const RasterImage_RGBa* patImg, 
										   const ImageRect* searchRect, 
										   const RasterImage_RGBa* img, 
										   Pattern2DMatchRecord* matchRecord)
{
    const float ONE_THIRD = 1.f / 3.f;
    const int   iLow = searchRect->getTop(),
                iHigh = searchRect->getBottom(),
                jLow = searchRect->getLeft(),
                jHigh = searchRect->getRight();
    const unsigned char* const* rgbaImg = img->getShiftedRGBa2D();
    float   patCorrScale[3] = {0, 0, 0};
    float   cov[3] = {0, 0, 0},
            corrScale[3] = {0, 0, 0},
            corr, 
            bestScore = -2;        
    int     bestI=-1, bestJ=-1;

    //  Compute the pattern's correlation scale factor
    patternCorrScale_(patImg, patCorrScale);

    //  For each possible location of the upper-left corner of the pattern
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            //  compute the variance and scaling factor
            covarianceRGBa_(rgbaImg, j, i, cov, corrScale);
            
            //  Compute the actual correlation value and compare it to best so far
            corr = ONE_THIRD * (cov[0] * corrScale[0] * patCorrScale[0] + 
                                cov[1] * corrScale[1] * patCorrScale[1] + 
                                cov[2] * corrScale[2] * patCorrScale[2]);
            if (corr > bestScore)
            {
                bestScore = corr;
                bestI = i;
                bestJ = j;
            }
        }
            
    //  At the end of the loop, we report the best correlation score and its location.
    //  Note that the "best" is not necessarily good.
    matchRecord->setScore(bestScore);  
    matchRecord->setBestMatch(bestJ, bestI);       
}


void PatternMatcher2D_scaledDiff::match_(const RasterImage_gray* patImg, 
                                   const ImageRect* searchRect, 
                                   const RasterImage_gray* img, 
                                   Pattern2DMatchRecord* matchRecord)
{
    const int   iLow = searchRect->getTop(),
                iHigh = searchRect->getBottom(),
                jLow = searchRect->getLeft(),
                jHigh = searchRect->getRight();
                
    const unsigned char* const* grayImg = img->getShiftedGray2D();

    //  Compute the pattern's correlation scale factor
    float       patCorrScale = 0;
    patternCorrScale_(patImg, &patCorrScale);
            
    float   cov = 0, corr, corrScale = 0, bestScore = -2;
    int     bestI=-1, bestJ=-1;
    
    //  For each possible location of the upper-left corner of the pattern
    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            //  compute the variance and scaling factor
            covarianceGray_(grayImg, j, i, &cov, &corrScale);
            
            //  Compute the actual correlation value and compare it to best so far
            corr = cov * corrScale * patCorrScale;
            if (corr > bestScore)
            {
                bestScore = corr;
                bestI = i;
                bestJ = j;
            }
        }
    }
            
    //  At the end of the loop, we report the best correlation score and its location.
    //  Note that the "best" is not necessarily good.
    matchRecord->setScore(bestScore);  
    matchRecord->setBestMatch(bestJ, bestI);       
}

void PatternMatcher2D_scaledDiff::match_(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                   const ImageRect* searchRect, 
                                   const RasterImage_RGBa* img, 
                                   Pattern2DMatchRecord* matchRecord)
{
    const   float   ONE_THIRD = 1.f / 3.f;
    const int   iLow = searchRect->getTop(),
                iHigh = searchRect->getBottom(),
                jLow = searchRect->getLeft(),
                jHigh = searchRect->getRight();
    const unsigned char* const* rgbaImg = img->getShiftedRGBa2D();
    float   patCorrScale[3] = {0, 0, 0},
            cov[3] = {0, 0, 0},
            corrScale[3] = {0, 0, 0},
            corr, 
            bestScore = -2;        
    int     bestI=-1, bestJ=-1;
            
    //  Compute the pattern's correlation scale factor
    patternCorrScale_(patImg, patBlob, patCorrScale);

    //  For each possible location of the upper-left corner of the pattern
    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            //  compute the variance and scaling factor
            covarianceRGBaBlob_(rgbaImg, j, i, cov, corrScale);
                        
            //  Compute the actual correlation value and compare it to best so far
            corr = ONE_THIRD * (cov[0] * corrScale[0] * patCorrScale[0] + 
                                cov[1] * corrScale[1] * patCorrScale[1] + 
                                cov[2] * corrScale[2] * patCorrScale[2]);
            if (corr > bestScore)
            {
                bestScore = corr;
                bestI = i;
                bestJ = j;
            }
        }
    }
            
    //  At the end of the loop, we report the best correlation score and its location.
    //  Note that the "best" is not necessarily good.
    matchRecord->setScore(bestScore);  
    matchRecord->setBestMatch(bestJ, bestI);       
}

void PatternMatcher2D_scaledDiff::match_(const RasterImage_gray* patImg, const Blob* patBlob, 
										   const ImageRect* searchRect, 
										   const RasterImage_gray* img, 
										   Pattern2DMatchRecord* matchRecord)
{
    const int   iLow = searchRect->getTop(),
                iHigh = searchRect->getBottom(),
                jLow = searchRect->getLeft(),
                jHigh = searchRect->getRight();
    const unsigned char* const* grayImg = img->getShiftedGray2D();
    float   patCorrScale = 0.f,
            corr = 0.f,
            corrScale = 0.f,
            cov = 0.f,
            bestScore = -2;
    int     bestI=-1, bestJ=-1;

    //  Compute the pattern's correlation scale factor
    patternCorrScale_(patImg, patBlob, &patCorrScale);

    //  For each possible location of the upper-left corner of the pattern
    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            //  compute the variance and scaling factor
            covarianceGrayBlob_(grayImg, j, i, &cov, &corrScale);
                                   
            //  Compute the actual correlation value and compare it to best so far
            corr = cov * corrScale * patCorrScale;
            if (corr > bestScore)
            {
                bestScore = corr;
                bestI = i;
                bestJ = j;
            }
        }
    }
            
    //  At the end of the loop, we report the best correlation score and its location.
    //  Note that the "best" is not necessarily good.
    matchRecord->setScore(bestScore);  
    matchRecord->setBestMatch(bestJ, bestI);       
}


#if 0
#pragma mark -
#endif


void PatternMatcher2D_scaledDiff::covariance(const RasterImage_RGBa* img, int x, int y, float* cov, 
                                       float* corrScale)
{
    FAIL_CONDITION(	cov == NULL,
                    kNullParameterError,
                    "NULL covariance array sent to PatternMatcher2D_scaledDiff::covariance");
    FAIL_CONDITION(	corrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_scaledDiff::covariance");
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image sent to PatternMatcher2D_scaledDiff::covariance");

    const unsigned char* const* rgbaImg = img->getShiftedRGBa2D();

    if (patBlob_==NULL)
    {
//         ImageRect   *patRect = patImg_->getValidRect();   // unused variable -CJC
        covarianceRGBa_(rgbaImg, x, y, cov, corrScale);
    }
    else
        covarianceRGBaBlob_(rgbaImg, x, y, cov, corrScale);

}

void PatternMatcher2D_scaledDiff::covariance(const RasterImage_gray* img, int x, int y, float* cov, 
                                       float* corrScale)
{
    FAIL_CONDITION(	cov == NULL,
                    kNullParameterError,
                    "NULL covariance array sent to PatternMatcher2D_scaledDiff::covariance");
    FAIL_CONDITION(	corrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_scaledDiff::covariance");
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image sent to PatternMatcher2D_scaledDiff::covariance");

    const unsigned char* const* grayImg = img->getShiftedGray2D();

    if (patBlob_==NULL)
    {
//         ImageRect   *patRect = patImg_->getValidRect(); // unused variable -CJC
        covarianceGray_(grayImg, x, y, cov, corrScale);
    }
    else
        covarianceGrayBlob_(grayImg, x, y, cov, corrScale);

}

void PatternMatcher2D_scaledDiff::covarianceRGBa_(const unsigned char* const* rgbaImg, int x, int y, 
                                            float* cov, float* corrScale)
{
    const int   nbPatPixels = patWidth_ * patHeight_;
    const float nbPixelsScale = 1.f / nbPatPixels;

    float   imgRedSum = 0, imgRedSumSq = 0, imgRedExp;
    float   imgGreenSum = 0, imgGreenSumSq = 0, imgGreenExp;
    float   imgBlueSum = 0, imgBlueSumSq = 0, imgBlueExp;
    
    //-------------------------------------------------------------------------
    //  Part 1: Compute expected value and scaling factor for this part of the 
    //          image
    //-------------------------------------------------------------------------

// unused variables ... -CJC
//     const int   kMin = y, kMax = y + patHeight_ - 1,
//                 lMin = x, lMax = x + patWidth_ - 1;

    for (int kP=0, kI=y; kP<patHeight_; kP++, kI++)
    {
        for (int lP=0, lI=x, fourLI=4*lI; lP<patWidth_; lP++, lI++, fourLI+=2)
        {
            //  red component
            imgRedSum += (rgbaImg_[kP][lP][0] = rgbaImg[kI][fourLI]);
            imgRedSumSq += rgbaImg_[kP][lP][0]*rgbaImg_[kP][lP][0];
            //  green component
            imgGreenSum += (rgbaImg_[kP][lP][1] = rgbaImg[kI][++fourLI]);
            imgGreenSumSq += rgbaImg_[kP][lP][1]*rgbaImg_[kP][lP][1];
            //  blue component
            imgBlueSum += (rgbaImg_[kP][lP][2] = rgbaImg[kI][++fourLI]);
            imgBlueSumSq += rgbaImg_[kP][lP][2]*rgbaImg_[kP][lP][2];
        }
    }
    
    imgRedExp = imgRedSum * nbPixelsScale;
    corrScale[0] = 1.f / sqrtf(imgRedSumSq*nbPixelsScale - imgRedExp*imgRedExp);
    imgGreenExp = imgGreenSum * nbPixelsScale;
    corrScale[1] = 1.f / sqrtf(imgGreenSumSq*nbPixelsScale - imgGreenExp*imgGreenExp);
    imgBlueExp = imgBlueSum * nbPixelsScale;
    corrScale[2] = 1.f / sqrtf(imgBlueSumSq*nbPixelsScale - imgBlueExp*imgBlueExp);

    //-------------------------------------------------------------------------
    //  Part 2: Now actually compute the covariance
    //-------------------------------------------------------------------------
    float   covRedSum = 0, covGreenSum = 0, covBlueSum = 0;
    for (int kP=0; kP<patHeight_; kP++)
    {
        for (int lP=0; lP<patWidth_; lP++)
        {
            //  red component
            covRedSum += rgbaPat_[kP][lP][0] * 
                          (rgbaImg_[kP][lP][0] - imgRedExp);
            
            //  green component
            covGreenSum += rgbaPat_[kP][lP][1] * 
                            (rgbaImg_[kP][lP][1] - imgGreenExp);
            
            //  blue component
            covBlueSum += rgbaPat_[kP][lP][2] * 
                           (rgbaImg_[kP][lP][2] - imgBlueExp);
            
        }
    }
    
    cov[0] = covRedSum * nbPixelsScale;
    cov[1] = covGreenSum * nbPixelsScale;
    cov[2] = covBlueSum * nbPixelsScale;
}

void PatternMatcher2D_scaledDiff::covarianceGray_(const unsigned char* const* grayImg, int x, int y, 
                                            float* cov, float* corrScale)
{
    const int   nbPatPixels = patHeight_ * patWidth_;
    const float nbPixelsScale = 1.f / nbPatPixels;

    //-------------------------------------------------------------------------
    //  Part 1: Compute expected value and scaling factor for this part of the 
    //          image
    //-------------------------------------------------------------------------
    float   imgSum = 0, imgSumSq = 0, imgExp;
    for (int kP=0, kI=y; kP<patHeight_; kP++, kI++)
        for (int lP=0, lI=x; lP<patWidth_; lP++, lI++)
        {
            imgSum += (grayImg_[kP][lP] = grayImg[kI][lI]);
            imgSumSq += grayImg_[kP][lP]*grayImg_[kP][lP];
        }
    imgExp = imgSum * nbPixelsScale;
    *corrScale = 1.f / sqrtf(imgSumSq*nbPixelsScale - imgExp*imgExp);

    //-------------------------------------------------------------------------
    //  Part 2: Now actually compute the covariance
    //-------------------------------------------------------------------------
    float covSum = 0.f;
    for (int kP=0; kP< patHeight_; kP++)
        for (int lP=0; lP<patWidth_; lP++)
            covSum += grayPat_[kP][lP] * (grayImg_[kP][lP] - imgExp);
    
    *cov = covSum * nbPixelsScale;
}

void PatternMatcher2D_scaledDiff::covarianceRGBaBlob_(const unsigned char* const* rgbaImg, int x, int y, 
                                                float* cov, float* corrScale)
{
    const   float   nbPixelsScale = 1.f / patBlob_->getNbPixels();

    float   imgRedSum = 0, imgRedSumSq = 0, imgRedExp;
    float   imgGreenSum = 0, imgGreenSumSq = 0, imgGreenExp;
    float   imgBlueSum = 0, imgBlueSumSq = 0, imgBlueExp;
    
    //-------------------------------------------------------------------------
    //  Part 1: Compute expected value and scaling factor for this part of the 
    //          image
    //-------------------------------------------------------------------------

// unused variables... -CJC
//     const int   kMin = y, kMax = y + patHeight_ - 1,
//                 lMin = x, lMax = x + patWidth_ - 1;
    for (int kP=0, kI=y; kP<patHeight_; kP++, kI++)
    {
        for (int lP=0, lI=x, fourLI=4*lI; lP<patWidth_; lP++, lI++)
        {
        	if (patMask_[kP][lP])
        	{
	            //  red component
	            imgRedSum += (rgbaImg_[kP][lP][0] = rgbaImg[kI][fourLI]);
	            imgRedSumSq += rgbaImg_[kP][lP][0]*rgbaImg_[kP][lP][0];
	            //  green component
	            imgGreenSum += (rgbaImg_[kP][lP][1] = rgbaImg[kI][++fourLI]);
	            imgGreenSumSq += rgbaImg_[kP][lP][1]*rgbaImg_[kP][lP][1];
	            //  blue component
	            imgBlueSum += (rgbaImg_[kP][lP][2] = rgbaImg[kI][++fourLI]);
	            imgBlueSumSq += rgbaImg_[kP][lP][2]*rgbaImg_[kP][lP][2];
	            fourLI+=2;
            }
            else
                fourLI+=4;
        }
    }
    
    imgRedExp = imgRedSum * nbPixelsScale;
    corrScale[0] = 1.f / sqrtf(imgRedSumSq*nbPixelsScale - imgRedExp*imgRedExp);
    imgGreenExp = imgGreenSum * nbPixelsScale;
    corrScale[1] = 1.f / sqrtf(imgGreenSumSq*nbPixelsScale - imgGreenExp*imgGreenExp);
    imgBlueExp = imgBlueSum * nbPixelsScale;
    corrScale[2] = 1.f / sqrtf(imgBlueSumSq*nbPixelsScale - imgBlueExp*imgBlueExp);


/*
float   patRedTerm = 0,
        patGreenTerm = 0,
        patBlueTerm = 0,
        imgRedTerm = 0,
        imgGreenTerm = 0,
        imgBlueTerm = 0;
*/

    //-------------------------------------------------------------------------
    //  Part 2: Now actually compute the covariance
    //-------------------------------------------------------------------------
    float   covRedSum = 0, covGreenSum = 0, covBlueSum = 0;
    for (int kP=0; kP<patHeight_; kP++)
    {
        for (int lP=0; lP<patWidth_; lP++)
        {
            if (patMask_[kP][lP])
            {
/*
patRedTerm += rgbaPat_[kP][lP][0];
patGreenTerm += rgbaPat_[kP][lP][1];
patBlueTerm += rgbaPat_[kP][lP][2];
imgRedTerm += (rgbaImg_[kP][lP][0] - imgRedExp);
imgGreenTerm += (rgbaImg_[kP][lP][1] - imgGreenExp);
imgBlueTerm += (rgbaImg_[kP][lP][2] - imgBlueExp);
*/
                //  red component
                covRedSum += rgbaPat_[kP][lP][0] * 
                              (rgbaImg_[kP][lP][0] - imgRedExp);
                
                //  green component
                covGreenSum += rgbaPat_[kP][lP][1] * 
                                (rgbaImg_[kP][lP][1] - imgGreenExp);
                
                //  blue component
                covBlueSum += rgbaPat_[kP][lP][2] * 
                               (rgbaImg_[kP][lP][2] - imgBlueExp);
            }
        }
    }
//float xxx = patRedTerm + patGreenTerm + patBlueTerm + imgRedTerm + imgGreenTerm + imgBlueTerm;
    
    cov[0] = covRedSum * nbPixelsScale;
    cov[1] = covGreenSum * nbPixelsScale;
    cov[2] = covBlueSum * nbPixelsScale;
}

void PatternMatcher2D_scaledDiff::covarianceGrayBlob_(const unsigned char* const* grayImg, int x, int y, 
                                                float* cov, float* corrScale)
{
    const   float   nbPixelsScale = 1.f / patBlob_->getNbPixels();

    //-------------------------------------------------------------------------
    //  Part 1: Compute expected value and scaling factor for this part of the 
    //          image
    //-------------------------------------------------------------------------
    float   imgSum = 0, imgSumSq = 0, imgExp;
    for (int kP=0, kI=y; kP<patHeight_; kP++, kI++)
        for (int lP=0, lI=x; lP<patWidth_; lP++, lI++)
        {
            if (patMask_[kP][lP])
            {
                imgSum += (grayImg_[kP][lP] = grayImg[kI][lI]);
                imgSumSq += grayImg_[kP][lP]*grayImg_[kP][lP];
            }
        }
    imgExp = imgSum * nbPixelsScale;
    *corrScale = 1.f / sqrtf(imgSumSq*nbPixelsScale - imgExp*imgExp);

    //-------------------------------------------------------------------------
    //  Part 2: Now actually compute the covariance
    //-------------------------------------------------------------------------
    float covSum = 0.f;
    for (int kP=0; kP< patHeight_; kP++)
        for (int lP=0; lP<patWidth_; lP++)
            if (patMask_[kP][lP])
                covSum += grayPat_[kP][lP] * (grayImg_[kP][lP] - imgExp);
    
    *cov = covSum * nbPixelsScale;
}

#if 0
#pragma mark -
#endif


void PatternMatcher2D_scaledDiff::patternCorrScale(const RasterImage_RGBa* patImg, float* patCorrScale)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_scaledDiff::patternCorrScale");
    FAIL_CONDITION(	patCorrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_scaledDiff::patternCorrScale");
    
    patternCorrScale_(patImg, patCorrScale);
}

void PatternMatcher2D_scaledDiff::patternCorrScale(const RasterImage_gray* patImg, float* patCorrScale)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_scaledDiff::patternCorrScale");
    FAIL_CONDITION(	patCorrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_scaledDiff::patternCorrScale");
    
    patternCorrScale_(patImg, patCorrScale);
}

void PatternMatcher2D_scaledDiff::patternCorrScale(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                             float* patCorrScale)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_scaledDiff::patternCorrScale");
    FAIL_CONDITION(	patCorrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_scaledDiff::patternCorrScale");

    if (patBlob == NULL)        
        patternCorrScale_(patImg, patCorrScale);
    else
        patternCorrScale_(patImg, patBlob, patCorrScale);

}

void PatternMatcher2D_scaledDiff::patternCorrScale(const RasterImage_gray* patImg, const Blob* patBlob,
                                             float* patCorrScale)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_scaledDiff::patternCorrScale");
    FAIL_CONDITION(	patCorrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_scaledDiff::patternCorrScale");

    if (patBlob == NULL)        
        patternCorrScale_(patImg, patCorrScale);
    else
        patternCorrScale_(patImg, patBlob, patCorrScale);

}


void PatternMatcher2D_scaledDiff::patternCorrScale_(const RasterImage_RGBa* patImg, 
                                              float* patCorrScale)
{
    setPattern(patImg);

    const unsigned char* const* rgbaPat = patImg->getRGBa2D();
    const int   nbPatPixels = patWidth_ * patHeight_;
    const float nbPixelsScale = 1.f / nbPatPixels;

    //  Calculate normalizing factor for the pattern.  I know that we are off by a factor
    //  of 1/sqrt(nbPixels).   I correct that when I multiply the two scaling factors
    float   patRedSum = 0, patRedSumSq = 0;
    float   patGreenSum = 0, patGreenSumSq = 0;
    float   patBlueSum = 0, patBlueSumSq = 0;
    for (int kP=0; kP<patHeight_; kP++)
        for (int lP=0, fourLP=0; lP<patWidth_; lP++, fourLP+=2)
        {
            //  red component
            patRedSum += rgbaPat[kP][fourLP];
            patRedSumSq += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
            //  green component
            patGreenSum += rgbaPat[kP][++fourLP];
            patGreenSumSq += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
            //  blue component
            patBlueSum += rgbaPat[kP][++fourLP];
            patBlueSumSq += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
        }

    patExp_[0] = patRedSum * nbPixelsScale;
    patCorrScale[0] = 1.f / sqrtf(patRedSumSq*nbPixelsScale - patExp_[0]*patExp_[0]);
    patExp_[1] = patGreenSum * nbPixelsScale;
    patCorrScale[1] = 1.f / sqrtf(patGreenSumSq*nbPixelsScale - patExp_[1]*patExp_[1]);
    patExp_[2] = patBlueSum * nbPixelsScale;
    patCorrScale[2] = 1.f / sqrtf(patBlueSumSq*nbPixelsScale - patExp_[2]*patExp_[2]);

    //  copy pattern data shifted by mean value
    for (int kP=0; kP<patHeight_; kP++)
        for (int lP=0, fourLP=0; lP<patWidth_; lP++, fourLP++)
        {
            rgbaPat_[kP][lP][0] = rgbaPat[kP][fourLP++] - patExp_[0];
            rgbaPat_[kP][lP][1] = rgbaPat[kP][fourLP++] - patExp_[1];
            rgbaPat_[kP][lP][2] = rgbaPat[kP][fourLP++] - patExp_[2];
        }
}

void PatternMatcher2D_scaledDiff::patternCorrScale_(const RasterImage_gray* patImg, 
                                              float* patCorrScale)
{
    setPattern(patImg);

    const unsigned char* const* grayPat = patImg->getGray2D();
    const int   nbPatPixels = patWidth_ * patHeight_;
    const float nbPixelsScale = 1.f / nbPatPixels;

    //  Calculate normalizing factor for the pattern.
    float   patSum = 0, patSumSq = 0;
    for (int kP=0; kP<patHeight_; kP++)
        for (int lP=0; lP<patWidth_; lP++)
        {
            patSum += grayPat[kP][lP];
            patSumSq += grayPat[kP][lP]*grayPat[kP][lP];
        }
    patExp_[0] = patSum * nbPixelsScale;
    *patCorrScale = 1.f / sqrtf(patSumSq*nbPixelsScale - patExp_[0]*patExp_[0]);

    //  copy pattern data shifted by mean value
    for (int kP=0; kP<patHeight_; kP++)
        for (int lP=0; lP<patWidth_; lP++)
            grayPat_[kP][lP] = grayPat[kP][lP] - patExp_[0];
            
}

void PatternMatcher2D_scaledDiff::patternCorrScale_(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                              float* patCorrScale)
{
    setPattern(patImg, patBlob);

    const unsigned char* const* rgbaPat = patImg->getRGBa2D();
    const int   nbPatPixels = patBlob->getNbPixels();
    const float nbPixelsScale = 1.f / nbPatPixels;

    //  First, reset the pattern mask
    for (int kP=0; kP<patHeight_; kP++)
        for (int lP=0; lP<patWidth_; lP++)
            patMask_[kP][lP] = (unsigned char) 0;


	//	Then go through the deque-list structure
    const LineSegDeque segDeque = patBlob->getSegDeque();
    HorizontalSegment   *theSeg;

    float   patRedSum = 0, patRedSumSq = 0;
    float   patGreenSum = 0, patGreenSumSq = 0;
    float   patBlueSum = 0, patBlueSumSq = 0;

    //  Calculate normalizing factor for the pattern.  
    LineSegDeque::const_iterator lastLine = segDeque.end();
    for (LineSegDeque::const_iterator lineIter = segDeque.begin(); lineIter != lastLine; lineIter++)
    {
        list<HorizontalSegment*>::const_iterator lastSeg = (*lineIter)->end();
        for (list<HorizontalSegment*>::const_iterator segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
        {
            theSeg = (*segIter);
            int kP = theSeg->y;
            for (int lP = theSeg->leftX, fourLP=4*lP; lP<=theSeg->rightX; lP++, fourLP+=2)
            {
                //  red component
                patRedSum += rgbaPat[kP][fourLP];
                patRedSumSq += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
                //  green component
                patGreenSum += rgbaPat[kP][++fourLP];
                patGreenSumSq += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
                //  blue component
                patBlueSum += rgbaPat[kP][++fourLP];
                patBlueSumSq += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];

                patMask_[kP][lP] = (unsigned char) 1;
            }
        }
    }

    patExp_[0] = patRedSum * nbPixelsScale;
    patCorrScale[0] = 1.f / sqrtf(patRedSumSq*nbPixelsScale - patExp_[0]*patExp_[0]);
    patExp_[1] = patGreenSum * nbPixelsScale;
    patCorrScale[1] = 1.f / sqrtf(patGreenSumSq*nbPixelsScale - patExp_[1]*patExp_[1]);
    patExp_[2] = patBlueSum * nbPixelsScale;
    patCorrScale[2] = 1.f / sqrtf(patBlueSumSq*nbPixelsScale - patExp_[2]*patExp_[2]);

float sumRed = 0;
//cout << "patCorrScale  ";
    //  copy pattern data shifted by mean value.  I think it is probably faster to
    //  apply the shift over the entire bounding rectangle than to check the mask
    for (int kP=0; kP<patHeight_; kP++)
        for (int lP=0, fourLP=0; lP<patWidth_; lP++, fourLP++)
        {
            rgbaPat_[kP][lP][0] = rgbaPat[kP][fourLP++] - patExp_[0];
            rgbaPat_[kP][lP][1] = rgbaPat[kP][fourLP++] - patExp_[1];
            rgbaPat_[kP][lP][2] = rgbaPat[kP][fourLP++] - patExp_[2];
            
            if (patMask_[kP][lP]) {
                sumRed += rgbaPat_[kP][lP][0];
//                cout << rgbaPat_[kP][lP][0] << " ";
            }
        }
//    cout << endl << "sumRed = " << sumRed << endl;
}

void PatternMatcher2D_scaledDiff::patternCorrScale_(const RasterImage_gray* patImg, const Blob* patBlob,
                                              float* patCorrScale)
{
    setPattern(patImg, patBlob);

    const unsigned char* const* grayPat = patImg->getGray2D();
    const int   nbPatPixels = patBlob->getNbPixels();
    const float nbPixelsScale = 1.f / nbPatPixels;

    //  First, reset the pattern mask
    for (int kP=0; kP<patHeight_; kP++)
        for (int lP=0; lP<patWidth_; lP++)
            patMask_[kP][lP] = (unsigned char) 0;

	//	Then go through the deque-list structure
    const LineSegDeque segDeque = patBlob->getSegDeque();
    HorizontalSegment   *theSeg;

    float   patSum = 0.f, patSumSq = 0.f;
    //  Calculate normalizing factor for the pattern.  I know that we are off by a factor
    //  of 1/sqrt(nbPixels).   I correct that when I multiply the two scaling factors
    LineSegDeque::const_iterator lastLine = segDeque.end();
    for (LineSegDeque::const_iterator lineIter = segDeque.begin(); lineIter != lastLine; lineIter++)
    {
        list<HorizontalSegment*>::iterator  segIter, lastSeg = (*lineIter)->end();
        for (segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
        {
            theSeg = (*segIter);
            int kP = theSeg->y;
            for (int lP = theSeg->leftX; lP<=theSeg->rightX; lP++)
            {
                patSum += grayPat[kP][lP];
                patSumSq += grayPat[kP][lP]*grayPat[kP][lP];
                patMask_[kP][lP] = (unsigned char) 1;
            }
        }
    }

    patExp_[0] = patSum * nbPixelsScale;
    *patCorrScale = 1.f / sqrtf(patSumSq*nbPixelsScale - patExp_[0]*patExp_[0]);

    //  copy pattern data shifted by mean value.  I think it is probably faster to
    //  apply the shift over the entire bounding rectangle than to check the mask
    for (int kP=0; kP<patHeight_; kP++)
        for (int lP=0; lP<patWidth_; lP++)
            grayPat_[kP][lP] = grayPat[kP][lP] - patExp_[0];
            
}


#if 0
#pragma mark -
#endif

bool PatternMatcher2D_scaledDiff::isProperMatchRecordType_(const Pattern2DMatchRecord* matchRecord) const
{
    //  the correlation matcher only requires a generic match record
    return true;
}

Pattern2DMatchRecord* PatternMatcher2D_scaledDiff::newPattern2DMatchRecord_(void)
{
    //  the correlaton matcher only requires a generic match record
    return new Pattern2DMatchRecord();
}


void PatternMatcher2D_scaledDiff::deleteRasters_(void)
{
    PatternMatcher2D::deleteRasters_();
    
    if (patExp_ != NULL)
        delete []patExp_;
}

void PatternMatcher2D_scaledDiff::allocateRasters_(const RasterImage* patImg, const Blob* patBlob)
{
    int patWidth = patImg->getWidth(),
        patHeight = patImg->getHeight();
    BaseRasterType  rasterType = patImg->getBaseRasterType();

    bool    mustReallocPatExp = (rasterType!=patRasterType_) || 
                                (patWidth>patWidth_) || 
                                (patHeight>patHeight_);

    PatternMatcher2D::allocateRasters_(patImg, patBlob);
    
    if ( mustReallocPatExp )
    {
        switch (rasterType)
        {
            case kGrayRaster:
                patExp_ = new float[1];
                break;

            case kRGBa32Raster:
                patExp_ = new float[3];
                break;

            default:
                break;
        }
   }
}
