/*  NAME:
        Function1D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Function1D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include "Function1D.h"

using namespace uriVL;

Function1D::Function1D(void)
		:	xMin_(-HUGE_VALF),
			xMax_(HUGE_VALF),
			isBounded_(false)
{
}


Function1D::Function1D(double xMin, double xMax)
		:	xMin_(xMin),
			xMax_(xMax),
			isBounded_(true)
{
}

Function1D::~Function1D(void)
{
}


#if 0
#pragma mark -
#endif

bool Function1D::isDefinedAt(double x) const
{
    return (x >= xMin_) && (x <= xMax_);
}


// Implementation of the second-order central difference approximation
double Function1D::dfunc(double x) const
{
    FAIL_CONDITION( isBounded_ && !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in Function1D::dfunc");

	const double DX = fabs(x) > LARGE_RELATIVE_TOLERANCE ? fabs(x)*LARGE_RELATIVE_TOLERANCE : LARGE_RELATIVE_TOLERANCE;

	return (0.5*(func(x+ DX) - func(x-DX))/DX);
}

// Implementation of the Richardson interpolation algorithm
double Function1D::dfunc(double x, int n) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
                    
    FAIL_CONDITION( isBounded_ && !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in Function1D::dfunc");
                    
   return HUGE_VAL;
}

// Implementation of the second-order difference approximation.
double Function1D::d2func(double x) const
{
    FAIL_CONDITION( isBounded_&& !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in Function1D::d2func");
                    
	const double DX = fabs(x) > LARGE_RELATIVE_TOLERANCE ? fabs(x)*LARGE_RELATIVE_TOLERANCE : LARGE_RELATIVE_TOLERANCE;

	return (func(x+ DX) - 2.0*func(x) + func(x-DX)) / (DX*DX);
}

            
#if 0
#pragma mark -
#endif
            
double Function1D::getLowerBound(void) const
{
    return xMin_;
}
            

double Function1D::getUpperBound(void) const
{
    return xMax_;
}
            
