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
#ifndef URIVL_FUNCTION_1D_H
#define URIVL_FUNCTION_1D_H

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
    class URIVL_EXPORT Function1D
    {
        public:

        	/** Creates a new function defined over the set of real numbers,
        	 *  ]-infinity, +infinity[
        	 */
            Function1D(void);

        	/** Creates a new function defined over the interval ]xMin, xMax[
        	 *  @param  xMin    lower bound of the domain of definition
        	 *  @param  xMax    upper bound of the domain of definition
        	 */
            Function1D(double xMin, double xMax);

            /**
             *	Destructor
             */          
            virtual ~Function1D(void);


            /** Evaluates the function at the point specified.
             *  @param  x   where the function should be evaluated
             *  @return     the value of the function at x
             */
			virtual double func(double x) const = 0;
           
			/**  Evaluates the first derivative of the function at the point specified.
             *   If the subclass does not implement its own derivative, then
             *   this function will be called instead, which implements the 
             *   second-order central difference approximation
             *   @param  x   where the function should be evaluated
             *   @return     the derivative of the function at x
             */
			virtual double dfunc(double x) const;

			/**  Evaluates the first derivative of the function at the point specified.
             *   If the subclass does not implement its own derivative, then
             *   this function will be called instead, which implements an n-th order 
             *   Richardson interpolation.
             *   @param  x   where the function should be evaluated
             *   @param  n   the degree of the Richardson approximation (n=1 is the basic 
             *   @return     the derivative of the function at x
             */
			double dfunc(double x, int n) const;

            /**  Evaluates the second derivative of the function at the point specified.
             *   If the subclass does not implement its own derivative, then
             *   this function will be called instead, which implements the 
             *   second-order difference approximation.
             *   @param  x   where the function should be evaluated
             *   @return     the derivative of the function at x
             */
			virtual double d2func(double x) const;

            
            /** returns the lower bound of this function's domain of definition
             */
            double getLowerBound(void) const;
            
            /** returns the upper bound of this function's domain of definition
             */
            double getUpperBound(void) const;
            
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
            bool isDefinedAt(double x) const;

            
        private:

            /** lower bound of this function's domain of definition
             */
            double xMin_;
            
            /** upper bound of this function's domain of definition
             */
            double xMax_;
			
			/**	True if finite bounds were set for the function
			 */
			bool isBounded_;
            
    };
}   

#endif  //  FUNCTION_1D_H
