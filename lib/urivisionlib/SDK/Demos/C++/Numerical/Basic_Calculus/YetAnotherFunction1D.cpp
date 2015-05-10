/*  NAME:
        YetAnotherFunction1D.cpp

    DESCRIPTION:
        Header for demonstration subclass of Function1D.  This function does not override
		the (approximate) 1st and 2nd derivative functions of the parent class.
		Implements x |--->  x^4 + 2 x^3 + 3 x^2 + 4 x + 5

    COPYRIGHT:
        (c) 2013-2014, 3D Group for Interactive Visualization
					University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "YetAnotherFunction1D.h"

using namespace uriVL;

//x^4 + 2 x^3 + 3 x^2 + 4 x + 5
double YetAnotherFunction1D::func(double x) const
{
	return 5.0 + x * (4.0 + x * (3.0 + x * (2.0 + x)));
}
	   
