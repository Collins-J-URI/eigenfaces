/*  NAME:
        _ImageWriter_QT.h

    DESCRIPTION:
        _ImageWriter_QT public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_WRITER_QT_H
#define URIVL_IMAGE_WRITER_QT_H

#if URIVL_USE_QUICKTIME // this file should not be included if we're not using QT

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"
#include "ImageWriter.h"


namespace uriVL
{
    /** A simple/lame class that writes image objects to files.
     * This one will (soon) implement the reader using the QuickTime importers.
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
     */
    class URIVL_EXPORT _ImageWriter_QT
    {

        private:

			/**	Data reference for the file currentlly assigned as destination to this writer.  Somewhat redundant with
			 *	filePath_
			 */
			Handle dataRef_;
			
			/**	Path to the file currentlly assigned as destination to this writer.  Somewhat redundant with
			 *	dataRef_
			 */
            char    *filePath_;
			
			/**	True when a data reference has been assigned as destination to this writer.
			 */
            bool    dataRefDefined_;


			/**	Default constructor
			 */
            _ImageWriter_QT(void);
			
			/**	Copy constructor
			 *	@param	obj		the object to copy
			 */
            _ImageWriter_QT(const _ImageWriter_QT& obj);
			
			/**	Creates a new ImageWriter with a destination already assigned
			 *	@param	filePath	path to the file assigned as destination to this writer
			 */
            _ImageWriter_QT(const char* filePath);

			/**	Creates a new ImageWriter with a destination already assigned
			 *	@param	theRef	data reference to the file assigned as destination to this writer
			 */
            _ImageWriter_QT(Handle theRef);
			
			/**	Destructor
			 */
            ~_ImageWriter_QT(void);

			/**	Copy operator
			 *	@param	obj		the object to copy
			 */
            const _ImageWriter_QT& operator =(const _ImageWriter_QT& obj);
			
			/**	Write the image to the currently assigned destination
			 *	@param the image to write
			 *	@param	writeUpsideDown		true if the image should be written upside down
			 *									(temp. setting for some tga files)
			 */
            void outputFrame_(const RasterImage* theImage, bool writeUpsideDown);


			/**	Write the image to the assigned destination
			 *	@param the image to write
			 *	@param	filePath	path to the new destination file
			 *	@param	writeUpsideDown		true if the image should be written upside down
			 *									(temp. setting for some tga files)
			 */
            static void writeImage_(const RasterImage* theImage, const char* filePath,
									bool writeUpsideDown = false);



			/**	Use the GUI to assign a new destination to the writer
			 */
            void setDestination_(void);

			/**	Assign as a new destination to the writer the file designated by the path.
			 *	@param	filePath	file to the new destination file
			 */
            void setDestination_(const char* filePath);

			/**	Assign as a new destination to the writer the file designated by the data reference.
			 *	@param	theRef	data reference to the new destination
			 */
            void setDestination_(Handle theRef);


        friend class ImageWriter;
    };
}

#else // if not using URIVL_USE_QUICKTIME
	#error URIVL_USE_QUICKTIME not defined but this file expected it to be
#endif // use URIVL_USE_QUICKTIME

#endif  //  URIVL_IMAGE_WRITER_QT_H
