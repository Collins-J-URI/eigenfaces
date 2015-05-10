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
#ifndef SIMPLE_FUNCTION_ND_H
#define SIMPLE_FUNCTION_ND_H

#include "FunctionND.h"

/** Examle of a 2D function to test the optimizer with.
 * 
 *  @author jean-yves herve', 3D Group, 
 *                              Department of Computer Science and Statistics,
 *                              University of Rhode Island
 */
class SimpleFunctionND : public uriVL::FunctionND
{
	public:

		/** Creates a new function defined over the set of real numbers,
		 *  ]-infinity, +infinity[
		 */
		SimpleFunctionND(void);

		/**
		 *	Destructor
		 */          
		~SimpleFunctionND(void);


		/** Evaluates the function at the point specified.
		 *  @param  x   where the function should be evaluated
		 *  @return     the value of the function at x
		 */
	   double func(const double *x) const;
	   
		/** Indicates whether the function implements its own gradient function.
		 *  @return <code>true if the function implements its own gradient function
		 */
		bool gradientIsImplemented(void) const
		{
			return false;
		}
		
		/** Indicates whether the function implements its own Hessian function.
		 *  @return <code>true if the function implements its own Hessian function
		 */
		bool hessianIsImplemented(void) const
		{
			return false;
		}
            

		
};

#endif  //  SIMPLE_FUNCTION_ND_H
