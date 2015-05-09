//
//  EigenSystem.cpp
//
//
//  Created by Christopher Glasz on 5/6/15.
//
//

//=================================
// included dependencies
#include "EigenSystem.h"

using namespace csc450Lib_linalg_base;
using namespace csc450Lib_linalg_eigensystems;

EigenSystem::EigenSystem() {
}

EigenSystem::EigenSystem(const Matrix *a, const Matrix *v, const ColumnVector *l) {
    this->a = Matrix::copyOf(a);
    this->l = (ColumnVector*)Matrix::copyOf(l);
    this->v = Matrix::copyOf(v);
}

Matrix* EigenSystem::getEigenVectors(void) const { return v; }

ColumnVector* EigenSystem::getEigenValues(void) const { return l; }

ColumnVector* EigenSystem::getEigenVector(int index) const {
    return v->getColumn(index);
}

float EigenSystem::getEigenValue(int index) const {
    return l->get(index);
}
