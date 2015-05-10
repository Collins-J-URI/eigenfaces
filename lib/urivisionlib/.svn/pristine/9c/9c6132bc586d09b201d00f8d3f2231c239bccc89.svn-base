/*  NAME:
        ImageWarper.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageWarper class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "ImageWarper.h"
#include "RasterImage.h"
#include "RasterImage_RGBa.h"

using namespace std;
using namespace uriVL;

//#define DEBUG

//----------------------------------------------------------
//  constructors & destructor
//----------------------------------------------------------
ImageWarper::ImageWarper(ImageWarperType type,
						 InnerWarpingMode innerMode, 
    		             BorderWarpingMode outerMode)
     :		warperType_(type),
			innerMode_(innerMode),
			outerMode_(outerMode),
			isSetUp_(),
			imgHeight_(0),
			imgWidth_(0),
			effImgWidth_(),
			rasterType_(),
			gridRows_(0),
			gridCols_(0),
			xy_(NULL),
			xy0_(NULL),
			verticalSpacing_(0),
			horizontalSpacing_(0),
			imgIn_(NULL),
			imgOut_(NULL),
			imgIn1D_(NULL),
			imgOut1D_(NULL)
{
}


ImageWarper::ImageWarper(const ImageWarper& theObj)
     :		warperType_(),
			innerMode_(),
			outerMode_(),
			isSetUp_(),
			imgHeight_(0),
			imgWidth_(0),
			effImgWidth_(),
			rasterType_(),
			gridRows_(0),
			gridCols_(0),
			xy_(NULL),
			xy0_(NULL),
			verticalSpacing_(0),
			horizontalSpacing_(0),
			imgIn_(NULL),
			imgOut_(NULL),
			imgIn1D_(NULL),
			imgOut1D_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageWarper copy constructor not implemented.");
}



ImageWarper::~ImageWarper(void)
{
    deleteGrids_();
}


const ImageWarper& ImageWarper::operator = (const ImageWarper& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageWarper copy operator not implemented.");

	return *this;
}


ImageWarperType ImageWarper::getWarperType(void) const
{
    return warperType_;
}

//----------------------------------------------------------
//  Setup functions:  Input/Output images and grid setting
//----------------------------------------------------------
			
void ImageWarper::setImages(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null input image assigned to an ImageWarper");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output image assigned to an ImageWarper");

    //  So far we only warp RGBa32 and gray rasters
	BaseRasterType  rasterType = imgIn->getBaseRasterType();
    FAIL_CONDITION((rasterType != kGrayRaster) && (rasterType != kRGBa32Raster),
                    kInvalidParameterError,
                    "only RGBa32 and gray rasters supported yet");
    FAIL_CONDITION( imgOut->getBaseRasterType() != rasterType,
                    kInvalidParameterError,
                    "input and output raster types must match (so far)");
	rasterType_ = rasterType;
	
    //  Verify that the input image specified has dimensions that are
    //  suitable to the warper
    const int   imgHeight = imgIn->getHeight(),
                imgWidth = imgIn->getWidth();
    int     loWidth, hiWidth, loHeight, hiHeight;
    getImageSuitableDimensions(imgWidth, imgHeight, &loWidth, &loHeight, &hiWidth, &hiHeight);
    FAIL_CONDITION(((loWidth != imgWidth) && (hiWidth != imgWidth)) || 
                   ((loHeight != imgHeight) && (hiHeight != imgHeight)),
                    kInvalidParameterError,
                    "input image has improper dimensions for warping");
    imgWidth_ = imgWidth;
    imgHeight_ = imgHeight;
    effImgWidth_ = getEffectivePixelsPerRow(imgWidth);
    imgIn_ = imgIn;
    imgIn1D_ = imgIn->getRaster();

    //  make sure that the output image is large enough to store the result
    //  of the warping
    if (!imgOut->contains(imgIn))
        imgOut->setBoundRect(imgIn->getBoundRect());
    imgOut_ = imgOut;
    imgOut1D_ = imgOut->getRaster(R_W_ACCESS);
//    imgOut_->setValidRect(imgIn->getValidRect());
    
    //  if grid dimensions were previously set, we can now initialize the grids
    if (xy_ != NULL)
        initializeGrids_();
}


void ImageWarper::setGrid(int gridRows, int gridCols)
{
    FAIL_CONDITION(	gridRows < 3 || gridCols < 3,
                    kInvalidParameterError,
                    "gridRows and gridCols must >= 3");
    
    //  if the user changed the grid's dimensions, we must delete the
    //  old grids first
    if ((gridRows != gridRows_) || (gridCols != gridCols_))
        deleteGrids_();
    
    //  set the grid's dimensions  
	gridRows_ = gridRows;
	gridCols_ = gridCols;
    allocateGrids_();
    
    //  if an image was previously set, we can also initialize the grid
    if (imgIn1D_ != NULL)
        initializeGrids_();
}

//----------------------------------------------------------
//  Get functions
//----------------------------------------------------------
#if 0
#pragma mark -
#endif

int	ImageWarper::getNbGridRows(void) const
{
	return gridRows_;
}

int	ImageWarper::getNbGridCols(void) const
{
	return gridCols_;
}

bool ImageWarper::isSetUp(void) const
{
	return isSetUp_;
}

float*** const ImageWarper::newWarpingGrid(void) const
{
    FAIL_CONDITION( !isSetUp_,
                    kNoninitializedObjectError,
                    "A Warper must be completely set up before it can produce a new grid");
    float*** grid = new float** [gridRows_];
    for (int i=0; i<gridRows_; i++)
    {
        grid[i] = new float*[gridCols_];
        for (int j=0; j<gridCols_; j++)
            grid[i][j] = new float[2];
    }

    for (int i=0; i<gridRows_; i++)
    {
        for (int j=0; j<gridCols_; j++)
        {
			grid[i][j][0] = xy0_[i][j][0];
			grid[i][j][1] = xy0_[i][j][1];
        }
    }
    
    return grid; 
}

InnerWarpingMode ImageWarper::getInnerWarpingMode(void) const
{
    return innerMode_;
}

BorderWarpingMode ImageWarper::getBorderWarpingMode(void) const
{
    return outerMode_;
}

const RasterImage* ImageWarper::getInputImage(void) const
{
    return imgIn_;
}
            
RasterImage* ImageWarper::getOutputImage(void) const
{
    return imgOut_;    
}             


//----------------------------------------------------------
//  Warping functions
//----------------------------------------------------------
#if 0
#pragma mark -
#endif

bool ImageWarper::isValidDisplacement(float*** xy) const
{
    bool    isValid = true;
    
    if (innerMode_ != INNER_FREE)
    {
    }
    
    if (outerMode_ != BORDER_FREE)
    {
    }
    
    return isValid;
}

void ImageWarper::warp(float*** xy, const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION( xy == NULL,
                    kNullParameterError,
                    "null grid coordinate array passed to ImageWarper::warp");
    FAIL_CONDITION( xy_ == NULL,
                    kNoninitializedObjectError,
                    "A warping grid must be specified before images can be warped");

    FAIL_CONDITION( !isValidDisplacement(xy),
                    kInvalidParameterError,
                    "Grid displacement specified not conform to this warper's grid modes");

    setImages(imgIn, imgOut);    

    warp_(xy);
}

void ImageWarper::warp(float*** xy)
{
    FAIL_CONDITION( !isSetUp_,
                    kNoninitializedObjectError,
                    "A Warper must be completely set up before it can be used");
    FAIL_CONDITION( !isValidDisplacement(xy),
                    kInvalidParameterError,
                    "Grid displacement specified not conform to this warper's grid modes");
    warp_(xy);
}

//----------------------------------------------------------
//  private functiond
//----------------------------------------------------------

void ImageWarper::allocateGrids_(void)
{
	xy_ = new float**[gridRows_];
	xy0_ = new float**[gridRows_];
	for (int i=0; i<gridRows_; i++)
	{
		xy_[i] = new float*[gridCols_];
		xy0_[i] = new float*[gridCols_];
		
		for (int j=0; j<gridCols_; j++)
		{
			xy_[i][j] = new float[2];
			xy0_[i][j] = new float[2];
		}
	}
}

void ImageWarper::initializeGrids_(void)
{
	verticalSpacing_ = (1.f * imgHeight_) / (gridRows_ - 1),
	horizontalSpacing_ = (1.f * imgWidth_) / (gridCols_ - 1);
	
	for (int i=0; i<gridRows_; i++)
	{
//jyh
//		int ip1 = i + 1;
		
		for (int j=0; j<gridCols_; j++)
		{
			// point grid x and y calculations
			xy_[i][j][0] = xy0_[i][j][0] = (int)(j * horizontalSpacing_ + 0.5f);
			xy_[i][j][1] = xy0_[i][j][1] = (int)(i * verticalSpacing_ + 0.5f);
		}
	}
	
	isSetUp_ = true;
}

void ImageWarper::deleteGrids_(void)
{
    if (xy_ != NULL)
    {
    	for (int i=0; i<gridRows_; i++)
    	{
    		for (int j=0; j<gridCols_; j++)
    			delete []xy_[i][j];
    		
    		delete []xy_[i];
    	}
    	delete []xy_;
    	xy_ = NULL;
    }

    if (xy_ != NULL)
    {
    	for (int i=0; i<gridRows_; i++)
    	{
    		for (int j=0; j<gridCols_; j++)
    			delete []xy0_[i][j];
    		
    		delete []xy0_[i];
    	}
    	delete []xy0_;
    	xy0_ = NULL;
    }
}
