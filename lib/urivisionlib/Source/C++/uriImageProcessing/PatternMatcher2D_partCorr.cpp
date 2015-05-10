/*  NAME:
        PatternMatcher2D_partCorr.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib PatternMatcher2D_partCorr class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "PatternMatcher2D_partCorr.h"
#include "Pattern2DMatchRecord_part.h"

using namespace std;
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

PatternMatcher2D_partCorr::PatternMatcher2D_partCorr(void)
    :   PatternMatcher2D( )
{
    isSubpixelMatcher_ = false;
    baseMatcherType_ = PATTERN_MATCHER_2D_PART_CORR;
}

PatternMatcher2D_partCorr::~PatternMatcher2D_partCorr(void)
{
}

#if 0
#pragma mark -
#endif



//----------------------------------------------------------
//  Private matching functions
//----------------------------------------------------------

//  For the RGBa correlation, I simply use the R, G, B components as independent
//  variables in the summation
void PatternMatcher2D_partCorr::match_(const RasterImage_RGBa* pat, const ImageRect* searchRect, 
                                       const RasterImage_RGBa* img, 
                                       Pattern2DMatchRecord* matchRecord)
{
//     ImageRect   *patRect = pat->getValidRect(); // unused variable. -CJC
    //
    const float ONE_THIRD = 1.f / 3.f;
    const int   iLow = searchRect->getTop(),
                iHigh = searchRect->getBottom(),
                jLow = searchRect->getLeft(),
                jHigh = searchRect->getRight();
    
    int     bestI=-1, bestJ=-1,
            quadBestI[4] = {-1, -1, -1, -1},
            quadBestJ[4] = {-1, -1, -1, -1};
    int     i, j;
    float   bestScore = -2, quadBestScore[4] = {-2, -2, -2, -2};
    float   patExp[][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    float   cov[][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    float   patCorrScale[][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    float   corrScale[][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    float   corr;
    
    //  Compute the pattern's correlation scale factor
    patternCorrScale_(pat, patExp, patCorrScale);

    //  For each possible location of the upper-left corner of the pattern
    for (i=iLow; i<=iHigh; i++)
    {
        for (j=jLow; j<=jHigh; j++)
        {
            //  compute the variance and scaling factor
            covariance_(pat, patExp, img, j, i, cov, corrScale);
            
            //  Compute the actual correlation value and compare it to best so far
        
            //  Compute the global correlation value and compare it to best so far
            corr = ONE_THIRD * (cov[0][0] * corrScale[0][0] * patCorrScale[0][0] + 
                                cov[1][0] * corrScale[1][0] * patCorrScale[1][0] + 
                                cov[2][0] * corrScale[2][0] * patCorrScale[2][0]);
            if (corr > bestScore)
            {
                bestScore = corr;
                bestI = i;
                bestJ = j;
            }

            //  same thing for each of the quadrants
            for (int q=0; q<4; q++)
            {
                corr = ONE_THIRD * (cov[0][q+1] * corrScale[0][q+1] * patCorrScale[0][q+1] + 
                                    cov[1][q+1] * corrScale[1][q+1] * patCorrScale[1][q+1] + 
                                    cov[2][q+1] * corrScale[2][q+1] * patCorrScale[2][q+1]);
                if (corr > quadBestScore[q])
                {
                    quadBestScore[q] = corr;
                    quadBestI[q] = i;
                    quadBestJ[q] = j;
                }
            }                        
        }
    }
            
    Pattern2DMatchRecord_part   *patRec = (Pattern2DMatchRecord_part *) matchRecord;

    //  At the end of the loop, we report the best correlation score and its location.
    //  Note that the "best" is not necessarily good.
    matchRecord->setScore(bestScore);  
    matchRecord->setBestMatch(bestJ, bestI);      
    
    patRec->setBestMatch(UPPER_LEFT_QUAD, quadBestJ[UPPER_LEFT_QUAD], 
                                          quadBestI[UPPER_LEFT_QUAD]);
    patRec->setScore(UPPER_LEFT_QUAD, quadBestScore[UPPER_LEFT_QUAD]);
    //
    patRec->setBestMatch(UPPER_RIGHT_QUAD, quadBestJ[UPPER_RIGHT_QUAD], 
                                           quadBestI[UPPER_RIGHT_QUAD]);
    patRec->setScore(UPPER_RIGHT_QUAD, quadBestScore[UPPER_RIGHT_QUAD]);
    //
    patRec->setBestMatch(LOWER_LEFT_QUAD, quadBestJ[LOWER_LEFT_QUAD], 
                                          quadBestI[LOWER_LEFT_QUAD]);
    patRec->setScore(LOWER_LEFT_QUAD, quadBestScore[LOWER_LEFT_QUAD]);
    //
    patRec->setBestMatch(LOWER_RIGHT_QUAD, quadBestJ[LOWER_RIGHT_QUAD], 
                                           quadBestI[LOWER_RIGHT_QUAD]);
    patRec->setScore(LOWER_RIGHT_QUAD, quadBestScore[LOWER_RIGHT_QUAD]);    
}


void PatternMatcher2D_partCorr::match_(const RasterImage_gray* pat, 
									   const ImageRect* searchRect, 
									   const RasterImage_gray* img,
									   Pattern2DMatchRecord* matchRecord)
{
//     ImageRect   *patRect = pat->getValidRect(); // unused variable. -CJC
    //
//     const float ONE_THIRD = 1.f / 3.f;   // unused variable. -CJC
    const int   iLow = searchRect->getTop(),
                iHigh = searchRect->getBottom(),
                jLow = searchRect->getLeft(),
                jHigh = searchRect->getRight();
    
    int     bestI=-1, bestJ=-1,
            quadBestI[4] = {-1, -1, -1, -1},
            quadBestJ[4] = {-1, -1, -1, -1};
    int     i, j;
    float   bestScore = -2, quadBestScore[4] = {-2, -2, -2, -2};
    float   patExp[5] = {0,0,0,0,0};
    float   cov[5] = {0,0,0,0,0};
    float   patCorrScale[5] = {0,0,0,0,0};
    float   corrScale[5] = {0,0,0,0,0};
    float   corr;
    
    //  Compute the pattern's correlation scale factor
    patternCorrScale_(pat, patExp, patCorrScale);

    //  For each possible location of the upper-left corner of the pattern
    for (i=iLow; i<=iHigh; i++)
    {
        for (j=jLow; j<=jHigh; j++)
        {
            //  compute the variance and scaling factor
            covariance_(pat, patExp, img, j, i, cov, corrScale);
            
            //  Compute the global correlation value and compare it to best so far
            corr = cov[0] * corrScale[0] * patCorrScale[0];

            if (corr > bestScore)
            {
                bestScore = corr;
                bestI = i;
                bestJ = j;
            }

            //  same thing for each of the quadrants
            for (int q=0, q1=1; q<4; q++, q1++)
            {
                corr = cov[q1] * corrScale[q1] * patCorrScale[q1];

                if (corr > quadBestScore[q])
                {
                    quadBestScore[q] = corr;
                    quadBestI[q] = i;
                    quadBestJ[q] = j;
                }
            }                        
        }
    }
            
    Pattern2DMatchRecord_part   *patRec = (Pattern2DMatchRecord_part *) matchRecord;

    //  At the end of the loop, we report the best correlation score and its location.
    //  Note that the "best" is not necessarily good.
    matchRecord->setScore(bestScore);  
    matchRecord->setBestMatch(bestJ, bestI);      
    
    patRec->setBestMatch(UPPER_LEFT_QUAD, quadBestJ[UPPER_LEFT_QUAD], 
                                          quadBestI[UPPER_LEFT_QUAD]);
    patRec->setScore(UPPER_LEFT_QUAD, quadBestScore[UPPER_LEFT_QUAD]);
    //
    patRec->setBestMatch(UPPER_RIGHT_QUAD, quadBestJ[UPPER_RIGHT_QUAD], 
                                           quadBestI[UPPER_RIGHT_QUAD]);
    patRec->setScore(UPPER_RIGHT_QUAD, quadBestScore[UPPER_RIGHT_QUAD]);
    //
    patRec->setBestMatch(LOWER_LEFT_QUAD, quadBestJ[LOWER_LEFT_QUAD], 
                                          quadBestI[LOWER_LEFT_QUAD]);
    patRec->setScore(LOWER_LEFT_QUAD, quadBestScore[LOWER_LEFT_QUAD]);
    //
    patRec->setBestMatch(LOWER_RIGHT_QUAD, quadBestJ[LOWER_RIGHT_QUAD], 
                                           quadBestI[LOWER_RIGHT_QUAD]);
    patRec->setScore(LOWER_RIGHT_QUAD, quadBestScore[LOWER_RIGHT_QUAD]);    
}



//  Partial computation for blobs is more complex than for rectangles.  The
//  vertical partition is easy enough (middle of the deque).  The horizontal
//  partition, however, must be determined separately for each row (list of
//  horizontal segments)
//
//  in the figure below the digits indicate the quadrant index
//
//          +------00-+---------+
//          |      000|111111   |
//          |     0000|1111     |
//          |   0000  | 11111   |
//          | 000  000|  111    |
//          +-00---00-+--111----+    <--- midI
//          22222  222|  333    |
//          |2222   22|  33333  |
//          |  22    2233333333 |
//          |2222     |333  3333|
//          +--22-----+-------333
//
//                    ^
//                    |___ mindJ
//
//
void PatternMatcher2D_partCorr::match_(const RasterImage_RGBa* pat, const Blob* patBlob, 
									   const ImageRect* searchRect, 
									   const RasterImage_RGBa* img, 
									   Pattern2DMatchRecord* matchRecord)
{
//     ImageRect   *patRect = pat->getValidRect();   // unused variable. -CJC
    //
    const float ONE_THIRD = 1.f / 3.f;
    const int   iLow = searchRect->getTop(),
                iHigh = searchRect->getBottom(),
                jLow = searchRect->getLeft(),
                jHigh = searchRect->getRight();
    
    int     bestI=-1, bestJ=-1,
            quadBestI[4] = {-1, -1, -1, -1},
            quadBestJ[4] = {-1, -1, -1, -1};
    int     i, j;
    float   bestScore = -2, quadBestScore[4] = {-2, -2, -2, -2};
    float   patExp[][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    float   cov[][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    float   patCorrScale[][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    float   corrScale[][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}};
    float   corr;
    
    //  Compute the pattern's correlation scale factor
    patternCorrScale_(pat, patBlob, patExp, patCorrScale);

    //  For each possible location of the upper-left corner of the pattern
    for (i=iLow; i<=iHigh; i++)
    {
        for (j=jLow; j<=jHigh; j++)
        {
            //  compute the variance and scaling factor
            covariance_(pat, patBlob, patExp, img, j, i, cov, corrScale);
            
            //  Compute the actual correlation value and compare it to best so far
        
            //  Compute the global correlation value and compare it to best so far
            corr = ONE_THIRD * (cov[0][0] * corrScale[0][0] * patCorrScale[0][0] + 
                                cov[1][0] * corrScale[1][0] * patCorrScale[1][0] + 
                                cov[2][0] * corrScale[2][0] * patCorrScale[2][0]);
            if (corr > bestScore)
            {
                bestScore = corr;
                bestI = i;
                bestJ = j;
            }

            //  same thing for each of the quadrants
            for (int q=0; q<4; q++)
            {
                corr = ONE_THIRD * (cov[0][q+1] * corrScale[0][q+1] * patCorrScale[0][q+1] + 
                                    cov[1][q+1] * corrScale[1][q+1] * patCorrScale[1][q+1] + 
                                    cov[2][q+1] * corrScale[2][q+1] * patCorrScale[2][q+1]);
                if (corr > quadBestScore[q])
                {
                    quadBestScore[q] = corr;
                    quadBestI[q] = i;
                    quadBestJ[q] = j;
                }
            }                        
        }
    }
            
    Pattern2DMatchRecord_part   *patRec = (Pattern2DMatchRecord_part *) matchRecord;

    //  At the end of the loop, we report the best correlation score and its location.
    //  Note that the "best" is not necessarily good.
    matchRecord->setScore(bestScore);  
    matchRecord->setBestMatch(bestJ, bestI);      
    
    patRec->setBestMatch(UPPER_LEFT_QUAD, quadBestJ[UPPER_LEFT_QUAD], 
                                          quadBestI[UPPER_LEFT_QUAD]);
    patRec->setScore(UPPER_LEFT_QUAD, quadBestScore[UPPER_LEFT_QUAD]);
    //
    patRec->setBestMatch(UPPER_RIGHT_QUAD, quadBestJ[UPPER_RIGHT_QUAD], 
                                           quadBestI[UPPER_RIGHT_QUAD]);
    patRec->setScore(UPPER_RIGHT_QUAD, quadBestScore[UPPER_RIGHT_QUAD]);
    //
    patRec->setBestMatch(LOWER_LEFT_QUAD, quadBestJ[LOWER_LEFT_QUAD], 
                                          quadBestI[LOWER_LEFT_QUAD]);
    patRec->setScore(LOWER_LEFT_QUAD, quadBestScore[LOWER_LEFT_QUAD]);
    //
    patRec->setBestMatch(LOWER_RIGHT_QUAD, quadBestJ[LOWER_RIGHT_QUAD], 
                                           quadBestI[LOWER_RIGHT_QUAD]);
    patRec->setScore(LOWER_RIGHT_QUAD, quadBestScore[LOWER_RIGHT_QUAD]);    
}



//  Partial computation for blobs is more complex than for rectangles.  The
//  vertical partition is easy enough (middle of the deque).  The horizontal
//  partition, however, must be determined separately for each row (list of
//  horizontal segments)
//
//  in the figure below the digits indicate the quadrant index
//
//          +------00-+---------+
//          |      000|111111   |
//          |     0000|1111     |
//          |   0000  | 11111   |
//          | 000  000|  111    |
//          +-00---00-+--111----+    <--- midI
//          22222  222|  333    |
//          |2222   22|  33333  |
//          |  22    2233333333 |
//          |2222     |333  3333|
//          +--22-----+-------333
//
//                    ^
//                    |___ mindJ
//
//
void PatternMatcher2D_partCorr::match_(const RasterImage_gray* pat, 
									   const Blob* patBlob,
									   const ImageRect* searchRect,
									   const RasterImage_gray* img,
									   Pattern2DMatchRecord* matchRecord)
{
//     ImageRect   *patRect = pat->getValidRect();   // unused variable. -CJC
    //
//     const float ONE_THIRD = 1.f / 3.f;   // unused variable. -CJC
    const int   iLow = searchRect->getTop(),
                iHigh = searchRect->getBottom(),
                jLow = searchRect->getLeft(),
                jHigh = searchRect->getRight();
    
    int     bestI=-1, bestJ=-1,
            quadBestI[4] = {-1, -1, -1, -1},
            quadBestJ[4] = {-1, -1, -1, -1};
    int     i, j;
    float   bestScore = -2, quadBestScore[4] = {-2, -2, -2, -2};
    float   patExp[5] = {0,0,0,0,0};
    float   cov[5] = {0,0,0,0,0};
    float   patCorrScale[5] = {0,0,0,0,0};
    float   corrScale[5] = {0,0,0,0,0};
    float   corr;
    
    //  Compute the pattern's correlation scale factor
    patternCorrScale_(pat, patBlob, patExp, patCorrScale);

    //  For each possible location of the upper-left corner of the pattern
    for (i=iLow; i<=iHigh; i++)
    {
        for (j=jLow; j<=jHigh; j++)
        {
            //  compute the variance and scaling factor
            covariance_(pat, patBlob, patExp, img, j, i, cov, corrScale);
            
            //  Compute the global correlation value and compare it to best so far
            corr = cov[0] * corrScale[0] * patCorrScale[0];

            if (corr > bestScore)
            {
                bestScore = corr;
                bestI = i;
                bestJ = j;
            }

            //  same thing for each of the quadrants
            for (int q=0, q1=1; q<4; q++, q1++)
            {
                corr = cov[q1] * corrScale[q1] * patCorrScale[q1];

                if (corr > quadBestScore[q])
                {
                    quadBestScore[q] = corr;
                    quadBestI[q] = i;
                    quadBestJ[q] = j;
                }
            }                        
        }
    }
            
    Pattern2DMatchRecord_part   *patRec = (Pattern2DMatchRecord_part *) matchRecord;

    //  At the end of the loop, we report the best correlation score and its location.
    //  Note that the "best" is not necessarily good.
    matchRecord->setScore(bestScore);  
    matchRecord->setBestMatch(bestJ, bestI);      
    
    patRec->setBestMatch(UPPER_LEFT_QUAD, quadBestJ[UPPER_LEFT_QUAD], 
                                          quadBestI[UPPER_LEFT_QUAD]);
    patRec->setScore(UPPER_LEFT_QUAD, quadBestScore[UPPER_LEFT_QUAD]);
    //
    patRec->setBestMatch(UPPER_RIGHT_QUAD, quadBestJ[UPPER_RIGHT_QUAD], 
                                           quadBestI[UPPER_RIGHT_QUAD]);
    patRec->setScore(UPPER_RIGHT_QUAD, quadBestScore[UPPER_RIGHT_QUAD]);
    //
    patRec->setBestMatch(LOWER_LEFT_QUAD, quadBestJ[LOWER_LEFT_QUAD], 
                                          quadBestI[LOWER_LEFT_QUAD]);
    patRec->setScore(LOWER_LEFT_QUAD, quadBestScore[LOWER_LEFT_QUAD]);
    //
    patRec->setBestMatch(LOWER_RIGHT_QUAD, quadBestJ[LOWER_RIGHT_QUAD], 
                                           quadBestI[LOWER_RIGHT_QUAD]);
    patRec->setScore(LOWER_RIGHT_QUAD, quadBestScore[LOWER_RIGHT_QUAD]);    
}

#if 0
#pragma mark -
#endif

void PatternMatcher2D_partCorr::covariance(const RasterImage_RGBa* patImg, float patExp[][5], 
                                           const RasterImage_RGBa* img, int x, int y, float cov[][5], 
                                           float corrScale[][5])
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	patExp == NULL,
                    kNullParameterError,
                    "NULL pattern color expected values sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	cov == NULL,
                    kNullParameterError,
                    "NULL covariance array sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	corrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image sent to PatternMatcher2D_corr::covariance");

    covariance_(patImg, patExp, img, x, y, cov, corrScale);
}

void PatternMatcher2D_partCorr::covariance(const RasterImage_gray* patImg, float* patExp, 
                                           const RasterImage_gray* img, int x, int y, 
                                           float* cov, float* corrScale)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	patExp == NULL,
                    kNullParameterError,
                    "NULL pattern color expected values sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	cov == NULL,
                    kNullParameterError,
                    "NULL covariance array sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	corrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image sent to PatternMatcher2D_corr::covariance");
    
    covariance_(patImg, patExp, img, x, y, cov, corrScale);    
}

void PatternMatcher2D_partCorr::covariance(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                           float patExp[][5], const RasterImage_RGBa* img, 
                                           int x, int y, float cov[][5], float corrScale[][5])
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	patExp == NULL,
                    kNullParameterError,
                    "NULL pattern color expected values sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	cov == NULL,
                    kNullParameterError,
                    "NULL covariance array sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	corrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image sent to PatternMatcher2D_corr::covariance");

    if (patBlob==NULL)
        covariance_(patImg, patExp, img, x, y, cov, corrScale);
    else
        covariance_(patImg, patBlob, patExp, img, x, y, cov, corrScale);

}

void PatternMatcher2D_partCorr::covariance(const RasterImage_gray* patImg, const Blob* patBlob, 
                                           float* patExp, const RasterImage_gray* img, 
                                           int x, int y, float* cov, float* corrScale)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	patExp == NULL,
                    kNullParameterError,
                    "NULL pattern color expected values sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	cov == NULL,
                    kNullParameterError,
                    "NULL covariance array sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	corrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_corr::covariance");
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image sent to PatternMatcher2D_corr::covariance");

    if (patBlob==NULL)
        covariance_(patImg, patExp, img, x, y, cov, corrScale);
    else
        covariance_(patImg, patBlob, patExp, img, x, y, cov, corrScale);

}

void PatternMatcher2D_partCorr::covariance_(const RasterImage_RGBa* patImg, float patExp[][5], 
                                            const RasterImage_RGBa* img, int x, int y, float cov[][5], 
                                            float corrScale[][5])
{
    const ImageRect   *patRect = patImg->getValidRect();

    const int   patHeight = patRect->getHeight(),
                patWidth = patRect->getWidth(),
                nbPatPixels = patHeight * patWidth;
    const int   midI = patHeight/2,
                midJ = patWidth/2;
    const int   quadTop[4] = {0, 0, midI + 1, midI + 1},
                quadBottom[4] = {midI, midI, patHeight-1, patHeight-1},
                quadLeft[4] = {0, midJ + 1, 0, midJ + 1},
                quadRight[4] = {midJ, patWidth-1, midJ, patWidth-1};
    const float nbPixelsScale = 1.f / nbPatPixels;
    const float quadPixelsScale[4] = {1.f/((midI+1)*(midJ+1)), 
                                      1.f/((midI+1)*(patWidth-midJ-1)),
                                      1.f/((patHeight-midI-1)*(midJ+1)), 
                                      1.f/((patHeight-midI-1)*(patWidth-midJ-1))};
    const unsigned char* const* rgbaPat = patImg->getRGBa2D();
    const unsigned char* const* rgbaImg = img->getShiftedRGBa2D();

    //-------------------------------------------------------------------------
    //  Part 1: Compute expected value and scaling factor for this part of the 
    //          image
    //-------------------------------------------------------------------------
    float   imgQuadRedSum[4] = {0, 0, 0, 0}, imgQuadRedSumSq[4] = {0, 0, 0, 0}, 
            imgQuadRedExp[4];
    float   imgQuadGreenSum[4] = {0, 0, 0, 0}, imgQuadGreenSumSq[4] = {0, 0, 0, 0}, 
            imgQuadGreenExp[4];
    float   imgQuadBlueSum[4] = {0, 0, 0, 0}, imgQuadBlueSumSq[4] = {0, 0, 0, 0}, 
            imgQuadBlueExp[4];
    float   imgRedSum = 0, imgRedSumSq = 0, imgRedExp;
    float   imgGreenSum = 0, imgGreenSumSq = 0, imgGreenExp;
    float   imgBlueSum = 0, imgBlueSumSq = 0, imgBlueExp;
    //  for each quadrant of the pattern
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        const int   kMin = y + quadTop[q], kMax = y + quadBottom[q],
                    lMin = x + quadLeft[q], lMax = x + quadRight[q];
        for (int kI=kMin; kI<=kMax; kI++)
            for (int lI=lMin, fourLI=4*lI; lI<=lMax; lI++, fourLI+=2)
            {
                //  red component
                imgQuadRedSum[q] += rgbaImg[kI][fourLI];
                imgQuadRedSumSq[q] += rgbaImg[kI][fourLI]*rgbaImg[kI][fourLI];
                //  green component
                imgQuadGreenSum[q] += rgbaImg[kI][++fourLI];
                imgQuadGreenSumSq[q] += rgbaImg[kI][fourLI]*rgbaImg[kI][fourLI];
                //  blue component
                imgQuadBlueSum[q] += rgbaImg[kI][++fourLI];
                imgQuadBlueSumSq[q] += rgbaImg[kI][fourLI]*rgbaImg[kI][fourLI];
            }
        imgQuadRedExp[q] = imgQuadRedSum[q] * quadPixelsScale[q];
        corrScale[0][q1] = 1.f / sqrtf(imgQuadRedSumSq[q]*quadPixelsScale[q] - 
                                            imgQuadRedExp[q]*imgQuadRedExp[q]);
        imgQuadGreenExp[q] = imgQuadGreenSum[q] * quadPixelsScale[q];
        corrScale[1][q1] = 1.f / sqrtf(imgQuadGreenSumSq[q]*quadPixelsScale[q] - 
                                            imgQuadGreenExp[q]*imgQuadGreenExp[q]);
        imgQuadBlueExp[q] = imgQuadBlueSum[q] * quadPixelsScale[q];
        corrScale[2][q1] = 1.f / sqrtf(imgQuadBlueSumSq[q]*quadPixelsScale[q] - 
                                            imgQuadBlueExp[q]*imgQuadBlueExp[q]);

        //  update of the summation and scaling factor for the whole part of the
        //  image
        imgRedSum += imgQuadRedSum[q];
        imgRedSumSq += imgQuadRedSumSq[q];
        imgGreenSum += imgQuadGreenSum[q];
        imgGreenSumSq += imgQuadGreenSumSq[q];
        imgBlueSum += imgQuadBlueSum[q];
        imgBlueSumSq += imgQuadBlueSumSq[q];
    }
    //  compute the global expexted value and scaling factors
    imgRedExp = imgRedSum * nbPixelsScale;
    corrScale[0][0] = 1.f / sqrtf(imgRedSumSq*nbPixelsScale - imgRedExp*imgRedExp);
    imgGreenExp = imgGreenSum * nbPixelsScale;
    corrScale[1][0] = 1.f / sqrtf(imgGreenSumSq*nbPixelsScale - imgGreenExp*imgGreenExp);
    imgBlueExp = imgBlueSum * nbPixelsScale;
    corrScale[2][0] = 1.f / sqrtf(imgBlueSumSq*nbPixelsScale - imgBlueExp*imgBlueExp);

    //-------------------------------------------------------------------------
    //  Part 2: Now actually compute the covariance
    //-------------------------------------------------------------------------
    float   covRedSum = 0, covGreenSum = 0, covBlueSum = 0;
    float   covQuadRedSum[4] = {0, 0, 0, 0}, covQuadGreenSum[4] = {0, 0, 0, 0}, 
            covQuadBlueSum[4] = {0, 0, 0, 0};

    //  for each quadrant of the pattern
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        //  Now calculate the correlation for the image region selected              
        for (int kP=quadTop[q], kI=y+kP; kP<=quadBottom[q]; kP++, kI++)
        {
            for (int lP=quadLeft[q], lI=x+lP, fourLP=4*lP, fourLI=4*lI; lP<=quadRight[q]; lP++, lI++)
            {
                //  red component
                covQuadRedSum[q] += (rgbaPat[kP][fourLP] - patExp[0][q1]) * 
                                     (rgbaImg[kI][fourLI] - imgQuadRedExp[q]);
                covRedSum += (rgbaPat[kP][fourLP] - patExp[0][0]) * 
                              (rgbaImg[kI][fourLI] - imgRedExp);
                
                //  green component
                fourLP++; fourLI++;
                covQuadGreenSum[q] += (rgbaPat[kP][fourLP] - patExp[1][q1]) * 
                                       (rgbaImg[kI][fourLI] - imgQuadGreenExp[q]);
                covGreenSum += (rgbaPat[kP][fourLP] - patExp[1][0]) * 
                                (rgbaImg[kI][fourLI] - imgGreenExp);
                
                //  blue component
                fourLP++; fourLI++;
                covQuadBlueSum[q] += (rgbaPat[kP][fourLP] - patExp[2][q1]) * 
                                      (rgbaImg[kI][fourLI] - imgQuadBlueExp[q]);
                covBlueSum += (rgbaPat[kP][fourLP] - patExp[2][0]) * 
                           (rgbaImg[kI][fourLI] - imgBlueExp);
                
                fourLP+=2; fourLI+=2;
            }
        }
        
        cov[0][q1] = covQuadRedSum[q] * quadPixelsScale[q];
        cov[1][q1] = covQuadGreenSum[q] * quadPixelsScale[q];
        cov[2][q1] = covQuadBlueSum[q] * quadPixelsScale[q];
    }
    
    cov[0][0] = covRedSum * nbPixelsScale;
    cov[1][0] = covGreenSum * nbPixelsScale;
    cov[2][0] = covBlueSum * nbPixelsScale;
}

void PatternMatcher2D_partCorr::covariance_(const RasterImage_gray* patImg, float* patExp, 
                                            const RasterImage_gray* img, int x, int y, 
                                            float* cov, float* corrScale)
{
    const ImageRect   *patRect = patImg->getValidRect();

    const int   patHeight = patRect->getHeight(),
                patWidth = patRect->getWidth(),
                nbPatPixels = patHeight * patWidth;
    const int   midI = patHeight/2,
                midJ = patWidth/2;
    const int   quadTop[4] = {0, 0, midI + 1, midI + 1},
                quadBottom[4] = {midI, midI, patHeight-1, patHeight-1},
                quadLeft[4] = {0, midJ + 1, 0, midJ + 1},
                quadRight[4] = {midJ, patWidth-1, midJ, patWidth-1};
    const float nbPixelsScale = 1.f / nbPatPixels;
    const float quadPixelsScale[4] = {1.f/((midI+1)*(midJ+1)), 
                                      1.f/((midI+1)*(patWidth-midJ-1)),
                                      1.f/((patHeight-midI-1)*(midJ+1)), 
                                      1.f/((patHeight-midI-1)*(patWidth-midJ-1))};
    const unsigned char* const* grayPat = patImg->getGray2D();
    const unsigned char* const* grayImg = img->getShiftedGray2D();

    //-------------------------------------------------------------------------
    //  Part 1: Compute expected value and scaling factor for this part of the 
    //          image
    //-------------------------------------------------------------------------
    float   imgQuadSum[4] = {0, 0, 0, 0}, imgQuadSumSq[4] = {0, 0, 0, 0}, 
            imgQuadExp[4];
    float   imgSum = 0, imgSumSq = 0, imgExp;

    //  for each quadrant of the pattern
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        //  Calculate the mean and scaling factor for this part of the 
        //  image
        const int   kMin = y + quadTop[q], kMax = y + quadBottom[q],
                    lMin = x + quadLeft[q], lMax = x + quadRight[q];
        for (int kI=kMin; kI<=kMax; kI++)
            for (int lI=lMin; lI<=lMax; lI++)
            {
                imgQuadSum[q] += grayImg[kI][lI];
                imgQuadSumSq[q] += grayImg[kI][lI]*grayImg[kI][lI];
            }

        //  mean and scaling factor for this quadrant
        imgQuadExp[q] = imgQuadSum[q] * quadPixelsScale[q];
        corrScale[q1] = 1.f / sqrtf(imgQuadSumSq[q]*quadPixelsScale[q] - 
                                    imgQuadExp[q]*imgQuadExp[q]);

        //  update of the summation and scaling factor for the whole part of the
        //  image
        imgSum += imgQuadSum[q];
        imgSumSq += imgQuadSumSq[q];
    }
    //  compute the global expexted value and scaling factor
    imgExp = imgSum * nbPixelsScale;
    corrScale[0] = 1.f / sqrtf(imgSumSq*nbPixelsScale - imgExp*imgExp);

    //-------------------------------------------------------------------------
    //  Part 2: Now actually compute the covariance
    //-------------------------------------------------------------------------
    float   covQuadSum[4] = {0, 0, 0, 0}, covSum = 0;
    //  for each quadrant of the pattern
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        //  Now calculate the correlation for the image region selected              
        for (int kP=quadTop[q], kI=y+kP; kP<=quadBottom[q]; kP++, kI++)
        {
            for (int lP=quadLeft[q], lI=x+lP; lP<=quadRight[q]; lP++, lI++)
            {
                covQuadSum[q] += (grayPat[kP][lP] - patExp[q1]) *  
                                  (grayImg[kI][lI] - imgQuadExp[q]);
            
                covSum += (grayPat[kP][lP] - patExp[0]) * (grayImg[kI][lI] - imgExp);
            }    
        }
        
        cov[q1] = covQuadSum[q] * quadPixelsScale[q];
    }
    //  Compute the actual correlation value and compare it to best so far
    cov[0] = covSum * nbPixelsScale;
}

void PatternMatcher2D_partCorr::covariance_(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                            float patExp[][5], const RasterImage_RGBa* img, 
                                            int x, int y, float cov[][5], float corrScale[][5])
{
    const int   patHeight = patBlob->getHeight(),
                patWidth = patBlob->getWidth(),
                nbPatPixels = patBlob->getNbPixels();
    const int   midI = patHeight/2,
                midJ = patWidth/2,
                halfTop[2] = {0, midI+1},
                halfBottom[2] = {midI, patHeight-1};
    const float quadPixelsScale[4] = {  1.f/patBlob->getNbPixels(UPPER_LEFT_QUAD),
                                        1.f/patBlob->getNbPixels(UPPER_RIGHT_QUAD),
                                        1.f/patBlob->getNbPixels(LOWER_LEFT_QUAD),
                                        1.f/patBlob->getNbPixels(LOWER_RIGHT_QUAD)};
    const float nbPixelsScale = 1.f / nbPatPixels;

    const unsigned char* const* rgbaPat = patImg->getRGBa2D();
    const unsigned char* const* rgbaImg = img->getShiftedRGBa2D();
    const LineSegDeque segDeque = patBlob->getSegDeque();

    float   imgQuadRedSum[4] = {0, 0, 0, 0}, imgQuadRedSumSq[4] = {0, 0, 0, 0}, 
            imgQuadRedExp[4];
    float   imgQuadGreenSum[4] = {0, 0, 0, 0}, imgQuadGreenSumSq[4] = {0, 0, 0, 0}, 
            imgQuadGreenExp[4];
    float   imgQuadBlueSum[4] = {0, 0, 0, 0}, imgQuadBlueSumSq[4] = {0, 0, 0, 0}, 
            imgQuadBlueExp[4];
    float   imgRedSum = 0, imgRedSumSq = 0, imgRedExp;
    float   imgGreenSum = 0, imgGreenSumSq = 0, imgGreenExp;
    float   imgBlueSum = 0, imgBlueSumSq = 0, imgBlueExp;

    
    //-------------------------------------------------------------------------
    //  Part 1: Compute expected value and scaling factor for this part of the 
    //          image
    //-------------------------------------------------------------------------
    //  Work on the top half, then the bottom half
    for (int qHalf=0; qHalf<2; qHalf++)
    {
        LineSegDeque::const_iterator lineIter = segDeque.begin() + halfTop[qHalf];
    
        for (int i=halfTop[qHalf]; i<=halfBottom[qHalf]; i++, lineIter++)
        {
            //  reinitialize q to be the left quadrant of the half being processed
            int  q = 2*qHalf, q1 = q + 1,
                 startX, endX;
            bool leftMode = true, segRightHalf = false;   
            list<HorizontalSegment*>::iterator  segIter = (*lineIter)->begin(), 
                                                endSeg = (*lineIter)->end();
            while (segIter != endSeg)
            {
                HorizontalSegment   *theSeg = (*segIter);
                int kI = y + theSeg->y;

                //  We are processing the left half, 
                if (leftMode)
                {
                    startX = x + theSeg->leftX;
                    
                    //  if the segment is entirely on the left half, just process the
                    //  whole segment and move to the next
                    if (theSeg->rightX <= midJ)
                    {
                        endX = x + theSeg->rightX;
                        segIter++;
                    }
                    else
                    {
                        //  this segment is completely on the right side
                        if (theSeg->leftX > midJ)
                        {
                            //  we are not in left mode anymore; data contribute to 
                            //  right half calculations
                            leftMode = false;
                            q1 = q++;
                            endX = x + theSeg->rightX;
                            segIter++;
                        }
                        //  if we have a split segment, this will be processed in two
                        //  passes of the loop.  We set flags to spot this case in the
                        //  next pass.
                        else
                        {
                            leftMode = false;
                            segRightHalf = true;
                            endX = x + midJ;
                        }
                    }
                } 
                //  if we are in the right mode, we only have to check if we are completing
                //  the right half of a straddling segment
                else
                {
                    endX = x + theSeg->rightX;
                    
                    //  if we are processingt the right half of a straddling segment, then 
                    //  we start from the middle border and start adding to the right half
                    if (segRightHalf)
                    {
                        segRightHalf = false;
                        startX = x + midJ + 1;
                        q1 = q++;
                    }
                    else
                        startX = x + theSeg->leftX;
                    
                    segIter++;
                }
                
                //  Now that we have gone through all this trouble we know what quadrant
                //  this segment (or segment fragment) belongs.  We can now use the data
                //  in our summation
                for (int lI = startX, fourLI=4*lI; lI<=endX; lI++, fourLI+=2)
                {
                    //  red component
                    imgQuadRedSum[q] += rgbaImg[kI][fourLI];
                    imgQuadRedSumSq[q] += rgbaImg[kI][fourLI]*rgbaImg[kI][fourLI];
                    //  green component
                    imgQuadGreenSum[q] += rgbaImg[kI][++fourLI];
                    imgQuadGreenSumSq[q] += rgbaImg[kI][fourLI]*rgbaImg[kI][fourLI];
                    //  blue component
                    imgQuadBlueSum[q] += rgbaImg[kI][++fourLI];
                    imgQuadBlueSumSq[q] += rgbaImg[kI][fourLI]*rgbaImg[kI][fourLI];
                }
            }
        }
    }

    //  We can now compute the expected values and correlation scales
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        imgQuadRedExp[q] = imgQuadRedSum[q] * quadPixelsScale[q];
        corrScale[0][q1] = 1.f / sqrtf(imgQuadRedSumSq[q]*quadPixelsScale[q] - 
                                            imgQuadRedExp[q]*imgQuadRedExp[q]);
        imgQuadGreenExp[q] = imgQuadGreenSum[q] * quadPixelsScale[q];
        corrScale[1][q1] = 1.f / sqrtf(imgQuadGreenSumSq[q]*quadPixelsScale[q] - 
                                            imgQuadGreenExp[q]*imgQuadGreenExp[q]);
        imgQuadBlueExp[q] = imgQuadBlueSum[q] * quadPixelsScale[q];
        corrScale[2][q1] = 1.f / sqrtf(imgQuadBlueSumSq[q]*quadPixelsScale[q] - 
                                            imgQuadBlueExp[q]*imgQuadBlueExp[q]);

        //  update of the summation and scaling factor for the whole part of the
        //  image
        imgRedSum += imgQuadRedSum[q];
        imgRedSumSq += imgQuadRedSumSq[q];
        imgGreenSum += imgQuadGreenSum[q];
        imgGreenSumSq += imgQuadGreenSumSq[q];
        imgBlueSum += imgQuadBlueSum[q];
        imgBlueSumSq += imgQuadBlueSumSq[q];
    }
    //  compute the global expexted value and scaling factors
    imgRedExp = imgRedSum * nbPixelsScale;
    corrScale[0][0] = 1.f / sqrtf(imgRedSumSq*nbPixelsScale - imgRedExp*imgRedExp);
    imgGreenExp = imgGreenSum * nbPixelsScale;
    corrScale[1][0] = 1.f / sqrtf(imgGreenSumSq*nbPixelsScale - imgGreenExp*imgGreenExp);
    imgBlueExp = imgBlueSum * nbPixelsScale;
    corrScale[2][0] = 1.f / sqrtf(imgBlueSumSq*nbPixelsScale - imgBlueExp*imgBlueExp);


    //-------------------------------------------------------------------------
    //  Part 2: Now actually compute the covariance
    //-------------------------------------------------------------------------
    float   covRedSum = 0, covGreenSum = 0, covBlueSum = 0;
    float   covQuadRedSum[4] = {0, 0, 0, 0}, covQuadGreenSum[4] = {0, 0, 0, 0}, 
            covQuadBlueSum[4] = {0, 0, 0, 0};

    //  Work on the top half, then the bottom half
    for (int qHalf=0; qHalf<2; qHalf++)
    {
        LineSegDeque::const_iterator lineIter = segDeque.begin() + halfTop[qHalf];
    
        for (int i=halfTop[qHalf]; i<=halfBottom[qHalf]; i++, lineIter++)
        {
            //  reinitialize q to be the left quadrant of the half being processed
            int  q = 2*qHalf, q1 = q+1,
                 startX, endX;
            bool leftMode = true, segRightHalf = false;   
            list<HorizontalSegment*>::iterator  segIter = (*lineIter)->begin(), 
                                                endSeg = (*lineIter)->end();
            while (segIter != endSeg)
            {
                HorizontalSegment   *theSeg = (*segIter);
                int kP = theSeg->y;
                int kI = y + kP;

                //  We are processing the left half, 
                if (leftMode)
                {
                    startX = theSeg->leftX;
                    
                    //  if the segment is entirely on the left half, just process the
                    //  whole segment and move to the next
                    if (theSeg->rightX <= midJ)
                    {
                        endX = theSeg->rightX;
                        segIter++;
                    }
                    else
                    {
                        //  this segment is completely on the right side
                        if (theSeg->leftX > midJ)
                        {
                            //  we are not in left mode anymore; data contribute to 
                            //  right half calculations
                            leftMode = false;
                            q1 = q++;
                            endX = theSeg->rightX;
                            segIter++;
                        }
                        //  if we have a split segment, this will be processed in two
                        //  passes of the loop.  We set flags to spot this case in the
                        //  next pass.
                        else
                        {
                            leftMode = false;
                            segRightHalf = true;
                            endX = midJ;
                        }
                    }
                } 
                //  if we are in the right mode, we only have to check if we are completing
                //  the right half of a straddling segment
                else
                {
                    endX = theSeg->rightX;
                    
                    //  if we are processingt the right half of a straddling segment, then 
                    //  we start from the middle border and start adding to the right half
                    if (segRightHalf)
                    {
                        segRightHalf = false;
                        startX = midJ + 1;
                        q1 = q++;
                    }
                    else
                        startX = theSeg->leftX;
                    
                    segIter++;
                }

                //  Now that we have gone through all this trouble we know what quadrant
                //  this segment (or segment fragment) belongs.  We can now use the data
                //  in our summation
                for (int lP=startX, lI=x+lP, fourLP=4*lP, fourLI=4*lI; lP<=endX; lP++, lI++)
                {
                    //  red component
                    covQuadRedSum[q] += (rgbaPat[kP][fourLP] - patExp[0][q1]) * 
                                         (rgbaImg[kI][fourLI] - imgQuadRedExp[q]);
                    covRedSum += (rgbaPat[kP][fourLP] - patExp[0][0]) * 
                                  (rgbaImg[kI][fourLI] - imgRedExp);
                    
                    //  green component
                    fourLP++; fourLI++;
                    covQuadGreenSum[q] += (rgbaPat[kP][fourLP] - patExp[1][q1]) * 
                                           (rgbaImg[kI][fourLI] - imgQuadGreenExp[q]);
                    covGreenSum += (rgbaPat[kP][fourLP] - patExp[1][0]) * 
                                    (rgbaImg[kI][fourLI] - imgGreenExp);
                    
                    //  blue component
                    fourLP++; fourLI++;
                    covQuadBlueSum[q] += (rgbaPat[kP][fourLP] - patExp[2][q1]) * 
                                          (rgbaImg[kI][fourLI] - imgQuadBlueExp[q]);
                    covBlueSum += (rgbaPat[kP][fourLP] - patExp[2][0]) * 
                               (rgbaImg[kI][fourLI] - imgBlueExp);
                    
                    fourLP+=2; fourLI+=2;
                }
            }
        }
    }

    for (int q=0, q1=1; q<4; q++, q1++)
    {
        cov[0][q1] = covQuadRedSum[q] * quadPixelsScale[q];
        cov[1][q1] = covQuadGreenSum[q] * quadPixelsScale[q];
        cov[2][q1] = covQuadBlueSum[q] * quadPixelsScale[q];
    }
    
    cov[0][0] = covRedSum * nbPixelsScale;
    cov[1][0] = covGreenSum * nbPixelsScale;
    cov[2][0] = covBlueSum * nbPixelsScale;
}

void PatternMatcher2D_partCorr::covariance_(const RasterImage_gray* patImg, const Blob* patBlob, 
                                            float* patExp, const RasterImage_gray* img, 
                                            int x, int y, float* cov, float* corrScale)
{
    const int   patHeight = patBlob->getHeight(),
                patWidth = patBlob->getWidth(),
                nbPatPixels = patBlob->getNbPixels();
    const int   midI = patHeight/2,
                midJ = patWidth/2,
                halfTop[2] = {0, midI+1},
                halfBottom[2] = {midI, patHeight-1};
    const float quadPixelsScale[4] = {  1.f/patBlob->getNbPixels(UPPER_LEFT_QUAD),
                                        1.f/patBlob->getNbPixels(UPPER_RIGHT_QUAD),
                                        1.f/patBlob->getNbPixels(LOWER_LEFT_QUAD),
                                        1.f/patBlob->getNbPixels(LOWER_RIGHT_QUAD)};
    const float nbPixelsScale = 1.f / nbPatPixels;

    const unsigned char* const* grayPat = patImg->getGray2D();
    const unsigned char* const* grayImg = img->getShiftedGray2D();
    const LineSegDeque segDeque = patBlob->getSegDeque();

    float   imgQuadSum[4] = {0, 0, 0, 0}, imgQuadSumSq[4] = {0, 0, 0, 0}, 
            imgQuadExp[4];
    float   imgSum = 0, imgSumSq = 0, imgExp;

    
    //-------------------------------------------------------------------------
    //  Part 1: Compute expected value and scaling factor for this part of the 
    //          image
    //-------------------------------------------------------------------------
    //  Work on the top half, then the bottom half
    for (int qHalf=0; qHalf<2; qHalf++)
    {
        LineSegDeque::const_iterator lineIter = segDeque.begin() + halfTop[qHalf];
    
        for (int i=halfTop[qHalf]; i<=halfBottom[qHalf]; i++, lineIter++)
        {
            //  reinitialize q to be the left quadrant of the half being processed
            int  q = 2*qHalf, q1 = q + 1,
                 startX, endX;
            bool leftMode = true, segRightHalf = false;   
            list<HorizontalSegment*>::iterator  segIter = (*lineIter)->begin(), 
                                                endSeg = (*lineIter)->end();
            while (segIter != endSeg)
            {
                HorizontalSegment   *theSeg = (*segIter);
                int kI = y + theSeg->y;

                //  We are processing the left half, 
                if (leftMode)
                {
                    startX = x + theSeg->leftX;
                    
                    //  if the segment is entirely on the left half, just process the
                    //  whole segment and move to the next
                    if (theSeg->rightX <= midJ)
                    {
                        endX = x + theSeg->rightX;
                        segIter++;
                    }
                    else
                    {
                        //  this segment is completely on the right side
                        if (theSeg->leftX > midJ)
                        {
                            //  we are not in left mode anymore; data contribute to 
                            //  right half calculations
                            leftMode = false;
                            q1 = q++;
                            endX = x + theSeg->rightX;
                            segIter++;
                        }
                        //  if we have a split segment, this will be processed in two
                        //  passes of the loop.  We set flags to spot this case in the
                        //  next pass.
                        else
                        {
                            leftMode = false;
                            segRightHalf = true;
                            endX = x + midJ;
                        }
                    }
                } 
                //  if we are in the right mode, we only have to check if we are completing
                //  the right half of a straddling segment
                else
                {
                    endX = x + theSeg->rightX;
                    
                    //  if we are processingt the right half of a straddling segment, then 
                    //  we start from the middle border and start adding to the right half
                    if (segRightHalf)
                    {
                        segRightHalf = false;
                        startX = x + midJ + 1;
                        q1 = q++;
                    }
                    else
                        startX = x + theSeg->leftX;
                    
                    segIter++;
                }
                
                //  Now that we have gone through all this trouble we know what quadrant
                //  this segment (or segment fragment) belongs.  We can now use the data
                //  in our summation
                for (int lI = startX; lI<=endX; lI++)
                {
                    imgQuadSum[q] += grayImg[kI][lI];
                    imgQuadSumSq[q] += grayImg[kI][lI]*grayImg[kI][lI];
                }
            }
        }
    }

    //  We can now compute the expected values and correlation scales
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        imgQuadExp[q] = imgQuadSum[q] * quadPixelsScale[q];
        corrScale[q1] = 1.f / sqrtf(imgQuadSumSq[q]*quadPixelsScale[q] - 
                                            imgQuadExp[q]*imgQuadExp[q]);

        //  update of the summation and scaling factor for the whole part of the
        //  image
        imgSum += imgQuadSum[q];
        imgSumSq += imgQuadSumSq[q];
    }
    //  compute the global expexted value and scaling factors
    imgExp = imgSum * nbPixelsScale;
    corrScale[0] = 1.f / sqrtf(imgSumSq*nbPixelsScale - imgExp*imgExp);


    //-------------------------------------------------------------------------
    //  Part 2: Now actually compute the covariance
    //-------------------------------------------------------------------------
    float   covSum = 0,
            covQuadSum[4] = {0, 0, 0, 0};

    //  Work on the top half, then the bottom half
    for (int qHalf=0; qHalf<2; qHalf++)
    {
        LineSegDeque::const_iterator lineIter = segDeque.begin() + halfTop[qHalf];
    
        for (int i=halfTop[qHalf]; i<=halfBottom[qHalf]; i++, lineIter++)
        {
            //  reinitialize q to be the left quadrant of the half being processed
            int  q = 2*qHalf, q1 = q+1,
                 startX, endX;
            bool leftMode = true, segRightHalf = false;   
            list<HorizontalSegment*>::iterator  segIter = (*lineIter)->begin(), 
                                                endSeg = (*lineIter)->end();
            while (segIter != endSeg)
            {
                HorizontalSegment   *theSeg = (*segIter);
                int kP = theSeg->y;
                int kI = y + kP;

                //  We are processing the left half, 
                if (leftMode)
                {
                    startX = theSeg->leftX;
                    
                    //  if the segment is entirely on the left half, just process the
                    //  whole segment and move to the next
                    if (theSeg->rightX <= midJ)
                    {
                        endX = theSeg->rightX;
                        segIter++;
                    }
                    else
                    {
                        //  this segment is completely on the right side
                        if (theSeg->leftX > midJ)
                        {
                            //  we are not in left mode anymore; data contribute to 
                            //  right half calculations
                            leftMode = false;
                            q1 = q++;
                            endX = theSeg->rightX;
                            segIter++;
                        }
                        //  if we have a split segment, this will be processed in two
                        //  passes of the loop.  We set flags to spot this case in the
                        //  next pass.
                        else
                        {
                            leftMode = false;
                            segRightHalf = true;
                            endX = midJ;
                        }
                    }
                } 
                //  if we are in the right mode, we only have to check if we are completing
                //  the right half of a straddling segment
                else
                {
                    endX = theSeg->rightX;
                    
                    //  if we are processingt the right half of a straddling segment, then 
                    //  we start from the middle border and start adding to the right half
                    if (segRightHalf)
                    {
                        segRightHalf = false;
                        startX = midJ + 1;
                        q1 = q++;
                    }
                    else
                        startX = theSeg->leftX;
                    
                    segIter++;
                }

                //  Now that we have gone through all this trouble we know what quadrant
                //  this segment (or segment fragment) belongs.  We can now use the data
                //  in our summation
                for (int lP=startX, lI=x+lP; lP<=endX; lP++, lI++)
                {
                    //  red component
                    covQuadSum[q] += (grayImg[kP][lP] - patExp[q1]) * 
                                         (grayImg[kI][lI] - imgQuadExp[q]);
                    covSum += (grayPat[kP][lP] - patExp[0]) * (grayImg[kI][lI] - imgExp);
                }
            }
        }
    }

    for (int q=0, q1=1; q<4; q++, q1++)
        cov[q1] = covQuadSum[q] * quadPixelsScale[q];
    
    cov[0] = covSum * nbPixelsScale;
}

#if 0
#pragma mark -
#endif

void PatternMatcher2D_partCorr::patternCorrScale(const RasterImage_RGBa* patImg, float patExp[][5], 
                                                float patCorrScale[][5])
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_corr::patternCorrScale");
    FAIL_CONDITION(	patExp == NULL,
                    kNullParameterError,
                    "NULL pattern color expected values sent to PatternMatcher2D_corr::patternCorrScale");
    FAIL_CONDITION(	patCorrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_corr::patternCorrScale");
    
    patternCorrScale_(patImg, patExp, patCorrScale);
}

void PatternMatcher2D_partCorr::patternCorrScale(const RasterImage_gray* patImg, float* patExp, 
                                                float* patCorrScale)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_corr::patternCorrScale");
    FAIL_CONDITION(	patExp == NULL,
                    kNullParameterError,
                    "NULL pattern color expected values sent to PatternMatcher2D_corr::patternCorrScale");
    FAIL_CONDITION(	patCorrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_corr::patternCorrScale");
    
    patternCorrScale_(patImg, patExp, patCorrScale);
}

void PatternMatcher2D_partCorr::patternCorrScale(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                                float patExp[][5], float patCorrScale[][5])
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_corr::patternCorrScale");
    FAIL_CONDITION(	patExp == NULL,
                    kNullParameterError,
                    "NULL pattern color expected values sent to PatternMatcher2D_corr::patternCorrScale");
    FAIL_CONDITION(	patCorrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_corr::patternCorrScale");

    if (patBlob == NULL)        
        patternCorrScale_(patImg, patExp, patCorrScale);
    else
        patternCorrScale_(patImg, patBlob, patExp, patCorrScale);

}

void PatternMatcher2D_partCorr::patternCorrScale(const RasterImage_gray* patImg, const Blob* patBlob,
                                                 float* patExp, float* patCorrScale)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL pattern passed as parameter to PatternMatcher2D_corr::patternCorrScale");
    FAIL_CONDITION(	patExp == NULL,
                    kNullParameterError,
                    "NULL pattern color expected values sent to PatternMatcher2D_corr::patternCorrScale");
    FAIL_CONDITION(	patCorrScale == NULL,
                    kNullParameterError,
                    "NULL correlation scale array sent to PatternMatcher2D_corr::patternCorrScale");

    if (patBlob == NULL)        
        patternCorrScale_(patImg, patExp, patCorrScale);
    else
        patternCorrScale_(patImg, patBlob, patExp, patCorrScale);

}


void PatternMatcher2D_partCorr::patternCorrScale_(const RasterImage_RGBa* patImg, float patExp[][5], 
                                                float patCorrScale[][5])
{
    const ImageRect   *patRect = patImg->getValidRect();
    //
    const int   patHeight = patRect->getHeight(),
                patWidth = patRect->getWidth(),
                nbPatPixels = patHeight * patWidth;
    const int   midI = patHeight/2,
                midJ = patWidth/2;
    const int   quadTop[4] = {0, 0, midI + 1, midI + 1},
                quadBottom[4] = {midI, midI, patHeight-1, patHeight-1},
                quadLeft[4] = {0, midJ + 1, 0, midJ + 1},
                quadRight[4] = {midJ, patWidth-1, midJ, patWidth-1};
    const float nbPixelsScale = 1.f / nbPatPixels;
    const float quadPixelsScale[4] = {1.f/((midI+1)*(midJ+1)), 
                                      1.f/((midI+1)*(patWidth-midJ-1)),
                                      1.f/((patHeight-midI-1)*(midJ+1)), 
                                      1.f/((patHeight-midI-1)*(patWidth-midJ-1))};
    const unsigned char* const* rgbaPat = patImg->getRGBa2D();
    
    //  Calculate normalizing factor for the pattern for each of the quadrants.  Then sum up for
    //  the entire pattern.  I know that we are off by a factor
    //  of 1/sqrt(nbPixels).   I correct that when I multiply the two scaling factors
    float   quadRedSum[4] = {0, 0, 0, 0}, quadRedSumSq[4] = {0, 0, 0, 0}; 
    float   quadGreenSum[4] = {0, 0, 0, 0}, quadGreenSumSq[4] = {0, 0, 0, 0};
    float   quadBlueSum[4] = {0, 0, 0, 0}, quadBlueSumSq[4] = {0, 0, 0, 0};
    float   patRedSum = 0, patRedSumSq = 0;
    float   patGreenSum = 0, patGreenSumSq = 0;
    float   patBlueSum = 0, patBlueSumSq = 0;
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        for (int kP=quadTop[q]; kP<=quadBottom[q]; kP++)
            for (int lP=quadLeft[q], fourLP=4*lP; lP<=quadRight[q]; lP++, fourLP+=2)
            {
                //  red component
                quadRedSum[q] += rgbaPat[kP][fourLP];
                quadRedSumSq[q] += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
                //  green component
                quadGreenSum[q] += rgbaPat[kP][++fourLP];
                quadGreenSumSq[q] += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
                //  blue component
                quadBlueSum[q] += rgbaPat[kP][++fourLP];
                quadBlueSumSq[q] += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
            }

        patExp[0][q1] = quadRedSum[q] * quadPixelsScale[q];
        patCorrScale[0][q1] = 1.f / sqrtf(quadRedSumSq[q]*quadPixelsScale[q] - 
                                      patExp[0][q1]*patExp[0][q1]);
        patExp[1][q1] = quadGreenSum[q] * quadPixelsScale[q];
        patCorrScale[1][q1] = 1.f / sqrtf(quadGreenSumSq[q]*quadPixelsScale[q] - 
                                        patExp[1][q1]*patExp[1][q1]);
        patExp[2][q1] = quadBlueSum[q] * quadPixelsScale[q];
        patCorrScale[2][q1] = 1.f / sqrtf(quadBlueSumSq[q]*quadPixelsScale[q] - 
                                       patExp[2][q1]*patExp[2][q1]);

        patRedSum += quadRedSum[q];
        patRedSumSq += quadRedSumSq[q];
        patGreenSum += quadGreenSum[q];
        patGreenSumSq += quadGreenSumSq[q];
        patBlueSum += quadBlueSum[q];
        patBlueSumSq += quadBlueSumSq[q];
    }
    
    patExp[0][0] = patRedSum * nbPixelsScale;
    patCorrScale[0][0] = 1.f / sqrtf(patRedSumSq*nbPixelsScale - patExp[0][0]*patExp[0][0]);
    patExp[1][0] = patGreenSum * nbPixelsScale;
    patCorrScale[1][0] = 1.f / sqrtf(patGreenSumSq*nbPixelsScale - patExp[1][0]*patExp[1][0]);
    patExp[2][0] = patBlueSum * nbPixelsScale;
    patCorrScale[2][0] = 1.f / sqrtf(patBlueSumSq*nbPixelsScale - patExp[2][0]*patExp[2][0]);
}

void PatternMatcher2D_partCorr::patternCorrScale_(const RasterImage_gray* patImg, float* patExp, 
                                                  float* patCorrScale)
{
    const ImageRect   *patRect = patImg->getValidRect();
    //
    const int   patHeight = patRect->getHeight(),
                patWidth = patRect->getWidth(),
                nbPatPixels = patHeight * patWidth;
    const int   midI = patHeight/2,
                midJ = patWidth/2;
    const int   quadTop[4] = {0, 0, midI + 1, midI + 1},
                quadBottom[4] = {midI, midI, patHeight-1, patHeight-1},
                quadLeft[4] = {0, midJ + 1, 0, midJ + 1},
                quadRight[4] = {midJ, patWidth-1, midJ, patWidth-1};
    const float nbPixelsScale = 1.f / nbPatPixels;
    const float quadPixelsScale[4] = {1.f/((midI+1)*(midJ+1)), 
                                      1.f/((midI+1)*(patWidth-midJ-1)),
                                      1.f/((patHeight-midI-1)*(midJ+1)), 
                                      1.f/((patHeight-midI-1)*(patWidth-midJ-1))};   
    const unsigned char* const* grayPat = patImg->getGray2D();
    
    //  Calculate normalizing factor for the pattern.  I know that we are off by a factor
    //  of 1/sqrt(nbPixels).   I correct that when I multiply the two scaling factors
    float   quadSum[4] = {0, 0, 0, 0}, quadSumSq[4] = {0, 0, 0, 0};
//             quadExp[4] = {0, 0, 0, 0}, quadScale[4] = {0, 0, 0, 0}; // unused variables. -CJC
    float   patSum = 0, patSumSq = 0;
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        for (int kP=quadTop[q]; kP<=quadBottom[q]; kP++)
            for (int lP=quadLeft[q]; lP<=quadRight[q]; lP++)
            {
                quadSum[q] += grayPat[kP][lP];
                quadSumSq[q] += grayPat[kP][lP]*grayPat[kP][lP];
            }

        patExp[q1] = quadSum[q] * quadPixelsScale[q];
        patCorrScale[q1] = 1.f / sqrtf(quadSumSq[q]*quadPixelsScale[q] - 
                                        patExp[q1]*patExp[q1]);
        
        patSum += quadSum[q];
        patSumSq += quadSumSq[q];
    }
    
    patExp[0] = patSum * nbPixelsScale;
    patCorrScale[0] = 1.f / sqrtf(patSumSq*nbPixelsScale - patExp[0]*patExp[0]);
}

void PatternMatcher2D_partCorr::patternCorrScale_(const RasterImage_RGBa* patImg, const Blob* patBlob, 
												  float patExp[][5], float patCorrScale[][5])
{
    const float quadPixelsScale[4] = {  1.f/patBlob->getNbPixels(UPPER_LEFT_QUAD),
                                        1.f/patBlob->getNbPixels(UPPER_RIGHT_QUAD),
                                        1.f/patBlob->getNbPixels(LOWER_LEFT_QUAD),
                                        1.f/patBlob->getNbPixels(LOWER_RIGHT_QUAD)};

    const int   patHeight = patBlob->getHeight(),
                patWidth = patBlob->getWidth(),
                nbPatPixels = patBlob->getNbPixels();
    const float nbPixelsScale = 1.f / nbPatPixels;
    const int   midI = patHeight/2,
                midJ = patWidth/2,
                halfTop[2] = {0, midI+1},
                halfBottom[2] = {midI, patHeight-1};

    const unsigned char* const* rgbaPat = patImg->getRGBa2D();
    const LineSegDeque segDeque = patBlob->getSegDeque();
    
    //  Calculate normalizing factor for the pattern for each of the quadrants.  Then sum up for
    //  the entire pattern.
    float   quadRedSum[4] = {0, 0, 0, 0}, quadRedSumSq[4] = {0, 0, 0, 0};
    float   quadGreenSum[4] = {0, 0, 0, 0}, quadGreenSumSq[4] = {0, 0, 0, 0};
    float   quadBlueSum[4] = {0, 0, 0, 0}, quadBlueSumSq[4] = {0, 0, 0, 0}; 
    float   patRedSum = 0, patRedSumSq = 0;
    float   patGreenSum = 0, patGreenSumSq = 0;
    float   patBlueSum = 0, patBlueSumSq = 0;
    //  Work on the top half, then the bottom half
    for (int qHalf=0; qHalf<2; qHalf++)
    {
        LineSegDeque::const_iterator lineIter = segDeque.begin() + halfTop[qHalf];
    
        for (int i=halfTop[qHalf]; i<=halfBottom[qHalf]; i++, lineIter++)
        {
            //  reinitialize q to be the left quadrant of the half being processed
            int  q = 2*qHalf, startX, endX;
            bool leftMode = true, segRightHalf = false;   
            list<HorizontalSegment*>::iterator  segIter = (*lineIter)->begin(), 
                                                endSeg = (*lineIter)->end();
            while (segIter != endSeg)
            {
                HorizontalSegment   *theSeg = (*segIter);
                int kP = theSeg->y;

                //  We are processing the left half, 
                if (leftMode)
                {
                    startX = theSeg->leftX;
                    
                    //  if the segment is entirely on the left half, just process the
                    //  whole segment and move to the next
                    if (theSeg->rightX <= midJ)
                    {
                        endX = theSeg->rightX;
                        segIter++;
                    }
                    else
                    {
                        //  this segment is completely on the right side
                        if (theSeg->leftX > midJ)
                        {
                            //  we are not in left mode anymore; data contribute to 
                            //  right half calculations
                            leftMode = false;
                            q++;
                            endX = theSeg->rightX;
                            segIter++;
                        }
                        //  if we have a split segment, this will be processed in two
                        //  passes of the loop.  We set flags to spot this case in the
                        //  next pass.
                        else
                        {
                            leftMode = false;
                            segRightHalf = true;
                            endX = midJ;
                        }
                    }
                } 
                //  if we are in the right mode, we only have to check if we are completing
                //  the right half of a straddling segment
                else
                {
                    endX = theSeg->rightX;
                    
                    //  if we are processingt the right half of a straddling segment, then 
                    //  we start from the middle border and start adding to the right half
                    if (segRightHalf)
                    {
                        segRightHalf = false;
                        startX = midJ + 1;
                        q++;
                    }
                    else
                        startX = theSeg->leftX;
                    
                    segIter++;
                }
                
                //  Now that we have gone through all this trouble we know what quadrant
                //  this segment (or segment fragment) belongs.  We can now use the data
                //  in our summation
                for (int lP = startX, fourLP=4*lP; lP<=endX; lP++, fourLP+=2)
                {
                    //  red component
                    quadRedSum[q] += rgbaPat[kP][fourLP];
                    quadRedSumSq[q] += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
                    //  green component
                    quadGreenSum[q] += rgbaPat[kP][++fourLP];
                    quadGreenSumSq[q] += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
                    //  blue component
                    quadBlueSum[q] += rgbaPat[kP][++fourLP];
                    quadBlueSumSq[q] += rgbaPat[kP][fourLP]*rgbaPat[kP][fourLP];
                }
            }
        }
    }

    //  We can now compute the expected values and correlation scales
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        patExp[0][q1] = quadRedSum[q] * quadPixelsScale[q];
        patCorrScale[0][q1] = 1.f / sqrtf(quadRedSumSq[q]*quadPixelsScale[q] - 
                                      patExp[0][q1]*patExp[0][q1]);
        patExp[1][q1] = quadGreenSum[q] * quadPixelsScale[q];
        patCorrScale[1][q1] = 1.f / sqrtf(quadGreenSumSq[q]*quadPixelsScale[q] - 
                                        patExp[1][q1]*patExp[1][q1]);
        patExp[2][q1] = quadBlueSum[q] * quadPixelsScale[q];
        patCorrScale[2][q1] = 1.f / sqrtf(quadBlueSumSq[q]*quadPixelsScale[q] - 
                                       patExp[2][q1]*patExp[2][q1]);
        patRedSum += quadRedSum[q]; 
        patRedSumSq += quadRedSumSq[q];
        patGreenSum += quadGreenSum[q]; 
        patGreenSumSq += quadGreenSumSq[q];
        patBlueSum += quadBlueSum[q]; 
        patBlueSumSq += quadBlueSumSq[q];     
    }

    patExp[0][0] = patRedSum * nbPixelsScale;
    patCorrScale[0][0] = 1.f / 
                            sqrtf(patRedSumSq*nbPixelsScale - patExp[0][0]*patExp[0][0]);
    patExp[1][0] = patGreenSum * nbPixelsScale;
    patCorrScale[1][0] = 1.f / 
                            sqrtf(patGreenSumSq*nbPixelsScale - patExp[1][0]*patExp[1][0]);
    patExp[2][0] = patBlueSum * nbPixelsScale;
    patCorrScale[2][0] = 1.f / 
                            sqrtf(patBlueSumSq*nbPixelsScale - patExp[2][0]*patExp[2][0]);
}

void PatternMatcher2D_partCorr::patternCorrScale_(const RasterImage_gray* patImg, const Blob* patBlob,
												  float* patExp, float* patCorrScale)
{
    const float quadPixelsScale[4] = {  1.f/patBlob->getNbPixels(UPPER_LEFT_QUAD),
                                        1.f/patBlob->getNbPixels(UPPER_RIGHT_QUAD),
                                        1.f/patBlob->getNbPixels(LOWER_LEFT_QUAD),
                                        1.f/patBlob->getNbPixels(LOWER_RIGHT_QUAD)};

    const int   patHeight = patBlob->getHeight(),
                patWidth = patBlob->getWidth(),
                nbPatPixels = patBlob->getNbPixels();
    const float nbPixelsScale = 1.f / nbPatPixels;
    const int   midI = patHeight/2,
                midJ = patWidth/2,
                halfTop[2] = {0, midI+1},
                halfBottom[2] = {midI, patHeight-1};

    const unsigned char* const* grayPat = patImg->getGray2D();
    const LineSegDeque segDeque = patBlob->getSegDeque();
    
    //  Calculate normalizing factor for the pattern for each of the quadrants.  Then sum up for
    //  the entire pattern.
    float   quadSum[4] = {0, 0, 0, 0}, quadSumSq[4] = {0, 0, 0, 0};
    float   patSum = 0, patSumSq = 0;
    //  Work on the top half, then the bottom half
    for (int qHalf=0; qHalf<2; qHalf++)
    {
        LineSegDeque::const_iterator lineIter = segDeque.begin() + halfTop[qHalf];
    
        for (int i=halfTop[qHalf]; i<=halfBottom[qHalf]; i++, lineIter++)
        {
            //  reinitialize q to be the left quadrant of the half being processed
            int  q = 2*qHalf, startX, endX;
            bool leftMode = true, segRightHalf = false;   
            list<HorizontalSegment*>::iterator  segIter = (*lineIter)->begin(), 
                                                endSeg = (*lineIter)->end();
            while (segIter != endSeg)
            {
                HorizontalSegment   *theSeg = (*segIter);
                int kP = theSeg->y;

                //  We are processing the left half, 
                if (leftMode)
                {
                    startX = theSeg->leftX;
                    
                    //  if the segment is entirely on the left half, just process the
                    //  whole segment and move to the next
                    if (theSeg->rightX <= midJ)
                    {
                        endX = theSeg->rightX;
                        segIter++;
                    }
                    else
                    {
                        //  this segment is completely on the right side
                        if (theSeg->leftX > midJ)
                        {
                            //  we are not in left mode anymore; data contribute to 
                            //  right half calculations
                            leftMode = false;
                            q++;
                            endX = theSeg->rightX;
                            segIter++;
                        }
                        //  if we have a split segment, this will be processed in two
                        //  passes of the loop.  We set flags to spot this case in the
                        //  next pass.
                        else
                        {
                            leftMode = false;
                            segRightHalf = true;
                            endX = midJ;
                        }
                    }
                } 
                //  if we are in the right mode, we only have to check if we are completing
                //  the right half of a straddling segment
                else
                {
                    endX = theSeg->rightX;
                    
                    //  if we are processingt the right half of a straddling segment, then 
                    //  we start from the middle border and start adding to the right half
                    if (segRightHalf)
                    {
                        segRightHalf = false;
                        startX = midJ + 1;
                        q++;
                    }
                    else
                        startX = theSeg->leftX;
                    
                    segIter++;
                }
                
                //  Now that we have gone through all this trouble we know what quadrant
                //  this segment (or segment fragment) belongs.  We can now use the data
                //  in our summation
                for (int lP = startX; lP<=endX; lP++)
                {
                    quadSum[q] += grayPat[kP][lP];
                    quadSumSq[q] += grayPat[kP][lP]*grayPat[kP][lP];
                }
            }
        }
    }

    //  We can now compute the expected values and correlation scales
    for (int q=0, q1=1; q<4; q++, q1++)
    {
        patExp[q1] = quadSum[q] * quadPixelsScale[q];
        patCorrScale[q1] = 1.f / sqrtf(quadSumSq[q]*quadPixelsScale[q] - 
                                      patExp[q1]*patExp[q1]);
        patSum += quadSum[q]; 
        patSumSq += quadSumSq[q];
    }

    patExp[0] = patSum * nbPixelsScale;
    patCorrScale[0] = 1.f / sqrtf(patSumSq*nbPixelsScale - patExp[0]*patExp[0]);
}

#if 0
#pragma mark -
#endif



bool PatternMatcher2D_partCorr::isProperMatchRecordType_(const Pattern2DMatchRecord* matchRecord) const
{
//    return (typeid(*matchRecord) == typeid(uriVL::Pattern2DMatchRecord_part));

    string s1 = matchRecord->getRecordType();
    return (s1.compare("partial match record") == 0);
}

Pattern2DMatchRecord* PatternMatcher2D_partCorr::newPattern2DMatchRecord_(void)
{
    return new Pattern2DMatchRecord_part();
}
