/*  NAME:
        _FrameGrabber_QT.cpp

    DESCRIPTION:
        implementation of the uriVisionLib _FrameGrabber_QT class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
 
//	Mostly there to facilitate life for Visual Studio developers, since in Xcode and
//	Cmake souce files are excluded directly from the project depending on the
//	state defined by the environment variables.
#if URIVL_USE_QUICKTIME

#include <cstdio>
//
#include "_FrameGrabber_QT.h"
#include "_GraphWorld_QT.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------
//	Definition of private free functions
//----------------------------------------------
namespace uriVL
{
	void *startGrabberThread_(void *arg); 

	pascal OSErr grabberDataProc_(SGChannel channel, Ptr data, long len, long *offset, long chRefCon, 
								  TimeValue time, short writeType, long param);

}


#if DEBUG_TO_FILE_LEVEL_LIB > 0

FILE* debugFG;
int localFrameIndex = 0;

bool initialize(void){
	debugFG = fopen("frameGrabberDebug.txt", "w");							///< write to this file to debub
	fprintf(debugFG, "\nInitialize FrameGrabber called\n");
	fflush(debugFG);
}

bool okGrabber = initialize();

#endif	





//===========================================================================
//	Class starts here
//===========================================================================

_FrameGrabber_QT::_FrameGrabber_QT(const ImageRect* rect) 
	:	frameIndex_(0),
		//
		grabImg_(NULL),
		destImg_(NULL),
		destGWorld_(NULL),
		seqGrab_(NULL),
		sgChannel_(NULL),
		imageSeq_(NULL),
		timeScale_(-1),
		grabThread_(),
		rasterLock_(),
		
		sourceRect_(),
		destRect_()
{
	destImg_ = new RasterImage_RGBa(rect);
	destImg_->setValidRect(rect);
	
	destGWorld_ = destImg_->gworld_->graphWorld_->gworld_;
	
	pthread_mutex_init(&rasterLock_, NULL);
}

_FrameGrabber_QT::~_FrameGrabber_QT(void)
{
/*
    if(pMungData) {
        if (pMungData->decomSeq)
        	CDSequenceEnd(pMungData->decomSeq);
        if (pMungData->drawSeq)
        	CDSequenceEnd(pMungData->drawSeq);
        if (pMungData->seqGrab)
    		CloseComponent(pMungData->seqGrab);
        if (pMungData->pGWorld) {
        	DisposeGWorld(pMungData->pGWorld);
        	pMungData->pGWorld = NULL;
        }
        DisposePtr((Ptr)pMungData);
        pMungData = NULL;
    }
*/
}

void _FrameGrabber_QT::setSource_(void)
{
	OSErr err = noErr;
	
	//	We specify what we expect from our camera: pretty light on requirements here.
	//	It has to operate as a sequence grabber component, can be DV (subType = 0),
	//	and we don't care about the manufacturer or other features.
	ComponentDescription compDesc;	
	compDesc.componentType = SeqGrabComponentType;
	compDesc.componentSubType = 0L;
	compDesc.componentManufacturer = 0L;
	compDesc.componentFlags = 0L;
	compDesc.componentFlagsMask = 0L;
	
	//	How many components verify these specs?
	int compCount = CountComponents(&compDesc);
cout << "found " << compCount << " sequence grabber components" << endl;
//GetComponentInfo();

    // open the default sequence grabber
    seqGrab_ = OpenDefaultComponent(SeqGrabComponentType, 0);
	
	FAIL_CONDITION( seqGrab_ == NULL,
					kOpenVideoComponentError,
					"Could not find a SequenceGrabber component to open");
					
	// initialize the default sequence grabber component
    err = SGInitialize(seqGrab_);
	if (err != noErr)
	{
    	CloseComponent(seqGrab_);
        seqGrab_ = NULL;
	
		FAIL_CONDITION( true,
						kVideoInitializingError,
						"Could not initialize the SequenceGrabber component");
	}
	
	//	set my destination image's GWorld as the SGworld for this sequence grabber
    err = SGSetGWorld(seqGrab_, destGWorld_, NULL);
	if (err != noErr)
	{
    	CloseComponent(seqGrab_);
        seqGrab_ = NULL;
	
		FAIL_CONDITION( true,
						kVideoInitializingError,
						"Could assign a GWorld to the SequenceGrabber component");
	}

	// specify the destination data reference for a record operation
	// tell it we're not making a movie
	// if the flag seqGrabDontMakeMovie is used, the sequence grabber still calls
	// your data function, but does not write any data to the movie file
	// writeType will always be set to seqGrabWriteAppend
	err = SGSetDataRef(seqGrab_,
					   0,
					   0,
					   seqGrabDontMakeMovie);
	if (err != noErr)
	{
    	CloseComponent(seqGrab_);
        seqGrab_ = NULL;
	
		FAIL_CONDITION( err != noErr,
						kVideoInitializingError,
						"Could assign a a destination data reference to the SequenceGrabber component");
	}

    long  flags = 0;
    err = SGNewChannel(seqGrab_, VideoMediaType, &sgChannel_);
	FAIL_CONDITION( err != noErr,
					kVideoInitializingError,
					"Failed to create a SGChannel in _FrameGrabber_QT::setSource_");

	#if URIVL_OS_MACOS
		GetPortBounds(destGWorld_, &destRect_);
	#elif URIVL_OS_WIN32
		destRect_ = destGWorld_->portRect;
	#endif

    err = SGSetChannelBounds(sgChannel_, &destRect_);

	FAIL_CONDITION( err != noErr,
					kVideoInitializingError,
					"Failed to set SGChannel bounds in _FrameGrabber_QT::setSource_");

	// set usage for new video channel to avoid playthrough
	// note we don't set seqGrabPlayDuringRecord
	err = SGSetChannelUsage(sgChannel_, flags | seqGrabRecord);
	    
	if (err != noErr) 
	{
		// clean up on failure
		SGDisposeChannel(seqGrab_, sgChannel_);
		sgChannel_ = NULL;
    	CloseComponent(seqGrab_);
        seqGrab_ = NULL;
	
		FAIL_CONDITION( true,
						kVideoInitializingError,
						"Failed to set SGChannel usage in _FrameGrabber_QT::setSource_");
    }

}

void _FrameGrabber_QT::initialize_(void)
{

}

void _FrameGrabber_QT::start_(void)
{
	//	Finally, create the grabber's thread
	pthread_create(&grabThread_, NULL, uriVL::startGrabberThread_, this);
	
	//	then leave some time for the grabber to do its business
	#if URIVL_OS_WIN32
		Sleep(33);
	#else
		usleep(5000);
	#endif
}


RasterImage* _FrameGrabber_QT::getFrame_(void)
{
	RasterImage_RGBa* grabImg = new RasterImage_RGBa(destImg_->getBoundRect());
	getFrame_(grabImg);
	return grabImg;
}

void _FrameGrabber_QT::getFrame_(RasterImage* grabImg)
{
	grabImg_ = grabImg;
	
#if DEBUG_TO_FILE_LEVEL_LIB > 0
	fprintf(debugFG, "Lock on rasterLock for frame %d\n", localFrameIndex);
	fflush(debugFG);
#endif
	//	first, block on the raster lock
	pthread_mutex_lock(&rasterLock_);

	//	when I wake up, my rast image is ready, so I just have to 
	//	prevent further copies while this image is being processed
	//	(until the next call to this function)
	grabImg_ = NULL;

#if DEBUG_TO_FILE_LEVEL_LIB > 0
	fprintf(debugFG, "\t ---> got frame %d\n\n", localFrameIndex);
	fflush(debugFG);
	localFrameIndex++;
#endif
}

int _FrameGrabber_QT::getWidth_(void)
{
    	return 0;
}

int _FrameGrabber_QT::getHeight_(void)
{
    	return 0;
}

const ImageRect* _FrameGrabber_QT::getRect_(void)
{
	return NULL;
}

long _FrameGrabber_QT::getFrameIndex_(void)
{
	return frameIndex_;
}


#if 0
#pragma mark -
#endif
//======================================================================
//	Free functions dealing with the creation of the graber thread

void *uriVL::startGrabberThread_(void *arg) 
{
	_FrameGrabber_QT *grabber = (_FrameGrabber_QT*) arg;

#if DEBUG_TO_FILE_LEVEL_LIB > 0
	fprintf(debugFG, "Lock on rasterLock\n");
	fflush(debugFG);
#endif

	//	First, I lock the mutex controling access to the grabbing raster
	pthread_mutex_lock(&(grabber->rasterLock_));
	
	// specify a sequence grabber data function
	OSErr err = SGSetDataProc(grabber->seqGrab_, NewSGDataUPP(grabberDataProc_), (long)grabber);
	
	err = SGStartRecord(grabber->seqGrab_);
	if (err != noErr) 
	{
		// clean up on failure
		SGDisposeChannel(grabber->seqGrab_, grabber->sgChannel_);
		grabber->sgChannel_ = NULL;
		CloseComponent(grabber->seqGrab_);
		grabber->seqGrab_ = NULL;
	
		FAIL_CONDITION( true,
						kVideoInitializingError,
						"Failed to set start sequence grabber in startGrabberThread_");
	}

	while(true)
	{
		err = SGIdle(grabber->seqGrab_);
		if (err /*&& err != pMungData->err*/) {
			// some error specific to SGIdle occurred - any errors returned from the
			// data proc will also show up here and we don't want to write over them
			
			// in QT 4 you would always encounter a cDepthErr error after a user drags
			// the window, this failure condition has been greatly relaxed in QT 5
			// it may still occur but should only apply to vDigs that really control
			// the screen
			
			// you don't always know where these errors originate from, some may come
			// from the VDig...
			// ...to fix this we simply call SGStop and SGStartRecord again
			// calling stop allows the SG to release and re-prepare for grabbing
			// hopefully fixing any problems, this is obviously a very relaxed
			// approach
			SGStop(grabber->seqGrab_);
			SGStartRecord(grabber->seqGrab_);
		}
		
		#if URIVL_OS_WIN32
			Sleep(33);
		#else
			usleep(5000);
		#endif
	}
}

pascal OSErr uriVL::grabberDataProc_(SGChannel channel, Ptr data, long len, long *offset, long chRefCon, 
									 TimeValue time, short writeType, long param)
{
	_FrameGrabber_QT *grabber = (_FrameGrabber_QT*) param;

	//	This is a perfect example of the way the QuickTime API sucks.  I could not complete my
	//	setup before entering the grabber loop (i.e. in the start_ instance function, where this
	//	belongs.  I actually have to have a "first frame special case".  Ridiculous
	if (grabber->frameIndex_ == 0)
	{
		OSErr err = SGGetChannelTimeScale(grabber->sgChannel_, &(grabber->timeScale_));
		if (err != noErr) 
		{
			// clean up on failure
			SGDisposeChannel(grabber->seqGrab_, grabber->sgChannel_);
			grabber->sgChannel_ = NULL;
			CloseComponent(grabber->seqGrab_);
			grabber->seqGrab_ = NULL;
		
			FAIL_CONDITION( true,
							kVideoInitializingError,
							"Failed to set SGChannel timescale in grabberDataProc_");
		}

		// retrieve a channel’s current sample description, the channel returns a sample description that is
		// appropriate to the type of data being captured
		ImageDescriptionHandle imageDesc = (ImageDescriptionHandle)NewHandle(0);
		err = SGGetChannelSampleDescription(grabber->sgChannel_, (Handle)imageDesc);
		if (err != noErr) 
		{
			// clean up on failure
			SGDisposeChannel(grabber->seqGrab_, grabber->sgChannel_);
			grabber->sgChannel_ = NULL;
			CloseComponent(grabber->seqGrab_);
			grabber->seqGrab_ = NULL;
		
			FAIL_CONDITION( true,
							kVideoInitializingError,
							"Failed to get SGChannel smaple description in grabberDataProc_");
		}

		// make a scaling matrix for the sequence	
		grabber->sourceRect_.left = grabber->sourceRect_.top = 0;
		grabber->sourceRect_.right = (**imageDesc).width;
		grabber->sourceRect_.bottom = (**imageDesc).height;
		MatrixRecord scaleMatrix;		
		RectMatrix(&scaleMatrix, &(grabber->sourceRect_), &(grabber->destRect_));
		
		// begin the process of decompressing a sequence of frames
		// this is a set-up call and is only called once for the sequence - the ICM will interrogate different codecs
		// and construct a suitable decompression chain, as this is a time consuming process we don't want to do this
		// once per frame (eg. by using DecompressImage)
		// for more information see Ice Floe #8 http://developer.apple.com/quicktime/icefloe/dispatch008.html
		// the destination is specified as the GWorld
		err = DecompressSequenceBegin(&(grabber->imageSeq_),								// pointer to field to receive unique ID for sequence
									  imageDesc,								// handle to image description structure
									  grabber->destGWorld_,   // port for the DESTINATION image
									  NULL,										// graphics device handle, if port is set, set to NULL
									  NULL,										// source rectangle defining the portion of the image to decompress 
									  &scaleMatrix,								// transformation matrix
									  srcCopy,									// transfer mode specifier
									  (RgnHandle)NULL,							// clipping region in dest. coordinate system to use as a mask
									  NULL,										// flags
									  codecNormalQuality,						// accuracy in decompression
									  bestSpeedCodec);							// compressor identifier or special identifiers ie. bestSpeedCodec

		DisposeHandle((Handle)imageDesc);         

		FAIL_CONDITION( err != noErr,
						kVideoInitializingError,
						"Failed to set start sequence decompression in _FrameGrabber_QT::setSource_");

	}

	//	decompress an image into destImg_
    CodecFlags	ignore;
	OSErr err = DecompressSequenceFrameS(grabber->imageSeq_,	// sequence ID returned by DecompressSequenceBegin
										 data,					// pointer to compressed image data
										 len,					// size of the buffer
										 0,						// in flags
										 &ignore,				// out flags
										 NULL);					// async completion proc
	
	//	has somebody requested a copy of the frame we just grabbed?
	//	That someone will be blocked on the rasterLock_
	if (grabber->grabImg_ != NULL) 
	{
		grabber->destImg_->copyInto(grabber->grabImg_);
		
#if DEBUG_TO_FILE_LEVEL_LIB > 0
	fprintf(debugFG, "Unlock...");
	fflush(debugFG);
#endif

		//	and then we wake up that somebody
		pthread_mutex_unlock(&(grabber->rasterLock_));
		
		grabber->grabImg_ = NULL;

#if DEBUG_TO_FILE_LEVEL_LIB > 0
	fprintf(debugFG, "on rasterLock\n");
	fflush(debugFG);
#endif
	}

	grabber->frameIndex_++;
	
	return err;
} 

#endif
