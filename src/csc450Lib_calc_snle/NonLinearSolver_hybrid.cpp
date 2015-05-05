//
//  NonLinearSolver_hybrid.cpp
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

#include "NonLinearSolver_hybrid.h"
using namespace csc450Lib_calc_base;
using namespace csc450Lib_calc_snle;

NonLinearSolver_hybrid::NonLinearSolver_hybrid(void){}

const SolutionNLE* NonLinearSolver_hybrid::solve(const Function1D* f,
                                                 float a, float b, float tol) {
    float c = a;
    int i = 0;
    
    float delta, fc, dc;
    
    float fa,fb,da,db;
    
    fa = f->func(a);
    da = f->dfunc(a);
    fb = f->func(b);
    db = f->dfunc(b);
    
    while (i <= 200) {
        i++;
        
        fc = f->func(c);
        dc = f->dfunc(c);
        
        // Search bracket doesn't have a guaranteed root
        if ((fa > 0) == (fb > 0))
            return new SolutionNLE(c, fc, i,
                                   SEARCH_FAILED_OUT_OF_RANGE);
        
        // Compare signs of f(c) and f(a)
        if ((fc > 0) == (fa > 0)) {
            a = c;
            fa = fc;
            da = dc;
            
            // Check for problematic results
            if ((fa > 0) != (da > 0) && da != 0) {
                delta = -fa/da;
                c = a + delta;
            } else
                c = (a + b) / 2;
        }
        
        // Compare signs of f(c) and f(b)
        else if ((fc > 0) == (fb > 0)) {
            b = c;
            fb = fc;
            db = dc;
            
            // Check for problematic results
            if ((fb > 0) == (db > 0) && db != 0) {
                delta = -fb/db;
                c = b + delta;
            } else
                c = (a + b) / 2;
        } else
            return new SolutionNLE(c, fc, i,
                                   SEARCH_FAILED_OTHER_REASON);
        
        // When search bracket is smaller than tolerance, we are done
        if ((b - a < 2 * tol) || fabs(delta) < tol)
            return new SolutionNLE(c, fc, i, SEARCH_SUCCESSFUL);
    }
    
    return new SolutionNLE(c, fc, i, SEARCH_FAILED_TOO_MANY_ITERATIONS);
}