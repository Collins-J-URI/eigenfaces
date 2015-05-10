/*  NAME:
        MedianFilter_I.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib MedianFilter_I class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "MedianFilter_I.h"
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
unsigned char getMedian_(unsigned char* dat, int n);


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


MedianFilter_I::MedianFilter_I(int theWidth)
		try	:	MedianFilter(theWidth, theWidth, true),
				//
				data_(new unsigned char[theWidth*theWidth])
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by MedianFilter_I constructor");
	throw e;
}


MedianFilter_I::MedianFilter_I(int theWidth, int theHeight)
		try	:	MedianFilter(theWidth, theHeight, true, kPositiveFloat),
				//
				data_(new unsigned char[theHeight*theWidth])
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by MedianFilter_I constructor");
	throw e;
}


MedianFilter_I::MedianFilter_I(const ImageRect* theRect)
		try	:	MedianFilter(theRect, true, kPositiveFloat),
				//
				data_(new unsigned char[theRect->getWidth()*theRect->getHeight()])
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by MedianFilter_I constructor");
	throw e;
}


MedianFilter_I::MedianFilter_I(const MedianFilter_I& theObj)
			:	MedianFilter(),
				//
				data_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "MedianFilter_I copy constructor not implemented.");
}



MedianFilter_I::~MedianFilter_I(void)
{
    delete []data_;
}

const MedianFilter_I& MedianFilter_I::operator = (const MedianFilter_I& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "MedianFilter_I copy operator not implemented.");

	return *this;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


RasterImage* MedianFilter_I::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to MedianFilter_I::applyTo");

	return applyTo(imgIn, imgIn->getValidRect());
}


RasterImage* MedianFilter_I::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter to MedianFilter_I::applyTo");

    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in to MedianFilter_I::applyTo");

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
									"image type not suported by median filter yet");
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
									"image type not suported by median filter yet");
					break;
					
			}
		}
		
		applyInto(imgIn, theRect, imgOut);

	}
	catch (ErrorReport& e) 
	{
		e.appendToMessage("called by MedianFilter_I::applyTo");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kImageProcessingError,
						"Error in the computation of MedianFilter_I::applyTo");
	}

	return imgOut;
}

void MedianFilter_I::applyInto(const RasterImage* imgIn, RasterImage* imgOut)
{
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");
    applyInto(imgIn, imgIn->getValidRect(), imgOut);
}


void MedianFilter_I::applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut)
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

    //  so far I only accept a RasterImage_gray[_F] as a destination
	FAIL_CONDITION( imgOut->getBaseRasterType() != kGrayRaster,
	                kInvalidRasterType,
	                "GaussianFilter_F::applyInto only accepts RasterImage_gray images as destination");

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
	//	STEP 2:		Get a gray-level input image
	//----------------------------------------------------------------
    bool    localGray = false;
    const RasterImage_gray* grayImg = RasterImage_gray::localRasterGray(imgIn, &localGray);

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
                
    const unsigned char* const* iGrayIn = grayImg->getShiftedGray2D();
    unsigned char* const* iGrayOut = (static_cast<RasterImage_gray*>(imgOut))->getShiftedGray2D(R_W_ACCESS);
    
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
            int  n = 0;
            //
            for (int k=kLow; k<=kHigh; k++)
                for (int l=lLow; l<=lHigh; l++)
                    data_[n++] = iGrayIn[k][l];

            iGrayOut[i][j] = getMedian_(data_, n);
        }
    } 

	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
	imgOut->setValidRect(outRect);
	imgOut->requestRasterUpdate(); 
    
    //  cleanup
    if (localGray)
        delete grayImg;
        
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Median computation (free function)
//------------------------------------------------------
#endif

//  Computation of the median using N. Wirth's algorithm
//  Implementation derived from kth_smallest implementation of N Devillard.
//  Note that if the array is even-sized, the function does not return the average of
//  the two central values but the lower median.
unsigned char getMedian_(unsigned char* dat, int n)
{
    const int k = (n%2) == 0 ? n/2 - 1 : n/2;
    //  Note for those who might want to reuse that code elsewhere: x should be of the
    //         same type as data_
    unsigned char x;
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
