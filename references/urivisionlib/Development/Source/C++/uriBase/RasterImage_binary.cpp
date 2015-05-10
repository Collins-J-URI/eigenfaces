/*  NAME:
        RasterImage_binary.cpp

    DESCRIPTION:
        implementation of the uriVisionLib RasterImage_binary class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "RasterImage_binary.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_RGBa_F.h"

using namespace uriVL;


#if 0
//----------------------------------
#pragma mark Constructors and destructor
//----------------------------------
#endif

RasterImage_binary::RasterImage_binary(void)
        :	RasterImage(),
			//
			grayForm_(NULL),
			unpackedRasterIsReference_(false)
{
	setHasOtherRaster_(true);
}


RasterImage_binary::RasterImage_binary(int nbRows, int nbCols)
        :	RasterImage(nbRows, nbCols),
			//
			grayForm_(NULL),
			unpackedRasterIsReference_(false)
{
	allocateGraphWorld_(nbRows, nbCols);
	allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
	//
    grayForm_ = new RasterImage_gray(nbRows, nbCols);
	setHasOtherRaster_(true);
}


RasterImage_binary::RasterImage_binary(const ImageRect* theRect)
        :	RasterImage(theRect),
			//
			grayForm_(NULL),
			unpackedRasterIsReference_(false)
{
	allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(), theRect->getTop(), theRect->getLeft());
	//
	setHasOtherRaster_(true);
}


RasterImage_binary::RasterImage_binary(int nbRows, int nbCols, const bool *theBw)
        :	RasterImage(nbRows, nbCols),
			//
			grayForm_(NULL),
			unpackedRasterIsReference_(true)
{
	allocateGraphWorld_(nbRows, nbCols);
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
    //
    grayForm_ = new RasterImage_gray(nbRows, nbCols);
    unsigned char* const* gray2D = grayForm_->getGray2D(R_W_ACCESS);
    long    l = 0;
    for (int i=0; i<nbRows; i++)
    {
        for (int j=0; j<nbCols; j++)
        {
            if (theBw[l++])
                gray2D[i][j] = static_cast<unsigned char>(255);
            else
                gray2D[i][j] = static_cast<unsigned char>(0);
        }
    }

	setHasOtherRaster_(true);
    setOtherRasterAsReference_();
}


RasterImage_binary::RasterImage_binary(const GraphWorld* theGworld)
        :	RasterImage(theGworld),
			//
			grayForm_(NULL),
			unpackedRasterIsReference_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");

}


RasterImage_binary::RasterImage_binary(GraphWorld* theGworld, bool keepThisGraphWorld)
        :	RasterImage(theGworld, keepThisGraphWorld),
			//
			grayForm_(NULL),
			unpackedRasterIsReference_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");
}


RasterImage_binary::RasterImage_binary(const RasterImage* img)
        :	RasterImage(img),
			//
			grayForm_(NULL),
			unpackedRasterIsReference_(false)
{
	try {
		img->copyInto(this, img->getValidRect());
	}
	catch(ErrorReport& e) {
		if (img != NULL) 
		{
			deleteShiftedRaster2D_(img->getTop());
			deleteRaster2D_();
		}

		e.appendToMessage("called by RasterImage_binary constructor");
		throw e;
	}
}

RasterImage_binary::RasterImage_binary(const RasterImage* img, const ImageRect* theRect)
	try	:	RasterImage( ),
			//
			grayForm_(NULL),
			unpackedRasterIsReference_(false)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_binary constructor");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in RasterImage_binary constructor");

	try {
		img->copyInto(this, theRect);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by RasterImage_binary constructor");
		throw e;
	}

    gworldIsReference_ = true;
    referenceHasChanged_ = true;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_binary constructor");
	throw e;
}




RasterImage_binary::RasterImage_binary(const RasterImage_binary& img)
        :	RasterImage( ),
			//
			grayForm_(NULL),
			unpackedRasterIsReference_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_binary copy constructor not implemented");
}



const RasterImage_binary& RasterImage_binary::operator = (const RasterImage_binary& thePt)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_binary copy operator not implemented");
	return *this;
}



RasterImage_binary::~RasterImage_binary(void)
{
    if (grayForm_ != NULL)
        delete grayForm_;

}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to the rasters
//------------------------------------------------------
#endif

BaseRasterType RasterImage_binary::getBaseRasterType(void) const
{
    return kBinaryRaster;
}


const unsigned char* RasterImage_binary::getRaster(void) const
{
    return getGrayRasterImage()->getRaster();
}


unsigned char* RasterImage_binary::getRaster(ReadWriteStatus rw)
{
    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the private gray raster is set to be the reference and is marked
    //  as 'changed'
    setOtherRasterAsReference_();
    unpackedRasterIsReference_= true;

    return getGrayRasterImage()->getRaster(rw);
}


const unsigned char* const* RasterImage_binary::getRaster2D(void) const
{
    return getGrayRasterImage()->getRaster2D();
}


unsigned char* const* RasterImage_binary::getRaster2D(ReadWriteStatus rw)
{
    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the private gray raster is set to be the reference and is marked
    //  as 'changed'
    setOtherRasterAsReference_();
    unpackedRasterIsReference_= true;

    return getGrayRasterImage()->getRaster2D(rw);
}


const unsigned char* const* RasterImage_binary::getShiftedRaster2D(void) const
{
    return getGrayRasterImage()->getShiftedRaster2D();
}

unsigned char* const* RasterImage_binary::getShiftedRaster2D(ReadWriteStatus rw)
{
    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the private gray raster is set to be the reference and is marked
    //  as 'changed'
    setOtherRasterAsReference_();
    unpackedRasterIsReference_= true;

    return getGrayRasterImage()->getShiftedRaster2D(rw);
}

const unsigned char* RasterImage_binary::getBw(void) const
{
    return getGrayRasterImage()->getRaster();
}


unsigned char* RasterImage_binary::getBw(ReadWriteStatus rw)
{
    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the private gray raster is set to be the reference and is marked
    //  as 'changed'
    setOtherRasterAsReference_();
    unpackedRasterIsReference_= true;

    return getGrayRasterImage()->getRaster(rw);
}


const unsigned char* const* RasterImage_binary::getBw2D(void) const
{
    return getGrayRasterImage()->getRaster2D();
}


unsigned char* const* RasterImage_binary::getBw2D(ReadWriteStatus rw)
{
    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the private gray raster is set to be the reference and is marked
    //  as 'changed'
    setOtherRasterAsReference_();
    unpackedRasterIsReference_= true;

    return getGrayRasterImage()->getRaster2D(rw);
}


const unsigned char* const* RasterImage_binary::getShiftedBw2D(void) const
{
    return getGrayRasterImage()->getShiftedRaster2D();
}


unsigned char* const* RasterImage_binary::getShiftedBw2D(ReadWriteStatus rw)
{
    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the private gray raster is set to be the reference and is marked
    //  as 'changed'
    setOtherRasterAsReference_();
    unpackedRasterIsReference_= true;

    return getGrayRasterImage()->getShiftedRaster2D(rw);
}


/*  Should update the non-reference raster, using the values stored in the reference
 *  raster.
 */
void RasterImage_binary::updateRaster_(void) const
{
    if (referenceHasChanged_)
    {
        //  Change this flag now to avoid an infinite loop in copyInto
        setReferenceHasChanged_(false);

        if (gworldIsReference_)
        {
            //  If we have never allocated the gray raster, now would be a good time
            //  to do so.
            if (grayForm_ == NULL)
                grayForm_ = new RasterImage_gray(getBoundRect());

            //  Copy the binary raster into the gray one
            copyInto(grayForm_);
        }
        else
        {
            //  To copy the gray raster into the binary one, we first need to
            //  "saturate the gray raster
			const ImageRect* validRect = getValidRect();
            const int   iLow = validRect->getTop(),
                        iHigh = validRect->getBottom(),
                        jLow = validRect->getLeft(),
                        jHigh = validRect->getRight();
            unsigned char   *const* gray = grayForm_->getShiftedRaster2D(R_W_ACCESS);

            for (int i=iLow; i<=iHigh; i++)
                for (int j=jLow; j<=jHigh; j++)
                    if (gray[i][j] != 0)
                        gray[i][j] = static_cast<unsigned char>(255);

            //  then we do the CopyBits
			//	OK, the const_cast is ugly, but this is really the only way to preserve
			//	logical constness
            grayForm_->copyInto(const_cast<RasterImage_binary*>(this));
        }
    }
}

RasterImage_gray* RasterImage_binary::getGrayRasterImage(void) const
{
    //  if the gworld is the reference
    if (gworldIsReference_)
    {
        //  If we have never allocated the gray raster, now would be a good time
        //  to do so.
        if (grayForm_ == NULL)
            grayForm_ = new RasterImage_gray(const_cast<RasterImage_binary*>(this));
        //  otherwise, udpdate the gray raster only if the binary raster has changed
        else if (referenceHasChanged_)
        {
            //  sets the flag to false to avoid loop repeats
            setReferenceHasChanged_(false);

            //  Copy the binary raster into the gray one
			//	I cast away this object's constness because the changes that may occur 
			//	are only of type update.  The "true" content is not changed
            (const_cast<RasterImage_binary*>(this))->copyInto(grayForm_);
        }
    }

    //  the gray raster is now the reference
    setOtherRasterAsReference_();
	unpackedRasterIsReference_ = true;
	
    return  grayForm_;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copy and Write
//------------------------------------------------------
#endif


void RasterImage_binary::copyInto(RasterImage* imgOut) const
{
    copyInto(imgOut, getValidRect(), NULL);
}


void RasterImage_binary::copyInto(RasterImage* imgOut, const ImageRect* theRect) const
{
    copyInto(imgOut, theRect, NULL);
}


void RasterImage_binary::copyInto(RasterImage* imgOut, const ImageRect* theRect, 
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


void RasterImage_binary::writeInto_(RasterImage* imgOut, const ImageRect* copyRect, 
                                    const ImagePoint* origin) const
{
    const unsigned char* const* graySource = NULL;
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

	const int   //iLowSource = sourceRect->getTop(),
				iLowDest = destRect->getTop(),
				//iHighSource = sourceRect->getBottom(),
				//jLowSource = sourceRect->getLeft(),
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

            imgOut->setGraphWorldAsReference_();
            imgOut->setReferenceHasChanged_(true);
            break;

        case kGrayRaster:
            //	if we are dealing with a plain gray raster image, just copy the bytes
            if (!imgOut->hasFloatRaster()) {

                //  At this point we are ready to perform the copy/conversion and we use
                //  the CopyBits function of QuickDraw/QuickTime
                gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, false);

				imgOut->setGraphWorldAsReference_();
                imgOut->setReferenceHasChanged_(true);
            }
            //
            //	if imgOut has a float, use this image's gray raster to do the copy.
            else
            {
                //  temporary
                const int   iLow = copyRect->getTop(),
                            iHigh = copyRect->getBottom(),
                            jLow = copyRect->getLeft(),
                            jHigh = copyRect->getRight();
                //
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

                for (int i=iLow; i<=iHigh; i++)
                    for (int j=jLow; j<=jHigh; j++)
                        fGrayDest[i][j] = graySource[i][j];

                imgOut->setFloatRasterAsReference_();
                imgOut->setReferenceHasChanged_(true);
                (static_cast<RasterImage_gray_F*>(imgOut))->setFloatConversionMode(kSaturatedPositive);
            }
            break;

        case kRGB24Raster:
            //  At this point we are ready to perform the copy/conversion and we use
            //  the CopyBits function of QuickDraw/QuickTime
            gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, true);

            imgOut->setGraphWorldAsReference_();
            imgOut->setReferenceHasChanged_(true);
            break;

        case kaRGB16Raster:

            //  At this point we are ready to perform the copy/conversion and we use
            //  the CopyBits function of QuickDraw/QuickTime
            gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, false);

            imgOut->setGraphWorldAsReference_();
            imgOut->setReferenceHasChanged_(true);
            break;

        case kRGBa32Raster:
            //	if we are dealing with a plain gray raster image, just copy the bytes
            if (!imgOut->hasFloatRaster())
            {
                //  At this point we are ready to perform the copy/conversion and we use
                //  the CopyBits function of QuickDraw/QuickTime
                gworld_->copyInto(imgOut->gworld_, sourceRect, destRect, true);

				imgOut->setGraphWorldAsReference_();
				imgOut->setReferenceHasChanged_(true);
            }
            //
            //	if imgOut has a float raster, we had better use it
            else
            {
                //  temporary
                const int   iLow = copyRect->getTop(),
                            iHigh = copyRect->getBottom(),
                            jLow = copyRect->getLeft(),
                            jHigh = copyRect->getRight();
                //
                graySource = getShiftedRaster2D();
                float* const* fRedDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
                float* const* fGreenDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
                float* const* fBlueDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);
                for (int i=iLow; i<=iHigh; i++)
                    for (int j=jLow; j<=jHigh; j++)
                        fRedDest[i][j] = fGreenDest[i][j] = fBlueDest[i][j] = graySource[i][j];

                imgOut->setFloatRasterAsReference_();
                imgOut->setReferenceHasChanged_(true);
                (static_cast<RasterImage_RGBa_F*>(imgOut))->setFloatConversionMode(kSaturatedPositive);
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

void RasterImage_binary::convertToGray(const unsigned char* theSource,
                                       unsigned char* theDestination,
                                       long nbPixels)
{
    const long  nbBytes = (nbPixels+7)/8;
    const int   remainder = static_cast<int>(nbPixels % 8);
    const unsigned char* source = theSource;
    unsigned char* dest = theDestination;
    const unsigned char BIN_0 = static_cast<unsigned char>(0),
                        BIN_255 = static_cast<unsigned char>(255);
                        
    //  do the conversion for the full bytes
    for (long l=0; l<nbBytes-1; l++)
    {
        unsigned char   c = *(source++);

        for (int k=7; k>=0; k--)
            *(dest++) = (c & (1 << k)) ? BIN_255 : BIN_0;

    }
    
    //  do the conversion for the remainder bits (incomplete byte)
    unsigned char   c = *(source++);
    for (int k=7; k>7-remainder; k--)
        *(dest++) = (c & (1 << k)) ? BIN_255 : BIN_0;

}


void RasterImage_binary::convertToGray(const unsigned char* theSource,
                                       float* theDestination,
                                       long nbPixels)
{
    const long  nbBytes = (nbPixels+7)/8;
    const int   remainder = static_cast<int>(nbPixels % 8);
    const unsigned char* source = theSource;
    float* dest = theDestination;
    const unsigned char BIN_0 = static_cast<unsigned char>(0),
                        BIN_255 = static_cast<unsigned char>(255);
                        
    //  do the conversion for the full bytes
    for (long l=0; l<nbBytes-1; l++)
    {
        unsigned char   c = *(source++);

        for (int k=7; k>=0; k--)
            *(dest++) = (c & (1 << k)) ? BIN_255 : BIN_0;

    }
    
    //  do the conversion for the remainder bits (incomplete byte)
    unsigned char   c = *(source++);
    for (int k=7; k>7-remainder; k--)
        *(dest++) = (c & (1 << k)) ? BIN_255 : BIN_0;

}


void RasterImage_binary::convertToRGBa(const unsigned char* theSource,
                                       unsigned char* theDestination,
                                       long nbPixels)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");

}



void RasterImage_binary::convertToRGB(const unsigned char* theSource,
                                      unsigned char* theDestination,
                                      long nbPixels)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");

}

void RasterImage_binary::convertToARGB16(const unsigned char* theSource,
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
#pragma mark Set Rects
//------------------------------------------------------
#endif


void RasterImage_binary::setBoundRect_(int x, int y, int theWidth, int theHeight)
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
            setBoundRect(x, y, theWidth, theHeight);
            setValidRect(x, y, theWidth, theHeight);
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
            deleteShiftedRaster2D_(getTop());
            deleteRaster2D_();

            allocateGraphWorld_(theHeight, theWidth);

            setBoundRect(x, y, theWidth, theHeight);
            setValidRect(x, y, theWidth, theHeight);
        }
    }

    //  The resizing also affects the gray form raster
    if (grayForm_ != NULL)
        grayForm_->setBoundRect(x, y, theWidth, theHeight);

}

void RasterImage_binary::setValidRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( propertiesAreLocked_,
                    kAccessToLockedData,
                    "Attempt to resize a RasterImage with locked properties");

    //  The root parent class already took care of this image's valid rect.
    //  All what remains to do is set the gray form's valid rect
    if (grayForm_ != NULL)
        grayForm_->setValidRect(x, y, theWidth, theHeight);

}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocations
//------------------------------------------------------
#endif


void RasterImage_binary::allocateGraphWorld_(int nbRows, int nbCols)
{
    gworld_ = new GraphWorld(1, nbRows, nbCols);
    FAIL_CONDITION( gworld_ == NULL,
                    kGWorldAllocationError,
                    "GraphWorld allocation failed in RasterImage_binary constructor");

}


//  A bunch of empty implementations

void RasterImage_binary::allocateRaster2D_(int nbRows, int nbCols)
{
}

void RasterImage_binary::deleteRaster2D_(void)
{
}

void RasterImage_binary::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
}

void RasterImage_binary::deleteShiftedRaster2D_(int iLow)
{
}

