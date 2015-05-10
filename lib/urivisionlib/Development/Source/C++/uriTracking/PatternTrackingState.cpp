/*  NAME:
        PatternTrackingState.cpp
 
    DESCRIPTION:
        PatternTrackingState source file.
        base class: TrackingState
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
 
#include "PatternTrackingState.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

PatternTrackingState::PatternTrackingState(void)
				:	TrackingState(),
					//
					size_(0)
{
}

PatternTrackingState::PatternTrackingState(float x, float y, long size)
				:	TrackingState(x, y), 
					//
					size_(size)
{
}


PatternTrackingState::PatternTrackingState(const PatternTrackingState& pts)
		try	:	TrackingState(pts),
				//
				size_(pts.size_)				
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by PatternTrackingState copy constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kUnknown,
					"Unidentified error in PatternTrackingState copy constructor");
}


PatternTrackingState::PatternTrackingState(const PatternTrackingState* pts)
		try	:	TrackingState(pts),
				//
				size_(pts->size_)				
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by PatternTrackingState copy constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kUnknown,
					"Unidentified error in PatternTrackingState copy constructor");
}


PatternTrackingState::~PatternTrackingState(void)
{

}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------
long PatternTrackingState::getSize(void) const
{
	return size_;
}

void PatternTrackingState::setSize(long size)
{
	size_ = size;
}

#if 0
#pragma mark -
#endif


