/*  NAME:
        TrackingState.h
 
    DESCRIPTION:
        TrackingState public header.
        TrackingState is a base class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_TRACKING_STATE_H
#define URIVL_TRACKING_STATE_H

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
#include "ImageOperator.h"


namespace uriVL
{
	//---------------------------------------------------------------------
	//	data structures
	//---------------------------------------------------------------------
	enum Result{
		TARGET_MATCH_LIST,
		TARGET_FOUND,
		TARGET_NOT_FOUND,
		WRONG_TARGET_FOUND
	};

	//---------------------------------------------------------------------
	//	class definition
	//---------------------------------------------------------------------
	class URIVL_EXPORT TrackingState
	{
		public:
		
			/**
			 *  Default constructor
			 *  Creates a TrackingState
			 */
			TrackingState(void);

			/**
			 *  Constructor
			 *  Creates a TrackingState
			 *  @param  x		x coordinate of the target
			 *  @param  y		y coordinate of the target
			 */
			TrackingState(float x, float y);

			/**
			 *  Constructor
			 *  Creates a TrackingState
			 *  @param  pos		position of the target
			 */
			TrackingState(const ImagePoint_F* pos);
										  
			/** Copy Constructor
			 *  @param  ts		TrackingState to copy
			 */
			TrackingState(const TrackingState& ts);
										  
			/** Copy Constructor
			 *  @param  ts		TrackingState to copy
			 */
			TrackingState(const TrackingState* ts);
										  
			/**
			 *	Destructor
			 */								  
			virtual ~TrackingState(void);
			
			
			/** Copy operator
			 *  @param  ts		TrackingState to copy
			 */
			const TrackingState& operator = (const TrackingState& ts);
										  

			/**
			 *  get the position in state information
			 *  @return		position as an ImagePoint
			 */
			const ImagePoint_F* getPos(void) const;

			/**
			 *  set the position in state information
			 *  @param		new position as an ImagePoint
			 */
			void setPos(const ImagePoint_F* pos);

			/**
			 *  set the position in state information
			 *  @param		x
			 *  @param		y
			 */
			void setPos(float x, float y);

			/**
			 *  get tracking result
			 */
			Result getResult(void) const;
						
			/**
			 *  set tracking result
			 *  @param		new result
			 */
			void setResult(Result r);

			/**
			 *  get targetGone_
			 */
			bool getTargetGone(void) const;
						
			/**
			 *  set targetGone_
			 */
			void setTargetGone(bool tg);
						


		private:
		
			/**
			 *	position of the target
			 */								  
			ImagePoint_F* pos_;
			
			/**
			 *	indicate tracking result
			 *	@see	enum Result
			 */								  
			Result result_;
			
			/**
			 *	if fails up to failCounts times, targeGone = true
			 */								  
			bool targetGone_;
	};
	
	typedef std::list<TrackingState *> TrackingStateList;

}

#endif		//  TRACKING_STATE_H
