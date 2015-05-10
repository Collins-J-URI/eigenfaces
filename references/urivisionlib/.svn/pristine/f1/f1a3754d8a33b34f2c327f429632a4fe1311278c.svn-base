/*  NAME:
        _ImageWriter_hack.cpp

    DESCRIPTION:
        implementation of the uriVisionLib _ImageWriter_hack class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

//	Mostly there to facilitate life for Visual Studio developers, since in Xcode and
//	Cmake souce files are excluded directly from the project depending on the
//	state defined by the environment variables.

// old world order just has USE_QT and USE_GEN (really URIVL_USE_HACK_RW)
// with addition of FreeImage, we need to protect files to only get included when they should
// but there never was a URIVL_USE_HACK_WRITER define so we overload URIVL_USE_HACK_RW for now
#if URIVL_USE_HACK_RW // overload URIVL_USE_HACK_RW to mean URIVL_USE_HACK_WRITER as well


#include <cstring>
#include <cstdio>
//
#include "ErrorReport.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "_ImageWriter_hack.h"

using namespace uriVL;

const char* kWriteExtensions_[3] = {
										"tga",
										"ppm",
										"pgm"
									};
int getWriteExtensionIndex_(const char* ext);



#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Constructors and destructor
//--------------------------------------------------------------------------
#endif

_ImageWriter_hack::_ImageWriter_hack(void)
		:	dataRef_(NULL),
			filePath_(NULL),
			dataRefDefined_(false)			
{
}

_ImageWriter_hack::_ImageWriter_hack(const char* theFilePath) 
		:	dataRef_(NULL),
			filePath_(NULL),
			dataRefDefined_(false)			
{
    setDestination_(theFilePath);
}

_ImageWriter_hack::_ImageWriter_hack(Handle dataRef) 
		:	dataRef_(NULL),
			filePath_(NULL),
			dataRefDefined_(false)			
{
    setDestination_(dataRef);
}


_ImageWriter_hack::~_ImageWriter_hack(void)
{
    if (filePath_ != NULL)
        delete []filePath_;
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Set destination
//--------------------------------------------------------------------------
#endif



void _ImageWriter_hack::setDestination_(void)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "file selection dialog not implemented yet");
}

void _ImageWriter_hack::setDestination_(const char* filePath)
{
    dataRefDefined_ = false;
	if (filePath_ != NULL)
		delete []filePath_;
    filePath_ = NULL;

    FAIL_CONDITION( filePath == NULL,
                    kFileNullPathError,
                    "null file path send as parameter to _ImageWriter_hack::setDestination_");

	try 
	{
		const unsigned long l = strlen(filePath) + 1;
		filePath_ = new char[l];
		strcpy(filePath_, filePath);
	}
	catch (...)
	{
		if (filePath_ != NULL)
			delete []filePath_;
		filePath_ = NULL;
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid file path string sent to _ImageWriter_hack::setDestination_");
	}
}

void _ImageWriter_hack::setDestination_(Handle dataRef)
{
	if (filePath_ != NULL)
		delete []filePath_;
    filePath_ = NULL;
    //
	dataRef_ = dataRef;
    dataRefDefined_ = true;
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Output
//--------------------------------------------------------------------------
#endif

void _ImageWriter_hack::outputFrame_(const RasterImage* theImage, bool writeUpsideDown)
{
	FAIL_CONDITION(	filePath_ == NULL,
					kFileNullPathError,
					"_ImageWriter_hack::outputFrame_ called before a destination was set");

	try
	{
		writeImage_(theImage, filePath_, writeUpsideDown);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by _ImageWriter_hack::outputFrame_");
		throw e;
	}
}


void _ImageWriter_hack::writeImage_(const RasterImage* theImage, const char* filePath,
								bool writeUpsideDown)
{
	// make sure we have something for a path
	FAIL_CONDITION( (filePath == NULL) || (strlen(filePath) == 0),
    	            kFileNullPathError,
    	            "Null file path error in _ImageWriter_hack::writeImage_");

	//  no point trying to write a null image
	FAIL_CONDITION( theImage == NULL,
                    kNullRasterImageError,
                    "Attempted to output a null RasterImage in _ImageWriter_hack::writeImage_");


	//	As a Mac programmer, I am really embarassed to be using something as
	//	crude as the "file extension" to find out what image format I am dealing
	//	with, but I lack the time to do something better.  Still, it's an ugly hack.
	unsigned long pathLength = strlen(filePath);
	const char* extension = filePath + (pathLength - 3);
	switch (getWriteExtensionIndex_(extension))
	{
		case kTGA:
			writeTGA_(theImage, filePath, writeUpsideDown);
			break;

        //  Feel free to disagree:  I think it is silly to write a gray-level image in a 24 bit
        //  rgb ppm file.  I don't want to override the name the user has chosen for this file,
        //  but since PPM and PGM are read by the same function (and most application happily
        //  clump these formats together anyway), I simply write a ppgm file with a .ppm extension
        case kPPM:
            if (theImage->getBaseRasterType() == kGrayRaster)
                writePPM_(theImage, filePath, writeUpsideDown);
            else
                writePPM_(theImage, filePath, writeUpsideDown);
            break;

        case kPGM:
            writePGM_(theImage, filePath, writeUpsideDown);
            break;

		default:
		    char    myError[256];
		    sprintf(myError, "Unknown file extension: %s.\n", filePath);
			FAIL_CONDITION( true,
			                kUnknownFileTypeError,
			                myError);
			break;
			
	}
}


//---------------------------------------------------------------------*
//	Function : writeTGA_
//	Description :   This function writes out an image of type TGA (24-bit color)
//
//----------------------------------------------------------------------*/
void _ImageWriter_hack::writeTGA_(const RasterImage* theImage, const char* filePath, bool upsideDown)
{
    FILE	*tga_out;
    char	head[18];

    const int nbRows = theImage->getHeight(),
            nbCols = theImage->getWidth();
    const unsigned long ulNbCols = static_cast<unsigned long>(nbCols);
    
    // Try to open the TGA output file
    tga_out = fopen(filePath, "wb");
    FAIL_CONDITION( tga_out == NULL,
                    kFileOpenReadError,
                    filePath);

    // --- create the header
    head[0] = 0;  		  					// ID field length.
    head[1] = 0;  		  					// Color map type.
    //  head[2] =????;  ------>  will be determined by the image's base raster type
    head[3] = head[4] = 0;    				// First color map entry.
    head[5] = head[6] = 0;    				// Color map lenght.
    head[7] = 0;  		  					// Color map entry size.
    head[8] = head[9] = 0;    				// Image X origin.
    head[10] = head[11] = 0;   		    // Image Y origin.
    head[13] = (char) (nbCols >> 8);  		// Image width.
    head[12] = (char) (nbCols & 0x0FF);
    head[15] = (char) (nbRows >> 8);  		// Image height.
    head[14] = (char) (nbRows & 0x0FF);
    //  head[16] = ????;  ------>  will be determined by the image's base raster type
	//   if (upsideDown)
	//       head[17] = 0x20;
	//  else
        head[17] = 0;  		  				// Image descriptor bits;

    //  Different output code for each of the base raster types: gray, rgb24, rgba32, argba16.
    switch (theImage->getBaseRasterType( ))
    {
        //  Case of a gray-level base type
        case kGrayRaster:
			{
            head[2] = 3;  		  					// Image type: gray level.
            head[16] = 8;  		 					// Bits per pixel.
            fwrite(head, sizeof(char), 18, tga_out);

            //	get the image's 2D gray raster
            const unsigned char* const* theRaster = theImage->getRaster2D();
            if (upsideDown)
                for (int i=nbRows-1; i>=0; i--)
                    fwrite(theRaster[i], sizeof(char), ulNbCols, tga_out);
            else
                for (int i=0; i<nbRows; i++)
                    fwrite(theRaster[i], sizeof(char), ulNbCols, tga_out);

            fclose(tga_out);
			}
            break;

        case kaRGB16Raster:
            fclose(tga_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "Output format not handled yet");
           break;

        case kRGB24Raster: {
            head[2] = 2;  		  					// Image type: true color, uncompressed.
            head[16] = 24;  		 				// Bits per pixel.
            fwrite(head, sizeof(char), 18, tga_out);

            //	get the image's 2D rgb raster
            const unsigned char* const* theRaster = theImage->getRaster2D();
            const int bytesPerRow = 3*nbCols;

            if (upsideDown)
                for (int i=nbRows-1; i>=0; i--)
                    //  don't forget that tga is bgr
                    for (int j=0; j<bytesPerRow; j+=3)
                    {
                        fwrite(theRaster[i]+j+2, sizeof(char), 1, tga_out);
                        fwrite(theRaster[i]+j+1, sizeof(char), 1, tga_out);
                        fwrite(theRaster[i]+j, sizeof(char), 1, tga_out);
                    }
            else
                for (int i=0; i<nbRows; i++)
                    //  don't forget that tga is bgr
                    for (int j=0; j<bytesPerRow; j+=3)
                    {
                        fwrite(theRaster[i]+j+2, sizeof(char), 1, tga_out);
                        fwrite(theRaster[i]+j+1, sizeof(char), 1, tga_out);
                        fwrite(theRaster[i]+j, sizeof(char), 1, tga_out);
                    }

            fclose(tga_out);
            }
            break;

        case kaRGB32Raster:
            fclose(tga_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "Output format not handled yet");
            break;

        case kRGBa32Raster: {

            head[2] = 2;  		  					// Image type: true color, uncompressed.
            head[16] = 24;  		 				// Bits per pixel.
            fwrite(head, sizeof(char), 18, tga_out);

            //	get the image's 2D rgba raster
            const unsigned char* const* theRaster = theImage->getRaster2D();
            const int bytesPerRow = 4*nbCols;
            if (upsideDown)
                for (int i=nbRows-1; i>=0; i--)
                    //  don't forget that tga is bgr
                    for (int j=bytesPerRow-4; j>=0; j-=4)
                    {
                        fwrite(theRaster[i]+j+2, sizeof(char), 1, tga_out);
                        fwrite(theRaster[i]+j+1, sizeof(char), 1, tga_out);
                        fwrite(theRaster[i]+j, sizeof(char), 1, tga_out);
                    }
            else
                for (int i=0; i<nbRows; i++)
                    //  don't forget that tga is bgr
                    for (int j=0; j<bytesPerRow; j+=4)
                    {
                        fwrite(theRaster[i]+j+2, sizeof(char), 1, tga_out);
                        fwrite(theRaster[i]+j+1, sizeof(char), 1, tga_out);
                        fwrite(theRaster[i]+j, sizeof(char), 1, tga_out);
                    }

            fclose(tga_out);
            }
            break;

        default:
            fclose(tga_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "Output format not handled yet");
            break;

    }
}




//----------------------------------------------------------------------
//	Function:   writePPM_
//
//	Description:	writes a 24-bit color image in PPM format (binary)
//
//	Input:	the image to read
//			the output file's path.
//----------------------------------------------------------------------

void _ImageWriter_hack::writePPM_(const RasterImage* theImage, const char* filePath, bool upsideDown)
{
    FILE    *ppm_out;
    const int   nbRows = theImage->getHeight(),
                nbCols = theImage->getWidth();


    // Try to open the TGA output file
    ppm_out = fopen(filePath, "wb");
    FAIL_CONDITION( ppm_out == NULL,
                    kFileOpenReadError,
                    filePath);

    fprintf(ppm_out, "P6\n");
    fprintf(ppm_out, "%d %d\n", nbCols, nbRows);
    fprintf(ppm_out, "255\n");


    //  Different output code for each of the base raster types: gray, rgb24, rgba32, argba16.
    switch (theImage->getBaseRasterType( ))
    {
        case kaRGB16Raster:
            fclose(ppm_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "argb16 output format not handled yet");
           break;

        case kRGB24Raster: {

            //	get the image's 2D rgb raster
            const unsigned char* const* theRaster = theImage->getRaster2D();
            const unsigned long bytesPerRow = static_cast<unsigned long>(3*nbCols);
            if (upsideDown)
                for (int i=nbRows-1; i>=0; i++)
                    fwrite(theRaster[i], sizeof(char), bytesPerRow, ppm_out);
             else
                for (int i=0; i<nbRows; i++)
                    fwrite(theRaster[i], sizeof(char), bytesPerRow, ppm_out);

            fclose(ppm_out);
            }
            break;

        case kaRGB32Raster:
            fclose(ppm_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "argb32 output format not handled yet");
            break;

        case kRGBa32Raster: {
            int bytesOnCol = 4*nbCols;

            //	get the image's 2D rgba raster
            const unsigned char* const* theRaster = theImage->getRaster2D();
            if (upsideDown)
                for (int i=nbRows-1; i>=0; i++)
                    for (int j=0; j<bytesOnCol; j+=4)
                        fwrite(theRaster[i]+j, sizeof(char), 3, ppm_out);
            else
                for (int i=0; i<nbRows; i++)
                    for (int j=0; j<bytesOnCol; j+=4)
                        fwrite(theRaster[i]+j, sizeof(char), 3, ppm_out);

            fclose(ppm_out);
            }
            break;

        default:
            fclose(ppm_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "Output format not handled yet");
            break;

    }
}


//----------------------------------------------------------------------
//	Function:   writePGM_
//
//	Description:	writes an 8-bit gray-level image in PGM format (binary)
//
//	Input:	the image to read
//			the output file's path.
//----------------------------------------------------------------------

void _ImageWriter_hack::writePGM_(const RasterImage* theImage, const char* filePath, bool upsideDown)
{
    FILE    *pgm_out;
    const int   nbRows = theImage->getHeight(),
                nbCols = theImage->getWidth();


    // Try to open the TGA output file
    pgm_out = fopen(filePath, "wb");
    FAIL_CONDITION( pgm_out == NULL,
                    kFileOpenReadError,
                    filePath);

    fprintf(pgm_out, "P5\n");
    fprintf(pgm_out, "%d %d\n", nbCols, nbRows);
    fprintf(pgm_out, "255\n");


    //  Different output code for each of the base raster types: gray, rgb24, rgba32, argba16.
    switch (theImage->getBaseRasterType( ))
    {
        //  Case of a gray-level base type
        case kGrayRaster:
{
            //	get the image's 2D gray raster
            const unsigned char* const* theRaster = theImage->getRaster2D();
            const unsigned long bytesPerRow = static_cast<unsigned long>(nbCols);
            if (upsideDown)
                for (int i=nbRows-1; i>=0; i++)
                    fwrite(theRaster[i], sizeof(char), bytesPerRow, pgm_out);
            else
                for (int i=0; i<nbRows; i++)
                    fwrite(theRaster[i], sizeof(char), bytesPerRow, pgm_out);

            fclose(pgm_out);
}
            break;

        case kaRGB16Raster:
            fclose(pgm_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "argb16 output to pgm not handled yet");
           break;

        case kRGB24Raster:
            fclose(pgm_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "argb16 output to pgm not handled yet");

            break;

        case kaRGB32Raster:
            fclose(pgm_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "argb32 output to pgm not handled yet");
            break;

        case kRGBa32Raster:
            fclose(pgm_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "rgba32 output to pgm not handled yet");

            break;

        default:
            fclose(pgm_out);
            FAIL_CONDITION( true,
                            kNoCode,
                            "Output format not handled yet");
           break;

    }
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Helper functions
//--------------------------------------------------------------------------
#endif

int getWriteExtensionIndex_(const char* ext)
{
    for (int i = 0; i < kNbImageFileExtensions; i++)
        if (strncmp(ext, kWriteExtensions_[i], strlen(kWriteExtensions_[i])) == 0)
            return (i);

    return (-1);
}



#endif // URIVL_USE_HACK_RW

