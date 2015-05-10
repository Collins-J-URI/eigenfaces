/*  NAME:
        _ImageReader_FreeImage.h

    DESCRIPTION:
        _ImageReader_FreeImage public header.

    COPYRIGHT:
        (c) 2012-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php


		  uses the FreeImage library
*/
#ifndef URIVL_IMAGE_READER_FREEIMAGE_H
#define URIVL_IMAGE_READER_FREEIMAGE_H

#if URIVL_USE_FREEIMAGE 

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//

#include <FreeImage.h>

#include "ImageReader.h"
#include "RasterImage.h"




namespace uriVL
{
	/**	_ImageReader_FreeImage objects read a raster image from an image file using
	 *	the FreeImage library.
     *
     *  Origins of this code:   This class started as a copy of _ImageReader_hack then added
	 *  in the FreeImage stuff.
     *
     *  Versions:
     *      - 05 Feb 2012  initial start for FreeImage code 
     */
    class URIVL_EXPORT _ImageReader_FreeImage
    {
        private:

				// use this ONLY when linking with FreeImage as a static library
				// set this as compiler option via IDE, not locally in the code
#ifdef FREEIMAGE_STATIC_LIB
				static mboHaveNotRegisteredFreeImage = true;
#endif

            char    *filePath_;

			/**	Default constructor
			 */
            _ImageReader_FreeImage(void);

            /**	Constructor that initializes the source for image input using a
             *	file's path
             *
             *	@param	filePath	path of the input image file
             */
            _ImageReader_FreeImage(const char* filePath);


			/**	Copy constructor. Disabled.
			 *	@param	obj		the object to copy
			 */
			_ImageReader_FreeImage(const _ImageReader_FreeImage& obj);
			
			/**	Destructor
			 */
            ~_ImageReader_FreeImage(void);


			/**	Copy operator. Disabled.
			 *	@param	obj		the object to copy
			 */
			const _ImageReader_FreeImage& operator = (const _ImageReader_FreeImage& obj);
			
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
            void setSource_(const FSSpec* fileSpec);

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

            static RasterImage* readFile_(const char* fileName, bool upsideDown);

			//RasterImage_gray_F* readGray_F_(const char* fileName, bool upsideDown);
			//RasterImage_RGBa_F* readRGBa_F_(const char* fileName, bool upsideDown);

        friend class ImageReader;
        
    };
}

#else // if not using URIVL_USE_FREEIMAGE
	#error URIVL_USE_FREEIMAGE not defined but this file expected it to be
#endif // use URIVL_USE_FREEIMAGE

#endif  //  URIVL_IMAGE_READER_FREEIMAGE_H
