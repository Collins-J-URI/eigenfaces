/*  NAME:
        Transformation3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Transformation3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Transformation3D.h"

using namespace uriVL;

Transformation3D::Transformation3D(void)
    :       homMat_(new HomogeneousMatrix()),
            matrixIsReference_(true),
            referenceHasChanged_(true)
{
}

Transformation3D::Transformation3D(const double* glMat)
    :       homMat_(new HomogeneousMatrix(glMat)),
            matrixIsReference_(true),
            referenceHasChanged_(true)
{
}

Transformation3D::Transformation3D(const double theMat[][4])
    :       homMat_(new HomogeneousMatrix(theMat)),
            matrixIsReference_(true),
            referenceHasChanged_(true)
{
}

Transformation3D::Transformation3D(const HomogeneousMatrix* theMat)
    :       homMat_(new HomogeneousMatrix(theMat)),
            matrixIsReference_(true),
            referenceHasChanged_(true)
{
}


Transformation3D::Transformation3D(const Transformation3D* theTransf)
    :       homMat_(NULL),
            matrixIsReference_(true),
            referenceHasChanged_(true)
{
    FAIL_CONDITION( theTransf==NULL,
                    kNullTransformation3D,
                    "Attempt to clone a NULL Transformation3D"); 

    //  this class can only work with matrices.  If subclasses want to
    //  be smarter, they will have to provide code for that
    if (!theTransf->matrixIsReference_)
        theTransf->updateMatrix_();
       
    //  and now we can copy the matrix.   
    homMat_ = new HomogeneousMatrix(theTransf->homMat_);
            
    matrixIsReference_ = true;
    referenceHasChanged_ = true;
}

Transformation3D::Transformation3D(const Transformation3D& obj)
    :       homMat_(NULL),
            matrixIsReference_(true),
            referenceHasChanged_(true)
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "Transformation3D copy constructor not implemented");
}
            
 
 Transformation3D::~Transformation3D(void)
{
	delete homMat_;
}


#if 0
#pragma mark -
#endif

const Transformation3D& Transformation3D::operator = (const Transformation3D& obj)
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "Transformation3D copy operator not implemented");
    return *this;
}
 

void Transformation3D::setMatrixAsReference_(bool state)
{
    matrixIsReference_ = state;
    referenceHasChanged_ = true;
}
            

#if 0
#pragma mark -
#endif


const HomogeneousMatrix* Transformation3D::getMatrix(void) const
{
    if (!matrixIsReference_  && referenceHasChanged_)
        updateMatrix_();
        
    return homMat_;
}

HomogeneousMatrix* Transformation3D::getMatrix(ReadWriteStatus rwa)
{
    if (!matrixIsReference_  &&referenceHasChanged_)
        updateMatrix_();
    referenceHasChanged_ = true;
        
    return homMat_;
}

const GLfloat* Transformation3D::getGLMatrix(void) const
{
    if (!matrixIsReference_  &&referenceHasChanged_)
        updateMatrix_();
        
    return homMat_->getGLMatrix();
}

//GLfloat* Transformation3D::getGLMatrix(ReadWriteStatus rwa)
//{
//    if (!matrixIsReference_  &&referenceHasChanged_)
//        updateMatrix_();
//        
//    return homMat_->getGLMatrix(rwa);
//}


//bool Transformation2D::checkOrthogonality_(void) const
//{
//	if (// last row must be  0 0 0 1
//		(mat_[3][0] != 0.f) || (mat_[3][1] != 0.f) || (mat_[3][2] != 0.f) || (mat_[3][3] != 1.f) ||
//		//	third column must be  0 0 1
//		(mat_[0][2] != 0.f) || (mat_[1][2] != 0.f) || (mat_[2][2] != 1.f) ||
//		//	third row must start with  0 0 
//		(mat_[2][0] != 0.f) || (mat_[2][1] != 0.f))
//		return false;
//		
//	const float SMALL_VAL = (float) 1.0E-5;
//	
//	const float dotProd01 = mat_[0][0]*mat_[0][1] + mat_[1][0]*mat_[1][1] + mat_[2][0]*mat_[2][1];
//	if (fabsf(dotProd01) > SMALL_VAL)
//		return false;
//	
//	const float norm0 = mat_[0][0]*mat_[0][0] + mat_[1][0]*mat_[1][0] + mat_[2][0]*mat_[2][0];
//	if (fabsf(norm0 - 1.f) > SMALL_VAL)
//		return false;
//	
//	const float norm1 = mat_[0][1]*mat_[0][1] + mat_[1][1]*mat_[1][1] + mat_[2][1]*mat_[2][1];
//	if (fabsf(norm1 - 1.f) > SMALL_VAL)
//		return false;
//	
//	const float norm2 = mat_[0][2]*mat_[0][2] + mat_[1][2]*mat_[1][2] + mat_[2][2]*mat_[2][2];
//	if (fabsf(norm2 - 1.f) > SMALL_VAL)
//		return false;
//	
//	return true;
//}
//
