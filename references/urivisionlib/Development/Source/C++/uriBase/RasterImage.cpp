/*  NAME:
        RasterImage.cpp

    DESCRIPTION:
        implementation of the uriVisionLib RasterImage class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "RasterImage.h"
#include "RasterImage_binary.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGBa_F.h"
//#include "RasterImage_aRGB16.h"

using namespace uriVL;

//----------------------------------------------------------
//  static members
//----------------------------------------------------------

unsigned int RasterImage::imgCount_ = 0;
ConversionToGrayMode RasterImage::defaultGrayConv_ = kAverageConversion;
float RasterImage::defaultConvWeight_[3] = {1.f/3.f, 1.f/3.f, 1.f/3.f};


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif


RasterImage::RasterImage(void)
        :   DrawableObject2D(),
			//
			id_(imgCount_++),
			gworld_ (NULL),
			gworldIsReference_(false),
			hasFloatRaster_(false),
			hasOtherRaster_(false),
			floatRasterIsReference_(false),
			referenceHasChanged_(false),
			timeStamp_(0),
			propertiesAreLocked_(false),
			pixmapIsLocked_(false),
			propertiesKey_(NULL),
			pixmapKey_(NULL)
{
}


RasterImage::RasterImage(int nbRows, int nbCols)
        :   DrawableObject2D(nbRows, nbCols),
			//
			id_(imgCount_++),
			gworld_ (NULL),
			gworldIsReference_(false),
			hasFloatRaster_(false),
			hasOtherRaster_(false),
			floatRasterIsReference_(false),
			referenceHasChanged_(false),
			timeStamp_(0),
			propertiesAreLocked_(false),
			pixmapIsLocked_(false),
			propertiesKey_(NULL),
			pixmapKey_(NULL)
{
}


RasterImage::RasterImage(const ImageRect* theRect)
	try	:   DrawableObject2D(*theRect),
			//
			id_(imgCount_++),
			gworld_ (NULL),
			gworldIsReference_(false),
			hasFloatRaster_(false),
			hasOtherRaster_(false),
			floatRasterIsReference_(false),
			referenceHasChanged_(false),
			timeStamp_(0),
			propertiesAreLocked_(false),
			pixmapIsLocked_(false),
			propertiesKey_(NULL),
			pixmapKey_(NULL)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by RasterImage constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid ImageRect parameter error in RasterImage constructor");
}


RasterImage::RasterImage(const ImageRect& theRect)
	try	:   DrawableObject2D(theRect),
			//
			id_(imgCount_++),
			gworld_ (NULL),
			gworldIsReference_(false),
			hasFloatRaster_(false),
			hasOtherRaster_(false),
			floatRasterIsReference_(false),
			referenceHasChanged_(false),
			timeStamp_(0),
			propertiesAreLocked_(false),
			pixmapIsLocked_(false),
			propertiesKey_(NULL),
			pixmapKey_(NULL)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by RasterImage constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid ImageRect parameter error in RasterImage constructor");
}


RasterImage::RasterImage(const GraphWorld* theGworld)
        :   DrawableObject2D(kDense),
			//
			id_(imgCount_++),
			gworld_ (NULL),
			gworldIsReference_(false),
			hasFloatRaster_(false),
			hasOtherRaster_(false),
			floatRasterIsReference_(false),
			referenceHasChanged_(false),
			timeStamp_(0),
			propertiesAreLocked_(false),
			pixmapIsLocked_(false),
			propertiesKey_(NULL),
			pixmapKey_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Constructor is not operative yet");
}



//  This function is potentially dangerous: use with extreme care.
RasterImage::RasterImage(GraphWorld *theGworld, bool keepThisGWorld)
        :   DrawableObject2D(kDense),
			//
			id_(imgCount_++),
			gworld_ (NULL),
			gworldIsReference_(true),
			hasFloatRaster_(false),
			hasOtherRaster_(false),
			floatRasterIsReference_(false),
			referenceHasChanged_(false),
			timeStamp_(0),
			propertiesAreLocked_(false),
			pixmapIsLocked_(false),
			propertiesKey_(NULL),
			pixmapKey_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Constructor is not operative yet");

    FAIL_CONDITION(theGworld == NULL, kGWorldIsNullError,
                   "NULL GraphWorld in RasterImage constructor");

    if (keepThisGWorld)
        gworld_ = theGworld;
    else
        gworld_ = new GraphWorld(theGworld);

	
	try {
		setBoundRect(0, 0, theGworld->getNbRows(), theGworld->getEffectiveNbCols());
	}
	catch (...) {
		FAIL_CONDITION( theGworld == NULL,
						kNullParameterError,
						"Null pointer passed to RasterImage constructor.");
	}

    if (UriVL::validDataRectIsSafe())
    {
        setValidRect(0, 0, 0, 0);
    }
    else
        setValidRect(0, 0, theGworld->getNbRows(), theGworld->getEffectiveNbCols());

}


RasterImage::RasterImage(const RasterImage* img)
    try	:   DrawableObject2D(*img),
			//
			id_(imgCount_++),
			gworld_ (NULL),
			gworldIsReference_(false),
			hasFloatRaster_(false),
			hasOtherRaster_(false),
			floatRasterIsReference_(false),
			referenceHasChanged_(false),
			timeStamp_(0),
			propertiesAreLocked_(false),
			pixmapIsLocked_(false),
			propertiesKey_(NULL),
			pixmapKey_(NULL)
{
    timeStamp_ = 0;
    propertiesAreLocked_ = pixmapIsLocked_ = false;
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by RasterImage constructor");
	throw e;
}
catch (...)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage constructor");

	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter error in RasterImage constructor");
}

RasterImage::RasterImage(const RasterImage& img)
        :   DrawableObject2D(),
			//
			id_(imgCount_++),
			gworld_ (NULL),
			gworldIsReference_(true),
			hasFloatRaster_(false),
			hasOtherRaster_(false),
			floatRasterIsReference_(false),
			referenceHasChanged_(false),
			timeStamp_(0),
			propertiesAreLocked_(false),
			pixmapIsLocked_(false),
			propertiesKey_(NULL),
			pixmapKey_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage copy constructor not implemented.");
}


RasterImage::~RasterImage(void)
{
    if (gworld_ != NULL)
        delete gworld_;
}


const RasterImage& RasterImage::operator = (const RasterImage& img) {
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage copy operator not implemented");
	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to the rasters
//------------------------------------------------------
#endif

const unsigned char* RasterImage::getRaster(void) const
{
    switch (getBaseRasterType())
    {
        case kGrayRaster:
            return ((RasterImage_gray* ) this)->getGray();
            break;

        case kRGB24Raster:
            return ((RasterImage_RGB* ) this)->getRGB();
            break;

        case kRGBa32Raster:
            return ((RasterImage_RGBa* ) this)->getRGBa();
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in RasterImage::getRaster");
            return NULL;
            break;
    }
}

unsigned char* RasterImage::getRaster(ReadWriteStatus rw)
{
    switch (getBaseRasterType())
    {
        case kGrayRaster:
            return ((RasterImage_gray* ) this)->getGray(R_W_ACCESS);
            break;

        case kRGB24Raster:
            return ((RasterImage_RGB* ) this)->getRGB(R_W_ACCESS);
            break;

        case kRGBa32Raster:
            return ((RasterImage_RGBa* ) this)->getRGBa(R_W_ACCESS);
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in RasterImage::getRaster");
            return NULL;
            break;
    }
}


const unsigned char* const* RasterImage::getRaster2D(void) const
{
    const unsigned char	*const* raster = NULL;

    switch (getBaseRasterType())
    {
        case kGrayRaster:
            raster = (static_cast< const RasterImage_gray*>(this))->getGray2D();
            break;

        case kRGB24Raster:
            raster = (static_cast<const RasterImage_RGB*>(this))->getRGB2D();
            break;

        case kRGBa32Raster:
            raster = (static_cast<const RasterImage_RGBa*>(this))->getRGBa2D();
            break;

//        case kaRGB16Raster:
//            raster = (static_cast<const RasterImage_aRGB16*>(this))->getARGB2D();
//            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in RasterImage::getRaster2D");
            break;

    }
    return raster;
}

unsigned char* const* RasterImage::getRaster2D(ReadWriteStatus rw)
{
    unsigned char	*const* raster = NULL;

    switch (getBaseRasterType())
    {
        case kGrayRaster:
            raster = (static_cast<RasterImage_gray*>(this))->getGray2D(R_W_ACCESS);
            break;

        case kRGB24Raster:
            raster = (static_cast<RasterImage_RGB*>(this))->getRGB2D(R_W_ACCESS);
            break;

        case kRGBa32Raster:
            raster = (static_cast<RasterImage_RGBa*>(this))->getRGBa2D(R_W_ACCESS);
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in RasterImage::getRaster2D");
            break;

    }
    return raster;
}


const unsigned char* const* RasterImage::getShiftedRaster2D(void) const
{
    const unsigned char	*const* raster = NULL;

    switch (getBaseRasterType())
    {
        case kGrayRaster:
            raster = (static_cast<const RasterImage_gray*>(this))->getShiftedGray2D();
            break;

        case kRGB24Raster:
            raster = (static_cast<const RasterImage_RGB*>(this))->getShiftedRGB2D();
            break;

        case kRGBa32Raster:
            raster = (static_cast<const RasterImage_RGBa*>(this))->getShiftedRGBa2D();
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in RasterImage::getShiftedRaster2D");
            break;

    }
    return raster;
}

unsigned char* const* RasterImage::getShiftedRaster2D(ReadWriteStatus rw)
{
    unsigned char* const* raster = NULL;

    switch (getBaseRasterType())
    {
        case kGrayRaster:
            raster = (static_cast<RasterImage_gray*>(this))->getShiftedGray2D(R_W_ACCESS);
            break;

        case kRGB24Raster:
            raster = (static_cast<RasterImage_RGB*>(this))->getShiftedRGB2D(R_W_ACCESS);
            break;

        case kRGBa32Raster:
            raster = (static_cast<RasterImage_RGBa*>(this))->getShiftedRGBa2D(R_W_ACCESS);
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in RasterImage::getShiftedRaster2D");
            break;

    }
    return raster;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copy and Write
//------------------------------------------------------
#endif




void RasterImage::copyInto_(RasterImage* imgOut, const ImageRect* theRect, const ImagePoint* origin) const
{
    FAIL_CONDITION(	imgOut == NULL,
                    kNullRasterImageError,
                    "NULL output raster image passed as parameter");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in RasterImage::copyInto_");

    //	We only copy from the valid data rect, so we need to calculate its intersection
    //	with theRect.
	
//	Here I am not sure:  Should we fail or simply do nothing????
    ImageRect* copyRect = ImageRect::intersection(getValidRect(), theRect);
    FAIL_CONDITION( copyRect == NULL,
                    kInvalidRectangleError,
                    "the copy rectangle specified is not within valid data in RasterImage::copyInto_");

    //  the destination rectangle has the same width and height as the copy rectangle,
    //  but with its upper-left corner at the location specified by origin
	//	If the origin has not been set (NULL pointer), then use the copyRect
    ImageRect* destRect;
    if (origin != NULL) {
		int x, y;
		try {
			x = origin->getX();
			y = origin->getY();
		}
		catch (...) {
			FAIL_CONDITION( true,
							kInvalidParameterError,
							"Invalid ImagePoint pointer in RasterImage::copyInto_");
		}

		int w, h;
		try {
			w = copyRect->getWidth();
			h = copyRect->getHeight();
		}
		catch (...) {
			FAIL_CONDITION( true,
							kInvalidParameterError,
							"Invalid ImageRect pointer in RasterImage::copyInto_");
		}
		
        destRect = new ImageRect(x, y, w, h);
    }
	else
        destRect = copyRect;


    bool	needToAllocateNewGWorld = false;
    bool	needToAllocateFloatRaster = false;
	
	try {
		//	If the destination image did not already have a raster allocated, we allocate
		//	one now, and the bound rect and valid rect coincide
		if ((imgOut->gworld_ == NULL) || (imgOut->boundRectIsEmpty()))
		{
			if (imgOut->hasFloatRaster())
				needToAllocateFloatRaster = true;
			else
				needToAllocateNewGWorld = true;

			imgOut->setBoundRect(destRect);
			imgOut->setBoundRect(destRect);
			
			//	since imgOut did not have any raster allocated, we must allocate one
			if (imgOut->graphWorldIsReference())
				needToAllocateNewGWorld = true;
			if (imgOut->hasFloatRaster())
				needToAllocateFloatRaster = true;

		}
		//	if the bounding rectangle of the destination field does not contain copyRect
		//	we need to resize it
		else if (!(imgOut->contains(destRect)))
		{
			imgOut->deleteShiftedRaster2D_(imgOut->getTop());
			imgOut->deleteRaster2D_( );

			//	The bound rect and valid rect coincide.
			imgOut->setBoundRect(destRect);
			imgOut->setBoundRect(destRect);

			//	if imgOut was GraphWorld-based, we must reallocate the GraphWorld
			if (imgOut->graphWorldIsReference())
			{
				delete imgOut->gworld_;
				needToAllocateNewGWorld = true;
			}
			if (imgOut->hasFloatRaster())
				needToAllocateFloatRaster = true;

		}
	}
	catch (ErrorReport& e) {

		delete copyRect;
		if (origin != NULL)
			delete destRect;

		e.appendToMessage("called from RasterImage::copyInto_");
		throw e;
	}
	catch (...) {

		delete copyRect;
		if (origin != NULL)
			delete destRect;

		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid output image pointer in RasterImage::copyInto_");
	}
	
    //	If either the output image's did not have a GraphWorld allocated or we just
    //	deleted it because its dimensions did not match those of the input image, we
    //	allocate a new GraphWorld now.
    if (needToAllocateNewGWorld)
    {
        switch (imgOut->getBaseRasterType())
        {
            case kBinaryRaster:
                imgOut->gworld_ = new GraphWorld(1, destRect);
                break;

            case kGrayRaster:
                imgOut->gworld_ = new GraphWorld(8, destRect);
                break;

            case kRGB24Raster:
                imgOut->gworld_ = new GraphWorld(24, destRect);
                break;

            case kRGBa32Raster:
                imgOut->gworld_ = new GraphWorld(32, destRect);
                break;

            default:
                FAIL_CONDITION( true,
                                kRasterImageError,
                                "Invalid image type in RasterImage::copyInto_");
                break;

        }
    }

    if (needToAllocateNewGWorld || needToAllocateFloatRaster)
    {
        imgOut->allocateRaster2D_(imgOut->getHeight(), imgOut->getWidth());
        imgOut->allocateShiftedRaster2D_(imgOut->getHeight(), imgOut->getWidth(), 
										 imgOut->getTop(), imgOut->getLeft());
    }

	//	the destRect defines the new valid rect of the destination image
	imgOut->setValidRect(destRect);

    delete copyRect;
    if (origin != NULL)
        delete destRect;

}

#if 0
#pragma mark -
#endif

void RasterImage::overlay(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL source raster image passed as parameter in RI::overlay");

    imgIn->writeInto(this, imgIn->getValidRect(), NULL);
}

void RasterImage::overlay(const RasterImage* imgIn, const ImagePoint* origin)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL source raster image passed as parameter in RI::overlay");

    imgIn->writeInto(this, imgIn->getValidRect(), origin);
}

void RasterImage::overlay(const RasterImage* imgIn, const ImageRect* theRect)
{
    imgIn->writeInto(this, theRect, NULL);
}

void RasterImage::overlay(const RasterImage* imgIn, const ImageRect* theRect, 
                          const ImagePoint* origin)
{
    imgIn->writeInto(this, theRect, origin);
}

#if 0
#pragma mark -
#endif

void RasterImage::writeInto(RasterImage* imgOut) const
{
    FAIL_CONDITION(	imgOut == NULL,
                    kNullRasterImageError,
                    "NULL output raster image passed as parameter in RI::writeInto");

    writeInto(imgOut, getValidRect(), NULL);
}

void RasterImage::writeInto(RasterImage* imgOut, const ImageRect* theRect) const
{
    FAIL_CONDITION(	imgOut == NULL,
                    kNullRasterImageError,
                    "NULL output raster image passed as parameter in RI::writeInto");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in RasterImage::writeInto");

    //  the copy rectangle must be entirely contained within this image's valid
    //  data rectangle
    FAIL_CONDITION( !this->contains(theRect),
                    kInvalidRectangleError,
                    "the copy rectangle is not containerd by the source image in RI::writeInto");

    //  We only write in the intersection of theREct and of the destination's valid
    //  data rectangle.
    ImageRect* writeRect = ImageRect::intersection(imgOut->getValidRect(), theRect);

    //  if writeRect is empty, no need to do anything (and no error)
    if (writeRect != NULL)
    {
        writeInto_(imgOut, writeRect, NULL);
        delete writeRect;
    }
}

void RasterImage::writeInto(RasterImage* imgOut, const ImagePoint* origin) const
{
    FAIL_CONDITION(	imgOut == NULL,
                    kNullRasterImageError,
                    "NULL output raster image passed as parameter in RI::writeInto");

    writeInto(imgOut, getValidRect(), origin);
}

void RasterImage::writeInto(RasterImage* imgOut, const ImageRect* theRect, const ImagePoint* origin) const
{
    FAIL_CONDITION(	imgOut == NULL,
                    kNullRasterImageError,
                    "NULL output raster image passed as parameter in RI::writeInto");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in RasterImage::writeInto");

    //  the copy rectangle must be entirely contained within this image's valid
    //  data rectangle
    FAIL_CONDITION( !this->contains(theRect),
                    kInvalidRectangleError,
                    "the copy rectangle is not containerd by the source image in RI::writeInto");

    //  We only write in the intersection of theRect and of the destination's valid
    //  data rectangle.
    ImageRect* destRect = NULL;
	ImageRect* writeRect = NULL;
    ImagePoint* trueOrigin = NULL;

    //  If no origin was specified, then we only have to make sure that we will copy
    //  the data into a rectangle that is within the destination's valid data rectangle
    if (origin == NULL)
        writeRect = ImageRect::intersection(imgOut->getValidRect(), theRect);

    //  If an origin was specified, then we may not have to copy the entire rectangle
    else
    {
        //  first determine the actual destination rectangle, that is, the one that
        //  will be written into
        destRect = new ImageRect(origin->getX(), origin->getY(),
                                 theRect->getWidth(), theRect->getHeight());
        writeRect = ImageRect::intersection(imgOut->getBoundRect(), destRect);

        if (writeRect != NULL)
        {
            //  the upper-left corner of that rectangle is the "true" origin.  It is a point
            //  on the border of the destination image's valid data rectangle
            trueOrigin = new ImagePoint(writeRect->getLeft(), writeRect->getTop());

            //  the rectangle to grab data from in this image has the same dimensions as
            //  writeRect.  Its UL corner is obtained by translating the UL corner of
            //  theRect by (trueOrigin - origin)
            writeRect->translate(theRect->getLeft() - origin->getX(),
                                       theRect->getTop() - origin->getY());
        }
    }

    //  if writeRect is empty, no need to do anything (and no error)
    if (writeRect != NULL)
    {
        writeInto_(imgOut, writeRect, trueOrigin);
        delete writeRect;
    }
    if (destRect != NULL)
        delete destRect;
    if (trueOrigin != NULL)
        delete trueOrigin;

}


#if 0
#pragma mark -
#endif

RasterImage* RasterImage::grab(const ImageRect* rect) const
{
    return grab(this, rect, NULL);
}

RasterImage* RasterImage::grab(const ImageRect* rect, const ImagePoint* origin) const
{
    return grab(this, rect, origin);
}


RasterImage* RasterImage::grab(const RasterImage* imgIn, const ImageRect* theRect)
{
    return grab(imgIn, theRect, NULL);
}

RasterImage* RasterImage::grab(const RasterImage* imgIn, const ImageRect* theRect, const ImagePoint* origin)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL output raster image passed as parameter in RI::grab");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in RasterImage::grab");

    RasterImage* grabImg = NULL;
    if (imgIn->contains(theRect))
    {
        const ImageRect* grabRect;
        if (origin == NULL)
            grabRect = theRect;
        else
            grabRect = new ImageRect(origin->getX(), origin->getY(),
                                     theRect->getWidth(), theRect->getHeight());

        switch (imgIn->getBaseRasterType())
        {
            case kBinaryRaster:
                grabImg = new RasterImage_binary(grabRect);
                break;

            case kGrayRaster:
                grabImg = new RasterImage_gray(grabRect);
                break;

            case kRGB24Raster:
                grabImg = new RasterImage_RGB(grabRect);
                break;

            case kRGBa32Raster:
                grabImg = new RasterImage_RGBa(grabRect);
                break;

            default:
                FAIL_CONDITION( true,
                                kRasterImageError,
                                "Invalid image type in RasterImage::grab");
                break;

        }

        imgIn->copyInto(grabImg, theRect, origin);

        if (origin != NULL)
            delete grabRect;

    }
    return grabImg;
}


RasterImage* RasterImage::createCopy(void) const
{
	RasterImage* imgOut = NULL;
	try 
	{
		imgOut = grab(this, this->getValidRect());
	}
	catch (ErrorReport& e) 
	{
		e.appendToMessage("called by RasterImage::createCopy");
		throw e;		
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kUnknownError,
						"Unexpected error in RasterImage::createCopy");
	}
	
	return imgOut;
}

RasterImage* RasterImage::createCopy(const RasterImage* imgIn)
{
	RasterImage* imgOut = NULL;
	try 
	{
		imgOut = grab(imgIn, imgIn->getValidRect());
	}
	catch (ErrorReport& e) 
	{
		e.appendToMessage("called by RasterImage::createCopy");
		throw e;		
	}
	catch (...)
	{
		FAIL_CONDITION( imgIn == NULL,
						kNullParameterError,
						"Null RasterImage pointer passed to RasterImage::createCopy");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid RasterImage pointer passed to RasterImage::createCopy");
				
	}
	
	return imgOut;
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Locks and stamps
//------------------------------------------------------
#endif


void RasterImage::lockProperties(void *key)
{
    FAIL_CONDITION( propertiesAreLocked_,
                    kInvalidLockAccess,
                    "Attempt to lock properties of an already-locked RasterImage");
    propertiesKey_ = key;
    propertiesAreLocked_ = true;
}

void RasterImage::unlockProperties(void *key)
{
    FAIL_CONDITION( !propertiesAreLocked_,
                    kInvalidLockAccess,
                    "Attempt to unlock pixels on an already unlocked image");
    FAIL_CONDITION( propertiesAreLocked_ && (key != propertiesKey_),
                    kInvalidLockAccess,
                    "Attempt to unlock pixels with incorrect key");
    propertiesAreLocked_ = false;
}


bool RasterImage::propertiesAreLocked(void) const
{
	return propertiesAreLocked_;
}
			

void RasterImage::lockPixels(void *key)
{
    FAIL_CONDITION( pixmapIsLocked_,
                    kInvalidLockAccess,
                    "Attempt to lock pixels of an already-locked RasterImage");
    pixmapKey_ = key;
    pixmapIsLocked_ = true;
}

void RasterImage::unlockPixels(void *key)
{
    FAIL_CONDITION( !pixmapIsLocked_,
                    kInvalidLockAccess,
                    "Attempt to unlock pixels on an already unlocked image");
    FAIL_CONDITION( pixmapIsLocked_ && (key != pixmapKey_),
                    kInvalidLockAccess,
                    "Attempt to unlock pixels with incorrect key");
    pixmapIsLocked_ = false;
}


bool RasterImage::pixelsAreLocked(void) const
{
	return pixmapIsLocked_;
}
			


long RasterImage::getTimeStamp(void) const
{
    return timeStamp_;
}

void RasterImage::setTimeStamp(long timeStamp)
{
    timeStamp_ = timeStamp;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Transformations
//------------------------------------------------------
#endif


void RasterImage::rotate(float angle, const ImagePoint* rotCenter)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


void RasterImage::applyTransformation(const Transformation2D *theTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Raster info getters and setters
//------------------------------------------------------
#endif


unsigned int RasterImage::getID(void) 
{
	return id_;
}

int RasterImage::getBytesPerRow(void) const
{
    if (gworld_ != NULL)
        return gworld_->getBytesPerRow( );
    else
        return 0;
}

int RasterImage::getPixelDepth(void) const
{
    if (gworld_ != NULL)
        return gworld_->getPixelDepth( );
    else
        return 0;
}

long RasterImage::getTotalByteCount(void) const
{
    if (gworld_ != NULL)
        return gworld_->getTotalByteCount( );
    else
        return 0;
}

bool RasterImage::hasFloatRaster(void) const
{
    return hasFloatRaster_;
}

void RasterImage::setHasFloatRaster_(bool hasRaster)
{
    hasFloatRaster_ = hasRaster;
}

void RasterImage::setHasOtherRaster_(bool hasRaster)
{
    hasOtherRaster_ = hasRaster;
}

bool RasterImage::graphWorldIsReference(void) const
{
    return gworldIsReference_;
}

bool RasterImage::referenceHasChanged(void) const
{
    return referenceHasChanged_;
}

void RasterImage::setReferenceHasChanged_(bool hasChanged) const
{
    referenceHasChanged_ = hasChanged;
}


bool RasterImage::floatRasterIsReference(void) const
{
    return (hasFloatRaster_ && floatRasterIsReference_);
}

void RasterImage::requestRasterUpdate(void) const
{
    referenceHasChanged_ = true;
}

void RasterImage::setGraphWorldAsReference_(void) const
{
    //	this should never happen.  I am considering making a FAIL_CONDITION
    if (gworld_ == NULL)
    {
//		if (isRef
//        updateRaster_( );
    }

	if (!gworldIsReference_)
		referenceHasChanged_ = true;
		
	gworldIsReference_ = true;
	floatRasterIsReference_ = false;		
}

void RasterImage::setFloatRasterAsReference_(void) const
{
    FAIL_CONDITION( !hasFloatRaster_,
                    kNullRasterError,
                    "RasterImage object lacks a float raster to set as reference");

	if (!floatRasterIsReference_)
		referenceHasChanged_ = true;

    gworldIsReference_ = false;
    floatRasterIsReference_ = true;
}

void RasterImage::setOtherRasterAsReference_(void) const
{
	referenceHasChanged_ = true;

    gworldIsReference_ = false;
    floatRasterIsReference_ = false;
}

bool RasterImage::graphWorldIsNull_(void) const
{
	return (gworld_ == NULL);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Rendering
//------------------------------------------------------
#endif


//  Does nothing.  all the work was done in DrawableObject2D
void RasterImage::setValidRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( propertiesAreLocked_,
                    kAccessToLockedData,
                    "Attempt to resize a RasterImage with locked properties");
}


void RasterImage::render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
						  float theZ, bool shifted, const ImagePoint* origin) const
{
    //--------------------------------------------------------------
    //	Part 1: make sure that what we draw is up to date
    //--------------------------------------------------------------
    //  Binary images are a special case.  OpenGL cannot draw QD binary
    //  raster, so we need to use the gray raster instead.
    bool    binaryRaster = (getBaseRasterType() == kBinaryRaster);
    if (binaryRaster)
    {
        //  if the gworld is the reference and has been modified, or
        //  the gray raster has been modified, update
        /// the gray raster (re-binarize it).
        if (gworldIsReference_ || referenceHasChanged_)
            updateRaster_( );

    }
    else
        //  For other image types, the base raster is the one to draw, so if it
        //  does not already exist, create it.  If it is not up-to-date, update it.
        if ( (gworld_ == NULL) || (!gworldIsReference_ && referenceHasChanged_) )
            updateRaster_( );


    //--------------------------------------------------------------
    //	Part 2: render the raster
    //--------------------------------------------------------------
    //	I apply a -1 scaling vertical scaling to my image so that it gets displayed
    //	with the right side up.
    glPushMatrix();
    glLoadIdentity();
    
    //subtracting .1f to make work with unix systems
    //otherwise the image starts 'off the screen' and
    //will not render at all
	#if URIVL_OS_UNIX
		glRasterPos3f(scaledLeft, scaledTop - 0.1f, theZ);
    #else
		glRasterPos3f(scaledLeft, scaledTop, theZ);
	#endif
	glPixelZoom(getRenderingScaleX(), -getRenderingScaleY());

    //  for binary images, really render the gray raster
    if (binaryRaster)
    {
        GraphWorld  *grayWorld = ((static_cast<const RasterImage_binary*>(this))->getGrayRasterImage())->gworld_;
        (grayWorld)->draw();
    }
    else
        gworld_->draw();

    glPopMatrix();
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark ImageSource and ImageEditing settings
//------------------------------------------------------
#endif


void RasterImage::setAsCurrentGraphWorld(void)
{
    if ( (gworld_ == NULL) || (!gworldIsReference_ && referenceHasChanged_) )
        updateRaster_( );

    gworld_->setGraphWorld(gworld_);
}

void RasterImage::setAsMovieGraphWorld(Movie theMovie)
{
    if ( (gworld_ == NULL) || (!gworldIsReference_ && referenceHasChanged_) )
        updateRaster_( );

    gworld_->setAsMovieGraphWorld(theMovie);
}

/*ErrorCode RasterImage::setAsGrabberGraphWorld(FrameGrabber *grabber)
{
    if ( (gworld_ == NULL) || (!gworldIsReference_ && referenceHasChanged_) )
        updateRaster_( );

    return gworld_->setAsGrabberGraphWorld(grabber);
}
*/


void RasterImage::setAsImporterGraphWorld(GraphicsImportComponent theImporter)
{
    if ( (gworld_ == NULL) || (!gworldIsReference_ && referenceHasChanged_) )
        updateRaster_( );

    gworld_->setAsImporterGraphWorld(theImporter);
}

void RasterImage::restoreGraphWorld(void)
{
    if ( (gworld_ == NULL) || (!gworldIsReference_ && referenceHasChanged_) )
        updateRaster_( );

    gworld_->restoreGraphWorld( );
}

/*
#if URIVL_USE_QUICKTIME
    void RasterImage::addToBufferRing(VdigBufferRec *bufferRec)
    {
		bufferRec->location.v = boundRect_->getTop();
	 	bufferRec->location.h = boundRect_->getLeft();
	 	bufferRec->reserved = 0;

        gworld_->addToBufferRing(bufferRec);
    }
#endif
*/

//----------------------------------------------------------
//  Conversion to gray preferences
//----------------------------------------------------------

const float* RasterImage::getConversionWeights(void) const
{
	//	if (hasIndividualConversionWeights_)
	//		return indivConvWeight_
	//	else
	return defaultConvWeight_;
}

const float* RasterImage::getDefaultConversionWeights(void)
{
	return defaultConvWeight_;
}


void RasterImage::setDefaultGrayConversionMode(const ConversionToGrayMode theMode)
{
    defaultGrayConv_ = theMode;
}

void RasterImage::setDefaultConversionWeights(const float redW, const float greenW, const float blueW)
{
    //  we can't trust the user to provide normalized weights, so we have to do it here
    float   sumW = redW + greenW + blueW;

    defaultConvWeight_[0] = redW / sumW;
    defaultConvWeight_[1] = greenW / sumW;
    defaultConvWeight_[2] = blueW / sumW;
}


void RasterImage::computeBoundingRect_(ImageRect& rect) const
{
	//	nothing to do
}

void RasterImage::computeValidRect_(ImageRect& rect) const
{
	//	nothing to do
}

void RasterImage::computeCentroid_(ImagePoint_F& cent) const
{
	//	nothing to do
}

void RasterImage::computeCenterOfMass_(ImagePoint_F& cent) const
{
	//	nothing to do
}

