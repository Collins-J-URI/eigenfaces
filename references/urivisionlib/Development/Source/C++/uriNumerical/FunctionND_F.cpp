/*  NAME:
        FunctionND_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib FunctionND_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "FunctionND_F.h"

using namespace std;
using namespace uriVL;


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif


FunctionND_F::FunctionND_F(int n)
		try	:	nDim_(n),
				isBounded_(false),
				xMin_(new float[n]),
				xMax_(new float[n]),
				xLocal_(new float[n]),
				xLocal2_(new float[n])
{
    for (int i=0; i<n; i++)
    {
        xMin_[i] = static_cast<float>(-HUGE_VALF);
        xMax_[i] = static_cast<float>(HUGE_VALF);
    }
}
catch (...)
{
	FAIL_CONDITION( n <= 0,
					kInvalidParameterError,
					"Invalid dimension passed to FunctionND_F constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to FunctionND_F constructor");
}


FunctionND_F::FunctionND_F(int n, const float* xMin, const float* xMax)
		try	:	nDim_(n),
				isBounded_(true),
				xMin_(new float[n]),
				xMax_(new float[n]),
				xLocal_(new float[n]),
				xLocal2_(new float[n])
{
	try
	{
		for (int i=0; i<n; i++)
		{
			xMin_[i] = xMin[i];
			xMax_[i] = xMax[i];
		}
	}
	catch(...)
	{
		FAIL_CONDITION( (xMin == NULL) || (xMax == NULL),
						kNullParameterError,
						"Null bound vector passed to FunctionND_F constructor");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid bound vector passed to FunctionND_F constructor");
	}
}
catch (...)
{
	FAIL_CONDITION( n <= 0,
					kInvalidParameterError,
					"Invalid dimension passed to FunctionND_F constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to FunctionND_F constructor");
}


FunctionND_F::FunctionND_F(const FunctionND_F& obj)
			:	nDim_(0),
				isBounded_(false),
				xMin_(NULL),
				xMax_(NULL),
				xLocal_(NULL),
				xLocal2_(NULL)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"FunctionND_F copy constructor not implemented");
}
			

FunctionND_F::~FunctionND_F(void)
{
    delete []xMin_;
    delete []xMax_;
	delete [] xLocal_;
	delete [] xLocal2_;
}

const FunctionND_F& FunctionND_F::operator =(const FunctionND_F& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"FunctionND_F copy operator not implemented");
	return *this;
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark func (lousy) implementations
//----------------------------------------------------------
#endif

float FunctionND_F::func(const std::vector<float>& x) const
{
	//-----------------------------------------------------
	//	First check that the function is defined at x
	//-----------------------------------------------------
	if (isBounded_)
	{
		bool isDefined = false;
		try 
		{
			isDefined = isDefinedAt(x);
		}
		catch (ErrorReport& e)
		{
			e.appendToMessage("called by FunctionND::func");
			throw e;
		}
		catch(...)
		{
			FAIL_CONDITION( true,
							kNumericalError,
							"Unexpected error in FunctionND::func");
							
		}
		
		FAIL_CONDITION( !isDefined,
						kDataRangeError,
						"x not within domain of definition in FunctionND::func");

	}

	//-----------------------------------------------------
	//	Make local copy of x
	//-----------------------------------------------------
	try
	{
		for (unsigned int k=0; k<x.size(); k++)
			xLocal_[k] = x[k];
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FunctionND_F::func");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error while copying input data in in FunctionND_F::func");
						
	}
	

	//-----------------------------------------------------
	//	Finally evaluate the function at x
	//-----------------------------------------------------
	float val = 0;
	try
	{
		val  = func(xLocal_);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FunctionND_F::func");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in FunctionND_F::func");
						
	}
	
	return val;
}
           


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark gradient implementations
//----------------------------------------------------------
#endif



// Implementation of the Richardson interpolation algorithm
void FunctionND_F::gradient(const float* x, float* gradVect) const
{
	//-----------------------------------------------------
	//	First check that the function is defined at x
	//-----------------------------------------------------
	if (isBounded_)
	{
		bool isDefined = false;
		try 
		{
			isDefined = isDefinedAt(x);
		}
		catch (ErrorReport& e)
		{
			e.appendToMessage("called by FunctionND_F::gradient");
			throw e;
		}
		catch(...)
		{
			FAIL_CONDITION( true,
							kNumericalError,
							"Unexpected error in FunctionND_F::gradient");
							
		}
		
		FAIL_CONDITION( !isDefined,
						kDataRangeError,
						"x not within domain of definition in FunctionND_F::gradient");

	}


	//-----------------------------------------------------
	//	Make local copies of x
	//-----------------------------------------------------
	float* xPlus = xLocal_;
	float* xMinus = xLocal2_;
	
	try
	{
		//	for each variable of x --> derivative
		for (int k=0; k<nDim_; k++)
		{
			//	compute a small discrete displacement along kth direction
			const float DX = fabsf(x[k]) > LARGE_RELATIVE_TOLERANCE_F ? 
								fabsf(x[k])*LARGE_RELATIVE_TOLERANCE_F
								: 
								LARGE_RELATIVE_TOLERANCE_F;
			const float TWO_OVER_DX = 2.f / DX;

			//	initialize the "plus" and "minus" arrays
			for (int i=0; i<nDim_; i++)
			{
				if (i!= k)
				{
					xPlus[k] = xMinus[k] = x[k];
				}
				else 
				{
					xPlus[k] = x[k] + DX;
					xMinus[k] = x[k] - DX;
				}
			}
			
			gradVect[k] = TWO_OVER_DX * (func(xPlus) - func(xMinus));
		}
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FunctionND::gradient");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in FunctionND::gradient");
						
	}
}




#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark Hessian implementations
//----------------------------------------------------------
#endif


void FunctionND_F::hessian(const float* x, float** hessianMat) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark getter functions
//----------------------------------------------------------
#endif


            
int FunctionND_F::getDimension(void) const
{
    return nDim_;
}
              
bool FunctionND_F::isDefinedAt(const float* x) const
{
	bool isDefined = true;
	
	try
	{
		for (int i=0; (i<nDim_) && isDefined; i++)
			if ((x[i] < xMin_[i]) || (x[i] > xMax_[i]))
				isDefined = false;

	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error encountered in FunctionND_F::isDefinedAt");
	}
	
    return isDefined;       
}


bool FunctionND_F::isDefinedAt(const vector<float>& x) const
{
	bool isDefined = true;
	
	try
	{
		for (int i=0; (i<nDim_) && isDefined; i++)
			if ((x[i] < xMin_[i]) || (x[i] > xMax_[i]))
				isDefined = false;

	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error encountered in FunctionND_F::isDefinedAt");
	}
	
    return isDefined;       
}


const float* FunctionND_F::getLowerBound(void) const
{
    return xMin_;
}
            

const float* FunctionND_F::getUpperBound(void) const
{
    return xMax_;
}
