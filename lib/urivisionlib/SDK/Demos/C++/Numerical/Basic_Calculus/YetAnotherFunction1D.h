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

#ifndef YET_ANOTHER_FUNCTION_1D_H
#define YET_ANOTHER_FUNCTION_1D_H

#include "Function1D.h"

class YetAnotherFunction1D : public uriVL::Function1D
{
	public:
	
		/**	Default constructor
		 */
		YetAnotherFunction1D(void) {}
		
		/**	Destructor
		 */
		~YetAnotherFunction1D(void) {}
		
		/** Evaluates the function at the point specified.
		 *  @param  x   where the function should be evaluated
		 *  @return     the value of the function at x
		 */
		double func(double x) const;
	   
		/** Indicates that the function implements its own first derivative function.
		 *  @return <code>true
		 */
		bool firstDerivativeIsImplemented(void) const
		{
			return false;
		}
		
		/** Indicates whether the function implements its own second derivative function.
		 *  @return <code>true
		 */
		bool secondDerivativeIsImplemented(void) const
		{
			return false;
		}
		
};



#endif YET_ANOTHER_FUNCTION_1D_H
