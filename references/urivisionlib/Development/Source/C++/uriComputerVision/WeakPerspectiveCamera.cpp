/*  NAME:
        WeakPerspectiveCamera.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib WeakPerspectiveCamera class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "WeakPerspectiveCamera.h"

using namespace uriVL;

WeakPerspectiveCamera::WeakPerspectiveCamera(IntrinsicParameters *params, 
                                             RigidTransformation3D *transf)
	    :  Camera(params, transf)
{
    FAIL_CONDITION( true,
                    kComputerVisionError,
                    "Class not open for business yet"); 
}

WeakPerspectiveCamera::~WeakPerspectiveCamera(void)
{
}

void WeakPerspectiveCamera::getImagePoint_(double X, double Y, double Z, double* x, double* y)
{
    FAIL_CONDITION( true,
                    kComputerVisionError,
                    "Class not open for business yet"); 
}

void WeakPerspectiveCamera::getImagePoint_(float X, float Y, float Z, float* x, float* y)
{
    FAIL_CONDITION( true,
                    kComputerVisionError,
                    "Class not open for business yet"); 
}
