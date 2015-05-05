//
//  LinearSystemRecord.h
//
//
//  Created by Christopher Glasz on 3/29/15.
//
//

//=================================
// include guard
#ifndef ____LinearSystemRecord_included__
#define ____LinearSystemRecord_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Matrix.h"
#include "LinearSystemStatus.h"

namespace csc450Lib_linalg_sle {
    /**
     * Defines a record of a linear solver
     */
    class LinearSystemRecord {
    private:
        /**
         * The status of the solution
         */
        LinearSystemStatus theStatus;
        
        /**
         * The result of the solver
         */
        const csc450Lib_linalg_base::Matrix *theSol;
        
    public:
        
        /**
         * Builds a new LinearSystemRecord object
         */
        LinearSystemRecord(const LinearSystemStatus theStatus,
                           const csc450Lib_linalg_base::Matrix *theSol);
        
        /**
         * Gives the solution to an SLE (a null reference 
         *  if the solution failed)
         */
        const csc450Lib_linalg_base::Matrix* getSolution() const;
        
        /**
         * Gives the status of an attempt to solve an SLE
         */
        const LinearSystemStatus getStatus() const;
    };
}
#endif /* defined(____LinearSystemRecord_included__) */