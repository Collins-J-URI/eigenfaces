/*  NAME:
        NonlinearOptimizerND.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib NonlinearOptimizerND class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "NonlinearOptimizerND.h"

using namespace std;
using namespace uriVL;

//	Local utility functions
void checkIfSearchStartPointIsValid_(const FunctionND_F* f, const float* xMin,  
							  const float* xMax, const float* startX);
void checkIfSearchStartPointIsValid_(const FunctionND_F* f, const vector<float>& xMin,  
							  const vector<float>& xMax, const vector<float>& startX);
void checkIfSearchStartPointIsValid_(const FunctionND* f, const double* xMin,  
							  const double* xMax, const double* startX);
void checkIfSearchStartPointIsValid_(const FunctionND* f, const vector<double>& xMin,  
							  const vector<double>& xMax, const vector<double>& startX);


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif

NonlinearOptimizerND::NonlinearOptimizerND(void)
			:	nDim_(0),
				fND_(NULL),
				fNF_(NULL)
				
{
}

NonlinearOptimizerND::NonlinearOptimizerND(int n)
			:	nDim_(n),
				fND_(NULL),
				fNF_(NULL)
{
	FAIL_CONDITION( n < 0,
					kInvalidParameterError,
					"Negative dimension passed to NonlinearOptimizerND constructor");
}

NonlinearOptimizerND::NonlinearOptimizerND(const FunctionND* f)
		try	:	nDim_(f->getDimension()),
				fND_(f),
				fNF_(NULL)
{
}
catch(...)
{
	FAIL_CONDITION( f == NULL,
					kNullFunctionError,
					"Null function pointer passed to NonlinearOptimizerND constructor");
					
	FAIL_CONDITION( true,
					kNumericalError,
					"Unexpected error in NonlinearOptimizerND constructor");
					
}	

NonlinearOptimizerND::NonlinearOptimizerND(const FunctionND_F* f)
		try	:	nDim_(f->getDimension()),
				fND_(NULL),
				fNF_(f)
{
}
catch(...)
{
	FAIL_CONDITION( f == NULL,
					kNullFunctionError,
					"Null function pointer passed to NonlinearOptimizerND constructor");
					
	FAIL_CONDITION( true,
					kNumericalError,
					"Unexpected error in NonlinearOptimizerND constructor");
					
}	



NonlinearOptimizerND::~NonlinearOptimizerND(void)
{
}

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark setters and getters
//----------------------------------------------------------
#endif

void NonlinearOptimizerND::setFunction(const FunctionND* f)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND constructor");

	//	don't waste time
	if (f != fND_)
	{
		//	call the private setting function (possibly overriden by child class)
		setFunction_(f);
		
		//	sets the variables
		fNF_ = NULL;
		fND_ = f;
	}
}

void NonlinearOptimizerND::setFunction(const FunctionND_F* f)
{
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND constructor");

	//	don't waste time
	if (f != fNF_)
	{
		//	call the private setting function (possibly overriden by child class)
		setFunction_(f);
		
		//	sets the variables
		fND_ = NULL;
		fNF_ = f;
	}
}


bool NonlinearOptimizerND::isOptimizationFunction(const FunctionND* f)
{
	return (fND_ == f);
}

bool NonlinearOptimizerND::isOptimizationFunction(const FunctionND_F* f)
{
	return (fNF_ == f);
}
			

int NonlinearOptimizerND::getDimension(void) const
{
	return nDim_;
}

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark public minimize functions
//----------------------------------------------------------
#endif


const  OptimizerRecordND* NonlinearOptimizerND::minimize(const FunctionND_F* f, 
														 const float* startX, 
														 const float* xMin, 
														 const float* xMax, float tol)
{
	//----------------------------------------------------
	//	First test for null parameters
	//----------------------------------------------------
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( startX == NULL,
                    kNullParameterError,
                    "null start point as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( xMin == NULL,
                    kNullParameterError,
                    "null min range as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( xMax == NULL,
                    kNullParameterError,
                    "null max range as parameter in NonlinearOptimizerND::minimize");


	//----------------------------------------------------------------------------------
	//	Check if the start point belongs to the search domain and domain of definition
	//----------------------------------------------------------------------------------
	try
	{
		checkIfSearchStartPointIsValid_(f, xMin, xMax, startX);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}	
	

	//----------------------------------------------------------------------------------
	//	Finally, perform the minimization
	//----------------------------------------------------------------------------------
	OptimizerRecordND* optRec = NULL;
	
	try
	{
		optRec = new OptimizerRecordND(f->getDimension());

		minimize_(f, startX, xMin, xMax, tol, optRec);
	}
	catch (ErrorReport& e)
	{
		if (optRec != NULL)
		{
			delete optRec;
			optRec = NULL;
		}
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}

    return optRec;
}



const  OptimizerRecordND* NonlinearOptimizerND::minimize(const FunctionND_F* f, 
														 const vector<float>& startX, 
														 const vector<float>& xMin, 
														 const vector<float>& xMax, float tol)
{
	//----------------------------------------------------
	//	First test for null parameters
	//----------------------------------------------------
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND::minimize");


	//----------------------------------------------------------------------------------
	//	Check if dimensions match
	//----------------------------------------------------------------------------------
	int fDim = -1, startDim = -2, minDim = -3, maxDim = -4;
	try
	{
		fDim = f->getDimension();
		startDim = startX.size();
		minDim = xMin.size();
		maxDim = xMax.size();
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid Parameter error in NonlinearOptimizerND::minimize");
	}
			
    FAIL_CONDITION( fDim != startDim,
                    kInvalidParameterError,
                    "function's and starting point's dimensions don't match");
    FAIL_CONDITION( fDim != minDim,
                    kInvalidParameterError,
                    "function's and lower bounds' dimensions don't match");
    FAIL_CONDITION( fDim != maxDim,
                    kInvalidParameterError,
                    "function's and upper bounds' dimensions don't match");



	//----------------------------------------------------------------------------------
	//	Check if the start point belongs to the search domain and domain of definition
	//----------------------------------------------------------------------------------
	try
	{
		checkIfSearchStartPointIsValid_(f, xMin, xMax, startX);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}	



	//----------------------------------------------------------------------------------
	//	Finally, perform the minimization
	//----------------------------------------------------------------------------------
	OptimizerRecordND* optRec = NULL;
	
	try
	{
		optRec = new OptimizerRecordND(f->getDimension());

		minimize_(f, startX, xMin, xMax, tol, optRec);
	}
	catch (ErrorReport& e)
	{
		if (optRec != NULL)
		{
			delete optRec;
			optRec = NULL;
		}
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}

    return optRec;
}



void NonlinearOptimizerND::minimize(const FunctionND_F* f, const float* startX, const float* xMin, 
									const float* xMax, float tol, OptimizerRecordND* optRec)
{
	//----------------------------------------------------
	//	First test for null parameters
	//----------------------------------------------------
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( startX == NULL,
                    kNullParameterError,
                    "null start point as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( xMin == NULL,
                    kNullParameterError,
                    "null min range as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( xMax == NULL,
                    kNullParameterError,
                    "null max range as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( optRec == NULL,
                    kNullParameterError,
                    "null optimizer record as parameter in NonlinearOptimizerND::minimize");

	//----------------------------------------------------------------------------------
	//	Check if dimensions match
	//----------------------------------------------------------------------------------
	int fDim = -1, recDim = -2;
	try
	{
		fDim = f->getDimension();
		recDim = optRec->nDim;
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid Parameter error in NonlinearOptimizerND::minimize");
	}			
    FAIL_CONDITION( fDim != recDim,
                    kInvalidParameterError,
                    "function's and optimizer's dimensions don't match");


	//----------------------------------------------------------------------------------
	//	Check if the start point belongs to the search domain and domain of definition
	//----------------------------------------------------------------------------------
	try
	{
		checkIfSearchStartPointIsValid_(f, xMin, xMax, startX);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}	


	//----------------------------------------------------------------------------------
	//	Finally, perform the minimization
	//----------------------------------------------------------------------------------
	try
	{
		minimize_(f, startX, xMin, xMax, tol, optRec);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
	}
}


void NonlinearOptimizerND::minimize(const FunctionND_F* f, const vector<float>& startX, 
									const vector<float>& xMin, const vector<float>& xMax, 
									float tol, OptimizerRecordND* optRec)
{
	//----------------------------------------------------
	//	First test for null parameters
	//----------------------------------------------------
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( optRec == NULL,
                    kNullParameterError,
                    "null optimizer record as parameter in NonlinearOptimizerND::minimize");

	//----------------------------------------------------------------------------------
	//	Check if dimensions match
	//----------------------------------------------------------------------------------
	int fDim = -1, startDim = -2, minDim = -3, maxDim = -4, recDim = -5;
	try
	{
		fDim = f->getDimension();
		recDim = optRec->nDim;
		startDim = startX.size();
		minDim = xMin.size();
		maxDim = xMax.size();
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid Parameter error in NonlinearOptimizerND::minimize");
	}
			
    FAIL_CONDITION( fDim != recDim,
                    kInvalidParameterError,
                    "function's and optimizer's dimensions don't match");
    FAIL_CONDITION( fDim != startDim,
                    kInvalidParameterError,
                    "function's and starting point's dimensions don't match");
    FAIL_CONDITION( fDim != minDim,
                    kInvalidParameterError,
                    "function's and lower bounds' dimensions don't match");
    FAIL_CONDITION( fDim != maxDim,
                    kInvalidParameterError,
                    "function's and upper bounds' dimensions don't match");



	//----------------------------------------------------------------------------------
	//	Check if the start point belongs to the search domain and domain of definition
	//----------------------------------------------------------------------------------
	try
	{
		checkIfSearchStartPointIsValid_(f, xMin, xMax, startX);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}	


	//----------------------------------------------------------------------------------
	//	Finally, perform the minimization
	//----------------------------------------------------------------------------------
	try
	{
		minimize_(f, startX, xMin, xMax, tol, optRec);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
	}
}


const OptimizerRecordND* NonlinearOptimizerND::minimize(const FunctionND* f, 
														const double* startX, 
														const double* xMin, 
														const double* xMax, double tol)
{
	//----------------------------------------------------
	//	First test for null parameters
	//----------------------------------------------------
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( startX == NULL,
                    kNullParameterError,
                    "null start point as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( xMin == NULL,
                    kNullParameterError,
                    "null min range as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( xMax == NULL,
                    kNullParameterError,
                    "null max range as parameter in NonlinearOptimizerND::minimize");

	
	//----------------------------------------------------------------------------------
	//	Check if the start point belongs to the search domain and domain of definition
	//----------------------------------------------------------------------------------
	try
	{
		checkIfSearchStartPointIsValid_(f, xMin, xMax, startX);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}	
	
	//----------------------------------------------------------------------------------
	//	Finally, perform the minimization
	//----------------------------------------------------------------------------------
	OptimizerRecordND* optRec = NULL;
	try
	{
		optRec = new OptimizerRecordND(f->getDimension());

		minimize_(f, startX, xMin, xMax, tol, optRec);
	}
	catch (ErrorReport& e)
	{
		if (optRec != NULL)
		{
			delete optRec;
			optRec = NULL;
		}
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}

    return optRec;
}


void NonlinearOptimizerND::minimize(const FunctionND* f, const double* startX, const double* xMin, 
									const double* xMax, double tol, OptimizerRecordND* optRec)
{
	//----------------------------------------------------
	//	First test for null parameters
	//----------------------------------------------------
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( startX == NULL,
                    kNullParameterError,
                    "null start point as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( xMin == NULL,
                    kNullParameterError,
                    "null min range as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( xMax == NULL,
                    kNullParameterError,
                    "null max range as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( optRec == NULL,
                    kNullParameterError,
                    "null optimizer record as parameter in NonlinearOptimizerND::minimize");

	//----------------------------------------------------------------------------------
	//	Check if dimensions match
	//----------------------------------------------------------------------------------
	int fDim = -1, recDim = -2;
	try
	{
		fDim = f->getDimension();
		recDim = optRec->nDim;
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid Parameter error in NonlinearOptimizerND::minimize");
	}
			
    FAIL_CONDITION( fDim != recDim,
                    kInvalidParameterError,
                    "function's and optimizer's dimensions don't match");
					

	//----------------------------------------------------------------------------------
	//	Check if the start point belongs to the search domain and domain of definition
	//----------------------------------------------------------------------------------
	try
	{
		checkIfSearchStartPointIsValid_(f, xMin, xMax, startX);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}	

				
	//----------------------------------------------------------------------------------
	//	Finally, perform the minimization
	//----------------------------------------------------------------------------------
	try
	{
		minimize_(f, startX, xMin, xMax, tol, optRec);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
	}
}


void NonlinearOptimizerND::minimize(const FunctionND* f, const vector<double>& startX, 
									const vector<double>& xMin, const vector<double>& xMax, 
									double tol, OptimizerRecordND* optRec)
{
	//----------------------------------------------------
	//	First test for null parameters
	//----------------------------------------------------
    FAIL_CONDITION( f == NULL,
                    kNullFunctionError,
                    "null function as parameter in NonlinearOptimizerND::minimize");
    FAIL_CONDITION( optRec == NULL,
                    kNullParameterError,
                    "null optimizer record as parameter in NonlinearOptimizerND::minimize");

	//----------------------------------------------------------------------------------
	//	Check if dimensions match
	//----------------------------------------------------------------------------------
	int fDim = -1, startDim = -2, minDim = -3, maxDim = -4, recDim = -5;
	try
	{
		fDim = f->getDimension();
		recDim = optRec->nDim;
		startDim = startX.size();
		minDim = xMin.size();
		maxDim = xMax.size();
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid Parameter error in NonlinearOptimizerND::minimize");
	}
			
    FAIL_CONDITION( fDim != recDim,
                    kInvalidParameterError,
                    "function's and optimizer's dimensions don't match");
    FAIL_CONDITION( fDim != startDim,
                    kInvalidParameterError,
                    "function's and starting point's dimensions don't match");
    FAIL_CONDITION( fDim != minDim,
                    kInvalidParameterError,
                    "function's and lower bounds' dimensions don't match");
    FAIL_CONDITION( fDim != maxDim,
                    kInvalidParameterError,
                    "function's and upper bounds' dimensions don't match");



	//----------------------------------------------------------------------------------
	//	Check if the start point belongs to the search domain and domain of definition
	//----------------------------------------------------------------------------------
	try
	{
		checkIfSearchStartPointIsValid_(f, xMin, xMax, startX);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
						
	}	


	//----------------------------------------------------------------------------------
	//	Finally, perform the minimization
	//----------------------------------------------------------------------------------
	try
	{
		minimize_(f, startX, xMin, xMax, tol, optRec);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by NonlinearOptimizerND::minimize");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in NonlinearOptimizerND::minimize");
	}
}



#if 0
//--------------------------------------------------------------------------
#pragma mark -
#pragma mark Private utility functions
//--------------------------------------------------------------------------
#endif

void checkIfSearchStartPointIsValid_(const FunctionND_F* f, const float* xMin,  
							  const float* xMax, const float* startX)
{
	bool inDomain = false;
	bool inSearchRange = true;
	
	try
	{
		inDomain = f->isDefinedAt(startX);
		
		int nDim = f->getDimension();
		inSearchRange = true;
		for (int k=0; inSearchRange && (k<nDim); k++)
		{
			inSearchRange = (startX[k] >= xMin[k]) && (startX[k] <= xMax[k]);
		}
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by uriVL::checkIfSearchStartPointIsValid_");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in uriVL::checkIfSearchStartPointIsValid_");
						
	}	
	
    FAIL_CONDITION( !inDomain,
                    kDataRangeError,
                    "start point not within function domain of definition in uriVL::checkIfSearchStartPointIsValid_");

    FAIL_CONDITION( !inSearchRange,
                    kDataRangeError,
                    "start point not within search range in uriVL::checkIfSearchStartPointIsValid_");
}

void checkIfSearchStartPointIsValid_(const FunctionND_F* f, const vector<float>& xMin,  
							  const vector<float>& xMax, const vector<float>& startX)
{
	bool inDomain = false;
	bool inSearchRange = true;
	
	try
	{
		inDomain = f->isDefinedAt(startX);
		
		int nDim = f->getDimension();
		inSearchRange = true;
		for (int k=0; inSearchRange && (k<nDim); k++)
		{
			inSearchRange = (startX[k] >= xMin[k]) && (startX[k] <= xMax[k]);
		}
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by uriVL::checkIfSearchStartPointIsValid_");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in uriVL::checkIfSearchStartPointIsValid_");
						
	}	
	
    FAIL_CONDITION( !inDomain,
                    kDataRangeError,
                    "start point not within function domain of definition in uriVL::checkIfSearchStartPointIsValid_");

    FAIL_CONDITION( !inSearchRange,
                    kDataRangeError,
                    "start point not within search range in uriVL::checkIfSearchStartPointIsValid_");
}



void checkIfSearchStartPointIsValid_(const FunctionND* f, const double* xMin,  
							  const double* xMax, const double* startX)
{
	bool inDomain = false;
	bool inSearchRange = true;
	
	try
	{
		inDomain = f->isDefinedAt(startX);
		
		int nDim = f->getDimension();
		inSearchRange = true;
		for (int k=0; inSearchRange && (k<nDim); k++)
		{
			inSearchRange = (startX[k] >= xMin[k]) && (startX[k] <= xMax[k]);
		}
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by uriVL::checkIfSearchStartPointIsValid_");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in uriVL::checkIfSearchStartPointIsValid_");
						
	}	
	
    FAIL_CONDITION( !inDomain,
                    kDataRangeError,
                    "start point not within function domain of definition in uriVL::checkIfSearchStartPointIsValid_");

    FAIL_CONDITION( !inSearchRange,
                    kDataRangeError,
                    "start point not within search range in uriVL::checkIfSearchStartPointIsValid_");

}


void checkIfSearchStartPointIsValid_(const FunctionND* f, const vector<double>& xMin,  
									 const vector<double>& xMax, const vector<double>& startX)
{
	bool inDomain = false;
	bool inSearchRange = true;
	
	try
	{
		inDomain = f->isDefinedAt(startX);
		
		int nDim = f->getDimension();
		inSearchRange = true;
		for (int k=0; inSearchRange && (k<nDim); k++)
		{
			inSearchRange = (startX[k] >= xMin[k]) && (startX[k] <= xMax[k]);
		}
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by uriVL::checkIfSearchStartPointIsValid_");
		throw e;
	}
	catch(...)
	{
		FAIL_CONDITION( true,
						kNumericalError,
						"Unexpected error in uriVL::checkIfSearchStartPointIsValid_");
						
	}	
	
    FAIL_CONDITION( !inDomain,
                    kDataRangeError,
                    "start point not within function domain of definition in uriVL::checkIfSearchStartPointIsValid_");

    FAIL_CONDITION( !inSearchRange,
                    kDataRangeError,
                    "start point not within search range in uriVL::checkIfSearchStartPointIsValid_");
}

