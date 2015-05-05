//
//  DeflatedFunction.cpp
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

#include "DeflatedFunction1D.h"
using namespace csc450Lib_calc_base;
using namespace csc450Lib_calc_snle;

DeflatedFunction1D::DeflatedFunction1D(const Function1D* f, const std::vector<float> xStar){
    this->f = f;
    this->xStar = xStar;
}

DeflatedFunction1D::~DeflatedFunction1D() {
}

float DeflatedFunction1D::func(float x) const {
    // Don't want to divide by zero
    for (int i = 0; i < xStar.size(); i++)
        if (x == xStar[i])
            x+=0.00001;
				
    float val = f->func(x);
    for (int i = 0; i < xStar.size(); i++) {
        val /= (x - xStar[i]);
    }
    return val;
}

bool DeflatedFunction1D::isExactDerivativeDefined() {
    return false;
}