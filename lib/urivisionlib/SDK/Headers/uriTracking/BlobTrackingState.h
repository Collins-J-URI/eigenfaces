/*  NAME:
        BlobTrackingState.h
 
    DESCRIPTION:
        BlobTrackingState public header.
        base class: TrackingState
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_BLOB_TRACKING_STATE_H
#define URIVL_BLOB_TRACKING_STATE_H

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
	class URIVL_EXPORT BlobTrackingState : public TrackingState
	{
		public:
		
			/**
			 *  Default constructor
			 *  Creates a BlobTrackingState
			 */
			BlobTrackingState(void);
										  
			/**
			 *  Constructor
			 *  Creates a BlobTrackingState
			 *  @param  x		x coordinate in state information
			 *  @param  y		y coordinate in state information
			 *	@param	size	size of the blob in state information
			 */
			BlobTrackingState(float x, float y, long size);
										  
			/** Copy constructor
			 *  @param  bts		BlobTrackingState to copy
			 */
			BlobTrackingState(const BlobTrackingState& bts);

			/** Copy constructor
			 *  @param  bts		BlobTrackingState to copy
			 */
			BlobTrackingState(const BlobTrackingState* bts);

			/**
			 *	Destructor
			 */								  
			~BlobTrackingState(void);
			
			
			/** Copy operator
			 *  @param  bts		BlobTrackingState to copy
			 */
			const BlobTrackingState& operator = (const BlobTrackingState& bts);

			/**
			 *  get the size of the blob
			 *  @return		size of the blob in state information
			 */
			long getSize(void) const;

			/**
			 *  set the size of the blob tracked
			 *  @param		size of the new pattern
			 */
			void setSize(long);

		private:
		
			/**
			 *	size of blob
			 */
			long size_;
	};

	typedef	std::list<BlobTrackingState*>	BlobTrackingStateList;
}

#endif		//  BLOB_TRACKING_STATE_H
