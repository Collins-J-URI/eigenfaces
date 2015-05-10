/*  NAME:
        OptimizerRecordND.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib OptimizerRecordND class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "OptimizerRecordND.h"

using namespace uriVL;

OptimizerRecordND::OptimizerRecordND(void)
			:	nDim(0),
				optX(NULL),
				optVal(-HUGE_VAL),
				nbOuterIters(-1),
				nbInnerIters(-1),
				isSuccess(false)
{
}

OptimizerRecordND::OptimizerRecordND(const int n)
		try	:	nDim(n),
				optX(new double[n]),
				optVal(-HUGE_VAL),
				nbOuterIters(-1),
				nbInnerIters(-1),
				isSuccess(false)
{
}
catch (...)
{
	FAIL_CONDITION( n <= 0,
					kInvalidParameterError,
					"Invalid dimension passed to OptimizerRecordND constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to OptimizerRecordND constructor");
}


OptimizerRecordND::OptimizerRecordND(const OptimizerRecordND* optRec)
		try	:	nDim(optRec->nDim),
				optX(new double[optRec->nDim]),
				optVal(optRec->optVal),
				nbOuterIters(optRec->nbOuterIters),
				nbInnerIters(optRec->nbInnerIters),
				isSuccess(optRec->isSuccess)
{
	try 
	{
		for (int i=0; i<nDim; i++)
			optX[i] = optRec->optX[i];

	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid parameter passed to OptimizerRecordND constructor");
	}
}
catch (...)
{
	FAIL_CONDITION( optRec == NULL,
					kNullParameterError,
					"Null parameter passed to OptimizerRecordND constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to OptimizerRecordND constructor");
}



OptimizerRecordND::OptimizerRecordND(const OptimizerRecordND& optRec)
		try	:	nDim(optRec.nDim),
				optX(new double[optRec.nDim]),
				optVal(optRec.optVal),
				nbOuterIters(optRec.nbOuterIters),
				nbInnerIters(optRec.nbInnerIters),
				isSuccess(optRec.isSuccess)
{
	try 
	{
		for (int i=0; i<nDim; i++)
			optX[i] = optRec.optX[i];
		
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid parameter passed to OptimizerRecordND constructor");
	}
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to OptimizerRecordND constructor");
}


OptimizerRecordND::~OptimizerRecordND(void)
{
	if (optX != NULL)
		delete []optX;
		
}



void OptimizerRecordND::setDimension(const int n)
{
	nDim = n;
	
	if (optX)
	{
		delete [] optX;
		optX = new double[n];
	}
}
