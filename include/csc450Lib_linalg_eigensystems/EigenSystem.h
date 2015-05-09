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


namespace csc450Lib_linalg_eigensystems {
    
    class EigenSystem {
    private:
        int size;
        csc450Lib_linalg_base::Matrix *a;
        csc450Lib_linalg_base::ColumnVector *l;
        csc450Lib_linalg_base::Matrix *v;
        
    public:
        EigenSystem(void);
        EigenSystem(const csc450Lib_linalg_base::Matrix *a,
                    const csc450Lib_linalg_base::Matrix *v,
                    const csc450Lib_linalg_base::ColumnVector *l);
        csc450Lib_linalg_base::Matrix* getEigenVectors(void) const;
        csc450Lib_linalg_base::ColumnVector* getEigenValues(void) const;
        csc450Lib_linalg_base::ColumnVector* getEigenVector(int index) const;
        float getEigenValue(int index) const;
        
        
    };
}
#endif /* defined(____EigenSystem_included__) */