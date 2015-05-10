/*  NAME:
        Camera.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Camera class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Camera.h"

using namespace uriVL;

Camera::Camera(IntrinsicParameters* params, RigidTransformation3D* transf)
        :   Entity3D(),
            //
            cam2World_(transf != NULL ? new RigidTransformation3D(transf) : new RigidTransformation3D()),
            cam2WorldIsUpToDate_(true),
            intrinsic_(params)
{
}

Camera::Camera(const Camera& obj)
        :   Entity3D(),
            //
            cam2World_(NULL),
            cam2WorldIsUpToDate_(true),
            intrinsic_(NULL)
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "Camera copy constructor not implemented");
}
            

Camera::~Camera(void)
{
    delete cam2World_;    
}

 

#if 0
#pragma mark -
#endif

const Camera& Camera::operator = (const Camera& obj)
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "Camera copy operator not implemented");
    return *this;
}

int Camera::getWidth(void)
{
	return intrinsic_ != NULL ? intrinsic_->getWidth() : 0;
}

int Camera::getHeight(void)
{
	return intrinsic_ != NULL ? intrinsic_->getHeight() : 0;
}


//------------------------------------------------------------------------------
//  A bunch of projection functions
//------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif


void Camera::getImagePoint(const float X, const float Y, const float Z, float* x, float* y)
{
    getImagePoint_(X, Y, Z, x, y);
}

void Camera::getImagePoint(const double X, const double Y, const double Z, double* x, double* y)
{
    getImagePoint_(X, Y, Z, x, y);
}

void Camera::getImagePoint(const float* XYZ, float* x, float* y)
{
    FAIL_CONDITION( XYZ == NULL,
                    kNullArrayError,
                    "null coordinate array passed as parameter to CAmera::getImagePoint");
    getImagePoint_(XYZ[0], XYZ[1], XYZ[2], x, y);
}

void Camera::getImagePoint(const double* XYZ, double* x, double* y)
{
    FAIL_CONDITION( XYZ == NULL,
                    kNullArrayError,
                    "null coordinate array passed as parameter to CAmera::getImagePoint");
    getImagePoint_(XYZ[0], XYZ[1], XYZ[2], x, y);
}

void Camera::getImagePoint(const float* XYZ, float* xy)
{
    getImagePoint_(XYZ[0], XYZ[1], XYZ[2], xy, xy+1);
}

void Camera::getImagePoint(const double* XYZ, double* xy)
{
    getImagePoint_(XYZ[0], XYZ[1], XYZ[2], xy, xy+1);
}

void Camera::getImagePoint(const float X, const float Y, const float Z, float* xy)
{
    getImagePoint_(X, Y, Z, xy, xy+1);
}

void Camera::getImagePoint(const double X, const double Y, const double Z, double* xy)
{
    getImagePoint_(X, Y, Z, xy, xy+1);
}

void Camera::getImagePoint(const Point3D* pt3D, float* x, float* y)
{
    FAIL_CONDITION( pt3D == NULL,
                    kNullParameterError,
                    "null 3DPoint passed as parameter to CAmera::getImagePoint");
    getImagePoint_(static_cast<float>(pt3D->getX()), static_cast<float>(pt3D->getY()), 
                    static_cast<float>(pt3D->getZ()), x, y);
}

void Camera::getImagePoint(const Point3D* pt3D, double* x, double* y)
{
    FAIL_CONDITION( pt3D == NULL,
                    kNullParameterError,
                    "null 3DPoint passed as parameter to CAmera::getImagePoint");
    getImagePoint_(pt3D->getX(), pt3D->getY(), pt3D->getZ(), x, y);
}

void Camera::getImagePoint(const Point3D* pt3D, ImagePoint_F* imgPt)
{
    FAIL_CONDITION( pt3D == NULL,
                    kNullParameterError,
                    "null 3DPoint passed as parameter to CAmera::getImagePoint");
    FAIL_CONDITION( imgPt == NULL,
                    kNullParameterError,
                    "null ImagePoint_F passed as parameter to CAmera::getImagePoint");
    float   x, y;
    getImagePoint_(static_cast<float>(pt3D->getX()), static_cast<float>(pt3D->getY()), 
                    static_cast<float>(pt3D->getZ()), &x, &y);
    imgPt->setCoordinates(x, y);
}

void Camera::getImagePoint(const float* XYZ, ImagePoint_F* imgPt)
{
    FAIL_CONDITION( imgPt == NULL,
                    kNullParameterError,
                    "null ImagePoint_F passed as parameter to CAmera::getImagePoint");
    float   x, y;
    getImagePoint_(XYZ[0], XYZ[0], XYZ[2], &x, &y);
    imgPt->setCoordinates(x, y);
}

void Camera::getImagePoint(const double* XYZ, ImagePoint_F* imgPt)
{
    FAIL_CONDITION( imgPt == NULL,
                    kNullParameterError,
                    "null ImagePoint_F passed as parameter to CAmera::getImagePoint");
    double   x, y;
    getImagePoint_(XYZ[0], XYZ[0], XYZ[2], &x, &y);
    imgPt->setCoordinates((float) x, (float) y); 
}

void Camera::getImagePoint(const float X, const float Y, const float Z, ImagePoint_F* imgPt)
{
    FAIL_CONDITION( imgPt == NULL,
                    kNullParameterError,
                    "null ImagePoint_F passed as parameter to CAmera::getImagePoint");
    float   x, y;
    getImagePoint_(X, Y, Z, &x, &y);
    imgPt->setCoordinates(x, y);
}

void Camera::getImagePoint(const double X, const double Y, const double Z, ImagePoint_F* imgPt)
{
    FAIL_CONDITION( imgPt == NULL,
                    kNullParameterError,
                    "null ImagePoint_F passed as parameter to CAmera::getImagePoint");
    double   x, y;
    getImagePoint_(X, Y, Z, &x, &y);
    imgPt->setCoordinates((float) x, (float) y); 
}

ImagePoint_F* Camera::getImagePoint(const float* XYZ)
{
    float   x, y;
    getImagePoint_(XYZ[0], XYZ[1], XYZ[2], &x, &y);
    return new ImagePoint_F(x, y);
}

ImagePoint_F* Camera::getImagePoint(const double* XYZ)
{
    double   x, y;
    getImagePoint_(XYZ[0], XYZ[1], XYZ[2], &x, &y);
    return new ImagePoint_F((float) x, (float) y);
}

ImagePoint_F* Camera::getImagePoint(const float X, const float Y, const float Z)
{
    float   x, y;
    getImagePoint_(X, Y, Z, &x, &y);
    return new ImagePoint_F(x, y);
}

ImagePoint_F* Camera::getImagePoint(const double X, const double Y, const double Z)
{
    double   x, y;
    getImagePoint_(X, Y, Z, &x, &y);
    return new ImagePoint_F((float) x, (float) y);
}

#if 0
#pragma mark -
#endif

void Camera::setCamToWorldTransf(RigidTransformation3D* transf)
{
	cam2World_ = new RigidTransformation3D(transf);
}

const RigidTransformation3D* Camera::getCamToWorldTransf(void)
{
	return cam2World_;
}

RigidTransformation3D* Camera::getCamToWorldTransf(ReadWriteStatus rwa)
{
	return cam2World_;
}

const IntrinsicParameters* Camera::getIntrinsicParameters(void) const
{
    return intrinsic_;
}
            

IntrinsicParameters* Camera::getIntrinsicParameters(ReadWriteStatus rwa) const
{
    return intrinsic_;
}
            
