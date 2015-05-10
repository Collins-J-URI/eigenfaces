/*  NAME:
        RectifiedStereoMatcher.h
 
    DESCRIPTION:
        RectifiedStereoMatcher public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RECTIFIED_STEREO_MATCHER_H
#define     URIVL_RECTIFIED_STEREO_MATCHER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "StereoMatcher.h"
#include "RectifiedStereoPair.h"
#include "StereoDisparity.h"

namespace uriVL
{
   
    /** Pure virtual parent class for all classes that implement a stereo matching 
     *  algorithm for rectified stereo pairs.
     *  A rectified stereo pair corresponds to cameras (whether real or idealized)
     *  that have the same focal length, frame grabber dimensions, paralllel optical
     *  axes, their baseline along their common X axis, and no image distortion.
     *  As a consequence, matching points are expected have the same y coordinate
     *  (give or take a pixel for sampling error).
     */
    class URIVL_EXPORT RectifiedStereoMatcher : public StereoMatcher 
    {
        public:

			/** Creates a new rectified stereo matcher and set the rectified stereo pair 
			 *  on which to operate.
             *  @param  pair    reference to the image pair to process
             */
			RectifiedStereoMatcher(RectifiedStereoPair *pair);

            /** Destructor
             */
			virtual ~RectifiedStereoMatcher(void);
           
        
            /** Assigns the stereo pair and processing rectangle for this matcher object.
             *  The call fails if the pair passed is not rectified.  This function
             *  overrides the one defined in the parent class.
             *  @param  pair    reference to the image pair to process
             */
            virtual void setStereoPair(StereoPair *pair);
            
			/**	assigns a maximum value to the disparity to restrict the search rectangle
			 *	@param	maxDisparity	new upper bound for disparity
			 */
			void setMaxDisparity(int maxDisparity);



		protected:
		
			/**	upper bound for the disparity
			 */
			int maxDisparity_;
			
			/** Default Constructor.
             *  Creates a new rectified stereo matcher.
             */
			RectifiedStereoMatcher(void);

		
    };
}
#endif	//	RECTIFIED_STEREO_MATCHER_H
