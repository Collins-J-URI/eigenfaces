/*  NAME:
        TrackingState.cpp
 
    DESCRIPTION:
        TrackingState source file.
        TrackingState is a base class
         
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
 
#include "TrackingState.h"

using namespace std;
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------
TrackingState::TrackingState(void)
			:	pos_(new ImagePoint_F(0.f, 0.f)),
				result_(TARGET_FOUND),
				targetGone_(false)
{
}

TrackingState::TrackingState(float x, float y)
			:	pos_(new ImagePoint_F(x, y)),
				result_(TARGET_FOUND),
				targetGone_(false)
{
}

TrackingState::TrackingState(const ImagePoint_F* pos)
		try	:	pos_(new ImagePoint_F(pos)),
				result_(TARGET_FOUND),
				targetGone_(false)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by TrackingState constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kUnknown,
					"Unidentified error in TrackingState constructor");
}


TrackingState::TrackingState(const TrackingState& ts)
		try	:	pos_(new ImagePoint_F(ts.pos_)),
				result_(ts.result_),
				targetGone_(ts.targetGone_)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by TrackingState copy constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kUnknown,
					"Unidentified error in TrackingState copy constructor");
}


TrackingState::TrackingState(const TrackingState* ts)
		try	:	pos_(new ImagePoint_F(ts->pos_)),
				result_(ts->result_),
				targetGone_(ts->targetGone_)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by TrackingState copy constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kUnknown,
					"Unidentified error in TrackingState copy constructor");
}


TrackingState::~TrackingState(void)
{
	if (pos_ != NULL)
		delete	pos_;
		
}

//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif


const TrackingState& TrackingState::operator = (const TrackingState& ts)
{
	try	
	{
		pos_ = new ImagePoint_F(ts.getPos());
		result_ = ts.getResult();
		targetGone_ = ts.getTargetGone();
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

Result TrackingState::getResult(void) const
{
	return result_;
}

void TrackingState::setResult(Result r)
{
	result_ = r;
}

bool TrackingState::getTargetGone(void) const
{
	return targetGone_;
}

void TrackingState::setTargetGone(bool tg)
{
	targetGone_ = tg;
}

const ImagePoint_F* TrackingState::getPos(void) const
{
	return pos_;
}

void TrackingState::setPos(const ImagePoint_F* pos)
{
	try	
	{
		pos_->setCoordinates(pos->getX(), pos->getY());
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by TrackingState::setPos");
		throw e;
	}
}

void TrackingState::setPos(float x, float y)
{
	pos_->setCoordinates(x, y);
}

#if 0
#pragma mark -
#endif
