//
//  NonLinearSolver_bisection.cpp
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

#include "NonLinearSolver_bisection.h"
using namespace csc450Lib_calc_base;
using namespace csc450Lib_calc_snle;

NonLinearSolver_bisection::NonLinearSolver_bisection(void){}

const SolutionNLE* NonLinearSolver_bisection::solve(const Function1D* f,
                                                    float a, float b,
                                                    float tol) {
    float fa = f->func(a);
    float fb = f->func(b);
    float fc;
    float c = (a + b) / 2;
    
    float i = 0;
    
    // Iterate until our search bracket is smaller than the tolerance
    while ((b - a > 2 * tol) && (c != a) && (c != b)) {
        i++;
        fc = f->func(c);
        
        // Compare signs of f(c) and f(a)
        if ((fc > 0) == (fa > 0)) {
            a = c;
            fa = fc;
        }
        // Compare signs of f(c) and f(b)
        else if ((fc > 0) == (fb > 0)) {
            b = c;
            fb = fc;
        } else {
            return new SolutionNLE(c, fc, i,
                                   SEARCH_FAILED_OTHER_REASON);
        }
        // Bisect
        c = (a + b) / 2;
    }
    
    return new SolutionNLE(c, fc, i, SEARCH_SUCCESSFUL);
}