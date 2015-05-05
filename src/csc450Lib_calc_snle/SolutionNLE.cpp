//
//  SolutionNLE.cpp
//
//
//  Created by Christopher Glasz on 3/31/15.
//
//

#include "SolutionNLE.h"
using namespace csc450Lib_calc_snle;

SolutionNLE::SolutionNLE(float x, float y, float i, SolutionStatus status) {
    xEstimate = x;
    yEstimate = y;
    numIterations = i;
    searchStatus = status;
}

SolutionNLE::~SolutionNLE(void) {
    xEstimate = 0;
    yEstimate = 0;
    numIterations = 0;
    searchStatus = SEARCH_SUCCESSFUL;
}

SolutionStatus SolutionNLE::getStatus(void) const { return searchStatus; }

float SolutionNLE::getSolution(void) const { return xEstimate; }

float SolutionNLE::getValueAtSolution(void) const { return yEstimate; }

int SolutionNLE::getNumberOfIterations(void) const { return numIterations; }
