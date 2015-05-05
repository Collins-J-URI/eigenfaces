//
//  PolyFunction1D.h
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

//=================================
// include guard
#ifndef ____PolyFunction1D_included__
#define ____PolyFunction1D_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Function1D.h"

namespace csc450Lib_calc_base {
    
    /**
     * PolyFunction1D defines a polynomial function of the form
     *		c[0]*x^(n-1) + c[1]*x^(n-2) + ... + c[n-2]*x + c[n-1]
     *	given some array of size n of coefficients c, and provides an exact
     *	derivative of the function
     */
    class PolyFunction1D : public Function1D {
        
    protected:
        
        /// Number of coefficients in the polynomial; on greater than the
        ///	degree of the polynomial
        int numCoeffs;
        
        /// Degree of the polynomial
        int degree;
        
        /// Array of floating point coefficients.
        const float *coefficients;
        
    public:
        
        /// Constructor initializes number of coefficients, and points the local
        ///	array of coefficients to the passed in address
        PolyFunction1D(int nbCoeffs, const float * coeff);
        
        /// Destructor deallocates memory. coefficients does not have to be
        ///	deallocated because it is only a pointer to some other array
        virtual ~PolyFunction1D(void);
        
        /// Returns value of the function at some passed in x using Horner's
        ///	method. This depends entirely on the coefficients given in the
        ///	constructor, and so should not be overridden in child classes
        float func(float x) const;
        
        /// Returns the value of the derivative of the function at some passed
        ///	in x, using a variation on Horner's method
        float dfunc(float x) const;
        
        /// Exact derivative is indeed defined, so return true
        bool isExactDerivativeDefined();
    };
}

#endif /* defined(____PolyFunction1D_included__) */