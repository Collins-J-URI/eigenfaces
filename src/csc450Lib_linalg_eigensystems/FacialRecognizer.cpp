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

FacialRecognizer::FacialRecognizer(const Matrix *faceclasses,
                 const Matrix *eigenfaces,
                 const ColumnVector *averageFace) {
    this->faceclasses = faceclasses;
    this->eigenfaces = eigenfaces;
    this->averageFace = averageFace;
}

FacialRecognizer::FacialRecognizer(const Matrix *faceclasses,
                 const Matrix *eigenfaces,
                 const ColumnVector *averageFace,
                 const ColumnVector *input) {
    this->faceclasses = faceclasses;
    this->eigenfaces = eigenfaces;
    this->averageFace = averageFace;
    this->input = (ColumnVector*)Matrix::copyOf(input);
}

FacialRecognizer::~FacialRecognizer(void) {
    
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
    ColumnVector *weights = getWeights(input);
    
}

bool FacialRecognizer::nearFaceSpace(const ColumnVector *input, float tol) {
    this->input = (ColumnVector*)Matrix::copyOf(input);
    return nearFaceSpace(tol);
}

bool FacialRecognizer::nearFaceClass(float tol) const {
    ColumnVector *weights = getWeights(input);
    
}

bool FacialRecognizer::nearFaceClass(const ColumnVector *input, float tol) {
    this->input = (ColumnVector*)Matrix::copyOf(input);
    return nearFaceClass(tol);
}

ColumnVector* FacialRecognizer::faceClass(void) const {
    ColumnVector *weights = getWeights(input);
    ColumnVector *currentweights;
    ColumnVector *ret;
    float dist = 0;
    float currentdist;
    for (int i = 0; i < eigenfaces->cols(); i++) {
        currentweights = getWeights(faceclasses->getColumn(i));
        currentdist = ((ColumnVector*)Matrix::subtract(weights,currentweights))->norm2();
        if (currentdist > dist)
            ret = faceclasses->getColumn(i);
    }
    return ret;
}

ColumnVector* FacialRecognizer::faceClass(const ColumnVector *input) {
    this->input = (ColumnVector*)Matrix::copyOf(input);
    return faceClass();
    
}
