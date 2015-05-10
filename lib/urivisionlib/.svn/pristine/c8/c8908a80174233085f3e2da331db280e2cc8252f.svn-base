/*  NAME:
        _ImageWriter_FreeImage.h

    DESCRIPTION:
        _ImageWriter_FreeImage public header.

    COPYRIGHT:
        (c) 2012-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_WRITER_FREEIMAGE_H
#define URIVL_IMAGE_WRITER_FREEIMAGE_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

#include "FreeImage.h"

#include "ImageWriter.h"

#include "RasterImage.h"

namespace uriVL
{
    /** This class uses the Freeimage library to read/write images
     *
     *  Versions:
     *      - 2012-02  created the class - Lewis Collier
     */

    class URIVL_EXPORT _ImageWriter_FreeImage
    {

        private:

				// use this ONLY when linking with FreeImage as a static library
				// set this as compiler option via IDE, not locally in the code
#ifdef FREEIMAGE_STATIC_LIB
				static mboHaveNotRegisteredFreeImage = true;
#endif

// leave code but #if0 out in case we change the concept of Handle in the future
#if URIVL_USE_QUICKTIME // no QT so no Handles			
			/**	Data reference for the file currentlly assigned as destination to this writer.  Somewhat redundant with
			 *	filePath_
			 */
			Handle dataRef_;
#else // just use a standard C FILE pointer
				FILE* dataRef_;
#endif
			
			/**	Path to the file currentlly assigned as destination to this writer.  Somewhat redundant with
			 *	dataRef_
			 */
            char    *filePath_;
			
			/**	True when a data reference has been assigned as destination to this writer.
			 */
            bool    dataRefDefined_;

 			/**	Default constructor
			 */
            _ImageWriter_FreeImage(void);

			/**	Copy constructor
			 *	@param	obj		the object to copy
			 */
            _ImageWriter_FreeImage(const _ImageWriter_FreeImage& obj);
			
			/**	Creates a new ImageWriter with a destination already assigned
			 *	@param	filePath	path to the file assigned as destination to this writer
			 */
            _ImageWriter_FreeImage(const char* filePath);

// leave code but #if0 out in case we change the concept of Handle in the future
#if URIVL_USE_QUICKTIME // no QT so no Handles
				/**	Creates a new ImageWriter with a destination already assigned
			 *	@param	theRef	data reference to the file assigned as destination to this writer
			 */
            _ImageWriter_FreeImage(Handle theRef);
#else
			_ImageWriter_FreeImage(FILE* theRef);
#endif

			/**	Destructor
			 */
            ~_ImageWriter_FreeImage(void);

			/**	Copy operator
			 *	@param	obj		the object to copy
			 */
            const _ImageWriter_FreeImage& operator =(const _ImageWriter_FreeImage& obj);
			
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

// leave code but #if0 out in case we change the concept of Handle in the future
#if URIVL_USE_QUICKTIME // no QT so no Handles
			/**	Assign as a new destination to the writer the file designated by the data reference.
			 *	@param	theRef	data reference to the new destination
			 */
            void setDestination_(Handle theRef);
#else // just use a standard C FILE pointer
			void setDestination_(FILE* theRef);
#endif

#if 0
			/**	Writes a RGB or RGBA image file
			 *	@param theImage the image to write
			 *	@param writeUpsideDown true if the image should be written upside down
			 * @param nRasterType the raster format (e.g. RGB, aRGB, or RGBa)
			 *									
			 */
          //void writeRGBbytes_(RasterImage* theImage, bool upsideDown, int nRasterType);

			/**	Writes a gray image file
			 *	@param theImage the image to write
			 *	@param writeUpsideDown true if the image should be written upside down
			 * @param nRasterType the raster format (e.g. RGB, aRGB, or RGBa)
			 *									
			 */
          //void writeGRAYbytes_(RasterImage* theImage, bool upsideDown, int nRasterType);

			/**	Writes a PGM image file
			 *	@param theImage	the image to write
			 *	@paramfilePath		path to the destination  file
			 *	@param	writeUpsideDown		true if the image should be written upside down
			 *									(temp. setting for some tga files)
			 */
            //void writePGM_(RasterImage* theImage, char* filePath, bool upsideDown);

			/**	Writes a TGA image file
			 *	@param theImage	the image to write
			 *	@paramfilePath		path to the destination  file
			 *	@param	writeUpsideDown		true if the image should be written upside down
			 *									(temp. setting for some tga files)
			 */
           //void writeTGA_(RasterImage* theImage, char* filePath, bool upsideDown);
#endif


        friend class ImageWriter;
    };
}

#endif  //  URIVL_IMAGE_WRITER_FREEIMAGE_H
