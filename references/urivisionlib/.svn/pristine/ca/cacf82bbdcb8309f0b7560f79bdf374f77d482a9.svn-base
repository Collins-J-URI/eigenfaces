/*  NAME:
        LinearSolver_Cholesky.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib LinearSolver_Cholesky class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColumnVector.h"
#include "LinearSolver_Cholesky.h"

using namespace uriVL;

LinearSolver_Cholesky::LinearSolver_Cholesky(Matrix* A, bool preserveMatrix)
        :   LinearSolver()
{
}

LinearSolver_Cholesky::~LinearSolver_Cholesky(void)
{
}

void LinearSolver_Cholesky::setMatrix(Matrix* A, bool preserveMatrix)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_Cholesky::solve(Matrix *B)
{
    return solve(B, 0);
}

LinearSolverRecord* LinearSolver_Cholesky::solve(Matrix *B, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_Cholesky");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
//    FAIL_CONDITION( nbRows != LU_->getNbRows(),
//                    kInvalidParameterError,
//                    "Right-side term's dimensions don't match that of the matrix");

    LinearSolverRecord *record = new LinearSolverRecord();
    if (nbCols == 1)
        record->sol = new ColumnVector(nbRows);
    else
        record->sol = new Matrix(nbRows, nbCols);
        
    solve(B, record, incrImprov);
    
    return record;
}

void LinearSolver_Cholesky::solve(Matrix *B, LinearSolverRecord *record)
{
    solve(B, record, 0);
}

void LinearSolver_Cholesky::solve(Matrix *B, LinearSolverRecord *record, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_Cholesky");
    FAIL_CONDITION( record == NULL,
                    kNullParameterError,
                    "Null LinearSolverRecord term sent to a LinearSolver_Cholesky");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
//    FAIL_CONDITION( nbRows != LU_->getNbRows(),
//                    kInvalidParameterError,
//                    "Right-side term's dimensions don't match that of the matrix");

    Matrix* X = record->sol;
    //  if the solution was not allocated we do it now
    if (X == NULL)
    {
        if (nbCols == 1)
            record->sol = new ColumnVector(nbRows);
        else
            record->sol = new Matrix(nbRows, nbCols);

    }
    else
    {
        FAIL_CONDITION( nbRows != X->getNbRows(),
                        kInvalidParameterError,
                        "Solution's dimensions don't match that of the SLE");
        FAIL_CONDITION( nbCols != X->getNbCols(),
                        kInvalidParameterError,
                        "Solution's dimensions don't match that of the right-side term");
    }


    //  do something

    record->status = getSingularityStatus_();
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_Cholesky::solve(Matrix* A, Matrix* B, bool preserveMatrix) 
{
    return solve(A, B, 0, preserveMatrix);
}


LinearSolverRecord* LinearSolver_Cholesky::solve(Matrix* A, Matrix* B, int incrImprov,
                                           bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_Cholesky");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_Cholesky");
    const int aRows = A->getNbRows(),
              aCols = A->getNbCols();
    FAIL_CONDITION( aRows != aCols,
                    kInvalidParameterError,
                    "Nonsquare matrix assigned to a LinearSolver_Cholesky");
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_Cholesky");

    LinearSolverRecord *record = new LinearSolverRecord();
    if (nbCols == 1)
        record->sol = new ColumnVector(nbRows);
    else
        record->sol = new Matrix(nbRows, nbCols);
     
       
    solve_(A, B, record, preserveMatrix, incrImprov);
    
    return record;
}

void LinearSolver_Cholesky::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            bool preserveMatrix) 
{
    solve(A, B, record, 0, preserveMatrix);
}


void LinearSolver_Cholesky::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            int incrImprov, bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_Cholesky");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_Cholesky");
    const int aRows = A->getNbRows(); /*,
              aCols = A->getNbCols();*/
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_Cholesky");

    //  was the record properly created?
    if (record->sol != NULL)
    {
        FAIL_CONDITION( (nbRows != record->sol->getNbRows()) || 
                        (nbCols != record->sol->getNbCols()),
                        kInvalidParameterError,
                        "right-side term and solution's dimensions dont match for LinearSolver_Cholesky");
    }
    //  if the solution was not allocated we do it now
    else
    {
        if (nbCols == 1)
            record->sol = new ColumnVector(nbRows);
        else
            record->sol = new Matrix(nbRows, nbCols);

    }
    
    solve_(A, B, record, preserveMatrix, incrImprov);
}

void LinearSolver_Cholesky::solve_(Matrix* A, Matrix* B, LinearSolverRecord *record,
                             int incrImprov, bool preserveMatrix) 
{
}


#if 0
#pragma mark -
#endif
