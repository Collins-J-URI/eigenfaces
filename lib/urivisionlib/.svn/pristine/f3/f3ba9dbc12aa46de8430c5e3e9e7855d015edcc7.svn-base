/*  NAME:
        VideoSource.cpp

    DESCRIPTION:
        implementation of the uriVisionLib VideoSource class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "VideoSource.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"

using namespace uriVL;

//----------------------------------------------------------------------------------
//-------------------------------Class implementation-------------------------------
//


VideoSource::VideoSource(void)
{
}

VideoSource::~VideoSource(void)
{
}

void VideoSource::updateImageRasterStatus_(RasterImage* img)
{
//wlcfixme this was done as a part of FreeImage reader and writer addition
#if URIVL_USE_FREEIMAGE // new way that lets a float raster be reference if it exists
	if (img->hasFloatRaster())
	{
		img->setFloatRasterAsReference_();
	}
	else
	{
		int nRasterType = img->getBaseRasterType( );
		// DO NOT set gworld as reference for binary
		if (nRasterType != kBinaryRaster)
		{
			img->setGraphWorldAsReference_();
		}
	}
	img->setReferenceHasChanged_(true);
#else // original way that forces graph world raster to be reference
	img->setGraphWorldAsReference_();
	img->setReferenceHasChanged_(true);
#endif

}

#if 0
#pragma mark -
#endif

#if URIVL_USE_QUICKTIME

	//======================================================================================
	//  Modified pieces of Apple's crappy so-called "SDK" go here.
	//  At least now we don't need the even-worse "QT Framework".
	//======================================================================================


	// getOneFileWithPreview
	// Display the appropriate file-opening dialog box, with an optional QuickTime preview pane. 
	//  If the user selects a file, return information about it using the theFSSpecPtr parameter.
	//
	// Note that both StandardGetFilePreview and NavGetFile use the function specified by 
	// theFilterProc as a file filter. This framework always passes NULL in the theFilterProc 
	// parameter. If you use this function in your own code, keep in mind that on Windows the 
	// function specifier must be of type FileFilterUPP and on Macintosh it must be of type
	// NavObjectFilterUPP. (You can use the QTFrame_GetFileFilterUPP to create
	// a function specifier of the appropriate type.) Also keep in mind that Navigation Services 
	// expects a file filter function to return true if a file is to be displayed, while the
	// Standard File Package expects the filter to return false if a file is to be displayed.
	//
	OSErr uriVL::getOneFileWithPreview_(short theNumTypes, TypeListPtr theTypeList, 
												  FSSpecPtr theFSSpecPtr, void *theFilterProc)
	{
		OSErr   myErr = noErr;
		//
		#if URIVL_OS_WIN32
			StandardFileReply	myReply;
		#elif URIVL_OS_MACOS 
			NavReplyRecord		myReply;
			NavDialogOptions	myDialogOptions;
			NavTypeListHandle	myOpenList = NULL;
			NavEventUPP			myEventUPP = NewNavEventUPP(handleNavEvent_);
		#endif

		//------------------------------------------------------------------------------------
		//  Win32-specific code, using StandardFile functions
		//------------------------------------------------------------------------------------
		//
		#if URIVL_OS_WIN32
			// prompt the user for a file
			
			// Since MSVC seems not to mind about static_casting a void pointer, but GCC does mind
			// we have two seperate castings of our void*. -C.Allen
			#if URIVL_MINGW32
				StandardGetFilePreview((FileFilterUPP)theFilterProc, theNumTypes, 
										  (ConstSFTypeListPtr)theTypeList, &myReply);
			#else
				StandardGetFilePreview(static_cast<FileFilterUPP>(theFilterProc), theNumTypes, 
									   static_cast<ConstSFTypeListPtr>(theTypeList), &myReply);
			#endif
			if (!myReply.sfGood)
				return(userCanceledErr);
			
			// make an FSSpec record
			myErr = FSMakeFSSpec(myReply.sfFile.vRefNum, myReply.sfFile.parID, myReply.sfFile.name, 
								 theFSSpecPtr);

		//------------------------------------------------------------------------------------
		//  Mac-specific code (using File Navigation services
		//------------------------------------------------------------------------------------
		//
		#elif URIVL_OS_MACOS 
			const OSType    kApplicationSignature  = FOUR_CHAR_CODE('????');
	//        const ResType   kOpenResourceType = FOUR_CHAR_CODE('open');
			const StringPtr kApplicationName =(StringPtr)("\pExample");
			
			// specify the options for the dialog box
			NavGetDefaultDialogOptions(&myDialogOptions);
			myDialogOptions.dialogOptionFlags -= kNavNoTypePopup;
			myDialogOptions.dialogOptionFlags -= kNavAllowMultipleFiles;
			BlockMoveData(kApplicationName, myDialogOptions.clientName, kApplicationName[0] + 1);
					// create a handle to an 'open' resource
			myOpenList = (NavTypeListHandle)createOpenHandle_(kApplicationSignature, theNumTypes, 
						  theTypeList);
			if (myOpenList != NULL)
				HLock((Handle)myOpenList);
			

			// prompt the user for a file

			//jyh
			//----  Reported leaks (by Xcode) occur between these two points.  What's going on?  This is 
			//  the same code as in the MovieGWorld demo for which there is no leak report.  Most likely 
			//  the Apple code is crap (as usual) and they simply inhibited the leak report [check on that]
			//  
			myErr = NavGetFile(NULL, &myReply, &myDialogOptions, myEventUPP, NULL, 
							   (NavObjectFilterUPP)theFilterProc, myOpenList, NULL);
			if ((myErr == noErr) && myReply.validRecord) {
				AEKeyword		myKeyword;
				DescType		myActualType;
				Size			myActualSize = 0;
			//-------
			//jyh
			
				// get the FSSpec for the selected file
				if (theFSSpecPtr != NULL)
					myErr = AEGetNthPtr(&(myReply.selection), 1, typeFSS, &myKeyword, 
										  &myActualType, theFSSpecPtr, sizeof(FSSpec), 
										  &myActualSize);

				NavDisposeReply(&myReply);
			}
			
			if (myOpenList != NULL) {
				HUnlock((Handle)myOpenList);
				DisposeHandle((Handle)myOpenList);
			}
			
			DisposeNavEventUPP(myEventUPP);
		#endif
	 
		return(myErr);
	}

	#if URIVL_OS_MACOS

		//////////
		//
		// CreateOpenHandle
		// Get the 'open' resource or dynamically create a NavTypeListHandle.
		//
		//////////
		Handle uriVL::createOpenHandle_(OSType theApplicationSignature, short theNumTypes, 
												  TypeListPtr theTypeList)
		{
			Handle			myHandle = NULL;
			
			// see if we have an 'open' resource...
			myHandle = Get1Resource('open', 128);
			if ( myHandle != NULL && ResError() == noErr ) {
				DetachResource( myHandle );
				return myHandle;
			} else {
				myHandle = NULL;
			}
			
			// nope, use the passed in types and dynamically create the NavTypeList
			if (theTypeList == NULL)
				return myHandle;
			
			if (theNumTypes > 0) {
				myHandle = NewHandle(sizeof(NavTypeList) + (theNumTypes * sizeof(OSType)));
				if (myHandle != NULL) {
					NavTypeListHandle 	myOpenResHandle	= (NavTypeListHandle)myHandle;
					
					(*myOpenResHandle)->componentSignature = theApplicationSignature;
					(*myOpenResHandle)->osTypeCount = theNumTypes;
					BlockMoveData(theTypeList, (*myOpenResHandle)->osType, theNumTypes * sizeof(OSType));
				}
			}
			
			return myHandle;
		}

		//////////
		//
		// HandleNavEvent
		// A callback procedure that handles events while a Navigation Service dialog box is displayed.
		//
		//////////
		pascal void uriVL::handleNavEvent_(NavEventCallbackMessage theCallBackSelector, 
									NavCBRecPtr theCallBackParms, void *theCallBackUD)
		{
		#pragma unused(theCallBackUD)
			
			if (theCallBackSelector == kNavCBEvent) {
				switch (theCallBackParms->eventData.eventDataParms.event->what) {
					case updateEvt:
						#if URIVL_OS_MACOS
							// Handle Update Event
						#endif
						break;
					case nullEvent:
						// Handle Null Event
						break;
				}
			}
		}

	#endif  //  URIVL_OS_MACOS

#endif  //  URIVL_USE_QUICKTIME


