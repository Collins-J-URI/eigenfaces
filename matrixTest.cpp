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
#include "Subject.h"
#include "FacialRecognizer.h"
using namespace csc450Lib_calc_base;
using namespace csc450Lib_linalg_base;
using namespace csc450Lib_linalg_sle;
using namespace csc450Lib_linalg_eigensystems;

int main() {
    srand(time(NULL));
    string base = "/Users/Christopher/Desktop/CSC 450 Coursework/eigenfaces/";
    
    string facedir = base + "doc/facetext/";
    string file = "subject01.txt";
    string path = facedir + file;
    
    vector<string> files = vector<string>();
    GetPixels::getdir(facedir, files);
    
    files.erase(files.begin(),files.begin()+3);
    
    for (int i = 0; i < files.size(); i++) {
        files[i] = facedir + files[i];
    }
    
	//images per person collected
	int ipp = 11;
	int subjectid = 0;
	int count = 0;
	int imagewidth = 320;
	int imageheight = 243;
	//Create our Subjects
    const Subject *subjects[files.size() / ipp];
	
	Matrix* temp;
	//for every picture
    cout << "Creating subjects";
    int i = 0;
	while (i <= files.size() && subjectid < files.size() / ipp) {

		//if it is the first time running for this subject
		if (count == 0) {
			//then initialize the first column of their vector
			float ** imagePix = GetPixels::getPixelSquare(files[i]);
			Matrix* imageMat = new Matrix(imageheight, imageheight, imagePix);
			ColumnVector *imageVect = Matrix::column(imageMat);
			temp = Matrix::copyOf(imageVect);
            count++;
            i++;
		}
		else if (count >= ipp) { //else if we are done gathering data for the subject
			//officially create the subject, reset counters
			subjects[subjectid] = new Subject(Matrix::copyOf(temp), subjectid+1);
            //if(subjectid >=subjects.size())
			count = 0;
			subjectid++;

		}
		else {
			float ** imagePix = GetPixels::getPixelSquare(files[i]);
			Matrix* imageMat = new Matrix(imageheight, imageheight, imagePix);
			ColumnVector *imageVect = Matrix::column(imageMat);
			temp->addColumn((ColumnVector*)Matrix::copyOf(imageVect));
			count++;
            i++;
		}
        if (i % (files.size()/5) == 0) {
            cout << ".";
            cout.flush();
        }
	}
    delete temp;
    temp = 0;
    cout << " Done.\n";
    
	//Put our pictures in random order
    for (int i = 0; i < files.size(); i++) {
        int rand = std::rand() % files.size();
        string temp = files[i];
        files[i] = files[rand];
        files[rand] = temp;
    }
    
    float** pixels = GetPixels::getPixelSquare(path);

    Matrix* image = new Matrix(imageheight, imageheight, pixels);
    ColumnVector *imagecol = Matrix::column(image);
    
    /********************************************
     *              DECLARATIONS                *
     ********************************************/
    
    int numImages = files.size();
    numImages = 40;
    int imageWidth = 243;
    
    // Seed the random matrix generator
    MatrixGenerator::seed();
    
    Matrix *mat;
    
    cout << "Loading images";
    
    // Image vectors
    ColumnVector *gamma[numImages];
    for (int i = 0; i < numImages; i++) {
        mat = new Matrix(imageWidth, imageWidth,
                         GetPixels::getPixelSquare(files[i]));
        gamma[i] = Matrix::column(mat);
        if (i % (numImages/3)==0) {
            cout << ".";
            cout.flush();
        }
    }
    
    cout << " Done.\n";
    
    // Matrix of image vectors
    Matrix *gammas = gamma[0];
    for (int i = 1; i < numImages; i++) {
        gammas->addColumn(gamma[i]);
    }
    
    // The average face
    const ColumnVector *psi = gammas->averageColumn();
    
    cout << "Creating A";
    // Matrix of differences between image vectors and the average face
    Matrix *A = Matrix::subtract(gamma[0], psi);
    ColumnVector *phi[numImages];
    for (int i = 1; i < numImages; i++) {
        phi[i] = (ColumnVector*)Matrix::subtract(gamma[i], psi);
        A->addColumn(phi[i]);
        if (i % (numImages/3)==0) {
            cout << ".";
            cout.flush();
        }
    }
    cout << " Done.\n\n";
    
    Matrix *L = Matrix::multiply(Matrix::transpose(A), A);
    Matrix *deflated = Matrix::copyOf(L);
    ColumnVector *eigenvectors[numImages];
    ColumnVector *diffs[numImages];
    float eigenvalues[numImages];
    
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
    
    FacialRecognizer *recognizer = new FacialRecognizer(15,
                                                        subjects,
                                                        eigenfaces,
                                                        psi,
                                                        gammas->getColumn(0));
    const Subject *recognizedSubject = recognizer->faceClass();
    cout << "\n\n\nRECOGNIZED SUBJECT AS SUBJECT " << recognizedSubject->getID() << "\n\n\n";
    
    Matrix *diffmatrix = (ColumnVector*)Matrix::subtract(Matrix::multiply(L, system->getEigenVector(0)),Matrix::multiply(system->getEigenValue(0), system->getEigenVector(0)));
    for (int i = 1; i < numImages; i++) {
        diffs[i] = (ColumnVector*)Matrix::subtract(Matrix::multiply(L, system->getEigenVector(i)),Matrix::multiply(system->getEigenValue(i), system->getEigenVector(i)));
    }
    for (int i = 1; i < numImages; i++) {
        diffmatrix->addColumn(diffs[i]);
    }
    
    cout << "Calculations complete\n";
    
    /********************************************
     *          COMMAND LINE OUTPUT             *
     ********************************************/
    //cout << "\n\n";
    
    //cout << "L: \n";
    //cout << L->toString("",""," ",true);
    //cout << "\n\n";
    
    //cout << "Enter into Mathematica to obtain\n";
    //cout << "\t{{eigenvalue, ...}, {eigenvector, ...}}\n";
    //cout << "Eigensystem[";
    //cout << L->toString("{","}",",",false);
    //cout << "]//N\n\n";
    
    //cout << "eigenvalues: \n";
    //cout << eigenvaluevector->toString("{","}",",",false);
    //cout << "\n";
    //cout << system->getEigenValues()->toString("{","}",",",false);
    //cout << "\n\n";
    
    //cout << "eigenvectors: \n";
    //cout << eigenvectormatrix->toString("{","}",",",false);
    //cout << "\n";
    //cout << system->getEigenVectors()->toString("{","}",",",false);
    //cout << "\n\n";
    
    cout << "Norm of difference:: \n";
    cout << diffmatrix->normInf();
    cout << "\n\n";
    
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
        cout << "Saving output/eigenface" + to_string(i) + ".txt\n";
        myfile.open ("output/eigenface" + to_string(i) + ".txt");
        
        current = Matrix::matrix(eigenfaces->getColumn(i), imageWidth);
        myfile << current->toString("{","}",",",false);
        
        // Close file
        myfile.close();
    }
    for (int i = 0; i < numImages; i++) {
        cout << "Saving output/face" + to_string(i) + ".txt\n";
        myfile.open ("output/face" + to_string(i) + ".txt");
        
        current = Matrix::matrix(gammas->getColumn(i), imageWidth);
        myfile << current->toString("{","}",",",false);
        
        // Close file
        myfile.close();
    }
}