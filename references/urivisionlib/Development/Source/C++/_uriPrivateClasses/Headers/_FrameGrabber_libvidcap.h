/*  NAME:
        _URIVL_USE_LIBVIDCAP.h

    DESCRIPTION:
        _URIVL_USE_LIBVIDCAP private header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <pthread.h>
//
#ifndef URIVL_FRAME_GRABBER_LIBVIDCAP_H
#define	URIVL_FRAME_GRABBER_LIBVIDCAP_H

#if ! URIVL_USE_QUICKTIME

#if !URIVL_XCODE
	#include "ErrorReport.h"
#endif
//
#include "VideoSource.h"
#include "RasterImage.h"

namespace uriVL
{
	/** "Generic" implementation of the reading of video data from a live video source.
	 *	This class only contains private functions and should therefore never be instantiated
	 *	directly in user code.  Instead, instantiate a FrameGrabber object, which will
	 *	create (and store a reference to) a _FrameGrabber_libvidcap object.
	 *
	 *	@author	 jean-yves herve'	3D Group for Interactive Visualization
	 *                              Dept. of Computer Science and Statistics
	 *                              University of Rhode Island
	 *
	 */
	class URIVL_EXPORT _FrameGrabber_libvidcap
	{
    	private:

    		/** Default constructor for the FrameGrabber
			 *	@param	rect	digitizing rectangle
    		 */
    		_FrameGrabber_libvidcap(const ImageRect* rect);

    		/** Destructor for the FrameGrabber
    		 */
    		~_FrameGrabber_libvidcap(void);

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

	};
}

#else // if using URIVL_USE_QUICKTIME
	#error URIVL_USE_QUICKTIME defined but this  header got loaded
#endif // use URIVL_USE_LIBVIDCAP


#endif	//	URIVL_FRAME_GRABBER_LIBVIDCAP_H

