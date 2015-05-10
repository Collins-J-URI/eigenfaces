/*  NAME:
        SegmentTrackingState.h
 
    DESCRIPTION:
        SegmentTrackingState public header.
        base class: TrackingState
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_SEGMENT_TRACKING_STATE_H
#define URIVL_SEGMENT_TRACKING_STATE_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "TrackingState.h"


namespace uriVL
{
	//---------------------------------------------------------------------
	//	class definition
	//---------------------------------------------------------------------
	class URIVL_EXPORT SegmentTrackingState : public TrackingState
	{
		public:
		
			/**
			 *  Constructor
			 *  Creates a SegmentTrackingState
			 *  @param  x		x coordinate in state information
			 *  @param  y		y coordinate in state information
			 *	@param	angle	angle in state information
			 *	@param	length	length in state information
			 */
			SegmentTrackingState(float x, float y, float angle, float length);
										  
			/**
			 *	Destructor
			 */								  
			virtual ~SegmentTrackingState(void);
			
			
			/**
			 *  get displacement on x in state information
			 *  @return		angle in state information
			 */
			float getAngle(void);

			/**
			 *  get displacement on y in state information
			 *  @return		length in state information
			 */
			float getLength(void);



		protected:
		
			/**
			 *	angle in state information
			 */
			float angle_;
			
			/**
			 *	length in state information
			 */
			float length_;
			
	};
}

#endif		//  SEGMENT_TRACKING_STATE_H
