/*  NAME:
        Function1D.h
 
    DESCRIPTION:
        Function1D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef SIMPLE_FUNCTION_1D_H
#define SIMPLE_FUNCTION_1D_H

#include "Function1D_F.h"

/** Examle of a 1D function to test the optimizer with.
 * 
 *  @author jean-yves herve', 3D Group, 
 *                              Department of Computer Science and Statistics,
 *                              University of Rhode Island
 */
class SimpleFunction1D : public uriVL::Function1D_F
{
	public:

		/** Creates a new function defined over the set of real numbers,
		 *  ]-infinity, +infinity[
		 */
		SimpleFunction1D(void);

		/**
		 *	Destructor
		 */          
		~SimpleFunction1D(void);


		/** Evaluates the function at the point specified.
		 *  @param  x   where the function should be evaluated
		 *  @return     the value of the function at x
		 */
	   float func(float x) const;
	   
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

#endif  //  SIMPLE_FUNCTION_1D_H
