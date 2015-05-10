/*  NAME:
        Transformation3D.h
 
    DESCRIPTION:
        Transformation3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_TRANSFORMATION_3D_H
#define URIVL_TRANSFORMATION_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "HomogeneousMatrix.h"


namespace uriVL
{

	/** Virtual base class for all 3D transformation that can be represented by
	 *  a 4 by 4 homogenous matrix.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT Transformation3D
    {
        public:

        	/**
        	 *	Constructor.
        	 *	Creates an identity transformation 3D.
         	 */
            Transformation3D(void);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the transformation matrix received
        	 *  as parameter
        	 *	@param	glMat	a flat, GL-style (transposed) transformation matrix
        	 */
            Transformation3D(const double* glMat);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the transformation matrix received
        	 *  as parameter
        	 *	@param	theMat	4 by 4 transformation matrix
        	 */
            Transformation3D(const double theMat[][4]);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the transformation matrix received
        	 *  as parameter
        	 *	@param	theMat	homogeneous transformation matrix
        	 */
            Transformation3D(const HomogeneousMatrix* theMat);

        	/**
        	 *	Clone constructor.
        	 *	Creates a 3D transformation identical to the one received as parameter
        	 *	@param	theTransf	transformation to clone
        	 */
            Transformation3D(const Transformation3D* theTransf);


            /**
             *	Destructor
             */          
            virtual ~Transformation3D(void);

            /** Returns a read-only reference frame to this transformation's matrix.  If the matrix
             *  was not the reference and not up-to-date, its value is updated first.
             *  @return		the transformation matrix of this transformation
             */ 
            const HomogeneousMatrix *getMatrix(void) const;

            /** Returns a writable reference frame to this transformation's matrix.  If the matrix
             *  was not the reference and not up-to-date, its value is updated first.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return		the transformation matrix of this transformation
             */ 
            HomogeneousMatrix *getMatrix(ReadWriteStatus rwa);

            /** Updates the transformation matrix if it was not the reference and 
             *  it has been changed and then returns it as a read-only "GL" matrix (a 1D
             *	array of 16 elements, read column by column, left to right).
             *  @return		the OpenGL transformation matrix of this transformation
             */ 
            const GLfloat* getGLMatrix(void) const;

//            /** Updates the transformation matrix if it was not the reference and 
//             *  it has been changed and then returns it as a writable "GL" matrix (a 1D
//             *	array of 16 elements, read column by column, left to right).
//             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
//             *  @return		the OpenGL transformation matrix of this transformation
//             */ 
//            GLfloat* getGLMatrix(ReadWriteStatus rwa);


        protected:

            /** the 4x4 homogeneous matrix that stores the transformation.
             *  This matrix is the reference by default (unless explicitely changed by
             *  the user). This variable is mutalbe because if it is not the reference
             *  then it can be updated without any real change to the object
             */
            mutable HomogeneousMatrix* homMat_;
            
            /** Indicates whether the transformation matrix (inherited from the parent class)
             *  stores the reference values for the transformation.
             *  Whenever redundant storage is used in this library, as with the transformation
             *  matrix and the Rotation3D and Translation3D objects of a RigidTransformation3D, 
             *  a boolean variable us used to indicate which is the current "true" value.
             */
            bool matrixIsReference_;
            
            /** Indicates that whatever constitues the reference for this object (e.g. matrix,
             *  rotation angles + translation) ha been modified.  When this is true, 
             *  non-reference representations must be updated before they are used
             */
            mutable bool referenceHasChanged_;
            
            /** Indicates whether the transformation matrix (inherited from the parent class)
             *  stores the reference values for the transformation.
             *  Whenever redundant storage is used in this library, as with the transformation
             *  matrix and the Rotation3D and Translation3D objects of a RigidTransformation3D, 
             *  a boolean variable is used to indicate which is the current "true" value.
             *
             *	@param	state	true if the matrix should be used as reference
             */
            void setMatrixAsReference_(bool state);
            
            /** Updates the transformation matrix if it was not the reference.
             *
             *  This virtual function should be implemented by all subclasses.
             */ 
            virtual void updateMatrix_(void) const = 0;
            
        private:
        
            /** Copy constructor. Disabled.
             *  @param obj  reference to the object to copy
             */
            Transformation3D(const Transformation3D& obj);
            
            /** Copy operator. Disabled.
             *  @param obj  reference to the object to copy
             */
            const Transformation3D& operator = (const Transformation3D& obj);
            
              

    };
}   

#endif  //  TRANSFORMATION_3D_H
