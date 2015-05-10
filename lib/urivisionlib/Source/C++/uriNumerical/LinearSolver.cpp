/*  NAME:
        LinearSolver.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib LinearSolver class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "LinearSolver.h"

using namespace uriVL;

LinearSolver::LinearSolver(void)
		:	nbRows_(0),
			nbCols_(0),
			nbStorageRows_(0),
			nbStorageCols_(0),
			matrixIsLocal_(true),
			//
			singularityStatus_(UNKNOWN_STATUS)
{
}

LinearSolver::LinearSolver(int nbRows, int nbCols)
		:	nbRows_(nbRows),
			nbCols_(nbCols),
			nbStorageRows_(nbRows),
			nbStorageCols_(nbCols),
			matrixIsLocal_(true),
			//
			singularityStatus_(UNKNOWN_STATUS)

{
        FAIL_CONDITION( (nbRows < 1) || (nbCols < 1),
                        kInvalidParameterError,
                        "Invalid dimension SLE assigned to a LinearSolver constructor");

}
        

LinearSolver::LinearSolver(Matrix* A, bool preserveMatrix)
	try	:	nbRows_(A->getNbRows()),
			nbCols_(A->getNbCols()),
			nbStorageRows_(A->getNbRows()),
			nbStorageCols_(A->getNbCols()),
			matrixIsLocal_(preserveMatrix),
			//
			singularityStatus_(UNKNOWN_STATUS)

{
}
catch (...) {
	FAIL_CONDITION( A == NULL,
					kNullParameterError,
					"Null pointer to Matrix passed to LinearSolver constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid pointer to Matrix passed to LinearSolver constructor");
}


LinearSolver::~LinearSolver(void)
{
}


void LinearSolver::setSingularityStatus_(LinearSolverRecordStatus singularityStatus)
{
	singularityStatus_ = singularityStatus;
}
			

LinearSolverRecordStatus LinearSolver::getSingularityStatus_(void)
{
	return singularityStatus_;
}
			
int LinearSolver::getNbRows_(void)
{
	return nbRows_;
}
 
int LinearSolver::getNbCols_(void)
{
	return nbCols_;
}
 
void LinearSolver::setMatrixDimensions_(int nbRows, int nbCols)
{
	nbRows_ = nbRows;
	nbCols_ = nbCols;
}

int LinearSolver::getNbStorageRows_(void)
{
	return nbStorageRows_;
}
 
int LinearSolver::getNbStorageCols_(void)
{
	return nbStorageCols_;
}
 
void LinearSolver::setMatrixStorageDimensions_(int nbRows, int nbCols)
{
	nbStorageRows_ = nbRows;
	nbStorageCols_ = nbCols;
}

bool LinearSolver::getMatrixIsLocal_(void)
{
	return matrixIsLocal_;
}


void LinearSolver::setMatrixIsLocal_(bool matrixIsLocal)
{
	matrixIsLocal_ = matrixIsLocal;
}
