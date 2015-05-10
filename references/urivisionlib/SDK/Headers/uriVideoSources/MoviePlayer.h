/*  NAME:
        MoviePlayer.h

    DESCRIPTION:
        MoviePlayer public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#if URIVL_CAN_PLAY_MOVIES

#ifndef URIVL_MOVIE_PLAYER_H
#define URIVL_MOVIE_PLAYER_H

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

    #if URIVL_USE_QUICKTIME
        class _MoviePlayer_QT;
    #else
        class _MoviePlayer_libquicktime;
    #endif
    

	/** Class that handles the reading of video data from a movie file.  This class uses
	 *	composition rather than inheritance to address platform-specific issues.  Depending on
	 *	precompiler settings, an object of this class will include either a _QT implementation
	 *	or some form of a -gen implementation (e.g. libquicktime).
	 *
	 *	Origins of this code:   
	 *  <ul>
	 *      <li> v. 0.1 [Summer 2003]: Brian Mullen wrote an application that was mixing 
	 *           Apple's SDK code with a very primitive early version of the RasterImage 
	 *           class.  
	 *      <li> v. 0.5 [Spring 2004] Jingjing Hu and I revised Brian's code to work with
	 *	         uriVisionLib (that version had a single, very complex RasterImage class)
	 *           but the code for playing movies was still located in the application's 
	 *           main program.	
	 *      <li> v. 0.6 [March 2005] This version finally turns the movie player into
	 *	         its own class and integrates it into the heavily modified uriVisionlib
	 *	         of Fall 2004.
	 *      <li> v. 0.7 [March 2007] Implemented _QT and _gen versions
	 *  </ul> 
	 *
	 *	@author	 jean-yves herve', Jingjing Hu, Brian Mullen
	 *                                  3D Group for Interactive Visualization
	 *                                  Dept. of Computer Science and Statistics
	 *                                  University of Rhode Island
	 *
	 */
    class URIVL_EXPORT MoviePlayer : public VideoSource
    {
        public:

			/**	Default constructor
			 */
            MoviePlayer(void);

			/**	Destructor
			 */
            ~MoviePlayer(void);


            /**	Select the input image file via a dialog window
             */
            void setSource(void);

            /**	Select the input image file using a file path
             *
             *	@param	thePath	path of the input image file
             */
            void setSource(const char* thePath);

            /** Sets a destination RasterImage object for this movie
             *
             *  @param  destination     the image to play this movie into
             */
            void setDestination(RasterImage* destination);

			/**	Get the next image frame from the preselected source.
			 *
			 *	Currently this function only returns <code>RasterImage_RGBa</code>
			 *	objects.  In the future, the function will return an image whose
			 *	type matches the color type of the movie.  User who wish to get a
			 *	specific type of image as output should provide an image of this type
			 *  as parameter.
			 *
			 *	@return		RasterImage object containing the image read
			 */
            RasterImage* getFrame(void);

			/**	Get the nth next image frame from the preselected source, skipping the next
			 *	n-1 frames in the process.
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
            RasterImage* getFrame(int n);

			/**	Get the next image frame from the preselected source, skipping the next
			 *	n-1 frames in the process.
			 *
			 *	@param	img			RasterImage object into which the image should be read
			 *	@param	n	the next frame to get (n-1 skipped)
			 */
            void getFrame(RasterImage* img);


			/**	Get the nth next image frame from the preselected source, skipping the next
			 *	n-1 frames in the process
			 *
			 *	@param	n	the next frame to get (n-1 skipped)
			 *	@param	img			RasterImage object into which the image should be read
			 */
            void getFrame(int n, RasterImage* img);

            /** Indicates whether the movie played by this player is finished
             */
            bool isFinished(void) const;

			/**	Returns to the beginning of the movie
			 */
			void goToBeginning(void);
			
			/**	Returns the preselected movie's width (-1 if no movie was selected)
			 *
			 *	@return		the preselected movie's width (-1 if no movie was selected)
			 */
			int getWidth(void) const;

			/**	Returns the preselected movie's height (-1 if no movie was selected)
			 *
			 *	@return		the preselected movie's height (-1 if no movie was selected)
			 */
			int getHeight(void) const;

			/**	Returns the bounding rectangle of the current movie
			 *  @return bounding rectangle of the current movie
			 */
			const ImageRect* getRect(void) const;

			/**	Returns the time of the current frame (last played)
			 *	@return	time of the current frame
			 */
			long getCurrentTime(void) const;

			/**	Returns the time of the next frame
			 *	@return	time of the next frame
			 */
			long getNextTime(void) const;

			/**	Returns the index of the current frame (last played)
			 *	@return	index of the last frame read
			 */
			long getFrameIndex(void) const;


        private:

            #if URIVL_USE_QUICKTIME
                /** Platform-specific implementation of the player: QuickTime version
                 */
                _MoviePlayer_QT   *player_;
            #else
                /** Platform-specific implementation of the player: "generic" version
                 */
                _MoviePlayer_libquicktime  *player_;
            #endif

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            MoviePlayer(const MoviePlayer& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const MoviePlayer& operator =(const MoviePlayer& obj);


    };
    
}

#endif	//	MOVIE_PLAYER_H

#endif	//	URIVL_CAN_PLAY_MOVIES
