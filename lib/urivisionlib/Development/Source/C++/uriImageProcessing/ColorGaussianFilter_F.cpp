/*  NAME:
        ColorGaussianFilter_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorGaussianFilter_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "ColorGaussianFilter_F.h"

using namespace std;
using namespace uriVL;


#define	kMaxHardCodedWidth	13

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ColorGaussianFilter_F::ColorGaussianFilter_F(double theScale)
		try	:	ColorGaussianFilter(theScale, false),
				//
				fg_(NULL)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian filter is 0.8");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_F constructor");
	throw e;
}


ColorGaussianFilter_F::ColorGaussianFilter_F(int theWidth)
		try	:	ColorGaussianFilter(theWidth, false),
				//
				fg_(NULL)				
{
    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest width admissible for a Gaussian filter is 3");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_F constructor");
	throw e;
}

			
ColorGaussianFilter_F::ColorGaussianFilter_F(double theScale, const ImageRect* theRect)
		try	:	ColorGaussianFilter(theScale, theRect, false),
				//
				fg_(NULL)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian filter is 0.8");

    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_F constructor");
	throw e;
}


ColorGaussianFilter_F::ColorGaussianFilter_F(int theWidth, const ImageRect* theRect)
		try	:	ColorGaussianFilter(theWidth, theRect, false),
				//
				fg_(NULL)				
{
    initializeFilter_( );
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_F constructor");
	throw e;
}

			
ColorGaussianFilter_F::ColorGaussianFilter_F(const ColorGaussianFilter_F& theObj)
		try:	ColorGaussianFilter(theObj),
				//
				fg_(NULL)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorGaussianFilter_F copy constructor not implemented.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorGaussianFilter_F constructor");
	throw e;
}
			

ColorGaussianFilter_F::~ColorGaussianFilter_F(void)
{
	delete [] fg_;
}



const ColorGaussianFilter_F& ColorGaussianFilter_F::operator = (const ColorGaussianFilter_F& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorGaussianFilter_F copy operator not implemented.");

	return *this;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


RasterImage* ColorGaussianFilter_F::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    return applyTo(imgIn, imgIn->getValidRect());
}

RasterImage* ColorGaussianFilter_F::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to ColorGaussianFilter_F::applyTo");
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in ColorGaussianFilter_F::applyTo");

	ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle in ColorGaussianFilter_F::applyTo");
	
	RasterImage	*imgOut = NULL;
    switch (imgIn->getBaseRasterType())
    {
        case kGrayRaster:
            FAIL_CONDITION(	true,
            				kFunctionNotImplemented,
            				"image type not suported by color Gaussian filter yet");
            break;
            
        case kRGB24Raster:
            FAIL_CONDITION(	true,
            				kFunctionNotImplemented,
            				"image type not suported by color Gaussian filter yet");
            break;
            
        case kRGBa32Raster:
            imgOut = new RasterImage_RGBa_F(destRect);
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


void ColorGaussianFilter_F::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to ColorGaussianFilter_F::applyInto");

    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}


void ColorGaussianFilter_F::applyInto(const RasterImage* imgIn, const ImageRect* theRect, 
                                      RasterImage* imgOut)
{
	//----------------------------------------------------------------
	//	STEP 0:		Test for null or invalid type parameters
	//----------------------------------------------------------------
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to ColorGaussianFilter_F::applyInto");

    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in ColorGaussianFilter_F::applyInto");

    FAIL_CONDITION(	imgOut == NULL,
    				kNullRasterImageError,
    				"NULL output raster image passed as parameter to ColorGaussianFilter_F::applyInto");

    //  so far I only accept a RasterImage_RGBa_F as a destination    
	FAIL_CONDITION( (imgOut->getBaseRasterType() != kRGBa32Raster) ||
                    (!imgOut->hasFloatRaster()),
	                kInvalidRasterType,
	                "ColorGaussianFilter_F::applyInto only accepts RasterImage_RGBa_F images as destination");


	//----------------------------------------------------------------
	//	STEP 1:		Test for rectangle intersection
	//----------------------------------------------------------------
	//  determine the output rectangle that the input image can produce.
	//		Here, because we apply a filter of width&height (2m+1), we
	//		"lose m all around".
	ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	//	... and determine its intersection with the user-specified output rectangle
	ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
	delete usableRect;
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle in ColorGaussianFilter_F::applyInto");
    //  if the output image does not contain the destination rect, it must be resized.
    if (!imgOut->contains(destRect))
    	imgOut->setBoundRect(destRect);
	

	//----------------------------------------------------------------
	//	STEP 2:		Operate only on an RGBa image
	//----------------------------------------------------------------
    bool                localRGBa;
	const RasterImage_RGBa* rgbaImg = RasterImage_RGBa::localRasterRGBa(imgIn, &localRGBa);


	//----------------------------------------------------------------
	//	STEP 3:		Perform the calculations
	//----------------------------------------------------------------
    //	allocate a 2D raster for intermediate calculations if needed
    bool newTempRaster = allocateTempRaster_(imgIn->getValidRect());

    switch (getWidth())
    {
        case 3:
            applyInto_3x3_(rgbaImg, destRect, imgOut);
            break;

        case 5:
            applyInto_5x5_(rgbaImg, destRect, imgOut);
            break;

        case 7:
            applyInto_7x7_(rgbaImg, destRect, imgOut);
            break;

        case 9:
            applyInto_9x9_(rgbaImg, destRect, imgOut);
            break;

        case 11:
            applyInto_11x11_(rgbaImg, destRect, imgOut);
            break;

        case 13:
            applyInto_13x13_(rgbaImg, destRect, imgOut);
            break;

        default:
            applyInto_(rgbaImg, destRect, imgOut);
            break;
    }

	//----------------------------------------------------------------
	//	STEP 4:		Set rectangle
	//----------------------------------------------------------------
	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
	imgOut->setValidRect(destRect);
	(static_cast<RasterImage_RGBa_F*>(imgOut))->setFloatConversionMode(getFloatConversionMode());

	//----------------------------------------------------------------
	//	STEP 5:		Free temporary storage (if needed)
	//----------------------------------------------------------------
	if (localRGBa)
	    delete rgbaImg;
	delete destRect;
    if (newTempRaster)
		freeTempRaster_();

}


#if 0
#pragma mark -
//-------------------------------------------------------------------------------
#pragma mark Hard-coded private methods that implement the convolution
//-------------------------------------------------------------------------------
#endif


void ColorGaussianFilter_F::applyInto_3x3_(const RasterImage* imgIn, const ImageRect* theRect,
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
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    float* const* redOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedRedF2D(R_W_ACCESS);
    float* const* greenOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedGreenF2D(R_W_ACCESS);
    float* const* blueOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedBlueF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kRGBa32Raster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iRGBa = imgIn->getShiftedRaster2D();
            int fourjR, fourjG, fourjB,         //  4*j     red green blue
                fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
                fourjp1R, fourjp1G, fourjp1B;   //  4*(j+1) red green blue

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            fourjm1R = 4*(jLow-1);
            fourjm1G = fourjm1R + 1;
            fourjm1B = fourjm1G + 1;
            fourjR   = 4*jLow;
            fourjG   = fourjR + 1;
            fourjB   = fourjG + 1;
            fourjp1R = 4*(jLow+1);
            fourjp1G = fourjp1R + 1;
            fourjp1B = fourjp1G + 1;
            //
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM1; i<=iHighP1; i++)
	            {
					redTemp[i][j] = fg_[1]*(iRGBa[i][fourjm1R] + iRGBa[i][fourjp1R]) +
                                        fg_[0]*iRGBa[i][fourjR];
					greenTemp[i][j] = fg_[1]*(iRGBa[i][fourjm1G] + iRGBa[i][fourjp1G]) +
                                        fg_[0]*iRGBa[i][fourjG];
					blueTemp[i][j] = fg_[1]*(iRGBa[i][fourjm1B] + iRGBa[i][fourjp1B]) +
                                        fg_[0]*iRGBa[i][fourjB];
				}
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                fourjm1R = fourjR;
                fourjm1G = fourjG;
                fourjm1B = fourjB;
                fourjR = fourjp1R;
                fourjG = fourjp1G;
                fourjB = fourjp1B;
                fourjp1R += 4;
                fourjp1G += 4;
                fourjp1B += 4;
            }
        }
        //  float
        else
        {
            const float* const* redIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedRedF2D();
            const float* const* greenIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedGreenF2D();
            const float* const* blueIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedBlueF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            int jm1 = jLow-1;
            int jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM1; i<=iHighP1; i++)
	            {
					redTemp[i][j] =     fg_[1]*(redIn[i][jm1] + redIn[i][jp1]) +
									    fg_[0]*redIn[i][j];
					greenTemp[i][j] =   fg_[1]*(greenIn[i][jm1] + greenIn[i][jp1]) +
									    fg_[0]*greenIn[i][j];
					blueTemp[i][j] = 	fg_[1]*(blueIn[i][jm1] + blueIn[i][jp1]) +
									    fg_[0]*blueIn[i][j];
				}
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm1 = j;
                jp1++;
			}

        }
        
        //-------------------------------------------------------
        // Then apply the 1D filter along the y direction
        //-------------------------------------------------------
        int im1 = iLow-1;
        int ip1 = iLow+1;
        //
        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                redOut[i][j] =      fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
                                    fg_[0]*redTemp[i][j];
            
                greenOut[i][j] =    fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
                                    fg_[0]*greenTemp[i][j];
            
                blueOut[i][j] =     fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
                                    fg_[0]*blueTemp[i][j];
            }
            //  shift the i indexes (i itself is taken care of in the loop
            //  statement)
            im1 = i;
            ip1++;
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

void ColorGaussianFilter_F::applyInto_5x5_(const RasterImage* imgIn, const ImageRect* theRect,
											RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int	iLowM2 = iLow - 2, iHighP2 = iHigh + 2;
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    float* const* redOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedRedF2D(R_W_ACCESS);
    float* const* greenOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedGreenF2D(R_W_ACCESS);
    float* const* blueOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedBlueF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kRGBa32Raster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iRGBa = imgIn->getShiftedRaster2D();
            int fourjR, fourjG, fourjB,         //  4*j     red green blue
                fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
                fourjp1R, fourjp1G, fourjp1B,   //  4*(j+1) red green blue
                fourjm2R, fourjm2G, fourjm2B,   //  4*(j-2) red green blue
                fourjp2R, fourjp2G, fourjp2B;   //  4*(j+2) red green blue

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            fourjm2R = 4*(jLow-2);
            fourjm2G = fourjm2R + 1;
            fourjm2B = fourjm2G + 1;
            fourjm1R = 4*(jLow-1);
            fourjm1G = fourjm1R + 1;
            fourjm1B = fourjm1G + 1;
            fourjR   = 4*jLow;
            fourjG   = fourjR + 1;
            fourjB   = fourjG + 1;
            fourjp1R = 4*(jLow+1);
            fourjp1G = fourjp1R + 1;
            fourjp1B = fourjp1G + 1;
            fourjp2R = 4*(jLow+2);
            fourjp2G = fourjp2R + 1;
            fourjp2B = fourjp2G + 1;
            //
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM2; i<=iHighP2; i++)
	            {
					redTemp[i][j] = fg_[2]*(iRGBa[i][fourjm2R] + iRGBa[i][fourjp2R]) +
                                    fg_[1]*(iRGBa[i][fourjm1R] + iRGBa[i][fourjp1R]) +
                                        fg_[0]*iRGBa[i][fourjR];
					greenTemp[i][j] = fg_[2]*(iRGBa[i][fourjm2G] + iRGBa[i][fourjp2G]) +
                                        fg_[1]*(iRGBa[i][fourjm1G] + iRGBa[i][fourjp1G]) +
                                        fg_[0]*iRGBa[i][fourjG];
					blueTemp[i][j] = fg_[2]*(iRGBa[i][fourjm2B] + iRGBa[i][fourjp2B]) +
                                        fg_[1]*(iRGBa[i][fourjm1B] + iRGBa[i][fourjp1B]) +
                                        fg_[0]*iRGBa[i][fourjB];
				}
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                fourjm2R = fourjm1R;
                fourjm2G = fourjm1G;
                fourjm2B = fourjm1B;
                fourjm1R = fourjR;
                fourjm1G = fourjG;
                fourjm1B = fourjB;
                fourjR = fourjp1R;
                fourjG = fourjp1G;
                fourjB = fourjp1B;
                fourjp1R = fourjp2R;
                fourjp1G = fourjp2G;
                fourjp1B = fourjp2B;
                fourjp2R += 4;
                fourjp2G += 4;
                fourjp2B += 4;
            }
        }
        //  float
        else
        {
            const float* const* redIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedRedF2D();
            const float* const* greenIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedGreenF2D();
            const float* const* blueIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedBlueF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            int jm2=jLow-2, jm1 = jLow-1;
            int jp2=jLow+2, jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM2; i<=iHighP2; i++)
	            {
					redTemp[i][j] =     fg_[2]*(redIn[i][jm2] + redIn[i][jp2]) +
									    fg_[1]*(redIn[i][jm1] + redIn[i][jp1]) +
									    fg_[0]*redIn[i][j];
					greenTemp[i][j] =   fg_[2]*(greenIn[i][jm2] + greenIn[i][jp2]) +
									    fg_[1]*(greenIn[i][jm1] + greenIn[i][jp1]) +
									    fg_[0]*greenIn[i][j];
					blueTemp[i][j] = 	fg_[2]*(blueIn[i][jm2] + blueIn[i][jp2]) +
									    fg_[1]*(blueIn[i][jm1] + blueIn[i][jp1]) +
									    fg_[0]*blueIn[i][j];
				}
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm2 = jm1;
                jm1 = j;
                jp1=jp2;
                jp2++;
			}

        }
        
        //-------------------------------------------------------
        // Then apply the 1D filter along the y direction
        //-------------------------------------------------------
        int im2 = iLow-2, im1 = iLow-1;
        int ip2 = iLow+2, ip1 = iLow+1;
        //
        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                redOut[i][j] =      fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
                                    fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
                                    fg_[0]*redTemp[i][j];
            
                greenOut[i][j] =    fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
                                    fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
                                    fg_[0]*greenTemp[i][j];
            
                blueOut[i][j] =     fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
                                    fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
                                    fg_[0]*blueTemp[i][j];
            }
            //  shift the i indexes (i itself is taken care of in the loop
            //  statement)
            im2 = im1;
            im1 = i;
            ip1 = ip2;
            ip2++;
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

void ColorGaussianFilter_F::applyInto_7x7_(const RasterImage* imgIn, const ImageRect* theRect,
                                           RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int	iLowM3 = iLow - 3, iHighP3 = iHigh + 3;
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    float* const* redOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedRedF2D(R_W_ACCESS);
    float* const* greenOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedGreenF2D(R_W_ACCESS);
    float* const* blueOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedBlueF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kRGBa32Raster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iRGBa = imgIn->getShiftedRaster2D();
            int fourjR, fourjG, fourjB,         //  4*j     red green blue
                fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
                fourjp1R, fourjp1G, fourjp1B,   //  4*(j+1) red green blue
                fourjm2R, fourjm2G, fourjm2B,   //  4*(j-2) red green blue
                fourjp2R, fourjp2G, fourjp2B,   //  4*(j+2) red green blue
                fourjm3R, fourjm3G, fourjm3B,   //  4*(j-3) red green blue
                fourjp3R, fourjp3G, fourjp3B;   //  4*(j+3) red green blue

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            fourjm3R = 4*(jLow-3);
            fourjm3G = fourjm3R + 1;
            fourjm3B = fourjm3G + 1;
            fourjm2R = 4*(jLow-2);
            fourjm2G = fourjm2R + 1;
            fourjm2B = fourjm2G + 1;
            fourjm1R = 4*(jLow-1);
            fourjm1G = fourjm1R + 1;
            fourjm1B = fourjm1G + 1;
            fourjR   = 4*jLow;
            fourjG   = fourjR + 1;
            fourjB   = fourjG + 1;
            fourjp1R = 4*(jLow+1);
            fourjp1G = fourjp1R + 1;
            fourjp1B = fourjp1G + 1;
            fourjp2R = 4*(jLow+2);
            fourjp2G = fourjp2R + 1;
            fourjp2B = fourjp2G + 1;
            fourjp3R = 4*(jLow+3);
            fourjp3G = fourjp3R + 1;
            fourjp3B = fourjp3G + 1;
            //
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM3; i<=iHighP3; i++)
	            {
					redTemp[i][j] = fg_[3]*(iRGBa[i][fourjm3R] + iRGBa[i][fourjp3R]) +
                                    fg_[2]*(iRGBa[i][fourjm2R] + iRGBa[i][fourjp2R]) +
                                    fg_[1]*(iRGBa[i][fourjm1R] + iRGBa[i][fourjp1R]) +
                                        fg_[0]*iRGBa[i][fourjR];
					greenTemp[i][j] = fg_[3]*(iRGBa[i][fourjm3G] + iRGBa[i][fourjp3G]) +
                                        fg_[2]*(iRGBa[i][fourjm2G] + iRGBa[i][fourjp2G]) +
                                        fg_[1]*(iRGBa[i][fourjm1G] + iRGBa[i][fourjp1G]) +
                                        fg_[0]*iRGBa[i][fourjG];
					blueTemp[i][j] = fg_[3]*(iRGBa[i][fourjm3B] + iRGBa[i][fourjp3B]) +
                                        fg_[2]*(iRGBa[i][fourjm2B] + iRGBa[i][fourjp2B]) +
                                        fg_[1]*(iRGBa[i][fourjm1B] + iRGBa[i][fourjp1B]) +
                                        fg_[0]*iRGBa[i][fourjB];
				}
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                fourjm3R = fourjm2R;
                fourjm3G = fourjm2G;
                fourjm3B = fourjm2B;
                fourjm2R = fourjm1R;
                fourjm2G = fourjm1G;
                fourjm2B = fourjm1B;
                fourjm1R = fourjR;
                fourjm1G = fourjG;
                fourjm1B = fourjB;
                fourjR = fourjp1R;
                fourjG = fourjp1G;
                fourjB = fourjp1B;
                fourjp1R = fourjp2R;
                fourjp1G = fourjp2G;
                fourjp1B = fourjp2B;
                fourjp2R = fourjp3R;
                fourjp2G = fourjp3G;
                fourjp2B = fourjp3B;
                fourjp3R += 4;
                fourjp3G += 4;
                fourjp3B += 4;
            }
        }
        //  float
        else
        {
            const float* const* redIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedRedF2D();
            const float* const* greenIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedGreenF2D();
            const float* const* blueIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedBlueF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            int jm3 = jLow-3, jm2=jLow-2, jm1 = jLow-1;
            int jp3 = jLow+3, jp2=jLow+2, jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM3; i<=iHighP3; i++)
	            {
					redTemp[i][j] =     fg_[3]*(redIn[i][jm3] + redIn[i][jp3]) +
									    fg_[2]*(redIn[i][jm2] + redIn[i][jp2]) +
									    fg_[1]*(redIn[i][jm1] + redIn[i][jp1]) +
									    fg_[0]* redIn[i][j];
					greenTemp[i][j] =   fg_[3]*(greenIn[i][jm3] + greenIn[i][jp3]) +
									    fg_[2]*(greenIn[i][jm2] + greenIn[i][jp2]) +
									    fg_[1]*(greenIn[i][jm1] + greenIn[i][jp1]) +
									    fg_[0]* greenIn[i][j];
					blueTemp[i][j] = 	fg_[3]*(blueIn[i][jm3] + blueIn[i][jp3]) +
									    fg_[2]*(blueIn[i][jm2] + blueIn[i][jp2]) +
									    fg_[1]*(blueIn[i][jm1] + blueIn[i][jp1]) +
									    fg_[0]*blueIn[i][j];
				}
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1=jp2;
                jp2=jp3;
                jp3++;
			}

        }
        
        //-------------------------------------------------------
        // Then apply the 1D filter along the y direction
        //-------------------------------------------------------
        int im3 = iLow-3, im2 = iLow-2, im1 = iLow-1;
        int ip3 = iLow+3, ip2 = iLow+2, ip1 = iLow+1;
        //
        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                redOut[i][j] =      fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
                                    fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
                                    fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
                                    fg_[0]*redTemp[i][j];
            
                greenOut[i][j] =    fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
                                    fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
                                    fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
                                    fg_[0]*greenTemp[i][j];
            
                blueOut[i][j] =     fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
                                    fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
                                    fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
                                    fg_[0]*blueTemp[i][j];
            }
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
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}

void ColorGaussianFilter_F::applyInto_9x9_(const RasterImage* imgIn, const ImageRect* theRect,
                                           RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int	iLowM4 = iLow - 4, iHighP4 = iHigh + 4;
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    float* const* redOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedRedF2D(R_W_ACCESS);
    float* const* greenOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedGreenF2D(R_W_ACCESS);
    float* const* blueOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedBlueF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kRGBa32Raster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iRGBa = imgIn->getShiftedRaster2D();
            int fourjR, fourjG, fourjB,         //  4*j     red green blue
                fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
                fourjp1R, fourjp1G, fourjp1B,   //  4*(j+1) red green blue
                fourjm2R, fourjm2G, fourjm2B,   //  4*(j-2) red green blue
                fourjp2R, fourjp2G, fourjp2B,   //  4*(j+2) red green blue
                fourjm3R, fourjm3G, fourjm3B,   //  4*(j-3) red green blue
                fourjp3R, fourjp3G, fourjp3B,   //  4*(j+3) red green blue
                fourjm4R, fourjm4G, fourjm4B,   //  4*(j-4) red green blue
                fourjp4R, fourjp4G, fourjp4B;   //  4*(j+4) red green blue

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            fourjm4R = 4*(jLow-4);
            fourjm4G = fourjm4R + 1;
            fourjm4B = fourjm4G + 1;
            fourjm3R = 4*(jLow-3);
            fourjm3G = fourjm3R + 1;
            fourjm3B = fourjm3G + 1;
            fourjm2R = 4*(jLow-2);
            fourjm2G = fourjm2R + 1;
            fourjm2B = fourjm2G + 1;
            fourjm1R = 4*(jLow-1);
            fourjm1G = fourjm1R + 1;
            fourjm1B = fourjm1G + 1;
            fourjR   = 4*jLow;
            fourjG   = fourjR + 1;
            fourjB   = fourjG + 1;
            fourjp1R = 4*(jLow+1);
            fourjp1G = fourjp1R + 1;
            fourjp1B = fourjp1G + 1;
            fourjp2R = 4*(jLow+2);
            fourjp2G = fourjp2R + 1;
            fourjp2B = fourjp2G + 1;
            fourjp3R = 4*(jLow+3);
            fourjp3G = fourjp3R + 1;
            fourjp3B = fourjp3G + 1;
            fourjp4R = 4*(jLow+4);
            fourjp4G = fourjp4R + 1;
            fourjp4B = fourjp4G + 1;
            //
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM4; i<=iHighP4; i++)
	            {
					redTemp[i][j] = fg_[4]*(iRGBa[i][fourjm4R] + iRGBa[i][fourjp4R]) +
                                    fg_[3]*(iRGBa[i][fourjm3R] + iRGBa[i][fourjp3R]) +
                                    fg_[2]*(iRGBa[i][fourjm2R] + iRGBa[i][fourjp2R]) +
                                    fg_[1]*(iRGBa[i][fourjm1R] + iRGBa[i][fourjp1R]) +
                                        fg_[0]*iRGBa[i][fourjR];
					greenTemp[i][j] = fg_[4]*(iRGBa[i][fourjm4G] + iRGBa[i][fourjp4G]) +
                                        fg_[3]*(iRGBa[i][fourjm3G] + iRGBa[i][fourjp3G]) +
                                        fg_[2]*(iRGBa[i][fourjm2G] + iRGBa[i][fourjp2G]) +
                                        fg_[1]*(iRGBa[i][fourjm1G] + iRGBa[i][fourjp1G]) +
                                        fg_[0]*iRGBa[i][fourjG];
					blueTemp[i][j] = fg_[4]*(iRGBa[i][fourjm4B] + iRGBa[i][fourjp4B]) +
                                        fg_[3]*(iRGBa[i][fourjm3B] + iRGBa[i][fourjp3B]) +
                                        fg_[2]*(iRGBa[i][fourjm2B] + iRGBa[i][fourjp2B]) +
                                        fg_[1]*(iRGBa[i][fourjm1B] + iRGBa[i][fourjp1B]) +
                                        fg_[0]*iRGBa[i][fourjB];
				}
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                fourjm4R = fourjm3R;
                fourjm4G = fourjm3G;
                fourjm4B = fourjm3B;
                fourjm3R = fourjm2R;
                fourjm3G = fourjm2G;
                fourjm3B = fourjm2B;
                fourjm2R = fourjm1R;
                fourjm2G = fourjm1G;
                fourjm2B = fourjm1B;
                fourjm1R = fourjR;
                fourjm1G = fourjG;
                fourjm1B = fourjB;
                fourjR = fourjp1R;
                fourjG = fourjp1G;
                fourjB = fourjp1B;
                fourjp1R = fourjp2R;
                fourjp1G = fourjp2G;
                fourjp1B = fourjp2B;
                fourjp2R = fourjp3R;
                fourjp2G = fourjp3G;
                fourjp2B = fourjp3B;
                fourjp3R = fourjp4R;
                fourjp3G = fourjp4G;
                fourjp3B = fourjp4B;
                fourjp4R += 4;
                fourjp4G += 4;
                fourjp4B += 4;
            }
        }
        //  float
        else
        {
            const float* const* redIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedRedF2D();
            const float* const* greenIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedGreenF2D();
            const float* const* blueIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedBlueF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            int jm4 = jLow-4, jm3 = jLow-3, jm2=jLow-2, jm1 = jLow-1;
            int jp4 = jLow+4, jp3 = jLow+3, jp2=jLow+2, jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM4; i<=iHighP4; i++)
	            {
					redTemp[i][j] =     fg_[4]*(redIn[i][jm4] + redIn[i][jp4]) +
									    fg_[3]*(redIn[i][jm3] + redIn[i][jp3]) +
									    fg_[2]*(redIn[i][jm2] + redIn[i][jp2]) +
									    fg_[1]*(redIn[i][jm1] + redIn[i][jp1]) +
									    fg_[0]* redIn[i][j];
					greenTemp[i][j] =   fg_[4]*(greenIn[i][jm4] + greenIn[i][jp4]) +
									    fg_[3]*(greenIn[i][jm3] + greenIn[i][jp3]) +
									    fg_[2]*(greenIn[i][jm2] + greenIn[i][jp2]) +
									    fg_[1]*(greenIn[i][jm1] + greenIn[i][jp1]) +
									    fg_[0]* greenIn[i][j];
					blueTemp[i][j] = 	fg_[4]*(blueIn[i][jm4] + blueIn[i][jp4]) +
									    fg_[3]*(blueIn[i][jm3] + blueIn[i][jp3]) +
									    fg_[2]*(blueIn[i][jm2] + blueIn[i][jp2]) +
									    fg_[1]*(blueIn[i][jm1] + blueIn[i][jp1]) +
									    fg_[0]*blueIn[i][j];
				}
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1=jp2;
                jp2=jp3;
                jp3=jp4;
                jp4++;
			}

        }
        
        //-------------------------------------------------------
        // Then apply the 1D filter along the y direction
        //-------------------------------------------------------
        int im4 = iLow-4, im3 = iLow-3, im2 = iLow-2, im1 = iLow-1;
        int ip4 = iLow+4, ip3 = iLow+3, ip2 = iLow+2, ip1 = iLow+1;
        //
        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                redOut[i][j] =      fg_[4]*(redTemp[im4][j] + redTemp[ip4][j]) +
                                    fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
                                    fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
                                    fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
                                    fg_[0]*redTemp[i][j];
            
                greenOut[i][j] =    fg_[4]*(greenTemp[im4][j] + greenTemp[ip4][j]) +
                                    fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
                                    fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
                                    fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
                                    fg_[0]*greenTemp[i][j];
            
                blueOut[i][j] =     fg_[4]*(blueTemp[im4][j] + blueTemp[ip4][j]) +
                                    fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
                                    fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
                                    fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
                                    fg_[0]*blueTemp[i][j];
            }
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
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}

void ColorGaussianFilter_F::applyInto_11x11_(const RasterImage* imgIn, const ImageRect* theRect,
                                             RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int	iLowM5 = iLow - 5, iHighP5 = iHigh + 5;
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    float* const* redOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedRedF2D(R_W_ACCESS);
    float* const* greenOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedGreenF2D(R_W_ACCESS);
    float* const* blueOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedBlueF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kRGBa32Raster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iRGBa = imgIn->getShiftedRaster2D();
            int fourjR, fourjG, fourjB,         //  4*j     red green blue
                fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
                fourjp1R, fourjp1G, fourjp1B,   //  4*(j+1) red green blue
                fourjm2R, fourjm2G, fourjm2B,   //  4*(j-2) red green blue
                fourjp2R, fourjp2G, fourjp2B,   //  4*(j+2) red green blue
                fourjm3R, fourjm3G, fourjm3B,   //  4*(j-3) red green blue
                fourjp3R, fourjp3G, fourjp3B,   //  4*(j+3) red green blue
                fourjm4R, fourjm4G, fourjm4B,   //  4*(j-4) red green blue
                fourjp4R, fourjp4G, fourjp4B,   //  4*(j+4) red green blue
                fourjm5R, fourjm5G, fourjm5B,   //  4*(j-5) red green blue
                fourjp5R, fourjp5G, fourjp5B;   //  4*(j+5) red green blue

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            fourjm5R = 4*(jLow-5);
            fourjm5G = fourjm5R + 1;
            fourjm5B = fourjm5G + 1;
            fourjm4R = 4*(jLow-4);
            fourjm4G = fourjm4R + 1;
            fourjm4B = fourjm4G + 1;
            fourjm3R = 4*(jLow-3);
            fourjm3G = fourjm3R + 1;
            fourjm3B = fourjm3G + 1;
            fourjm2R = 4*(jLow-2);
            fourjm2G = fourjm2R + 1;
            fourjm2B = fourjm2G + 1;
            fourjm1R = 4*(jLow-1);
            fourjm1G = fourjm1R + 1;
            fourjm1B = fourjm1G + 1;
            fourjR   = 4*jLow;
            fourjG   = fourjR + 1;
            fourjB   = fourjG + 1;
            fourjp1R = 4*(jLow+1);
            fourjp1G = fourjp1R + 1;
            fourjp1B = fourjp1G + 1;
            fourjp2R = 4*(jLow+2);
            fourjp2G = fourjp2R + 1;
            fourjp2B = fourjp2G + 1;
            fourjp3R = 4*(jLow+3);
            fourjp3G = fourjp3R + 1;
            fourjp3B = fourjp3G + 1;
            fourjp4R = 4*(jLow+4);
            fourjp4G = fourjp4R + 1;
            fourjp4B = fourjp4G + 1;
            fourjp5R = 4*(jLow+5);
            fourjp5G = fourjp5R + 1;
            fourjp5B = fourjp5G + 1;
            //
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM5; i<=iHighP5; i++)
	            {
					redTemp[i][j] = fg_[5]*(iRGBa[i][fourjm5R] + iRGBa[i][fourjp5R]) +
                                    fg_[4]*(iRGBa[i][fourjm4R] + iRGBa[i][fourjp4R]) +
                                    fg_[3]*(iRGBa[i][fourjm3R] + iRGBa[i][fourjp3R]) +
                                    fg_[2]*(iRGBa[i][fourjm2R] + iRGBa[i][fourjp2R]) +
                                    fg_[1]*(iRGBa[i][fourjm1R] + iRGBa[i][fourjp1R]) +
                                        fg_[0]*iRGBa[i][fourjR];
					greenTemp[i][j] = fg_[5]*(iRGBa[i][fourjm5G] + iRGBa[i][fourjp5G]) +
                                        fg_[4]*(iRGBa[i][fourjm4G] + iRGBa[i][fourjp4G]) +
                                        fg_[3]*(iRGBa[i][fourjm3G] + iRGBa[i][fourjp3G]) +
                                        fg_[2]*(iRGBa[i][fourjm2G] + iRGBa[i][fourjp2G]) +
                                        fg_[1]*(iRGBa[i][fourjm1G] + iRGBa[i][fourjp1G]) +
                                        fg_[0]*iRGBa[i][fourjG];
					blueTemp[i][j] = fg_[5]*(iRGBa[i][fourjm5B] + iRGBa[i][fourjp5B]) +
                                        fg_[4]*(iRGBa[i][fourjm4B] + iRGBa[i][fourjp4B]) +
                                        fg_[3]*(iRGBa[i][fourjm3B] + iRGBa[i][fourjp3B]) +
                                        fg_[2]*(iRGBa[i][fourjm2B] + iRGBa[i][fourjp2B]) +
                                        fg_[1]*(iRGBa[i][fourjm1B] + iRGBa[i][fourjp1B]) +
                                        fg_[0]*iRGBa[i][fourjB];
				}
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                fourjm5R = fourjm5R;
                fourjm5G = fourjm5G;
                fourjm5B = fourjm5B;
                fourjm4R = fourjm3R;
                fourjm4G = fourjm3G;
                fourjm4B = fourjm3B;
                fourjm3R = fourjm2R;
                fourjm3G = fourjm2G;
                fourjm3B = fourjm2B;
                fourjm2R = fourjm1R;
                fourjm2G = fourjm1G;
                fourjm2B = fourjm1B;
                fourjm1R = fourjR;
                fourjm1G = fourjG;
                fourjm1B = fourjB;
                fourjR = fourjp1R;
                fourjG = fourjp1G;
                fourjB = fourjp1B;
                fourjp1R = fourjp2R;
                fourjp1G = fourjp2G;
                fourjp1B = fourjp2B;
                fourjp2R = fourjp3R;
                fourjp2G = fourjp3G;
                fourjp2B = fourjp3B;
                fourjp3R = fourjp4R;
                fourjp3G = fourjp4G;
                fourjp3B = fourjp4B;
                fourjp4R = fourjp5R;
                fourjp4G = fourjp5G;
                fourjp4B = fourjp5B;
                fourjp5R += 4;
                fourjp5G += 4;
                fourjp5B += 4;
            }
        }
        //  float
        else
        {
            const float* const* redIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedRedF2D();
            const float* const* greenIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedGreenF2D();
            const float* const* blueIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedBlueF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            int jm5 = jLow-5, jm4 = jLow-4, jm3 = jLow-3, jm2=jLow-2, jm1 = jLow-1;
            int jp5 = jLow+5, jp4 = jLow+4, jp3 = jLow+3, jp2=jLow+2, jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM5; i<=iHighP5; i++)
	            {
					redTemp[i][j] =     fg_[5]*(redIn[i][jm5] + redIn[i][jp5]) +
									    fg_[4]*(redIn[i][jm4] + redIn[i][jp4]) +
									    fg_[3]*(redIn[i][jm3] + redIn[i][jp3]) +
									    fg_[2]*(redIn[i][jm2] + redIn[i][jp2]) +
									    fg_[1]*(redIn[i][jm1] + redIn[i][jp1]) +
									    fg_[0]* redIn[i][j];
					greenTemp[i][j] =   fg_[5]*(greenIn[i][jm5] + greenIn[i][jp5]) +
									    fg_[4]*(greenIn[i][jm4] + greenIn[i][jp4]) +
									    fg_[3]*(greenIn[i][jm3] + greenIn[i][jp3]) +
									    fg_[2]*(greenIn[i][jm2] + greenIn[i][jp2]) +
									    fg_[1]*(greenIn[i][jm1] + greenIn[i][jp1]) +
									    fg_[0]* greenIn[i][j];
					blueTemp[i][j] = 	fg_[5]*(blueIn[i][jm5] + blueIn[i][jp5]) +
									    fg_[4]*(blueIn[i][jm4] + blueIn[i][jp4]) +
									    fg_[3]*(blueIn[i][jm3] + blueIn[i][jp3]) +
									    fg_[2]*(blueIn[i][jm2] + blueIn[i][jp2]) +
									    fg_[1]*(blueIn[i][jm1] + blueIn[i][jp1]) +
									    fg_[0]*blueIn[i][j];
				}
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm5 = jm4;
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1=jp2;
                jp2=jp3;
                jp3=jp4;
                jp4=jp5;
                jp5++;
			}

        }
        
        //-------------------------------------------------------
        // Then apply the 1D filter along the y direction
        //-------------------------------------------------------
        int im5 = iLow-5, im4 = iLow-4, im3 = iLow-3, im2 = iLow-2, im1 = iLow-1;
        int ip5 = iLow+5, ip4 = iLow+4, ip3 = iLow+3, ip2 = iLow+2, ip1 = iLow+1;
        //
        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                redOut[i][j] =      fg_[5]*(redTemp[im5][j] + redTemp[ip5][j]) +
                                    fg_[4]*(redTemp[im4][j] + redTemp[ip4][j]) +
                                    fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
                                    fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
                                    fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
                                    fg_[0]*redTemp[i][j];
            
                greenOut[i][j] =    fg_[5]*(greenTemp[im5][j] + greenTemp[ip5][j]) +
                                    fg_[4]*(greenTemp[im4][j] + greenTemp[ip4][j]) +
                                    fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
                                    fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
                                    fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
                                    fg_[0]*greenTemp[i][j];
            
                blueOut[i][j] =     fg_[5]*(blueTemp[im5][j] + blueTemp[ip5][j]) +
                                    fg_[4]*(blueTemp[im4][j] + blueTemp[ip4][j]) +
                                    fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
                                    fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
                                    fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
                                    fg_[0]*blueTemp[i][j];
            }
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
    else
    {
        //  we only accept gray images so far
        FAIL_CONDITION(	true,
        				kFunctionNotImplemented,
        				"image type not suported by Gaussian filter yet");
    }
}

void ColorGaussianFilter_F::applyInto_13x13_(const RasterImage* imgIn, const ImageRect* theRect,
                                             RasterImage* imgOut)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int	iLowM6 = iLow - 6, iHighP6 = iHigh + 6;
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    float* const* redOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedRedF2D(R_W_ACCESS);
    float* const* greenOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedGreenF2D(R_W_ACCESS);
    float* const* blueOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedBlueF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kRGBa32Raster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iRGBa = imgIn->getShiftedRaster2D();
            int fourjR, fourjG, fourjB,         //  4*j     red green blue
                fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
                fourjp1R, fourjp1G, fourjp1B,   //  4*(j+1) red green blue
                fourjm2R, fourjm2G, fourjm2B,   //  4*(j-2) red green blue
                fourjp2R, fourjp2G, fourjp2B,   //  4*(j+2) red green blue
                fourjm3R, fourjm3G, fourjm3B,   //  4*(j-3) red green blue
                fourjp3R, fourjp3G, fourjp3B,   //  4*(j+3) red green blue
                fourjm4R, fourjm4G, fourjm4B,   //  4*(j-4) red green blue
                fourjp4R, fourjp4G, fourjp4B,   //  4*(j+4) red green blue
                fourjm5R, fourjm5G, fourjm5B,   //  4*(j-5) red green blue
                fourjp5R, fourjp5G, fourjp5B,   //  4*(j+5) red green blue
                fourjm6R, fourjm6G, fourjm6B,   //  4*(j-6) red green blue
                fourjp6R, fourjp6G, fourjp6B;   //  4*(j+6) red green blue

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            fourjm6R = 4*(jLow-6);
            fourjm6G = fourjm6R + 1;
            fourjm6B = fourjm6G + 1;
            fourjm5R = 4*(jLow-5);
            fourjm5G = fourjm5R + 1;
            fourjm5B = fourjm5G + 1;
            fourjm4R = 4*(jLow-4);
            fourjm4G = fourjm4R + 1;
            fourjm4B = fourjm4G + 1;
            fourjm3R = 4*(jLow-3);
            fourjm3G = fourjm3R + 1;
            fourjm3B = fourjm3G + 1;
            fourjm2R = 4*(jLow-2);
            fourjm2G = fourjm2R + 1;
            fourjm2B = fourjm2G + 1;
            fourjm1R = 4*(jLow-1);
            fourjm1G = fourjm1R + 1;
            fourjm1B = fourjm1G + 1;
            fourjR   = 4*jLow;
            fourjG   = fourjR + 1;
            fourjB   = fourjG + 1;
            fourjp1R = 4*(jLow+1);
            fourjp1G = fourjp1R + 1;
            fourjp1B = fourjp1G + 1;
            fourjp2R = 4*(jLow+2);
            fourjp2G = fourjp2R + 1;
            fourjp2B = fourjp2G + 1;
            fourjp3R = 4*(jLow+3);
            fourjp3G = fourjp3R + 1;
            fourjp3B = fourjp3G + 1;
            fourjp4R = 4*(jLow+4);
            fourjp4G = fourjp4R + 1;
            fourjp4B = fourjp4G + 1;
            fourjp5R = 4*(jLow+5);
            fourjp5G = fourjp5R + 1;
            fourjp5B = fourjp5G + 1;
            fourjp6R = 4*(jLow+6);
            fourjp6G = fourjp6R + 1;
            fourjp6B = fourjp6G + 1;
            //
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM6; i<=iHighP6; i++)
	            {
					redTemp[i][j] = fg_[6]*(iRGBa[i][fourjm6R] + iRGBa[i][fourjp6R]) +
                                    fg_[5]*(iRGBa[i][fourjm5R] + iRGBa[i][fourjp5R]) +
                                    fg_[4]*(iRGBa[i][fourjm4R] + iRGBa[i][fourjp4R]) +
                                    fg_[3]*(iRGBa[i][fourjm3R] + iRGBa[i][fourjp3R]) +
                                    fg_[2]*(iRGBa[i][fourjm2R] + iRGBa[i][fourjp2R]) +
                                    fg_[1]*(iRGBa[i][fourjm1R] + iRGBa[i][fourjp1R]) +
                                        fg_[0]*iRGBa[i][fourjR];
					greenTemp[i][j] = fg_[6]*(iRGBa[i][fourjm6G] + iRGBa[i][fourjp6G]) +
                                        fg_[5]*(iRGBa[i][fourjm5G] + iRGBa[i][fourjp5G]) +
                                        fg_[4]*(iRGBa[i][fourjm4G] + iRGBa[i][fourjp4G]) +
                                        fg_[3]*(iRGBa[i][fourjm3G] + iRGBa[i][fourjp3G]) +
                                        fg_[2]*(iRGBa[i][fourjm2G] + iRGBa[i][fourjp2G]) +
                                        fg_[1]*(iRGBa[i][fourjm1G] + iRGBa[i][fourjp1G]) +
                                        fg_[0]*iRGBa[i][fourjG];
					blueTemp[i][j] = fg_[6]*(iRGBa[i][fourjm6B] + iRGBa[i][fourjp6B]) +
                                        fg_[5]*(iRGBa[i][fourjm5B] + iRGBa[i][fourjp5B]) +
                                        fg_[4]*(iRGBa[i][fourjm4B] + iRGBa[i][fourjp4B]) +
                                        fg_[3]*(iRGBa[i][fourjm3B] + iRGBa[i][fourjp3B]) +
                                        fg_[2]*(iRGBa[i][fourjm2B] + iRGBa[i][fourjp2B]) +
                                        fg_[1]*(iRGBa[i][fourjm1B] + iRGBa[i][fourjp1B]) +
                                        fg_[0]*iRGBa[i][fourjB];
				}
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                fourjm6R = fourjm6R;
                fourjm6G = fourjm6G;
                fourjm6B = fourjm6B;
                fourjm5R = fourjm5R;
                fourjm5G = fourjm5G;
                fourjm5B = fourjm5B;
                fourjm4R = fourjm3R;
                fourjm4G = fourjm3G;
                fourjm4B = fourjm3B;
                fourjm3R = fourjm2R;
                fourjm3G = fourjm2G;
                fourjm3B = fourjm2B;
                fourjm2R = fourjm1R;
                fourjm2G = fourjm1G;
                fourjm2B = fourjm1B;
                fourjm1R = fourjR;
                fourjm1G = fourjG;
                fourjm1B = fourjB;
                fourjR = fourjp1R;
                fourjG = fourjp1G;
                fourjB = fourjp1B;
                fourjp1R = fourjp2R;
                fourjp1G = fourjp2G;
                fourjp1B = fourjp2B;
                fourjp2R = fourjp3R;
                fourjp2G = fourjp3G;
                fourjp2B = fourjp3B;
                fourjp3R = fourjp4R;
                fourjp3G = fourjp4G;
                fourjp3B = fourjp4B;
                fourjp4R = fourjp5R;
                fourjp4G = fourjp5G;
                fourjp4B = fourjp5B;
                fourjp5R = fourjp6R;
                fourjp5G = fourjp6G;
                fourjp5B = fourjp6B;
                fourjp6R += 4;
                fourjp6G += 4;
                fourjp6B += 4;
            }
        }
        //  float
        else
        {
            const float* const* redIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedRedF2D();
            const float* const* greenIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedGreenF2D();
            const float* const* blueIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedBlueF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            int jm6 = jLow-6, jm5 = jLow-5, jm4 = jLow-4, jm3 = jLow-3, 
                jm2=jLow-2, jm1 = jLow-1;
            int jp6 = jLow-6, jp5 = jLow+5, jp4 = jLow+4, jp3 = jLow+3, 
                jp2=jLow+2, jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowM6; i<=iHighP6; i++)
	            {
					redTemp[i][j] =     fg_[6]*(redIn[i][jm6] + redIn[i][jp6]) +
									    fg_[5]*(redIn[i][jm5] + redIn[i][jp5]) +
									    fg_[4]*(redIn[i][jm4] + redIn[i][jp4]) +
									    fg_[3]*(redIn[i][jm3] + redIn[i][jp3]) +
									    fg_[2]*(redIn[i][jm2] + redIn[i][jp2]) +
									    fg_[1]*(redIn[i][jm1] + redIn[i][jp1]) +
									    fg_[0]* redIn[i][j];
					greenTemp[i][j] =   fg_[6]*(greenIn[i][jm6] + greenIn[i][jp6]) +
									    fg_[5]*(greenIn[i][jm5] + greenIn[i][jp5]) +
									    fg_[4]*(greenIn[i][jm4] + greenIn[i][jp4]) +
									    fg_[3]*(greenIn[i][jm3] + greenIn[i][jp3]) +
									    fg_[2]*(greenIn[i][jm2] + greenIn[i][jp2]) +
									    fg_[1]*(greenIn[i][jm1] + greenIn[i][jp1]) +
									    fg_[0]* greenIn[i][j];
					blueTemp[i][j] = 	fg_[6]*(blueIn[i][jm6] + blueIn[i][jp6]) +
									    fg_[5]*(blueIn[i][jm5] + blueIn[i][jp5]) +
									    fg_[4]*(blueIn[i][jm4] + blueIn[i][jp4]) +
									    fg_[3]*(blueIn[i][jm3] + blueIn[i][jp3]) +
									    fg_[2]*(blueIn[i][jm2] + blueIn[i][jp2]) +
									    fg_[1]*(blueIn[i][jm1] + blueIn[i][jp1]) +
									    fg_[0]*blueIn[i][j];
				}
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm6 = jm5;
                jm5 = jm4;
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1=jp2;
                jp2=jp3;
                jp3=jp4;
                jp4=jp5;
                jp5=jp6;
                jp6++;
			}

        }
        
        //-------------------------------------------------------
        // Then apply the 1D filter along the y direction
        //-------------------------------------------------------
        int im6 = iLow-6, im5 = iLow-5, im4 = iLow-4, im3 = iLow-3, 
            im2 = iLow-2, im1 = iLow-1;
        int ip6 = iLow+6, ip5 = iLow+5, ip4 = iLow+4, ip3 = iLow+3, 
            ip2 = iLow+2, ip1 = iLow+1;
        //
        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                redOut[i][j] =      fg_[6]*(redTemp[im6][j] + redTemp[ip6][j]) +
                                    fg_[5]*(redTemp[im5][j] + redTemp[ip5][j]) +
                                    fg_[4]*(redTemp[im4][j] + redTemp[ip4][j]) +
                                    fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
                                    fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
                                    fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
                                    fg_[0]*redTemp[i][j];
            
                greenOut[i][j] =    fg_[6]*(greenTemp[im6][j] + greenTemp[ip6][j]) +
                                    fg_[5]*(greenTemp[im5][j] + greenTemp[ip5][j]) +
                                    fg_[4]*(greenTemp[im4][j] + greenTemp[ip4][j]) +
                                    fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
                                    fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
                                    fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
                                    fg_[0]*greenTemp[i][j];
            
                blueOut[i][j] =     fg_[6]*(blueTemp[im6][j] + blueTemp[ip6][j]) +
                                    fg_[5]*(blueTemp[im5][j] + blueTemp[ip5][j]) +
                                    fg_[4]*(blueTemp[im4][j] + blueTemp[ip4][j]) +
                                    fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
                                    fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
                                    fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
                                    fg_[0]*blueTemp[i][j];
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


void ColorGaussianFilter_F::applyInto_(const RasterImage* imgIn, const ImageRect* theRect,
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
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    float* const* redOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedRedF2D(R_W_ACCESS);
    float* const* greenOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedGreenF2D(R_W_ACCESS);
    float* const* blueOut = static_cast<RasterImage_RGBa_F*>(imgOut)->getShiftedBlueF2D(R_W_ACCESS);
    
    if (imgIn->getBaseRasterType() == kRGBa32Raster)
    {
        //  case of an "integer" gray level image
        if (!imgIn->hasFloatRaster())
        {
            const unsigned char* const* iRGBa = imgIn->getShiftedRaster2D();
            int fourjR, fourjG, fourjB,         //  4*j     red green blue
                fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
                fourjp1R, fourjp1G, fourjp1B,   //  4*(j+1) red green blue
                fourjm2R, fourjm2G, fourjm2B,   //  4*(j-2) red green blue
                fourjp2R, fourjp2G, fourjp2B,   //  4*(j+2) red green blue
                fourjm3R, fourjm3G, fourjm3B,   //  4*(j-3) red green blue
                fourjp3R, fourjp3G, fourjp3B,   //  4*(j+3) red green blue
                fourjm4R, fourjm4G, fourjm4B,   //  4*(j-4) red green blue
                fourjp4R, fourjp4G, fourjp4B,   //  4*(j+4) red green blue
                fourjm5R, fourjm5G, fourjm5B,   //  4*(j-5) red green blue
                fourjp5R, fourjp5G, fourjp5B,   //  4*(j+5) red green blue
                fourjm6R, fourjm6G, fourjm6B,   //  4*(j-6) red green blue
                fourjp6R, fourjp6G, fourjp6B;   //  4*(j+6) red green blue

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            fourjm6R = 4*(jLow-6);
            fourjm6G = fourjm6R + 1;
            fourjm6B = fourjm6G + 1;
            fourjm5R = 4*(jLow-5);
            fourjm5G = fourjm5R + 1;
            fourjm5B = fourjm5G + 1;
            fourjm4R = 4*(jLow-4);
            fourjm4G = fourjm4R + 1;
            fourjm4B = fourjm4G + 1;
            fourjm3R = 4*(jLow-3);
            fourjm3G = fourjm3R + 1;
            fourjm3B = fourjm3G + 1;
            fourjm2R = 4*(jLow-2);
            fourjm2G = fourjm2R + 1;
            fourjm2B = fourjm2G + 1;
            fourjm1R = 4*(jLow-1);
            fourjm1G = fourjm1R + 1;
            fourjm1B = fourjm1G + 1;
            fourjR   = 4*jLow;
            fourjG   = fourjR + 1;
            fourjB   = fourjG + 1;
            fourjp1R = 4*(jLow+1);
            fourjp1G = fourjp1R + 1;
            fourjp1B = fourjp1G + 1;
            fourjp2R = 4*(jLow+2);
            fourjp2G = fourjp2R + 1;
            fourjp2B = fourjp2G + 1;
            fourjp3R = 4*(jLow+3);
            fourjp3G = fourjp3R + 1;
            fourjp3B = fourjp3G + 1;
            fourjp4R = 4*(jLow+4);
            fourjp4G = fourjp4R + 1;
            fourjp4B = fourjp4G + 1;
            fourjp5R = 4*(jLow+5);
            fourjp5G = fourjp5R + 1;
            fourjp5B = fourjp5G + 1;
            fourjp6R = 4*(jLow+6);
            fourjp6G = fourjp6R + 1;
            fourjp6B = fourjp6G + 1;
            //
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowMm; i<=iHighPm; i++)
	            {
					redTemp[i][j] = fg_[6]*(iRGBa[i][fourjm6R] + iRGBa[i][fourjp6R]) +
                                    fg_[5]*(iRGBa[i][fourjm5R] + iRGBa[i][fourjp5R]) +
                                    fg_[4]*(iRGBa[i][fourjm4R] + iRGBa[i][fourjp4R]) +
                                    fg_[3]*(iRGBa[i][fourjm3R] + iRGBa[i][fourjp3R]) +
                                    fg_[2]*(iRGBa[i][fourjm2R] + iRGBa[i][fourjp2R]) +
                                    fg_[1]*(iRGBa[i][fourjm1R] + iRGBa[i][fourjp1R]) +
                                        fg_[0]*iRGBa[i][fourjR];
					greenTemp[i][j] = fg_[6]*(iRGBa[i][fourjm6G] + iRGBa[i][fourjp6G]) +
                                        fg_[5]*(iRGBa[i][fourjm5G] + iRGBa[i][fourjp5G]) +
                                        fg_[4]*(iRGBa[i][fourjm4G] + iRGBa[i][fourjp4G]) +
                                        fg_[3]*(iRGBa[i][fourjm3G] + iRGBa[i][fourjp3G]) +
                                        fg_[2]*(iRGBa[i][fourjm2G] + iRGBa[i][fourjp2G]) +
                                        fg_[1]*(iRGBa[i][fourjm1G] + iRGBa[i][fourjp1G]) +
                                        fg_[0]*iRGBa[i][fourjG];
					blueTemp[i][j] = fg_[6]*(iRGBa[i][fourjm6B] + iRGBa[i][fourjp6B]) +
                                        fg_[5]*(iRGBa[i][fourjm5B] + iRGBa[i][fourjp5B]) +
                                        fg_[4]*(iRGBa[i][fourjm4B] + iRGBa[i][fourjp4B]) +
                                        fg_[3]*(iRGBa[i][fourjm3B] + iRGBa[i][fourjp3B]) +
                                        fg_[2]*(iRGBa[i][fourjm2B] + iRGBa[i][fourjp2B]) +
                                        fg_[1]*(iRGBa[i][fourjm1B] + iRGBa[i][fourjp1B]) +
                                        fg_[0]*iRGBa[i][fourjB];

					for (int l=kMaxHardCodedWidth+1, /* fourl=4*(kMaxHardCodedWidth+1), unused var. -CJC*/
					         fourjml = fourjR - 4*(kMaxHardCodedWidth+1),
					         fourjpl = fourjR + 4*(kMaxHardCodedWidth+1); l<=m; l++, fourjml--, fourjpl++)
                    {
						redTemp[i][j]   += fg_[l] * (iRGBa[i][fourjml--] + 
                                                     iRGBa[i][fourjpl++]);
						greenTemp[i][j] += fg_[l] * (iRGBa[i][fourjml--] + 
                                                     iRGBa[i][fourjpl++]);
						blueTemp[i][j]  += fg_[l] * (iRGBa[i][fourjml--] + 
                                                     iRGBa[i][fourjpl++]);
                    }
				}
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                fourjm6R = fourjm6R;
                fourjm6G = fourjm6G;
                fourjm6B = fourjm6B;
                fourjm5R = fourjm5R;
                fourjm5G = fourjm5G;
                fourjm5B = fourjm5B;
                fourjm4R = fourjm3R;
                fourjm4G = fourjm3G;
                fourjm4B = fourjm3B;
                fourjm3R = fourjm2R;
                fourjm3G = fourjm2G;
                fourjm3B = fourjm2B;
                fourjm2R = fourjm1R;
                fourjm2G = fourjm1G;
                fourjm2B = fourjm1B;
                fourjm1R = fourjR;
                fourjm1G = fourjG;
                fourjm1B = fourjB;
                fourjR = fourjp1R;
                fourjG = fourjp1G;
                fourjB = fourjp1B;
                fourjp1R = fourjp2R;
                fourjp1G = fourjp2G;
                fourjp1B = fourjp2B;
                fourjp2R = fourjp3R;
                fourjp2G = fourjp3G;
                fourjp2B = fourjp3B;
                fourjp3R = fourjp4R;
                fourjp3G = fourjp4G;
                fourjp3B = fourjp4B;
                fourjp4R = fourjp5R;
                fourjp4G = fourjp5G;
                fourjp4B = fourjp5B;
                fourjp5R = fourjp6R;
                fourjp5G = fourjp6G;
                fourjp5B = fourjp6B;
                fourjp6R += 4;
                fourjp6G += 4;
                fourjp6B += 4;
            }
        }
        //  float
        else
        {
            const float* const* redIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedRedF2D();
            const float* const* greenIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedGreenF2D();
            const float* const* blueIn = static_cast<const RasterImage_RGBa_F*>(imgIn)->getShiftedBlueF2D();

			//-------------------------------------------------------
			//	first apply the 1D filter along the x direction
			//-------------------------------------------------------
			//	Note that we have to apply this filter out of the destination rect since
			//	some of these elements will be needed we we apply a filter along the
			//	other direction.
            int jm6 = jLow-6, jm5 = jLow-5, jm4 = jLow-4, jm3 = jLow-3, 
                jm2=jLow-2, jm1 = jLow-1;
            int jp6 = jLow-6, jp5 = jLow+5, jp4 = jLow+4, jp3 = jLow+3, 
                jp2=jLow+2, jp1 = jLow+1;
            for (int j=jLow; j<=jHigh; j++)
            {
	            for (int i=iLowMm; i<=iHighPm; i++)
	            {
					redTemp[i][j] =     fg_[6]*(redIn[i][jm6] + redIn[i][jp6]) +
									    fg_[5]*(redIn[i][jm5] + redIn[i][jp5]) +
									    fg_[4]*(redIn[i][jm4] + redIn[i][jp4]) +
									    fg_[3]*(redIn[i][jm3] + redIn[i][jp3]) +
									    fg_[2]*(redIn[i][jm2] + redIn[i][jp2]) +
									    fg_[1]*(redIn[i][jm1] + redIn[i][jp1]) +
									    fg_[0]* redIn[i][j];
					greenTemp[i][j] =   fg_[6]*(greenIn[i][jm6] + greenIn[i][jp6]) +
									    fg_[5]*(greenIn[i][jm5] + greenIn[i][jp5]) +
									    fg_[4]*(greenIn[i][jm4] + greenIn[i][jp4]) +
									    fg_[3]*(greenIn[i][jm3] + greenIn[i][jp3]) +
									    fg_[2]*(greenIn[i][jm2] + greenIn[i][jp2]) +
									    fg_[1]*(greenIn[i][jm1] + greenIn[i][jp1]) +
									    fg_[0]* greenIn[i][j];
					blueTemp[i][j] = 	fg_[6]*(blueIn[i][jm6] + blueIn[i][jp6]) +
									    fg_[5]*(blueIn[i][jm5] + blueIn[i][jp5]) +
									    fg_[4]*(blueIn[i][jm4] + blueIn[i][jp4]) +
									    fg_[3]*(blueIn[i][jm3] + blueIn[i][jp3]) +
									    fg_[2]*(blueIn[i][jm2] + blueIn[i][jp2]) +
									    fg_[1]*(blueIn[i][jm1] + blueIn[i][jp1]) +
									    fg_[0]*blueIn[i][j];

        			for (int l= kMaxHardCodedWidth+1, jml=j-l, jpl=j+l; l<=m; l++, jpl++, jml--)
                    {
						redTemp[i][j] += fg_[l] * (redIn[i][jml] + redIn[i][jpl]);
						greenTemp[i][j] += fg_[l] * (greenIn[i][jml] + greenIn[i][jpl]);
						blueTemp[i][j] += fg_[l] * (blueIn[i][jml] + blueIn[i][jpl]);
                    }
				}
						
                //  shift the j indexes (j itself is taken care of in the loop
                //  statement)
                jm6 = jm5;
                jm5 = jm4;
                jm4 = jm3;
                jm3 = jm2;
                jm2 = jm1;
                jm1 = j;
                jp1=jp2;
                jp2=jp3;
                jp3=jp4;
                jp4=jp5;
                jp5=jp6;
                jp6++;
			}

        }
        
        //-------------------------------------------------------
        // Then apply the 1D filter along the y direction
        //-------------------------------------------------------
        int im6 = iLow-6, im5 = iLow-5, im4 = iLow-4, im3 = iLow-3, 
            im2 = iLow-2, im1 = iLow-1;
        int ip6 = iLow+6, ip5 = iLow+5, ip4 = iLow+4, ip3 = iLow+3, 
            ip2 = iLow+2, ip1 = iLow+1;
        //
        for (int i=iLow; i<=iHigh; i++)
        {
            for (int j=jLow; j<=jHigh; j++)
            {
                redOut[i][j] =      fg_[6]*(redTemp[im6][j] + redTemp[ip6][j]) +
                                    fg_[5]*(redTemp[im5][j] + redTemp[ip5][j]) +
                                    fg_[4]*(redTemp[im4][j] + redTemp[ip4][j]) +
                                    fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
                                    fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
                                    fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
                                    fg_[0]*redTemp[i][j];
            
                greenOut[i][j] =    fg_[6]*(greenTemp[im6][j] + greenTemp[ip6][j]) +
                                    fg_[5]*(greenTemp[im5][j] + greenTemp[ip5][j]) +
                                    fg_[4]*(greenTemp[im4][j] + greenTemp[ip4][j]) +
                                    fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
                                    fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
                                    fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
                                    fg_[0]*greenTemp[i][j];
            
                blueOut[i][j] =     fg_[6]*(blueTemp[im6][j] + blueTemp[ip6][j]) +
                                    fg_[5]*(blueTemp[im5][j] + blueTemp[ip5][j]) +
                                    fg_[4]*(blueTemp[im4][j] + blueTemp[ip4][j]) +
                                    fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
                                    fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
                                    fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
                                    fg_[0]*blueTemp[i][j];

                for (int k= kMaxHardCodedWidth+1, imk=i-k, ipk=i+k; k<=m; k++, ipk++, imk--)
                {
                    redOut[i][j]   += fg_[k] * (redTemp[imk][j] + redTemp[ipk][j] );
                    greenOut[i][j] += fg_[k] * (greenTemp[imk][j] + greenTemp[ipk][j] );
                    blueOut[i][j]  += fg_[k] * (blueTemp[imk][j] + blueTemp[ipk][j] );
                }
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
#endif


void ColorGaussianFilter_F::initializeFilter_(void)
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
