/*  NAME:
        Tracker2D.cpp
 
    DESCRIPTION:
        Tracker2D source file.
        Tracker2D is an abstract class (with pure virtual member function)
        Inherited by: Tracker2D_PID, BlobTracker2D, ShapeTracker2D, CornerTracker2D...
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Tracker2D.h"

using namespace std;
using namespace uriVL;

#define FAIL_COUNTS 5

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

Tracker2D::Tracker2D(void)
{
	detector_ = NULL;
	currentState_ = NULL;
	savePred_ = false;
	saveState_ = false;
	failCounts_	= FAIL_COUNTS;
}

Tracker2D::Tracker2D(FeatureDetector *detector,
					 float metricX,
					 float metricY,
					 TrackingState *initState,
					 bool savePred, /* = false */
					 bool saveState /* = false */)
{
	detector_ = detector;
	metricX_ = metricX;
	metricY_ = metricY;
	currentState_ = new TrackingState(initState);
	savePred_ = savePred;
	saveState_ = saveState;
	predRecord_.push_back(currentState_);
	stateRecord_.push_back(currentState_);
	failCounts_	= FAIL_COUNTS;
}

Tracker2D::Tracker2D(float metricX,
					 float metricY,
					 TrackingState *initState,
					 bool savePred, /* = false */
					 bool saveState /* = false */)
{
	detector_ = NULL;
	metricX_ = metricX;
	metricY_ = metricY;
	currentState_ = new TrackingState(initState);
	savePred_ = savePred;
	saveState_ = saveState;
	predRecord_.push_back(currentState_);
	stateRecord_.push_back(currentState_);
	failCounts_	= FAIL_COUNTS;
}

Tracker2D::~Tracker2D(void)
{
	delete currentState_;
}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

/*
TrackingState *Tracker2D::track(RasterImage* img)
{

}
*/

TrackingStateList Tracker2D::getPredRecord(void)
{
	return predRecord_;
}

TrackingStateList Tracker2D::getStateRecord(void)
{
	return stateRecord_;
}

TrackingState *Tracker2D::getCurrState(void)
{
	return currentState_;
}

//----------------------------------------------------------
//  private and protected functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

