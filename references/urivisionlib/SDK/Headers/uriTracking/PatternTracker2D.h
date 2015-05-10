/*  NAME:
        PatternTracker2D.h
 
    DESCRIPTION:
        PatternTracker2D public header.
        (abstract)base class: Tracker2D
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_TRACKER_2D_H
#define URIVL_PATTERN_TRACKER_2D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Tracker2D.h"
#include "PatternTrackingState.h"
//#include "BackgroundRemover_Static.h"
//#include "BackgroundRemover_dynamic_MW.h"
#include "GrayThresholdLabeler.h"
#include "BlobFinder.h"
#include "ImagePoint.h"
#include "ImagePoint_F.h"
//#include "PatternMatcher2D_warp.h"


namespace uriVL
{

	class URIVL_EXPORT PatternTracker2D : public Tracker2D
	{
		friend class PatternTracker2DController;
		
		public:
		
			/**
			 *  Constructor
			 *  Creates a PatternTracker2D
			 *  @param  pattern					pattern as a Pattern2D
			 *	@param	metricX					metric (such as displacement) on x coordinate used for tracking
			 *	@param	metricY					metric (such as displacement) on y coordinate used for tracking
			 *	@param	bound					bounding rectangle of searching area
			 *	@param	initState				initial PatternTrackingState
			 *	@param	scoreThreshold			score threshold
			 *  @param  savePred				default argument indicating whether to save prediction state
			 *  @param  saveState				default argument indicating whether to save real state
			 */
			PatternTracker2D(Pattern2D* pattern,
						  const ImageRect* bound,
						  float scoreThreshold,
						  float metricX,
						  float metricY,
						  PatternTrackingState *initState,
						  bool savePred = false,
						  bool saveState = false);
										  
			/**
			 *	Destructor
			 */								  
			~PatternTracker2D(void);
			

			/**
			 *  track an object in the scene
			 *	@param			img				Image within which to apply patternmatch
			 *  @return			currentState_
			 */
			TrackingState *track(RasterImage* img,
									BackgroundRemover_dynamic_MW *remover);
			
			/**
			 *  get current Pattern2D
			 *  @return			pattern_
			 */
			Pattern2D* getCurrPattern(void);
			
			/**
			 *  get the prediction state record of the target
			 *  @return			predRecord_
			 */
			TrackingStateList getPredRecord(void);
			
			/**
			 *  get the real state record of the target
			 *  @return			stateRecord_
			 */
			TrackingStateList getStateRecord(void);
			
			/**
			 *  get the current state of the target
			 *  @return			currentState_
			 */
			TrackingState *getCurrState(void);
			
//			PatternMatcher2D_warp *getMatcher(void);
			
			RasterImage_gray* getDifferenceImage(void);

			
		protected:
		
			/**
			 *	normalized pattern
			 */
			Pattern2D	*pattern_;

			/**
			 *	normalized pattern
			 */
			Pattern2D	*scaledPattern_;
			
			/**
			 *	score Threshold
			 */
			float		scoreThreshold_;			
			
			/**
			 *	PatternMatcher2D_correlation
			 */
			PatternMatcher2D_warp	*matcher_;	
			
			/**
			 *	Pattern2DMatchRecord
			 */
			Pattern2DMatchRecord	*matchRecord_;		
			
			/**
			 *	the ratio of the real rect used as a base to calculate the
			 *	square's size to the blob's valid rect
			 */
			float rectRatio_;
			
			/**
			 *	x coordinate inside the newly grabbed image that is used for matching
			 */
			int matchX_;
			
			/**
			 *	y coordinate inside the newly grabbed image that is used for matching
			 */
			int matchY_;
			
			/**
			 *	grabbed image width
			 */
			int grabbedImgWidth_;
			
			/**
			 *	grabbed image height
			 */
			int grabbedImgHeight_;
			
			/**
			 *	indicate whether the newly detected object
			 */
			bool isNewObj_;
			
			/**
			 *	grabbed image rectangle's left
			 */
			int grabbedImgLf_;
			
			/**
			 *	grabbed image rectangle's top
			 */
			int grabbedImgTp_;
			
			/**
			 *	grabbed image rectangle in current image
			 */
			ImageRect* grabPatternRect_;			

			/**
			 *	grabbing point in current image
			 */
			ImagePoint* grabPoint_;			
			
			/** intensity threshold to decide whether to merge a pixel
			 *	as part of a blob
			 */
			int* mergeThreshold_;
			
			/** intensity threshold to decide whether to remove a pixel from a blob
			 */
			int* carveThreshold_;
			
			/** number of neighbor pixels to merge
			 */
			int nbNeighborPixels_;
			
			/**	differenced RasterImage_gray after background subtraction 
			 */
			RasterImage_gray* diffImg_;

			/**	raster2D of diffImg_
			 */	
			const unsigned char* const* diffImgRaster2D_;


			/**
			 *	grab a image for later matching
			 *	@param	img		the global image used for grabbing
			 *	@param	predX	predicted x in global image
			 *	@param	predY	predicted y in global image
			 *
			 *	@return	grabbed image
			 */
			RasterImage* grabImage_(RasterImage* img,
									int predX, int predY);
								
			/**
			 *	merge blob using both background subtraction (and
			 *	maybe motion information) so that the pattern to be grabbed
			 *	can be more accurate by incorporating history data
			 *	@param	img			global image
			 *	@param	remover		BackgroundRemover_dynamic_MW
			 *	@param	blob		blob to be merged
			 */
			void mergeBlob_(RasterImage* img,
							BackgroundRemover_dynamic_MW *remover,
							Blob* blob);


			void verticalExtend_(list<HorizontalSegment*> *segList, const ImageRect* searchRect,
										Blob* blob, bool searchUp);

			int findFirstLeft_(int leftX, int rightX, int y,
							   int limLeft);

			int findLeft_(ImagePoint* pos, int limLeft);
			
			int findRight_(ImagePoint* pos, int limRight);


	};
	
	typedef	list<PatternTracker2D*>	PatternTracker2DList;
}

#endif		//  PATTERN_TRACKER_2D_H
