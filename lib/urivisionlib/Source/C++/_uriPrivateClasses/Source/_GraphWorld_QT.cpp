/*  NAME:
        _GraphWorld_QT.cpp

    DESCRIPTION:
        implementation of the uriVisionLib _GraphWorld_QT class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

//	Mostly there to facilitate life for Visual Studio developers, since in Xcode and
//	Cmake souce files are excluded directly from the project depending on the
//	state defined by the environment variables.
#if URIVL_USE_QUICKTIME

#include "_GraphWorld_QT.h"

/* Implements the _GraphWorld_QT class.
 *
 *	Wrapper class for the GWorld structure (or rather, pointer to a GWorld
 *	structure) of Apple's QuickDraw and QuickTime libraries.
 *
 *  Important: The PixMap stored by the GWorld pads the rows of the raster to
 *  round their length in bytes to a multiple of 32.  Make sure not to confuse
 *  the nominal bounding rectangle of the _GraphWorld_QT with the effective rect.
 */

using namespace uriVL;

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif

_GraphWorld_QT::_GraphWorld_QT(int pixelDepth, const ImageRect* theBoundsRect)
	try	:	    
			gworld_(NULL),
			nbRows_(theBoundsRect->getHeight()),
			pixelDepth_(pixelDepth),
			baseType_(kUndefRasterType),
			pixmap_(NULL),
			device_(NULL),
			currentGWorld_(NULL),
			previousGWorld_(NULL),
			currenGDevice_(NULL),
			previousGDevice_(NULL)
{
    CTabHandle  clut;
    QDErr		myError;
    Rect        myRect = theBoundsRect->rect_;
    
    //	The _GraphWorld_QT's rectangle always has its origin at (0, 0)
    MacOffsetRect(&myRect, static_cast<short>(-myRect.left), static_cast<short>(-myRect.top));

    switch (pixelDepth)
    {
        //  binary raster
        case 1:
            baseType_ = kBinaryRaster;
            clut = (ColorTable **) NULL;
            break;

        //  gray-level raster
        case 8:
            baseType_ = kGrayRaster;
            clut = GetCTable (40);
            break;

        //  16-bit color raster
        case 16:
            baseType_ = kaRGB16Raster;
            clut = (ColorTable **) NULL;
            break;

        //  RGB raster
        case 24:
            baseType_ = kRGB24Raster;
            clut = (ColorTable **) NULL;
            break;

        //  ARGB raster
        case 32:
            baseType_ = kRGBa32Raster;
            clut = (ColorTable **) NULL;
            myRect.bottom++;    //  extra row for ARGB --> RGBA shift
            break;

        default:
            FAIL_CONDITION( true, kGWorldInvalidPixelDepth,
                            "_GraphWorld_QT pixel depth must be 8, 16, 24, or 32");
            break;

    }

    //  Note that the boundRect_ member of an ImageRect object uses the pixel-as-rectangle
    //  convention, which is the one that the GWorld uses too.
    gworld_ = NULL;
    myError = NewGWorld(&gworld_,
                        (short) pixelDepth,
                        &myRect,
                        clut,
                        (GDevice **) NULL,
                        0L);
    FAIL_CONDITION ((myError!= noErr) || (gworld_==NULL), kGWorldAllocationError,
                    "GWorld allocation failed in _GraphWorld_QT constructor");

    pixmap_ = GetGWorldPixMap(gworld_);
    FAIL_CONDITION (pixmap_==NULL, kGWorldNullPixMapError,
                    "GWorld has a NULL PixMap in _GraphWorld_QT constructor");

    //  lock the pixmap
    lock_();
}
catch(...) {
	FAIL_CONDITION( theBoundsRect == NULL,
					kNullParameterError,
					"null ImageRect pointer passed to _GraphWorld_QT constructor.");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid ImageRect pointer passed to _GraphWorld_QT constructor.");
}


_GraphWorld_QT::_GraphWorld_QT(int pixelDepth, int nbRows, int nbCols)
		:	gworld_(NULL),
			nbRows_(nbRows),
			pixelDepth_(pixelDepth),
			baseType_(kUndefRasterType),
			pixmap_(NULL),
			device_(NULL),
			currentGWorld_(NULL),
			previousGWorld_(NULL),
			currenGDevice_(NULL),
			previousGDevice_(NULL)
{
	FAIL_CONDITION( (nbRows <= 0) || (nbRows >= 65536) || (nbCols <= 0) || (nbCols >= 65536),
					kInvalidParameterError,
					"Invalid dimensions passed to _GraphWorld_QT constructor");
					
    CTabHandle  clut;
    QDErr		myError;
    //  Pixel-as-rectangle convention
    Rect    myRect = {0, 0, static_cast<short>(nbRows), static_cast<short>(nbCols)};

    switch (pixelDepth)
    {
        //  binary raster
        case 1:
            baseType_ = kBinaryRaster;
            clut = (ColorTable **) NULL;
            break;

        //  gray-level raster
        case 8:
            baseType_ = kGrayRaster;
            clut = GetCTable (40);
            break;

        //  16-bit color raster
        case 16:
            baseType_ = kaRGB16Raster;
            clut = (ColorTable **) NULL;
            break;

        //  RGB raster
        case 24:
            baseType_ = kRGB24Raster;
            clut = (ColorTable **) NULL;
            break;

        //  ARGB raster
        case 32:
            baseType_ = kRGBa32Raster;
            clut = (ColorTable **) NULL;
            myRect.bottom++;    //  extra row for ARGB --> RGBA shift
            break;

        default:
            FAIL_CONDITION( true, kGWorldInvalidPixelDepth,
                            "_GraphWorld_QT pixel depth must be 8, 16, 24, or 32");
            break;

    }

    gworld_ = NULL;
    myError = NewGWorld(&gworld_,
                        (short) pixelDepth,
                        &myRect,
                        clut,
                        (GDevice **) NULL,
                        0L);
    FAIL_CONDITION ((myError!= noErr) || (gworld_==NULL), 
					kGWorldAllocationError,
                    "GWorld allocation failed in _GraphWorld_QT constructor");

    pixmap_ = GetGWorldPixMap(gworld_);
	if (pixmap_ == NULL) {
		DisposeGWorld(gworld_);
	
		FAIL_CONDITION (true, 
						kGWorldNullPixMapError,
						"GWorld has a NULL PixMap in _GraphWorld_QT constructor");

	}
	
    //  lock the pixmap
    lock_();
}


_GraphWorld_QT::_GraphWorld_QT(const _GraphWorld_QT* theGWorld)
		:	gworld_(NULL),
			nbRows_(0),
			pixelDepth_(0),
			baseType_(kUndefRasterType),
			pixmap_(NULL),
			device_(NULL),
			currentGWorld_(NULL),
			previousGWorld_(NULL),
			currenGDevice_(NULL),
			previousGDevice_(NULL)
{
//    PixMapHandle    thePixmap = GetGWorldPixMap(theGWorld->gworld_);

    FAIL_CONDITION( true,
                    kGWorldInvalidPixelDepth,
                    "_GraphWorld_QT clone constructor is not operative yet");
}

_GraphWorld_QT::_GraphWorld_QT(const _GraphWorld_QT& theGWorld)
		:	gworld_(NULL),
			nbRows_(0),
			pixelDepth_(0),
			baseType_(kUndefRasterType),
			pixmap_(NULL),
			device_(NULL),
			currentGWorld_(NULL),
			previousGWorld_(NULL),
			currenGDevice_(NULL),
			previousGDevice_(NULL)
{
//    PixMapHandle    thePixmap = GetGWorldPixMap(theGWorld->gworld_);

    FAIL_CONDITION( true,
                    kGWorldInvalidPixelDepth,
                    "_GraphWorld_QT clone constructor is not operative yet");
}


_GraphWorld_QT::~_GraphWorld_QT()
{
    unlock_();
    DisposeGWorld(gworld_);
}


const _GraphWorld_QT& _GraphWorld_QT::operator = (const _GraphWorld_QT& theGWorld)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "_GraphWorld_QT = operator is not operative yet");
	return *this;
}

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark access functions
//----------------------------------------------------------
#endif


int _GraphWorld_QT::getBaseRasterType_(void) const
{
    return baseType_;
}


int _GraphWorld_QT::getBytesPerRow_(void) const
{
    return (**pixmap_).rowBytes & 0x3FFF;
}


int _GraphWorld_QT::getEffectiveNbCols_(void) const
{
    if (pixelDepth_ >= 8)
        return getBytesPerRow_()/(pixelDepth_/8);
    else
        return 8*(getBytesPerRow_()/pixelDepth_);

}

int _GraphWorld_QT::getPixelDepth_(void) const
{
    return pixelDepth_;
}

//int _GraphWorld_QT::getBytesPerPixel_(void) const
//{
//    int nbBytes;
//    
//    switch (baseType_)
//    {
//        //  binary raster
//        case kBinaryRaster:
//            nbBytes = 0;
//            break;
//
//        //  gray-level raster
//        case kGrayRaster:
//            nbBytes = 1;
//            break;
//
//        //  16-bit color raster
//        case kaRGB16Raster:
//            nbBytes = 2;
//            break;
//
//        //  RGB raster
//        case kRGB24Raster:
//            nbBytes = 3;
//            break;
//
//        //  ARGB raster
//        case kRGBa32Raster:
//            nbBytes = 4;
//            break;
//
//        default:
//            break;
//
//    }
//    
//    return nbBytes;
//}



int _GraphWorld_QT::getNbRows_(void) const
{
    return nbRows_;
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark copy and raster access
//----------------------------------------------------------
#endif

//  Does a CopyBits to copy one _GraphWorld_QT into another, possibly modifying dimensions
//  and pixel depth on the way
void _GraphWorld_QT::copyInto_(_GraphWorld_QT* destination, const ImageRect* sourceRect, 
                                const ImageRect* destRect, bool inverseGray)
{
    //  First, store the current graphic port and device
    CGrafPtr	origPort;
    GDHandle    origDev;
	GetGWorld(&origPort, &origDev);

    //  Extract the PixMap (handle) of the source and destination GWorlds
	PixMapHandle    sourceMap = GetGWorldPixMap(this->gworld_),
	                destMap = GetGWorldPixMap(destination->gworld_);

    //  Lock the PixMap of the source GWorld
    FAIL_CONDITION( !(LockPixels(sourceMap)),
                    kPixMapLockFailed,
                    "failed to lock source PixMap prior to CopyBits");

    //  Prepare the copy
  	ForeColor(blackColor);
	BackColor(whiteColor);
//	SetGWorld(gworld_, NULL);
//jyh May 08 2008
	SetGWorld(destination->gworld_, NULL);

    if (inverseGray)
        CopyBits((BitMap *) *sourceMap, (BitMap *) *destMap,
    				sourceRect->getRectQT_(), destRect->getRectQT_(), notSrcCopy, NULL);
    else
        CopyBits((BitMap *) *sourceMap, (BitMap *) *destMap,
    				sourceRect->getRectQT_(), destRect->getRectQT_(), srcCopy, NULL);

    //  Unlock the PixMap and restore initial port and device
    UnlockPixels(sourceMap);
    SetGWorld(origPort, origDev);
}


unsigned char* _GraphWorld_QT::getRaster_(void) const
{
    if (baseType_ != kRGBa32Raster)
        return (unsigned char* ) GetPixBaseAddr(pixmap_);
    else
        //  for the time being, the rgba raster has been allocated with an extra
        //  row so that we can shift all addresses by one to get ARGB -> RGBA
        return (unsigned char* ) (GetPixBaseAddr(pixmap_)+1);

}


//  OpenGL must use the effective number of columns (i.e., including the row padding
//  of the GWorld) for its rendering of the raster image.
void _GraphWorld_QT::draw_(void)
{
    RGBColor blackColour = { 0x0000, 0x0000, 0x0000 };
    RGBColor whiteColour = { 0xFFFF, 0xFFFF, 0xFFFF };

    setGraphWorld_();
    RGBBackColor(&whiteColour);
    RGBForeColor(&blackColour);

    switch (baseType_)
    {
        case kBinaryRaster:
            glDrawPixels( getEffectiveNbCols_(), nbRows_,
                          GL_LUMINANCE,
                          GL_UNSIGNED_BYTE,
                          getRaster_());
            break;

        case kGrayRaster:
            glDrawPixels( getEffectiveNbCols_(), nbRows_,
                          GL_LUMINANCE,
                          GL_UNSIGNED_BYTE,
                          getRaster_());
            break;

        case kRGB24Raster:
            glDrawPixels( getEffectiveNbCols_(), nbRows_,
                          GL_RGB,
                          GL_UNSIGNED_BYTE,
                          getRaster_());
            break;

        case kRGBa32Raster:
            glDrawPixels( getEffectiveNbCols_(), nbRows_,
                          GL_RGBA,
                          GL_UNSIGNED_BYTE,
                          getRaster_());
            break;

        default:
            break;

    }

    restoreGraphWorld_();
    RGBBackColor(&whiteColour);
    RGBForeColor(&blackColour);
}

void _GraphWorld_QT::lock_()
{
    bool	lockOK = LockPixels(pixmap_);

    FAIL_CONDITION(!lockOK, kGWorldCannotLockPixels,
                   "Cannot lock _GraphWorld_QT's pixels");
}

void _GraphWorld_QT::unlock_()
{
    bool	unlockOK = LockPixels(pixmap_);

    FAIL_CONDITION(!unlockOK, kGWorldCannotUnlockPixels,
                   "Cannot unlock _GraphWorld_QT's pixels");
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark set source and destination
//----------------------------------------------------------
#endif

void _GraphWorld_QT::setAsMovieGraphWorld_(Movie theMovie)
{
    FAIL_CONDITION( URIVL_OS_MACOS && (baseType_ == kRGB24Raster),
                    kGWorldIncompatiblePixelDepth,
                    "MAC OS does not support 24-bit GWorlds");

    SetMovieGWorld(theMovie, (GWorldPtr) gworld_, NULL);
    OSErr	myErr = GetMoviesError();
    FAIL_CONDITION( myErr != noErr,
                    kMovieToolboxError,
                    "Cannot assign RasterImage's GWorld to the movie");

}

void _GraphWorld_QT::setAsImporterGraphWorld_(GraphicsImportComponent importer)
{
    OSErr myErr = GraphicsImportSetGWorld(importer, (GWorldPtr) gworld_, NULL);

    FAIL_CONDITION( myErr != noErr,
                    kMovieToolboxError,
                    "Cannot assign RasterImage's GWorld to the graphic importer");

}

/*
ErrorCode _GraphWorld_QT::setAsGrabberGraphWorld_(FrameGrabber* grabber)
{
    OSErr err = LockPixels(GetGWorldPixMap(gworld_));

    err = SGSetGWorld(grabber, (GWorldPtr) gworld_, NULL);


			ImageDescriptionHandle imageDesc = (ImageDescriptionHandle)NewHandle(0);
            
            // retrieve a channel’s current sample description, the channel returns a sample description that is
            // appropriate to the type of data being captured
            err = SGGetChannelSampleDescription(videoChannel, (Handle)imageDesc);

    		Rect				   sourceRect = { 0, 0 };
			MatrixRecord		   scaleMatrix;
			RectMatrix(&scaleMatrix, &sourceRect, destRect->getRectQT_());
            
            
			err = DecompressSequenceBegin(decomSeq,	// pointer to field to receive unique ID for sequence
										  imageDesc,			// handle to image description structure
										  gworld_,   // port for the DESTINATION image
										  NULL,					// graphics device handle, if port is set, set to NULL
										  NULL,					// source rectangle defining the portion of the image to decompress 
                                          &scaleMatrix,			// transformation matrix
                                          srcCopy,				// transfer mode specifier
                                          (RgnHandle)NULL,		// clipping region in dest. coordinate system to use as a mask
                                          NULL,					// flags
                                          codecNormalQuality, 	// accuracy in decompression
                                          bestSpeedCodec);		// compressor identifier or special identifiers ie. bestSpeedCodec

    DisposeHandle((Handle)imageDesc);

	return myErr == err = noErr ? kNoError : kVideoInitializingError;	
}



void _GraphWorld_QT::addToBufferRing_(VdigBufferRec *bufferRec)
{
    bufferRec->dest = pixmap_;
}

*/

void _GraphWorld_QT::setGraphWorld_(void)
{
    GetGWorld(&previousGWorld_, &previousGDevice_);
    SetGWorld(gworld_, (GDevice **) NULL);
    currentGWorld_ = gworld_;
    currenGDevice_ = (GDevice **) NULL;
}

void _GraphWorld_QT::setGraphWorld_(const _GraphWorld_QT* theGWorld)
{
    GetGWorld(&previousGWorld_, &previousGDevice_);
    SetGWorld(theGWorld->gworld_, (GDevice **) NULL);
    currentGWorld_ = theGWorld->gworld_;
    currenGDevice_ = (GDevice **) NULL;
}

void _GraphWorld_QT::restoreGraphWorld_(void)
{
    currentGWorld_ = previousGWorld_;
    currenGDevice_ = previousGDevice_;
}

#endif
