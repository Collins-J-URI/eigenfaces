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

EigenSystem::EigenSystem(const Matrix *A) {
    this->A = Matrix::copyOf(A);
    
    this->size = A->rows();
    
    Matrix *deflated = Matrix::copyOf(A);
    ColumnVector *vectors[size];
    float values[size];
    
    ColumnVector *init;
    
    for (int i = 0; i < size; i++) {
        init = MatrixGenerator::getRandomColumn(size);
        
        vectors[i] = deflated->eigenvector(init, 1000000, 0.0001);
        values[i] = deflated->eigenvalue(init, 1000000, 0.0001);
        
        deflated = Matrix::deflate(deflated, vectors[i], values[i]);
    }
    
    this->eigenvalues = new ColumnVector(size, values);
    
    this->eigenvectors = vectors[0];
    for (int i = 1; i < size; i++) {
        eigenvectors->addColumn(vectors[i]);
    }
}

Matrix* EigenSystem::getEigenVectors(void) const { return eigenvectors; }

ColumnVector* EigenSystem::getEigenValues(void) const { return eigenvalues; }

ColumnVector* EigenSystem::getEigenVector(int index) const {
    return eigenvectors->getColumn(index);
}

float EigenSystem::getEigenValue(int index) const {
    return eigenvalues->get(index);
}
