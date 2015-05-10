/*  NAME:
        _ImageWriter_QT.cpp

    DESCRIPTION:
        implementation of the uriVisionLib _ImageWriter_QT class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

//	Mostly there to facilitate life for Visual Studio developers, since in Xcode and
//	Cmake souce files are excluded directly from the project depending on the
//	state defined by the environment variables.
#if URIVL_USE_QUICKTIME

#include "_ImageWriter_QT.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"

using namespace std;
using namespace uriVL;

OSType getWriteExtensionType_(const char* ext);
OSErr getFSSpecOut_(const char* unixPath, FSSpec* theFSSpec);

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Constructors and destructor
//--------------------------------------------------------------------------
#endif

_ImageWriter_QT::_ImageWriter_QT(void) 
		:	dataRef_(NULL),
			filePath_(NULL),
			dataRefDefined_(false)			
{
}

_ImageWriter_QT::_ImageWriter_QT(const _ImageWriter_QT& obj) 
		:	dataRef_(NULL),
			filePath_(NULL),
			dataRefDefined_(false)
{
    FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_ImageWriter_QT coopy constructor not implemented");
}

_ImageWriter_QT::_ImageWriter_QT(const char* theFilePath) 
		:	dataRef_(NULL),
			filePath_(NULL),
			dataRefDefined_(false)
{
    setDestination_(theFilePath);
}

_ImageWriter_QT::_ImageWriter_QT(Handle theRef) 
		:	dataRef_(NULL),
			filePath_(NULL),
			dataRefDefined_(false)
{
    setDestination_(theRef);
}


_ImageWriter_QT::~_ImageWriter_QT(void)
{
    if (filePath_ != NULL)
        delete []filePath_;
}

const _ImageWriter_QT& _ImageWriter_QT::operator = (const _ImageWriter_QT& obj) 
{
    FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"_ImageWriter_QT coopy constructor not implemented");
	return *this;
}


#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Set destination
//--------------------------------------------------------------------------
#endif


void _ImageWriter_QT::setDestination_(void)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "file selection dialog not implemented yet in _ImageWriter_QT::setDestination_");
}

void _ImageWriter_QT::setDestination_(const char* filePath)
{
    dataRefDefined_ = false;
	if (filePath_ != NULL)
		delete []filePath_;
    filePath_ = NULL;

    FAIL_CONDITION( filePath == NULL,
                    kFileNullPathError,
                    "null file path send as parameter to _ImageWriter_QT::setDestination_");

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

void _ImageWriter_QT::setDestination_(Handle theRef)
{
	if (filePath_ != NULL)
		delete []filePath_;
    filePath_ = NULL;
    //
	dataRef_ = theRef;
    dataRefDefined_ = true;
}

#if 0
#pragma mark -
#endif

void _ImageWriter_QT::outputFrame_(const RasterImage* theImage, bool writeUpsideDown)
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
		e.appendToMessage("called by _ImageWriter_QT::outputFrame_");
		throw e;
	}
} // end outputFrame_()


//#if URIVL_OS_MACOS
//
//	OSErr _ImageReader_QT::getFSSpec(const char* unixPath, FSSpec* theFSSpec)
//	{
//		FSRef myRef;	
//		OSStatus myErr = FSPathMakeRef((UInt8*)unixPath, &myRef, NULL);
//		FAIL_CONDITION( myErr!=noErr,
//						kFileIOError,
//						"Failed to find file in _ImageReader_QT::getFSSpec");
//		
//		// maps FSRef to FSSpec
//		return FSGetCatalogInfo(&myRef, kFSCatInfoNone, NULL, NULL, theFSSpec, NULL);	
//	}
//#endif

void _ImageWriter_QT::writeImage_(const RasterImage* theImage, const char* filePath,
								  bool writeUpsideDown)
{
	// make sure we have something for a path
	FAIL_CONDITION( (filePath == NULL) || (strlen(filePath) == 0),
    	            kFileNullPathError,
    	            "Null file path error in _ImageWriter_QT::writeImage_");
	//  no point trying to write a null image
	FAIL_CONDITION( theImage == NULL,
                    kNullRasterImageError,
                    "Attempted to output a null RasterImage in _ImageWriter_QT::writeImage_");


	unsigned long pathLength = strlen(filePath);
	const char* extension = filePath + (pathLength - 3);
	OSType writeType = getWriteExtensionType_(extension);
	FAIL_CONDITION( writeType == -1,
                    kWrongFileTypeError,
                    "Output file extension not supported in _ImageWriter_QT::writeImage_");
	

	GraphicsExportComponent myExportComponent = NULL;
	OSErr   myErr = noErr;
	
	#if URIVL_OS_MACOS
	
		FSSpec theFSSpec;
		//  get FSSpec from filepath
		myErr = getFSSpecOut_(filePath, &theFSSpec);
		
		FAIL_CONDITION( myErr,
					   kFileIOError,
					   "getFSSpec failed in _ImageWriter_QT::writeImage_");
		

	//------------------------------------------------------------------------------------
	//  The Windows code is based on the ASCIIMoviePlayerSample code
	//------------------------------------------------------------------------------------
	#elif URIVL_OS_WIN32 
	
		FSSpec theFSSpec;
		short  resRefNum = -1;
		
		char zePath[256];
		strcpy(zePath, filePath);
		myErr = NativePathNameToFSSpec(zePath, &theFSSpec, kErrorIfFileNotFound /* flags */);
		FAIL_CONDITION( myErr,
					   kFileIOError,
					   "NativePathNameToFSSpec failed in _ImageWriter_QT::writeImage_");
		
	
	#else   
		#error  I should not be ending up here
	#endif					

	OSStatus status = noErr;

	//	Open the Exporter Component.
	status = OpenADefaultComponent(	GraphicsExporterComponentType,
									//writeType,
									kQTFileTypeTargaImage,
									&myExportComponent);
	FAIL_CONDITION( status != noErr,
				   kFileIOError,
				   "Could not create an export component in _ImageWriter_QT::writeImage_");
	FAIL_CONDITION( myExportComponent == NULL,
					kFileIOError,
					"Could not create an exporter for the selected file in _ImageReader_QT::readImage");

	
	status = GraphicsExportSetOutputFile(myExportComponent, &theFSSpec);
	FAIL_CONDITION( status != noErr,
				   kFileIOError,
				   "Could not create an output file in _ImageWriter_QT::writeImage_");
				   
	//  Do the export, write the file.
	status = GraphicsExportDoExport(myExportComponent, NULL);
	FAIL_CONDITION( status != noErr,
				   kFileIOError,
				   "Could not output image file in _ImageWriter_QT::writeImage_");

}

#if URIVL_OS_MACOS

	OSErr getFSSpecOut_(const char* unixPath, FSSpec* theFSSpec)
	{
		FSRef myRef;	
		OSStatus myErr = FSPathMakeRef((UInt8*)unixPath, &myRef, NULL);
		FAIL_CONDITION( myErr!=noErr,
					   kFileIOError,
					   "Failed to find file in _ImageReader_QT::getFSSpec");
		
		// maps FSRef to FSSpec
		return FSGetCatalogInfo(&myRef, kFSCatInfoNone, NULL, NULL, theFSSpec, NULL);	
	}

#endif

OSType getWriteExtensionType_(const char* ext)
{
	char lcExt[5];
	int k;
	for (k=0; ext[k] != 0; k++)
	{
		lcExt[k] = tolower(ext[k]);
	}
	lcExt[k] = 0;
	
	if (strncmp(lcExt, "jpg", 3) == 0)
		return kQTFileTypeJPEG;
	else if (strncmp(lcExt, "jpeg", 4) == 0)
		return kQTFileTypeJPEG;
	else if (strncmp(lcExt, "bmp", 3) == 0)
		return kQTFileTypeBMP;
	else if (strncmp(lcExt, "tga", 3) == 0)
		return kQTFileTypeTargaImage;
	else if (strncmp(lcExt, "tif", 3) == 0)
		return kQTFileTypeTIFF;
	else if (strncmp(lcExt, "tiff", 4) == 0)
		return kQTFileTypeTIFF;
	else if (strncmp(lcExt, "png", 3) == 0)
		return kQTFileTypePNG;
	else if (strncmp(lcExt, "gif", 3) == 0)
		return kQTFileTypeGIF;
	else if (strncmp(lcExt, "psd", 3) == 0)
		return kQTFileTypePhotoShop;
	else if (strncmp(lcExt, "sgi", 3) == 0)
		return kQTFileTypeSGIImage;
	else if (strncmp(lcExt, "rgb", 3) == 0)
		return kQTFileTypeSGIImage;
	else if (strncmp(lcExt, "rgba", 4) == 0)
		return kQTFileTypeSGIImage;

    return (-1);
}



#endif // URIVL_USE_QUICKTIME
