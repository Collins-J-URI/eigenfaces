/*  NAME:
        RasterImage_aRGB16.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RasterImage_aRGB16 class
 
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
#include "RasterImage_aRGB16.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_RGBa_F.h"
#include "RasterImage_HSV.h"

using namespace uriVL;

//-----------------------------------
//  Redeclaration of static variables
//-----------------------------------

float* RasterImage_aRGB16::lutGrayF_ = NULL;
short *RasterImage_aRGB16::lutHue_ = NULL;
unsigned char* RasterImage_aRGB16::lutSat_ = NULL;
unsigned char* RasterImage_aRGB16::lutVal_ = NULL;
			
#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

RasterImage_aRGB16::RasterImage_aRGB16(void)
		:	RasterImage( ),
			//
			argb2D_(NULL),
			shiftedArgb2D_(NULL)
{
}


RasterImage_aRGB16::RasterImage_aRGB16(int nbRows, int nbCols)
	try	:	RasterImage(nbRows, nbCols),
			//
			argb2D_(NULL),
			shiftedArgb2D_(NULL)

{
	try {
		allocateGraphWorld_(nbRows, nbCols);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_aRGB16 constructor");
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
	e.appendToMessage("called by RasterImage_aRGB16 constructor");
	throw e;
}


RasterImage_aRGB16::RasterImage_aRGB16(const ImageRect* theRect)
	try	:	RasterImage(theRect),
			//
			argb2D_(NULL),
			shiftedArgb2D_(NULL)
{
	try {
		allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_aRGB16 constructor");
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
	e.appendToMessage("called by RasterImage_aRGB16 constructor");
	throw e;
}


RasterImage_aRGB16::RasterImage_aRGB16(int nbRows, int nbCols, const unsigned char* theARGB)
	try	:	RasterImage(nbRows, nbCols),
			//
			argb2D_(NULL),
			shiftedArgb2D_(NULL)
{
	try {
		allocateGraphWorld_(nbRows, nbCols);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_aRGB16 constructor");
		throw e;
	}

    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
    //
	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = true;

	try {
		const unsigned char   *source = theARGB;
		for (int i=0; i<nbRows; i++)
		{
			memcpy(argb2D_[i], source, static_cast<unsigned long>(2L*nbCols));
			source += 2*nbCols;
		}
	}
	catch (...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"Data copy failure in RasterImage_aRGB16 constructor");
	}
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_aRGB16 constructor");
	throw e;
}



RasterImage_aRGB16::RasterImage_aRGB16(GraphWorld* theGworld, bool keepThisGraphWorld)
	try	:	RasterImage(theGworld, keepThisGraphWorld),
			//
			argb2D_(NULL),
			shiftedArgb2D_(NULL)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"RasterImage_aRGB16 Constructor not implemented");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_aRGB16 constructor");
	throw e;
}


RasterImage_aRGB16::RasterImage_aRGB16(RasterImage* img)
	try	:	RasterImage( ),
			//
			argb2D_(NULL),
			shiftedArgb2D_(NULL)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_aRGB16 constructor");

	try {
		img->copyInto(this);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_aRGB16 copy constructor");
		throw e;
	}
	
    gworldIsReference_ = true;
    referenceHasChanged_ = false;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_aRGB16 copy constructor");
	throw e;
}


RasterImage_aRGB16::RasterImage_aRGB16(RasterImage* img, const ImageRect* theRect)
	try	:	RasterImage( ),
			//
			argb2D_(NULL),
			shiftedArgb2D_(NULL)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_aRGB16 constructor");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in RasterImage_aRGB16 constructor");

	try {
		img->copyInto(this, theRect);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_aRGB16 constructor");
		throw e;
	}

    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_aRGB16 copy constructor");
	throw e;
}


RasterImage_aRGB16::RasterImage_aRGB16(const RasterImage_aRGB16& img)
    try    :	RasterImage( ),
			//
			argb2D_(NULL),
			shiftedArgb2D_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_aRGB16 copy constructor not implemented");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_aRGB16 constructor");
	throw e;
}

RasterImage_aRGB16::~RasterImage_aRGB16(void)
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

BaseRasterType RasterImage_aRGB16::getBaseRasterType(void) const
{
    return kaRGB16Raster;
}

const unsigned char* RasterImage_aRGB16::getARGB(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_aRGB16 subclass did not properly override getRGBa()");

    return gworld_->getRaster();
}


unsigned char* RasterImage_aRGB16::getARGB(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_aRGB16 subclass did not properly override getRGBa()");
    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return gworld_->getRaster();
}



const unsigned char* const* RasterImage_aRGB16::getARGB2D(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_aRGB16 subclass did not properly override getRGBa2D()");
    return argb2D_;
}


unsigned char* const* RasterImage_aRGB16::getARGB2D(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_aRGB16 subclass did not properly override getRGBa2D()");

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return argb2D_;
}

const unsigned char* const* RasterImage_aRGB16::getShiftedARGB2D(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_aRGB16 subclass did not properly override getRGBa2D()");

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return argb2D_;
    else
        return shiftedArgb2D_;
}

unsigned char* const* RasterImage_aRGB16::getShiftedARGB2D(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_aRGB16 subclass did not properly override getRGBa2D()");

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return argb2D_;
    else
        return shiftedArgb2D_;
}


/*     Should update the non-reference raster, using the values stored in the reference raster.
 *      In the case of an rgb image, the only raster is the one stored in the GraphWorld.  There
 *      is therefore nothing to do.
 */
void RasterImage_aRGB16::updateRaster_(void) const
{
    referenceHasChanged_ = false;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copy and Write
//------------------------------------------------------
#endif

const RasterImage_aRGB16& RasterImage_aRGB16::operator = (const RasterImage_aRGB16& img) {
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_aRGB16 copy operator not implemented");
	return *this;
}

void RasterImage_aRGB16::copyInto(RasterImage* imgOut) const
{
    copyInto(imgOut, getValidRect(), NULL);
}

void RasterImage_aRGB16::copyInto(RasterImage* imgOut, const ImageRect* theRect) const
{
    copyInto(imgOut, theRect, NULL);
}

void RasterImage_aRGB16::copyInto(RasterImage* imgOut, const ImageRect* theRect, 
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

void RasterImage_aRGB16::writeInto_(RasterImage* imgOut, const ImageRect* copyRect,
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
                const int   iLowSource = copyRect->getTop(),
                            iHighSource = copyRect->getBottom(),
                            jLowSource = copyRect->getLeft();
                int         iLowDest, jLowDest;
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

                const unsigned char* const* argbSource = getShiftedARGB2D();
                float* const* fltGrayDest= ((RasterImage_gray_F *)imgOut)->getShiftedGrayF2D(R_W_ACCESS);
                for (int i=iLowSource, iDest=iLowDest; i<=iHighSource; i++, iDest++)
                    convertToGray(argbSource[i]+jLowSource, fltGrayDest[iDest]+jLowDest, 
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
//jyh
//                    ((RasterImage_HSV* ) imgOut)->convertFromARGB16_(this, copyRect);

					FAIL_CONDITION( true,
									kFunctionNotImplemented,
									"aRGB16 to HSV not implemented yet");

                    imgOut->gworldIsReference_ = false;
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


                const unsigned char* const* rgbaSource = getShiftedARGB2D();
                float* const* fltRedDest= (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
                float* const* fltGreenDest= (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
                float* const* fltBlueDest= (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);
                for (int i=iLowSource, iDest=iLowDest; i<=iHighSource; i++, iDest++)
    				for(int j=jLowSource, k=4*jLowSource, jDest=jLowDest; j<=jHighSource; j++, k++, jDest++)
	                {
	                    fltRedDest[iDest][jDest] = rgbaSource[i][k++];
						fltGreenDest[iDest][jDest] = rgbaSource[i][k++];
						fltBlueDest[iDest][jDest] = rgbaSource[i][k++];
					}

                ((RasterImage_RGBa_F *)imgOut)->setFloatConversionMode(kSaturatedPositive);
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
#endif
//------------------------------------------------------
//	Conversion
//------------------------------------------------------

void RasterImage_aRGB16::convertToGray(const unsigned char* theSource,
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

void RasterImage_aRGB16::convertToGray(const unsigned char* theSource,
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


void RasterImage_aRGB16::convertToGray(const unsigned char* theSource,
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

void RasterImage_aRGB16::convertToGray(const unsigned char* theSource,
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


void RasterImage_aRGB16::convertToRGBa(const unsigned char* theSource,
                                     unsigned char* theDestination,
                                     long nbPixels)
{
    memcpy(theDestination, theSource, static_cast<unsigned long>(2L*nbPixels));
}


void RasterImage_aRGB16::convertToRGB(const unsigned char* theSource,
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

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocations
//------------------------------------------------------
#endif

void RasterImage_aRGB16::allocateGraphWorld_(int nbRows, int nbCols)
{
	gworld_ = new GraphWorld(16, nbRows, nbCols);
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldAllocationError,
                    "GraphWorld allocation failed in RasterImage_aRGB16::allocateGraphWorld_");
}


RasterImage_aRGB16* RasterImage_aRGB16::localRasterARGB16(RasterImage* imgIn, bool* localARGB16)
{
    return localRasterARGB16(imgIn, imgIn->getValidRect(), localARGB16);
}


RasterImage_aRGB16* RasterImage_aRGB16::localRasterARGB16(RasterImage* imgIn, 
                                                    const ImageRect* rect, bool* localARGB16)
{
    RasterImage_aRGB16* argb16Img = NULL;

    //	if imgIn is alread a ReasterImage_aRGB16 --> then just use its rasters
    if (imgIn->getBaseRasterType() == kaRGB16Raster)
    {
        argb16Img = static_cast<RasterImage_aRGB16*>(imgIn);
        *localARGB16 = false;
    }
    //	if imgIn was not a RasterImage_aRGB16, we need to create a local argb16
    //	image that is converted from imgIn
    else
    {
        argb16Img = new RasterImage_aRGB16(imgIn, rect);
        *localARGB16 = true;
    }

    return argb16Img;
}




void RasterImage_aRGB16::allocateRaster2D_(int nbRows, int nbCols)
{
    unsigned char* argb = gworld_->getRaster();
    const int rowBytes = gworld_->getBytesPerRow();

    if (argb2D_ != NULL)
        delete []argb2D_;

    argb2D_ = new unsigned char*[nbRows];
    FAIL_CONDITION(argb2D_ == NULL, kRasterAllocationFailure,
                   "Could not allocate rgba2D_ array");

    argb2D_[0] = argb;
    for (int i=1; i<nbRows; i++)
        argb2D_[i] = argb2D_[i-1] + rowBytes;

}


void RasterImage_aRGB16::deleteRaster2D_(void)
{
    if (argb2D_ != NULL)
    {
        delete []argb2D_;
        argb2D_ = NULL;
    }
}

void RasterImage_aRGB16::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
    {
        const int   iHigh = iLow + nbRows - 1,
                    bytesPerRow = getBytesPerRow();

        deleteShiftedRaster2D_(iLow);
        shiftedArgb2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	shiftedArgb2D_ == NULL,
                        kRasterAllocationFailure,
                        "2D RGBa color  raster allocation failed");

        shiftedArgb2D_ -= iLow;
        //
        shiftedArgb2D_[iLow] = getARGB(R_W_ACCESS) - 4*jLow;
        for (int i = iLow; i < iHigh; i++)
            shiftedArgb2D_[i+1] = shiftedArgb2D_[i] + bytesPerRow;

    }
    else
        shiftedArgb2D_ = NULL;

}

void RasterImage_aRGB16::deleteShiftedRaster2D_(int iLow)
{
    if (shiftedArgb2D_ != NULL)
    {
        shiftedArgb2D_ += iLow;
        delete []shiftedArgb2D_;
        shiftedArgb2D_ = NULL;
    }
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Lookup Table
//------------------------------------------------------
#endif

void RasterImage_aRGB16::initializeLutGray(void)
{
	if (lutGrayF_ != NULL)
	{
		lutGrayF_ = new float[32768];
		lutGrayF_ -= 32768;
	}
}

void RasterImage_aRGB16::initializeLutHSV(void)
{
	if (lutHue_ != NULL)
	{
		lutHue_ = new short[32768];
		lutHue_ -= 32768;
		//
		lutSat_ = new unsigned char[32768];
		lutSat_ -= 32768;
		//
		lutVal_ = new unsigned char[32768];
		lutVal_ -= 32768;
	}
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Set Rects
//------------------------------------------------------
#endif

void RasterImage_aRGB16::setBoundRect_(int x, int y, int theWidth, int theHeight)
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
						colShift = 2*(x - jLow);

if(shiftedArgb2D_)
{
            for (int i=iLow; i<=iHigh; i++)
                shiftedArgb2D_[i] += colShift;
            shiftedArgb2D_ += rowShift;
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
