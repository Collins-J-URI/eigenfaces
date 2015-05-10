/*  NAME:
        _ImageReader_QT.cpp

    DESCRIPTION:
        implementation of the uriVisionLib _ImageReader_QT class

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

#include <iostream>
//
#include "_ImageReader_QT.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"

using namespace std;
using namespace uriVL;


OSErr getFSSpecIn_(const char* unixPath, FSSpec* theFSSpec);

//----------------------------------------------------------------------------------
//-------------------------------Class implementation-------------------------------
//


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructors
//----------------------------------------------------------
#endif


_ImageReader_QT::_ImageReader_QT(void)
	:	fsspecDefined_(false),
		filePath_(NULL),
		fsspec_()
{
}

_ImageReader_QT::_ImageReader_QT(const char* theFilePath)
	:	fsspecDefined_(false),
		filePath_(NULL),
		fsspec_()
{
    setSource_(theFilePath);
}

_ImageReader_QT::_ImageReader_QT(const _ImageReader_QT& reader)
	:	fsspecDefined_(false),
		filePath_(NULL),
		fsspec_()
{
    FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Copy constructor not implemented");
}

_ImageReader_QT::~_ImageReader_QT(void)
{
    if (filePath_ != NULL)
        delete []filePath_;
}

const _ImageReader_QT& _ImageReader_QT::operator = (const _ImageReader_QT& reader)
{
    FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Copy constructor not implemented");
	return *this;
}


#if 0
#pragma mark -
#endif

//  This function should let the user select a file through a dialog window.
//  Will be implemented when I complete the QT "takeover"
void _ImageReader_QT::setSource_(void)
{
	OSErr 			myErr = noErr;
    const int       kTypeListCount = 1;
	OSType 			myTypeList[kTypeListCount] = {kQTFileTypeQuickTimeImage};
	FSSpec			theFSSpec;

    myErr = getOneFileWithPreview_(kTypeListCount, (TypeListPtr)&myTypeList, &theFSSpec, NULL);
    FAIL_CONDITION( myErr,
                    kFileIOError,
                    "Could not open dialog window with preview in _ImageReader_QT::setSource");

}

//  Sets the source (image file) by specifying a path
void _ImageReader_QT::setSource_(const char* thePath)
{
    fsspecDefined_ = false;
    filePath_ = NULL;

    FAIL_CONDITION( thePath == NULL,
                    kFileNullPathError,
                    "null file path send as parameter");

    unsigned long l = strlen(thePath) + 1;
    filePath_ = new char[l];
    strcpy(filePath_, thePath);
}

#if 0
#pragma mark -
#endif



RasterImage* _ImageReader_QT::readImage_(bool readUpsideDown)
{
	OSErr 			myErr = noErr;
    const int       kTypeListCount = 1;
	OSType 			myTypeList[kTypeListCount] = {kQTFileTypeQuickTimeImage};
	GraphicsImportComponent		importer = NULL;

    //------------------------------------------------------------------------------------
    //  This code should be replaced by calls using the DataRef structures of "current" QT.
    //  (or whatever will be the new thing in QTKit)
    //------------------------------------------------------------------------------------
    #if URIVL_OS_MACOS
    
		FSSpec			theFSSpec;
	
        //  Select an image file using a dialog window
        myErr = getOneFileWithPreview_(kTypeListCount, (TypeListPtr)&myTypeList, &theFSSpec, NULL);
		
		FAIL_CONDITION( myErr,
                        kFileIOError,
                        "Could not open dialog window with preview in _ImageReader_QT::setSource");

        //  Create an image importer for the file selected
    	GetGraphicsImporterForFile(&theFSSpec, &importer);
    	FAIL_CONDITION( importer == NULL,
    	                kFileIOError,
    	                "Could not create an importer for selected file");

    //------------------------------------------------------------------------------------
    //  Win32 platform
    //------------------------------------------------------------------------------------
    #elif URIVL_OS_WIN32

    	FSSpec			theFSSpec;

        //  Select an image file using a dialog window
        myErr = getOneFileWithPreview_(kTypeListCount, (TypeListPtr)&myTypeList, &theFSSpec, NULL);
        FAIL_CONDITION( myErr,
                        kFileIOError,
                        "Could not open dialog window with preview in _ImageReader_QT::setSource");

        //  Create an image importer for the file selected
    	GetGraphicsImporterForFile(&theFSSpec, &importer);
    	FAIL_CONDITION( importer == NULL,
    	                kFileIOError,
    	                "Could not create an importer for selected file");

    #endif

    //------------------------------------------------------------------------------------
    //  Now that we have an importer for the file, we just read from it
    //------------------------------------------------------------------------------------
    RasterImage* img = simpleGetImageFromImporter_(importer);
    CloseComponent(importer);
    
    return img;
}


RasterImage* _ImageReader_QT::readImage_(const char* filePath, bool readUpsideDown)
{
	// make sure we have something for a path
	FAIL_CONDITION( (filePath == NULL) || (strlen(filePath) == 0),
    	            kFileNullPathError,
    	            "Null file path error in _ImageReader_QT::readImage_");

	GraphicsImportComponent importer = NULL;
	OSErr   myErr = noErr;
	
	#if URIVL_OS_MACOS
	
		FSSpec theFSSpec;
		//  get FSSpec from filepath
		myErr = getFSSpecIn_(filePath, &theFSSpec);
		
		FAIL_CONDITION( myErr,
					   kFileIOError,
					   "Failed to find file in _ImageReader_QT::readImage");
		
		//  Create an image importer for the file selected
		GetGraphicsImporterForFile(&theFSSpec, &importer);
		FAIL_CONDITION( importer == NULL,
						kFileIOError,
						"Could not create an importer for selected file in _ImageReader_QT::readImage");
	
	
	//------------------------------------------------------------------------------------
	//  The Windows code is based on the ASCIIMoviePlayerSample code
	//------------------------------------------------------------------------------------
	#elif URIVL_OS_WIN32 
	
		FSSpec theFSSpec;
		short  resRefNum = -1;
		
		char    zePath[256];
		strcpy(zePath, filePath);
		myErr = NativePathNameToFSSpec(zePath, &theFSSpec, kErrorIfFileNotFound /* flags */);
		FAIL_CONDITION( myErr,
					   kFileIOError,
					   "NativePathNameToFSSpec failed in _ImageReader_QT::readImage");
		
		//  Create an image importer for the file selected
		GetGraphicsImporterForFile(&theFSSpec, &importer);
		FAIL_CONDITION( importer == NULL,
					   kFileIOError,
					   "Could not create an importer for the selected file in _ImageReader_QT::readImage");
	
	#else   
		#error  I should not be ending up here
	#endif					

	//------------------------------------------------------------------------------------
	//  Now that we have an importer for the file, we just read from it
	//------------------------------------------------------------------------------------
	RasterImage* img = simpleGetImageFromImporter_(importer);
	CloseComponent(importer);
	
	return img;
}


RasterImage* _ImageReader_QT::getFrame_(bool readUpsideDown)
{
	FAIL_CONDITION( filePath_ == NULL,
					kVideoSourceNotSetError,
					"Source file path not defined prior to call to _ImageReader_QT::getFrame_");

	return readImage_(filePath_, readUpsideDown);
}



void _ImageReader_QT::getFrame_(RasterImage* imgOut, bool readUpsideDown)
{
	FAIL_CONDITION( filePath_ == NULL,
					kVideoSourceNotSetError,
					"Source file path not defined prior to call to _ImageReader_QT::getFrame_");

	try
	{
		RasterImage* imgTemp = readImage_(filePath_, readUpsideDown);
		imgTemp->copyInto(imgOut);
		delete imgTemp;
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by _ImageReader_QT::getFrame_");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kNoCode,
						"Unexpected error encountered in _ImageReader_QT::getFrame_");
	}
}




#if 0
#pragma mark -
#endif

//////////
//
// QTGraph_ShowImageFromFile
// Open the specified image file in a window on the screen.
//
//////////

RasterImage* uriVL::simpleGetImageFromImporter_(GraphicsImportComponent importer)
{	
	Rect						rect;
	RasterImage                 *img = NULL;
    OSErr                       err = noErr;

    //----------------------------------------------------------------------
    /// Fix the rectangle of the importer (origin at (0, 0)
    //----------------------------------------------------------------------
	GraphicsImportGetNaturalBounds(importer, &rect);
	MacOffsetRect(&rect, static_cast<short>(-rect.left), static_cast<short>(-rect.top));
	
    //----------------------------------------------------------------------
	//  Get from the importer a description record for the image
    //----------------------------------------------------------------------
    ImageDescriptionHandle desc = NULL;
    err = GraphicsImportGetImageDescription( importer, &desc ); 
	FAIL_CONDITION( (err!= noErr) || (desc == NULL) || (*desc == NULL),
	                kFileIOError,
	                "Could not get a description for the selected file");
    
    //----------------------------------------------------------------------
    //  Extract fields from the record
    //----------------------------------------------------------------------
    int nbCols = (*desc)->width;
    int nbRows = (*desc)->height;
    int pixelDepth = (*desc)->depth;
    int clutID = (*desc)->clutID;
    
    //----------------------------------------------------------------------
    //  Allocate the appropriate raster image
    //----------------------------------------------------------------------
    switch (pixelDepth) 
    {
        //  binary image
        case 1: {
            //CTabHandle colorTable = NULL;
            //err = GetImageDescriptionCTable(desc, &colorTable);
            //DisposeCTable( colorTable );
            FAIL_CONDITION( true,
                            kUnsupportedImageFormatError,
                            "Not ready yet to ready binary images");
            }
            break;
            
        //  8-bit gray-level image
        case 40: 
            //  so far we only handle "normal" gray-level images
            FAIL_CONDITION( (clutID != -1) && (clutID != 40),
                            kUnsupportedImageFormatError,
                            "Not ready yet to ready binary images");
            img = new RasterImage_gray(nbRows, nbCols);
            //  set importer to invert grays
            err = GraphicsImportSetGraphicsMode(importer, notSrcCopy, NULL);
            break;
            
        //  16-bit color image
        case 16: {
            //CTabHandle colorTable = NULL;
            //err = GetImageDescriptionCTable(desc, &colorTable);
            //DisposeCTable( colorTable );
            FAIL_CONDITION( true,
                            kUnsupportedImageFormatError,
                            "Not ready yet to ready binary images");
            }
            break;
            
        //  24-bit color image and 32-bit color images both are read into
        //  a RasterImage_RGBa object
        case 24:
        case 32:
            img = new RasterImage_RGBa(nbRows, nbCols);
            break;

        //  Other formats not handled yet
        default:
            FAIL_CONDITION( true,
                            kUnsupportedImageFormatError,
                            "Image format not supported yet.");
    }

    //----------------------------------------------------------------------
    //  And now we read the image into the new RasterImage's graphworld
    //----------------------------------------------------------------------
    img->setAsImporterGraphWorld(importer);
    // draw the image
    err = GraphicsImportDraw(importer);
    // close the importer instance
    CloseComponent(importer);

    //  let's not forget to set the image's valid data rect before returning it
    img->setValidRect(0, 0, nbCols, nbRows);
    return img;		
}


#if URIVL_OS_MACOS

	OSErr getFSSpecIn_(const char* unixPath, FSSpec* theFSSpec)
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

#endif // use URIVL_USE_QUICKTIME
