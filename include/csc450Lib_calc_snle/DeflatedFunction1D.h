//
//  DeflatedFunction.h
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

//=================================
// include guard
#ifndef ____DeflatedFunction_included__
#define ____DeflatedFunction_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "NonLinearSolver.h"
#include "SolutionStatus.h"
#include "Function1D.h"
#include "SolutionNLE.h"
#include <vector>

namespace csc450Lib_calc_snle {
    
    /**
     * Defines a deflated function. Given a function and a vector of roots,
     *	an instance of this class will produce a "deflated" version of the
     *	given function with the given roots removed
     */
    class DeflatedFunction1D : public csc450Lib_calc_base::Function1D {
    private:
        
        /// The given function
        const csc450Lib_calc_base::Function1D* f;
        
        /// The vector of roots
        std::vector<float> xStar;
        
    public:
        
        /// Constructor initializes variables
        DeflatedFunction1D(const csc450Lib_calc_base::Function1D* f,
                           const std::vector<float> xStar);
        
        /// Destructor need not do anything
        ~DeflatedFunction1D();
        
        /// Returns the value of the deflated function at some passed in x.
        float func(float x) const;
        
        // Exact derivative is not defined, so return false
        bool isExactDerivativeDefined();
    };
}
#endif /* defined(____DeflatedFunction_included__) */