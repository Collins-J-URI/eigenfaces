/*  NAME:
        RigidTransformation2D.h
 
    DESCRIPTION:
        RigidTransformation2D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_RIGID_TRANSFORMATION_2D_H
#define URIVL_RIGID_TRANSFORMATION_2D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Transformation2D.h"

namespace uriVL
{
    class Rotation2D;
    class Translation2D;
    
    /** Class for all 2D transformation composed of a translation and a rotation.  The translation
     *  component of the transformation is always stored in the object's matrix.  The rotation 
     *  component can be sotred either in the matrix or as an angle.  The variables matrixIsReference_
     *  and angleIsReference_ indicate which stores the "correct" reference value.
     *  @see    matrixIsReference_
     *  @see    angleIsReference_
     * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RigidTransformation2D: public Transformation2D
    {
        public:

            /**
             *  Constructor.
             *  Creates an identity transformation.
             */
            RigidTransformation2D(void);

            /**
             *  Constructor.
             *  Creates a 2D transformation based on the transformation matrix received
             *  as parameter
             *  @param  theMat  3 by 3 transformation matrix
             */
            RigidTransformation2D(const float theMat[][3]);

            /**
             *  Clone constructor.
             *  Creates a 2D transformation identical to the one received as parameter
             *  @param  theTransf   transformation to clone
             */
            RigidTransformation2D(const RigidTransformation2D* theTransf);

            /**
             *  Constructor.
             *  Creates a 2D transformation based on the rotation and translation received as parameters
             *  @param  theRot  the rotation transformation
             *  @param  theTrans    the translation transformation
             */
            RigidTransformation2D(Rotation2D *theRot, Translation2D *theTrans);

            /**
             *  Destructor
             */          
            virtual ~RigidTransformation2D(void);


            /** Assign a new matrix to this transformation.  The new matrix is now the reference
             *  for this transformation.
             *  @param  theMat  3x3 transformation matrix
             */
            void setMatrix(const float theMat[][3]);


            /** Returns the x component of the translation
             *  @return     x component of the translation  
             */
            float getTransX(void) const;
            
            /** Returns the y component of the translation
             *  @return     y component of the translation  
             */
            float getTransY(void) const;
            
            /** Returns the rotation's angle.
             *  If the matrix is the reference, this call may require a re-orthogonalization
             *  of the matrix.
             *
             *  @return     the rotation's angle
             */
            float getAngle(void) const;
            
            /** Sets the rotation's angle (in radians)
             *
             *  @param theAngle     the rotation's new angle
             */
            void setAngle(float theAngle);

			/**	Sets the translation component of this transformation
			 *	@param	tx	x translation component
			 *	@param	ty	x translation component
			 */
			void setTranslation(float tx, float ty);
                        

			/**	Sets the translation component of this transformation
			 *	@param	trans	translation component
			 */
			void setTranslation(const float trans[]);
                        

       private:

            /** Angle of the rotation component
             */
            mutable float angle_;
            

            /** Indicates whether the angle is the reference
             */
            bool    angleIsReference_;
            
            
            /** Indicates that the transformation matrix (inherited from the parent class)
             *  stores the reference values for the transformation.
             *  Whenever redundant storage is used in this library, as with the transformation
             *  matrix and the Rotation2D and Translation2D objects of a RigidTransformation2D, 
             *  a boolean variable us used to indicate which is the current "true" value.  
             */
            void setMatrixAsReference_(void);
            

            /** Updates the transformation matrix if it was not the reference, and
             *  if it is the reference, then re-orthogonalizes it.
             */ 
            void updateMatrix_(void);


            /** Ensures that the transformation matrix is still orthogonal.
             */ 
            virtual void orthogonalizeMatrix_(void);

    };
}   

#endif  //  RIGID_TRANSFORMATION_2D_H
