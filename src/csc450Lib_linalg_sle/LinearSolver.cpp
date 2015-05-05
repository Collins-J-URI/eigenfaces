//
//  LinearSolver.cpp
//
//
//  Created by Christopher Glasz on 3/29/15.
//
//

#include "LinearSolver.h"
using namespace csc450Lib_linalg_base;
using namespace csc450Lib_linalg_sle;

void LinearSolver::setMatrix(const Matrix *a) {
    this->a = a;
}

void LinearSolver::setRightSideTerm(const ColumnVector *b) {
    this->b = b;
}

void LinearSolver::setSLE(const Matrix *a, const ColumnVector *b) {
    this->a = a;
    this->b = b;
}