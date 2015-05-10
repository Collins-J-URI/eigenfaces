/*  NAME:
        PatternTracker2DController.cpp
 
    DESCRIPTION:
        PatternTracker2DController source file.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "PatternTracker2DController.h"
#include "ImageWriter.h"
#include "RasterImage_HSV.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------
PatternTracker2DController::PatternTracker2DController(RasterImage* firstBkgd)
						: Tracker2DController()
{
	remover_		= new BackgroundRemover_dynamic_MW();
	remover_->setBackground(firstBkgd);
	differImg_		= new RasterImage_gray(firstBkgd->getBoundRect());
	differImg_->setValidRect(firstBkgd->getValidRect());
	thresholder_	= new GrayThresholdLabeler();
	grayParams_		= new ColorSegParams;
	label_			= new LabelRaster(firstBkgd->getValidRect());
	blobFinder_		= new BlobFinder();
	target_			= new char[1];
	detector_		= NULL;
	blobMinSize_	= 220;//160//1200;
	noiseSize_		= 220;//1200;
	frameCount_		= 0;
	bgUpdateRate_	= 1;
	motionDetectRate_ = 15;//30;//6;

	differImgHue_	= new RasterImage_gray(firstBkgd->getBoundRect());
	differImgHue_->setValidRect(firstBkgd->getValidRect());
	
	switch (firstBkgd->getBaseRasterType())
	{
	    case kGrayRaster:
	        upperBcknd_ = new RasterImage_gray(firstBkgd);
	        break;
	        
	    case kRGBa32Raster:
	        upperBcknd_ = new RasterImage_RGBa(firstBkgd);
	        //upperBcknd_ = new RasterImage_HSV(firstBkgd);
	        break;
	        
	    default:
	        FAIL_CONDITION( true,
	                        kTrackingError,
	                        "Only gray-level and RGBA images should be used as background");
	}


    grayParams_->nbModes	= 1;
    grayParams_->colorMode	= false;
    grayParams_->colorReg	= NULL;
    grayParams_->grayReg	= new GrayRegion[grayParams_->nbModes];
    grayParams_->fillMode	= kOrigColorFill;
    grayParams_->grayReg[0].labelStr =  "small difference";
    grayParams_->grayReg[0].vMin = 25;//20;//15;//12;//17;//15;
    grayParams_->grayReg[0].vMax = 255;
    grayParams_->grayReg[0].fillColor[0] = (unsigned char) 255;
    grayParams_->grayReg[0].fillColor[1] = (unsigned char) 0;
    grayParams_->grayReg[0].fillColor[2] = (unsigned char) 0;
    grayParams_->grayReg[0].label = (char) 1;			//  must begin from n>0
    target_[0] = grayParams_->grayReg[0].label;

	blobList_	= new BlobList[1];
}

PatternTracker2DController::~PatternTracker2DController(void)
{
	delete remover_;
	delete differImg_;
	delete differImgHue_;
	delete thresholder_;
	delete grayParams_;
	delete label_;
	delete blobFinder_;
	delete []target_;
	delete []blobList_;
}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

RasterImage_gray* PatternTracker2DController::getDifferImg(void)
{
	return differImg_;
}

RasterImage_gray* PatternTracker2DController::getDifferImgHue(void)
{
	return differImgHue_;
}

RasterImage* PatternTracker2DController::getCurrentBackground(void)
{
	return remover_->getBackground();
}

RasterImage* PatternTracker2DController::getUpperBackground(void)
{
	return upperBcknd_;
}

Pattern2DList *PatternTracker2DController::getNewPatternList(void)
{
	return &newPatternList_;
}

list<ImageRect* > *PatternTracker2DController::getNewPatternRectList(void)
{
	return &newPatternRectList_;
}

BlobList *PatternTracker2DController::getNewBlobList(void)
{
	return &newBlobList_;
}

PatternTracker2DList *PatternTracker2DController::getCurrTrackerList(void)
{
	return &trackerList_;
}

void PatternTracker2DController::action(RasterImage* img)
{

	PatternTracker2DList::iterator	trackerIter;
// 	ImagePoint_F			*center				= NULL; // unused variable. -CJC
	const ImageRect* imgRect			= img->getValidRect(),
					*blobRect			= NULL;
	int		origX, origY, wd, ht;
	long	patternSize;
	bool	detectNew		= false;
// 	ImagePoint* blobOrig	= new ImagePoint(0, 0);  // unused variable. -CJC
	
// 	int i, j;  // unused variable. -CJC
// 	int imgWd = imgRect->getWidth(),  // unused variable. -CJC
// 		imgHt = imgRect->getHeight();  // unused variable. -CJC
// 	int hueThreshold = 10;//3;  // unused variable. -CJC
	
	//newPatternList_.clear();
	BlobList::iterator blobIter = newBlobList_.begin();
	list<ImageRect* >::iterator rectIter = newPatternRectList_.begin();
	for ( ; blobIter != newBlobList_.end(); blobIter++)
		delete (*blobIter);
	for ( ; rectIter != newPatternRectList_.end(); rectIter++)
		delete (*rectIter);
	newBlobList_.clear();
	newPatternRectList_.clear();
	
	if (frameCount_ % motionDetectRate_ == 0)
		detectNew = true;

    RasterImage* currentBckgnd = remover_->getBackground();
    currentBckgnd->copyInto(upperBcknd_);
    
	//---------------------------------------------------
	//	continuous tracking
	//---------------------------------------------------
	if (!trackerList_.empty())
	{
//cout << "Frame " << frameCount_ << endl;

		trackerIter	= trackerList_.begin();

		if ((*trackerIter) != NULL)
		{
			for ( ; trackerIter!=trackerList_.end(); trackerIter++)
			{
				PatternTrackingState 	*trackingState = (PatternTrackingState *) (*trackerIter)->track(img,remover_);

				//	if the tracker lost object, kill it 
				if (trackingState->getTargetGone())
				{
					delete *trackerIter;
					trackerList_.erase(trackerIter);
				}
					
				else
				{
					Pattern2D* targetPattern = (*trackerIter)->pattern_;


//  print info about that pattern's blob
LineSegDeque *segDeque = targetPattern->getBlob()->getSegDeque();
cout << "\t-------Blob-out of track----------"<<endl;
deque< list<HorizontalSegment*> *>::iterator lineIter, lastLine = segDeque->end();
for (lineIter = segDeque->begin(); lineIter != lastLine; lineIter++)
{
    cout << "\t\ty = " << (*((*lineIter)->begin()))->y << ": ";
    list<HorizontalSegment*>::iterator  segIter, lastSeg = (*lineIter)->end();
    for (segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
    {
        HorizontalSegment   *theSeg;
        theSeg = (*segIter);
        cout << "<" << theSeg->leftX << ", " << theSeg->rightX << ">  ";
    }
    cout << endl << flush;
}


					const ImageRect* rc = targetPattern->getBlob()->getValidRect();
				    ImagePoint_F    *targetPos = trackingState->getPos();
				    ImagePoint      targetPos_i(static_cast<int>(round(targetPos->getX() - rc->getLeft())), 
				                                static_cast<int>(round(targetPos->getY() - rc->getTop())));
				    
//					blobRect	= (*trackerIter)->pattern_->getValidRect();
//					blobOrig->setCoordinates(blobRect->getLeft(), blobRect->getTop());
//					(*trackerIter)->pattern_->writeInto(upperBcknd_, blobOrig);
//					upperBcknd_->overlay((*trackerIter)->pattern_->getPattern(), &targetPos_i);
					targetPattern->writeInto(upperBcknd_, &targetPos_i);
//					(*trackerIter)->pattern_->writeInto(remover_->getBackground(), blobOrig);
				}
			}
		}
	}

/*
ImageWriter	*myWriter = new ImageWriter("newUpper.tga");
myWriter->outputFrame(upperBcknd_);
delete myWriter;
*/
		
	//---------------------------------------------------
	//	detect new objects every motionDetectRate_ frame
	//---------------------------------------------------
	if (detectNew)
	{
		//	do background subtraction, thresholding
		ImageArithmetics::difference(upperBcknd_, img, differImg_, MAX_DIFFERENCE);
		/*				
		BackgroundRemover::difference(upperBcknd_, img, differImgHue_, HUE_DIFFERENCE);

		unsigned char	**diff2D = differImg_->getShiftedRaster2D(),
						**diff2DHue = differImgHue_->getShiftedRaster2D(),
						**bkgdVal2D = ((RasterImage_HSV* )upperBcknd_)->getShiftedVal2D(),
						**imgVal2D = ((RasterImage_HSV* )img)->getShiftedVal2D();
						
		for (i=0; i<imgHt; i++)
			for (j=0; j<imgWd; j++)
			{
				if (diff2DHue[i][j] < hueThreshold
					)//&& imgVal2D[i][j] < bkgdVal2D[i][j])	// regions (shadows) that are darker in img than in bkgd
					diff2D[i][j] = diff2DHue[i][j];
			}
		*/
		
		thresholder_->applyInto(differImg_, grayParams_, label_);
		blobFinder_->findAllBlobs(label_, grayParams_->nbModes, target_, blobMinSize_, blobList_, true);
		
		blobIter	= blobList_[0].begin();
		
		int sz = static_cast<int>(blobList_[0].size());
		cout << "blobList_[0] size: " << sz << endl;
		
		//	create tracker for each detected blob
		for ( ; blobIter!=blobList_[0].end(); blobIter++)
		{
			//	reject small noise patterns
			if ((*blobIter)->getNbPixels() > noiseSize_)
			{		
				blobRect		= (*blobIter)->getBoundRect();	
				origX			= blobRect->getLeft();
				origY			= blobRect->getTop();
				wd				= blobRect->getWidth();
				ht				= blobRect->getHeight();
				patternSize		= wd * ht;
				PatternTrackingState 	trackingState(origX, origY, patternSize);
				
				//	grab the target pattern
				///(*blobIter)->translate(-origX, -origY);
				Pattern2D	patt(*blobIter, img);
				///Pattern2D	*patt = new Pattern2D(blobRect, img);
				//newPatternList_.push_back(patt);
				newBlobList_.push_back(*blobIter);
				newPatternRectList_.push_back(new ImageRect(blobRect));
				
//ImageWriter	*myWriter = new ImageWriter("mw.tga");
//myWriter->outputFrame(patt->getPattern());
//delete myWriter;
//exit(0);


				//	push into trackerList_
				PatternTracker2D *newTracker = new PatternTracker2D(&patt, imgRect, -0.6f,
																	0.f, 0.f, &trackingState);
				trackerList_.push_back(newTracker);
			}
		}
		
		//	push newObject's rectangle into rectList_		
		for (blobIter=blobList_[0].begin(); blobIter!=blobList_[0].end(); blobIter++)
			rectList_.push_back(new ImageRect(origX, origY, wd, ht));
			
		for (blobIter != blobList_[0].begin(); blobIter != blobList_[0].end(); blobIter++)
			delete (*blobIter);
		blobList_[0].clear();	
	}
	
	//	update background
	if (rectList_.empty())
		rectList_.push_back(NULL);
//	if (frameCount_ % bgUpdateRate_ == 0)
//		remover_->updateBackground(img, rectList_);			
//	for (rectIter != rectList_.begin(); rectIter != rectList_.end(); rectIter++)
//		delete (*rectIter);
//	rectList_.clear();
	frameCount_++;
}


void PatternTracker2DController::scaleImage(BackgroundRemover_dynamic_MW *remover,
											RasterImage* srcImg, RasterImage* destImg,
											ScaleType scaleType)
{
    FAIL_CONDITION(	srcImg == NULL,
                    kNullRasterImageError,
                    "NULL RasterImage srcImg passed as parameter to Pattern2DController");
    FAIL_CONDITION(	destImg == NULL,
                    kNullRasterImageError,
                    "NULL RasterImage destImg passed as parameter to Pattern2DController");

	const ImageRect	*rect = srcImg->getBoundRect();
	const int	iLow = rect->getTop(),
				iHigh = rect->getBottom(),
				jLow = rect->getLeft(),
				jHigh = rect->getRight();
	
	const unsigned char	*const* sIm	= srcImg->getShiftedRaster2D();
	unsigned char** dIm	= destImg->getShiftedRaster2D(R_W_ACCESS);
	double	**a = remover->getCoefficients();
	double a0[3] = {a[0][0], a[0][1], a[0][2]},
		   a1[3] = {a[1][0], a[1][1], a[1][2]},
		   a2[3] = {a[2][0], a[2][1], a[2][2]},
		   a3[3] = {a[3][0], a[3][1], a[3][2]},
		   a4[3] = {a[4][0], a[4][1], a[4][2]},
		   a5[3] = {a[5][0], a[5][1], a[5][2]},
		   a6[3] = {a[6][0], a[6][1], a[6][2]},
		   a7[3] = {a[7][0], a[7][1], a[7][2]},
		   a8[3] = {a[8][0], a[8][1], a[8][2]},
		   a9[3] = {a[9][0], a[9][1], a[9][2]};
	
	for(int i=iLow; i<=iHigh; i++)		// update every pixel of background_ using the model
	{
		double	i2 = i*i,
				i3 = i2*i;
			
		for(int j=jLow; j<=jHigh; j++)
		{
			double	j2 = j*j,
					j3 = j2*j,
					ixj = i*j;
					
			double	scaleFactorR = ((a0[0]*i3 + a1[0]*j3
									+ a2[0]*i2*j + a3[0]*i*j2)
									+ (a4[0]*i2 + a5[0]*j2
									+ a6[0]*ixj)
									+ (a7[0]*i + a8[0]*j))+ a9[0];
			double	scaleFactorG = ((a0[1]*i3 + a1[1]*j3
									+ a2[1]*i2*j + a3[1]*i*j2)
									+ (a4[1]*i2 + a5[1]*j2
									+ a6[1]*ixj)
									+ (a7[1]*i + a8[1]*j)) + a9[1];
			double	scaleFactorB = ((a0[2]*i3 + a1[2]*j3
									+ a2[2]*i2*j + a3[2]*i*j2)
									+ (a4[2]*i2 + a5[2]*j2
									+ a6[2]*ixj)
									+ (a7[2]*i + a8[2]*j)) + a9[2];
	
			int rEntry = 4*j;
			int gEntry = rEntry+1;
			int bEntry = gEntry+1;
			int alphaEntry = bEntry+1;

			if(scaleType == NORMALIZE)
			{
				scaleFactorR = 1/scaleFactorR;
				scaleFactorG = 1/scaleFactorG;
				scaleFactorB = 1/scaleFactorB;
			}
			
			double	tempRed		= (scaleFactorR * sIm[i][rEntry]),
					tempGreen 	= (scaleFactorG * sIm[i][gEntry]),   
					tempBlue 	= (scaleFactorB * sIm[i][bEntry]);
													  
			if(tempRed<=255)
				if(tempRed>0)
					dIm[i][rEntry] = (unsigned char)tempRed;
				else
					dIm[i][rEntry] = 0;
			else
				dIm[i][rEntry] = (unsigned char)255;

			if(tempGreen<=255)
				if(tempGreen>0)
					dIm[i][gEntry] = (unsigned char)tempGreen;
				else
					dIm[i][gEntry] = 0;
			else
				dIm[i][gEntry] = (unsigned char)255;
				
			if(tempBlue<=255)
				if(tempBlue>0)
					dIm[i][bEntry] = (unsigned char)tempBlue;
				else
					dIm[i][bEntry] = 0;
			else
				dIm[i][bEntry] = (unsigned char)255;

			dIm[i][alphaEntry] = (unsigned char) 255;		    
		}
	}
	
}			


//----------------------------------------------------------
//  private and protected functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

