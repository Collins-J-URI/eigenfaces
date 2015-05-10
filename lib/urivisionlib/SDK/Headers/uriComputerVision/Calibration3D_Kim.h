/*  NAME:
        Calibration3D_Kim.h
 
    DESCRIPTION:
        Calibration3D_Kim public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CALIBRATION_KIM_H
#define URIVL_CALIBRATION_KIM_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include <vector>
//
#include "Calibration3D.h"


namespace uriVL
{

	/** Class that implements the Kim algorithm for the calibration of a camera
	 *
	 *	Version history
	 *		- 2007/12/14 [jyh]		first "implementation"
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
	class URIVL_EXPORT Calibration3D_Kim : public Calibration3D
	{

		public:

			 /** Default contructor.
             *  This constructor simply sets to 0 or NULL the basic instance variables
             */
			Calibration3D_Kim(void);

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			~Calibration3D_Kim(void);

            /** Calibrates a camera using a list of pairs (3D reference point, 2D image point).
             *  Different registration algorithms and types of cameras may require different
             *  number of pairs.  Check the individual calibration subclasses' documentation.
             *  The camera must provide nominal values for some of its intrinsic parameters (a
             *  focal length in particular).
             *
             *  Note that this function allocates and free matrices and a solver each time it
             *  is called.  In theory I don't see why this should be a problem because it would
             *  not make sense to call this function at video rate (a dynamic calibration
             *  algorithm would be more appropriate).
             *  
             *  Note that this version only accepts a perspective camera with "TsaiW" type
             *  of intrinsic parameters.
             *
             *  @param  calibData    list of calibration pairs (3D reference point, image point)
             *  @param  cam          the camera object to calibrate
             */
            void calibrate(const std::vector<CalibrationPair*> *calibData, Camera *cam);
            
			
        private:
        
            /** Computes the solution to the LLS associated to the calibration problem
             *  @param  calibData   list of calibration pairs (3D reference point, image point)
             *  @param  stateVect   coming in: starting point of the minimization process.  Coming out: 
             *                      solution to the minimization problem
             */
            void calibrate_linear_(const std::vector<CalibrationPair*> *calibData, double stateVect[]);
            
            /** Computes the solution to the minimization associated to the calibration problem
             *  @param  calibData   list of calibration pairs (3D reference point, image point)
             *  @param  stateVect   solution to the minimization problem
             */
            void calibrate_nonlinear_(const std::vector<CalibrationPair*> *calibData, double stateVect[]);
            
        
	};

}

#endif	//	CALIBRATION_KIM_H
