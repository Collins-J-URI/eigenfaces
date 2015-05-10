/*  NAME:
        NonMaximumSuppressor.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib NonMaximumSuppressor class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <string>
#include <typeinfo>
#include <cmath>
//
#include "NonMaximumSuppressor.h"
#include "VectorField_F.h"
#include "VectorField_I.h"

#include <iostream>

using namespace std;
using namespace uriVL;

#define kNormThreshold  0.1f


//----------------------------------------------------------
//  method implementations
//----------------------------------------------------------


NonMaximumSuppressor::NonMaximumSuppressor(void)
        :	ImageOperator( )
{
}


NonMaximumSuppressor::~NonMaximumSuppressor(void)
{}



#if 0
#pragma mark -
#endif


RasterImage* NonMaximumSuppressor::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    return applyTo_st(imgIn, imgIn->getValidRect());
}

RasterImage* NonMaximumSuppressor::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    return applyTo_st(imgIn, theRect);
}


void NonMaximumSuppressor::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    applyInto_st(imgIn, imgIn->getValidRect(), imgOut);
}


void NonMaximumSuppressor::applyInto(const RasterImage* imgIn, const ImageRect* theRect,
                                     RasterImage* imgOut)
{
    applyInto_st(imgIn, theRect, imgOut);
}


#if 0
#pragma mark -
#endif


RasterImage* NonMaximumSuppressor::applyTo_st(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    return applyTo_st(imgIn, imgIn->getValidRect());
}

RasterImage* NonMaximumSuppressor::applyTo_st(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in VectorField_F::copyInto");

    ImageRect	*destRect = ImageRect::intersection(imgIn->getValidRect(), theRect);
    FAIL_CONDITION( destRect == NULL,
                    kNullRectangleError,
                    "Destination rectangle does not intersect valid data rectangle");

    RasterImage	*imgOut = NULL;
    switch (imgIn->getBaseRasterType())
    {
        case kGrayRaster:
            imgOut = new RasterImage_gray_F(destRect);
            break;

        case kRGB24Raster:
            FAIL_CONDITION(	true,
                            kNoCode,
                            "image type not suported yet by NonMaximumSuppressor");
            //            imgOut = new RasterImage_RGB_F(destRect);
            break;

        case kRGBa32Raster:
            FAIL_CONDITION(	true,
                            kNoCode,
                            "image type not suported yet by NonMaximumSuppressor");
            //            imgOut = new RasterImage_RGBa_F(destRect);
            break;

        default:
            FAIL_CONDITION(	true,
                            kNoCode,
                            "image type not suported yet by NonMaximumSuppressor");
            break;

    }

    applyInto_st(imgIn, theRect, imgOut);

    delete destRect;

    return imgOut;
}


void NonMaximumSuppressor::applyInto_st(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    applyInto_st(imgIn, imgIn->getValidRect(), imgOut);
}


void NonMaximumSuppressor::applyInto_st(const RasterImage* imgIn, const ImageRect* theRect,
                                     RasterImage* imgOut)
{
    //----------------------------------------------------------------
    //	STEP 0:		Test for null or invalid type parameters
    //----------------------------------------------------------------
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in NonMaximumSuppressor::applyInto");

    FAIL_CONDITION(	imgOut == NULL,
                    kNullRasterImageError,
                    "NULL output raster image passed as parameter");


    //----------------------------------------------------------------
    //	STEP 1:		Test for rectangle intersection
    //----------------------------------------------------------------
    //  determine the output rectangle that the input image can produce.
    ImageRect	*destRect = ImageRect::intersection(imgIn->getValidRect(), theRect);
    FAIL_CONDITION( destRect == NULL,
                    kNullRectangleError,
                    "Destination rectangle does not intersect valid data rectangle");
    //  if the output image does not contain the destination rect, it must be resized.
    if (!imgOut->contains(destRect))
        imgOut->setBoundRect(destRect);


    //----------------------------------------------------------------
    //	STEP 2:		Check out the type of the input and output images
    //				and call the appropriate function
    //----------------------------------------------------------------
    //	So far I only work with RasterImage_gray_F input and destination images.
    FAIL_CONDITION( (typeid(*imgIn) != typeid(uriVL::RasterImage_gray_F)) ,
                    kInvalidRasterType,
                    "NonMaximumSuppressor::applyInto only accepts RasterImage_gray_F images as input");
    FAIL_CONDITION( (typeid(*imgOut) != typeid(uriVL::RasterImage_gray_F)) ,
                    kInvalidRasterType,
                    "NonMaximumSuppressor::applyInto only accepts RasterImage_gray_F images as destination");



    applyInto_grayF_((RasterImage_gray_F *) imgIn, destRect, (RasterImage_gray_F *) imgOut);

    //----------------------------------------------------------------
    //	STEP 3:		Set rectangle
    //----------------------------------------------------------------
    //	Very important:  At the end of the computation, don't forget to set the
    //	output's valid data rectangle.  In "safe" mode, the library has no way to guess
    //	what the valid data rectangle should be and sets it to its minimum value... empty
    imgOut->setValidRect(destRect);
    //imgOut->setFloatRasterAsReference();
    //	((RasterImage_gray_F *) imgOut)->setFloatConversionMode(floatConversionMode_);

    //----------------------------------------------------------------
    //	STEP 4:		Free temporary storage (if needed)
    //----------------------------------------------------------------
    delete destRect;
}


#if 0
#pragma mark -
#endif


VectorField *NonMaximumSuppressor::applyTo(const VectorField *vectIn)
{
    FAIL_CONDITION(	vectIn == NULL,
                    kNullVectorFieldError,
                    "NULL input vector field passed as parameter");

    return applyTo_st(vectIn, vectIn->getValidRect());
}

VectorField *NonMaximumSuppressor::applyTo(const VectorField *vectIn, const ImageRect* theRect)
{
    return applyTo_st(vectIn, theRect);
}


void NonMaximumSuppressor::applyInto(const VectorField *vectIn, VectorField *vectOut)
{
    FAIL_CONDITION(	vectIn == NULL,
                    kNullRasterImageError,
                    "NULL input vector field passed as parameter");

    applyInto_st(vectIn, vectIn->getValidRect(), vectOut);
}


void NonMaximumSuppressor::applyInto(const VectorField *vectIn, const ImageRect* theRect,
                                     VectorField *vectOut)
{
    applyInto_st(vectIn, theRect, vectOut);
}


#if 0
#pragma mark -
#endif

VectorField *NonMaximumSuppressor::applyTo_st(const VectorField *vectIn)
{
    FAIL_CONDITION(	vectIn == NULL,
                    kNullVectorFieldError,
                    "NULL input vector field passed as parameter");

    return applyTo_st(vectIn, vectIn->getValidRect());
}

VectorField *NonMaximumSuppressor::applyTo_st(const VectorField *vectIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	vectIn == NULL,
                    kNullVectorFieldError,
                    "NULL input vector field passed as parameter");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in VectorField_F::copyInto");

    ImageRect	*destRect = ImageRect::intersection(vectIn->getValidRect(), theRect);
    FAIL_CONDITION( destRect == NULL,
                    kNullRectangleError,
                    "Destination rectangle does not intersect valid data rectangle");

    VectorField	*vectOut = NULL;
    if (vectIn->hasFloatRaster())
        vectOut = new VectorField_F(destRect);
    else
        vectOut = new VectorField_I(destRect);

    applyInto_st(vectIn, theRect, vectOut);

    delete destRect;

    return vectOut;
}


void NonMaximumSuppressor::applyInto_st(const VectorField* vectIn, VectorField* vectOut)
{
    FAIL_CONDITION(	vectIn == NULL,
                    kNullRasterImageError,
                    "NULL input vector field passed as parameter");

    applyInto_st(vectIn, vectIn->getValidRect(), vectOut);
}


void NonMaximumSuppressor::applyInto_st(const VectorField* vectIn, const ImageRect* theRect,
                                     VectorField* vectOut)
{
    //----------------------------------------------------------------
    //	STEP 0:		Test for null or invalid type parameters
    //----------------------------------------------------------------
    FAIL_CONDITION(	vectIn == NULL,
                    kNullVectorFieldError,
                    "NULL input vector field passed as parameter");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in NonMaximumSuppressor::applyInto");
    FAIL_CONDITION(	vectOut == NULL,
                    kNullVectorFieldError,
                    "NULL output vector field passed as parameter");

    //----------------------------------------------------------------
    //	STEP 1:		Test for rectangle intersection
    //----------------------------------------------------------------
    //  determine the output rectangle that the input image can produce.
    ImageRect	*inRect = new ImageRect(vectIn->getValidRect());
    inRect->removeAllAround(1);
    ImageRect	*destRect = ImageRect::intersection(inRect, theRect);
    delete inRect;
    FAIL_CONDITION( destRect == NULL,
                    kNullRectangleError,
                    "Destination rectangle does not intersect valid data rectangle");

    //  if the output image does not contain the destination rect, it must be resized.
    if (!vectOut->contains(destRect))
        vectOut->setBoundRect(destRect);

    //----------------------------------------------------------------
    //	STEP 2:		Check out the type of the input and output vector fields
    //				and call the appropriate function
    //----------------------------------------------------------------
    //	So far I only work with VectorField_F as input and destination.
    FAIL_CONDITION( !vectIn->hasFloatRaster(),
                    kInvalidRasterType,
                    "NonMaximumSuppressor::applyInto only accepts VectorField_F as input");
    FAIL_CONDITION( !vectOut->hasFloatRaster(),
                    kInvalidRasterType,
                    "NonMaximumSuppressor::applyInto only accepts VectorField_F as destination");

    applyInto_vectF_((VectorField_F *) vectIn, destRect, (VectorField_F *) vectOut);

    //----------------------------------------------------------------
    //	STEP 3:		Set rectangle
    //----------------------------------------------------------------
    //	Very important:  At the end of the computation, don't forget to set the
    //	output's valid data rectangle.  In "safe" mode, the library has no way to guess
    //	what the valid data rectangle should be and sets it to its minimum value... empty
    vectOut->setValidRect(destRect);
    vectOut->setRasterAsReference();
    vectOut->forceRasterUpdate();


    //----------------------------------------------------------------
    //	STEP 4:		Free temporary storage (if needed)
    //----------------------------------------------------------------
    delete destRect;
}


#if 0
#pragma mark -
#endif


void NonMaximumSuppressor::applyInto_grayF_(const RasterImage_gray_F* imgIn, const ImageRect* theRect,
        RasterImage_gray_F *imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.

// unused variables... -CJC
//     int   iLow = theRect->getTop(),
//                    iHigh = theRect->getBottom(),
//                            jLow = theRect->getLeft(),
//                                   jHigh = theRect->getRight();
//     float   **inGray = imgIn->getShiftedGrayF2D();
//     float   **outGray = imgOut->getShiftedGrayF2D();

    FAIL_CONDITION( true,
                    kNoCode,
                    "not open for business");

}

void NonMaximumSuppressor::applyInto_vectF_(const VectorField_F* vectIn, const ImageRect* theRect,
        VectorField_F *vectOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow  = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();

    //  Get a reference to the coordinate rasters of the input and output vector fields
    const float* const* vxIn = static_cast<const float* const*>(vectIn->getShiftedRasterX2D());
    const float* const* vyIn = static_cast<const float* const*>(vectIn->getShiftedRasterY2D());
    float   **vxOut = static_cast<float** >(vectOut->getShiftedRasterX2D(R_W_ACCESS)),
            **vyOut = static_cast<float** >(vectOut->getShiftedRasterY2D(R_W_ACCESS));

    //  Make sure that the norm and phase we have for the input vector field are up-to-date
    //  and then get a reference to these rasters.  Here I call "compute" first to get a bit
    //  of speedup in case both rasters need to be calculated (typically)
    vectIn->computeNormAndPhase();
    const RasterImage_gray_F* normImg = vectIn->getNormAsRasterImage();
	const RasterImage_gray_F* phaseImg = vectIn->getPhaseAsRasterImage();
    const float* const* normIn = normImg->getShiftedGrayF2D();
    const float* const* phaseIn = phaseImg->getShiftedGrayF2D();
    const float FOUR_DIV_PI = static_cast<float>(4.L / PI);
    int   gradDirIndex;
    float   dx, dy, normE1, normE2;

    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            float   norm = normIn[i][j];
            float   phase = phaseIn[i][j];

            //  small values are directly eliminated
            if (norm < kNormThreshold)
            {
                vxOut[i][j] = vyOut[i][j] = 0.f;
            }
            //  for the other ones, we determined whether the direction of
            //  the edge (perp. to the gradient) is predominantly vert. or hor.
            else
            {
                gradDirIndex = 4 + static_cast<int>(FOUR_DIV_PI * phase);

                switch(gradDirIndex)
                {
                        //  [0, pi/4] or [-pi, -3pi/4]
                    case 0:
                    case 4:
                        dy = vyIn[i][j] / vxIn[i][j];
                        normE1 = dy * normIn[i-1][j+1] + (1.f - dy) * normIn[i][j+1];
                        if (norm > normE1)
                        {
                            normE2 = dy * normIn[i+1][j-1] + (1.f - dy) * normIn[i][j-1];
                            if (norm > normE2)
                            {
                                vxOut[i][j] = vxIn[i][j];
                                vyOut[i][j] = vyIn[i][j];
                            }
                            else
                                vxOut[i][j] = vyOut[i][j] = 0.f;

                            break;
                        }
                        else
                            vxOut[i][j] = vyOut[i][j] = 0.f;

                        break;

                        //  [pi/4, pi/2] or [-3pi/4, -pi/2]
                    case 1:
                    case 5:
                        dx = vxIn[i][j] / vyIn[i][j];
                        normE1 = dx * normIn[i-1][j+1] + (1.f - dx) * normIn[i-1][j];
                        if (norm > normE1)
                        {
                            normE2 = dx * normIn[i+1][j-1] + (1.f - dx) * normIn[i+1][j];
                            if (norm > normE2)
                            {
                                vxOut[i][j] = vxIn[i][j];
                                vyOut[i][j] = vyIn[i][j];
                            }
                            else
                                vxOut[i][j] = vyOut[i][j] = 0.f;

                            break;
                        }
                        else
                            vxOut[i][j] = vyOut[i][j] = 0.f;

                        break;

                        //  [pi/2, 3pi/4] or [-pi/2, -pi/4]
                    case 2:
                    case 6:
                        dx = -vxIn[i][j] / vyIn[i][j];
                        normE1 = dx * normIn[i-1][j-1] + (1.f - dx) * normIn[i-1][j];
                        if (norm > normE1)
                        {
                            normE2 = dx * normIn[i+1][j+1] + (1.f - dx) * normIn[i+1][j];
                            if (norm > normE2)
                            {
                                vxOut[i][j] = vxIn[i][j];
                                vyOut[i][j] = vyIn[i][j];
                            }
                            else
                                vxOut[i][j] = vyOut[i][j] = 0.f;

                            break;
                        }
                        else
                            vxOut[i][j] = vyOut[i][j] = 0.f;

                        break;

                        //  [3pi/4, pi] or [pi/4, 0]
                    case 3:
                    case 7:
                        dy = -vyIn[i][j] / vxIn[i][j];
                        normE1 = dy * normIn[i-1][j-1] + (1.f - dy) * normIn[i][j-1];
                        if (norm > normE1)
                        {
                            normE2 = dy * normIn[i+1][j+1] + (1.f - dy) * normIn[i][j+1];
                            if (norm > normE2)
                            {
                                vxOut[i][j] = vxIn[i][j];
                                vyOut[i][j] = vyIn[i][j];
                            }
                            else
                                vxOut[i][j] = vyOut[i][j] = 0.f;

                            break;
                        }
                        else
                            vxOut[i][j] = vyOut[i][j] = 0.f;

                        break;

                    default:
                        FAIL_CONDITION( true,
                                        kNoCode,
                                        "should not happen");
                        break;
                }
            }
        }
    }
}

