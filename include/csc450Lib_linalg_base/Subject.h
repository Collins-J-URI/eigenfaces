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

	/* The Subject class is responsible for storing each subject's individual ID 
	*as well as the image data that belongs to them
	*/
	class Subject
    {
    private:
		/* Variable to store the Subjects ID */
        int idnum;
		/* A variable to store all the images and data for this subject */
        Matrix *images;
        
	public:

		/*
		*	Constructor for the subject class
		*	@param images A matrix of all the images that this subject owns
		*	@param idnum the ID of the subject
		*/
		Subject(Matrix *images, int idnum);
		~Subject();

		/*
		*	Get the current Subject's ID
		*	return The Matrix containing all the images and data for this subject
		*/
        int getID(void) const;

		/*
		*	Get's the Matrix of images for this subject
		*	return The Matrix containing all the images and data for this subject
		*/
        Matrix* getImages(void) const;

		/*
		*	Get a requested image from the list of images that this subject has
		*	@param index the index of which image for this subject you would like to receive
		*	return A columnVector of the Pixel Data of the requested image
		*/
        ColumnVector* getImage(int index) const;

		/*
		*	Calculate the Class Vector of this Subject(person)
		*	@param eigenfaces A matrix of all the eigenfaces calculated
		*	@param averageFace A ColumnVector of the averageFace.
		*	return A columnVector that is the classvector for this subject
		*/
        const ColumnVector* calculateClassVector(const Matrix *eigenfaces,
                                           const ColumnVector *averageFace) const;
        
	};

}
#endif /* defined(____Subject_included__) */

