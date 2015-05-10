/*  NAME:
        ImageLaplacian_Gaussian_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageLaplacian_Gaussian_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "ImageLaplacian_Gaussian_F.h"
#include "RasterImage_gray_F.h"

using namespace std;
using namespace uriVL;

#define	kMaxHardCodedWidth	13


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ImageLaplacian_Gaussian_F::ImageLaplacian_Gaussian_F(double theScale)
		try	:	ImageLaplacian(2*(((int ) (5*theScale))/2) + 1, 2*(((int ) (5*theScale))/2) + 1, false),
				//
				scale_(theScale),
				fL_(NULL)		
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Laplacian filter is 1.0");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageLaplacian_Gaussian_F constructor");
	throw e;
}


ImageLaplacian_Gaussian_F::ImageLaplacian_Gaussian_F(int theWidth)
		try	:	ImageLaplacian(theWidth, theWidth, true),
				//
				scale_(0.2L*getWidth()),
				fL_(NULL)		
{
    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageLaplacian_Gaussian_F constructor");
	throw e;
}
			

ImageLaplacian_Gaussian_F::ImageLaplacian_Gaussian_F(const ImageLaplacian_Gaussian_F& theObj)
		try	:	ImageLaplacian(theObj),
				//
				scale_(0.L),
				fL_(NULL)		
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageLaplacian_Gaussian_F copy constructor not implemented.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageLaplacian_Gaussian_I constructor");
	throw e;
}



ImageLaplacian_Gaussian_F::~ImageLaplacian_Gaussian_F(void)
{
	if (fL_ != NULL)
		delete []fL_;
}



const ImageLaplacian_Gaussian_F& ImageLaplacian_Gaussian_F::operator = (const ImageLaplacian_Gaussian_F& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageLaplacian_Gaussian_F copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters
//------------------------------------------------------
#endif


float ImageLaplacian_Gaussian_F::getScale(void) const
{
    return static_cast<float>(scale_);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif



RasterImage* ImageLaplacian_Gaussian_F::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    return applyTo(imgIn, imgIn->getValidRect());
}

RasterImage* ImageLaplacian_Gaussian_F::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
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


void ImageLaplacian_Gaussian_F::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}


void ImageLaplacian_Gaussian_F::applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut)
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
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	//	... and determine its intersection with the user-specified output rectangle
	ImageRect	*destRect = ImageRect::intersection(theRect, usableRect);
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
    switch (getWidth())
    {
        case 5:
            applyInto_5x5_(grayImg, destRect, imgOut);
            break;

        case 7:
            applyInto_7x7_(grayImg, destRect, imgOut);
            break;

        case 9:
            applyInto_9x9_(grayImg, destRect, imgOut);
            break;

        case 11:
            applyInto_11x11_(grayImg, destRect, imgOut);
            break;

        case 13:
            applyInto_13x13_(grayImg, destRect, imgOut);
            break;

        default:
            applyInto_(grayImg, destRect, imgOut);
            break;

    }

	//----------------------------------------------------------------
	//	STEP 4:		Set rectangle
	//----------------------------------------------------------------
	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
	imgOut->setValidRect(destRect);
	(static_cast<RasterImage_gray_F*>(imgOut))->setFloatConversionMode(getFloatConversionMode());

	//----------------------------------------------------------------
	//	STEP 5:		Free temporary storage (if needed)
	//----------------------------------------------------------------
	if (localGray)
	    delete grayImg;

}


#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Hard-coded private methods that implement the convolution
//-------------------------------------------------------------------------------


void ImageLaplacian_Gaussian_F::applyInto_5x5_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	jm1, jm2, jp1, jp2;
    int	im1, im2, ip1, ip2;
    float* const* fOut = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

            im1 = iLow-1; im2 = iLow-2;
    		ip1 = iLow+1; ip2 = iLow+2;

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; 
	            jp1 = jLow+1; jp2 = jLow+2; 

	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[8]*(iGray[im2][jm2] + iGray[im2][jp2] +
											iGray[ip2][jm2] + iGray[ip2][jp2]) +
									fL_[5]*(iGray[im1][jm2] + iGray[im1][jp2] +
											iGray[ip1][jm2] + iGray[ip1][jp2] +
											iGray[im2][jm1] + iGray[im2][jp1] +
											iGray[ip2][jm1] + iGray[ip2][jp1]) +
									fL_[4]*(iGray[im2][j] + iGray[ip2][j] +
											iGray[i][jm2] + iGray[i][jp2]) +
									fL_[2]*(iGray[im1][jm1] + iGray[im1][jp1] +
											iGray[ip1][jm1] + iGray[ip1][jp1]) +
									fL_[1]*(iGray[i][jm1] + iGray[i][jp1] +
											iGray[im1][j] + iGray[ip1][j]) +
									fL_[0]* iGray[i][j];

                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
                im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2++;

			}
        }
        //  float
        else
        {
            const float* const* fGray = (static_cast<const RasterImage_gray_F *>(imgIn))->getShiftedGrayF2D();

            im1 = iLow-1; im2 = iLow-2;
    		ip1 = iLow+1; ip2 = iLow+2;

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; 
	            jp1 = jLow+1; jp2 = jLow+2; 

	            for (int j=jLow; j<=jHigh; )
	            {
						
					fOut[i][j] = 	fL_[8]*(fGray[im2][jm2] + fGray[im2][jp2] +
											fGray[ip2][jm2] + fGray[ip2][jp2]) +
									fL_[5]*(fGray[im1][jm2] + fGray[im1][jp2] +
											fGray[ip1][jm2] + fGray[ip1][jp2] +
											fGray[im2][jm1] + fGray[im2][jp1] +
											fGray[ip2][jm1] + fGray[ip2][jp1]) +
									fL_[4]*(fGray[im2][j] + fGray[ip2][j] +
											fGray[i][jm2] + fGray[i][jp2]) +
									fL_[2]*(fGray[im1][jm1] + fGray[im1][jp1] +
											fGray[ip1][jm1] + fGray[ip1][jp1]) +
									fL_[1]*(fGray[i][jm1] + fGray[i][jp1] +
											fGray[im1][j] + fGray[ip1][j]) +
									fL_[0]* fGray[i][j];

                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
                im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2++;
			}
        }
    }
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kNoCode,
        				"image type not suported by Gaussian filter yet");
    }
}

void ImageLaplacian_Gaussian_F::applyInto_7x7_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	 jm1, jm2, jm3, jp1, jp2, jp3;
    int	 im1, im2, im3, ip1, ip2, ip3;
    float* const* fOut = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 

	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[18]*(iGray[im3][jm3] + iGray[im3][jp3] +
											iGray[ip3][jm3] + iGray[ip3][jp3]) +
									fL_[13]*(iGray[im3][jm2] + iGray[im3][jp2] +
											iGray[ip3][jm2] + iGray[ip3][jp2] +
											iGray[im2][jm3] + iGray[im2][jp3] +
											iGray[ip2][jm3] + iGray[ip2][jp3]) +
									fL_[10]*(iGray[im3][jm1] + iGray[im3][jp1] +
											iGray[ip3][jm1] + iGray[ip3][jp1] +
											iGray[im1][jm3] + iGray[im1][jp3] +
											iGray[ip1][jm3] + iGray[ip1][jp3]) +
									fL_[9]*(iGray[im3][j] + iGray[ip3][j] +
											iGray[i][jm3] + iGray[i][jp3]) +
									fL_[8]*(iGray[im2][jm2] + iGray[im2][jp2] +
											iGray[ip2][jm2] + iGray[ip2][jp2]) +
									fL_[5]*(iGray[im1][jm2] + iGray[im1][jp2] +
											iGray[ip1][jm2] + iGray[ip1][jp2] +
											iGray[im2][jm1] + iGray[im2][jp1] +
											iGray[ip2][jm1] + iGray[ip2][jp1]) +
									fL_[4]*(iGray[im2][j] + iGray[ip2][j] +
											iGray[i][jm2] + iGray[i][jp2]) +
									fL_[2]*(iGray[im1][jm1] + iGray[im1][jp1] +
											iGray[ip1][jm1] + iGray[ip1][jp1]) +
									fL_[1]*(iGray[i][jm1] + iGray[i][jp1] +
											iGray[im1][j] + iGray[ip1][j]) +
									fL_[0]* iGray[i][j];
						
                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3++;
			}
        }
        //  float
        else
        {
            const float* const* fGray = (static_cast<const RasterImage_gray_F *>(imgIn))->getShiftedGrayF2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 

	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[18]*(fGray[im3][jm3] + fGray[im3][jp3] +
											fGray[ip3][jm3] + fGray[ip3][jp3]) +
									fL_[13]*(fGray[im3][jm2] + fGray[im3][jp2] +
											fGray[ip3][jm2] + fGray[ip3][jp2] +
											fGray[im2][jm3] + fGray[im2][jp3] +
											fGray[ip2][jm3] + fGray[ip2][jp3]) +
									fL_[10]*(fGray[im3][jm1] + fGray[im3][jp1] +
											fGray[ip3][jm1] + fGray[ip3][jp1] +
											fGray[im1][jm3] + fGray[im1][jp3] +
											fGray[ip1][jm3] + fGray[ip1][jp3]) +
									fL_[9]*(fGray[im3][j] + fGray[ip3][j] +
											fGray[i][jm3] + fGray[i][jp3]) +
									fL_[8]*(fGray[im2][jm2] + fGray[im2][jp2] +
											fGray[ip2][jm2] + fGray[ip2][jp2]) +
									fL_[5]*(fGray[im1][jm2] + fGray[im1][jp2] +
											fGray[ip1][jm2] + fGray[ip1][jp2] +
											fGray[im2][jm1] + fGray[im2][jp1] +
											fGray[ip2][jm1] + fGray[ip2][jp1]) +
									fL_[4]*(fGray[im2][j] + fGray[ip2][j] +
											fGray[i][jm2] + fGray[i][jp2]) +
									fL_[2]*(fGray[im1][jm1] + fGray[im1][jp1] +
											fGray[ip1][jm1] + fGray[ip1][jp1]) +
									fL_[1]*(fGray[i][jm1] + fGray[i][jp1] +
											fGray[im1][j] + fGray[ip1][j]) +
									fL_[0]* fGray[i][j];
						
                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3++;
			}
        }
    }
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kNoCode,
        				"image type not suported by Gaussian filter yet");
    }
}

void ImageLaplacian_Gaussian_F::applyInto_9x9_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	jm1, jm2, jm3, jm4, jp1, jp2, jp3, jp4;
    int	im1, im2, im3, im4, ip1, ip2, ip3, ip4;
    float* const* fOut = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4;

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jm4 = jLow-4;
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
	            jp4 = jLow+4;

	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[32]*(iGray[im4][jm4] + iGray[im4][jp4] +
											iGray[ip4][jm4] + iGray[ip4][jp4]) +
									fL_[25]*(iGray[im4][jm3] + iGray[im4][jp3] +
											iGray[ip4][jm3] + iGray[ip4][jp3] +
											iGray[im3][jm4] + iGray[im3][jp4] +
											iGray[ip3][jm4] + iGray[ip3][jp4]) +
									fL_[20]*(iGray[im4][jm2] + iGray[im4][jp2] +
											iGray[ip4][jm2] + iGray[ip4][jp2] +
											iGray[im2][jm4] + iGray[im2][jp4] +
											iGray[ip2][jm4] + iGray[ip2][jp4]) +
									fL_[18]*(iGray[im3][jm3] + iGray[im3][jp3] +
											iGray[ip3][jm3] + iGray[ip3][jp3]) +
									fL_[17]*(iGray[im4][jm1] + iGray[im4][jp1] +
											iGray[ip4][jm1] + iGray[ip4][jp1] +
											iGray[im1][jm4] + iGray[im1][jp4] +
											iGray[ip1][jm4] + iGray[ip1][jp4]) +
									fL_[16]*(iGray[im4][j] + iGray[ip4][j] +
											iGray[i][jm4] + iGray[i][jp4]) +
									fL_[13]*(iGray[im3][jm2] + iGray[im3][jp2] +
											iGray[ip3][jm2] + iGray[ip3][jp2] +
											iGray[im2][jm3] + iGray[im2][jp3] +
											iGray[ip2][jm3] + iGray[ip2][jp3]) +
									fL_[10]*(iGray[im3][jm1] + iGray[im3][jp1] +
											iGray[ip3][jm1] + iGray[ip3][jp1] +
											iGray[im1][jm3] + iGray[im1][jp3] +
											iGray[ip1][jm3] + iGray[ip1][jp3]) +
									fL_[9]*(iGray[im3][j] + iGray[ip3][j] +
											iGray[i][jm3] + iGray[i][jp3]) +
									fL_[8]*(iGray[im2][jm2] + iGray[im2][jp2] +
											iGray[ip2][jm2] + iGray[ip2][jp2]) +
									fL_[5]*(iGray[im1][jm2] + iGray[im1][jp2] +
											iGray[ip1][jm2] + iGray[ip1][jp2] +
											iGray[im2][jm1] + iGray[im2][jp1] +
											iGray[ip2][jm1] + iGray[ip2][jp1]) +
									fL_[4]*(iGray[im2][j] + iGray[ip2][j] +
											iGray[i][jm2] + iGray[i][jp2]) +
									fL_[2]*(iGray[im1][jm1] + iGray[im1][jp1] +
											iGray[ip1][jm1] + iGray[ip1][jp1]) +
									fL_[1]*(iGray[i][jm1] + iGray[i][jp1] +
											iGray[im1][j] + iGray[ip1][j]) +
									fL_[0]* iGray[i][j];
						
                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm4 = jm3;
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3 = jp4;
                    jp4++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3 = ip4;
				ip4++;
			}
        }
        //  float
        else
        {
            const float* const* fGray = (static_cast<const RasterImage_gray_F *>(imgIn))->getShiftedGrayF2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4;

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jm4 = jLow-4;
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
	            jp4 = jLow+4;

	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[32]*(fGray[im4][jm4] + fGray[im4][jp4] +
											fGray[ip4][jm4] + fGray[ip4][jp4]) +
									fL_[25]*(fGray[im4][jm3] + fGray[im4][jp3] +
											fGray[ip4][jm3] + fGray[ip4][jp3] +
											fGray[im3][jm4] + fGray[im3][jp4] +
											fGray[ip3][jm4] + fGray[ip3][jp4]) +
									fL_[20]*(fGray[im4][jm2] + fGray[im4][jp2] +
											fGray[ip4][jm2] + fGray[ip4][jp2] +
											fGray[im2][jm4] + fGray[im2][jp4] +
											fGray[ip2][jm4] + fGray[ip2][jp4]) +
									fL_[18]*(fGray[im3][jm3] + fGray[im3][jp3] +
											fGray[ip3][jm3] + fGray[ip3][jp3]) +
									fL_[17]*(fGray[im4][jm1] + fGray[im4][jp1] +
											fGray[ip4][jm1] + fGray[ip4][jp1] +
											fGray[im1][jm4] + fGray[im1][jp4] +
											fGray[ip1][jm4] + fGray[ip1][jp4]) +
									fL_[16]*(fGray[im4][j] + fGray[ip4][j] +
											fGray[i][jm4] + fGray[i][jp4]) +
									fL_[13]*(fGray[im3][jm2] + fGray[im3][jp2] +
											fGray[ip3][jm2] + fGray[ip3][jp2] +
											fGray[im2][jm3] + fGray[im2][jp3] +
											fGray[ip2][jm3] + fGray[ip2][jp3]) +
									fL_[10]*(fGray[im3][jm1] + fGray[im3][jp1] +
											fGray[ip3][jm1] + fGray[ip3][jp1] +
											fGray[im1][jm3] + fGray[im1][jp3] +
											fGray[ip1][jm3] + fGray[ip1][jp3]) +
									fL_[9]*(fGray[im3][j] + fGray[ip3][j] +
											fGray[i][jm3] + fGray[i][jp3]) +
									fL_[8]*(fGray[im2][jm2] + fGray[im2][jp2] +
											fGray[ip2][jm2] + fGray[ip2][jp2]) +
									fL_[5]*(fGray[im1][jm2] + fGray[im1][jp2] +
											fGray[ip1][jm2] + fGray[ip1][jp2] +
											fGray[im2][jm1] + fGray[im2][jp1] +
											fGray[ip2][jm1] + fGray[ip2][jp1]) +
									fL_[4]*(fGray[im2][j] + fGray[ip2][j] +
											fGray[i][jm2] + fGray[i][jp2]) +
									fL_[2]*(fGray[im1][jm1] + fGray[im1][jp1] +
											fGray[ip1][jm1] + fGray[ip1][jp1]) +
									fL_[1]*(fGray[i][jm1] + fGray[i][jp1] +
											fGray[im1][j] + fGray[ip1][j]) +
									fL_[0]* fGray[i][j];
						
                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm4 = jm3;
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3 = jp4;
                    jp4++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3 = ip4;
				ip4++;
			}
        }
    }
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kNoCode,
        				"image type not suported by Gaussian filter yet");
    }
}

void ImageLaplacian_Gaussian_F::applyInto_11x11_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	jm1, jm2, jm3, jm4, jm5, jp1, jp2, jp3, jp4, jp5;
    int	im1, im2, im3, im4, im5, ip1, ip2, ip3, ip4, ip5;
    float* const* fOut = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5;

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jm4 = jLow-4; jm5 = jLow-5;
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
	            jp4 = jLow+4; jp5 = jLow+5;

	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[50]*(iGray[im5][jm5] + iGray[im5][jp5] +
											iGray[ip5][jm5] + iGray[ip4][jp5]) +
									fL_[41]*(iGray[im5][jm4] + iGray[im5][jp4] +
											iGray[ip5][jm4] + iGray[ip5][jp4] +
											iGray[im4][jm5] + iGray[im4][jp5] +
											iGray[ip4][jm5] + iGray[ip4][jp5]) +
									fL_[34]*(iGray[im5][jm3] + iGray[im5][jp3] +
											iGray[ip5][jm3] + iGray[ip5][jp3] +
											iGray[im3][jm5] + iGray[im3][jp5] +
											iGray[ip3][jm5] + iGray[ip3][jp5]) +
									fL_[32]*(iGray[im4][jm4] + iGray[im4][jp4] +
											iGray[ip4][jm4] + iGray[ip4][jp4]) +
									fL_[29]*(iGray[im5][jm2] + iGray[im5][jp2] +
											iGray[ip5][jm2] + iGray[ip5][jp2] +
											iGray[im2][jm5] + iGray[im2][jp5] +
											iGray[ip2][jm5] + iGray[ip2][jp5]) +
									fL_[26]*(iGray[im5][jm1] + iGray[im5][jp1] +
											iGray[ip5][jm1] + iGray[ip5][jp1] +
											iGray[im1][jm5] + iGray[im1][jp5] +
											iGray[ip1][jm5] + iGray[ip1][jp5]) +
									fL_[25]*(iGray[im5][j] + iGray[ip5][j] +
											iGray[i][jm5] + iGray[i][jp5] +
											iGray[im4][jm3] + iGray[im4][jp3] +
											iGray[ip4][jm3] + iGray[ip4][jp3] +
											iGray[im3][jm4] + iGray[im3][jp4] +
											iGray[ip3][jm4] + iGray[ip3][jp4]) +
									fL_[20]*(iGray[im4][jm2] + iGray[im4][jp2] +
											iGray[ip4][jm2] + iGray[ip4][jp2] +
											iGray[im2][jm4] + iGray[im2][jp4] +
											iGray[ip2][jm4] + iGray[ip2][jp4]) +
									fL_[18]*(iGray[im3][jm3] + iGray[im3][jp3] +
											iGray[ip3][jm3] + iGray[ip3][jp3]) +
									fL_[17]*(iGray[im4][jm1] + iGray[im4][jp1] +
											iGray[ip4][jm1] + iGray[ip4][jp1] +
											iGray[im1][jm4] + iGray[im1][jp4] +
											iGray[ip1][jm4] + iGray[ip1][jp4]) +
									fL_[16]*(iGray[im4][j] + iGray[ip4][j] +
											iGray[i][jm4] + iGray[i][jp4]) +
									fL_[13]*(iGray[im3][jm2] + iGray[im3][jp2] +
											iGray[ip3][jm2] + iGray[ip3][jp2] +
											iGray[im2][jm3] + iGray[im2][jp3] +
											iGray[ip2][jm3] + iGray[ip2][jp3]) +
									fL_[10]*(iGray[im3][jm1] + iGray[im3][jp1] +
											iGray[ip3][jm1] + iGray[ip3][jp1] +
											iGray[im1][jm3] + iGray[im1][jp3] +
											iGray[ip1][jm3] + iGray[ip1][jp3]) +
									fL_[9]*(iGray[im3][j] + iGray[ip3][j] +
											iGray[i][jm3] + iGray[i][jp3]) +
									fL_[8]*(iGray[im2][jm2] + iGray[im2][jp2] +
											iGray[ip2][jm2] + iGray[ip2][jp2]) +
									fL_[5]*(iGray[im1][jm2] + iGray[im1][jp2] +
											iGray[ip1][jm2] + iGray[ip1][jp2] +
											iGray[im2][jm1] + iGray[im2][jp1] +
											iGray[ip2][jm1] + iGray[ip2][jp1]) +
									fL_[4]*(iGray[im2][j] + iGray[ip2][j] +
											iGray[i][jm2] + iGray[i][jp2]) +
									fL_[2]*(iGray[im1][jm1] + iGray[im1][jp1] +
											iGray[ip1][jm1] + iGray[ip1][jp1]) +
									fL_[1]*(iGray[i][jm1] + iGray[i][jp1] +
											iGray[im1][j] + iGray[ip1][j]) +
									fL_[0]* iGray[i][j];
						
                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm5 = jm4;
                    jm4 = jm3;
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3 = jp4;
                    jp4 = jp5;
                    jp5++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3 = ip4;
				ip4 = ip5;
				ip5++;
			}
        }
        //  float
        else
        {
            const float* const* fGray = (static_cast<const RasterImage_gray_F *>(imgIn))->getShiftedGrayF2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5;

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jm4 = jLow-4; jm5 = jLow-5;
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
	            jp4 = jLow+4; jp5 = jLow+5;

	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[50]*(fGray[im5][jm5] + fGray[im5][jp5] +
											fGray[ip5][jm5] + fGray[ip4][jp5]) +
									fL_[41]*(fGray[im5][jm4] + fGray[im5][jp4] +
											fGray[ip5][jm4] + fGray[ip5][jp4] +
											fGray[im4][jm5] + fGray[im4][jp5] +
											fGray[ip4][jm5] + fGray[ip4][jp5]) +
									fL_[34]*(fGray[im5][jm3] + fGray[im5][jp3] +
											fGray[ip5][jm3] + fGray[ip5][jp3] +
											fGray[im3][jm5] + fGray[im3][jp5] +
											fGray[ip3][jm5] + fGray[ip3][jp5]) +
									fL_[32]*(fGray[im4][jm4] + fGray[im4][jp4] +
											fGray[ip4][jm4] + fGray[ip4][jp4]) +
									fL_[29]*(fGray[im5][jm2] + fGray[im5][jp2] +
											fGray[ip5][jm2] + fGray[ip5][jp2] +
											fGray[im2][jm5] + fGray[im2][jp5] +
											fGray[ip2][jm5] + fGray[ip2][jp5]) +
									fL_[26]*(fGray[im5][jm1] + fGray[im5][jp1] +
											fGray[ip5][jm1] + fGray[ip5][jp1] +
											fGray[im1][jm5] + fGray[im1][jp5] +
											fGray[ip1][jm5] + fGray[ip1][jp5]) +
									fL_[25]*(fGray[im5][j] + fGray[ip5][j] +
											fGray[i][jm5] + fGray[i][jp5] +
											fGray[im4][jm3] + fGray[im4][jp3] +
											fGray[ip4][jm3] + fGray[ip4][jp3] +
											fGray[im3][jm4] + fGray[im3][jp4] +
											fGray[ip3][jm4] + fGray[ip3][jp4]) +
									fL_[20]*(fGray[im4][jm2] + fGray[im4][jp2] +
											fGray[ip4][jm2] + fGray[ip4][jp2] +
											fGray[im2][jm4] + fGray[im2][jp4] +
											fGray[ip2][jm4] + fGray[ip2][jp4]) +
									fL_[18]*(fGray[im3][jm3] + fGray[im3][jp3] +
											fGray[ip3][jm3] + fGray[ip3][jp3]) +
									fL_[17]*(fGray[im4][jm1] + fGray[im4][jp1] +
											fGray[ip4][jm1] + fGray[ip4][jp1] +
											fGray[im1][jm4] + fGray[im1][jp4] +
											fGray[ip1][jm4] + fGray[ip1][jp4]) +
									fL_[16]*(fGray[im4][j] + fGray[ip4][j] +
											fGray[i][jm4] + fGray[i][jp4]) +
									fL_[13]*(fGray[im3][jm2] + fGray[im3][jp2] +
											fGray[ip3][jm2] + fGray[ip3][jp2] +
											fGray[im2][jm3] + fGray[im2][jp3] +
											fGray[ip2][jm3] + fGray[ip2][jp3]) +
									fL_[10]*(fGray[im3][jm1] + fGray[im3][jp1] +
											fGray[ip3][jm1] + fGray[ip3][jp1] +
											fGray[im1][jm3] + fGray[im1][jp3] +
											fGray[ip1][jm3] + fGray[ip1][jp3]) +
									fL_[9]*(fGray[im3][j] + fGray[ip3][j] +
											fGray[i][jm3] + fGray[i][jp3]) +
									fL_[8]*(fGray[im2][jm2] + fGray[im2][jp2] +
											fGray[ip2][jm2] + fGray[ip2][jp2]) +
									fL_[5]*(fGray[im1][jm2] + fGray[im1][jp2] +
											fGray[ip1][jm2] + fGray[ip1][jp2] +
											fGray[im2][jm1] + fGray[im2][jp1] +
											fGray[ip2][jm1] + fGray[ip2][jp1]) +
									fL_[4]*(fGray[im2][j] + fGray[ip2][j] +
											fGray[i][jm2] + fGray[i][jp2]) +
									fL_[2]*(fGray[im1][jm1] + fGray[im1][jp1] +
											fGray[ip1][jm1] + fGray[ip1][jp1]) +
									fL_[1]*(fGray[i][jm1] + fGray[i][jp1] +
											fGray[im1][j] + fGray[ip1][j]) +
									fL_[0]* fGray[i][j];
						
                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm5 = jm4;
                    jm4 = jm3;
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3 = jp4;
                    jp4 = jp5;
                    jp5++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3 = ip4;
				ip4 = ip5;
				ip5++;
			}
        }
    }
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kNoCode,
        				"image type not suported by Gaussian filter yet");
    }
}

void ImageLaplacian_Gaussian_F::applyInto_13x13_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    int	im1, im2, im3, im4, im5, im6, ip1, ip2, ip3, ip4, ip5, ip6;
    float* const* fOut = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
	            jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[72]*(iGray[im6][jm6] + iGray[im6][jp6] +
											iGray[ip6][jm6] + iGray[ip6][jp6]) +
									fL_[61]*(iGray[im6][jm5] + iGray[im6][jp5] +
											iGray[ip6][jm5] + iGray[ip6][jp5] +
											iGray[im5][jm6] + iGray[im5][jp6] +
											iGray[ip5][jm6] + iGray[ip5][jp6]) +
									fL_[52]*(iGray[im6][jm4] + iGray[im6][jp4] +
											iGray[ip6][jm4] + iGray[ip6][jp4] +
											iGray[im4][jm6] + iGray[im4][jp6] +
											iGray[ip4][jm6] + iGray[ip4][jp6]) +
									fL_[50]*(iGray[im5][jm5] + iGray[im5][jp5] +
											iGray[ip5][jm5] + iGray[ip5][jp5]) +
									fL_[45]*(iGray[im6][jm3] + iGray[im6][jp3] +
											iGray[ip6][jm3] + iGray[ip6][jp3] +
											iGray[im3][jm6] + iGray[im3][jp6] +
											iGray[ip3][jm6] + iGray[ip3][jp6]) +
									fL_[41]*(iGray[im5][jm4] + iGray[im5][jp4] +
											iGray[ip5][jm4] + iGray[ip5][jp4] +
											iGray[im4][jm5] + iGray[im4][jp5] +
											iGray[ip4][jm5] + iGray[ip4][jp5]) +
									fL_[40]*(iGray[im6][jm2] + iGray[im6][jp2] +
											iGray[ip6][jm2] + iGray[ip6][jp2] +
											iGray[im2][jm6] + iGray[im2][jp6] +
											iGray[ip2][jm6] + iGray[ip2][jp6]) +
									fL_[37]*(iGray[im6][jm1] + iGray[im6][jp1] +
											iGray[ip6][jm1] + iGray[ip6][jp1] +
											iGray[im1][jm6] + iGray[im1][jp6] +
											iGray[ip1][jm6] + iGray[ip1][jp6]) +
									fL_[36]*(iGray[im6][j] + iGray[ip6][j] +
											iGray[i][jm6] + iGray[i][jp6]) +
									fL_[34]*(iGray[im5][jm3] + iGray[im5][jp3] +
											iGray[ip5][jm3] + iGray[ip5][jp3] +
											iGray[im3][jm5] + iGray[im3][jp5] +
											iGray[ip3][jm5] + iGray[ip3][jp5]) +
									fL_[32]*(iGray[im4][jm4] + iGray[im4][jp4] +
											iGray[ip4][jm4] + iGray[ip4][jp4]) +
									fL_[29]*(iGray[im5][jm2] + iGray[im5][jp2] +
											iGray[ip5][jm2] + iGray[ip5][jp2] +
											iGray[im2][jm5] + iGray[im2][jp5] +
											iGray[ip2][jm5] + iGray[ip2][jp5]) +
									fL_[26]*(iGray[im5][jm1] + iGray[im5][jp1] +
											iGray[ip5][jm1] + iGray[ip5][jp1] +
											iGray[im1][jm5] + iGray[im1][jp5] +
											iGray[ip1][jm5] + iGray[ip1][jp5]) +
									fL_[25]*(iGray[im5][j] + iGray[ip5][j] +
											iGray[i][jm5] + iGray[i][jp5] +
											iGray[im4][jm3] + iGray[im4][jp3] +
											iGray[ip4][jm3] + iGray[ip4][jp3] +
											iGray[im3][jm4] + iGray[im3][jp4] +
											iGray[ip3][jm4] + iGray[ip3][jp4]) +
									fL_[20]*(iGray[im4][jm2] + iGray[im4][jp2] +
											iGray[ip4][jm2] + iGray[ip4][jp2] +
											iGray[im2][jm4] + iGray[im2][jp4] +
											iGray[ip2][jm4] + iGray[ip2][jp4]) +
									fL_[18]*(iGray[im3][jm3] + iGray[im3][jp3] +
											iGray[ip3][jm3] + iGray[ip3][jp3]) +
									fL_[17]*(iGray[im4][jm1] + iGray[im4][jp1] +
											iGray[ip4][jm1] + iGray[ip4][jp1] +
											iGray[im1][jm4] + iGray[im1][jp4] +
											iGray[ip1][jm4] + iGray[ip1][jp4]) +
									fL_[16]*(iGray[im4][j] + iGray[ip4][j] +
											iGray[i][jm4] + iGray[i][jp4]) +
									fL_[13]*(iGray[im3][jm2] + iGray[im3][jp2] +
											iGray[ip3][jm2] + iGray[ip3][jp2] +
											iGray[im2][jm3] + iGray[im2][jp3] +
											iGray[ip2][jm3] + iGray[ip2][jp3]) +
									fL_[10]*(iGray[im3][jm1] + iGray[im3][jp1] +
											iGray[ip3][jm1] + iGray[ip3][jp1] +
											iGray[im1][jm3] + iGray[im1][jp3] +
											iGray[ip1][jm3] + iGray[ip1][jp3]) +
									fL_[9]*(iGray[im3][j] + iGray[ip3][j] +
											iGray[i][jm3] + iGray[i][jp3]) +
									fL_[8]*(iGray[im2][jm2] + iGray[im2][jp2] +
											iGray[ip2][jm2] + iGray[ip2][jp2]) +
									fL_[5]*(iGray[im1][jm2] + iGray[im1][jp2] +
											iGray[ip1][jm2] + iGray[ip1][jp2] +
											iGray[im2][jm1] + iGray[im2][jp1] +
											iGray[ip2][jm1] + iGray[ip2][jp1]) +
									fL_[4]*(iGray[im2][j] + iGray[ip2][j] +
											iGray[i][jm2] + iGray[i][jp2]) +
									fL_[2]*(iGray[im1][jm1] + iGray[im1][jp1] +
											iGray[ip1][jm1] + iGray[ip1][jp1]) +
									fL_[1]*(iGray[i][jm1] + iGray[i][jp1] +
											iGray[im1][j] + iGray[ip1][j]) +
									fL_[0]* iGray[i][j];
						
                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm6 = jm5;
                    jm5 = jm4;
                    jm4 = jm3;
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3 = jp4;
                    jp4 = jp5;
                    jp5 = jp6;
                    jp6++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im6 = im5;
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3 = ip4;
				ip4 = ip5;
				ip5 = ip6;
				ip6++;
			}
        }
        //  float
        else
        {
            const float* const *fGray = (static_cast<const RasterImage_gray_F *>(imgIn))->getShiftedGrayF2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
	            jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[72]*(fGray[im6][jm6] + fGray[im6][jp6] +
											fGray[ip6][jm6] + fGray[ip6][jp6]) +
									fL_[61]*(fGray[im6][jm5] + fGray[im6][jp5] +
											fGray[ip6][jm5] + fGray[ip6][jp5] +
											fGray[im5][jm6] + fGray[im5][jp6] +
											fGray[ip5][jm6] + fGray[ip5][jp6]) +
									fL_[52]*(fGray[im6][jm4] + fGray[im6][jp4] +
											fGray[ip6][jm4] + fGray[ip6][jp4] +
											fGray[im4][jm6] + fGray[im4][jp6] +
											fGray[ip4][jm6] + fGray[ip4][jp6]) +
									fL_[50]*(fGray[im5][jm5] + fGray[im5][jp5] +
											fGray[ip5][jm5] + fGray[ip5][jp5]) +
									fL_[45]*(fGray[im6][jm3] + fGray[im6][jp3] +
											fGray[ip6][jm3] + fGray[ip6][jp3] +
											fGray[im3][jm6] + fGray[im3][jp6] +
											fGray[ip3][jm6] + fGray[ip3][jp6]) +
									fL_[41]*(fGray[im5][jm4] + fGray[im5][jp4] +
											fGray[ip5][jm4] + fGray[ip5][jp4] +
											fGray[im4][jm5] + fGray[im4][jp5] +
											fGray[ip4][jm5] + fGray[ip4][jp5]) +
									fL_[40]*(fGray[im6][jm2] + fGray[im6][jp2] +
											fGray[ip6][jm2] + fGray[ip6][jp2] +
											fGray[im2][jm6] + fGray[im2][jp6] +
											fGray[ip2][jm6] + fGray[ip2][jp6]) +
									fL_[37]*(fGray[im6][jm1] + fGray[im6][jp1] +
											fGray[ip6][jm1] + fGray[ip6][jp1] +
											fGray[im1][jm6] + fGray[im1][jp6] +
											fGray[ip1][jm6] + fGray[ip1][jp6]) +
									fL_[36]*(fGray[im6][j] + fGray[ip6][j] +
											fGray[i][jm6] + fGray[i][jp6]) +
									fL_[34]*(fGray[im5][jm3] + fGray[im5][jp3] +
											fGray[ip5][jm3] + fGray[ip5][jp3] +
											fGray[im3][jm5] + fGray[im3][jp5] +
											fGray[ip3][jm5] + fGray[ip3][jp5]) +
									fL_[32]*(fGray[im4][jm4] + fGray[im4][jp4] +
											fGray[ip4][jm4] + fGray[ip4][jp4]) +
									fL_[29]*(fGray[im5][jm2] + fGray[im5][jp2] +
											fGray[ip5][jm2] + fGray[ip5][jp2] +
											fGray[im2][jm5] + fGray[im2][jp5] +
											fGray[ip2][jm5] + fGray[ip2][jp5]) +
									fL_[26]*(fGray[im5][jm1] + fGray[im5][jp1] +
											fGray[ip5][jm1] + fGray[ip5][jp1] +
											fGray[im1][jm5] + fGray[im1][jp5] +
											fGray[ip1][jm5] + fGray[ip1][jp5]) +
									fL_[25]*(fGray[im5][j] + fGray[ip5][j] +
											fGray[i][jm5] + fGray[i][jp5] +
											fGray[im4][jm3] + fGray[im4][jp3] +
											fGray[ip4][jm3] + fGray[ip4][jp3] +
											fGray[im3][jm4] + fGray[im3][jp4] +
											fGray[ip3][jm4] + fGray[ip3][jp4]) +
									fL_[20]*(fGray[im4][jm2] + fGray[im4][jp2] +
											fGray[ip4][jm2] + fGray[ip4][jp2] +
											fGray[im2][jm4] + fGray[im2][jp4] +
											fGray[ip2][jm4] + fGray[ip2][jp4]) +
									fL_[18]*(fGray[im3][jm3] + fGray[im3][jp3] +
											fGray[ip3][jm3] + fGray[ip3][jp3]) +
									fL_[17]*(fGray[im4][jm1] + fGray[im4][jp1] +
											fGray[ip4][jm1] + fGray[ip4][jp1] +
											fGray[im1][jm4] + fGray[im1][jp4] +
											fGray[ip1][jm4] + fGray[ip1][jp4]) +
									fL_[16]*(fGray[im4][j] + fGray[ip4][j] +
											fGray[i][jm4] + fGray[i][jp4]) +
									fL_[13]*(fGray[im3][jm2] + fGray[im3][jp2] +
											fGray[ip3][jm2] + fGray[ip3][jp2] +
											fGray[im2][jm3] + fGray[im2][jp3] +
											fGray[ip2][jm3] + fGray[ip2][jp3]) +
									fL_[10]*(fGray[im3][jm1] + fGray[im3][jp1] +
											fGray[ip3][jm1] + fGray[ip3][jp1] +
											fGray[im1][jm3] + fGray[im1][jp3] +
											fGray[ip1][jm3] + fGray[ip1][jp3]) +
									fL_[9]*(fGray[im3][j] + fGray[ip3][j] +
											fGray[i][jm3] + fGray[i][jp3]) +
									fL_[8]*(fGray[im2][jm2] + fGray[im2][jp2] +
											fGray[ip2][jm2] + fGray[ip2][jp2]) +
									fL_[5]*(fGray[im1][jm2] + fGray[im1][jp2] +
											fGray[ip1][jm2] + fGray[ip1][jp2] +
											fGray[im2][jm1] + fGray[im2][jp1] +
											fGray[ip2][jm1] + fGray[ip2][jp1]) +
									fL_[4]*(fGray[im2][j] + fGray[ip2][j] +
											fGray[i][jm2] + fGray[i][jp2]) +
									fL_[2]*(fGray[im1][jm1] + fGray[im1][jp1] +
											fGray[ip1][jm1] + fGray[ip1][jp1]) +
									fL_[1]*(fGray[i][jm1] + fGray[i][jp1] +
											fGray[im1][j] + fGray[ip1][j]) +
									fL_[0]* fGray[i][j];
						
                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm6 = jm5;
                    jm5 = jm4;
                    jm4 = jm3;
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3 = jp4;
                    jp4 = jp5;
                    jp5 = jp6;
                    jp6++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im6 = im5;
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3 = ip4;
				ip4 = ip5;
				ip5 = ip6;
				ip6++;
			}
        }
    }
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kNoCode,
        				"image type not suported by Gaussian filter yet");
    }
}


void ImageLaplacian_Gaussian_F::applyInto_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   m = getWidth()/2;
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    int	im1, im2, im3, im4, im5, im6, ip1, ip2, ip3, ip4, ip5, ip6;
    float* const* fOut = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
	            jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;

	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[72]*(iGray[im6][jm6] + iGray[im6][jp6] +
											iGray[ip6][jm6] + iGray[ip6][jp6]) +
									fL_[61]*(iGray[im6][jm5] + iGray[im6][jp5] +
											iGray[ip6][jm5] + iGray[ip6][jp5] +
											iGray[im5][jm6] + iGray[im5][jp6] +
											iGray[ip5][jm6] + iGray[ip5][jp6]) +
									fL_[52]*(iGray[im6][jm4] + iGray[im6][jp4] +
											iGray[ip6][jm4] + iGray[ip6][jp4] +
											iGray[im4][jm6] + iGray[im4][jp6] +
											iGray[ip4][jm6] + iGray[ip4][jp6]) +
									fL_[50]*(iGray[im5][jm5] + iGray[im5][jp5] +
											iGray[ip5][jm5] + iGray[ip5][jp5]) +
									fL_[45]*(iGray[im6][jm3] + iGray[im6][jp3] +
											iGray[ip6][jm3] + iGray[ip6][jp3] +
											iGray[im3][jm6] + iGray[im3][jp6] +
											iGray[ip3][jm6] + iGray[ip3][jp6]) +
									fL_[41]*(iGray[im5][jm4] + iGray[im5][jp4] +
											iGray[ip5][jm4] + iGray[ip5][jp4] +
											iGray[im4][jm5] + iGray[im4][jp5] +
											iGray[ip4][jm5] + iGray[ip4][jp5]) +
									fL_[40]*(iGray[im6][jm2] + iGray[im6][jp2] +
											iGray[ip6][jm2] + iGray[ip6][jp2] +
											iGray[im2][jm6] + iGray[im2][jp6] +
											iGray[ip2][jm6] + iGray[ip2][jp6]) +
									fL_[37]*(iGray[im6][jm1] + iGray[im6][jp1] +
											iGray[ip6][jm1] + iGray[ip6][jp1] +
											iGray[im1][jm6] + iGray[im1][jp6] +
											iGray[ip1][jm6] + iGray[ip1][jp6]) +
									fL_[36]*(iGray[im6][j] + iGray[ip6][j] +
											iGray[i][jm6] + iGray[i][jp6]) +
									fL_[34]*(iGray[im5][jm3] + iGray[im5][jp3] +
											iGray[ip5][jm3] + iGray[ip5][jp3] +
											iGray[im3][jm5] + iGray[im3][jp5] +
											iGray[ip3][jm5] + iGray[ip3][jp5]) +
									fL_[32]*(iGray[im4][jm4] + iGray[im4][jp4] +
											iGray[ip4][jm4] + iGray[ip4][jp4]) +
									fL_[29]*(iGray[im5][jm2] + iGray[im5][jp2] +
											iGray[ip5][jm2] + iGray[ip5][jp2] +
											iGray[im2][jm5] + iGray[im2][jp5] +
											iGray[ip2][jm5] + iGray[ip2][jp5]) +
									fL_[26]*(iGray[im5][jm1] + iGray[im5][jp1] +
											iGray[ip5][jm1] + iGray[ip5][jp1] +
											iGray[im1][jm5] + iGray[im1][jp5] +
											iGray[ip1][jm5] + iGray[ip1][jp5]) +
									fL_[25]*(iGray[im5][j] + iGray[ip5][j] +
											iGray[i][jm5] + iGray[i][jp5] +
											iGray[im4][jm3] + iGray[im4][jp3] +
											iGray[ip4][jm3] + iGray[ip4][jp3] +
											iGray[im3][jm4] + iGray[im3][jp4] +
											iGray[ip3][jm4] + iGray[ip3][jp4]) +
									fL_[20]*(iGray[im4][jm2] + iGray[im4][jp2] +
											iGray[ip4][jm2] + iGray[ip4][jp2] +
											iGray[im2][jm4] + iGray[im2][jp4] +
											iGray[ip2][jm4] + iGray[ip2][jp4]) +
									fL_[18]*(iGray[im3][jm3] + iGray[im3][jp3] +
											iGray[ip3][jm3] + iGray[ip3][jp3]) +
									fL_[17]*(iGray[im4][jm1] + iGray[im4][jp1] +
											iGray[ip4][jm1] + iGray[ip4][jp1] +
											iGray[im1][jm4] + iGray[im1][jp4] +
											iGray[ip1][jm4] + iGray[ip1][jp4]) +
									fL_[16]*(iGray[im4][j] + iGray[ip4][j] +
											iGray[i][jm4] + iGray[i][jp4]) +
									fL_[13]*(iGray[im3][jm2] + iGray[im3][jp2] +
											iGray[ip3][jm2] + iGray[ip3][jp2] +
											iGray[im2][jm3] + iGray[im2][jp3] +
											iGray[ip2][jm3] + iGray[ip2][jp3]) +
									fL_[10]*(iGray[im3][jm1] + iGray[im3][jp1] +
											iGray[ip3][jm1] + iGray[ip3][jp1] +
											iGray[im1][jm3] + iGray[im1][jp3] +
											iGray[ip1][jm3] + iGray[ip1][jp3]) +
									fL_[9]*(iGray[im3][j] + iGray[ip3][j] +
											iGray[i][jm3] + iGray[i][jp3]) +
									fL_[8]*(iGray[im2][jm2] + iGray[im2][jp2] +
											iGray[ip2][jm2] + iGray[ip2][jp2]) +
									fL_[5]*(iGray[im1][jm2] + iGray[im1][jp2] +
											iGray[ip1][jm2] + iGray[ip1][jp2] +
											iGray[im2][jm1] + iGray[im2][jp1] +
											iGray[ip2][jm1] + iGray[ip2][jp1]) +
									fL_[4]*(iGray[im2][j] + iGray[ip2][j] +
											iGray[i][jm2] + iGray[i][jp2]) +
									fL_[2]*(iGray[im1][jm1] + iGray[im1][jp1] +
											iGray[ip1][jm1] + iGray[ip1][jp1]) +
									fL_[1]*(iGray[i][jm1] + iGray[i][jp1] +
											iGray[im1][j] + iGray[ip1][j]) +
									fL_[0]* iGray[i][j];

					for (int l= kMaxHardCodedWidth+1; l<=m; l++)
						fOut[i][j] += fL_[l] * (iGray[i][j-l] + iGray[i][j+l] );


                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm6 = jm5;
                    jm5 = jm4;
                    jm4 = jm3;
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3 = jp4;
                    jp4 = jp5;
                    jp5 = jp6;
                    jp6++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im6 = im5;
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3 = ip4;
				ip4 = ip5;
				ip5 = ip6;
				ip6++;
			}
        }
        //  float
        else
        {
            const float* const* fGray = (static_cast<const RasterImage_gray_F *>(imgIn))->getShiftedGrayF2D();

            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;

            for (int i=iLow; i<=iHigh; )
            {
	            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
	            jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
	            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
	            jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;

	            for (int j=jLow; j<=jHigh; )
	            {
					fOut[i][j] = 	fL_[72]*(fGray[im6][jm6] + fGray[im6][jp6] +
											fGray[ip6][jm6] + fGray[ip6][jp6]) +
									fL_[61]*(fGray[im6][jm5] + fGray[im6][jp5] +
											fGray[ip6][jm5] + fGray[ip6][jp5] +
											fGray[im5][jm6] + fGray[im5][jp6] +
											fGray[ip5][jm6] + fGray[ip5][jp6]) +
									fL_[52]*(fGray[im6][jm4] + fGray[im6][jp4] +
											fGray[ip6][jm4] + fGray[ip6][jp4] +
											fGray[im4][jm6] + fGray[im4][jp6] +
											fGray[ip4][jm6] + fGray[ip4][jp6]) +
									fL_[50]*(fGray[im5][jm5] + fGray[im5][jp5] +
											fGray[ip5][jm5] + fGray[ip5][jp5]) +
									fL_[45]*(fGray[im6][jm3] + fGray[im6][jp3] +
											fGray[ip6][jm3] + fGray[ip6][jp3] +
											fGray[im3][jm6] + fGray[im3][jp6] +
											fGray[ip3][jm6] + fGray[ip3][jp6]) +
									fL_[41]*(fGray[im5][jm4] + fGray[im5][jp4] +
											fGray[ip5][jm4] + fGray[ip5][jp4] +
											fGray[im4][jm5] + fGray[im4][jp5] +
											fGray[ip4][jm5] + fGray[ip4][jp5]) +
									fL_[40]*(fGray[im6][jm2] + fGray[im6][jp2] +
											fGray[ip6][jm2] + fGray[ip6][jp2] +
											fGray[im2][jm6] + fGray[im2][jp6] +
											fGray[ip2][jm6] + fGray[ip2][jp6]) +
									fL_[37]*(fGray[im6][jm1] + fGray[im6][jp1] +
											fGray[ip6][jm1] + fGray[ip6][jp1] +
											fGray[im1][jm6] + fGray[im1][jp6] +
											fGray[ip1][jm6] + fGray[ip1][jp6]) +
									fL_[36]*(fGray[im6][j] + fGray[ip6][j] +
											fGray[i][jm6] + fGray[i][jp6]) +
									fL_[34]*(fGray[im5][jm3] + fGray[im5][jp3] +
											fGray[ip5][jm3] + fGray[ip5][jp3] +
											fGray[im3][jm5] + fGray[im3][jp5] +
											fGray[ip3][jm5] + fGray[ip3][jp5]) +
									fL_[32]*(fGray[im4][jm4] + fGray[im4][jp4] +
											fGray[ip4][jm4] + fGray[ip4][jp4]) +
									fL_[29]*(fGray[im5][jm2] + fGray[im5][jp2] +
											fGray[ip5][jm2] + fGray[ip5][jp2] +
											fGray[im2][jm5] + fGray[im2][jp5] +
											fGray[ip2][jm5] + fGray[ip2][jp5]) +
									fL_[26]*(fGray[im5][jm1] + fGray[im5][jp1] +
											fGray[ip5][jm1] + fGray[ip5][jp1] +
											fGray[im1][jm5] + fGray[im1][jp5] +
											fGray[ip1][jm5] + fGray[ip1][jp5]) +
									fL_[25]*(fGray[im5][j] + fGray[ip5][j] +
											fGray[i][jm5] + fGray[i][jp5] +
											fGray[im4][jm3] + fGray[im4][jp3] +
											fGray[ip4][jm3] + fGray[ip4][jp3] +
											fGray[im3][jm4] + fGray[im3][jp4] +
											fGray[ip3][jm4] + fGray[ip3][jp4]) +
									fL_[20]*(fGray[im4][jm2] + fGray[im4][jp2] +
											fGray[ip4][jm2] + fGray[ip4][jp2] +
											fGray[im2][jm4] + fGray[im2][jp4] +
											fGray[ip2][jm4] + fGray[ip2][jp4]) +
									fL_[18]*(fGray[im3][jm3] + fGray[im3][jp3] +
											fGray[ip3][jm3] + fGray[ip3][jp3]) +
									fL_[17]*(fGray[im4][jm1] + fGray[im4][jp1] +
											fGray[ip4][jm1] + fGray[ip4][jp1] +
											fGray[im1][jm4] + fGray[im1][jp4] +
											fGray[ip1][jm4] + fGray[ip1][jp4]) +
									fL_[16]*(fGray[im4][j] + fGray[ip4][j] +
											fGray[i][jm4] + fGray[i][jp4]) +
									fL_[13]*(fGray[im3][jm2] + fGray[im3][jp2] +
											fGray[ip3][jm2] + fGray[ip3][jp2] +
											fGray[im2][jm3] + fGray[im2][jp3] +
											fGray[ip2][jm3] + fGray[ip2][jp3]) +
									fL_[10]*(fGray[im3][jm1] + fGray[im3][jp1] +
											fGray[ip3][jm1] + fGray[ip3][jp1] +
											fGray[im1][jm3] + fGray[im1][jp3] +
											fGray[ip1][jm3] + fGray[ip1][jp3]) +
									fL_[9]*(fGray[im3][j] + fGray[ip3][j] +
											fGray[i][jm3] + fGray[i][jp3]) +
									fL_[8]*(fGray[im2][jm2] + fGray[im2][jp2] +
											fGray[ip2][jm2] + fGray[ip2][jp2]) +
									fL_[5]*(fGray[im1][jm2] + fGray[im1][jp2] +
											fGray[ip1][jm2] + fGray[ip1][jp2] +
											fGray[im2][jm1] + fGray[im2][jp1] +
											fGray[ip2][jm1] + fGray[ip2][jp1]) +
									fL_[4]*(fGray[im2][j] + fGray[ip2][j] +
											fGray[i][jm2] + fGray[i][jp2]) +
									fL_[2]*(fGray[im1][jm1] + fGray[im1][jp1] +
											fGray[ip1][jm1] + fGray[ip1][jp1]) +
									fL_[1]*(fGray[i][jm1] + fGray[i][jp1] +
											fGray[im1][j] + fGray[ip1][j]) +
									fL_[0]* fGray[i][j];

					for (int l= kMaxHardCodedWidth+1; l<=m; l++)
						fOut[i][j] += fL_[l] * (fGray[i][j-l] + fGray[i][j+l] );

                    //  shift the j indexes (j itself is taken care of in the loop
                    //  statement)
                    jm6 = jm5;
                    jm5 = jm4;
                    jm4 = jm3;
                    jm3 = jm2;
                    jm2 = jm1;
                    jm1 = j;
                    j = jp1;
                    jp1 = jp2;
                    jp2 = jp3;
                    jp3 = jp4;
                    jp4 = jp5;
                    jp5 = jp6;
                    jp6++;
                }
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im6 = im5;
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
				i = ip1;
				ip1 = ip2;
				ip2 = ip3;
				ip3 = ip4;
				ip4 = ip5;
				ip5 = ip6;
				ip6++;
			}
        }
    }
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kNoCode,
        				"image type not suported by Gaussian filter yet");
    }
}


#if 0
#pragma mark -
#endif


void ImageLaplacian_Gaussian_F::initializeFilter_(void)
{
    const int   m = getWidth()/2;
    const double  sigma2 = scale_ * scale_,
//     		twoSigma2 = 2.L * sigma2, // unused variable -CJC
    		sigma2Scale = -1.L / sigma2,
            piSigma6 = -1.L / (2*PI * sigma2 * sigma2 * sigma2);

    fL_ = new float[2*m*m+1];
    FAIL_CONDITION( fL_ == NULL,
                    kFilterAllocationError,
                    "filter allocation failed");
    
	for (int i=0; i<=m; i++)
		for (int j=0; j<=i; j++)
		{
			int r2 = i*i + j*j;
			
//	    	fL_[r2] = (float) ((r2 - twoSigma2) * piSigma6 * exp(r2*sigma2Scale));
	    	fL_[r2] = (float) ((r2 - sigma2) * piSigma6 * exp(r2*sigma2Scale));
	    }

}


