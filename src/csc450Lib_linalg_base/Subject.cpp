//
//  Subject.cpp
//
//
//  Created by James Collins on 5/11/15.
//
//

#include "Subject.h"

using namespace csc450Lib_linalg_base;

Subject::Subject(Matrix *images, int id)
{
	this->id = id;
	this->images = images
}

Subject::getID() {
	return id;
}


