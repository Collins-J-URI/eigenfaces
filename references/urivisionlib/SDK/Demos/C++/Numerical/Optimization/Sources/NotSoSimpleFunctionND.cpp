/*  NAME:
        NotSoSimpleFunctionND.cpp
 
    DESCRIPTION:
        Example of a 2D function to optimize
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|	
+--------------------------------------------------------------------------*/

#include "NotSoSimpleFunctionND.h"

using namespace std;
using namespace uriVL;

//=============================================================================
//	Function Implementations
//=============================================================================


NotSoSimpleFunctionND::NotSoSimpleFunctionND(void)
    :   FunctionND(2)
{
}


NotSoSimpleFunctionND::~NotSoSimpleFunctionND(void)
{
}

double NotSoSimpleFunctionND::func(const double* x) const
{
    return (2.0*cos(1.0 - 0.2*x[0]) + sin(1.0 + 0.4*x[1]) + 1.0) *
                    (sin(-0.3*x[0] + 0.1*x[1] + 1.0) + sin(-0.3*x[0] - 0.2*x[1] -1.0));
}

//	Here I decided to override the "vector" version of the
//	function to make it more efficient (no copying of data)
double NotSoSimpleFunctionND::func(const vector<double>& x) const
{
    return (2.0*cos(1.0 - 0.2*x[0]) + sin(1.0 + 0.4*x[1]) + 1.0) *
		(sin(-0.3*x[0] + 0.1*x[1] + 1.0) + sin(-0.3*x[0] - 0.2*x[1] -1.0));
}
