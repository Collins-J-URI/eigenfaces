/*  NAME:
        FunctionNDSection1D_F.h
 
    DESCRIPTION:
        FunctionND_F public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_FUNCTION_ND_SECTION_1D_F_H
#define URIVL_FUNCTION_ND_SECTION_1D_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Function1D_F.h"
#include "FunctionND_F.h"

namespace uriVL
{

	/** Class defining a 1D section of an n-dimensional function.
	 *
	 *  A 1D section of an nD function f is defined as the function<p>
	 *                f1:  s --> f(x0 + s.v),<p>
	 *  where x0 is an n-dimensional point in f's domain, v is an n-dimensional
	 *  vector/direction, and s is a real number.
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT FunctionNDSection1D_F : public Function1D_F
    {
        public:

        	/** Creates a new section of an n-dimensional function.  The domain of 
        	 *  definition of the section is defined by that of the nD function.
        	 *  @param  f   the n-dimensional function this one is a section of
        	 */
            FunctionNDSection1D_F(const FunctionND_F* fn);

            /**
             *	Destructor
             */          
            virtual ~FunctionNDSection1D_F(void);

            /** Sets the origin and direction of the section
             *  @param  x0  the origin of the section
             *  @param  u   the direction of the section
             */
            void setSection(const float* x0, const float* u);
            

            /** Evaluates the function at the point specified.
             *  @param  x   where the function should be evaluated
             *  @return     the value of the function at x
             */
			float func(float x) const;
           
            /** Evaluates the function at the point specified.  If the nD function implements
			 *	its gradient, then that value will be used to compute the derivative.  Otherwise,
			 *	we use a second-order central difference approximation.
             *  @param  x   where the function should be evaluated
             *  @return     the value of the function at x
             */
			float dfunc(float x) const;
           
            /**	Evaluates the second derivative of the function at the point specified.
             *	If the nD function implements its hessian, then that value will be used 
			 *	to compute the second derivative.  Otherwise,
			 *	we use a second-order central difference approximation.
             *  @param  x   where the function should be evaluated
             *  @return     the derivative of the function at x
             */
			float d2func(float x) const;


            /** Indicates whether the function implements its own first derivative function.
             *  @return <code>true if the function implements its own derivative function
             */
            bool firstDerivativeIsImplemented(void) const;
            
            /** Indicates whether the function implements its own second derivative function.
             *  @return <code>true if the function implements its own derivative function
             */
            bool secondDerivativeIsImplemented(void) const;
            

            /** Indicates whther a point is within the domain of definition of this
             *  function
             *  @param  x   the point to test
             *  @return <code>true</code> if this function is defined at x
             */
            virtual bool isDefinedAt(float x) const;
            

            /** returns the lower bound of this function's domain of definition
             */
            float getLowerBound(void) const;
            
            /** returns the upper bound of this function's domain of definition
             */
            float getUpperBound(void) const;
            


        protected:

            /** The n-dimensional function this one is a 1D section of
             */
            const FunctionND_F* fn_;
            
            /** Dimension of the function
             */
            int nDim_;
            
            /** The origin of the section
             */
            float* x0_;
            
            /** Where the base n-dimensional function is evaluated, x = x0 + s.u
             */
            float* x_;
            
            /** The direction of the section (not neccessarily normalized)
             */
            float* u_;
			
			/**	If the nD function implementes its gradient, then used to store
			 *	the gradient at local point
			 */
			float* localGradient_;
			
			/**	If the nD function implementes its hessian, then used to store
			 *	the hessian at local point
			 */
			float** localHessian_;
			
            
            /** True when a section has been defined
             */
            bool sectionIsDefined_;
			
			/**	so that we don't have to keep asking the question to the nD function
			 */
			bool firstDerivativeIsImplemented_;

			/**	so that we don't have to keep asking the question to the nD function
			 */
			bool secondDerivativeIsImplemented_;
			
		private:
		
           /**	Copy constructor
			 *	@param	obj	object to copy
			 */
			FunctionNDSection1D_F(const FunctionNDSection1D_F& obj);
			
            /**	Copy operator
			 *	@param	obj	object to copy
			 */
			const FunctionNDSection1D_F operator = (const FunctionNDSection1D_F& obj);
		
            
    };
}   

#endif  //  FUNCTION_ND_SECTION_1D_F_H
