/*  NAME:
        ColumnVector.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColumnVector class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "RowVector.h"
#include "ColumnVector.h"
#include "RandomGenerator_C.h"

using namespace uriVL;

ColumnVector::ColumnVector(const int nbRows)
		try	:   Matrix(nbRows, 1)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by ColumnVector constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid dimensions in ColumnVector constructor");
}


ColumnVector::ColumnVector(const int nbRows, const double v)
		try	:   Matrix(nbRows, 1)
{
    for (int i = 0; i < nbRows_; i++)
	{
        mat_[i][0] = v;
	}
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by ColumnVector constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid dimensions in ColumnVector constructor");
}


ColumnVector::ColumnVector(const int nbRows, const double* v)
		try	:   Matrix(nbRows, 1)
{
    for (int i = 0; i < nbRows_; i++)
	{
        mat_[i][0] = v[i];
	}
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by ColumnVector constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid dimensions in ColumnVector constructor");
}


ColumnVector::ColumnVector(const ColumnVector& b)
		try	:   Matrix(b)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by ColumnVector constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid ColumnVector parameter error in ColumnVector constructor");
}
      


ColumnVector::~ColumnVector(void)
{
}

      
ColumnVector* ColumnVector::randomVector(const int nbRows, const double lowerBound, 
                                    const double upperBound)
{
    ColumnVector* v = new ColumnVector(nbRows);

    for (int i = 0; i < nbRows; i++)
        v->mat_[i][0] = RandomGenerator_C::nextDouble_st(upperBound, lowerBound);

    return v;
}


//------------------------------------------
//  addition/subtraction methods
//------------------------------------------
#if 0
#pragma mark -
#endif


//const ColumnVector uriVL::operator +(const ColumnVector& a, const ColumnVector& b) 
//{
//    const int   nbRows = a.getNbRows();
//                
//    FAIL_CONDITION( b.getNbRows() != nbRows,
//                    kMatrixIncorrectDimensionsError,
//                    "parameter has incorrect dimensions in ColumnVector::operator+");
//
//    double  *m = new double[nbRows];
//	double	const*const*const A = a.getMatrix();
//	double	const*const*const B = b.getMatrix();
//    for (int i=0; i<nbRows; i++)
//    {
//		m[i] = A[i][0] + B[i][0];
//    }
//
////jyh
////fix that for CV
//    MatrixIndicators matrixIndic = {IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN,
//                                    IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN};
//    
//    return ColumnVector(nbRows, m);
//}
//
//
//const ColumnVector uriVL::operator -(const ColumnVector& a, const ColumnVector& b) 
//{
//    const int   nbRows = a.getNbRows();
//                
//    FAIL_CONDITION( b.getNbRows() != nbRows,
//                    kMatrixIncorrectDimensionsError,
//                    "parameter has incorrect dimensions in ColumnVector::operator-");
//
//    double  *m = new double[nbRows];
//	double	const*const*const A = a.getMatrix();
//	double	const*const*const B = b.getMatrix();
//	
//    for (int i=0; i<nbRows; i++)
//    {
//		
//		m[i] = A[i][0] - B[i][0];
//    }
//
////jyh
////fix that for CV
//    MatrixIndicators matrixIndic = {IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN,
//                                    IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN, IS_UNKNOWN};
//    
//    return ColumnVector(nbRows, m);
//}



void ColumnVector::operator +=(const ColumnVector& b)
{
    FAIL_CONDITION( b.nbRows_ != nbRows_,
                    kMatrixIncorrectDimensionsError,
                    "parameter has incorrect dimensions in ColumnVector::operator +=");

    for (int i=0; i<nbRows_; i++)
        mat_[i][0] += b.mat_[i][0];
  
}



void ColumnVector::operator -=(const ColumnVector& b)
{
    FAIL_CONDITION( b.nbRows_ != nbRows_,
                    kMatrixIncorrectDimensionsError,
                    "parameter has incorrect dimensions in ColumnVector::subtractFrom");

    for (int i=0; i<nbRows_; i++)
        mat_[i][0] -= b.mat_[i][0];

}


#if 0
#pragma mark -
#endif


double ColumnVector::norm2(void) const 
{
    double sum = 0.L;

    for (int i = 0; i < nbRows_; i++)
        sum += mat_[i][0] * mat_[i][0];

    return sqrt(sum);
}


double ColumnVector::dotProd(const ColumnVector* b) const 
{
    FAIL_CONDITION(	nbRows_ != b->nbRows_,
    				kMatrixIncorrectDimensionsError,
    				"Vector dimensions don't match in ColumnVector::dotProd");
    double sum = 0.L;

    for (int i=0; i<nbRows_; i++)
        sum += mat_[i][0] * b->mat_[i][0];

    return sum;
}


Matrix* ColumnVector::transposition(void) const 
{
    RowVector *rowVect = new RowVector(nbRows_);

    for (int i=0; i<nbRows_; i++)
        rowVect->mat_[0][i] = mat_[i][0];

    return rowVect;
}


void ColumnVector::normalize1(void)
{
	if (isNormalized1_ != IS_TRUE)
	{
		double  normVal = norm1();
		if (normVal != 0)
		{
			double  normScale = 1.L / normVal;
			for (int i=0; i<nbRows_; i++)
				mat_[i][0] *= normScale;       
		}
		
		isNormalized1_ = IS_TRUE;
	}
}



void ColumnVector::normalize2(void)
{
	if (isNormalized2_ != IS_TRUE)
	{
		double  normVal = norm2();
		if (normVal != 0)
		{
			double  normScale = 1.L / normVal;
			for (int i=0; i<nbRows_; i++)
				mat_[i][0] *= normScale;       
		}
		if ((isNormalized1_ == IS_TRUE) && (fabs(normVal - 1.0) > STD_RELATIVE_TOLERANCE))
			isNormalized1_ = IS_FALSE;

		if ((isNormalizedInf_ == IS_TRUE) && (fabs(normVal - 1.0) > STD_RELATIVE_TOLERANCE))
			isNormalizedInf_ = IS_FALSE;

		isNormalized2_ = IS_TRUE;
	}
}

void ColumnVector::normalizeInf(void)
{
	if (isNormalizedInf_ != IS_TRUE)
	{
		double  normVal = normInf();
		if (normVal != 0)
		{
			double  normScale = 1.L / normVal;
			for (int i=0; i<nbRows_; i++)
				mat_[i][0] *= normScale;       
		}

		isNormalizedInf_ = IS_TRUE;
	}
}

