/*  NAME:
        StereoDisparity.h
 
    DESCRIPTION:
        StereoDisparity public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_STEREO_DISPARITY_H
#define     URIVL_STEREO_DISPARITY_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Disparity.h"


namespace uriVL
{
    /**	Possible configurations for two cameras that can lead to a stereo
	 *	disparity.  Generally in stereo systems one camera is the "left camera" and
	 *	the other is the "right camera."  In that case I may want to compute the disparity
	 *	"left to right or right to left."    But the cameras could also be disposed in a 
	 *	vertical configuration with a "top camera" and a "bottom camera," in which case
	 *	the disparity could be computed "top to bottom" or "botom to top."  Finally,
	 *	for those other "Generic" configurations the disparity can be computed
	 "	from image 1 to image 2 or from image 2 to image 1.
	 */
	typedef enum StereoDisparityType {
										HORIZONTAL_L2R = 0,
										HORIZONTAL_R2L,
										VERTICAL_T2B,
										VERTICAL_B2T,
										//	In the following 2 configs, one camera is artitrarily 
										//	identified as "left camera" and the other as "right camera"
										GENERIC_1_2,
										GENERIC_2_1
	} StereoDisparityType;
										
	
	/**	A StereoDisparity is established between two static cameras.
	 *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT StereoDisparity : public Disparity 
    {
    	public:
    	
        	/**	Constructor.  Should only be used by subclasses
        	 *	that don't need to allocate the disparity point
             *  @param  type       the type of this diparity object
			 *	@param isRect		true if the stereo system is rectified
        	 */
    		StereoDisparity(StereoDisparityType type, bool isRect = true);
    		
    		/**
             *  Copy constructor.
             *  @param  disp    disparity object to copy
             */
    		StereoDisparity(const StereoDisparity* disp);

    		/**
             *  Copy constructor.
             *  @param  disp    disparity object to copy
             */
    		StereoDisparity(const StereoDisparity& disp);

    		/**
             *  Constructor.
             *  Allocates a stereo disparity with a confidence.
             *  @param  disp        disparity as a 2D vector
             *  @param  conf        confidence
             *  @param  type       the type of this diparity object
			 *	@param isRect		true if the stereo system is rectified
             */
    		StereoDisparity(const Vector2D* disp, float conf, StereoDisparityType type, bool isRect = true);

    		/**
             *  Constructor.
             *  Allocates a disparity with a confidence.
             *  @param  dx        horizontal component of disparity
             *  @param  dy        vertical coordinate of disparity
             *  @param  conf        confidence
             *  @param  type       the type of this diparity object
			 *	@param isRect		true if the stereo system is rectified
             */
    		StereoDisparity(int dx, int dy, float conf, StereoDisparityType type, bool isRect = true);

    		/**
             *  Constructor.
             *  Allocates a subpixel disparity with a confidence.
             *  @param  dx        horizontal component of disparity
             *  @param  dy        vertical coordinate of disparity
             *  @param  conf        confidence
             *  @param  type       the type of this diparity object
			 *	@param isRect		true if the stereo system is rectified
             */
    		StereoDisparity(float dx, float dy, float conf, StereoDisparityType type, bool isRect = true);


    		/** Destructor
             */
    		~StereoDisparity(void);

    		/**
             *  Copy operator.
             *  @param  disp    disparity object to copy
             */
    		const StereoDisparity& operator = (const StereoDisparity& disp);
    		
    		 
            /**
             *  Indicates whether the disparity stored works from the left to the right
             *  @return  true if the disparity stored works from the left to the right
             */
    		StereoDisparityType getType(void) const;
    		

    	    /** Indicates whether this disparity is computed for a rectified stereo system.
    	     *  If <ccode>true</code> this indicates that the disparity can be obtained
    	     *  as a single number
    	     *  @return     <code>true</code> if this disparity was computed for a rectified
    	     *              stereo system
    	     */
    	    bool isRectifiedStereoDisparity(void) const;




    	private:
        
    		/**	Type of disparity stored
    		 */
    		StereoDisparityType type_;
    		
    		/** Indicates whether this disparity is computed for a rectified stereo system.
             */
            bool isRectifiedStereoDisparity_;

    };
}
#endif	//	STEREO_DISPARITY_H
