/*  NAME:
        PatternTrackingState.h
 
    DESCRIPTION:
        PatternTrackingState public header.
        base class: TrackingState
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_TRACKING_STATE_H
#define URIVL_PATTERN_TRACKING_STATE_H

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
	class URIVL_EXPORT PatternTrackingState : public TrackingState
	{
		public:
		
			/**
			 *  Default constructor
			 *  Creates a PatternTrackingState
			 */
			PatternTrackingState();
										  
			/**
			 *  Constructor
			 *  Creates a PatternTrackingState
			 *  @param  x		x coordinate in state information
			 *  @param  y		y coordinate in state information
			 *	@param	size	size of the pattern in state information
			 */
			PatternTrackingState(float x, float y, long size);
										  
			/**
			 *  Constructor
			 *  Creates a PatternTrackingState
			 *  @param  bts		BlobTrackingState
			 */
			PatternTrackingState(const PatternTrackingState* pts);

			/**
			 *  Constructor
			 *  Creates a PatternTrackingState
			 *  @param  bts		BlobTrackingState
			 */
			PatternTrackingState(const PatternTrackingState& pts);

			/**
			 *	Destructor
			 */								  
			~PatternTrackingState(void);
			
			
			/**
			 *  get the size of the pattern
			 *  @return		size of the pattern in state information
			 */
			long getSize(void) const;

			/**
			 *  set the size of the pattern
			 *  @param		size of the new pattern
			 */
			void setSize(long);


		private:
		
			/**
			 *	size of pattern_ in patternTracker2D
			 */
			long size_;
			
	};
	
	typedef	std::list<PatternTrackingState*>	PatternTrackingStateList;
}

#endif		//  PATTERN_TRACKING_STATE_H
