/*  NAME:
        Quaternion.h
 
    DESCRIPTION:
        Quaternion public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_QUATERNION_H
#define URIVL_QUATERNION_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
    #include "uriVisionLib.h"
#endif

namespace uriVL
{

	/** Quaternion Class.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT Quaternion
    {
        public:

        	/** Creates a quaternion corresponding to a rotation of angle 0 around the Z axis.
        	 */
            Quaternion(void);

        	/** Copy constructor.
        	 *	Creates a quaternion clone of the one received as parameter.
        	 *  @param  theQuat     pointer to the Quaternion to create a copy of
        	 */
            Quaternion(Quaternion *theQuat);

            /** Create a unit Quaternion corresponding to the rotation of axis and angle specified.
             *  @param  angle   the angle of the rotation in radians
             *  @param  axis    the axis of the rotation
             */
            Quaternion(double angle, const double* axis);

                        
            /** Create a Quaternion defined by a quadruplet of double values
             *  @param  quat    the values of the quaternion
             */
            Quaternion(const double* quat);
                       
            /**
             *	Destructor
             */          
            ~Quaternion(void);


            /** Normalizes the Quaternion
             */
            void normalize(void);
            
            /** Indicates whether this Quaternion is normalized
             *  @return true if this Quaternion  is normalized
             */
            bool isNormalized(void);

        private:

            /** Storage of the quaternion's parameters [w, x, y, z] into a 4-tupple array
             */
            double quat_[4];     
            
            /** Indicates whether the quaternion is properly normalized
             */
            bool isNormalized_;
              
            
    };
}   

#endif  //  QUATERNION_H
