/*  NAME:
        ImageReader.cpp

    DESCRIPTION:
        implementation of the uriVisionLib ImageReader class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"
//
#include "ImageReader.h"
#if URIVL_USE_QUICKTIME
    #include "_ImageReader_QT.h"
#elif URIVL_USE_FREEIMAGE
    #include "_ImageReader_FreeImage.h"
#elif URIVL_USE_HACK_RW
   #include "_ImageReader_hack.h"
#else
	#error no reader defined
#endif

using namespace uriVL;


//----------------------------------------------------------------------------------
//-------------------------------Class implementation-------------------------------
//

ImageReader::ImageReader(void)
	: VideoSource( ),
	//
	reader_(NULL)
{
    #if URIVL_USE_QUICKTIME
        reader_ = new _ImageReader_QT( );
	 #elif URIVL_USE_FREEIMAGE
		reader_ = new _ImageReader_FreeImage( );
	 #elif  URIVL_USE_HACK_RW
		reader_ = new _ImageReader_hack( );
    #else
		#error No reader defined in ImageReader::ImageReader(void)
    #endif
}

ImageReader::ImageReader(const char* theFilePath)
	: VideoSource( ),
	//
	reader_(NULL)
{
	 #if URIVL_USE_QUICKTIME
        reader_ = new _ImageReader_QT(theFilePath);
	 #elif URIVL_USE_FREEIMAGE
		reader_ = new _ImageReader_FreeImage(theFilePath);
	 #elif  URIVL_USE_HACK_RW
		reader_ = new _ImageReader_hack(theFilePath);
    #else
		#error No reader defined in ImageReader::ImageReader(const char* theFilePath)
    #endif
}


ImageReader::ImageReader(const ImageReader& reader) 
	: VideoSource( ),
	//
	reader_(NULL)
{
	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"Copy constructor not implemented in ImageReader class");
}
			
ImageReader::~ImageReader(void)
{
	if (reader_ != NULL)
	{
		delete reader_;
	}
}

#if 0
#pragma mark -
#endif

const ImageReader& ImageReader::operator = (const ImageReader& reader) {

	FAIL_CONDITION(	true,
					kFunctionNotImplemented,
					"Copy operator not implemented in ImageReader class");
	return *this;
}


//  This function should let the user select a file through a dialog window.
void ImageReader::setSource(void)
{
    reader_->setSource_();
}

//  Sets the source (image file) by specifying a path
void ImageReader::setSource(const char* thePath)
{
    reader_->setSource_(thePath);
}

#if 0
#pragma mark -
#endif


RasterImage* ImageReader::readImage(bool readUpsideDown)
{
	RasterImage* imgOut = NULL;

	try
	{
		#if URIVL_USE_QUICKTIME
			imgOut = _ImageReader_QT::readImage_(readUpsideDown);
		#elif URIVL_USE_FREEIMAGE
			imgOut = _ImageReader_FreeImage::readImage_(readUpsideDown);
		#elif  URIVL_USE_HACK_RW
			imgOut = _ImageReader_hack::readImage_(readUpsideDown);
		#else
			#error No reader defined in ImageReader::ImageReader(const char* theFilePath)
		#endif

	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageReader::readImage");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in ImageReader::readImage");
	}
	
	updateImageRasterStatus_(imgOut);
	return imgOut;
}


RasterImage* ImageReader::readImage(const char* thePath, bool readUpsideDown)
{
	RasterImage* imgOut = NULL;	
	try
	{
		#if URIVL_USE_QUICKTIME
			imgOut = _ImageReader_QT::readImage_(thePath, readUpsideDown);
		#elif URIVL_USE_FREEIMAGE
			imgOut = _ImageReader_FreeImage::readImage_(thePath, readUpsideDown);
		#elif  URIVL_USE_HACK_RW
			imgOut = _ImageReader_hack::readImage_(thePath, readUpsideDown);
		#else
			//	we should not make it there
			#error No reader defined in ImageReader::readImage(const char* thePath, bool readUpsideDown)
		#endif
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageReader::readImage");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kFileIOError,
						"Unexpected error encountered in ImageReader::readImage");
	}
	
	updateImageRasterStatus_(imgOut);
	return imgOut;
}


RasterImage* ImageReader::getFrame(void)
{
	RasterImage* imgOut = NULL;
	
	try 
	{
		imgOut = getFrame(false);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageReader::getFrame");
		throw e;
	}

	return imgOut;
}


RasterImage* ImageReader::getFrame(bool readUpsideDown)
{
	RasterImage* imgOut = NULL;

	try
	{
		imgOut = reader_->getFrame_(readUpsideDown);
		updateImageRasterStatus_(imgOut);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageReader::readImage");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in ImageReader::readImage");
	}

	return imgOut;
}


void ImageReader::getFrame(RasterImage* imgOut)
{
	try
	{
		getFrame(imgOut, false);	
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageReader::getFrame");
		throw e;
	}
}


void ImageReader::getFrame(RasterImage* imgOut, bool readUpsideDown)
{
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "Nuul image passed as destination to an ImageReader");

	try
	{
		reader_->getFrame_(imgOut, readUpsideDown);	
		updateImageRasterStatus_(imgOut);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageReader::getFrame");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in ImageReader::getFrame");
	}
}

