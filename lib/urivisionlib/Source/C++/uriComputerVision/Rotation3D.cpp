/*  NAME:
        Rotation3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Rotation3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Rotation3D.h"

using namespace uriVL;

Rotation3D::Rotation3D(void)
            :   ref_(ROLL_PITCH_YAW_REF),
                //
                mat_(NULL),
                quat_(NULL),
                rollPitchYaw_(new double[3]),
                euler_(NULL),
                //
                matrixIsUpToDate_(false),
                quaternionIsUpToDate_(false),
                rollPitchYawIsUpToDate_(true),
                eulerIsUpToDate_(false)
{
    rollPitchYaw_[0] = rollPitchYaw_[1] = rollPitchYaw_[2] = 0.L;
    
    euler_ = NULL;
    mat_ = NULL;
    quat_ = NULL;
    
    rollPitchYawIsUpToDate_ = true;
    quaternionIsUpToDate_ =  matrixIsUpToDate_ = eulerIsUpToDate_ = false;
}


Rotation3D::Rotation3D(const Rotation3D *theRot)
        try:    ref_(theRot->ref_),
                //
                mat_(NULL),
                quat_(NULL),
                rollPitchYaw_(NULL),
                euler_(NULL),
                //
                matrixIsUpToDate_(theRot->matrixIsUpToDate_),
                rollPitchYawIsUpToDate_(theRot->rollPitchYawIsUpToDate_),
                quaternionIsUpToDate_(theRot->quaternionIsUpToDate_),
                eulerIsUpToDate_(theRot->eulerIsUpToDate_)
{
    switch (ref_)
    {
        case MATRIX_REF:
            if (quaternionIsUpToDate_)
                quat_ = new Quaternion(theRot->quat_);
            else
                quat_ = NULL;
                
            if (rollPitchYawIsUpToDate_)
            {
                rollPitchYaw_ = new double[3];
                rollPitchYaw_[0] = theRot->rollPitchYaw_[0];   
                rollPitchYaw_[1] = theRot->rollPitchYaw_[1];   
                rollPitchYaw_[2] = theRot->rollPitchYaw_[2];   
            }
            else
                rollPitchYaw_ = NULL;

            if (eulerIsUpToDate_)
            {
                euler_ = new double[3];
                euler_[0] = theRot->euler_[0];   
                euler_[1] = theRot->euler_[1];   
                euler_[2] = theRot->euler_[2];   
            }
            else
                euler_ = NULL;
                
            mat_ = new Matrix(theRot->mat_);
            break;
            
        case QUATERNION_REF:
            if (matrixIsUpToDate_)
                mat_ = new Matrix(theRot->mat_);
            else
                mat_ = NULL;
                                
            if (rollPitchYawIsUpToDate_)
            {
                rollPitchYaw_ = new double[3];
                rollPitchYaw_[0] = theRot->rollPitchYaw_[0];   
                rollPitchYaw_[1] = theRot->rollPitchYaw_[1];   
                rollPitchYaw_[2] = theRot->rollPitchYaw_[2];   
            }
            else
                rollPitchYaw_ = NULL;

            if (eulerIsUpToDate_)
            {
                euler_ = new double[3];
                euler_[0] = theRot->euler_[0];   
                euler_[1] = theRot->euler_[1];   
                euler_[2] = theRot->euler_[2];   
            }
            else
                euler_ = NULL;
                
            quat_ = new Quaternion(theRot->quat_);
            break;
            
        case ROLL_PITCH_YAW_REF:
            if (matrixIsUpToDate_)
                mat_ = new Matrix(theRot->mat_);
            else
                mat_ = NULL;
                                
            if (quaternionIsUpToDate_)
                quat_ = new Quaternion(theRot->quat_);
            else
                quat_ = NULL;
            
            if (eulerIsUpToDate_)
            {
                euler_ = new double[3];
                euler_[0] = theRot->euler_[0];   
                euler_[1] = theRot->euler_[1];   
                euler_[2] = theRot->euler_[2];   
            }
            else
                euler_ = NULL;
                
            rollPitchYaw_ = new double[3];
            rollPitchYaw_[0] = theRot->rollPitchYaw_[0];   
            rollPitchYaw_[1] = theRot->rollPitchYaw_[1];   
            rollPitchYaw_[2] = theRot->rollPitchYaw_[2];   
            break;
            
        case EULER_ANGLES_REF:
            if (matrixIsUpToDate_)
                mat_ = new Matrix(theRot->mat_);
			else
			    mat_ = NULL;
                                
			if (quaternionIsUpToDate_)
			    quat_ = new Quaternion(theRot->quat_);
			else
			    quat_ = NULL;
			    
            if (rollPitchYawIsUpToDate_)
            {
                rollPitchYaw_ = new double[3];
                rollPitchYaw_[0] = theRot->rollPitchYaw_[0];   
                rollPitchYaw_[1] = theRot->rollPitchYaw_[1];   
                rollPitchYaw_[2] = theRot->rollPitchYaw_[2];   
            }
            else
			    rollPitchYaw_ = NULL;
			
            euler_ = new double[3];
            euler_[0] = theRot->euler_[0];   
            euler_[1] = theRot->euler_[1];   
            euler_[2] = theRot->euler_[2];   
            break;
          
    }
}
catch (...)
{
    FAIL_CONDITION( theRot == NULL,
                    kNullTransformation3D,
                    "Null Rotation3D passed as argument to Rotation3D cloner constructor");
    
    FAIL_CONDITION( true,
                    kMiscError,
                    "Unknown error in Rotation3D cloner constructor");
    
    
    
}

Rotation3D::Rotation3D(RotationRef dataType, const double* rotData)
            :   ref_(dataType),
                //
                mat_(NULL),
                quat_(NULL),
                rollPitchYaw_(NULL),
                euler_(NULL),
                //
                matrixIsUpToDate_(false),
                quaternionIsUpToDate_(false),
                rollPitchYawIsUpToDate_(false),
                eulerIsUpToDate_(false)
{
    switch (dataType)
    {
        case MATRIX_REF:
            if (rotData != NULL) 
            {
                mat_ = new Matrix(3, 3);
                double* const* matData = mat_->getMatrix(R_W_ACCESS);
                for (int j=0, l=0; j<3; j++)
                    for (int i=0; i<3; i++)
                       matData[i][j] = rotData[l++];

            }
            else
			{
                mat_ = Matrix::identityMatrix(3); 
			}
			          
            matrixIsUpToDate_ = true;
            quaternionIsUpToDate_ =  rollPitchYawIsUpToDate_ = eulerIsUpToDate_ = false;
            break;

        case QUATERNION_REF:
            quat_ = new Quaternion(rotData);
            quaternionIsUpToDate_ = true;
            rollPitchYawIsUpToDate_ =  matrixIsUpToDate_ = eulerIsUpToDate_ = false;
            break;

        case ROLL_PITCH_YAW_REF:
            rollPitchYaw_ = new double[3];
            rollPitchYaw_[0] = rotData[0];
            rollPitchYaw_[1] = rotData[1];
            rollPitchYaw_[2] = rotData[2];
            rollPitchYawIsUpToDate_ = true;
            quaternionIsUpToDate_ =  matrixIsUpToDate_ = eulerIsUpToDate_ = false;
            break;

        case EULER_ANGLES_REF:
            euler_ = new double[3];
            euler_[0] = rotData[0];
            euler_[1] = rotData[1];
            euler_[2] = rotData[2];
            eulerIsUpToDate_ = true;
            quaternionIsUpToDate_ =  matrixIsUpToDate_ = rollPitchYawIsUpToDate_ = false;
            break;
    }
}


Rotation3D::Rotation3D(const Rotation3D& obj)
            :   ref_(ROLL_PITCH_YAW_REF),
                //
                mat_(NULL),
                quat_(NULL),
                rollPitchYaw_(NULL),
                euler_(NULL),
                //
                matrixIsUpToDate_(false),
                quaternionIsUpToDate_(false),
                rollPitchYawIsUpToDate_(false),
                eulerIsUpToDate_(false)
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "Rotation3D copy constructor not implemented");
}
            
 


Rotation3D::~Rotation3D(void)
{
    if  (rollPitchYaw_ != NULL)
        delete []rollPitchYaw_;
    if  (euler_ != NULL)
        delete []euler_;
}

#if 0
#pragma mark -
#endif

const Rotation3D& Rotation3D::operator = (const Rotation3D& obj)
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "Rotation3D copy operator not implemented");
    return *this;
}
 

const Matrix *Rotation3D::getMatrix(void) const
{
    if (!matrixIsUpToDate_)
        updateMatrix_();
        
    return mat_;
}

Matrix *Rotation3D::getMatrix(ReadWriteStatus rwa)
{
    if (!matrixIsUpToDate_)
        updateMatrix_();

    ref_ = MATRIX_REF;
    matrixIsUpToDate_ = true;
    quaternionIsUpToDate_ = rollPitchYawIsUpToDate_ = eulerIsUpToDate_ = false;
        
    return mat_;
}

const Quaternion *Rotation3D::getQuarternion(void) const
{
    if (!quaternionIsUpToDate_)
        updateQuarternion_();
        
    return quat_;
}

Quaternion *Rotation3D::getQuarternion(ReadWriteStatus rwa)
{
    if (!quaternionIsUpToDate_)
        updateQuarternion_();

    ref_ = QUATERNION_REF;
    quaternionIsUpToDate_ = true;
    matrixIsUpToDate_ = rollPitchYawIsUpToDate_ = eulerIsUpToDate_ = false;
        
    return quat_;
}

const double* Rotation3D::getRollPitchYawAngles(void) const
{
    if (!rollPitchYawIsUpToDate_)
        updateRollPitchYaw_();
        
    return rollPitchYaw_;
}

double* Rotation3D::getRollPitchYawAngles(ReadWriteStatus rwa)
{
    if (!rollPitchYawIsUpToDate_)
        updateRollPitchYaw_();
        
    rollPitchYawIsUpToDate_ = true;
    matrixIsUpToDate_ = quaternionIsUpToDate_= eulerIsUpToDate_ = false;

    return rollPitchYaw_;
}

const double* Rotation3D::getEulerAngles(void) const
{
    if (!eulerIsUpToDate_)
        updateEulerAngles_();
        
    return euler_;
}

double* Rotation3D::getEulerAngles(ReadWriteStatus rwa)
{
    if (!eulerIsUpToDate_)
        updateEulerAngles_();
        
    eulerIsUpToDate_ = true;
    matrixIsUpToDate_ = rollPitchYawIsUpToDate_ = quaternionIsUpToDate_ = false;

    return euler_;
}

#if 0
#pragma mark -
#endif

void Rotation3D::updateMatrix_(void) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not ready for business");
    matrixIsUpToDate_ = true;
}        

void Rotation3D::updateQuarternion_(void) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not ready for business");
    quaternionIsUpToDate_ = true;
}        

void Rotation3D::updateRollPitchYaw_(void) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not ready for business");
    rollPitchYawIsUpToDate_ = true;
}        

void Rotation3D::updateEulerAngles_(void) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not ready for business");
    eulerIsUpToDate_ = true;
}        

void Rotation3D::orthogonalizeMatrix_(void) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not ready for business");
}        
