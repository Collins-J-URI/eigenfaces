/*  NAME:
        ImageLaplacian_Sobel.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageLaplacian_Sobel class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ImageLaplacian_Sobel.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGBa_F.h"

using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif

ImageLaplacian_Sobel::ImageLaplacian_Sobel(LaplacianSobelForm form)
        :	ImageLaplacian(3, 3, true),
			form_(form)
{
}

ImageLaplacian_Sobel::~ImageLaplacian_Sobel(void)
{}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters
//------------------------------------------------------
#endif


float ImageLaplacian_Sobel::getScale(void) const
{
    return 0.6f;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif

RasterImage* ImageLaplacian_Sobel::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    return applyTo(imgIn, imgIn->getValidRect());
}

RasterImage* ImageLaplacian_Sobel::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
    usableRect->removeAllAround(1);
    ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION( destRect == NULL,
                    kNullRectangleError,
                    "Destination rectangle does not intersect valid data rectangle");

    RasterImage	*imgOut = new RasterImage_gray_F(destRect);
    applyInto(imgIn, theRect, imgOut);

    delete destRect;
    delete usableRect;

    return imgOut;
}


void ImageLaplacian_Sobel::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}


void ImageLaplacian_Sobel::applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut)
{
    //----------------------------------------------------------------
    //	STEP 0:		Test for null or invalid type parameters
    //----------------------------------------------------------------
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in ImageLaplacian_Sobel::applyInto");

    FAIL_CONDITION(	imgOut == NULL,
                    kNullRasterImageError,
                    "NULL output raster image passed as parameter");

    //  so far I only accept a RasterImage_gray_F as a destination
    FAIL_CONDITION( (imgOut->getBaseRasterType() != kGrayRaster)  ||
                    !(imgOut->hasFloatRaster()),
                    kInvalidRasterType,
                    "ImageLaplacian_Sobel::applyInto only accepts RasterImage_gray_F images as destination");


    //----------------------------------------------------------------
    //	STEP 1:		Test for rectangle intersection
    //----------------------------------------------------------------
    //  determine the output rectangle that the input image can produce.
    //		Here, because we apply a filter of width&height 3, we
    //		"lose m all around".
    ImageRect* usableRect = new ImageRect(imgIn->getValidRect());
    usableRect->removeAllAround(1);
    //	... and determine its intersection with the user-specified output rectangle
    ImageRect* destRect = ImageRect::intersection(theRect, usableRect);
    delete usableRect;
    FAIL_CONDITION( destRect == NULL,
                    kNullRectangleError,
                    "Destination rectangle does not intersect valid data rectangle");
    //  if the output image does not contain the destination rect, it must be resized.
    if (!imgOut->contains(destRect))
        imgOut->setBoundRect(destRect);


    //----------------------------------------------------------------
    //	STEP 2:		Operate only on a gray-level image
    //----------------------------------------------------------------
    bool localGray;
    const RasterImage_gray* grayImg = RasterImage_gray::localRasterGray(imgIn, &localGray);


    //----------------------------------------------------------------
    //	STEP 3:		Perform the calculations
    //----------------------------------------------------------------
    switch (form_)
    {
        case LAPLACIAN_SOBEL_FORM_1:
            applyInto_form1_(grayImg, destRect, imgOut);
            break;

        case LAPLACIAN_SOBEL_FORM_2:
            applyInto_form2_(grayImg, destRect, imgOut);
            break;

        case LAPLACIAN_SOBEL_FORM_3:
            applyInto_form3_(grayImg, destRect, imgOut);
            break;

        default:
            FAIL_CONDITION(	true,
                            kImageProcessingError,
                            "invalid Laplacian Sobel form");
            break;
    }

    //----------------------------------------------------------------
    //	STEP 4:		Set rectangle
    //----------------------------------------------------------------
    //	Very important:  At the end of the computation, don't forget to set the
    //	output's valid data rectangle.  In "safe" mode, the library has no way to guess
    //	what the valid data rectangle should be and sets it to its minimum value... empty
    imgOut->setValidRect(destRect);
    ((RasterImage_gray_F *) imgOut)->setFloatConversionMode(getFloatConversionMode());

    //----------------------------------------------------------------
    //	STEP 5:		Free temporary storage (if needed)
    //----------------------------------------------------------------
    if (localGray)
        delete grayImg;
    delete destRect;
}

#if 0
#pragma mark -
#endif


void ImageLaplacian_Sobel::applyInto_form1_(const RasterImage* imgIn, const ImageRect* theRect,
        RasterImage* imgOut)
{
    const int   iLow  = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();

    /// 2 cases
    if (imgIn->hasFloatRaster())
    {
        const float* const* inGray = (static_cast<const RasterImage_gray_F*>(imgIn))->getShiftedGrayF2D();
        float* const* outGray = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);

        //  compute convolution over the processing rectangle
        int   im1 = iLow-1, ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; i++)
        {
            int	jm1 = jLow-1, jp1 = jLow+1;

            for (int j=jLow; j<=jHigh; j++)
            {
                outGray[i][j] = inGray[i][jp1] + inGray[i][jm1] +
                                inGray[im1][j] + inGray[ip1][j] -
                                4.f*inGray[i][j];

                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm1 = j;
                jp1++;
            }

            //  shift the i indexes (i itself is taken care of in the loop
            //  statement)
            im1 = i;
            ip1++;
        }
    }
    else
    {
        const unsigned char* const* inGray = imgIn->getShiftedRaster2D();
        float* const* outGray = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);

        //  compute convolution over the processing rectangle
        int   im1 = iLow-1, ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; i++)
        {
            int	jm1 = jLow-1, jp1 = jLow+1;

            for (int j=jLow; j<=jHigh; j++)
            {
                outGray[i][j] = inGray[i][jp1] + inGray[i][jm1] +
                                inGray[im1][j] + inGray[ip1][j] -
                                4.f*inGray[i][j];

                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm1 = j;
                jp1++;
            }

            //  shift the i indexes (i itself is taken care of in the loop
            //  statement)
            im1 = i;
            ip1++;
        }
    }
}


void ImageLaplacian_Sobel::applyInto_form2_(const RasterImage* imgIn, const ImageRect* theRect,
        RasterImage* imgOut)
{
    const int   iLow  = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();

    /// 2 cases
    if (imgIn->hasFloatRaster())
    {
        const float* const* inGray = (static_cast<const RasterImage_gray_F*>(imgIn))->getShiftedGrayF2D();
        float* const* outGray = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);

        //  compute convolution over the processing rectangle
        int   im1 = iLow-1, ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; i++)
        {
            int	jm1 = jLow-1, jp1 = jLow+1;

            for (int j=jLow; j<=jHigh; j++)
            {
                outGray[i][j] = inGray[im1][jp1] + inGray[ip1][jm1] +
                                inGray[im1][jm1] + inGray[ip1][jp1] -
                                4.f*inGray[i][j];

                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm1 = j;
                jp1++;
            }

            //  shift the i indexes (i itself is taken care of in the loop
            //  statement)
            im1 = i;
            ip1++;
        }
    }
    else
    {
        const unsigned char* const* inGray = imgIn->getShiftedRaster2D();
        float* const* outGray = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);

        //  compute convolution over the processing rectangle
        int   im1 = iLow-1, ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; i++)
        {
            int	jm1 = jLow-1, jp1 = jLow+1;

            for (int j=jLow; j<=jHigh; j++)
            {
                outGray[i][j] = inGray[im1][jp1] + inGray[ip1][jm1] +
                                inGray[im1][jm1] + inGray[ip1][jp1] -
                                4.f*inGray[i][j];

                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm1 = j;
                jp1++;
            }

            //  shift the i indexes (i itself is taken care of in the loop
            //  statement)
            im1 = i;
            ip1++;
        }
    }
}

void ImageLaplacian_Sobel::applyInto_form3_(const RasterImage* imgIn, const ImageRect* theRect,
        RasterImage* imgOut)
{
    const int   iLow  = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();

    /// 2 cases
    if (imgIn->hasFloatRaster())
    {
        const float* const* inGray = (static_cast<const RasterImage_gray_F*>(imgIn))->getShiftedGrayF2D();
        float* const* outGray = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);

        //  compute convolution over the processing rectangle
        int   im1 = iLow-1, ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; i++)
        {
            int	jm1 = jLow-1, jp1 = jLow+1;

            for (int j=jLow; j<=jHigh; j++)
            {
                outGray[i][j] = (inGray[im1][jm1] + inGray[im1][jp1] +
                                 inGray[ip1][jm1] + inGray[ip1][jp1]) +
                                4.f*(inGray[im1][j] + inGray[i][jm1] +
                                     inGray[i][jp1] + inGray[ip1][j]) -
                                20.f*inGray[i][j];

                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm1 = j;
                jp1++;
            }

            //  shift the i indexes (i itself is taken care of in the loop
            //  statement)
            im1 = i;
            ip1++;
        }
    }
    else
    {
        const unsigned char* const* inGray = imgIn->getShiftedRaster2D();
        float* const* outGray = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);

        //  compute convolution over the processing rectangle
        int   im1 = iLow-1, ip1 = iLow+1;
        for (int i=iLow; i<=iHigh; i++)
        {
            int	jm1 = jLow-1, jp1 = jLow+1;

            for (int j=jLow; j<=jHigh; j++)
            {
                outGray[i][j] = (inGray[im1][jm1] + inGray[im1][jp1] +
                                 inGray[ip1][jm1] + inGray[ip1][jp1]) +
                                4.f*(inGray[im1][j] + inGray[i][jm1] +
                                     inGray[i][jp1] + inGray[ip1][j]) -
                                20.f*inGray[i][j];

                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm1 = j;
                jp1++;
            }

            //  shift the i indexes (i itself is taken care of in the loop
            //  statement)
            im1 = i;
            ip1++;
        }
    }
}



