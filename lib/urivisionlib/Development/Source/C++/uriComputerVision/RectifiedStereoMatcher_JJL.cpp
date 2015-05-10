/*  NAME:
        RectifiedStereoMatcher_JJL.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RectifiedStereoMatcher_JJL class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include    "RectifiedStereoMatcher_JJL.h"
#include    <cmath>

#error Work in progress: do not use this class yet

using namespace uriVL;

RectifiedStereoMatcher_JJL::RectifiedStereoMatcher_JJL(ImageRect* rect, int minWinWidth,
                                    int maxWinWidth, int minWinHeight, int maxWinHeight, 
                                    int maxDisp)
    :   RectifiedStereoMatcher()
{
    //  Clone operation rectangle
	rect_ = new ImageRect(rect);
	
	//  copy min and max matching window dimensions
	minWinWidth_ = minWinWidth,
	maxWinWidth_ = maxWinWidth,
    minWinHeight_ = minWinHeight,                                 
    maxWinHeight_ = maxWinHeight,
    
    maxDisp_ = maxDisp;
    
    //  allocate integral images
    errorIntegralImage_ = new RasterImage_gray_F*[maxDisp+1];
    sqrErrorIntegralImage_ = new RasterImage_gray_F*[maxDisp+1];
    for (int i=0; i<=maxDisp; i++)
    {                             
    	errorIntegralImage_[i] = new RasterImage_gray_F(rect_);
    	sqrErrorIntegralImage_[i] = new RasterImage_gray_F(rect_);
    }
    
    //  then store the 2D rasters of these integral images
    //  (so that we won't have to reallocate/initialize them
    //  in the match functions
    integralError_ = new float** [maxDisp+1];
    integralSqrError_ = new float** [maxDisp+1];
    for (int i=0; i<=maxDisp; i++)
    {                             
    	integralError_[i] = errorIntegralImage_[i]->getShiftedGrayF2D();
    	integralSqrError_[i] = sqrErrorIntegralImage_[i]->getShiftedGrayF2D();
    }
    prevColSum_ = new float[rect_->getHeight()];
    prevColSum_ -= rect_->getTop();
    prevColSqSum_ = new float[rect_->getHeight()];
    prevColSqSum_ -= rect_->getBottom();
    
    prevRowSum_ = new float[rect_->getWidth()];
    prevRowSum_ -= rect_->getLeft();
    prevRowSqSum_ = new float[rect_->getWidth()];
    prevRowSqSum_ -= rect_->getLeft();
}

RectifiedStereoMatcher_JJL::~RectifiedStereoMatcher_JJL(void)
{
	if (rect_ != NULL)
		delete rect_;
		
	if (integralError_ != NULL)
	{
	    for (int i = 0; i<=maxDisp_; i++)
	        delete integralError_[i];
		delete []integralError_;
	}	
	if (integralSqrError_ != NULL)
	{
	    for (int i = 0; i<=maxDisp_; i++)
	        delete integralSqrError_[i];
		delete []integralSqrError_;
	}	
		
    if (integralError_ != NULL)
        delete []integralError_;
    if (integralSqrError_ != NULL)
        delete []integralSqrError_;
        
    if (prevRowSum_ != NULL)
    {
        prevRowSum_ += rect_->getLeft();
        delete []prevRowSum_;
    }
    if (prevRowSqSum_ != NULL)
    {
        prevRowSqSum_ += rect_->getLeft();
        delete []prevRowSqSum_;
    }
    if (prevColSum_ != NULL)
    {
        prevColSum_ += rect_->getTop();
        delete []prevColSum_;
    }
    if (prevColSqSum_ != NULL)
    {
        prevColSqSum_ += rect_->getTop();
        delete []prevColSqSum_;
    }
}

#if 0
#pragma mark -
#endif

float RectifiedStereoMatcher_JJL::match(RectifiedStereoPair *imgPair, StereoImageID l_or_r, ImagePoint* pt)
{
	FAIL_CONDITION( imgPair == NULL,
    				kNullStereoPairError,
    				"NULL stereo pair passed as parameter");
	ImageRect	*destRect = imgPair->getValidRect(l_or_r);
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in RectifiedStereoMatcher_JJL::setStereoPair");
	
	setStereoPair(imgPair, destRect);

	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"not implemented yet");
				
	match(l_or_r, pt);
}            				
           
void RectifiedStereoMatcher_JJL::match(RectifiedStereoPair *imgPair, StereoImageID l_or_r, 
                                        ImageRect* rect, RasterImage_gray_F *disparityMap)
{
	FAIL_CONDITION( imgPair == NULL,
    				kNullStereoPairError,
    				"NULL stereo pair passed as parameter");
   
	setStereoPair(imgPair, rect);
	
	//	calculate disparity
	match(l_or_r, rect, disparityMap);
}            				

float RectifiedStereoMatcher_JJL::match(StereoImageID l_or_r, ImagePoint* pt)
{
	FAIL_CONDITION( pt == NULL,
    				kNullStereoPairError,
    				"NULL point passed as parameter");
    
	ImageRect	*destRect;
	if (l_or_r == 0)
		destRect = leftImage_->getValidRect();
	else
		destRect = rightImage_->getValidRect();
    
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in RectifiedStereoMatcher_JJL::setStereoPair");
	
	//	calculate disparity at this point
}            				
           
void RectifiedStereoMatcher_JJL::match(StereoImageID l_or_r, ImageRect* rect, 
                                        RasterImage_gray_F *disparityMap)
{
	FAIL_CONDITION( disparityMap == NULL,
    				kNullStereoPairError,
    				"NULL disparity map passed as destination");
   
	//	calculate disparity map
}            				

#if 0
#pragma mark -
#endif

void RectifiedStereoMatcher_JJL::setStereoPair(RectifiedStereoPair *imgPair)
{
	FAIL_CONDITION( imgPair == NULL,
    				kNullStereoPairError,
    				"NULL stereo pair passed as parameter");
	ImageRect	*destRect = imgPair->getValidRect(LEFT_IMAGE);
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in RectifiedStereoMatcher_JJL::setStereoPair");

	setStereoPair(imgPair, destRect);
}

void RectifiedStereoMatcher_JJL::setStereoPair(RectifiedStereoPair *imgPair, ImageRect* rect)
{
    int d;
    
	if (!rect_->contains(rect))
	{
	for (d=0; d<=maxDisp_; d++)
	    {
		    errorIntegralImage_[d]->setBoundRect(rect);
		    sqrErrorIntegralImage_[d]->setBoundRect(rect);
		    
    	 	integralError_[d] = errorIntegralImage_[d]->getShiftedGrayF2D();
        	integralSqrError_[d] = sqrErrorIntegralImage_[d]->getShiftedGrayF2D();
      	}
	}
		
	leftImage_ = (RasterImage_gray* ) (imgPair->getLeftImage());
	rightImage_ = (RasterImage_gray* ) (imgPair->getRightImage());
	
	//	calculate integral image
	for (d=0; d<=maxDisp_; d++)
	    computeIntegralImage_(d);
	
	//  update valid data rectangles
	for (d=0; d<=maxDisp_; d++)
	    errorIntegralImage_[d]->setValidRect(rect);
	    sqrErrorIntegralImage_[d]->setValidRect(rect);
}

void RectifiedStereoMatcher_JJL::computeIntegralImage_(int disp)
{
    int   iLow = rect_->getTop(),
            iHigh = rect_->getBottom(),
            jLow = rect_->getLeft() + disp,
            jHigh = rect_->getRight(),
            newWidth = jHigh - jLow + 1,
            height = iHigh - iLow + 1,
            i, j;
    unsigned char   **leftGray = leftImage_->getShiftedGray2D();
    unsigned char   **rightGray = rightImage_->getShiftedGray2D();
    
    //  For each disparity considered
    for (int d=0; d<=maxDisp_; d++)
    {
        //  calculate the error at each pixel
        calculateError_(leftGray, rightGray, d, iLow, iHigh, jLow, jHigh);

        //  perform the summation along the columns
        for (i=iLow;i<=iHigh; i++)
        {
            prevColSum_[i]= 0;
            prevColSqSum_[i] = 0;
        }
        for (j=jLow;j<=jHigh; j++)
        {
            for (i=iLow;i<=iHigh; i++)
            {
                integralError_[disp][i][j] += prevColSum_[j];
                integralSqrError_[disp][i][j] += prevColSqSum_[j];
                prevColSum_[j] = integralError_[disp][i][j];
                prevColSqSum_[j] = integralSqrError_[disp][i][j];
            }
        }
        
        //  perform the summation along the rows
        for (j=jLow;j<=jHigh; j++)
        {
            prevRowSum_[j]= 0;
            prevRowSqSum_[j] = 0;
        }
        for (i=iLow;i<=iHigh; i++)
        {
            for (j=jLow;j<=jHigh; j++)
            {
                integralError_[disp][i][j] += prevRowSum_[j];
                integralSqrError_[disp][i][j] += prevRowSqSum_[j];
                prevRowSum_[j] = integralError_[disp][i][j];
                prevRowSqSum_[j] = integralSqrError_[disp][i][j];
            }
        }
        
        //  set valid data rectangles    
	    errorIntegralImage_[d]->setValidRect(jLow, iLow, newWidth, height);
	    sqrErrorIntegralImage_[d]->setValidRect(jLow, iLow, newWidth, height);
	    errorIntegralImage_[d]->setFloatConversionMode(kPositiveFloat);
	    sqrErrorIntegralImage_[d]->setFloatConversionMode(kPositiveFloat);
    }
}

RasterImage_gray_F *RectifiedStereoMatcher_JJL::getIntegralImage(int d)
{
    return errorIntegralImage_[d];
}

void RectifiedStereoMatcher_JJL::calculateError_(unsigned char** leftGray, 
                                    unsigned char** rightGray,  int d, int iLow, 
                                    int iHigh, int jLow, int jHigh)
{
    int i,j;
	float a,b,c,errorL,errorR,minError;
	
	for (i=iLow; i<=iHigh; i++)
	{

		for (j=jLow; j<=jHigh; j++)
		{
			
			a = abs(leftGray[i][j] - rightGray[i][j]);
			if (j != jLow)
			b = abs(leftGray[i][j] - (rightGray[i][j-d] + rightGray[i][j-d-1])*0.5);
			else
			b = a;
			if (j != jHigh)
			    c = abs(leftGray[i][j] - (rightGray[i][j-d] + rightGray[i][j-d+1])*0.5);
			else
			    c = a;
			
			errorL = MIN(a,c);
			if (b <errorL)
			    errorL = b;
			    
			if (j != jLow)
			    b = abs((leftGray[i][j] + leftGray[i][j-1])*0.5 - rightGray[i][j-d]);
			else
			    b = a;
			if ( j != jHigh)
			    c = abs((leftGray[i][j] + leftGray[i][j+1])*0.5 - rightGray[i][j-d]);
			else
			    c = a;
			errorR = MIN(a,c);
			if (b <errorR)
			    errorR = b;
			
			minError = MIN(errorL,errorR);
			
			integralError_[d][i][j] = minError;
			integralSqrError_[d][i][j] = minError*minError;	
		}
	}      
}