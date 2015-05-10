/*  NAME:
        LinearSolver.h
 
    DESCRIPTION:
        LinearSolver public header.
 
    COPYRIGHT:
       (c) 2003-2014, 3D Group for Interactive Visualization
             University of Rhode Island.
 
    Licensed under the Academic Free License - v. 2.1
    For more information see http://opensource.org/licenses/afl-2.1.php
*/
#ifndef URIVL_LINEAR_SOLVER_H
#define URIVL_LINEAR_SOLVER_H

#ifndef TINY
#define TINY 1.5e-16
#endif

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Matrix.h"
#include "LinearSolverRecord.h"

namespace uriVL
{
    /** Parent class to all classes that implement an algorithm for solving a
     *  system of linear equations.
     *  <b>Important note about LinearSolver classes</b>:  These classes have been built for
     *  speed, not for OOP orthodoxy.  We try to minimize as much as possible the number of
     *  malloc/delete operations.  The default behavior of most functions in these 
     *  classes is to only keep references to the SLE's matrix, not copies of it, and to
     *  perform claculations directly in that matrix (destructive behavior).  If this 
     *  behavior is not desirable for your application, then call the function with the
     *  parameter preserveMatrix set to true, when available.
     *
     *  @author     jean-yves herve',
     *                      3D Group for Interactive Visualization,
     *                      Department of Computer Science and Statistics,
     *                      University of Rhode Island
     */
    class URIVL_EXPORT LinearSolver
    {
        public:

            /** Destructor
             */
            virtual ~LinearSolver(void);
            
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
             *  @param  b   the right-side term of the SLE to solve
             *  @return a LinearSolverRecord object that stores the solution to the SLE
             *          and indication about the matrix's regularity.
             */
            virtual LinearSolverRecord* solve(Matrix *b) = 0;
            
            /** Solves the SLE for the matrix assigned  through the constructor or through
             *  a call to setMatrix and the matrix right-side term received as parameter
             *  @param  b   the reigh-side term of the SLE to solve
             */
            virtual void solve(Matrix *b, LinearSolverRecord *record) = 0;
            

        protected:
        
            /** Default constructor
             */
            LinearSolver(void);
            
           /** Constructor: initializes the dimensions of the linear solver
             * @param nbRows the maximum number of rows in the matrix
             * @param nbCols the maximum number of columns in the matrix
             */
            LinearSolver(int nbRows, int nbCols);
        
           /** Constructor: initializes the dimensions of the linear solver
             *
             *  @param  A   the matrix of SLEs to solve
             *  @param  preserveMatrix  when true, the LinearSolver will keep and work on a copy of 
             *                          the matrix (non-destructive mode).
             */
            LinearSolver(Matrix* a, bool preserveMatrix=false);
			
			/**	Sets the singularityStatus_.  The conditions for singularity are solver-dependent.
			 *	The child class solver makes its own singularity test and sets the value of
			 *	singularityStatus_
			 *	@see singularityStatus_
			 *	@param	singularityStatus	new value for singularityStatus_
			 */
			void setSingularityStatus_(LinearSolverRecordStatus singularityStatus);
			
			/**	Get the singularityStatus_
			 *	@see singularityStatus_
			 *	@return value of singularityStatus_
			 */
			LinearSolverRecordStatus getSingularityStatus_(void);
			
            /**	Reports the number of rows of the solver's matrix.
			 *	@return		number of rows of the solver's matrix
			 */
			int getNbRows_(void);
			
            /**	Reports the number of rows of the solver's matrix.
			 *	@return		number of rows of the solver's matrix
			 */
			int getNbCols_(void);
			
			/**	Sets the dimensions of the solver's matrix
			 *	@param	nbRows		new number of rows
			 *	@param	nbCols		new number of columns
			 */
			void setMatrixDimensions_(int nbRows, int nbCols);

            /**	Reports the number of storage rows of the solver's matrix.
			 *	@return		number of storage rows of the solver's matrix
			 */
			int getNbStorageRows_(void);
			
            /**	Reports the number of storage rows of the solver's matrix.
			 *	@return		number of storage rows of the solver's matrix
			 */
			int getNbStorageCols_(void);
			
			/**	Sets the storage dimensions of the solver's matrix.
             *  To facilitate the reuse of a matrix object
             *  in video processing applications, the matrix's data array may be allocated at a 
             *  larger size (the "storage" dimensions) than the matrix's nominal dimensions.
			 *	@param	nbRows		new number of storage rows
			 *	@param	nbCols		new number of storage columns
			 */
			void setMatrixStorageDimensions_(int nbRows, int nbCols);

			/**	Reports whether the solver keeps a local copy of the SLE's matrix
			 *	@return  true if the solver keeps a local copy of the SLE's matrix
			 */
			bool getMatrixIsLocal_(void);

			/**	Sets whether the solver keeps a local copy of the SLE's matrix
			 *	@param  matrixIsLocal indicates whether the solver keeps a local copy of the SLE's matrix
			 */
			void setMatrixIsLocal_(bool matrixIsLocal);

		private:

            /** Effective number of rows of the matrix
             */
            int nbRows_;
            
            /** Effective number of columns of the matrix
             */
            int nbCols_;
            
            /** Number of storage rows of the matrix, in case the matrix is stored locally.  
             *  To facilitate the reuse of a matrix object
             *  in video processing applications, the matrix's data array may be allocated at a 
             *  larger size (the "storage" dimensions) than the matrix's nominal dimensions
             */
            int nbStorageRows_;
            
            /** Number of storage columns of the matrix, in case the matrix is stored locally.  
             *  To facilitate the reuse of a matrix object
             *  in video processing applications, the matrix's data array may be allocated at a 
             *  larger size (the "storage" dimensions) than the matrix's nominal dimensions
             */
            int nbStorageCols_;
            /** Indicates whether this object stores a local copy of the SLE's matrix or just
             *  a reference to that matrix
             */
            bool matrixIsLocal_;
            
            /** Set by factorization functions
             */
            LinearSolverRecordStatus singularityStatus_;
            
            
    };
}

#endif  //  LINEAR_SOLVER_H
