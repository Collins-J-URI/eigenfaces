//
//  RowVector.h
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

//=================================
// include guard
#ifndef ____RowVector_included__
#define ____RowVector_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Matrix.h"

namespace csc450Lib_linalg_base {
    
    /**
     * Defines a row vector
     */
    class RowVector : public Matrix {
    public:
        
        /**
         * Creates and initializes a new row vector
         */
        RowVector(int nbCols, const float * v);
        
        /**
         * Creates a new row vector (initialized to zero values)
         */
        RowVector(int nbCols);
        
        /**
         * Returns the value of the element at the position specified
         */
        float get(int theCol) const;
        
        /**
         * Returns the value of the element at the position specified
         */
        void set(int theCol, float theVal) const;
        
        /**
         * Returns a new matrix, the transpose of the one received as parameter
         */
        static const ColumnVector* transpose(const RowVector *matA);
        
    };
}
#endif /* defined(____RowVector_included__) */