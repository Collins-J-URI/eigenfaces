/*  NAME:
        RectifiedStereoMatcher_JJL.h
 
    DESCRIPTION:
        RectifiedStereoMatcher_JJL public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RECTIFIED_STEREO_MATCHER_JJL_H
#define     URIVL_RECTIFIED_STEREO_MATCHER_JJL_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RectifiedStereoMatcher.h"

namespace uriVL
{
   
    class URIVL_EXPORT RectifiedStereoMatcher_JJL : public RectifiedStereoMatcher 
    {
        public:
        	 /**
             *  Constructor.
             *  Creates an image stereo matcher.
             *  @param  rect            image rectangle for the matching
             *  @param  minWinWidth     Minimum width of the window
             *  @param  maxWinWidth     Maximum width of the window
             *  @param  minWinHeight    Minimum height of the window
             *  @param  maxWinHeight    Maximum height of the window
             *  @param  maxDisp         Maximum number of disparity
             */
        	RectifiedStereoMatcher_JJL(ImageRect* rect, int minWinWidth,int maxWinWidth,
        	                            int minWinHeight, int maxWinHeight, int maxDisp);
        	/**
             *  Destructor
             */          
        	virtual ~RectifiedStereoMatcher_JJL(void);
        	
    		float match(RectifiedStereoPair *pair, StereoImageID l_or_r, ImagePoint* pt);
            void match(RectifiedStereoPair *pair, StereoImageID l_or_r, ImageRect* rect,
            			RasterImage_gray_F *disparityMap);
    		float match(StereoImageID l_or_r, ImagePoint* pt);
            void match(StereoImageID l_or_r, ImageRect* rect, RasterImage_gray_F *disparityMap);
            void setStereoPair(RectifiedStereoPair *pair);
            
            /** Assigns the stereo pair and processing rectangle for this matcher object
             *  @param  pair    reference to the image pair to process
             *  @param  rect    image rectangle for the match
             */
            void setStereoPair(RectifiedStereoPair *pair, ImageRect* rect);
            
//temporary stuff
RasterImage_gray_F *getIntegralImage(int d);
            
		protected:
			ImageRect	*rect_;
			
			RasterImage_gray	*leftImage_;
			
			RasterImage_gray	*rightImage_;
			
			/** Minimum width of the window
			 */
			int minWinWidth_;
			/** Maximum width of the window
			 */
			int maxWinWidth_;
			/** Minimum height of the window
			 */
			int minWinHeight_;
			/** Maximum height of the window
			 */
			int maxWinHeight_;
			/** Maximum number of disparity
			 */
			int maxDisp_;
			/** error intergral image
			*/
			RasterImage_gray_F	**errorIntegralImage_;
			
			/** square error intergral image
			*/
			RasterImage_gray_F  **sqrErrorIntegralImage_;
			
			float   ***integralError_;
			
			float   ***integralSqrError_;
			
			float   *prevRowSum_;
			
			float   *prevRowSqSum_;	
			
			float   *prevColSum_;
			
			float   *prevColSqSum_;		
                        
            void computeIntegralImage_(int disp);
            
            void calculateError_(unsigned char** leftGray, unsigned char** rightGray, 
                                int d, int iLow, int iHigh, int jLow, int jHigh);
            
    };
}
#endif	//	RECTIFIED_STEREO_MATCHER_JJL_H
