/*  NAME:
        Calibration3D.h
 
    DESCRIPTION:
        Calibration3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CALIBRATION_H
#define URIVL_CALIBRATION_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include <vector>
//
#include "Entity3D.h"
#include "Point3D.h"
#include "ImagePoint_F.h"
#include "IntrinsicParameters.h"
#include "Camera.h"


namespace uriVL
{
    /** Struct that stores a "calibration pair", that is, a 3D point and the corresponding
     *  image point observed
     */
    typedef struct CalibrationPair {
                                         const Point3D *pt3D;
                                         const ImagePoint_F *imgPt;
    } CalibrationPair;

	/** The base virtual Calibration3D class.  A Calibration3D object computes the extrinsic
     *  parameters (always) of a camera relative to the world reference frame, and
     *  (sometimes) some of the camera's intrinsic parameters.
     *  The frontier between calibration and registration is somewhat fuzzy.  Some authors
     *  use calibration for static computation of extrinsic and intrinsic parameters, while
     *  using registration to refer to the computation of an object's pose.  
     *
	 *	Version history
	 *		- 2007/12/14 [jyh]		first implementation (after multiple false starts)
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
	class URIVL_EXPORT Calibration3D
	{

		public:

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			virtual ~Calibration3D(void);

            /** Calibrates a camera using a list of pairs (3D reference point, 2D image point).
             *  Different registration algorithms and types of cameras may require different
             *  number of pairs.  Check the individual calibration subclasses' documentation.
             *  If the camera has initial values for its intrinsic parameters (e.g. focal length)
             *  then these may be used as initial values in the computations.
             *
             *  @param calibData    list of pairs (3D reference point, image point)
             *  @param cam          the camera object to calibrate
             */
            virtual void calibrate(const std::vector<CalibrationPair*> *calibData, Camera *cam) = 0;
            
            /** Calibrates a camera using a calibration grid and the matching grid of image points
             *  observed (if a marker was not visible, then the image point is set to null in the 
             *  grid).  This version of the function calls the other, vector-based form of the
             *  function, which is implemented by the subclasses.
             *  Different registration algorithms and types of cameras may require different
             *  number of pairs.  Check the individual calibration subclasses' documentation.
             *  If the camera has initial values for its intrinsic parameters (e.g. focal length)
             *  then these may be used as initial values in the computations.
             *
             *  @param nbRows       number of rows of the calibration grid
             *  @param nbCols       number of columns of the calibration grid
             *  @param refGrid      a 2D array of 3D reference points
             *  @param imgGrid      the matching 2D array of image points
             *  @param cam          the camera object to calibrate
             */
            void calibrate(int nbRows, int nbCols, const Point3D *const*const*const refGrid,
                                   const ImagePoint_F *const*const* imgGrid, Camera *cam);
            
			
        protected:
        
            /** Default constructor
             */
            Calibration3D(void);
            
        

	};

}

#endif	//	CALIBRATION_H
