//
//  LinearSystemStatus.h
//
//
//  Created by Christopher Glasz on 3/29/15.
//
//

//=================================
// include guard
#ifndef ____LinearSystemStatus_included__
#define ____LinearSystemStatus_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies

namespace csc450Lib_linalg_sle {
    /**
     * Enumeration to describe the possible solution statuses produced by a 
     *  linear solver
     */
    enum LinearSystemStatus {
        ILL_CONDITIONED_MATRIX,
        LINEAR_SOLVER_FAILED,
        LINEAR_SOLVER_SUCCEEDED,
        REGULAR_MATRIX,
        SINGULAR_MATRIX,
        UNKNOWN_STATUS
    };
}
#endif /* defined(____LinearSystemStatus_included__) */