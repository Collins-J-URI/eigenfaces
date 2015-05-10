/*  NAME:
        SequenceGrabber.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib SequenceGrabber class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "SequenceGrabber.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"

using namespace std;
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructors
//----------------------------------------------------------

SequenceGrabber::SequenceGrabber(void)
	: VideoSource( )
{
    videoRect_ = NULL;
    destination_ = NULL;
    sequenceGrabber_ = NULL;
    videoChannel_ = NULL;
}

SequenceGrabber::~SequenceGrabber(void)
{
    delete videoRect_;
}


#if 0
#pragma mark -
#endif

void SequenceGrabber::setSource(void)
{
	sequenceGrabber_ = NULL;
	OSErr			 err = noErr;

    // open the default sequence grabber
    sequenceGrabber_ = OpenDefaultComponent(SeqGrabComponentType, 0);
    if (sequenceGrabber_ != NULL) { 
        // initialize the default sequence grabber component
        err = SGInitialize(sequenceGrabber_);

        if (err == noErr)
        {
        }
        err = SGSetDataRef(sequenceGrabber_,
                           0,
                           0,
                           seqGrabDontMakeMovie);
        if (err == noErr)
        {
        }
                               
    //      temporary
    videoRect_ = new ImageRect(0, 0, 640, 480);
    Rect    rect;
    rect.top = rect.left = 0;
    rect.right=640;
    rect.bottom=480;

        err = SGNewChannel(sequenceGrabber_, VideoMediaType, &videoChannel_);
        if (err == noErr) {
            long  flags = 0;
            err = SGSetChannelBounds(videoChannel_, &rect);
            if (err == noErr)
                // set usage for new video channel to avoid playthrough
                // note we don't set seqGrabPlayDuringRecord
                err = SGSetChannelUsage(videoChannel_, flags | seqGrabRecord );
            
            if (err != noErr) {
                // clean up on failure
                SGDisposeChannel(sequenceGrabber_, videoChannel_);
                videoChannel_ = NULL;
            }
        }
    }
}

void SequenceGrabber::setDestination(RasterImage *destination)
{
    FAIL_CONDITION( destination == NULL,
                    kNullRasterImageError,
                    "Attempt to set a null RasterImage as destination for a SequenceGrabber");

/*    FAIL_CONDITION( !(destination->contains(movieRect_)),
                    kInvalidRectangleError,
                    "the movie's rectangle is not contained by the destination image in oviePlayer_QT::setDestination_");
            
*/
    if (destination != destination_)
    {
//       if (destination->contains(movieRect_))
//            destination->setValidRect(movieRect_);
//        else
//            destination->setValidRect(destination->getBoundRect());
            
        destination_ = destination;
        destination->setAsGrabberGraphWorld(sequenceGrabber_, videoChannel_, videoRect_, &decomSeq_);
    }
}


#pragma mark -

RasterImage *SequenceGrabber::getFrame(void)
{
	return getFrame(1);
;
}


RasterImage *SequenceGrabber::getFrame(short n)
{
	RasterImage	*imgOut = NULL;//new RasterImage_RGBa(movieRect_);
	getFrame_(n, imgOut);
	
	return imgOut;
}


void SequenceGrabber::getFrame(RasterImage *imgOut)
{
    FAIL_CONDITION( imgOut == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as destination to a sequence grabber");
	
	getFrame_(1, imgOut);
}

void SequenceGrabber::getFrame(short n, RasterImage *imgOut)
{
    FAIL_CONDITION( imgOut == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as destination to a sequence grabber");
	
	getFrame_(n, imgOut);	
}


#pragma mark -


short SequenceGrabber::getWidth(void)
{
    if (videoRect_ != NULL)
        return videoRect_->getWidth();
    else
    	return 0;
}

short SequenceGrabber::getHeight(void)
{
    if (videoRect_ != NULL)
        return videoRect_->getHeight();
    else
    	return 0;
}

ImageRect *SequenceGrabber::getRect(void)
{
	return videoRect_;
}



#pragma mark -




void SequenceGrabber::getFrame_(short n, RasterImage *imgOut)
{
    if (n>0)
    {
        OSErr	err = noErr;
        OSType	mediaType = VideoMediaType;

        
        //	skip n-1 frames
        for (short i = 1; i < n; i++)
        {

            err = SGIdle(sequenceGrabber_);
    //        SetMovieTimeValue(movie_, currentTime_);
    //        UpdateMovie(movie_);
    //        MoviesTask(movie_, 0);

            
        }
    }
    imgOut->setValidRect(videoRect_);
}




