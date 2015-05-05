//
//  Function1D.h
//
//
//  Created by Christopher Glasz on 3/28/15.
//
//

//=================================
// include guard
#ifndef ____Function1D_included__
#define ____Function1D_included__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <tgmath.h>

namespace csc450Lib_calc_base {
    /**
     * Function1D defines a mathematical function, and provides a derivitive
     *	approximation is there is no exact derivative defined.
     */
    class Function1D {
        
    private:
        
        /// Simple approximation of a function's derivative
        float phi(float x, float h) const;
        
        /// Higher-order estimate of a function's derivative by use of
        ///	Richardson Extrapolation
        float approxd(float x, float h, int n, int m) const;
        
    public:
        
        /// Constructor does nothing
        Function1D(void);
        
        /// Nor does the destructor
        virtual ~Function1D(void);
        
        /// Returns the value of the function at some passed in x. This method
        ///	must be overriden by child classes
        virtual float func(float x) const = 0;
        
        /// Returns the value of the derivative of the function at some passed
        ///	in x. If not overridden, this method returns an approximation
        ///	reached using Richardson Extrapolation.
        virtual float dfunc(float x) const;
        
        /// Returns boolean true if dfunc has been overridden with an exact
        ///	derivative. This method must be overridden by child classes.
        virtual bool isExactDerivativeDefined(void) = 0;
        
    };
}
#endif /* defined(____Function1D_included__) */