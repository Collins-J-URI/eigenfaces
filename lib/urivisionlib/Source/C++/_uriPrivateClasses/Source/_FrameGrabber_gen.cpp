/*  NAME:
        _FrameGrabber_libvidcap.cpp

    DESCRIPTION:
        implementation of the uriVisionLib _FrameGrabber_libvidcap class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
 
//	Mostly there to facilitate life for Visual Studio developers, since in Xcode and
//	Cmake souce files are excluded directly from the project depending on the
//	state defined by the environment variables.
#if URIVL_USE_LIBVIDCAP


#include "ErrorReport.h"
//#include "FrameGrabber_libvidcap.h"
#include "FrameGrabber_gen.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"

using namespace uriVL;

_FrameGrabber_libvidcap::_FrameGrabber_libvidcap(const ImageRect* rect) 
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
}

_FrameGrabber_libvidcap::~_FrameGrabber_libvidcap(void)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
}

void _FrameGrabber_libvidcap::setSource_(void)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
}


RasterImage* _FrameGrabber_libvidcap::getFrame_(void)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
	return NULL;
}


void _FrameGrabber_libvidcap::getFrame_(RasterImage* grabImage)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
}


int _FrameGrabber_libvidcap::getWidth_(void)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
	return -1;
}

int _FrameGrabber_libvidcap::getHeight_(void)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
	return -1;
}


const ImageRect* _FrameGrabber_libvidcap::getRect_(void)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
	return NULL;
}



long _FrameGrabber_libvidcap::getFrameIndex_(void)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
	return -1L;
}


void _FrameGrabber_libvidcap::initialize_(void)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
}


void _FrameGrabber_libvidcap::start_(void)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_FrameGrabber_libvidcap class not implmented yet");
}


#endif	//	URIVL_USE_LIBVIDCAP
