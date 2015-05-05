//
//  NonLinearSolver_secant.cpp
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

#include "NonLinearSolver_secant.h"
using namespace csc450Lib_calc_base;
using namespace csc450Lib_calc_snle;

NonLinearSolver_secant::NonLinearSolver_secant(void){}

const SolutionNLE* NonLinearSolver_secant::solve(const Function1D* f,
                                                 float a, float b, float tol) {
    float c = a;
    
    // Choose some arbitrary small offset
    float h = 0.001;
    int i = 0;
    
    float delta, fc, dc, fa, slope;
    
    while (i <= 200) {
        i++;
        
        fa = f->func(c);
        fc = f->func(c+h);
        slope = (fc - fa) / (h);
        
        // Search will always fail if we are at a local extreme
        if (slope == 0)
            return new SolutionNLE(c, fc, i,
                                   SEARCH_FAILED_NUMERICAL_ERROR);
        
        delta = -fa/slope;
        c += delta;
        
        // If delta is smaller than tolerance, we are done
        if (fabs(delta) < tol)
            return new SolutionNLE(c, fc, i, SEARCH_SUCCESSFUL);
        
        // Gone out of range
        if (c <= a || c >= b)
            return new SolutionNLE(c, fc, i,
                                   SEARCH_FAILED_OUT_OF_RANGE);
        
    }
    
    return new SolutionNLE(c, fc, i, SEARCH_FAILED_TOO_MANY_ITERATIONS);
    
}