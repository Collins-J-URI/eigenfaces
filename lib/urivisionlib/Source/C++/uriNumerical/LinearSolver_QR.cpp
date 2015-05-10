/*  NAME:
        LinearSolver_QR.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib LinearSolver_QR class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <algorithm>
#include <iostream>
#include <cmath>
//
#include "ColumnVector.h"
#include "LinearSolver_LU.h"
#include "LinearSolver_QR.h"

using namespace std;
using namespace uriVL;

//------------------------------
//  Redefines static members
//------------------------------
const double LinearSolver_QR::ILL_COND_THRESHOLD = 1.E-7;


//------------------------------
//  Constructors & destructors
//------------------------------

LinearSolver_QR::LinearSolver_QR(int nbRows, int nbCols)

    try :   LinearSolver(nbRows, nbCols),
            //
            R_(new Matrix(nbRows, nbCols)),
            v_(new double*[nbCols]),
            colR_ (new double[nbRows])
{
    if (nbRows < nbCols)
    {
        delete R_;
        delete []v_;
        delete []colR_;
        
        FAIL_CONDITION( true,
                        kInvalidParameterError,
                        "Underdetermined SLE assigned to a LinearSolver_QR");

    }

    for (int j=0, lv=nbRows; j<nbCols; j++, lv--)
        v_[j] = new double[lv];                       
}
catch (ErrorReport& e) {
	e.appendToMessage("called by LinearSolver_QR constructor");
	throw e;
}


LinearSolver_QR::LinearSolver_QR(Matrix* A, bool preserveMatrix)
     try   :	LinearSolver(A, preserveMatrix),
				//
				R_(preserveMatrix ? new Matrix(A) : A),
				v_(new double*[A->getNbCols()]),
				colR_(new double[A->getNbRows()])				
{
    const int nbRows = A->getNbRows();
    const int nbCols = A->getNbCols();
    
	if (nbRows < nbCols)
	{
		delete []v_;
		delete []colR_;
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Underdetermined SLE assigned to a LinearSolver_QR");
	}
	
	setMatrixDimensions_(nbRows, nbCols);
    if (preserveMatrix)
    {
		setMatrixStorageDimensions_(nbRows, nbCols);
    }
    setMatrixIsLocal_(preserveMatrix);
    
    for (int j=0, lv=nbRows; j<nbCols; j++, lv--)
        v_[j] = new double[lv];                       
    
    double* const*const r = R_->getMatrix(R_W_ACCESS);
    setSingularityStatus_(factorize_(nbRows, nbCols, r, colR_, v_));
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by LinearSolver_QR constructor");
	throw e;
}
catch (...)
{
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_QR");
}


LinearSolver_QR::LinearSolver_QR(const LinearSolver_QR& obj)
		:   LinearSolver(),
            //
            R_(NULL),
            v_(NULL),
            colR_ (NULL)
{
	FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"LinearSolver_QR copy constructor not inplemented");
}


LinearSolver_QR::~LinearSolver_QR(void)
{
    if (getMatrixIsLocal_())
        delete R_;

    for (int j=0; j<getNbStorageCols_(); j++)
        delete []v_[j];
    delete []v_;

    delete []colR_;
}

const LinearSolver_QR& LinearSolver_QR::operator = (const LinearSolver_QR& obj)
{
	FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"LinearSolver_QR copy opertator not inplemented");
	return *this;
}


void LinearSolver_QR::setMatrix(Matrix* A, bool preserveMatrix)
{
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_QR");
    const int nbRows = A->getNbRows();
    const int nbCols = A->getNbCols();
    FAIL_CONDITION( nbRows < nbCols,
                    kInvalidParameterError,
                    "Underdetermined SLE assigned to a LinearSolver_QR");
	int newStorageRows = getNbStorageRows_(),
		newStorageCols = getNbStorageCols_();

    //  First, take care of v_
    if ((getNbRows_() != nbRows) || (getNbCols_() != nbCols))
    {
		if (nbCols > getNbStorageCols_())
		{
			for (int j=0; j<nbCols; j++)
				delete []v_[j];
			delete []v_;
			newStorageCols = nbCols;
			
			v_ = new double*[nbCols];
			for (int j=0, lv=max(nbRows, newStorageRows); j<nbCols; j++, lv--)
				v_[j] = new double[lv];                       
			
		}
		
        if (R_->getNbRows() > newStorageRows)
        {
            delete []colR_;
            colR_ = new double[nbRows];
			newStorageRows = nbRows;
        }
		setMatrixDimensions_(nbRows, nbCols);
		setMatrixStorageDimensions_(newStorageRows, newStorageCols);
    }


    //  If we were already storing a local copy of a previous SLE's matrix,
    //  then we may have to do delete/reallocate that local copy
    if (getMatrixIsLocal_())   
    {
        if (preserveMatrix)
        {
            //  If the new matrix has different dimensions from the old one, we 
            //  must delete & reallocate R_
            if ((R_->getNbRows() < nbRows) || (R_->getNbCols() < nbCols))
            {
                if (R_ != NULL)
                    delete R_;
                R_ = new Matrix(A);
            }
            //  if the dimensions did not change we simply copy the data
            else
                (*R_) = (*A);

        }
        //  If the matrix was local, but should not be anymore, we simply delete the
        //  current matrix and store the reference to the new one
        else
        {
            delete R_;
            R_ = A;
            setMatrixIsLocal_(false);
        }
    }
    else
    {
        if (preserveMatrix) 
        {
            R_ = new Matrix(A);
            setMatrixIsLocal_(true);
        }
        else
            R_ = A;
            
    }
        
    setMatrixIsLocal_(preserveMatrix);
    
    double* const*const r = R_->getMatrix(R_W_ACCESS);
    setSingularityStatus_(factorize_(nbRows, nbCols, r, colR_, v_));
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_QR::solve(Matrix *B)
{
    return solve(B, 0);
}

LinearSolverRecord* LinearSolver_QR::solve(Matrix *B, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_QR");
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

void LinearSolver_QR::solve(Matrix *B, LinearSolverRecord *record)
{
    solve(B, record, 0);
}

void LinearSolver_QR::solve(Matrix *B, LinearSolverRecord *record, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_QR");
    FAIL_CONDITION( record == NULL,
                    kNullParameterError,
                    "Null LinearSolverRecord term sent to a LinearSolver_QR");
    const int nbRowsB = B->getNbRows(),
              nbColsB = B->getNbCols();
    FAIL_CONDITION( nbRowsB != getNbRows_(),
                    kInvalidParameterError,
                    "Right-side term's dimensions don't match that of the matrix");

    //  If the SLE's matrix is singular we bail out
    FAIL_CONDITION( getSingularityStatus_() == SINGULAR_MATRIX,
                    kSingularMatrixError,
                    "The matrix of the SLE is singular");
        
    Matrix* X = record->sol;
    //  if the solution was not allocated we do it now
    if (X == NULL)
    {
        if (nbColsB == 1)
            X = record->sol = new ColumnVector(getNbCols_());
        else
            X = record->sol = new Matrix(getNbCols_(), nbColsB);

    }
    else
    {
        FAIL_CONDITION( getNbCols_() != X->getNbRows(),
                        kInvalidParameterError,
                        "Solution's dimensions don't match that of the SLE's matrix");
        FAIL_CONDITION( nbColsB != X->getNbCols(),
                        kInvalidParameterError,
                        "Solution's dimensions don't match that of the right-side term");
    }

    Matrix *newB;
    if (getMatrixIsLocal_())
        newB = new Matrix(B);
    else
        newB = B;
    double* const*const b = newB->getMatrix(R_W_ACCESS);

    record->status = getSingularityStatus_();
    
    applyQt_(getNbRows_(), getNbCols_(), nbColsB, v_, b);
    solveBS_(getNbCols_(), nbColsB, R_->getMatrix(), b, X->getMatrix(R_W_ACCESS));

    if (getMatrixIsLocal_())
        delete newB;
        
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_QR::solve(Matrix* A, Matrix* B, bool preserveMatrix) 
{
    return solve(A, B, 0, preserveMatrix);
}


LinearSolverRecord* LinearSolver_QR::solve(Matrix* A, Matrix* B, int incrImprov,
                                           bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_QR");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_QR");
    const int aRows = A->getNbRows();
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_QR");
    FAIL_CONDITION( preserveMatrix && (incrImprov>0),
                    kInvalidParameterError,
                    "Impossible to do incremental improvement w/o preserving the matrix in LinearSolver_QR:solve");
    LinearSolverRecord *record = new LinearSolverRecord();
    if (nbCols == 1)
        record->sol = new ColumnVector(nbRows);
    else
        record->sol = new Matrix(nbRows, nbCols);
     
       
    solve_(A, B, record, incrImprov, preserveMatrix);
    
    return record;
}

void LinearSolver_QR::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            bool preserveMatrix) 
{
    solve(A, B, record, 0, preserveMatrix);
}


void LinearSolver_QR::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            int incrImprov, bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_QR");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_QR");
    const int aRows = A->getNbRows(); /*,
              aCols = A->getNbCols();*/
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_QR");

    //  was the record properly created?
    if (record->sol != NULL)
    {
        FAIL_CONDITION( (nbRows != record->sol->getNbRows()) || 
                        (nbCols != record->sol->getNbCols()),
                        kInvalidParameterError,
                        "right-side term and solution's dimensions dont match for LinearSolver_QR");
    }
    //  if the solution was not allocated we do it now
    else
    {
        if (nbCols == 1)
            record->sol = new ColumnVector(nbRows);
        else
            record->sol = new Matrix(nbRows, nbCols);

    }
    
    solve_(A, B, record, incrImprov, preserveMatrix);
}

#if 0
#pragma mark -
#endif

void LinearSolver_QR::solve_(Matrix* A, Matrix* B, LinearSolverRecord *record,
                             int incrImprov, bool preserveMatrix) 
{
    const int nbRowsA = A->getNbRows(),
              nbColsA = A->getNbCols();
    const int nbRowsB = B->getNbRows(),
              nbColsB = B->getNbCols();

    Matrix *QR;
    Matrix *newB;
    if (preserveMatrix)
	{
        QR = new Matrix(A);
        newB = new Matrix(B);
    }
	else
    {
	    QR = A;
        newB = B;
    }
	    
    Matrix* X = record->sol;
	if (X == NULL)
    {
        if (nbColsB == 1)
            X = record->sol = new ColumnVector(nbColsA);
        else
            X = record->sol = new Matrix(nbColsA, nbColsB);

    }
    else
    {
        FAIL_CONDITION( nbRowsA != X->getNbRows(),
                        kInvalidParameterError,
                        "Solution's dimensions don't match that of the SLE's matrix");
        FAIL_CONDITION( nbColsB != X->getNbCols(),
                        kInvalidParameterError,
                        "Solution's dimensions don't match that of the right-side term");
    }

    double* const*const qr = QR->getMatrix(R_W_ACCESS);

    double** v = new double*[nbColsA];
    for (int j=0, lv=nbRowsA; j<nbColsA; j++, lv--)
        v[j] = new double[lv];                       
    double* colR = new double[nbRowsA];

    record->status = factorize_(nbRowsA, nbColsA, qr, colR, v);

    double* const*const b = newB->getMatrix(R_W_ACCESS);
    applyQt_(nbRowsB, nbColsB, nbColsB, v, b);
    solveBS_(nbColsA, nbColsB, qr, b, X->getMatrix(R_W_ACCESS));

    delete []colR;
    for (int j=0; j<nbColsA; j++)
        delete []v[j];
    delete []v;
    if (preserveMatrix)
	{
        delete QR;
		delete newB;
	}
}


void LinearSolver_QR::solveBS_(int nbRows, int nbCols, const double* const*const r, 
								const double* const* const b, double* const*const x)
{
    //  We want to solve R . X = B
    //  The outer loop deals with the columns of the right-side term
    for (int j=0; j<nbCols; j++)
    {
        if (r[nbRows-1][nbRows-1] != 0)
            x[nbRows-1][j] = b[nbRows-1][j] / r[nbRows-1][nbRows-1];
        else
            x[nbRows-1][j] = 0.L;
        
        for (int i=nbRows-2; i>=0; i--)
        {
            x[i][j] = b[i][j];
            for (int k=i+1; k<nbRows; k++)
                x[i][j] -= r[i][k] * x[k][j];
          
            if (r[i][i] != 0)
                x[i][j] /= r[i][i];

        }    
    }
}
 

void LinearSolver_QR::applyQt_(int nbRows, int nbColsR, int nbColsB, const double* const*const v, 
							   double* const*const b)
{
    //  We apply the Hv matrices (under the form of their v vector) that were computed 
	//	for the different columns (left to right) of the SLE's original matrix
    for (int j=0; j<nbColsR; j++)
    {
        //  Vector v is applied to all columns of the right-side term
        for (int l=0; l<nbColsB; l++)
        {
            double  bv = 0.L;
            //  compute the dot product of the (partial) column and v
            for (int i=j, k=0; i<nbRows; i++, k++)
                bv += b[i][l]*v[j][k];
                
            //  and subtract 2 v^T a v from a
            bv *= 2.L;
            for (int i=j, k=0; i<nbRows; i++, k++)
                b[i][l] -= bv * v[j][k];

        }
    }
}

#if 0
#pragma mark -
#endif

LinearSolverRecordStatus LinearSolver_QR::factorize_(int nbRows, int nbCols, double* const*const r,
                                                    double* const colR, double* const*const v)
{
    LinearSolverRecordStatus status = REGULAR_MATRIX, colStatus;
    
    //  We proceed column by column, from the left to the right
    for (int j=0, lv=nbRows; j<nbCols; j++, lv--)
    {
        //  Step 0:  Copy the data from column j of the matrix
        for (int i=j; i<nbRows; i++)
            colR[i] = r[i][j];
	
        //  Step 1:  Determine the v vector that constructs the orthogonal Householder Hv matrix
        //           Hv = I - 2 (v v^T) / (v^T v)
        ///          v is computed from elements from rows j to m-1 of column j
        double alphaV;
        colStatus = computeHouseholderV_(lv, colR+j, v[j], &alphaV); 

        if (colStatus == SINGULAR_MATRIX)
            status = SINGULAR_MATRIX;
        else if ((colStatus == ILL_CONDITIONED_MATRIX) && (status != SINGULAR_MATRIX))
            status = ILL_CONDITIONED_MATRIX;
            
        if (status != SINGULAR_MATRIX)
        {
            //  column j is "compressed"
            r[j][j] = alphaV;
            //  This is not really needed.  I do it so far for aesthetic reasons but as soon
            //  as the code has been thoroughly tested, I will remove this for better
            //  performance.
            for (int i=j+1; i<nbRows; i++)
                r[i][j] = 0;
                
            //  Vector v is applied to all other columns right of j.  This only afffects
            //  rows j and below
            for (int l=j+1; l<nbCols; l++)
            {
                double  av = 0;
                //  compute the dot product of the (partial) column and v
                for (int i=j, k=0; i<nbRows; i++, k++)
                    av += r[i][l]*v[j][k];
                    
                //  and subtract 2 v^T a v from a
                av *= 2.L;
                for (int i=j, k=0; i<nbRows; i++, k++)
                    r[i][l] -= av * v[j][k];

            }
        }
    }
    
    return status;
}


LinearSolverRecordStatus LinearSolver_QR::computeHouseholderV_(int n, double* const a, double* const v,
                                                              double* alphaV)
{
    LinearSolverRecordStatus status = REGULAR_MATRIX;
    double  invNormV, normA, normA1 = 0.L;
    
    
    for (int i=1; i<n; i++)
    {
        v[i] = a[i];
        normA1 += a[i]*a[i];
    }
    normA = sqrt(normA1 + a[0]*a[0]);
    
    if (normA == 0.L)
        status = SINGULAR_MATRIX;
    else 
    {
        if (normA < ILL_COND_THRESHOLD)
			status = ILL_CONDITIONED_MATRIX;    
        
        if (a[0] > 0.L)
        {
            v[0] = a[0] + normA;
            *alphaV = -normA;
            invNormV = 1.L / sqrt(normA1 + v[0]*v[0]);
        }
        else
        {
            v[0] = a[0] - normA;
            *alphaV = normA;
            invNormV = 1.L / sqrt(normA1 + v[0]*v[0]);
        }

        //  we normalize v to speedup later calculations
        for (int i=0; i<n; i++)
            v[i] *= invNormV;

    }
    
    return status;
}
