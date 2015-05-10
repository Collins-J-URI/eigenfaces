/*  NAME:
        PerspectiveCamera.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib PerspectiveCamera class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "PerspectiveCamera.h"

using namespace uriVL;

PerspectiveCamera::PerspectiveCamera(IntrinsicParameters_pinhole *params,
                                     RigidTransformation3D *transf)
	    :  Camera(params, transf)
{
}

PerspectiveCamera::~PerspectiveCamera(void)
{
}

void PerspectiveCamera::getImagePoint_(double X, double Y, double Z, double* x, double* y)
{
    IntrinsicParameters_pinhole *params = dynamic_cast<IntrinsicParameters_pinhole*>(intrinsic_);
	*x = params->getOriginX() - params->getFocalLengthX() * X / Z;
	*y = params->getOriginY() - params->getFocalLengthY() * Y / Z;
}

void PerspectiveCamera::getImagePoint_(float X, float Y, float Z, float* x, float* y)
{
    IntrinsicParameters_pinhole *params = dynamic_cast<IntrinsicParameters_pinhole*>(intrinsic_);
	*x = static_cast<float>(params->getOriginX() - params->getFocalLengthX() * X / Z);
	*y = static_cast<float>(params->getOriginY() - params->getFocalLengthY() * Y / Z);
}


/*
void PerspectiveCamera::setCameraForRendering(void)
{
}
*/
