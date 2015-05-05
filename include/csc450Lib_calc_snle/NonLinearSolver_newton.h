//
//  NonLinearSolver_newton.h
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

//=================================
// include guard
#ifndef ____NonLinearSolver_newton_included__
#define ____NonLinearSolver_newton_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "NonLinearSolver.h"
#include "SolutionStatus.h"
#include "Function1D.h"
#include "SolutionNLE.h"

namespace csc450Lib_calc_snle {
    
    /**
     * Subclass of NonLinearSolver which implements the newton-raphson algorithm
     */
    class NonLinearSolver_newton : public NonLinearSolver {
    private:
        
    public:
        
        /// Constructor initializes variables
        NonLinearSolver_newton(void);
        
        /// Returns a solution to the given function between the passed
        ///	in points
        const SolutionNLE* solve(const csc450Lib_calc_base::Function1D* f,
                                 float a, float b, float tol);
        
    };
}
#endif /* defined(____NonLinearSolver_newton_included__) */