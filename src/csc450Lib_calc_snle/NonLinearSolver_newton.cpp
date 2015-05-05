//
//  NonLinearSolver_newton.cpp
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

#include "NonLinearSolver_newton.h"
using namespace csc450Lib_calc_base;
using namespace csc450Lib_calc_snle;

NonLinearSolver_newton::NonLinearSolver_newton(void){}

const SolutionNLE* NonLinearSolver_newton::solve(const Function1D* f,
                                                 float a, float b, float tol) {
    
    float c = a;
    int i = 0;
    
    float delta, fc, dc;
    
    while (i <= 200) {
        i++;
        
        fc = f->func(c);
        dc = f->dfunc(c);
        
        // Search will always fail if we are at a local extreme
        if (dc == 0)
            return new SolutionNLE(c, fc, i,
                                   SEARCH_FAILED_NUMERICAL_ERROR);
        
        delta = -fc/dc;
        c += delta;
        
        // If delta is smaller than tolerance, we are done
        if (fabs(delta) < tol)
            return new SolutionNLE(c, fc, i, SEARCH_SUCCESSFUL);
        
        // Gone out of range
        if (c <= a || c > b)
            return new SolutionNLE(c, fc, i,
                                   SEARCH_FAILED_OUT_OF_RANGE);
        
    }
    
    return new SolutionNLE(c, fc, i, SEARCH_FAILED_TOO_MANY_ITERATIONS);
}