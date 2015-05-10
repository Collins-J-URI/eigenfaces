/*  NAME:
        FrameGrabber.h

    DESCRIPTION:
        FrameGrabber public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#if URIVL_CAN_GRAB_VIDEOS

#ifndef	URIVL_FRAME_GRABBER_H
#define	URIVL_FRAME_GRABBER_H

#if !URIVL_XCODE
	#include "ErrorReport.h"
#endif
//
#include "VideoSource.h"
#include "RasterImage.h"

namespace uriVL
{
	#if URIVL_USE_QUICKTIME
		class _FrameGrabber_QT;
	#else
		class _FrameGrabber_libvidcap;
	#endif
		
	/** Class that handles the reading of video data from a live video source.  This class uses
	 *	composition rather than inheritance to address platform-specific issues.  Depending on
	 *	precompiler settings, an object of this class will include either a _QT implementation
	 *	or some form of a -gen implementation (e.g. libvidcap).
	 *
	 *	@author	 jean-yves herve'	3D Group for Interactive Visualization
	 *                              Dept. of Computer Science and Statistics
	 *                              University of Rhode Island
	 *
	 */
	class URIVL_EXPORT FrameGrabber : public VideoSource
	{
		public:

			/** Constructor for the FrameGrabber class
			 *	@param	rect	digitizing rectangle
			 */
			FrameGrabber(const ImageRect* rect);

			/** Destructor for the FrameGrabber
			 */
			~FrameGrabber(void);

			/** Finds the first available video input source capable
			 *		of digitizing video and defaults to using a 
			 *		BufferRing of five indicies
			 **/
			void setSource(void);

			/**	Starts the frame grabber (creates a thread that continuously
			 *	performs synchronous grabs
			 */
			void start(void);

			/** Returns the last grabbed frame as a 
			 *		pointer to a RasterImage object.
			 */
			RasterImage* getFrame(void);

			/** So far this version of the function only exists for compatibility with
			 *  the parent class.
			 *
			 *	@param	image  pointer to the RasterImage to grab into
			 */
			void getFrame(RasterImage* image);

			/**	
			 *	Returns the preselected video source's width (-1 if no source was selected)
			 *
			 *	@return	the preselected movie's width (-1 if no source was selected)
			 */
			short getWidth(void);

			/**	
			 *	Returns the preselected video source's height (-1 if no source was selected)
			 *
			 *	@return		the preselected video source's height (-1 if no source was selected)
			 */
			short getHeight(void);

			/**	
			 *	Returns the bounding rectangle of the current video source.
			 *
			 *	@return bounding rectangle of the current video source
			 */
			const ImageRect* getRect(void);

			/**	
			 *	Returns the index of the last frame grabbed
			 *	@return		index of the last frame grabbed
			 */
			long getFrameIndex(void);


		private:
			#if URIVL_USE_QUICKTIME
				/**
				 *	Platform-specific implementation of the grabber: QuickTime version
				 */
				_FrameGrabber_QT   *grabber_;
			#else
				/**
				 *	Platform-specific implementation of the grabber: "generic" version
				 */
				_FrameGrabber_libvidcap  *grabber_;
			#endif


	};
	
	

}
#endif	//	URIVL_FRAME_GRABBER_H

#endif // URIVL_CAN_GRAB_VIDEOS
