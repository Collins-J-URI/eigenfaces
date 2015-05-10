/*  NAME:
    LinearSolver_SOR.h
 
  DESCRIPTION:
    LinearSolver_SOR public header.
 
  COPYRIGHT:
    (c) 2010-2014, 3D Group for Interactive Visualization
              University of Rhode Island.
 
    Licensed under the Academic Free License - v. 2.1
    For more information see http://opensource.org/licenses/afl-2.1.php
*/
#ifndef URIVL_LINEAR_SOLVER_SOR_H
#define URIVL_LINEAR_SOLVER_SOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "LinearSolver.h"

namespace uriVL
{
    /** Class that implement the Successive Over-relaxation (SOR) iterative algorithm for solving SLEs.
	 *
     *  <p><b>Important note about LinearSolver classes</b>:  These classes have been built for
     *  speed, not for OOP orthodoxy.  We try to minimize as much as possible the number of
     *  malloc/delete operations.  The default behavior of most functions in these 
     *  classes is to only keep references to the SLE's matrix, not copies of it, and to
     *  perform claculations directly in that matrix (destructive behavior).  If this 
     *  behavior is not desirable for your application, then call the function with the
     *  parameter preserveMatrix set to true, when available.</p>
     *
     *  @author     jean-yves herve',
     *                      3D Group for Interactive Visualization,
     *                      Department of Computer Science and Statistics,
     *                      University of Rhode Island
     */
    class URIVL_EXPORT LinearSolver_SOR : public LinearSolver
    {
        public:

            /** Space is allocated for a new LinearSolver_SOR object. That matrix must have
             *  as many rows as it has columns otherwise the call will fail (throw
             *  an exception or make an assert call depending on the application settings).
             *  Only create a LinearSolver if you are going to reuse the same matrix for
             *  different right-side terms, or at least matrices with the same dimensions.
             *  Otherwise, just use the static solve function
             * @param nbRows the maximum number of rows in the matrix
             * @param nbCols the maximum number of columns in the matrix
             */
            LinearSolver_SOR(int nbRows, int nbCols);


            /** Constructor.  A LinearSolver_SOR is created for a specific matrix.
             *  That matrix must be square otherwise the call will fail (throw an
             *  exception or make an assert call depending on the application settings).
             *  Only create a LinearSolver if you are going to reuse the same matrix for
             *  different right-side terms, or at least matrices with the same dimensions.
             *  Otherwise, just use the static solve function
             *
             *  @param  A   the matrix of SLEs to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             */
            LinearSolver_SOR(Matrix* A, bool preserveMatrix=false);
            
//            /** Constructor.  A LinearSolver_LU is created for a specific dimension of the SLE
//             *  and number of columns of the right-side term.  This is the version of the 
//             *  constructor to use if you are going to always operate in destructive mode and
//             *  are going to request a couple of steps of incremental improvement of the solution.
//             *  Only create a LinearSolver if you are going to reuse the same matrix for
//             *  different right-side terms, or at least matrices with the same dimensions.
//             *  Otherwise, just use the static solve function
//             *
//             *  @param  n  dimension of the SLE
//             *  @param  nbRighSideCols  number of columns of the right-side term (if known in 
//             *                          advance.
//             */
//            LinearSolver_SOR(int n, int nbRighSideCols=1);
            
            
            /** Destructor
             */
            ~LinearSolver_SOR(void);
            
            /** Assigns a new matrix to this LinearSolver.  This call only makes sense 
             *  if the new matrix has the same dimensions as the previous one (it then saves
             *  a destruction and an allocation of a matrix). Otherwise, just use the 
             *  static solve function.
             *  <p><b>Important:</b> If you don't set <code>preserveMatrix</code> true, then 
             *  the LinearSolver only stores a reference to matrix it receives as a parameter.
             *  It will do calculations in this matrix. 
             *  <ol>
             *      <li>You cannot delete it before you call the <code>solve</code> function,</li>
             *      <li>The matrix's elements will be modified (destructive behavior)
             *  </ol>
             *  @param  A   the matrix of SLEs to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             */
            void setMatrix(Matrix* A, bool preserveMatrix=false);
            
            /** Solves the SLE for the matrix assigned  through the constructor or through
             *  a call to setMatrix and the matrix right-side term received as parameter
             *  @param  b   the reigh-side term of the SLE to solve
             *  @return a LinearSolverRecord object that stores the solution to the SLE
             *          and indication about the matrix's regularity.
             */
            LinearSolverRecord* solve(Matrix *b);
            
            /** Solves the SLE for the matrix assigned  through the constructor or through
             *  a call to setMatrix and the matrix right-side term received as parameter
             *  @param  b   the reigh-side term of the SLE to solve
             *  @param  incrImprov  the number of steps of incremental improvement of the 
             *                      solution (by solving for corrective term with residual as
             *                      right-side term).  It is highly recommended tp apply at least
             *                      1 such step.  2-3 should be plenty enough.
             *  @return a LinearSolverRecord object that stores the solution to the SLE
             *          and indication about the matrix's regularity.
             */
            LinearSolverRecord* solve(Matrix *b, int incrImprov);
            
            /** Solves the SLE for the matrix assigned  through the constructor or through
             *  a call to setMatrix and the matrix right-side term received as parameter.
			 *	Note that this version of the function uses the solution matrix stored by
			 *	the LinearSolverRecord received as parameter as an initial state for the iteration.
             *  @param  b   the reigh-side term of the SLE to solve
             *  @param  record stores the solution to the SLe and indication about the 
             *          matrix's regularity.
             */
            void solve(Matrix *b, LinearSolverRecord *record);


            /** Solves the SLE for the matrix assigned  through the constructor or through
             *  a call to setMatrix and the matrix right-side term received as parameter.
			 *	Note that this version of the function uses the solution matrix stored by
			 *	the LinearSolverRecord received as parameter as an initial state for the iteration.
             *  @param  b   the reigh-side term of the SLE to solve
             *  @param  incrImprov  the number of steps of incremental improvement of the 
             *                      solution (by solving for corrective term with residual as
             *                      right-side term).  It is highly recommended tp apply at least
             *                      1 such step.  2-3 should be plenty enough.
             *  @param  record stores the solution to the SLe and indication about the 
             *          matrix's regularity.
             */
            void solve(Matrix *b, LinearSolverRecord *record, int incrImprov);


            /** Solves an SLE for the matrix and right-side term received as parameters
             *  @param  A   the matrix of the SLE to solve
             *  @param  B   the right-side term of the SLE to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             *  @param  incrImprov  the number of steps of incremental improvement of the 
             *                      solution (by solving for corrective term with residual as
             *                      right-side term).  It is highly recommended tp apply at least
             *                      1 such step.  2-3 should be plenty enough.
             *  @return a LinearSolverRecord object that stores the solution to the SLE
             *          and indication about the matrix's regularity.
             */
            static LinearSolverRecord* solve(Matrix* A, Matrix* B, 
                                             bool preserveMatrix=false);
 
            /** Solves an SLE for the matrix and right-side term received as parameters
             *  @param  A   the matrix of the SLE to solve
             *  @param  B   the right-side term of the SLE to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             *  @param  incrImprov  the number of steps of incremental improvement of the 
             *                      solution (by solving for corrective term with residual as
             *                      right-side term).  It is highly recommended tp apply at least
             *                      1 such step.  2-3 should be plenty enough.
             *  @return a LinearSolverRecord object that stores the solution to the SLE
             *          and indication about the matrix's regularity.
             */
            static LinearSolverRecord* solve(Matrix* A, Matrix* B, 
                                             int incrImprov, bool preserveMatrix=false);
 
            /** Solves an SLE for the matrix and right-side term received as parameters.
			 *	Note that this version of the function uses the solution matrix stored by
			 *	the LinearSolverRecord received as parameter as an initial state for the iteration.
             *  @param  A   the matrix of the SLE to solve
             *  @param  B   the right-side term of the SLE to solve
             *  @param  record stores the solution to the SLe and indication about the 
             *          matrix's regularity.
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             *  @param  incrImprov  the number of steps of incremental improvement of the 
             *                      solution (by solving for corrective term with residual as
             *                      right-side term).  It is highly recommended tp apply at least
             *                      1 such step.  2-3 should be plenty enough.
             */
            static void solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                              bool preserveMatrix=false);
 
            
            /** Solves an SLE for the matrix and right-side term received as parameters.
			 *	Note that this version of the function uses the solution matrix stored by
			 *	the LinearSolverRecord received as parameter as an initial state for the iteration.
             *  @param  A   the matrix of the SLE to solve
             *  @param  B   the right-side term of the SLE to solve
             *  @param  record stores the solution to the SLe and indication about the 
             *          matrix's regularity.
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             *  @param  incrImprov  the number of steps of incremental improvement of the 
             *                      solution (by solving for corrective term with residual as
             *                      right-side term).  It is highly recommended tp apply at least
             *                      1 such step.  2-3 should be plenty enough.
             */
            static void solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                              int incrImprov, bool preserveMatrix=false);
 
            
        private:
            
            /** Minimum value for a pivot before the matrix is considered to be 
             *  ill-conditioned
             */
            static const double STOP_ITERATION_THRESHOLD;
        
            /** Matrix of the SLE. Only an alias if preserveMatrix was false.
             */
            Matrix* A_;
			
			/**	Work matrix for the iterated solution
			 */
			Matrix* currentX_;
			
			/**	Second work matrix for the iterated solution
			 */
			Matrix* previousX_;
			
			/**	Array storing the inverses of diagonal elements of the system's matrix
			 */
			double* invDiag_;
			
            
            /** Copy constructor: disabled.
			 *	@param	obj		reference to the object to copy
             */
            LinearSolver_SOR(const LinearSolver_SOR& obj);

            /** Copy operator: disabled.
			 *	@param	obj		reference to the object to copy
             */
            const LinearSolver_SOR& operator = (const LinearSolver_SOR& obj);


            /** Solves an SLE for the matrix and right-side term received as parameters
             *  @param  A			the matrix of the SLE to solve
             *  @param  B			the right-side term of the SLE to solve
             *  @param  record		stores the solution to the SLe and indication about the 
             *						matrix's regularity
			 *	@param	currentX	one of two matrices used to compute the iterative solution
			 *	@param	proviousX	one of two matrices used to compute the iterative solution
			 *	@param	invDiag		1D array storing the inverses of the matrix's diagonal elements
             *  @param  incrImprov  the number of steps of incremental improvement of the 
             *                      solution (by solving for corrective term with residual as
             *                      right-side term).  It is highly recommended tp apply at least
             *                      1 such step.  2-3 should be plenty enough
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode)
             */
            static void solve_(Matrix* A, Matrix* B, LinearSolverRecord *record,
							   Matrix* currentX, Matrix* previousX, double* invDiag,
							   int incrImprov, bool preserveMatrix);
 

			/**
			 *
			 */
			static LinearSolverRecordStatus checkMatrixStatus_(Matrix* A, double* invDiag);

    };
}

#endif  //  LINEAR_SOLVER_SOR_H
