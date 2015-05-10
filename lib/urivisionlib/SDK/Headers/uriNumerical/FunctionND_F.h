/*  NAME:
        FunctionND_F.h
 
    DESCRIPTION:
        FunctionND_F public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_FUNCTION_ND_F_H
#define URIVL_FUNCTION_ND_F_H

#include <vector>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

namespace uriVL
{

	/** Pure virtual parent class for all n-dimensional functions.
	 * 
     * The function's actual implementation will be done in the child class and it will
     * always be <strong>hard-coded</strong>.<p>
     * Note that, contrary to a mathematical function, a <code>Function</code> is always
     * defined on an <i>interval</i> that could be  -&infin; to +&infin;, but cannot have
     * "holes" in it.
	 *
	 *	I have finally decided to remove all versions of func, gradient, and hessian that
	 *	take (pointers to) ColumnVector and Matrix objects as parameters:  Since all classes
	 *	in the linear algebra package only use double, it was a form of perversity to go
	 *	back and form between double and float.
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT FunctionND_F 
    {
        public:

        	/** Creates a new n-dimensional function defined over ]-infinity, +infinity[^n.
        	 */
            FunctionND_F(int n);

        	/** Creates a new n-dimensional function defined over the range specified.
        	 *  @param  xMin    lower bound of the domain of definition
        	 *  @param  xMax    upper bound of the domain of definition
        	 */
            FunctionND_F(int n, const float* xMin, const float* xMax);


            /**
             *	Destructor
             */          
            virtual ~FunctionND_F(void);


			/** Evaluates the function at the nD point specified.  This is the only version
			 *	of func that <b>must</b> be implemented in the child class.
			 *  @param  x   where the function should be evaluated
			 *  @return     the value of the function at x
			 */
           virtual float func(const float* x) const = 0;

			/** Evaluates the function at the nD point specified.
			 *
			 *	If you are going to use this function a lot you should override it in your custom
			 *	child class because this implementation is (by necessity) very inefficient:  It copies
			 *	all the data into an array before calling the array version of func.
			 *
			 *  @param  x   where the function should be evaluated
			 *  @return     the value of the function at x
			 */
           virtual float func(const std::vector<float>& x) const;
           

           /**  Evaluates the gradient of the function at the point specified.
            *   If the subclass does not implement its own gradient function, then
            *   this function will be called instead, which implements the 
            *   second-order central difference approximation for all directions.
            *  @param  x   where the function should be evaluated
            *  @param  gradVect  the gradient of the function at x
            */
           virtual void gradient(const float* x, float* gradVect) const;

			/**  Evaluates the Hessian matrix of the function at the point specified.
			 *   If the subclass does not implement its own gradient function, then
			 *   this function will be called instead, which implements the 
			 *   second-order difference approximation for all directions.
			 *  @param  x   where the function should be evaluated
			 *  @param  hessianMat  the Hessian matrix of the function at x
			 */
			virtual void hessian(const float* x, float** hessianMat) const;


            /** Indicates whether the function implements its own gradient function.
             *  @return <code>true if the function implements its own gradient function
             */
            virtual bool gradientIsImplemented(void) const = 0;
            
            /** Indicates whether the function implements its own Hessian function.
             *  @return <code>true if the function implements its own Hessian function
             */
            virtual bool hessianIsImplemented(void) const = 0;
            
            /** Indicates whther a point is within the domain of definition of this
             *  function
             *  @param  x   the point to test
             *  @return <code>true</code> if this function is defined at x
             */
            bool isDefinedAt(const float* x) const;

            /** Indicates whther a point is within the domain of definition of this
             *  function
             *  @param  x   the point to test
             *  @return <code>true</code> if this function is defined at x
             */
            bool isDefinedAt(const std::vector<float>& x) const;


            /** Returns the lower bounds of this function's domain of definition
             */
            const float* getLowerBound(void) const;
            
            /** Returns the upper bounds of this function's domain of definition
             */
            const float* getUpperBound(void) const;
            
            /** Returns the dimension of this function's domain
             */
            int getDimension(void) const;
            


        protected:

            /** Dimension of the source space of this function
             */
            int nDim_;
            
			/**	Is set true when bounds xMin_ and xMax_ were set for the domain
			 *	of definition of the function.
			 */
			bool isBounded_;
			
            /** nDim-sized array of lower bounds of the domain of definition.
			 * Admitedly,
			 *	this is a somewhat naive/restrictive form of domain of definition, but
			 *  let's not forget that our underlying space is based on rectangular images.
             */
            float* xMin_;
            
            /** nDim-sized array of upper bounds of the domain of definition.
			 * Admitedly,
			 *	this is a somewhat naive/restrictive form of domain of definition, but
			 *  let's not forget that our underlying space is based on rectangular images.
             */
            float* xMax_;
            
			/**	Array that stores a copy of the point where the function is evaluated
			 *  (for std::vector and ColumnVector versions of the function)
			 */
			mutable float* xLocal_;
            
			/**	Array that stores another copy of the point where the function is evaluated
			 *  (for std::vector and ColumnVector versions of the function and derivative)
			 */
			mutable float* xLocal2_;

		private:
		
			/**	Copy constructor
			 *	@param	obj		object to copy
			 */
			FunctionND_F(const FunctionND_F& obj);
			
			/**	Copy operator
			 *	@param	obj		object to copy
			 */
			const FunctionND_F& operator =(const FunctionND_F& obj);
			
    };
}   

#endif  //  FUNCTION_ND_F_H
