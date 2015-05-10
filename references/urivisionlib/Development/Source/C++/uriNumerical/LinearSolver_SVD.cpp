/*  NAME:
        LinearSolver_SVD.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib LinearSolver_SVD class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <algorithm>
//
#include "ColumnVector.h"
#include "LinearSolver_SVD.h"

using namespace std;
using namespace uriVL;

const double EPS = pow(2.L,-52.L);


//--------------------------------------------------------------------------
//	Constructors and destructor
//--------------------------------------------------------------------------

LinearSolver_SVD::LinearSolver_SVD(int nbRows, int nbCols)
  try	:   LinearSolver(nbRows, nbCols),
			//
			bigDim_(max(nbRows, nbCols)),
			smallDim_(min(nbRows, nbCols)),
			A_(new Matrix(nbRows, nbCols)),
			U_(new Matrix(nbRows, nbCols)),
			V_(new Matrix(nbCols, nbCols)),
			S_(new RowVector(nbCols))
{
	//	so far we cannot handle underdetermined SLEs
	if (nbRows < nbCols)
	{
		delete S_;
		delete V_;
		delete U_;
		if (getMatrixIsLocal_())
			delete A_;
			
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"So far the LinearSolver_SVD class cannot handle underdetermined SLEs");
	}
	
	if ((nbRows < 2) || (nbCols < 2))
	{
		delete S_;
		delete V_;
		delete U_;
		if (getMatrixIsLocal_())
			delete A_;
			
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"nummber of rows < 2 passed to LinearSolver_SVD constructor");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"nummber of columns < 2 passed to LinearSolver_SVD constructor");
	}
}
catch (ErrorReport& e) {
	e.appendToMessage("called by LinearSolver_SVD constructor");
	throw e;
}


LinearSolver_SVD::LinearSolver_SVD(Matrix* A, bool preserveMatrix)
  try	:   LinearSolver(A, preserveMatrix),
			//
			bigDim_(max(A->getNbRows(), A->getNbCols())),
			smallDim_(min(A->getNbRows(), A->getNbCols())),
			A_(preserveMatrix ? new Matrix(A) : A),
			U_(new Matrix(A->getNbRows(), A->getNbCols())),
			V_(new Matrix(A->getNbCols(), A->getNbCols())),
			S_(new RowVector(A->getNbCols()))
{
	const int nbRows = A->getNbRows();
	const int nbCols = A->getNbCols();
    
	//	so far we cannot handle underdetermined SLEs
	if (nbRows < nbCols)
	{
		delete S_;
		delete V_;
		delete U_;
		if (getMatrixIsLocal_())
			delete A_;
			
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"So far the LinearSolver_SVD class cannot handle underdetermined SLEs");
	}
	
	if ((nbRows < 2) || (nbCols < 2))
	{
		delete S_;
		delete V_;
		delete U_;
		if (getMatrixIsLocal_())
			delete A_;
			
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"nummber of rows < 2 passed to LinearSolver_SVD constructor");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"nummber of columns < 2 passed to LinearSolver_SVD constructor");
	}
	
    setSingularityStatus_(factorize_(A_, U_, S_, V_));
}
catch (ErrorReport& e) {
	e.appendToMessage("called by LinearSolver_SVD constructor");
	throw e;
}

LinearSolver_SVD::LinearSolver_SVD(const LinearSolver_SVD& obj)
		:   LinearSolver(),
            //
			bigDim_(-1),
			smallDim_(-1),
			A_(NULL),
			U_(NULL),
			V_(NULL),
			S_(NULL)
{
	FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"LinearSolver_LU copy constructor not inplemented");
}




LinearSolver_SVD::~LinearSolver_SVD(void)
{
    if (U_ != NULL)
        delete U_;
    if (V_ != NULL)
        delete V_; 
    if (S_ != NULL)
        delete S_;
    if (getMatrixIsLocal_())
        delete A_;
    
}


const LinearSolver_SVD& LinearSolver_SVD::operator = (const LinearSolver_SVD& obj)
{
	FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"LinearSolver_SVD copy opertator not inplemented");
	return *this;
}


void LinearSolver_SVD::setMatrix(Matrix* A, bool preserveMatrix)
{
    if (U_ != NULL)
        delete U_;
    if (V_ != NULL)
        delete V_; 
    if (S_ != NULL)
        delete S_;
    if (getMatrixIsLocal_())
        delete A_;

	const int nbRows = A->getNbRows();
	const int nbCols = A->getNbCols();
    
	//	so far we cannot handle underdetermined SLEs
	if (nbRows < nbCols)
	{
		delete S_;
		delete V_;
		delete U_;
		if (getMatrixIsLocal_())
			delete A_;
			
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"So far the LinearSolver_SVD class cannot handle underdetermined SLEs");
	}
	
	if ((nbRows < 2) || (nbCols < 2))
	{
		delete S_;
		delete V_;
		delete U_;
		if (getMatrixIsLocal_())
			delete A_;
			
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"nummber of rows < 2 passed to LinearSolver_SVD constructor");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"nummber of columns < 2 passed to LinearSolver_SVD constructor");
	}
	
	bigDim_ = max(nbRows, nbCols);
	smallDim_ = min(nbRows, nbCols);
	A_ = preserveMatrix ? new Matrix(A) : A;
	U_ = new Matrix(nbRows, nbCols);
	V_ = new Matrix(nbCols, nbCols);
	S_ = new RowVector(nbCols);
	
    setSingularityStatus_(factorize_(A_, U_, S_, V_));
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_SVD::solve(Matrix *B)
{
    return solve(B, 0);
}

LinearSolverRecord* LinearSolver_SVD::solve(Matrix *B, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_SVD");
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

void LinearSolver_SVD::solve(Matrix *B, LinearSolverRecord *record)
{
    solve(B, record, 0);
}

void LinearSolver_SVD::solve(Matrix *B, LinearSolverRecord *record, int incrImprov)
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term sent to a LinearSolver_SVD");
    FAIL_CONDITION( record == NULL,
                    kNullParameterError,
                    "Null LinearSolverRecord term sent to a LinearSolver_SVD");
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

    record->status = getSingularityStatus_();
	
	solve_(U_, S_, V_, B, record);
}

#if 0
#pragma mark -
#endif

LinearSolverRecord* LinearSolver_SVD::solve(Matrix* A, Matrix* B, bool preserveMatrix) 
{
    return solve(A, B, 0, preserveMatrix);
}


LinearSolverRecord* LinearSolver_SVD::solve(Matrix* A, Matrix* B, int incrImprov,
                                           bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_SVD");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_SVD");
    const int aRows = A->getNbRows(),
              aCols = A->getNbCols();
    FAIL_CONDITION( aRows != aCols,
                    kInvalidParameterError,
                    "Nonsquare matrix assigned to a LinearSolver_SVD");
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_SVD");

    LinearSolverRecord *record = new LinearSolverRecord();
    if (nbCols == 1)
        record->sol = new ColumnVector(nbRows);
    else
        record->sol = new Matrix(nbRows, nbCols);
     
       
    solve(A, B, record, incrImprov, preserveMatrix);
    
    return record;
}

void LinearSolver_SVD::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            bool preserveMatrix) 
{
    solve(A, B, record, 0, preserveMatrix);
}


void LinearSolver_SVD::solve(Matrix* A, Matrix* B, LinearSolverRecord *record,
                            int incrImprov, bool preserveMatrix) 
{
    FAIL_CONDITION( B == NULL,
                    kNullParameterError,
                    "Null right-side term assigned to a LinearSolver_SVD");
    const int nbRows = B->getNbRows(),
              nbCols = B->getNbCols();
    FAIL_CONDITION( A == NULL,
                    kNullParameterError,
                    "Null matrix assigned to a LinearSolver_SVD");
    const int aRows = A->getNbRows(),
              aCols = A->getNbCols();
    FAIL_CONDITION( aRows != nbRows,
                    kInvalidParameterError,
                    "Matrix and right-side term dimensions dont match for LinearSolver_SVD");

    //  was the record properly created?
    if (record->sol != NULL)
    {
        FAIL_CONDITION( (nbRows != record->sol->getNbRows()) || 
                        (nbCols != record->sol->getNbCols()),
                        kInvalidParameterError,
                        "right-side term and solution's dimensions dont match for LinearSolver_SVD");
    }
    //  if the solution was not allocated we do it now
    else
    {
        if (nbCols == 1)
            record->sol = new ColumnVector(nbRows);
        else
            record->sol = new Matrix(nbRows, nbCols);

    }
    
	Matrix* myA = preserveMatrix ? new Matrix(A) : A;
	Matrix* U = new Matrix(nbRows, aCols);
	Matrix* V = new Matrix(aCols, aCols);
	RowVector* S = new RowVector(aCols);
	
	record->status = factorize_(myA, U, S, V);

	if (preserveMatrix)
		delete myA;

    solve_(U, S, V, B, record);
	
	delete U;
	delete V;
	delete S;
}

void LinearSolver_SVD::solve_(Matrix* U, RowVector* S, Matrix* V, Matrix* B, LinearSolverRecord *record) 
{
	const int nbRows = U->getNbRows();
	const int aCols = U->getNbCols();
	const int bCols = B->getNbCols();
	//
	double const*const* u = U->getMatrix();
	double const*const* v = V->getMatrix();
	double const*const* b = B->getMatrix();
	double const*s = (S->getMatrix())[0];
	
/*	cout << "U matrix:\n=========\n";
	for (int i=0; i<nbRows; i++)
	{
		for (int j=0; j<aCols; j++)
		{
			cout << u[i][j] << "    ";
		}
		cout << endl;
	}
	cout << endl;
	
	cout << "S diag:\n=========\n";
	for (int i=0; i<aCols; i++)
		cout << s[i] << "    ";
	cout << endl << endl;

	cout << "V matrix:\n=========\n";
	for (int i=0; i<aCols; i++)
	{
		for (int j=0; j<aCols; j++)
		{
			cout << v[i][j] << "    ";
		}
		cout << endl;
	}
	cout << endl;
*/

	//                 t
	//	compute UtB = U . B
	double** utb = new double*[aCols];
	for (int i=0; i<aCols; i++)
	{
		utb[i] = new double[bCols];
		//
		for (int j=0; j<bCols; j++)
		{
			utb[i][j] = 0.L;
			//
			for (int k=0; k<aCols; k++)
			{
				utb[i][j] += u[k][i] * b[k][j];
			}
		}
	}
	
	//                  -1    t
	// compute S1UtB = S   . U . B
	double** s1utb = utb;
	for (int i=0; i<aCols; i++)
	{
		if (s[i] != 0.L)
		{
			double invS = 1.L / s[i];
			for (int j=0; j<bCols; j++)
			{
				s1utb[i][j] = invS * utb[i][j];
			}
		}
		else
		{
			for (int j=0; j<bCols; j++)
			{
				s1utb[i][j] = 0;
			}
		}
	}
	
	//                           -1    t
	// Finally, compute x = V . S   . U . B
	double* const* x = record->sol->getMatrix(R_W_ACCESS);
	for (int i=0; i<nbRows; i++)
	{
		for (int j=0; j<bCols; j++)
		{
			x[i][j] = 0.L;
			
			for (int k=0; k<aCols; k++)
			{
				x[i][j] += v[i][k]*s1utb[k][j];
			}
		}
	}

	for (int i=0; i<aCols; i++)
		delete []utb[i];
	delete []utb;	
}


#if 0
#pragma mark -
#endif

//============================================================================================
//  The factorization code is at the very least largely inspired and in most places simply
//  copied/adapted from the public domain Jama implementation of SVD
//      http://math.nist.gov/javanumerics/jama/
//============================================================================================
 
LinearSolverRecordStatus LinearSolver_SVD::factorize_(Matrix* A, Matrix* U, RowVector* S, 
														Matrix* V)
{
    LinearSolverRecordStatus status = REGULAR_MATRIX;
                
    double  *const*const    a = A->getMatrix(R_W_ACCESS);
    double  *const*const    u = U->getMatrix(R_W_ACCESS);
    double  *const*const    v = V->getMatrix(R_W_ACCESS);
    double  *s = (S->getMatrix(R_W_ACCESS))[0];
    
	const int nbRows = A->getNbRows();
	const int nbCols = A->getNbCols();
	const int smallDim = min(nbRows, nbCols);
	
//	cout << "A matrix [initial]:\n=================\n";
//	for (int i=0; i<nbRows; i++)
//	{
//		for (int j=0; j<nbCols; j++)
//		{
//			cout << a[i][j] << "    ";
//		}
//		cout << endl;
//	}
//	cout << endl;


	for (int i=0; i<nbRows; i++)
	{
		for (int j=0; j<nbCols; j++)
		{
			u[i][j] = 0.L;
		}
	}
	for (int i=0; i<nbCols; i++)
	{
		for (int j=0; j<nbCols; j++)
		{
			v[i][j] = 0.L;
		}
	}
	
    //  Define and allocate a few local arrays
    double  *e = new double[nbCols];
    double  *work = new double[nbRows];

    //---------------------------------------------------------------------------
    //  Step 1: Reduce A to bidiagonal form, storing the diagonal elements in s 
    //          and the super-diagonal elements in e
    //---------------------------------------------------------------------------
    int nct = min(nbRows-1, nbCols);
    int nrt = max(0,min(nbCols-2, nbRows));

    int nmaxt = max(nct, nrt);
    
    for (int k = 0; k < nmaxt; k++) 
	{
        if (k < nct) 
        {
            // Compute the transformation for the k-th column and
            // place the k-th diagonal in s[k].
            // Compute 2-norm of k-th column without under/overflow.
            s[k] = 0;
            for (int i = k; i < nbRows; i++) 
            {
                s[k] = hypot(s[k], a[i][k]);
            }
            if (s[k] != 0.L) 
            {
                if (a[k][k] < 0.L) 
                {
                    s[k] = -s[k];
                }

                for (int i = k; i < nbRows; i++) 
                {
                    a[i][k] /= s[k];
                }

                a[k][k] += 1.0L;

                s[k] = -s[k];
            }

        }

		for (int j = k+1; j < nbCols; j++) 
		{
			if ((k < nct) && (s[k] != 0.L))  
			{
				// Apply the transformation.

				double t = 0.L;
				for (int i = k; i < nbRows; i++) 
					t += a[i][k]*a[i][j];

				t = -t / a[k][k];

				for (int i = k; i < nbRows; i++) 
					a[i][j] += t*a[i][k];

			}

			// Place the k-th row of A into e for the
			// subsequent calculation of the row transformation.
			e[j] = a[k][j];
		}

		if (k < nct)
		{
			// Place the transformation in U for subsequent back
			// multiplication.

			for (int i = k; i < nbRows; i++)
				u[i][k] = a[i][k];

		}

		if (k < nrt) 
		{
			// Compute the k-th row transformation and place the
			// k-th super-diagonal in e[k].
			// Compute 2-norm without under/overflow.
			e[k] = 0;
			for (int i = k+1; i < nbCols; i++)
				e[k] = hypot(e[k],e[i]);

			if (e[k] != 0.L)
			{
				if (e[k+1] < 0.L)
					e[k] = -e[k];
			
				for (int i = k+1; i < nbCols; i++) 
					e[i] /= e[k];

				e[k+1] += 1.L;
			}
			
			e[k] = -e[k];
			if ((k+1 < nbRows) && (e[k] != 0.L)) 
			{
				// Apply the transformation.

				for (int i = k+1; i < nbRows; i++)
					work[i] = 0.0;

				for (int j = k+1; j < nbRows; j++)
					for (int i = k+1; i < nbRows; i++)
						work[i] += e[j]*a[i][j];

				for (int j = k+1; j < nbCols; j++)
				{
					double t = (-e[j]/e[k+1]);
				
					for (int i = k+1; i < nbRows; i++)
						a[i][j] += t*work[i];

				}
			}
				
			// Place the transformation in V for subsequent
			// back multiplication.
			for (int i = k+1; i < nbCols; i++) 
				v[i][k] = e[i];

		}
    }
//	cout << "A matrix [after 1]:\n=================\n";
//	for (int i=0; i<nbRows; i++)
//	{
//		for (int j=0; j<nbCols; j++)
//		{
//			cout << a[i][j] << "    ";
//		}
//		cout << endl;
//	}
//	cout << endl;
//

    //---------------------------------------------------------------------------
    //  Step 2: Set up the final bidiagonal matrix or order p.
    //---------------------------------------------------------------------------
	int p = min(nbCols, nbRows + 1);
	if (nct < nbCols)
		s[nct] = a[nct][nct];
	
	if (nbRows < p)
		s[p-1] = 0.0;
	
	if (nrt+1 < p)
		e[nrt] = a[nrt][p-1];
	
	e[p-1] = 0.0;
	

//	cout << "U matrix [before step 3]:\n====================\n";
//	for (int i=0; i<nbRows; i++)
//	{
//		for (int j=0; j<nbCols; j++)
//		{
//			cout << u[i][j] << "    ";
//		}
//		cout << endl;
//	}
//	cout << endl;
    //---------------------------------------------------------------------------
    //  Step 3: Generate U.
    //---------------------------------------------------------------------------
	for (int j = nct; j < smallDim; j++) 
	{
		for (int i = 0; i < nbRows; i++) 
			u[i][j] = 0.0;
		
		u[j][j] = 1.0;
	}
	
	for (int k = nct-1; k >= 0; k--) 
	{
		if (s[k] != 0.0) 
		{
			for (int j = k+1; j < smallDim; j++) 
			{
				double t = 0.L;
				for (int i = k; i < nbRows; i++)
					t += u[i][k]*u[i][j];
				
				t = -t / u[k][k];
				for (int i = k; i < nbRows; i++)
					u[i][j] += t * u[i][k];
			}
			
			for (int i = k; i < nbRows; i++ ) 
				u[i][k] = -u[i][k];
			
			u[k][k]++;
			
			for (int i = 0; i < k-1; i++)
				u[i][k] = 0.L;
			
		} 
		else 
		{
			for (int i = 0; i < nbRows; i++)
				u[i][k] = 0.L;
			
			u[k][k] = 1.L;
		}
	}
	
//	cout << "U matrix [after step 3]:\n====================\n";
//	for (int i=0; i<nbRows; i++)
//	{
//		for (int j=0; j<nbCols; j++)
//		{
//			cout << u[i][j] << "    ";
//		}
//		cout << endl;
//	}
//	cout << endl;
//	

    //---------------------------------------------------------------------------
    //  Step 4: Generate V.
    //---------------------------------------------------------------------------
	for (int k = nbCols - 1; k >= 0; k--) 
	{
		if ((k < nrt) && (e[k] != 0.L)) 
		{
			for (int j = k+1; j < smallDim; j++) 
			{
				double t = 0.L;
				for (int i = k+1; i < nbCols; i++)
					t += v[i][k] * v[i][j];
				
				t = -t / v[k+1][k];
				
				for (int i = k+1; i < nbCols; i++)
					v[i][j] += t * v[i][k];
				
			}
		}
		for (int i = 0; i < nbCols; i++)
			v[i][k] = 0.L;
		
		v[k][k] = 1.L;
	}

//	cout << "V matrix [after step 4]:\n=======================\n";
//	for (int i=0; i<nbCols; i++)
//	{
//		for (int j=0; j<nbCols; j++)
//		{
//			cout << v[i][j] << "    ";
//		}
//		cout << endl;
//	}
//	cout << endl;
//

    //---------------------------------------------------------------------------
	// Step 5: Main iteration loop for the singular values.
    //---------------------------------------------------------------------------
	int pp = p - 1;
	int iter = 0;
	while (p > 0) 
	{
		// Here is where a test for too many iterations would go.
		
		// This section of the program inspects for
		// negligible elements in the s and e arrays.  On
		// completion the variables kase and k are set as follows.
		
		// kase = 1     if s(p) and e[k-1] are negligible and k<p
		// kase = 2     if s(k) is negligible and k<p
		// kase = 3     if e[k-1] is negligible, k<p, and
		//              s(k), ..., s(p) are not negligible (qr step).
		// kase = 4     if e(p-1) is negligible (convergence).
		int k;
		int kase=0;
		for (k = p-2; k >= 0; k--) 
		{
			if (fabs(e[k]) <= EPS*(fabs(s[k]) + fabs(s[k+1]))) 
			{
				e[k] = 0.L;
				break;
			}
		}
		
		if (k == p-2)
			kase = 4;
		else
		{
			int ks;
			for (ks = p-1; ks > k; ks--) 
			{
				double t =	(ks != p ? fabs(e[ks]) : 0.L) + 
							(ks != k+1 ? fabs(e[ks-1]) : 0.L);
				
				if (fabs(s[ks]) <= EPS*t)  
				{
					s[ks] = 0.0;
					break;
				}
			}
			if (ks == k)
				kase = 3;
			else if (ks == p-1)
				kase = 1;
			else
			{
				kase = 2;
				k = ks;
			}
		}

		k++;
		
		// Perform the task indicated by kase.
		switch (kase) 
		{
            // Deflate negligible s(p).
			case 1: 
			{
				double f = e[p-2];
				e[p-2] = 0.L;

				for (int j = p-2; j >= k; j--) 
				{
					const double t = hypot(s[j], f);
					const double cs = s[j] / t;
					const double sn = f / t;
					
					s[j] = t;
					if (j != k) 
					{
						f = -sn * e[j-1];
						e[j-1] = cs * e[j-1];
					}
					
					for (int i = 0; i < nbCols; i++) 
					{
						const double th = cs * v[i][j] + sn * v[i][p-1];
						v[i][p-1] = -sn * v[i][j] + cs * v[i][p-1];
						v[i][j] = th;
					}
				}
			}
			break;
				
			// Split at negligible s(k).
			case 2: 
			{
				double f = e[k-1];
				e[k-1] = 0.L;
				
				for (int j = k; j < p; j++) 
				{
					const double t = hypot(s[j], f);
					const double cs = s[j] / t;
					const double sn = f / t;

					s[j] = t;
					f = -sn * e[j];
					e[j] = cs * e[j];

					for (int i = 0; i < nbRows; i++) 
					{
						const double th = cs * u[i][j] + sn * u[i][k-1];
						u[i][k-1] = -sn * u[i][j] + cs * u[i][k-1];
						u[i][j] = th;
					}
				}
			}
			break;
				
            // Perform one qr step.
			case 3: 
			{
				// Calculate the shift.
				const double scale = 1.L / max(max(max(max(fabs(s[p-1]), fabs(s[p-2])), 
													   fabs(e[p-2])), fabs(s[k])), fabs(e[k]));
				const double sp = s[p-1] * scale;
				const double spm1 = s[p-2] * scale;
				const double epm1 = e[p-2] * scale;
				const double sk = s[k] * scale;
				const double ek = e[k] * scale;
				const double b = ((spm1 + sp)*(spm1 - sp) + epm1*epm1) * 0.5L;
				const double c = (sp*epm1)*(sp*epm1);
				double shift = 0.0;

				if ((b != 0.L) || (c != 0.L)) 
				{
					shift = sqrt(b*b + c);
					if (b < 0.L) {
						shift = -shift;
					}
					shift = c/(b + shift);
				}

				double f = (sk + sp)*(sk - sp) + shift;
				double g = sk*ek;
				
				// Chase zeros.
				for (int j = k; j < p-1; j++) 
                {
					double t = hypot(f, g);
					double cs = f/t;
					double sn = g/t;
					if (j != k) 
						e[j-1] = t;
					f = cs*s[j] + sn*e[j];
					e[j] = cs*e[j] - sn*s[j];
					g = sn*s[j+1];
					s[j+1] = cs*s[j+1];
					for (int i = 0; i < nbCols; i++) 
					{
						const double th = cs*v[i][j] + sn*v[i][j+1];
						v[i][j+1] = -sn*v[i][j] + cs*v[i][j+1];
						v[i][j] = th;
					}

					t = hypot(f, g);
					cs = f/t;
					sn = g/t;
					s[j] = t;
					f = cs*e[j] + sn*s[j+1];

					s[j+1] = -sn*e[j] + cs*s[j+1];
					g = sn*e[j+1];
					e[j+1] = cs*e[j+1];
					if (j < nbRows-1) 
					{
						for (int i = 0; i < nbCols; i++) 
						{
							const double th = cs*u[i][j] + sn*u[i][j+1];
							u[i][j+1] = -sn*u[i][j] + cs*u[i][j+1];
							u[i][j] = th;
						}
					}
				}

				e[p-2] = f;
				iter = iter + 1;
			}
			break;
				
			// Convergence.
			case 4: 
			{
				// Make the singular values positive.
				if (s[k] <= 0.L) 
				{
					s[k] = (s[k] < 0.L ? -s[k] : 0.0);
					for (int i = 0; i <= pp; i++)
						v[i][k] = -v[i][k];

				}
				
				// Order the singular values.
				while (k < pp) {
					if (s[k] >= s[k+1]) {
						break;
					}
					double t = s[k];
					s[k] = s[k+1];
					s[k+1] = t;
					if (k < nbCols-1) 
						for (int i = 0; i < nbCols; i++) {
							t = v[i][k+1]; 
							v[i][k+1] = v[i][k]; 
							v[i][k] = t;
						}
					if (k < nbRows-1) 
						for (int i = 0; i < nbRows; i++) 
						{
							t = u[i][k+1]; 
							u[i][k+1] = u[i][k]; 
							u[i][k] = t;
						}

					k++;
				}

				iter = 0;
				p--;
			}
			break;
		}
	}
	
    delete []work;
    delete []e;

/*
	cout << "U matrix:\n=========\n";
	for (int i=0; i<nbRows; i++)
	{
		for (int j=0; j<nbCols; j++)
		{
			cout << u[i][j] << "    ";
		}
		cout << endl;
	}
	cout << endl;
	
	cout << "S diag:\n=========\n";
	for (int i=0; i<nbCols; i++)
		cout << s[i] << "    ";
	cout << endl << endl;

	cout << "V matrix:\n=========\n";
	for (int i=0; i<nbCols; i++)
	{
		for (int j=0; j<nbCols; j++)
		{
			cout << v[i][j] << "    ";
		}
		cout << endl;
	}
	cout << endl;
*/
	
	return status;
}

