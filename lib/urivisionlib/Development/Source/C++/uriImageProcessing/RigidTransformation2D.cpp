/*  NAME:
        RigidTransformation2D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RigidTransformation2D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "RigidTransformation2D.h"
#include "Translation2D.h"
#include "Rotation2D.h"

using namespace std;
using namespace uriVL;

RigidTransformation2D::RigidTransformation2D(void)
        :   Transformation2D(true),
			//
			angle_(0.f),
			angleIsReference_(false)
{
}

RigidTransformation2D::RigidTransformation2D(const float theMat[][3])
   try	:   Transformation2D(theMat),
			//
			angle_(0.f),
			angleIsReference_(false)
{
	FAIL_CONDITION( !isRigidTransformation(),
					kInvalidParameterError,
					"The matrix passed to RigidTransformation2D is not that of a rigid transformation");
					
    angleIsReference_ = false;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RigidTransformation2D constructor.");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid parameter passed to RigidTransformation2D constructor.");		
}


RigidTransformation2D::RigidTransformation2D(const RigidTransformation2D* theTransf)
        :   Transformation2D(true),
			//
			angle_(0.f),
			angleIsReference_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Copy constructor not open for business yet");
}

RigidTransformation2D::RigidTransformation2D(Rotation2D *theRot, Translation2D *theTrans)
        :   Transformation2D(true),
			//
			angle_(0.f),
			angleIsReference_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Constructor not fully open for business yet");
}

RigidTransformation2D::~RigidTransformation2D(void)
{
}

void RigidTransformation2D::setMatrix(const float theMat[][3])
{
	Transformation2D::setMatrix(theMat);
	
    FAIL_CONDITION( !checkOrthogonality_(),
                    kInvalidParameterError,
                    "input matrix is not orthogonal in RigidTransformation2D::setMatrix");

	angle_ = atan2f(theMat[1][0], theMat[0][0]);
}



void RigidTransformation2D::orthogonalizeMatrix_(void)
{
	float *const* mat = getMatrix(R_W_ACCESS);
	
    FAIL_CONDITION( mat[2][2] == 0.f,
                    kMatrixCalculationError,
                    "Cannot orthogonalize a matrix when scale element is 0");
    if (mat[2][2] != 1.f)
    {
        for (int i=0; i<2; i++)
            for (int j=0; j<3; j++)
                mat[i][j] /= mat[2][2];

    }
    //  force the bottom row to be {0, 0, 1}
    mat[2][0] = mat[2][1] = 0.f; mat[2][2] = 1.f;

    //  If the determinant is  negative this is not a rigid transformation matrix    
    FAIL_CONDITION( (mat[0][0]*mat[1][1] - mat[1][0] * mat[0][1]) <= 0,
                    kMatrixCalculationError,
                    "Matrix determinant <= 0: it cannot be a rigid transformation");
    
    //  Now I compute the 2x2 orthogonal matrix closest to the current matrix
    //  We expect the matrix to be very close to orthogonal, so basically we can
    //  look for the angle theta s.t. the upper-left part 2x2 submatrix looks like
    //  {{cos(theta), -sin(theta)}, {sin(theta), cos(theta)}}
    const float v0 = mat[0][0] + mat[1][1];
    const float v1 = mat[1][0] - mat[0][1];
    const float nv = sqrtf(v0*v0+v1*v1);
    const float cTheta = v0/nv;
    const float sTheta = v1/nv;
    mat[0][0] = mat[1][1] = cTheta;
    mat[0][1] = - (mat[1][0] = sTheta);
    
	angle_ = atan2f(sTheta, cTheta);
}


float RigidTransformation2D::getTransX(void) const
{
	const float *const* mat = getMatrix();
    return mat[0][2];
}

float RigidTransformation2D::getTransY(void) const
{
	const float *const* mat = getMatrix();
    return mat[1][2];
}

float RigidTransformation2D::getAngle(void) const
{
	return angle_;

}

void RigidTransformation2D::setAngle(float angle)
{
    angle_ = angle;

	float *const* mat = getMatrix(R_W_ACCESS);
    mat[2][0] = mat[2][1] = 0.f; mat[2][2] = 1.f;
	const float ct = cosf(angle_),
				st = sinf(angle_);
	mat[0][0] = mat[1][1] = ct;
	mat[0][1] = -st;
	mat[1][0] = st;
}

void RigidTransformation2D::setTranslation(float tx, float ty)
{
	float *const* mat = getMatrix(R_W_ACCESS);
    mat[0][2] = tx;
	mat[1][2] = ty;
}


void RigidTransformation2D::setTranslation(const float trans[])
{
	float *const* mat = getMatrix(R_W_ACCESS);
	try {
		mat[0][2] = trans[0];
		mat[1][2] = trans[1];
	}
	catch (...)
	{
		FAIL_CONDITION( trans == NULL,
						kNullParameterError,
						"null array passed to RigidTransformation2D::setTranslation.");		
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid pointer passed to IRigidTransformation2D::setTranslation.");		
	}
	
}

