/*  NAME:
        RasterImage_HSV.cpp

    DESCRIPTION:
        implementation of the uriVisionLib RasterImage_HSV class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <string>
#include <typeinfo>
#include <cstring>
//
#include "RasterImage_HSV.h"
#include "RasterImage_HSV.h"


#if 0
//----------------------------------------
#pragma mark Local private functions
//----------------------------------------
#endif
namespace uriVL {
	#if URIVL_LUT_RGB_TO_HSV
		bool initializeForwardLUT_(void);
	#endif

	#if URIVL_LUT_HSV_TO_RGB
		bool initializeBackwardLUT_(void);
	#endif
}

using namespace std;
using namespace uriVL;

#if 0
//----------------------------------------
#pragma mark -
#pragma mark Static variables redefined
//----------------------------------------
#endif
#if URIVL_LUT_RGB_TO_HSV
	int* RasterImage_HSV::hueFromRGB_;
	unsigned char* RasterImage_HSV::satFromRGB_;
	unsigned char* RasterImage_HSV::valFromRGB_;
#endif

#if URIVL_LUT_HSV_TO_RGB
	int*** RasterImage_HSV::rgbaFromHSV_;
#endif


// If we use "fast" color conversion mode, then pre-allocate LUT
#if URIVL_LUT_RGB_TO_HSV
	bool initFLUT = initializeForwardLUT_();
#endif
#if URIVL_LUT_HSV_TO_RGB
	bool initFLUT = initializeBackwardLUT_();
#endif


#if 0
//----------------------------------------
#pragma mark -
#pragma mark Constructors and destructor
//----------------------------------------
#endif

RasterImage_HSV::RasterImage_HSV(void)
			:	RasterImage_RGBa( ),
				//
				hue_(NULL),
				sat_(NULL),
				val_(NULL),
				hue2D_(NULL),
				sat2D_(NULL),
				val2D_(NULL),
				shiftedHue2D_(NULL),
				shiftedSat2D_(NULL),
				shiftedVal2D_(NULL),
				//
				hsvIsReference_(false)		
{
	setHasOtherRaster_(true);
//    setHsvAsReference();
    setReferenceHasChanged_(false);
}


RasterImage_HSV::RasterImage_HSV(int nbRows, int nbCols)
		try	:	RasterImage_RGBa(nbRows, nbCols),
				//
				hue_(NULL),
				sat_(NULL),
				val_(NULL),
				hue2D_(NULL),
				sat2D_(NULL),
				val2D_(NULL),
				shiftedHue2D_(NULL),
				shiftedSat2D_(NULL),
				shiftedVal2D_(NULL),
				//
				hsvIsReference_(false)		
{
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
	//
	setHasOtherRaster_(true);
//    setHsvAsReference();
    setReferenceHasChanged_(false);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_HSV constructor");
	throw e;
}


RasterImage_HSV::RasterImage_HSV(const ImageRect* theRect)
		try	:	RasterImage_RGBa(theRect),
				//
				hue_(NULL),
				sat_(NULL),
				val_(NULL),
				hue2D_(NULL),
				sat2D_(NULL),
				val2D_(NULL),
				shiftedHue2D_(NULL),
				shiftedSat2D_(NULL),
				shiftedVal2D_(NULL),
				//
				hsvIsReference_(false)		
{
//	allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(),
							 theRect->getTop(), theRect->getLeft());
	//
	setHasOtherRaster_(true);
//    setHsvAsReference();
    setReferenceHasChanged_(false);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_HSV constructor");
	throw e;
}

RasterImage_HSV::RasterImage_HSV(int nbRows, int nbCols, const unsigned int* hue,
								 const unsigned char* sat, const unsigned char* val)
		try	:	RasterImage_RGBa(nbRows, nbCols),
			//
			hue_(NULL),
			sat_(NULL),
			val_(NULL),
			hue2D_(NULL),
			sat2D_(NULL),
			val2D_(NULL),
			shiftedHue2D_(NULL),
			shiftedSat2D_(NULL),
			shiftedVal2D_(NULL),
			//
			hsvIsReference_(false)		
{
    FAIL_CONDITION(	(hue==NULL) || (sat==NULL) || (val==NULL),
                    kNullRasterError,
                    "NULL array passed to RasterImage_HSV constructor");

    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
	//
    const long	nbPixels = nbRows * nbCols;

    //	There is no padding for the H, S, & V arrays, so global memcpy calls are possible
    memcpy((char* ) hue_, (char* ) hue, static_cast<unsigned long>(sizeof(int)*nbPixels));
    memcpy((char* ) val_, (char* ) val, static_cast<unsigned long>(nbPixels));
    memcpy((char* ) sat_, (char* ) sat, static_cast<unsigned long>(nbPixels));

	setValidRect(0, 0, nbCols, nbRows);
    setHsvAsReference();
	setHasOtherRaster_(true);
    setReferenceHasChanged_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_HSV constructor");
	throw e;
}

RasterImage_HSV::RasterImage_HSV(const RasterImage* img)
		try	:	RasterImage_RGBa(img),
				//
				hue_(NULL),
				sat_(NULL),
				val_(NULL),
				hue2D_(NULL),
				sat2D_(NULL),
				val2D_(NULL),
				shiftedHue2D_(NULL),
				shiftedSat2D_(NULL),
				shiftedVal2D_(NULL),
				//
				hsvIsReference_(false)		
{
    const ImageRect* inRect = img->getValidRect();
    FAIL_CONDITION( inRect == NULL,
                    kNullRectangleError,
                    "Attempt to clone a RasterImage with null valid data rect in RasterImage_HSV constructor");

    allocateRaster2D_(inRect->getHeight(), inRect->getWidth());
    allocateShiftedRaster2D_(inRect->getHeight(), inRect->getWidth(),
							 inRect->getTop(), inRect->getLeft());

    switch( img->getBaseRasterType() )
    {
        case kGrayRaster:
            convertFromGray_((RasterImage_gray* ) img, inRect);
            break;

        case kRGB24Raster:
            convertFromRGB_((RasterImage_RGB* ) img, inRect);
            break;

        case kRGBa32Raster:
            convertFromRGBa_((RasterImage_RGBa* ) img, inRect);

            break;

        default:
            break;

    }

    setHsvAsReference();
	setHasOtherRaster_(true);
    setReferenceHasChanged_(false);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_HSV constructor");
	throw e;
}


RasterImage_HSV::RasterImage_HSV(const RasterImage* img, const ImageRect* theRect)
		try	:	RasterImage_RGBa(img, theRect),
				//
				hue_(NULL),
				sat_(NULL),
				val_(NULL),
				hue2D_(NULL),
				sat2D_(NULL),
				val2D_(NULL),
				shiftedHue2D_(NULL),
				shiftedSat2D_(NULL),
				shiftedVal2D_(NULL),
				//
				hsvIsReference_(false)		
{
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(),
							 theRect->getTop(), theRect->getLeft());

    switch( img->getBaseRasterType() )
    {
        case kGrayRaster:
            convertFromGray_((RasterImage_gray* ) img, theRect);
            break;

        case kRGB24Raster:
            convertFromRGB_((RasterImage_RGB* ) img, theRect);
            break;

        case kRGBa32Raster:
            convertFromRGBa_((RasterImage_RGBa* ) img, theRect);

            break;

        default:
            break;

    }

    setHsvAsReference();
	setHasOtherRaster_(true);
    setReferenceHasChanged_(false);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_HSV constructor");
	throw e;
}



RasterImage_HSV::RasterImage_HSV(const RasterImage_HSV& img)
		try	:	RasterImage_RGBa(),
				//
				hue_(NULL),
				sat_(NULL),
				val_(NULL),
				hue2D_(NULL),
				sat2D_(NULL),
				val2D_(NULL),
				shiftedHue2D_(NULL),
				shiftedSat2D_(NULL),
				shiftedVal2D_(NULL),
				//
				hsvIsReference_(false)		
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_HSV copy constructor not implemented");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_HSV constructor");
	throw e;
}


RasterImage_HSV::~RasterImage_HSV()
{
    deleteHSV_( );
    deleteRaster2D_();
    deleteShiftedRaster2D_(getTop());
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to the HSV rasters
//------------------------------------------------------
#endif

const int* RasterImage_HSV::getHue(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    return hue_;
}

int* RasterImage_HSV::getHue(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  A side-effect of calling the read-write version of a getH-S-V function is
    //  that the base (GraphWorld) raster is set not to be the reference and that the
    //  reference is marked as 'changed'
    setHsvAsReference();

    return hue_;
}

const unsigned char* RasterImage_HSV::getSat(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    return sat_;
}

unsigned char* RasterImage_HSV::getSat(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  A side-effect of calling the read-write version of a getH-S-V function is
    //  that the base (GraphWorld) raster is set not to be the reference and that the
    //  reference is marked as 'changed'
    setHsvAsReference();

    return sat_;
}

const unsigned char* RasterImage_HSV::getVal(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    return val_;
}

unsigned char* RasterImage_HSV::getVal(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  A side-effect of calling the read-write version of a getH-S-V function is
    //  that the base (GraphWorld) raster is set not to be the reference and that the
    //  reference is marked as 'changed'
    setHsvAsReference();

    return val_;
}

const int* const* RasterImage_HSV::getHue2D(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    return hue2D_;
}

int* const* RasterImage_HSV::getHue2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  A side-effect of calling the read-write version of a getH-S-V function is
    //  that the base (GraphWorld) raster is set not to be the reference and that the
    //  reference is marked as 'changed'
    setHsvAsReference();

    return hue2D_;
}

const unsigned char* const* RasterImage_HSV::getSat2D(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    return sat2D_;
}

unsigned char* const* RasterImage_HSV::getSat2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  A side-effect of calling the read-write version of a getH-S-V function is
    //  that the base (GraphWorld) raster is set not to be the reference and that the
    //  reference is marked as 'changed'
    setHsvAsReference();

    return sat2D_;
}

const unsigned char* const* RasterImage_HSV::getVal2D(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    return val2D_;
}

unsigned char* const* RasterImage_HSV::getVal2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  A side-effect of calling the read-write version of a getH-S-V function is
    //  that the base (GraphWorld) raster is set not to be the reference and that the
    //  reference is marked as 'changed'
    setHsvAsReference();

    return val2D_;
}

const int* const* RasterImage_HSV::getShiftedHue2D(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return hue2D_;
    else
        return shiftedHue2D_;

}

int* const* RasterImage_HSV::getShiftedHue2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  A side-effect of calling the read-write version of a getH-S-V function is
    //  that the base (GraphWorld) raster is set not to be the reference and that the
    //  reference is marked as 'changed'
    setHsvAsReference();

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return hue2D_;
    else
        return shiftedHue2D_;

}

const unsigned char* const* RasterImage_HSV::getShiftedSat2D(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return sat2D_;
    else
        return shiftedSat2D_;

}

unsigned char* const* RasterImage_HSV::getShiftedSat2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  A side-effect of calling the read-write version of a getH-S-V function is
    //  that the base (GraphWorld) raster is set not to be the reference and that the
    //  reference is marked as 'changed'
    setHsvAsReference();

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return sat2D_;
    else
        return shiftedSat2D_;

}

const unsigned char* const* RasterImage_HSV::getShiftedVal2D(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return val2D_;
    else
        return shiftedVal2D_;

}

unsigned char* const* RasterImage_HSV::getShiftedVal2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_( );

    //  A side-effect of calling the read-write version of a getH-S-V function is
    //  that the base (GraphWorld) raster is set not to be the reference and that the
    //  reference is marked as 'changed'
    setHsvAsReference();

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return val2D_;
    else
        return shiftedVal2D_;

}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to the rasters of the parent class
//------------------------------------------------------
#endif

const unsigned char* RasterImage_HSV::getRGBa(void) const
{
    if ( (graphWorldIsNull_()) ||
            (!graphWorldIsReference() && referenceHasChanged()) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    return gworld_->getRaster();
}


unsigned char* RasterImage_HSV::getRGBa(ReadWriteStatus rw)
{
    if ( (graphWorldIsNull_()) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return gworld_->getRaster();
}


const unsigned char* const* RasterImage_HSV::getRGBa2D(void) const
{
    if ( (graphWorldIsNull_()) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    return RasterImage_RGBa::getRGBa2D();
}

unsigned char* const* RasterImage_HSV::getRGBa2D(ReadWriteStatus rw)
{
    if ( (graphWorldIsNull_()) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return RasterImage_RGBa::getRGBa2D(R_W_ACCESS);
}

const unsigned char* const* RasterImage_HSV::getShiftedRGBa2D(void) const
{
    if ( (graphWorldIsNull_()) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    return RasterImage_RGBa::getShiftedRGBa2D();
}

unsigned char* const* RasterImage_HSV::getShiftedRGBa2D(ReadWriteStatus rw)
{
    if ( (graphWorldIsNull_()) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return RasterImage_RGBa::getShiftedRGBa2D(R_W_ACCESS);
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copy and Write
//------------------------------------------------------
#endif


const RasterImage_HSV& RasterImage_HSV::operator = (const RasterImage_HSV& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_HSV copy operator not implemented");
	return *this;
}



void RasterImage_HSV::setHsvAsReference(void)
{
    setOtherRasterAsReference_();
    hsvIsReference_ = true;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Conversion
//------------------------------------------------------
#endif


void RasterImage_HSV::convertFromGray_(const RasterImage_gray* imgIn)
{
    convertFromGray_(imgIn, imgIn->getValidRect());
}

void RasterImage_HSV::convertFromGray_(const RasterImage_gray* imgIn, const ImageRect* theRect)
{
    const unsigned char	*const* gray = imgIn->getShiftedGray2D();
    unsigned char* const* sat = getShiftedSat2D(R_W_ACCESS),
                  *const* val = getShiftedVal2D(R_W_ACCESS);
    int           *const* hue = getShiftedHue2D(R_W_ACCESS);

    const int     iLow = theRect->getTop( ),
                  iHigh = theRect->getBottom( ),
                  jLow = theRect->getLeft( ),
                  jHigh = theRect->getRight( );

    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            sat[i][j] = 0;
            val[i][j] = gray[i][j];
            hue[i][j] = 0;
        }
    }
}


void RasterImage_HSV::convertFromRGBa_(const RasterImage_RGBa* imgIn)
{
    convertFromRGBa_(imgIn, imgIn->getValidRect());
}

void RasterImage_HSV::convertFromRGBa_(const RasterImage_RGBa* imgIn, const ImageRect* theRect)
{
    const unsigned char* const* rgba = imgIn->getShiftedRGBa2D();
    unsigned char* const* sat = getShiftedSat2D(R_W_ACCESS),
                  *const* val = getShiftedVal2D(R_W_ACCESS);
    int           *const* hue = getShiftedHue2D(R_W_ACCESS);

    const int   iLow = theRect->getTop( ),
                iHigh = theRect->getBottom( ),
                jLow = theRect->getLeft( ),
                jHigh = theRect->getRight( );

	#if URIVL_LUT_RGB_TO_HSV

		for (int i=iLow; i<=iHigh; i++)
		{
			for (int j=jLow, rgbaIndex=4*jLow; j<=jHigh; j++, rgbaIndex+=4)
			{
				int lutIndex =  (rgba[i][rgbaIndex] << 16) |
								(rgba[i][rgbaIndex+1] << 8) |
								(rgba[i][rgbaIndex+2]);

				hue[i][j] = hueFromRGB_[lutIndex];
				sat[i][j] = satFromRGB_[lutIndex];
				val[i][j] = valFromRGB_[lutIndex];
			}
		}
		
	#else
	
		unsigned char	maxVal, minVal, valRange,
						pRed, pGreen, pBlue;

		for (int i=iLow; i<=iHigh; i++)
		{
			for (int j=jLow, rgbaIndex=4*jLow; j<=jHigh; j++, rgbaIndex++)
			{
				pRed = rgba[i][rgbaIndex++];
				pGreen = rgba[i][rgbaIndex++];
				pBlue = rgba[i][rgbaIndex++];
				maxVal = MAX(MAX(pRed, pGreen), pBlue);
				minVal = MIN(MIN(pRed, pGreen), pBlue);
				valRange = static_cast<unsigned char>(maxVal - minVal);

				/*	HueSaturationV	*/
				val[i][j] = maxVal;

				if (valRange != 0)
				{
					sat[i][j] = static_cast<unsigned char>((valRange*100)/maxVal);

					if ( pRed == maxVal )
						hue[i][j] = (pGreen-pBlue)*60/valRange;
					else if ( pGreen == maxVal )
						hue[i][j] = 120 + (pBlue-pRed)*60/valRange;
					else
						hue[i][j] = 240 + (pRed-pGreen)*60/valRange;

					//	we want the hue_ in the range [0, 360).
					if ( hue[i][j] < 0)
						hue[i][j] += 360;
				}
				else
				{
					sat[i][j] = 0;
					hue[i][j] = 0;
				}
			}
		}
	#endif
	
}

void RasterImage_HSV::convertFromRGB_(const RasterImage_RGB* imgIn)
{
    convertFromRGB_(imgIn, imgIn->getValidRect());
}

void RasterImage_HSV::convertFromRGB_(const RasterImage_RGB* imgIn, const ImageRect* theRect)
{
    const unsigned char* const* rgb = imgIn->getShiftedRGB2D();
    unsigned char* const* sat = getShiftedSat2D(R_W_ACCESS),
                  *const* val = getShiftedVal2D(R_W_ACCESS);
    int           *const* hue = getShiftedHue2D(R_W_ACCESS);

    const int   iLow = theRect->getTop( ),
                iHigh = theRect->getBottom( ),
                jLow = theRect->getLeft( ),
                jHigh = theRect->getRight( );

	#if URIVL_LUT_RGB_TO_HSV

		for (int i=iLow; i<=iHigh; i++)
		{
			for (int j=jLow, rgbIndex=3*jLow; j<=jHigh; j++, rgbIndex+=3)
			{
				int lutIndex =  (rgb[i][rgbIndex] << 16) |
								(rgb[i][rgbIndex+1] << 8) |
								(rgb[i][rgbIndex+2]);

				hue[i][j] = hueFromRGB_[lutIndex];
				sat[i][j] = satFromRGB_[lutIndex];
				val[i][j] = valFromRGB_[lutIndex];
			}
		}

	#else

		unsigned char	maxVal, minVal, valRange,
						pRed, pGreen, pBlue;

		for (int i=iLow; i<=iHigh; i++)
		{
			for (int j=jLow, rgbIndex=3*jLow; j<=jHigh; j++)
			{
				pRed = rgb[i][rgbIndex++];
				pGreen = rgb[i][rgbIndex++];
				pBlue = rgb[i][rgbIndex++];
				maxVal = MAX(MAX(pRed, pGreen), pBlue);
				minVal = MIN(MIN(pRed, pGreen), pBlue);
				valRange = static_cast<unsigned char>(maxVal - minVal);

				/*	HueSaturationV	*/
				val[i][j] = maxVal;

				if (valRange != 0)
				{
					sat[i][j] = static_cast<unsigned char>((valRange*100)/maxVal);

					if ( pRed == maxVal )
						hue[i][j] = (pGreen-pBlue)*60/valRange;
					else if ( pGreen == maxVal )
						hue[i][j] = 120 + (pBlue-pRed)*60/valRange;
					else
						hue[i][j] = 240 + (pRed-pGreen)*60/valRange;

					//	we want the hue_ in the range [0, 360).
					if ( hue[i][j] < 0)
						hue[i][j] += 360;
				}
				else
				{
					sat[i][j] = 0;
					hue[i][j] = 0;
				}
			}
		}
	#endif
}

void RasterImage_HSV::forceConversionToSaturatedRGBa(void)
{
    //  if the GWorld has not been allocated then we must allocate
    //  it at the same dimensions (bounding rectangle) as the float raster
    if (graphWorldIsNull_())
    {
        allocateGraphWorld_(getHeight(), getWidth());
        RasterImage_RGBa::allocateRaster2D_(getHeight(), getWidth());
        RasterImage_RGBa::allocateShiftedRaster2D_(getHeight(), getWidth(), getTop(), getLeft());
    }

    //  And then we convert from HSV to RGBa
    convertToSaturatedRGBa_(getValidRect());
    setGraphWorldAsReference_();
    referenceHasChanged_ = true;
}


void RasterImage_HSV::updateRaster_(void) const 
{
    //  only need to update rasters if the reference (whatever it is) has changed
    if (referenceHasChanged() || (graphWorldIsNull_()))
    {
        //  do this now to avoid infinite loops later
        setReferenceHasChanged_(false);

		//	Not the prettiest solution, but allows to respect logical constness
		RasterImage_HSV* updateableThis = const_cast<RasterImage_HSV*>(this);
		
        //	First find out which raster is the reference to update from
        //
        //  If the graphworld is the reference we need to to an RGBa --> HSV
        //  convertion over the valid data rect
        if (graphWorldIsReference())
        {
            updateableThis->convertFromRGBa_(this, getValidRect());
        }
        //
        //  If the HSV rasters form the reference...
        else
        {
            //  if the GWorld is not the reference and has not been allocated (typically
            //  the first time we render a RasterImage_HSV object) then we must allocate
            //  it at the same dimensions (bounding rectangle) as the float raster
            if (graphWorldIsNull_())
            {
				updateableThis->allocateGraphWorld_(getHeight(), getWidth());

				updateableThis->RasterImage_RGBa::allocateRaster2D_(getHeight(), getWidth());
				updateableThis->RasterImage_RGBa::allocateShiftedRaster2D_(getHeight(), getWidth(), getTop(), getLeft());
            }

            //  And then we convert from HSV to RGBa
            updateableThis->convertToRGBa_(getValidRect());
        }
    }
}


#if 0
#pragma mark -
#endif

void RasterImage_HSV::rgb2HSV(unsigned char r, unsigned char g, unsigned char b,
                              int* hue, unsigned char* sat, unsigned char* val)
{
    unsigned char	maxVal = MAX(MAX(r, g), b),
                    minVal = MIN(MIN(r, g), b);

    /*	HueSaturationV	*/
    *val = maxVal;

    int valRange = maxVal - minVal;

    if (valRange !=0)
    {
        *sat = static_cast<unsigned char>((valRange*100)/maxVal);

        if ( r == maxVal )
            *hue = (g-b)*60/valRange;
        else if ( g == maxVal )
            *hue = 120 + (b-r)*60/valRange;
        else
            *hue = 240 + (r-g)*60/valRange;

        //	we want the hue in the range [0, 360).
        if ( *hue < 0)
            *hue += 360;
    }
    else
    {
        *sat = static_cast<unsigned char>(0);
        *hue = 0;
    }
}

void RasterImage_HSV::rgb2HSV(const unsigned char rgb[],
                              int* hue, unsigned char* sat, unsigned char* val)
{
    unsigned char	maxVal = MAX(MAX(rgb[0], rgb[1]), rgb[2]),
                    minVal = MIN(MIN(rgb[0], rgb[1]), rgb[2]);

    /*	HueSaturationV	*/
    *val = maxVal;

    int valRange = maxVal - minVal;

    if (valRange !=0)
    {
        *sat = static_cast<unsigned char>((valRange*100)/maxVal);

        if ( rgb[0] == maxVal )
            *hue = (rgb[1]-rgb[2])*60/valRange;
        else if ( rgb[1] == maxVal )
            *hue = 120 + (rgb[2]-rgb[0])*60/valRange;
        else
            *hue = 240 + (rgb[0]-rgb[1])*60/valRange;

        //	we want the hue in the range [0, 360).
        if ( *hue < 0)
            *hue += 360;
    }
    else
    {
        *sat = static_cast<unsigned char>(0);
        *hue = 0;
    }
}

void RasterImage_HSV::convertToRGBa_(void)
{
    convertToRGBa_(getValidRect());
}

void RasterImage_HSV::convertToRGBa_(const ImageRect* theRect)
{
    const unsigned char* const* sat = getShiftedSat2D();
    const unsigned char* const* val = getShiftedVal2D();
    const int* const* hue = getShiftedHue2D();
    unsigned char	maxVal, minVal, rangeVal,
                    *const* rgba = getShiftedRGBa2D(R_W_ACCESS);

    const int   iLow = theRect->getTop( ),
                iHigh = theRect->getBottom( ),
                jLow = theRect->getLeft( ),
                jHigh = theRect->getRight( );

    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow, rgbaIndex=4*jLow; j<=jHigh; j++)
        {
            //  get minimum and maximum values converted to range 0-255
            maxVal = val[i][j];
            minVal = static_cast<unsigned char>(maxVal - ((maxVal * sat[i][j]) / 100));
            rangeVal = static_cast<unsigned char>(maxVal - minVal);

            //  determine which region for the HSV cone we are in
            int hueRegion = hue[i][j]/60;
            switch (hueRegion)
            {
                    //  hue in [0, 60] -- red-green, dominant red
                case 0:
                    rgba[i][rgbaIndex++] = maxVal;                              //  red is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(minVal +  //  green is
                                            (hue[i][j] *rangeVal)/ 60);         //      calculated
                    rgba[i][rgbaIndex++] = minVal;                              //  blue is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255);                 //  alpha
                    break;

                    //  hue in [60, 120] -- red-green, dominant green
                case 1:
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(minVal +  //  red is
                                            ((120-hue[i][j]) *rangeVal)/ 60);   //      calculated
                    rgba[i][rgbaIndex++] = maxVal;                              //  green is max
                    rgba[i][rgbaIndex++] = minVal;                              //  blue is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255);                 //  alpha
                    break;

                    //  hue in [120, 180] -- green-blue, dominant green
                case 2:
                    rgba[i][rgbaIndex++] = minVal;                              //  red is min
                    rgba[i][rgbaIndex++] = maxVal;                              //  green is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(minVal +  //  blue is
                                            ((hue[i][j]-120) *rangeVal)/ 60);   //      calculated
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255);                 //  alpha
                    break;

                    //  hue in [180, 240] -- green-blue, dominant blue
                case 3:
                    rgba[i][rgbaIndex++] = minVal;                              //  red is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(minVal +  //  green is
                                            ((240-hue[i][j]) *rangeVal)/ 60);   //      calculated
                    rgba[i][rgbaIndex++] = maxVal;                              //  blue is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255);                 //  alpha
                    break;

                    //  hue in [240, 300] -- blue-red, dominant blue
                case 4:
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(minVal +  //  red is
                                            ((hue[i][j]-240) *rangeVal)/ 60);   //  calculated
                    rgba[i][rgbaIndex++] = minVal;                              //  green is min
                    rgba[i][rgbaIndex++] = maxVal;                              //  blue is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255);                 //  alpha
                    break;

                    //  hue in [300, 360] -- blue-red, dominant red
                case 5:
                    rgba[i][rgbaIndex++] = maxVal;                              //  red is max
                    rgba[i][rgbaIndex++] = minVal;                              //  green is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(minVal +  //  blue is
                                            ((360-hue[i][j]) *rangeVal)/ 60);   //      calculated
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255);                 //  alpha
                    break;

                default:
                    FAIL_CONDITION( true,
                                    kImageProcessingError,
                                    "Invalid hue data in RasterImage_HSV::convertToRGBa_");
                    break;

            }
        }
    }
}

void RasterImage_HSV::convertToSaturatedRGBa_(void)
{
    convertToRGBa_(getValidRect());
}

void RasterImage_HSV::convertToSaturatedRGBa_(const ImageRect* theRect)
{
    //  In this version we simply force the minVal to zero (saturation = 100)
    //  and rescale the max value to 255.
    //  and re-use otherwise the code of the function convertToRGBa_
    //  (we could optimize but this is a low-priority item at this point).
    unsigned char* const* rgba = getShiftedRGBa2D(R_W_ACCESS);
    const int* const* hue = getShiftedHue2D();

    const int   iLow = theRect->getTop( ),
                iHigh = theRect->getBottom( ),
                jLow = theRect->getLeft( ),
                jHigh = theRect->getRight( );

    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow, rgbaIndex=4*jLow; j<=jHigh; j++)
        {
	//  determine which region for the HSV cone we are in
//	#if URIVL_LUT_RGB_TO_HSV
            int hueRegion = hue[i][j]/60;
//	#else
//	#endif
            switch (hueRegion)
            {
                    //  hue in [0, 60] -- red-green, dominant red
                case 0:
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  red is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>       //  green is
                                           ((hue[i][j]*255)/ 60);           //      calculated
                    rgba[i][rgbaIndex++] = 0;                               //  blue is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  alpha
                    break;

                    //  hue in [60, 120] -- red-green, dominant green
                case 1:
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>       //  red is
                                           (((120-hue[i][j])*255)/ 60);     //      calculated
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  green is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(0);   //  blue is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  alpha
                    break;

                    //  hue in [120, 180] -- green-blue, dominant green
                case 2:
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(0);   //  red is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  green is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>       //  blue is
                                           (((hue[i][j]-120)*255)/ 60);     //      calculated
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  alpha
                    break;

                    //  hue in [180, 240] -- green-blue, dominant blue
                case 3:
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(0);   //  red is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>       //  green is
                                           (((240-hue[i][j])*255)/ 60);     //      calculated
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  blue is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  alpha
                    break;

                    //  hue in [240, 300] -- blue-red, dominant blue
                case 4:
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>       //  red is
                                           (((hue[i][j]-240)*255)/ 60);     //      calculated
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(0);   //  green is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  blue is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  alpha
                    break;

                    //  hue in [300, 360] -- blue-red, dominant red
                case 5:
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  red is max
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(0);   //  green is min
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>       //  blue is
                                           (((360-hue[i][j])*255)/ 60);     //      calculated
                    rgba[i][rgbaIndex++] = static_cast<unsigned char>(255); //  alpha
                    break;

                default:
                    FAIL_CONDITION( true,
                                    kImageProcessingError,
                                    "Invalid hue data in RasterImage_HSV::convertToRGBa_");
                    break;

            }
        }
    }
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Local HSV
//------------------------------------------------------
#endif


const RasterImage_HSV* RasterImage_HSV::localRasterHSV(const RasterImage* imgIn, bool *localHSV)
{
    return localRasterHSV(imgIn, imgIn->getValidRect(), localHSV);
}

const RasterImage_HSV* RasterImage_HSV::localRasterHSV(const RasterImage* imgIn, 
                                                 const ImageRect* rect, bool *localHSV)
{
    const RasterImage_HSV* hsvImg = NULL;

    //	we want hue, sat, val to store HSV values corresponding to
    //	whatever is store in imgIn
    //
    //	case 1, imgIn is alread a ReasterImage_HSV --> then just use its rasters
    if (typeid(*imgIn) == typeid(uriVL::RasterImage_HSV))
    {
        hsvImg = static_cast<const RasterImage_HSV*>(imgIn);
        *localHSV = false;
    }
    //	if imgIn was not a RasterImage_HSV, we need to create a local HSV
    //	image that is converted from imgIn
    else
    {
        hsvImg = new RasterImage_HSV(imgIn);
        *localHSV = true;
    }

    return hsvImg;
}

RasterImage_HSV* RasterImage_HSV::localRasterHSV(RasterImage* imgIn, bool* localHSV)
{
    return const_cast<RasterImage_HSV*>(localRasterHSV(static_cast<const RasterImage* >(imgIn), 
										 imgIn->getValidRect(), localHSV));
}


RasterImage_HSV* RasterImage_HSV::localRasterHSV(RasterImage* imgIn, 
												 const ImageRect* rect, bool* localHSV)
{
    return const_cast<RasterImage_HSV*>(localRasterHSV(static_cast<const RasterImage* >(imgIn), 
										 rect, localHSV));
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocations
//------------------------------------------------------
#endif


//------------------------------------------------------
//	Allocation and de-allocation
//------------------------------------------------------

void RasterImage_HSV::allocateRaster2D_(int nbRows, int nbCols)
{
    const long  nbPixels = nbRows*nbCols;

    if (hue_ == NULL)
    {
        hue_ = new int[nbPixels];
        FAIL_CONDITION(	hue_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate hue_ in RasterImage allocateRaster2D_");
        hue2D_ = new int*[nbRows];
        FAIL_CONDITION(	hue2D_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate hue2D_ in RasterImage allocateRaster2D_");
        hue2D_[0] = hue_;
        for (int i=1; i<nbRows; i++)
            hue2D_[i] = hue2D_[i-1] + nbCols;

    }
    if (sat_ == NULL)
    {
        sat_ = new unsigned char[nbPixels];
        FAIL_CONDITION(	sat_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate sat_ in RasterImage allocateRaster2D_");
        sat2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	sat2D_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate sat2D_ in RasterImage allocateRaster2D_");
        sat2D_[0] = sat_;
        for (int i=1; i<nbRows; i++)
            sat2D_[i] = sat2D_[i-1] + nbCols;

    }
    if (val_ == NULL)
    {
        val_ = new unsigned char[nbPixels];
        FAIL_CONDITION(	val_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate value in RasterImage allocateRaster2D_");
        val2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	val2D_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate val2D_ in RasterImage allocateRaster2D_");
        val2D_[0] = val_;
        for (int i=1; i<nbRows; i++)
            val2D_[i] = val2D_[i-1] + nbCols;
    }
}


void RasterImage_HSV::deleteHSV_(void)
{
    if (hue_ != NULL)
    {
        delete hue_;
        hue_ = NULL;
    }
    if (sat_ != NULL)
    {
        delete sat_;
        sat_ = NULL;
    }
    if (val_ != NULL)
    {
        delete val_;
        val_ = NULL;
    }
}

void RasterImage_HSV::deleteRaster2D_(void)
{
    if (hue2D_ != NULL)
    {
        delete hue2D_;
        hue2D_ = NULL;
    }
    if (sat2D_ != NULL)
    {
        delete sat2D_;
        sat2D_ = NULL;
    }
    if (val2D_ != NULL)
    {
        delete val2D_;
        val2D_ = NULL;
    }
}

void RasterImage_HSV::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()!=0) || (getLeft()!=0))
    {
        const int iHigh = iLow + nbRows - 1;

        deleteShiftedRaster2D_(iLow);

        shiftedHue2D_ = new int*[nbRows];
        FAIL_CONDITION(	shiftedHue2D_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate shiftedHue2D_ in RasterImage_HSV::allocateRaster2D_");
        shiftedHue2D_ -= iLow;
        shiftedHue2D_[iLow] = hue_ - jLow;
        for (int i=iLow; i<iHigh; i++)
            shiftedHue2D_[i+1] = shiftedHue2D_[i] + nbCols;

        shiftedSat2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	shiftedSat2D_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate shiftedHue2D_ in RasterImage_HSV::allocateRaster2D_");
        shiftedSat2D_ -= iLow;
        shiftedSat2D_[iLow] = sat_ - jLow;
        for (int i=iLow; i<iHigh; i++)
            shiftedSat2D_[i+1] = shiftedSat2D_[i] + nbCols;

        shiftedVal2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	shiftedVal2D_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate shiftedVal2D_ in RasterImage_HSV::allocateRaster2D_");
        shiftedVal2D_ -= iLow;
        shiftedVal2D_[iLow] = val_ - jLow;
        for (int i=iLow; i<iHigh; i++)
            shiftedVal2D_[i+1] = shiftedVal2D_[i] + nbCols;

    }
    else
    {
        shiftedHue2D_ = NULL;
        shiftedSat2D_ = NULL;
        shiftedVal2D_ = NULL;
    }
}

void RasterImage_HSV::deleteShiftedRaster2D_(int iLow)
{
    if (shiftedHue2D_ != NULL)
    {
        shiftedHue2D_ += iLow;
        delete []shiftedHue2D_;
        shiftedHue2D_ = NULL;
    }
    if (shiftedSat2D_ != NULL)
    {
        shiftedSat2D_ += iLow;
        delete []shiftedSat2D_;
        shiftedSat2D_ = NULL;
    }
    if (shiftedVal2D_ != NULL)
    {
        shiftedVal2D_ += iLow;
        delete []shiftedVal2D_;
        shiftedVal2D_ = NULL;
    }
}


#if URIVL_LUT_RGB_TO_HSV
	bool uriVL::initializeForwardLUT_(void)
	{
		const int SIZE_LUT = 256*256*256;
		
		int* lHueFromRGB = RasterImage_HSV::hueFromRGB_ = new int[SIZE_LUT];
		unsigned char* lSatFromRGB = RasterImage_HSV::satFromRGB_ = new unsigned char[SIZE_LUT];
		unsigned char* lValFromRGB = RasterImage_HSV::valFromRGB_ = new unsigned char[SIZE_LUT];
		
		for (int r=0, rgb=0; r<256; r++) 
			for (int g=0; g<256; g++) 
				for (int b=0; b<256; b++, rgb++) 
					RasterImage_HSV::rgb2HSV(static_cast<unsigned char>(r), 
											 static_cast<unsigned char>(g),
											 static_cast<unsigned char>(b), 
											 lHueFromRGB + rgb, lSatFromRGB + rgb, lValFromRGB + rgb);

		return true;
	}
#endif


#if URIVL_LUT_HSV_TO_RGB
	bool uriVL::initializeBackwardLUT_(void)
	{
		return true;
	}
#endif
