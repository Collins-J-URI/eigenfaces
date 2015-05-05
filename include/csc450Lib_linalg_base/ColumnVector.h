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
    };
}
#endif /* defined(____ColumnVector_included__) */