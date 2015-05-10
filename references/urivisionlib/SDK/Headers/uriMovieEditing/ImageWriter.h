/*  NAME:
        ImageWriter.h

    DESCRIPTION:
        ImageWriter public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_WRITER_H
#define URIVL_IMAGE_WRITER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"
#include "VideoDestination.h"
//for the ImageFileType hack
#include "ImageReader.h"


namespace uriVL
{

	#if URIVL_OS_UNIX
		typedef char** Handle;
	#endif

    #if URIVL_USE_QUICKTIME
        class _ImageWriter_QT;
	 #elif URIVL_USE_FREEIMAGE
		class _ImageWriter_FreeImage;
	 #elif URIVL_USE_HACK_RW // overload HACK_READER to include HACK_WRITER
        class _ImageWriter_hack;
    #else
		  #error no writer defined
    #endif
    

    /** A simple/lame class that writes image objects to files.
     *  Currently handles only the following image formats
     *      - PPM [both ASCII and binary]
     *      - PGM [both ASCII and binary]
     *      - tga (Targa) [Both 24 and 8 bit formats]
     *
     *  I haven't bothered adding support for GIF or JPEG because I don't want
     *  to mess with compression.  If I had to handle these image formats, I
     *  would probably use QuickTime's export libraries.
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
     *      - 05/07/2004  derived from VideoDestination
     *      - 05/29/2013  revised to match VideoSource update
     */
    class URIVL_EXPORT ImageWriter : public VideoDestination
    {

        public:

			/**	Default constructor
			 */
            ImageWriter(void);

			/**	Creates a new ImageWriter with a destination already assigned
			 *	@param	filePath	path to the file assigned as destination to this writer
			 */
            ImageWriter(const char* filePath);

			#if (URIVL_USE_QUICKTIME || URIVL_USE_HACK_RW)
				/**	Creates a new ImageWriter with a destination already assigned
				 *	@param	theRef	data reference to the file assigned as destination to this writer
				 */
				ImageWriter(Handle theRef);
			#endif			

			/**	Destructor
			 */
            ~ImageWriter(void);
 
			/**	Use the GUI to assign a new destination to the writer
			 */
			void setDestination(void);

			/**	Assign as a new destination to the writer the file designated by the path.
			 *	@param	filePath	path to the new destination file
			 */
            void setDestination(const char* filePath);

			#if (URIVL_USE_QUICKTIME || URIVL_USE_HACK_RW)
				/**	Assign as a new destination to the writer the file designated by the data reference.
				 *	@param	theRef	data reference to the new destination
				 */
				void setDestination(Handle dataRef);
			#endif

			/**	Write the image to the currently assigned destination
			 *	@param the image to write
			 *	@param	writeUpsideDown		true if the image should be written upside down
			 *									(temp. setting for some tga files)
			 */
            void outputFrame(const RasterImage* theImage, bool writeUpsideDown = false);

			/**	Write the image to the currently assigned destination
			 *	@param the image to write
			 *	@param	writeUpsideDown		true if the image should be written upside down
			 *									(temp. setting for some tga files)
			 */
            void writeImage(const RasterImage* theImage, bool writeUpsideDown = false);

			/**	Write the image to the currently assigned destination
			 *	@param the image to write
			 *	@param	filePath	path to the new destination file
			 *	@param	writeUpsideDown		true if the image should be written upside down
			 *									(temp. setting for some tga files)
			 */
            static void writeImage(const RasterImage* theImage, const char* filePath,
									bool writeUpsideDown = false);


			/**	Identifies the type of an image file from its file extension (as long as it is
			 *	ppm, pgm, pbm, or tga)
			 *	@param ext		the file's extension
			 *	@return	the type of the image
			 */
            static int extensionIndex(const char* ext);

   
		private:

			/**	List of file extensions supported by this class.  Not sure why I should still need that
			 *	stuff when the QT file importer is supposed to handle all that
			 */
            static const char* kWriteExtensions_[ 4 ];

			
            #if URIVL_USE_QUICKTIME
                /** Platform-specific implementation of the writer: QuickTime version
                 */
                _ImageWriter_QT* writer_;

			#elif URIVL_USE_FREEIMAGE
                /** Platform-specific implementation of the writer: Freeimage version
                 */
				_ImageWriter_FreeImage *writer_;

			#elif URIVL_USE_HACK_RW // overload HACK_READER to include HACK_WRITER
                /** Platform-specific implementation of the writer: "generic version
                 */
				_ImageWriter_hack  *writer_;

            #else
                #error no writer defined 
            #endif

			/**	Copy constructor
			 *	@param	obj		the object to copy
			 */
            ImageWriter(const ImageWriter& obj);
			
 			/**	Copy operator
			 *	@param	obj		the object to copy
			 */
            const ImageWriter& operator =(const ImageWriter& obj);
			


    };
}

#endif  //  IMAGE_WRITER_H
