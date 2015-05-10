/*  NAME:
        FunctionND.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib FunctionND class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "FunctionND.h"

using namespace std;
using namespace uriVL;

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif

FunctionND::FunctionND(int n)
		try	:	nDim_(n),
				isBounded_(false),
				xMin_(new double[n]),
				xMax_(new double[n]),
				xLocal_(new double[n]),
				xLocal2_(new double[n])
{
    for (int i=0; i<n; i++)
    {
        xMin_[i] = -HUGE_VALF;
        xMax_[i] = HUGE_VALF;
    }
}
catch (...)
{
	FAIL_CONDITION( n <= 0,
					kInvalidParameterError,
					"Invalid dimension passed to FunctionND constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to FunctionND constructor");
}


FunctionND::FunctionND(int n, const double* xMin, const double* xMax)
		try	:	nDim_(n),
				isBounded_(true),
				xMin_(new double[n]),
				xMax_(new double[n]),
				xLocal_(new double[n]),
				xLocal2_(new double[n])
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
						"Null bound vector passed to FunctionND constructor");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid bound vector passed to FunctionND constructor");
	}
}
catch (...)
{
	FAIL_CONDITION( n <= 0,
					kInvalidParameterError,
					"Invalid dimension passed to FunctionND constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to FunctionND constructor");
}


FunctionND::FunctionND(const FunctionND& obj)
			:	nDim_(0),
				isBounded_(false),
				xMin_(NULL),
				xMax_(NULL),
				xLocal_(NULL),
				xLocal2_(NULL)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"FunctionND copy constructor not implemented");
}
			


FunctionND::~FunctionND(void)
{
    delete []xMin_;
    delete []xMax_;
	delete [] xLocal_;
	delete [] xLocal2_;
}

const FunctionND& FunctionND::operator =(const FunctionND& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"FunctionND copy operator not implemented");
	return *this;
}
			

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark func (lousy) implementations
//----------------------------------------------------------
#endif

double FunctionND::func(const std::vector<double>& x) const
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
		for (int k=0; k<nDim_; k++)
			xLocal_[k] = x[k];
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
						"Unexpected error while copying input data in in FunctionND::func");
						
	}
	

	//-----------------------------------------------------
	//	Finally evaluate the function at x
	//-----------------------------------------------------
	double val = 0;
	try
	{
		val  = func(xLocal_);
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
	
	return val;
}


double FunctionND::func(const ColumnVector* x) const
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
	//	make a copy of x into a 1D double array
	//-----------------------------------------------------
	try
	{
		const double *const* xV = x->getMatrix(); 

		for (int k=0; k<nDim_; k++)
		{
			xLocal_[k] = xV[k][0];
		}
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
						"Unexpected error copying data in FunctionND::func");
						
	}

	//-----------------------------------------------------
	//	Finally evaluate the function at x
	//-----------------------------------------------------
	double val = 0;
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
						"FunctionND_F::func");
						
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
void FunctionND::gradient(const double* x, double* gradVect) const
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
			e.appendToMessage("called by FunctionND::gradient");
			throw e;
		}
		catch(...)
		{
			FAIL_CONDITION( true,
							kNumericalError,
							"Unexpected error in FunctionND::gradient");
							
		}
		
		FAIL_CONDITION( !isDefined,
						kDataRangeError,
						"x not within domain of definition in FunctionND::gradient");

	}


	//-----------------------------------------------------
	//	Make local copies of x
	//-----------------------------------------------------
	double* xPlus = xLocal_;
	double* xMinus = xLocal2_;
	
	try
	{
		//	for each variable of x --> derivative
		for (int k=0; k<nDim_; k++)
		{
			//	compute a small discrete displacement along kth direction
			const double DX = fabs(x[k]) > LARGE_RELATIVE_TOLERANCE ? 
								fabs(x[k])*LARGE_RELATIVE_TOLERANCE
								: 
								LARGE_RELATIVE_TOLERANCE;
			const double TWO_OVER_DX = 2.0 / DX;

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



void FunctionND::gradient(const ColumnVector* x, ColumnVector* grad) const
{
    FAIL_CONDITION( x == NULL,
                    kNullParameterError,
                    "Null parameter passed to FunctionND::gradient");
    FAIL_CONDITION( grad == NULL,
                    kNullParameterError,
                    "Null parameter passed to FunctionND::gradient");


	//-----------------------------------------------------
	//	make a copy of x into a 1D array of double
	//-----------------------------------------------------
	double* xLocal = xLocal_;
	try
	{
		const double *const* xV = x->getMatrix(); 

		for (int k=0; k<nDim_; k++)
		{
			xLocal[k] = xV[k][0];
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
						"Unexpected error copying data in FunctionND::gradient");
						
	}

	//-----------------------------------------------------
	//	compute the gradient
	//-----------------------------------------------------
	double* gradLocal = xLocal2_;
	try
	{
		gradient(xLocal, gradLocal);
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

	//-----------------------------------------------------
	//	copy back gradient values
	//-----------------------------------------------------
	try
	{
		double *const* gradVect = grad->getMatrix(R_W_ACCESS);

		for (int k=0; k<nDim_; k++)
		{
			gradVect[k][0] = gradLocal[k];
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
						"Unexpected error copying back data in FunctionND::gradient");
						
	}
}

const ColumnVector* FunctionND::gradient(const ColumnVector* x) const
{
	ColumnVector* grad = NULL;
	
	try
	{
		grad = new ColumnVector(nDim_);
		
		gradient(x, grad);
	}
	catch (ErrorReport& e)
	{
		delete grad;
		grad = NULL;
		e.appendToMessage("called by FunctionND::gradient");
		throw e;
	}
	catch(...)
	{
		delete grad;
		grad = NULL;
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in FunctionND::gradient");
						
	}

	return grad;
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark Hessian implementations
//----------------------------------------------------------
#endif


void FunctionND::hessian(const double* x, double*const* hessianMat) const
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}


void FunctionND::hessian(const ColumnVector* x, Matrix* hess) const
{
    FAIL_CONDITION( x == NULL,
                    kNullParameterError,
                    "Null parameter passed to FunctionND::hessian");
    FAIL_CONDITION( hess == NULL,
                    kNullParameterError,
                    "Null parameter passed to FunctionND::hessian");

	//	make a copy of x into a 1D double array
	try
	{
		const double *const* xV = x->getMatrix(); 
		for (int k=0; k<nDim_; k++)
			xLocal_[k] = xV[k][0];
		
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FunctionND::hessian");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error while copying data in FunctionND::hessian");
						
	}

	//	compute the hessian
	try
	{
		double *const* hessMat = hess->getMatrix(R_W_ACCESS);
		hessian(xLocal_, hessMat);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FunctionND::hessian");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in FunctionND::hessian");
						
	}
}

const Matrix* FunctionND::hessian(const ColumnVector* x) const
{
	Matrix* hess = NULL;
	
	try
	{
		hess = new Matrix(nDim_, nDim_);
		
		hessian(x, hess);
	}
	catch (ErrorReport& e)
	{
		delete hess;
		hess = NULL;
		e.appendToMessage("called by FunctionND::hessian");
		throw e;
	}
	catch(...)
	{
		delete hess;
		hess = NULL;
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in FunctionND::hessian");
						
	}

	return hess;
}

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark getter functions
//----------------------------------------------------------
#endif



int FunctionND::getDimension(void) const
{
    return nDim_;
}
              
			  
bool FunctionND::isDefinedAt(const double* x) const
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
		FAIL_CONDITION( x == NULL,
						kNullParameterError,
						"Null pointer passed to FunctionND::isDefinedAt");
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error encountered in FunctionND::isDefinedAt");
	}
	
    return isDefined;       
}


bool FunctionND::isDefinedAt(const vector<double>& x) const
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
						"Unexpected error encountered in FunctionND::isDefinedAt");
	}
	
    return isDefined;       
}

bool FunctionND::isDefinedAt(const ColumnVector* x) const
{
	bool isDefined = true;
	
	try
	{
		const double *const* xv = x->getMatrix();
		
		for (int i=0; (i<nDim_) && isDefined; i++)
			if ((xv[i][0] < xMin_[i]) || (xv[i][0] > xMax_[i]))
				isDefined = false;

	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FunctionND::isDefinedAt");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( x == NULL,
						kNullParameterError,
						"Null pointer passed to FunctionND::isDefinedAt");
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error encountered in FunctionND::isDefinedAt");
	}
	
    return isDefined;       
}


const double* FunctionND::getLowerBound(void) const
{
    return xMin_;
}
            

const double* FunctionND::getUpperBound(void) const
{
    return xMax_;
}
            
