/*  NAME:
        _ImageWriter_FreeImage.cpp

    DESCRIPTION:
        implementation of the uriVisionLib _ImageWriter_FreeImage class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php

*/

//	Mostly there to facilitate life for Visual Studio developers, since in Xcode and
//	Cmake souce files are excluded directly from the project depending on the
//	state defined by the environment variables.
#if URIVL_USE_FREEIMAGE


#include <cstring>
#include <cstdio>
//
#include "_ImageWriter_FreeImage.h"
#include "RasterImage_aRGB16.h" // --> FIT_BITMAP(8 bits per pixel, RGBA)
#include "RasterImage_binary.h" // --> FIT_BITMAP(1 bit per pixel)
#include "RasterImage_gray.h"   // --> FIT_BITMAP(8 bits per pixel, R=G=B=gray)
#include "RasterImage_gray_F.h" // --> FIT_FLOAT
#include "RasterImage_HSV.h"    // --> FIT_BITMAP(8 bits per pixel, RGBA) from RI_RGBa
//#include "RasterImage_HSV24.h"  // --> FIT_BITMAP(8 bits per pixel, RGBA) from RI_RGBa
#include "RasterImage_RGB.h"    // --> FIT_BITMAP(8 bits per pixel, RGBA)
#include "RasterImage_RGBa.h"   // --> FIT_BITMAP(8 bits per pixel, RGBA)
#include "RasterImage_RGBa_F.h" // --> FIT_RGBAF (uses graphworld alpha since there is no float alpha channel)

using namespace uriVL;

// conversion helper functions
bool CVT_URIVLRI_aRGB16_to_FIT_BITMAP32_(const RasterImage_aRGB16* pRasterImage, FIBITMAP *pFI_DIB);
bool CVT_URIVLRI_Binary_to_FIT_BITMAP01_(const RasterImage_binary* pRasterImage, FIBITMAP *pFI_DIB); // not used since it doesn't work
bool CVT_URIVLRI_Binary_to_FIT_BITMAP08_(const RasterImage_binary* pRasterImage, FIBITMAP *pFI_DIB);
bool CVT_URIVLRI_Gray_to_FIT_BITMAP08_(const RasterImage_gray* pRasterImage, FIBITMAP *pFI_DIB);
bool CVT_URIVLRI_GrayF_to_FIT_FLOAT_(const RasterImage_gray_F* pRasterImage, FIBITMAP *pFI_DIB);
bool CVT_URIVLRI_RGB_to_FIT_BITMAP24_(const RasterImage_RGB* pRasterImage, FIBITMAP *pFI_DIB);
bool CVT_URIVLRI_RGBA_to_FIT_BITMAP32_(const RasterImage_RGBa* pRasterImage, FIBITMAP *pFI_DIB);
bool CVT_URIVLRI_RGBAF_to_FIT_RGBAF_(const RasterImage_RGBa_F* pRasterImage, FIBITMAP *pFI_DIB);
//
//	FreeImage IO "helper" functions
static unsigned DLL_CALLCONV urivlFIWreadProc_(void* buffer, unsigned size, unsigned count, fi_handle handle);
static unsigned DLL_CALLCONV urivlFIWwriteProc_(void* buffer, unsigned size, unsigned count, fi_handle handle);
static int  DLL_CALLCONV urivlFIWseekProc_(fi_handle handle, long offset, int origin);
static long  DLL_CALLCONV urivlFIWtellProc_(fi_handle handle);
static void urivlFIWimageErrorHandler_(FREE_IMAGE_FORMAT fif, const char* message);



//----------------------------------------------------------------------------------
//-------------------------------Class implementation-------------------------------
//

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Constructors and destructor
//--------------------------------------------------------------------------
#endif

_ImageWriter_FreeImage::_ImageWriter_FreeImage(void)
		:	dataRef_(NULL),
			filePath_(NULL),
			dataRefDefined_(false)			
{
}

/////////////////////////////////////////

_ImageWriter_FreeImage::_ImageWriter_FreeImage(const char* theFilePath) 
		:	dataRef_(NULL),
			filePath_(NULL),
			dataRefDefined_(false)			
{
    setDestination_(theFilePath);
}

/////////////////////////////////////////

//#if URIVL_USE_QUICKTIME
//_ImageWriter_FreeImage::_ImageWriter_FreeImage(Handle dataRef)
//#else
//_ImageWriter_FreeImage::_ImageWriter_FreeImage(FILE* dataRef)
//#endif
//		:	dataRef_(NULL),
//			filePath_(NULL),
//			dataRefDefined_(false)			
//{
//    setDestination_(dataRef);
//}


_ImageWriter_FreeImage::~_ImageWriter_FreeImage(void)
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


void _ImageWriter_FreeImage::setDestination_(void)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "file selection dialog not available for URIVL_USE_FREEIMAGE yet");
}

void _ImageWriter_FreeImage::setDestination_(const char* filePath)
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
						"Invalid file path string sent to _ImageWriter_QT::setDestination_");
	}
}

//#if URIVL_USE_QUICKTIME
//void _ImageWriter_FreeImage::setDestination_(Handle dataRef)
//#else
void _ImageWriter_FreeImage::setDestination_(FILE* dataRef)
//#endif
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

void _ImageWriter_FreeImage::outputFrame_(const RasterImage* theImage, bool writeUpsideDown)
{
	FAIL_CONDITION(	filePath_ == NULL,
					kFileNullPathError,
					"_ImageWriter_FreeImage::outputFrame_ called before a destination was set");

	try
	{
		writeImage_(theImage, filePath_, writeUpsideDown);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by _ImageWriter_FreeImage::outputFrame_");
		throw e;
	}
}



void _ImageWriter_FreeImage::writeImage_(const RasterImage* theImage, const char* filePath,
										 bool writeUpsideDown)
{
	// make sure we have something for a path
	FAIL_CONDITION( (filePath == NULL) || (strlen(filePath) == 0),
    	            kFileNullPathError,
    	            "Null file path error in _ImageWriter_FreeImage::writeImage_");

	//  no point trying to write a null image
	FAIL_CONDITION( theImage == NULL,
                    kNullRasterImageError,
                    "Attempted to output a null RasterImage in _ImageWriter_FreeImage::writeImage_");

	// local variables
	int nRows, nCols;
	FIBITMAP *pDIB_output;
	bool boFIWstatus;

	bool boCVTstatus = false;

	// initialize our own FreeImage IO functions
	FreeImage_SetOutputMessage(urivlFIWimageErrorHandler_);
	FreeImageIO io;
	io.read_proc  = (FI_ReadProc)  urivlFIWreadProc_;
	io.write_proc = (FI_WriteProc) urivlFIWwriteProc_;
	io.seek_proc  = (FI_SeekProc)  urivlFIWseekProc_;
	io.tell_proc  = (FI_TellProc)  urivlFIWtellProc_;

	#ifdef FREEIMAGE_STATIC_LIB
		// call this ONLY when linking with FreeImage as a static library
		if (mboHaveNotRegisteredFreeImage)
		{
			FreeImage_Initialise();
			atexit(FreeImage_DeInitialise);
			mboHaveNotRegisteredFreeImage = false;
		}
	#endif // FREEIMAGE_STATIC_LIB

	// get size of RasterImage
	nRows = theImage->getHeight();
	nCols = theImage->getWidth();

	// first, check the output format from the file name or file extension
	FREE_IMAGE_FORMAT out_fif = FreeImage_GetFIFFromFilename(filePath);
	FAIL_CONDITION( out_fif == FIF_UNKNOWN,
					kWrongFileTypeError,
					"FreeImage cannot determine file type from path name");

	//  Different output code for each of the base raster types: gray, rgb24, rgba32, argba16.
	int nRasterType = theImage->getBaseRasterType( );
	switch (nRasterType)
	{
		// aRGB16
		case kaRGB16Raster:   //  16 bits per pixel color (1a-5r-5g-5b)
			// allocate a FI space to work in
			pDIB_output = FreeImage_AllocateT(FIT_BITMAP,   // Use 16 bpp instead of 24 to prevent color conversion, 
												       nCols,nRows,  // height and width of the image
												       32);          // bits per pixel
			FAIL_CONDITION( pDIB_output == NULL,
							kVideoBufferAllocError,
							"FreeImage cannot allocate a workspace buffer");
			boCVTstatus = CVT_URIVLRI_aRGB16_to_FIT_BITMAP32_((const RasterImage_aRGB16*) theImage, pDIB_output);
			break;

		// binary
		case kBinaryRaster:   //  should be 1 bit per pixel gray-level but that didn;t seem to work so use std gray-8bit
			// allocate a FI space to work in
			pDIB_output = FreeImage_AllocateT(FIT_BITMAP,   // Use 8 bpp instead of 1 since 1 didn;t seem to work :(, 
												       nCols,nRows,  // height and width of the image
												       8);           // bits per pixel
			FAIL_CONDITION( pDIB_output == NULL,
							kVideoBufferAllocError,
							"FreeImage cannot allocate a workspace buffer");
			//boCVTstatus = CVT_URIVLRI_Binary_to_FIT_BITMAP01_(theImage, pDIB_output); // doesn't seem to work
			boCVTstatus = CVT_URIVLRI_Binary_to_FIT_BITMAP08_((const RasterImage_binary*) theImage, pDIB_output);
			break;

		// gray
		case kGrayRaster:     //  8 bits per pixel gray-level
			if (theImage->hasFloatRaster())
			{
				// allocate a FI space to work in
				pDIB_output = FreeImage_AllocateT(FIT_FLOAT,   // Use 16 bpp instead of 24 to prevent color conversion, 
															 nCols,nRows, // height and width of the image
															 32);         // bits per pixel
				FAIL_CONDITION( pDIB_output == NULL,
								kVideoBufferAllocError,
								"FreeImage cannot allocate a workspace buffer");

				boCVTstatus = CVT_URIVLRI_GrayF_to_FIT_FLOAT_((const RasterImage_gray_F*) theImage, pDIB_output);
			}
			else
			{
				// allocate a FI space to work in
				pDIB_output = FreeImage_AllocateT(FIT_BITMAP,   // Use 16 bpp instead of 24 to prevent color conversion, 
															 nCols,nRows,  // height and width of the image
															 8);          // bits per pixel
				boCVTstatus = CVT_URIVLRI_Gray_to_FIT_BITMAP08_((const RasterImage_gray*) theImage, pDIB_output);
				FAIL_CONDITION( pDIB_output == NULL,
								kVideoBufferAllocError,
								"FreeImage cannot allocate a workspace buffer");
			}
			break;

		case kLongGrayRaster: //  12-16 bits per pixel gray-level
			FAIL_CONDITION( true,
							kNoCode,
							"Output format not handled yet");
			break;

      case kRGB24Raster:    // 24 bits per pixel color (8r-8g-8b)
			// allocate a FI space to work in
			pDIB_output = FreeImage_AllocateT(FIT_BITMAP,   // Use 16 bpp instead of 24 to prevent color conversion, 
												       nCols,nRows,  // height and width of the image
												       24);          // bits per pixel
			FAIL_CONDITION( pDIB_output == NULL,
							kVideoBufferAllocError,
							"FreeImage cannot allocate a workspace buffer");
			boCVTstatus = CVT_URIVLRI_RGB_to_FIT_BITMAP24_((const RasterImage_RGB*) theImage, pDIB_output);
			break;

      case kRGBa32Raster:   // 32 bits per pixel color (8r-8g-8b-8a)
			// allocate a FI space to work in
			pDIB_output = FreeImage_AllocateT(FIT_BITMAP,   // Use 16 bpp instead of 24 to prevent color conversion, 
												       nCols,nRows,  // height and width of the image
												       32);          // bits per pixel
			FAIL_CONDITION( pDIB_output == NULL,
							kVideoBufferAllocError,
							"FreeImage cannot allocate a workspace buffer");
			boCVTstatus = CVT_URIVLRI_RGBA_to_FIT_BITMAP32_((const RasterImage_RGBa*) theImage, pDIB_output);
			break;

		case kaRGB32Raster:   // 32 bits per pixel color (8a-8r-8g-8b)
								    // there is no class for this :(
									 // handle in writeRGBbytes_() when there is!
			FAIL_CONDITION( true,
							kNoCode,
							"Output format not handled yet");
         break;

		default:
			FAIL_CONDITION( true,
							kNoCode,
							"Output format not handled yet");
         break;
	} // end switch over URIVL RI types

	if (boCVTstatus)
	{
		// save the FI space as a file
		boFIWstatus = FreeImage_Save(out_fif, pDIB_output, filePath, 0);
 		FAIL_CONDITION( !boFIWstatus,
						kFileWritingError,
						"Error saving RasterImage via FreeImage");
	 }
	 else
	 {
	 }
	// free up our FI space when done
	FreeImage_Unload(pDIB_output);
}



#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark functions to convert URIVL rasters to FreeImage rasters
//--------------------------------------------------------------------------
#endif


bool CVT_URIVLRI_aRGB16_to_FIT_BITMAP32_(const RasterImage_aRGB16* pRasterImage, FIBITMAP* pFI_DIB)
{
	// cast output raster and get pointer to the pixels
	const unsigned char* pubRIPixel = pRasterImage->getRaster();

	// get number of bytes used to represent each pixel
	int nBytesPerPixel = FreeImage_GetLine(pFI_DIB) / FreeImage_GetWidth(pFI_DIB);

	// get size of the raster
	int nRows = FreeImage_GetHeight(pFI_DIB);
	int nCols = FreeImage_GetWidth(pFI_DIB);

	// loop over rows in the input image to map into the output raster
	// FI stores raster upside down
	for(int y = nRows-1; y >= 0 ; y--) 
	{
		// use a float array to represent subpixels since we mapped to a FreeImage float type
		unsigned char* pubFIPixel = (unsigned char*)FreeImage_GetScanLine(pFI_DIB, y);

		// loop over columns in this row
		for(int x = 0; x < nCols ; x++) 
		{
			//  16 bits per pixel color (1a-5r-5g-5b)
			pubFIPixel[FI_RGBA_RED]   = ((*pubRIPixel)&0x7c00)>>7; // normalize MSB to 8 bits
			pubFIPixel[FI_RGBA_GREEN] = ((*pubRIPixel)&0x03e0)>>2; // normalize MSB to 8 bits
			pubFIPixel[FI_RGBA_BLUE]  = ((*pubRIPixel)&0x001f)<<3; // normalize MSB to 8 bits
			if ((*pubRIPixel)&0x8000)
			{
				pubFIPixel[FI_RGBA_ALPHA]  = 255;
			}
			else
			{
				pubFIPixel[FI_RGBA_ALPHA]  = 0;
			}

			// increment out pixel pointers
			pubRIPixel++;  
			pubFIPixel += nBytesPerPixel;

		}
	}

	return true;
}

/////////////////////////////////////////

bool CVT_URIVLRI_Binary_to_FIT_BITMAP01_(const RasterImage_binary* pRasterImage, FIBITMAP* pFI_DIB)
{
	// cast output raster and get pointer to the pixels
	const unsigned char* pubRIPixel = pRasterImage->getRaster();

	// get size of the raster
	int nRows = FreeImage_GetHeight(pFI_DIB);
	int nCols = FreeImage_GetWidth(pFI_DIB);

	// loop over rows in the input image to map into the output raster
	// FI stores raster upside down
	for(int y = nRows-1; y >= 0 ; y--) 
	{
		// use a float array to represent subpixels since we mapped to a FreeImage float type
		unsigned char *pubFIPixel = (unsigned char*)FreeImage_GetScanLine(pFI_DIB, y);

		unsigned char ub8pixels = 255; // set inverted 11111111 then clear out set bits (no clue why FI works this way)
		int nBit = 1;
		// loop over columns in this row
		for(int x = 0; x < nCols; x++) 
		{
			unsigned char ubThisRIpixel = *pubRIPixel++;
			if (ubThisRIpixel != 0)
			{
				ub8pixels &= (0<<(8-nBit)); // AND in pixels bit by bit into this byte
			}
			if ( (nBit%8==0) || (x==(nCols-1)) )
			{
				*pubFIPixel++ = ub8pixels;
				ub8pixels = 255;
				nBit = 1;
			}
			else
			{
				nBit++;
			}
		}
	}

	return true;
}

/////////////////////////////////////////

bool CVT_URIVLRI_Binary_to_FIT_BITMAP08_(const RasterImage_binary* pRasterImage, FIBITMAP *pFI_DIB)
{
	// cast output raster and get pointer to the pixels
	const unsigned char *pubRIPixel = pRasterImage->getRaster();

	// get number of bytes used to represent each pixel
	int nBytesPerLine = FreeImage_GetLine(pFI_DIB);

	// get size of the raster
	int nRows = FreeImage_GetHeight(pFI_DIB);
	int nCols = FreeImage_GetWidth(pFI_DIB);

	int nBytesPerPixel =  nBytesPerLine / nCols;

	// loop over rows in the input image to map into the output raster
	// FI stores raster upside down
	for(int y = nRows-1; y >= 0 ; y--) 
	{
		// use a float array to represent subpixels since we mapped to a FreeImage float type
		unsigned char *pubFIPixel = (unsigned char*)FreeImage_GetScanLine(pFI_DIB, y);

		// loop over columns in this row
		for(int x = 0; x < nCols; x++) 
		{
			if (*pubRIPixel == 0)
			{
				*pubFIPixel = 0; 
			}
			else
			{
				*pubFIPixel = 255; 
			}

			// increment out pixel pointers
			pubRIPixel++;  
			pubFIPixel += nBytesPerPixel;
		}

	}

	return true;
}

/////////////////////////////////////////

bool CVT_URIVLRI_Gray_to_FIT_BITMAP08_(const RasterImage_gray* pRasterImage, FIBITMAP* pFI_DIB)
{
	// cast output raster and get pointer to the pixels
	const unsigned char* pubRIPixel = pRasterImage->getRaster();

	// get number of bytes used to represent each pixel
	int nBytesPerPixel = FreeImage_GetLine(pFI_DIB) / FreeImage_GetWidth(pFI_DIB);

	// get size of the raster
	int nRows = FreeImage_GetHeight(pFI_DIB);
	int nCols = FreeImage_GetWidth(pFI_DIB);

	// loop over rows in the input image to map into the output raster
	// FI stores raster upside down
	for(int y = nRows-1; y >= 0 ; y--) 
	{
		// use a float array to represent subpixels since we mapped to a FreeImage float type
		unsigned char *pubFIPixel = (unsigned char*)FreeImage_GetScanLine(pFI_DIB, y);

		// loop over columns in this row
		for(int x = 0; x < nCols ; x++) 
		{
			*pubFIPixel = *pubRIPixel;

			// increment out pixel pointers
			pubRIPixel++;  
			pubFIPixel += nBytesPerPixel;

		}

	}

	return true;
} 

/////////////////////////////////////////

bool CVT_URIVLRI_GrayF_to_FIT_FLOAT_(const RasterImage_gray_F* pRasterImage, FIBITMAP* pFI_DIB)
{
	// cast output raster and get pointer to the pixels
	const float* pfRIPixel = pRasterImage->getGrayF();

	// get size of the raster
	int nRows = FreeImage_GetHeight(pFI_DIB);
	int nCols = FreeImage_GetWidth(pFI_DIB);

	// loop over rows in the input image to map into the output raster
	// FI stores raster upside down
	for(int y = nRows-1; y >= 0 ; y--) 
	{
		// use a float array to represent subpixels since we mapped to a FreeImage float type
		float* pfFIPixel = (float*)FreeImage_GetScanLine(pFI_DIB, y);

		// loop over columns in this row
		for(int x = 0; x < nCols ; x++) 
		{
			*pfFIPixel = *pfRIPixel;

			// increment out pixel pointers
			pfRIPixel++;  
			pfFIPixel++;

		}
	}

	return true;
}

/////////////////////////////////////////

bool CVT_URIVLRI_RGB_to_FIT_BITMAP24_(const RasterImage_RGB* pRasterImage, FIBITMAP* pFI_DIB)
{
	// cast output raster and get pointer to the pixels
	const unsigned char *pubRIPixel = pRasterImage->getRaster();

	// get number of bytes used to represent each pixel
	int nBytesPerPixel = FreeImage_GetLine(pFI_DIB) / FreeImage_GetWidth(pFI_DIB);

	// get size of the raster
	int nRows = FreeImage_GetHeight(pFI_DIB);
	int nCols = FreeImage_GetWidth(pFI_DIB);

	// loop over rows in the input image to map into the output raster
	// FI stores raster upside down
	for(int y = nRows-1; y >= 0 ; y--) 
	{
		// use a float array to represent subpixels since we mapped to a FreeImage float type
		unsigned char *pubFIPixel = static_cast<unsigned char*>(FreeImage_GetScanLine(pFI_DIB, y));

		// loop over columns in this row
		for(int x = 0; x < nCols ; x++) 
		{
			pubFIPixel[FI_RGBA_RED]   = *pubRIPixel++;
			pubFIPixel[FI_RGBA_GREEN] = *pubRIPixel++;
			pubFIPixel[FI_RGBA_BLUE]  = *pubRIPixel++;

			// increment out pixel pointers
			pubFIPixel += nBytesPerPixel;
		}
	}

	return true;
}

/////////////////////////////////////////

bool CVT_URIVLRI_RGBA_to_FIT_BITMAP32_(const RasterImage_RGBa* pRasterImage, FIBITMAP* pFI_DIB)
{
	// cast output raster and get pointer to the pixels
	const unsigned char* pubRIPixel = pRasterImage->getRaster();

	// get number of bytes used to represent each pixel
	int nBytesPerPixel = FreeImage_GetLine(pFI_DIB) / FreeImage_GetWidth(pFI_DIB);

	// get size of the raster
	int nRows = FreeImage_GetHeight(pFI_DIB);
	int nCols = FreeImage_GetWidth(pFI_DIB);

	// loop over rows in the input image to map into the output raster
	// FI stores raster upside down
	for(int y = nRows-1; y >= 0 ; y--) 
	{
		// use a float array to represent subpixels since we mapped to a FreeImage float type
		unsigned char* pubFIPixel = (unsigned char*)FreeImage_GetScanLine(pFI_DIB, y);

		// loop over columns in this row
		for(int x = 0; x < nCols ; x++) 
		{
			pubFIPixel[FI_RGBA_RED]   = *pubRIPixel++;
			pubFIPixel[FI_RGBA_GREEN] = *pubRIPixel++;
			pubFIPixel[FI_RGBA_BLUE]  = *pubRIPixel++;
			pubFIPixel[FI_RGBA_ALPHA] = *pubRIPixel++;

			// increment out pixel pointers
			pubFIPixel += nBytesPerPixel;
		}
	}
	return true;
}

/////////////////////////////////////////

bool CVT_URIVLRI_RGBAF_to_FIT_RGBAF_(const RasterImage_RGBa_F* pRasterImage, FIBITMAP* pFI_DIB)
{
	// cast output raster and get pointer to the pixels
	const float* pfRIPixelRED = pRasterImage->getRedF();
	const float* pfRIPixelGRN = pRasterImage->getGreenF();
	const float* pfRIPixelBLU = pRasterImage->getBlueF();
	// get character pointer to access alpha channel
	const unsigned char *pubRIPixel = pRasterImage->getRaster();

	// get number of bytes used to represent each pixel
	int nBytesPerPixel = FreeImage_GetLine(pFI_DIB) / FreeImage_GetWidth(pFI_DIB);

	// get size of the raster
	int nRows = FreeImage_GetHeight(pFI_DIB);
	int nCols = FreeImage_GetWidth(pFI_DIB);

	// loop over rows in the input image to map into the output raster
	// FI stores raster upside down
	for(int y = nRows-1; y >= 0 ; y--) 
	{
		// use a float array to represent subpixels since we mapped to a FreeImage float type
		float *pfFIPixel = (float*)FreeImage_GetScanLine(pFI_DIB, y);

		// loop over columns in this row
		for(int x = 0; x < nCols ; x++) 
		{
			pfFIPixel[FI_RGBA_RED]   = *pfRIPixelRED++;
			pfFIPixel[FI_RGBA_GREEN] = *pfRIPixelGRN++;
			pfFIPixel[FI_RGBA_BLUE]  = *pfRIPixelBLU++;

			pubRIPixel+=3; // skip over RGB that we acccessed as float
			float fAlpha = (float)(*pubRIPixel);
			pubRIPixel++;
			pfFIPixel[FI_RGBA_ALPHA] = fAlpha/255.0f;

			// increment out pixel pointers
			pfFIPixel += nBytesPerPixel;
		}
	}

	return true;
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark FreeImage IO "helper" functions
//--------------------------------------------------------------------------
#endif

static unsigned DLL_CALLCONV urivlFIWreadProc_(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
	unsigned uN = fread(buffer, size, count, (FILE *)handle);
	return uN;
}

static unsigned DLL_CALLCONV urivlFIWwriteProc_(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
	unsigned uN = fwrite(buffer, size, count, (FILE *)handle);
	return uN;
}

static int  DLL_CALLCONV urivlFIWseekProc_(fi_handle handle, long offset, int origin)
{
	int nN = fseek((FILE *)handle, offset, origin);
	return nN;
}

static long  DLL_CALLCONV urivlFIWtellProc_(fi_handle handle)
{
	long lN = ftell((FILE *)handle);
	return lN;
}

static void urivlFIWimageErrorHandler_(FREE_IMAGE_FORMAT fif, const char* message)
{
	printf("\n*** "); 
	if(fif != FIF_UNKNOWN) 
	{
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf("%s", message);
	printf(" ***\n");
}


#endif // use URIVL_USE_FREEIMAGE
