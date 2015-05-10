/*  NAME:
        ColumnVector.h
 
    DESCRIPTION:
        ColumnVector public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_COLUMN_VECTOR_H
#define URIVL_COLUMN_VECTOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Matrix.h"

namespace uriVL
{
    class RowVector;

    /**
     * ColumnVector implements the ColumnVector class of Linear Algebra column vectors.
     */
    class URIVL_EXPORT ColumnVector : public Matrix
    {
        /**
         * To increase the efficiency of the transpose function, friending the RowVector becomes necessary.
         */
        friend class RowVector;

//        /**
//         * This operator creates a new Matrix equal to the sum of the two terms.  Throws an
//         * exception if the matrices' dimensions do not match.
//         *
//         * @param a  left term in the addition
//         * @param b  right term in the addition
//         * @return a reference to <code>a</code> + <code>b</code>
//         */
//        friend const ColumnVector operator +(const ColumnVector& a, const ColumnVector& b);
//
//
//        /**
//         * This operator creates a new Matrix equal to the difference of the two terms.  Throws an
//         * exception if the matrices' dimensions do not match.
//         *
//         * @param a  left term in the difference
//         * @param b  right term in the difference
//         * @return a reference to <code>a</code> - <code>b</code>
//         */
//        friend const ColumnVector operator -(const ColumnVector& a, const ColumnVector& b);

        public:
		
            /** Creates a new zero row vector at the dimensions specified
             *  @param  nbRows  number of rows of the new column vector
             */
            ColumnVector(const int nbRows);

            /** Creates a new constant column vector at the dimensions specified
             *  @param  nbRows  number of rows of the new column vector
             *  @param  v       the value at which to initialize all elements of the vector
             */
            ColumnVector(const int nbRows, const double v);

            /** Creates a new column vector at the dimensions specified and initializes it with 
             *  the values of a 1D array
             *  @param  nbRows  number of rows of the new column vector
             *  @param  v       the array of values at which to initialize the elements of the vector
             */
            ColumnVector(const int nbRows, const double* v);

            /** Copy constructor
             *
             * @param b the ColumnVector to copy
             */
            ColumnVector(const ColumnVector& b);

            /** Destructor
             */
            ~ColumnVector(void);

            /**
             * This function adds the parameter <code>ColumnVector b</code> to <code>this</code>
             * thereby modifying <code>this</code>.
             *
             * @param b the ColumnVector to add to <code>this</code>
             */
            void operator +=(const ColumnVector& b);

            /** 
             * This function subtracts the parameter <code>Matrix </code> from <code>this</code>
             * thereby modifying <code>this</code>.
             *
             * @param b the Matrix to subtract from <code>this</code>
             */
            virtual void operator -=(const ColumnVector& b);


            /** produces a new vector filled with random values between lowerBound (inclusive) and 
             *  upperBound (exclusive)
             *  @param nbRows the number of rows in the vector
             *  @param lowerBound   lower bound for the range of random values
             *  @param upperBound   upper bound for the range of random values
             *  @return a new nbRows x nbCols random matrix
             */
            static ColumnVector *randomVector(const int nbRows, const double lowerBound, 
                                              const double upperBound);

            /** Returns the Euclidean norm of this vector
             *  @return the Euclidean norm of this vector
             */
            double norm2() const;

            /** Computes the dot product of this vector and of the column vector received as parameter
             *  @return the dot product of the two vectors
             */
            double dotProd(const ColumnVector *b) const;

            /** Computes the transpose of this vector, which is a row vector
             *  @return the transpose of this column vector: a row vector
             */
            Matrix *transposition(void) const;


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
