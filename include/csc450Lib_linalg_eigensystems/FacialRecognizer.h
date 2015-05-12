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

		/** Calculates the distance from the Face Space */
        float distFromFaceSpace(void) const;

		/** Calculates the distance from the given subject */
        float distFromFaceClass(const csc450Lib_linalg_base::Subject *subject) const;

		/** Calculates if a faces is within tolerance range */
        bool nearFaceSpace(float tol) const;

		/**Determines if an inputed image is a face or not, or is within tolerance to be a face */
        bool nearFaceSpace(const csc450Lib_linalg_base::ColumnVector *input,
                           float tol);
        
		/** Calculates if an image is of one of the subjects */
        bool nearFaceClass(float tol) const;

		/** Calculates if the given input matches a Subject's known face*/
        bool nearFaceClass(const csc450Lib_linalg_base::ColumnVector *input,
                           float tol);

		/** */
        const csc450Lib_linalg_base::Subject* faceClass(void) const;

		/** */
        const csc450Lib_linalg_base::Subject* faceClass(const csc450Lib_linalg_base::ColumnVector *input);
    };
}
#endif /* defined(____FacialRecognizer_included__) */