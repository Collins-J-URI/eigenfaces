/*  NAME:
        Tracker2D.h
 
    DESCRIPTION:
        Tracker2D public header.
        Tracker2D is an abstract class (with pure virtual member function)
        Inherited by: Tracker2D_PID, BlobTracker2D, ShapeTracker2D, CornerTracker2D...
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_TRACKER_2D_H
#define URIVL_TRACKER_2D_H

#include <list>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray_F.h"
#include "VectorField.h"
#include "VectorField_F.h"
#include "ImagePoint_F.h"
#include "TrackingState.h"
#include "FeatureDetector.h"


namespace uriVL
{
	/**	Right now I am confused as to what the Tracker2D class is really supposed to
	 *	be doing:  Track a single object/feature or a collection of these?
	 */
	class URIVL_EXPORT Tracker2D
	{
		public:
		
			/**
			 *  Default Constructor
			 *  Creates a Tracker2D
			 */
			Tracker2D(void);
										  
			/**
			 *  Constructor
			 *  Creates a Tracker2D
			 *  @param  detector				feature detector(e.g., BlobFinder)
			 *	@param	metricX					metric (such as displacement) on x coordinate used for tracking
			 *	@param	metricY					metric (such as displacement) on y coordinate used for tracking
			 *	@param	initState				initial TrackingState
			 *  @param  savePred				default argument indicating whether to save prediction state
			 *  @param  saveState				default argument indicating whether to save real state
			 */
			Tracker2D(FeatureDetector* detector,
					  float metricX,
					  float metricY,
					  TrackingState *initState,
					  bool savePred = false,
					  bool saveState = false);
										  
			/**
			 *  Constructor
			 *  Creates a Tracker2D
			 *	@param	metricX					metric (such as displacement) on x coordinate used for tracking
			 *	@param	metricY					metric (such as displacement) on y coordinate used for tracking
			 *	@param	initState				initial TrackingState
			 *  @param  savePred				default argument indicating whether to save prediction state
			 *  @param  saveState				default argument indicating whether to save real state
			 */
			Tracker2D(float metricX,
					  float metricY,
					  TrackingState* initState,
					  bool savePred = false,
					  bool saveState = false);
					 
			/**
			 *	Destructor
			 */								  
			virtual ~Tracker2D(void);
			
			

			/**
			 *  track an object in the scene
			 *	@param			blobList		unrecognized objects
			 *  @return			currentState_
			 */
			//virtual TrackingState *track(BlobList *blobList) = 0;

			/**
			 *  get the prediction state record of the target(s)
			 *  @return			predRecord_
			 */
			virtual const TrackingStateList& getPredRecord(void) = 0;
			
			/**
			 *  get the real state record of the target(s)
			 *  @return			stateRecord_
			 */
			virtual const TrackingStateList& getStateRecord(void) = 0;
			
			/**
			 *  get the current state of the target
			 *  @return			currentState_
			 */
			virtual TrackingState* getCurrState(void) = 0;


		protected:
		
			/**
			 *	(pending... maybe later should use a class Metric)
			 *	metric(such as displacement...) on x coordinate used for tracking
			 */
			float metricX_;
			 
			/**
			 *	(pending... maybe later should use a class Metric)
			 *	metric(such as displacement...) on y coordinate used for tracking
			 */
			float metricY_;
			
			/**
			 *  pattern as a RasterImage this tracked object
			 */
			RasterImage* pattern_;
			
			/**
			 *  left bound of the searching area
			 */
			int leftBound_;
			
			/**
			 *  right bound of the searching area
			 */
			int rightBound_;

			/**
			 *  top bound of the searching area
			 */
			int topBound_;

			/**
			 *  bottom bound of the searching area
			 */
			int bottomBound_;

			/**
			 *	whether to save prediction state
			 */
			bool savePred_;
			
			/**
			 *	whether to save real state
			 */			
			bool saveState_;
			
			/**
			 *	current state of the target
			 */								  
			TrackingState* currentState_;
			 
			/**
			 *	a TrackingStateList of prediction state found in all input images up to now
			 */								  
			TrackingStateList predRecord_;

			/**
			 *	a TrackingStateList of real state found in all input images up to now
			 */								  
			TrackingStateList stateRecord_;			
			
			/**
			 *	maximum fail times allowed
			 */								  
			int failCounts_;
			
			/**
			 *	FeatureDetector
			 */								  
			FeatureDetector *detector_;
	};
}

#endif		//  TRACKER_2D_H
