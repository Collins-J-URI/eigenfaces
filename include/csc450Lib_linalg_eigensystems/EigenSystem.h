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
		/** size of the eigensystem */
        int size;

		/** Matrix A */
        csc450Lib_linalg_base::Matrix *a;

		/** Variable Lambda */
        csc450Lib_linalg_base::ColumnVector *l;

		/** Variable EigenVector */
        csc450Lib_linalg_base::Matrix *v;
        
    public:

        EigenSystem(void);
        EigenSystem(const csc450Lib_linalg_base::Matrix *a,
                    const csc450Lib_linalg_base::Matrix *v,
                    const csc450Lib_linalg_base::ColumnVector *l);
		/* Return the EigenVectors of this EigenSystem
		*	return the eigen vectors of this system
		*/
        csc450Lib_linalg_base::Matrix* getEigenVectors(void) const;

		/* Return the EigenValues of this EigenSystem
		*	return the eigen values of this system
		*/
        csc450Lib_linalg_base::ColumnVector* getEigenValues(void) const;

		/* Return the Requested EigenVector of this EigenSystem
		*	@param index of which EigenVector is requested
		*	return Requested EigenVector of this system
		*/
        csc450Lib_linalg_base::ColumnVector* getEigenVector(int index) const;

		/* Return the Requested EigenValue of this EigenSystem
		*	@param index of which EigenVector is requested
		*	return the requested EigenValue of this system
		*/
        float getEigenValue(int index) const;
        
        
    };
}
#endif /* defined(____EigenSystem_included__) */