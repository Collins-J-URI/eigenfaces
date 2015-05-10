/*  NAME:
        IntrinsicParameters_pinhole.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib IntrinsicParameters_pinhole class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "IntrinsicParameters_pinhole.h"

using namespace uriVL;

IntrinsicParameters_pinhole::IntrinsicParameters_pinhole(const IntrinsicParameters_pinhole *theParams)
	try	:	IntrinsicParameters(theParams),
			//
			fx_(theParams->fx_),
			fy_(theParams->fy_),
			Ox_(theParams->Ox_),
			Oy_(theParams->Oy_)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by IntrinsicParameters_pinhole constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( theParams == NULL,
					kNullParameterError,
					"Null pointer passed to IntrinsicParameters_pinhole constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_pinhole constructor");
}


IntrinsicParameters_pinhole::IntrinsicParameters_pinhole(int theWidth, int theHeight, 
														double originX, double originY, 
														double theFx, double theFy)
	try	:	IntrinsicParameters(theWidth, theHeight),
			//
			fx_(theFx),
			fy_(theFy),
			Ox_(originX),
			Oy_(originY)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by IntrinsicParameters_pinhole constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_pinhole constructor");
}


IntrinsicParameters_pinhole::IntrinsicParameters_pinhole(int theWidth, int theHeight,
														double theFx, double theFy)
	try	:	IntrinsicParameters(theWidth, theHeight),
			//
			fx_(theFx),
			fy_(theFy),
			Ox_(0.5f * (theWidth - 1)),
			Oy_(0.5f * (theHeight - 1))

{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by IntrinsicParameters_pinhole constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to IntrinsicParameters_pinhole constructor");
}


IntrinsicParameters_pinhole::~IntrinsicParameters_pinhole(void)
{
}

double IntrinsicParameters_pinhole::getFocalLengthX(void) const
{
	return fx_;
}

double IntrinsicParameters_pinhole::getFocalLengthY(void) const
{
	return fy_;
}


double IntrinsicParameters_pinhole::getOriginX(void) const
{
	return Ox_;
}

double IntrinsicParameters_pinhole::getOriginY(void) const
{
	return Oy_;
}


bool IntrinsicParameters_pinhole::hasDistortion(void) const
{
	return false;
}

bool IntrinsicParameters_pinhole::hasUndistortion(void) const
{
	return false;
}


/*	Normally, this function should be overridden by subclasses that have "real"
 *	dimensions  for sensing element, but in case we are dealing with a silly
 *	person, we're better off having a "straight wire" function.
 *	Here, we simply translate the coordinates
 */
void IntrinsicParameters_pinhole::metricToPixel(double mx, double my, double* px, double* py) const
{
    *px = mx + Ox_;
    *py = my + Oy_;
}

/*	Normally, this function should be overridden by subclasses that have "real"
 *	dimensions  for sensing element, but in case we are dealing with a silly
 *	person, we're better off having a "straight wire" function.
 */
void IntrinsicParameters_pinhole::pixelToMetric(double px, double py, double* mx, double* my) const
{
    //  apply scale
    *mx = px - Ox_;
    *my = py - Oy_;
}
