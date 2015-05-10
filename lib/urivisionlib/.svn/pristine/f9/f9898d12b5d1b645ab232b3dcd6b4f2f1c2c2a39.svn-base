/*  NAME:
        OrthographicCamera.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib OrthographicCamera class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "OrthographicCamera.h"


using namespace uriVL;

OrthographicCamera::OrthographicCamera(IntrinsicParameters *params, RigidTransformation3D *transf)
  :  Camera(params, transf)
{
}

OrthographicCamera::~OrthographicCamera(void)
{
}

void OrthographicCamera::getImagePoint_(double X, double Y, double Z, double* x, double* y)
{
	*x = X;
	*y = Y;
}

void OrthographicCamera::getImagePoint_(float X, float Y, float Z, float* x, float* y)
{
	*x = X;
	*y = Y;
}

/*
void OrthographicCamera::setCameraForRendering(void)
{
}
*/
