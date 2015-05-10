/*  NAME:
        LinearSolver_SVD.h
 
    DESCRIPTION:
        LinearSolver_SVD public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_LINEAR_SOLVER_SVD_H
#define URIVL_LINEAR_SOLVER_SVD_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "LinearSolver.h"
#include "RowVector.h"

#ifndef TINY
#define TINY 1.5e-16
#endif

namespace uriVL
{
    /** Class that implement the Singular Value Decomposition algorithm for solving
     *  arbitrary SLEs.
     *
     *  <p><b>Important note about LinearSolver classes</b>:  These classes have been built for
     *  speed, not for OOP orthodoxy.  We try to minimize as much as possible the number of
     *  malloc/delete operations.  The default behavior of most functions in these 
     *  classes is to only keep references to the SLE's matrix, not copies of it, and to
     *  perform claculations directly in that matrix (destructive behavior).  If this 
     *  behavior is not desirable for your application, then call the function with the
     *  parameter preserveMatrix set to true, when available.</p>
     *
     *  <b>IP note:</b> The factorization code is everywhere largely inspired and in 
     *  most places simply copied/adapted from the public domain Jama implementation of SVD:
     *      http://math.nist.gov/javanumerics/jama/
     *  I am not quite sure why jama does a non-standard decomposition in which the U matrix is
	 *	m x n rather than m x m and the diagonal matrix is n x n rather than m x n.
	 *
     *  @author     jean-yves herve',
     *                      3D Group for Interactive Visualization,
     *                      Department of Computer Science and Statistics,
     *                      University of Rhode Island
     */
    class URIVL_EXPORT LinearSolver_SVD : public LinearSolver
    {

         public :

            /** Space is allocated for a new LinearSolver_SVD object. That matrix must have
             *  at least as many rows as it has columns otherwise the call will fail (throw
             *  an exception or make an assert call depending on the application settings).
             *  Only create a LinearSolver if you are going to reuse the same matrix for
             *  different right-side terms, or at least matrices with the same dimensions.
             *  Otherwise, just use the static solve function
             *  @param nbRows   the maximum number of rows in the matrix
             *  @param nbCols   the maximum number of columns in the matrix
             */
            LinearSolver_SVD(int nbRows, int nbCols);


            /** Constructor.  A LinearSolver_SVD is created for a specific matrix.
             *  Only create a LinearSolver if you are going to reuse the same matrix for
             *  different right-side terms, or at least matrices with the same dimensions.
             *  Otherwise, just use the static solve function
             *
             *  @param  A   the matrix of SLEs to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             */
            LinearSolver_SVD(Matrix* a, bool preserveMatrix=false);
            

            /** Destructor
             */
            ~LinearSolver_SVD(void);
            

			/**	returns a pointer to the private V matrix
			 *	@return pointer to the private V matrix
			 */
			const Matrix* getV(void)
			{
				return V_;
			}
			
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
             *  @param  A  the matrix of SLEs to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             */
            void setMatrix(Matrix* a, bool preserveMatrix=false);

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
             *  a call to setMatrix and the matrix right-side term received as parameter
             *  @param  b   the reigh-side term of the SLE to solve
             *  @param  record stores the solution to the SLe and indication about the 
             *          matrix's regularity.
             */
            void solve(Matrix *b, LinearSolverRecord *record);


            /** Solves the SLE for the matrix assigned  through the constructor or through
             *  a call to setMatrix and the matrix right-side term received as parameter
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
 
            /** Solves an SLE for the matrix and right-side term received as parameters
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
 
            
            /** Solves an SLE for the matrix and right-side term received as parameters
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
        
            /** The larger of the two dimensions of the SLE's matrix
             */
            int bigDim_;
            
            
            /** The smaller of the two dimensions of the SLE's matrix
             */
            int smallDim_;
            
            
            /** (Possibly) Local copy of the SLE's matrix
             */
            Matrix *A_;
            
            /** Left singular matrix
             */
            Matrix *U_;
            
            
            /** Right singular matrix
             */
            Matrix *V_;
            
            
            /** Array of diagonal singular values
             */
            RowVector *S_;
            
             
            /**                                                    t
			 *  Computes the SVD factorization of an SLE: A = U.S.V,
			 *	Where A is  m x n,  U is m x n orthogonal, S is  n x n  diagonal,
			 *	and V is  n x n  orthogonal
			 *	@param A	the matrix to factorize
			 *	@param U	the first orthogonal term of the SVD factorization
			 *	@param S	the second diagonal term of the SVD factorization
			 *	@param V	the transpose of third orthogonal term of the SVD factorization
             *  @return     a matrix singularity status
             */
            static LinearSolverRecordStatus factorize_(Matrix* A, Matrix* U, RowVector* S, 
														Matrix* V);

            /** Copy constructor: disabled.
			 *	@param	obj		reference to the object to copy
             */
            LinearSolver_SVD(const LinearSolver_SVD& obj);

            /** Copy operator: disabled.
			 *	@param	obj		reference to the object to copy
             */
            const LinearSolver_SVD& operator = (const LinearSolver_SVD& obj);


            /** Solves an SLE for the matrix SVD decomposition and right-side term received as parameters
			 *	@param U	the first orthogonal term of the SVD factorization
			 *	@param S	the second diagonal term of the SVD factorization
			 *	@param V	the transpose of third orthogonal term of the SVD factorization
             *  @param  B   the right-side term of the SLE to solve
             *  @param  record stores the solution to the SLe and indication about the 
             *          matrix's regularity.
             */
            static void solve_(Matrix* U, RowVector* S, Matrix* V, Matrix* B, LinearSolverRecord *record);
 

    };
}

#endif  //  LINEAR_SOLVER_SVD_H

