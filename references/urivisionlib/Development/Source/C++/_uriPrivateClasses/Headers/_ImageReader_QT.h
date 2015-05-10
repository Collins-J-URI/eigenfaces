/*  NAME:
        _ImageReader_QT.h

    DESCRIPTION:
        _ImageReader_QT public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_READER_QT_H
#define URIVL_IMAGE_READER_QT_H

#if URIVL_USE_QUICKTIME

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageReader.h"
#include "RasterImage.h"


namespace uriVL
{
    //----------------------------------------------------------------------------------
    //--------------------------------Class definition----------------------------------
    //

	/**	_ImageReader_QT objects read a raster image from an image file specified either
	 *	by its file path or FSSpec descriptor, or selected via a dialog window.
	 *
	 *  This subclass of the ImageReader class uses the QuickTime library to read images.
	 *  Or, rather, it will do so shortly.
	 *  Currently handles only the following image formats:
     *      - PPM [both ASCII and binary]
     *      - PGM [both ASCII and binary]
     *      - tga (Targa) [Both 24 and 8 bit formats, but has problems with
     *              comments in images]
     *
     *   My plan is still to scrap most of this code to use the QuickTime importers
     *  when possible.
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
    class URIVL_EXPORT _ImageReader_QT
    {
        private:

			/**	File Spec record (Mac OS - QuickTime data structure) for the video
			 *	source.  Only applies to image files, folders, movie files, but not
			 *	to "live" sources (video digitizer, sequence grabber, etc.)
			 */
            FSSpec  fsspec_;

			/**	File path for the video source.  Only applies to image files, folders,
			 *	movie files, but not
			 *	to "live" sources (video digitizer, sequence grabber, etc.)
			 */
            char*	filePath_;

            /** True once an FSSpec has been defined for the source
             */
            bool    fsspecDefined_;


			/**	Default constructor
			 */
            _ImageReader_QT(void);

            /**	Constructor that initializes the source for image input using a
             *	file's path
             *
             *	@param	filePath	path of the input image file
             */
            _ImageReader_QT(const char*	filePath);

            /**	Copy constructor (not implemented)
             *
             *	@param	reader	object to copy
             */
            _ImageReader_QT(const _ImageReader_QT& reader);

            /**	Copy operator (not implemented)
             *
             *	@param	reader	object to copy
             */
            const _ImageReader_QT& operator = (const _ImageReader_QT& reader);


			/**	Destructor
			 */
            ~_ImageReader_QT(void);

            /**	Select the input image file via a dialog window
             */
            void setSource_(void);

            /**	Select the input image file using a file path
             *
             *	@param	fileName	path of the input image file
             */
            void setSource_(const char*	fileName);

            /**	Select the input image file using a Mac OS/QuickTime FSSpec file descriptor
             *
             *	@param	fileSpec	Mac OS/QuickTime FSSpec descriptor of input image file
             */
            void setSource_(const FSSpec*	fileSpec);


			/**	Read an image frame from a source file selected through a dialog window
			 *
			 *	@param	upsideDown	is true if the image should be read upside down
			 *	@return		RasterImage object containing the image read
			 */
            static RasterImage*	readImage_(bool upsideDown);

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


//            int choiceIndex_(char* ext, const char* choices[], int nbChoices);
            void swapRGB_(unsigned char* theData, int nbRows, int nbCols);
            //
            static void getHeaderInfoPPM_(FILE* inPPM, const char* filePath, BaseRasterType* theType,
                                   bool* isAscii, int* nbRows, int* nbCols, int* maxVal);
            static RasterImage* readPPM_(const char* fileName, bool upsideDown);
//            static RasterImage* readPGM(const char* fileName, bool upsideDown);

		
		
		//
		//            RasterImage* readTGA_(const char* fileName, bool upsideDown);
		//            //
		//            void getHeaderInfoBMP_(FILE* inBMP, char* filePath, BaseRasterType* theType,
		//                                   CompressionMode* compMode, int* nbRows, int* nbCols,
		//                                   int* maxVal);
		//            RasterImage* readBMP_(const char* fileName);
		//            RasterImage* readBMP_(const char* fileName, bool upsideDown);
		


        friend class ImageReader;
        
    };

    RasterImage* simpleGetImageFromImporter_(GraphicsImportComponent importer);
			
}

#endif //	URIVL_USE_QUICKTIME

#endif  //  IMAGE_READER_QT_H
