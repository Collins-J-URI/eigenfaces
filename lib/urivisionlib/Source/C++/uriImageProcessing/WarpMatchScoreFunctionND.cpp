/*	NAME:
		WarpMatchScoreFunctionND.cpp
		
	DESCRIPTION:
		implementation of the uriVisionLib WarpMatchScoreFunctionND class
		
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include "Pattern2D.h"
#include "ImageWarper_GL.h"
#include "WarpMatchScoreFunctionND.h"

using namespace std;
using namespace uriVL;

//-------------------------------------------
//	constructor & destructor
//-------------------------------------------
WarpMatchScoreFunctionND::WarpMatchScoreFunctionND(BasePatternMatcher2DType matcherType,
													 ImageWarperType warperType,
													 int gridRows, 
													 int gridCols,
													 InnerWarpingMode innerMode, 
													 BorderWarpingMode outerMode)
            : FunctionND(functionDim_(gridRows, gridCols, innerMode, outerMode))
{
    switch(matcherType)
    {
        case PATTERN_MATCHER_2D_CORR:
            matcher_ = new PatternMatcher2D_corr();
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;
            
        case PATTERN_MATCHER_2D_PART_CORR:
            matcher_ = new PatternMatcher2D_partCorr();
            matchRecord_ = new Pattern2DMatchRecord_part();
            patScale1D_ = NULL;
            imgScale1D_ = NULL;
            imgScore1D_ = NULL;
            patScale2D_ = new float*[3];
            imgScale2D_ = new float*[3];
            imgScore2D_ = new float*[3];
            for (int i=0; i<3; i++)
            {
                patScale2D_[i] = new float[5];
                imgScale2D_[i] = new float[5];
                imgScore2D_[i] = new float[5];
            }
            break;
        
        case PATTERN_MATCHER_2D_MEDIAN:
            matcher_ = new PatternMatcher2D_scaledDiff();
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;
        
        case PATTERN_MATCHER_2D_SCALED_DIFF:
            matcher_ = new PatternMatcher2D_medianDiff();
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;

        default:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Unknown base pattern matcher type");
            break;
        
    }
    matcherIsLocal_ = true;

    warper_ = new ImageWarper_GL(innerMode, outerMode);
    warper_->setGrid(gridRows, gridCols);
    outerMode_ = outerMode;
    innerMode_ = innerMode;
    gridRows_ = gridRows;
    gridCols_ = gridCols;
    grid_ = NULL;
    warperIsLocal_ = true;
    
    //  the input image and pattern are not defined yet
    imgIn_ = imgOut_ = NULL;
    pat_ = NULL;
}


WarpMatchScoreFunctionND::WarpMatchScoreFunctionND(BasePatternMatcher2DType matcherType,
			                                       ImageWarper *warper)
            : FunctionND(functionDim_(warper->getNbGridRows(), warper->getNbGridCols(), 
                                      warper->getInnerWarpingMode(), 
                                      warper->getBorderWarpingMode()))
{
    switch(matcherType)
    {
        case PATTERN_MATCHER_2D_CORR:
            matcher_ = new PatternMatcher2D_corr();
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;
            
        case PATTERN_MATCHER_2D_PART_CORR:
            matcher_ = new PatternMatcher2D_partCorr();
            matchRecord_ = new Pattern2DMatchRecord_part();
            patScale1D_ = NULL;
            imgScale1D_ = NULL;
            imgScore1D_ = NULL;
            patScale2D_ = new float*[3];
            imgScale2D_ = new float*[3];
            imgScore2D_ = new float*[3];
            for (int i=0; i<3; i++)
            {
                patScale2D_[i] = new float[5];
                imgScale2D_[i] = new float[5];
                imgScore2D_[i] = new float[5];
            }
            break;
        
        case PATTERN_MATCHER_2D_MEDIAN:
            matcher_ = new PatternMatcher2D_scaledDiff();
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;
        
        case PATTERN_MATCHER_2D_SCALED_DIFF:
            matcher_ = new PatternMatcher2D_medianDiff();
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;

        default:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Unknown base pattern matcher type");
            break;
        
    }
    matcherIsLocal_ = true;

    warper_ = warper;
    innerMode_ = warper->getInnerWarpingMode();
    outerMode_ = warper->getBorderWarpingMode();
    gridRows_ = warper->getNbGridRows();
    gridCols_ = warper->getNbGridCols();
    grid_ = NULL;
    warperIsLocal_ = false;
    
    //  the input image and pattern are not defined yet
    imgIn_ = imgOut_ = NULL;
    pat_ = NULL;
}


WarpMatchScoreFunctionND::WarpMatchScoreFunctionND(PatternMatcher2D* matcher,
    			                         ImageWarperType warperType,
        		                         int gridRows, 
        		                         int gridCols,
    			                         InnerWarpingMode innerMode, 
        		                         BorderWarpingMode outerMode)
            : FunctionND(functionDim_(gridRows, gridCols, innerMode, outerMode))
{
    matcher_ = matcher;
    switch(matcher->getBaseMatcherType())
    {
        case PATTERN_MATCHER_2D_CORR:
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;
            
        case PATTERN_MATCHER_2D_PART_CORR:
            matchRecord_ = new Pattern2DMatchRecord_part();
            patScale1D_ = NULL;
            imgScale1D_ = NULL;
            imgScore1D_ = NULL;
            patScale2D_ = new float*[3];
            imgScale2D_ = new float*[3];
            imgScore2D_ = new float*[3];
            for (int i=0; i<3; i++)
            {
                patScale2D_[i] = new float[5];
                imgScale2D_[i] = new float[5];
                imgScore2D_[i] = new float[5];
            }
            break;
        
        case PATTERN_MATCHER_2D_MEDIAN:
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;
        
        case PATTERN_MATCHER_2D_SCALED_DIFF:
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;

        default:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Unknown base pattern matcher type");
            break;
        
    }
    matcherIsLocal_ = false;

    warper_ = new ImageWarper_GL(innerMode, outerMode);
    warper_->setGrid(gridRows, gridCols);
    outerMode_ = outerMode;
    innerMode_ = innerMode;
    gridRows_ = gridRows;
    gridCols_ = gridCols;
    warperIsLocal_ = true;
    grid_ = NULL;
    
    //  the input image and pattern are not defined yet
    imgIn_ = imgOut_ = NULL;
    pat_ = NULL;
}

WarpMatchScoreFunctionND::WarpMatchScoreFunctionND(PatternMatcher2D* matcher,
			                                       ImageWarper *warper)
            : FunctionND(functionDim_(warper->getNbGridRows(), warper->getNbGridCols(), 
                                     warper->getInnerWarpingMode(), 
                                     warper->getBorderWarpingMode()))
{
    matcher_ = matcher;
    switch(matcher->getBaseMatcherType())
    {
        case PATTERN_MATCHER_2D_CORR:
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;
            
        case PATTERN_MATCHER_2D_PART_CORR:
            matchRecord_ = new Pattern2DMatchRecord_part();
            patScale1D_ = NULL;
            imgScale1D_ = NULL;
            imgScore1D_ = NULL;
            patScale2D_ = new float*[3];
            imgScale2D_ = new float*[3];
            imgScore2D_ = new float*[3];
            for (int i=0; i<3; i++)
            {
                patScale2D_[i] = new float[5];
                imgScale2D_[i] = new float[5];
                imgScore2D_[i] = new float[5];
            }
            break;
        
        case PATTERN_MATCHER_2D_MEDIAN:
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;
        
        case PATTERN_MATCHER_2D_SCALED_DIFF:
            matchRecord_ = new Pattern2DMatchRecord();
            patScale1D_ = new float[3];
            imgScale1D_ = new float[3];
            imgScore1D_ = new float[3];
            patScale2D_ = NULL;
            imgScale2D_ = NULL;
            imgScore2D_ = NULL;
            break;

        default:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Unknown base pattern matcher type");
            break;
        
    }
    matcherIsLocal_ = false;

    warper_ = warper;
    innerMode_ = warper->getInnerWarpingMode();
    outerMode_ = warper->getBorderWarpingMode();
    gridRows_ = warper->getNbGridRows();
    gridCols_ = warper->getNbGridCols();
    warperIsLocal_ = false;
    grid_ = NULL;

    //  the input image and pattern are not defined yet
    imgIn_ = imgOut_ = NULL;
    pat_ = NULL;
}


WarpMatchScoreFunctionND::~WarpMatchScoreFunctionND(void)
{
    if (warperIsLocal_)
        delete warper_;
    
    if (matcherIsLocal_)
        delete matcher_;
        
    if (grid_ != NULL)
        deleteGrid_();
        
    if (pat_ != NULL)
        delete pat_;
        
    if (patScale1D_ != NULL)
        delete []patScale1D_;
    if (imgScale1D_ != NULL)
        delete []patScale1D_;
    if (imgScore1D_ != NULL)
        delete []patScale1D_;
    
    if (patScale2D_ != NULL)
    {
        for (int i=0; i<3; i++)
            delete []patScale2D_[i];
        delete []patScale2D_;
    }
    if (imgScale2D_ != NULL)
    {
        for (int i=0; i<3; i++)
            delete []imgScale2D_[i];
        delete []imgScale2D_;
    }
    if (imgScore2D_ != NULL)
    {
        for (int i=0; i<3; i++)
            delete []imgScore2D_[i];
        delete []imgScore2D_;
    }
}

#if 0
#pragma mark -
#endif


//-------------------------------------------
//	public methods
//-------------------------------------------

void WarpMatchScoreFunctionND::newVarArray(float* x) const
{
    int k = 0;
    //  copy data from x to grid_
    switch(outerMode_)
    {
        case BORDER_FREE:
        case BORDER_INWARD:
            for (int i=0; i<gridRows_; i++)
                for (int j=0; j<gridCols_; j++)
                {
                    x[k++] = grid_[i][j][0];
                    x[k++] = grid_[i][j][1];
                }
            break;
            
        case BORDER_SLIDE:
            //   on top row only x coordinates change (slide)
            for (int j=1; j<gridCols_-1; j++)
                x[k++] = grid_[0][j][0];

            for (int i=1; i<gridRows_-1; i++)
            {
                //  points on the left border can only slide in y direction
                x[k++] = grid_[i][0][1];
                //  inner points can move freely
                for (int j=0; j<gridCols_; j++)
                {
                    x[k++] = grid_[i][j][0];
                    x[k++] = grid_[i][j][1];
                }
                //  points on the right border can only slide in y direction
                x[k++] = grid_[i][gridCols_-1][1];
            }

            //   on bottom row only x coordinates change (slide)
            for (int j=1; j<gridCols_-1; j++)
                x[k++] = grid_[gridRows_-1][j][0];

            break;
        
        case BORDER_FIXED:
            for (int i=1; i<gridRows_-1; i++)
                for (int j=1; j<gridCols_-1; j++)
                {
                    x[k++] = grid_[i][j][0];
                    x[k++] = grid_[i][j][1];
                }
            break;

        default:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Unknown base pattern matcher type");
            break;
        
    }
}

float* WarpMatchScoreFunctionND::newVarArray(void) const
{
    float* x = new float[this->getDimension()];
    
    newVarArray(x);
    
    return x;
}


float*** const WarpMatchScoreFunctionND::getWarpingGrid(void) const
{
    return grid_;
}


float*** const WarpMatchScoreFunctionND::newWarpingGrid(void) const
{
    return warper_->newWarpingGrid(); 
}


#if 0
#pragma mark -
#endif


void WarpMatchScoreFunctionND::setPattern(const Pattern2D* pat)
{
    FAIL_CONDITION(	pat == NULL,
                    kNullParameterError,
                    "NULL pattern sent to PatternMatcher2D_corr::setPattern");
    setPattern(pat->getPattern(), pat->getBlob());
}

void WarpMatchScoreFunctionND::setPattern(const RasterImage* patImg,
										  const Blob* patBlob /* = NULL */)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL image sent as parameter to PatternMatcher2D_corr::setPattern");
    
    BaseRasterType patRasterType = patImg->getBaseRasterType();
    FAIL_CONDITION(	(patRasterType != kGrayRaster) && (patRasterType != kRGBa32Raster),
                    kInvalidParameterError,
                    "WarpMatchScoreFunctionND::setPattern only takes gray and RGBa images");
    FAIL_CONDITION( (imgIn_ != NULL) &&  (imgIn_->getBaseRasterType() != patRasterType),
                    kInvalidParameterError,
                    "Pattern and image base raster types don't match");
        
	patRasterType_ = patRasterType;
	
    matcher_->setPattern(patImg, patBlob);
    if (pat_ == NULL)
        pat_ = new Pattern2D();
    pat_->assignPattern(patImg, patBlob);

    switch (matcher_->getBaseMatcherType())
    {
        case PATTERN_MATCHER_2D_CORR:
        	if (patRasterType_ == kGrayRaster)
	            ((PatternMatcher2D_corr *)matcher_)->patternCorrScale((RasterImage_gray* )patImg, patBlob, patScale1D_);
	        else
	            ((PatternMatcher2D_corr *)matcher_)->patternCorrScale((RasterImage_RGBa* )patImg, patBlob, patScale1D_);
            break;
            
/*
        case PATTERN_MATCHER_2D_PART_CORR:
        	if (patRasterType_ == kGrayRaster)
	            ((PatternMatcher2D_partCorr *)matcher_)->patternCorrScale((RasterImage_gray* )patImg, patBlob, patScale2D_);
	        else
	            ((PatternMatcher2D_partCorr *)matcher_)->patternCorrScale((RasterImage_RGBa* )patImg, patBlob, patScale1D_);
            break;
*/            
        case PATTERN_MATCHER_2D_MEDIAN:
            break;
            
        case PATTERN_MATCHER_2D_SCALED_DIFF:
            break;
            
        default:
            break;
    }
    
}    



void WarpMatchScoreFunctionND::setImage(const RasterImage* img, const ImagePoint* matchPoint)
{
    FAIL_CONDITION( pat_ == NULL,
                    kNoninitializedObjectError,
                    "A pattern must be set before the input image");
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "null search image assigned to WarpMatchScoreFunctionND");
    
    //  we find out which image dimensions are recommended by the warper (closest
    //  to the input image's dimensions)
    int imgHeight = img->getHeight(),
        imgWidth = img->getWidth();
    int loWidth, hiWidth, loHeight, hiHeight;
    warper_->getImageSuitableDimensions(imgWidth, imgHeight, &loWidth, &loHeight, 
                                        &hiWidth, &hiHeight);
    //  if the dimensions of the image are not equal to the low or high fits, we
    //  just bail out
    FAIL_CONDITION( ((imgWidth != loWidth) && (imgWidth != hiWidth)) || 
                    ((imgHeight != loHeight) && (imgHeight != hiHeight)),
                    kInvalidParameterError,
                    "Image not properly sized for the warper");
    imgIn_ = img;

    bool createNewImage = false;
    //  if imgOut_ was null we allocate it
    if (imgOut_ == NULL)
        createNewImage = true;
    //  if imgOut_ had a different type, or different dimensions than img, we must delete it first
    //  
    else if ((img->getBaseRasterType() != imgOut_->getBaseRasterType()) ||
             (imgWidth != imgOut_->getWidth()) ||
             (imgHeight != imgOut_->getHeight()))
    {
        delete imgOut_;
        createNewImage = true;
    }

    if (createNewImage)
    {
        switch (patRasterType_)
        {
            case kGrayRaster:
                imgOut_ = new RasterImage_gray(imgHeight, imgWidth);
                break;

            case kRGBa32Raster:
                imgOut_ = new RasterImage_RGBa(imgHeight, imgWidth);
                break;

            default:
                break;
        }
        imgOut_->setValidRect(0, 0, imgWidth, imgHeight);
    }

    //  finally we assing the input and output images to the warper
    warper_->setImages(imgIn_, imgOut_);

    //  we need an array of grid point coordinates to pass warping commands
    if (grid_ != NULL)
        deleteGrid_();
    grid_ = warper_->newWarpingGrid();
    
    matchPointX_ = matchPoint->getX();
    matchPointY_ = matchPoint->getY();
}


void WarpMatchScoreFunctionND::getImageSuitableDimensions(int imgWidth, 
                                                          int imgHeight,
                                                          int* lowImgWidth, 
                                                          int* lowImgHeight,
                                                          int* highImgWidth, 
                                                          int* highImgHeight) const
{
    warper_->getImageSuitableDimensions(imgWidth, imgHeight, lowImgWidth, lowImgHeight,
                                        highImgWidth, highImgHeight);
}

ImageWarper *WarpMatchScoreFunctionND::getWarper(void) const
{
	return warper_;
}

RasterImage* WarpMatchScoreFunctionND::getWarpedImage(void) const
{
    return imgOut_;
}

#if 0
#pragma mark -
#endif



float WarpMatchScoreFunctionND::func(const float* x)
{
    int k = 0;
    //  copy data from x to grid_
    switch(outerMode_)
    {
        case BORDER_FREE:
        case BORDER_INWARD:
            for (int i=0; i<gridRows_; i++)
                for (int j=0; j<gridCols_; j++)
                {
                    grid_[i][j][0] = x[k++];
                    grid_[i][j][1] = x[k++];
                }
            break;
            
        case BORDER_SLIDE:
            //   on top row only x coordinates change (slide)
            for (int j=1; j<gridCols_-1; j++)
                grid_[0][j][0] = x[k++];

            for (int i=1; i<gridRows_-1; i++)
            {
                //  points on the left border can only slide in y direction
                grid_[i][0][1] = x[k++];
                //  inner points can move freely
                for (int j=0; j<gridCols_; j++)
                {
                    grid_[i][j][0] = x[k++];
                    grid_[i][j][1] = x[k++];
                }
                //  points on the right border can only slide in y direction
                grid_[i][gridCols_-1][1] = x[k++];
            }

            //   on bottom row only x coordinates change (slide)
            for (int j=1; j<gridCols_-1; j++)
                grid_[gridRows_-1][j][0] = x[k++];

            break;
        
        case BORDER_FIXED:
            for (int i=1; i<gridRows_-1; i++)
                for (int j=1; j<gridCols_-1; j++)
                {
                    grid_[i][j][0] = x[k++];
                    grid_[i][j][1] = x[k++];
                }
            break;

        default:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Unknown base pattern matcher type");
            break;
        
    }
    
    warper_->warp(grid_);

    //  Finally calculate the matching score
    float score = 0.f;
    switch(matcher_->getBaseMatcherType())
    {
        case PATTERN_MATCHER_2D_CORR:
        	if (patRasterType_ == kGrayRaster)
            {
	            ((PatternMatcher2D_corr *)matcher_)->covariance((RasterImage_gray* )imgOut_, 
	            												matchPointX_, 
	            												matchPointY_, 
	            												imgScore1D_,
	                                                            imgScale1D_);
            	score = -imgScore1D_[0] * imgScale1D_[0] * patScale1D_[0];
	        }
	        else
            {
            	((PatternMatcher2D_corr *)matcher_)->covariance((RasterImage_RGBa* )imgOut_,
            													matchPointX_,
            													matchPointY_, 
            													imgScore1D_,
                                                            	imgScale1D_);
	            score = -1.f / 3.f * (imgScore1D_[0] * imgScale1D_[0] * patScale1D_[0]
	            					+ imgScore1D_[1] * imgScale1D_[1] * patScale1D_[1]
	            					+ imgScore1D_[2] * imgScale1D_[2] * patScale1D_[2]);
            					   
            }
                                                            	        
            break;
            
        case PATTERN_MATCHER_2D_PART_CORR:  //  not ready yet
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Not implemented yet");
            break;
            
        case PATTERN_MATCHER_2D_MEDIAN:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Not implemented yet");
            break;
            
        case PATTERN_MATCHER_2D_SCALED_DIFF:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Not implemented yet");
            break;
            
        default:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Not implemented yet");
            break;
        
    }
    
    return score;
}

float WarpMatchScoreFunctionND::func(const vector<float>& x)
{
    FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "Not implemented yet");

    float score = 0.f;
/*
    int k = 0;
    //  copy data from x to grid_
    switch(outerMode_)
    {
        case BORDER_FREE:
        case BORDER_INWARD:
            for (int i=0; i<gridRows_; i++)
                for (int j=0; j<gridCols_; j++)
                {
                    grid_[i][j][0] = x[k++];
                    grid_[i][j][1] = x[k++];
                }
            break;
            
        case BORDER_SLIDE:
            //   on top row only x coordinates change (slide)
            for (int j=1; j<gridCols_-1; j++)
                grid_[0][j][0] = x[k++];

            for (int i=1; i<gridRows_-1; i++)
            {
                //  points on the left border can only slide in y direction
                grid_[i][0][1] = x[k++];
                //  inner points can move freely
                for (int j=0; j<gridCols_; j++)
                {
                    grid_[i][j][0] = x[k++];
                    grid_[i][j][1] = x[k++];
                }
                //  points on the right border can only slide in y direction
                grid_[i][gridCols_-1][1] = x[k++];
            }

            //   on bottom row only x coordinates change (slide)
            for (int j=1; j<gridCols_-1; j++)
                grid_[gridRows_-1][j][0] = x[k++];

            break;
        
        case BORDER_FIXED:
            for (int i=1; i<gridRows_-1; i++)
                for (int j=1; j<gridCols_-1; j++)
                {
                    grid_[i][j][0] = x[k++];
                    grid_[i][j][1] = x[k++];
                }
            break;

        default:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Unknown base pattern matcher type");
            break;
        
    }
    
    warper_->warp(grid_);

    //  Finally calculate the matching score
    float score = 0.f;
    switch(matcher_->getBaseMatcherType())
    {
        case PATTERN_MATCHER_2D_CORR:
        	if (patRasterType_ == kGrayRaster)
            {
	            ((PatternMatcher2D_corr *)matcher_)->covariance((RasterImage_gray* )imgOut_, 
	            												matchPointX_, 
	            												matchPointY_, 
	            												imgScore1D_,
	                                                            imgScale1D_);
            	score = -imgScore1D_[0] * imgScale1D_[0] * patScale1D_[0];
	        }
	        else
            {
            	((PatternMatcher2D_corr *)matcher_)->covariance((RasterImage_RGBa* )imgOut_,
            													matchPointX_,
            													matchPointY_, 
            													imgScore1D_,
                                                            	imgScale1D_);
	            score = -1.f / 3.f * (imgScore1D_[0] * imgScale1D_[0] * patScale1D_[0]
	            					+ imgScore1D_[1] * imgScale1D_[1] * patScale1D_[1]
	            					+ imgScore1D_[2] * imgScale1D_[2] * patScale1D_[2]);
            					   
            }
                                                            	        
            break;
            
        case PATTERN_MATCHER_2D_PART_CORR:  //  not ready yet
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Not implemented yet");
            break;
            
        case PATTERN_MATCHER_2D_MEDIAN:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Not implemented yet");
            break;
            
        case PATTERN_MATCHER_2D_SCALED_DIFF:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Not implemented yet");
            break;
            
        default:
            FAIL_CONDITION( true,
                            kInvalidParameterError,
                            "Not implemented yet");
            break;
        
    }
*/    
    return score;
}

//--------------------------------------------------
//	protected funcs
//--------------------------------------------------


int WarpMatchScoreFunctionND::functionDim_(int gridRows, int gridCols,
        			                       InnerWarpingMode innerMode, 
            		                       BorderWarpingMode outerMode)
{
    int n = 2 * gridRows * gridCols;
    
    switch(outerMode)
    {
        case BORDER_SLIDE:
            //  the 4 corners are fixed and the other border points can only
            //  change 1 coordinate
            n -= 4*2 + 2*(gridRows-2) + 2*(gridCols-2);
            break;
        
        case BORDER_FIXED:
            //  the 4 corners and all other border points are fixed
            n -= 4*2 + 4*(gridRows-2) + 4*(gridCols-2);
            break;

        case BORDER_FREE:
        case BORDER_INWARD:
            FAIL_CONDITION( true, 
							kNoCode, 
							"Unhandled case in a switch statement");
    }
    
    return n;
}

void WarpMatchScoreFunctionND::deleteGrid_(void)
{
    for (int i=0; i<gridRows_; i++)
        delete []grid_[i];
    delete []grid_;
    grid_ = NULL;
}
