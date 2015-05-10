/*  NAME:
        BlobTracker2D.h
 
    DESCRIPTION:
        BlobTracker2D public header.
        (abstract)base class: Tracker2D
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_BLOB_TRACKER_2D_H
#define URIVL_BLOB_TRACKER_2D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Tracker2D.h"
#include "BlobTrackingState.h"
#include "BackgroundRemover_static.h"
#include "BackgroundRemover_dynamic_MW.h"
#include "GrayThresholdLabeler.h"
#include "BlobFinder.h"
#include "ImagePoint.h"
#include "ImagePoint_F.h"

namespace uriVL
{
	class URIVL_EXPORT BlobTracker2D : public Tracker2D
	{
		friend class BlobTracker2DController;
		
		public:
		
			/**
			 *  Constructor
			 *  Creates a BlobTracker2D
			 *  @param  detector				FeatureDetector used for this tracker
			 *	@param	metricX					metric (such as displacement) on x coordinate used for tracking
			 *	@param	metricY					metric (such as displacement) on y coordinate used for tracking
			 *	@param	initState				initial BlobTrackingState
			 *  @param  savePred				default argument indicating whether to save prediction state
			 *  @param  saveState				default argument indicating whether to save real state
			 */
			BlobTracker2D(FeatureDetector *detector,
						  float metricX,
						  float metricY,
						  BlobTrackingState *initState,
						  //RasterImage* bkgdImg,
						  bool savePred = false,
						  bool saveState = false);
										  
			/**
			 *	Destructor
			 */								  
			~BlobTracker2D(void);
			
			TrackingState *track(BlobList *blobList, LabelRaster *label);
			
			/**
			 *  virtual function
			 *  track the state of the target
			 *  @param	img		input RasterImage(after background update)
			 *  @return 		a pointer to TrackingState	Object
			 */
			//TrackingState *track(RasterImage* img);

			/**
			 *  overloaded function
			 *  track the state of the target
			 *  @param	img		input RasterImage
			 *  @return 		a pointer to TrackingState	Object
			 */
			/*
			TrackingState *track(RasterImage* img,
								 RasterImage* bkgdImg);
			*/
			
			/**
			 *  get current blob
			 *  @return			predRecord_
			 */
			Blob* getCurrentBlob(void);
			
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
			
			
			
		protected:
		
			/**
			 *	current blob being tracked
			 */
			Blob		*blob_;
			
			/**
			 *	tolorance range of distance (on both x and y) during tracking
			 */
			int			distanceRange_;
	};
	typedef	std::list<BlobTracker2D*>	BlobTracker2DList;
}

#endif		//  BLOB_TRACKER_2D_H
