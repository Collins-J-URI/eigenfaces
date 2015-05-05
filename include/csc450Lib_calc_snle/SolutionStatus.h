//
//  SolutionStatus.h
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

#ifndef ____SolutionStatus_included__
#define ____SolutionStatus_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies

namespace csc450Lib_calc_snle {
    
    /**
     * Enumeration to describe possible success and failure statuses for
     *	solving non-linear equations
     */
    enum SolutionStatus {
        SEARCH_SUCCESSFUL = 0,
        SEARCH_FAILED_TOO_MANY_ITERATIONS = 1,
        SEARCH_FAILED_OUT_OF_RANGE = 2,
        SEARCH_FAILED_NUMERICAL_ERROR = 3,
        SEARCH_FAILED_OTHER_REASON = 4
    };
}

#endif /* defined(____SolutionStatus_included__) */