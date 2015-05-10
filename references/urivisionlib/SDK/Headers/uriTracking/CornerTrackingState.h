/*  NAME:
        CornerTrackingState.h
 
    DESCRIPTION:
        CornerTrackingState public header.
        base class: TrackingState
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CORNER_TRACKING_STATE_H
#define URIVL_CORNER_TRACKING_STATE_H

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
	class URIVL_EXPORT CornerTrackingState : public TrackingState
	{
		public:
		
			/**
			 *  Constructor
			 *  Creates a CornerTrackingState
			 *  @param  x		x coordinate in state information
			 *  @param  y		y coordinate in state information
			 *	@param	dx		displacement on x in state information
			 *	@param	dy		displacement on y in state information
			 */
			CornerTrackingState(float x, float y, float dx, float dy);
										  
			/**
			 *	Destructor
			 */								  
			virtual ~CornerTrackingState(void);
			
			
			/**
			 *  get displacement on x in state information
			 *  @return		displacement on x in state information
			 */
			float getDx(void);

			/**
			 *  get displacement on y in state information
			 *  @return		displacement on y in state information
			 */
			float getDy(void);



		protected:
		
			/**
			 *	displacement on x in state information
			 */
			float dx_;
			
			/**
			 *	displacement on x in state information
			 */
			float dy_;
			
	};
}

#endif		//  CORNER_TRACKING_STATE_H
