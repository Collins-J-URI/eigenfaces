/*  NAME:
        LinearSolver_GaussSeidel.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib LinearSolver_GaussSeidel class
 
    COPYRIGHT:
        (c) 2010-2014,	3D Group for Interactive Visualization
					University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <cmath>
//
#include "ColumnVector.h"
#include "LinearSolver_GaussSeidel.h"

using namespace std;
using namespace uriVL;

//------------------------------
//  Redefines static members
//------------------------------
const double LinearSolver_GaussSeidel::STOP_ITERATION_THRESHOLD = 1.E-5;


//------------------------------
//  Constructors & destructors
//------------------------------

LinearSolver_GaussSeidel::LinearSolver_GaussSeidel(int nbRows, int nbCols)

    try :   LinearSolver(nbRows, nbCols),
			//
			A_(new Matrix(nbRows, nbCols)),
			currentX_(NULL),
			previousX_(NULL),
			invDiag_(NULL)
{
    if (nbRows != nbCols)
    {
		delete A_;
        FAIL_CONDITION( true,
                        kInvalidParameterError,
                        "Nonsquare SLE assigned to a LinearSolver_GaussSeidel");
    }
	setMatrixIsLocal_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by LinearSolver_GaussSeidel constructor");
	throw e;
}


LinearSolver_GaussSeidel::LinearSolver_GaussSeidel(Matrix* A, bool preserveMatrix)
    try	:   LinearSolver(A, preserveMatrix),
			//
			A_(preserveMatrix ? new Matrix(A) : A),
			currentX_(NULL),
			previousX_(NULL),
			invDiag_(NULL)
{
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_GaussSeidel");

	int nbRows = A->getNbRows();
	int nbCols = A->getNbCols();
	if (nbRows != nbCols)
	{
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Nonsquare matrix assigned to a LinearSolver_GaussSeidel");
	}

	invDiag_ = new double[nbRows];
	setSingularityStatus_(checkMatrixStatus_(A_, invDiag_));
}
catch (ErrorReport& e) {
	e.appendToMessage("called by LinearSolver_GaussSeidel constructor");
	throw e;
}

LinearSolver_GaussSeidel::LinearSolver_GaussSeidel(const LinearSolver_GaussSeidel& obj)
		:   LinearSolver(),
			//
			A_(NULL),
			currentX_(NULL),
			previousX_(NULL),
			invDiag_(NULL)
{
	FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"LinearSolver_GaussSeidel copy constructor not inplemented");
}



LinearSolver_GaussSeidel::~LinearSolver_GaussSeidel(void)
{
	if (getMatrixIsLocal_() && (A_ != NULL))
		delete A_;
	if (currentX_ != NULL)
		delete currentX_;
	if (previousX_ != NULL)
		delete previousX_;
	if (invDiag_ != NULL)
		delete []invDiag_;

}




const LinearSolver_GaussSeidel& LinearSolver_GaussSeidel::operator = (const LinearSolver_GaussSeidel& obj)
{
	FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"LinearSolver_GaussSeidel copy opertator not inplemented");
	return *this;
}



void LinearSolver_GaussSeidel::setMatrix(Matrix* A, bool preserveMatrix)
{
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_GaussSeidel");
    const int nbRows = A->getNbRows(),
              nbCols = A->getNbCols();
    FAIL_CONDITION( nbRows != nbCols,
                    kInvalidParameterError,
                    "Nonsquare matrix assigned to a LinearSolver_GaussSeidel");

	if (invDiag_ != NULL)
	{
		if (A_->getNbRows() != nbRows) 
		{
			delete []invDiag_;
			invDiag_ = new double[nbRows];
		}
	}
	else 
	{
		invDiag_ = new double[nbRows];
	}

    setMatrixDimensions_(nbRows, nbRows);                     

    //  If we were already storing a local copy of a previous SLE's matrix,
    //  then we may have to do delete/reallocate that local copy
    if (getMatrixIsLocal_())   
    {
        if (preserveMatrix)
        {
            //  If the new matrix has different dimensions from the old one, we 
            //  must delete & reallocate LU_ and the permutation array
            if ((A_->getNbRows() != nbRows) || (A_->getNbCols() != nbCols))
            {
                if (A_ != NULL)
                    delete A_;
                A_ = new Matrix(A);
                
            }
            //  if the dimensions did not change we simply copy the data
            else
                (*A_) = (*A);

            setMatrixStorageDimensions_(nbRows, nbRows);
        }
        //  If the matrix was local, but should not be anymore, we simply delete the
        //  current matrix and store the reference to the new one
        else
        {
            delete A_;
            A_ = A;
        }
    }
    else
    {
        if (preserveMatrix)
            A_ = new Matrix(A);
        else
            A_  =A;
            
    }
        
    setMatrixIsLocal_(preserveMatrix);
    
	setSingularityStatus_(checkMatrixStatus_(A_, invDiag_));
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_GaussSeidel::solve(Matrix *B)
{
    return solve(B, 0);
}

LinearSolverRecord* LinearSolver_GaussSeidel::solve(Matrix *B, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_GaussSeidel");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( nbRows != getNbRows_(),
                    kInvalidParameterError,
                    "Right-side term's dimensions don't match that of the matrix");

    LinearSolverRecord *record = new LinearSolverRecord();
    if (nbCols == 1)
        record->sol = new ColumnVector(nbRows);
    else
        record->sol = new Matrix(nbRows, nbCols);
		
    solve(B, record, incrImprov);
    
    return record;
}

void LinearSolver_GaussSeidel::solve(Matrix *B, LinearSolverRecord *record)
{
    solve(B, record, 0);
}

void LinearSolver_GaussSeidel::solve(Matrix *B, LinearSolverRecord *record, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_GaussSeidel");
    FAIL_CONDITION( record == NULL,
                    kNullParameterError,
                    "Null LinearSolverRecord term sent to a LinearSolver_GaussSeidel");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( nbRows != getNbRows_(),
                    kInvalidParameterError,
                    "Right-side term's dimensions don't match that of the matrix");

    Matrix* X = record->sol;
    //  if the solution was not allocated we do it now
    if (X == NULL)
    {
        if (nbCols == 1)
            X = record->sol = new ColumnVector(nbRows);
        else
            X = record->sol = new Matrix(nbRows, nbCols);

		double* const* x = X->getMatrix(R_W_ACCESS);
		for (int i=0; i<nbRows; i++)
			for (int j=0; j<nbCols; j++)
				x[i][j] = 0.L;
		
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
    
	record->status = getSingularityStatus_();
    if ((record->status != SINGULAR_MATRIX) && (record->status != ILL_CONDITIONED_MATRIX))
	{
		Matrix* currentX = NULL;
		Matrix* previousX = NULL;

        if (nbCols == 1)
		{
			currentX = new ColumnVector(nbRows);
			previousX = new ColumnVector(nbRows);
		}	
        else
		{
			currentX = new Matrix(nbRows, nbCols);
			previousX = new Matrix(nbRows, nbCols);
		}
		
		solve_(A_, B, record, currentX, previousX, invDiag_, getMatrixIsLocal_(), incrImprov);

		delete currentX;
		delete previousX;
	}
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_GaussSeidel::solve(Matrix* A, Matrix* B, bool preserveMatrix) 
{
    return solve(A, B, 0, preserveMatrix);
}


LinearSolverRecord* LinearSolver_GaussSeidel::solve(Matrix* A, Matrix* B, int incrImprov,
                                           bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_GaussSeidel");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_GaussSeidel");
    const int aRows = A->getNbRows(),
              aCols = A->getNbCols();
    FAIL_CONDITION( aRows != aCols,
                    kInvalidParameterError,
                    "Nonsquare matrix assigned to a LinearSolver_GaussSeidel");
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_GaussSeidel");

    LinearSolverRecord *record = new LinearSolverRecord();
    if (nbCols == 1)
        record->sol = new ColumnVector(nbRows);
    else
        record->sol = new Matrix(nbRows, nbCols);
     
	double* const* x = record->sol->getMatrix(R_W_ACCESS);
	for (int i=0; i<nbRows; i++)
		for (int j=0; j<nbCols; j++)
			x[i][j] = 0.L;		
       
    solve(A, B, record, preserveMatrix, incrImprov);
    
    return record;
}

void LinearSolver_GaussSeidel::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            bool preserveMatrix) 
{
    solve(A, B, record, 0, preserveMatrix);
}


void LinearSolver_GaussSeidel::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            int incrImprov, bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_GaussSeidel");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_GaussSeidel");
    const int aRows = A->getNbRows(),
              aCols = A->getNbCols();
    FAIL_CONDITION( aRows != aCols,
                    kInvalidParameterError,
                    "Nonsquare matrix assigned to a LinearSolver_GaussSeidel");
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_GaussSeidel");

    //  was the record properly created?
    if (record->sol != NULL)
    {
        FAIL_CONDITION( (nbRows != record->sol->getNbRows()) || 
                        (nbCols != record->sol->getNbCols()),
                        kInvalidParameterError,
                        "right-side term and solution's dimensions dont match for LinearSolver_GaussSeidel");
    }
    //  if the solution was not allocated we do it now.  In any case, we need to alocate
	//	the "current" and "provious" iterated solutions
    else
    {
        if (nbCols == 1)
		{
            record->sol = new ColumnVector(nbRows);
		}	
        else
		{
            record->sol = new Matrix(nbRows, nbCols);
		}

		double* const* x = record->sol->getMatrix(R_W_ACCESS);
		for (int i=0; i<nbRows; i++)
			for (int j=0; j<nbCols; j++)
				x[i][j] = 0.L;		

   }
	
	Matrix* currentX = NULL;
	Matrix* previousX = NULL;

	if (nbCols == 1)
	{
		currentX = new ColumnVector(nbRows);
		previousX = new ColumnVector(nbRows);
	}	
	else
	{
		currentX = new Matrix(nbRows, nbCols);
		previousX = new Matrix(nbRows, nbCols);
	}
	
	double* invDiag = new double[nbRows];
	record->status = checkMatrixStatus_(A, invDiag);
	
    if ((record->status != SINGULAR_MATRIX) && (record->status != ILL_CONDITIONED_MATRIX))
	{
		solve_(A, B, record, currentX, previousX, invDiag, preserveMatrix, incrImprov);
	}
	
	delete currentX;
	delete previousX;
	delete []invDiag;
	
}

#if 0
#pragma mark -
#endif

void LinearSolver_GaussSeidel::solve_(Matrix* A, Matrix* B, LinearSolverRecord *record,
								 Matrix* currentX, Matrix* previousX, double* invDiag,
								 int incrImprov, bool preserveMatrix) 
{
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();

	//	if one of the diagonal elements is 0 we can't solve the SLE
	const double* const* a = A->getMatrix(); 
	double*const* x = record->sol->getMatrix(R_W_ACCESS);
	double*const* xk = currentX->getMatrix(R_W_ACCESS);
	double*const* xkp1 = previousX->getMatrix(R_W_ACCESS);
	const double*const* b = B->getMatrix(R_W_ACCESS);
       
	//	The initial state is the one provided by the record passed as parameter
	for (int i=0; i<nbRows; i++)
		for (int j=0; j<nbCols; j++)
			xkp1[i][j] = x[i][j];
	
	int nbIterIncrDist = 0;
	double prevDist = 1.0E10;
	bool notConverged = true;
	bool notDiverged = true;
	do 
	{
		//	swap the "current" and "previous" states
		{
			double* const* temp = xk;
			xk = xkp1;
			xkp1 = temp;
		}
		
		for (int i=0; i<nbRows; i++)
		{
			for (int j=0; j<nbCols; j++)
			{
				xkp1[i][j] = b[i][j];
				
				for (int l=0; l<i; l++)
					if (l != i)
						xkp1[i][j] -= a[i][l]*xkp1[l][j];
					
				for (int l=i+1; l<nbRows; l++)
					if (l != i)
						xkp1[i][j] -= a[i][l]*xk[l][j];
					
				xkp1[i][j] *= invDiag[i];
			}
		}

		//	compute the change between initial and previous state
		//	We use infinity norm: the distance for all terms must be
		//	less than the threshold
		double dist = 0.L;
		for (int i=0; i<nbRows; i++)
		{
			for (int j=0; j<nbCols; j++)
			{
				double dx = fabs(xkp1[i][j] - xk[i][j]);
				if (dx > dist)
					dist = dx;
			}
		}
		if (dist < STOP_ITERATION_THRESHOLD)
			notConverged = false;
		else if (dist > prevDist)
		{
			nbIterIncrDist++;
			if (nbIterIncrDist > 6)
				notDiverged = false;
		}
		else 
		{
			nbIterIncrDist = 0;
		}
	}
	while (notConverged && notDiverged);
	
	if (notDiverged) 
	{
		record->status = LINEAR_SOLVER_SUCCEDED;

		//	after we leave the loop, copy the last computed state into the record to return
		for (int i=0; i<nbRows; i++)
			for (int j=0; j<nbCols; j++)
				x[i][j] = xkp1[i][j];
	
	}
	else
	{
		record->status = LINEAR_SOLVER_FAILED;
	}
}


#if 0
#pragma mark -
#endif

LinearSolverRecordStatus LinearSolver_GaussSeidel::checkMatrixStatus_(Matrix* A, double* invDiag)
{
	const double* const* a = A->getMatrix(); 
    const int nbRows = A->getNbRows();
	bool isSingular = false;
	for (int i=0; (i<nbRows) && !isSingular; i++)
		if (a[i][i] != 0.L)
			invDiag[i] = 1.0L / a[i][i];
		else
			isSingular = true;

	if (isSingular)
		return SINGULAR_MATRIX;
	else
		return REGULAR_MATRIX;

}
