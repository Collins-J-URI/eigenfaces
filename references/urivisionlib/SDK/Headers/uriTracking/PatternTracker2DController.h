/*  NAME:
        PatternTracker2DController.h
 
    DESCRIPTION:
        PatternTracker2DController public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_TRACKER_2D_CONTROLLER_H
#define URIVL_PATTERN_TRACKER_2D_CONTROLLER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "PatternTracker2D.h"
#include "BackgroundRemover_dynamic_MW.h"
#include "Tracker2DController.h"
#include "PatternMatcher2D_corr.h"


namespace uriVL
{
	//---------------------------------------------------------------------
	//	class definition
	//---------------------------------------------------------------------
	class URIVL_EXPORT PatternTracker2DController : public Tracker2DController
	{
		typedef enum ScaleType
		{
			NORMALIZE = 0,
			SCALE
		} ScaleType;
		
		friend class PatternTracker2D;
		
		public:
		
			/**
			 *  Constructor
			 *  Creates a PatternTracker2DController
			 */
			PatternTracker2DController(RasterImage* firstBkgd);
										  
			/**
			 *	Destructor
			 */								  
			virtual ~PatternTracker2DController(void);
			
			RasterImage_gray* getDifferImg(void);
			
			RasterImage_gray* getDifferImgHue(void);
			
			RasterImage	*getCurrentBackground(void);
			/**
			 *	Begin control loop
			 *	find all blobs, (if some one new, send a tracker), 
			 *	update background using dynamic backgroundremover, track every blob
			 *				  
			 */								  
			void action(RasterImage* img);

			/**
			 *	Get current list<PatternTracker2D*>
			 */
			list<PatternTracker2D*> *getCurrTrackerList(void);

			/**
			 *	scale or normalize patterImg
			 */
			static void scaleImage(BackgroundRemover_dynamic_MW *remover,
									RasterImage* srcImg,
									RasterImage* destImg,
									ScaleType scaleType);

			/**
			 *	return upperBcknd_
			 */
			RasterImage* getUpperBackground(void);

			/**
			 *	return &newPatternList_
			 */
			Pattern2DList *getNewPatternList(void);

			/**
			 *	return &newPatternRectList_
			 */
			list<ImageRect* > *getNewPatternRectList(void);

			/**
			 *	return &newBlobList_
			 */
			BlobList *getNewBlobList(void);


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
	
			/**	differenced gray image in hue (comparison of current image with background)
			 */
			RasterImage_gray			*differImgHue_;
			
			/**	upper background as RasterImage
			 */
			RasterImage                 *upperBcknd_;
			
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

			/**	list of PatternTracker2D
			 */
			list<PatternTracker2D*>		trackerList_;
			
			/** list of blobs' bounding rectangle
			 */
			RectList					rectList_;
			
			/**	the movie frame index
			 */
			short						frameCount_;
			
			/**	noise size used for filtering noise
			 */
			short						noiseSize_;
			
			/**	Period of motion detection
			 */
			int							motionDetectRate_;
			
			/**	period of background update
			 */
			int							bgUpdateRate_;
			
			Pattern2DList				newPatternList_;
			
			list<ImageRect* >			newPatternRectList_;
			
			BlobList					newBlobList_;
	};

}

#endif	//	PATTERN_TRACKER_2D_CONTROLLER_H
