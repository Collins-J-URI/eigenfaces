/*  NAME:
        _MoviePlayer_QT.h

    DESCRIPTION:
        _MoviePlayer_QT public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_MOVIE_PLAYER_QT_H
#define URIVL_MOVIE_PLAYER_QT_H

#if URIVL_USE_QUICKTIME

#if URIVL_OS_WIN32
    #include <StandardFile.h>
#endif
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "MoviePlayer.h"
#include "RasterImage.h"

namespace uriVL
{
    //----------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------
    //--------------------------------Class definition----------------------------------
    //

	/**   A simple/lame class that reads raster images from QuickTime movie files.
	 *
	 *  This is one of the "sausage factories" of the library.  You probably don't want
	 *  to look at it too closely as long as it works properly.  As a further warning to
	 *  adventurous souls:  most of this class is based on code samples of Apple's
	 *  crappy QuickTime "SDK", which is to say that it is ugly, complicated, lacks
	 *  comments, betrays poor design choices,...  Uh, is it clear yet that I don't think
	 *  much of the QuickTime "SDK"?
	 *
	 *	Origins of this code:   
	 *  <ul>
	 *      <li> v. 0.7 [March 2007] Implemented _QT and _gen versions of MoviePlayer
	 *      <li> v. 0.8 [April 2007] Integrated ugly code from Apple SDK
	 *  </ul> 
	 *
	 *	@author	 jean-yves herve'	    3D Group for Interactive Visualization
	 *                                  Dept. of Computer Science and Statistics
	 *                                  University of Rhode Island
	 */
    class URIVL_EXPORT _MoviePlayer_QT
    {
        private:

			/**	Current movie
			 */
			Movie		movie_;

            /** Movie controler that plays the movie
             */
            MovieController controller_;
            
			/**	Image rectangle of the current movie
			 */
			ImageRect	*movieRect_;

			/**	Current destination (image to play into)
			 *	@see setImageAsDestination
			 */
			RasterImage	*destination_;
            /** Duration of the current movie
             */
            TimeValue   duration_;
            
			/**	Time of the current frame (last played)
			 *	@see getCurrentTime
			 */
			TimeValue	currentTime_;

			/**	Time of the next frame
			 *	@see getNextTime
			 */
			TimeValue	nextTime_;

            /** Number of frames in the movie
             */
            long nbFrames_;
            
			/**	Index of the current movie frame (last played)
			 *	@see getFrameIndex
			 */
			long frameIndex_;


			/**	Default constructor
			 */
            _MoviePlayer_QT(void);

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            _MoviePlayer_QT(const _MoviePlayer_QT& obj);

			/**	Destructor
			 */
            ~_MoviePlayer_QT(void);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const _MoviePlayer_QT& operator =(const _MoviePlayer_QT& obj);

            /**	Select the input image file via a dialog window
             */
            void setSource_(void);

            /**	Select the input image file using a file path
             *
             *	@param	fileName	path of the input image file
             */
            void setSource_(const char* const thePath);

            /** Sets a destination RasterImage object for this movie
             *
             *  @param  destination     the image to play this movie into
             */
            void setDestination_(RasterImage* destination);


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
            RasterImage* getFrame_(int n);

			/**	Get the nth next image frame from the preselected source, skipping the next
			 *	n-1 frames in the process
			 *
			 *	@param	n	the next frame to get (n-1 skipped)
			 *	@param	img			RasterImage object into which the image should be read
			 */
            void getFrame_(int n, RasterImage* img);


            /** Indicates whether the movie played by this player is finished
             */
            bool isFinished_(void) const;

			/**	Returns to the beginning of the movie
			 */
			void goToBeginning_(void);
			
			/**	Returns the preselected movie's width (-1 if no movie was selected)
			 *
			 *	@return		the preselected movie's width (-1 if no movie was selected)
			 */
			int getWidth_(void) const;

			/**	Returns the preselected movie's height (-1 if no movie was selected)
			 *
			 *	@return		the preselected movie's height (-1 if no movie was selected)
			 */
			int getHeight_(void) const;

			/**	Returns the bounding rectangle of the current movie
			 *  @return bounding rectangle of the current movie
			 */
			const ImageRect* getRect_(void) const;

			/**	Returns the time of the current frame (last played)
			 *	@return	time of the current frame
			 */
			long getCurrentTime_(void) const;

			/**	Returns the time of the next frame
			 *	@return	time of the next frame
			 */
			long getNextTime_(void) const;

			/**	Returns the index of the current frame (last played)
			 *	@return	index of the last frame read
			 */
			long getFrameIndex_(void) const;

			/**	Opens a dialog window to select a movie file
			 */
			OSErr simpleGetMovie_(Movie *theMovie);

			/**	Prepares the movie for playing.
			 */
			void initializeMovie_(void);


        friend class MoviePlayer;
        
    };

    //------------------------------------------------------------------------------
    // Prototypes for the functions of Apple's crap "SDK"
    //------------------------------------------------------------------------------

    void simpleGetMovie_(Movie *theMovie);																		// Simpler version of querying for a movie and return it.
}

#else // if not using URIVL_USE_QUICKTIME
	#error URIVL_USE_QUICKTIME not defined but this file expected it to be
#endif // use URIVL_USE_QUICKTIME


#endif	//	MOVIE_PLAYER_QT_H
