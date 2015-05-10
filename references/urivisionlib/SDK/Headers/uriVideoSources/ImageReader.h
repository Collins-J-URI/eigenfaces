/*  NAME:
        ImageReader.h

    DESCRIPTION:
        ImageReader public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_READER_H
#define URIVL_IMAGE_READER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "VideoSource.h"
#include "RasterImage.h"


namespace uriVL
{
    //----------------------------------------------------------------------------------
    //--------------------------Enum and Struct type definitions------------------------
    //

    typedef enum CompressionMode
    {
        kUncompressed = 0,
        kRLE4Bits,
        kRLE8Bits
    } CompressionMode;

    typedef enum ImageFileType
    {
        kUnknownImgFileType = -1,
        kTGA,
        kPPM,                   //  24-bit color image
        kPGM,                   //  8-bit gray-level image
        kBMP,
        //
        kNbImageFileExtensions

    } ImageFileType;

    //----------------------------------------------------------------------------------
    //--------------------------------Class definition----------------------------------
    //

    #if URIVL_USE_QUICKTIME
        class _ImageReader_QT;
	 #elif URIVL_USE_FREEIMAGE
	    class _ImageReader_FreeImage;
	 #elif URIVL_USE_HACK_RW
	    class _ImageReader_hack;
    #else
	    #error URIVL_USE_ not defined but this file expected it to be

    #endif
    
    
	/**	ImageReader objects read a raster image from an image file specified either
	 *	by its file path or FSSpec descriptor, or selected via a dialog window.
	 *
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
    class URIVL_EXPORT ImageReader : public VideoSource
    {
        public:

			/**	Default constructor
			 */
            ImageReader(void);

            /**	Constructor that initializes the source for image input using a
             *	file's path
             *
             *	@param	filePath	path of the input image file
             */
            ImageReader(const char* filePath);


			/**	Destructor
			 */
            ~ImageReader(void);
            

            /**	Select the input image file via a dialog window
             */
            void setSource(void);


            /**	Select the input image file using a file path
             *
             *	@param	fileName	path of the input image file
             */
            void setSource(const char* fileName);


			/**	Get an image frame from the preselected source
			 *
			 *	@param	upsideDown	is true if the image should be read upside down
			 *	@return		RasterImage object containing the image read
			 */
            RasterImage* getFrame(void);

			/**	Get an image frame from the preselected source
			 *
			 *	@param	upsideDown	is true if the image should be read upside down
			 *	@return		RasterImage object containing the image read
			 */
            RasterImage* getFrame(bool upsideDown=false);

			
			/**	Get an image frame from the preselected source
			 *
			 *	@param	img			RasterImage object into which the image should be read
			 */
            void getFrame(RasterImage* img);

			/**	Get an image frame from the preselected source
			 *
			 *	@param	img			RasterImage object into which the image should be read
			 *	@param	upsideDown	is true if the image should be read upside down
			 */
            void getFrame(RasterImage* img, bool upsideDown=false);


			/**	Read an image frame from a source file selected through a dialog window
			 *
			 *	@param	upsideDown	is true if the image should be read upside down
			 *	@return		RasterImage object containing the image read
			 */
            static RasterImage* readImage(bool upsideDown=false);

			/**	Read an image frame from the designated source file
			 *
             *	@param	filePath	path of the input image file
			 *	@param	upsideDown	is true if the image should be read upside down
			 *	@return		RasterImage object containing the image read
			 */
            static RasterImage* readImage(const char* filePath, bool upsideDown=false);




        private:


            #if URIVL_USE_QUICKTIME
                /** Platform-specific implementation of the reader: QuickTime version
                 */
                _ImageReader_QT*			reader_;
				
			#elif URIVL_USE_FREEIMAGE
                /** Platform-specific implementation of the reader: Freeimage version
                 */
				_ImageReader_FreeImage*	reader_;
					
            #elif URIVL_USE_HACK_RW
                /** Platform-specific implementation of the reader: "generic" version
                 */
                _ImageReader_hack*		reader_;
			
			#else
				#error URIVL_USE_some_form_of-reading-able_lib not defined but this file expected it to be
            #endif
			
			/**	Copy constructor.
			 *	not implemented
			 *	@param	img		image to copy
			 */
			ImageReader(const ImageReader& reader);
			
			/**	Copy operator.
			 *	not implemented
			 *	@param	img		image to copy
			 */
			const ImageReader& operator = (const ImageReader& reader);
			
			

    };
}

#endif  //  IMAGE_READER_H
