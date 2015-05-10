/*  NAME:
        ZeroCrossingDetector.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ZeroCrossingDetector class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ZeroCrossingDetector.h"
#include "RasterImage_gray.h"

using namespace uriVL;
using namespace std;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

ZeroCrossingDetector::ZeroCrossingDetector(void)
{}


ZeroCrossingDetector::~ZeroCrossingDetector(void)
{}



#if 0
#pragma mark -
#endif
//----------------------------------------------------------
//  method implementations
//----------------------------------------------------------


LabelRaster *ZeroCrossingDetector::applyTo(const RasterImage* imgIn, const char* params)
{
    return applyTo_st(imgIn);
}

LabelRaster *ZeroCrossingDetector::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION(	labelOut == NULL,
                    kRasterAllocationFailure,
                    "output LabelRaster allocation failed in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, imgIn->getValidRect(), labelOut);
    return labelOut;
}

LabelRaster *ZeroCrossingDetector::applyTo(const RasterImage* imgIn, const char* params,
                                            const ImageRect* theRect)
{
    return applyTo_st(imgIn, theRect);
}

LabelRaster *ZeroCrossingDetector::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ZeroCrossingDetector::applyTo");

    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, theRect, labelOut);
    return labelOut;
}

#if 0
#pragma mark -
#endif

LabelRaster *ZeroCrossingDetector::applyTo_st(const RasterImage* imgIn)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION(	labelOut == NULL,
                    kRasterAllocationFailure,
                    "output LabelRaster allocation failed in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, imgIn->getValidRect(), labelOut);
    return labelOut;
}

LabelRaster *ZeroCrossingDetector::applyTo_st(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ZeroCrossingDetector::applyTo");

    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, theRect, labelOut);
    return labelOut;
}

#if 0
#pragma mark -
#endif

void ZeroCrossingDetector::applyInto(const RasterImage* imgIn, const char* params,
                                     LabelRaster *labelOut)
{
    applyInto(imgIn, labelOut);
}


void ZeroCrossingDetector::applyInto(const RasterImage* imgIn, const char* params,
                                     const ImageRect* theRect, LabelRaster *labelOut)
{
    applyInto(imgIn, theRect, labelOut);
}



void ZeroCrossingDetector::applyInto(const RasterImage* imgIn, LabelRaster *labelOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( imgIn->getBaseRasterType() != kGrayRaster,
                    kInvalidRasterType,
                    "input raster in ZeroCrossingDetector::applyTo should be gray level");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in ZeroCrossingDetector::applyTo");

    applyInto((RasterImage_gray* ) imgIn, imgIn->getValidRect(), labelOut);
}


void ZeroCrossingDetector::applyInto(const RasterImage* imgIn, const ImageRect* theRect,
                                     LabelRaster *labelOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( imgIn->getBaseRasterType() != kGrayRaster,
                    kInvalidRasterType,
                    "input raster in ZeroCrossingDetector::applyTo should be gray level");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, theRect, labelOut);
}


void ZeroCrossingDetector::applyInto(const RasterImage* imgIn, const char* params,
                                     RasterImage* imgOut)
{
    applyInto(imgIn, imgOut);
}


void ZeroCrossingDetector::applyInto(const RasterImage* imgIn, const char* params,
                                     const ImageRect* theRect, RasterImage* imgOut)
{
    applyInto(imgIn, theRect, imgOut);
}

void ZeroCrossingDetector::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( imgIn->getBaseRasterType() != kGrayRaster,
                    kInvalidRasterType,
                    "input raster in ZeroCrossingDetector::applyTo should be gray level");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output RasterImage as parameter in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, imgIn->getValidRect(), imgOut);
}

void ZeroCrossingDetector::applyInto(const RasterImage* imgIn, const ImageRect* theRect,
                                     RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( imgIn->getBaseRasterType() != kGrayRaster,
                    kInvalidRasterType,
                    "input raster in ZeroCrossingDetector::applyTo should be gray level");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null RasterImage as destination in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, theRect, imgOut);
}


#if 0
#pragma mark -
#endif

void ZeroCrossingDetector::applyInto_st(const RasterImage* imgIn, LabelRaster *labelOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( imgIn->getBaseRasterType() != kGrayRaster,
                    kInvalidRasterType,
                    "input raster in ZeroCrossingDetector::applyTo should be gray level");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in ZeroCrossingDetector::applyTo");

    applyInto_st((RasterImage_gray* ) imgIn, imgIn->getValidRect(), labelOut);
}


void ZeroCrossingDetector::applyInto_st(const RasterImage* imgIn, const ImageRect* theRect,
                                     LabelRaster *labelOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( imgIn->getBaseRasterType() != kGrayRaster,
                    kInvalidRasterType,
                    "input raster in ZeroCrossingDetector::applyTo should be gray level");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, theRect, labelOut);
}


void ZeroCrossingDetector::applyInto_st(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( imgIn->getBaseRasterType() != kGrayRaster,
                    kInvalidRasterType,
                    "input raster in ZeroCrossingDetector::applyTo should be gray level");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output RasterImage as parameter in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, imgIn->getValidRect(), imgOut);
}

void ZeroCrossingDetector::applyInto_st(const RasterImage* imgIn, const ImageRect* theRect,
                                     RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( imgIn->getBaseRasterType() != kGrayRaster,
                    kInvalidRasterType,
                    "input raster in ZeroCrossingDetector::applyTo should be gray level");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null RasterImage as destination in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ZeroCrossingDetector::applyTo");

    applyInto_((RasterImage_gray* ) imgIn, theRect, imgOut);
}


#if 0
#pragma mark -
#endif


/*------------------------------------------------------------------------------+
|	detects zero crossings in a raster image									|
+------------------------------------------------------------------------------*/

void ZeroCrossingDetector::applyInto_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                      LabelRaster *labelOut)
{
    ImageRect* usableRect = new ImageRect(imgIn->getValidRect());
    usableRect->removeAllAround(1);
    ImageRect* destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION(	destRect == NULL,
                    kInvalidRectangleError,
                    "input image valid data rect and output rectangles must overlap");

//     unsigned char 	**gray = imgIn->getShiftedGray2D(); // unused var. -CJC
    char 	**label = labelOut->getShiftedLabel2D(R_W_ACCESS);

    const int   iLow = destRect->getTop( ),
                iHigh = destRect->getBottom( ),
                jLow = destRect->getLeft( ),
                jHigh = destRect->getRight( );
    int im1, ip1, jm1, jp1;

    if (imgIn->hasFloatRaster() && imgIn->floatRasterIsReference())
    {
        const float* const* frIn = (static_cast<const RasterImage_gray_F*>(imgIn))->getShiftedGrayF2D();

        im1 = iLow-1;
        ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; )
        {
            jm1 = jLow-1;
            jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; )
            {
                if ( (frIn[im1][jm1]*frIn[ip1][jp1] < 0.f) ||
                        (frIn[im1][jp1]*frIn[ip1][jm1] < 0.f) ||
                        (frIn[i][jm1]*frIn[i][jp1] < 0.f)     ||
                        (frIn[im1][j]*frIn[ip1][j] < 0.f) )
                    label[i][j] = (char) 1;
                else
                    label[i][j] = (char) 0;

                jm1 = j;
                j = jp1;
                jp1++;
            }

            im1 = i;
            i=ip1;
            ip1++;
        }
    }
    else
    {
        const unsigned char* const* irIn = imgIn->getShiftedRaster2D();

        im1 = iLow-1;
        ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; )
        {
            jm1 = jLow-1;
            jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; )
            {
                if ( (irIn[im1][jm1]*irIn[ip1][jp1] < 0) ||
                        (irIn[im1][jp1]*irIn[ip1][jm1] < 0) ||
                        (irIn[i][jm1]*irIn[i][jp1] < 0)     ||
                        (irIn[im1][j]*irIn[ip1][j] < 0) )
                    label[i][j] = (char) 1;
                else
                    label[i][j] = (char) 0;

                jm1 = j;
                j = jp1;
                jp1++;
            }

            im1 = i;
            i=ip1;
            ip1++;
        }
    }

    labelOut->setValidRect(destRect);
    delete usableRect;
    delete destRect;
}


void ZeroCrossingDetector::applyInto_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                      RasterImage* imgOut)
{
    bool localGray;
    RasterImage_gray* grayOut = RasterImage_gray::localRasterGray(imgOut, &localGray);

    ImageRect* usableRect = new ImageRect(imgIn->getValidRect());
    usableRect->removeAllAround(1);
    ImageRect* destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION(	destRect == NULL,
                    kInvalidRectangleError,
                    "input image valid date rect and output rectangles must overlap");

    unsigned char* const* rasterOut = grayOut->getShiftedRaster2D(R_W_ACCESS);

    const int   iLow = destRect->getTop( ),
                iHigh = destRect->getBottom( ),
                jLow = destRect->getLeft( ),
                jHigh = destRect->getRight( );
    int im1, ip1, jm1, jp1;

    if (imgIn->hasFloatRaster() && imgIn->floatRasterIsReference())
    {
        const float* const* frIn = (static_cast<const RasterImage_gray_F*>(imgIn))->getShiftedGrayF2D();

        im1 = iLow-1;
        ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; )
        {
            jm1 = jLow-1;
            jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; )
            {
                if ( (frIn[im1][jm1]*frIn[ip1][jp1] < 0.f) ||
                        (frIn[im1][jp1]*frIn[ip1][jm1] < 0.f) ||
                        (frIn[i][jm1]*frIn[i][jp1] < 0.f)     ||
                        (frIn[im1][j]*frIn[ip1][j] < 0.f) )
                    rasterOut[i][j] = (unsigned char) 255;
                else
                    rasterOut[i][j] = (unsigned char) 0;

                jm1 = j;
                j = jp1;
                jp1++;
            }

            im1 = i;
            i=ip1;
            ip1++;
        }
    }
    else
    {
        const unsigned char* const* irIn = imgIn->getShiftedRaster2D();

        im1 = iLow-1;
        ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; )
        {
            jm1 = jLow-1;
            jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; )
            {
                if ( (irIn[im1][jm1]*irIn[ip1][jp1] < 0) ||
                        (irIn[im1][jp1]*irIn[ip1][jm1] < 0) ||
                        (irIn[i][jm1]*irIn[i][jp1] < 0)     ||
                        (irIn[im1][j]*irIn[ip1][j] < 0) )
                    rasterOut[i][j] = (unsigned char) 255;
                else
                    rasterOut[i][j] = (unsigned char) 0;

                jm1 = j;
                j = jp1;
                jp1++;
            }

            im1 = i;
            i=ip1;
            ip1++;
        }
    }

    grayOut->setValidRect(theRect);
    if (localGray)
    {
        grayOut->copyInto(imgOut);
        delete grayOut;
    }

    delete usableRect;
    delete destRect;
}

void ZeroCrossingDetector::showSignAsColor(const RasterImage_gray_F* imgIn, RasterImage_RGBa* imgOut,
										   const unsigned char colorChoice[][3])
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ZeroCrossingDetector::applyTo");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output RasterImage as parameter in ZeroCrossingDetector::applyTo");

    ImageRect* inRect = new ImageRect(imgIn->getValidRect());
    ImageRect* destRect = ImageRect::intersection(inRect, imgOut->getBoundRect());
    FAIL_CONDITION(	destRect == NULL,
                    kInvalidRectangleError,
                    "input image valid date rect and output rectangles must overlap");

    unsigned char* const* rgba = imgOut->getShiftedRaster2D(R_W_ACCESS);
    const float* const* fGray = imgIn->getShiftedGrayF2D();

    const int   iLow = destRect->getTop( ),
                iHigh = destRect->getBottom( ),
                jLow = destRect->getLeft( ),
                jHigh = destRect->getRight( );

    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            if (fGray[i][j] > 0.f)
            {
                rgba[i][4*j] = colorChoice[0][0];
                rgba[i][4*j+1] = colorChoice[0][1];
                rgba[i][4*j+2] = colorChoice[0][2];
                rgba[i][4*j+3] = (unsigned char) 255;
            }
            else if (fGray[i][j] < 0.f)
            {
                rgba[i][4*j] = colorChoice[1][0];
                rgba[i][4*j+1] = colorChoice[1][1];
                rgba[i][4*j+2] = colorChoice[1][2];
                rgba[i][4*j+3] = (unsigned char) 255;
            }
            else
            {
                rgba[i][4*j] = colorChoice[2][0];
                rgba[i][4*j+1] = colorChoice[2][1];
                rgba[i][4*j+2] = colorChoice[2][2];
                rgba[i][4*j+3] = (unsigned char) 255;
            }
        }
    }
    delete destRect;
}

