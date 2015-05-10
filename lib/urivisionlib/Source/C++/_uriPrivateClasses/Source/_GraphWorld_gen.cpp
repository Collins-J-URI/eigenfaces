/*  NAME:
        GraphWorld.cpp

    DESCRIPTION:
        implementation of the uriVisionLib GraphWorld class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

//	Mostly there to facilitate life for Visual Studio developers, since in Xcode and
//	Cmake souce files are excluded directly from the project depending on the
//	state defined by the environment variables.
#if !URIVL_USE_QUICKTIME


#include <cstring>
//
#include "RasterImage_binary.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "_GraphWorld_gen.h"


using namespace uriVL;

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif

_GraphWorld_gen::_GraphWorld_gen(int pixelDepth, const ImageRect* boundsRect)
	try	:	baseType_(kUndefRasterType),
			nbRows_(boundsRect->getHeight()),
			nbCols_(boundsRect->getWidth()),
			rowBytes_(0),
			pixelDepth_(pixelDepth),
			raster_(NULL)
{
    long nbBytes;

    raster_ = NULL;
    switch (pixelDepth)
    {
        //  binary raster
        case 1:
            baseType_ = kBinaryRaster;
            nbBytes = (nbCols_+7)/8;
            rowBytes_ = (nbCols_+7)/8;
            nbBytes = (1L*nbRows_)*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        //  gray-level raster
        case 8:
            baseType_ = kGrayRaster;
            rowBytes_ = ((nbCols_ + 3)/4)*4;
            nbBytes = (1L*nbRows_)*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        //  16-bit color raster
        case 16:
            baseType_ = kaRGB16Raster;
            rowBytes_ = ((2*nbCols_ + 3)/4)*4;
            nbBytes = (1L*nbRows_)*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        //  RGB raster
        case 24:
            baseType_ = kRGB24Raster;
            rowBytes_ = ((3*nbCols_ + 3)/4)*4;
            nbBytes = (1L*nbRows_)*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        //  ARGB raster
        case 32:
            baseType_ = kRGBa32Raster;
            rowBytes_ = 4*nbCols_;
            nbBytes = (1L*nbRows_)*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        default:
            FAIL_CONDITION( true, kGWorldInvalidPixelDepth,
                            "_GraphWorld_gen pixel depth must be 1, 8, 16, 24, or 32");
            break;

    }

    FAIL_CONDITION( raster_==NULL, 
                    kGWorldAllocationError,
                    "GWorld allocation failed in _GraphWorld_gen constructor");

    pixelDepth_ = pixelDepth;
}
catch(...) {
	FAIL_CONDITION( boundsRect == NULL,
					kNullParameterError,
					"null ImageRect pointer passed to _GraphWorld_QT constructor.");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid ImageRect pointer passed to _GraphWorld_QT constructor.");
}


_GraphWorld_gen::_GraphWorld_gen(int pixelDepth, int nbRows, int nbCols)
		:	baseType_(kUndefRasterType),
			nbRows_(nbRows),
			nbCols_(nbCols),
			rowBytes_(0),
			pixelDepth_(pixelDepth),
			raster_(NULL)
{
    long nbBytes;

    raster_ = NULL;
    switch (pixelDepth)
    {
        //  binary raster
        case 1:
            baseType_ = kBinaryRaster;
            nbBytes = (nbCols_+7)/8;
            rowBytes_ = (nbCols_+7)/8;
            nbBytes = nbRows_*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        //  gray-level raster
        case 8:
            baseType_ = kGrayRaster;
            rowBytes_ = ((nbCols_ + 3)/4)*4;
            nbBytes = (1L*nbRows_)*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        //  16-bit color raster
        case 16:
            baseType_ = kaRGB16Raster;
            rowBytes_ = ((2*nbCols_ + 3)/4)*4;
            nbBytes = (1L*nbRows_)*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        //  RGB raster
        case 24:
            baseType_ = kRGB24Raster;
            rowBytes_ = ((3*nbCols_ + 3)/4)*4;
            nbBytes = (1L*nbRows_)*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        //  ARGB raster
        case 32:
            baseType_ = kRGBa32Raster;
            rowBytes_ = 4*nbCols_;
            nbBytes = (1L*nbRows_)*rowBytes_;
            raster_ = new unsigned char[nbBytes];
            break;

        default:
            FAIL_CONDITION( true, kGWorldInvalidPixelDepth,
                            "_GraphWorld_gen pixel depth must be 1, 8, 16, 24, or 32");
            break;

    }

    FAIL_CONDITION( raster_==NULL, 
                    kGWorldAllocationError,
                    "GWorld allocation failed in _GraphWorld_gen constructor");

}


_GraphWorld_gen::_GraphWorld_gen(const _GraphWorld_gen* theGWorld)
		:	baseType_(kUndefRasterType),
			nbRows_(0),
			nbCols_(0),
			rowBytes_(0),
			pixelDepth_(0),
			raster_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "_GraphWorld_gen clone constructor is not operative yet");
}

_GraphWorld_gen::_GraphWorld_gen(const _GraphWorld_gen& theGWorld)
		:	baseType_(kUndefRasterType),
			nbRows_(0),
			nbCols_(0),
			rowBytes_(0),
			pixelDepth_(0),
			raster_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "_GraphWorld_gen clone constructor is not operative yet");
}


_GraphWorld_gen::~_GraphWorld_gen()
{
    delete []raster_;
}

const _GraphWorld_gen& _GraphWorld_gen::operator = (const _GraphWorld_gen& theGWorld)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "_GraphWorld_gen = operator is not operative yet");
	return *this;
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark access functions
//----------------------------------------------------------
#endif


int _GraphWorld_gen::getBaseRasterType_(void) const
{
    return baseType_;
}


int _GraphWorld_gen::getBytesPerRow_(void) const
{
    return rowBytes_;
}


int _GraphWorld_gen::getNbRows_(void) const
{
    return nbRows_;
}


int _GraphWorld_gen::getEffectiveNbCols_(void) const
{
    if (baseType_ == kBinaryRaster)
        return rowBytes_*8;
    else
        return rowBytes_ / (pixelDepth_/8);

}

int _GraphWorld_gen::getPixelDepth_(void) const
{
    return pixelDepth_;
}

int _GraphWorld_gen::getBytesPerPixel_(void) const
{
    if (baseType_ == kBinaryRaster)
        return 0;
    else
        return pixelDepth_/8;
}



#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark copy and raster access
//----------------------------------------------------------
#endif


//  Does a CopyBits to copy one _GraphWorld_gen into another, possibly modifying dimensions
//  and pixel depth on the way
void _GraphWorld_gen::copyInto_(_GraphWorld_gen* destination, const ImageRect* sourceRect, 
                               const ImageRect* destRect, bool inverseGray)
{
    //  At this point copyInto does not do any kind of scaling.
    FAIL_CONDITION( (sourceRect->getWidth() != destRect->getWidth()) ||
                    (sourceRect->getHeight() != destRect->getHeight()),
                    kInvalidParameterError,
                    "Generic copyInto does not do any kind of scaling");                    

    switch (baseType_)
    {
        case kBinaryRaster:
            copyBinaryInto_(destination, sourceRect, destRect, inverseGray);
            break;

        case kGrayRaster:
            copyGrayInto_(destination, sourceRect, destRect, inverseGray);
            break;

        case kRGB24Raster:
            copyRGB24Into_(destination, sourceRect, destRect, inverseGray);
            break;

        case kRGBa32Raster:
            copyRGBa32Into_(destination, sourceRect, destRect, inverseGray);
            break;

        default:
            break;

    }
}

//  No need to perform any data validation here.  Everthing was done upstream of this
//  call
void _GraphWorld_gen::copyBinaryInto_(_GraphWorld_gen* destination, const ImageRect* sourceRect, 
                                     const ImageRect* destRect, bool inverseGray)
{
    const int   rowBytesIn = getBytesPerRow_(),
                rowBytesOut = destination->getBytesPerRow_(),
                pixelBytesIn = getBytesPerPixel_(),
                pixelBytesOut = destination->getBytesPerPixel_(),
                nbRows = sourceRect->getHeight(),
                nbCols = sourceRect->getWidth();
                
        
    unsigned char  *imgIn = getRaster_() + rowBytesIn*sourceRect->getTop()
                                         + pixelBytesIn*sourceRect->getLeft(),
                   *imgOut = destination->getRaster_() + rowBytesOut*destRect->getTop()
                                         + pixelBytesOut*destRect->getLeft();

    switch (destination->baseType_)
    {
        case kBinaryRaster: {
                const int nbBytes = (nbCols+7)/8;
                for (int i=0; i<nbRows; i++)
                {
                    memcpy(imgOut, imgIn, nbBytes);
                    imgIn += rowBytesIn;
                    imgOut += rowBytesOut;
                }
            }
            break;

        case kGrayRaster:
            for (int i=0; i<nbRows; i++)
            {
                RasterImage_binary::convertToGray(imgIn, imgOut, nbCols);
                imgIn += rowBytesIn;
                imgOut += rowBytesOut;
            }
            break;

        case kRGB24Raster:
            FAIL_CONDITION( true,
                            kFunctionNotImplemented,
                            "_GraphWorld_gen::copyBinaryInto_ --> RGB is not operative yet");
            break;

        case kRGBa32Raster:
            FAIL_CONDITION( true,
                            kFunctionNotImplemented,
                            "_GraphWorld_gen::copyBinaryInto_ --> RGB is not operative yet");
            break;

        default:
            break;

    }
}


//  No need to perform any data validation here.  Everything was done upstream of this
//  call
void _GraphWorld_gen::copyGrayInto_(_GraphWorld_gen *destination, const ImageRect* sourceRect, 
                                   const ImageRect* destRect, bool inverseGray)
{
    const int   rowBytesIn = getBytesPerRow_(),
                rowBytesOut = destination->getBytesPerRow_();
    const int	pixelBytesIn = getBytesPerPixel_(),
                pixelBytesOut = destination->getBytesPerPixel_(),
                nbRows = sourceRect->getHeight(),
                nbCols = sourceRect->getWidth();
                
        
    unsigned char  *imgIn = getRaster_() + rowBytesIn*sourceRect->getTop()
                                         + pixelBytesIn*sourceRect->getLeft(),
                   *imgOut = destination->getRaster_() + rowBytesOut*destRect->getTop()
                                         + pixelBytesOut*destRect->getLeft();

    switch (destination->baseType_)
    {
        case kBinaryRaster:
            for (int i=0; i<nbRows; i++)
            {
                RasterImage_gray::convertToBinary(imgIn, imgOut, nbCols);
                imgIn += rowBytesIn;
                imgOut += rowBytesOut;
            }
            break;

        case kGrayRaster:
            for (int i=0; i<nbRows; i++)
            {
                memcpy(imgOut, imgIn, nbCols);
                imgIn += rowBytesIn;
                imgOut += rowBytesOut;
            }
            break;

        case kRGB24Raster:
            for (int i=0; i<nbRows; i++)
            {
                RasterImage_gray::convertToRGB(imgIn, imgOut, nbCols);
                imgIn += rowBytesIn;
                imgOut += rowBytesOut;
            }
            break;

        case kRGBa32Raster:
            for (int i=0; i<nbRows; i++)
            {
                RasterImage_gray::convertToRGBa(imgIn, imgOut, nbCols);
                imgIn += rowBytesIn;
                imgOut += rowBytesOut;
            }
            break;

        default:
            break;

    }
}

//  No need to perform any data validation here.  Everything was done upstream of this
//  call
void _GraphWorld_gen::copyRGB24Into_(_GraphWorld_gen* destination, const ImageRect* sourceRect, 
                                    const ImageRect* destRect, bool inverseGray)
{
    const int   rowBytesIn = getBytesPerRow_(),
                rowBytesOut = destination->getBytesPerRow_();
	const int   pixelBytesIn = getBytesPerPixel_(),
                pixelBytesOut = destination->getBytesPerPixel_(),
                nbRows = sourceRect->getHeight(),
                nbCols = sourceRect->getWidth();
                
        
    unsigned char  *imgIn = getRaster_() + rowBytesIn*sourceRect->getTop()
                                         + pixelBytesIn*sourceRect->getLeft(),
                   *imgOut = destination->getRaster_() + rowBytesOut*destRect->getTop()
                                         + pixelBytesOut*destRect->getLeft();

    switch (destination->baseType_)
    {
        case kBinaryRaster:
            FAIL_CONDITION( true,
                            kFunctionNotImplemented,
                            "_GraphWorld_gen::copyInto --> binary is not operative yet");
            break;

        case kGrayRaster:
            for (int i=0; i<nbRows; i++)
            {
                RasterImage_RGB::convertToGray(imgIn, imgOut, nbCols, kAverageConversion);
                imgIn += rowBytesIn;
                imgOut += rowBytesOut;
            }
            break;

        case kRGB24Raster:
            {
                const int nbBytes = 3*nbCols;
                
                for (int i=0; i<nbRows; i++)
                {
                    memcpy(imgOut, imgIn, nbBytes);
                    imgIn += rowBytesIn;
                    imgOut += rowBytesOut;
                }
            }
            break;

        case kRGBa32Raster:
            for (int i=0; i<nbRows; i++)
            {
                RasterImage_RGB::convertToRGBa(imgIn, imgOut, nbCols);
                imgIn += rowBytesIn;
                imgOut += rowBytesOut;
            }
            break;

        default:
            break;

    }
}

//  No need to perform any data validation here.  Everthing was done upstream of this
//  call
void _GraphWorld_gen::copyRGBa32Into_(_GraphWorld_gen* destination, const ImageRect* sourceRect, 
                                     const ImageRect* destRect, bool inverseGray)
{
    const int   rowBytesIn = getBytesPerRow_(),
                rowBytesOut = destination->getBytesPerRow_(),
                pixelBytesIn = getBytesPerPixel_(),
                pixelBytesOut = destination->getBytesPerPixel_(),
                nbRows = sourceRect->getHeight(),
                nbCols = sourceRect->getWidth();
                
        
    unsigned char  *imgIn = getRaster_() + rowBytesIn*sourceRect->getTop()
                                         + pixelBytesIn*sourceRect->getLeft(),
                   *imgOut = destination->getRaster_() + rowBytesOut*destRect->getTop()
                                         + pixelBytesOut*destRect->getLeft();

    switch (destination->baseType_)
    {
        case kBinaryRaster:
            FAIL_CONDITION( true,
                            kFunctionNotImplemented,
                            "_GraphWorld_gen::copyInto --> binary is not operative yet");
            break;

        case kGrayRaster:
            for (int i=0; i<nbRows; i++)
            {
                RasterImage_RGBa::convertToGray(imgIn, imgOut, nbCols, kAverageConversion);
                imgIn += rowBytesIn;
                imgOut += rowBytesOut;
            }
            break;

        case kRGB24Raster:
            for (int i=0; i<nbRows; i++)
            {
                RasterImage_RGBa::convertToRGB(imgIn, imgOut, nbCols);
                imgIn += rowBytesIn;
                imgOut += rowBytesOut;
            }
            break;

        case kRGBa32Raster:
            {
                const int nbBytes = 4*nbCols;
                
                for (int i=0; i<nbRows; i++)
                {
                    memcpy(imgOut, imgIn, nbBytes);
                    imgIn += rowBytesIn;
                    imgOut += rowBytesOut;
                }
            }
            break;

        default:
            break;

    }
}


//  OpenGL must use the effective number of columns (i.e., including the row padding
//  of the GWorld) for its rendering of the raster image.
void _GraphWorld_gen::draw_(void)
{
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
}

unsigned char* _GraphWorld_gen::getRaster_(void)
{
    return raster_;
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark set source and destination
//----------------------------------------------------------
#endif

void _GraphWorld_gen::setAsMovieGraphWorld_(Movie theMovie)
{
}

void _GraphWorld_gen::setAsImporterGraphWorld_(GraphicsImportComponent importer)
{
}

void _GraphWorld_gen::setGraphWorld_(void)
{
}

void _GraphWorld_gen::setGraphWorld_(GDHandle theDevice)
{
}

void _GraphWorld_gen::setGraphWorld_(const _GraphWorld_gen* theGWorld)
{
}

void _GraphWorld_gen::setGraphWorld_(const _GraphWorld_gen* theGWorld, GDHandle theDevice )
{
}


void _GraphWorld_gen::restoreGraphWorld_(void)
{
}

#endif

