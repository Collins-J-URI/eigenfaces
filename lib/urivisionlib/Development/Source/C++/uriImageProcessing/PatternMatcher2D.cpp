/*  NAME:
        PatternMatcher2D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib PatternMatcher2D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "PatternMatcher2D.h"

using namespace std;
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


PatternMatcher2D::PatternMatcher2D(void)
{
	grayPat_ = grayImg_ = NULL;
	rgbaPat_ = rgbaImg_ = NULL;
	patMask_ = NULL;
	patImg_ = img_ = NULL;
	patBlob_ = NULL;
}

PatternMatcher2D::~PatternMatcher2D(void)
{
}

bool PatternMatcher2D::isSubpixelMatcher(void)
{
    return isSubpixelMatcher_;
}

BasePatternMatcher2DType PatternMatcher2D::getBaseMatcherType(void) const
{
    return baseMatcherType_;
}
    
#if 0
#pragma mark -
#endif


void PatternMatcher2D::setPattern(const Pattern2D* pat)
{
    FAIL_CONDITION(	pat == NULL,
                    kNullParameterError,
                    "NULL pattern sent as parameter to PatternMatcher2D_corr::setPattern");
    setPattern(pat->getPattern(), pat->getBlob());
}

void PatternMatcher2D::setPattern(const RasterImage* patImg, const Blob* patBlob)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL image sent as parameter to PatternMatcher2D_corr::setPattern");
    
    BaseRasterType patRasterType = patImg->getBaseRasterType();
    FAIL_CONDITION(	(patRasterType != kGrayRaster) && (patRasterType != kRGBa32Raster),
                    kInvalidParameterError,
                    "PatternMatcher2D_corr::setPattern only takes gray and RGBa images");

    //  if needed, allocate new temporary storage raster for the pattern and image
    allocateRasters_(patImg, patBlob);
    patBlob_ = patBlob;
    patImg_ = patImg;
}

void PatternMatcher2D::setImage(const RasterImage* img, const ImagePoint* matchPoint)
{
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL image sent as parameter to PatternMatcher2D_corr::setImage");
    
    FAIL_CONDITION(	img->getBaseRasterType() != patRasterType_,
                    kInvalidParameterError,
                    "image type must match pattern type (so far)");

    img_ = img;
    
    //  this function should be overridden by subclasses that can do something
    //  with the match point
}




#if 0
#pragma mark -
#endif
//----------------------------------------------------------
//  Matching
//----------------------------------------------------------



Pattern2DMatchRecord* PatternMatcher2D::match(const Pattern2D* patternSought, const ImageRect* searchRect, 
											  const RasterImage* img)
{
    Pattern2DMatchRecord* matchRecord = newPattern2DMatchRecord_();

    match(patternSought, searchRect, img, matchRecord);

    return matchRecord;
}

Pattern2DMatchRecord* PatternMatcher2D::match(const Pattern2D* patternSought, const ImageRect* searchRect, 
											  const RasterImage* img, list<Blob*> *occluder)
{
    Pattern2DMatchRecord* matchRecord = newPattern2DMatchRecord_();

    match(patternSought, searchRect, img, matchRecord, occluder);

    return matchRecord;
}

void PatternMatcher2D::match(const Pattern2D* patternSought, const ImageRect* searchRect, 
                             const RasterImage* img, Pattern2DMatchRecord* matchRecord)
{
    FAIL_CONDITION(	patternSought == NULL,
                    kNullParameterError,
                    "NULL pattern passed as parameter to PatternMatcher2D::match");
    FAIL_CONDITION(	searchRect == NULL,
                    kNullRectangleError,
                    "NULL search rectangle passed as parameter to PatternMatcher2D::match");
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image passed as parameter to PatternMatcher2D::match");
    FAIL_CONDITION(	matchRecord == NULL,
                    kNullParameterError,
                    "NULL match record parameter passed to PatternMatcher2D::match");
    FAIL_CONDITION(	!isProperMatchRecordType_(matchRecord),
                    kInvalidParameterError,
                    "match record does not have the proper type for this matcher");

	/*	handling of scaleParam to be added here
	 */
	 
	 
    //  First determine the effective search rectangle  
    const ImageRect   *imgRect = img->getValidRect();  
    ImageRect   *usableRect = new ImageRect(imgRect->getLeft(), imgRect->getTop(),
                                            imgRect->getWidth() - patternSought->getWidth(),
                                            imgRect->getHeight() - patternSought->getHeight());
    ImageRect   *effectiveRect = ImageRect::intersection(usableRect, searchRect);
    FAIL_CONDITION(	effectiveRect == NULL,
                    kNullRectangleError,
                    "Search region does not contain valid data in PatternMatcher2D_corr::match");

    const RasterImage* patImg = patternSought->getPattern();
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "patterns stores NULL raster image in PatternMatcher2D_corr::match");
    
	const Blob    *patBlob = patternSought->getBlob();

	///////////
	// Visual doesn't like variable declaration inside a switch
	bool	localRGBa;
	bool	localGray;
	const RasterImage_RGBa* rgbaImg = NULL;
	const RasterImage_gray* grayImg = NULL;
    switch (patImg->getBaseRasterType())
    {
        case kGrayRaster:
        	//	temp call
        	grayImg = RasterImage_gray::localRasterGray(img, effectiveRect, &localGray);
        	if (patBlob == NULL)
            	match_(static_cast<const RasterImage_gray*>(patImg), effectiveRect, grayImg, matchRecord);
            else
            	match_(static_cast<const RasterImage_gray*>(patImg), patBlob, effectiveRect, grayImg, matchRecord);
            	
        	if (localGray)
        		delete grayImg;
            break;
            
        case kRGB24Raster:
            break;

        case kRGBa32Raster:
        	//	temp call
        	rgbaImg = RasterImage_RGBa::localRasterRGBa(img, effectiveRect, &localRGBa);

        	if (patBlob == NULL)
            	match_(static_cast<const RasterImage_RGBa*>(patImg), effectiveRect, rgbaImg, matchRecord);
            else
            	match_(static_cast<const RasterImage_RGBa*>(patImg), patBlob, effectiveRect, rgbaImg, matchRecord);

        	if (localRGBa)
        		delete rgbaImg;
            break;

        case kUndefRasterType:
        case kBinaryRaster:
        case kLongGrayRaster:
        case kaRGB16Raster:
        case kaRGB32Raster:
            FAIL_CONDITION( true, 
							kNoCode, 
							"Unhandled case in a switch statement");
    }
    
    delete usableRect;
    delete effectiveRect;    
}

void PatternMatcher2D::match(const Pattern2D* patternSought, const ImageRect* searchRect, 
                             const RasterImage* img, Pattern2DMatchRecord* matchRecord, 
                             list<Blob*> *occluder)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Function not implemented");
}

#if 0
#pragma mark -
#endif

void PatternMatcher2D::deleteRasters_(void)
{
    switch (patRasterType_)
    {
        //  delete old gray rasters
        case kGrayRaster:
            for (int i=0; i<patHeight_; i++)
            {
                delete []grayPat_[i];
                delete []grayImg_[i];
            }
            delete []grayPat_;
            delete []grayImg_;
            break;

        //  delete old rgba rasters
        case kRGBa32Raster:
            for (int i=0; i<patHeight_; i++)
            {
                for (int j=0; j<patWidth_; j++)
                {
                    delete []rgbaPat_[i][j];
                    delete []rgbaImg_[i][j];
                }
                delete []rgbaPat_[i];
                delete []rgbaImg_[i];
            }
            delete []rgbaPat_;
            delete []rgbaImg_;
            break;

        default:
            break;
    }

/*
    if (patMask_ != NULL)
    {
        for (int i=0; i<patHeight_; i++)
                delete []patMask_[i];
        delete []patMask_;
    }
*/    
    grayPat_ = grayImg_ = NULL;
    rgbaPat_ = rgbaImg_ = NULL;
    patMask_ = NULL;    
}


void PatternMatcher2D::allocateRasters_(const RasterImage* patImg, const Blob* patBlob)
{
    int patWidth = patImg->getWidth(),
        patHeight = patImg->getHeight();
    BaseRasterType  rasterType = patImg->getBaseRasterType();
    
    //  do we need to delete old rasters?
    //  Either new type or larger dimensions
    if ( (rasterType!=patRasterType_) || (patWidth>patWidth_) || (patHeight>patHeight_))
    {
        if (patRasterType_ != kUndefRasterType)
            deleteRasters_();
    
        switch (rasterType)
        {
            case kGrayRaster:
                grayPat_ = new float*[patHeight];
                grayImg_ = new float*[patHeight];
                for (int i=0; i<patHeight; i++)
                {
                    grayPat_[i] = new float[patWidth];
                    grayImg_[i] = new float[patWidth];
                }
                break;

            case kRGBa32Raster:
                rgbaPat_ = new float**[patHeight];
                rgbaImg_ = new float**[patHeight];
                for (int i=0; i<patHeight; i++)
                {
                    rgbaPat_[i] = new float*[patWidth];
                    rgbaImg_[i] = new float*[patWidth];
                    for (int j=0; j<patWidth; j++)
                    {
                        rgbaPat_[i][j] = new float[3];
                        rgbaImg_[i][j] = new float[3];               
                    }
                }
                break;

            default:
                break;
        }
       
        //   If we did not have a mask and the new pattern requires one, we allocate
        //   a raster for the mask
        if ((patMask_ == NULL) && (patBlob != NULL))
        {
            patMask_ = new unsigned char*[patHeight];
            for (int i=0; i<patHeight; i++)
                patMask_[i] = new unsigned char[patWidth];
        
        }
       
       patRasterType_ = rasterType;
       patWidth_ = patWidth;
       patHeight_ = patHeight;
   }
}
