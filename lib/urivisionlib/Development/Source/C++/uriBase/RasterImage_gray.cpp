/*  NAME:
        RasterImage_gray.cpp

    DESCRIPTION:
        implementation of the uriVisionLib RasterImage_gray class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstring>
#include <cmath>
//
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa_F.h"

using namespace uriVL;

#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

RasterImage_gray::RasterImage_gray(void)
        :	RasterImage(),
			//
			gray2D_(NULL),
			shiftedGray2D_(NULL)
{
}


RasterImage_gray::RasterImage_gray(int nbRows, int nbCols)
	try	:	RasterImage(nbRows, nbCols),
			//
			gray2D_(NULL),
			shiftedGray2D_(NULL)
{
	allocateGraphWorld_(nbRows, nbCols);
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
    //
	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = false;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray constructor");
	throw e;
}

RasterImage_gray::RasterImage_gray(int nbRows, int nbCols, const unsigned char* theGray)
	try	:	RasterImage(nbRows, nbCols),
			//
			gray2D_(NULL),
			shiftedGray2D_(NULL)
{
	allocateGraphWorld_(nbRows, nbCols);
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
    //
	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = true;

	try {
		const unsigned char   *source = theGray;
		for (int i=0; i<nbRows; i++)
		{
			memcpy(gray2D_[i], source, static_cast<unsigned long>(nbCols));
			source += nbCols;
		}
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_gray constructor");
		throw e;
	}

	setValidRect(0, 0, nbCols, nbRows);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray constructor");
	throw e;
}



RasterImage_gray::RasterImage_gray(const ImageRect* theRect)
    try	:	RasterImage(theRect),
			//
			gray2D_(NULL),
			shiftedGray2D_(NULL)

{
	allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(), theRect->getTop(), theRect->getLeft());
	//
	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = false;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray constructor");
	throw e;
}


RasterImage_gray::RasterImage_gray(const ImageRect* theRect, const unsigned char* theGray)
    try	:	RasterImage(theRect),
			//
			gray2D_(NULL),
			shiftedGray2D_(NULL)

{
	allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(), theRect->getTop(), theRect->getLeft());

    const int   iLow = theRect->getTop(),
        		iHigh = theRect->getBottom(),
        		jLow = theRect->getLeft(),
        		jHigh = theRect->getRight();

	try {
		long    lSource = 0;
		for (int i=iLow; i<=iHigh; i++)
			for (int j=jLow; j<=jHigh; j++)
				shiftedGray2D_[i][j] = theGray[lSource++];
	}
	catch(...) {
		FAIL_CONDITION( theGray == NULL,
						kNullRasterError,
						"null raster as parameter in RasterImage_gray_F constructor");
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"Data copy error in RasterImage_gray_F constructor");
	}

	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = false;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray constructor");
	throw e;
}



RasterImage_gray::RasterImage_gray(const GraphWorld* theGworld)
	try	:	RasterImage(theGworld),
			//
			gray2D_(NULL),
			shiftedGray2D_(NULL)
{
//    gworld_ = new GraphWorld(theGworld);
//    FAIL_CONDITION( gworld_ == NULL,
//                    kGWorldAllocationError,
//                    "GraphWorld allocation failed in RasterImage_gray constructor");
//
////    allocateRaster2D_();
////    allocateShiftedRaster2D_( );
//
//	setDrawableInformationType(kDense);
//    gworldIsReference_ = true;
//    referenceHasChanged_ = true;
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"RasterImage_gray Constructor not implemented");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray constructor");
	throw e;
}


RasterImage_gray::RasterImage_gray(GraphWorld* theGworld, bool keepThisGraphWorld)
		try	:	RasterImage(theGworld, keepThisGraphWorld),
				//
				gray2D_(NULL),
				shiftedGray2D_(NULL)
{
//    allocateRaster2D_();
//    allocateShiftedRaster2D_( );
//
//	setDrawableInformationType(kDense);
//    gworldIsReference_ = true;
//    referenceHasChanged_ = false;

	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"RasterImage_gray Constructor not implemented");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray constructor");
	throw e;
}


RasterImage_gray::RasterImage_gray(const RasterImage* img)
      try	:	RasterImage(img),
				//
				gray2D_(NULL),
				shiftedGray2D_(NULL)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_gray constructor");

	try {
		img->copyInto(this);
	}
	catch (ErrorReport& e) {
		if (img != NULL) 
		{
			deleteShiftedRaster2D_(img->getTop());
			deleteRaster2D_();
		}
		e.appendToMessage("called by RasterImage_gray constructor");
		throw e;
	}

	setDrawableInformationType(kDense);
    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray constructor");
	throw e;
}


RasterImage_gray::RasterImage_gray(const RasterImage* img, const ImageRect* theRect)
		try	:	RasterImage(img),
				//
				gray2D_(NULL),
				shiftedGray2D_(NULL)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_gray constructor");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in RasterImage_gray constructor");

	try {
		img->copyInto(this, theRect);
	}
	catch (ErrorReport& e) {
		if (theRect != NULL)
		{
			deleteShiftedRaster2D_(theRect->getTop());
			deleteRaster2D_();
		}
		e.appendToMessage("called by RasterImage_gray constructor");
		throw e;
	}

    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray constructor");
	throw e;
}


RasterImage_gray::RasterImage_gray(const RasterImage_gray& img)
        :	RasterImage( ),
			//
			gray2D_(NULL),
			shiftedGray2D_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_gray copy constructor not implemented");
}



RasterImage_gray::~RasterImage_gray(void)
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

BaseRasterType RasterImage_gray::getBaseRasterType(void) const
{
    return kGrayRaster;
}


const unsigned char* RasterImage_gray::getGray(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_gray subclass did not properly override getGray()");

    return gworld_->getRaster();
}


unsigned char* RasterImage_gray::getGray(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_gray subclass did not properly override getGray()");

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return gworld_->getRaster();
}


const unsigned char* const* RasterImage_gray::getGray2D(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_gray subclass did not properly override getGray()");
    return gray2D_;
}


unsigned char*const* RasterImage_gray::getGray2D(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_gray subclass did not properly override getGray()");
    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return gray2D_;
}


const unsigned char* const* RasterImage_gray::getShiftedGray2D(void) const
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_gray subclass did not properly override getGray()");

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return gray2D_;
    else
        return shiftedGray2D_;
}


unsigned char*const* RasterImage_gray::getShiftedGray2D(ReadWriteStatus rw)
{
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldIsNullError,
                    "RasterImage_gray subclass did not properly override getGray()");

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return gray2D_;
    else
        return shiftedGray2D_;
}

/*     Should update the non-reference raster, using the values stored in the reference raster.
 *      In the case of an rgb image, the only raster is the one stored in the GraphWorld.  There
 *      is therefore nothing to do.
 */
void RasterImage_gray::updateRaster_(void) const
{
	//something is not right.  We end up here for an gray_F image!!
    if (hasFloatRaster_ && floatRasterIsReference_)
	{	
		RasterImage_gray_F* tempF = (RasterImage_gray_F*)this;
		tempF->updateRaster_();
	}
	

    referenceHasChanged_ = false;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copy and Write
//------------------------------------------------------
#endif

const RasterImage_gray& RasterImage_gray::operator = (const RasterImage_gray& img) {
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_gray copy operator not implemented");
	return *this;
}
 
void RasterImage_gray::copyInto(RasterImage* imgOut) const
{
    copyInto(imgOut, getValidRect(), NULL);
}


void RasterImage_gray::copyInto(RasterImage* imgOut, const ImageRect* theRect) const
{
    copyInto(imgOut, theRect, NULL);
}


void RasterImage_gray::copyInto(RasterImage* imgOut, const ImageRect* theRect, 
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

void RasterImage_gray::writeInto_(RasterImage* imgOut, const ImageRect* copyRect,
								  const ImagePoint* origin) const
{
    const unsigned char* const* graySource;

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
            gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, false);

            imgOut->gworldIsReference_ = true;
            imgOut->referenceHasChanged_ = true;
            break;

        case kGrayRaster:
            //	if we are dealing with a plain gray raster image, just copy the bytes
            if (!imgOut->hasFloatRaster())
            {
                //  At this point we are ready to perform the copy/conversion and we use
                //  the CopyBits function of QuickDraw/QuickTime
                gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, false);

                imgOut->gworldIsReference_ = true;
                imgOut->referenceHasChanged_ = true;
            }
            //
            //	if imgOut has a float raster, we had better use it
            else
            {
                graySource = getShiftedRaster2D();
                float* const* fGrayDest = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);

				//	This call may have been made from within the RasterImage_gray_F constructor of imgOut,
				//	in which case grayDest has not been allocated yet
				if (fGrayDest == NULL) 
				{
					imgOut->allocateRaster2D_(nbRows, nbCols);
					imgOut->allocateShiftedRaster2D_(nbRows, nbCols, iLowDest, jLowDest);
					fGrayDest = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);
				}

                for (int i=iLowSource, iDest=jLowDest; i<=iHighSource; i++, iDest++)
                    for (int j=jLowSource, jDest=jLowDest; j<=jHighSource; j++, jDest++)
                        fGrayDest[iDest][jDest] = graySource[i][j];

                imgOut->gworldIsReference_ = false;
                imgOut->referenceHasChanged_ = true;
                (static_cast<RasterImage_gray_F*>(imgOut))->setFloatConversionMode(kSaturatedPositive);
            }

            break;

        case kRGB24Raster:
            //  At this point we are ready to perform the copy/conversion and we use
            //  the CopyBits function of QuickDraw/QuickTime
            gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, true);

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
            //	if we are dealing with a plain gray raster image, just copy the bytes
            if (!imgOut->hasFloatRaster())
            {
                //  At this point we are ready to perform the copy/conversion and we use
                //  the CopyBits function of QuickDraw/QuickTime
                gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, true);

                imgOut->gworldIsReference_ = true;
                imgOut->referenceHasChanged_ = true;
            }
            //
            //	if imgOut has a float raster, we had better use it
            else
            {
                graySource = getShiftedRaster2D();

				float*const* fRedOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
				float*const* fGreenOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
				float*const* fBlueOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);

				//	This call may have been made from within the RasterImage_gray_F constructor of imgOut,
				//	in which case grayDest has not been allocated yet
				if ((fRedOut == NULL) || (fGreenOut == NULL)  || (fBlueOut == NULL))
				{
					imgOut->allocateRaster2D_(nbRows, nbCols);
					imgOut->allocateShiftedRaster2D_(nbRows, nbCols, iLowDest, jLowDest);
					fRedOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
					fGreenOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
					fBlueOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);
				}

                for (int i=iLowSource, iDest=jLowDest; i<=iHighSource; i++, iDest++)
                    for (int j=jLowSource, jDest=jLowDest; j<=jHighSource; j++, jDest++)
                    {
                        fRedOut[iDest][jDest] = graySource[i][j];
                        fGreenOut[iDest][jDest] = graySource[i][j];
                        fBlueOut[iDest][jDest] = graySource[i][j];
                    }
                    
                imgOut->gworldIsReference_ = false;
                imgOut->referenceHasChanged_ = true;
                (static_cast<RasterImage_RGBa_F*>(imgOut))->setFloatConversionMode(kSaturatedPositive);
            }
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in RasterImage_gray::copyInto");
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


void RasterImage_gray::convertToBinary(const unsigned char* theSource,
                                       unsigned char* theDestination,
                                       long nbPixels)
{
    const long  nbBytes = (nbPixels+7)/8;
    const int   remainder = static_cast<int>(nbPixels % 8);
    const unsigned char* source = theSource;
    unsigned char* dest = theDestination;
    const unsigned char BIN_0 = static_cast<unsigned char>(0),
                        BIN_HALF = static_cast<unsigned char>(127);
    unsigned char d;
                       
    //  do the conversion for the full bytes
    for (long l=0; l<nbBytes-1; l++)
    {
        d = BIN_0;

        for (int k=7; k>=0; k--)
        {
            if (*(source++) > BIN_HALF)
                d |= (1 << k);
        }
        *(dest++) = d;

    }
    
    //  do the conversion for the remainder bits (incomplete byte)
    d = BIN_0;
    if (remainder > 0)
    {
        for (int k=7; k>7-remainder; k--)
        {
            if (*(source++) > BIN_HALF)
                d |= (1 << k);
        }
        *(dest++) = d;
    }
}



void RasterImage_gray::convertToGray(const unsigned char* theSource,
                                     unsigned char* theDestination,
                                     long nbPixels)
{
	try {
		memcpy(theDestination, theSource, nbPixels);
	}
	catch(...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"memcopy failed in RasterImage_gray::convertToGray uc->uc");
	}
}


void RasterImage_gray::convertToGray(const unsigned char* theSource,
                                     float* theDestination,
                                     long nbPixels)
{
	try {
		for (long l=0; l<nbPixels; l++)
			theDestination[l] = theSource[l];
	}
	catch(...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"memcopy failed in RasterImage_gray::convertToGray uc->f");
	}        
}

void RasterImage_gray::convertToRGBa(const unsigned char* theSource,
                                     unsigned char* theDestination,
                                     long nbPixels)
{
    unsigned char   *dest = theDestination;
    const unsigned char* source = theSource;

	try {
		for (long j=0; j<nbPixels; j++)
		{
			*(dest++) = *source;        //  red
			*(dest++) = *source;        //  green
			*(dest++) = *(source++);    //  blue
			*(dest++) = static_cast<unsigned char>(255);
		}
	}
	catch(...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"memcopy failed in RasterImage_gray::convertToGray uc->rgba");
	}
}



void RasterImage_gray::convertToRGB(const unsigned char* theSource,
                                    unsigned char* theDestination,
                                    long nbPixels)
{
    unsigned char   *dest = theDestination;
    const unsigned char* source = theSource;

	try {
		for (long j=0; j<nbPixels; j++)
		{
			*(dest++) = *source;        //  red
			*(dest++) = *source;        //  green
			*(dest++) = *(source++);    //  blue
		}
	}
	catch(...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"memcopy failed in RasterImage_gray::convertToGray uc->rgba");
	}
}

void RasterImage_gray::convertToARGB16(const unsigned char* theSource,
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
#pragma mark Local gray
//------------------------------------------------------
#endif


const RasterImage_gray* RasterImage_gray::localRasterGray(const RasterImage* imgIn, bool* localGray)
{
    return localRasterGray(imgIn, imgIn->getValidRect(), localGray);
}

const RasterImage_gray* RasterImage_gray::localRasterGray(const RasterImage* imgIn, const ImageRect* rect, 
                                                    bool* localGray)
{
    const RasterImage_gray* grayImg = NULL;

    //	if imgIn is alread a ReasterImage_gray --> then just use its rasters
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        grayImg = static_cast<const RasterImage_gray*>(imgIn);
        *localGray = false;
    }
    //	if imgIn was not a RasterImage_gray, we need to create a local gray
    //	image that is converted from imgIn
    else
    {
        grayImg = new RasterImage_gray(imgIn, rect);
        *localGray = true;
    }

    return grayImg;
}

RasterImage_gray* RasterImage_gray::localRasterGray(RasterImage* imgIn, bool* localGray)
{
    return const_cast<RasterImage_gray*>(localRasterGray(static_cast<const RasterImage* >(imgIn), 
														 imgIn->getValidRect(), localGray));
}

RasterImage_gray* RasterImage_gray::localRasterGray(RasterImage* imgIn, const ImageRect* rect, 
                                                    bool* localGray)
{
    return const_cast<RasterImage_gray*>(localRasterGray(static_cast<const RasterImage* >(imgIn), 
														 rect, localGray));
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocations
//------------------------------------------------------
#endif


void RasterImage_gray::allocateGraphWorld_(int nbRows, int nbCols)
{
    gworld_ = new GraphWorld(8, nbRows, nbCols);
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldAllocationError,
                    "GraphWorld allocation failed in RasterImage_gray constructor");
}

void RasterImage_gray::allocateRaster2D_(int nbRows, int nbCols)
{
    unsigned char   *gray = gworld_->getRaster();
    const int       rowBytes = gworld_->getBytesPerRow();

    if (gray2D_ != NULL)
        delete []gray2D_;

    gray2D_ = new unsigned char*[nbRows];
    FAIL_CONDITION(gray2D_ == NULL, kRasterAllocationFailure,
                   "Could not allocate gray2D_ array");

    gray2D_[0] = gray;
    for (int i=1; i<nbRows; i++)
        gray2D_[i] = gray2D_[i-1] + rowBytes;

}


void RasterImage_gray::deleteRaster2D_(void)
{
    if (gray2D_ != NULL)
    {
        delete []gray2D_;
        gray2D_ = NULL;
    }
}



void RasterImage_gray::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
    {
        const int   iHigh = iLow + nbRows - 1,
                    bytesPerRow = getBytesPerRow();

        //  Here we want to make sure that we don't call the child's class
        //  (I don't really understand why this happens deleteShiftedRaster2D_
        //  is not a virtual function
        RasterImage_gray::deleteShiftedRaster2D_(iLow);
        shiftedGray2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	shiftedGray2D_ == NULL,
                        kRasterAllocationFailure,
                        "2D gray-level  raster allocation failed");

        shiftedGray2D_ -= iLow;
        //
//jyh
//        shiftedGray2D_[iLow] = getGray() - jLow;
        shiftedGray2D_[iLow] = gworld_->getRaster() - jLow;
        FAIL_CONDITION(	shiftedGray2D_ == NULL,
                        kRasterAllocationFailure,
                        "2D gray-level  raster allocation failed");
        for (int i = iLow; i < iHigh; i++)
            shiftedGray2D_[i+1] = shiftedGray2D_[i] + bytesPerRow;

    }
    else
        shiftedGray2D_ = NULL;

}

void RasterImage_gray::deleteShiftedRaster2D_(int iLow)
{
    if (shiftedGray2D_ != NULL)
    {
        shiftedGray2D_ += iLow;
        delete []shiftedGray2D_;
        shiftedGray2D_ = NULL;
    }
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Set Rects
//------------------------------------------------------
#endif

void RasterImage_gray::setBoundRect_(int x, int y, int theWidth, int theHeight)
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
						colShift = x - jLow;

			if (shiftedGray2D_) 
			{
						for (int i=iLow; i<=iHigh; i++)
							shiftedGray2D_[i] += colShift;
						shiftedGray2D_ += rowShift;
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
        if (gworld_ != NULL)
        {
            delete gworld_;
            deleteShiftedRaster2D_(y);
            deleteRaster2D_();

            gworld_ = new GraphWorld(1, theHeight, theWidth);

//	Now done in DrawableObject2D
//            setBoundRect(x, y, theWidth, theHeight);
//            setValidRect(x, y, theWidth, theHeight);

            allocateRaster2D_(theHeight, theWidth);
            allocateShiftedRaster2D_(theHeight, theWidth, y, x);
        }
    }
}



