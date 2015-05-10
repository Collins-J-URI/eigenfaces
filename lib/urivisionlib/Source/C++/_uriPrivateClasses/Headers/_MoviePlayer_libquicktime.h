/*  NAME:
        _MoviePlayer_libquicktime.h

    DESCRIPTION:
        _MoviePlayer_libquicktime public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_MOVIE_PLAYER_LIBQUICKTIME_H
#define URIVL_MOVIE_PLAYER_LIBQUICKTIME_H

#if URIVL_USE_LIBQUICKTIME

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "MoviePlayer.h"
#include "RasterImage.h"

namespace uriVL
{
	/** This class provides a xine-based implementation of a movie
	 *  player.
	 *
	 *	@author	 jean-yves herve'
	 *
	 */
    class URIVL_EXPORT _MoviePlayer_libquicktime
    {
        private:

			/**
			 *the handle to pass to libquicktime 
			 */
			quicktime_s* libFile;

			/**	Image rectangle of the current movie
			 */
			ImageRect* movieRect_;

			/**	Current movie
			 */
//			Movie		movie_;

			/**	Current destination (image to play into)
			 *	@see setImageAsDestination
			 */
//			RasterImage	*destination_;

			/**	Index of the current movie frame (last played)
			 *	@see getFrameIndex
			 */
			long frameIndex_;

			/**	File path for the video source.  Only applies to image files, folders,
			 *	movie files, but not
			 *	to "live" sources (video digitizer, sequence grabber, etc.)
			 */
            char* filePath_;

			/**	Default constructor
			 */
            _MoviePlayer_libquicktime(void);

			/**	Destructor
			 */
            ~_MoviePlayer_libquicktime(void);


            /**	Select the input image file via a dialog window
             */
            void setSource_(void);

            /**	Select the input image file using a file path
             *
             *	@param	fileName	path of the input image file
             */
            void setSource_(const char* thePath);

            /** Sets a destination RasterImage object for this movie
             *
             *  @param  destination     the image to play this movie into
             */
            void setDestination_(RasterImage* destination);


			/**	Get the nth next image frame from the preselected source, skipping the next
			 * 
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
            RasterImage* getFrame_(int n);

			/**	Get the nth next image frame from the preselected source, skipping the next
			 *	n-1 frames in the process
			 *
			 *	@param	n	the next frame to get (n-1 skipped)
			 *	@param	img			RasterImage object into which the image should be read
			 */
            void getFrame_(int n, RasterImage* img);


			/**	Returns the preselected movie's width (-1 if no movie was selected)
			 *
			 *	@return		the preselected movie's width (-1 if no movie was selected)
			 */
			 int getWidth_(void);

			/**	Returns the preselected movie's height (-1 if no movie was selected)
			 *
			 *	@return		the preselected movie's height (-1 if no movie was selected)
			 */
			int getHeight_(void);

			/**	Returns the bounding rectangle of the current movie
			 *  @return bounding rectangle of the current movie
			 */
			ImageRect* getRect_(void);

            /** Indicates whether the movie played by this player is finished
             */
            bool isFinished_(void) const;

			/**	Returns to the beginning of the movie
			 */
			void goToBeginning_(void) const;
			
			/**	Returns the time of the current frame (last played)
			 *	@return	time of the current frame
			 */
			long getCurrentTime_(void) const;

			/**	Returns the time of the next frame
			 *	@return	time of the next frame
			 */
			long getNextTime_(void) const;

			/**	Returns the index of the last frame played
			 *	@return		index of the last frame played
			 */
			long getFrameIndex_(void) const;


//			/**	Prepares the movie for playing
//			 */
//			void initializeMovie_(void);


        friend class MoviePlayer;
        
    };
}
#else // if not using URIVL_USE_LIBQUICKTIME
	#error URIVL_USE_LIBQUICKTIME not defined but this file expected it to be
#endif // use URIVL_USE_LIBQUICKTIME


#endif	//	URIVL_MOVIE_PLAYER_LIBQUICKTIME_H
