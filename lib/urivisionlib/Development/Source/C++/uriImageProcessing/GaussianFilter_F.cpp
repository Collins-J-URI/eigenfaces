/*  NAME:
        GaussianFilter_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib GaussianFilter_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "GaussianFilter_F.h"

using namespace std;
using namespace uriVL;


#define	kMaxHardCodedWidth	13


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


GaussianFilter_F::GaussianFilter_F(double theScale)
		try	:	GaussianFilter(theScale, false),
				//
				fg_(NULL)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian filter is 0.8");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter_F constructor");
	throw e;
}


GaussianFilter_F::GaussianFilter_F(int theWidth)
		try	:	GaussianFilter(theWidth, false),
				//
				fg_(NULL)				
{
    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest width admissible for a Gaussian filter is 3");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter_F constructor");
	throw e;
}

			
GaussianFilter_F::GaussianFilter_F(double theScale, const ImageRect* theRect)
		try	:	GaussianFilter(theScale, theRect, false),
				//
				fg_(NULL)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian filter is 0.8");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter_F constructor");
	throw e;
}


GaussianFilter_F::GaussianFilter_F(int theWidth, const ImageRect* theRect)
		try	:	GaussianFilter(theWidth, theRect, false),
				//
				fg_(NULL)				
{
    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter_F constructor");
	throw e;
}

			
GaussianFilter_F::GaussianFilter_F(const GaussianFilter_F& theObj)
		try	:	GaussianFilter(theObj),
				//
				fg_(NULL)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "GaussianFilter_F copy constructor not implemented.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by GaussianFilter_F constructor");
	throw e;
}



GaussianFilter_F::~GaussianFilter_F(void)
{
	delete [] fg_;
}

const GaussianFilter_F& GaussianFilter_F::operator = (const GaussianFilter_F& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "GaussianFilter_F copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


RasterImage* GaussianFilter_F::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to GaussianFilter_F::applyTo");

    return applyTo(imgIn, imgIn->getValidRect());
}

RasterImage* GaussianFilter_F::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to GaussianFilter_F::applyTo");
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in GaussianFilter_F::applyTo");

	ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
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
            				kFunctionNotImplemented,
            				"image type not suported by Gaussian filter yet");
//            imgOut = new RasterImage_RGB_F(destRect);
            break;
            
        case kRGBa32Raster:
            FAIL_CONDITION(	true,
            				kFunctionNotImplemented,
            				"image type not suported by Gaussian filter yet");
//            imgOut = new RasterImage_RGBa_F(destRect);
            break;
            
        default:
            FAIL_CONDITION(	true,
            				kFunctionNotImplemented,
            				"image type not suported by Gaussian filter yet");
            break;
            
    }
    
    applyInto(imgIn, theRect, imgOut);

	delete destRect;
	delete usableRect;
	
	return imgOut;
}


void GaussianFilter_F::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to GaussianFilter_F::applyTo");

    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}


void GaussianFilter_F::applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut)
{
	//----------------------------------------------------------------
	//	STEP 0:		Test for null or invalid type parameters
	//----------------------------------------------------------------
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in GaussianFilter_F::applyInto");

    FAIL_CONDITION(	imgOut == NULL,
    				kNullRasterImageError,
    				"NULL output raster image passed as parameter");

    //  so far I only accept a RasterImage_gray_F as a destination
    
	FAIL_CONDITION( (imgOut->getBaseRasterType() != kGrayRaster) ||
                    (!imgOut->hasFloatRaster()),
	                kInvalidRasterType,
	                "GaussianFilter_F::applyInto only accepts RasterImage_gray_F images as destination");


	//----------------------------------------------------------------
	//	STEP 1:		Test for rectangle intersection
	//----------------------------------------------------------------
	//  determine the output rectangle that the input image can produce.
	//		Here, because we apply a filter of width&height (2m+1), we
	//		"lose m all around".
	ImageRect* usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	//	... and determine its intersection with the user-specified output rectangle
	ImageRect* destRect = ImageRect::intersection(usableRect, theRect);
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
    //	allocate a 2D raster for intermediate calculations if needed
    bool newTempRaster = allocateTempRaster_(imgIn->getValidRect());
	
    switch (getWidth())
    {
        case 3:
            applyInto_3x3_(grayImg, destRect, imgOut);
            break;

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
	delete destRect;
    if (newTempRaster)
		freeTempRaster_();

}


#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Hard-coded private methods that implement the convolution
//-------------------------------------------------------------------------------


void GaussianFilter_F::applyInto_3x3_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int	iLowM1 = iLow - 1, iHighP1 = iHigh + 1;
    int	    jm1, jp1;
    int	    im1, ip1;
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    float* const* fOut = static_cast<RasterImage_gray_F*>(imgOut)->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1;
            jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM1; i<=iHighP1; i++)
					fTemp[i][j] = 	fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
									fg_[0]* iGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm1 = j;
                jp1++;
            }
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1;
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
    				fOut[i][j] = 	fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
				
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im1 = i;
				ip1++;
			}
        }
        //  float
        else
        {
            const float* const* fGray = static_cast<const RasterImage_gray_F*>(imgIn)->getShiftedGrayF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1;
            jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM1; i<=iHighP1; i++)
					fTemp[i][j] = 	fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
									fg_[0]* fGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm1 = j;
                jp1++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; 
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im1 = i;
				ip1++;
			}
        }
    }
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}

void GaussianFilter_F::applyInto_5x5_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	iLowM2 = iLow - 2, iHighP2 = iHigh + 2;
    int	jm1, jm2, jp1, jp2;
    int	im1, im2, ip1, ip2;
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    float* const* fOut = static_cast<RasterImage_gray_F*>(imgOut)->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2;
            jp1 = jLow+1; jp2 = jLow+2;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM2; i<=iHighP2; i++)
					fTemp[i][j] = 	fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
									fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
									fg_[0]* iGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2;
    		ip1 = iLow+1; ip2 = iLow+2;
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
									fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im2 = im1;
				im1 = i;
				ip1 = ip2;
				ip2++;
			}
        }
        //  float
        else
        {
            const float* const* fGray = static_cast<const RasterImage_gray_F*>(imgIn)->getShiftedGrayF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2;
            jp1 = jLow+1; jp2 = jLow+2;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM2; i<=iHighP2; i++)
					fTemp[i][j] = 	fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
									fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
									fg_[0]* fGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2;
    		ip1 = iLow+1; ip2 = iLow+2;
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
									fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im2 = im1;
				im1 = i;
				ip1 = ip2;
				ip2++;
			}
        }
    }
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}

void GaussianFilter_F::applyInto_7x7_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	iLowM3 = iLow - 3, iHighP3 = iHigh + 3;
    int	jm1, jm2, jm3, jp1, jp2, jp3;
    int	im1, im2, im3, ip1, ip2, ip3;
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    float* const* fOut = static_cast<RasterImage_gray_F*>(imgOut)->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 

            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM3; i<=iHighP3; i++)
					fTemp[i][j] = 	fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
									fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
									fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
									fg_[0]* iGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
									fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
									fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im3 = im2;
				im2 = im1;
				im1 = i;
				ip1 = ip2;
				ip2 = ip3;
				ip3++;
			}
        }
        //  float
        else
        {
            const float* const* fGray = static_cast<const RasterImage_gray_F*>(imgIn)->getShiftedGrayF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 

            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM3; i<=iHighP3; i++)
					fTemp[i][j] = 	fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
									fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
									fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
									fg_[0]* fGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
									fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
									fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im3 = im2;
				im2 = im1;
				im1 = i;
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
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}

void GaussianFilter_F::applyInto_9x9_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	iLowM4 = iLow - 4, iHighP4 = iHigh + 4;
    int	jm1, jm2, jm3, jm4, jp1, jp2, jp3, jp4;
    int	im1, im2, im3, im4, ip1, ip2, ip3, ip4;
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    float* const* fOut = static_cast<RasterImage_gray_F*>(imgOut)->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jm4 = jLow-4;
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
            jp4 = jLow+4;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM4; i<=iHighP4; i++)
					fTemp[i][j] = 	fg_[4]*(iGray[i][jm4] + iGray[i][jp4]) +
									fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
									fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
									fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
									fg_[0]* iGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3 = jp4;
                jp4++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4;
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
									fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
									fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
									fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
				ip1 = ip2;
				ip2 = ip3;
				ip3 = ip4;
				ip4++;
			}
        }
        //  float
        else
        {
            const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jm4 = jLow-4;
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
            jp4 = jLow+4;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM4; i<=iHighP4; i++)
					fTemp[i][j] = 	fg_[4]*(fGray[i][jm4] + fGray[i][jp4]) +
									fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
									fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
									fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
									fg_[0]* fGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3 = jp4;
                jp4++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4;
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
									fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
									fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
									fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
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
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}

void GaussianFilter_F::applyInto_11x11_(const RasterImage* imgIn, const ImageRect* theRect,
                                                RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	iLowM5 = iLow - 5, iHighP5 = iHigh + 5;
    int	jm1, jm2, jm3, jm4, jm5, jp1, jp2, jp3, jp4, jp5;
    int	im1, im2, im3, im4, im5, ip1, ip2, ip3, ip4, ip5;
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    float* const* fOut = static_cast<RasterImage_gray_F*>(imgOut)->getShiftedGrayF2D(R_W_ACCESS);

    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jm4 = jLow-4; jm5 = jLow-5;
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
            jp4 = jLow+4; jp5 = jLow+5;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM5; i<=iHighP5; i++)
					fTemp[i][j] = 	fg_[5]*(iGray[i][jm5] + iGray[i][jp5]) +
									fg_[4]*(iGray[i][jm4] + iGray[i][jp4]) +
									fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
									fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
									fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
									fg_[0]* iGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm5 = jm4;
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3 = jp4;
                jp4 = jp5;
                jp5++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5;
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[5]*(fTemp[im5][j] + fTemp[ip5][j]) +
									fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
									fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
									fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
									fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
                 
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
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
            const float* const* fGray = static_cast<const RasterImage_gray_F*>(imgIn)->getShiftedGrayF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jm4 = jLow-4; jm5 = jLow-5;
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
            jp4 = jLow+4; jp5 = jLow+5;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM5; i<=iHighP5; i++)
					fTemp[i][j] = 	fg_[5]*(fGray[i][jm5] + fGray[i][jp5]) +
									fg_[4]*(fGray[i][jm4] + fGray[i][jp4]) +
									fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
									fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
									fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
									fg_[0]* fGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm5 = jm4;
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3 = jp4;
                jp4 = jp5;
                jp5++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5;
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[5]*(fGray[im5][j] + fGray[ip5][j]) +
									fg_[4]*(fGray[im4][j] + fGray[ip4][j]) +
									fg_[3]*(fGray[im3][j] + fGray[ip3][j]) +
									fg_[2]*(fGray[im2][j] + fGray[ip2][j]) +
									fg_[1]*(fGray[im1][j] + fGray[ip1][j]) +
									fg_[0]* fGray[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
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
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}

void GaussianFilter_F::applyInto_13x13_(const RasterImage* imgIn, const ImageRect* theRect,
										RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int	iLowM6 = iLow - 6, iHighP6 = iHigh + 6;
    int	jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    int	im1, im2, im3, im4, im5, im6, ip1, ip2, ip3, ip4, ip5, ip6;
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    float* const* fOut = static_cast<RasterImage_gray_F*>(imgOut)->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
            jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM6; i<=iHighP6; i++)
					fTemp[i][j] = 	fg_[6]*(iGray[i][jm6] + iGray[i][jp6]) +
									fg_[5]*(iGray[i][jm5] + iGray[i][jp5]) +
									fg_[4]*(iGray[i][jm4] + iGray[i][jp4]) +
									fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
									fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
									fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
									fg_[0]* iGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm6 = jm5;
                jm5 = jm4;
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3 = jp4;
                jp4 = jp5;
                jp5 = jp6;
                jp6++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[6]*(fTemp[im6][j] + fTemp[ip6][j]) +
									fg_[5]*(fTemp[im5][j] + fTemp[ip5][j]) +
									fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
									fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
									fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
									fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
									fg_[0]* fTemp[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im6 = im5;
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
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
            const float* const* fGray = static_cast<const RasterImage_gray_F*>(imgIn)->getShiftedGrayF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
            jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM6; i<=iHighP6; i++)
					fTemp[i][j] = 	fg_[6]*(fGray[i][jm6] + fGray[i][jp6]) +
									fg_[5]*(fGray[i][jm5] + fGray[i][jp5]) +
									fg_[4]*(fGray[i][jm4] + fGray[i][jp4]) +
									fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
									fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
									fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
									fg_[0]* fGray[i][j];
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm6 = jm5;
                jm5 = jm4;
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3 = jp4;
                jp4 = jp5;
                jp5 = jp6;
                jp6++;
			}
				
			//-------------------------------------------------------
			//	then apply the 1D filter along the y direction
			//-------------------------------------------------------
            im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
    		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
    		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
    		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
					fOut[i][j] = 	fg_[6]*(fGray[im6][j] + fGray[ip6][j]) +
									fg_[5]*(fGray[im5][j] + fGray[ip5][j]) +
									fg_[4]*(fGray[im4][j] + fGray[ip4][j]) +
									fg_[3]*(fGray[im3][j] + fGray[ip3][j]) +
									fg_[2]*(fGray[im2][j] + fGray[ip2][j]) +
									fg_[1]*(fGray[im1][j] + fGray[ip1][j]) +
									fg_[0]* fGray[i][j];
                
				//  shift the i indexes (i itself is taken care of in the loop
				//  statement)
				im6 = im5;
				im5 = im4;
				im4 = im3;
				im3 = im2;
				im2 = im1;
				im1 = i;
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
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}


void GaussianFilter_F::applyInto_(const RasterImage* imgIn, const ImageRect* theRect,
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
    const int	iLowMm = iLow - m, iHighPm = iHigh + m;
    //
    int     jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    int     im1, im2, im3, im4, im5, im6, ip1, ip2, ip3, ip4, ip5, ip6;
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    float* const* fOut = static_cast<RasterImage_gray_F*>(imgOut)->getShiftedGrayF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kGrayRaster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
            jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowMm; i<=iHighPm; i++)
	            {
					fTemp[i][j] = 	fg_[6]*(iGray[i][jm6] + iGray[i][jp6]) +
									fg_[5]*(iGray[i][jm5] + iGray[i][jp5]) +
									fg_[4]*(iGray[i][jm4] + iGray[i][jp4]) +
									fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
									fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
									fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
									fg_[0]* iGray[i][j];
						
					for (int l= kMaxHardCodedWidth+1; l<=m; l++)
						fTemp[i][j] += fg_[l] * (iGray[i][j-l] + iGray[i][j+l]);

                }

                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm6 = jm5;
                jm5 = jm4;
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3 = jp4;
                jp4 = jp5;
                jp5 = jp6;
                jp6++;
			}
        }
        //  float
        else
        {
            const float* const* fGray = static_cast<const RasterImage_gray_F*>(imgIn)->getShiftedGrayF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
            jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
            jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
            jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowMm; i<=iHighPm; i++)
	            {
					fTemp[i][j] = 	fg_[6]*(fGray[i][jm6] + fGray[i][jp6]) +
									fg_[5]*(fGray[i][jm5] + fGray[i][jp5]) +
									fg_[4]*(fGray[i][jm4] + fGray[i][jp4]) +
									fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
									fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
									fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
									fg_[0]* fGray[i][j];
						
					for (int l= kMaxHardCodedWidth+1; l<=m; l++)
						fTemp[i][j] += fg_[l] * (fGray[i][j-l] + fGray[i][j+l] );

                }

                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm6 = jm5;
                jm5 = jm4;
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1 = jp2;
                jp2 = jp3;
                jp3 = jp4;
                jp4 = jp5;
                jp5 = jp6;
                jp6++;
			}
				
        }

            
        //-------------------------------------------------------
        //	then apply the 1D filter along the y direction
        //-------------------------------------------------------
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
        im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
        ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
        ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                fOut[i][j] = 	fg_[6]*(fTemp[im6][j] + fTemp[ip6][j]) +
                                fg_[5]*(fTemp[im5][j] + fTemp[ip5][j]) +
                                fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
                                fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
                                fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
                                fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
                                fg_[0]* fTemp[i][j];

                for (int k= kMaxHardCodedWidth+1; k<=m; k++)
                    fOut[i][j] += fg_[k] * (fTemp[i-k][j] + fTemp[i+k][j] );

            }
            
            //  shift the i indexes (i itself is taken care of in the loop
            //  statement)
            im6 = im5;
            im5 = im4;
            im4 = im3;
            im3 = im2;
            im2 = im1;
            im1 = i;
            ip1 = ip2;
            ip2 = ip3;
            ip3 = ip4;
            ip4 = ip5;
            ip5 = ip6;
            ip6++;
        }
    }
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Initialization
//------------------------------------------------------
#endif



void GaussianFilter_F::initializeFilter_(void)
{
    const int   m = getWidth()/2;
	const double scale = getScale();
    const double sigma2Scale = 1.L / (2.L * scale * scale),
				 piSigma = 1.L / (sqrt(2*PI) * scale);

    fg_ = new float[m+1];
    FAIL_CONDITION( fg_ == NULL,
                    kFilterAllocationError,
                    "filter allocation failed");
    
    for (int r=0; r<=m; r++)
    	fg_[r] = static_cast<float>(piSigma * exp(-r*r*sigma2Scale));

}
