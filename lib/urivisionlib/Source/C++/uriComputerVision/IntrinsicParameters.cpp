/*  NAME:
        IntrinsicParameters.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib IntrinsicParameters class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include "IntrinsicParameters.h"

using namespace std;

using namespace uriVL;

IntrinsicParameters::IntrinsicParameters(const IntrinsicParameters *theParams)
	try	:	Nfx_(theParams->Nfx_),
			Nfy_(theParams->Nfy_)

{
	FAIL_CONDITION(	theParams == NULL,
					kNullParameterError,
					"Attempt to clone a null IntrinsicParameters object");

}
catch(...) {
	FAIL_CONDITION( theParams == NULL,
					kNullParameterError,
					"null pointer passed to IntrinsicParameters copy constructor.");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid parameter passed to IntrinsicParameters copy constructor.");
}

IntrinsicParameters::IntrinsicParameters(int theWidth, int theHeight)
		:	Nfx_(theWidth),
			Nfy_(theHeight)
{
	FAIL_CONDITION( (theWidth < 3) || (theHeight < 3),
					kInvalidParameterError,
					"invalid dimensions passed to IntrinsicParameters constructor.");
	
cout << "IntrinsicParameters int-int constructor" << endl;
}

/*
IntrinsicParameters::IntrinsicParameters(void)
{
cout << "IntrinsicParameters default constructor" << endl;
	hasDistortion_ = false;
	hasUndistortion_ = false;
}
*/

IntrinsicParameters::~IntrinsicParameters(void)
{
}

int IntrinsicParameters::getWidth(void) const
{
	return Nfx_;
}

int IntrinsicParameters::getHeight(void) const
{
	return Nfy_;
}



/*	Normally, the user should always check if the camera intrinsic parameters
 *	object has a distortion function, but in case we are dealing with a silly
 *	person, we're better off having a "straight wire" function.
 */
void IntrinsicParameters::distortPixels(double ux, double uy, double* dx, double* dy) const
{
	*dx = ux;
	*dy = uy;
}
			
/*	Normally, the user should always check if the camera intrinsic parameters
 *	object has a distortion function, but in case we are dealing with a silly
 *	person, we're better off having a "straight wire" function.
 */
void IntrinsicParameters::distortMetric(double ux, double uy, double* dx, double* dy) const
{
	*dx = ux;
	*dy = uy;
}

/*	Normally, the user should always check if the camera intrinsic parameters
 *	object has a distortion function, but in case we are dealing with a silly
 *	person, we're better off having a "straight wire" function.
 */
void IntrinsicParameters::undistortPixel(double dx, double dy, double* ux, double* uy) const
{
	*ux = dx;
	*uy = dy;
}


/*	Normally, the user should always check if the camera intrinsic parameters
 *	object has a distortion function, but in case we are dealing with a silly
 *	person, we're better off having a "straight wire" function.
 */
void IntrinsicParameters::undistortMetric(double dx, double dy, double* ux, double* uy) const
{
	*ux = dx;
	*uy = dy;
}

/*	Normally, this function should be overridden by subclasses that have "real"
 *	dimensions  for sensing element, but in case we are dealing with a silly
 *	person, we're better off having a "straight wire" function.
 */
void IntrinsicParameters::metricToPixel(double mx, double my, double* px, double* py) const
{
    *px = mx;
    *py = my;
}

/*	Normally, this function should be overridden by subclasses that have "real"
 *	dimensions  for sensing element, but in case we are dealing with a silly
 *	person, we're better off having a "straight wire" function.
 */
void IntrinsicParameters::pixelToMetric(double px, double py, double* mx, double* my) const
{
    //  apply scale
    *mx = px;
    *my = py;
}
