#include "Subject.h"



Subject::Subject(Matrix *images, int id)
{
	this->id = id;
	this->images = images
}

Subject::getID() {
	return id;
}


