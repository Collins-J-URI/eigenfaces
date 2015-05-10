/*  NAME:
        Matrix.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Matrix class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <sstream>
#include <cmath>
//
#include "Matrix.h"
#include "RandomGenerator_C.h"

using namespace uriVL;
using namespace std;

//------------------------------
//  Redefines static members
//------------------------------

string  Matrix::beginArrayStr_ = "[";   //  beginning of row or col array
string  Matrix::endArrayStr_ = "]";     //  end of a column or row
string  Matrix::elmtSepStr_ = ", ";     //  separator between elements
bool    Matrix::eolAtEor_ = true;       //  end of line at end of row

const double Matrix::MATRIX_PROP_REL_TOL = 1.E-7;
const double Matrix::MATRIX_PROP_ABS_TOL = 1.E-7;


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif


Matrix::Matrix(int nbRows, int nbCols)
	:	nbRows_(nbRows),
		nbCols_(nbCols),
		nbStorageRows_(nbRows),
		nbStorageCols_(nbCols),
		mat_(NULL),
		storageIsLocal_(false),
		//
		isDiagonal_(IS_TRUE),
		isIdentity_(IS_FALSE),
		isUpperTriangular_(IS_TRUE),
		isLowerTriangular_(IS_TRUE),
		isFullRank_(IS_FALSE),
		isSymmetric_(IS_TRUE),
		isAntisymmetric_(IS_TRUE),
		isOrthogonal_(IS_FALSE),
		isNormalized1_(IS_FALSE),
		isNormalized2_(IS_FALSE),
		isNormalizedInf_(IS_FALSE)
{
    FAIL_CONDITION( (nbRows < 1) || (nbCols<1),
                    kInvalidParameterError,
                    "Wrong dimensions in Matrix constructor");
      
    allocateMatrix_();
    
    if (nbRows == nbCols)
        isAntisymmetric_ = IS_TRUE;
    else
        isAntisymmetric_ = IS_FALSE;
    isOrthogonal_ = IS_FALSE;
}


Matrix::Matrix(int nbRows, int nbCols, double v)
	:	nbRows_(nbRows),
		nbCols_(nbCols),
		nbStorageRows_(nbRows),
		nbStorageCols_(nbCols),
		mat_(NULL),
		storageIsLocal_(false),
		//
		isDiagonal_(IS_TRUE),
		isIdentity_((nbRows_ == nbCols_) && (v== 1.L) ? IS_TRUE : IS_FALSE),
        isUpperTriangular_(v==0.L ? IS_TRUE : IS_FALSE),
        isLowerTriangular_(v==0.L ? IS_TRUE : IS_FALSE),
		isFullRank_((nbRows_ >= nbCols_) && (v!= 0.L) ? IS_TRUE : IS_FALSE),
		isSymmetric_(nbRows_ == nbCols_ ? IS_TRUE : IS_FALSE),
		isAntisymmetric_((nbRows_ == nbCols_) && (v== 0.L) ? IS_TRUE : IS_FALSE),
		isOrthogonal_((nbRows_ >= nbCols_) && (v*v != 1.L) ? IS_TRUE : IS_FALSE),
		isNormalized1_((fabs(fabs(nbRows*v) - 1) < 1E-6) ? IS_TRUE : IS_FALSE),
		isNormalized2_((nbRows_ == nbCols_) && (fabs(fabs(v)*nbRows - 1.0) < 1E-6) ? IS_TRUE : IS_FALSE),
		isNormalizedInf_((fabs(fabs(nbCols*v) - 1.0) < 1E-6) ? IS_TRUE : IS_FALSE)

{
    FAIL_CONDITION( (nbRows < 1) || (nbCols<1),
                    kInvalidParameterError,
                    "Wrong dimensions in Matrix constructor");

    allocateMatrix_();
    for (int i=0; i<nbRows_; i++)
        for (int j=0; j<nbCols_; j++)
            mat_[i][j] = v;  

}

Matrix::Matrix(int nbRows, int nbCols, double** m, bool safe, 
				const MatrixIndicators* matrixIndic) 
	:	nbRows_(nbRows),
		nbCols_(nbCols),
		nbStorageRows_(nbRows),
		nbStorageCols_(nbCols),
		mat_(NULL),
		storageIsLocal_(safe),
		//
		//
		isDiagonal_(IS_UNKNOWN),
		isIdentity_(IS_UNKNOWN),
		isUpperTriangular_(IS_UNKNOWN),
		isLowerTriangular_(IS_UNKNOWN),
		isFullRank_(IS_UNKNOWN),
		isSymmetric_(IS_UNKNOWN),
		isAntisymmetric_(IS_UNKNOWN),
		isOrthogonal_(IS_UNKNOWN),
		isNormalized1_(IS_UNKNOWN),
		isNormalized2_(IS_UNKNOWN),
		isNormalizedInf_(IS_UNKNOWN)
{
    FAIL_CONDITION( (nbRows < 1) || (nbCols<1),
                    kInvalidParameterError,
                    "Wrong dimensions in Matrix constructor");

    if (storageIsLocal_)
    {
        allocateMatrix_();

        for (int i=0; i<nbRows_; i++)
            for (int j=0; j<nbCols_; j++)
                mat_[i][j] = m[i][j];
    }
    else
        mat_ = m;

    if (matrixIndic != NULL)
    {
        isSymmetric_ = matrixIndic->symmetryStatus;
        isAntisymmetric_ = matrixIndic->antisymmetryStatus;
        isDiagonal_ = matrixIndic->diagonalityStatus;
        isIdentity_ = matrixIndic->identityStatus;
        isOrthogonal_ = matrixIndic->orthogonalityStatus;
        isUpperTriangular_ = matrixIndic->upperTriangularityStatus;
        isLowerTriangular_ = matrixIndic->lowerTriangularityStatus;
        isFullRank_ = matrixIndic->fullRankStatus;
		isNormalized1_ = matrixIndic->normalized1Status;
		isNormalized2_ = matrixIndic->normalized2Status;
		isNormalizedInf_ = matrixIndic->normalizedInfStatus;
    }
    else
    {
        //  only a square matrix can be identity, symmetric, or antisymmetric
        if (nbRows != nbCols)
        {
            isIdentity_ = IS_FALSE;
            isSymmetric_ = IS_FALSE;
            isAntisymmetric_ = IS_FALSE;
        }
    }
    
}


Matrix::Matrix(const Matrix* b)
	try	:		nbRows_(b->nbRows_),
				nbCols_(b->nbCols_),
				nbStorageRows_(b->nbRows_),
				nbStorageCols_(b->nbCols_),
				mat_(NULL),
				storageIsLocal_(false),
				//
				isDiagonal_(b->isDiagonal_),
				isIdentity_(b->isIdentity_),
				isUpperTriangular_(b->isUpperTriangular_),
				isLowerTriangular_(b->isLowerTriangular_),
				isFullRank_(b->isFullRank_),
				isSymmetric_(b->isSymmetric_),
				isAntisymmetric_(b->isAntisymmetric_),
				isOrthogonal_(b->isOrthogonal_),
				isNormalized1_(b->isNormalized1_),
				isNormalized2_(b->isNormalized2_),
				isNormalizedInf_(b->isNormalizedInf_)
{
    allocateMatrix_();

    for (int i=0; i<nbRows_; i++)
        for (int j=0; j<nbCols_; j++)
            mat_[i][j] = b->mat_[i][j];
    
}
catch (...)
{
    FAIL_CONDITION( b == NULL,
                    kNullParameterError,
                    "Null parameter in Matrix copy constructor");
}


Matrix::Matrix(const Matrix& b)
	try	:		nbRows_(b.nbRows_),
				nbCols_(b.nbCols_),
				nbStorageRows_(b.nbRows_),
				nbStorageCols_(b.nbCols_),
				mat_(NULL),
				storageIsLocal_(false),
				//
				isDiagonal_(b.isDiagonal_),
				isIdentity_(b.isIdentity_),
				isUpperTriangular_(b.isUpperTriangular_),
				isLowerTriangular_(b.isLowerTriangular_),
				isFullRank_(b.isFullRank_),
				isSymmetric_(b.isSymmetric_),
				isAntisymmetric_(b.isAntisymmetric_),
				isOrthogonal_(b.isOrthogonal_),
				isNormalized1_(b.isNormalized1_),
				isNormalized2_(b.isNormalized2_),
				isNormalizedInf_(b.isNormalizedInf_)
{
    allocateMatrix_();

    for (int i=0; i<nbRows_; i++)
        for (int j=0; j<nbCols_; j++)
            mat_[i][j] = b.mat_[i][j];
    
}
catch (...)
{
    FAIL_CONDITION( true,
                    kInvalidReferenceError,
                    "Invalid reference in Matrix copy constructor");
}


Matrix::~Matrix(void)
{
    if (mat_ != NULL)
    {
    	for (int i=0; i<nbRows_; i++)
    		delete []mat_[i];
    			
    	delete []mat_;
    }
}


//========================================================================
#if 0
#pragma mark -
#endif

Matrix* Matrix::copyMatrix(int nbRows, int nbCols, const double*const* a) 
{
    FAIL_CONDITION( (nbRows < 1) || (nbCols<1),
                    kInvalidParameterError,
                    "Wrong dimensions in Matrix::copyMatrix");

    Matrix *m = new Matrix(nbRows, nbCols);
    m->nbRows_ = nbRows;
    m->nbCols_ = nbCols;

    for (int i=0; i<nbRows; i++)
        for (int j=0; j<nbCols; j++)
            m->mat_[i][j] = a[i][j];

    m->isDiagonal_ = IS_UNKNOWN;
    m->isUpperTriangular_ = IS_UNKNOWN;
    m->isLowerTriangular_ = IS_UNKNOWN;
    m->isFullRank_ = IS_FALSE;
    m->isOrthogonal_ = IS_UNKNOWN;
    //  only a square matrix can be identity, symmetric, or antisymmetric
    if (nbRows == nbCols)
    {
        m->isIdentity_ = IS_UNKNOWN;
        m->isSymmetric_ = IS_UNKNOWN;
        m->isAntisymmetric_ = IS_UNKNOWN;
    }
    else
    {
        m->isIdentity_ = IS_FALSE;
        m->isSymmetric_ = IS_FALSE;
        m->isAntisymmetric_ = IS_FALSE;
    }
    
    return m;
}



Matrix* Matrix::identityMatrix(int n)
{
    return diagonalMatrix(n, n, 1.L);
}


Matrix* Matrix::diagonalMatrix(int nbRows, int nbCols, double v)
{
    Matrix  *diag = new Matrix(nbRows, nbCols);

    int smallerDim = (nbRows < nbCols) ? nbRows : nbCols;

    for (int i = 0; i < smallerDim; i++)
        diag->mat_[i][i] = v;

    diag->isDiagonal_ = IS_TRUE;
    diag->isUpperTriangular_ = IS_TRUE;
    diag->isLowerTriangular_ = IS_TRUE;

    if (v != 0.L)
	{
        diag->isFullRank_ = IS_TRUE;
	}
    else
	{
        diag->isFullRank_ = IS_FALSE;
	}
	
    //  only a square matrix can be identity, symmetric, or antisymmetric
    if (nbRows == nbCols)
    {
        if (v == 1.L)
        {
            diag->isIdentity_ = IS_TRUE;
            diag->isOrthogonal_ = IS_TRUE;
        }
        else if (v == -1.L)
        {
            diag->isIdentity_ = IS_FALSE;
            diag->isOrthogonal_ = IS_TRUE;
        }
        else
        {
            diag->isIdentity_ = IS_FALSE;
            diag->isOrthogonal_ = IS_FALSE;
        }
        diag->isSymmetric_ = IS_TRUE;
        diag->isAntisymmetric_ = IS_TRUE;
    }
    else
    {
        diag->isIdentity_ = IS_FALSE;
        diag->isSymmetric_ = IS_FALSE;
        diag->isAntisymmetric_ = IS_FALSE;
        diag->isOrthogonal_ = IS_FALSE;
    }
    
    return diag;
}

Matrix* Matrix::diagonalMatrix(int nbRows, int nbCols, const double* v)
{
    FAIL_CONDITION( v == NULL,
                    kNullParameterError,
                    "Null array passed as parameter to Matrix::diagonalMatrix");
    Matrix  *diag = new Matrix(nbRows, nbCols);

    //  true no matter what v looks like
    diag->isDiagonal_ = IS_TRUE;
    diag->isUpperTriangular_ = IS_TRUE;
    diag->isLowerTriangular_ = IS_TRUE;
    //  subject to change
    diag->isFullRank_ = IS_TRUE;
    diag->isIdentity_ = IS_TRUE;
    
    if (nbRows != nbCols)
    {
        diag->isSymmetric_ = IS_FALSE;
        diag->isAntisymmetric_ = IS_FALSE;
        diag->isOrthogonal_ = IS_FALSE;
    }
    else
    {
        diag->isSymmetric_ = IS_TRUE;
        diag->isAntisymmetric_ = IS_TRUE;
        diag->isOrthogonal_ = IS_TRUE;
    }

    int smallerDim = (nbRows < nbCols) ? nbRows : nbCols;
    for (int i = 0; i < smallerDim; i++)
    {
        const double  vi = v[i];
        if (vi != 1.L)
            diag->isIdentity_ = IS_FALSE;
        else if (vi != -1.L)
            diag->isOrthogonal_ = IS_FALSE;
        else if (vi == 0.L)
        {
            diag->isFullRank_ = IS_FALSE;
        }
        
        diag->mat_[i][i] = vi;
    }
    
    return diag;
}


Matrix *Matrix::randomMatrix(int nbRows, int nbCols, double lowerBound, double upperBound)
{
    Matrix *m = new Matrix(nbRows, nbCols);
    for (int i = 0; i < nbRows; i++)
        for (int j = 0; j < nbCols; j++)
            m->mat_[i][j] = RandomGenerator_C::nextDouble_st(upperBound, lowerBound);

    m->isDiagonal_ = IS_FALSE;
    m->isIdentity_ = IS_FALSE;
    m->isUpperTriangular_ = IS_FALSE;
    m->isLowerTriangular_ = IS_FALSE;
    m->isFullRank_ = IS_UNKNOWN;
    m->isSymmetric_ = IS_FALSE;
    m->isAntisymmetric_ = IS_FALSE;
    m->isOrthogonal_ = IS_FALSE;

    return m;
}



//------------------------------------------
//  access methods
//------------------------------------------
#if 0
#pragma mark -
#endif


int Matrix::getNbRows() const
{
    return nbRows_;
}

int Matrix::getNbCols() const
{
    return nbCols_;
}

double* const* Matrix::getMatrix(ReadWriteStatus rwa)
{
    isDiagonal_ = IS_UNKNOWN;
    isIdentity_ = IS_UNKNOWN;
    isUpperTriangular_ = IS_UNKNOWN;
    isLowerTriangular_ = IS_UNKNOWN;
    isFullRank_ = IS_UNKNOWN;
    isSymmetric_ = IS_UNKNOWN;
    isAntisymmetric_ = IS_UNKNOWN;
    isOrthogonal_ = IS_UNKNOWN;

    return mat_;
}

const double* const* Matrix::getMatrix(void) const
{
    return mat_;
}


double Matrix::operator ()(int theRow, int theCol) const
{
  FAIL_CONDITION( (theRow < 0) || (theCol < 0) || (theRow >= nbRows_) || (theCol >= nbCols_),
                  kMatrixIncorrectDimensionsError,
                  "dimension error in Matrix::()");
  return mat_[theRow][theCol];
}


//------------------------------------------
//  addition/subtraction methods
//------------------------------------------
#if 0
#pragma mark -
#endif


//const Matrix uriVL::operator +(const Matrix& a, const Matrix& b)
//{
//    const int   nbRows = a.nbRows_,
//                nbCols = a.nbCols_;
//                
//    FAIL_CONDITION( (nbRows != b.nbRows_) || (nbCols != b.nbCols_),
//                    kMatrixIncorrectDimensionsError,
//                    "parameter has incorrect dimensions in Matrix::operator+");
//
//    double  **m = new double*[nbRows];
//    for (int i=0; i<nbRows; i++)
//    {
//        m[i] = new double[nbCols];
//        for (int j=0; j<nbCols; j++)
//        {
//            m[i][j] = a.mat_[i][j] + b.mat_[i][j];
//        }
//    }
//
//    MatrixIndicators matrixIndic = {IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN,
//                                    IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN};
//    
//    return Matrix(nbRows, nbCols, m, false, &matrixIndic);
//}
//
//
//const Matrix uriVL::operator -(const Matrix& a, const Matrix& b) 
//{
//    const int   nbRows = a.nbRows_,
//                nbCols = a.nbCols_;
//                
//    FAIL_CONDITION( (nbRows != b.nbRows_) || (nbCols != b.nbCols_),
//                    kMatrixIncorrectDimensionsError,
//                    "parameter has incorrect dimensions in Matrix::operator-");
//
//    double  **m = new double*[nbRows];
//    for (int i=0; i<nbRows; i++)
//    {
//        m[i] = new double[nbCols];
//        for (int j=0; j<nbCols; j++)
//        {
//            m[i][j] = a.mat_[i][j] - b.mat_[i][j];
//        }
//    }
//
//    MatrixIndicators matrixIndic = {IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN,
//                                    IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN};
//    
//    return Matrix(nbRows, nbCols, m, false, &matrixIndic);
//}



void Matrix::operator +=(const Matrix& b)
{
    FAIL_CONDITION( (b.nbRows_ != nbRows_) || (b.nbCols_ != nbCols_),
                    kMatrixIncorrectDimensionsError,
                    "parameter has incorrect dimensions in Matrix::operator +=");

    for (int i=0; i<nbRows_; i++)
        for (int j=0; j<nbCols_; j++)
            mat_[i][j] += b.mat_[i][j];
  
}



void Matrix::operator -=(const Matrix& b)
{
    FAIL_CONDITION( (b.nbRows_ != nbRows_) || (b.nbCols_ != nbCols_),
                    kMatrixIncorrectDimensionsError,
                    "parameter has incorrect dimensions in Matrix::subtractFrom");

    for (int i=0; i<nbRows_; i++)
        for (int j=0; j<nbCols_; j++)
            mat_[i][j] -= b.mat_[i][j];

}


//------------------------------------------
//  multiplication methods
//------------------------------------------
#if 0
#pragma mark -
#endif

const Matrix uriVL::operator *(const Matrix& a, double s) 
{
    return s * a;
}

const Matrix uriVL::operator *(double s, const Matrix& a) 
{
    const int   nbRows = a.nbRows_,
                nbCols = a.nbCols_;
                
    double  **m = new double*[nbRows];
    for (int i=0; i<nbRows; i++)
    {
        m[i] = new double[nbCols];
        for (int j=0; j<nbCols; j++)
            m[i][j] = s*a.mat_[i][j];
    }

    MatrixIndicators matrixIndic = {IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN,
                                    IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN,
									IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN};
    
    return Matrix(nbRows, nbCols, m, false, &matrixIndic);
}

void Matrix::operator*=(const double s)
{
    for (int i=0; i<nbRows_; i++)
        for (int j=0; j<nbCols_; j++)
            mat_[i][j] *= s;

}


const Matrix uriVL::operator *(const Matrix& a, const Matrix& b) 
{
    const int   nbRows = a.nbRows_,
                n = a.nbCols_,
                nbCols = b.nbCols_;
                
	FAIL_CONDITION( b.nbRows_ != n,
					kMatrixIncorrectDimensionsError,
					"Matrices dimsnions dont match in Matrix::operator*");

	double** c = new double*[nbRows];
	for (int i=0; i<nbRows; i++)
	{
		c[i] = new double[nbCols];
	}
	
	Matrix::product_(a.mat_, b.mat_, c, nbRows, n, nbCols);

    MatrixIndicators matrixIndic = {IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN,
                                    IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN,
									IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN};
    
    return Matrix(nbRows, nbCols, c, false, &matrixIndic);
}



void Matrix::operator *=(const Matrix& b)
{
	FAIL_CONDITION( b.nbRows_ != nbCols_,
	              kMatrixIncorrectDimensionsError,
	              "parameter has incorrect dimensions in Matrix::*=");

	double** c = new double*[nbRows_];
	for (int i=0; i<nbRows_; i++)
		c[i] = new double[b.nbCols_];

	for (int i=0; i<nbRows_; i++)
		for (int j=0; j<b.nbCols_; j++)
		{
		 	c[i][j] = 0.L;
			for (int k=0; k<nbCols_; k++)
				c[i][j] += mat_[i][k] * b.mat_[k][j];
		}

	killMat_();
	mat_ = c;
	nbCols_ = b.nbCols_;
}



void Matrix::preMultiplyBy(const Matrix* b)
{
    FAIL_CONDITION( b == NULL,
                    kNullParameterError,
                    "Null parameter in Matrix::postMultiplyBy");
	FAIL_CONDITION( b->nbCols_ != nbRows_,
	              kMatrixIncorrectDimensionsError,
	              "parameter has incorrect dimensions in Matrix::postMultiplyBy");

    const int   newRows = b->nbRows_;
	double** c = new double*[newRows];
	for (int i=0; i<newRows; i++)
		c[i] = new double[nbCols_];

	for (int i=0; i<newRows; i++)
		for (int j=0; j<nbCols_; j++)
		{
		 	c[i][j] = 0.L;
			for (int k=0; k<nbRows_; k++)
				c[i][j] += b->mat_[i][k]*mat_[k][j];
		}

	killMat_();
	mat_ = c;
	nbRows_ = newRows;
}


void Matrix::postMultiplyBy(const Matrix* b)
{
    FAIL_CONDITION( b == NULL,
                    kNullParameterError,
                    "Null parameter in Matrix::postMultiplyBy");
    (*this) *= (*b);
}


//------------------------------------------
//  equality methods
//------------------------------------------
#if 0
#pragma mark -
#endif

const Matrix& Matrix::operator=(const Matrix& b)
{
    //  if the dimensions don't match, delete this matrix an reallocate it.
    if ((nbRows_ != b.nbRows_) || (nbCols_ != b.nbCols_))
    {
        killMat_();
        
        nbRows_ = b.nbRows_;
        nbCols_= b.nbCols_;
        
        mat_ = new double*[nbRows_];
        for (int i=0; i<nbRows_; i++)
        {
            mat_[i] = new double[nbCols_];
            for (int j=0; j<nbCols_; j++)
                mat_[i][j] = b.mat_[i][j];
        }  
    }
    else
    {
        for (int i=0; i<nbRows_; i++)
            for (int j=0; j<nbCols_; j++)
                mat_[i][j] = b.mat_[i][j];

    }
    
    return *this;
}

TriState Matrix::operator ==(const Matrix& b) const
{
    if ((b.nbRows_ == nbRows_) && (b.nbCols_ == nbCols_))
    {
        double maxDev = 0.L;
        
        for (int i = 0; i < nbRows_; i++)
            for (int j = 0; j < nbCols_; j++)
            {
                double dev = fabs(mat_[i][j] - b.mat_[i][j]);
                    
                if (dev > MATRIX_PROP_REL_TOL*fabs(mat_[i][j]))
                    return IS_FALSE;

                if (dev > maxDev)
                    maxDev = dev;
            }

        //   if we made it all the way here, then the matrix is symmetric
        if (maxDev < STD_RELATIVE_TOLERANCE)
            return IS_TRUE;
        else
            return IS_FALSE;
        
    }
    else
        return IS_FALSE;

}


//------------------------------------------
//  normalization methods
//------------------------------------------
#if 0
#pragma mark -
#endif


void Matrix::rowSwap(int r1, int r2)
{
    FAIL_CONDITION ( (r1 < 0) || (r2 < 0) || (r1 >= nbRows_) || (r2 >= nbRows_),
                     kInvalidParameterError,
                     "Invalid row choices in Matrix::rowSwap");

    if (r1 != r2)
    {
        double* temp = mat_[r1];
        mat_[r1] = mat_[r2];
        mat_[r2] = temp;
        
        //  row swap pretty much breaks all properties
        isDiagonal_ = IS_UNKNOWN;
        isUpperTriangular_ = IS_UNKNOWN;
        isLowerTriangular_ = IS_UNKNOWN;
        isFullRank_ = IS_UNKNOWN;
        isSymmetric_ = IS_UNKNOWN;
        isAntisymmetric_ = IS_UNKNOWN;
        isOrthogonal_ = IS_UNKNOWN;
    }
}

void Matrix::colSwap(int c1, int c2)
{
  FAIL_CONDITION ( (c1 < 0) || (c2 < 0) || (c1 >= nbCols_) || (c2 >= nbCols_),
                 kInvalidParameterError,
                 "Invalid column choices in Matrix::rowSwap");
    if (c1 != c2)
    {
        for (int i = 0; i < nbRows_; i++)
        {
            double temp = mat_[i][c1];
            mat_[i][c1] = mat_[i][c2];
            mat_[i][c2] = temp;
        }

        //  column swap pretty much breaks all properties
        isDiagonal_ = IS_UNKNOWN;
        isUpperTriangular_ = IS_UNKNOWN;
        isLowerTriangular_ = IS_UNKNOWN;
        isFullRank_ = IS_UNKNOWN;
        isSymmetric_ = IS_UNKNOWN;
        isAntisymmetric_ = IS_UNKNOWN;
        isOrthogonal_ = IS_UNKNOWN;
    }
}

//------------------------------------------
//  normalization methods
//------------------------------------------
#if 0
#pragma mark -
#endif

double Matrix::normInf(void) const
{
    double  maxVal = 0.L, rowSum;

    for (int i=0; i<nbRows_; i++)
    {
        rowSum = 0.L;
        for (int j=0; j<nbCols_; j++)
            rowSum += fabs(mat_[i][j]);

        if (rowSum  > maxVal)
            maxVal = rowSum;

    }

  return maxVal;
}


double Matrix::norm1(void) const
{
    double  maxVal = 0.L, colSum;

    for (int j=1; j<nbCols_; j++)
    {
        colSum = 0.f;
        for (int i=0; i<nbRows_; i++)
            colSum += fabs(mat_[i][j]);

        if (colSum > maxVal)
            maxVal = colSum;

    }

  return maxVal;
}


//------------------------------------------
//  check matrix properties
//------------------------------------------
#if 0
#pragma mark -
#endif

TriState Matrix::isSymmetric(void)
{
    double  maxDev = 0.L;
    
    if (nbRows_ == nbCols_)
    {
        for (int i=0; i<nbRows_; i++)
        {
            for (int j=0; j<i; j++)
            {
                if (mat_[i][j] != 0.L)
                {
                    double dev = fabs(mat_[i][j] - mat_[j][i]);
                    
                    if (dev > MATRIX_PROP_REL_TOL*fabs(mat_[i][j]))
                        return (isSymmetric_ = IS_FALSE);

                    if (dev > maxDev)
                        maxDev = dev;
                }
                else
                {
                    double dev = fabs(mat_[j][i]);
                    if (dev > MATRIX_PROP_ABS_TOL)

                    if (dev > maxDev)
                        maxDev = dev;
                }
            }
           
        }
        //   if we made it all the way here, then the matrix is symmetric
        if (maxDev < STD_RELATIVE_TOLERANCE)
            return (isSymmetric_ = IS_TRUE);
        else
            return IS_FALSE;
        
    }
    else
        return (isSymmetric_ = IS_FALSE);
        
}

TriState Matrix::isAntisymmetric(void)
{
    double  maxDev = 0.L, dev;
    
    if (nbRows_ == nbCols_)
    {
        for (int i=0; i<nbRows_; i++)
        {
            //  Check nondiagonal elements
            for (int j=0; j<i; j++)
            {
                if (mat_[i][j] != 0.L)
                {
                    dev = fabs(mat_[i][j] + mat_[j][i]);
                    
                    if (dev > MATRIX_PROP_REL_TOL*fabs(mat_[i][j]))
                        return (isAntisymmetric_ = IS_FALSE);

                    if (dev > maxDev)
                        maxDev = dev;
                }
                else
                {
                    dev = fabs(mat_[j][i]);
                    if (dev > MATRIX_PROP_ABS_TOL)
                        return (isAntisymmetric_ = IS_FALSE);

                    if (dev > maxDev)
                        maxDev = dev;
                }
            }
        
            //  Now check the diagonal element for this row
            dev = fabs(mat_[i][i]);
            if (dev > MATRIX_PROP_ABS_TOL)
                return (isAntisymmetric_ = IS_FALSE);

            if (dev > maxDev)
                maxDev = dev;
                        
        }
        //   if we made it all the way here, then the matrix is antisymmetric
        if (maxDev < STD_RELATIVE_TOLERANCE)
            return (isAntisymmetric_ = IS_TRUE);
        else
            return (isAntisymmetric_ = IS_FALSE);
        
    }
    else
        return (isAntisymmetric_ = IS_FALSE);
        
}


TriState Matrix::isOrthogonal(void)
{
	FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "method not open for business yet"); 
    return IS_UNKNOWN;
}


TriState Matrix::isDiagonal(void)
{
    //  The matrix is diagonal iff it is both upper-diagonal and lower_diagonal
    if (isUpperTriangular() == IS_TRUE)
    {
        isLowerTriangular();
        if (isLowerTriangular() == IS_TRUE)
            return  (isDiagonal_ = IS_TRUE);
        else
            return (isDiagonal_ = IS_FALSE);

    }
    else
        return (isDiagonal_ = IS_FALSE);

}


TriState Matrix::isIdentity(void)
{
    if (nbRows_ == nbCols_)
    {
        if (isDiagonal() == IS_TRUE)
        {
            double maxDev = 0.L;
            for (int i=0; i<nbRows_; i++)
            {
                double dev = fabs(mat_[i][i] - 1.L);

                if (dev > MATRIX_PROP_REL_TOL)
                    return (isIdentity_ = IS_FALSE);

                if (dev > maxDev)
                    maxDev = dev;
            }    
            //   if we made it all the way here, then the matrix is indetity
            if (maxDev < STD_RELATIVE_TOLERANCE)
                return (isIdentity_ = IS_TRUE);
            else
                return (isIdentity_ = IS_FALSE);
            
        }
        else
            return (isIdentity_ = IS_FALSE);
        
    }
    else
        return (isIdentity_ = IS_FALSE);
        
}


TriState Matrix::isUpperTriangular(void)
{
    double  maxDev = 0.L, dev;
    const int jMax = nbRows_ > nbCols_ ? nbCols_ : nbRows_;
    
    //  check that all subdiagonal elements are nearly equal to zero
    for (int j=0; j<jMax; j++)
    {
        for (int i=j+1; i<nbRows_; i++)
        {
            dev = fabs(mat_[i][j]);
            if (dev > MATRIX_PROP_ABS_TOL)
                return (isUpperTriangular_ = IS_FALSE);

            if (dev > maxDev)
                maxDev = dev;

        }
    }
         
    //   if we made it all the way here, then the matrix is upper-triangular
    if (maxDev < STD_RELATIVE_TOLERANCE)
        return (isUpperTriangular_ = IS_TRUE);
    else
        return (isUpperTriangular_ = IS_FALSE);

}

TriState Matrix::isLowerTriangular(void)
{
    const int iMax = nbRows_ > nbCols_ ? nbCols_ : nbRows_;
    double  maxDev = 0.L, dev;
    
    //  check that all superdiagonal elements are nearly equal to zero
    for (int i=0; i<iMax; i++)
    {
        for (int j=i+1; j<nbCols_; j++)
        {
            dev = fabs(mat_[i][j]);
            if (dev > MATRIX_PROP_ABS_TOL)
                return (isLowerTriangular_ = IS_FALSE);

            if (dev > maxDev)
                maxDev = dev;

        }
    }
                        
    //   if we made it all the way here, then the matrix is lower-triangular
    if (maxDev < STD_RELATIVE_TOLERANCE)
        return (isLowerTriangular_ = IS_TRUE);
    else
        return (isLowerTriangular_ = IS_FALSE);

}

//------------------------------------------
//  enforce matrix properties
//------------------------------------------
#if 0
#pragma mark -
#endif


void Matrix::enforceSymmetry(void)
{
    FAIL_CONDITION(	nbRows_ != nbCols_,
    				kMatrixIncorrectDimensionsError,
    				"only square matrices can be forced to be symmetric");

    for (int i=0; i<nbRows_; i++)
        for (int j=0; j<i; j++)
        {
            double midPt = 0.5L*(mat_[i][j] + mat_[j][i]);
            mat_[i][j] = mat_[j][i] = midPt;
        }		

    isSymmetric_ = IS_TRUE;
}

void Matrix::enforcesAntisymmetry(void)
{
    FAIL_CONDITION(	nbRows_ != nbCols_,
    				kMatrixIncorrectDimensionsError,
    				"only square matrices can be forced to be symmetric");

    for (int i=0; i<nbRows_; i++)
    {
        for (int j=0; j<i; j++)
        {
            double midPt = 0.5L*(mat_[i][j] - mat_[j][i]);
            mat_[i][j] = midPt;
            mat_[j][i] = -midPt;
        }		

        mat_[i][i] = 0.L;
    }
    
    isAntisymmetric_ = IS_TRUE;
}

void Matrix::enforceOthogonality(void)
{
	FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "method not open for business yet"); 
}

void Matrix::enforceDiagonality(void)
{
    for (int i=0; i<nbRows_; i++)
        for (int j=0; j<nbCols_; j++)
            if (i!=j)
                mat_[i][j] = 0.L;

    isDiagonal_ = IS_TRUE;                
}

void Matrix::enforceUpperTriangularity(void)
{
    const int jMax = nbRows_ > nbCols_ ? nbCols_ : nbRows_;
    
    //  set all subdiagonal elements to zero
    for (int j=0; j<jMax; j++)
        for (int i=j+1; i<nbRows_; i++)
            mat_[i][j] = 0.L;

    isUpperTriangular_ = IS_TRUE;
}

void Matrix::enforceLowerTriangularity(void)
{
    const int iMax = nbRows_ > nbCols_ ? nbCols_ : nbRows_;
    
    //  set all superdiagonal elements to zero
    for (int i=0; i<iMax; i++)
        for (int j=i+1; j<nbCols_; j++)
            mat_[i][j] = 0.L;

    isLowerTriangular_ = IS_TRUE;
}




#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Transpose, inverse, determinant
//------------------------------------------------------
#endif


Matrix* Matrix::transposition() const
{
    Matrix  *b = new Matrix(nbCols_, nbRows_);
    double** bMat = b->mat_;

    //  This can be optimized based on the matrix's properties
    for (int i=0; i<nbCols_; i++)
        for (int j=0; j<nbRows_; j++)
            bMat[i][j] = mat_[j][i];

    //  update properties of matrix b
    b->isDiagonal_ = isDiagonal_;
    b->isUpperTriangular_ = isLowerTriangular_;
    b->isLowerTriangular_ = isUpperTriangular_;
    b->isFullRank_ = isFullRank_;
    b->isSymmetric_ = isSymmetric_;
    b->isAntisymmetric_ = isAntisymmetric_;
    b->isOrthogonal_ = isOrthogonal_;

    return b;
}

void Matrix::transpose()
{
    //  if the matrix is not sqaure, we need to allocate a new array
    if (nbRows_ != nbCols_)
    {
        double** mat2_ = new double*[nbCols_];

        for (int i=0; i < nbCols_; i++)
            mat2_[i] = new double[nbRows_];

        for (int i=0; i<nbCols_; i++)
            for (int j=0; j<nbRows_; j++)
                mat2_[i][j] = mat_[j][i];

        killMat_();

        mat_ = mat2_;
    }
    //  otherwise we transpose within the array
    else
    {
        for (int i=0; i<nbRows_; i++)
            for (int j=0; j<i; j++)
            {
                double temp = mat_[i][j];
                mat_[i][j] = mat_[j][i];
                mat_[i][j] = temp;
            }    
    }

    //  update properties of the matrix
    TriState tempProp = isUpperTriangular_;
    isUpperTriangular_ = isLowerTriangular_;
    isLowerTriangular_ = tempProp;
}


//------------------------------------------
//  inverse methods
//------------------------------------------
#if 0
#pragma mark -
#endif

Matrix* Matrix::inverse(void) const 
{
    FAIL_CONDITION( nbRows_ != nbCols_,
                    kNumericalError,
                    "Not a square matrix in Matrix::inverse()");

    //------------------------------------------------------------
    //  Special cases: exact properties
    //------------------------------------------------------------
    if (isIdentity_ == IS_TRUE)
        return Matrix::identityMatrix(nbRows_);
    //
    else if (isDiagonal_ == IS_TRUE)
    {
        double  *v = new double[nbRows_];
        for (int i=0; i<nbRows_; i++)
        {
            FAIL_CONDITION( mat_[i][i] == 0.L,
                            kNumericalError,
                            "Attempt to invert a singular diagonal matrix");
            v[i] = 1.L / mat_[i][i];
        }
        Matrix *x = Matrix::diagonalMatrix(nbRows_, nbRows_, v);
        delete []v;
        return x;
    }
    //
    else if (isOrthogonal_ == IS_TRUE)
    {
        return transposition();
    }
    
//  This is completely absurd:  The LU factorization is done twice!!
    double det = this->determinant();
    FAIL_CONDITION( det == 0,
    kNumericalError,
    "Not a nonsingular matrix in Matrix::inverse()");

/* 
    Matrix* i = Matrix::identityMatrix(nbRows_);

  LinearSolver_LU* myLU = new LinearSolver_LU();

    Matrix* x = myLU->solve(this,i);

    delete i;

    delete myLU;

    x->isDiagonal_ = isDiagonal_;
    x->isIdentity_ = isIdentity_;
    if (isUpperTriangular_ == IS_TRUE)
        x->isUpperTriangular_ = IS_APPROX_TRUE;
    else
        x->isUpperTriangular_ = isUpperTriangular_;
    if (isLowerTriangular_ == IS_TRUE)
        x->isLowerTriangular_ = IS_APPROX_TRUE;
    else
        x->isLowerTriangular_ = isLowerTriangular_;
    x->isFullRank_ = IS_TRUE;
    x->isSymmetric_ = isSymmetric_;
    x->isAntisymmetric_ = isAntisymmetric_;
    x->isOrthogonal_ = isOrthogonal_;

    return x;  
*/
return NULL;
}

void Matrix::invert(void) 
{
    Matrix* x = inverse();

    double** xMat = x->mat_;

    for (int i = 0; i < nbRows_; i++)
        for (int j = 0; j < nbCols_; j++)
            mat_[i][j] = xMat[i][j];

    isDiagonal_ = x->isDiagonal_;
    isIdentity_ = x->isIdentity_;
    isUpperTriangular_ = x->isUpperTriangular_;
    isLowerTriangular_ = x->isLowerTriangular_;
    isFullRank_ = x->isFullRank_;
    isSymmetric_ = x->isSymmetric_;
    isAntisymmetric_ = x->isAntisymmetric_;
    isOrthogonal_ = x->isOrthogonal_;

    delete x;
}


double Matrix::determinant(void) const
{
/*
  FAIL_CONDITION( nbRows_ != nbCols_,
                  kNumericalError,
                  "Not a square matrix in Matrix::determinant()");
  Factorization_LU* myLU = new Factorization_LU(this);
  
  int* p = myLU->getP();
  Matrix* LU = myLU->getLU();

  double det = 1;
  for (int i = 0; i < nbRows_; i++)
    det *= (*LU)(p[i],i);
  
  delete[] p;
  delete LU;
  delete myLU;

  return det;
*/
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");

    return 0.L;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Private calculation functions
//------------------------------------------------------
#endif


void Matrix::sum_(const double *const* a, const double *const* b, double *const* c, int m, int n)
{
	for (int i=0; i<m; i++)
	{
		for (int j=0; j<n; j++)
		{
		 	c[i][j] = a[i][j] + b[i][j];
		}
    }
}

void Matrix::difference_(const double *const* a, const double *const* b, double *const* c, int m, int n)
{
	for (int i=0; i<m; i++)
	{
		for (int j=0; j<n; j++)
		{
		 	c[i][j] = a[i][j] - b[i][j];
		}
    }
}

void Matrix::product_(const double *const* a, const double *const* b, double *const* c, int m, int n, int p)
{
	for (int i=0; i<m; i++)
	{
		for (int j=0; j<p; j++)
		{
		 	c[i][j] = 0.L;
			for (int k=0; k<n; k++)
				c[i][j] += a[i][k] * b[k][j];

		}
    }
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Output functions
//------------------------------------------------------
#endif



string Matrix::toString(void) const
{ 
  return toString(beginArrayStr_, endArrayStr_, elmtSepStr_, eolAtEor_);
}


string Matrix::toString(string theBeginArrayStr, string theEndArrayStr,
                string theElmtSepStr, bool theEolAtEor) const
{ 
  FAIL_CONDITION ( (theBeginArrayStr.length()==0) || (theEndArrayStr.length()==0) || (theElmtSepStr.length()==0),
                   kInvalidParameterError,
                   "Null seperator string");

  string  outStr, myEndRow;
    
  if (theEolAtEor)
    myEndRow = "\n";
  else
    myEndRow = "";
  
  outStr = theBeginArrayStr;    //  beginning of the array, say "{"
  for (int i=0; i<nbRows_; i++)
  {
    outStr.append(theBeginArrayStr);
    std::ostringstream o;
    o << mat_[i][0];
    outStr.append(o.str());
  
    for (int j=1; j<nbCols_; j++)
    {
      outStr.append(theElmtSepStr);
      std::ostringstream m;
      m << mat_[i][j];
      outStr.append(m.str());
    }

    outStr.append(theEndArrayStr);

    if (i!=nbRows_-1)
    {
      outStr.append(theElmtSepStr);
      outStr.append(myEndRow);
    }
    else  
      outStr.append(theEndArrayStr);
     
  }
  return outStr;
}


void Matrix::setToStringFormat(string theBeginArrayStr, string theEndArrayStr,
            string theElmtSepStr, bool theEolAtEor)
{
  FAIL_CONDITION ( (theBeginArrayStr.length()==0) || (theEndArrayStr.length()==0) || (theElmtSepStr.length()==0),
                   kInvalidParameterError,
                   "Null seperator string");
    
  beginArrayStr_ = theBeginArrayStr;
  endArrayStr_ = theEndArrayStr;
  elmtSepStr_ = theElmtSepStr;
  eolAtEor_ = theEolAtEor;
}  



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocation and deallocation functions
//------------------------------------------------------
#endif

void Matrix::allocateMatrix_(void)
{
    mat_ = new double*[nbStorageRows_];
    for (int i=0; i < nbStorageRows_; i++)
        mat_[i] = new double[nbStorageCols_];

    storageIsLocal_ = true;
}

void Matrix::killMat_(void)
{
    if (storageIsLocal_)
    {
        for (int i = 0; i < nbStorageRows_; i++)
            delete[] mat_[i];
    
        delete[] mat_;
    }

    mat_ = NULL;
}


