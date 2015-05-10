/*  NAME:
        ColorMedianFilter_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorMedianFilter_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColorMedianFilter_F.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGBa_F.h"

using namespace std;
using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Local free function prototype
//------------------------------------------------------
#endif

float getColorMedian_(float* dat, int n);


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif

ColorMedianFilter_F::ColorMedianFilter_F(int theWidth)
		try	:	ColorMedianFilter(theWidth, theWidth, false, kPositiveFloat),
				//
				redData_(new float[theWidth*theWidth]),
				greenData_(new float[theWidth*theWidth]),
				blueData_(new float[theWidth*theWidth])				
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorMedianFilter_F constructor");
	throw e;
}

ColorMedianFilter_F::ColorMedianFilter_F(int theWidth, int theHeight)
		try	:	ColorMedianFilter(theWidth, theHeight, false, kPositiveFloat),
				//
				redData_(new float[theWidth*theHeight]),
				greenData_(new float[theWidth*theHeight]),
				blueData_(new float[theWidth*theHeight])				
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorMedianFilter_F constructor");
	throw e;
}

ColorMedianFilter_F::ColorMedianFilter_F(const ImageRect* theRect)
		try	:	ColorMedianFilter(theRect, false, kPositiveFloat),
				//
				redData_(new float[theRect->getWidth() * theRect->getHeight()]),
				greenData_(new float[theRect->getWidth() * theRect->getHeight()]),
				blueData_(new float[theRect->getWidth() * theRect->getHeight()])				
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorMedianFilter_F constructor");
	throw e;
}


ColorMedianFilter_F::~ColorMedianFilter_F(void)
{
    delete []redData_;
    delete []greenData_;
    delete []blueData_;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


RasterImage* ColorMedianFilter_F::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to ColorMedianFilter_F::applyTo");
    
	return applyTo(imgIn, imgIn->getValidRect());
}


RasterImage* ColorMedianFilter_F::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to ColorMedianFilter_F::applyTo");

    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in ColorMedianFilter_F::applyTo");

	RasterImage* imgOut = NULL;
	try {
		switch (imgIn->getBaseRasterType())
		{
			case kGrayRaster:
				imgOut = new RasterImage_gray_F(imgIn->getBoundRect());
				break;
				
			case kRGBa32Raster:
				imgOut = new RasterImage_RGBa_F(imgIn->getBoundRect());
				break;
				
			default:
				FAIL_CONDITION(	true,
								kFunctionNotImplemented,
								"image type not suported by Gaussian filter yet");
				break;
				
		}
		
		applyInto(imgIn, theRect, imgOut);

    }
	catch (ErrorReport& e) {
		e.appendToMessage("called by ColorMedianFilter_F::applyTo");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kImageProcessingError,
						"Error in the computation of ColorMedianFilter_F::applyTo");
	}

	return imgOut;
}

void ColorMedianFilter_F::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}


void ColorMedianFilter_F::applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut)
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

    FAIL_CONDITION(	imgOut == NULL,
    				kNullRasterImageError,
    				"NULL output raster image passed as parameter");

    //  so far I only accept a RasterImage_RGBa_F as a destination
	FAIL_CONDITION( imgOut->getBaseRasterType() != kRGBa32Raster ||
                    (!imgOut->hasFloatRaster()),
	                kInvalidRasterType,
	                "GaussianFilter_F::applyInto only accepts RasterImage_RGBa_F images as destination");

	//----------------------------------------------------------------
	//	STEP 1:		Test for rectangle intersection
	//----------------------------------------------------------------
	//  determine the output rectangle that the input image can produce.
	//		Here, it is the entire valid data rectangle.
    const ImageRect   *outRect = imgIn->getValidRect();
	//	... and determine its intersection with the user-specified output rectangle
	ImageRect	*destRect = ImageRect::intersection(theRect, outRect);
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle");
    //  if the output image does not contain the destination rect, it must be resized.
    if (!outRect->contains(destRect))
    	imgOut->setBoundRect(destRect);
                    
	//----------------------------------------------------------------
	//	STEP 2:		Get an RGBa input image
	//----------------------------------------------------------------
    bool    localRGBa = false;
    const RasterImage_RGBa* rgbaImg = RasterImage_RGBa::localRasterRGBa(imgIn, &localRGBa);

	//----------------------------------------------------------------
	//	STEP 3:		Perform the calculations
	//----------------------------------------------------------------
    const int   iLow = destRect->getTop(),
                iHigh = destRect->getBottom(),
                jLow = destRect->getLeft(),
                jHigh = destRect->getRight();
	const ImageRect* operRect = getRect();
    const int   recTop = operRect->getTop(),
                recBottom = operRect->getBottom(),
                recLeft = operRect->getLeft(),
                recRight = operRect->getRight();

	if (rgbaImg->hasFloatRaster() && rgbaImg->floatRasterIsReference())
	{
        const float* const* redIn = (static_cast<const RasterImage_RGBa_F*>(rgbaImg))->getShiftedRedF2D();
        const float* const* greenIn = (static_cast<const RasterImage_RGBa_F*>(rgbaImg))->getShiftedGreenF2D();
        const float* const* blueIn = (static_cast<const RasterImage_RGBa_F*>(rgbaImg))->getShiftedBlueF2D();
        //
        float* const* redOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
        float* const* greenOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
        float* const* blueOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);
	    //
	    for (int i=iLow; i<= iHigh; i++)
	    {
            const int kLow = MAX(i+recTop, iLow);
            const int kHigh = MIN(i+recBottom, iHigh);
	        //
	        for (int j=jLow; j<=jHigh; j++)
	        {
	            //  collect the data around the pixel.
                const int lLow = MAX(j+recLeft, jLow);
                const int lHigh = MIN(j+recRight, jHigh);
	            int   n = 0;
	            //
	            for (int k=kLow; k<=kHigh; k++)
	                for (int l=lLow; l<=lHigh; l++)
	                {
	                    redData_[n] = redIn[k][l];
	                    greenData_[n] = greenIn[k][l];
	                    blueData_[n++] = blueIn[k][l];
	                }

	            redOut[i][j] = getColorMedian_(redData_, n);
	            greenOut[i][j] = getColorMedian_(greenData_, n);
	            blueOut[i][j] = getColorMedian_(blueData_, n);
	        }
	    }
	}
	else
	{
        const unsigned char* const* rgbaIn = rgbaImg->getShiftedRGBa2D();
        //
        float* const* redOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
        float* const* greenOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
        float* const* blueOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);
        //
        for (int i=iLow; i<= iHigh; i++)
        {
            const int kLow = MAX(i+recTop, iLow);
            const int kHigh = MIN(i+recBottom, iHigh);
//             int fourj = 4*jLow; // unused var. -CJC
            //
            for (int j=jLow; j<=jHigh; j++)
            {
                //  collect the data around the pixel.
                const int lLow = MAX(j+recLeft, jLow);
                const int lHigh = MIN(j+recRight, jHigh);
                const int fourlMin = 4*lLow;
                int  n = 0;
                //
                for (int k=kLow; k<=kHigh; k++)
                {
                    int fourl = fourlMin;
                    for (int l=lLow; l<=lHigh; l++)
                    {
                        redData_[n] = rgbaIn[k][fourl++];
                        greenData_[n] = rgbaIn[k][fourl++];
                        blueData_[n++] = rgbaIn[k][fourl++];
                        fourl++;
                    }
                }

	            redOut[i][j] = getColorMedian_(redData_, n);
	            greenOut[i][j] = getColorMedian_(greenData_, n);
	            blueOut[i][j] = getColorMedian_(blueData_, n);
	        }
	    }
	}

	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
	imgOut->setValidRect(outRect);	
	imgOut->requestRasterUpdate();    

    //  cleanup
    if (localRGBa)
        delete rgbaImg;
        
}


#if 0
#pragma mark -
#endif

//  Computation of the median using N. Wirth's algorithm
//  Implementation derived from kth_smallest implementation of N Devillard.
//  Note that if the array is even-sized, the function does not return the average of
//  the two central values but the lower median.
float getColorMedian_(float* dat, int n)
{
    const int k = (n%2) == 0 ? n/2 - 1 : n/2;
    float   x;
    int     i, j, 
            l = 0, 
            m = n - 1;
    
    while (l<m)
    {
        x = dat[k];
        i = l;
        j = m;
        do
        {
            while (dat[i] < x)
                i++;
            while (dat[j] > x)
                j--;
            
            if (i <= j)
            {
                float temp = dat[i];
                dat[i] = dat[j];
                dat[j] = temp;
                i++;
                j--;
            }
            
        } while (i <= j);
        
        if (j < k)
            l = i;
        if (i > k)
            m = j;
    }
    
    return dat[k];
}
