/*  NAME:
        RasterImage_HSV24.cpp

    DESCRIPTION:
        implementation of the uriVisionLib RasterImage_HSV24 class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstring>
//
#include "RasterImage_HSV24.h"

using namespace uriVL;

#error Work in progress: do not use this class yet

#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif


RasterImage_HSV24::RasterImage_HSV24(int theRows, int theCols)
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
    boundRect_ = new ImageRect(0, 0, theCols, theRows);

    gworldIsReference_ = false;
    referenceHasChanged_ = false;

    allocateRaster2D_( );
    allocateShiftedRaster2D_( );
}

RasterImage_HSV24::RasterImage_HSV24(ImageRect* theRect)
        :	RasterImage_RGBa( )
{
    gworld_ = NULL;
    hue_ = NULL;
    sat_ = val_ = NULL;
    hue2D_ = shiftedHue2D_ = NULL;
    sat2D_ = val2D_ = shiftedSat2D_ = shiftedVal2D_ = NULL;
    gworldIsReference_ = false;
    referenceHasChanged_ = false;

    boundRect_ = new ImageRect(theRect);
    FAIL_CONDITION( boundRect_ == NULL,
                    kNullRectangleError,
                    "ImageRect creation failed in RasterImage_HSV24 constructor");

    allocateRaster2D_( );
}

RasterImage_HSV24::RasterImage_HSV24(int theRows, int theCols, unsigned char* hue,
                                     unsigned char* sat, unsigned char* val)
        : RasterImage_RGBa( )
{
    FAIL_CONDITION(	(hue==NULL) || (sat==NULL) || (val==NULL),
                    kNullRasterError,
                    "NULL array passed to RasterImage_HSV24 constructor");

    boundRect_ = new ImageRect(0, 0, theCols, theRows);
    long	nbPixels = theRows * theCols;

    hue_ = NULL;
    sat_ = val_ = NULL;
    hue2D_ = shiftedHue2D_ = NULL;
    sat2D_ = val2D_ = shiftedSat2D_ = shiftedVal2D_ = NULL;
    allocateRaster2D_( );

    //	There is no padding for the H, S, & V arrays, so global memcpy calls are possible
    memcpy((char* ) hue_, (char* ) hue, nbPixels);
    memcpy((char* ) val_, (char* ) val, nbPixels);
    memcpy((char* ) sat_, (char* ) sat, nbPixels);
}

RasterImage_HSV24::RasterImage_HSV24(RasterImage* img)
        : RasterImage_RGBa( )
{
    FAIL_CONDITION( true,
                    kRasterImageError,
                    "RasterImage_HSV24 cloning constructor not implemented yet");

}

RasterImage_HSV24::~RasterImage_HSV24()
{
    deleteHSV_( );
    deleteRaster2D_();
}

#if 0
#pragma mark -
#endif
//------------------------------------------------------
//	Conversion methods
//------------------------------------------------------

void RasterImage_HSV24::convertFromRGBa_()
{
    unsigned char	maxVal, minVal,
    pRed, pGreen, pBlue,
    **rgba = getRGBa2D();

    int           nbRows = getHeight(),
                             nbCols = getWidth();

    if (gworldIsReference_ && referenceHasChanged_)
    {
        for (int i=0; i<nbRows; i++)
        {
            for (int j=0, rgbaIndex=0; j<nbCols; j++, rgbaIndex++)
            {
                pRed = rgba[i][rgbaIndex++];
                pGreen = rgba[i][rgbaIndex++];
                pBlue = rgba[i][rgbaIndex++];
                maxVal = MAX(MAX(pRed, pGreen), pBlue);
                minVal = MIN(MIN(pRed, pGreen), pBlue);

                /*	HueSaturationV	*/
                val2D_[i][j] = (unsigned char) ((100 * maxVal) / 255);

                if (maxVal != minVal)
                {
                    sat2D_[i][j] = ((maxVal-minVal)*100)/maxVal;

                    if ( pRed == maxVal )
                        hue2D_[i][j] = (pGreen-pBlue)*42/(maxVal-minVal);
                    else if ( pGreen == maxVal )
                        hue2D_[i][j] = 83 + (pBlue-pRed)*43/(maxVal-minVal);
                    else
                        hue2D_[i][j] = 128 + (pRed-pGreen)*42/(maxVal-minVal);

                    //	we want the hue_ in the range [0, 255).
                    if ( hue2D_[i][j] < 0)
                        hue2D_[i][j] += 255;
                }
                else
                {
                    sat2D_[i][j] = 0;
                    hue2D_[i][j] = 0;
                }
            }
        }
    }
    referenceHasChanged_ = false;
}

void RasterImage_HSV24::convertFromRGBa_(ImageRect* theRect)
{
    if (theRect == NULL)
        convertFromRGBa_( );

    else
    {
        unsigned char	maxVal, minVal,
        pRed, pGreen, pBlue,
        **rgba = getRGBa2D();

        int           nbRows = getHeight(),
                                 nbCols = getWidth(),
                                          iLow = theRect->getTop( ),
                                                 iHigh = theRect->getBottom( ),
                                                         jLow = theRect->getLeft( ),
                                                                jHigh = theRect->getRight( );

        FAIL_CONDITION( (iLow < 0) || (iHigh > nbRows) || (jLow < 0) || (jHigh > nbCols),
                        kRasterDimensionsDontMatch,
                        "Invalid rectangle dimensions for RGB to HSV conversion");

        if (gworldIsReference_ && referenceHasChanged_)
        {
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow, rgbaIndex=4*jLow; j<=jHigh; j++, rgbaIndex++)
                {
                    pRed = rgba[i][rgbaIndex++];
                    pGreen = rgba[i][rgbaIndex++];
                    pBlue = rgba[i][rgbaIndex++];
                    maxVal = MAX(MAX(pRed, pGreen), pBlue);
                    minVal = MIN(MIN(pRed, pGreen), pBlue);

                    /*	HueSaturationV	*/
                    val2D_[i][j] = (unsigned char) ((100 * maxVal) / 255);

                    if (maxVal != minVal)
                    {
                        sat2D_[i][j] = ((maxVal-minVal)*100)/maxVal;

                        if ( pRed == maxVal )
                            hue2D_[i][j] = (pGreen-pBlue)*42/(maxVal-minVal);
                        else if ( pGreen == maxVal )
                            hue2D_[i][j] = 83 + (pBlue-pRed)*42/(maxVal-minVal);
                        else
                            hue2D_[i][j] = 128 + (pRed-pGreen)*42/(maxVal-minVal);

                        //	we want the hue_ in the range [0, 255).
                        if ( hue2D_[i][j] < 0)
                            hue2D_[i][j] += 255;
                    }
                    else
                    {
                        sat2D_[i][j] = 0;
                        hue2D_[i][j] = 0;
                    }
                }
            }
        }
        referenceHasChanged_ = false;
    }
}

void RasterImage_HSV24::rgb2HSV(unsigned char r, unsigned char g, unsigned char b,
                                unsigned char* hue, unsigned char* sat, unsigned char* val)
{
    unsigned char	maxVal = MAX(MAX(r, g), b),
                           minVal = MIN(MIN(r, g), b);

    /*	HueSaturationV	*/
    *val = (unsigned char) ((100 * maxVal) / 255);

    if (maxVal != minVal)
    {
        *sat = ((maxVal-minVal)*100)/maxVal;

        if ( r == maxVal )
            *hue = (g-b)*42/(maxVal-minVal);
        else if ( g == maxVal )
            *hue = 83 + (b-r)*42/(maxVal-minVal);
        else
            *hue = 128 + (r-g)*42/(maxVal-minVal);

        //	we want the hue in the range [0, 255).
        if ( *hue < 0)
            *hue += 255;
    }
    else
    {
        *sat = 0;
        *hue = 0;
    }
}

void RasterImage_HSV24::rgb2HSV(unsigned char rgb[],
                                unsigned char* hue, unsigned char* sat, unsigned char* val)
{
    unsigned char	maxVal = MAX(MAX(rgb[0], rgb[1]), rgb[2]),
                           minVal = MIN(MIN(rgb[0], rgb[1]), rgb[2]);

    /*	HueSaturationV	*/
    *val = (unsigned char) ((100 * maxVal) / 255);

    if (maxVal != minVal)
    {
        *sat = ((maxVal-minVal)*100)/maxVal;

        if ( rgb[0] == maxVal )
            *hue = (rgb[1]-rgb[2])*42/(maxVal-minVal);
        else if ( rgb[1] == maxVal )
            *hue = 83 + (rgb[2]-rgb[0])*42/(maxVal-minVal);
        else
            *hue = 128 + (rgb[0]-rgb[1])*42/(maxVal-minVal);

        //	we want the hue in the range [0, 255).
        if ( *hue < 0)
            *hue += 255;
    }
    else
    {
        *sat = 0;
        *hue = 0;
    }
}

/*
void RasterImage_HSV24::convertToRGB_(void)
{
}

void RasterImage_HSV24::convertToRGB_(ImageRect* theRect)
{
}
*/
#if 0
#pragma mark -
#endif
//------------------------------------------------------
//	Access to the rasters
//------------------------------------------------------

unsigned char* RasterImage_HSV24::getHue(void)
{
    if (gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    return hue_;
}

unsigned char* RasterImage_HSV24::getSat(void)
{
    if (gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    return sat_;
}

unsigned char* RasterImage_HSV24::getVal(void)
{
    if (gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    return val_;
}

unsigned char** RasterImage_HSV24::getHue2D(void)
{
    if (gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    return hue2D_;
}

unsigned char** RasterImage_HSV24::getSat2D(void)
{
    if (gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    return sat2D_;
}

unsigned char** RasterImage_HSV24::getVal2D(void)
{
    if (gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    return val2D_;
}

unsigned char** RasterImage_HSV24::getShiftedHue2D(void)
{
    if (gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if (boundRect_->upperLeftIsAtOrigin())
        return hue2D_;
    else
        return shiftedHue2D_;

}

unsigned char** RasterImage_HSV24::getShiftedSat2D(void)
{
    if (gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if (boundRect_->upperLeftIsAtOrigin())
        return sat2D_;
    else
        return shiftedSat2D_;

}

unsigned char** RasterImage_HSV24::getShiftedVal2D(void)
{
    if (gworldIsReference_ && referenceHasChanged_)
        updateRaster_( );

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if (boundRect_->upperLeftIsAtOrigin())
        return val2D_;
    else
        return shiftedVal2D_;

}


#if 0
#pragma mark -
#endif
//------------------------------------------------------
//	Allocation and de-allocation
//------------------------------------------------------


void RasterImage_HSV24::allocateRaster2D_(int nbRows)
{
    int   iLow  = boundRect_->getTop(),
                    iHigh = boundRect_->getBottom(),
                            jLow = boundRect_->getLeft(),
                                   jHigh = boundRect_->getRight(),
                                           nbRows = getHeight(),
                                                    nbCols = getWidth();
    long    nbPixels = nbRows*nbCols;

    if (hue_ == NULL)
    {
        hue_ = new unsigned char[nbPixels];
        FAIL_CONDITION(	hue_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate hue_ in RasterImage allocateRaster2D_");
        hue2D_ = new unsigned char*[nbRows];
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

void RasterImage_HSV24::deleteHSV_(void)
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

void RasterImage_HSV24::deleteRaster2D_(void)
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

void RasterImage_HSV24::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
    const int   iHigh = iLow + nbRows - 1,
				jHigh = jLow + nbCols - 1;

    if (hue_ != NULL)
    {
        shiftedHue2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	shiftedHue2D_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate shiftedHue2D_ in RasterImage_HSV24::allocateRaster2D_");
        shiftedHue2D_[0] = hue_ - iLow;
        for (int i=iLow; i<iHigh; i++)
            shiftedHue2D_[i+1] = shiftedHue2D_[i] + nbCols;

    }
    if (sat_ != NULL)
    {
        shiftedSat2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	shiftedSat2D_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate shiftedHue2D_ in RasterImage_HSV24::allocateRaster2D_");
        shiftedSat2D_[0] = sat_ - iLow;
        for (int i=iLow; i<iHigh; i++)
            shiftedSat2D_[i+1] = shiftedSat2D_[i] + nbCols;

    }
    if (val_ != NULL)
    {
        shiftedVal2D_ = new unsigned char*[nbRows];
        FAIL_CONDITION(	shiftedVal2D_==NULL,
                        kRasterAllocationFailure,
                        "Could not allocate shiftedVal2D_ in RasterImage_HSV24::allocateRaster2D_");
        shiftedVal2D_[0] = val_ - iLow;
        for (int i=iLow; i<iHigh; i++)
            shiftedVal2D_[i+1] = shiftedVal2D_[i] + nbCols;

    }
}

void RasterImage_HSV24::deleteShiftedRaster2D_(void)
{
    int   iLow  = boundRect_->getTop(),
                    iHigh = boundRect_->getBottom();
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

