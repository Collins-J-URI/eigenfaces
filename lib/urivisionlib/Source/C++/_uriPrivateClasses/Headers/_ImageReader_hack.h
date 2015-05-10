/*  NAME:
        _ImageReader_hack.h

    DESCRIPTION:
        _ImageReader_hack public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_READER_GEN_H
#define URIVL_IMAGE_READER_GEN_H

#if URIVL_USE_HACK_RW

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageReader.h"
#include "RasterImage.h"


namespace uriVL
{
	/**	_ImageReader_hack objects read a raster image from an image file specified either
	 *	by its file path or FSSpec descriptor, or selected via a dialog window.
	 *
	 *  This subclass of the ImageReader class so far uses only generic C++ to read images.
	 *  Currently handles only the following image formats:
     *      - PPM [both ASCII and binary]
     *      - PGM [both ASCII and binary]
     *      - tga (Targa) [Both 24 and 8 bit formats, but has problems with
     *              comments in images]
     *
     *   My plan is still to scrap most of this code to use some open-source library.
     *
     *  Origins of this code:   The PPM code is mine.  The TGA code is not.
     *  I lifted it from my student Claude Duchesne's augmented reality
     *  project [GRPR, EPM, 1999-2002]. I have seen minor variations of
     *  that code in many programs that I have downloaded, but I do not
     *  know who was the original author.
     *
     *  Versions:
     *      - 01/10/2003  basic code for reading PPM/tga files
     *      - 02/10/2003  cleanup and bug fixes
     */
    class URIVL_EXPORT _ImageReader_hack
    {
        private:

			/**	File path for the video source.  Only applies to image files, folders,
			 *	movie files, but not
			 *	to "live" sources (video digitizer, sequence grabber, etc.)
			 */
            char    *filePath_;

			/**	Default constructor
			 */
            _ImageReader_hack(void);

            /**	Constructor that initializes the source for image input using a
             *	file's path
             *
             *	@param	filePath	path of the input image file
             */
            _ImageReader_hack(const char* filePath);


			/**	Copy constructor. Disabled.
			 *	@param	obj		the object to copy
			 */
			_ImageReader_hack(const _ImageReader_hack& obj);
			
			/**	Destructor
			 */
            ~_ImageReader_hack(void);


			/**	Copy operator. Disabled.
			 *	@param	obj		the object to copy
			 */
			const _ImageReader_hack& operator = (const _ImageReader_hack& obj);
			
            /**	Select the input image file via a dialog window
             */
            void setSource_(void);

            /**	Select the input image file using a file path
             *
             *	@param	fileName	path of the input image file
             */
            void setSource_(const char* fileName);

            /**	Select the input image file using a Mac OS/QuickTime FSSpec file descriptor
             *
             *	@param	fileSpec	Mac OS/QuickTime FSSpec descriptor of input image file
             */
            void setSource_(const FSSpec *fileSpec);

			/**	Read an image frame from a source file selected through a dialog window
			 *
			 *	@param	upsideDown	is true if the image should be read upside down
			 *	@return		RasterImage object containing the image read
			 */
            static RasterImage* readImage_(bool upsideDown);

			/**	Read an image frame from the designated source file
			 *
             *	@param	filePath	path of the input image file
			 *	@param	upsideDown	is true if the image should be read upside down
			 *	@return		RasterImage object containing the image read
			 */
            static RasterImage* readImage_(const char* filePath, bool upsideDown);


			/**	Get an image frame from the preselected source
			 *
			 *	@param	upsideDown	is true if the image should be read upside down
			 *	@return		RasterImage object containing the image read
			 */
            RasterImage* getFrame_(bool upsideDown=false);


			/**	Get an image frame from the preselected source
			 *
			 *	@param	upsideDown	is true if the image should be read upside down
			 *	@param	img			RasterImage object into which the image should be read
			 */
            void getFrame_(RasterImage* img, bool upsideDown=false);

            static void swapRGB_(unsigned char* theData, int nbRows, int nbCols);

            static void getHeaderInfoPPM_(FILE *inPPM, const char* filePath, BaseRasterType* theType,
                                   bool* isAscii, int* nbRows, int* nbCols, int* maxVal);

            static RasterImage* readPPM_(const char* fileName, bool upsideDown);

            static RasterImage* readTGA_(const char* fileName, bool upsideDown);



        friend class ImageReader;
        
    };
}

#else // if not using URIVL_USE_HACK_RW
	#error URIVL_USE_HACK_RW not defined but this file expected it to be
#endif // use URIVL_USE_HACK_RW

#endif  //  URIVL_IMAGE_READER_GEN_H
