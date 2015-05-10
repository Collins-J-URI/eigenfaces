/*  NAME:
        MyFunction1D.cpp

    DESCRIPTION:
        Demonstration subclass of Function1D.  This function overrides the
		(approximate) first and second derivative functions of the parent class.
		Implements x |--->  x^2 * cos(x)


    COPYRIGHT:
        (c) 2013-2014, 3D Group for Interactive Visualization
					University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "MyFunction1D.h"

using namespace uriVL;

double MyFunction1D::func(double x) const
{
	return x * x * cos(x);
}
	   
double MyFunction1D::dfunc(double x) const
{
	return 2.0 * x * cos(x) - x * x * sin(x);
}

double MyFunction1D::dfunc2(double x) const
{
	return (2.0 - x * x) * cos(x) - 4 * x * sin(x);
}
