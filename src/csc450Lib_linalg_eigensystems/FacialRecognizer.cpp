//
//  FacialRecognizer.cpp
//
//
//  Created by Christopher Glasz on 5/11/15.
//
//


//=================================
// included dependencies
#include "FacialRecognizer.h"

using namespace csc450Lib_linalg_base;
using namespace csc450Lib_linalg_eigensystems;


FacialRecognizer::FacialRecognizer(void) {}

FacialRecognizer::FacialRecognizer(int numFaceClasses,
                                   const Subject *faceclasses[],
                                   const Matrix *eigenfaces,
                                   const ColumnVector *averageFace) {
    this->numFaceClasses = numFaceClasses;
    this->faceclasses = faceclasses;
    this->eigenfaces = eigenfaces;
    this->averageFace = averageFace;
}

FacialRecognizer::FacialRecognizer(int numFaceClasses,
                                   const Subject *faceclasses[],
                                   const Matrix *eigenfaces,
                                   const ColumnVector *averageFace,
                                   const ColumnVector *input) {
    this->numFaceClasses = numFaceClasses;
    this->faceclasses = faceclasses;
    this->eigenfaces = eigenfaces;
    this->averageFace = averageFace;
    this->input = (ColumnVector*)Matrix::copyOf(input);
}

FacialRecognizer::~FacialRecognizer(void) {
    
}

float FacialRecognizer::distFromFaceSpace() const {
    ColumnVector *phi = (ColumnVector*)Matrix::subtract(input, averageFace);
    ColumnVector *weights = getWeights(input);
    ColumnVector *phif = (ColumnVector*)Matrix::multiply(weights->get(0), eigenfaces->getColumn(0));
    for (int i = 1; i < eigenfaces->cols(); i++) {
        phif = (ColumnVector*)Matrix::add(phif, Matrix::multiply(weights->get(i), eigenfaces->getColumn(i)));
    }
    return ((ColumnVector*)Matrix::subtract(phi, phif))->norm2();
}

float FacialRecognizer::distFromFaceClass(const Subject* subject) const {
    ColumnVector *weights = getWeights(input);
    ColumnVector *classVector = (ColumnVector*)Matrix::copyOf(subject->calculateClassVector(eigenfaces, averageFace));
    return ((ColumnVector*)Matrix::subtract(weights,classVector))->norm2();
}


ColumnVector* FacialRecognizer::getWeights(const ColumnVector *input) const {
    ColumnVector* weights = new ColumnVector(eigenfaces->cols());
    for (int i = 0; i < eigenfaces->cols(); i++) {
        weights->set(i,Matrix::multiply(Matrix::transpose(eigenfaces->getColumn(i)),
                                        Matrix::subtract(input, averageFace))->get(0,0));
    }
    return weights;
}

bool FacialRecognizer::nearFaceSpace(float tol) const {
    return distFromFaceSpace() < tol;
    
}

bool FacialRecognizer::nearFaceSpace(const ColumnVector *input, float tol) {
    this->input = (ColumnVector*)Matrix::copyOf(input);
    return nearFaceSpace(tol);
}

bool FacialRecognizer::nearFaceClass(float tol) const {
    float dist = distFromFaceClass(faceclasses[0]);
    float currentdist;
    for (int i = 0; i < numFaceClasses; i++) {
        currentdist = distFromFaceClass(faceclasses[i]);
        if (currentdist < dist)
            dist = currentdist;
    }
    return dist < tol;
}

bool FacialRecognizer::nearFaceClass(const ColumnVector *input, float tol) {
    this->input = (ColumnVector*)Matrix::copyOf(input);
    return nearFaceClass(tol);
}

const Subject* FacialRecognizer::faceClass(void) const {
    int retind = 0;
    float dist = distFromFaceClass(faceclasses[0]);
    float currentdist;
    for (int i = 1; i < numFaceClasses; i++) {
        currentdist = distFromFaceClass(faceclasses[i]);
        if (currentdist < dist) {
            retind = i;
            dist = currentdist;
        }
    }
    return faceclasses[retind];
}

const Subject* FacialRecognizer::faceClass(const ColumnVector *input) {
    this->input = (ColumnVector*)Matrix::copyOf(input);
    return faceClass();
    
}
