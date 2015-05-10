/*  NAME:
        LinearSolverRecord.h
 
    DESCRIPTION:
        LinearSolverRecord public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_LINEAR_SOLVER_RECORD_H
#define URIVL_LINEAR_SOLVER_RECORD_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

#include "Matrix.h"

namespace uriVL
{
    /** Tri-state enumerated type generally set by a factorization method
     */
    typedef enum LinearSolverRecordStatus {
                                            UNKNOWN_STATUS = -1,
											//
											REGULAR_MATRIX = 0,
                                            SINGULAR_MATRIX = 1,
                                            ILL_CONDITIONED_MATRIX = 2,
											//
											LINEAR_SOLVER_SUCCEDED = 3,
											LINEAR_SOLVER_FAILED = 4
    } LinearSolverRecordStatus;
     
	/** A simple structure to hold the results of an SLE solution process.
	 *  All variables in this class are public.
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT LinearSolverRecord
    {
        public:

        	/** Creates a new 1D optimizer record
        	 */
            LinearSolverRecord(void);

            /** Destructor
             */          
            ~LinearSolverRecord(void);

            /** The solution to the SLE.  Note that if the SLE's right-side term is
             *  a ColumnVector, then the solution can be cast back to a ColumnVector.
             */
            Matrix *sol;
            
            /** Matrix singularity status.
             *  <ul>
             *      <li> if REGULAR_MATRIX, the solution is probably good;</li>
             *      <li> if ILL_CONDITIONED_MATRIX, use with caution, maybe you
             *          should be using a LinearSolver_SVD instead;</li>
             *      <li> if SINGULAR_MATRIX, then the solution is probably worthless.</li>
             *  </ul>
             */
            LinearSolverRecordStatus status;
			
			
		private:
		
			/**	Copy constructor: disabled.
			 *	Declared just to eliminate gcc warning.  This function makes no sense
			 *	in terms of what a LinearSolverRecord is.
			 *	@param	obj		the object to copy
			 */
			LinearSolverRecord(const LinearSolverRecord& obj);
			
			/**	Copy operator: disabled.
			 *	Declared just to eliminate gcc warning.  This function makes no sense
			 *	in terms of what a LinearSolverRecord is.
			 *	@param	obj		the object to copy
			 */
			const LinearSolverRecord& operator = (const LinearSolverRecord& obj);
			
    };
}   

#endif  //  LINEAR_SOLVER_RECORD_H
