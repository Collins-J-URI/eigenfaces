/*  NAME:
        BufferRing.h

    DESCRIPTION:
        BufferRing public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef	URIVL_BUFFER_RING_H
#define	URIVL_BUFFER_RING_H
#include <iostream>
#include "VideoSource.h"
#include "FrameGrabber_QT.h"
#include "RasterImage_RGBa.h"

#define DEFAULT_RING_SIZE 5


namespace uriVL
{
    /** The purpose of this class is to provide a tri-indexed Buffer Ring for use
     *		of storing and reading Raster Images grabbed from a video input source.
     *
     *	@version
     *		v1.0 [6/29/2007] - Initial implementation of the BufferRing class.
     *						   [very VERY basic idea of it at least]
     *
     *	@copyright
     *      (c) 2003-2014, 3D Group for Interactive Visualization
     *                     University of Rhode Island.
     *
     *	@author
     *		Christopher T. S. Allen
     */
    class BufferRing
    {
        public:

        	/**
        	 *	Creates the BufferRing with the number of specified indices
        	 *	
        	 *	@param ringSize  number of buffers for the BufferRing
        	 */
        	BufferRing(int ringSize = DEFAULT_RING_SIZE);

        	/**
        	 *	Destructor cleans up the mess when the BufferRing
        	 *	is no longer needed.
        	 */
        	~BufferRing(void);

        	/**
        	 *	Clears the BufferRing and resets the position to the start of the BufferRing
        	 */
        	void clear(void);

        	/**
        	 *	Reads the Raster Image at m_read and returns a pointer to it
        	 *
        	 *	@returns    a pointer to the Raster Image at m_read.
        	 */
        	RasterImage* read(void);

        	/**
        	 *	Writes a Raster Image into the Buffer Ring at m_write.
        	 *
        	 *	@param image    the image to be written into the Buffer Ring.
        	 *	
        	 *	@returns    true if write is successful
        	 */
        	bool write(RasterImage* image);

        	/**
        	 *	Locks current buffer while index is being used
        	 */
        	void lock(void);

        	/**
        	 *	Locks a specified buffer of the BufferRing
        	 *
        	 *	@param index    index of buffer to be locked.
        	 */
        	void lock(int index);

        	/**
        	 *	Unlocks specified buffer allowing it to once again be written to.
        	 *
        	 *	@param index    index of buffer to be unlocked
        	 */
        	void unlock(int index);

        	/**
        	 *	Determines whether or not a given index is free to be written to.
        	 *
        	 *	@param  index   index to determine whether or not a lock exists
        	 *
        	 *	@returns    true if the current index is locked.
        	 */
        	bool isLocked(int index);

        	/**
        	 *	Cycles the pointers into the array to facilitate frame grabbing
        	 *
        	 *	@returns    false if failed to find an unlocked buffer to set write_ to
        	 */
        	bool cycle(void);

        	//-----------------
        	//testing functions
        	//-----------------
        	int getSize(void);
        	int getWriteIndex(void);
        	int getReadIndex(void);

        	/**
        	 *	Accessor method for getting the index of the most recently locked item
        	 *
        	 *	@returns    most recently locked index
        	 */
        	int recentLock(void);

        private:

        	/**
        	 *	Gets the pointer to the Raster Image at m_read in the Buffer Ring
        	 *
        	 *	@returns
        	 *		a pointer to the Raster Image at m_read
        	 */
        	long getlong();

        	int	read_;			// start position to read from BufferRing
        	int curr_;			// current position being written into the BufferRing
        	int	write_;		// start position to write into BufferRing
        	int	ringSize_;		// size of BufferRing
        	int	recentLock_;	// recently locked index
        	bool* locked_;		// pointer into the locked array
        	RasterImage *ring_;	// pointer to the stored Raster Images read

        	RasterImage *bufferedImage_;	//array of buffered raster images

    };
}
#endif
