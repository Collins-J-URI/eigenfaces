/*  NAME:
        BlobTrackingState.cpp
 
    DESCRIPTION:
        BlobTrackingState source file.
        base class: TrackingState
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
 
#include "BlobTrackingState.h"

using namespace std;
using namespace uriVL;

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Constructors and destructor
//--------------------------------------------------------------------------
#endif

BlobTrackingState::BlobTrackingState(void)
			:	TrackingState(), 
				//
				size_(0)
{

}

BlobTrackingState::BlobTrackingState(float x, float y,
									 long size)
			:	TrackingState(x, y), 
				//
				size_(size)
{

}

BlobTrackingState::BlobTrackingState(const BlobTrackingState& bts)
		try	:	TrackingState(bts),
				//
				size_(bts.size_)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by BlobTrackingState constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kUnknown,
					"Unidentified error in BlobTrackingState constructor");
}

BlobTrackingState::BlobTrackingState(const BlobTrackingState* bts)
		try	:	TrackingState(bts),
				//
				size_(bts->size_)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by BlobTrackingState constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kUnknown,
					"Unidentified error in BlobTrackingState constructor");
}

BlobTrackingState::~BlobTrackingState(void)
{

}


#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Copy and access methods
//--------------------------------------------------------------------------
#endif


const BlobTrackingState& BlobTrackingState::operator = (const BlobTrackingState& ts)
{
	try	
	{
		static_cast<TrackingState>(*this) = static_cast<const TrackingState&> (ts);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by TrackingState copy operator");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kUnknown,
						"Unidentified error in TrackingState copy operator");
	}
	
	return *this;
}


long BlobTrackingState::getSize(void) const
{
	return size_;
}

void BlobTrackingState::setSize(long size)
{
	size_ = size;
}

