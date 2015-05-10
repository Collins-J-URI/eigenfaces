/*  NAME:
        IntrinsicParameters_CCD.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib IntrinsicParameters_CCD class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include "IntrinsicParameters_CCD.h"

using namespace std;
using namespace uriVL;

IntrinsicParameters_CCD::IntrinsicParameters_CCD(const IntrinsicParameters_CCD* theParams)
	try	:	IntrinsicParameters(theParams),
			//
			Ncx_(theParams->Ncx_),
			Ncy_(theParams->Ncy_),
			dx_(theParams->dx_),
			dy_(theParams->dy_),
			dpx_(theParams->dpx_),
			dpy_(theParams->dpy_),
			m2pixX_(theParams->dpx_),
			m2pixY_(theParams->dpy_)			
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by IntrinsicParameters_CCD constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( theParams == NULL,
					kNullParameterError,
					"Null pointer passed to IntrinsicParameters_CCD constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_CCD constructor");
}


IntrinsicParameters_CCD::IntrinsicParameters_CCD(int theWidth, int theHeight)
	try	:	IntrinsicParameters(theWidth, theHeight),
			//
			Ncx_(theWidth),
			Ncy_(theHeight),
			dx_(1.0E-6),		//  set arbitrary sensor element dimensions
			dy_(1.0E-6),			//  set arbitrary sensor element dimensions
			dpx_(1.0E-6),		//  set arbitrary sensor element dimensions
			dpy_(1.0E-6),		//  set arbitrary sensor element dimensions
			m2pixX_(1.f / dpx_),
			m2pixY_(1.f / dpy_)			
{
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_CCD constructor");
}



IntrinsicParameters_CCD::IntrinsicParameters_CCD(int theWidth, int theHeight, 
												int nbSensorsX, int nbSensorsY, 
												double dx, double dy)
	try	:	IntrinsicParameters(theWidth, theHeight),
			//
			Ncx_(nbSensorsX),
			Ncy_(nbSensorsY),
			dx_(dx),
			dy_(dy),
			dpx_(nbSensorsX * dx / theWidth),
			dpy_(nbSensorsY * dy / theHeight),
			m2pixX_(1.f / dpx_),
			m2pixY_(1.f / dpy_)
{
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_CCD constructor");
}



IntrinsicParameters_CCD::~IntrinsicParameters_CCD(void)
{
}


bool IntrinsicParameters_CCD::hasDistortion(void) const
{
	return false;
}

bool IntrinsicParameters_CCD::hasUndistortion(void) const
{
	return false;
}




void IntrinsicParameters_CCD::metricToPixel(double mx, double my, double* px, double* py) const
{
    //  apply metric-to-pixel scale
    *px = m2pixX_ * mx;
    *py = m2pixY_ * my;
}

void IntrinsicParameters_CCD::pixelToMetric(double px, double py, double* mx, double* my) const
{
    //  apply pixel to metric scale
    *mx = dpx_ * px;
    *my = dpy_ * py;
}

