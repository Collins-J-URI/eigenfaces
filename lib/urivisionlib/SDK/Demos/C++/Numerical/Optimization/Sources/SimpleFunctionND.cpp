/*  NAME:
        SimpleFunctionND.cpp
 
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

#include "SimpleFunctionND.h"

using namespace std;
using namespace uriVL;

//=============================================================================
//	Function Implementations
//=============================================================================


SimpleFunctionND::SimpleFunctionND(void)
    :   FunctionND(2)
{
}


SimpleFunctionND::~SimpleFunctionND(void)
{
}

double SimpleFunctionND::func(const double* x) const
{
	//-----------------------------------------------------
	//	Optional:  If you want to play it safe
	//-----------------------------------------------------
	bool isDefined = false;
	try 
	{
		isDefined = isDefinedAt(x);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by SimpleFunctionND::func");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in SimpleFunctionND::func");
						
	}
	
    FAIL_CONDITION( !isDefined,
                    kDataRangeError,
                    "x not within domain of definition in SimpleFunctionND::func");


    return (1.0 + (x[0] - 1.0)*(x[0] - 1.0) + (x[1] + 2.0)*(x[1] + 2.0));
}


