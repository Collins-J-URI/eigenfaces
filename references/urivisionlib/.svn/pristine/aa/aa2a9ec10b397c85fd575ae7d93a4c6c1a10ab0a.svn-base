/*  NAME:
        NonlinearOptimizerND_conjDir.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib NonlinearOptimizerND_conjDir class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <cmath>
#include "NonlinearOptimizerND_conjDir.h"
#include "NonlinearOptimizer1D_gss.h"

using namespace std;
using namespace uriVL;

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif

NonlinearOptimizerND_conjDir::NonlinearOptimizerND_conjDir(int n, DirectionUpdateMethod updateMethod, 
															int reset, DirectionResetMode resetMethod)
		try	:   NonlinearOptimizerND(n),
				//
				updateMethod_(updateMethod),
				reset_(reset >= 0 ? reset : n),
				resetMethod_(resetMethod),
				userDirections_(NULL),
				lineMinimizer_(new NonlinearOptimizer1D_gss()),
				lineRecord_(new OptimizerRecord1D()),
				ffSec1D_(NULL),
				fdSec1D_(NULL),
				u_(NULL),
				P0_(NULL),
				Pi_(NULL),
				bestDirIndex_(0),
				cycleCounter_(0)				
{
	allocateInstanceArrays_(n);
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by NonlinearOptimizerND_conjDir constructor");
	throw e;
}

NonlinearOptimizerND_conjDir::NonlinearOptimizerND_conjDir(const FunctionND* fn, DirectionUpdateMethod updateMethod, 
															 int reset, 
															 DirectionResetMode resetMethod)
		try	:   NonlinearOptimizerND(fn),
				//
				updateMethod_(updateMethod),
				reset_(reset >= 0 ? reset : fn->getDimension()),
				resetMethod_(resetMethod),
				userDirections_(NULL),
				lineMinimizer_(new NonlinearOptimizer1D_gss()),
				lineRecord_(new OptimizerRecord1D()),
				ffSec1D_(NULL),
				fdSec1D_(NULL),
				u_(NULL),
				P0_(NULL),
				Pi_(NULL),
				bestDirIndex_(0),
				cycleCounter_(0)				
{
	allocateInstanceArrays_(fn->getDimension());
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by NonlinearOptimizerND_conjDir constructor");
	throw e;
}


NonlinearOptimizerND_conjDir::NonlinearOptimizerND_conjDir(const FunctionND_F* fn, DirectionUpdateMethod updateMethod, 
															 int reset, 
															 DirectionResetMode resetMethod)
		try	:   NonlinearOptimizerND(fn),
				//
				updateMethod_(updateMethod),
				reset_(reset >= 0 ? reset : fn->getDimension()),
				resetMethod_(resetMethod),
				userDirections_(NULL),
				lineMinimizer_(new NonlinearOptimizer1D_gss()),
				lineRecord_(new OptimizerRecord1D()),
				ffSec1D_(NULL),
				fdSec1D_(NULL),
				u_(NULL),
				P0_(NULL),
				Pi_(NULL),
				bestDirIndex_(0),
				cycleCounter_(0)				
{
	allocateInstanceArrays_(fn->getDimension());
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by NonlinearOptimizerND_conjDir constructor");
	throw e;
}


NonlinearOptimizerND_conjDir::~NonlinearOptimizerND_conjDir(void)
{
	deleteInstanceArrays_(getDimension());
	
	if (ffSec1D_ != NULL)
        delete ffSec1D_;
    if (fdSec1D_ != NULL)
        delete fdSec1D_;

	delete lineMinimizer_;
    delete lineRecord_;
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark private setters and getters
//----------------------------------------------------------
#endif

void NonlinearOptimizerND_conjDir::setFunction_(const FunctionND* fn)
{
    FAIL_CONDITION( fn == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND_conjDir::setFunction");

    allocateInstanceArrays_(fn->getDimension());
    
    //  (re-)allocate the 1D section function if needed.
    if (fdSec1D_ != NULL)
    {
        delete fdSec1D_;
        fdSec1D_ = NULL;
    }
    if (ffSec1D_ == NULL)
        delete ffSec1D_;

    ffSec1D_ = new FunctionNDSection1D(fn);    
}

void NonlinearOptimizerND_conjDir::setFunction_(const FunctionND_F* fn)
{
    FAIL_CONDITION( fn == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND_conjDir::setFunction");

    allocateInstanceArrays_(fn->getDimension());
    
    //  (re-)allocate the 1D section function if needed.
    if (ffSec1D_ != NULL)
    {
        delete ffSec1D_;
        ffSec1D_ = NULL;
    }
    if (fdSec1D_ == NULL)
        delete fdSec1D_;

    fdSec1D_ = new FunctionNDSection1D_F(fn);
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark private minimize functions
//----------------------------------------------------------
#endif


void NonlinearOptimizerND_conjDir::minimize_(const FunctionND* f,const double* startX, 
											 const double* xMin, const double* xMax, 
											 double tol, OptimizerRecordND* optRec)
{
	//	If the function had not been pre-set we need to perform some allocations
	if (!isOptimizationFunction(f))
	{
		setFunction(f);
	}
	
	//	if USER_SPECIFIED_DIRECTIONS was selected as direction update mode but the user has 
	//	not set update directions, the mode is changed to CANONICAL_DIRECTIONS
	if ((resetMethod_ == USER_SPECIFIED_DIRECTIONS) && (userDirections_ == NULL))
		resetMethod_ = CANONICAL_DIRECTIONS;
		
	updateConjugateDirections_();
	
	
	const int nDim = getDimension();
	
    //  set the starting position and the conjugate directions
    for (int i=0; i<nDim; i++)
    {
        Pi_[i] = P0_[i] = startX[i];
        
        //  the conjugate directions are initially the cardinal directions
        for (int j=0; j<nDim; j++)
            u_[i][j] = 0.L;
        u_[i][i] = 1.0L;
    }

//jyh 2013-08-30
//    //  inform the section function of the bounds for this search
//    fdSec1D_->setBounds(xMin, xMax);
    
    //  the best value found so far is the one at our starting point
    double optVal = f->func(startX);
    
    cycleCounter_ = 0;
    optRec->nbOuterIters = 0;
    optRec->nbInnerIters = 0;
    optRec->isSuccess = true;   //  the power of positive thinking

    bool ongoingSearch = true;;
    //  repeat the cycle until a minimum is found or one of the 
    //  line searches fail
    while (ongoingSearch)
    {
        //  One cycle over the conjugate directions
        cycleCounter_++;
        
        //  best direction so far
        bestDirIndex_ = -1;
        double   bestReduction = -1;
        double   inCycleOptVal = optVal;
        
        for (int i=0; (i<nDim) && ongoingSearch; i++)
        {
            //  perform line minimization along direction u[i]
//jyh 2013-08-30
//            fdSec1D_->setSection(Pi_, u_[i]);
            double   sMin = fdSec1D_->getLowerBound(),
                     sMax = fdSec1D_->getUpperBound();
            lineMinimizer_->minimize(fdSec1D_, 0, sMin, sMax, tol, lineRecord_);

            //  if the line minimization succeeded, we need to update the state to prepare
            //  for the next direction or cycle
            if (lineRecord_->isSuccess)
            {
                float valReduction = inCycleOptVal - lineRecord_->optVal;
                if (valReduction > bestReduction)
                {   
                    bestReduction  = valReduction;
                    bestDirIndex_ = i;
                }
                inCycleOptVal = lineRecord_->optVal;
                optRec->nbInnerIters += lineRecord_->nbIters;
                
                /// move to the new point before changing direction
                double   s = lineRecord_->optX;
                for (int j=0; j<nDim; j++)
                    Pi_[j] += s * u_[i][j];

            }
            //  if the line minimization failed, there is no need to search any further
            else
            {
                optRec->isSuccess = false;
                ongoingSearch = false;
            }
            
            optRec->nbOuterIters++;            
        }
        
        //  are we done yet?  What fraction of its value did the function vary over a full
        //  cycle?
        if (tol*(fabs(inCycleOptVal) + fabs(optVal)) > 2.0*(optVal - inCycleOptVal))
            ongoingSearch = false;
        else
            updateConjugateDirections_();

        optVal = inCycleOptVal;
    }
    
    //  we store the results of our iteration in the optimizer record.
    //  The numbers of iterations are already up-to-date
    for (int j=0; j<nDim; j++)
        optRec->optX[j] = Pi_[j];
    optRec->optVal = optVal;
}



void NonlinearOptimizerND_conjDir::minimize_(const FunctionND_F* f, const float* startX, 
											 const float* xMin, const float* xMax, 
											 float tol, OptimizerRecordND* optRec)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Float forms of NonlinearOptimizerND_conjDir::minimize_ not implemented.  Will be translated from double versions once the latter works fine.");

}




void NonlinearOptimizerND_conjDir::minimize_(const FunctionND* f, const std::vector<double>& startX, 
										     const std::vector<double>& xMin, 
										     const std::vector<double>& xMax, double tol, 
										     OptimizerRecordND* optRec)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Vector forms of NonlinearOptimizerND_conjDir::minimize_ not implemented.  Will be translated from array version once the latter works fine.");
}

void NonlinearOptimizerND_conjDir::minimize_(const FunctionND_F* f, const std::vector<float>& startX, 
										     const std::vector<float>& xMin, 
										     const std::vector<float>& xMax, float tol, 
										     OptimizerRecordND* optRec)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Vector forms of NonlinearOptimizerND_conjDir::minimize_ not implemented.  Will be translated from array version once the latter works fine.");
}



void NonlinearOptimizerND_conjDir::updateConjugateDirections_(void)
{
	const int nDim = getDimension();
	
	//	First, if we have regular reset, check if it's time to do one
	if ((updateMethod_ != NO_UPDATE) && ((cycleCounter_ % (reset_ * nDim)) == 0))
	{
		switch (resetMethod_)
		{
			case USER_SPECIFIED_DIRECTIONS:
				FAIL_CONDITION(	true, 
								kCaseNotImplemented, 
								"case not implemented yet in NonlinearOptimizerND_conjDir::updateConjugateDirections_");
				break;
				
			RANDOM_ORTHOGONAL_DIRECTIONS:
				FAIL_CONDITION(	true, 
								kCaseNotImplemented, 
								"case not implemented yet in NonlinearOptimizerND_conjDir::updateConjugateDirections_");
				break;
				
			CANONICAL_DIRECTIONS:
			default:
                for (int i=0; i<nDim; i++)
                {
                    for (int j=0; j<nDim; j++)
                        u_[i][j] = 0.L;
                    u_[i][i] = 1.0L;
                }
			break;
		}
	}
	//	else, go for regular update
	else
	{
		switch (updateMethod_)
		{
			case NO_UPDATE:
				//	
				//	do nothing
				break;
				
			case POWELL_UN_IS_XN_X0:
				//  simply permutate the pointers
				for (int i=0; i<nDim-1; i++)
					u_[i] = u_[i+1];
					
				//  new direction is given by P0-PN
				for (int j=0; j<nDim-1; j++)
					u_[nDim-1][j] = Pi_[j] - P0_[j];
				break;


			case POWELL_DISCARD_LARGEST_DECREASE_DIR:
				FAIL_CONDITION( true,
								kCaseNotImplemented,
								"case not implemented yet in NonlinearOptimizerND_conjDir::updateConjugateDirections_");
				break;

			default:
				FAIL_CONDITION(	true, 
								kCaseNotImplemented, 
								"case not implemented yet in NonlinearOptimizerND_conjDir::updateConjugateDirections_");
		}
	}
}



#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark private housekeeping functions
//----------------------------------------------------------
#endif


void NonlinearOptimizerND_conjDir::allocateInstanceArrays_(const int n)
{
    P0_ = new double[n];
	Pi_ = new double[n];

	u_ = new double*[n];
	for (int i=0; i<n; i++)
		u_[i] = new double[n];

}

void NonlinearOptimizerND_conjDir::deleteInstanceArrays_(int n)
{
    if (P0_ != NULL)
        delete []P0_;
	P0_ = NULL;
	
    if (Pi_ != NULL)
        delete []Pi_;
	Pi_ = NULL;
	
    if (u_ != NULL)
    {
        for (int i=0; i<n; i++)
            delete []u_[i];
        delete []u_;
    }
	u_ = NULL;
	
    if (userDirections_ != NULL)
    {
        for (int i=0; i<n; i++)
            delete []userDirections_[i];
        delete []userDirections_;
    }
	userDirections_ = NULL;
}

