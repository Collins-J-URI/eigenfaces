/*  NAME:
        ShapeTrackingState.h
 
    DESCRIPTION:
        ShapeTrackingState public header.
        base class: TrackingState
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_SHAPE_TRACKING_STATE_H
#define URIVL_SHAPE_TRACKING_STATE_H

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
	class URIVL_EXPORT ShapeTrackingState : public TrackingState
	{
		public:
		
			/**
			 *  Constructor
			 *  Creates a ShapeTrackingState
			 *  @param  x		x coordinate in state information
			 *  @param  y		y coordinate in state information
			 *	@param	angle	angle in state information
			 *	@param	scaling	scaling in state information
			 *	@param	warp	warp in state information
			 */
			ShapeTrackingState(float x, float y, float angle, float scaling, float warp);
										  
			/**
			 *	Destructor
			 */								  
			virtual ~ShapeTrackingState(void);
			
			
			/**
			 *  get angle in state information
			 *  @return		angle in state information
			 */
			float getAngle(void);

			/**
			 *  get scaling in state information
			 *  @return		scaling in state information
			 */
			float getScaling(void);

			/**
			 *  get warp in state information
			 *  @return		warp in state information
			 */
			float getWarp(void);



		protected:
		
			/**
			 *	angle in state information
			 */
			float angle_;
			
			/**
			 *	scaling in state information
			 */
			float scaling_;

			/**
			 *	warp in state information
			 */
			float warp_;
			
	};
}

#endif		//  SHAPE_TRACKING_STATE_H
