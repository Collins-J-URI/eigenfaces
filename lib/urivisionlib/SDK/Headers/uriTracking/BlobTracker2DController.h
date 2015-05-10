/*  NAME:
        BlobTracker2DController.h
 
    DESCRIPTION:
        BlobTracker2DController public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_BLOB_TRACKER_2D_CONTROLLER_H
#define URIVL_BLOB_TRACKER_2D_CONTROLLER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "BlobTracker2D.h"
#include "BackgroundRemover_dynamic_MW.h"
#include "Tracker2DController.h"

namespace uriVL
{
	/*
	//---------------------------------------------------------------------
	//	data structures
	//---------------------------------------------------------------------
	typedef struct NewBlob
	{
		Blob* blob;
		short count;
	} NewBlob;
	
	typedef std::list<NewBlob* > NewBlobList;
	*/
	//---------------------------------------------------------------------
	//	class definition
	//---------------------------------------------------------------------
	class URIVL_EXPORT BlobTracker2DController : public Tracker2DController
	{
		public:
		
			/**
			 *  Constructor
			 *  Creates a BlobTracker2DController
			 */
			BlobTracker2DController(RasterImage* firstBkgd);
										  
			/**
			 *	Destructor
			 */								  
			virtual ~BlobTracker2DController(void);
			
			/**
			 *	@return	RasterImage_gray	differenced gray Image
			 */
			RasterImage_gray* getDifferImg(void);
			
			/**
			 *	@return	RaterImage		current estimated background
			 */
			RasterImage	*getCurrentBackground(void);

			/**
			 *	Begin control loop
			 *	find all blobs, (if some one new, send a tracker), 
			 *	update background using dynamic backgroundremover, track every blob
			 *				  
			 */								  
			void action(RasterImage* img);

			/**
			 *	Get current BlobTracker2DList
			 */
			BlobTracker2DList getCurrTrackerList(void);



		protected:
		
			/** @see BackgroundRemover_dynamic_MW
			 */
			BackgroundRemover_dynamic_MW *remover_;
			
			/** feature detector
			 */
			FeatureDetector				*detector_;
	
			/**	differenced gray image (comparison of current image with background)
			 */
			RasterImage_gray			*differImg_;
			
			/**	thresholder
			 */
			GrayThresholdLabeler		*thresholder_;

			/** ColorSegParams (for thresholding)
			 */
			ColorSegParams				*grayParams_;

			/** LabelRaster (for thresholding)
			 */
			LabelRaster					*label_;

			/** result of BlobFinder
			 */
			char 						*target_;

			/** pointed minimum size of blob used for BlobFinder
			 */
			long						blobMinSize_;

			/** BlobFinder
			 */
			BlobFinder					*blobFinder_;
	
			/** result of BlobFinder
			 */
			BlobList 					*blobList_;

			/** list of BlobTracker2D
			 */
			BlobTracker2DList			trackerList_;
			
			/** list of blobs' bounding rectangle
			 */
			ConstRectList				rectList_;
			
			/**	the movie frame index
			 */
			short						frameCount_;
			
			/**	noise size used for filtering noise
			 */
			short						noiseSize_;
			
			/**	Period of new object detection
			 */
			int							PeriodOfDetection_;
	};

}

#endif	//	BLOB_TRACKER_2D_CONTROLLER_H
