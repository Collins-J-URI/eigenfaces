/*  NAME:
        ColorMedianFilter_I.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorMedianFilter_I class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColorMedianFilter_I.h"
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

///**	Computes the median value of an array of dat using N. Wirth's algorithm
// *	@param	dat	the array of values to compute the median of
// *	@param	n	 size of the data array
// *	@return		median value of dat array
// */
unsigned char getColorMedian_(unsigned char* dat, int n);


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ColorMedianFilter_I::ColorMedianFilter_I(int theWidth)
		try	:	ColorMedianFilter(theWidth, theWidth, true),
				//
				redData_(new unsigned char[theWidth*theWidth]),
				greenData_(new unsigned char[theWidth*theWidth]),
				blueData_(new unsigned char[theWidth*theWidth])				
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorMedianFilter_I constructor");
	throw e;
}

ColorMedianFilter_I::ColorMedianFilter_I(int theWidth, int theHeight)
		try	:	ColorMedianFilter(theWidth, theHeight, true),
				//
				redData_(new unsigned char[theWidth*theHeight]),
				greenData_(new unsigned char[theWidth*theHeight]),
				blueData_(new unsigned char[theWidth*theHeight])				
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorMedianFilter_I constructor");
	throw e;
}

ColorMedianFilter_I::ColorMedianFilter_I(const ImageRect* theRect)
		try	:	ColorMedianFilter(theRect, false, kPositiveFloat),
				//
				redData_(new unsigned char[theRect->getWidth() * theRect->getHeight()]),
				greenData_(new unsigned char[theRect->getWidth() * theRect->getHeight()]),
				blueData_(new unsigned char[theRect->getWidth() * theRect->getHeight()])				
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorMedianFilter_I constructor");
	throw e;
}


ColorMedianFilter_I::ColorMedianFilter_I(const ColorMedianFilter_I& theObj)
			:	ColorMedianFilter(),
				//
				redData_(NULL),
				greenData_(NULL),
				blueData_(NULL)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorMedianFilter_I copy constructor not implemented.");
}


ColorMedianFilter_I::~ColorMedianFilter_I(void)
{
    delete []redData_;
    delete []greenData_;
    delete []blueData_;
}


const ColorMedianFilter_I& ColorMedianFilter_I::operator = (const ColorMedianFilter_I& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorMedianFilter_I copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


RasterImage* ColorMedianFilter_I::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to ColorMedianFilter_I::applyTo");
    
	return applyTo(imgIn, imgIn->getValidRect());
}


RasterImage* ColorMedianFilter_I::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in ColorMedianFilter_F::applyTo");

	RasterImage	*imgOut = NULL;
	try {
		if (imgIn->hasFloatRaster())
		{
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
		}
		else
		{
			switch (imgIn->getBaseRasterType())
			{
				case kGrayRaster:
					imgOut = new RasterImage_gray(imgIn->getBoundRect());
					break;
					
				case kRGB24Raster:
					imgOut = new RasterImage_RGB(imgIn->getBoundRect());
					break;
					
				case kRGBa32Raster:
					imgOut = new RasterImage_RGBa(imgIn->getBoundRect());
					break;
					
				default:
					FAIL_CONDITION(	true,
									kFunctionNotImplemented,
									"image type not suported by Gaussian filter yet");
					break;
					
			}
		}
		
		applyInto(imgIn, theRect, imgOut);

    }
	catch (ErrorReport& e) {
		e.appendToMessage("called by ColorMedianFilter_I::applyTo");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kImageProcessingError,
						"Error in the computation of ColorMedianFilter_I::applyTo");
	}

	return imgOut;
}

void ColorMedianFilter_I::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");
    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}


void ColorMedianFilter_I::applyInto(const RasterImage* imgIn, const ImageRect* theRect, 
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
                    "NULL ImageRect parameter in VectorField_F::copyInto");

    FAIL_CONDITION(	imgOut == NULL,
    				kNullRasterImageError,
    				"NULL output raster image passed as parameter");

    //  so far I only accept a RasterImage_RGBa as a destination
	FAIL_CONDITION( imgOut->getBaseRasterType() != kRGBa32Raster,
	                kInvalidRasterType,
	                "GaussianFilter_F::applyInto only accepts RasterImage_RGBa images as destination");

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

    const unsigned char* const* rgbaIn = rgbaImg->getShiftedRGBa2D();
    unsigned char* const* rgbaOut = (static_cast<RasterImage_RGBa*>(imgOut))->getShiftedRGBa2D(R_W_ACCESS);
    for (int i=iLow; i<= iHigh; i++)
    {
        const int kLow = MAX(i+recTop, iLow);
        const int kHigh = MIN(i+recBottom, iHigh);
        int fourj = 4*jLow;
        for (int j=jLow; j<=jHigh; j++)
        {
            //  collect the data around the pixel.
            const int lLow = MAX(j+recLeft, jLow);
            const int lHigh = MIN(j+recRight, jHigh);
            const int fourlMin = 4*lLow;
            int  n = 0;
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
            rgbaOut[i][fourj++] = getColorMedian_(redData_, n);
            rgbaOut[i][fourj++] = getColorMedian_(greenData_, n);
            rgbaOut[i][fourj++] = getColorMedian_(blueData_, n);
            rgbaOut[i][fourj++] = static_cast<unsigned char>(255);
        }
    }


	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
	imgOut->setValidRect(outRect);	    

    //  cleanup
    if (localRGBa)
        delete rgbaImg;
        
}

//  Computation of the median using N. Wirth's algorithm
//  Implementation derived from kth_smallest implementation of N Devillard.
//  Note that if the array is even-sized, the function does not return the average of
//  the two central values but the lower median.
unsigned char getColorMedian_(unsigned char* dat, int n)
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
                unsigned char temp = dat[i];
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
