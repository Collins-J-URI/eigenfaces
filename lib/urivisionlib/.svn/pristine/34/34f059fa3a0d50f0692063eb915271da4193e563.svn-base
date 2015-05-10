/*  NAME:
        BackgroundRemover.cpp
 
    DESCRIPTION:
        BackgroundRemover source file
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "BackgroundRemover.h"
#include "RasterImage_gray_F.h"
#include "ImageArithmetics.h"


using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


BackgroundRemover::BackgroundRemover(void)
        :       background_(NULL),
                mode_(MAX_DIFFERENCE)    //	default color difference mode

{
}

BackgroundRemover::BackgroundRemover(const BackgroundRemover& obj)
        :       background_(NULL),
                mode_(MAX_DIFFERENCE)    //	default color difference mode

{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "BackgroundRemover copy constructor disabled");
}



BackgroundRemover::~BackgroundRemover(void)
{
    if (background_ != NULL)
        delete background_;
}


const BackgroundRemover& BackgroundRemover::operator = (const BackgroundRemover& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "BackgroundRemover copy operator disabled");
    return *this;
}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

RasterImage* BackgroundRemover::getBackground(void)
{
	return background_;
}

ColorDifferenceMode BackgroundRemover::getColorDifferenceMode(void)
{
    return mode_;
}


//	pure virtual func
void BackgroundRemover::assignBackground_(RasterImage* img)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in BackgroundRemover::setBackground");

    switch (img->getBaseRasterType())
    {
        case kGrayRaster:
            if(background_ != NULL)
			{
                if (background_->getBaseRasterType() == kGrayRaster)
                {
                    if (img->hasFloatRaster() && img->floatRasterIsReference() &&
                            !background_->hasFloatRaster())
                    {
                        delete background_;
                        background_ = NULL;
                    }
                    else
                        img->copyInto(background_);
                }
                else
                {
                    delete background_;
                    background_ = NULL;
                }
			}

            if(background_ == NULL)
			{
                if (img->hasFloatRaster() && img->floatRasterIsReference())
                    background_ = new RasterImage_gray_F(img);
                else
                    background_ = new RasterImage_gray(img);
			
			}
            break;

        case kRGB24Raster:
            if(background_ != NULL)
			{
                if (background_->getBaseRasterType() == kRGB24Raster)
                    img->copyInto(background_);
                else
                {
                    delete background_;
                    background_ = NULL;
                }
			}
			
            if(background_ == NULL)
                background_ = new RasterImage_RGBa(img);
            break;

        case kRGBa32Raster:
            if(background_ != NULL)
			{
                if (background_->getBaseRasterType() == kRGBa32Raster)
                    img->copyInto(background_);
                else
                {
                    delete background_;
                    background_ = NULL;
                }
			}
			
            if(background_ == NULL)
                background_ = new RasterImage_RGBa(img);
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in BackgroundRemover_static::setBackground");
            break;

    }
}


void BackgroundRemover::setColorDifferenceMode(ColorDifferenceMode mode)
{
    mode_ = mode;
}

//-----------------------------------
//	Background removal functions
//-----------------------------------
#if 0
#pragma mark -
#endif

RasterImage_gray* BackgroundRemover::removeBackground(RasterImage* imgIn)
{
    FAIL_CONDITION( background_ == NULL,
                    kNullRasterImageError,
                    "Background was not set properly for this object");

    return ImageArithmetics::difference(imgIn, background_, mode_);
}


RasterImage_gray* BackgroundRemover::removeBackground(RasterImage* imgIn,
        ColorDifferenceMode mode)
{
    FAIL_CONDITION( background_ == NULL,
                    kNullRasterImageError,
                    "Background was not set properly for this object");

    return ImageArithmetics::difference(imgIn, background_, mode);
}

void BackgroundRemover::removeBackground(RasterImage* imgIn, RasterImage_gray* imgOut)
{
    FAIL_CONDITION( background_ == NULL,
                    kNullRasterImageError,
                    "Background was not set properly for this object");

    ImageArithmetics::difference(imgIn, background_, imgOut, mode_);
}

void BackgroundRemover::removeBackground(RasterImage* imgIn, RasterImage_gray* imgOut,
        ColorDifferenceMode mode)
{
    FAIL_CONDITION( background_ == NULL,
                    kNullRasterImageError,
                    "Background was not set properly for this object");

    ImageArithmetics::difference(imgIn, background_, imgOut, mode);
}


