//
//  NonLinearSolver_secant.h
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

//=================================
// include guard
#ifndef ____NonLinearSolver_secant_included__
#define ____NonLinearSolver_secant_included__

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
     * Subclass of NonLinearSolver which implements the newton-raphson
     *	algorithm, but using approximated derivatives using secants
     */
    class NonLinearSolver_secant : public NonLinearSolver {
    private:
        
    public:
        
        /// Constructor initializes variables
        NonLinearSolver_secant(void);
        
        /// Returns a solution to the given function between the passed
        ///	in points
        const SolutionNLE* solve(const csc450Lib_calc_base::Function1D* f,
                                 float a, float b, float tol);
        
    };
}
#endif /* defined(____NonLinearSolver_secant_included__) */