//
//  Subject.cpp
//
//
//  Created by James Collins on 5/11/15.
//
//

#include "Subject.h"

using namespace csc450Lib_linalg_base;

Subject::Subject(Matrix *images, int idnum)
{
    this->idnum = idnum;
    this->images = images;
}

int Subject::getID() const {
    return idnum;
}

Matrix* Subject::getImages(void) const {
    return images;
}

ColumnVector* Subject::getImage(int index) const {
    return images->getColumn(index);
}

const ColumnVector* Subject::calculateClassVector(const Matrix *eigenfaces,
                                            const ColumnVector *averageFace) const {
    ColumnVector *input = images->getColumn(0);
    ColumnVector *weights = new ColumnVector(eigenfaces->cols());
    for (int j = 0; j < eigenfaces->cols(); j++) {
        weights->set(j,Matrix::multiply(Matrix::transpose(eigenfaces->getColumn(j)),
                                        Matrix::subtract(input, averageFace))->get(0,0));
    }
    
    Matrix *patternVectors = weights;
    for (int i = 1; i < images->cols(); i++) {
        input = images->getColumn(i);
        weights = new ColumnVector(eigenfaces->cols());
        for (int j = 0; j < eigenfaces->cols(); j++) {
            weights->set(j,Matrix::multiply(Matrix::transpose(eigenfaces->getColumn(j)),
                                            Matrix::subtract(input, averageFace))->get(0,0));
        }
        patternVectors->addColumn(weights);
    }
    return patternVectors->averageColumn();
    
}

