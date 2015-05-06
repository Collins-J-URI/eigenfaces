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
    
    int numImages = 4;
    int imageWidth = 40;
    
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
    
    Matrix *L = Matrix::multiply(Matrix::transpose(A), A);
    Matrix *deflated = Matrix::copyOf(L);
    ColumnVector *eigenvectors[numImages];
    ColumnVector *diffs[numImages];
    float eigenvalues[numImages];
    
    /********************************************
     *              COMPUTATION                 *
     ********************************************/
    
    ColumnVector* init = MatrixGenerator::getRandomColumn(numImages);
    
    ColumnVector *eigenvector = deflated->eigenvector(init, 1000000, 0.0001);
    float eigenvalue = deflated->eigenvalue(init, 1000000, 0.0001);
    
    //ColumnVector *diff = (ColumnVector*)Matrix::subtract(Matrix::multiply(L, eigenvector),ColumnVector::multiply(eigenvalue, eigenvector));
    
    //Matrix *delatedL = Matrix::deflate(L, eigenvector, eigenvalue);
    
    //ColumnVector *eigenvector2 = delatedL->eigenvector(init, 1000000000, 0.001);
    //float eigenvalue2 = delatedL->eigenvalue(init, 1000000000, 0.001);
    
    //ColumnVector *diff2 = (ColumnVector*)Matrix::subtract(Matrix::multiply(delatedL, eigenvector2),ColumnVector::multiply(eigenvalue2, eigenvector2));
    
    for (int i = 0; i < numImages; i++) {
        cout << "Calculating eigenvector " << i << "\n";
        eigenvectors[i] = deflated->eigenvector(init, 1000000, 0.0001);
        cout << "Calculating eigenvalue " << i << "\n";
        eigenvalues[i] = deflated->eigenvalue(init, 1000000, 0.0001);
        
        diffs[i] = (ColumnVector*)Matrix::subtract(Matrix::multiply(deflated, eigenvectors[i]),Matrix::multiply(eigenvalues[i], eigenvectors[i]));
        
        deflated = Matrix::deflate(deflated, eigenvectors[i], eigenvalues[i]);
        init = MatrixGenerator::getRandomColumn(numImages);
    }
    
    ColumnVector *eigenvaluevector = new ColumnVector(numImages, eigenvalues);
    Matrix *eigenvectormatrix = eigenvectors[0];
    Matrix *diffmatrix = diffs[0];
    for (int i = 1; i < numImages; i++) {
        eigenvectormatrix->addColumn(eigenvectors[i]);
        diffmatrix->addColumn(diffs[i]);
    }
    
    /********************************************
     *          COMMAND LINE OUTPUT             *
     ********************************************/
    cout << "\n\n";
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
    
    /*
    cout << "Verification :\n";
    cout << diff->toString("",""," ",true);
    cout << "\n\n";
    
    cout << "Verification :\n";
    cout << diff2->toString("",""," ",true);
    cout << "\n\n";
     */
    
    cout << "eigenvalues: \n";
    cout << eigenvaluevector->toString("{","}",",",false);
    cout << "\n\n";
    
    cout << "eigenvectors: \n";
    cout << eigenvectormatrix->toString("{","}",",",false);
    cout << "\n\n";
    
    cout << "diffs: \n";
    cout << diffmatrix->toString("",""," ",true);
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