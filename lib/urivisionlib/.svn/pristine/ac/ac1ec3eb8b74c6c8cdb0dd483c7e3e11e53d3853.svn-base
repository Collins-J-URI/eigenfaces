/*  NAME:
		Registration2D.cpp

		DESCRIPTION:
			implementation of the uriVisionLib Registration2D class

		COPYRIGHT:
			(c) 2003-2014, 3D Group for Interactive Visualization
							University of Rhode Island.

		Licensed under the Academic Free License - v. 2.1
		For more information see http://opensource.org/licenses/afl-2.1.php
*/

#include <iostream>
//
#include "Registration2D.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  static members
//----------------------------------------------------------

const float Registration2D::DEFAULT_NO_MATCH_THRESHOLD = 5;
const float Registration2D::DEFAULT_NO_MATCH_PENALTY = 10;
            

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


Registration2D::Registration2D()
    :   regType_(RIGID_NONLINEAR_REG_2D),
        ptSelectMode_(SELECT_ALL_POINTS),
        matchingMetric_(POINT_POINT_DISTANCE_SQ),
        pairRejectionMode_(THRESHOLD_BASED_REJECT),
        pairWeightingMode_(EQUAL_WEIGHT),      
        //
        noMatchThreshold_(DEFAULT_NO_MATCH_THRESHOLD),
        noMatchPenalty_(DEFAULT_NO_MATCH_PENALTY)     

{
}

Registration2D::~Registration2D(void)
{
}

void Registration2D::setRegistrationType(Registration2DType regType)
{
    regType_ = regType;
}


void Registration2D::setPointSelectionMode(Point2DSelectionMode ptSelectMode)
{
    ptSelectMode_ = ptSelectMode;
}

void Registration2D::setMatchingMetric(Point2DMatchingMetric matchingMetric)
{
    matchingMetric_ = matchingMetric;
}

void Registration2D::setPairRejectionMode(Point2DPairRejectMode pairRejectionMode)
{
    pairRejectionMode_ = pairRejectionMode;
}

void Registration2D::setPairWeightingMode(MatchPairWeightingMode pairWeightingMode)
{
    pairWeightingMode_ = pairWeightingMode;     
}


void Registration2D::setNoMatchThreshold(float noMatchThreshold)
{
    noMatchThreshold_ = noMatchThreshold;
}

void Registration2D::setNoMatchPenalty(float noMatchPenalty)
{
    noMatchPenalty_ = noMatchPenalty;            
}

