/*  NAME:
        SimpleFunction1D.cpp
 
    DESCRIPTION:
        Example of a 1D function to optimize
 
    COPYRIGHT:
        (c) 2003-2015, 3D Group for Interactive Visualization
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

#include "SimpleFunction1D.h"

using namespace std;
using namespace uriVL;

//=============================================================================
//	Function Implementations
//=============================================================================


SimpleFunction1D::SimpleFunction1D(void)
    :   Function1D_F()
{
}


SimpleFunction1D::~SimpleFunction1D(void)
{
}

float SimpleFunction1D::func(float x) const
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
		e.appendToMessage("called by SimpleFunction1D::func");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in SimpleFunction1D::func");
						
	}
	
    FAIL_CONDITION( !isDefined,
                    kDataRangeError,
                    "x not within domain of definition in SimpleFunction1D::func");


	//-----------------------------------------------------
	//	Then evaluate the function at x
	//-----------------------------------------------------
    return (1.0f + (cosf(0.2f*x)/sqrtf(fabsf(x+12.f)+1.f)))*(x/6.f - 3.f)*(x/6.f - 3.f) -
                sqrtf(fabsf(0.5f*x + 1.f));
}

