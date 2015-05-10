/*  NAME:
        LinearSolverRecord.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib LinearSolverRecord class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "LinearSolverRecord.h"

using namespace uriVL;


LinearSolverRecord::LinearSolverRecord(void)
    :   sol(NULL), 
		status(SINGULAR_MATRIX)
{
}


LinearSolverRecord::~LinearSolverRecord(void)
{
    if (sol != NULL)
        delete sol;
        
}


LinearSolverRecord::LinearSolverRecord(const LinearSolverRecord& obj)
    :   sol(NULL), 
		status(SINGULAR_MATRIX)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"LinearSolverRecord copy constructor not implemented (meaningless)");
//	return *this;
}


const LinearSolverRecord& LinearSolverRecord::operator = (const LinearSolverRecord& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"LinearSolverRecord copy operator not implemented (meaningless)");
	return *this;
}


