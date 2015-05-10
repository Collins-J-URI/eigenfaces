/*  NAME:
        BlobTracker2DController.cpp
 
    DESCRIPTION:
        BlobTracker2DController source file.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*	version 1.1 , deal with blobs not overlap	*/

#include <iostream>
#include <list>
//
#include "BlobTracker2DController.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------
RasterImage_gray* BlobTracker2DController::getDifferImg(void)
{
	return differImg_;
}

RasterImage* BlobTracker2DController::getCurrentBackground(void)
{
	return remover_->getBackground();
}

BlobTracker2DController::BlobTracker2DController(RasterImage* firstBkgd)
						: Tracker2DController()
{
	remover_	= new BackgroundRemover_dynamic_MW();
	remover_->setBackground(firstBkgd);
	detector_		= new BlobFinder();
	differImg_		= new RasterImage_gray(firstBkgd->getBoundRect());
	thresholder_	= new GrayThresholdLabeler();
	grayParams_		= new ColorSegParams;
	label_			= new LabelRaster(firstBkgd->getValidRect());
	blobFinder_		= new BlobFinder();
	target_			= new char[1];
	blobMinSize_	= 5;//20;
	frameCount_		= 0;
	PeriodOfDetection_ = 0;
	noiseSize_		= 2;//15;

    grayParams_->nbModes = 1;
    grayParams_->colorMode = false;
    grayParams_->colorReg = NULL;
    grayParams_->grayReg = new GrayRegion[grayParams_->nbModes];
    grayParams_->fillMode = kOrigColorFill;
    grayParams_->grayReg[0].labelStr =  "small difference";
    grayParams_->grayReg[0].vMin = 6;
    grayParams_->grayReg[0].vMax = 255;
    grayParams_->grayReg[0].fillColor[0] = (unsigned char) 255;
    grayParams_->grayReg[0].fillColor[1] = (unsigned char) 0;
    grayParams_->grayReg[0].fillColor[2] = (unsigned char) 0;
    grayParams_->grayReg[0].label = (char) 1;			//  must begin from n>0
    target_[0] = grayParams_->grayReg[0].label;

	blobList_	= new BlobList[1];
}

BlobTracker2DController::~BlobTracker2DController(void)
{
	delete remover_;
	delete detector_;
	delete differImg_;
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


BlobTracker2DList BlobTracker2DController::getCurrTrackerList(void)
{
	return trackerList_;
}

void BlobTracker2DController::action(RasterImage* img)
{
//---------------------------------------------------
//	do background subtraction, thresholding
//---------------------------------------------------
	remover_->removeBackground(img, differImg_);
	thresholder_->applyInto(differImg_, grayParams_, label_);

//---------------------------------------------------
//  tracking control
//---------------------------------------------------
	BlobList::iterator		blobIter;
	list<const ImageRect*>::iterator		rectIter;
	BlobTracker2DList::iterator	trackerIter;
	const ImagePoint_F			*center		= NULL;
	BlobTrackingState 		*trackingState		= NULL;

	//	initial setup
	if(frameCount_ == 0)
	{
		blobFinder_->findAllBlobs(label_, grayParams_->nbModes, target_, blobMinSize_, blobList_);
		
		blobIter	= blobList_[0].begin();
		trackerIter	= trackerList_.begin();
		
		//	create tracker each detected blob
		for( ; blobIter!=blobList_[0].end(); blobIter++)
		{
			// reject small noise blobs
			if((*blobIter)->getNbPixels() > noiseSize_)
			{			
				center	= (*blobIter)->getCentroid();
				cout << "x, y = " << center->getX() << " , " << center->getY() << endl;
				trackingState	= new BlobTrackingState(center->getX(),center->getY(),5);
				
				// push into trackerList_
				trackerList_.push_back(new BlobTracker2D(new BlobFinder(), 0.f, 0.f, trackingState));
				
				delete	center;
				delete	trackingState;
			}
		}
		
		//---------------------------------------------------
		//  update background using dynamic backgroundremover.
		//---------------------------------------------------
		for( ; blobIter!=blobList_[0].end(); blobIter++)
			rectList_.push_back((*blobIter)->getBoundRect());
		if(rectList_.empty())
			rectList_.push_back(NULL);
		remover_->updateBackground(img, rectList_);
		
		//	blobList is supposed to store the wrongly found blobs, here it is only temporarily
		//	used in setup phase, therefore we clear it after temporary use
		for (blobIter != blobList_[0].begin(); blobIter != blobList_[0].end(); blobIter++)
			delete (*blobIter);
		for (rectIter != rectList_.begin(); rectIter != rectList_.end(); rectIter++)
			delete (*rectIter);
		blobList_[0].clear();
		rectList_.clear();
		
		frameCount_++;
		PeriodOfDetection_++;
	}

	//	continuous tracking
	else
	{
		float	xTracker,
				yTracker,
				xBlob,
				yBlob,
				xTemp,
				yTemp,
				dx,
				dy;
		int		distanceRange;
		
		// create a temporary variable in order to prevent label_ from being changed
		LabelRaster	*tempLabel	= new LabelRaster(label_);

		blobIter	= blobList_[0].begin();
		trackerIter	= trackerList_.begin();
		
		//	check each detected blob
		if((*trackerIter) != NULL)
		{
			for( ; trackerIter!=trackerList_.end(); trackerIter++)
			{
				trackingState = (BlobTrackingState *) (*trackerIter)->track(&blobList_[0], label_);
				
				xTracker = (*trackerIter)->getCurrentBlob()->getCentroid()->getX();
				yTracker = (*trackerIter)->getCurrentBlob()->getCentroid()->getY();
				distanceRange = (*trackerIter)->distanceRange_;
				
				switch (trackingState->getResult())
				{
					//	if find a wrong one, push into blobList_[0]
					case WRONG_TARGET_FOUND:
						blobList_[0].push_back((*trackerIter)->getCurrentBlob());			
						//	the tracker lost its targets
						if(trackingState->getTargetGone() == true)
							trackerList_.erase(trackerIter);
						break;
					
					//	if match one in blobList_[0], delete the matched one
					case TARGET_MATCH_LIST:
						if((*blobIter) != NULL)
						{
							for( ; blobIter!=blobList_[0].end(); blobIter++)
							{
								xBlob	= (*blobIter)->getCentroid()->getX();
								yBlob	= (*blobIter)->getCentroid()->getY();
								dx		= xTracker - xBlob;
								dy		= yTracker - yBlob;
								if(dx+distanceRange>0 && dx-distanceRange<0
								   && dy+distanceRange>0 && dy-distanceRange<0)
								{
									blobList_[0].erase(blobIter);
								}
							}
						}
						break;
					
					//	if no target found, check if the tracker lost its target
					case TARGET_NOT_FOUND:
						if(trackingState->getTargetGone() == true)
							trackerList_.erase(trackerIter);
						break;
						
					default:
						break;
				}
				
			}
		}
		
		frameCount_++;		//	increasing frameCount_
		PeriodOfDetection_++;	//	increasing PeriodOfDetection_

		///*	PENDING
		//	new object detection and handling (T = 5)
		if(PeriodOfDetection_ == 5)
		{
			// once a new object found
			BlobList	*tempList	= new BlobList[1];
			
			// (maybe we can use NormalFlow instead of/together with BackgroundRemover)
			blobFinder_->findAllBlobs(tempLabel, grayParams_->nbModes, target_, blobMinSize_, tempList);
						
			// compare two list, if a new object is found, send it a new Tracker2D
			/* put it into the NewBlobList, meanwhile, update NewBlobList*/
			BlobList::iterator tempIter = tempList[0].begin();
			//NewBlobList::iterator nbIter = newBlobList_.begin();
			bool match;
			for( ; tempIter != tempList[0].end(); tempIter++)
			{
				xTemp = (*tempIter)->getCentroid()->getX();
				yTemp = (*tempIter)->getCentroid()->getY();
				
				match = false;
				if((*trackerIter) != NULL)
				{
					for(trackerIter=trackerList_.begin(); trackerIter!=trackerList_.end(); trackerIter++)
					{
						xTracker	= (*trackerIter)->getCurrentBlob()->getCentroid()->getX();
						yTracker	= (*trackerIter)->getCurrentBlob()->getCentroid()->getY();
						distanceRange = (*trackerIter)->distanceRange_;
						dx			= xTracker - xTemp;
						dy			= yTracker - yTemp;
						if(dx+distanceRange>0 && dx-distanceRange<0
						   && dy+distanceRange>0 && dy-distanceRange<0)
						{
							match = true;
							break;	
						}
							
					}
					
					if(match == false)
					{
						/*
						for( ; nbIter != newBlobList_.end(); nbIter++)
						{
							if((*nbIter)->blob->getCentroid()->getX() != (*tempIter)->getCentroid()->getX()){
								NewBlob* nb;
								nb->blob = new Blob(*tempIter);
								nb->count = 0;
								newBlobList_.push_back(nb);
							}
						*/
						// reject small noise blobs
						if((*tempIter)->getNbPixels() > noiseSize_)
						{			
							trackingState	= new BlobTrackingState(xTemp,yTemp,5);
				
							// push into trackerList_
							trackerList_.push_back(new BlobTracker2D(new BlobFinder(), 0.f, 0.f, trackingState));
						}
					}
				}
				else
				{
					for (trackerIter != trackerList_.begin(); trackerIter != trackerList_.end(); trackerIter++)
						delete (*trackerIter);
					trackerList_.clear();
					trackingState	= new BlobTrackingState(xTemp,yTemp,5);
		
					// push into trackerList_
					trackerList_.push_back(new BlobTracker2D(new BlobFinder(), 0.f, 0.f, trackingState));
				}
			}
			
			/*
			// check every member's count in the NewBlobList, if it is not recognized
			// for T times, send a new tracker to it and erase it from the NewBlobList
			*/
			
			for (blobIter != tempList[0].begin(); blobIter != tempList[0].end(); blobIter++)
				delete (*blobIter);
			tempList[0].clear();
			delete []tempList;
			
			PeriodOfDetection_ = 0;
		}
		
		delete tempLabel;
	
		//---------------------------------------------------
		//  update background using dynamic backgroundremover.
		//---------------------------------------------------
		//	iterate blobList_[0]
		for(blobIter=blobList_[0].begin(); blobIter!=blobList_[0].end(); blobIter++)
			rectList_.push_back((*blobIter)->getBoundRect());
		
		//	iterate trackerList_	
		Blob* blob = NULL;
		for(trackerIter=trackerList_.begin(); trackerIter!=trackerList_.end(); trackerIter++)
		{
			blob = (*trackerIter)->getCurrentBlob();
			if(blob != NULL)
				rectList_.push_back(blob->getBoundRect());
		}
		
		if(rectList_.empty())
			rectList_.push_back(NULL);		
		remover_->updateBackground(img, rectList_);
		for (rectIter != rectList_.begin(); rectIter != rectList_.end(); rectIter++)
			delete (*rectIter);
		rectList_.clear();
	}
	
	cout << "frameCount_" << frameCount_ << endl;
	cout << "PeriodOfDetection_" << PeriodOfDetection_ << endl;
}



//----------------------------------------------------------
//  private and protected functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

