//
//  NonLinearSolver_hybrid.h
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

//=================================
// include guard
#ifndef ____NonLinearSolver_hybrid_included__
#define ____NonLinearSolver_hybrid_included__

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
     * Subclass of NonLinearSolver which implements a combination of bisection
     *	and the newton-raphson algorithm.
     */
    class NonLinearSolver_hybrid : public NonLinearSolver {
    private:
        
    public:
        
        /// Constructor initializes variables
        NonLinearSolver_hybrid(void);
        
        /// Returns a solution to the given function between the passed
        ///	in points
        const SolutionNLE* solve(const csc450Lib_calc_base::Function1D* f,
                                 float a, float b, float tol);
        
    };
}
#endif /* defined(____NonLinearSolver_hybrid_included__) */