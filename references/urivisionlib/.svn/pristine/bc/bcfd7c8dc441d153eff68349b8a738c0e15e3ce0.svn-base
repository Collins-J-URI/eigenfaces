/*  NAME:
        Transformation2D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Transformation2D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <string>
#include <typeinfo>
//
#include "Transformation2D.h"

using namespace std;
using namespace uriVL;

#if 0
//----------------------------------------------------------
#pragma mark constructors, destructors, operators
//----------------------------------------------------------
#endif

Transformation2D::Transformation2D(bool isRigid)
		:	mat_(new float*[3]),
		    isRigidTransformation_(isRigid),
			quality_(kUnknown)
{
    //  initializes an identity transformation matrix
    for (int i=0; i<3; i++)
        mat_[i] = new float[3];
    
    mat_[0][1] = mat_[0][2] = 0;
    mat_[1][0] = mat_[1][2] = 0;
    mat_[2][0] = mat_[2][1] = 0;
    mat_[0][0] = mat_[1][1] = mat_[2][2] = 1;
}



Transformation2D::Transformation2D(const float theMat[][3])
		:	mat_(new float*[3]),
		    isRigidTransformation_(false),
			quality_(kUnknown)
{
    for (int i=0; i<3; i++)
        mat_[i] = new float[3];

    setMatrix(theMat);

    isRigidTransformation_ = checkOrthogonality_();
}

Transformation2D::Transformation2D(const float*const* theMat)
		:	mat_(NULL),
		    isRigidTransformation_(false),
			quality_(kUnknown)
{
	FAIL_CONDITION( theMat == NULL,
					kNullParameterError,
					"Null 2d pointer passed to Transformation2D constructor.");
	
	mat_ = new float*[3];
	for (int i=0; i<3; i++)
        mat_[i] = new float[3];

	try {
		setMatrix(theMat);
	}
	catch (ErrorReport& e) {
		for (int i=0; i<3; i++)
			delete []mat_[i];
		delete []mat_;
		
		e.appendToMessage("called from Transformation2D constructor.");
		throw e;
	}

    isRigidTransformation_ = checkOrthogonality_();
}

Transformation2D::Transformation2D(const Transformation2D* theTransf)
	
		:	mat_(NULL),
		    isRigidTransformation_(false),
			quality_(kUnknown)
{
	FAIL_CONDITION( theTransf==NULL,
					kNullParameterError,
					"NULL Transformation2D passed to Transformation2D copy constructor");

    //  this class can only work with matrices.  If subclasses want to
    //  be smarter, they will have to provide code for that
	try {
		mat_ = new float*[3];
		for (int i=0; i<3; i++)
		{
			mat_[i] = new float[3];
			for (int j=0; j<3; j++)
				mat_[i][j] = theTransf->mat_[i][j];

		}
    }
	catch (...) {
		for (int i=0; i<3; i++)
			delete []mat_[i];
		delete []mat_;
	
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid Transformation2D passed to Transformation2D copy constructor");
	}
	
    //  if the transformation to copy is orthofgonal, use this info, otherwise verify 
	//	the matrix.
    isRigidTransformation_ = theTransf->isRigidTransformation_ ? true : checkOrthogonality_();
}


Transformation2D::Transformation2D(const Transformation2D& theTransf)
	
		:	mat_(NULL),
		    isRigidTransformation_(false),
			quality_(kUnknown)
{
    //  this class can only work with matrices.  If subclasses want to
    //  be smarter, they will have to provide code for that
	try {
		mat_ = new float*[3];
		for (int i=0; i<3; i++)
		{
			mat_[i] = new float[3];
			for (int j=0; j<3; j++)
				mat_[i][j] = theTransf.mat_[i][j];

		}
    }
	catch (...) {
		for (int i=0; i<3; i++)
			delete []mat_[i];
		delete []mat_;
	
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid Transformation2D passed to Transformation2D copy constructor");
	}
	
    //  if the transformation to copy is orthofgonal, use this info, otherwise verify 
	//	the matrix.
    isRigidTransformation_ = theTransf.isRigidTransformation_ ? true : checkOrthogonality_();
}


Transformation2D::~Transformation2D(void)
{
    for (int i=0; i<3; i++)
        delete []mat_[i];

    delete []mat_;
}

const Transformation2D& Transformation2D::operator = (const Transformation2D& theTransf)
{
    FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Transformation2D = operator not implemented yet");
	return *this;
}

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark checks and computations
//----------------------------------------------------------
#endif


bool Transformation2D::checkOrthogonality_(void) const
{
	// last row must be  0 0 1
	if ((mat_[2][0] != 0.f) || (mat_[2][1] != 0.f) || (mat_[2][2] != 1.f))
		return false;
		
	const float SMALL_VAL = (float) 1.0E-5;
	
	const float dotProd01 = mat_[0][0]*mat_[0][1] + mat_[1][0]*mat_[1][1];
	if (fabsf(dotProd01) > SMALL_VAL)
		return false;
	
	const float norm0 = mat_[0][0]*mat_[0][0] + mat_[1][0]*mat_[1][0];
	if (fabsf(norm0 - 1.f) > SMALL_VAL)
		return false;
	
	const float norm1 = mat_[0][1]*mat_[0][1] + mat_[1][1]*mat_[1][1];
	if (fabsf(norm1 - 1.f) > SMALL_VAL)
		return false;

	return true;
}



#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark access functions
//----------------------------------------------------------
#endif


void Transformation2D::setMatrix(const float theMat[][3])
{
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            mat_[i][j] = theMat[i][j];

}

void Transformation2D::setMatrix(const float*const* theMat)
{
    FAIL_CONDITION( theMat == NULL,
                    kNullParameterError,
                    "NULL matrix passed to Transformation2D::setMatrix");

    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            mat_[i][j] = theMat[i][j];

}

float* const* const Transformation2D::getMatrix(ReadWriteStatus rwa)
{
    return mat_;
}

const float* const* const Transformation2D::getMatrix(void) const
{
    return mat_;
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark other access functions
//----------------------------------------------------------
#endif


bool Transformation2D::isRigidTransformation(void) const
{
    return isRigidTransformation_;
}


