//
//  PolyFunction1D.cpp
//
//
//  Created by Christopher Glasz on 3/13/15.
//
//

#include "PolyFunction1D.h"
using namespace csc450Lib_calc_base;

PolyFunction1D::PolyFunction1D(int nbCoeffs, const float * coeff) {
    numCoeffs = nbCoeffs;
    degree = numCoeffs - 1;
    coefficients = coeff;
}

PolyFunction1D::~PolyFunction1D(void){
    numCoeffs = 0;
    degree = 0;
}

float PolyFunction1D::func(float x) const {
    float sum = 0;
    for (int i = degree; i >= 0; i--) {
        sum = coefficients[i] + x*sum;
    }
    return sum;
}

float PolyFunction1D::dfunc(float x) const {
    float sum = 0;
    for (int i = degree; i > 0; i--) {
        sum = i*coefficients[i] + x*sum;
    }
    return sum;
}

bool PolyFunction1D::isExactDerivativeDefined() {
    return true;
}