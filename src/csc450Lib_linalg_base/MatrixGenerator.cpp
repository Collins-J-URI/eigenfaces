//
//  MatrixGenerator.cpp
//
//
//  Created by Christopher Glasz on 3/29/15.
//
//

//=================================
// included dependencies
#include "MatrixGenerator.h"
using namespace csc450Lib_linalg_base;

/**
 * creates an identity matrix with n columns and rows
 * @param n the number of rows and columns in identity matrix
 * @return an identity matrix with n rows and n columns
 */
Matrix* MatrixGenerator::getIdentity(int n){
    float **element = new float*[n];
    for (int i=0; i<n; i++){
        element[i] = new float[n];
        element[i][i] = 1;
    }
    return new Matrix(n,n,element);
}


Matrix* MatrixGenerator::getRandom(int m, int n){
    float **element = new float*[n];
    for(int i=0; i<m; i++){
        element[i] = new float[n];
        for(int j=0; j<n; j++){
            element[i][j] = (rand() % 1000) / 1000.0f;
        }
    }
    return new Matrix(m,n,element);
}


Matrix* MatrixGenerator::getRandomSymmetric(int n){
    float **element = new float*[n];
    for(int i=0; i<n; i++){
        element[i] = new float[n];
        for(int j=i; j<n; j++){
            element[i][j] = (rand() % 1000) / 1000.0f;
            if(i!=j){
                element[j][i] = element[i][j];
            }
        }
    }
    return new Matrix(n,n,element);
}


Matrix* MatrixGenerator::getRandomUpperDiagonal(int n){
    float **element = new float*[n];
    for(int i=0; i<n; i++){
        element[i] = new float[n];
        for(int j=i; j<n; j++){
            element[i][j] = (rand() % 1000) / 1000.0f;
            if(i!=j){
                element[j][i] = 0;
            }
        }
    }
    return new Matrix(n,n,element);
}

Matrix* MatrixGenerator::getRandomLowerDiagonal(int n){
    float **element = new float*[n];
    for(int i=0; i<n; i++){
        element[i] = new float[n];
        for(int j=0; j<=i; j++){
            element[i][j] = (rand() % 1000) / 1000.0f;
        }
    }
    return new Matrix(n,n,element);
}

Matrix* MatrixGenerator::getRandomLowerUnitDiagonal(int n){
    float **element = new float*[n];
    for(int i=0; i<n; i++){
        element[i] = new float[n];
        for(int j=0; j<=i; j++){
            if(i!=j){
                element[i][j] = (rand() % 1000) / 1000.0f;
            }
            else{
                element[i][j] = 1;
            }
        }
    }
    return new Matrix(n,n,element);
}



Matrix* MatrixGenerator::getSquareDiagonal(int n, float *d){
    float **element = new float*[n];
    for(int i=0; i<n; i++){
        element[i] = new float[n];
        for(int j=0; j<n; j++){
            if(i==j){
                element[i][j] = d[i];
            }
            else{
                element[i][j] = 0;
            }
        }
    }
    return new Matrix(n,n,element);
}


Matrix* MatrixGenerator::getRandomHessenberg(int n){
    Matrix *A = MatrixGenerator::getRandomUpperDiagonal(n);
    float **a = A->getArray();
    for(int i=1; i<n; i++){
        a[i][i-1] = (rand() % 1000) / 1000.0f;
    }
    return new Matrix(n,n,a);
    
}

Matrix* MatrixGenerator::getHilbert(int n){
    float **h = new float*[n];
    for(int i=0; i<n; i++){
        h[i] = new float[n];
        for(int j=0; j<n; j++){
            h[i][j] = 1.0f/(i+j+1.0f);
        }
    }
    return new Matrix(n,n,h);
}

Matrix* MatrixGenerator::getPolynomial(int n, float *xs) {
    float **element = new float*[n];
    for(int i=0; i<n; i++){
        element[i] = new float[n];
        for(int j=0; j<n; j++){
            element[i][j] = pow(xs[i],j);
        }
    }
    return new Matrix(n,n,element);
}

Matrix* MatrixGenerator::getPolynomial(int n, ColumnVector *xs) {
    float **element = new float*[n];
    for(int i=0; i<n; i++){
        element[i] = new float[n];
        for(int j=0; j<n; j++){
            element[i][j] = pow(xs->get(i),j);
        }
    }
    return new Matrix(n,n,element);
}

Matrix* MatrixGenerator::getRandomPolynomial(int n, float L) {
    
    float *xs = new float[n];
    for (int i = 0; i < n; i++) {
        xs[i] = L * i / (float)n;
    }
    
    float **element = new float*[n];
    for(int i=0; i<n; i++){
        element[i] = new float[n];
        for(int j=0; j<n; j++){
            element[i][j] = pow(xs[i],j);
        }
    }
    return new Matrix(n,n,element);
}

Matrix* MatrixGenerator::getTrigonometric(int n, float L) {
    float *xs = new float[2*n + 1];
    for (int j = 0; j <= 2 * n; j++) {
        xs[j] = (float)(j * L) / (float)(2 * n + 1);
    }
    
    float **element = new float*[2 * n + 1];
    for (int i = 0; i <= 2 * n; i++) {
        element[i] = new float[2 * n + 1];
        element[i][0] = 1;
        for (int k = 1; k <= n; k++) {
           element[i][k] = cos(2 * k * M_PI * xs[i] / L);
        }
        for (int k = 1; k <= n; k++) {
           element[i][n + k] = sin(2 * k * M_PI * xs[i] / L);
        }
    }
    return new Matrix(2 * n + 1, 2 * n + 1, element);
}

ColumnVector* MatrixGenerator::getUniformSample(int n, float L) {
    int numPairs = 2*n +1;
    float *element = new float[numPairs];
    for (int j = 0; j < numPairs; j++) {
        element[j] = (float)(j * L) / (float)(2 * n + 1);
    }
    return new ColumnVector(numPairs,element);
}



ColumnVector* MatrixGenerator::getRandomColumn(int m) {
    
    float *element = new float[m];
    for(int i=0; i<m; i++){
        element[i] = (rand() % 1000) / 1000.0f;
    }
    return new ColumnVector(m,element);
}

RowVector* MatrixGenerator::getRandomRow(int n) {
    
    float *element = new float[n];
    for(int i=0; i<n; i++){
        element[i] = (rand() % 1000) / 1000.0f;
    }
    return new RowVector(n,element);
}

void MatrixGenerator::seed() {
    srand(time(NULL));
}
