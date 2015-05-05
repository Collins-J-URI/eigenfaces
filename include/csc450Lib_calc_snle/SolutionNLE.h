//
//  SolutionNLE.h
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

//=================================
// include guard
#ifndef ____SolutionNLE_included__
#define ____SolutionNLE_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "SolutionStatus.h"

namespace csc450Lib_calc_snle {
    
    /**
     * Defines a solution to a non-linear equation. Contains a root for a
     *	function, the value at that root, the number of iterations it took to
     *	calculate that value, and the solution status
     */
    class SolutionNLE {
    private:
        /// Root
        float xEstimate;
        
        /// Value at the root
        float yEstimate;
        
        /// Number of iterations it took to calculate the value
        float numIterations;
        
        /// Status of the calculation
        SolutionStatus searchStatus;
        
    public:
        
        /// Constructor initializes variables
        SolutionNLE(float x, float y, float i, SolutionStatus status);
        
        /// Destructor resets variables
        ~SolutionNLE(void);
        
        /// Accessor method for the calculation status
        SolutionStatus getStatus(void) const;
        
        /// Accessor method for the root
        float getSolution(void) const;
        
        /// Accessor method for the value at the root
        float getValueAtSolution(void) const;
        
        /// Accessor method for number of iterations
        int getNumberOfIterations(void) const;
        
    };
}
#endif /* defined(____SolutionNLE_included__) */