//
//  Subject.h
//
//
//  Created by James Collins on 5/11/15.
//
//

//=================================
// include guard
#ifndef ____Subject_included__
#define ____Subject_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Matrix.h"
#include "ColumnVector.h"

namespace csc450Lib_linalg_base {

	class Subject
    {
    private:
        int idnum;
    
        Matrix *images;
        
	public:
		Subject(Matrix *images, int idnum);
		~Subject();
        int getID(void) const;
        Matrix* getImages(void) const;
        ColumnVector* getImage(int index) const;
        const ColumnVector* calculateClassVector(const Matrix *eigenfaces,
                                           const ColumnVector *averageFace);
        
	};

}
#endif /* defined(____Subject_included__) */

