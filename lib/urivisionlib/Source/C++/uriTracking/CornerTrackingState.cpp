/*  NAME:
        CornerTrackingState.cpp
 
    DESCRIPTION:
        CornerTrackingState source file.
        base class: TrackingState
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
 
#include "CornerTrackingState.h"

using namespace std;
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

CornerTrackingState::CornerTrackingState(float x, float y,
										 float dx, float dy)
				   : TrackingState(x, y), dx_(dx), dy_(dy)
{

}

CornerTrackingState::~CornerTrackingState(void)
{

}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif


float CornerTrackingState::getDx(void)
{
	return dx_;
}

float CornerTrackingState::getDy(void)
{
	return dy_;
}

#if 0
#pragma mark -
#endif
