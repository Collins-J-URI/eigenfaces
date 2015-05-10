/*  NAME:
        _FrameGrabber_QT.h

    DESCRIPTION:
        _FrameGrabber_QT public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


#ifndef URIVL_FRAME_GRABBER_QT_H
#define	URIVL_FRAME_GRABBER_QT_H

#if URIVL_USE_QUICKTIME 

#include <pthread.h>
//
#if !URIVL_XCODE
	#include "ErrorReport.h"
#endif
//
#include "VideoSource.h"
#include "RasterImage_RGBa.h"

#include <iostream>

namespace uriVL
{
	/** QuickTime implementation of the reading of video data from a live video source.
	 *	This class only contains private functions and should therefore never be instantiated
	 *	directly in user code.  Instead, instantiate a FrameGrabber object, which will
	 *	create (and store a reference to) a _FrameGrabber_QT object.
	 *
	 *  This is one of the "sausage factories" of the library.  You probably don't want
	 *  to look at it too closely as long as it works properly.  As a further warning to
	 *  adventurous souls:  most of this class is based on code samples of Apple's
	 *  crappy QuickTime "SDK", which is to say that it is ugly, complicated, lacks
	 *  comments, betrays poor design choices,...  Uh, is it clear yet that I don't think
	 *  much of the QuickTime "SDK"?
     *
	 *	@author	 jean-yves herve'	3D Group for Interactive Visualization
	 *                              Dept. of Computer Science and Statistics
	 *                              University of Rhode Island
	 *
	 */
	class URIVL_EXPORT _FrameGrabber_QT
	{
    	private:

    		/**	
    		 *	Index of the current movie frame (last played)
    		 *	@see getFrameIndex
    		 */
    		long	frameIndex_;
			
			
			/**	The raster image to grab the next frame into (really, into which to copy the grab raster).
			 *	Will be set to null when there is no request for a frame.  Set by the getFrame function.
			 *	@see getFrame
			 */
			RasterImage* grabImg_;
			
			/**	The grab destination image
			 */
			RasterImage_RGBa* destImg_;
			
			/**	QuickTime GWorld of the destination raster image
			 */
			GWorldPtr destGWorld_;
			
			
			/**	QT SequenceGrabber object used to capture video data
			 */
			SeqGrabComponent seqGrab_;
			
			/**	Video channel for the sequence grabber
			 */
			SGChannel	sgChannel_;
			
			/**	ImageSequence destination for the grabbing
			 */
			ImageSequence 	imageSeq_;

			/**	Timescale of the frame grabber
			 */
			TimeScale	timeScale_;
			

			/**	Grabber's thread
			 */
			pthread_t grabThread_;
			
            /** Mutex that protects access to the local grabbing raster
             */
            pthread_mutex_t rasterLock_;
			
			/**	Native digitizing rectangle of the video source
			 */
			Rect sourceRect_;
			
			/**	rectangle of the destination GWorld
			 */
			Rect destRect_;
			

    		/** Constructor for the FrameGrabber
			 *	@param	rect	digitizing rectangle
    		 */
    		_FrameGrabber_QT(const ImageRect* rect);

    		/** Copy constructor for the FrameGrabber (disabled)
			 *	@param	obj	the object to copy
    		 */
    		_FrameGrabber_QT(const _FrameGrabber_QT& obj);

    		/** Destructor for the FrameGrabber
    		 */
    		~_FrameGrabber_QT(void);

    		/** Copy operator for the FrameGrabber (disabled)
			 *	@param	obj	the object to copy
    		 */
    		const _FrameGrabber_QT& operator =(const _FrameGrabber_QT& obj);

    		/** Finds the first available video input source capable
    		 *		of digitizing video and defaults to using a 
    		 *		BufferRing of five indicies
    		 */
    		void setSource_(void);

    		/** Returns the last grabbed frame from the BufferRing as a pointer to a 
    		 *  RasterImage object.
    		 */
    		RasterImage* getFrame_(void);

    		/** Returns the next available frame grabbed from the BufferRing
    		 *		that has a timestamp more current than the RasterImage reference
    		 *		that is being passed as a parameter.
    		 *
    		 *  @param	grabImage    a pointer to the RasterImage to grab into
    		 */
    		void getFrame_(RasterImage* grabImage);

    		/**	
    		 *	Returns the preselected video source's width (-1 if no source was selected)
    		 *
    		 *	@return	the preselected movie's width (-1 if no source was selected)
    		 */
    		int getWidth_(void);

    		/**	
    		 *	Returns the preselected video source's height (-1 if no source was selected)
    		 *
    		 *	@return		the preselected video source's height (-1 if no source was selected)
    		 */
    		int getHeight_(void);

    		/**	
    		 *	Returns the bounding rectangle of the current video source.
    		 *
    		 * 	@return bounding rectangle of the current video source
    		 */
    		const ImageRect* getRect_(void);

    		/**	
    		 *	Returns the index of the last frame played
    		 *	@return		index of the last frame played
    		 */
    		long getFrameIndex_(void);

    		/** Once we've acquired a video digitizing component
    		 *	we can set the appropriate flags for it and intialize
    		 *	the buffer ring to prepare the _FrameGrabber_QT for
    		 *		acquiring new images from the video digitizing component.
    		 */
    		void initialize_(void);

			/**	Sets up the timer and starts grabbing frames
			 */
			void start_(void);
			

		friend class FrameGrabber;

		friend void *startGrabberThread_(void *arg);

		#if !URIVL_VISUAL
			friend pascal OSErr uriVL::grabberDataProc_(SGChannel channel, Ptr data, long len, 
														long *offset, long chRefCon, 
														TimeValue time, short writeType, long param);
		#else
			friend pascal OSErr grabberDataProc_(SGChannel channel, Ptr data, long len, 
														long *offset, long chRefCon, 
														TimeValue time, short writeType, long param);
		#endif
	};
	
}

#else // if not using URIVL_USE_QUICKTIME
	#error URIVL_USE_QUICKTIME not defined but this file expected it to be
#endif // use URIVL_USE_QUICKTIME

#endif
