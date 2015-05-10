/*  NAME:
        PolynomialFunction1D.h
 
    DESCRIPTION:
        PolynomialFunction1D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_POLYNOMIAL_FUNCTION_1D_H
#define URIVL_POLYNOMIAL_FUNCTION_1D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

#include "Function1D.h"

namespace uriVL
{

	/** Class that implements 1-dimensional polynomial functions and their derivatives.
	 *	I guess we could store the arrays of coefficients for the first and second
	 *	derivatives of the function.  Right now this is not really needed, so... pass.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT PolynomialFunction1D : public Function1D
    {
        public:

        	/** Creates a new function defined over the set of real numbers,
        	 *  ]-infinity, +infinity[
        	 *  @param  n   degree of the polynomial function
        	 *  @param  a   array of n+1 coefficients of the polynomial function (array index corresponds
        	 *                  to power of the monomial)
        	 */
            PolynomialFunction1D(int n, const double* a);

        	/** Creates a new function defined over the interval ]xMin, xMax[
        	 *  @param  xMin    lower bound of the domain of definition
        	 *  @param  xMax    upper bound of the domain of definition
        	 *  @param  n   degree of the polynomial function
        	 *  @param  a   array of n+1 coefficients of the polynomial function (array index corresponds
        	 *                  to power of the monomial)
        	 */
            PolynomialFunction1D(int n, const double* a, const double xMin, const double xMax);

            /**
             *	Destructor
             */          
            ~PolynomialFunction1D(void);


            /** Evaluates the function at the point specified.
             *  @param  x   where the function should be evaluated
             *  @return     the value of the function at x
             */
           double func(double x) const;
          
           
           /**  Evaluates the first derivative of the function at the point specified.
			*  @param  x   where the function should be evaluated
            *  @return     the first derivative of the function at x
            */
           double dfunc(double x) const;

           /**  Evaluates the second derivative of the function at the point specified.
			*  @param  x   where the function should be evaluated
            *  @return     the second derivative of the function at x
            */
           double d2func(double x) const;

           /**  Evaluates the nth derivative of the function at the point specified.
			*  @param  x   where the function should be evaluated
			*  @param n		the order of the derivative
            *  @return     the nth derivative of the function at x
            */
           double dnfunc(double x, int n) const;


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
				return true;
			}
            
        private:

            /** degree of the polynomial
             */
            int n_;
            
            /** array of coefficients of the polynomial and of all of its derivatives
             */
            double** a_;
            
            
            /** Initializes the array of coefficients of the polynomial and of its derivatives
             */
            void initCoeffArray_(int n, const double* a);

            
            
    };
}   

#endif  //  POLYNOMIAL_FUNCTION_1D_H
