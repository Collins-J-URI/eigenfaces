/*  NAME:
        OtherFunction1D.cpp

    DESCRIPTION:
        Header for demonstration subclass of Function1D.  This function overrides the
		(approximate) derivative function of the parent class.
		Implements x |--->  x^2 * cos(x)

    COPYRIGHT:
        (c) 2013-2014, 3D Group for Interactive Visualization
					University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef OTHER_FUNCTION_1D_H
#define OTHER_FUNCTION_1D_H

#include "Function1D.h"

class OtherFunction1D : public uriVL::Function1D
{
	public:
	
		/**	Default constructor
		 */
		OtherFunction1D(void) {}
		
		/**	Destructor
		 */
		~OtherFunction1D(void) {}
		
		/** Evaluates the function at the point specified.
		 *  @param  x   where the function should be evaluated
		 *  @return     the value of the function at x
		 */
		double func(double x) const;
	   
	   /**  Evaluates the first derivative of the function at the point specified.
		*   This function overrides the one defined in the parent class
		*   @param  x   where the function should be evaluated
		*   @return     the derivative of the function at x
		*/
	   double dfunc(double x) const;

		/** Indicates that the function implements its own first derivative function.
		 *  @return <code>true
		 */
		bool firstDerivativeIsImplemented(void) const
		{
			return true;
		}
		
		/** Indicates whether the function implements its own second derivative function.
		 *  @return <code>true
		 */
		bool secondDerivativeIsImplemented(void) const
		{
			return false;
		}
		
};



#endif OTHER_FUNCTION_1D_H
