/*  NAME:
        IterationStatus.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib IterationStatus class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "IterationStatus.h"

using namespace std;
using namespace uriVL;

IterationStatus::IterationStatus(void)
		:	hasConverged_(false),
			nbIterations_(0),
			message_("")
{
	for (int i=0; i< MEASUREMENT_VECTOR_DIM; i++)
		measurements_[i] = 0.f;

}

IterationStatus::~IterationStatus(void)
{
}


void IterationStatus::setConverged(void)
{
	hasConverged_ = true;
}


bool IterationStatus::hasConverged(void) const
{
	return hasConverged_;
}


void IterationStatus::setMeasurements(const float* meas)
{
	for (int i=0; i< MEASUREMENT_VECTOR_DIM; i++)
		measurements_[i] = meas[i];
}


void IterationStatus::getMeasurements(float* meas) const
{
	for (int i=0; i< MEASUREMENT_VECTOR_DIM; i++)
		meas[i] = measurements_[i];
}


const float* IterationStatus::getMeasurements(void) const
{
	return measurements_;
}


void IterationStatus::setMessage(const string& message)
{
	message_ = message;
}
			
const string& IterationStatus::getMessage(void) const
{
	return message_;
}

