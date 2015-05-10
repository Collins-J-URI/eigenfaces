/*  NAME:
        WeakPerspectiveCamera.h
 
    DESCRIPTION:
        WeakPerspectiveCamera public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_WEAK_PERSPECTIVE_CAMERA_H
#define URIVL_WEAK_PERSPECTIVE_CAMERA_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Camera.h"
#include "IntrinsicParameters_pinhole.h"

namespace uriVL
{
	/** Implements the WeakPerspectiveCamera class.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 *
	 *	@see		IntrinsicParameters_TsaiW
	 */
	class URIVL_EXPORT WeakPerspectiveCamera : public Camera
	{

		public:

			 /** The new camera stores the pointer to the IntrinsicParameter object passed
			  * as parameter, but it makes a copy of the rigid transformation data.
              *  @param  params     pointer to intrinsic parameter object that this camera will refer to
              *  @param  transf     rigid transformation data that this camera will make a copy of
              */
			WeakPerspectiveCamera(IntrinsicParameters *params, RigidTransformation3D *transf=NULL);

			~WeakPerspectiveCamera(void);

        private:
        
//           /**
//             *  Copy Constructor. Disabled
//             *
//             *  @param      theObj   reference to the object to copy
//             */
//            DepthMap(const DepthMap& theObj);
//
//
//            /**
//             *  Copy operator. Disabled
//             *
//             *  @param      theObj   reference to the object to copy
//             */
//            const DepthMap& operator = (const DepthMap& theObj);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  x   x coordinate of projected image point
             *  @param  y   y coordinate of projected image point
             */
			void getImagePoint_(const float X, const float Y, const float Z, float* x, float* y);


            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  x  address of x coordinate of projected image point
             *  @param  y  address of y coordinate of projected image point
             */
			void getImagePoint_(const double X, const double Y, const double Z, double* x, double* y);

            
	};
}	

#endif	//	WEAK_PERSPECTIVE_CAMERA_H
