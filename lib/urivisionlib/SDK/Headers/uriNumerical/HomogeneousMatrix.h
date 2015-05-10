/*  NAME:
        HomogeneousMatrix.h
 
    DESCRIPTION:
        HomogeneousMatrix public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef URIVL_HOMOGENEOUS_MATRIX_H
#define URIVL_HOMOGENEOUS_MATRIX_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Matrix.h"

namespace uriVL
{
	/**	Implements 4x4 homogeneous matrices used to represent 3D transformations.
	 *
	 */
	 
	class URIVL_EXPORT HomogeneousMatrix : public Matrix
	{
		public:
		
		    /**  Creates an identity 4x4 matrix
		     */
			HomogeneousMatrix(void);
			
			/**  Copy constructor
			 *  @param  theMat  the matrix to copy
			 */
			HomogeneousMatrix(const HomogeneousMatrix *theMat);

            /** Initializes the new matrix with the values of a flat, "GL-style" matrix
             *  @param  glMat   a flat, column-ordered 16-element GL matrix
             */
			HomogeneousMatrix(const double* glMat);
			
			/** Initializes this matrix with the values of the one received as parameter
			 *  @param  theMat  a 4x4 homogeneous matrix
			 */
			HomogeneousMatrix(const double theMat[][4]);
			
			/** Destructor
			 */
			~HomogeneousMatrix(void);
			
            /** Returns a read-only pointer to the matrix data.  If you want to have full read-write
             *  access to the data, call instead getMatrixRW.
             *  @return the matrice's private read-only 2D array of data
             */
			const GLfloat* getGLMatrix(void);

            /** Returns a pointer to the matrix data that allows to set new values to the matrix's 
             *  elements.
             *  As a side effect, this function sets to IS_UNKNOWN all the matrix's indicators.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return the matrix's private read-write 2D array of data
             */
			GLfloat* getGLMatrix(ReadWriteStatus rwa);
			
		protected:
		
		    /** copy of the mat_ matrix used for OpenGL transformations
		     */
			GLfloat	glMat_[16];
			
	};

}

#endif	//	HOMOGENEOUS_MATRIX_H
