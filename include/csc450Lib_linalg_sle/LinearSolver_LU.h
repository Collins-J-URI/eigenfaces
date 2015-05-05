//
//  LinearSolver_LU.h
//
//
//  Created by Christopher Glasz on 4/01/15.
//
//

//=================================
// include guard
#ifndef ____LinearSolver_LU_included__
#define ____LinearSolver_LU_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "LinearSolver.h"

namespace csc450Lib_linalg_sle {
    
    /**
     * Subclass of LinearSolver which implements LU factorization
     */
    class LinearSolver_LU : public LinearSolver {
    private:
        
        /**
         * Factorize the given matrix into its lower and upper triangular
         *  components, returned together in one matrix
         */
        const csc450Lib_linalg_base::Matrix* factorize(const csc450Lib_linalg_base::Matrix *a) const;
        
        /**
         * Factorize the given matrix into its lower and upper triangular
         *  components. Overwrites the original matrix with the components
         *  together in one matrix. Returns an array of swapped indices
         */
        const int* partPivot(csc450Lib_linalg_base::Matrix *a) const;
        
        /**
         * Use backsubstitution to find x, given a column vector y and the
         *  upper triangular component of A
         */
        const csc450Lib_linalg_base::ColumnVector* backsubstitute(const csc450Lib_linalg_base::Matrix *u, const csc450Lib_linalg_base::ColumnVector *y) const;
        
        /**
         * Use forward substitution to find y, given a column vector b and the
         *  lower triangular component of A
         */
        const csc450Lib_linalg_base::ColumnVector* forwardsubstitute(const csc450Lib_linalg_base::Matrix *l, const csc450Lib_linalg_base::ColumnVector *b) const;
        
        /**
         * Find the error term in a solution, given A and b and the calculated x
         */
        const csc450Lib_linalg_base::ColumnVector* error(const csc450Lib_linalg_base::Matrix *a, const csc450Lib_linalg_base::ColumnVector *x, const csc450Lib_linalg_base::ColumnVector *b) const;
        
        /**
         * Improve the given solution using the given error term
         */
        const csc450Lib_linalg_base::ColumnVector* improve(const csc450Lib_linalg_base::Matrix *lu, const csc450Lib_linalg_base::ColumnVector *x, const csc450Lib_linalg_base::ColumnVector *err) const;
        
    public:
        
        /**
         * Solves the SLE (with the preset system matrix and right-side term)
         */
        const LinearSystemRecord* solve(void) const;
        
        /**
         * Solves the SLE (with the preset system matrix)
         */
        const LinearSystemRecord* solve(const csc450Lib_linalg_base::ColumnVector *b);
        
        /**
         * Solves the SLE
         */
        const LinearSystemRecord* solve(const csc450Lib_linalg_base::Matrix *a,
                                        const csc450Lib_linalg_base::ColumnVector *b);
        
        /**
         * Computes the determinant of the preset system matrix
         */
        float determinant(void) const;
        
        /**
         * Computes the determinant of the passed in matrix
         */
        float determinant(const csc450Lib_linalg_base::Matrix *a);
        
        /**
         * Computes the inverse of the preset system matrix
         */
        const LinearSystemRecord* inversion(void) const;
        
        /**
         * Computes the inverse of the passed in matrix
         */
        const LinearSystemRecord* inversion(const csc450Lib_linalg_base::Matrix *a);
    };
}
#endif /* defined(____LinearSolver_LU_included__) */