/*  NAME:
        Quaternion.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Quaternion class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "Quaternion.h"

using namespace std;
using namespace uriVL;


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

Quaternion::Quaternion(void)
	:	isNormalized_(true)
{
    quat_[0] = quat_[1] = quat_[2] = 0.L;
    quat_[3] = 1.L;
}

Quaternion::Quaternion(Quaternion *theQuat)
		try	:	isNormalized_(theQuat->isNormalized_)
{
    FAIL_CONDITION( theQuat == NULL,
                    kNullParameterError,
                    "Null Quaternion  passed to copy constructor"); 
    for (int i=0; i<4; i++)
        quat_[i] = theQuat->quat_[i];
    isNormalized_ = theQuat->isNormalized_;
}
catch (...)
{
    FAIL_CONDITION( theQuat == NULL,
                    kNullParameterError,
                    "Null parameter in Quaternion copy constructor");
    FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "Invalid parameter in Quaternion copy constructor");
}


Quaternion::Quaternion(double angle, const double* axis)
	:	isNormalized_(true)
{
    FAIL_CONDITION( axis == NULL,
                    kNullParameterError,
                    "Null rotation axis array passed to Quaternion constructor"); 
    //  I don't trust the user to send me a normalized vector
    double norm = sqrt(axis[1]*axis[1] + axis[2]*axis[2] + axis[3]*axis[3]);
    FAIL_CONDITION( norm == 0.L,
                    kInvalidParameterError,
                    "Zero rotation axis vector passed to Quaternion constructor"); 

    const double scale = sin(0.5L * angle) / norm;
	for (int i=1; i<4; i++)
		quat_[i] = static_cast<float>(axis[i-1] * scale);

    quat_[0] = static_cast<float>(cos(0.5 * angle));
}


Quaternion::Quaternion(const double* quat)
	:	isNormalized_(false)
{
    FAIL_CONDITION( quat == NULL,
                    kNullParameterError,
                    "Null array  passed to Quaternion constructor"); 
	for (int i=0; i<4; i++)
		quat_[i] = static_cast<float>(quat[i]);
	
	isNormalized_ = fabs((	quat[0]*quat[0] + quat[1]*quat[1] + 
							quat[2]*quat[2] +quat[3]*quat[3] 
							- 1.L)) < 1.E-6;
}


Quaternion::~Quaternion(void)
{
}


#if 0
//----------------------------------------
#pragma mark Normalization
//----------------------------------------
#endif


void Quaternion::normalize(void)
{
    if (!isNormalized_)     
    {
        double norm = sqrt( quat_[0]*quat_[0] + 
                            quat_[1]*quat_[1] + quat_[2]*quat_[2] + quat_[3]*quat_[3]);

        if ((norm != 0.L) && (norm != 1.L))
            for (int i=0; i<4; i++)
                quat_[i] /= norm;

        isNormalized_ = true;            
    }
}

bool Quaternion::isNormalized(void)
{
    return isNormalized_;
}

