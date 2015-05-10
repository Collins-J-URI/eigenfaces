/*  NAME:
        RectifiedStereoMatcher_corr.h
 
    DESCRIPTION:
        RectifiedStereoMatcher_corr public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RECTIFIED_STEREO_MATCHER_CORR_H
#define     URIVL_RECTIFIED_STEREO_MATCHER_CORR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RectifiedStereoMatcher.h"

namespace uriVL
{   
    class URIVL_EXPORT RectifiedStereoMatcher_corr : public RectifiedStereoMatcher 
    {
        public:
			
			/**	Constructor.  Creates a new stereo matcher that uses matching windows of the
			 *	size specified and that operates on images of the type indicated
			 *	@param	rasterType	the type of images this matcher works on (will convert
			 *							images of different type)
			 *	@param	nbRowsS		height of all match patterns for small scale
			 *	@param	nbColsS		width of all match patterns  for small scale
			 *	@param	verticalPadding	space to add on top and bottom of the search rect
			 *				to accomodate for less-than-perfect stereo rectification
			 *	@param	nbRows		height of all match patterns 
			 *	@param	nbCols		width of all match patterns 
			 *	@param	verticalPadding	space to add on top and bottom of the search rect
			 *				to accomodate for less-than-perfect stereo rectification
			 *	@param	nbRows		height of all match patterns 
			 *	@param	nbCols		width of all match patterns 
			 *	@param	verticalPadding	space to add on top and bottom of the search rect
			 *				to accomodate for less-than-perfect stereo rectification
			 */
        	RectifiedStereoMatcher_corr(BaseRasterType rasterType, int nbRowsS,
        								int nbColsS, int verticalPaddingS, int nbRowsM,
        								int nbColsM, int verticalPaddingM, int nbRowsL,
        								int nbColL, int verticalPaddingL);
        	
        	/**
             *  Destructor
             */          
        	virtual ~RectifiedStereoMatcher_corr(void);
        	
			
			/**
			 *	Returns the width of the matching window
			 *	@return		width of the matching window
			 */
            int getSWidth(void);

			/**
			 *	Returns the height of the matching window
			 *	@return		height of the matching window
			 */
            int getSHeight(void);
            /**
			 *	Returns the width of the matching window
			 *	@return		width of the matching window
			 */
            int getMWidth(void);

			/**
			 *	Returns the height of the matching window
			 *	@return		height of the matching window
			 */
            int getMHeight(void);

			/**
			 *	Returns the width of the matching window
			 *	@return		width of the matching window
			 */
            int getLWidth(void);

			/**
			 *	Returns the height of the matching window
			 *	@return		height of the matching window
			 */
            int getLHeight(void);


		protected:
		
			/**	Width of all matching windows (fixed)
			 */
			int	sWidth_;
			
			/**	Height of all matching windows (fixed)
			 */
			int	sHeight_;
			
			/**	Width of all matching windows (fixed)
			 */
			int	mWidth_;
			
			/**	Height of all matching windows (fixed)
			 */
			int	mHeight_;
			
			/**	Width of all matching windows (fixed)
			 */
			int	lWidth_;
			
			/**	Height of all matching windows (fixed)
			 */
			int	lHeight_;
			
			/**	Pre-allocated pattern used by this matcher
			 */
			Pattern2D* sPat_;
			
			/**	Pre-allocated pattern used by this matcher
			 */
			Pattern2D* mPat_;
			
			/**	Pre-allocated pattern used by this matcher
			 */
			Pattern2D* lPat_;
			
			/**	Image rectangle of dimensions width_ x height_ used to grab image data
			 *	about the points to match.
			 */
			ImageRect* sGrabRect_;

			/**	Image rectangle of dimensions width_ x height_ used to grab image data
			 *	about the points to match.
			 */
			ImageRect* mGrabRect_;

			/**	Image rectangle of dimensions width_ x height_ used to grab image data
			 *	about the points to match.
			 */
			ImageRect* lGrabRect_;

			/**	space to add on top and bottom of the search rect
			 *	to accomodate for less-than-perfect stereo rectification
			 */
			int sVerticalPadding_;
			
			/**	space to add on top and bottom of the search rect
			 *	to accomodate for less-than-perfect stereo rectification
			 */
			int mVerticalPadding_;
			
			/**	space to add on top and bottom of the search rect
			 *	to accomodate for less-than-perfect stereo rectification
			 */
			int lVerticalPadding_;
			
			/** Matching record for the 2D pattern matcher to report its results into
			 */
			Pattern2DMatchRecord* patMatchRecord_;
			
			
        	/**
             *  medium-scale stereo matcher
             */
        	PatternMatcher2D* mMatcher_;
        	
        	/**
             *  large-scale stereo matcher
             */
        	PatternMatcher2D* lMatcher_;
        	
        	/**	Computes a search rectangle for the target point
        	 *	@param	pt		image point to find a match for
        	 *	@param	imgRect	valid data rect (or computation rectangle) of the image
        	 *	@param	l_or_r	is pt defined in the left or right image?
        	 *	@param	rect	search rectangle for the point to match
        	 */
        	void computeSearchRect_(ImagePoint* pt, ImageRect* imgRect, 
        							const StereoImageID l_or_r, ImageRect* rect=  NULL);


			void computeSearchRects_(ImagePoint* pt, ImageRect* imgRect, 
									  const int theWidth, const int theHeight,
									  const int verticalPadding,
									  const StereoImageID l_or_r, ImageRect* rect);
									  
            /** Computes the stereo disparity at 
             *  the point indicated for the stereo pair.  This function is an exception
             *  to the (unofficial) rule we have been following so far in this library, that
             *  private/protected function don't do any data validatgion anymore because this
             *  should have been done in the superclass.  Here I could not find a way to get
             *  this to work out.  I still have to check whether my stereo pair is rectified.
             *  @param  pt          the point for which to compute the disparity
             *  @param  l_or_r      are we working left to right or right to left
             *  @param  matchRec    the best match (and score) for the point indicated
             *  @param  searchRect  bounds for the search of a matching point in the other
             *                      image.  
             */
            void match_(StereoPair *stereoPair, ImagePoint* pt, const StereoImageID l_or_r, 
            		    StereoMatchRecord *matchRec, ImageRect* searchRect=NULL);
    };
}
#endif	//	RECTIFIED_STEREO_MATCHER_CORR_H
