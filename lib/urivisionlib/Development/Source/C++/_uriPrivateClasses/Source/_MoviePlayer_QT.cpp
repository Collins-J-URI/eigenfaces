/*  NAME:
        _MoviePlayer_QT.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib _MoviePlayer_QT class
 
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

#include <iostream>
//
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"
#include "_MoviePlayer_QT.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

_MoviePlayer_QT::_MoviePlayer_QT(void)
		:	movie_(NULL),
			controller_(NULL),
			movieRect_(NULL),
			destination_(NULL),
			duration_(0),
			currentTime_(0),
			nextTime_(0),
			nbFrames_(0L),
			frameIndex_(0L)
{
}

_MoviePlayer_QT::_MoviePlayer_QT(const _MoviePlayer_QT& theObj)
		:	movie_(NULL),
			controller_(NULL),
			movieRect_(NULL),
			destination_(NULL),
			duration_(0),
			currentTime_(0),
			nextTime_(0),
			nbFrames_(0L),
			frameIndex_(0L)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "_MoviePlayer_QT copy constructor not implemented.");
}


_MoviePlayer_QT::~_MoviePlayer_QT(void)
{
    delete movieRect_;
}

const _MoviePlayer_QT& _MoviePlayer_QT::operator = (const _MoviePlayer_QT& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "_MoviePlayer_QT copy operator not implemented.");

	return *this;
}
#if 0
#pragma mark -
#endif


void _MoviePlayer_QT::setSource_(void)
{
    uriVL::simpleGetMovie_(&movie_);

    //  Having succesfully gotten a reference to the movie, we can extract information
    //  about its dimensions, duration, etc.
    initializeMovie_();
}


void _MoviePlayer_QT::setSource_(const char* const thePath)
{
    short   resId = DoTheRightThing;
    OSErr   myErr = noErr;
    
    //------------------------------------------------------------------------------------
    //  The XCode source utilizes the latest Carbon demos of the Apple SDK (as of 2007/04/07).
    //  Of course, since all these demos suck massively, and many don't even compile, 
    //  who knows what is the correct way to do any of that in Carbon anymore?
    //------------------------------------------------------------------------------------
    #if URIVL_XCODE 
    	CFStringRef inPath = CFStringCreateWithCString(NULL, thePath, 
    	                                               CFStringGetSystemEncoding());
        FAIL_CONDITION( !inPath,
                        kFileIOError,
                        "Could not get CFString in _MoviePlayer_QT::setSource");
    	
    	// create the data reference
        Handle  myDataRef;
    	OSType  myDataRefType;
        myErr = QTNewDataReferenceFromFullPathCFString(inPath, kQTNativeDefaultPathStyle, 0, 
                                                       &myDataRef, &myDataRefType);
        FAIL_CONDITION( myErr,
                        kFileIOError,
                        "Could not get DataRef in _MoviePlayer_QT::setSource");

        // get the Movie
        myErr = NewMovieFromDataRef(&movie_, newMovieActive,
                                    &resId, myDataRef, myDataRefType);
        FAIL_CONDITION( myErr,
                        kFileIOError,
                        "Could not get Movie from DataRef in _MoviePlayer_QT::setSource");

        // dispose the data reference handle - we no longer need it
        DisposeHandle(myDataRef);

    //------------------------------------------------------------------------------------
    //  The Windows code is based on the ASCIIMoviePlayerSample code
    //------------------------------------------------------------------------------------
    #elif URIVL_OS_WIN32 

    	FSSpec theFSSpec;
        short  resRefNum = -1;

        char    zePath[256];
        strcpy(zePath, thePath);
        myErr = NativePathNameToFSSpec(zePath, &theFSSpec, kErrorIfFileNotFound /* flags */);
        FAIL_CONDITION( myErr,
                        kFileIOError,
                        "NativePathNameToFSSpec failed in _MoviePlayer_QT::setSource");

        myErr = OpenMovieFile(&theFSSpec, &resRefNum, 0);
        FAIL_CONDITION( myErr,
                        kFileIOError,
                        "OpenMovieFile failed in _MoviePlayer_QT::setSource");

    		
//    	myErr = NewMovieFromFile(&movie_, resRefNum, &resId, (unsigned char* ) 0, 0, 
//    	                        (Boolean *) 0);
    	Str255  movieName;	
    	Boolean wasChanged;
    	myErr = NewMovieFromFile(&movie_, resRefNum, &resId, movieName, newMovieActive, 
    	                         &wasChanged);
        FAIL_CONDITION( myErr,
                        kFileIOError,
                        "NewMovieFromFile failed in _MoviePlayer_QT::setSource");

    	if (resRefNum != -1)
    		CloseMovieFile(resRefNum);
    #else   
        #error  I should not be ending up here
    #endif

    //------------------------------------------------------------------------------------
    //  Having succesfully gotten a reference to the movie, we can extract information
    //  about its dimensions, duration, etc.
    //------------------------------------------------------------------------------------
    initializeMovie_();
}

void _MoviePlayer_QT::setDestination_(RasterImage* destination)
{
    FAIL_CONDITION( destination == NULL,
                    kNullRasterImageError,
                    "Attempt to set a null RasterImage as destination for a _MoviePlayer_QT");

/*    FAIL_CONDITION( !(destination->contains(movieRect_)),
                    kInvalidRectangleError,
                    "the movie's rectangle is not contained by the destination image in oviePlayer_QT::setDestination_");
            
*/
    if (destination != destination_)
    {
        if (destination->contains(movieRect_))
            destination->setValidRect(movieRect_);
        else
            destination->setValidRect(destination->getBoundRect());
            
        destination_ = destination;
        destination->setAsMovieGraphWorld(movie_);

    }
}


#if 0
#pragma mark -
#endif


RasterImage* _MoviePlayer_QT::getFrame_(int n)
{
    //  By default (so far), read into an RGBa image
	RasterImage* img = new RasterImage_RGBa(movieRect_);
	getFrame_(n, img);
	return img;
}


void _MoviePlayer_QT::getFrame_(int n, RasterImage* imgOut)
{
    FAIL_CONDITION( imgOut == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as destination to a movie player");
    FAIL_CONDITION( n < 1, 
                    kInvalidParameterError, 
                    "movie player asked to skip invalid number of frames");
    setDestination_(imgOut);
    
    if (nextTime_ != -1)
    {
        OSType	mediaType = VideoMediaType;

        //	skip n-1 frames
        for (int k=1; (k < n) && (nextTime_ != -1); k++)
        {
            frameIndex_++;

            // Get next sample, stop if the time value for next sample is -1 (no more samples)
            GetMovieNextInterestingTime(movie_, nextTimeMediaSample, 1, &mediaType,
                                        currentTime_, 0, &nextTime_, NULL);
            
        }
        if (nextTime_ != -1)
        {
            currentTime_ = nextTime_;
            SetMovieTimeValue(movie_, currentTime_);
            UpdateMovie(movie_);
            MoviesTask(movie_, 0);
            // Get next sample (the end if the time value for next sample is -1, 
            //  i.e no more samples)
            GetMovieNextInterestingTime(movie_, nextTimeMediaSample, 1, &mediaType,
                                        currentTime_, 0, &nextTime_, NULL);

            frameIndex_++;
//            imgOut->restoreGraphWorld( );
        }
    }

}


#if 0
#pragma mark -
#endif

bool _MoviePlayer_QT::isFinished_(void) const
{
    return (nextTime_ == -1);
}

void _MoviePlayer_QT::goToBeginning_(void)
{
    currentTime_ = nextTime_ = -1;
    frameIndex_ = 0;

	TimeValue   interestingDuration = 0;
	OSType  mediaType = VideoMediaType;
    GetMovieNextInterestingTime(movie_, nextTimeMediaSample+nextTimeEdgeOK, (TimeValue)1, 
                                &mediaType, 0, fixed1, &nextTime_, &interestingDuration);

}


int _MoviePlayer_QT::getWidth_(void) const
{
    return movieRect_->getWidth();
}

int _MoviePlayer_QT::getHeight_(void) const
{
    return movieRect_->getWidth();
}

const ImageRect* _MoviePlayer_QT::getRect_(void) const
{
    return movieRect_;
}

long _MoviePlayer_QT::getCurrentTime_(void) const
{
    return static_cast<long>(currentTime_);
}


long _MoviePlayer_QT::getNextTime_(void) const
{
    return static_cast<long>(nextTime_);
}

long _MoviePlayer_QT::getFrameIndex_(void) const
{
    return frameIndex_;
}

#if 0
#pragma mark -
#endif

void _MoviePlayer_QT::initializeMovie_(void)
{
	OSErr 			myErr = noErr;

    //  Dimensions of the movie
    URIVL_Rect  bounds;
	GetMovieBox(movie_, &bounds);
    
    //  This is not pretty.  I need to bypass typecasting problems with the Win32 version
    movieRect_ = new ImageRect(bounds);
    movieRect_->setOrigin(0, 0);
    bounds.left = bounds.top = 0;
    bounds.right = movieRect_->getWidth();
    bounds.bottom = movieRect_->getHeight();
	SetMovieBox(movie_, &bounds); 
    
    //  duration and frame rate
    duration_ = GetMovieDuration(movie_);
    
//    long timeScale = GetMovieTimeScale(movie_);
    currentTime_ = nextTime_ = -1;
    frameIndex_ = 0;

	TimeValue   interestingDuration = 0;
	OSType  mediaType = VideoMediaType;
    GetMovieNextInterestingTime(movie_, nextTimeMediaSample+nextTimeEdgeOK, (TimeValue)1, 
                                &mediaType, 0, fixed1, &nextTime_, &interestingDuration);
	myErr = GetMoviesError(); 
	nbFrames_ = static_cast<long>(duration_ / interestingDuration);
}

#if 0
#pragma mark -
#endif

//======================================================================================
//  Modified pieces of Apple's crappy so-called "SDK" go here.
//  At least now we don't need the even-worse "QT Framework".
//======================================================================================


/*______________________________________________________________________
	simpleGetMovie_ - Get a Movie from a specific file (simpler version)

void simpleGetMovie_(Movie *theMovie)

theMovie				will contain the selected movie when function exits.

DESCRIPTION
	simpleGetMovie_ is a simplified version of getting a movie from a file, no need for
	returning refnums, res IDs of keeping track of FSSpecs (compared with QTUGetMovie)

*/

void uriVL::simpleGetMovie_(Movie *theMovie)
{
	OSErr 			myErr = noErr;
    short           resRefNum = -1;
	short			resID = 0;
	Str255			movieName;
	Boolean			wasChanged;
    const int       kTypeListCount = 1;
	OSType 			myTypeList[kTypeListCount] = {kQTFileTypeMovie/*, kQTFileTypeQuickTimeImage*/};
	FSSpec			theFSSpec;

    myErr = getOneFileWithPreview_(kTypeListCount, (TypeListPtr)&myTypeList, &theFSSpec, NULL);
    FAIL_CONDITION( myErr,
                    kFileIOError,
                    "Could not open dialog window with preview in _MoviePlayer_QT::setSource");

	myErr = OpenMovieFile(&theFSSpec, &resRefNum, fsRdPerm); 
    FAIL_CONDITION( myErr,
                    kFileIOError,
                    "Could not open movie file in _MoviePlayer_QT::setSource");


	myErr = NewMovieFromFile(theMovie, resRefNum, &resID, movieName, newMovieActive, &wasChanged);
    FAIL_CONDITION( myErr,
                    kFileIOError,
                    "Could not create movie object in _MoviePlayer_QT::setSource");

	if (resRefNum != -1)
    	CloseMovieFile(resRefNum);

}

#endif

