/*  NAME:
        LinearSolver_LU.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib LinearSolver_LU class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColumnVector.h"
#include "LinearSolver_LU.h"

using namespace uriVL;

//------------------------------
//  Redefines static members
//------------------------------
const double LinearSolver_LU::ILL_COND_PIVOT_THRESHOLD = 1.E-7;


LinearSolver_LU::LinearSolver_LU(int nbRows, int nbCols)

    try :   LinearSolver(nbRows, nbCols),
            //
            LU_(new Matrix(nbRows, nbCols)),
            p_(new int[nbRows]),
            v_(new double[nbRows])
{
    if (nbRows != nbCols)
    {
        delete LU_;
        delete []v_;
        delete []p_;
        
        FAIL_CONDITION( true,
                        kInvalidParameterError,
                        "Nonsquare SLE assigned to a LinearSolver_LU");

    }
	setMatrixIsLocal_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by LinearSolver_LU constructor");
	throw e;
}


LinearSolver_LU::LinearSolver_LU(Matrix* A, bool preserveMatrix)
    try	:   LinearSolver(A, preserveMatrix),
			//
			LU_(preserveMatrix ? new Matrix(A) : A),
			p_(new int[A->getNbRows()]),
			v_(new double[A->getNbRows()])
{
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_LU");

	if (getNbRows_() != getNbCols_())
	{
		delete []v_;
		delete []p_;
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Nonsquare matrix assigned to a LinearSolver_LU");
	}
	
    double* const*const lu = LU_->getMatrix(R_W_ACCESS);
    setSingularityStatus_(factorize_(getNbRows_(), lu, p_, v_));
}
catch (ErrorReport& e) {
	e.appendToMessage("called by LinearSolver_LU constructor");
	throw e;
}

LinearSolver_LU::LinearSolver_LU(const LinearSolver_LU& obj)
		:   LinearSolver(),
            //
            LU_(NULL),
            p_(NULL),
            v_(NULL)
{
	FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"LinearSolver_LU copy constructor not inplemented");
}



LinearSolver_LU::~LinearSolver_LU(void)
{
    if (getMatrixIsLocal_())
        delete LU_;
        
    delete[] p_;
    delete[] v_;
}




const LinearSolver_LU& LinearSolver_LU::operator = (const LinearSolver_LU& obj)
{
	FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"LinearSolver_LU copy opertator not inplemented");
	return *this;
}



void LinearSolver_LU::setMatrix(Matrix* A, bool preserveMatrix)
{
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_LU");
    const int nbRows = A->getNbRows(),
              nbCols = A->getNbCols();
    FAIL_CONDITION( nbRows != nbCols,
                    kInvalidParameterError,
                    "Nonsquare matrix assigned to a LinearSolver_LU");

    //  First, take care of p_ and v_
    if (LU_->getNbRows() != nbRows)
    {
        delete []v_;
        delete []p_;
        v_ = new double[nbRows];                        
        p_ = new int[nbRows];
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
            if ((LU_->getNbRows() != nbRows) || (LU_->getNbCols() != nbCols))
            {
                if (LU_ != NULL)
                    delete LU_;
                LU_ = new Matrix(A);
                
            }
            //  if the dimensions did not change we simply copy the data
            else
                (*LU_) = (*A);

            setMatrixStorageDimensions_(nbRows, nbRows);                     
        }
        //  If the matrix was local, but should not be anymore, we simply delete the
        //  current matrix and store the reference to the new one
        else
        {
            delete LU_;
            LU_ = A;
        }
    }
    else
    {
        if (preserveMatrix)
            LU_ = new Matrix(A);
        else
            LU_  =A;
            
    }
        
    setMatrixIsLocal_(preserveMatrix);
    
    double* const*const lu = LU_->getMatrix(R_W_ACCESS);
    setSingularityStatus_(factorize_(nbRows, lu, p_, v_));
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_LU::solve(Matrix *B)
{
    return solve(B, 0);
}

LinearSolverRecord* LinearSolver_LU::solve(Matrix *B, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_LU");
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

void LinearSolver_LU::solve(Matrix *B, LinearSolverRecord *record)
{
    solve(B, record, 0);
}

void LinearSolver_LU::solve(Matrix *B, LinearSolverRecord *record, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_LU");
    FAIL_CONDITION( record == NULL,
                    kNullParameterError,
                    "Null LinearSolverRecord term sent to a LinearSolver_LU");
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
    
    double* const*const lu = LU_->getMatrix(R_W_ACCESS);
    double* const*const b = B->getMatrix(R_W_ACCESS);
    double* const*const x = X->getMatrix(R_W_ACCESS);
    forwardSubstitution_(nbRows, nbCols, lu, b, p_, x);
    backSubstitution_(nbRows, nbCols, lu, x, p_, x);

/*
    //  Incremental improvement (using residual as right-side term for the
    //  corrective term)
    for (int k=0; k< incrImprov; k++)
    {
        Matrix R = 
    }
*/
    record->status = getSingularityStatus_();
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_LU::solve(Matrix* A, Matrix* B, bool preserveMatrix) 
{
    return solve(A, B, 0, preserveMatrix);
}


LinearSolverRecord* LinearSolver_LU::solve(Matrix* A, Matrix* B, int incrImprov,
                                           bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_LU");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_LU");
    const int aRows = A->getNbRows(),
              aCols = A->getNbCols();
    FAIL_CONDITION( aRows != aCols,
                    kInvalidParameterError,
                    "Nonsquare matrix assigned to a LinearSolver_LU");
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_LU");

    LinearSolverRecord *record = new LinearSolverRecord();
    if (nbCols == 1)
        record->sol = new ColumnVector(nbRows);
    else
        record->sol = new Matrix(nbRows, nbCols);
     
       
    solve_(A, B, record, incrImprov, preserveMatrix);
    
    return record;
}

void LinearSolver_LU::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            bool preserveMatrix) 
{
    solve(A, B, record, 0, preserveMatrix);
}


void LinearSolver_LU::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            int incrImprov, bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_LU");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_LU");
    const int aRows = A->getNbRows(),
              aCols = A->getNbCols();
    FAIL_CONDITION( aRows != aCols,
                    kInvalidParameterError,
                    "Nonsquare matrix assigned to a LinearSolver_LU");
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_LU");

    //  was the record properly created?
    if (record->sol != NULL)
    {
        FAIL_CONDITION( (nbRows != record->sol->getNbRows()) || 
                        (nbCols != record->sol->getNbCols()),
                        kInvalidParameterError,
                        "right-side term and solution's dimensions dont match for LinearSolver_LU");
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

void LinearSolver_LU::solve_(Matrix* A, Matrix* B, LinearSolverRecord *record,
                             int incrImprov, bool preserveMatrix) 
{
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    Matrix *X = record->sol;
    int* p = new int[nbRows];
    double* v = new double[nbRows];
    
    Matrix *LU;
    if (preserveMatrix)
        LU = new Matrix(A);
    else
        LU = A;
        
    double* const*const lu = LU->getMatrix(R_W_ACCESS);
    double* const*const b = B->getMatrix(R_W_ACCESS);
    double* const*const x = X->getMatrix(R_W_ACCESS);

    record->status = factorize_(nbRows, lu, p, v);

    forwardSubstitution_(nbRows, nbCols, lu, b, p, x);
    backSubstitution_(nbRows, nbCols, lu, x, p, x);

    delete []p;
    delete []v;
    if (preserveMatrix)
        delete LU;
        
}


#if 0
#pragma mark -
#endif

LinearSolverRecordStatus LinearSolver_LU::factorize_(int n, double* const* const a, 
                                                    int* p, double* v)
{
    //  a, l, u, lu are all aliases of the same array
    double* const*const l = a;
    double* const*const lu = a;
    double* const*const u = a;
    LinearSolverRecordStatus status = REGULAR_MATRIX;
    
    for (int i=0; i<n; i++)
        p[i] = i;

    //-----------------------------------------------------------
    //  Step 1: Determine largest element of each row
    //-----------------------------------------------------------
    for (int i=0; i<n; i++)
    {
        double val, valMax = a[i][0];
        for (int j=1; j<n; j++)
            if ((val = fabs(a[i][j])) > valMax)
                valMax = val;
        v[i] = valMax;
    }
    
    //-----------------------------------------------------------
    //  Step 2: Apply Crout's algorithm with partial pivoting
    //-----------------------------------------------------------
    for (int j=0; j<n; j++)
    {
        //-------------------------------------------
        //  process superdiagonal elements
        //-------------------------------------------
        for (int i=0; i<j; i++)
            //  u[p[i]][j] stores a[p[i]][j]
            for (int k=0; k<i; k++)
                u[p[i]][j] -= l[p[i]][k] * u[p[k]][j];

        //-------------------------------------------
        //  process diagonal and subdiagonal elements
        //-------------------------------------------
        for (int i=j; i<n; i++)
            //  lu[p[i]][j] stores a[p[i]][j]
            for (int k=0; k<j; k++)
                lu[p[i]][j] -= l[p[i]][k] * u[p[k]][j];

        //  The last column has no subdiagonal elements
        if (j < n-1)
        {
            //-------------------------------------------
            //  find index of best pivot, rows j to n-1
            //-------------------------------------------
            double maxRatio = v[p[j]] != 0 ? fabs(lu[p[j]][j]) / v[p[j]] : 0.L;
            double ratio;
            int    iBest = j;
 
            for (int i=j+1; i<n; i++)
            {
                if ((ratio = fabs(lu[p[i]][j]) / v[p[i]]) > maxRatio)
                {
                    maxRatio = ratio;
                    iBest = i;
                }
            }
            //  if j is not the best pivot, swap rows
            if (iBest != j)
            {
                int temp = p[j];
                p[j] = p[iBest];
                p[iBest] = temp;
            }

            
            //-------------------------------------------
            //  Compute the scale and apply is to all
            //  subdiagonal elements
            //-------------------------------------------
            double s;
            if (fabs(u[p[j]][j]) > ILL_COND_PIVOT_THRESHOLD)
                s = 1.L / u[p[j]][j];
            else if (u[p[j]][j] == 0.L)
            {
                status = SINGULAR_MATRIX;
                s = 0.L;
            }
            else
            {
                if (status != SINGULAR_MATRIX)
                    status = ILL_CONDITIONED_MATRIX;
                
                s = 1.L / u[p[j]][j];
            }
            for (int i=j+1; i<n; i++)
                l[p[i]][j] *= s;
            
        }
    }    

    return status;
}


void LinearSolver_LU::forwardSubstitution_(int m, int n, double* const*const l, 
                                           double* const*const b,
                                           int* p, double* const*const y)
{
    //  We want to solve P(L) . Y = B
    //  The row permutations applied to L do not affect Y, but must be applied to B
    //  as well to preserve the equation

    //  The outer loop deals with the columns of the right-side term
    for (int j=0; j<n; j++)
    {
        int ip = p[0];
        y[0][j] = b[ip][j];
        
        for (int i=1; i<m; i++)
        {
            ip = p[i];
            y[i][j] = b[ip][j];
            for (int k=0; k<i; k++)
                y[i][j] -= l[ip][k] * y[k][j];
                
        }    
    }
}

void LinearSolver_LU::backSubstitution_(int m, int n, double* const*const u, double* const*const y,
                                        int* p, double* const*const x)
{
    //  We want to solve P(U). X = Y
    //  The row permutations applied to  do not affect X, but must be applied to Y
    //  as well to preserve the equation

    //  The outer loop deals with the columns of the right-side term
    for (int j=0; j<n; j++)
    {
        int ip = p[m-1];
        if (u[ip][m-1] != 0)
            x[m-1][j] = y[m-1][j] / u[ip][m-1];
        else
            x[m-1][j] = 0.L;
        
        for (int i=m-2; i>=0; i--)
        {
            ip = p[i];
            x[i][j] = y[i][j];
            for (int k=i+1; k<m; k++)
                x[i][j] -= u[ip][k] * x[k][j];
          
            if (u[ip][i] != 0)
                x[i][j] /= u[ip][i];

        }    
    }
}
