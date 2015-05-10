/*  NAME:
        ImageGradient_Sobel.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageGradient_Sobel class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ImageGradient_Sobel.h"
#include "VectorField_I.h"
#include "VectorField_F.h"

using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ImageGradient_Sobel::ImageGradient_Sobel(void)
		try	:	ImageGradient(1, true)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Sobel constructor");
	throw e;
}


ImageGradient_Sobel::~ImageGradient_Sobel(void)
{
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters
//------------------------------------------------------
#endif


float ImageGradient_Sobel::getScale(void) const
{
    return 0.6f;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


VectorField* ImageGradient_Sobel::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    return applyTo(imgIn, imgIn->getValidRect());
}

VectorField* ImageGradient_Sobel::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in ImageGradient_Sobel::ApplyTo");

    ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
    usableRect->removeAllAround(1);
    ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION( destRect == NULL,
                    kNullRectangleError,
                    "Destination rectangle does not intersect valid data rectangle");

    VectorField	        *vectOut;
    if (imgIn->hasFloatRaster())
        vectOut = new VectorField_F(destRect);
    else
        vectOut = new VectorField_I(destRect);

    applyInto(imgIn, theRect, vectOut);

    delete destRect;
    delete usableRect;

    return vectOut;
}

void ImageGradient_Sobel::applyInto(const RasterImage* imgIn, VectorField* vectOut)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");
    FAIL_CONDITION( vectOut == NULL,
                    kNullVectorFieldError,
                    "null VectorField passed as destination to ImageGradient_Sobel::applyInto");

    applyInto(imgIn, imgIn->getValidRect(), vectOut);
}

void ImageGradient_Sobel::applyInto(const RasterImage* imgIn, const ImageRect* theRect,
                                    VectorField* vectOut)
{
    //----------------------------------------------------------------
    //	STEP 0:		Test for null or invalid type parameters
    //----------------------------------------------------------------
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in VectorField_F::copyInto");

    FAIL_CONDITION(	vectOut == NULL,
                    kNullVectorFieldError,
                    "NULL output vector field passed as parameter");


    //----------------------------------------------------------------
    //	STEP 1:		Test for rectangle intersection
    //----------------------------------------------------------------
    //  determine the output rectangle that the input image can produce.
    //		Here, because we apply a filter of width&height (2m+1), we
    //		"lose 1 all around".
    ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
    usableRect->removeAllAround(1);
    //	... and determine its intersection with the user-specified output rectangle
    ImageRect	*destRect = ImageRect::intersection(theRect, usableRect);
    delete usableRect;
    FAIL_CONDITION( destRect == NULL,
                    kNullRectangleError,
                    "Destination rectangle does not intersect valid data rectangle");
    //  if the output field does not contain the destination rect, it must be resized.
    if (!vectOut->contains(destRect))
        vectOut->setBoundRect(destRect);


    //----------------------------------------------------------------
    //	STEP 2:		Operate only on a gray-level image
    //----------------------------------------------------------------
    bool localGray;
    const RasterImage_gray	*grayImg = RasterImage_gray::localRasterGray(imgIn, &localGray);


    //----------------------------------------------------------------
    //	STEP 3:		Perform the calculations
    //----------------------------------------------------------------
    //  There are 4 cases, depending on the presence of a float raster
    //  in the input and the destination
    if (grayImg->hasFloatRaster())
        if (vectOut->hasFloatRaster())
            applyIntoF_(static_cast<const RasterImage_gray_F*>(grayImg), destRect, 
                        static_cast<VectorField_F*>(vectOut));
        else
            applyIntoF_(static_cast<const RasterImage_gray_F*>(grayImg), destRect, 
                        static_cast<VectorField_I*>(vectOut));
    else
        if (vectOut->hasFloatRaster())
            applyIntoI_(grayImg, destRect, static_cast<VectorField_F*>(vectOut));
        else
            applyIntoI_(grayImg, destRect, static_cast<VectorField_I*>(vectOut));
        

    //----------------------------------------------------------------
    //	STEP 4:		Set rectangle
    //----------------------------------------------------------------
    //	Very important:  At the end of the computation, don't forget to set the
    //	output's valid data rectangle.  In "safe" mode, the library has no way to guess
    //	what the valid data rectangle should be and sets it to its minimum value... empty
    vectOut->setValidRect(destRect);
    vectOut->setRasterAsReference();
    vectOut->forceRasterUpdate();


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


void ImageGradient_Sobel::applyIntoF_(const RasterImage_gray_F *grayImg, const ImageRect* theRect,
                                     VectorField_F *vectOut)
{
    const int   iLow  = theRect->getTop(),
            iHigh = theRect->getBottom(),
            jLow = theRect->getLeft(),
            jHigh = theRect->getRight();
    int i, im1, ip1, j, jm1, jp1;

    const float* const* fGray = grayImg->getShiftedGrayF2D();
    float   **fvx = (float**) (vectOut->getShiftedRasterX2D(R_W_ACCESS));
    float   **fvy = (float**) (vectOut->getShiftedRasterY2D(R_W_ACCESS));

    //  compute convolution over the processing rectangle
    im1 = iLow-1;
    ip1 = iLow+1;
    for (i=iLow; i<=iHigh; i++)
    {
        jm1 = jLow-1;
        jp1 = jLow+1;
        for (j=jLow; j<=jHigh; j++)
        {
            fvx[i][j] = 0.25f*(fGray[i][jp1] - fGray[i][jm1]) +
                        0.125f*((fGray[im1][jp1] - fGray[im1][jm1]) +
                                (fGray[ip1][jp1] - fGray[ip1][jm1]));

            fvy[i][j] = 0.25f*(fGray[ip1][j] - fGray[im1][j]) +
                        0.125f*((fGray[ip1][jp1] - fGray[im1][jp1]) +
                                (fGray[ip1][jm1] - fGray[im1][jm1]));

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

void ImageGradient_Sobel::applyIntoF_(const RasterImage_gray_F *grayImg, const ImageRect* theRect,
                                     VectorField_I *vectOut)
{
    int iLow  = theRect->getTop(),
        iHigh = theRect->getBottom(),
        jLow = theRect->getLeft(),
        jHigh = theRect->getRight(),
        i, im1, ip1, j, jm1, jp1;

    const float* const* fGray = grayImg->getShiftedGrayF2D();
    int   **vx = (int**) (vectOut->getShiftedRasterX2D(R_W_ACCESS));
    int   **vy = (int**) (vectOut->getShiftedRasterY2D(R_W_ACCESS));

    //  compute convolution over the processing rectangle
    im1 = iLow-1;
    ip1 = iLow+1;
    for (i=iLow; i<=iHigh; i++)
    {
        jm1 = jLow-1;
        jp1 = jLow+1;
        for (j=jLow; j<=jHigh; j++)
        {
            vx[i][j] = static_cast<int>(0.25f*(fGray[i][jp1] - fGray[i][jm1]) +
                                        0.125f*((fGray[im1][jp1] - fGray[im1][jm1]) +
                                                (fGray[ip1][jp1] - fGray[ip1][jm1])));

            vy[i][j] = static_cast<int>(0.25f*(fGray[ip1][j] - fGray[im1][j]) +
                                        0.125f*((fGray[ip1][jp1] - fGray[im1][jp1]) +
                                                (fGray[ip1][jm1] - fGray[im1][jm1])));

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

void ImageGradient_Sobel::applyIntoI_(const RasterImage_gray* grayImg, const ImageRect* theRect,
                                     VectorField_F *vectOut)
{
    int iLow  = theRect->getTop(),
        iHigh = theRect->getBottom(),
        jLow = theRect->getLeft(),
        jHigh = theRect->getRight(),
        i, im1, ip1, j, jm1, jp1;

    const unsigned char* const* iGray = grayImg->getShiftedGray2D();
    float   **fvx = (float**) (vectOut->getShiftedRasterX2D(R_W_ACCESS));
    float   **fvy = (float**) (vectOut->getShiftedRasterY2D(R_W_ACCESS));


    //  compute convolution over the processing rectangle
    im1 = iLow-1;
    ip1 = iLow+1;
    for (i=iLow; i<=iHigh; i++)
    {
        jm1 = jLow-1;
        jp1 = jLow+1;
        for (j=jLow; j<=jHigh; j++)
        {
            fvx[i][j] = 0.25f*(iGray[i][jp1] - iGray[i][jm1]) +
                        0.125f*((iGray[im1][jp1] - iGray[im1][jm1]) +
                                (iGray[ip1][jp1] - iGray[ip1][jm1]));

            fvy[i][j] = 0.25f*(iGray[ip1][j] - iGray[im1][j]) +
                        0.125f*((iGray[ip1][jp1] - iGray[im1][jp1]) +
                                (iGray[ip1][jm1] - iGray[im1][jm1]));

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

void ImageGradient_Sobel::applyIntoI_(const RasterImage_gray* grayImg, const ImageRect* theRect,
                                     VectorField_I *vectOut)
{
    int iLow  = theRect->getTop(),
        iHigh = theRect->getBottom(),
        jLow = theRect->getLeft(),
        jHigh = theRect->getRight(),
        i, im1, ip1, j, jm1, jp1;

    const unsigned char* const* iGray = grayImg->getShiftedRaster2D();
    int** vx = (int**) (vectOut->getShiftedRasterX2D(R_W_ACCESS));
    int** vy = (int**) (vectOut->getShiftedRasterY2D(R_W_ACCESS));

    //  compute convolution over the processing rectangle
    im1 = iLow-1;
    ip1 = iLow+1;
    for (i=iLow; i<=iHigh; i++)
    {
        jm1 = jLow-1;
        jp1 = jLow+1;
        for (j=jLow; j<=jHigh; j++)
        {
            vx[i][j] = (2*(iGray[i][jp1] - iGray[i][jm1]) +
                        (iGray[im1][jp1] - iGray[im1][jm1]) +
                        (iGray[ip1][jp1] - iGray[ip1][jm1]))/8;

            vy[i][j] = (2*(iGray[ip1][j] - iGray[im1][j]) +
                        (iGray[ip1][jp1] - iGray[im1][jp1]) +
                        (iGray[ip1][jm1] - iGray[im1][jm1]))/8;

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

