/*  NAME:
        LinearSolver_QR.h
 
    DESCRIPTION:
        LinearSolver_QR public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
             University of Rhode Island.
 
    Licensed under the Academic Free License - v. 2.1
    For more information see http://opensource.org/licenses/afl-2.1.php
*/
#ifndef URIVL_LINEAR_SOLVER_QR_H
#define URIVL_LINEAR_SOLVER_QR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "LinearSolver.h"

namespace uriVL
{
    /** Class that implement the Householder QR factorization algorithm for solving
     *  overdetermined SLEs.
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
    class URIVL_EXPORT LinearSolver_QR : public LinearSolver
    {
        public:

            /** Space is allocated for a new LinearSolver_QR object. That matrix must have
             *  at least as many rows as it has columns otherwise the call will fail (throw
             *  an exception or make an assert call depending on the application settings).
             *  Only create a LinearSolver if you are going to reuse the same matrix for
             *  different right-side terms, or at least matrices with the same dimensions.
             *  Otherwise, just use the static solve function
             * @param nbRows the maximum number of rows in the matrix
             * @param nbCols the maximum number of columns in the matrix
             */
            LinearSolver_QR(int nbRows, int nbCols);

            /** A LinearSolver_QR is created for a specific matrix.
             *  That matrix must have at least as many rows as it has columns otherwise 
             *  the call will fail (throw an
             *  exception or make an assert call depending on the application settings).
             *  Only create a LinearSolver if you are going to reuse the same matrix for
             *  different right-side terms, or at least matrices with the same dimensions.
             *  Otherwise, just use the static solve function.
             *
             *  @param  A   the matrix of SLEs to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             */
            LinearSolver_QR(Matrix* a, bool preserveMatrix=false);
            
            /** Destructor
             */
            ~LinearSolver_QR(void);
            
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

            /** Minimum value for the norm of a partial column before the matrix is considered
             *  to be ill-conditioned
             */
            static const double ILL_COND_THRESHOLD;

            /** Matrix in which the QR factorization calculations are done.  Initially, this matrix
             *  will be a copy of the input matrix A (if preserveMatrix was set true) or simply a
             *  reference to it (preserveMatrix false for "destructive" version)
             */
            Matrix *R_;
            
            /** 2D array that stores the v vectors of the Householder algorithm used to generate
             *  the orthogonal matrices of the factorization.  The vectors are stored on <b>rows</b>
             *  of this array.  If the SLE'smatrix is mxn, then the first vector v_[0] has m 
             *  elements and the last vector v_[n-1] has m-n-1 elements.
             */
            double** v_;
            
            /** Vector used to store (part of) a column of R_
             */
            double* colR_;

            /** Copy constructor: disabled.
			 *	@param	obj		reference to the object to copy
             */
            LinearSolver_QR(const LinearSolver_QR& obj);

            /** Copy operator: disabled.
			 *	@param	obj		reference to the object to copy
             */
            const LinearSolver_QR& operator = (const LinearSolver_QR& obj);


            /** Computes the QR factorization of an SLE
             *  @param  m       number of rows of the SLE
             *  @param  n       number of columns of the SLE
             *  @param  a       the matrix of the SLE (will store R in output)
             *  @param  colR    stores (part of) a column  of a for faster calculations
             *  @param  v       stores the construction vectors of Q^t (by row)
             *  @return     a matrix singularity status
             */
            static LinearSolverRecordStatus factorize_(int m, int n, double* const*const a,
                                                      double* const colR, double* const*const v);

            /** Computes the Householder construction vector for a partial column of the matrix
             *  @param  n       dimension of the partial column
             *  @param  a       the partial column to operate on
             *  @param  v       the construction vector calculated for a
             *  @param  alphaV  will store in output the signed Euclidean norm of a
             *  @return     a matrix singularity status
             */
            static LinearSolverRecordStatus computeHouseholderV_(int n, double* const a, 
                                                                double* const v,  double* alphaV);

            /** Solves an SLE for the matrix and right-side term received as parameters
             *  @param  A   the matrix of the SLE to solve
             *  @param  B   the right-side term of the SLE to solve
             *  @param  record stores the solution to the SLe and indication about the 
             *          matrix's regularity.
             *  @param  incrImprov  the number of steps of incremental improvement of the 
             *                      solution (by solving for corrective term with residual as
             *                      right-side term).  It is highly recommended tp apply at least
             *                      1 such step.  2-3 should be plenty enough.
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             */
            static void solve_(Matrix* A, Matrix* B, LinearSolverRecord *record,
                               int incrImprov, bool preserveMatrix);
 
            /** Solves an SLE for the matrix and right-side term received as parameters
             *  @param  nbRows  number of rows of the SLE
             *  @param  nbCols  number of columns of the SLE
             *  @param  r   the upper triangular matrix of the QR factorization
             *  @param  b   the right-side term of the SLE (after multiplication by Q^T
             *  @param  x   the solution to the SLE
             */
            static void solveBS_(int nbRows, int nbCols, const double* const*const r, 
								 const double* const* const b, double* const*const x);
 

            /** Applies the Q matrix of a QR factorization (stored under the form of Householder 
             *  V vectors) to the right side of an SLE.
             *  @param  nbRows  number of rows of the SLE's matrix
             *  @param  nbColsR  number of columns of the SLE's matrix
             *  @param  nbColsB  number of columns of the right-side term
             *  @param  v   the array of Householder vectors
             *  @param  b   (possibly, a copy of) the right-side term of the SLE.  At the end
             *              of the function, stores Q^T . b
             */
            static void applyQt_(int nbRows, int nbColsR, int nbColsB, const double* const*const v, 
								 double* const*const b);
    };
}

#endif  //  LINEAR_SOLVER_QR_H
