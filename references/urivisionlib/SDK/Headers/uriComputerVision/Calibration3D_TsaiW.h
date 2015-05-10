/*  NAME:
        Calibration3D_TsaiW.h
 
    DESCRIPTION:
        Calibration3D_TsaiW public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CALIBRATION_TSAIW_H
#define URIVL_CALIBRATION_TSAIW_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include <vector>
//
#include "Calibration3D.h"


namespace uriVL
{

	/** Class that implements the Tsai-Wilsson algorithm for the calibration of a camera
	 *
	 *	Version history
	 *		- 2007/12/14 [jyh]		first "implementation"
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
	class URIVL_EXPORT Calibration3D_TsaiW : public Calibration3D
	{

		public:

			 /** Default contructor.
             *  This constructor simply sets to 0 or NULL the basic instance variables
             */
			Calibration3D_TsaiW(void);

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			~Calibration3D_TsaiW(void);

            /** Calibrates a camera using a list of pairs (3D reference point, 2D image point).
             *  Different registration algorithms and types of cameras may require different
             *  number of pairs.  Check the individual calibration subclasses' documentation.
             *  If the camera has initial values for its intrinsic parameters (e.g. focal length)
             *  then these may be used as initial values in the computations.
             *
             *  @param calibData    list of pairs (3D reference point, image point)
             *  @param cam          the camera object to calibrate
             */
            void calibrate(const std::vector<CalibrationPair*> *calibData, Camera *cam);
            
			
        protected:
        
	};

}

#endif	//	CALIBRATION_TSAIW_H
