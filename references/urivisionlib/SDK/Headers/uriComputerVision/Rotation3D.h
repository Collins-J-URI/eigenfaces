/*  NAME:
        Rotation3D.h
 
    DESCRIPTION:
        Rotation3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ROTATION_3D_H
#define URIVL_ROTATION_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Quaternion.h"
#include "Matrix.h"


namespace uriVL
{
    typedef enum RotationRef {
            MATRIX_REF = 0,
            QUATERNION_REF,
            ROLL_PITCH_YAW_REF,
            EULER_ANGLES_REF
    } RotationRef;
    

    
	/** A Rotation3D is a component of any
	 *  RigidTransformation3D.  It could be specified by a transformation matrix, by three angles in 
	 *  the Roll-Pitch-Yaw convention, by 3 angles
	 *  in the Euler angles convention, or by a quaternion.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT Rotation3D
    {
        public:

        	/**
        	 *	Constructor.
        	 *	Creates an identity rotation.
        	 */
            Rotation3D(void);

        	/**
        	 *	Clone donstructor.
        	 *	Creates a 3D rotation identical to the one received as parameter
        	 *	@param	theRot      3D rotation to clone
        	 */
            Rotation3D(const Rotation3D* theRot);

            /** Creates a new Rotation3D object specified by a GL-style matrix, a quaternion,
             *   a triplet of R-P-Y angles, or a triplet of Euler angles
             *  @param  dataType    indicates what kind of data is stored in the double array
             *  @param  rotData     1D array of double storing a representation for the rotation
             */
            Rotation3D(RotationRef dataType, const double* rotData);
            
            /**
             *	Destructor
             */          
            ~Rotation3D(void);
			
            /** Returns a read-only reference to this rotation's matrix.  If the matrix
             *  was not the reference and not up-to-date, its value is updated first.
             *  @return		the matrix of this rotation
             */ 
            const Matrix* getMatrix(void) const;

            /** Returns a writable reference to this rotation's matrix.  If the matrix
             *  was not the reference and not up-to-date, its value is updated first.  After this 
             *  call the matrix will now be the reference.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return		the matrix of this rotation
             */ 
            Matrix* getMatrix(ReadWriteStatus rwa);

            /** Returns a read-only reference to this rotation's quaternion.  If the quaternion
             *  was not the reference and not up-to-date, its value is updated first.
             *  @return		the rotation matrix of this rotation
             */ 
            const Quaternion *getQuarternion(void) const;

            /** Returns a writable reference to this rotation's quaternion.  If the quaternion
             *  was not the reference and not up-to-date, its value is updated first.  After this 
             *  call the quaternion will now be the reference.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return		the quaternion of this rotation
             */ 
            Quaternion* getQuarternion(ReadWriteStatus rwa);

            /** Returns a read-only reference frame to this rotation's roll-pitch-yaw angles.  
             *  If the roll-pitch-yaw angles were not the reference and not up-to-date, their 
             *  value is updated first.
             *  @return		the roll-pitch-yaw angles of this rotation
             */ 
            const double* getRollPitchYawAngles(void) const;

            /** Returns a writable reference frame to this rotation's roll-pitch-yaw angles.  
             *  If the roll-pitch-yaw angles were not the reference and not up-to-date, their 
             *  value is updated first.  After this 
             *  call the r-p-y angles will now be the reference.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return		the roll-pitch-yaw angles of this rotation
             */ 
            double* getRollPitchYawAngles(ReadWriteStatus rwa);

            /** Returns a read-only reference frame to this rotation's Euler angles.  
             *  If the Euler angles were not the reference and not up-to-date, their 
             *  value is updated first.
             *  @return		the Euler angles of this rotation
             */ 
            const double* getEulerAngles(void) const;

            /** Returns a writable reference frame to this rotation's Euler angles.  
             *  If the Euler angles were not the reference and not up-to-date, their 
             *  value is updated first.  After this 
             *  call the Euler angles will now be the reference.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return		the Euler angles of this rotation
             */ 
            double* getEulerAngles(ReadWriteStatus rwa);



        private:
        
            /** Which of the 4 types of rotation representations is the reference
             *  (matrix is default).
             */ 
            RotationRef ref_;  
            
            /** the 4x4 homogeneous matrix that stores the rotation.
             *  This matrix is the reference by default (unless explicitely changed by
             *  the user)
             */
            mutable Matrix* mat_;
            
            /** Quaternion object that could represent this rotation
             */
            mutable Quaternion* quat_;
            
            /** Array of three angles in the R-P-Y convention
             */
            mutable double* rollPitchYaw_;       

            /** Array of three angles in the Euler angles convention
             */
            mutable double* euler_;    
            
            /** true if the Matrix object is up-to-date compared to reference representation
             *  @see ref_
             */
            mutable bool matrixIsUpToDate_;

            /** true if the Quarternion object is up-to-date compared to reference representation
             *  @see ref_
             */
            mutable bool quaternionIsUpToDate_;

            /** true if the roll-pitch-yaw angles are up-to-date compared to reference representation
             *  @see ref_
             */
            mutable bool rollPitchYawIsUpToDate_;

            /** true if the Euler angles are up-to-date compared to reference representation
             *  @see ref_
             */
            mutable bool eulerIsUpToDate_;


            /** Copy constructor. Disabled.
             *  @param obj  reference to the object to copy
             */
            Rotation3D(const Rotation3D& obj);
            
            /** Copy operator. Disabled.
             *  @param obj  reference to the object to copy
             */
            const Rotation3D& operator = (const Rotation3D& obj);
            
              
            /** Updates the rotation matrix if it was not the reference.
             *
             */ 
            void updateMatrix_(void) const;


            /** Updates the quarternion if it was not up-to-date
             *
             */ 
            void updateQuarternion_(void) const;

            /** Updates the rooll-pitch-yaw representation if it was not up-to-date
             *
             */ 
            void updateRollPitchYaw_(void) const;

            /** Updates the Euler angle representation if it was not up-to-date
             *
             */ 
            void updateEulerAngles_(void) const;

            /** Ensures that the transformation matrix is still that of a rotation
             *
             */ 
            void orthogonalizeMatrix_(void) const;

        

            
    };
}   

#endif  //  ROTATION_3D_H
