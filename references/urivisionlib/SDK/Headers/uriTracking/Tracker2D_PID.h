/*  NAME:
        Tracker2D_PID.h
 
    DESCRIPTION:
        Tracker2D_PID public header.
        (abstract)base class: Tracker2D
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_TRACKER_2D_PID_H
#define URIVL_TRACKER_2D_PID_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Tracker2D.h"


namespace uriVL
{
	class URIVL_EXPORT Tracker2D_PID : public Tracker2D
	{
		public:
		
			/**
			 *  Constructor
			 *  Creates a Tracker2D_PID
			 *  @param  detector				FeatureDetector used for this tracker
			 *	@param	metricX					metric (such as displacement) on x coordinate used for tracking
			 *	@param	metricY					metric (such as displacement) on y coordinate used for tracking
			 *	@param	initState				initial TrackingState
			 *  @param  savePred				default argument indicating whether to save prediction state
			 *  @param  saveState				default argument indicating whether to save real state
			 */
			Tracker2D_PID(FeatureDetector *detector,
						  float metricX,
						  float metricY,
						  TrackingState *initState,
						  bool savePred = false,
						  bool saveState = false);
										  
			/**
			 *	Destructor
			 */								  
			~Tracker2D_PID(void);
			
			
			/**
			 *  pure virtual function
			 *  track the state of the target
			 *  @param	img		input RasterImage
			 *  @return 		a pointer to TrackingState	Object
			 */
			//TrackingState *track(RasterImage* img);

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

	};
}

#endif		//  TRACKER_2D_PID_H
