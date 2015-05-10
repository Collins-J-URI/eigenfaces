/*  NAME:
        _ImageReader_FreeImage.cpp

    DESCRIPTION:
        implementation of the uriVisionLib _ImageReader_FreeImage class

    COPYRIGHT:
        (c) 2012-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#if URIVL_USE_FREEIMAGE // yes, this should be clipped out in the base ImageRaster class,
                        // but this way, the files do not have to be excluded from the project

#include <cstring>
#include <cstdio>
//
#include "_ImageReader_FreeImage.h"
#include "RasterImage_binary.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_RGBa_F.h"

using namespace uriVL;
using namespace std;

// conversion helper functions
bool CVT_FIT_FLOAT_to_URIVLRI_Binary_(FIBITMAP* cvtDIB, RasterImage *outImage);
bool CVT_FIT_FLOAT_to_URIVLRI_GrayF_(FIBITMAP* cvtDIB, RasterImage *outImage, bool boInvertGrayScale);
bool CVT_FIT_FLOAT_to_URIVLRI_Gray_(FIBITMAP* cvtDIB, RasterImage *outImage, bool boInvertGrayScale);
bool CVT_FIT_RGBF_to_URIVLRI_RGB_(FIBITMAP* cvtDIB, RasterImage *outImage);
bool CVT_FIT_RGBA_to_URIVLRI_RGBA_(FIBITMAP* cvtDIB, RasterImage *outImage);



//----------------------------------------------------------------------------------
//-------------------------------FreeImage IO "helper" functions -------------------
//
static unsigned DLL_CALLCONV urivlFIRreadProc_(void* buffer, unsigned size, unsigned count, fi_handle handle)
{
	unsigned uN = fread(buffer, size, count, (FILE *)handle);
	return uN;
}

static unsigned DLL_CALLCONV urivlFIRwriteProc_(void* buffer, unsigned size, unsigned count, fi_handle handle)
{
	unsigned uN = fwrite(buffer, size, count, (FILE *)handle);
	return uN;
}

static int  DLL_CALLCONV urivlFIRseekProc_(fi_handle handle, long offset, int origin)
{
	int nN = fseek((FILE *)handle, offset, origin);
	return nN;
}

static long  DLL_CALLCONV urivlFIRtellProc_(fi_handle handle)
{
	long lN = ftell((FILE *)handle);
	return lN;
}

static void urivlFIRimageErrorHandler_(FREE_IMAGE_FORMAT fif, const char *message)
{
	printf("\n*** "); 
	if(fif != FIF_UNKNOWN) 
	{
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf("%s", message);
	printf(" ***\n");
}

//----------------------------------------------------------------------------------
//-------------------------------Class implementation-------------------------------
//
#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif


_ImageReader_FreeImage::_ImageReader_FreeImage(void)
	:	filePath_(NULL)
{
    filePath_ = NULL;
}

_ImageReader_FreeImage::_ImageReader_FreeImage(const char* theFilePath)
	:	filePath_(NULL)
{
    setSource_(theFilePath);
}

_ImageReader_FreeImage::_ImageReader_FreeImage(const _ImageReader_FreeImage& ovbj)
	:	filePath_(NULL)
{
    FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_ImageReader_FreeImage copy constructor not implemented.");
}

_ImageReader_FreeImage::~_ImageReader_FreeImage(void)
{
    if (filePath_ != NULL)
        delete []filePath_;
}

const _ImageReader_FreeImage& _ImageReader_FreeImage:: operator = (const _ImageReader_FreeImage& ovbj)
{
    FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_ImageReader_FreeImage copy operator not implemented.");
	return *this;
}



//  This function should let the user select a file through a dialog window.
//  Will be implemented later
void _ImageReader_FreeImage::setSource_(void)
{
    FAIL_CONDITION( true,
                    kFileIOError,
                    "file selection dialog not implemented yet");
}

//  Sets the source (image file) by specifying a path
void _ImageReader_FreeImage::setSource_(const char* thePath)
{
	if (filePath_ != NULL)
	{
		delete filePath_; //wlcfixme need to do this in all other readers also to prevent memory leak
	}

    filePath_ = NULL;

    FAIL_CONDITION( thePath == NULL,
                    kFileNullPathError,
                    "NULL file path sent as parameter");

    const   int nThePathLength = strlen(thePath) + 1;
    filePath_ = new char[nThePathLength];
    strcpy(filePath_, thePath);
}

RasterImage* _ImageReader_FreeImage::readImage_(bool readUpsideDown)
{

	FAIL_CONDITION( true,
	                kFunctionNotImplemented,
	                "function readImage_(bool readUpsideDown) not implemented yet");
    return NULL;
}

RasterImage* _ImageReader_FreeImage::readImage_(const char* filePath, bool readUpsideDown)
{
	RasterImage* outImage = NULL;
	int nlRows, nlCols;
	FREE_IMAGE_COLOR_TYPE tColorType;
	FIBITMAP *inDIB, *cvtDIB;
	unsigned int unBitsPerPixel;
	bool boInvertGrayScale = false;
	bool boCVTstatus;

#ifdef FREEIMAGE_STATIC_LIB
	// call this ONLY when linking with FreeImage as a static library
	if (mboHaveNotRegisteredFreeImage)
	{
		FreeImage_Initialise();
		atexit(FreeImage_DeInitialise);
		mboHaveNotRegisteredFreeImage = false;
	}
#endif // FREEIMAGE_STATIC_LIB

	// initialize your own IO functions
	FreeImage_SetOutputMessage(urivlFIRimageErrorHandler_);
	FreeImageIO io;
	io.read_proc  = (FI_ReadProc)  urivlFIRreadProc_;
	io.write_proc = (FI_WriteProc) urivlFIRwriteProc_;
	io.seek_proc  = (FI_SeekProc)  urivlFIRseekProc_;
	io.tell_proc  = (FI_TellProc)  urivlFIRtellProc_;

	// open the file
	FILE *pFile = fopen(filePath, "rb");

	// if the file was opened AOK
	if (pFile != NULL) 
	{
		// find the buffer format
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromHandle(&io, (fi_handle)pFile, 0);

		// if the file format is known
		if(fif != FIF_UNKNOWN) 
		{
			// load from the file handle
			inDIB = FreeImage_LoadFromHandle(fif, &io, (fi_handle)pFile, 0);

			if (inDIB != NULL)
			{
				// get the number of rows and columns in the input image
				nlCols = FreeImage_GetWidth(inDIB);
				nlRows = FreeImage_GetHeight(inDIB);

				// decide the output rasterImage type based on the input format
				FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(inDIB);
				switch (image_type) // in order of TABLE 2 in FI 3.15.1 documentation
				{
					// unknown type :: don't map it
					case FIT_UNKNOWN:
						boCVTstatus = false;
						break;

					// bitmaps :: convert to RGB(a) or grayscale as appropriate
					case FIT_BITMAP:
						tColorType = FreeImage_GetColorType(inDIB);
						unBitsPerPixel = FreeImage_GetBPP(inDIB);
						switch (tColorType)
						{
							// MINISxxx means it is greyscale
							case FIC_MINISWHITE:
								boInvertGrayScale = true;
							case FIC_MINISBLACK:
								// use FI_CTT() to convert ALL grayscales to float to minimize conversion cases
								cvtDIB = FreeImage_ConvertToType(inDIB, FIT_FLOAT);
								FAIL_CONDITION(	cvtDIB==NULL,
												kRasterAllocationFailure,
												"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");
								if (unBitsPerPixel == 1) // return binary raster
								{
									outImage   = new RasterImage_binary(nlRows, nlCols);
									FAIL_CONDITION(	outImage==NULL,
													kRasterAllocationFailure,
													"outImage was not allocated in _ImageReader_FreeImage::readImage_");
									boCVTstatus = CVT_FIT_FLOAT_to_URIVLRI_Binary_(cvtDIB, outImage); // no invert needed 0,1 only
								}
								else if (unBitsPerPixel > 8) // return gray_F since it won't fit in a byte
								{
									outImage   = new RasterImage_gray_F(nlRows, nlCols);
									FAIL_CONDITION(	outImage==NULL,
													kRasterAllocationFailure,
													"outImage was not allocated in _ImageReader_FreeImage::readImage_");
									boCVTstatus = CVT_FIT_FLOAT_to_URIVLRI_GrayF_(cvtDIB, outImage, boInvertGrayScale);
								}
								else // return gray in a byte
								{
									outImage   = new RasterImage_gray(nlRows, nlCols);
									FAIL_CONDITION(	outImage==NULL,
													kRasterAllocationFailure,
													"outImage was not allocated in _ImageReader_FreeImage::readImage_");
									boCVTstatus = CVT_FIT_FLOAT_to_URIVLRI_Gray_(cvtDIB, outImage, boInvertGrayScale);
								}
								break;

							// standard color pallete of some kind WITHOUT alpha 
							case FIC_PALETTE:
								if (unBitsPerPixel == 1) // return binary raster
								{
									// use FI_CTT() to convert binary to float to minimize conversion cases
									cvtDIB = FreeImage_ConvertToType(inDIB, FIT_FLOAT);
									FAIL_CONDITION(	cvtDIB==NULL,
													kRasterAllocationFailure,
													"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");

									outImage   = new RasterImage_binary(nlRows, nlCols);
									FAIL_CONDITION(	outImage==NULL,
													kRasterAllocationFailure,
													"outImage was not allocated in _ImageReader_FreeImage::readImage_");
									boCVTstatus = CVT_FIT_FLOAT_to_URIVLRI_Binary_(cvtDIB, outImage); // no invert needed 0,1 only
								}
								else if (unBitsPerPixel > 8) // return gray_F since it won't fit in a byte
								{
									// use FI_CTT() to convert binary to float to minimize conversion cases
									cvtDIB = FreeImage_ConvertToType(inDIB, FIT_RGBF);
									FAIL_CONDITION(	cvtDIB==NULL,
													kRasterAllocationFailure,
													"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");

									outImage   = new RasterImage_gray_F(nlRows, nlCols);
									FAIL_CONDITION(	outImage==NULL,
													kRasterAllocationFailure,
													"outImage was not allocated in _ImageReader_FreeImage::readImage_");
									boCVTstatus = CVT_FIT_FLOAT_to_URIVLRI_GrayF_(cvtDIB, outImage, boInvertGrayScale);
								}
								else // return gray in a byte
								{
									// use FI_CTT() to convert RGBF to minimize conversion cases
									cvtDIB = FreeImage_ConvertToType(inDIB, FIT_RGBF);

									outImage   = new RasterImage_gray(nlRows, nlCols);
									FAIL_CONDITION(	outImage==NULL,
													kRasterAllocationFailure,
													"outImage was not allocated in _ImageReader_FreeImage::readImage_");
									boCVTstatus = CVT_FIT_FLOAT_to_URIVLRI_Gray_(cvtDIB, outImage, boInvertGrayScale);
								}

								break;

							// standard color pallete of some kind WITHOUT alpha 
							case FIC_RGB:
								//	RGB not to be used (as much as possible) in uriVL.  In fact, the only reason
								//	RasterImage_RG  was not removed altogether is because QuickTime for Windows
								//	only grabs properly in an RGB raster
								//
								// use FI_CTT() to convert RGBF to minimize conversion cases
								cvtDIB = FreeImage_ConvertTo32Bits(inDIB);
								FAIL_CONDITION(	cvtDIB==NULL,
												kRasterAllocationFailure,
												"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");
								outImage   = new RasterImage_RGBa(nlRows, nlCols);
								FAIL_CONDITION(	outImage==NULL,
												kRasterAllocationFailure,
												"outImage was not allocated in _ImageReader_FreeImage::readImage_");
								boCVTstatus = CVT_FIT_RGBA_to_URIVLRI_RGBA_(cvtDIB, outImage);

//								// use FI_CTT() to convert RGBF to minimize conversion cases
//								cvtDIB = FreeImage_ConvertToType(inDIB, FIT_RGBA);
//								FAIL_CONDITION(cvtDIB==NULL,	
//												kRasterAllocationFailure,
//												"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");
//								outImage   = new RasterImage_RGB(nlRows, nlCols);
//								FAIL_CONDITION(	outImage==NULL,
//												kRasterAllocationFailure,
//												"outImage was not allocated in _ImageReader_FreeImage::readImage_");
//								boCVTstatus = CVT_FIT_RGBF_to_URIVLRI_RGB_(cvtDIB, outImage);

								break;

							// standard color pallete of some kind WITH alpha
							case FIC_RGBALPHA:
								// no convert since there is no BITMAP to RGBA conversion function
								cvtDIB = FreeImage_Clone(inDIB);
								FAIL_CONDITION(	cvtDIB==NULL,
												kRasterAllocationFailure,
												"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");
								outImage   = new RasterImage_RGBa(nlRows, nlCols);							
								FAIL_CONDITION(	outImage==NULL,
												kRasterAllocationFailure,
												"outImage was not allocated in _ImageReader_FreeImage::readImage_");
								boCVTstatus = CVT_FIT_RGBA_to_URIVLRI_RGBA_(cvtDIB, outImage);
								break;

							case FIC_CMYK:
								// use FI_CTT() to convert RGBF to handle CMYK to RGB
								cvtDIB = FreeImage_ConvertToType(inDIB, FIT_RGBF);
								FAIL_CONDITION(	cvtDIB==NULL,
												kRasterAllocationFailure,
												"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");
								outImage   = new RasterImage_RGB(nlRows, nlCols);							
								FAIL_CONDITION(	outImage==NULL,
												kRasterAllocationFailure,
												"outImage was not allocated in _ImageReader_FreeImage::readImage_");
								boCVTstatus = CVT_FIT_RGBF_to_URIVLRI_RGB_(cvtDIB, outImage);
								break;

							default:
								boCVTstatus = false;
								break;

						} // end switch over bitmap types
						break;

					// grey-sale bitmaps :: map all to gray_F
					case FIT_UINT16:
					case FIT_UINT32:
					case FIT_INT32:
					case FIT_FLOAT:
						boInvertGrayScale = false;
						// use FI_CTT() to convert ALL grayscales to float to minimize conversion cases
						cvtDIB = FreeImage_ConvertToType(inDIB, FIT_FLOAT);
						FAIL_CONDITION(	cvtDIB==NULL,
										kRasterAllocationFailure,
										"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");
						outImage   = new RasterImage_gray_F(nlRows, nlCols);
						FAIL_CONDITION(	outImage==NULL,
										kRasterAllocationFailure,
										"outImage was not allocated in _ImageReader_FreeImage::readImage_");
						boCVTstatus = CVT_FIT_FLOAT_to_URIVLRI_GrayF_(cvtDIB, outImage, boInvertGrayScale);
						break;

					// no FI_CVT_DOUBLE to FLOAT so don't map it
					case FIT_DOUBLE:
					// FICOMPLEX (2x64-bit IEEE floating point) :: don't map it
					case FIT_COMPLEX:
						boCVTstatus = false;
						break;

					// floating point or large integer color WITHOUT ALPHA
					case FIT_RGB16:
					case FIT_RGBF:
						// use FI_CTT() to convert to RGBF 
						cvtDIB = FreeImage_ConvertToType(inDIB, FIT_RGBF);
						FAIL_CONDITION(	cvtDIB==NULL,
										kRasterAllocationFailure,
										"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");
						outImage = new RasterImage_RGB(nlRows, nlCols);							
						FAIL_CONDITION(	outImage==NULL,
										kRasterAllocationFailure,
										"outImage was not allocated in _ImageReader_FreeImage::readImage_");
						boCVTstatus = CVT_FIT_RGBF_to_URIVLRI_RGB_(cvtDIB, outImage);
						break;

					// floating point or large integer color WITH ALPHA
					case FIT_RGBA16:
					case FIT_RGBAF:
						// no convert since there is no BITMAP to RGBA conversion function
						cvtDIB = FreeImage_Clone(inDIB);
						FAIL_CONDITION(	cvtDIB==NULL,
										kRasterAllocationFailure,
										"FreeImage cvtDIB was not allocated in _ImageReader_FreeImage::readImage_");
						outImage = new RasterImage_RGBa(nlRows, nlCols);							
						FAIL_CONDITION(	outImage==NULL,
										kRasterAllocationFailure,
										"outImage was not allocated in _ImageReader_FreeImage::readImage_");
						boCVTstatus = CVT_FIT_RGBA_to_URIVLRI_RGBA_(cvtDIB, outImage);
						break;

					// we don't know how to map it :(
					default:
						boCVTstatus = false;
						break;

				} // end switch over image types

				if (boCVTstatus)
				{
					// cleanup and init when done
					outImage->setValidRect(0, 0, nlCols, nlRows);

					// MOVEME to CVT function based on data range


				}
				else
				{
					outImage = NULL;
				}
				
				// clean up converted FreeImage memory space since we're done with it
				FreeImage_Unload(cvtDIB);
				// clean up inputDIB since we're done with it
				FreeImage_Unload(inDIB);
			}
			else
			{
				FAIL_CONDITION( true,
								kRasterAllocationFailure,
								"FreeImage input DIB was not allocated in _ImageReader_FreeImage::readImage_");
			} // didn't get the inDIB
		} 
		else
		{
			FAIL_CONDITION( true,
							kUnsupportedImageFormatError,
							"File Image Format is Unknown in _ImageReader_FreeImage::readImage_");
		} // endif the file format is known

		fclose(pFile);
	} 
	else
	{
		FAIL_CONDITION( true,
						kFileNullPathError,
						"file path could not be opened in _ImageReader_FreeImage::readImage_");
	} // endif file was opened

	return outImage;
}

RasterImage* _ImageReader_FreeImage::getFrame_(bool readUpsideDown)
{
	return readImage_(filePath_, readUpsideDown);
}


void _ImageReader_FreeImage::getFrame_(RasterImage* imgOut, bool readUpsideDown)
{
	FAIL_CONDITION( true,
	                kFunctionNotImplemented,
	                "function not implemented yet");
}

#if 0
//------------------------------------------------------------------
#pragma mark -
#pragma mark local conversion functions
//------------------------------------------------------------------
#endif

bool CVT_FIT_FLOAT_to_URIVLRI_Binary_(FIBITMAP* cvtDIB, RasterImage* outImage)
{
	bool boRetValue = false;

	if ( (cvtDIB != NULL) && (outImage != NULL) )
	{
		// cast output raster and get pointer to the pixels
		unsigned char *pubOutPixel = ((RasterImage_binary*)outImage)->getRaster(R_W_ACCESS);
		int nThisOutPixel = 0;

		int nRows = FreeImage_GetHeight(cvtDIB);
		int nCols = FreeImage_GetWidth(cvtDIB);

		// loop over rows in the input image to map into the output raster
		// FI reads in upside down by default
		for(int y = nRows-1; y >= 0 ; y--) 
		{
			// use a float array to represent subpixels since we mapped to a FreeImage float type
			float *pfInPixel = (float*)FreeImage_GetScanLine(cvtDIB, y);
			int nThisInPixel = 0;

			// loop over columns in this row
			for(int x = 0; x < nCols ; x++) 
			{
				float fThisInPixel = pfInPixel[nThisInPixel];
				// binary is 0 or 1 if !0
				if ( fThisInPixel == 0.0f)
				{
					pubOutPixel[nThisOutPixel] = 0;
				}
				else
				{
					pubOutPixel[nThisOutPixel] = 1;
				}

				// increment out pixel counters
				nThisInPixel++; 
				nThisOutPixel++;

			} // end loop over columns in this line

		} // end loop over lines in this image
		
		boRetValue = true;
	} 
	else
	{
		FAIL_CONDITION( true, 
						kInvalidReferenceError, 
						"Invalid Input Pointer Value in CVT_FIT_FLOAT_to_URIVLRI_Binary_");
	} // bad input

	return(boRetValue);
} // end CVT_FIT_FLOAT_to_URIVLRI_Binary_()


bool CVT_FIT_FLOAT_to_URIVLRI_GrayF_(FIBITMAP* cvtDIB, RasterImage* outImage, bool boInvertGrayScale)
{
	bool boRetValue = false;
	float fMaxPixel = -1.0f;// anything below 0.0 will do
	float fMinPixel =  1.1f; // anything above 1.0 will do
	if ( (cvtDIB != NULL) && (outImage != NULL) )
	{
		// cast output raster and get pointer to the pixels
		float *pfOutGreyPixel = ((RasterImage_gray_F*)outImage)->getGrayF(R_W_ACCESS);
		int nThisOutPixel = 0;

		int nRows = FreeImage_GetHeight(cvtDIB);
		int nCols = FreeImage_GetWidth(cvtDIB);

		// loop over rows in the input image to map into the output raster
		// FI reads in upside down by default
		for(int y = nRows-1; y >= 0 ; y--) 
		{
			// use a float array to represent subpixels since we mapped to a FreeImage float type
			float *pfInPixel = (float*)FreeImage_GetScanLine(cvtDIB, y);
			int nThisInPixel = 0;

			// loop over columns in this row
			for(int x = 0; x < nCols ; x++) 
			{
				// grey scale only has one subpixel
				pfOutGreyPixel[nThisOutPixel] = pfInPixel[nThisInPixel];

				// update min and max for use in selecting the scaling range
				if (pfInPixel[nThisInPixel] > fMaxPixel) fMaxPixel = pfInPixel[nThisInPixel];
				if (pfInPixel[nThisInPixel] < fMinPixel) fMinPixel = pfInPixel[nThisInPixel];

				// increment out pixel counters
				nThisInPixel++; 
				nThisOutPixel++;

			} // end loop over columns in this line

		} // end loop over lines in this image

		// invert the grayscale if needed
		nThisOutPixel = 0; // reset pixel indexer
		if (boInvertGrayScale)
		{
			// loop over the rows
			for(int y = 0; y < nRows; y++) 
			{
				// loop over columns in this row
				for(int x = 0; x < nCols ; x++) 
				{
					pfOutGreyPixel[nThisOutPixel] = fMinPixel + (fMaxPixel - pfOutGreyPixel[nThisOutPixel]);
				}
			}
			// now flip min and max values
			float fTempMaxPixel = fMaxPixel;
			fMaxPixel = -fMinPixel;
			fMinPixel = -fTempMaxPixel;
		} // end invert grey scale

		// update the scaling range flag for this raster
		// see RasterImage.h for details of the parameters

		// default to kRangeFloat
	  	((RasterImage_gray_F*)outImage)->setFloatConversionMode(kRangeFloat);

#if 0 // only kSignedFloat seems to work correctly
		// change conversion mode if some special condition is met
		float fMidPoint = fMinPixel + (fMaxPixel-fMinPixel)/2.0f;
		if (fMinPixel>=0.0f)
		{
			((RasterImage_gray_F*)outImage)->setFloatConversionMode(kPositiveFloat);
		} 
		else if (fMinPixel == -fMaxPixel)
		{
			((RasterImage_gray_F*)outImage)->setFloatConversionMode(kSignedFloat);
		}
		else if (fMidPoint > 0.0f)
		{
			((RasterImage_gray_F*)outImage)->setFloatConversionMode(kSaturatedPositive);
		}
		else if (fMidPoint < 0.0f)
		{
			((RasterImage_gray_F*)outImage)->setFloatConversionMode(kSaturatedSigned);
		}
#endif
		((RasterImage_gray_F*)outImage)->setFloatConversionMode(kSignedFloat);

		boRetValue = true;
	} 
	else
	{
		FAIL_CONDITION( true, 
						kInvalidReferenceError, 
						"Invalid Input Pointer Value in CVT_FIT_FLOAT_to_URIVLRI_GrayF_");
	} // bad input

	return(boRetValue);
} // end CVT_FIT_FLOAT_to_URIVLRI_GrayF_()



bool CVT_FIT_FLOAT_to_URIVLRI_Gray_(FIBITMAP* cvtDIB, RasterImage* outImage, bool boInvertGrayScale)
{
	bool boRetValue = false;
	if ( (cvtDIB != NULL) && (outImage != NULL) )
	{
		// cast output raster and get pointer to the pixels
		unsigned char *pubOutPixel = ((RasterImage_gray*)outImage)->getRaster(R_W_ACCESS);
		int nThisOutPixel = 0;

		int nRows = FreeImage_GetHeight(cvtDIB);
		int nCols = FreeImage_GetWidth(cvtDIB);

		// loop over rows in the input image to map into the output raster
		// FI reads in upside down by default
		for(int y = nRows-1; y >= 0 ; y--) 
		{
			// use a float array to represent subpixels since we mapped to a FreeImage float type
			float *pfInPixel = (float*)FreeImage_GetScanLine(cvtDIB, y);
			int nThisInPixel = 0;

			// loop over columns in this row
			for(int x = 0; x < nCols ; x++) 
			{
				if (boInvertGrayScale)
				{
					pubOutPixel[nThisOutPixel] = 255 - (unsigned char)(255 * pfInPixel[nThisInPixel]);
				}
				else
				{
					pubOutPixel[nThisOutPixel] = (unsigned char)(255 * pfInPixel[nThisInPixel]);
				}

				// increment out pixel counters
				nThisInPixel++; 
				nThisOutPixel++;

			} // end loop over columns in this line

		} // end loop over lines in this image

		boRetValue = true;
	} 
	else
	{
		FAIL_CONDITION( true, 
						kInvalidReferenceError, 
						"Invalid Input Pointer Value in CVT_FIT_FLOAT_to_URIVLRI_Gray_");
	} // bad input

	return(boRetValue);
} // end CVT_FIT_FLOAT_to_URIVLRI_Gray_()


bool CVT_FIT_RGBF_to_URIVLRI_RGB_(FIBITMAP* cvtDIB, RasterImage* outImage)
{
	bool boRetValue = false;
	if ( (cvtDIB != NULL) && (outImage != NULL) )
	{
		// cast output raster and get pointer to the pixels
		unsigned char *pubOutPixel = ((RasterImage_RGB*)outImage)->getRGB(R_W_ACCESS);

		// get number of bytes used to represent each pixel
		int nBytesPerPixel = FreeImage_GetLine(cvtDIB) / FreeImage_GetWidth(cvtDIB);

		int nRows = FreeImage_GetHeight(cvtDIB);
		int nCols = FreeImage_GetWidth(cvtDIB);

		// loop over rows in the input image to map into the output raster
		// FI reads in upside down by default
		for(int y = nRows-1; y >= 0 ; y--) 
		{
			// use a float array to represent subpixels since we mapped to a FreeImage float type
			float *pfInPixel = (float*)FreeImage_GetScanLine(cvtDIB, y);

			// loop over columns in this row
			for(int x = 0; x < nCols ; x++) 
			{
				*pubOutPixel++ = (unsigned char) (pfInPixel[FI_RGBA_BLUE]   * 255.0f);
				*pubOutPixel++ = (unsigned char) (pfInPixel[FI_RGBA_GREEN] * 255.0f);
				*pubOutPixel++ = (unsigned char) (pfInPixel[FI_RGBA_RED]  * 255.0f);
				// there is no alpha in this raster so don't try to set it

				// increment out pixel pointers
				pfInPixel += nBytesPerPixel/sizeof(float); // since we're pointing to floats

			} // end loop over columns in this line

		} // end loop over lines in this image

		boRetValue = true;
	} 
	else
	{
		FAIL_CONDITION( true, 
						kInvalidReferenceError, 
						"Invalid Input Pointer Value in CVT_FIT_RGBF_to_URIVLRI_RGB_");
	} // bad input

	return(boRetValue);
} // end CVT_FIT_RGBF_to_URIVLRI_RGB_()



bool CVT_FIT_RGBA_to_URIVLRI_RGBA_(FIBITMAP* cvtDIB, RasterImage* outImage)
{
	bool boRetValue = false;
	if ( (cvtDIB != NULL) && (outImage != NULL) )
	{
		// cast output raster and get pointer to the pixels
		unsigned char* pubOutPixel = ((RasterImage_RGBa*)outImage)->getRGBa(R_W_ACCESS);
	// get number of bytes used to represent each pixel
		int nBytesPerPixel = FreeImage_GetLine(cvtDIB) / FreeImage_GetWidth(cvtDIB);

		int nRows = FreeImage_GetHeight(cvtDIB);
		int nCols = FreeImage_GetWidth(cvtDIB);

		// loop over rows in the input image to map into the output raster
		// FI reads in upside down by default
		for(int y = nRows-1; y >= 0 ; y--) 
		{
			// use a float array to represent subpixels since we mapped to a FreeImage float type
			unsigned char *pubInPixel = (unsigned char*)FreeImage_GetScanLine(cvtDIB, y);

			// loop over columns in this row
			for(int x = 0; x < nCols ; x++) 
			{
//				*pubOutPixel++ = pubInPixel[FI_RGBA_BLUE];
//				*pubOutPixel++ = pubInPixel[FI_RGBA_GREEN];
//				*pubOutPixel++ = pubInPixel[FI_RGBA_RED];
//				*pubOutPixel++ = pubInPixel[FI_RGBA_ALPHA];
				*pubOutPixel++ = pubInPixel[FI_RGBA_RED];
				*pubOutPixel++ = pubInPixel[FI_RGBA_GREEN];
				*pubOutPixel++ = pubInPixel[FI_RGBA_BLUE];
				*pubOutPixel++ = pubInPixel[FI_RGBA_ALPHA];

				// increment out pixel pointers
				pubInPixel += nBytesPerPixel/sizeof(unsigned char); // since we're pointing to unsigned chars

			} // end loop over columns in this line

		} // end loop over lines in this image

		boRetValue = true;
	} 
	else
	{
		FAIL_CONDITION( true, 
						kInvalidReferenceError, 
						"Invalid Input Pointer Value in CVT_FIT_RGBA_to_URIVLRI_RGBA_");
	} // bad input

	return(boRetValue);
} // end CVT_FIT_RGBA_to_URIVLRI_RGBA_()


#endif // use URIVL_USE_FREEIMAGE
