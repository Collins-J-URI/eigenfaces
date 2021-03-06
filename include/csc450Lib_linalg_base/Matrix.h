//
//  Matrix.h
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

//=================================
// include guard
#ifndef ____Matrix_included__
#define ____Matrix_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <fstream>
#include <unistd.h>

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
//#include "ColumnVector.h"
using namespace std;


namespace csc450Lib_linalg_base {
    
    //=================================
    // forward declared dependencies
    class ColumnVector;
    class RowVector;
    
    /**
     * Defines a matrix of floating point values, along with a decent number of
     *  mathematical functions that can be performed on it
     */
    class Matrix {
    protected:
        /**
         * Two dimensional array to define the matrix
         */
        float ** a;
        
        /**
         * Number of rows
         */
        int nbRows;
        
        /**
         * Number of columns
         */
        int nbCols;
        
    public:
        
        /**
         * Creates and initializes a matrix at the dimensions specified.
         *  If we had plenty of time we would do some data validation and
         *  exception handling. Here, if array a does not have the
         *  proper dimensions, just quit.
         *
         * @param nbRows
         *          Number of rows
         *
         * @param nbCols
         *          Number of columns
         *
         * @param a
         *          The matrix
         */
        Matrix(int nbRows, int nbCols, float ** a);
        
        /**
         * Creates a matrix at the dimensions specified
         *
         * @param nbRows
         *          Number of rows
         *
         * @param nbCols
         *          Number of columns
         */
        Matrix(int nbRows, int nbCols);
        
        /**
         * Destructor
         */
        ~Matrix();
        
        /**
         * Returns a new matrix, the transpose of the one received as parameter
         */
        static const Matrix* transpose(const Matrix *matA);
        
        /**
         * Adds two matrices. If we had plenty of time we would do some data
         *  validation and exception handling. Here, if array a does not have
         *  the proper dimensions, just quit.
         */
        static  Matrix* add(const Matrix *matA,
                            const Matrix *matB);
        
        /**
         * Multiplies two matrices. If we had plenty of time we would do some
         *  data validation and exception handling. Here, if array a does not
         *  have the proper dimensions, just quit.
         */
        static  Matrix* multiply(const Matrix *matA,
                                 const Matrix *matB);
        
        /**
         * Multiplies two matrices If we had plenty of time we would do some
         *  data validation and exception handling. Here, if array a does not
         *  have the proper dimensions, just quit.
         */
        static Matrix* multiply(float mult,
                                const Matrix *mat);
        
        /**
         * Computes the outer product of two column vectors
         */
        static const Matrix* outerProduct(const ColumnVector *u,
                                          const ColumnVector *v);
        
        /**
         * Subtracts two matrices If we had plenty of time we would do some
         *  data validation and exception handling. Here, if array a does not
         *  have the proper dimensions, just quit.
         */
        static  Matrix* subtract(const Matrix *matA,
                                 const Matrix *matB);
        
        /**
         * Subtracts two matrices If we had plenty of time we would do some
         *  data validation and exception handling. Here, if array a does not
         *  have the proper dimensions, just quit.
         */
        static  ColumnVector* subtract(const ColumnVector *matA,
                                       const ColumnVector *matB);
        
        /**
         * Computes the "dot product" of two column vectors
         *  (with the same dimensions)
         */
        static float dotProduct(const ColumnVector *u,
                                const ColumnVector *v);
        
        /**
         * Masks the given matrix, and returns a new matrix which matches the
         *  values of the passed in matrix for elements which are 1 in mask,
         *  all other elements being zero. Can also be used as an
         *  element-by-element multiplication function
         */
        static const Matrix* mask(const Matrix *matA,
                                  const Matrix *mask);
        
        /**
         * Returns a copy of the passed in matrix
         */
        static Matrix* copyOf(const Matrix *matA);
        
        /**
         * Creates the column vector of every element in the matrix, read left
         *  to right, top to bottom
         */
        static ColumnVector* column(const Matrix *matA);
        
        /**
         * Creates the matrix of every element in a column vector
         */
        static Matrix* matrix(const ColumnVector *vecA, int nbCols);
        
        static Matrix* deflate(const Matrix *matA,
                               const ColumnVector *eigenvector,
                               float eigenvalue);
        
        /**
         * Returns the number of columns of this matrix
         *
         * @return
         *          The number of columns
         */
        int cols(void) const;
        
        /**
         * Returns the number of rows of this matrix
         *
         * @return
         *          The number of rows
         */
        int rows(void) const;
        
        /**
         * Returns the value of the element at the position specified
         *
         * @param theRow
         *          The row
         *
         * @param theCol
         *          The column
         *
         * @return
         *          The value at the specified index
         */
        float get(int theRow, int theCol) const;
        
        /**
         * Returns the value of the column at the position specified
         *
         * @param theCol
         *          The column
         *
         * @return
         *          The column at the specified index
         */
        ColumnVector* getColumn(int theCol) const;
        
        /**
         * Returns the two dimensional array defining this matrix
         */
        float** getArray() const;
        
        /**
         * Returns the value of the norm 1 for this matrix
         */
        float norm1(void) const;
        
        /**
         * Returns the value of the norm 1 for this matrix
         */
        float norm2(void) const;
        
        /**
         * Returns the value of the norm infinity for this matrix
         */
        float normInf(void) const;
        
        /**
         * Sets the value of the element at the position specified
         */
        void set(int theRow, int theCol, float theVal);
        
        /**
         * Adds the given row to the bottom of the matrix
         */
        void addRow(const RowVector *row);
        
        /**
         * Adds the given column to the right hand side of the matrix
         */
        void addColumn(const ColumnVector *col);
        
        /**
         * Computes the average row in the matrix
         */
        const RowVector* averageRow(void) const;
        
        /**
         * Computes the average column in the matrix
         */
        const ColumnVector* averageColumn(void) const;
        
        /**
         * Computes the largest eigenvector for this matrix.
         *
         * @param init
         *          Initial vector to start power iteration at
         *
         * @param kmax
         *          Maximum iterations
         *
         * @param tol
         *          Tolerance
         *
         * @return
         *          The largest eigenvector of the matrix
         */
        ColumnVector* eigenvector(const ColumnVector *init, int kmax, float tol) const;
        
        /**
         * Computes the largest eigenvalue for this matrix.
         *
         * @param init
         *          Initial vector to start power iteration at
         *
         * @param kmax
         *          Maximum iterations
         *
         * @param tol
         *          Tolerance
         *
         * @return
         *          The largest eigenvalue of the matrix
         */
        float eigenvalue(const ColumnVector *init, int kmax, float tol) const;
        
        /**
         * Swap the rows of the passed in indices
         */
        void swapRows(int r1, int r2);
        
        /**
         * Assigns new values to the elements of the matrix. If we had plenty
         *  of time we would do some data validation and exception handling.
         *  Here, if array a does not have the proper dimensions, just quit
         */
        void setMatrix(float ** a);
        
        /**
         * Transposes this matrix
         */
        void transpose(void);
        
        /**
         * Converts the matrix into a string according to the formatting strings
         *  received as parameters. * For example,
         *      a Mathematica-compatible output would be produced by
         *          the parameters ("[", "]", ",", false)
         *      a formatted console output would be produced by
         *          the parameters ("{", "}", ",", true) or the
         *          parameters ("", "", " \t", true)
         */
        char* toString(const char* theBeginArrayStr,
                       const char* theEndArrayStr,
                       const char* theElmtSepStr,
                       bool theEolAtEor) const;
        
    };
}
#endif /* defined(____Matrix_included__) */