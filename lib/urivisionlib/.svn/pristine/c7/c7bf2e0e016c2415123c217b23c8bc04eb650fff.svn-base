/*  NAME:
        FunctionNDSection1D_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib FunctionNDSection1D_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include "FunctionNDSection1D_F.h"

using namespace uriVL;

#define DEBUG 0

FunctionNDSection1D_F::FunctionNDSection1D_F(const FunctionND_F* fn)
		try	:   Function1D_F(),
				//
				fn_(fn),
				nDim_(fn->getDimension()),
				x0_(new float[nDim_]),
				x_(new float[nDim_]),
				u_(new float[nDim_]),
				localGradient_(NULL),
				localHessian_(NULL),
				sectionIsDefined_(false),
				firstDerivativeIsImplemented_(fn->gradientIsImplemented()),
				secondDerivativeIsImplemented_(fn->hessianIsImplemented())
{
	if (firstDerivativeIsImplemented_)
	{
		localGradient_ = new float[nDim_];
	}
	if (secondDerivativeIsImplemented_)
	{
		localHessian_ = new float*[nDim_];
		for (int i=0; i<nDim_; i++)
			localHessian_[i] = new float[nDim_];
			
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


FunctionNDSection1D_F::~FunctionNDSection1D_F(void)
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

void FunctionNDSection1D_F::setSection(const float* x0, const float* u)
{
    for (int i=0; i<nDim_; i++)
    {
        x0_[i] = x0[i];
        u_[i] = u[i];
    }
        
    sectionIsDefined_ = true;
}

bool FunctionNDSection1D_F::isDefinedAt(float s) const
{
    FAIL_CONDITION( !sectionIsDefined_,
                    kFunctionNotImplemented,
                    "1D section not properly initialized");
                    
    for (int i=0; i<nDim_; i++)
        x_[i] = x0_[i] + s*u_[i];        

    return fn_->isDefinedAt(x_);
}


// Implementation of the 1D section
float FunctionNDSection1D_F::func(float s) const
{
    FAIL_CONDITION( !sectionIsDefined_,
                    kFunctionNotImplemented,
                    "1D section not properly initialized");
                        
    for (int i=0; i<nDim_; i++)
        x_[i] = x0_[i] + s*u_[i];
        
    return fn_->func(x_);
}



// Implementation of the second-order central difference approximation
float FunctionNDSection1D_F::dfunc(float s) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");
                    
   return HUGE_VALF;
}

// Implementation of the second-order difference approximation.
float FunctionNDSection1D_F::d2func(float s) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");
                    
   return HUGE_VALF;
}


#if 0
#pragma mark -
#endif

bool FunctionNDSection1D_F::firstDerivativeIsImplemented(void) const
{
	return firstDerivativeIsImplemented_;
}

bool FunctionNDSection1D_F::secondDerivativeIsImplemented(void) const
{
	return secondDerivativeIsImplemented_;
}


#if 0
#pragma mark -
#endif



float FunctionNDSection1D_F::getLowerBound(void) const
{
    float sMin = HUGE_VALF;

    if (sectionIsDefined_)
    {
		const float* xMin = fn_->getLowerBound();			
		const float* xMax = fn_->getUpperBound();

		for (int i=0; i<nDim_; i++)
		{
			if (u_[i] > 0) 
			{
				float s = (xMin[i] - x0_[i]) / u_[i];
				if (s < sMin)
					sMin = s;
			}
			else if (u_[i] < 0)
			{
				float s = (xMax[i] - x0_[i]) / u_[i];
				if (s < sMin)
					sMin = s;
			}
		}
	}
	
    return sMin;
}
            

float FunctionNDSection1D_F::getUpperBound(void) const
{
    float sMax = -HUGE_VALF;

    if (sectionIsDefined_)
    {
		const float* xMin = fn_->getLowerBound();			
		const float* xMax = fn_->getUpperBound();

		for (int i=0; i<nDim_; i++)
		{
			if (u_[i] > 0)
			{
				float s = (xMax[i] - x0_[i]) / u_[i];
				if (s > sMax)
					sMax = s;
			}
			else if (u_[i] < 0)
			{
				float s = (xMin[i] - x0_[i]) / u_[i];
				if (s > sMax)
					sMax = s;
			}
		}
	}
	
    return sMax;
}
  
			
					  
          
