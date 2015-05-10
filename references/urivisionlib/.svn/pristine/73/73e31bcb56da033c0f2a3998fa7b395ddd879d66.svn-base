/*  NAME:
        RowVector.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RowVector class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColumnVector.h"
#include "RowVector.h"
#include "RandomGenerator_C.h"

using namespace uriVL;

RowVector::RowVector(int nbCols)
    :   Matrix(1, nbCols)
{
}


RowVector::RowVector(int nbCols, double v)
    :   Matrix(1, nbCols, v)
{
}


RowVector::RowVector(int nbCols, const double* const v)
    :   Matrix(1, nbCols)
{
    for (int j=0; j<nbCols_; j++)
        mat_[0][j] = v[j];
}


RowVector::RowVector(const RowVector* b)
:   Matrix(b)
{
    isNormalized1_ = b->isNormalized1_;
    isNormalized2_ = b->isNormalized2_;
    isNormalizedInf_ = b->isNormalizedInf_;
}
      
RowVector::~RowVector(void)
{
}
      
RowVector *RowVector::randomVector(int nbCols, double lowerBound, double upperBound)
{
    RowVector *v = new RowVector(nbCols);

    for (int j = 0; j < nbCols; j++)
        v->mat_[0][j] = RandomGenerator_C::nextDouble_st(upperBound, lowerBound);

    return v;
}

//void RowVector::resize(int newNbCols)
//{
//    Matrix::resize(1, newNbCols);
//}
//


double RowVector::norm2(void) const 
{
    double sum = 0.L;

    for (int j=0; j<nbCols_; j++)
        sum += mat_[0][j] * mat_[0][j];

    return sqrt(sum);
}


//double RowVector::dotProd(const RowVector* b) const 
//{
//    double sum = 0.L;
//
//    for (int j=0; j<nbCols_; j++)
//        sum += mat_[0][j] * b->mat_[0][j];
//
//    return sum;
//}
//

Matrix* RowVector::transposition(void) const 
{
    ColumnVector *colVect = new ColumnVector(nbCols_);

    for (int j=0; j<nbCols_; j++)
        colVect->mat_[j][0] = mat_[0][j];

    return colVect;
}


void RowVector::normalize1(void)
{
    double  normVal = norm1();
    if (normVal != 0)
    {
        double  normScale = 1.L / normVal;
        for (int j=0; j<nbCols_; j++)
            mat_[0][j] *= normScale;       
    }
}


void RowVector::normalize2(void)
{
    double  normVal = norm2();
    if (normVal != 0)
    {
        double  normScale = 1.L / normVal;
        for (int j=0; j<nbCols_; j++)
            mat_[0][j] *= normScale;       
    }
}

void RowVector::normalizeInf(void)
{
    double  normVal = normInf();
    if (normVal != 0)
    {
        double  normScale = 1.L / normVal;
        for (int j=0; j<nbCols_; j++)
            mat_[0][j] *= normScale;       
    }
}
