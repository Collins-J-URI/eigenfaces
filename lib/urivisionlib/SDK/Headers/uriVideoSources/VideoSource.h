/*  NAME:
        VideoSource.h

    DESCRIPTION:
        VideoSource public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VIDEO_SOURCE_H
#define URIVL_VIDEO_SOURCE_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"


namespace uriVL
{

	//=============================================================================
	//      Some place holder data types needed by non-QT image reader/writer
	//-----------------------------------------------------------------------------

	#if !URIVL_USE_QUICKTIME

		/** Placeholder until we define the FSSpec type (if ever)
		 */
		typedef void*	FSSpec; 

		/** Placeholder until we define the OSErr type (if ever)
		 */
		typedef int		OSErr;
		
	#endif


	/**	VideoSource pure virtual parent class.
	 *	All sources of images (live CCTV camera, FireWire camera, analog frame grabber,
	 *	movie file, image file, folder of image files or image stack) are implemented
	 *	as subclasses of this class, and provide therefore the same basic interface.
	 *
	 */
    class URIVL_EXPORT VideoSource
    {
        public:

			/**	Default constructor
			 */
            VideoSource(void);

			/**	Destructor (frees the video source)
			 */
            virtual ~VideoSource(void);

            /**	Select the video source via a dialog window
             */
            virtual void setSource(void) = 0;

			/**	Get an image frame from the selected input
			 *
			 *	@return		RasterImage object containing the image read
			 */
            virtual RasterImage* getFrame(void) = 0;

			/**	Get an image frame from the selected input
			 *
			 *	@param	img			RasterImage object into which the frame should be read
			 */
            virtual void getFrame(RasterImage* img) = 0;

			/**	Returns the preselected movie's width (-1 if no movie was selected)
			 *
			 *	@return		the preselected movie's width (-1 if no movie was selected)
			 */
			 int getWidth(void);

			/**	Returns the preselected movie's height (-1 if no movie was selected)
			 *
			 *	@return		the preselected movie's height (-1 if no movie was selected)
			 */
			int getHeight(void);
			
		protected:
				
			/**	This function is a bit of a hack.  One problem with subclasses of RasterImage that
			 *	have redundant storage (e.g. RasterImage_HSV) is that we must make sure that we
			 *	keep track of which raster stores the reference.  This function should be called by
			 *	any subclass of VideoSource.  It ensures that the raster that got read into is
			 *	marked as reference and modified.
			 *	@param	img		the RasterImage object whose status needs updating
			 */
			static void updateImageRasterStatus_(RasterImage* img);


    };

    #if URIVL_USE_QUICKTIME
        //------------------------------------------------------------------------------
        // Prototypes for the functions of Apple's crap "SDK"
        //------------------------------------------------------------------------------

        typedef const OSTypePtr     TypeListPtr;

        #if URIVL_OS_MACOS    
            typedef MenuHandle			MenuReference;
            typedef WindowPtr			WindowReference;
            typedef NavObjectFilterUPP	QTFrameFileFilterUPP;
        #elif URIVL_OS_WIN32
        	typedef HMENU				MenuReference;
        	typedef HWND				WindowReference;
        	typedef FileFilterUPP		QTFrameFileFilterUPP;

        #endif

        OSErr getOneFileWithPreview_(short theNumTypes, TypeListPtr theTypeList,
                                     FSSpecPtr theFSSpecPtr, void *theFilterProc);

        #if URIVL_OS_MACOS
            Handle createOpenHandle_(OSType theApplicationSignature, short theNumTypes, 
                                     TypeListPtr theTypeList);

            pascal void handleNavEvent_(NavEventCallbackMessage theCallBackSelector, 
                                        NavCBRecPtr theCallBackParms, void *theCallBackUD);
        #elif URIVL_OS_WIN32

        #endif
        
    #endif  //  URIVL_USE_QUICKTIME
}

#endif  //  VIDEO_SOURCE_H
