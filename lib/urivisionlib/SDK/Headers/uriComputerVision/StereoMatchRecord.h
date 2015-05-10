/*  NAME:
        StereoMatchRecord.h
 
    DESCRIPTION:
        StereoMatchRecord public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_STEREO_MATCH_RECORD_H
#define     URIVL_STEREO_MATCH_RECORD_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImagePoint.h"


namespace uriVL
{

    /** A StereoMatchRecord stores a matching point and matching score resulting
     *  from a call to a stereo matcher
     */   
    class URIVL_EXPORT StereoMatchRecord
    {
        public:
        
        	/**	Default constructor.  Should only be used by subclasses
        	 *	that don't need to allocate the disparity point
        	 */
    		StereoMatchRecord(void);
    		
    		/**
             *  Cloner constructor.
             *  @param  disp    disparity object to clone
             */
    		StereoMatchRecord(StereoMatchRecord *disp);

    		/**
             *  Constructor.
             *  Allocates a stereo disparity with a confidence.
             *  @param  disp        disparity
             *  @param  conf        confidence
             *  @param  isLeft      true if this match was computed in the left image
             *                      (right to left stereo match)
             */
    		StereoMatchRecord(ImagePoint* disp, float conf, bool isLeft);

    		/**
             *  Constructor.
             *  Allocates a disparity with a confidence.
             *  @param  dx        horizontal component of disparity
             *  @param  dy        vertical coordinate of disparity
             *  @param  conf        confidence
             *  @param  isLeft      true if this match was computed in the left image
             *                      (right to left stereo match)
             */
    		StereoMatchRecord(int dx, int dy, float conf, bool isLeft);


    		/** Destructor
             */
    		~StereoMatchRecord(void);
    		
    		 
            /**
             *  Indicates whether the disparity stored works from the left to the right
             *  @return  true if the disparity stored works from the left to the right
             */
    		bool isLeftMatch(void);
    		

    	    /** Indicates whether this match is computed for a rectified stereo system.
    	     *  @return     <code>true</code> if this match was computed for a rectified
    	     *              stereo system
    	     */
    	    bool isRectifiedStereoMatch(void);

    	    /** Sets the flag indicating whether this match is computed for a rectified 
    	     *  stereo system.    	     
    	     *  @param isSubpixel     true if this match is computed for a rectified
    	     *                          stereo pair
    	     */
    	    void setIsRectifiedStereoMatch(bool isRect);


    	    /** Indicates whether the match computed uses integers of floating point
    	     *  (subpixel) numbers
    	     *  @return     true if the stereo matcher computes subpixel disparity
    	     */
    	    bool isSubpixelMatch(void);

    	    /** Sets the flag indicating whether this is a subpixel disparity record.
    	     *  Typically set by a StereoMatcher object.
    	     *  @param isSubpixel     true if this disparity can return a floating point,
    	     *                          subpixel disparity value
    	     */
    	    void setIsSubpixelMatch(bool isSubpixel);


            /** Copies into this object the data from the one received as parameter
             *  @param  the object to copy data from
             */
            void copy(StereoMatchRecord *disp);


            /** Returns the score of the best match found
             */
            float getScore(void);

            /** Returns the location of the best match
             */
            ImagePoint* getBestMatch(void);

            /** Sets the score
             *  @param  score   the new score for this record
             */
            void setScore(float score);

            /** Sets the coordinates of the best match
             *  @param  x   new x coordinate for this record's best match point
             *  @param  y   new y coordinate for this record's best match point
             */
            void setBestMatch(int x, int y);

            /** Sets the coordinates of the best match
             *  @param  bestMatch   new best match point for this record
             */
            void setBestMatch(ImagePoint* bestMatch);

    	protected:
        
        	/**	Stores the best match found
        	 */
    		ImagePoint	*bestMatch_;
    		
    		/**	Matching confidence, in [0, 1]
    		 */
    		float	score_;       


            /** Indicates whether the stereo match was calculated with subpixel
             *  accuracy
             */
            bool isSubpixelMatch_;
            
            
    		/**	True if this object stores a point in the left image
    		 */
    		bool isLeftMatch_;
    		
    		/** Indicates whether this disparity is computed for a rectified stereo system.
             */
            bool isRectifiedStereoMatch_;

           /**
             *  Copy Constructor. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            StereoMatchRecord(const StereoMatchRecord& theObj);


            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const StereoMatchRecord& operator = (const StereoMatchRecord& theObj);

    };
}
#endif  //  STEREO_MATCH_RECORD_H
