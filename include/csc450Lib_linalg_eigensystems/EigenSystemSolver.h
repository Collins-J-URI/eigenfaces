//
//  EigenSystemSolver.h
//
//
//  Created by Christopher Glasz on 4/01/15.
//
//

//=================================
// include guard
#ifndef ____EigenSystemSolver_included__
#define ____EigenSystemSolver_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>

#include "Matrix.h"
#include "MatrixGenerator.h"
#include "ColumnVector.h"
#include "RowVector.h"
#include "EigenSystem.h"
#include "LinearSolver_LU.h"

namespace csc450Lib_linalg_eigensystems {
    
    /**
     * Subclass of LinearSolver which implements LU factorization
     */
    class EigenSystemSolver {
    private:
        
        const csc450Lib_linalg_base::Matrix *a;
        
    public:
        
        EigenSystemSolver(void);
        EigenSystemSolver(const csc450Lib_linalg_base::Matrix *a);
        ~EigenSystemSolver(void);
        
		/**
		* Calculates an Eigensystem using power iteration
		*/
        static EigenSystem* power(const csc450Lib_linalg_base::Matrix *a,
                                  const csc450Lib_linalg_base::ColumnVector *init,
                                  int iterations, float tol);
        
		/**
		* Calculates an EigenSystem using Rayleigh Quotient Iteration
		*/
        static EigenSystem* rayleigh(const csc450Lib_linalg_base::Matrix *a,
                                    const csc450Lib_linalg_base::ColumnVector *v,
                                    float l, int iterations, float tol);
       
		/**
		* Calculates a deflated Matrix for the given EigenSystem
		*/
        static csc450Lib_linalg_base::Matrix* deflate(const csc450Lib_linalg_base::Matrix *a,
                                                      const csc450Lib_linalg_base::ColumnVector *v,
                                                      float l);
        
        /**
         * Calculates the Eigensystem (for the given matrix)
         */
        const EigenSystem* solve(const csc450Lib_linalg_base::Matrix *a);
        
        /**
         * Calculates the Eigensystem (with the preset matrix)
         */
        const EigenSystem* solve(void) const;
        
        /**
         * Sets the Matrix
         */
        void setA(const csc450Lib_linalg_base::Matrix *a);
    };
}
#endif /* defined(____EigenSystemSolver_included__) */