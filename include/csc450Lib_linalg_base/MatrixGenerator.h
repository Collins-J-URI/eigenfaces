//
//  MatrixGenerator.h
//
//
//  Created by Christopher Glasz on 3/29/15.
//
//

//=================================
// include guard
#ifndef ____MatrixGenerator_included__
#define ____MatrixGenerator_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <unistd.h>
#include <tgmath.h>
#include "Matrix.h"
#include "ColumnVector.h"
#include "RowVector.h"

namespace csc450Lib_linalg_base {
    
    /**
     * Utility class for matrix generation
     */
    class MatrixGenerator {
    public:
        
        /**
         * creates an identity matrix with n columns and rows
         *
         * @param n
         *          Number of columns
         *
         * @return 
         *          The generated matrix
         */
        static Matrix* getIdentity(int n);
        
        /**
         * Creates a random matrix with n columns and m rows
         *
         * @param n
         *          Number of columns
         *
         * @param m
         *          Number of rows
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getRandom(int m, int n);
        
        /**
         * Creates a random symmetric matrix with n columns and rows
         *
         * @param n
         *          Number of columns
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getRandomSymmetric(int n);
        
        /**
         * Creates a random upper diagonal matrix with n columns and rows
         *
         * @param n
         *          Number of columns
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getRandomUpperDiagonal(int n);
        
        /**
         * Creates a random lower diagonal matrix with n columns and rows
         */
        static Matrix* getRandomLowerDiagonal(int n);
        
        /**
         * Creates a random lower unit diagonal matrix with n columns and rows
         *
         * @param n
         *          Number of columns
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getRandomLowerUnitDiagonal(int n);
        
        /**
         * Creates a matrix of polynomial equations using the passed in x values
         *
         * @param n
         *          Number of columns
         *
         * @param xs
         *          The x-coordinates
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getPolynomial(int n, float *xs);
        
        /**
         * Creates a matrix of polynomial equations using the passed in x values
         *
         * @param n
         *          Number of columns
         *
         * @param xs
         *          The x-coordinates
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getPolynomial(int n, ColumnVector *xs);
        
        /**
         * Creates a matrix of polynomial equations using random x values
         *
         * @param n
         *          Number of columns
         *
         * @param L
         *          The upper bound of x values
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getRandomPolynomial(int n, float L);
        
        /**
         * Creates a matrix for use in trigonometric interpolation
         *
         * @param n
         *          The number of columns will be 2 * n + 1
         *
         * @param L
         *          The upper bound of x values
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getTrigonometric(int n, float L);
        
        /**
         * Creates a matrix for use in trigonometric interpolation
         *
         * @param n
         *          The number of columns will be 2 * n + 1
         *
         * @param L
         *          The upper bound of x values
         *
         * @return
         *          The generated matrix
         */
        static ColumnVector* getUniformSample(int n, float L);
        
        /**
         * Creates a square diagonal matrix
         *
         * @param n
         *          Number of columns
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getSquareDiagonal(int n, float *d);
        
        /**
         * Creates a random Hessenberg matrix (whatever the hell that is)
         *
         * @param n
         *          Number of columns
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getRandomHessenberg(int n);
        
        /**
         * I don't know what this is
         *
         * @param n
         *          Number of columns
         *
         * @return
         *          The generated matrix
         */
        static Matrix* getHilbert(int n);
        
        /**
         * Creates a random column vector with m rows
         *
         * @param m
         *          Number of rows
         *
         * @return
         *          The generated matrix
         */
        static ColumnVector* getRandomColumn(int m);
        
        /**
         * Creates a random row vector with n columns
         *
         * @param n
         *          Number of columns
         *
         * @return
         *          The generated matrix
         */
        static RowVector* getRandomRow(int n);
        
        /**
         * Seed the generator. This must be performed before using any random
         *  matrix generators.
         */
        static void seed();
    };
}
#endif /* defined(____MatrixGenerator_included__) */