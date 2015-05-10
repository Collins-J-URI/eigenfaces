/*  NAME:
        FunctionNDSection1D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib FunctionNDSection1D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include "FunctionNDSection1D.h"

using namespace uriVL;

#define DEBUG 0

FunctionNDSection1D::FunctionNDSection1D(const FunctionND* fn)
		try	:   Function1D(),
				//
				fn_(fn),
				nDim_(fn->getDimension()),
				x0_(new double[nDim_]),
				x_(new double[nDim_]),
				u_(new double[nDim_]),
				localGradient_(NULL),
				localHessian_(NULL),
				sectionIsDefined_(false),
				firstDerivativeIsImplemented_(fn->gradientIsImplemented()),
				secondDerivativeIsImplemented_(fn->hessianIsImplemented())
{
	if (firstDerivativeIsImplemented_)
	{
		localGradient_ = new double[nDim_];
	}
	if (secondDerivativeIsImplemented_)
	{
		localHessian_ = new double*[nDim_];
		for (int i=0; i<nDim_; i++)
			localHessian_[i] = new double[nDim_];
			
	}
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by FunctionNDSection1D constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kNumericalError,
					"Unexpected error encountered in FunctionNDSection1D constructor");
}

FunctionNDSection1D::~FunctionNDSection1D(void)
{
    if (x0_ != NULL)
        delete []x0_;
    if (x_ != NULL)
        delete []x_;
    if (u_ != NULL)
        delete []u_;
	if (localGradient_ != NULL)
		delete []localGradient_;
	if (localHessian_ != NULL)
	{
		for (int i=0; i<nDim_; i++)
			delete []localHessian_[i];
		delete localHessian_;
	}
}

void FunctionNDSection1D::setSection(const double* x0, const double* u)
{
    for (int i=0; i<nDim_; i++)
    {
        x0_[i] = x0[i];
        u_[i] = u[i];
    }
        
    sectionIsDefined_ = true;
}

bool FunctionNDSection1D::isDefinedAt(double s) const
{
    FAIL_CONDITION( !sectionIsDefined_,
                    kFunctionNotImplemented,
                    "1D section not properly initialized");
                    
    for (int i=0; i<nDim_; i++)
        x_[i] = x0_[i] + s*u_[i];
        

    return fn_->isDefinedAt(x_);
}


// Implementation of the 1D section
double FunctionNDSection1D::func(double s) const
{
    FAIL_CONDITION( !sectionIsDefined_,
                    kFunctionNotImplemented,
                    "1D section not properly initialized");
                        
    for (int i=0; i<nDim_; i++)
        x_[i] = x0_[i] + s*u_[i];
        

    return fn_->func(x_);
}



// Implementation of the second-order central difference approximation
double FunctionNDSection1D::dfunc(double s) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");
                    
    FAIL_CONDITION( !sectionIsDefined_,
                    kFunctionNotImplemented,
                    "1D section not properly initialized");
                        
    for (int i=0; i<nDim_; i++)
        x_[i] = x0_[i] + s*u_[i];

	if (firstDerivativeIsImplemented_)
	{
		//	get gradient of nd function at x
	}
	else 
	{
		
	}

   return HUGE_VAL;
}

// Implementation of the second-order difference approximation.
double FunctionNDSection1D::d2func(double s) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");
                    
   return HUGE_VAL;
}


#if 0
#pragma mark -
#endif

bool FunctionNDSection1D::firstDerivativeIsImplemented(void) const
{
	return firstDerivativeIsImplemented_;
}

bool FunctionNDSection1D::secondDerivativeIsImplemented(void) const
{
	return secondDerivativeIsImplemented_;
}


#if 0
#pragma mark -
#endif


double FunctionNDSection1D::getLowerBound(void) const
{
    double sMin = HUGE_VALF;

    if (sectionIsDefined_)
    {
		const double* xMin = fn_->getLowerBound();			
		const double* xMax = fn_->getUpperBound();
		
		for (int i=0; i<nDim_; i++)
		{
			if (u_[i] > 0) 
			{
				double s = (xMin[i] - x0_[i]) / u_[i];
				if (s < sMin)
					sMin = s;
			}
			else if (u_[i] < 0)
			{
				double s = (xMax[i] - x0_[i]) / u_[i];
				if (s < sMin)
					sMin = s;
			}
		}
	}
	
    return sMin;
}
            

double FunctionNDSection1D::getUpperBound(void) const
{
    double sMax = -HUGE_VALF;

    if (!sectionIsDefined_)
    {
		const double* xMin = fn_->getLowerBound();			
		const double* xMax = fn_->getUpperBound();
		
		for (int i=0; i<nDim_; i++)
		{
			if (u_[i] > 0)
			{
				double s = (xMax[i] - x0_[i]) / u_[i];
				if (s > sMax)
					sMax = s;
			}
			else if (u_[i] < 0)
			{
				double s = (xMin[i] - x0_[i]) / u_[i];
				if (s > sMax)
					sMax = s;
			}
		}
	}
	
    return sMax;
}
            
