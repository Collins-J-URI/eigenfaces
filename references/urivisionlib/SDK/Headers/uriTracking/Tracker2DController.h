/*  NAME:
        Tracker2DController.h
 
    DESCRIPTION:
        Tracker2DController public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_TRACKER_2D_CONTROLLER_H
#define URIVL_TRACKER_2D_CONTROLLER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Tracker2D.h"


namespace uriVL
{
	//---------------------------------------------------------------------
	//	class definition
	//---------------------------------------------------------------------
	class URIVL_EXPORT Tracker2DController
	{
		public:
		
			/**
			 *  Default Constructor
			 *  Creates a Tracker2DController
			 */
			Tracker2DController(void);
										  
			/**
			 *	Destructor
			 */								  
			virtual ~Tracker2DController(void);
			
			
			
			/**
			 *	Begin control loop
			 *				  
			 */								  
			virtual void action(RasterImage* img) = 0;

		protected:
		
	};

}

#endif	//	TRACKER_2D_CONTROLLER_H
