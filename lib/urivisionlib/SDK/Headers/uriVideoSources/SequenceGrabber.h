/*  NAME:
        SequenceGrabber.h

    DESCRIPTION:
        SequenceGrabber public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_SEQUENCE_GRABBER_H
#define URIVL_SEQUENCE_GRABBER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "VideoSource.h"
#include "RasterImage.h"


namespace uriVL
{
    //----------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------
    //--------------------------------Class definition----------------------------------
    //

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
    class URIVL_EXPORT SequenceGrabber : public VideoSource
    {
        public:

			/**	Default constructor
			 */
            SequenceGrabber(void);

			/**	Destructor
			 */
            ~SequenceGrabber(void);


            /**	Select the video source via a dialog window
             */
            void setSource(void);

            /** Sets the destination image for a SequenceGrabber object
             */
            void setDestination(RasterImage *destination);

			/**	Get an image frame from the preselected source.
			 *
			 *	Currently this function only returns <code>RasterImage_RGBa</code>
			 *	objects.  In the future, the function will return an image whose
			 *	type matches the color type of the movie.  User who wish to get a
			 *	specific type of image as output should provide an image of this type
			 *  as parameter.
			 *
			 *	@return		RasterImage object containing the image read
			 */
            RasterImage *getFrame(void);

			/**	Get the nth next image frame from the preselected source, skipping the next
			 *	n-1 frames in the process
			 *
			 *	Currently this function only returns <code>RasterImage_RGBa</code>
			 *	objects.  In the future, the function will return an image whose
			 *	type matches the color type of the movie.  User who wish to get a
			 *	specific type of image as output should provide an image of this type
			 *  as parameter.
			 *
			 *	@param	n	the next frame to get (n-1 skipped)
			 *	@return		RasterImage object containing the image read
			 */
            RasterImage *getFrame(short n);

			/**	Get an image frame from the preselected source
			 *
			 *	@param	img			RasterImage object into which the image should be read
			 */
            void getFrame(RasterImage *img);

			/**	Get the nth next image frame from the preselected source, skipping the next
			 *	n-1 frames in the process
			 *
			 *	@param	n	the next frame to get (n-1 skipped)
			 *	@param	img			RasterImage object into which the image should be read
			 */
            void getFrame(short n, RasterImage *img);


			/**	Returns the preselected video source's width (-1 if no source was selected)
			 *
			 *	@return		the preselected movie's width (-1 if no source was selected)
			 */
			 short getWidth(void);

			/**	Returns the preselected video source's height (-1 if no source was selected)
			 *
			 *	@return		the preselected video source's height (-1 if no source was selected)
			 */
			short getHeight(void);

			/**	Returns the bounding rectangle of the current video source.
			 *
			 *  @return bounding rectangle of the current video source
			 */
			ImageRect *getRect(void);


			/**	Returns the index of the last frame played
			 *	@return		index of the last frame played
			 */
			long getFrameIndex(void);

        private:

			/**	Image rectangle of the current video source
			 */
			ImageRect	*videoRect_;

			/**	Current destination (image to play into)
			 *	@see setImageAsDestination
			 */
			RasterImage	*destination_;

            SeqGrabComponent sequenceGrabber_;

            SGChannel videoChannel_;

            /**     unique identifier for our decompression sequence
             */
            ImageSequence decomSeq_;

			/**	Index of the current movie frame (last played)
			 *	@see getFrameIndex
			 */
			long	frameIndex_;

			/**	Get the nth next image frame from the preselected source, skipping the next
			 *	n-1 frames in the process
			 *
			 *	@param	n	the next frame to get (n-1 skipped)
			 *	@param	img			RasterImage object into which the image should be read
			 *
			 *	@see getFrame
			 */
            void getFrame_(short n, RasterImage *img);

    };
}

#endif	//	SEQUENCE_GRABBER_H
