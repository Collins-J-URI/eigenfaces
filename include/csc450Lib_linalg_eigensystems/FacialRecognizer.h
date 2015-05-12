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
        
        int numFaceClasses;
        const csc450Lib_linalg_base::Subject **faceclasses;
        const csc450Lib_linalg_base::Matrix *eigenfaces;
        const csc450Lib_linalg_base::ColumnVector *averageFace;
        csc450Lib_linalg_base::ColumnVector *input;
        
        csc450Lib_linalg_base::ColumnVector* getWeights(const csc450Lib_linalg_base::ColumnVector *input) const;
        
    public:
        
        FacialRecognizer(void);
        FacialRecognizer(int numFaceClasses,
                         const csc450Lib_linalg_base::Subject *faceclasses[],
                         const csc450Lib_linalg_base::Matrix *eigenfaces,
                         const csc450Lib_linalg_base::ColumnVector *averageFace);
        FacialRecognizer(int numFaceClasses,
                         const csc450Lib_linalg_base::Subject *faceclasses[],
                         const csc450Lib_linalg_base::Matrix *eigenfaces,
                         const csc450Lib_linalg_base::ColumnVector *averageFace,
                         const csc450Lib_linalg_base::ColumnVector *input);
        ~FacialRecognizer(void);
        
        float distFromFaceSpace(void) const;
        float distFromFaceClass(const csc450Lib_linalg_base::Subject *subject) const;
        
        bool nearFaceSpace(float tol) const;
        bool nearFaceSpace(const csc450Lib_linalg_base::ColumnVector *input,
                           float tol);
        
        bool nearFaceClass(float tol) const;
        bool nearFaceClass(const csc450Lib_linalg_base::ColumnVector *input,
                           float tol);
        
        const csc450Lib_linalg_base::Subject* faceClass(void) const;
        const csc450Lib_linalg_base::Subject* faceClass(const csc450Lib_linalg_base::ColumnVector *input);
    };
}
#endif /* defined(____FacialRecognizer_included__) */