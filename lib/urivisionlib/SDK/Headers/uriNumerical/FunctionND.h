/*  NAME:
        FunctionND.h
 
    DESCRIPTION:
        FunctionND public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_FUNCTION_ND_H
#define URIVL_FUNCTION_ND_H

#include <vector>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ColumnVector.h"

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
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT FunctionND
    {
        public:

        	/** Creates a new n-dimensional function defined over ]-infinity, +infinity[^n.
        	 */
            FunctionND(int n);

        	/** Creates a new n-dimensional function defined over the range specified.
        	 *  @param  xMin    lower bound of the domain of definition
        	 *  @param  xMax    upper bound of the domain of definition
        	 */
            FunctionND(int n, const double* xMin, const double* xMax);


            /**
             *	Destructor
             */          
            virtual ~FunctionND(void);


			/** Evaluates the function at the nD point specified.  This is the only version
			 *	of func that <b>must</b> be implemented in the child class.
			 *  @param  x   where the function should be evaluated
			 *  @return     the value of the function at x
			 */
			virtual double func(const double* x) const = 0;

			/** Evaluates the function at the nD point specified.
			 *
			 *	If you are going to use this function a lot you should override it in your custom
			 *	child class because this implementation is (by necessity) very inefficient:  It copies
			 *	all the data into an array before calling the array version of func.
			 *
			 *  @param  x   where the function should be evaluated
			 *  @return     the value of the function at x
			 */
			virtual double func(const std::vector<double>& x) const;
           
            /** Evaluates the function at the nD point specified.
			 *
			 *	If you are going to use this function a lot, you should override it in your custom
			 *	child class because this implementation is (by necessity) very inefficient:  It copies
			 *	all the data into an array before calling the array version of func.
			 *
             *  @param  x   where the function should be evaluated
             *  @return     the value of the function at x
             */
			virtual double func(const ColumnVector* x) const;

			/**  Evaluates the gradient of the function at the point specified.
			 *   If the subclass does not implement its own gradient function, then
			 *   this function will be called instead, which implements the 
			 *   second-order central difference approximation for all directions.
			 *  @param  x   where the function should be evaluated
			 *  @param  gradVect  the gradient of the function at x
			 */
			virtual void gradient(const double* x, double* gradVect) const;

			/**  Evaluates the gradient of the function at the point specified.
			 *
			 *	If you are going to use this function a lot, you should override it in your custom
			 *	child class because this implementation is (by necessity) very inefficient:  It copies
			 *	all the data into an array before calling the array version of gradient.
			 *
             *  @param  x   where the function should be evaluated
             *  @param  gradVect  the gradient of the function at x
             */
			virtual void gradient(const ColumnVector* x, ColumnVector* gradVect) const;

			/**  Evaluates the gradient of the function at the point specified.
             *  @param  x   where the function should be evaluated
             *  @return  the gradient of the function at x
             */
			const ColumnVector* gradient(const ColumnVector* x) const;


			/**  Evaluates the Hessian matrix of the function at the point specified.
             *   If the subclass does not implement its own hessian function, then
             *   this function will be called instead, which implements the 
             *   second-order difference approximation for all directions.
             *  @param  x   where the function should be evaluated
             *  @param  hessianMat  the Hessian matrix of the function at x
             */
			virtual void hessian(const double* x, double*const* hessianMat) const;

			/**  Evaluates the gradient of the function at the point specified.
			 *
			 *	If you are going to use this function a lot, you should override it in your custom
			 *	child class because this implementation is (by necessity) very inefficient:  It copies
			 *	all the data into an array before calling the array version of hessian.
			 *
             *  @param  x   where the function should be evaluated
             *  @param  hess  the Hessian matrix of the function at x
             */
			virtual void hessian(const ColumnVector* x, Matrix* hess) const;

			/**  Evaluates the gradient of the function at the point specified.
             *  @param  x   where the function should be evaluated
             *  @return  the Hessian matrix of the function at x
             */
           const Matrix* hessian(const ColumnVector* x) const;


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
            bool isDefinedAt(const double* x) const;

            /** Indicates whther a point is within the domain of definition of this
             *  function
             *  @param  x   the point to test
             *  @return <code>true</code> if this function is defined at x
             */
            bool isDefinedAt(const std::vector<double>& x) const;

            /** Indicates whther a point is within the domain of definition of this
             *  function
             *  @param  x   the point to test
             *  @return <code>true</code> if this function is defined at x
             */
            bool isDefinedAt(const ColumnVector* x) const;

            /** Returns the lower bounds of this function's domain of definition
             */
            const double* getLowerBound(void) const;
            
            /** Returns the upper bounds of this function's domain of definition
             */
            const double* getUpperBound(void) const;
            
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
            double* xMin_;
            
            /** nDim-sized array of upper bounds of the domain of definition.
			 * Admitedly,
			 *	this is a somewhat naive/restrictive form of domain of definition, but
			 *  let's not forget that our underlying space is based on rectangular images.
             */
            double* xMax_;
			
			/**	Array that stores a copy of the point where the function is evaluated
			 *  (for std::vector and ColumnVector versions of the function)
			 */
			mutable double* xLocal_;
            
			/**	Array that stores another copy of the point where the function is evaluated
			 *  (for std::vector and ColumnVector versions of the function and derivative)
			 */
			mutable double* xLocal2_;
            
			
		private:
		
			/**	Copy constructor
			 *	@param	obj		object to copy
			 */
			FunctionND(const FunctionND& obj);
			
			/**	Copy operator
			 *	@param	obj		object to copy
			 */
			const FunctionND& operator =(const FunctionND& obj);
			
			
			
    };
}   

#endif  //  URIVL_FUNCTION_ND_H
