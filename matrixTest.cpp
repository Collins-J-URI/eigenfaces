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
    
    // Seed the random matrix generator
    MatrixGenerator::seed();
    
    Matrix *mat;
    ColumnVector *cols[5];
    for (int i = 0; i < 5; i++) {
        mat = MatrixGenerator::getRandom(8,8);
        cols[i] = Matrix::column(mat);
    }
    
    /********************************************
     *              COMPUTATION                 *
     ********************************************/
    
    Matrix *A = cols[0];
    for (int i = 1; i < 5; i++) {
        A->addColumn(cols[i]);
    }
    
    const Matrix *mattmat = Matrix::multiply(Matrix::transpose(mat), mat);
    const ColumnVector* init = MatrixGenerator::getRandomColumn(8);
    
    ColumnVector *eigenvector = mattmat->eigenvector(init, 10000, 0.01);
    float eigenvalue = mattmat->eigenvalue(init, 10000, 0.01);
    
    ColumnVector *diff = (ColumnVector*)Matrix::subtract(Matrix::multiply(mattmat, eigenvector),ColumnVector::multiply(eigenvalue, eigenvector));
    
    /********************************************
     *          COMMAND LINE OUTPUT             *
     ********************************************/
    
    cout << "A: \n";
    cout << A->toString("",""," ",true);
    cout << "\n\n";
    
    cout << "ave col: \n";
    cout << A->averageColumn()->toString("",""," ",true);
    cout << "\n\n";
    
    cout << "mat: \n";
    cout << mat->toString("",""," ",true);
    cout << "\n\n";
    
    cout << "mattmat: \n";
    cout << mattmat->toString("",""," ",true);
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
    cout << mattmat->toString("{","}",",",false);
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