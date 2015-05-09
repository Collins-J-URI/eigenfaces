//
//  LinearSolver_LU.cpp
//
//
//  Created by Christopher Glasz on 4/01/15.
//
//

#include "LinearSolver_LU.h"

using namespace std;
using namespace csc450Lib_linalg_sle;
using namespace csc450Lib_linalg_base;

const Matrix* LinearSolver_LU::factorize(const Matrix *a) const {
    // Upper triangular component
    Matrix *u = new Matrix(a->rows(), a->cols());
    
    // Lower triangular component
    Matrix *l = new Matrix(a->rows(), a->cols());
    
    // Intermediary matrix
    Matrix *lu = new Matrix(a->rows(), a->cols());
    
    // For all columns of matrix A
    for (int j = 0; j < a->cols(); j++) {
        // For all elements above the diagonal
        for (int i = 0; i < j; i++) {
            float uij = a->get(i, j);
            for (int k = 0; k <= i - 1; k++) {
                uij -= l->get(i, k) * u->get(k, j);
            }
            u->set(i, j, uij);
        }
        
        // For all elements on or below the diagonal
        for (int i = j; i < a->cols(); i++) {
            float luij = a->get(i, j);
            for (int k = 0; k <= j - 1; k++) {
                luij -= l->get(i, k) * u->get(k, j);
            }
            lu->set(i, j, luij);
        }
        
        u->set(j, j, lu->get(j, j));
        
        float s = u->get(j, j) == 0 ? 0 : 1.0f / u->get(j, j);
        
        // For all elements below the diagonal
        for (int i = j + 1; i < a->cols(); i++) {
            l->set(i, j, s * lu->get(i, j));
        }
    }
    
    Matrix *ret = new Matrix(a->rows(), a->cols());
    
    // Create combined LU matrix
    for (int i = 0; i < a->rows(); i++) {
        for (int j = 0; j < a->cols(); j++) {
            if (i > j)
                ret->set(i, j, l->get(i, j));
            else
                ret->set(i, j, u->get(i, j));
        }
    }
    
    return ret;
}

const int* LinearSolver_LU::partPivot(Matrix *a) const {
    int n = a->cols();
    
    ColumnVector *v = new ColumnVector(n);
    
    // Upper triangular component
    Matrix *u = new Matrix(n, n);
    
    // Lower triangular component
    Matrix *l = new Matrix(n, n);
    
    // Intermediary matrix
    Matrix *lu = new Matrix(n, n);
    
    // Create array of row indices
    int *pivoted = new int[n+1];
    for (int i = 0; i < n; i++) {
        pivoted[i] = i;
    }
    pivoted[n] = 1;
    
    // For all rows of matrix A
    for (int i = 0; i < n; i++) {
        float max = 0;
        for (int j = 0; j < n; j++) {
            if (abs(a->get(i, j)) > max) {
                max = abs(a->get(i, j));
            }
        }
        v->set(i, max);
    }
    
    // For all columns of matrix A
    for (int j = 0; j < n; j++) {
        // For all elements above the diagonal
        for (int i = 0; i < j; i++) {
            float uij = a->get(i, j);
            for (int k = 0; k <= i - 1; k++) {
                uij -= l->get(i, k) * u->get(k, j);
            }
            u->set(i, j, uij);
        }
        
        // For all elements on or below the diagonal
        for (int i = j; i < n; i++) {
            float luij = a->get(i, j);
            for (int k = 0; k <= j - 1; k++) {
                luij -= l->get(i, k) * u->get(k, j);
            }
            lu->set(i, j, luij);
        }
        
        // Determine i*
        int istar = j;
        float abslu = 0;
        for (int i = j; i < n; i++) {
            if (abs(lu->get(i,j))/v->get(i) > abslu) {
                abslu = abs(lu->get(i,j))/v->get(i);
                istar = i;
            }
        }
        
        // Swap rows
        if (istar != j) {
            a->swapRows(istar, j);
            l->swapRows(istar, j);
            u->swapRows(istar, j);
            lu->swapRows(istar, j);
            
            // Record swapping
            int temp = pivoted[j];
            pivoted[j] = pivoted[istar];
            pivoted[istar] = temp;
            
            // For determinant
            pivoted[n] *= -1;
        }
        
        u->set(j, j, lu->get(j, j));
        
        float s = u->get(j, j) == 0 ? 0 : 1.0f / u->get(j, j);
        
        // For all elements below the (possibly new) diagonal
        for (int i = j + 1; i < n; i++) {
            l->set(i, j, s * lu->get(i, j));
        }
    }
    
    // Overwrite a with LU
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i > j)
                a->set(i, j, l->get(i, j));
            else
                a->set(i, j, u->get(i, j));
        }
    }
    
    return pivoted;
}


const ColumnVector*  LinearSolver_LU::backsubstitute(const Matrix *u, const ColumnVector *y) const {
    ColumnVector *x = new ColumnVector(u->cols());
    
    int n = u->cols();
    
    x->set(n-1, y->get(n-1) / u->get(n-1, n-1));
    
    for (int i = n-2; i >= 0; i--) {
        float val = y->get(i);
        for (int j = i + 1; j < n; j++) {
            val -= u->get(i, j) * x->get(j);
        }
        val /= u->get(i, i);
        x->set(i, val);
    }
    
    return x;
}

const ColumnVector* LinearSolver_LU::forwardsubstitute(const Matrix *l, const ColumnVector *b) const {
    ColumnVector *y= new ColumnVector(l->cols());
    
    int n = l->cols();
    
    y->set(0, b->get(0));
    
    for (int i = 1; i < n; i++) {
        float val = b->get(i);
        for (int j = 0; j <= i-1; j++) {
            if (i == j)
                val -= y->get(j);
            else
                val -= l->get(i, j) * y->get(j);
        }
        y->set(i, val);
    }
    
    return y;
    
}

const ColumnVector* LinearSolver_LU::error(const Matrix *a,
                                           const ColumnVector *x,
                                           const ColumnVector *b) const {
    ColumnVector *bhat = (ColumnVector*)Matrix::multiply(a, x);
    ColumnVector *err = (ColumnVector*)Matrix::subtract(bhat, b);
    
    return err;
}

const ColumnVector* LinearSolver_LU::improve(const Matrix *lu,
                                             const ColumnVector *x,
                                             const ColumnVector *err) const {
    
    // Use forward and backsubstitution to produce delta x
    const ColumnVector *y = forwardsubstitute(lu, err);
    const ColumnVector *dx = backsubstitute(lu, y);
    
    return (ColumnVector*)Matrix::subtract(x, dx);
}

const LinearSystemRecord* LinearSolver_LU::solve(void) const {
    LinearSystemRecord *resultRecord = new LinearSystemRecord(UNKNOWN_STATUS, NULL);
    
    if (a->rows() < a->cols()) {
        resultRecord = new LinearSystemRecord(REGULAR_MATRIX, NULL);
    } else if (a->rows() != b->rows()) {
        resultRecord = new LinearSystemRecord(LINEAR_SOLVER_FAILED, NULL);
    } else {
        // Don't alter a
        Matrix *lu = Matrix::copyOf(a);
        
        // Pivoted b
        const ColumnVector *pb = new ColumnVector(b->rows());
        
        // Step 1:
        //  Factorize
        const int *pivoted = partPivot(lu);
        
        // Swap rows of b
        for (int i = 0; i < b->rows(); i++) {
            pb->set(i, b->get(pivoted[i]));
        }
        
        // Step 2:
        //  Solve
        //      L * y = b
        //  Using forward stubstitution
        const ColumnVector *y = forwardsubstitute(lu, pb);
        
        // Step 3:
        //  Solve
        //      U * x = y
        //  Using backsubstitution
        const ColumnVector *x = backsubstitute(lu, y);
        
        // Step 4:
        //  Iterative improvement
        for (int i = 0; i < 2; i++) {
            // Error term
            const ColumnVector *err = error(a, x, b);
            
            // Pivoted error term
            const ColumnVector *perr = new ColumnVector(err->rows());
            
            // Swap rows of err
            for (int j = 0; j < err->rows(); j++) {
                perr->set(j, err->get(pivoted[j]));
            }
            x = improve(lu, x, perr);
        }
        
        resultRecord = new LinearSystemRecord(LINEAR_SOLVER_SUCCEEDED, x);
    }
    
    return resultRecord;
}

const LinearSystemRecord* LinearSolver_LU::solve(const ColumnVector *b)  {
    setRightSideTerm(b);
    return solve();
}

const LinearSystemRecord* LinearSolver_LU::solve(const Matrix *a, const ColumnVector *b)  {
    setSLE(a,b);
    return solve();
}

float LinearSolver_LU::determinant(void) const {
    
    // Don't alter a
    Matrix *lu = Matrix::copyOf(a);
    
    // Step 1:
    //  Factorize
    const int *pivoted = partPivot(lu);
    
    // Swapping rows changes sign of determinant
    float det = pivoted[b->rows()];
    
    // Det(A) = det(L)*det(U) = sum of diagonal terms in U
    for (int i = 0; i < lu->rows(); i++) {
        det *= lu->get(i, i);
    }
    
    return det;
}

float LinearSolver_LU::determinant(const Matrix *a)  {
    setMatrix(a);
    return determinant();
}


const LinearSystemRecord* LinearSolver_LU::inversion(void) const {
    LinearSystemRecord *resultRecord = new LinearSystemRecord(UNKNOWN_STATUS, NULL);
    int n = a->rows();
    
    Matrix *inversion = new Matrix(n, n);
    ColumnVector *b = new ColumnVector(n);
    
    // Individual column solution
    const Matrix *colSol;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            b->set(j, 0);
        }
        b->set(i,1);
        
        const LinearSystemRecord *r = solve();
        if (r->getStatus() != LINEAR_SOLVER_SUCCEEDED) {
            return r;
        }
        
        colSol = r->getSolution();
        
        // Add column to inversion matrix
        for (int j = 0; j < n; j++) {
            inversion->set(i,j,colSol->get(j,0));
        }
    }
    
    resultRecord = new LinearSystemRecord(LINEAR_SOLVER_SUCCEEDED, inversion);
    return resultRecord;
}

const LinearSystemRecord* LinearSolver_LU::inversion(const Matrix *a) {
    setMatrix(a);
    return inversion();
    
}