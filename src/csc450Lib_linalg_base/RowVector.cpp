//
//  RowVector.cpp
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

#include "RowVector.h"
#include "ColumnVector.h"

using namespace csc450Lib_linalg_base;

RowVector::RowVector(int nbCols, const float * v) : Matrix(1, nbCols) {
    for (int i = 0; i < nbCols; i++)
        this->a[0][i] = v[i];
}

RowVector::RowVector(int nbCols) : Matrix(1, nbCols) {
}

float RowVector::get(int theCol) const {
    return a[0][theCol];
}

void RowVector::set(int theCol, float theVal) const {
    a[0][theCol] = theVal;
}

const ColumnVector* RowVector::transpose(const RowVector *matA) {
    ColumnVector* t = new ColumnVector(matA->cols());
    
    for (int i = 0; i < matA->cols(); i++) {
        t->set(i, matA->get(i));
    }
    
    return t;
}