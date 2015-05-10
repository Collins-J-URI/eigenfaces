/*  NAME:
        _MoviePlayer_libquicktime.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib _MoviePlayer_libquicktime class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

//	Mostly there to facilitate life for Visual Studio developers, since in Xcode and
//	Cmake souce files are excluded directly from the project depending on the
//	state defined by the environment variables.
#if URIVL_USE_LIBQUICKTIME


#include <cstring>
#include <cstdio>
//
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"
#include "_MoviePlayer_libquicktime.h"

using namespace uriVL;
using namespace std;

#define TRACK 0

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

_MoviePlayer_libquicktime::_MoviePlayer_libquicktime(void)
		:	libFile(NULL),
			movieRect_(NULL),
			frameIndex_(0L),
			filePath_(NULL)
{
}

_MoviePlayer_libquicktime::~_MoviePlayer_libquicktime(void)
{
	delete movieRect_;
}

#if 0
#pragma mark -
#endif


void _MoviePlayer_libquicktime::setSource_(void)
{
	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"GUI version of _MoviePlayer_libquicktime::setSource_ not implemented");
}

void _MoviePlayer_libquicktime::setSource_(const char* thePath)
{
	//libFile is the basic wrapper class for the movie file
	libFile = lqt_open_read(thePath);
	
	FAIL_CONDITION(	libFile == NULL,
					kFileNotFoundError,
					"video file not found");

	FAIL_CONDITION(	!quicktime_video_tracks(libFile),
					kVideoSourceError,
					"video file does not have any tracks");
	
	try 
	{
		//this defines which part of the video image is valid
		movieRect_=new ImageRect(0,0,quicktime_video_width(libFile,0),quicktime_video_height(libFile,0));
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by _MoviePlayer_libquicktime::setSource_");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in _MoviePlayer_libquicktime::setSource_");
	}
    
    //not sure we need this for generic
    //keeps track of which frame we're on
    frameIndex_ = 0;
}

void _MoviePlayer_libquicktime::setDestination_(RasterImage* destination)
{
	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"_MoviePlayer_libquicktime::setDestination_ not implemented");
}


#if 0
#pragma mark -
#endif

RasterImage* _MoviePlayer_libquicktime::getFrame_(int n)
{
	FAIL_CONDITION( libFile==NULL,
					kNoninitializedObjectError,
					"Source not set");
//	FAIL_CONDITION(	true,
//					kFunctionNotImplemented,
//					"_MoviePlayer_libquicktime::getFrame_(int n) not implemented");
	printf("_MoviePlayer_libquicktime::getFrame_(int n) not fully implemented yet");
	return NULL;
}


void _MoviePlayer_libquicktime::getFrame_(int n, RasterImage* imgOut)
{
		
    FAIL_CONDITION( imgOut == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as destination to a movie player");
    FAIL_CONDITION( n < 1, 
                    kInvalidParameterError, 
                    "movie player asked to skip invalid (negative) number of frames");
    
    //from _QT version setDestination
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "Attempt to set a null RasterImage as destination for a _MoviePlayer_QT");
    FAIL_CONDITION( !(imgOut->contains(movieRect_)),
                    kInvalidRectangleError,
                    "the movie's rectangle is not contained by the destination image in oviePlayer_QT::setDestination_");
    
	try
	{
		imgOut->setValidRect(movieRect_);
		
		//int width=quicktime_video_width(libFile,TRACK);
		
		//int height=quicktime_video_height(libFile,TRACK);
	 
		int cmodel=lqt_get_cmodel(libFile,TRACK);

//TODO: should not have to set this for every frame
		//defines the format of the output buffer
		lqt_set_cmodel(libFile,TRACK, 7);
		
		FAIL_CONDITION( !quicktime_reads_cmodel(libFile,cmodel,TRACK),
						kVideoSourceError,
						"Invalid cmodel of video (unrecognized format?)");
		
		//set the buffer to be the RasterImage    				
		uint8_t** buffer = const_cast<uint8_t**>(imgOut->getRaster2D(R_W_ACCESS));
		
		//TODO: Could initialize buffer to 0 if we knew the size.
		for(int i=0;i<n;i++)
			lqt_decode_video(libFile, buffer, TRACK);
		
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by _MoviePlayer_libquicktime::getFrame_");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in _MoviePlayer_libquicktime::getFrame_");
	}
}

#if 0
#pragma mark -
#endif

int _MoviePlayer_libquicktime::getWidth_(void)
{
	printf("Warning (_MoviePlayer_libquicktime): Function getWidth_ not implemented yet\n");
    return 0;
}

int _MoviePlayer_libquicktime::getHeight_(void)
{
    return quicktime_video_height(libFile,TRACK);
}

ImageRect* _MoviePlayer_libquicktime::getRect_(void)
{
    return movieRect_;
}

bool _MoviePlayer_libquicktime::isFinished_(void) const
{
	int total = quicktime_video_length(libFile,TRACK);
	int current = quicktime_video_position(libFile,TRACK);
    return current>=total;
}

void _MoviePlayer_libquicktime::goToBeginning_(void) const
{
	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"_MoviePlayer_libquicktime::goToBeginning_ not implemented");
}


long _MoviePlayer_libquicktime::getCurrentTime_(void) const
{
	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"_MoviePlayer_libquicktime::getCurrentTime_ not implemented");

    return static_cast<long>(0);
}


long _MoviePlayer_libquicktime::getNextTime_(void) const
{
	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"_MoviePlayer_libquicktime::getNextTime_ not implemented");

    return static_cast<long>(0);
}

long _MoviePlayer_libquicktime::getFrameIndex_(void) const
{
	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"_MoviePlayer_libquicktime::getFrameIndex_ not implemented");

    return static_cast<long>(0);
}

#endif	//	URIVL_USE_LIBQUICKTIME
