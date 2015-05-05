//
//  ColumnVector.cpp
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

#include "ColumnVector.h"
#include "RowVector.h"

using namespace csc450Lib_linalg_base;

ColumnVector::ColumnVector(int nbRows, const float * v) : Matrix(nbRows, 1) {
    for (int i = 0; i < nbRows; i++)
        this->a[i][0] = v[i];
}

ColumnVector::ColumnVector(int nbRows) : Matrix(nbRows, 1) {
}

float ColumnVector::get(int theRow) const {
    return a[theRow][0];
}

float ColumnVector::max() const {
    float max = a[0][0];
    for (int i = 1; i< nbRows; i++)
        if (a[i][0] > max)
            max = a[i][0];
    return max;
}

int ColumnVector::maxInd() const {
    int max = 0;
    for (int i = 1; i< nbRows; i++)
        if (a[i][0] > a[max][0])
            max = i;
    return max;
}

void ColumnVector::set(int theRow, float theVal) const {
    a[theRow][0] = theVal;
}

const RowVector* ColumnVector::transpose(const ColumnVector *matA) {
    RowVector* t = new RowVector(matA->rows());
    
    for (int i = 0; i < matA->rows(); i++) {
        t->set(i, matA->get(i));
    }
    
    return t;
}

ColumnVector* ColumnVector::copyOf(const ColumnVector *matA) {
    ColumnVector *copy = new ColumnVector(matA->rows());
    for (int i = 0; i < matA->rows(); i++) {
        copy->set(i, matA->get(i));
    }
    return copy;
}

 ColumnVector* ColumnVector::multiply(float mult,
                                           const ColumnVector *mat) {
    ColumnVector * prod = new ColumnVector(mat->rows());
    for (int i = 0; i < mat->rows(); i++) {
        prod->set(i, mult * mat->get(i));
        
    }
    return prod;
}
