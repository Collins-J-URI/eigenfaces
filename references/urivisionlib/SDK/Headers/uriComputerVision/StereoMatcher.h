/*  NAME:
        StereoMatcher.h
 
    DESCRIPTION:
        StereoMatcher public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_STEREO_MATCHER_H
#define     URIVL_STEREO_MATCHER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "VectorField_F.h"
#include "VectorField_I.h"
#include "ImagePoint.h"
#include "StereoPair.h"
#include "StereoDisparity.h"
#include "StereoMatchRecord.h"
#include "PatternMatcher2D.h"
#include "ConfidenceRecord.h"


namespace uriVL
{
    /** Pure virtual parent class for all classes that implement a stereo matching 
     *  algorithm.
     */
    class URIVL_EXPORT StereoMatcher
    {
        public:

			/** Creates a new stereo matcher and set the stereo pair on which to operate.
             *  @param  pair    reference to the image pair to process
             */
			StereoMatcher(const StereoPair* stereoPair);

			/**
             *  Destructor
             */          
			~StereoMatcher(void);
           
            /** Assigns the stereo pair and processing rectangle for this matcher object
             *  @param  stereoPair    reference to the image pair to process
             */
            virtual void setStereoPair(const StereoPair* stereoPair);
            
            /** Computes the stereo match for the point indicated.
             *  @param  stereoPair  the stereo pair on which to operate
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image
             *  @return     the best match (and score) for the point indicated
             */
            StereoMatchRecord *match(const StereoPair* stereoPair, ImagePoint* pt, 
                                   StereoImageID l_or_r, 
                                   const ImageRect* searchRect = NULL);

            /** Computes the stereo match for the point indicated for the stereo pair
             *  set by an earlier call to setStereoPair (hopefully).
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image
             *  @return     the best match (and score) for the point indicated
             */
            StereoMatchRecord *match(ImagePoint* pt, StereoImageID l_or_r, 
                                   const ImageRect* searchRect = NULL);


            /** Computes the stereo match for the point indicated.
             *  @param  stereoPair  the stereo pair on which to operate
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  matchRec    the best match (and score) for the point indicated
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image
             */
            void match(StereoPair *stereoPair, ImagePoint* pt, StereoImageID l_or_r, 
            		           StereoMatchRecord *matchRec, const ImageRect* searchRect = NULL);

            /** Computes the stereo match for the point indicated for the stereo pair
             *  set by an earlier call to setStereoPair (hopefully).
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  matchRec    the best match (and score) for the point indicated
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image
             */
            void match(ImagePoint* pt, StereoImageID l_or_r, StereoMatchRecord *matchRec,
                               const ImageRect* searchRect = NULL);

            /** Computes the stereo disparity at the point indicated.
             *  @param  stereoPair  the stereo pair on which to operate
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image
             *  @return     the disparity (and score) for the point indicated
             */
            StereoDisparity *disparity(StereoPair *stereoPair, ImagePoint* pt, 
                                   StereoImageID l_or_r, 
                                   const ImageRect* searchRect = NULL);

            /** Computes the stereo disparity at the point indicated for the stereo pair
             *  set by an earlier call to setStereoPair (hopefully).
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image
             *  @return     the disparity (and score) for the point indicated
             */
            StereoDisparity *disparity(ImagePoint* pt, StereoImageID l_or_r, 
                                   const ImageRect* searchRect = NULL);


            /** Computes the stereo disparity at the point indicated.
             *  @param  stereoPair  the stereo pair on which to operate
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  disp        the disparity (and score) for the point indicated
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image
             */
            void disparity(StereoPair *stereoPair, ImagePoint* pt, StereoImageID l_or_r, 
            		           StereoDisparity *disp, const ImageRect* searchRect = NULL);

            /** Computes the stereo disparity at the point indicated for the stereo pair
             *  set by an earlier call to setStereoPair (hopefully).
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  disp        the disparity (and score) for the point indicated
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image
             */
            void disparity(ImagePoint* pt, StereoImageID l_or_r, StereoDisparity *disp,
                               const ImageRect* searchRect = NULL);


/*
            VectorField *match(StereoPair *stereoPair, StereoImageID l_or_r, 
            							ConfidenceRecord *conf);
            							
            VectorField *match(StereoPair *stereoPair, StereoImageID l_or_r, 
            							ImageRect* rect, ConfidenceRecord *conf);
            							
            void match(StereoPair *stereoPair, StereoImageID l_or_r, 
            					VectorField *matchOut, ConfidenceRecord *conf);

            void match(StereoPair *stereoPair, StereoImageID l_or_r, ImageRect* rect, 
            					VectorField *matchOut, ConfidenceRecord *conf);

*/
	
			/**	Indicates whether this stereo matcher computes integer or float (subpixel)
        	 *	disparity values
        	 *  @return     true if this matcher computes a subpixel disparity
        	 */
        	bool isSubpixelMatcher(void);
        	


         protected:
        	/**
             *  a stereo match
             */
        	PatternMatcher2D* matcher_;
        	
            /** StereoPair for this matcher to operate on (if one was set)
             */
            StereoPair *stereoPair_;
            
            /** Search rectangle stored during a call to match
            */
            ImageRect* searchRect_;
		    /** StereoMatchRecord record used to call private match function.   This object
		     *  is reused rather than being created at each call of the public match functions.
		     */
		    StereoMatchRecord *matchRecord_;
		
            
        	/**	Indicates whether this stereo matcher computes integer or float (subpixel)
        	 *	disparity values.  SShould be initialized by subclasses implementing
        	 *  an algorithm.
        	 */
    	    bool    isSubpixelMatcher_;
        	
        	
		

			/** Default Constructor.
             *  Creates a new stereo matcher.
             */
			StereoMatcher(void);

           /**
             *  Copy Constructor. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            StereoMatcher(const StereoPair& theObj);


            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const StereoMatcher& operator = (const StereoMatcher& theObj);
			
			
        	/**	Computes a search rectangle for the target point
        	 *	@param	pt		image point to find a match for
        	 *	@param	imgRect	valid data rect (or computation rectangle) of the image.
        	 *                  Defaults to the entire image
        	 *	@param	l_or_r	is pt defined in the left or right image?
        	 *	@param	rect	search rectangle for the point to match
        	 */
        	virtual void computeSearchRect_(ImagePoint* pt, const ImageRect* imgRect, 
        									const StereoImageID l_or_r, ImageRect* rect=  NULL) = 0;

            /** Function implemented by subclasses that computes the stereo disparity at 
             *  the point indicated for the stereo pair set by an earlier call to setStereoPair 
             *  (hopefully).
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  matchRec    the best match (and score) for the point indicated
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image.  
             */
            virtual void match_(StereoPair *stereoPair, ImagePoint* pt, const StereoImageID l_or_r, 
            				    StereoMatchRecord *matchRec, const ImageRect* searchRect=NULL) = 0;

/*
            virtual void match_(StereoPair *imgPair, StereoImageID l_or_r, ImageRect* rect, 
            					VectorField *matchOut, ConfidenceRecord *conf) = 0;

*/
    };
}
#endif	//	STEREO_MATCHER_H
