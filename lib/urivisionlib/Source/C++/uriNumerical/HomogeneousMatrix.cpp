/*  NAME:
        HomogeneousMatrix.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib HomogeneousMatrix class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <string>
#include <cmath>
//
#include "HomogeneousMatrix.h"

using namespace uriVL;

HomogeneousMatrix::HomogeneousMatrix()
	:	Matrix(4, 4)
{
    mat_[3][3] = 1.;
}


HomogeneousMatrix::HomogeneousMatrix(const double* glMat)
	:	Matrix(4, 4)
{
    int k = 0;
    for (int j=0; j<4; j++)
        for (int i=0; i<4; i++)
            mat_[i][j] = glMat[k++];
            
}

HomogeneousMatrix::HomogeneousMatrix(const double theMat[][4])
	:	Matrix(4, 4)
{
    for (int j=0; j<4; j++)
        for (int i=0; i<4; i++)
            mat_[i][j] = theMat[i][j];
            
}



HomogeneousMatrix::HomogeneousMatrix(const HomogeneousMatrix* theMat)
	:	Matrix(theMat)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "constructor not open for business yet"); 
/*
    double** zeMat = theMat->getMatrix();

    for (int j=0; j<4; j++)
        for (int i=0; i<4; i++)
            mat_[i][j] = zeMat[i][j];

  isDiagonal_ = theMat->isDiagonal_;
  isUpperTriangular_ = theMat->isUpperTriangular_;
  isLowerTriangular_ = theMat->isLowerTriangular_;
  isSymmetric_ = theMat->isSymmetric_;
  isFullRank_ = theMat->isFullRank_;
  isAntisymmetric_ = theMat->isAntisymmetric_;
  isOrthogonal_ = theMat->isOrthogonal_;
*/
}

HomogeneousMatrix::~HomogeneousMatrix()
{
}

const GLfloat* HomogeneousMatrix::getGLMatrix(void)
{
	glMat_[0] = static_cast<GLfloat>(mat_[0][0]);
	glMat_[1] = static_cast<GLfloat>(mat_[1][0]);
	glMat_[2] = static_cast<GLfloat>(mat_[2][0]);
	glMat_[3] = static_cast<GLfloat>(mat_[3][0]);
	glMat_[4] = static_cast<GLfloat>(mat_[0][1]);
	glMat_[5] = static_cast<GLfloat>(mat_[1][1]);
	glMat_[6] = static_cast<GLfloat>(mat_[2][1]);
	glMat_[7] = static_cast<GLfloat>(mat_[3][1]);
	glMat_[8] = static_cast<GLfloat>(mat_[0][2]);
	glMat_[9] = static_cast<GLfloat>(mat_[1][2]);
	glMat_[10] = static_cast<GLfloat>(mat_[2][2]);
	glMat_[11] = static_cast<GLfloat>(mat_[3][2]);
	glMat_[12] = static_cast<GLfloat>(mat_[0][3]);
	glMat_[13] = static_cast<GLfloat>(mat_[1][3]);
	glMat_[14] = static_cast<GLfloat>(mat_[2][3]);
	glMat_[15] = static_cast<GLfloat>(mat_[3][3]);
		
	return glMat_;
}

GLfloat* HomogeneousMatrix::getGLMatrix(ReadWriteStatus rwa)
{
    return const_cast<GLfloat* >(getGLMatrix());
}
