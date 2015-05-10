/*  NAME:
        VectorField.cpp

    DESCRIPTION:
        implementation of the uriVisionLib RasterImage_RGB class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstring>
//
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_RGBa_F.h"

using namespace uriVL;

#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

RasterImage_RGB::RasterImage_RGB(void)
		:	RasterImage( ),
			//
			rgb2D_(NULL),
			shiftedRgb2D_(NULL)
{
}


RasterImage_RGB::RasterImage_RGB(int nbRows, int nbCols)
	try	:	RasterImage(nbRows, nbCols),
			//
			rgb2D_(NULL),
			shiftedRgb2D_(NULL)
{
	try {
		allocateGraphWorld_(nbRows, nbCols);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGB constructor");
		throw e;
	}

    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
    //
	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGB constructor");
	throw e;
}


RasterImage_RGB::RasterImage_RGB(const ImageRect* theRect)
	try	:	RasterImage(theRect),
			//
			rgb2D_(NULL),
			shiftedRgb2D_(NULL)
{
	try {
		allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGB constructor");
		throw e;
	}

    allocateRaster2D_(getHeight(), getWidth());
    allocateShiftedRaster2D_(getHeight(), getWidth(), getTop(), getLeft());
    //
	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGB constructor");
	throw e;
}


RasterImage_RGB::RasterImage_RGB(int nbRows, int nbCols, const unsigned char* theRGB)
	try	:	RasterImage(nbRows, nbCols),
			//
			rgb2D_(NULL),
			shiftedRgb2D_(NULL)
{
	try {
		allocateGraphWorld_(nbRows, nbCols);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGB constructor");
		throw e;
	}

    allocateRaster2D_(getHeight(), getWidth());
    allocateShiftedRaster2D_(getHeight(), getWidth(), getTop(), getLeft());
    //
	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = true;

	try {
		const unsigned char   *source = theRGB;
		for (int i=0; i<nbRows; i++)
		{
			memcpy(rgb2D_[i], source, static_cast<unsigned long>(3*nbCols));
			source += 3*nbCols;
		}
	}
	catch (...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"Data copy failure in RasterImage_RGB constructor");
	}
	
	setValidRect(0, 0, nbCols, nbRows);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGB constructor");
	throw e;
}


RasterImage_RGB::RasterImage_RGB(const GraphWorld* theGworld)
	try	:	RasterImage(theGworld),
			//
			rgb2D_(NULL),
			shiftedRgb2D_(NULL)
{
//    allocateRaster2D_(getHeight(), getWidth());
//    allocateShiftedRaster2D_(getHeight(), getWidth(), getTop(), getLeft());
//    //
//	setDrawableInformationType(kDense);
//    gworldIsReference_ = true;
//    referenceHasChanged_ = true;
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"RasterImage_RGB Constructor not implemented");

}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGB constructor");
	throw e;
}


RasterImage_RGB::RasterImage_RGB(GraphWorld* theGworld, bool keepThisGraphWorld)
	try	:	RasterImage(theGworld, keepThisGraphWorld),
			//
			rgb2D_(NULL),
			shiftedRgb2D_(NULL)
{
//    allocateRaster2D_(getHeight(), getWidth());
//    allocateShiftedRaster2D_(getHeight(), getWidth(), getTop(), getLeft());
//    //
//	setDrawableInformationType(kDense);
//    gworldIsReference_ = true;
//    referenceHasChanged_ = true;
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"RasterImage_RGB Constructor not implemented");

}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGB constructor");
	throw e;
}


RasterImage_RGB::RasterImage_RGB(const RasterImage* img)
	try	:	RasterImage( ),
			//
			rgb2D_(NULL),
			shiftedRgb2D_(NULL)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_RGB constructor");

	try {
		img->copyInto(this, img->getValidRect());
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGB constructor");
		throw e;
	}

    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGB constructor");
	throw e;
}


RasterImage_RGB::RasterImage_RGB(RasterImage* img, const ImageRect* theRect)
	try	:	RasterImage( ),
			//
			rgb2D_(NULL),
			shiftedRgb2D_(NULL)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_RGB constructor");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in RasterImage_RGB constructor");

	try {
		img->copyInto(this, theRect);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGB constructor");
		throw e;
	}

    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGB constructor");
	throw e;
}


RasterImage_RGB::RasterImage_RGB(RasterImage_RGB& img)
    try	:	RasterImage( ),
			//
			rgb2D_(NULL),
			shiftedRgb2D_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_RGB copy constructor not implemented");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGB constructor");
	throw e;
}


RasterImage_RGB::~RasterImage_RGB(void)
{
    deleteShiftedRaster2D_(getTop());
    deleteRaster2D_();
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to the rasters
//------------------------------------------------------
#endif

BaseRasterType RasterImage_RGB::getBaseRasterType(void) const
{
    return kRGB24Raster;
}

const unsigned char* RasterImage_RGB::getRGB(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGB subclass did not properly override getGray()");
    return gworld_->getRaster();
}


unsigned char* RasterImage_RGB::getRGB(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGB subclass did not properly override getGray()");

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return gworld_->getRaster();
}



const unsigned char* const* RasterImage_RGB::getRGB2D(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGB subclass did not properly override getGray()");
    return rgb2D_;
}


unsigned char* const* RasterImage_RGB::getRGB2D(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGB subclass did not properly override getGray()");

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return rgb2D_;
}

const unsigned char* const* RasterImage_RGB::getShiftedRGB2D(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGBa subclass did not properly override getRGBa2D()");

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return rgb2D_;
    else
        return shiftedRgb2D_;
}

unsigned char* const* RasterImage_RGB::getShiftedRGB2D(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGBa subclass did not properly override getRGBa2D()");

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return rgb2D_;
    else
        return shiftedRgb2D_;
}


/*     Should update the non-reference raster, using the values stored in the reference raster.
 *      In the case of an rgb image, the only raster is the one stored in the GraphWorld.  There
 *      is therefore nothing to do.
 */
void RasterImage_RGB::updateRaster_(void) const
{
    referenceHasChanged_ = false;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copy and Write
//------------------------------------------------------
#endif

const RasterImage_RGB& RasterImage_RGB::operator = (const RasterImage_RGB& img) {
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_RGB copy operator not implemented");
	return *this;
}
 

void RasterImage_RGB::copyInto(RasterImage* imgOut) const
{
    copyInto(imgOut, getValidRect(), NULL);
}

void RasterImage_RGB::copyInto(RasterImage* imgOut, const ImageRect* theRect) const
{
    copyInto(imgOut, theRect, NULL);
}

void RasterImage_RGB::copyInto(RasterImage* imgOut, const ImageRect* theRect, 
                               const ImagePoint* origin) const
{
    //	All the rectangle testing and resizing is done in the parent class' function
    RasterImage::copyInto_(imgOut, theRect, origin);

    //	We only copy from the validRect_, so we need to calculate its intersection
    //	with theRect.
    ImageRect* copyRect = ImageRect::intersection(getValidRect(), theRect);
    FAIL_CONDITION( copyRect == NULL,
                    kUriBaseError,
                    "the copy rectangle specified is not within valid data");


    //	For the time being, we only handle regular dense rasters
    imgOut->setDrawableInformationType(kDense);

    writeInto_(imgOut, copyRect, origin);
	imgOut->setValidRect(copyRect);

    delete copyRect;
}

void RasterImage_RGB::writeInto_(RasterImage* imgOut, const ImageRect* copyRect, 
                                 const ImagePoint* origin) const
{
	//  The GWorld only works with unshifted rasters so we need to compute
	//  the "true" copy rectangles first
    ImageRect*  sourceRect = new ImageRect(copyRect->getLeft() - getLeft(),
										   copyRect->getTop() - getTop(),
										   copyRect->getWidth(), copyRect->getHeight());
	ImageRect*  destRect =  origin == NULL ?
							new ImageRect(copyRect->getLeft() - imgOut->getLeft(),
                                          copyRect->getTop() - imgOut->getTop(),
                                          copyRect->getWidth(), copyRect->getHeight())
							:
							new ImageRect(origin->getX() - imgOut->getLeft(),
                                          origin->getY() - imgOut->getTop(),
                                          copyRect->getWidth(), copyRect->getHeight());

    //  Subclasses should override this function, but in case they don't, we need to
    //  do the conversion through the base rgb raster.
    if (!gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    switch( imgOut->getBaseRasterType() )
    {
        case kBinaryRaster:
            //  The GWorld only works with unshifted rasters so we need to compute
            //  the "true" copy rectangles first
            //  At this point we are ready to perform the copy/conversion and we use
            //  the CopyBits function of QuickDraw/QuickTime
            gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, true);

            imgOut->gworldIsReference_ = true;
            imgOut->referenceHasChanged_ = true;
            break;

        case kGrayRaster:
            //	are we dealing with a gray or gray_F image?
            //
            //  The output image is a "regular" gray image [unsigned char]
            if (!imgOut->hasFloatRaster())
            {
                //  At this point we are ready to perform the copy/conversion and we use
                //  the CopyBits function of QuickDraw/QuickTime
                gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, true);

                imgOut->gworldIsReference_ = true;
                imgOut->referenceHasChanged_ = true;
              }
            //	if the image has a floating point raster, we write into it.
			//	why???
				else
            {
                const int   iLowSource = copyRect->getTop(),
                            iHighSource = copyRect->getBottom(),
                            jLowSource = copyRect->getLeft();
                int    iLowDest, jLowDest;
                //
                if (origin==NULL)
                {
                    iLowDest = iLowSource;
                    jLowDest = jLowSource;
                }
                else
                {
                    iLowDest = origin->getY();
                    jLowDest = origin->getX();
                }
                const int nbCols = getWidth();

                const unsigned char* const* rgbSource = getShiftedRGB2D();
                float* const* fltGrayDest= (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);
                for (int i=iLowSource, iDest=iLowDest; i<=iHighSource; i++, iDest++)
                    convertToGray(rgbSource[i]+jLowSource, fltGrayDest[iDest]+jLowDest, 
                                   nbCols, defaultGrayConv_);

                (static_cast<RasterImage_gray_F*>(imgOut))->setFloatConversionMode(kSaturatedPositive);
                imgOut->gworldIsReference_ = false;
                imgOut->referenceHasChanged_ = true;
            }
            break;

        case kRGB24Raster:
            //  At this point we are ready to perform the copy/conversion and we use
            //  the CopyBits function of QuickDraw/QuickTime
            gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, false);

            imgOut->gworldIsReference_ = true;
            imgOut->referenceHasChanged_ = true;
             break;

        case kRGBa32Raster:
            //	are we dealing with a RGB or RGB_F image?
            //
            //  The output image is a "regular" RGB image [unsigned char]
            if (!imgOut->hasFloatRaster())
            {
                 //  At this point we are ready to perform the copy/conversion and we use
                //  the CopyBits function of QuickDraw/QuickTime
                gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, false);

                imgOut->gworldIsReference_ = true;
                imgOut->referenceHasChanged_ = true;
            }
            // imgOut has float type
            else
            {
                const int   iLowSource = copyRect->getTop(),
                            iHighSource = copyRect->getBottom(),
                            jLowSource = copyRect->getLeft(),
                            jHighSource = copyRect->getRight();
                int iLowDest, jLowDest;
                //
                if (origin==NULL)
                {
                    iLowDest = iLowSource;
                    jLowDest = jLowSource;
                }
                else
                {
                    iLowDest = origin->getY();
                    jLowDest = origin->getX();
                }


                const unsigned char* const* rgbSource = getShiftedRGB2D();
                float* const* fltRedDest= (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
                float* const* fltGreenDest= (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
                float* const* fltBlueDest= (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);
                for (int i=iLowSource, iDest=iLowDest; i<=iHighSource; i++, iDest++)
    				for(int j=jLowSource, k=3*jLowSource, jDest=jLowDest; j<=jHighSource; j++, jDest++)
	                {
	                    fltRedDest[iDest][jDest] = rgbSource[i][k++];
						fltGreenDest[iDest][jDest] = rgbSource[i][k++];
						fltBlueDest[iDest][jDest] = rgbSource[i][k++];
					}

                (static_cast<RasterImage_RGBa_F*>(imgOut))->setFloatConversionMode(kSaturatedPositive);
                imgOut->gworldIsReference_ = false;
                imgOut->referenceHasChanged_ = true;
            }
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in RasterImage_gray::writeInto_");
            break;

    }

    if (sourceRect != NULL)
        delete sourceRect;
    if (destRect != NULL)
        delete destRect;

}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Conversion
//------------------------------------------------------
#endif


void RasterImage_RGB::convertToGray(const unsigned char* theSource,
                                    unsigned char* theDestination,
                                    long nbPixels,
                                    ConversionToGrayMode myGrayConv)
{
    const unsigned char   *source = theSource;
    unsigned char   *dest = theDestination;
    int             sum;
    float           sumF;

    //  how do we convert from rgb to gray?
    switch (myGrayConv)
    {

        case kMaxFieldConversion:

            for (long j=0; j<nbPixels; j++)
            {
                //  optimization "in the small" left for a more clever programmer
                unsigned char   red = *(source++),
                                green = *(source++),
                                blue = *(source++);
                *(dest++) = MAX(red, MAX(green, blue));
            }
            break;

        case kAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                sum = *(source++);
                sum += *(source++);
                sum += *(source++);
                *(dest++) = static_cast<unsigned char>(sum/3);
            }
            break;

        case kWeightedAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                sumF = defaultConvWeight_[0] ** (source++);
                sumF += defaultConvWeight_[1] ** (source++);
                sumF += defaultConvWeight_[2] ** (source++);
                *(dest++) = static_cast<unsigned char>(sumF);
            }
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "unknown conversion to gray mode encountered");
            break;

    }
}


void RasterImage_RGB::convertToGray(const unsigned char* theSource,
                                    unsigned char* theDestination,
                                    long nbPixels,
                                    const float convWeight[])
{
    const unsigned char   *source = theSource; 
    unsigned char   *dest = theDestination;
    float           sumF;


    for (long j=0; j<nbPixels; j++)
    {
        sumF = convWeight[0] ** (source++);
        sumF += convWeight[1] ** (source++);
        sumF += convWeight[2] ** (source++);
        *(dest++) = static_cast<unsigned char>(sumF);
    }
}


void RasterImage_RGB::convertToGray(const unsigned char* theSource,
                                    float* theDestination,
                                    long nbPixels,
                                    ConversionToGrayMode myGrayConv)
{
    const unsigned char* source = theSource;
    float           *dest = theDestination;
    float           sum;
    const float		div3 = 1.f/3.f;

    //  how do we convert from rgb to gray?
    switch (myGrayConv)
    {
        case kMaxFieldConversion:

            for (long j=0; j<nbPixels; j++)
            {
                //  optimization "in the small" left for a more clever programmer
                float   red = *(source++),
                        green = *(source++),
                        blue = *(source++);
                *(dest++) = MAX(red, MAX(green, blue));
            }
            break;

        case kAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                sum = *(source++);
                sum += *(source++);
                sum += *(source++);
                *(dest++) = sum*div3;
            }
            break;

        case kWeightedAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                sum = defaultConvWeight_[0] ** (source++);
                sum += defaultConvWeight_[1] ** (source++);
                sum += defaultConvWeight_[2] ** (source++);
                *(dest++) = sum;
            }
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "unknown conversion to gray mode encountered");
            break;

    }
}

void RasterImage_RGB::convertToGray(const unsigned char* theSource,
                                    float* theDestination,
                                    long nbPixels,
                                    const float convWeight[])
{
    const unsigned char   *source = theSource;
    float           *dest = theDestination;
    float           sum;

    for (long j=0; j<nbPixels; j++)
    {
        sum = convWeight[0] ** (source++);
        sum += convWeight[1] ** (source++);
        sum += convWeight[2] ** (source++);
        *(dest++) = sum;
    }
}

void RasterImage_RGB::convertToRGBa(const unsigned char* theSource,
                                    unsigned char* theDestination, 
                                    long nbPixels)
{
    unsigned char   *dest = theDestination;
    const unsigned char* source = theSource;

	try {
		for (long j=0; j<nbPixels; j++)
		{
			*(dest++) = *(source++);            //  red
			*(dest++) = *(source++);            //  green
			*(dest++) = *(source++);            //  blue
			*(dest++) = static_cast<unsigned char>(255);    //  alpha
		}
	}
	catch(...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"memcopy failed in RasterImage_RGB::convertToRGBa");
	}
}

void RasterImage_RGB::convertToRGB(const unsigned char* theSource,
                                   unsigned char* theDestination, 
                                   long nbPixels)
{
	try {
		//  copy the data (including possible padding bytes)
		memcpy(theDestination, theSource, static_cast<unsigned long>(3L*nbPixels));
	}
	catch(...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"memcopy failed in RasterImage_RGB::convertToRGB");
	}
}



void RasterImage_RGB::convertToARGB16(const unsigned char* theSource,
                                      unsigned char* theDestination,
                                      long nbPixels)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");

}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocations
//------------------------------------------------------
#endif

void RasterImage_RGB::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( propertiesAreLocked_,
                    kAccessToLockedData,
                    "Attempt to resize a RasterImage with locked properties");

    //	if width and height have not changed, then the operation is just a translation.
    //  All we need to do is update the ImageRect member and update the "shifted" array
    /// (no re-allocation needed)
    if ((theWidth == getWidth()) && (theHeight == getHeight()))
    {
        if ((x != getLeft()) || (y != getTop()))
        {
            //  Since the rasters have the same dimensions, there is no need
            //  to delete and reallocate the shifted rasters.  Just apply a
            //  relative shift to them
            const int	iLow  = getTop(),
						iHigh = getBottom(),
						jLow = getLeft(),
						rowShift = y - iLow,
						colShift = 3*(x - jLow);

if(shiftedRgb2D_)
{
            for (int i=iLow; i<=iHigh; i++)
                shiftedRgb2D_[i] += colShift;
            shiftedRgb2D_ += rowShift;
}

        }
        //  if nothing has changed, there is nothing to do
    }
    //	if the dimensions of the rectangles are different, then we must delete
    //	and reallocate the GraphWorld as well as the 0-based 2D raster.

    else
    {
        //	I am not sure if this is the right thing to do: should subclasses completely
        //	reimplement this or call this function?  This is opting for the latter.
        if (!graphWorldIsNull_())
        {
            delete gworld_;
            deleteShiftedRaster2D_(y);
            deleteRaster2D_();

            allocateGraphWorld_(theHeight, theWidth);


            allocateRaster2D_(theHeight, theWidth);
            allocateShiftedRaster2D_(theHeight, theWidth, y, x);
        }
    }
}


void RasterImage_RGB::allocateGraphWorld_(int nbRows, int nbCols)
{
	gworld_ = new GraphWorld(24, nbRows, nbCols);
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldAllocationError,
                    "GraphWorld allocation failed in RasterImage_gray constructor");
}


void RasterImage_RGB::allocateRaster2D_(int nbRows, int nbCols)
{
    unsigned char* rgb = gworld_->getRaster();
    const int   rowBytes = gworld_->getBytesPerRow();

    if (rgb2D_ != NULL)
        delete []rgb2D_;

    rgb2D_ = new unsigned char*[nbRows];
    FAIL_CONDITION(rgb2D_ == NULL, kRasterAllocationFailure,
                   "Could not allocate rgb2D_ array");

    rgb2D_[0] = rgb;
    for (int i=1; i<nbRows; i++)
        rgb2D_[i] = rgb2D_[i-1] + rowBytes;

}

void RasterImage_RGB::deleteRaster2D_(void)
{
    if (rgb2D_ != NULL)
    {
        delete []rgb2D_;
        rgb2D_ = NULL;
    }
}

void RasterImage_RGB::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
    {
        const int   iHigh = iLow + nbRows - 1,
                    bytesPerRow = getBytesPerRow();

        deleteShiftedRaster2D_(iLow);
        shiftedRgb2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	shiftedRgb2D_ == NULL,
                        kRasterAllocationFailure,
                        "2D RGB color  raster allocation failed");

        shiftedRgb2D_ -= iLow;
        //
        shiftedRgb2D_[iLow] = getRGB(R_W_ACCESS) - 3*jLow;
        FAIL_CONDITION(	shiftedRgb2D_ == NULL,
                        kRasterAllocationFailure,
                        "2D RGB color  raster allocation failed");
        for (int i = iLow; i < iHigh; i++)
            shiftedRgb2D_[i+1] = shiftedRgb2D_[i] + bytesPerRow;

    }
    else
        shiftedRgb2D_ = NULL;

}

void RasterImage_RGB::deleteShiftedRaster2D_(int iLow)
{
    if (shiftedRgb2D_ != NULL)
    {
        shiftedRgb2D_ += iLow;
        delete []shiftedRgb2D_;
        shiftedRgb2D_ = NULL;
    }
}


