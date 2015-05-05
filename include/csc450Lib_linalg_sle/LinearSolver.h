//
//  LinearSolver.h
//
//
//  Created by Christopher Glasz on 3/29/15.
//
//

//=================================
// include guard
#ifndef ____LinearSolver_included__
#define ____LinearSolver_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Matrix.h"
#include "ColumnVector.h"
#include "LinearSystemRecord.h"

namespace csc450Lib_linalg_sle {
    
    /**
     * Defines the base class for a solver of systems of linear equations
     */
    class LinearSolver {
    protected:
        /**
         * The A bit of Ax = b
         */
        const csc450Lib_linalg_base::Matrix *a;
        
        /**
         * The b bit of Ax = b
         */
        const csc450Lib_linalg_base::ColumnVector *b;
        
    public:
        
        /**
         * Sets a system matrix to the SLE
         */
        void setMatrix(const csc450Lib_linalg_base::Matrix *a);
        
        /**
         * Sets a new right-side term to the SLE
         */
        void setRightSideTerm(const csc450Lib_linalg_base::ColumnVector *b);
        
        /**
         * Sets a new matrix and right side term for the SLE
         */
        void setSLE(const csc450Lib_linalg_base::Matrix *a, const csc450Lib_linalg_base::ColumnVector *b);
        
        /**
         * Solves the SLE (with the preset system matrix and right-side term), 
         *  using the implementation of the child class
         */
        virtual const LinearSystemRecord* solve(void) const = 0;
        
        /**
         * Solves the SLE (with the preset system matrix), using the 
         *  implementation of the child class
         */
        virtual const LinearSystemRecord* solve(const csc450Lib_linalg_base::ColumnVector *b) = 0;
        
        /**
         * Solves the SLE, using the implementation of the child class
         */
        virtual const LinearSystemRecord* solve(const csc450Lib_linalg_base::Matrix *a,
                                                const csc450Lib_linalg_base::ColumnVector *b) = 0;
    };
}
#endif /* defined(____LinearSolver_included__) */