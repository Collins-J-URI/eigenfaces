/*  NAME:
        RowVector.h
 
    DESCRIPTION:
        RowVector public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ROW_VECTOR_H
#define URIVL_ROW_VECTOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Matrix.h"

namespace uriVL
{
    class ColumnVector;

    /** Implements the RowVector class of Linear Algebra row vectors.
     */
    class URIVL_EXPORT RowVector : public Matrix
    {
        friend class ColumnVector;

        public:
            /** Creates a new zero row vector at the dimensions specified
             *  @param  nbCols  number of columns of the new row vector
             */
            RowVector(int nbCols);

            /** Creates a new constant row vector at the dimensions specified
             *  @param  nbCols  number of columns of the new row vector
             *  @param  v       the value at which to initialize all elements of the vector
             */
            RowVector(int nbCols, double v);

            /** Creates a new row vector at the dimensions specified and initializes it with 
             *  the values of a 1D array
             *  @param  nbCols  number of columns of the new row vector
             *  @param  v       the array of values at which to initialize the elements of the vector
             */
            RowVector(int nbCols, const double* v);

            /**
             * Copy constructor
             *
             * @param b the RowVector to copy
             */
            RowVector(const RowVector *b);

            /** Destructor
             */
            ~RowVector(void);

            /** produces a new column vector filled with random values between lowerBound (inclusive) and 
             *  upperBound (exclusive)
             *  @param nbCols the number of columns in the vector
             *  @param lowerBound   lower bound for the range of random values
             *  @param upperBound   upper bound for the range of random values
             *  @return a new nbRows x nbCols random matrix
             */
            static RowVector* randomVector(int nbCols, double lowerBound, 
                                           double upperBound);

//            /** Resizes the RowVector at the new dimension specified.  If this new dimension
//             *  is less than the previous number of  storage columns, only nbCols_ is
//             *  modified, otherwise the mat_ array is reallocated.
//             *  @param  newNbCols   new number of columns for this matrix
//             */
//            void resize(int newNbCols);
            
             /** Returns the Euclidean norm of this vector
             *  @return the Euclidean norm of this vector
             */
            double norm2(void) const;

            /** Computes the dot product of this vector and of the row vector received as parameter. Fails (throws an exception
			 *	or aborts) if the vectors' dimensions don't match. 
			 *	@param	b	the row vector to compute the 
             *  @return the dot product of the two vectors
             */
            double dotProduct(const RowVector *b) const;

            /** Computes the dot product of this vector and of the column vector received as parameter. Fails (throws an exception
			 *	or aborts) if the vectors' dimensions don't match.
			 *	@param	b	the row vector to compute the 
             *  @return the dot product of the two vectors
             */
            double dotProduct(const ColumnVector *b) const;

            /** Computes the outer product matrix of this mx1 column vector and of the 1xn row vector received as parameter.
			 *	The output matrix will be mxn, and is in fact simply computed as the regular matrix product of this and b
			 *	@param	b	the row vector to compute the outer product with
             *  @return the dot product of the two vectors
             */
            Matrix* outerProduct(const RowVector *b) const;

            /** Computes the outer product matrix of this mx1 column vector and of the 1xn row vector received as parameter.
			 *	The output matrix will be mxn.
			 *	@param	b	the row vector to compute the outer product with
             *  @return the dot product of the two vectors
             */
            Matrix* outerProduct(const ColumnVector *b) const;


            /** Computes the transpose of this vector, which is a column vector
             *  @return the transpose of this row vector: a column vector
             */
            Matrix* transposition(void) const;

            /** Normalize the vector according to norm N1
             */
            void normalize1(void);
            
            /** Normalize the vector according to norm N2
             */
            void normalize2(void);

            /** Normalize the vector according to norm Ninf
             */
            void normalizeInf(void);
			
			
		private:
		




    };
}


#endif
