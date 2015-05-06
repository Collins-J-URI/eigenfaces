//
//  EigenSystem.h
//
//
//  Created by Christopher Glasz on 5/6/15.
//
//

//=================================
// include guard
#ifndef ____EigenSystem_included__
#define ____EigenSystem_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Matrix.h"
#include "MatrixGenerator.h"
#include "ColumnVector.h"
using namespace std;


namespace csc450Lib_linalg_base {
    
    class EigenSystem {
    private:
        int size;
        Matrix *A;
        ColumnVector *eigenvalues;
        Matrix *eigenvectors;
        
    public:
        EigenSystem(const Matrix *A);
        Matrix* getEigenVectors(void) const;
        ColumnVector* getEigenValues(void) const;
        ColumnVector* getEigenVector(int index) const;
        float getEigenValue(int index) const;
        
        
    };
}
#endif /* defined(____EigenSystem_included__) */