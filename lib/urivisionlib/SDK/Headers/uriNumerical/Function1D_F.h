/*  NAME:
        Function1D_D.h
 
    DESCRIPTION:
        Function1D_D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_FUNCTION_1D_F_H
#define URIVL_FUNCTION_1D_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

namespace uriVL
{

	/** Pure virtual parent class for all 1-dimensional functions.
     * The function's actual implementation will be done in the child class and it will
     * always be <strong>hard-coded</strong>.<p>
     * Note that, contrary to a mathematical function, a <code>Function</code> is always
     * defined on an <i>interval</i> that could be  -&infin; to +&infin;, but cannot have
     * "holes" in it.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT Function1D_F
    {
        public:

        	/** Creates a new function defined over the set of real numbers,
        	 *  ]-infinity, +infinity[
      	     */
            Function1D_F(void);

        	/** Creates a new function defined over the interval ]xMin, xMax[
        	 *  @param  xMin    lower bound of the domain of definition
        	 *  @param  xMax    upper bound of the domain of definition
        	 */
            Function1D_F(float xMin, float xMax);

            /**
             *	Destructor
             */          
            virtual ~Function1D_F(void);


            /** Evaluates the function at the point specified.
             *  @param  x   where the function should be evaluated
             *  @return     the value of the function at x
             */
			virtual float func(float x) const = 0;
           
			/**  Evaluates the first derivative of the function at the point specified.
			 *   If the subclass does not implement its own derivative, then
			 *   this function will be called instead, which implements the 
			 *   second-order central difference approximation
			 *   @param  x   where the function should be evaluated
			 *   @return     the derivative of the function at x
			 */
			virtual float dfunc(float x) const;

			/**  Evaluates the first derivative of the function at the point specified.
             *   If the subclass does not implement its own derivative, then
             *   this function will be called instead, which implements an n-th order 
             *   Richardson interpolation.
             *   @param  x   where the function should be evaluated
             *   @param  n   the degree of the Richardson approximation (n=1 is the basic 
             *   @return     the derivative of the function at x
             */
			virtual float dfunc(float x, int n) const;

            /**  Evaluates the second derivative of the function at the point specified.
             *   If the subclass does not implement its own derivative, then
             *   this function will be called instead, which implements the 
             *   second-order difference approximation.
             *   @param  x   where the function should be evaluated
             *   @return     the derivative of the function at x
             */
			virtual float d2func(float x) const;

            /** returns the lower bound of this function's domain of definition
             */
            float getLowerBound(void) const;
            
            /** returns the upper bound of this function's domain of definition
             */
            float getUpperBound(void) const;
            
            /** Indicates whether the function implements its own first derivative function.
             *  @return <code>true if the function implements its own derivative function
             */
            virtual bool firstDerivativeIsImplemented(void) const = 0;
            
            /** Indicates whether the function implements its own second derivative function.
             *  @return <code>true if the function implements its own derivative function
             */
            virtual bool secondDerivativeIsImplemented(void) const = 0;
            
            /** Indicates whther a point is within the domain of definition of this
             *  function
             *  @param  x   the point to test
             *  @return <code>true</code> if this function is defined at x
             */
            bool isDefinedAt(float x) const;

            
        protected:

            /** lower bound of this function's domain of definition
             */
            float xMin_;
            
            /** upper bound of this function's domain of definition
             */
            float xMax_;
            
			/**	True if finite bounds were set for the function
			 */
			bool isBounded_;
            
            
    };
}   

#endif  //  FUNCTION_1D_F_H
