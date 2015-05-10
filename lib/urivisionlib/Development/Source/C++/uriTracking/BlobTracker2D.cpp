/*  NAME:
        BlobTracker2D.cpp
 
    DESCRIPTION:
        BlobTracker2D source file.
        (abstract)base class: Tracker2D
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*	version 1.1 (with introduction of Tracker2DControl)	*/

#include <iostream>
//
#include "BlobTracker2D.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

BlobTracker2D::BlobTracker2D(FeatureDetector *detector,
							 float metricX,
							 float metricY,
							 BlobTrackingState *initState,
							 //RasterImage* bkgdImg,
							 bool savePred, /* = false */
							 bool saveState /* = false */)
//	: Tracker2D(detector, metricX, metricY, initState, savePred, saveState)
{
	//thresholder_	= new GrayThresholdLabeler();
	//colorParams_	= new ColorSegParams;
	//remover_		= new BackgroundRemover_Static();
	//backgroundImg_	= bkgdImg;
	//currImg_		= NULL;
	detector_ = detector;
	currentState_ = new BlobTrackingState(initState);
	metricX_ = metricX;
	metricY_ = metricY;
	savePred_ = savePred;
	saveState_ = saveState;
	predRecord_.push_back(currentState_);
	stateRecord_.push_back(currentState_);
	failCounts_	= 0;
	blob_		= NULL;
	
	distanceRange_ = 10;
	
	/*
	colorParams_->fillMode	= kOrigColorFill;
	colorParams_->nbModes	= 1;
	colorParams_->colorMode	= false;
	colorParams_->colorReg	= NULL;
	colorParams_->grayReg	= new GrayRegion[colorParams_->nbModes];
    colorParams_->grayReg[0].labelStr =  "medium gray";
    colorParams_->grayReg[0].vMin = 5;
    colorParams_->grayReg[0].vMax = 255;
    colorParams_->grayReg[0].fillColor[0] = 0;
    colorParams_->grayReg[0].fillColor[1] = (unsigned char) 255;
    colorParams_->grayReg[0].fillColor[2] = (unsigned char) 0;
    colorParams_->grayReg[0].label = 1;
	//remover_->setBackground(bkgdImg);
	//remover_->setBackground(backgroundImg_);
	*/
}

BlobTracker2D::~BlobTracker2D(void)
{
	//delete thresholder_;
	//delete colorParams_;
	//delete remover_;
	//delete currImg_;
	delete currentState_;

	TrackingStateList::iterator predIter = predRecord_.begin();
	TrackingStateList::iterator stateIter = stateRecord_.begin();
	for ( ; predIter != predRecord_.end(); predIter++)
		delete (*predIter);
	for ( ; stateIter != stateRecord_.end(); stateIter++)
		delete (*stateIter);
	predRecord_.clear();
	stateRecord_.clear();

	delete blob_;
}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

TrackingState* BlobTracker2D::track(BlobList *blobList, LabelRaster* label)
{
	const ImageRect* theRect = label->getBoundRect();
	
	float oldX = currentState_->getPos()->getX();
	float oldY = currentState_->getPos()->getY();	
	
	float predX = oldX+metricX_,
		  predY = oldY+metricY_;
	if(predX > theRect->getRight())
		predX = theRect->getRight();
	else if(predX < theRect->getLeft())
		predX = theRect->getRight();
	if(predY > theRect->getBottom())
		predY = theRect->getBottom();
	else if(predY < theRect->getTop())
		predY = theRect->getTop();
		
	//----------------------------------------------------------------
	//	prediction
	//----------------------------------------------------------------
	if(savePred_ == true)		//  save to predRecord_
	{		
		BlobTrackingState *predState = new BlobTrackingState(predX, predY,
											((BlobTrackingState *)currentState_)->getSize());
		predRecord_.push_back(predState);
	}
	ImagePoint* predPos = new ImagePoint((int) predX, (int) predY);
	ImagePoint_F *predPosF = new ImagePoint_F((float) predX, (float) predY);


	//----------------------------------------------------------------
	//	track based on prediction
	//----------------------------------------------------------------
	float	realX, realY, dx, dy;
		
	bool flag = true;
	//	check the *blobList to see if any matches
	if(!blobList->empty())
	{
		BlobList::iterator	iter = blobList->begin();
		for( ; iter!=blobList->end(); iter++)
		{
			realX	= (*iter)->getCentroid()->getX(),
			realY	= (*iter)->getCentroid()->getY(),
			dx		= realX - predX,
			dy		= realY - predY;
			//	Any one matches?
			//	yes
			if(dx<50 && dx>-50 && dy<50 && dy>-50)
			{
				//  update currentState_, metricX_, metricY_, failCounts_
				currentState_->setResult(TARGET_MATCH_LIST);
				failCounts_				= 0;			//	reset failCounts_
				currentState_->setPos((*iter)->getCentroid());
				metricX_ = realX - oldX;
				metricY_ = realY - oldY;
				((BlobTrackingState *) currentState_)->setSize((*iter)->getNbPixels());
				if(saveState_ == true)
				{					//  save to stateRecord_
					BlobTrackingState *realState = new BlobTrackingState(currentState_->getPos()->getX(),
													 		currentState_->getPos()->getY(),
													 		((BlobTrackingState *)currentState_)->getSize());
					stateRecord_.push_back(realState);
				}
				blob_ = *iter;
				flag = false;						//	no need to search blob
				break;
			}
		}
			
	}

	//	still need to search blob
	if(flag == true)
	{
		long	sizeRange = 8,
				temp = ((BlobTrackingState *)currentState_)->getSize() - sizeRange,
				blobMinSize = temp > 0 ? temp : (temp+sizeRange);

		blob_				= ((BlobFinder*) detector_)->findBlobAbout(label, predPos, 
												 (char)1,
												 blobMinSize, theRect);
		

		//	Found something?
		//	yes
		if(blob_ != NULL)
		{
			realX	= blob_->getCentroid()->getX(),
			realY	= blob_->getCentroid()->getY(),
			dx		= realX - predX,
			dy		= realY - predY;
			//	Is the right target?
			//	yes
			if(dx<distanceRange_ && dx>-distanceRange_ && dy<distanceRange_ && dy>-distanceRange_)
			{
				//  update currentState_, metricX_, metricY_, failCounts_
				currentState_->setResult(TARGET_FOUND);
				failCounts_				= 0;			//	reset failCounts_
				currentState_->setPos(blob_->getCentroid());
				metricX_ = realX - oldX;
				metricY_ = realY - oldY;
				((BlobTrackingState *) currentState_)->setSize(blob_->getNbPixels());
				if(saveState_ == true)
				{					//  save to stateRecord_
					BlobTrackingState *realState = new BlobTrackingState(currentState_->getPos()->getX(),
													 		currentState_->getPos()->getY(),
													 		((BlobTrackingState *)currentState_)->getSize());
					stateRecord_.push_back(realState);
				}
			}
			//	no
			else
			{
				currentState_->setResult(WRONG_TARGET_FOUND);
				failCounts_++;
				currentState_->setPos(predPosF);
			}
		}
		//	no
		else
		{
			currentState_->setResult(TARGET_NOT_FOUND);
			failCounts_++;
			currentState_->setPos(predPosF);
		}
		
		//	set targetGone_ if necessary
		if(failCounts_ >= 5)
			currentState_->setTargetGone(true);
	}
	
	delete predPos;
	delete predPosF;
	
	return currentState_;
}


TrackingStateList BlobTracker2D::getPredRecord(void)
{
	return Tracker2D::getPredRecord();
}

TrackingStateList BlobTracker2D::getStateRecord(void)
{
	return Tracker2D::getStateRecord();
}

Blob* BlobTracker2D::getCurrentBlob(void)
{
	return blob_;
}
			
TrackingState *BlobTracker2D::getCurrState(void)
{
	return Tracker2D::getCurrState();
}


//----------------------------------------------------------
//  private and protected functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif





/*	version 1.0 (without introduction of Tracker2DControl, with backgroundremover inside)
	runs well for Tracking2D DEMO

//  BlobTracker2D.cpp

#include "BlobTracker2D.h"
#include <iostream>

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

BlobTracker2D::BlobTracker2D(FeatureDetector *detector,
							 float metricX,
							 float metricY,
							 BlobTrackingState *initState,
							 RasterImage* bkgdImg,
							 bool savePred, 
							 bool saveState )
	: Tracker2D(detector, metricX, metricY, initState, savePred, saveState)
{
	thresholder_	= new GrayThresholdLabeler();
	colorParams_	= new ColorSegParams;
	remover_		= new BackgroundRemover_Static();
	backgroundImg_	= bkgdImg;
	currImg_		= NULL;
	
	colorParams_->fillMode	= kOrigColorFill;
	colorParams_->nbModes	= 1;
	colorParams_->colorMode	= false;
	colorParams_->colorReg	= NULL;
	colorParams_->grayReg	= new GrayRegion[colorParams_->nbModes];
    colorParams_->grayReg[0].labelStr =  "medium gray";
    colorParams_->grayReg[0].vMin = 5;
    colorParams_->grayReg[0].vMax = 255;
    colorParams_->grayReg[0].fillColor[0] = 0;
    colorParams_->grayReg[0].fillColor[1] = (unsigned char) 255;
    colorParams_->grayReg[0].fillColor[2] = (unsigned char) 0;
    colorParams_->grayReg[0].label = 1;
	remover_->setBackground(backgroundImg_);

}

BlobTracker2D::~BlobTracker2D(void)
{
	delete thresholder_;
	delete colorParams_;
	delete remover_;
	delete currImg_;
}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

TrackingState* BlobTracker2D::track(RasterImage* img)
{
	ImageRect	*theRect = ImageRect::intersection(img->getBoundRect(), backgroundImg_->getBoundRect());
	
	float oldX = currentState_->getPos()->getX();
	float oldY = currentState_->getPos()->getY();	
	
	float predX = oldX+metricX_,
		  predY = oldY+metricY_;
	if(predX > theRect->getRight())
		predX = theRect->getRight();
	else if(predX < theRect->getLeft())
		predX = theRect->getRight();
	if(predY > theRect->getBottom())
		predY = theRect->getBottom();
	else if(predY < theRect->getTop())
		predY = theRect->getTop();

	//	prediction 
	if(savePred_ == true){				//  save to predRecord_
		BlobTrackingState *predState = new BlobTrackingState(predX, predY,
											((BlobTrackingState *)currentState_)->getSize());
		predRecord_.push_back(predState);
	}
	ImagePoint* predPos = new ImagePoint((int) predX, (int) predY);
	//	end of prediction


	//	track based on prediction  
	currImg_ = img;
	RasterImage_gray	*differImg	= new RasterImage_gray(theRect);
	long	sizeRange = 8,
			temp = ((BlobTrackingState *)currentState_)->getSize() - sizeRange,
			blobMinSize = temp > 0 ? temp : (temp+sizeRange);

	remover_->removeBackground(currImg_, differImg, MAX_DIFFERENCE);
	LabelRaster	*label	= thresholder_->applyTo(differImg, colorParams_);
	Blob		*blob	= ((BlobFinder*) detector_)->findBlobAbout(label, predPos, 
											 colorParams_->grayReg[0].label,
											 blobMinSize, theRect);
	//  end of track based on prediction 
	
	
	//	real case 
	//  update currentState_, metricX_, metricY_
	currentState_->setPos(blob->getCentroid());
	metricX_ = currentState_->getPos()->getX() - oldX;
	metricY_ = currentState_->getPos()->getY() - oldY;
	((BlobTrackingState *) currentState_)->setSize(blob->getNbPixels());
	if(saveState_ == true){					//  save to stateRecord_
		BlobTrackingState *realState = new BlobTrackingState(currentState_->getPos()->getX(),
										 		currentState_->getPos()->getY(),
										 		((BlobTrackingState *)currentState_)->getSize());
		stateRecord_.push_back(realState);
	}
	//	end of real case	
	
	delete differImg;
	delete predPos;
	delete label;
	delete blob;
	
	return currentState_;
}


TrackingStateList BlobTracker2D::getPredRecord(void)
{
	Tracker2D::getPredRecord();
}

TrackingStateList BlobTracker2D::getStateRecord(void)
{
	Tracker2D::getStateRecord();
}

//----------------------------------------------------------
//  private and protected functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif
*/

