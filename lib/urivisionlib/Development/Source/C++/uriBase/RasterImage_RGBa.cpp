/*  NAME:
        VectorField.cpp

    DESCRIPTION:
        implementation of the uriVisionLib RasterImage_RGBa class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <string>
#include <typeinfo>
#include <cmath>
#include <cstring>
//
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_RGBa_F.h"
#include "RasterImage_HSV.h"

using namespace uriVL;

#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

RasterImage_RGBa::RasterImage_RGBa(void)
		:	RasterImage( ),
			//
			rgba2D_(NULL),
			shiftedRgba2D_(NULL)		
{
}


RasterImage_RGBa::RasterImage_RGBa(int nbRows, int nbCols)
	try	:	RasterImage(nbRows, nbCols),
			//
			rgba2D_(NULL),
			shiftedRgba2D_(NULL)
{
	try {
		allocateGraphWorld_(nbRows, nbCols);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGBa constructor");
		throw e;
	}
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
    //
	setDrawableInformationType(kDense);
    setGraphWorldAsReference_();
    referenceHasChanged_ = false;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa constructor");
	throw e;
}



RasterImage_RGBa::RasterImage_RGBa(int nbRows, int nbCols, const unsigned char* theRGBa)
	try	:	RasterImage(nbRows, nbCols),
			//
			rgba2D_(NULL),
			shiftedRgba2D_(NULL)
{
	try {
		allocateGraphWorld_(nbRows, nbCols);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGBa constructor");
		throw e;
	}

    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);

	try {
		const unsigned char   *source = theRGBa;
		for (int i=0; i<nbRows; i++)
		{
			memcpy(rgba2D_[i], source, static_cast<unsigned long>(4L*nbCols));
			source += 4*nbCols;
		}
	}
	catch (...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"Data copy failure in RasterImage_RGBa constructor");
	}

	setValidRect(0, 0, nbCols, nbRows);
    //
	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa constructor");
	throw e;
}



RasterImage_RGBa::RasterImage_RGBa(const ImageRect* theRect)
	try	:	RasterImage(theRect),
			//
			rgba2D_(NULL),
			shiftedRgba2D_(NULL)
{
	try {
		allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGBa constructor");
		throw e;
	}
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(), theRect->getTop(), theRect->getLeft());
	//
	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = false;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa constructor");
	throw e;
}



RasterImage_RGBa::RasterImage_RGBa(const ImageRect* theRect, const unsigned char* theRGBa)
	try	:	RasterImage(theRect),
			//
			rgba2D_(NULL),
			shiftedRgba2D_(NULL)
{
	try {
		allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGBa constructor");
		throw e;
	}

    const int   iLow = theRect->getTop(),
        		jLow = theRect->getLeft(),
				nbRows = theRect->getHeight(),
				nbCols = theRect->getWidth();

    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, iLow, jLow);

	try {
		const unsigned char   *source = theRGBa;
		for (int i=0; i<nbRows; i++)
		{
			memcpy(rgba2D_[i], source, static_cast<unsigned long>(4L*nbCols));
			source += 4*nbCols;
		}
	}
	catch (...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"Data copy failure in RasterImage_RGBa constructor");
	}

	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa constructor");
	throw e;
}



RasterImage_RGBa::RasterImage_RGBa(GraphWorld* theGworld, bool keepThisGraphWorld)
	try	:	RasterImage(theGworld, keepThisGraphWorld),
			//
			rgba2D_(NULL),
			shiftedRgba2D_(NULL)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"RasterImage_RGBa Constructor not implemented");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa constructor");
	throw e;
}


RasterImage_RGBa::RasterImage_RGBa(const RasterImage* img)
	try	:	RasterImage( ),
			//
			rgba2D_(NULL),
			shiftedRgba2D_(NULL)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_RGBa constructor");

	try {
		img->copyInto(this, img->getValidRect());
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGBa constructor");
		throw e;
	}
	
    gworldIsReference_ = true;
    referenceHasChanged_ = false;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa constructor");
	throw e;
}


RasterImage_RGBa::RasterImage_RGBa(const RasterImage* img, const ImageRect* theRect)
	try	:	RasterImage( ),
			//
			rgba2D_(NULL),
			shiftedRgba2D_(NULL)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_RGBa constructor");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in RasterImage_RGBa constructor");

	try {
		img->copyInto(this, theRect);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGBa constructor");
		throw e;
	}

    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa constructor");
	throw e;
}


RasterImage_RGBa::RasterImage_RGBa(RasterImage_RGBa& img)
    try	:	RasterImage( ),
			//
			rgba2D_(NULL),
			shiftedRgba2D_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_RGBa copy constructor not implemented");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa constructor");
	throw e;
}

RasterImage_RGBa::~RasterImage_RGBa(void)
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

BaseRasterType RasterImage_RGBa::getBaseRasterType(void) const
{
    return kRGBa32Raster;
}

const unsigned char* RasterImage_RGBa::getRGBa(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGBa subclass did not properly override getRGBa()");

    return gworld_->getRaster();
}


unsigned char* RasterImage_RGBa::getRGBa(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGBa subclass did not properly override getRGBa()");
    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return gworld_->getRaster();
}



const unsigned char* const* RasterImage_RGBa::getRGBa2D(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGBa subclass did not properly override getRGBa2D()");
    return rgba2D_;
}


unsigned char*const* RasterImage_RGBa::getRGBa2D(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGBa subclass did not properly override getRGBa2D()");

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return rgba2D_;
}

const unsigned char* const* RasterImage_RGBa::getShiftedRGBa2D(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_RGBa subclass did not properly override getRGBa2D()");

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return rgba2D_;
    else
        return shiftedRgba2D_;

}

unsigned char* const* RasterImage_RGBa::getShiftedRGBa2D(ReadWriteStatus rw)
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
        return rgba2D_;
    else
        return shiftedRgba2D_;

}


/*     Should update the non-reference raster, using the values stored in the reference raster.
 *      In the case of an rgb image, the only raster is the one stored in the GraphWorld.  There
 *      is therefore nothing to do.
 */
void RasterImage_RGBa::updateRaster_(void) const
{
	//something is not right.  We end up here for an RGBa image!!
    if (hasFloatRaster_) 
	{
		if (floatRasterIsReference_)
		{	
			RasterImage_RGBa_F* tempF = (RasterImage_RGBa_F*)this;
			tempF->updateRaster_();
		}
	}
	else
	{	
		//	ugly hack.  Change this ASAP
		if (hasOtherRaster_ && !gworldIsReference_ && !referenceHasChanged_)
		{
			RasterImage_HSV* tempHSV = (RasterImage_HSV*)this;
			tempHSV->updateRaster_();
		}

	}
	
    referenceHasChanged_ = false;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copy and Write
//------------------------------------------------------
#endif

const RasterImage_RGBa& RasterImage_RGBa::operator = (const RasterImage_RGBa& img) {
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_RGBa copy operator not implemented");
	return *this;
}

void RasterImage_RGBa::copyInto(RasterImage* imgOut) const
{
    copyInto(imgOut, getValidRect(), NULL);
}

void RasterImage_RGBa::copyInto(RasterImage* imgOut, const ImageRect* theRect) const
{
    copyInto(imgOut, theRect, NULL);
}

void RasterImage_RGBa::copyInto(RasterImage* imgOut, const ImageRect* theRect, 
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

    delete copyRect;
}

void RasterImage_RGBa::writeInto_(RasterImage* imgOut, const ImageRect* copyRect,
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

	const unsigned char* const* rgbaSource = getShiftedRGBa2D();
	const int   iLowSource = sourceRect->getTop(),
				iLowDest = destRect->getTop(),
				iHighSource = sourceRect->getBottom(),
				jLowSource = sourceRect->getLeft(),
				jHighSource = sourceRect->getRight(),
				jLowDest = destRect->getLeft();
	const int	nbRows = sourceRect->getHeight(),
				nbCols = sourceRect->getWidth();

    //  Subclasses should override this function, but in case they don't, we need to
    //  do the conversion through the base gray-level raster.
    if (!gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    switch( imgOut->getBaseRasterType() )
    {
        case kBinaryRaster:

            //  At this point we are ready to perform the copy/conversion and we use
            //  the CopyBits function of QuickDraw/QuickTime
            gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, true);

            imgOut->gworldIsReference_ = true;
            imgOut->referenceHasChanged_ = true;
            break;

        case kGrayRaster:
            //	are we dealing with a gray, gray_F, or gray_D image?
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
            //	if the image has a floating point raster, we have no choice but to do the
            //  conversion "by hand"
            else
            {
                float* const* fltGrayDest= ((RasterImage_gray_F *)imgOut)->getShiftedGrayF2D(R_W_ACCESS);
                for (int i=iLowSource, iDest=iLowDest; i<=iHighSource; i++, iDest++)
                    convertToGray(rgbaSource[i]+jLowSource, fltGrayDest[iDest]+jLowDest, 
                                   nbCols, defaultGrayConv_);

                ((RasterImage_gray_F *)imgOut)->setFloatConversionMode(kSaturatedPositive);
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

        case kaRGB16Raster:
            //  At this point we are ready to perform the copy/conversion and we use
            //  the CopyBits function of QuickDraw/QuickTime
            gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, false);

            imgOut->gworldIsReference_ = true;
            imgOut->referenceHasChanged_ = true;
            break;

        case kRGBa32Raster:
        	// imgOut has integer type
			if (!imgOut->hasFloatRaster())
	        {
                //  This is a temporary hack until we agree on a general strategy
                //  for handling future (third-party) color space implementations
                if (typeid(*imgOut) == typeid(uriVL::RasterImage_HSV))
                {
                    //  we already know that imgOut is at least as large as the
                    //  copy region
                    //((RasterImage_HSV* ) imgOut)->convertFromRGBa_(this, copyRect);
					//
					//jyh 2012-12-01 
					//We still copy the data into the gwordl
                    gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, false);

                    imgOut->gworldIsReference_ = true;
                    imgOut->referenceHasChanged_ = true;
                }
                else
                {
                    //  At this point we are ready to perform the copy/conversion and we use
                    //  the CopyBits function of QuickDraw/QuickTime
                    gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, false);

                    imgOut->gworldIsReference_ = true;
                    imgOut->referenceHasChanged_ = true;
                }
            }
            // imgOut has float type
            else
            {
                float* const* fRedDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
                float* const* fGreenDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
                float* const* fBlueDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);

				//	This call may have been made from within the RasterImage_RGBa_F constructor of imgOut,
				//	in which case grayDest has not been allocated yet
				if ((fRedDest == NULL) || (fGreenDest == NULL)  || (fBlueDest == NULL))
				{
					imgOut->allocateRaster2D_(nbRows, nbCols);
					imgOut->allocateShiftedRaster2D_(nbRows, nbCols, iLowDest, jLowDest);
					fRedDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
					fGreenDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
					fBlueDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);
				}

                for (int i=iLowSource, iDest=iLowDest; i<=iHighSource; i++, iDest++)
    				for(int j=jLowSource, k=4*jLowSource, jDest=jLowDest; j<=jHighSource; j++, k++, jDest++)
	                {
	                    fRedDest[iDest][jDest] = rgbaSource[i][k++];
						fGreenDest[iDest][jDest] = rgbaSource[i][k++];
						fBlueDest[iDest][jDest] = rgbaSource[i][k++];
					}

                static_cast<RasterImage_RGBa_F *>(imgOut)->setFloatConversionMode(kSaturatedPositive);
                imgOut->gworldIsReference_ = false;
                imgOut->referenceHasChanged_ = true;
            }
            break;

        default:
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


void RasterImage_RGBa::convertToGray(const unsigned char* theSource,
                                     unsigned char* theDestination,
                                     long nbPixels,
                                     ConversionToGrayMode myGrayConv)
{
    const unsigned char   *source = theSource;
    unsigned char   *dest = theDestination;
    int     sum;
    float   sumF;

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
                source++;
            }
            break;

        case kAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                sum = *(source++);
                sum += *(source++);
                sum += *(source++);
                *(dest++) = static_cast<unsigned char>(sum/3);
                source++;
            }
            break;

        case kWeightedAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                sumF = defaultConvWeight_[0] ** (source++);
                sumF += defaultConvWeight_[1] ** (source++);
                sumF += defaultConvWeight_[2] ** (source++);
                *(dest++) = static_cast<unsigned char>(sumF);
                source++;
            }
            break;

        default:
            FAIL_CONDITION( true, kRasterImageError,
                            "unknown conversion to gray mode encountered");
            break;

    }
}

void RasterImage_RGBa::convertToGray(const unsigned char* theSource,
                                     unsigned char* theDestination,
                                     long nbPixels,
                                     const float convWeight[])
{
    const unsigned char   *source = theSource;
    unsigned char   *dest = theDestination;
    float           sumF;

    dest = theDestination;

    for (long j=0; j<nbPixels; j++)
    {
        sumF = convWeight[0] ** (source++);
        sumF += convWeight[1] ** (source++);
        sumF += convWeight[2] ** (source++);
        *(dest++) = static_cast<unsigned char>(sumF);
        source++;
    }
}


void RasterImage_RGBa::convertToGray(const unsigned char* theSource,
                                     float* theDestination,
                                     long nbPixels,
                                     ConversionToGrayMode myGrayConv)
{
    const unsigned char   *source = theSource;
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
                source++;
            }
            break;

        case kAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                sum = *(source++);
                sum += *(source++);
                sum += *(source++);
                *(dest++) = sum*div3;
                source++;
            }
            break;

        case kWeightedAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                sum = defaultConvWeight_[0] ** (source++);
                sum += defaultConvWeight_[1] ** (source++);
                sum += defaultConvWeight_[2] ** (source++);
                *(dest++) = sum;
                source++;
            }
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "unknown conversion to gray mode encountered");
            break;

    }
}

void RasterImage_RGBa::convertToGray(const unsigned char* theSource,
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
        source++;
    }
}


void RasterImage_RGBa::convertToRGBa(const unsigned char* theSource,
                                     unsigned char* theDestination,
                                     long nbPixels)
{
    memcpy(theDestination, theSource, static_cast<unsigned long>(4L*nbPixels));
}


void RasterImage_RGBa::convertToRGB(const unsigned char* theSource,
                                    unsigned char* theDestination,
                                    long nbPixels)
{
    unsigned char   *dest = theDestination;
    const unsigned char* source = theSource;

    for (long j=0; j<nbPixels; j++)
    {
        *(dest++) = *(source++);        //  red
        *(dest++) = *(source++);        //  green
        *(dest++) = *(source++);        //  blue
        source++;
    }
}

void RasterImage_RGBa::convertToARGB16(const unsigned char* theSource,
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
#pragma mark Local RGBa
//------------------------------------------------------
#endif


const RasterImage_RGBa* RasterImage_RGBa::localRasterRGBa(const RasterImage* imgIn, bool* localRGBa)
{
    return localRasterRGBa(imgIn, imgIn->getValidRect(), localRGBa);
}


const RasterImage_RGBa* RasterImage_RGBa::localRasterRGBa(const RasterImage* imgIn, 
                                                    const ImageRect* rect, bool* localRGBa)
{
    const RasterImage_RGBa* rgbaImg = NULL;

    //	if imgIn is alread a ReasterImage_RGBa --> then just use its rasters
    if (imgIn->getBaseRasterType() == kRGBa32Raster)
    {
        rgbaImg = static_cast<const RasterImage_RGBa*>(imgIn);
        *localRGBa = false;
    }
    //	if imgIn was not a RasterImage_RGBa, we need to create a local rgba
    //	image that is converted from imgIn
    else
    {
        rgbaImg = new RasterImage_RGBa(imgIn, rect);
        *localRGBa = true;
    }

    return rgbaImg;
}

RasterImage_RGBa* RasterImage_RGBa::localRasterRGBa(RasterImage* imgIn, bool* localRGBa)
{
    return const_cast<RasterImage_RGBa*>(localRasterRGBa(static_cast<const RasterImage* >(imgIn), 
										 imgIn->getValidRect(), localRGBa));
}


RasterImage_RGBa* RasterImage_RGBa::localRasterRGBa(RasterImage* imgIn, 
                                                    const ImageRect* rect, bool* localRGBa)
{
    return const_cast<RasterImage_RGBa*>(localRasterRGBa(static_cast<const RasterImage* >(imgIn), 
										 rect, localRGBa));
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocations
//------------------------------------------------------
#endif

void RasterImage_RGBa::allocateGraphWorld_(int nbRows, int nbCols)
{
	gworld_ = new GraphWorld(32, nbRows, nbCols);
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldAllocationError,
                    "GraphWorld allocation failed in RasterImage_RGBa::allocateGraphWorld_");
}


void RasterImage_RGBa::allocateRaster2D_(int nbRows, int nbCols)
{
    unsigned char* rgba = gworld_->getRaster();
    const int     rowBytes = gworld_->getBytesPerRow();

    if (rgba2D_ != NULL)
        delete []rgba2D_;

    rgba2D_ = new unsigned char*[nbRows];
    FAIL_CONDITION(rgba2D_ == NULL, kRasterAllocationFailure,
                   "Could not allocate rgba2D_ array");

    rgba2D_[0] = rgba;
    for (int i=1; i<nbRows; i++)
        rgba2D_[i] = rgba2D_[i-1] + rowBytes;

}


void RasterImage_RGBa::deleteRaster2D_(void)
{
    if (rgba2D_ != NULL)
    {
        delete []rgba2D_;
        rgba2D_ = NULL;
    }
}

void RasterImage_RGBa::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
    {
        const int   iHigh = iLow + nbRows - 1,
                    bytesPerRow = getBytesPerRow();

        deleteShiftedRaster2D_(iLow);
        shiftedRgba2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	shiftedRgba2D_ == NULL,
                        kRasterAllocationFailure,
                        "2D RGBa color  raster allocation failed");

        shiftedRgba2D_ -= iLow;
        //
        shiftedRgba2D_[iLow] = getRGBa(R_W_ACCESS) - 4*jLow;
        for (int i = iLow; i < iHigh; i++)
            shiftedRgba2D_[i+1] = shiftedRgba2D_[i] + bytesPerRow;

    }
    else
        shiftedRgba2D_ = NULL;

}

void RasterImage_RGBa::deleteShiftedRaster2D_(int iLow)
{
    if (shiftedRgba2D_ != NULL)
    {
        shiftedRgba2D_ += iLow;
        delete []shiftedRgba2D_;
        shiftedRgba2D_ = NULL;
    }
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Set Rects
//------------------------------------------------------
#endif

void RasterImage_RGBa::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( propertiesAreLocked_,
                    kAccessToLockedData,
                    "Attempt to resize a RasterImage with locked properties");

    //	if width and height have not changed, then the operation is just a translation.
    //  The ImageRect member has been updated in DrawableObject2D::setBoundRect, so we only need
	//	to update the "shifted" array (no re-allocation needed)
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
						colShift = 4*(x - jLow);

			if(shiftedRgba2D_)
			{
						for (int i=iLow; i<=iHigh; i++)
							shiftedRgba2D_[i] += colShift;
						shiftedRgba2D_ += rowShift;
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


