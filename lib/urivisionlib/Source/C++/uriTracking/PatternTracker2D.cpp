/*  NAME:
        PatternTracker2D.cpp
 
    DESCRIPTION:
        PatternTracker2D source file.
        (abstract)base class: Tracker2D
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


#include <iostream>
#include <stack>
//
#include "PatternTracker2D.h"
#include "ImageWriter.h"

using namespace std;
using namespace uriVL;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

PatternTracker2D::PatternTracker2D(Pattern2D* pattern,
        							 const ImageRect* bound,
        							 float scoreThreshold,
        							 float metricX,
        							 float metricY,
        							 PatternTrackingState *initState,
        							 bool savePred, /* = false */
        							 bool saveState /* = false */)
	    : Tracker2D()
{
	const ImageRect		*rect = pattern->getBoundRect();
	
	if (pattern->getBlob() != NULL)
		pattern_		= new Pattern2D(pattern->getBlob(), pattern->getPattern());
	else
		pattern_		= new Pattern2D(pattern->getBoundRect(), pattern->getPattern());

	scoreThreshold_ = scoreThreshold;
	leftBound_		= bound->getLeft();
	rightBound_		= bound->getRight();
	topBound_		= bound->getTop();
	bottomBound_	= bound->getBottom();
	
	metricX_		= metricX;
	metricY_		= metricY;
	currentState_	= new PatternTrackingState(initState->getPos()->getX(),
											   initState->getPos()->getY(),
											   initState->getSize());
	savePred_		= savePred;
	saveState_		= saveState;

	BaseRasterType	rasterType = pattern->getPattern()->getBaseRasterType();
	RasterImage		*ri;
	///Blob			*blob = new Blob(pattern->getBlob());
    switch (rasterType)
    {
        case kBinaryRaster:
            ri = new RasterImage_binary(rect);
			ri->setValidRect(rect);
            break;

        case kGrayRaster:
            ri = new RasterImage_gray(rect);
			ri->setValidRect(rect);
            break;

        case kRGBa32Raster:
            ri = new RasterImage_RGBa(rect);
			ri->setValidRect(rect);
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in PatternTracker2D constructor");
            break;

    }
	///scaledPattern_	= new Pattern2D(blob, ri);
	scaledPattern_	= new Pattern2D(rect, ri);
									
	matchRecord_ = new Pattern2DMatchRecord_warp();//NULL;
	
	matcher_ = new PatternMatcher2D_warp(PATTERN_MATCHER_2D_CORR,
										 NO_UPDATE, GL_WARPER, 5, 5,
										 INNER_BOUNDS, BORDER_FIXED);
	
	rectRatio_ = 1.2f;
	matchX_ = 0;
	matchY_ = 0;
	grabbedImgWidth_ = grabbedImgHeight_ = 0;
	grabPatternRect_ = new ImageRect(0, 0, 0, 0);	
	grabPoint_ = new ImagePoint(0, 0);	
	isNewObj_ = true;
	
	nbNeighborPixels_ = 3;	

	//  as we go away from the border it becomes increasingly hard for a pixel
	//  to pass the merge test
	mergeThreshold_ = new int[nbNeighborPixels_+1];
	mergeThreshold_[0] = 20;//scoreThreshold_ + 2;
	for (int i=1; i<=nbNeighborPixels_; i++)
	    mergeThreshold_[i] = mergeThreshold_[i-1] + 2;//20;

	//  as we go away from the border it becomes increasingly hard for a pixel
	//  to pass the carve test
	carveThreshold_ = new int[nbNeighborPixels_+1];
//	carveThreshold_[0] = scoreThreshold_ - 2;
	carveThreshold_[0] = 20;//scoreThreshold_ - 2;
	for (int i=1; i<=nbNeighborPixels_; i++)
	    carveThreshold_[i] = carveThreshold_[i-1] - 2;//20;

	diffImg_ = NULL;
	diffImgRaster2D_ = NULL;
}

PatternTracker2D::~PatternTracker2D(void)
{
	delete matcher_;
	delete pattern_;
	delete scaledPattern_;
	delete matchRecord_;
	delete grabPatternRect_;
	delete grabPoint_;
	delete diffImg_;
	delete []mergeThreshold_;
}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

TrackingState* PatternTracker2D::track(RasterImage* img,
										BackgroundRemover_dynamic_MW *remover)
{
	float oldX = currentState_->getPos()->getX();
	float oldY = currentState_->getPos()->getY();	
	
	float predX = oldX+metricX_,
		  predY = oldY+metricY_;
	if (predX > rightBound_)
		predX = rightBound_;
	else if (predX < leftBound_)
		predX = leftBound_;
	if (predY > bottomBound_)
		predY = bottomBound_;
	else if (predY < topBound_)
		predY = topBound_;
		
	//----------------------------------------------------------------
	//	prediction
	//----------------------------------------------------------------
	if (savePred_ == true)		//  save to predRecord_
	{		
		PatternTrackingState *predState = new PatternTrackingState(predX, predY,
											((PatternTrackingState *)currentState_)->getSize());
		predRecord_.push_back(predState);
	}

	//----------------------------------------------------------------
	//	track based on prediction
	//----------------------------------------------------------------
	float	realX, realY;	// realX, realY refer to upperLeft	
	float	score;
	RasterImage	*grabbedImg = NULL;
// 	Pattern2D	*newPattern = NULL;  // unused variable. -CJC
// 	ImagePoint	*realMatchPoint = NULL;  // unused variable. -CJC
	Contour_I *ct = NULL;
	const ImageRect	*blobRect = NULL;
// 				*patRect = NULL; // unused variable. -CJC
	
	//	grab pattern & create matcher
	if (isNewObj_)
		isNewObj_ = false;	

	grabbedImg = this->grabImage_(img, static_cast<int>(predX), static_cast<int>(predY));
		
	// apply pattern matching
	ImagePoint matchPoint(matchX_, matchY_);
	matcher_->match(pattern_, &matchPoint, grabbedImg, matchRecord_);
	
	score = matchRecord_->getScore();
										
	//	found target
	//	yes
	if (score < scoreThreshold_)//true)
	{
		//	update pattern_
		ct = ((Pattern2DMatchRecord_warp*)matchRecord_)->getReverseWarpContour();
		Blob blob(ct);

{
    //  print info about that pattern's blob
    const LineSegDeque segDeque = blob.getSegDeque();
    cout << "\t-------Blob before merge----------"<<endl;
    LineSegDeque::const_iterator lastLine = segDeque.end();
    for (LineSegDeque::const_iterator lineIter = segDeque.begin(); lineIter != lastLine; lineIter++)
    {
        cout << "\t\ty = " << (*((*lineIter)->begin()))->y << ": ";
        list<HorizontalSegment*>::const_iterator lastSeg = (*lineIter)->end();
        for (list<HorizontalSegment*>::const_iterator segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
        {
            HorizontalSegment   *theSeg;
            theSeg = (*segIter);
            cout << "<" << theSeg->leftX << ", " << theSeg->rightX << ">  ";
        }
        cout << endl << flush;
    }
}

		//	merge blob using background subtraction (and maybe motion information)
		mergeBlob_(img, remover, &blob);

{
    //  print info about that pattern's blob
    const LineSegDeque segDeque = blob.getSegDeque();
    cout << "\t-------Blob out of merge----------"<<endl;
    LineSegDeque::const_iterator lastLine = segDeque.end();
    for (LineSegDeque::const_iterator lineIter = segDeque.begin(); lineIter != lastLine; lineIter++)
    {
        cout << "\t\ty = " << (*((*lineIter)->begin()))->y << ": ";
        list<HorizontalSegment*>::iterator lastSeg = (*lineIter)->end();
        for (list<HorizontalSegment*>::iterator segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
        {
            HorizontalSegment   *theSeg;
            theSeg = (*segIter);
            cout << "<" << theSeg->leftX << ", " << theSeg->rightX << ">  ";
        }
        cout << endl << flush;
    }
}

//grabbedImg->translate(grabbedImgLf_, grabbedImgTp_);
//		mergeBlob_(grabbedImg, remover, &blob);
//grabbedImg->translate(-grabbedImgLf_, -grabbedImgTp_);

		Pattern2D newPat(&blob, grabbedImg);
		pattern_->update(&newPat, true);
//		pattern_->update(grabbedImg, &blob, true);
//		delete grabbedImg;
//  print info about that pattern's blob
{
const LineSegDeque segDeque = pattern_->getBlob()->getSegDeque();
cout << "\t-------Blob-out of update----------"<<endl;
LineSegDeque::const_iterator lastLine = segDeque.end();
for (LineSegDeque::const_iterator lineIter = segDeque.begin(); lineIter != lastLine; lineIter++)
{
    cout << "\t\ty = " << (*((*lineIter)->begin()))->y << ": ";
    list<HorizontalSegment*>::const_iterator lastSeg = (*lineIter)->end();
    for (list<HorizontalSegment*>::const_iterator segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
    {
        HorizontalSegment   *theSeg;
        theSeg = (*segIter);
        cout << "<" << theSeg->leftX << ", " << theSeg->rightX << ">  ";
    }
    cout << endl << flush;
}
}
		
		blobRect = blob.getValidRect();
	
		//	realX, realY in global image coordinate
		realX = grabbedImgLf_ + blobRect->getLeft();
		realY = grabbedImgTp_ + blobRect->getTop();
		
		//	update metricX_, metricY_
		metricX_ = realX - oldX;
		metricY_ = realY - oldY;
		
		//	reset failCounts_
		failCounts_	= 0;					
	
		//	update currentState_
		currentState_->setResult(TARGET_FOUND);
		currentState_->setPos(realX, realY);
		long patSize = pattern_->getNbPixels();
		((PatternTrackingState *) currentState_)->setSize(patSize);
		
		//  if required, save to stateRecord_
		if (saveState_ == true)
		{					
			PatternTrackingState *realState = new PatternTrackingState(realX, realY, patSize);
			stateRecord_.push_back(realState);
		}
	}
	//	no
	else
	{
		currentState_->setResult(TARGET_NOT_FOUND);
		failCounts_++;
		currentState_->setPos(predX, predY);
	}
	
	//	set targetGone_ if necessary
	if (failCounts_ >= 3)
		currentState_->setTargetGone(true);
	
	return currentState_;
}


TrackingStateList PatternTracker2D::getPredRecord(void)
{
	return Tracker2D::getPredRecord();
}


TrackingStateList PatternTracker2D::getStateRecord(void)
{
	return Tracker2D::getStateRecord();
}

Pattern2D* PatternTracker2D::getCurrPattern(void)
{
	return pattern_;
}
			
TrackingState *PatternTracker2D::getCurrState(void)
{
	return Tracker2D::getCurrState();
}

PatternMatcher2D_warp *PatternTracker2D::getMatcher(void)
{
	return matcher_;
}

RasterImage_gray* PatternTracker2D::getDifferenceImage(void)
{
	return diffImg_;
}


//----------------------------------------------------------
//  private and protected functions
//----------------------------------------------------------

RasterImage* PatternTracker2D::grabImage_(RasterImage* img,
										  int predX, int predY)
{
	RasterImage* grabbedImg = NULL;
	const ImageRect	*imgRect = img->getValidRect(),
				    *blobRect = pattern_->getBlob()->getValidRect();
	const int blobWidth = blobRect->getWidth();
	const int blobHeight = blobRect->getHeight();
	
	int newSize = static_cast<int>(blobWidth * rectRatio_);
	int temp = 1, temp2 = 2;
    //  get powers of 2 that bracket the blob's width
	while (temp2 < newSize) 
	{
		temp = temp2;
        temp2 *= 2;
	}
    int newWidth = temp;
    
    //  do same thing for the height
	newSize = static_cast<int>(blobHeight * rectRatio_);
    temp = 1, temp2 = 2;
	while (temp2 < newSize) 
	{
		temp = temp2;
        temp2 *= 2;
	}
    int newHeight = newSize;

    //  Now check which dimensions the warper advocates for this pattern
    int loWidth, hiWidth, loHeight, hiHeight;
    matcher_->getImageSuitableDimensions(newWidth, newHeight, &loWidth, &loHeight, 
                                        &hiWidth, &hiHeight);

	int patLeft = predX - (newWidth - blobWidth) / 2;
	int patTop = predY - (newHeight - blobHeight) / 2;
	int patRight = patLeft + newWidth;
	int patBottom = patTop + newHeight;
	
	//	shift lf & tp (if necessary) to make the bigRect
	//	always inside the image
	int rtMore = patRight - imgRect->getWidth();
	int btMore = patBottom - imgRect->getHeight();
	if (patLeft < 0)
		patLeft = 0;
	else if (rtMore > 0)
		patLeft -= rtMore;
	if (patTop < 0)
		patTop = 0;
	else if (btMore > 0)
		patTop -= btMore;
	
    grabPatternRect_->setRect(patLeft, patTop, newWidth, newHeight);	
	grabPoint_->setCoordinates(0, 0);		
	grabbedImg = RasterImage::grab(img, grabPatternRect_, grabPoint_);

	//	set matchX_ and matchY_ which are inside this newly grabbed image for later matching
	matchX_ = predX - patLeft;
	matchY_ = predY - patTop;
	
	//	update grabbed image dimensions
	grabbedImgWidth_ = newWidth;
	grabbedImgHeight_ = newHeight;
		
	//	update grabbedImgLf_ & grabbedImgTp
	grabbedImgLf_ = patLeft;
	grabbedImgTp_ = patTop;
	
	return grabbedImg;
}

void PatternTracker2D::mergeBlob_(RasterImage* img,
								  BackgroundRemover_dynamic_MW *remover,
								  Blob* blob)
{
	const ImageRect* imgRect = img->getValidRect();

	//	one time allocation
	if (diffImg_ == NULL)
	{
		diffImg_ = new RasterImage_gray(imgRect);
		diffImg_->setValidRect(imgRect);
	};
	
	int i, n;
	LineSegDeque segDeque = blob->getSegDeque();
	LineSegDeque::iterator	lineIter = segDeque.begin(),
							endLineIter = segDeque.end();
	list<HorizontalSegment*>::iterator  segIter, endSegIter;
//  in the future img should only be the grabImg so that imgLeftBound is not
//  always 0
	int imgLeftBound = grabbedImgLf_, 
	    imgRightBound=grabbedImgLf_ + grabbedImgWidth_ - 1,
		leftBound, rightBound,
		segLf, segRt, segY,
// 		prevSegRt, nextSegLf,  // unused variables. -CJC
		xIndex;
	
	//	background subtraction
	remover->removeBackground(img, diffImg_, MAX_DIFFERENCE);
	diffImgRaster2D_ = diffImg_->getShiftedRaster2D();
	
	//	translate blob into the global image coordinate
	blob->translate(grabbedImgLf_, grabbedImgTp_);

	//	iterate all the lines of the blob
    long dequeSize = static_cast<long>(segDeque.size());
    for (long k=0; k<dequeSize; k++)
	{
		//	iterate all the HSeg in this line
		endSegIter = lineIter[k]->end();
		segIter=lineIter[k]->begin();
		leftBound = imgLeftBound;

		for ( ; segIter!=endSegIter; segIter++)
		{
			segLf = (*segIter)->leftX;
			segRt = (*segIter)->rightX;
			segY = (*segIter)->y;

            //--------------------------------
            //  Left border of the segment
            //--------------------------------
			//	If the left border pixel does not overlap with leftBound and has a background
			//  difference above the threshold, then we can try to expand to the left (outward)
			if ((segLf!=leftBound) && (diffImgRaster2D_[segY][segLf] > mergeThreshold_[0]))
			{
				//	decide left bound
				i=0; n = segLf - leftBound;
				if (n > nbNeighborPixels_)
				    n = nbNeighborPixels_;
				bool keepGoing = true;
				while (keepGoing && (i<n))
				{
					i++;
					//	expand left part of this HSeg to this pixel
					xIndex = segLf - i;
					if (diffImgRaster2D_[segY][xIndex] < mergeThreshold_[i])
						keepGoing = false;
				}
				(*segIter)->leftX = xIndex + 1;
				
    			//  If this segment's left end hit the left neighbor, then we must merge the segments
    			if (((*segIter)->leftX == leftBound) && (leftBound != imgLeftBound))
    			{
    			    //  delete this segment
    			    delete (*segIter);
    			    //  remove the element from the list
    			    list<HorizontalSegment*>::iterator  tempIter = segIter;
    			    segIter--;
    			    lineIter[k]->erase(tempIter);
    			    //  update the right bound of the merged segment
    			    (*segIter)->rightX = segRt;
    			    
    			    //  and then go to the "process right side of segment" section
    			}

			}
			//	Else if the left border pixel has a difference below the threshold we can try
			//  to shrink leftX to the right (inward)
			else if (diffImgRaster2D_[segY][segLf] < carveThreshold_[0])
			{
				//	decide right bound
				i=0; n = segRt - segLf;
				if (n > nbNeighborPixels_)
				    n = nbNeighborPixels_;
				bool keepGoing = true;
				while (keepGoing && (i<n))
				{
					i++;
					//	shrink left part of this HSeg to this pixel
					xIndex = segLf + i;
					if (diffImgRaster2D_[segY][xIndex] > carveThreshold_[i])
						keepGoing = false;
				}
				(*segIter)->leftX = xIndex - 1;
			}
			segLf = (*segIter)->leftX;


            //--------------------------------
            //  Right border of the segment
            //--------------------------------
            //  changed to false if this segments merges with its right neighbor
            bool lastPass = false;
            while (!lastPass)
            {
                lastPass = true;
    			bool hasRightNeighbor = false;
    			//  if this is the rightmost segment, the right bound is defined by
    			//  the image, otherwise it is defined by the right neighbor (next segment)
    			segIter++;
    			if (segIter == endSegIter)
    			    rightBound  = imgRightBound;
    			else
    			{
    			    rightBound = (*segIter)->leftX - 1;
    			    hasRightNeighbor = true;
    			}
			    segIter--;

    			//	If the right border pixel does not overlap with rightBound and has a background
    			//  difference above the threshold, then we can try to expand to the right (outward)
    			if ((segRt!=rightBound) & (diffImgRaster2D_[segY][segRt] > mergeThreshold_[0]))
    			{
    				i=0; n = rightBound - segRt;
    				if (n > nbNeighborPixels_)
    				    n = nbNeighborPixels_;
    				bool keepGoing = true;
    				while (keepGoing && (i<n))
    				{
    					i++;
    					//	expand right part of this HSeg to this pixel
    					xIndex = segRt + i;
    					if (diffImgRaster2D_[segY][xIndex] < mergeThreshold_[i])
    						keepGoing = false;
    				}
    				(*segIter)->rightX = xIndex - 1;
    				
    				//  if this segment's right end colided with the left end of its right neighbor
    				//  then we must merge the two segments.
    				if (((*segIter)->rightX == rightBound) && hasRightNeighbor)
    				{
        			    //  delete this segment
        			    delete (*segIter);
        			    //  remove the element from the list
        			    list<HorizontalSegment*>::iterator  tempIter = segIter;
        			    segIter++;
        			    lineIter[k]->erase(tempIter);
        			    //  update the right bound of the merged segment
        			    (*segIter)->leftX = segLf;
        			    
        			    //  we must still process the right end of the newly merged segment
        			    lastPass = false;
    				}
                }
    			//	If the right border pixel has a background difference below the threshold, 
    			//  then we can try to shrink to the left (inward)
    			else if (diffImgRaster2D_[segY][segRt] < carveThreshold_[0])
    			{
    				//	decide right bound
    				i=0; n = segRt - segLf;
    				if (n > nbNeighborPixels_)
    				    n = nbNeighborPixels_;
    				bool keepGoing = true;
    				while (keepGoing && (i<n))
    				{
    					i++;
    					//	expand right part of this HSeg to this pixel
    					xIndex = segRt - i;
    					if (diffImgRaster2D_[segY][xIndex] < carveThreshold_[i])
    						keepGoing = false;
    				}
    				(*segIter)->rightX = xIndex + 1;

        			//  If this segment's right end hit its left end, then the segment should be removed
        			//  Not sure if this should happen
        			if ((*segIter)->rightX < (*segIter)->leftX)
        			{
        			    FAIL_CONDITION( true,
        			                    kFunctionNotImplemented,
        			                    "add code to remove the segment");
        			}
    			}
    			
    			//	from the second seg (if any) on, leftBound is not imgLeftBound any more 
    //			leftBound = prevSegRt;
    			leftBound = (*segIter)->rightX + 1;
		    }
		}
	}
	
	//	check the part outside the range of top & bottom of this blob
//	ImageRect* blobRect = blob->getValidRect();
//	ImageRect searchRect(blobRect->getLeft(), grabPatternRect_->getTop(),
//						 blobRect->getWidth(), grabPatternRect_->getHeight());
	//	extend up from blob's top
//	k = 0;
//	segIter=lineIter[k]->begin();
//	segLf = (*segIter)->leftX;
//	segY = (*segIter)->y;
//	ImagePoint upperLeftPt(segLf, segY);
//	verticalExtend_(lineIter[0], grabPatternRect_, blob, true);
	
	//	extend down from blob's bottom
//	k = dequeSize;
//	segIter=lineIter[k]->begin();
//	segLf = (*segIter)->leftX;
//	segY = (*segIter)->y;
//	ImagePoint bottomLeftPt(segLf, segY);
//	verticalExtend_(lineIter[dequeSize-1], grabPatternRect_, blob, false);

		
	//	translate blob back to the reference pattern local coordinate
	blob->translate(-grabbedImgLf_, -grabbedImgTp_);
}

void PatternTracker2D::verticalExtend_(list<HorizontalSegment*> *segList, const ImageRect* searchRect,
										Blob* blob, bool searchUp)
{

    ImagePoint	pos2(0, 0);
    HorizontalSegment	seg;
    stack<HorizontalSegment>	segStack;
    int	leftX, rightX, /* x, unused variables. -CJC  */ y, r;

//    leftX=findLeft_(pos, searchRect->getLeft());
//    rightX=findRight_(pos, searchRect->getRight());

//    blob->addSegment(leftX, rightX, y);
/*
	//	push on the stack segments verticallly above/below the top/bottom row segments of the blob
	list<HorizontalSegment*>::iterator  segIter, endSegIter = segList->end();
	for (segIter = segList->begin(); segIter != endSegIter; segIter++)
	{
		HorizontalSegment *theSeg = *segIter;
		
		if (searchUp)
		{
		    if (theSeg->y > searchRect->getTop())
		    {
		        //	push
		        seg.leftX = theSeg->leftX;
		        seg.rightX = theSeg->rightX;
		        seg.y = theSeg->y - 1;
		        segStack.push(seg);
		    }
		}
		else
		{
		    if (theSeg->y < searchRect->getBottom())
		    {
		        //	push
		        seg.leftX = theSeg->leftX;
		        seg.rightX = theSeg->rightX;
		        seg.y = theSeg->y + 1;
		        segStack.push(seg);
		    }
	    }
    }
    
    while (!segStack.empty())
    {
        //	pop
        seg = segStack.top();
        segStack.pop();
        leftX = seg.leftX;
        rightX = seg.rightX;
        y = seg.y;
        leftX = findFirstLeft_(leftX, rightX, y, searchRect->getLeft());

        while (leftX<=rightX)
        {
            pos2.setCoordinates(leftX, y);

            r = findRight_(&pos2, searchRect->getRight());

            blob->addSegment(leftX, r, y);

            if (leftX <r)
            {
                if (searchUp)
                {
                	if (y > searchRect->getTop())
	                {
	                    //	push
	                    seg.leftX = leftX;
	                    seg.rightX = r;
	                    seg.y = y - 1;
	                    segStack.push(seg);
	                }
	            }
	            else
	            {
		            if (y < searchRect->getBottom())
	                {
	                    //	push
	                    seg.leftX = leftX;
	                    seg.rightX = r;
	                    seg.y = y + 1;
	                    segStack.push(seg);
	                }
	            }
            }

            if (r+2 <= rightX)
                leftX = findFirstLeft_(r+2, rightX, y, searchRect->getLeft());
            else
                leftX = rightX+1;

        }
    }
*/

	LineSegDeque::iterator	lineIter;
	unsigned long dequeSize;
	list<HorizontalSegment*> *mySegList;
	list<HorizontalSegment*>::iterator  segIter, endSegIter;
	bool keepGoing = true;
	while (keepGoing)
	{
		LineSegDeque segDeque = blob->getSegDeque();
		dequeSize = segDeque.size();
		lineIter = segDeque.begin();
		mySegList = lineIter[static_cast<long>(dequeSize-1)];
		endSegIter = mySegList->end();
		
		for (segIter = mySegList->begin(); segIter != endSegIter; segIter++)
		{
			HorizontalSegment *theSeg = *segIter;
			
			if (searchUp)
			{
			    if (theSeg->y > searchRect->getTop())
			    {
			        //	push
			        seg.leftX = theSeg->leftX;
			        seg.rightX = theSeg->rightX;
			        seg.y = theSeg->y - 1;
			        segStack.push(seg);
			    }
			}
			else
			{
			    if (theSeg->y < searchRect->getBottom())
			    {
			        //	push
			        seg.leftX = theSeg->leftX;
			        seg.rightX = theSeg->rightX;
			        seg.y = theSeg->y + 1;
			        segStack.push(seg);
			    }
		    }
	    }
	    
	    keepGoing = false;
	    while (!segStack.empty())
	    {
	        //	pop
	        seg = segStack.top();
	        segStack.pop();
	        leftX = seg.leftX;
	        rightX = seg.rightX;
	        y = seg.y;
	        leftX = findFirstLeft_(leftX, rightX, y, searchRect->getLeft());

	        while (leftX<=rightX)
	        {
	            pos2.setCoordinates(leftX, y);

	            r = findRight_(&pos2, searchRect->getRight());

	            blob->addSegment(leftX, r, y);

	            if (r+2 <= rightX)
	                leftX = findFirstLeft_(r+2, rightX, y, searchRect->getLeft());
	            else
	                leftX = rightX+1;

				//	if some segment added to compose a new line
				keepGoing = true;
	        }
		}
	}
}

int PatternTracker2D::findFirstLeft_(int leftX, int rightX, int y,
                                 	 int limLeft)
{	
    int	x = leftX;

    if (leftX > rightX)
        return (leftX);

    if (diffImgRaster2D_[y][x] >= mergeThreshold_[0])
    {
        while ( (x>=limLeft) && (diffImgRaster2D_[y][x] >= mergeThreshold_[0]) )
            x--;

        return (x+1);
    }
    else
    {
        while ( (x<=rightX) && (diffImgRaster2D_[y][x] < mergeThreshold_[0]) )
            x++;

        return (x);
    }
}

int PatternTracker2D::findLeft_(ImagePoint* pos, int limLeft)
{
    int	x = pos->getX(),
        y = pos->getY();

    while ( (x>=limLeft) && (diffImgRaster2D_[y][x] >= mergeThreshold_[0]) )
        x--;

    return (x+1);
}

int PatternTracker2D::findRight_(ImagePoint* pos, int limRight)
{
    int	x = pos->getX(),
        y = pos->getY();

    while ( (x<=limRight) && (diffImgRaster2D_[y][x] >= mergeThreshold_[0]) )
        x++;

    return (x-1);
}

#if 0
#pragma mark -
#endif
