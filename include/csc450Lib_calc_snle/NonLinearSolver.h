//
//  NonLinearSolver.h
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

//=================================
// include guard
#ifndef ____NonLinearSolver_included__
#define ____NonLinearSolver_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "SolutionStatus.h"
#include "Function1D.h"
#include "SolutionNLE.h"

namespace csc450Lib_calc_snle {
    
    /**
     * Defines the base class for non-linear equation solvers
     */
    class NonLinearSolver {
    private:
        
    public:
        
        /// Constructor initializes variables
        NonLinearSolver(void);
        
        /// Returns a solution to the given function between the passed
        ///	in points
        virtual const SolutionNLE* solve(const csc450Lib_calc_base::Function1D* f,
                                 float a, float b, float tol) = 0;
        
    };
}
#endif /* defined(____NonLinearSolver_included__) */