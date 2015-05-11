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
#include "EigenSystem.h"
#include "EigenSystemSolver.h"
#include "GetPixels.h"
#include <regex>
using namespace csc450Lib_calc_base;
using namespace csc450Lib_linalg_base;
using namespace csc450Lib_linalg_sle;
using namespace csc450Lib_linalg_eigensystems;

int main() {
    srand ( unsigned ( std::time(0) ) );
    string base = "/Users/Christopher/Desktop/CSC 450 Coursework/eigenfaces/";
    
    string facedir = base + "doc/facetext/";
    string file = "subject01.txt";
    string path = facedir + file;
    
    vector<string> files = vector<string>();
    GetPixels::getdir(facedir, files);
    
    files.erase(files.begin(),files.begin()+3);
    
    for (int i = 0; i < files.size(); i++) {
        files[i] = facedir + files[i];
        cout << files[i] << "\n";
    }
    
    random_shuffle(files.begin(), files.end());
    
    float** pixels = GetPixels::getPixelSquare(path);
    int imagewidth = 320;
    int imageheight = 243;
    Matrix* image = new Matrix(imageheight, imageheight, pixels);
    ColumnVector *imagecol = Matrix::column(image);
    
    /********************************************
     *              DECLARATIONS                *
     ********************************************/
    
    int numImages = files.size();
    numImages = 20;
    int imageWidth = 243;
    
    // Seed the random matrix generator
    MatrixGenerator::seed();
    
    Matrix *mat;
    
    // Image vectors
    ColumnVector *gamma[numImages];
    for (int i = 0; i < numImages; i++) {
        cout << "Adding Gamma " << i << "\n";
        mat = MatrixGenerator::getRandom(imageWidth,imageWidth);
        mat = new Matrix(imageWidth, imageWidth,
                         GetPixels::getPixelSquare(files[i]));
        gamma[i] = Matrix::column(mat);
    }
    
    cout << "Done loading images\n";
    
    // Matrix of image vectors
    Matrix *gammas = gamma[0];
    for (int i = 1; i < numImages; i++) {
        gammas->addColumn(gamma[i]);
    }
    
    // The average face
    const ColumnVector *psi = gammas->averageColumn();
    cout << "Got average face\n";
    
    // Matrix of differences between image vectors and the average face
    Matrix *A = Matrix::subtract(gamma[0], psi);
    ColumnVector *phi[numImages];
    for (int i = 1; i < numImages; i++) {
        phi[i] = (ColumnVector*)Matrix::subtract(gamma[i], psi);
        A->addColumn(phi[i]);
    }
    
    cout << "A created\n";
    
    cout << "A size: " << A->rows() << "x" << A->cols() << "\n";
    
    Matrix *L = Matrix::multiply(Matrix::transpose(A), A);
    Matrix *deflated = Matrix::copyOf(L);
    ColumnVector *eigenvectors[numImages];
    ColumnVector *diffs[numImages];
    float eigenvalues[numImages];
    
    cout << "L size: " << L->rows() << "x" << L->cols() << "\n";
    cout << "Done with declarations\n";
    
    /********************************************
     *              COMPUTATION                 *
     ********************************************/
    
    
    EigenSystemSolver *solver = new EigenSystemSolver(L);
    const EigenSystem *system = solver->solve();
    
    cout << "Eigensystem solved\n";
    
    ColumnVector *faces[numImages];
    for (int l = 0; l < numImages; l++) {
        faces[l] = new ColumnVector(imageWidth*imageWidth);
        for (int j = 0; j < imageWidth*imageWidth; j++) {
            faces[l]->set(j, 0);
        }
        for (int k = 0; k < numImages; k++) {
            faces[l] = (ColumnVector*)Matrix::add(faces[l],Matrix::multiply(system->getEigenVector(l)->get(k), A->getColumn(k)));
        }
    }
    
    Matrix *eigenfaces = faces[0];
    for (int i = 1; i < numImages; i++) {
        eigenfaces->addColumn(faces[i]);
    }
    
    cout << "Eigenfaces calculated\n";
    
    Matrix *diffmatrix = (ColumnVector*)Matrix::subtract(Matrix::multiply(L, system->getEigenVector(0)),Matrix::multiply(system->getEigenValue(0), system->getEigenVector(0)));
    for (int i = 1; i < numImages; i++) {
        diffs[i] = (ColumnVector*)Matrix::subtract(Matrix::multiply(L, system->getEigenVector(i)),Matrix::multiply(system->getEigenValue(i), system->getEigenVector(i)));
    }
    for (int i = 1; i < numImages; i++) {
        diffmatrix->addColumn(diffs[i]);
    }
    
    
    Matrix *first = Matrix::matrix(eigenfaces->getColumn(0),243);
    
    
    
    cout << "Calculations complete\n";
    
    /********************************************
     *          COMMAND LINE OUTPUT             *
     ********************************************/
    cout << "\n\n";
    
    cout << "L: \n";
    cout << L->toString("",""," ",true);
    cout << "\n\n";
    
    cout << "Enter into Mathematica to obtain\n";
    cout << "\t{{eigenvalue, ...}, {eigenvector, ...}}\n";
    cout << "Eigensystem[";
    cout << L->toString("{","}",",",false);
    cout << "]//N\n\n";
    
    //cout << "eigenvalues: \n";
    //cout << eigenvaluevector->toString("{","}",",",false);
    //cout << "\n";
    cout << system->getEigenValues()->toString("{","}",",",false);
    cout << "\n\n";
    
    //cout << "eigenvectors: \n";
    //cout << eigenvectormatrix->toString("{","}",",",false);
    //cout << "\n";
    cout << system->getEigenVectors()->toString("{","}",",",false);
    cout << "\n\n";
    
    cout << "diffs: \n";
    cout << diffmatrix->toString("",""," ",true);
    cout << "\n\n";
    
    //cout << "First eigenface: \n";
    //cout << first->toString("{","}",",",false);
    //cout << "\n\n";
    
    //cout << "EIGENFACES: \n";
    //cout << eigenfaces->toString("",""," ",true);
    //cout << "\n\n";
    
    /********************************************
     *              WRITE TO FILE               *
     ********************************************/
    
    ofstream myfile;
    int mprime = 10;
    Matrix *current;
    for (int i = 0; i < mprime; i++) {
        myfile.open ("output/eigenface" + to_string(i) + ".txt");
        
        current = Matrix::matrix(eigenfaces->getColumn(i), imageWidth);
        myfile << current->toString("{","}",",",false);
        
        // Close file
        myfile.close();
    }
}