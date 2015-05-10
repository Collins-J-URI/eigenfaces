/*  NAME:
        RectifiedStereoPair.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RectifiedStereoPair class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "RectifiedStereoPair.h"
#include "Translation3D.h"
#include "IntrinsicParameters_pinhole.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"

//jyh
//Currently all source code commented out.  The StereoPair hierarchy needs complete overhaul

using namespace uriVL;

//---------------------------
//  Perspective cameras
//---------------------------

RectifiedStereoPair::RectifiedStereoPair(RasterImage* leftImg, RasterImage* rightImg, 
						float B, double focX, double focY, RigidTransformation3D* w2s) 
		try	:	StereoPair(leftImg, rightImg, w2s, true)		
{
//	FAIL_CONDITION(	leftImg == NULL,
//                    kNullRasterImageError,
//                    "NULL raster image passed as parameter to RectifiedStereoPair::RectifiedStereoPair");
//    FAIL_CONDITION(	rightImg == NULL,
//                    kNullRasterImageError,
//                    "NULL raster image passed as parameter to RectifiedStereoPair::RectifiedStereoPair");
//       
//    IntrinsicParameters_pinhole *params = 
//            new IntrinsicParameters_pinhole(leftImg->getWidth(), leftImg->getHeight(), 
//    										0.5L*leftImg->getWidth(), 0.5L*leftImg->getHeight(),
//    										focX, focY);
//
//	//	create the reference frame for the stereo system.
//    ReferenceFrame3D* newFrame = new ReferenceFrame3D(w2s);
//	setReferenceFrame(newFrame);
//	
//    RigidTransformation3D *s2L = new RigidTransformation3D();
//    double* const *const matS2L = (s2L->getMatrix(R_W_ACCESS))->getMatrix(R_W_ACCESS);
//    matS2L[0][3] = -0.5L*B;
//    ReferenceFrame3D *leftCamFrame = new ReferenceFrame3D(newFrame, s2L);
//    leftCamera_ = new PerspectiveCamera(params);
//    leftCamera_->setReferenceFrame(leftCamFrame);
//
//    RigidTransformation3D *s2R = new RigidTransformation3D();
//    double* const *const matW2R = (s2R->getMatrix(R_W_ACCESS))->getMatrix(R_W_ACCESS);
//    matW2R[0][3] = 0.5L*B;
//    ReferenceFrame3D *rightCamFrame = new ReferenceFrame3D(newFrame, s2R);
//    rightCamera_ = new PerspectiveCamera(params);
//    rightCamera_->setReferenceFrame(rightCamFrame);
//
//	transf_ = new RigidTransformation3D();
//	double* const *const matL2R = transf_->getMatrix(R_W_ACCESS)->getMatrix(R_W_ACCESS);
//    matL2R[0][3] = -B;
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RectifiedStereoPair constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "invalid parameter passed to RectifiedStereoPair constructor");
}

RectifiedStereoPair::RectifiedStereoPair(ImageRect* rect, const float B, const double focX, 
										const double focY, RigidTransformation3D *w2s)
	:   StereoPair(true)
{
//    FAIL_CONDITION( rect == NULL,
//                    kNullRectangleError,
//                    "null ImageRect parameter in RectifiedStereoPair constructor");
//
//    isRectified_ = true;
//
//    IntrinsicParameters_pinhole *params = 
//            new IntrinsicParameters_pinhole(rect->getWidth(), rect->getHeight(), 
//    										0.5L*rect->getWidth(), 0.5L*rect->getHeight(),
//    										focX, focY);
//
//	//	create the reference frame for the stereo system.
//    ReferenceFrame3D* newFrame = new ReferenceFrame3D(w2s);
//	setReferenceFrame(newFrame);
//	
//    RigidTransformation3D *s2L = new RigidTransformation3D();
//    double* const *const matS2L = (s2L->getMatrix(R_W_ACCESS))->getMatrix(R_W_ACCESS);
//    matS2L[0][3] = -0.5L*B;
//    ReferenceFrame3D *leftCamFrame = new ReferenceFrame3D(newFrame, s2L);
//    leftCamera_ = new PerspectiveCamera(params);
//    leftCamera_->setReferenceFrame(leftCamFrame);
//
//    RigidTransformation3D *s2R = new RigidTransformation3D();
//    double* const *const matW2R = (s2R->getMatrix(R_W_ACCESS))->getMatrix(R_W_ACCESS);
//    matW2R[0][3] = 0.5L*B;
//    ReferenceFrame3D *rightCamFrame = new ReferenceFrame3D(newFrame, s2R);
//    rightCamera_ = new PerspectiveCamera(params);
//    rightCamera_->setReferenceFrame(rightCamFrame);
//
//	transf_ = new RigidTransformation3D();
//	double* const *const matL2R = transf_->getMatrix(R_W_ACCESS)->getMatrix(R_W_ACCESS);
//    matL2R[0][3] = -B;
}
						
						
//---------------------------
//  Orthographic cameras
//---------------------------


RectifiedStereoPair::RectifiedStereoPair(ImageRect* rect, float B, RigidTransformation3D *w2s)
        :   StereoPair()
{
//	
//	FAIL_CONDITION( rect == NULL,
//                    kNullRectangleError,
//                    "null ImageRect parameter in RectifiedStereoPair constructor");
//	IntrinsicParameters *params = 
//            new IntrinsicParameters(rect->getWidth(), rect->getHeight());
//
//    isRectified_ = true;
//
//	//	create the reference frame for the stereo system.
//    ReferenceFrame3D* newFrame = new ReferenceFrame3D(w2s);
//	setReferenceFrame(newFrame);
//	
//    RigidTransformation3D *s2L = new RigidTransformation3D();
//    double* const *const matS2L = (s2L->getMatrix(R_W_ACCESS))->getMatrix(R_W_ACCESS);
//    matS2L[0][3] = -0.5L*B;
//    ReferenceFrame3D *leftCamFrame = new ReferenceFrame3D(newFrame, s2L);
//    leftCamera_ = new OrthographicCamera(params);
//    leftCamera_->setReferenceFrame(leftCamFrame);
//
//    RigidTransformation3D *s2R = new RigidTransformation3D();
//    double* const *const matW2R = (s2R->getMatrix(R_W_ACCESS))->getMatrix(R_W_ACCESS);
//    matW2R[0][3] = 0.5L*B;
//    ReferenceFrame3D *rightCamFrame = new ReferenceFrame3D(newFrame, s2R);
//    rightCamera_ = new OrthographicCamera(params);
//    rightCamera_->setReferenceFrame(rightCamFrame);
//
//	transf_ = new RigidTransformation3D();
//	double* const *const matL2R = transf_->getMatrix(R_W_ACCESS)->getMatrix(R_W_ACCESS);
//    matL2R[0][3] = -B;
}


RectifiedStereoPair::RectifiedStereoPair(RasterImage* leftImg, RasterImage* rightImg, 
						float B, RigidTransformation3D *w2s)
        :   StereoPair()
{
//	FAIL_CONDITION(	leftImg == NULL,
//                    kNullRasterImageError,
//                    "NULL raster image passed as parameter to RectifiedStereoPair::RectifiedStereoPair");
//    FAIL_CONDITION(	rightImg == NULL,
//                    kNullRasterImageError,
//                    "NULL raster image passed as parameter to RectifiedStereoPair::RectifiedStereoPair");
//                    
//    isRectified_ = true;
//
//	IntrinsicParameters *params = 
//            new IntrinsicParameters(leftImg->getWidth(), leftImg->getHeight());
//
//	//	create the reference frame for the stereo system.
//    ReferenceFrame3D* newFrame = new ReferenceFrame3D(w2s);
//	setReferenceFrame(newFrame);
//	
//    RigidTransformation3D *s2L = new RigidTransformation3D();
//    double* const *const matS2L = (s2L->getMatrix(R_W_ACCESS))->getMatrix(R_W_ACCESS);
//    matS2L[0][3] = -0.5L*B;
//    ReferenceFrame3D *leftCamFrame = new ReferenceFrame3D(newFrame, s2L);
//    leftCamera_ = new OrthographicCamera(params);
//    leftCamera_->setReferenceFrame(leftCamFrame);
//
//    RigidTransformation3D *s2R = new RigidTransformation3D();
//    double* const *const matW2R = (s2R->getMatrix(R_W_ACCESS))->getMatrix(R_W_ACCESS);
//    matW2R[0][3] = 0.5L*B;
//    ReferenceFrame3D *rightCamFrame = new ReferenceFrame3D(newFrame, s2R);
//    rightCamera_ = new OrthographicCamera(params);
//    rightCamera_->setReferenceFrame(rightCamFrame);
//
//	transf_ = new RigidTransformation3D();
//	double* const *const matL2R = transf_->getMatrix(R_W_ACCESS)->getMatrix(R_W_ACCESS);
//    matL2R[0][3] = -B;
}

RectifiedStereoPair::RectifiedStereoPair(Camera *leftCam, Camera *rightCam)
{
//	 leftCamera_ = leftCam;
//	 rightCamera_ = rightCam;
}

//  Destructor
RectifiedStereoPair::~RectifiedStereoPair(void)
{
}

