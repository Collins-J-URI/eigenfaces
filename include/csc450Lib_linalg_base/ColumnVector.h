//
//  ColumnVector.h
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

//=================================
// include guard
#ifndef ____ColumnVector_included__
#define ____ColumnVector_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Matrix.h"
#include "RowVector.h"

namespace csc450Lib_linalg_base {
    
    /**
     * Defines a column vector
     */
    class ColumnVector : public Matrix {
    public:
        
        /**
         * Creates and initializes a new column vector
         */
        ColumnVector(int nbRows, const float * v);
        
        /**
         * Creates a new column vector (initialized to zero values)
         */
        ColumnVector(int nbRows);
        
        /**
         * Returns the value of the element at the position specified
         */
        float get(int theRow) const;
        
        float max(void) const;
        int maxInd(void) const;
        
        /**
         * Returns the value of the element at the position specified
         */
        void set(int theRow, float theVal) const;
        
        /**
         * Returns a new matrix, the transpose of the one received as parameter
         */
        static const RowVector* transpose(const ColumnVector *matA);
        
        /**
         * Returns a copy of the passed in matrix
         */
        static ColumnVector* copyOf(const ColumnVector *matA);
        
        /**
         * Multiplies two matrices If we had plenty of time we would do some
         *  data validation and exception handling. Here, if array a does not
         *  have the proper dimensions, just quit.
         */
        static  ColumnVector* multiply(float mult,
                                      const ColumnVector *mat);
        
    };
}
#endif /* defined(____ColumnVector_included__) */