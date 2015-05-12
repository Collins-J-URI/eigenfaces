//
//  FacialRecognizer.h
//
//
//  Created by Christopher Glasz on 5/11/15.
//
//

//=================================
// include guard
#ifndef ____FacialRecognizer_included__
#define ____FacialRecognizer_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>

#include "Matrix.h"
#include "MatrixGenerator.h"
#include "ColumnVector.h"
#include "RowVector.h"
#include "EigenSystem.h"
#include "Subject.h"

namespace csc450Lib_linalg_eigensystems {
    
    /**
     * Subclass of LinearSolver which implements LU factorization
     */
    class FacialRecognizer {
    private:
        
        const csc450Lib_linalg_base::Subject **faceclasses;
        const csc450Lib_linalg_base::Matrix *eigenfaces;
        const csc450Lib_linalg_base::ColumnVector *averageFace;
        csc450Lib_linalg_base::ColumnVector *input;
        
        csc450Lib_linalg_base::ColumnVector* getWeights(const csc450Lib_linalg_base::ColumnVector *input) const;
        
    public:
        
        FacialRecognizer(void);
        FacialRecognizer(const csc450Lib_linalg_base::Subject **faceclasses,
                         const csc450Lib_linalg_base::Matrix *eigenfaces,
                         const csc450Lib_linalg_base::ColumnVector *averageFace);
        FacialRecognizer(const csc450Lib_linalg_base::Matrix *faceclasses,
                         const csc450Lib_linalg_base::Matrix *eigenfaces,
                         const csc450Lib_linalg_base::ColumnVector *averageFace,
                         const csc450Lib_linalg_base::ColumnVector *input);
        ~FacialRecognizer(void);
        
        bool nearFaceSpace(float tol) const;
        bool nearFaceSpace(const csc450Lib_linalg_base::ColumnVector *input,
                           float tol);
        
        bool nearFaceClass(float tol) const;
        bool nearFaceClass(const csc450Lib_linalg_base::ColumnVector *input,
                           float tol);
        
        csc450Lib_linalg_base::ColumnVector* faceClass(void) const;
        csc450Lib_linalg_base::ColumnVector* faceClass(const csc450Lib_linalg_base::ColumnVector *input);
    };
}
#endif /* defined(____FacialRecognizer_included__) */