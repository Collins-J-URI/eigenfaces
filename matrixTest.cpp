#include <iostream>
#include <fstream>
#include <unistd.h>
#include <tgmath.h>
#include "Matrix.h"
#include "ColumnVector.h"
#include "LinearSolver.h"
#include "LinearSolver_LU.h"
#include "PolyFunction1D.h"
#include "MatrixGenerator.h"
using namespace csc450Lib_calc_base;
using namespace csc450Lib_linalg_base;
using namespace csc450Lib_linalg_sle;

int main() {
    
    /********************************************
     *              DECLARATIONS                *
     ********************************************/
    
    int numImages = 16;
    int imageWidth = 256;
    
    // Seed the random matrix generator
    MatrixGenerator::seed();
    
    Matrix *mat;
    
    // Image vectors
    ColumnVector *gamma[numImages];
    for (int i = 0; i < numImages; i++) {
        mat = MatrixGenerator::getRandom(imageWidth,imageWidth);
        gamma[i] = Matrix::column(mat);
    }
    
    // Matrix of image vectors
    Matrix *gammas = gamma[0];
    for (int i = 1; i < numImages; i++) {
        gammas->addColumn(gamma[i]);
    }
    
    
    // The average face
    const ColumnVector *psi = gammas->averageColumn();
    
    cout << gamma[0]->rows() << ", " << psi->rows() << "\n\n";
    
    // Matrix of differences between image vectors and the average face
    Matrix *A = Matrix::subtract(gamma[0], psi);
    ColumnVector *phi[numImages];
    for (int i = 1; i < numImages; i++) {
        phi[i] = (ColumnVector*)Matrix::subtract(gamma[i], psi);
        A->addColumn(phi[i]);
    }
    
    cout << A->rows() << ", " << A->cols() << "\n\n";
    Matrix *L = Matrix::multiply(Matrix::transpose(A), A);
    cout << L->rows() << ", " << L->cols() << "\n\n";
    
    /********************************************
     *              COMPUTATION                 *
     ********************************************/
    
    const ColumnVector* init = MatrixGenerator::getRandomColumn(numImages);
    
    ColumnVector *eigenvector = L->eigenvector(init, 1000000000, 0.001);
    float eigenvalue = L->eigenvalue(init, 1000000000, 0.001);
    
    ColumnVector *diff = (ColumnVector*)Matrix::subtract(Matrix::multiply(L, eigenvector),ColumnVector::multiply(eigenvalue, eigenvector));
    
    /********************************************
     *          COMMAND LINE OUTPUT             *
     ********************************************/
    
    //cout << "Gammas: \n";
    //cout << gammas->toString("",""," ",true);
    //cout << "\n\n";
    
    //cout << "Psi: \n";
    //cout << psi->toString("",""," ",true);
    //cout << "\n\n";
    
    //cout << "A: \n";
    //cout << A->toString("",""," ",true);
    //cout << "\n\n";
    
    cout << "L: \n";
    cout << L->toString("",""," ",true);
    cout << "\n\n";
    
    cout << "eigenvalue: \n";
    cout << eigenvalue;
    cout << "\n\n";
    
    cout << "eigenvector: \n";
    cout << eigenvector->toString("{","}",",",false);
    cout << "\n\n";
    
    cout << "Enter into Mathematica to obtain\n";
    cout << "\t{{eigenvalue, ...}, {eigenvector, ...}}\n";
    cout << "Eigensystem[";
    cout << L->toString("{","}",",",false);
    cout << "]//N\n\n";
    
    cout << "Verification :\n";
    cout << diff->toString("",""," ",true);
    cout << "\n\n";
    
    /********************************************
     *              WRITE TO FILE               *
     ********************************************/
    /*
    ofstream myfile;
    myfile.open ("output/output.txt");
    
    myfile << len << '\n';
    
    for (int i = 0; i < len; i++) {
        myfile << xvals->get(i) << ' ' << b->get(i) << '\n';
    }
    
    // Display a little more than the lefmost and rightmost values
    int numpts = (int)((max - min) / 0.001);
    
    // Write number of points
    myfile << numpts << '\n';
    float xval = min;
    
    // Write each coordinate
    for (int i = 0; i < numpts; i++) {
        myfile << xval << ' ' << f1->func(xval) << '\n';
        xval+=0.001;
    }
    
    // Write number of points
    myfile << numpts << '\n';
    xval = min;
    
    // Write each coordinate
    for (int i = 0; i < numpts; i++) {
        myfile << xval << ' ' << f2->func(xval) << '\n';
        xval+=0.001;
    }
    
    // Write number of points
    myfile << numpts << '\n';
    xval = min;
    
    // Write each coordinate
    for (int i = 0; i < numpts; i++) {
        myfile << xval << ' ' << f3->func(xval) << '\n';
        xval+=0.001;
    }
    
    // Close file
    myfile.close();
    */
}