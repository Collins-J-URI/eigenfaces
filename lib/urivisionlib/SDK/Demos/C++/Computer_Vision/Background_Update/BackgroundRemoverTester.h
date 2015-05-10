/*	NAME:
		BackgroundRemoverTester.h
	DESCRIPTION:
		BackgroundRemoverTester header
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef BACKGROUND_REMOVER_TESTER_H
#define BACKGROUND_REMOVER_TESTER_H

#include "BackgroundRemover_dynamic_MW.h"
#include "LabelRaster.h"
#include "GrayThresholdLabeler.h"
#include "BlobFinder.h"

using namespace std;
using namespace uriVL;

namespace uriComputerVision
{
	enum BkgdTestCase
	{
		INTENSITY_RATIO,
		THRESHOLD_NO_OBJECT,
		PERCENTAGE_NO_SMOOTHING,
		PERCENTAGE_NO_OBJECT,
		ALL_OBJECT,
		MISS_SOME_OBJECT
	};

	class URIVL_EXPORT BackgroundRemoverTester
	{
		public:
		
			/**
			 *	Constructor
			 */
			BackgroundRemoverTester(int nbRects, RasterImage *firstBackground);
			
			/**
			 *	Destructor
			 */
			virtual ~BackgroundRemoverTester(void);
			
			/**
			 *	testing
			 */
			void test(RasterImage *img, BkgdTestCase testCase_, int rectStep = 1);

			/**
			 *	get the difference of 2 images as a 2-D array int data
			 */
			int **getDiffData(void);
			
			int ***getDiffData3D(void);

			/**
			 *	get the difference of 2 images as a RasterImage_gray
			 */
			RasterImage_gray *getDiffImg(void);
			
			/**
			 *	get the total intensity of the current image
			 */
			double getTotalIntensity(void);
			
			/**
			 *	get the percentage of covered (by objects) area
			 */
			float getCoveredPercent(void);
			
			/**
			 *	get the percentage of covered (by objects) area
			 *	with only fitting (no smoothing)
			 */
			float getCoveredPctNoSmooth(void);
			
			/**
			 *	get the current background Image within remover_, which is
			 *	a BackgroundRemover_dynamic_MW
			 */
			RasterImage *getBackground(void);
			
			/**
			 *	get a right threshold such that we can find no blobs
			 *	in thresholded diffImg_, when there is not any in real image
			 */
			int getRightThreshold(void);
			
			/**
			 *	get the difference of 2 images as a RasterImage_gray
			 */
			int getDimX(void);
			
			/**
			 *	get the difference of 2 images as a RasterImage_gray
			 */
			int getDimY(void);
			
			/**
			 *	get xUnit_ in backgroundRemover
			 */
			int getXUnit(void);
			
			/**
			 *	get yUnit_ in backgroundRemover
			 */
			int getYUnit(void);
			
			/**
			 *	get intensity ratio of currentImage and firstBackground
			 */
			float **getRatio(void);
			
			RasterImage_gray_F *getRatioAsRasterImage(void);
			
			RasterImage_gray_F *getFittedRatioAsRasterImage(void);
		
			float *getCoveredPercNoSmooth(void);

			int getNbIncreaseRect(void);
			
			RasterImage	*getFirstBackground(void);
			
		
		protected:
		
			RasterImage	*firstBackground_;
			
			BackgroundRemover_dynamic_MW *remover_;
			
			int frameCount_;
			
			int nbRects_;
			
			int rectStep_;
			
			RectList rectList_;
			
			int top_;
			
			int left_;
			
			int height_;
			
			int width_;
			
			int rectSize_;
			
			RasterImage *estimateImg_;
			
			/**
			 *	difference as a RasterImage_gray
			 */
			RasterImage_gray *diffImg_;

			/**
			 *	difference as a 2-D array of int data
			 */
			int **diffData_;
			
			int ***diffData3D_;
			
			int dimX_;
			
			int dimY_;
			
			/**
			 *	total amount of rectangles divided in remover_
			 */
			int totalRects_;
			
			
			GrayThresholdLabeler		*thresholder_;

			ColorSegParams				*grayParams_;

			LabelRaster					*label_;
			
			RasterImage_gray_F			*grayRatio_;
			
			RasterImage_gray_F			*fittedGrayRatio_;

			char 						*target_;

			long						blobMinSize_;

			BlobFinder					*blobFinder_;
	
			BlobList 					*blobList_;
			
			int							minThreshold_;
			
			int							noiseSize_;

			BkgdTestCase testCase_;
			
			bool objectMatched_;
			
			list<bool> objectDetectList_;
			
			int nbObjectMissed_;
			
			// array of ratio value
			float	**intensityRatio_;
			
			// array of fitted(using model) ratio value
			//float	**fittedIntensityRatio_;
			
			// array of firstBG squares' internal intensity
			float	**intensityBg_;
			
			// array of currImg squares' internal intensity
			float	**intensityImg_;
			
			// array of fittedCurrImg(using model) squares' internal intensity
			//float	**fittedIntensityImg_;
			
			// total intensity of the current image
			double totalIntensity_;
			
			// percentage of covered (by objects) area
			float coveredPercent_;

			// percentage of covered (by objects) area
			// with only fitting (no smoothing)
			float coveredPctNoSmooth_;
	
			// nb of increasing rectangles in each frame
			int nbIncreaseRect_;
			
			// array with size nbIncreseRect_
			float *coveredPercNoSmooth_;

			unsigned char **firstBg2D_;



			/**
			 *	BkgdTestCase : INTENSITY_RATIO
			 */
			void test0_(RasterImage *img);
			
			/**
			 *	BkgdTestCase : THRESHOLD_NO_OBJECT
			 */
			void test1_(RasterImage *img);
			
			/**
			 *	BkgdTestCase : PERCENTAGE_NO_SMOOTHING
			 */
			void test21_(RasterImage *img);

			/**
			 *	BkgdTestCase : PERCENTAGE_NO_OBJECT
			 */
			void test22_(RasterImage *img);
			
			/**
			 *	BkgdTestCase : ALL_OBJECT
			 */
			void test3_(RasterImage *img);
			
			/**
			 *	BkgdTestCase : MISS_SOME_OBJECT
			 */
			void test4_(RasterImage *img);
			

	};
}

#endif	// BACKGROUND_REMOVER_TESTER_H
