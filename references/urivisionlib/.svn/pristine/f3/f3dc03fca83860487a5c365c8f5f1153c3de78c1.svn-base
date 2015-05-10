/*  NAME:
        Function1D_F_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Function1D_F_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include "Function1D_F.h"

using namespace uriVL;

Function1D_F::Function1D_F(void)
		:	xMin_(-HUGE_VALF),
			xMax_(HUGE_VALF),
			isBounded_(false)
{
}


Function1D_F::Function1D_F(float xMin, float xMax)
		:	xMin_(xMin),
			xMax_(xMax),
			isBounded_(false)
{
}

Function1D_F::~Function1D_F(void)
{
}


#if 0
#pragma mark -
#endif

bool Function1D_F::isDefinedAt(float x) const
{
    return (x >= xMin_) && (x <= xMax_);
}


// Implementation of the second-order central difference approximation
float Function1D_F::dfunc(float x) const
{
    FAIL_CONDITION( isBounded_ && !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in Function1D_F::dfunc");

	const float DX = fabsf(x) > LARGE_RELATIVE_TOLERANCE_F ? fabsf(x)*LARGE_RELATIVE_TOLERANCE_F : LARGE_RELATIVE_TOLERANCE_F;

	return (0.5f*(func(x+ DX) - func(x-DX))/DX);
}

// Implementation of the Richardson interpolation algorithm
float Function1D_F::dfunc(float x, int n) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
                    
    FAIL_CONDITION( isBounded_ && !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in Function1D_F::dfunc");
                    
   return HUGE_VALF;
}

// Implementation of the second-order difference approximation.
float Function1D_F::d2func(float x) const
{
    FAIL_CONDITION( isBounded_ && !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in Function1D_F::d2func");
                    
	const float DX = fabsf(x) > LARGE_RELATIVE_TOLERANCE_F ? fabsf(x)*LARGE_RELATIVE_TOLERANCE_F : LARGE_RELATIVE_TOLERANCE_F;

	return (func(x+ DX) - 2.f*func(x) + func(x-DX)) / (DX*DX);
}


#if 0
#pragma mark -
#endif
            
            
float Function1D_F::getLowerBound(void) const
{
    return xMin_;
}
            

float Function1D_F::getUpperBound(void) const
{
    return xMax_;
}
  
            
