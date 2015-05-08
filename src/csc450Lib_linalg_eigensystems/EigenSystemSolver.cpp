//
//  EigenSystemSolver.cpp
//
//
//  Created by Christopher Glasz on 4/01/15.
//
//
//=================================
// included dependencies
#include "EigenSystemSolver.h"

using namespace csc450Lib_linalg_base;
using namespace csc450Lib_linalg_eigensystems;

EigenSystem* EigenSystemSolver::power(const Matrix *a,
                                      const ColumnVector *init,
                                      int iterations, float tol) {
    // make a copy of the initial vector
    ColumnVector *x = (ColumnVector*)Matrix::copyOf(init);
    
    int k = 1;
    int imax = 0;
    float s = 1;
    bool converged = false;
    
    // get an initial l and lastl
    const ColumnVector *y = (ColumnVector*)Matrix::multiply(a, x);
    float lambda = y->normInf();
    float lastlambda = lambda + 100;
    
    // loop until converged
    for (k = 1; k < iterations && !converged; k++) {
        y = (ColumnVector*)Matrix::multiply(a, x);
        lambda = y->normInf();
        if (abs(lambda - lastlambda) < tol)
            converged = true;
        lastlambda = lambda;
        imax = y->maxInd();
        
        // sign of eigenalue
        s = (y->get(imax) * x->get(imax)) < 0 ? -1 : 1;
        x = (ColumnVector*)Matrix::multiply(1.0 / lambda, y);
    }
    float val = s * lambda;
    
    Matrix *v = x;
    ColumnVector *l = new ColumnVector(1);
    l->set(0, val);
    return new EigenSystem(a, v, l);
}

const EigenSystem* EigenSystemSolver::rayleigh(const Matrix *a,
                                               const ColumnVector *v,
                                               float l) const {
    
}


EigenSystemSolver::EigenSystemSolver(void) {
    
}

EigenSystemSolver::EigenSystemSolver(const Matrix *a) {
    this->a = a;
}

EigenSystemSolver::~EigenSystemSolver(void) {
    
}

Matrix* EigenSystemSolver::deflate(const Matrix *a,
                                         const ColumnVector *v,
                                         float l) {
    float norm = v->norm2();
    ColumnVector *u = (ColumnVector*)Matrix::multiply(l/(norm * norm),
                                                      v);
    
    return Matrix::subtract(a, Matrix::multiply(v, Matrix::transpose(u)));
}

const EigenSystem* EigenSystemSolver::solve(const Matrix *a) {
    this->a = a;
    return solve();
}


const EigenSystem* EigenSystemSolver::solve(void) const {
    int size = a->rows();
    
    const Matrix *deflated = Matrix::copyOf(a);
    ColumnVector *vectors[size];
    float values[size];
    
    ColumnVector *init;
    
    EigenSystem *currentSystem;
    
    for (int i = 0; i < size; i++) {
        init = MatrixGenerator::getRandomColumn(size);
        
        currentSystem = power(deflated, init, 1000000, 0.0001);
        
        vectors[i] = currentSystem->getEigenVector(0);
        values[i] = currentSystem->getEigenValue(0);
        
        deflated = deflate(a, vectors[i], values[i]);
    }
    
    ColumnVector *l = new ColumnVector(size, values);
    
    Matrix *v = vectors[0];
    for (int i = 1; i < size; i++) {
        v->addColumn(vectors[i]);
    }
    
    const EigenSystem *system = new EigenSystem(a, v, l);
    return system;
}


void EigenSystemSolver::setA(const Matrix *a) { this->a = a; }