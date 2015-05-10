/*  NAME:
        LinearSolver_Cholesky.h
 
    DESCRIPTION:
        LinearSolver_Cholesky public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_LINEAR_SOLVER_CHOLESKY_H
#define URIVL_LINEAR_SOLVER_CHOLESKY_H

#ifndef TINY
#define TINY 1.5e-16
#endif

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "LinearSolver.h"

namespace uriVL
{
    class URIVL_EXPORT LinearSolver_Cholesky : public LinearSolver
    {

         public :

            /** Constructor.  A LinearSolver_Cholesky is created for a specific matrix.
             *  That matrix must be square, positive definite, symmetric, otherwise the call 
             *  will fail (throw an
             *  exception or make an assert call depending on the application settings).
             *  Only create a LinearSolver if you are going to reuse the same matrix for
             *  different right-side terms, or at least matrices with the same dimensions.
             *  Otherwise, just use the static solve function
             *
             *  @param  A   the matrix of SLEs to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             */
            LinearSolver_Cholesky(Matrix* a, bool preserveMatrix=false);
            
            /** Destructor
             */
            ~LinearSolver_Cholesky(void);
            
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
             *  @param  a   the matrix of SLEs to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             */
            virtual void setMatrix(Matrix* a, bool preserveMatrix=false) = 0;
            
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
 


    };
}
#endif  //  LINEAR_SOLVER_CHOLESKY_H

//LinearSolver_Cholesky.h
