/*!
 *	NAME:
 *		FrameGrabber.cpp
 *
 *	DESCRIPTION:
 *		Implementation of the FrameGrabber class
 *
 *	COPYRIGHT:
 *		(c) 2003-2014, 3D Group for Interactive Visualization
 *				University of Rhode Island.
 *
 *		Licensed under the Academic Free License - v. 3.0
 *		For more information see http://opensource.org/licenses/academic.php
 **/

#if URIVL_CAN_GRAB_VIDEOS

#include "FrameGrabber.h"
#if URIVL_USE_QUICKTIME
    #include "_FrameGrabber_QT.h"
#elif URIVL_USE_LIBVIDCAP
   #include "_FrameGrabber_libvidcap.h"
#endif

#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"

using namespace std;
using namespace uriVL;

FrameGrabber::FrameGrabber(const ImageRect* rect) 
    :   VideoSource()
{
    FAIL_CONDITION( rect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in FrameGrabber constructor");

	#if URIVL_USE_QUICKTIME
		grabber_ = new _FrameGrabber_QT(rect);
	#else
		grabber_ = new _FrameGrabber_libvidcap(rect);
	#endif
	
}

FrameGrabber::~FrameGrabber()
{
	delete grabber_;
}

void FrameGrabber::setSource()
{
	try
	{
		grabber_->setSource_();
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FrameGrabber::setSource");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in FrameGrabber::setSource");
	}
}

void FrameGrabber::start(void)
{
	grabber_->start_();
}

RasterImage* FrameGrabber::getFrame()
{
	RasterImage* grabImg = NULL;
	
	try
	{
		grabImg = grabber_->getFrame_();
		updateImageRasterStatus_(grabImg);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FrameGrabber::getFrame");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in FrameGrabber::getFrame");
	}
	
	return grabImg;
}

void FrameGrabber::getFrame(RasterImage* grabImg)
{
	FAIL_CONDITION(	grabImg == NULL, 
					kNullRasterImageError,
					"Null image passed as destination to a FrameGrabber.");

	try
	{
		grabber_->getFrame_(grabImg);
		updateImageRasterStatus_(grabImg);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FrameGrabber::getFrame");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in FrameGrabber::getFrame");
	}
}

#if 0
//---------------------------------------------------------
#pragma mark -
#pragma mark misc access functions
//---------------------------------------------------------
#endif

short FrameGrabber::getWidth(void)
{
	return grabber_->getWidth_();
}

short FrameGrabber::getHeight(void)
{
	return grabber_->getHeight_();
}

const ImageRect* FrameGrabber::getRect(void)
{
	return grabber_->getRect_();
}

long FrameGrabber::getFrameIndex(void)
{
	return grabber_->getFrameIndex_();
}

#endif // URIVL_CAN_GRAB_VIDEOS
