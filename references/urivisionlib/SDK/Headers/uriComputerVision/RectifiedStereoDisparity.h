/*  NAME:
        RectifiedStereoDisparity.h
 
    DESCRIPTION:
        RectifiedStereoDisparity public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RECTIFIED_STEREO_DISPARITY_H
#define     URIVL_RECTIFIED_STEREO_DISPARITY_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "StereoDisparity_F.h"
#include "RectifiedStereoDisparity.h"

namespace uriVL
{
    class URIVL_EXPORT RectifiedStereoDisparity : public StereoDisparity_F,
                                                    public RectifiedStereoDisparity 
    {    
    	public:

    		/**
             *  Constructor.
             */
    		RectifiedStereoDisparity(void);

    		/**
             *  Cloner constructor.
             *  Allocates a disparity with a confidence.
             *  @param  disp    disparity object to clone
             */
    		RectifiedStereoDisparity(RectifiedStereoDisparity disp);

    		/**
             *  Constructor.
             *  Allocates a disparity with a confidence.
             *  @param  disp        disparity
             *  @param  conf        confidence
             *  @param  l2r         true if the disparity stored works left
    		 *	                    to right, false if the disparity stored 
    		 *                      works right to left
             */
    		RectifiedStereoDisparity(float disp, float conf, bool l2r);


    		/**
             *  Destructor
             */          
    		virtual ~RectifiedStereoDisparity_F(void);

    		/**
             *  Returns the disparity
             *  @return     disparity
             */
    		float getDisparity1DF(void);
    	
            /** Assigns new disparity
             *  @param  d   horizontal disparity
             */
            void setDisparity(float d);


        protected:
        
    		
        	/**	Stores displacement between the left and right
        	 *	matching points
        	 *	@see	isLeftToRight_
        	 */
    		float fd_;       
    };
}
#endif	//	RECTIFIED_STEREO_DISPARITY_H
