/*  NAME:
        NonlinearOptimizer1D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib NonlinearOptimizer1D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "NonlinearOptimizer1D.h"

using namespace uriVL;

#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructor
//-------------------------------------------------------------------
#endif

NonlinearOptimizer1D::NonlinearOptimizer1D(void)
		:	fD_(NULL),
			fF_(NULL)
{

}

NonlinearOptimizer1D::NonlinearOptimizer1D(const Function1D* f)
		:	fD_(f),
			fF_(NULL)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizer1D constructor");
	
}


NonlinearOptimizer1D::NonlinearOptimizer1D(const Function1D_F* f)
		:	fD_(NULL),
			fF_(f)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizer1D constructor");
	
}



NonlinearOptimizer1D::~NonlinearOptimizer1D(void)
{

}


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark setters and getters
//-------------------------------------------------------------------
#endif

void NonlinearOptimizer1D::setFunction(const Function1D* f)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizer1D constructor");

	//	don't waste time
	if (f != fD_)
	{
		//	call the private setting function (possibly overriden by child class)
		setFunction_(f);
		
		//	sets the variables
		fF_ = NULL;
		fD_ = f;
	}
}


void NonlinearOptimizer1D::setFunction(const Function1D_F* f)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizer1D constructor");
	
	//	don't waste time
	if (f != fF_)
	{
		//	call the private setting function (possibly overriden by child class)
		setFunction_(f);
		
		//	sets the variables
		fD_ = NULL;
		fF_ = f;
	}
}


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark minimization public functions
//-------------------------------------------------------------------
#endif

const OptimizerRecord1D* NonlinearOptimizer1D::minimize(const Function1D* f, double startX, 
                               double xMin, double xMax, double tol)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( !f->isDefinedAt(xMin) || !f->isDefinedAt(xMax),
                    kDataRangeError,
                    "function not defined on the entire search range in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( (startX<xMin) || (startX>xMax),
                    kDataRangeError,
                    "starting point not in search range in NonlinearOptimizer1D::minimize");

    OptimizerRecord1D* optRec = new OptimizerRecord1D();
    minimize_(f, startX, xMin, xMax, tol, optRec);

    return optRec;
}

void NonlinearOptimizer1D::minimize(const Function1D* f, double startX, double xMin, 
              double xMax, const double tol, OptimizerRecord1D* optRec)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( optRec == NULL,
                    kNullParameterError,
                    "null optimizer record as parameter in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( !f->isDefinedAt(xMin) || !f->isDefinedAt(xMax),
                    kDataRangeError,
                    "function not defined on the entire search range in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( (startX<xMin) || (startX>xMax),
                    kDataRangeError,
                    "starting point not in search range in NonlinearOptimizer1D::minimize");

    minimize_(f, startX, xMin, xMax, tol, optRec);
}


const OptimizerRecord1D* NonlinearOptimizer1D::minimize(const Function1D_F* f, float startX, 
                               float xMin, float xMax, float tol)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( !f->isDefinedAt(xMin) || !f->isDefinedAt(xMax),
                    kDataRangeError,
                    "function not defined on the entire search range in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( (startX<xMin) || (startX>xMax),
                    kDataRangeError,
                    "starting point not in search range in NonlinearOptimizer1D::minimize");

    OptimizerRecord1D* optRec = new OptimizerRecord1D();
    minimize_(f, startX, xMin, xMax, tol, optRec);
    return optRec;
}

void NonlinearOptimizer1D::minimize(const Function1D_F* f, float startX, float xMin, 
									float xMax, float tol, OptimizerRecord1D* optRec)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( optRec == NULL,
                    kNullParameterError,
                    "null optimizer record as parameter in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( !f->isDefinedAt(xMin) || !f->isDefinedAt(xMax),
                    kDataRangeError,
                    "function not defined on the entire search range in NonlinearOptimizer1D::minimize");
    FAIL_CONDITION( (startX<xMin) || (startX>xMax),
                    kDataRangeError,
                    "starting point not in search range in NonlinearOptimizer1D::minimize");

    minimize_(f, startX, xMin, xMax, tol, optRec);
}
