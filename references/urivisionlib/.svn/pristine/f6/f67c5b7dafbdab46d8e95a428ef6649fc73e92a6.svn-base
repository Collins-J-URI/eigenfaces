/*  NAME:
        ImageWriter.cpp

    DESCRIPTION:
        implementation of the uriVisionLib ImageWriter class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ErrorReport.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
//
#include "ImageWriter.h"
#if URIVL_USE_QUICKTIME
	#include "_ImageWriter_QT.h"
#elif URIVL_USE_FREEIMAGE
	#include "_ImageWriter_FreeImage.h"
#elif URIVL_USE_HACK_RW
	#include "_ImageWriter_hack.h"
#else
	#error no writer defined
#endif

using namespace uriVL;


const char	*ImageWriter::kWriteExtensions_[4] = {
													"tga",
													"ppm",
													"pgm",
													"bmp"
											    	};


ImageWriter::ImageWriter(void) 
    :   VideoDestination( ),
		//
		#if URIVL_USE_QUICKTIME
			writer_(new _ImageWriter_QT( ))
		#elif URIVL_USE_FREEIMAGE
			writer_(new _ImageWriter_FreeImage( ))
		#elif URIVL_USE_HACK_RW
			writer_(new _ImageWriter_hack( ))
		#else
			  #error no writer defined
		#endif
{

}

ImageWriter::ImageWriter(const char* theFilePath) 
    :   VideoDestination( ),
		//
		#if URIVL_USE_QUICKTIME
			writer_(new _ImageWriter_QT(theFilePath))
		#elif URIVL_USE_FREEIMAGE
			writer_(new _ImageWriter_FreeImage(theFilePath))
		#elif URIVL_USE_HACK_RW
			writer_(new _ImageWriter_hack(theFilePath))
		#else
			#error no writer defined
    #endif
{

}

#if (URIVL_USE_QUICKTIME || URIVL_USE_HACK_RW)
	ImageWriter::ImageWriter(Handle dataRef) 
			:   VideoDestination( ),
				//
				#if URIVL_USE_QUICKTIME
					writer_(new _ImageWriter_QT(dataRef))
				#else
					writer_(new _ImageWriter_hack(dataRef))
				#endif
	{
	
	}

#endif

ImageWriter::~ImageWriter(void)
{
	if (writer_ != NULL)
	{
		delete writer_;
	}	
}

#if 0
#pragma mark -
#endif


void ImageWriter::setDestination(void)
{
    writer_->setDestination_( );
}

void ImageWriter::setDestination(const char* filePath)
{
    writer_->setDestination_(filePath);
}

#if (URIVL_USE_QUICKTIME || URIVL_USE_HACK_RW)
	void ImageWriter::setDestination(Handle dataRef)
	{
		writer_->setDestination_(dataRef);
	}
#endif

#if 0
#pragma mark -
#endif


void ImageWriter::outputFrame(const RasterImage* theImage, bool writeUpsideDown)
{
    writer_->outputFrame_(theImage, writeUpsideDown);
}


void ImageWriter::writeImage(const RasterImage* theImage, bool writeUpsideDown)
{
    writer_->outputFrame_(theImage, writeUpsideDown);
}

void ImageWriter::writeImage(const RasterImage* theImage, const char* filePath, bool writeUpsideDown)
{
	try
	{
		#if URIVL_USE_QUICKTIME
			_ImageWriter_QT::writeImage_(theImage, filePath, writeUpsideDown);
		#elif URIVL_USE_FREEIMAGE
			_ImageWriter_FreeImage::writeImage_(theImage, filePath, writeUpsideDown);
		#elif  URIVL_USE_HACK_RW
			_ImageWriter_hack::writeImage_(theImage, filePath, writeUpsideDown);
		#else
			//	we should not make it there
			#error No writer defined in ImageWriter::writeImage
		#endif
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageWriter::writeImage");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kFileIOError,
						"Unexpected error encountered in ImageWriter::writeImage");
	}
}

