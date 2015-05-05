//
//  Function1D.cpp
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

#include "Function1D.h"
using namespace csc450Lib_calc_base;

float Function1D::phi(float x, float h) const {
    return (1/(2*h)) * (func(x + h) - func(x - h));
}

float Function1D::approxd(float x, float h, int n, int m) const {
    if (m == 0)
        return phi(x, h/(pow(2,n)));
    
    return (approxd(x, h, n, m-1) + (1/(pow(4,m) - 1)) *
            (approxd(x, h, n, m-1) - approxd(x, h, n-1, m-1)));
}

Function1D::Function1D(void) {}

Function1D::~Function1D(void) {}

float Function1D::dfunc(float x) const {
    return approxd(x, 0.0001, 5, 5);
}



