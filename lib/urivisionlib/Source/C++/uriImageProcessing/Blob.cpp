/*  NAME:
        Blob.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Blob class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <cstdlib>
//
#include "Blob.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"

using namespace std;
using namespace uriVL;

//----------------------------------
//  Constructors & destructor
//----------------------------------

Blob::Blob(void)
        :   ImageFeature(),
			//	
            segDeque_(LineSegDeque()),
			contour_(NULL),
			contourIsUpToDate_(false),
			top_(0),
			bottom_(0),
			nbHSeg_(0),
			nbPixels_(0),
			drawContour_(false),
			quadNbPixelsAreUpToDate_(false)
{
}

Blob::Blob(const Contour_I* contour)
     try   :	ImageFeature(*contour),
				//	
				segDeque_(LineSegDeque()),
				contour_(new Contour_I(*contour)),
				contourIsUpToDate_(true),
				top_(contour->getBoundRect()->getTop()),
				bottom_(contour->getBoundRect()->getBottom()),
				nbHSeg_(0),
				nbPixels_(0),
				drawContour_(false),
				quadNbPixelsAreUpToDate_(false)
{
	const list<ImagePoint* >* contourList = contour->getPoints();
	
	list<Edge *>  AETList;				// active edge table
	list<Edge *>* ETArrayList = NULL;	// edge table
	
	const ImageRect* rect = contour->getBoundRect();
	int tp = rect->getTop();
	int	height = rect->getHeight();
	ETArrayList = new list<Edge *>[height];
	
	list<ImagePoint* >::const_iterator	currIter, nextIter,
										endIter;			
	list<Edge *>::iterator	ETCurrIter, ETNextIter, AETCurrIter, AETNextIter,
							endETIter, endETIter2,
							endAETIter, endAETIter2;
	Edge *temp = NULL;
	nbHSeg_ = 0;
	nbPixels_ = 0;
	
	currIter = nextIter = contourList->begin();
	nextIter++;
	endIter = contourList->end();
	AETCurrIter = AETList.begin();
	endAETIter = AETList.end();
	endAETIter2 = endAETIter;
	
	//---------------------------------
	//	create ET
	//---------------------------------
	for ( ; nextIter!=endIter; currIter++, nextIter++)
	{
		Edge *edge = new Edge;

		const int x1 = (*currIter)->getX();
		const int y1 = (*currIter)->getY();
		const int x2 = (*nextIter)->getX();
		const int y2 = (*nextIter)->getY();
		int yMin;
			
		// not horizontal edge
		if (y1 != y2)
		{
			if (y1 > y2)
			{
				edge->yMax = y1;
				yMin = y2;
				edge->x = x2;
			}
			else
			{
				edge->yMax = y2;
				yMin = y1;
				edge->x = x1;
			}
			
			//	1/slope actually
			edge->slope = ((float) (x1 - x2)) / (y1 - y2);

			yMin -= tp;
			ETArrayList[yMin].push_back(edge);
		}
		//	otherwise, don't push_back this horizontal edge
	}
	
	//---------------------------------
	//	sort ET
	//---------------------------------
/*
	for (i=0; i<height; i++)
	{
		if (ETArrayList[i].size() != 0)			// if exists
		{
			//	sort the list in x ascending order (such that AET ascending)
			endETIter = ETArrayList[i].end();
			endETIter2 = endETIter;
			endETIter2--;
			ETCurrIter = ETNextIter = ETArrayList[i].begin();
			ETNextIter++;
			for ( ; ETCurrIter!=endETIter2; ETCurrIter++)
			{

				ETNextIter = ETCurrIter;
				ETNextIter++;
				for ( ; ETNextIter!=endETIter; ETNextIter++)
				{
					if ((*ETCurrIter)->x > (*ETNextIter)->x)
					{
						temp = *ETCurrIter;
						*ETCurrIter = *ETNextIter;
						*ETNextIter = temp;
					}
				}
			}
		}
	}
*/

	//---------------------------------
	//	scan AET to form each line of the target blob
	//	& update AET inside each loop
	//---------------------------------
	for (int i=0; i<height; i++)
	{
		//	remove the edges that do not contribute to the scan of
		//	this line in AET
		endAETIter = AETList.end();
		for (AETCurrIter=AETList.begin(); AETCurrIter!=endAETIter; AETCurrIter++)
		{
			if ((*AETCurrIter)->yMax == i+tp)
			{
				delete *AETCurrIter;
				AETList.erase(AETCurrIter);
			}
		}
		
		//	move new edges from ET into AET
		endETIter = ETArrayList[i].end();
		if (ETArrayList[i].size() != 0)
		{
			for (ETCurrIter = ETArrayList[i].begin(); ETCurrIter!=endETIter; ETCurrIter++)
				AETList.push_back(*ETCurrIter);
			ETArrayList[i].clear();
		}
		
		//	scan current line in AET to compose the HSeg for blob_
		//	& update x for each edge in AET
        list<HorizontalSegment*>* segList = new list<HorizontalSegment *>();
		
		//	sort AET according to x (ascending order)

		endAETIter = AETList.end();
		endAETIter2 = endAETIter;
        endAETIter2--;
		for (AETCurrIter=AETList.begin(); AETCurrIter!=endAETIter2; AETCurrIter++)
		{

			AETNextIter = AETCurrIter;
			AETNextIter++;
			for ( ; AETNextIter!=endAETIter; AETNextIter++)
			{
				if ((*AETCurrIter)->x > (*AETNextIter)->x)
				{
					temp = *AETCurrIter;
					*AETCurrIter = *AETNextIter;
					*AETNextIter = temp;
				}
			}
		}

		//	AETList only contains a single point
		if (AETList.size() == 1)
		{
			//	scan line
			int leftX = (*AETCurrIter)->x;
			//	update x
			(*AETCurrIter)->x += static_cast<int>((*AETCurrIter)->slope);
			
			int rightX = (*AETCurrIter)->x;
			
			HorizontalSegment   *seg = new HorizontalSegment;
            seg->leftX = leftX;
            seg->rightX = rightX;
            seg->y = i + tp;
            segList->push_back(seg);

		    nbHSeg_++;
		    nbPixels_ += 1;
		}
		//	AETList contains more than one point
		else
		{
			for (AETCurrIter=AETList.begin();
				 AETCurrIter!=endAETIter2 && AETCurrIter!=endAETIter;
				 AETCurrIter++)
			{	
				//	scan line
				int leftX = (*AETCurrIter)->x;
				//	update x
				(*AETCurrIter)->x += static_cast<int>((*AETCurrIter)->slope);
				
				AETCurrIter++;
				
				int rightX = (*AETCurrIter)->x;
				//	update x
				(*AETCurrIter)->x += static_cast<int>((*AETCurrIter)->slope);
				
				//  Before adding the segment, check whether it is connected to rightmost
				//  segment of the line list.  If it is, then we simply merge both segments
				//  (set the rightX of the one already in the list). Normally this should
				//  not happen but currently there is a bug in the scan algorithm
				bool  addNewSeg = true;
				if (segList->size()>0)
				{
    				list<HorizontalSegment*>::iterator tailIter = segList->end();
    				tailIter--;
    				if ((*tailIter)->rightX == leftX-1)
    				{
    				    addNewSeg = false;
    				    (*tailIter)->rightX = rightX;
    				}
    		    }
				if (addNewSeg)
				{   
    				HorizontalSegment   *seg = new HorizontalSegment;
    	            seg->leftX = leftX;
    	            seg->rightX = rightX;
    	            seg->y = i + tp;
    	            segList->push_back(seg);
    			    nbHSeg_++;
                }
			    nbPixels_ += rightX - leftX + 1;
			}
		}
		if (segList->empty())
		    delete segList;
		else
    		segDeque_.push_back(segList);
	}
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by Blob constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter passed to Blob constructor");
}

Blob::Blob(const Blob* blob, const ImagePoint* origin)
        :   ImageFeature(*blob),
			//	
            segDeque_(LineSegDeque()),
			contour_(NULL),
			contourIsUpToDate_(false),
			top_(blob->top_),
			bottom_(blob->bottom_),
			nbHSeg_(blob->nbHSeg_),
			nbPixels_(blob->nbPixels_),
			drawContour_(false),
			quadNbPixelsAreUpToDate_(blob->quadNbPixelsAreUpToDate_)
{
    FAIL_CONDITION(	blob == NULL,
                    kNullParameterError,
                    "NULL Blob passed as parameter to Pattern2D constructor");

        
    //--------------------------------------------
    //  We copy the easy stuff
    //--------------------------------------------
    setColor(blob->getColor());
	if (blob->contour_ != NULL)
		setContourColor(blob->contour_->getColor());

    //--------------------------------------------
    //  ... and the not-too-hard...
    //--------------------------------------------
    //  If an origin was specified, we compute the translation components
    int tX = 0, tY = 0;
	if (origin != NULL)
    {
        const ImageRect   *blobRect = blob->getValidRect();
        tX = origin->getX() - blobRect->getLeft();
		tY = origin->getY() - blobRect->getTop();

        translate(tX, tY);
    	top_ += tY;
    	bottom_ += tY;
    }

    //--------------------------------------------
    //  We copy the blob's segments
    //--------------------------------------------
    //  We proceed top to bottom.  There is a list of segments for each value of y
    LineSegDeque::const_iterator lastLine = blob->segDeque_.end();
    for (LineSegDeque::const_iterator lineIter = blob->segDeque_.begin(); lineIter != lastLine; lineIter++)
    {
        //  copy the list of HSegs for this value of y
        list<HorizontalSegment*>  *sourceSegList = (*lineIter),
								  *segList = new list<HorizontalSegment*>;
        list<HorizontalSegment*>::const_iterator lastSeg = sourceSegList->end();
        for (list<HorizontalSegment*>::const_iterator segIter = sourceSegList->begin(); segIter != lastSeg; segIter++)
        {
            HorizontalSegment   *copySeg = new HorizontalSegment;
            *copySeg = *(*segIter);
            copySeg->leftX += tX;
            copySeg->rightX += tX;
            copySeg->y += tY;
            segList->push_back(copySeg);
        }
        //  then add the list to the deque
        segDeque_.push_back(segList);
    }
    

    //  We could make a copy of the blob's contour.  To be added later.

    
    //  copy pixel count in quadrants if valid
    if (quadNbPixelsAreUpToDate_)
    {
        for (int q=0; q<4; q++)
            quadNbPixels_[q] = blob->quadNbPixels_[q];

    }

}


Blob::Blob(const Blob& theObj)
        :   ImageFeature(),
			//	
            segDeque_(LineSegDeque()),
			contour_(NULL),
			contourIsUpToDate_(false),
			top_(0),
			bottom_(0),
			nbHSeg_(0),
			nbPixels_(0),
			drawContour_(false),
			quadNbPixelsAreUpToDate_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Blob copy constructor not implemented.");
}



Blob::~Blob(void)
{
    deleteSegments_();
    
    if (contour_ != NULL)
        delete contour_;
        
}

const Blob& Blob::operator = (const Blob& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Blob copy operator not implemented.");

	return *this;
}

#if 0
#pragma mark -
#endif


void Blob::addSegment(int leftX, int rightX, int y)
{
    //  Create the segment we need to add to our data structure
    HorizontalSegment	*mySeg=new HorizontalSegment;
    mySeg->leftX = leftX;
    mySeg->rightX = rightX;
    mySeg->y = y;
    //
    int segLength = rightX - leftX + 1;
    nbPixels_ += segLength;
    nbHSeg_ ++;
    
    //--------------------------------------------------------------------
    //  Case 0:  If this is the first segment we need to initialize the deque
    //--------------------------------------------------------------------
    if (nbHSeg_ == 1)
    {
        top_ = bottom_ = y;

        list<HorizontalSegment *>   *newLine = new list<HorizontalSegment *>;
        newLine->push_back(mySeg);
        segDeque_.push_back(newLine);

        //  create and initialize the bounding box.  This will allow us to use
        //  if-else statements in computeBoundingRect_
		setValidRect(leftX, y, segLength, 1);
    }
    
    //--------------------------------------------------------------------
    //  Case 1:  If the new segment is higher than the previous top, then
    //  we must create a new list and insert it at the beginning of the deque
    //--------------------------------------------------------------------
    else if (y < top_)    //  can only be top_ - 1  since the blob is connected
    {
        //  create a new line
        list<HorizontalSegment *>   *newLine = new list<HorizontalSegment *>;
        newLine->push_back(mySeg);
        
        //  insert it at the beginning of the deque
        segDeque_.push_front(newLine);
        
        //  The top is now one pixel higher
        top_--;
    }
    
    //--------------------------------------------------------------------
    //  Case 2:  If the new segment is between the current top and bottom
    //  we can access directly the list of segments for that line and
    //  insert the new segment at the right position there
    //--------------------------------------------------------------------
    else if (y <= bottom_)
    {
        //  direct access to the line corresponding to the value of y
        LineSegDeque::iterator    lineIter;
        lineIter = segDeque_.begin() + (y - top_);
        list<HorizontalSegment*> *theLine = (*lineIter);
        
        //  Find the segment immediately on the right of the new segment
        list<HorizontalSegment*>::iterator  segIter, endSeg = theLine->end();
        for (segIter = theLine->begin(); (segIter!=endSeg) && (leftX>(*segIter)->rightX); segIter++)
        {
            HorizontalSegment   *theSeg;
            theSeg = (*segIter);        
        }
        //  insert on the left of that segment
        theLine->insert(segIter, mySeg);        
    }
    
    //--------------------------------------------------------------------
    //  Case 3:  If the new segment is lower than the previous bottom, then
    //  we must create a new list and append it at the tail of the deque
    //--------------------------------------------------------------------
    else    //  y == bottom_ + 1
    {
        //  create a new line
        list<HorizontalSegment *>   *newLine = new list<HorizontalSegment *>;
        newLine->push_back(mySeg);
        
        //  insert it at the back of the deque
        segDeque_.push_back(newLine);
        
        //  the bottom is now one pixel lower
        bottom_++;
    }
    
    setCentroidIsUpToDate_(false);
    setCenterOfMassIsUpToDate_(false);
    setBoundingRectIsUpToDate_(false);
    contourIsUpToDate_ = true;

    //  if we had a contour, it is not valid anymore.
    //  It would need to be recalculated, but we should not waste the time
    //  to to it now.  It will be done if/when needed
    if (contour_ != NULL)
    {
        delete contour_;
     	contour_ = NULL;   
    }
        
}

void Blob::copyInto(Blob* destBlob)
{
    FAIL_CONDITION( true,
                    kNoCode,
                    "buggy code.  Must be copied from copy constructor");
                    
}




#if 0
#pragma mark -
#endif

bool Blob::contains(const ImagePoint* q) const
{
    FAIL_CONDITION( q == NULL,
                    kNullParameterError,
                    "Null ImagePoint passed to Blob::isInside");

    return contains(q->getX(), q->getY());
}

bool Blob::contains(int x, int y) const
{
    //  no point wasting time if the point is not in the vertical range
    if ((y>=top_) && (y<=bottom_))
    {
        LineSegDeque::const_iterator lineIter = segDeque_.begin() + (y-top_);

        list<HorizontalSegment*>::iterator  segIter, lastSeg = (*lineIter)->end();
        for (segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
        {
            HorizontalSegment   *theSeg;
            theSeg = (*segIter);

            //  if (x, y) belongs to this segment, return true
            if ((x >= theSeg->leftX) && (x<=theSeg->rightX))
                return true;

        }
    }
    //  if the search failed...
    return false;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Geometric operations
//------------------------------------------------------
#endif

void Blob::translate(int dx, int dy)
{
    LineSegDeque::iterator lineIter, lastLine = segDeque_.end();
    for (lineIter = segDeque_.begin(); lineIter != lastLine; lineIter++)
    {
        list<HorizontalSegment*>::iterator  segIter, lastSeg = (*lineIter)->end();
        for (segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
        {
            HorizontalSegment   *theSeg;
            theSeg = (*segIter);

            theSeg->leftX += dx;
            theSeg->rightX += dx;
            theSeg->y += dy;
        }
    }

	DrawableObject2D::translate(dx, dy);
        
    if (contour_ != NULL)
        contour_->translate(dx, dy);
        
}

void Blob::translate(float dx, float dy)
{
	const int	idx = static_cast<int>(roundf(dx)),
				idy = static_cast<int>(roundf(dy));
				
    LineSegDeque::iterator lineIter, lastLine = segDeque_.end();
    for (lineIter = segDeque_.begin(); lineIter != lastLine; lineIter++)
    {
        list<HorizontalSegment*>::iterator  segIter, lastSeg = (*lineIter)->end();
        for (segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
        {
            HorizontalSegment   *theSeg;
            theSeg = (*segIter);

            theSeg->leftX += idx;
            theSeg->rightX += idx;
            theSeg->y += idy;
        }
    }

	DrawableObject2D::translate(dx, dy);
        
    if (contour_ != NULL)
        contour_->translate(dx, dy);
        
}

void Blob::rotate(float angle, const ImagePoint* rotCenter)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


void Blob::applyTransformation(const Transformation2D *theTransf)
{
    FAIL_CONDITION( theTransf == NULL,
                    kNullParameterError,
                    "null Transformation2D passed to Blob::applyTransformation");
    if (theTransf->isRigidTransformation())
    {
        RigidTransformation2D   *rigidTransf = (RigidTransformation2D*) theTransf;
        
        rotate(rigidTransf->getAngle());

        translate(rigidTransf->getTransX(), rigidTransf->getTransY());
	}
    else
        FAIL_CONDITION( true,
                        kFunctionNotImplemented,
                        "Function not implemented yet");
    
    setCentroidIsUpToDate_(false);
    setCenterOfMassIsUpToDate_(false);
    setBoundingRectIsUpToDate_(false);
    contourIsUpToDate_ = true;
}



#if 0
#pragma mark -
#endif



long Blob::getNbPixels(void) const
{
    return nbPixels_;
}


long Blob::getNbPixels(QuadrantIndex quad) const
{
    if (!quadNbPixelsAreUpToDate_)
         computeQuadNbPixels_();
    return quadNbPixels_[quad];
}



const LineSegDeque& Blob::getSegDeque(void) const
{
    return segDeque_;
}

const Contour_I *Blob::getContour(void) const
{
    if (contour_ == NULL || !contourIsUpToDate_)
    {
        computeContour_();
        contourIsUpToDate_ = true;
    }
        
    return contour_;
}    
    

float Blob::diskCoverage(int radius)
{
    //------------------------------------------------------------------------
    //  0.  Update the bounding box if necessary
    //------------------------------------------------------------------------
//    if (!boundingRectIsUpToDate_)
//        computeBoundingRect_();
        
    //  yMin and yMax define the range of feasible absolute y values for the test 
    //  (defined by the intersection of the circle and of the vertical spread of the blob)
    //  relYup & relYdown will store y values relative to the disk's center.

    //------------------------------------------------------------------------
    //  1.  Compute maximum dimensions of the disk and the intersection
    //      of these max dimensions with the blob's bounding box
    //------------------------------------------------------------------------
    const double radius2 = radius*radius;
	const ImageRect* vRect = getValidRect();
    const int cx = (vRect->getLeft() + vRect->getRight())/2,
              cy = (top_ + bottom_)/2;
    const int diskTop = cy - radius,
              diskBottom = cy + radius;
    const int yMin = diskTop < top_ ? top_ : diskTop,
              yMax = diskBottom > bottom_ ? bottom_ : diskBottom;
//              topIndex = yMin - top_;

    //------------------------------------------------------------------------
    //  2.  Run a single loop with 2 indices (one running upward, one downward), 
    //      both starting from the center of the disk.  At each "lattitude" 
    //      (scan line), count the number of blob pixels inside the disk
    //------------------------------------------------------------------------
    bool goingUp = true, goingDown = true;
    //  yUp & yDown are absolute y coordinates for the "up" and "down" scan lines
    int yUp=cy, yDown=cy, 
    //  relYup & relYdown are the y coordinates relative to the top of the blob
    //  They can be used as indices to access the deque
        relYup=yUp-top_, relYdown=yDown-top_;
    long covCount = 0, diskCount = 0;
	    for (int dy=0; dy<radius; dy++, yUp--, relYup--, yDown++, relYdown++)
    {
        //  check whether we went beyond the top or bottom limits
        if (yUp < yMin)
            goingUp = false;
        if (yDown > yMax)
            goingDown = false;
            
        //  compute the half width of the disk at that "lattitude" and add to disk counter
//  Note to myself: This could be spedup by using some form of the Bresenham alg.
        const int dx = static_cast<int>(sqrt(radius2 - dy*dy));
        const int xMin = cx - dx,
                  xMax = cx + dx;
        if (dy !=0)
            diskCount += 4*dx+2;
        else
            diskCount += 2*dx+1;
                
        //  If we are still going up, count the blob pixels that belong to the disk
        if (goingUp)
        {
            int xLeft = xMin, xRight = xMax;
            list<HorizontalSegment*> *upLine = segDeque_[relYup];
            list<HorizontalSegment*>::iterator  segIter, lastSeg = upLine->end();
            bool lineNotDone = true;
            for (segIter = upLine->begin(); (segIter != lastSeg) && lineNotDone; segIter++)
            {
                HorizontalSegment *theSeg = (*segIter);
                
                //  we don't care about segments left of xMin or right of xMax
                if ((theSeg->rightX >= xLeft) && (theSeg->leftX <= xRight))
                {
                    int firstX = xLeft > theSeg->leftX ? xLeft : theSeg->leftX;
                    int lastX;
                    if (xRight < theSeg->rightX)
                    {
                        lastX = xRight;
                        lineNotDone = false;
                    }
                    else
                        lastX = theSeg->rightX;
                        
                    //  update the counter
                    covCount += lastX - firstX + 1;
                    
                    if (lineNotDone)
                        xLeft = lastX + 1;

                }
            }
        }
        
        //  If we are still going down and this is not the central line (we only want
        //  to count it once), count the blob pixels that belong to the disk
        if (goingDown && (dy!=0))
        {
            int xLeft = xMin, xRight = xMax;
            list<HorizontalSegment*> *downLine = segDeque_[relYdown];
            list<HorizontalSegment*>::iterator  segIter, lastSeg = downLine->end();
            bool lineNotDone = true;
            for (segIter = downLine->begin(); (segIter != lastSeg) && lineNotDone; segIter++)
            {
                HorizontalSegment *theSeg = (*segIter);
                
                //  we don't care about segments left of xMin or right of xMax
                if ((theSeg->rightX >= xLeft) && (theSeg->leftX <= xRight))
                {
                    int firstX = xLeft > theSeg->leftX ? xLeft : theSeg->leftX;
                    int lastX;
                    if (xRight < theSeg->rightX)
                    {
                        lastX = xRight;
                        lineNotDone = false;
                    }
                    else
                        lastX = theSeg->rightX;
                        
                    //  update the counter
                    covCount += lastX - firstX + 1;
                    
                    if (lineNotDone)
                        xLeft = lastX + 1;

                }
            }
        }
    }
    
    return static_cast<float>((1.L*covCount)/diskCount);
}


#if 0
#pragma mark -
//--------------------------------------------------------------
#pragma mark rHole filling
//--------------------------------------------------------------
#endif

void Blob::fillHoles(void)
{
	//----------------------------------------------
	//	local variables
	//----------------------------------------------
	deque<vector<int>*>	indexDequeTD,	// a deque of lists(lines) of indices of the
										// first element of a pair to connect
						indexDequeBU;	// a deque of list(lines) of indices of the
										// first element of a pair to connect


	list<HorizontalSegment*>::iterator	leftIter, rightIter;

	const int DEQUE_SIZE = segDeque_.size();
	
cout << "------------------------------------------------------------" << endl;
cout << "-----------------------------TOP_DOWN-----------------------" << endl;
cout << "------------------------------------------------------------" << endl;


cout << "initializing range vect, nb segments on line: " << (segDeque_[0]->size()) << endl;
	//----------------------------------------------
	//	top-down pass
	//----------------------------------------------
	//	use the top line for the initialization of the rangeVect
	vector<int>* rangeVectTD = new vector<int>;		// a vector of ranges of covered area
	const_HSegIterator	tdEndIter = segDeque_[0]->end();
int kSeg=0;
	for (const_HSegIterator segIter = segDeque_[0]->begin();  segIter != tdEndIter; segIter++)
	{
cout << "\tpushing segment " << kSeg << " [" << (*segIter)->leftX << ", " << (*segIter)->rightX
		<< "]" << endl;
kSeg++;
		rangeVectTD->push_back((*segIter)->leftX);
		rangeVectTD->push_back((*segIter)->rightX);
	}
	indexDequeTD.push_back(NULL);
	//	iterate the following lines
	for (int i=1; i<DEQUE_SIZE-1; i++)
	{
		fillHolesOnePass_(&indexDequeTD, &rangeVectTD, segDeque_[i], true);
	}
	indexDequeTD.push_back(NULL);
	
	//cleanup range vect
	rangeVectTD->clear();
	delete rangeVectTD;
	
cout << "------------------------------------------------------------" << endl;
cout << "----------------------------BOTTOM_UP-----------------------" << endl;
cout << "------------------------------------------------------------" << endl;
	//----------------------------------------------
	//	bottom-up pass
	//----------------------------------------------
	//	use the bottom line for the initialization of the rangeVect
	vector<int>* rangeVectBU = new vector<int>;		// a vector of ranges of covered area
	const_HSegIterator	buEndIter = segDeque_[DEQUE_SIZE-1]->end();
	for (const_HSegIterator segIter = segDeque_[DEQUE_SIZE-1]->begin(); segIter != buEndIter; segIter++)
	{
		rangeVectBU->push_back((*segIter)->leftX);
		rangeVectBU->push_back((*segIter)->rightX);
	}
	indexDequeBU.push_front(NULL);
	//	iterate the following lines
	for (int i=DEQUE_SIZE-2; i>0; i--)
	{
		fillHolesOnePass_(&indexDequeBU, &rangeVectBU, segDeque_[i], false);
	}
	indexDequeBU.push_front(NULL);
	
	//cleanup range vect
	rangeVectBU->clear();
	delete rangeVectBU;
	
	
	//----------------------------------------------
	//	only connect those holes that are both top and bottom closed
	//----------------------------------------------
	//	iterate each line
	for (int i=1; i < DEQUE_SIZE-1; i++)
	{
		const vector<int>*	tdVect = indexDequeTD[i];
		const vector<int>*	buVect = indexDequeBU[i];

//cout << "deque line " << i << ", y = " << (*(segDeque_[i]->begin()))->y << endl;
//cout << "\tsegments: ";
//{
//	const_HSegIterator	endIter = segDeque_[i]->end();
//	for (const_HSegIterator segIter = segDeque_[i]->begin(); segIter != endIter; segIter++)
//	{
//		cout << "[" << ((*segIter)->leftX) << ", " << ((*segIter)->rightX) << "]   ";
//	}
//	cout << endl;
//}
//cout << "\ttdVect = {";
//if (tdVect != NULL)
//	for (unsigned int k=0; k<tdVect->size(); k++)
//	{
//		cout << (*tdVect)[k];
//		if (k<tdVect->size()-1)
//			cout << "  ";
//			
//	}	
//cout << "}" << endl;
//cout << "\tbuVect = {";
//if (buVect != NULL)
//	for (unsigned int k=0; k<buVect->size(); k++)
//	{
//		cout << (*buVect)[k];
//		if (k<buVect->size()-1)
//			cout << "  ";
//			
//	}	
//cout << "}" << endl;

		//	only when both indexDeque do not store NULL
		if (tdVect!=NULL && buVect!=NULL)		
		{
			int nbGapsFilled = 0;
			bool stillGapCandidates = true;
			
			unsigned int tdIndex = 0, buIndex = 0;
			int segIndex = 0;

			list<HorizontalSegment*>* currSegList = segDeque_[i];
			//	new list of HSegments for this line
			list<HorizontalSegment*>* newSegList = new list<HorizontalSegment*>();
			
			const_HSegIterator	endIter = currSegList->end();
			for (const_HSegIterator segIter = currSegList->begin(); segIter != endIter; segIter++, segIndex++)
			{
				if (stillGapCandidates)
				{
					//	if the segment (really the gap between this segment and the next) appears 
					//	on both the td and bu vects, then we have to do some filling
					if ((segIndex == (*tdVect)[tdIndex]) && (segIndex == (*buVect)[buIndex]))
					{
						//	record the left endpoint of the fillable sequence
						int newSegLeftX = (*segIter)->leftX;
						
						//	go to the end of the sequence of gaps to fill
						while ((segIndex == (*tdVect)[tdIndex]) && (segIndex == (*buVect)[buIndex]))
						{
//cout << "\t\t\t*segment " << segIndex << " [" << (*segIter)->leftX << ", " << (*segIter)->rightX << 
//		"] deleted" << endl;

							//	delete that segment from the old list
							delete (*segIter);
										
							segIter++; 
							segIndex++;
							tdIndex++;
							buIndex++;
							if ((tdIndex == tdVect->size()) || (buIndex == buVect->size()) )
								stillGapCandidates = false;
							nbGapsFilled++;
						}
					
						//	When we get out of the loop segIter points to the the last segment of the
						//	fillable sequence.  All the other segments have been deleted, so we
						//	simply update this segment's left endpoint. It will be pushed onto
						//	the line list at the top of the loop.
						(*segIter)->leftX = newSegLeftX;
						newSegList->push_back(*segIter);
//cout << "\t\t\tsegment " << segIndex << " updated to [" << (*segIter)->leftX << ", " << (*segIter)->rightX << 
//		"] and pushed" << endl;
					}
					//	If the segment is still on the left side of the first candidates for  filling,
					//	simply add the segment to the new list
					else//if ((segIndex < (*tdVect)[tdIndex]) && (segIndex < (*buVect)[buIndex]))
					{
//cout << "\t\tsegment " << segIndex << " [" << (*segIter)->leftX << ", " << (*segIter)->rightX << 
//		"] pushed on new list" << endl;
			
						newSegList->push_back(*segIter);
					}
				
					//	if the tdIndex lags behind, do some catch-up
					while (stillGapCandidates && (segIndex >= (*tdVect)[tdIndex]))
					{
						tdIndex++;
						if (tdIndex == tdVect->size())
							stillGapCandidates = false;
							
					}
						
					//	if the buIndex lags behind, do some catch-up
					while (stillGapCandidates && (segIndex >= (*buVect)[buIndex]))
					{
						buIndex++;

						if (buIndex == buVect->size())
							stillGapCandidates = false;
					}
				}

				//	if there are no gap candidates left, simply push the segment onto the new list
				else
				{
//cout << "\t\tsegment " << segIndex << " [" << (*segIter)->leftX << ", " << (*segIter)->rightX << 
//		"] pushed on new list" << endl;
					newSegList->push_back(*segIter);
				}				
			}
			
			//	we can delete the old list: All the segments that needed to be deleted
			//	have been deleted already
			delete currSegList;
			
			//	record the new list
			segDeque_[i] = newSegList;
			
			//	updated nbHSeg_ for the blob
			nbHSeg_ -= nbGapsFilled;
		}
		else {
//cout << "\t--> line list unchanged" << endl;;
		}

	}

	//----------------------------------------------
	//	garbage collection
	//----------------------------------------------
	for (int i=0; i < DEQUE_SIZE; i++)
	{
		delete indexDequeTD[i];
		delete indexDequeBU[i];
	}
	indexDequeTD.clear();
	indexDequeBU.clear();

	//----------------------------------------------
	//	mark changed variables
	//----------------------------------------------
	setCenterOfMassIsUpToDate_(false);
}


void Blob::fillHolesOnePass_(deque<vector<int> *>* indexDeque, vector<int>** rangeVect,
							 list<HorizontalSegment*>* currSegList, bool topDown)
{
	//--------------------------------------------------------------------------
	//	local variables
	//--------------------------------------------------------------------------
	//	This vector keeps the indices of segments candidates for being
	//	connected to the one following them
	vector<int>* indexList = NULL;
	//
	//	The new range vect that we are going to build
	vector<int>* prevRangeVect = *rangeVect;
	vector<int>* newRangeVect = new vector<int>();
	unsigned int	rangeIndex = 0;
	bool noRangeLeft = false;

	//--------------------------------------------------------------------------
	//	Define iterators for the list of HSegments on the current line
	//--------------------------------------------------------------------------
	//	Define const iterators to the first and second elements, and to the end of the list
	const_HSegIterator  beginSegIter = currSegList->begin();
	const_HSegIterator  secondSegIter = beginSegIter;
	secondSegIter++;
	const_HSegIterator  endSegIter = currSegList->end();

//int lineY = (*beginSegIter)->y;
//cout << "Processing line y=" << lineY << endl;
//cout << "\tcoming in, range vect = {";
//for (unsigned int vk=0; vk < prevRangeVect->size()/2; vk++)
//	cout << " [" << (*prevRangeVect)[2*vk] << ", " << (*prevRangeVect)[2*vk+1] << "] ";
//cout << "}" << endl;;

	//--------------------------------------------------------------------------
	//	Skip all ranges on old range vect on the left of the first segment
	//--------------------------------------------------------------------------
	for ( ; (rangeIndex < prevRangeVect->size()) && (*prevRangeVect)[rangeIndex+1] < (*beginSegIter)->leftX; rangeIndex+=2)
	{
	}
	int	rangeLeft, rangeRight;
	if (rangeIndex < prevRangeVect->size()-1)
	{
		rangeLeft = (*prevRangeVect)[rangeIndex];
		rangeRight = (*prevRangeVect)[rangeIndex+1];
	}
	else 
	{
		noRangeLeft = true;
	}

	
	//--------------------------------------------------------------------------
	//	Iterate over segments on the curent line
	//--------------------------------------------------------------------------
	//	If the gap between two consecutive segments is completely contained within a valid
	//	range of the previous line, we have a fillable gap candidate *for this direction*
	int segIndex = 0;
	for (const_HSegIterator segIter=beginSegIter, nextSegIter=secondSegIter; segIter != endSegIter; segIter++, nextSegIter++, segIndex++)
	{
		int segLeft = (*segIter)->leftX;
		int segRight = (*segIter)->rightX;
		
//cout << "\t\tsegment # " << segIndex << " --> ["  << segLeft << ", " << segRight << "]" << endl;
		
		newRangeVect->push_back(segLeft);

//cout << "\t\t\tpushing "  << (*newRangeVect)[newRangeVect->size()-1] << " on range vect" << endl;

		//	If the segment finishes before the start of the range, this segment
		//	is a full range, the gap is not a candidate for filling, 
		//	move to the next segment
		if (segRight < rangeLeft)
		{
			newRangeVect->push_back(segRight);
//			cout << "\t\t\tpushing "  << (*newRangeVect)[newRangeVect->size()-1] << " on range vect (end of segment, no gap overlap)" << endl;
		}
		//	The segment's left end point is inside the range (we already skipped
		//	all the ranges on the left of the segment)
		else 
		{
			//	We may have a gap with overlap
			bool processingGap = true;
			while (processingGap && (nextSegIter != endSegIter) )
			{
				//	first, consummate all ranges until the next segment
				for ( ; (rangeIndex < prevRangeVect->size()) && rangeRight < segRight; )
				{
					rangeIndex+=2;
					rangeLeft = (*prevRangeVect)[rangeIndex];
					rangeRight = (*prevRangeVect)[rangeIndex+1];
//	cout << "\t\t\tconsummating range " << (rangeIndex/2) << endl;
				}
				//	we have consummated all the ranges from the previous line
				if (rangeIndex > prevRangeVect->size()-1)
				{
					noRangeLeft = true;
				}

				//	If the next segment starts after the range, then the gap extends beyond the
				//	end of the range and is not a candidate for filling.  The rightmost point
				//	of the current segment defines the end of the new range
				if (noRangeLeft || (*nextSegIter)->leftX > rangeRight)
				{
					newRangeVect->push_back(segRight);
					processingGap = false;
//cout << "\t\t\tpushing "  << (*newRangeVect)[newRangeVect->size()-1] << " on range vect" << endl;
				}
				//	otherwise, the gap was completely included in the previous range. 
				//	we add it to the list of candidates for filling and move on to the next segment
				else
				{
//cout << "\t\t\t(absorbing (and pushing on list) gap " << segIndex << ") " << endl;
					if (indexList == NULL)
						indexList = new vector<int>;
					indexList->push_back(segIndex);
					segIter++;
					nextSegIter++;
					segIndex++;
					segLeft = (*segIter)->leftX;
					segRight = (*segIter)->rightX;
				}
			}
			
			//	if we got out of the loop on the last segment, then we push the end of this segment
			//	on the new range and get out
			if (nextSegIter == endSegIter)
			{
				newRangeVect->push_back(segRight);
//cout << "\t\t\tpushing "  << (*newRangeVect)[newRangeVect->size()-1] << " on range vect (last segment)" << endl;
			}
		}
	}

	//	insert indexList, so that line-line comparison can be done
	//	between indexDequeTD and indexDequeBU in func fillHoles()
	if (topDown)
	{
		indexDeque->push_back(indexList);
	}
	
	else	// bottom-up
	{
		indexDeque->push_front(indexList);
	}
	
										
	//	update the range vector
	*rangeVect = newRangeVect;
}


#if 0
#pragma mark -
//--------------------------------------------------------------
#pragma mark Computation of blob's geometric features
//--------------------------------------------------------------
#endif

//----------------------------------
//  protected Functions
//----------------------------------

void Blob::computeContour_(void) const
{
	if (contour_ != NULL)
		delete contour_;
	
    if (nbHSeg_ > 0)
    {
    	int	nbPts=0;
		//	The list of points we build the contour into
    	ImagePointList ptsList;
		
        LineSegDeque::const_iterator	currLineIter, nextLineIter,
										firstLineIter = segDeque_.begin(),
										belowLastLineIter = segDeque_.end();
		//	iterator referring to the current segment
    	list<HorizontalSegment *>::iterator	currSegIter;
		
		//	reverse iterator used in bottom-up direction
		list<HorizontalSegment *>::reverse_iterator	currRevSegIter;    	
    	
		//	Will store a pointer to the first segment.  Used to check whether loop is complete.
		HorizontalSegment* firstSeg;
											
        //------------------------------
        //	initialization
        //------------------------------
		currLineIter = nextLineIter = firstLineIter;
        currSegIter = (*currLineIter)->begin();
		firstSeg = *((*currLineIter)->begin());
	
		//	coordinates of the start/current segment
		int currLeftX = (*currSegIter)->leftX;
        int currRightX = (*currSegIter)->rightX;
        int currY = (*currSegIter)->y;
        
        //------------------------------
        //	only one segment
        //------------------------------
        nextLineIter++; 
        if (nextLineIter == belowLastLineIter)
        {
			//	the contour consists of the four "corners" of the segment 
			//	(in "pixel as a little square" view
        	ptsList.push_back(new ImagePoint(currLeftX, currY));
        	ptsList.push_back(new ImagePoint(currLeftX, currY+1));
			ptsList.push_back(new ImagePoint(currRightX+1, currY+1));
			ptsList.push_back(new ImagePoint(currRightX+1, currY));
		}
		
		//------------------------------
		//	more than one segment
		//------------------------------
		else
		{
			bool	justStarted = true;	//	false after first seg has been processed
			bool	stillGoing = true;	//	condition for the outer loop
			//	The last two are opposite of each other, redundancy chosen for 
			//	increased readability
			bool	topDown = true,
					bottomUp = false;

			//	get all the contour points
			while (stillGoing)
			{
				//============================================================
				//	The search for the contour consists in a repetition of
				//	[top-down travel, then bottom-up travel] until we have
				//	returned to the starting point
				//============================================================

				
				//============================================================
				//	Part I
				//	Loop top-down direction
				//============================================================
				while (topDown)
				{
 					//	coordinates of the current segment
					currY = (*currSegIter)->y;
					currLeftX = (*currSegIter)->leftX;
					currRightX = (*currSegIter)->rightX;

					//	outer loop over if coming back to the starting point 
					if (justStarted)
						justStarted = false;
					else
					{
						if ((*currSegIter) == firstSeg)
						{
							stillGoing = false; // already finished computing contour
							break;		
						}
					}
						
					//	add upper-left vertex to ptsList
					ptsList.push_back(new ImagePoint(currLeftX, currY));
					
					//----------------------------------------------------
					//	find leftBelowSegIter (prepare for the loop below)
					//----------------------------------------------------

					//	iterator referring to the leftmost segment
					//	connected to current in line below (when top-down)
					list<HorizontalSegment *>::iterator	leftBelowSegIter;
					
					//	x coordinates of candidates "left below" segments
					int lfLeftBelow, rtLeftBelow;
					bool lbSegFound = false;				

					nextLineIter = currLineIter + 1;
					// before reaching the bottom line
					if (nextLineIter != belowLastLineIter)		
					{
						list<HorizontalSegment *>::iterator endSegIter = (*nextLineIter)->end();

						//	going from front to back to find the leftBelowSegIter (leftmost)
						for (leftBelowSegIter = (*nextLineIter)->begin();
							 !lbSegFound && leftBelowSegIter != endSegIter;
							 leftBelowSegIter++)
						{
							lfLeftBelow = (*leftBelowSegIter)->leftX;
							rtLeftBelow = (*leftBelowSegIter)->rightX;
							
							//	find the leftBelowSegIter (leftBelowSegIter should always be found)
							if ((lfLeftBelow>=currLeftX && lfLeftBelow<=currRightX)
								|| (rtLeftBelow>=currLeftX && rtLeftBelow<=currRightX)
								|| (lfLeftBelow<=currLeftX && rtLeftBelow>=currRightX))
							{
								lbSegFound = true;
								break;
							}
						}
					}
					//	no leftBelowSegIter (incl. reaching the bottom line), add the lower-left corner
					//	to the contour and invert travel direction
					if (!lbSegFound)
					{
						ptsList.push_back(new ImagePoint(currLeftX, currY+1));

						//	I can't get the @$#%$ regular-to-inverse iterator conversion to work, 
						//	so I had to code my own
						for (list<HorizontalSegment *>::reverse_iterator revIter = (*currLineIter)->rbegin();
							revIter != (*currLineIter)->rend(); revIter++)
						{
							if ((*revIter) == (*currSegIter)) {
								currRevSegIter = revIter;
								break;
							}
						}

						topDown = false;
						bottomUp = true;
						currLineIter--;
					}
					
					//---------------------------------------------------------------------
					//	Even if a left-below segment was found and we are still marked as
					//	going "top-down", we may still have to
					//	invert the direction if that left-below segment is top-connected
					//	with the current segment's left neighbor
					//---------------------------------------------------------------------
					if (topDown)
					{
						//	We enter this block because leftBelowSegIter refers to a valid
						//	segment, which is the "below left-most" connected to the current seg.
						//	lfLeftBelow and rtLeftBelow store the x bounds of that segment
						
						//	If the current segment does not have a left neighbor enter the 
						//	"lower-left corner of this segment to the contour (unless it coincides with 
						//	the upper-left of leftBelowSegIter) and go to leftBelowSegIter
						if (currSegIter == (*currLineIter)->begin())
						{
							if (currLeftX != (*leftBelowSegIter)->leftX) {
						
								ptsList.push_back(new ImagePoint(currLeftX, currY+1));
							}
							currSegIter = leftBelowSegIter;
						}			
										
						//	If the current segment has a left neighbor, we have to check whether
						//	leftBelowSegIter is top-connected to that neighbor
						else
						{	
							//	iterator referring to the segment on the left of current
							list<HorizontalSegment *>::iterator	currLeftSegIter = currSegIter;
							currLeftSegIter--;
							
							//	if leftBelowSegIter is top-connected to currLeftSegIter, 
							//	invert scan direction to go up
							int rtLeft = (*currLeftSegIter)->rightX;
							if (lfLeftBelow <= rtLeft)
							{
								//	fill ptsList & update currSegIter
								ptsList.push_back(new ImagePoint(currLeftX, currY+1));

								currSegIter = currLeftSegIter;
								
								//	I can't get the @$#%$ regular-to-inverse iterator conversion to work, 
								//	so I had to code my own
								for (list<HorizontalSegment *>::reverse_iterator revIter = (*currLineIter)->rbegin();
									revIter != (*currLineIter)->rend(); revIter++)
								{
									if ((*revIter) == (*currSegIter)) {
										currRevSegIter = revIter;
										break;
									}
								}

								topDown = false;		// turn to bottomUp direction
								bottomUp = true;
								break;
							}
							
							//	otherwise, enter the  "lower-left corner of this segment to the 
							//	contour and keep going down to leftBelowSegIter
							else
							{
								ptsList.push_back(new ImagePoint(currLeftX, currY+1));
							
								//	fill ptsList & update currSegIter
								currSegIter = leftBelowSegIter;
							}
						}
					}
					
					currLineIter++;
				}				
			
				//============================================================
				//	Part II
				//	loop bottom-up direction
				//============================================================
				
				while (bottomUp)
				{
					//	in this direction we need to work right-to-left, so we use the 
					//	reverse iterator currRevSegIter that was initialized when we inverted
					//	the direction from top-down, and upodated afterwards within this loop

					//	coordinates of the current segment
					currY = (*currRevSegIter)->y;
					currLeftX = (*currRevSegIter)->leftX;
					currRightX = (*currRevSegIter)->rightX;

					//	add lower-right vertex to ptsList
					ptsList.push_back(new ImagePoint(currRightX+1, currY+1));
					
					//-------------------------------------------------------
					//	find rightAboveSegIter (prepare for the loop below)
					//-------------------------------------------------------

					//	rev_iterator referring to the rightmost segment 
					//	connected to current in line above (when bottom up)
					list<HorizontalSegment *>::reverse_iterator	rightAboveSegIter;

					//	x coordinates of "right above" segment
					int lfRightAbove, rtRightAbove;
					bool raSegFound = false;

					nextLineIter = currLineIter;
					if (currLineIter != firstLineIter)		// before reaching the top line
					{
						nextLineIter--;

						list<HorizontalSegment *>::reverse_iterator	endSegIter = (*nextLineIter)->rend();
						
						for (rightAboveSegIter = (*nextLineIter)->rbegin(); 
							 !raSegFound && (rightAboveSegIter != endSegIter); 
							 rightAboveSegIter++)
						{
							lfRightAbove = (*rightAboveSegIter)->leftX;
							rtRightAbove = (*rightAboveSegIter)->rightX;
								
							//	rightAboveSegIter is top-connected to currSegIter
							if ((lfRightAbove>=currLeftX && lfRightAbove<=currRightX)
								|| (rtRightAbove>=currLeftX && rtRightAbove<=currRightX)
								|| (lfRightAbove<=currLeftX && rtRightAbove>=currRightX))
							{
								raSegFound = true;
								break;
							}
						}
					}
					//	rightAboveSegIter not found, invert travel direction
					if (!raSegFound)
					{
						//	add upper-right vertex to ptsList
						ptsList.push_back(new ImagePoint(currRightX+1, currY));

						topDown = true;
						bottomUp = false;
						
						//	we must go back from a reverse iterator to a forward iterator
						//	I can't get the @$#%$ inverse-to-regular iterator conversion to work, 
						//	so I had to code my own
						for (list<HorizontalSegment *>::iterator iter = (*currLineIter)->begin();
							iter != (*currLineIter)->end(); iter++)
						{
							if ((*iter) == (*currRevSegIter)) {
								currSegIter = iter;
								break;
							}
						}
						
						//	to cancel out the -- done in the loop
						currLineIter++;

					}
					
					//---------------------------------------------------------------------
					//	Even if a right-above segment was found and we are still marked
					//	as going "bottom-up", we may still have to
					//	invert the direction if that right-above segment is bottom-connected
					//	with the current segment's right neighbor
					//---------------------------------------------------------------------
					
					if (bottomUp) 
					{
						//	We enter this block because rightAboveSegIter refers to a valid
						//	segment, which is the "above right-most" connected to the current seg.
						//	lfRightAbove and rtRightAbove store the x bounds of that segment
					
						//	If the current segment does not have a right neighbor enter the 
						//	"upper-right corner of this segment to the contour (unless it coincides with 
						//	the lower-right of rightAboveSegIter) and go to rightAboveSegIter
						if (currRevSegIter == (*currLineIter)->rbegin())
						{
							if (currRightX != (*rightAboveSegIter)->rightX) {
								ptsList.push_back(new ImagePoint(currRightX+1, currY));
							}
							currRevSegIter = rightAboveSegIter;
						}			
										
						//	If the current segment has a right neighbor, we have to check whether
						//	rightAboveSegIter is top-connected to that neighbor
						else
						{	
							//	iterator referring to the segment on the right of current
							list<HorizontalSegment *>::reverse_iterator	currRightSegIter = currRevSegIter;
							currRightSegIter--;
				
							//	if rightAboveSegIter is bottom-connected to currRightSegIter, 
							//	invert scan direction to go down
							int lfRight = (*currRightSegIter)->leftX;
							if (rtRightAbove >= lfRight)
							{
								//	fill ptsList & update currSegIter
								ptsList.push_back(new ImagePoint(currRightX+1, currY));

								currRevSegIter = currRightSegIter;

								//	we must go back from a reverse iterator to a forward iterator
								//	I can't get the @$#%$ inverse-to-regular iterator conversion to work, 
								//	so I had to code my own
								for (list<HorizontalSegment *>::iterator iter = (*currLineIter)->begin();
									iter != (*currLineIter)->end(); iter++)
								{
									if ((*iter) == (*currRevSegIter)) {
										currSegIter = iter;
										break;
									}
								}
								
								topDown = true;		// turn to top-down direction
								bottomUp = false;
								break;
							}
							
							//	otherwise, enter the  "upper-right corner of this segment to the 
							//	contour and keep going down to rightAboveSegIter
							else
							{
								ptsList.push_back(new ImagePoint(currRightX+1, currY));
							
								//	fill ptsList & update currSegIter
								currRevSegIter = rightAboveSegIter;
							}
						}
					}

					currLineIter--;
				}
			}
		}
    	
    	//	create contour_      
    	nbPts = static_cast<int>(ptsList.size());
    	contour_ = new Contour_I(&ptsList);
    	
    	//	garbage collection
        ImagePointList::iterator iter = ptsList.begin();
        for( ;iter!=ptsList.end(); iter++)
            delete (*iter);
        ptsList.clear();
    }
    
    contourIsUpToDate_ = true;
}


void Blob::computeBoundingRect_(ImageRect& rect) const
{
	int     bLeft, bRight;

	//  The top and bottom of the box were updated each time a segment was added.
	bLeft = IMAGE_SPACE_BOUND;
	bRight = -IMAGE_SPACE_BOUND;

	LineSegDeque::const_iterator lineIter, lastLine = segDeque_.end();
	for (lineIter = segDeque_.begin(); lineIter != lastLine; lineIter++)
	{
		list<HorizontalSegment*>::iterator  segIter, lastSeg = (*lineIter)->end();
		for (segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
		{
			HorizontalSegment   *theSeg;
			theSeg = (*segIter);

/*
			if (theSeg->y < bTop)
			{
				bTop = theSeg->y;
				boxHasChanged = true;
			}
			else if (theSeg->y > bBottom)
			{
				bBottom = theSeg->y;
				boxHasChanged = true;
			}
*/
			if (theSeg->leftX < bLeft)
				bLeft = theSeg->leftX;

			else if (theSeg->rightX > bRight)
				bRight = theSeg->rightX;

		}
	}

    rect.setRect(bLeft, top_, bRight-bLeft+1, bottom_ - top_ + 1);
}


void Blob::computeValidRect_(ImageRect& rect) const
{
	rect.setRect(getBoundRect());
}


//This implementation seems to give a reasonable estimation, but is not perfect
//Perhaps it could be improved
void Blob::computeCentroid_(ImagePoint_F& cent) const
{
	const ImageRect* bRect = getBoundRect();
	cent.setCoordinates((bRect->getLeft() + bRect->getRight())/2,
						(bRect->getTop() + bRect->getBottom())/2);

}


//This implementation seems to give a reasonable estimation, but is not perfect
//Perhaps it could be improved
void Blob::computeCenterOfMass_(ImagePoint_F& cent) const
{
	long cx = 0, cy = 0;
	long a=0;
	//
	LineSegDeque::const_iterator lineIter, lastLine = segDeque_.end();
	for (lineIter = segDeque_.begin(); lineIter != lastLine; lineIter++)
	{
		list<HorizontalSegment*>::iterator  segIter, lastSeg = (*lineIter)->end();
		for (segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
		{
			HorizontalSegment   *theSeg;
			theSeg = (*segIter);

			//+-1 compensates for pixel-based math
			int n = theSeg->rightX - theSeg->leftX+1;
			cx += theSeg->rightX*(theSeg->rightX+1) -
				  theSeg->leftX*(theSeg->leftX-1);
			cy += theSeg->y*n;
			a += n;
		}
	}
	double invA = 1.L / a;
	cent.setCoordinates(static_cast<float>(0.5L*cx*invA), static_cast<float>(cy*invA));

}


void Blob::computeQuadNbPixels_(void) const
{
    const ImageRect* rect = getBoundRect();
    const int   midI = (rect->getTop() + rect->getBottom())/2,
                midJ = (rect->getLeft() + rect->getRight())/2;
    const int   halfTop[2] = {  rect->getTop(), midI+1},
                halfBottom[2] = { midI, rect->getBottom()};
    
    quadNbPixels_[0] = quadNbPixels_[1] = quadNbPixels_[2] = quadNbPixels_[3] = 0;
    
    //  Work on the top half, then the bottom half
    for (int qHalf=0; qHalf<2; qHalf++)
    {
        LineSegDeque::const_iterator lineIter = segDeque_.begin() + halfTop[qHalf];
    
        for (int i=halfTop[qHalf]; i<=halfBottom[qHalf]; i++, lineIter++)
        {
            //  reinitialize q to be the left quadrant of the half being processed
            int  q = 2*qHalf, startX, endX;
            bool leftMode = true, segRightHalf = false;   
            list<HorizontalSegment*>::iterator  segIter = (*lineIter)->begin(), 
                                                endSeg = (*lineIter)->end();
            while (segIter != endSeg)
            {
                HorizontalSegment   *theSeg = (*segIter);

                //  We are processing the left half, 
                if (leftMode)
                {
                    startX = theSeg->leftX;
                    
                    //  if the segment is entirely on the left half, just process the
                    //  whole segment and move to the next
                    if (theSeg->rightX <= midJ)
                    {
                        endX = theSeg->rightX;
                        segIter++;
                    }
                    else
                    {
                        //  this segment is completely on the right side
                        if (theSeg->leftX > midJ)
                        {
                            //  we are not in left mode anymore; data contribute to 
                            //  right half calculations
                            leftMode = false;
                            q++;
                            endX = theSeg->rightX;
                            segIter++;
                        }
                        //  if we have a split segment, this will be processed in two
                        //  passes of the loop.  We set flags to spot this case in the
                        //  next pass.
                        else
                        {
                            leftMode = false;
                            segRightHalf = true;
                            endX = midJ;
                        }
                    }
                } 
                //  if we are in the right mode, we only have to check if we are completing
                //  the right half of a straddling segment
                else
                {
                    endX = theSeg->rightX;
                    
                    //  if we are processingt the right half of a straddling segment, then 
                    //  we start from the middle border and start adding to the right half
                    if (segRightHalf)
                    {
                        segRightHalf = false;
                        startX = midJ + 1;
                        q++;
                    }
                    else
                        startX = theSeg->leftX;
                    
                    segIter++;
                }
                
                //  increment the pixel counter for the quadrant
                quadNbPixels_[q] += endX - startX + 1;
            }
        }
    }

    quadNbPixelsAreUpToDate_ = true;
}

#if 0
#pragma mark -
#endif

void Blob::renderInto(RasterImage* img, bool clearFirst)
{
	FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"Work in progress");

	try {
		switch (img->getBaseRasterType())
		{
			case kGrayRaster: 
			{
				unsigned char* gray = dynamic_cast<RasterImage_gray*>(img)->getGray(R_W_ACCESS);
//				unsigned char* const* gray2D = dynamic_cast<RasterImage_gray*>(img)->getGray2D(R_W_ACCESS);
				if (clearFirst)
				{
					const long byteCount = img->getTotalByteCount();
					for (long l=0; l<byteCount; l++)
						gray[l] = static_cast<unsigned char> (0);
				}
			}
			break;
				
			case kRGBa32Raster:
			{
			}
			break;
				
			default:
				FAIL_CONDITION (true,
								kInvalidParameterError,
								"Only gray-level and RGBa images currently supported by Blob::renderInto");
			break;
		}
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by Blob::renderInto");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION (img == NULL,
						kNullParameterError,
						"Null pointer to a RasterImage passed to Blob::renderInto");
		FAIL_CONDITION (true,
						kInvalidParameterError,
						"Invalid pointer to a RasterImage passed to Blob::renderInto");
	}
}


void Blob::renderContour(float theZ, bool shifted, const ImagePoint* origin) const
{
	getContour()->render(theZ,  shifted, origin);
}


void Blob::render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
					float theZ, bool shifted, const ImagePoint* origin) const
{
	const int	left = getLeft(),
				top = getTop();

    //--------------------------------------------------------------
    //	Part 1: render the blob
    //--------------------------------------------------------------
    glColor3fv(getColor());
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(scaledLeft, scaledTop, theZ);
	glScalef(getRenderingScaleX(), -getRenderingScaleY(), 1.f);
	
    glBegin(GL_QUADS);

        LineSegDeque::const_iterator lastLine = segDeque_.end();
        for (LineSegDeque::const_iterator lineIter = segDeque_.begin(); lineIter != lastLine; lineIter++)
        {
            list<HorizontalSegment*>::const_iterator lastSeg = (*lineIter)->end();
            for (list<HorizontalSegment*>::const_iterator segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
            {
                HorizontalSegment   *theSeg;
                theSeg = (*segIter);
                glVertex2i(theSeg->leftX - left, theSeg->y - top);
                glVertex2i(theSeg->leftX - left, theSeg->y+1 - top);
                glVertex2i(theSeg->rightX+1 - left, theSeg->y+1 - top);
                glVertex2i(theSeg->rightX+1 - left, theSeg->y - top);
            }
        }

    glEnd();
    glPopMatrix();

    //--------------------------------------------------------------
    //	Part 2: if needed, draw the blob's contour
    //--------------------------------------------------------------
    if (drawContour_)
	{
//        getContour()->render(theZ,  shifted, origin);
        getContour()->render(true);
	}
	
}


void Blob::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
	//	I should probably cull the segments/pixels that fall out of the
	//	bounding box
}



void Blob::setValidRect_(int x, int y, int theWidth, int theHeight)
{
	//	I should probably cull the segments/pixels that fall out of the
	//	bounding box
}

#if 0
#pragma mark -
//--------------------------------------------------------------
#pragma mark rendering setters
//--------------------------------------------------------------
#endif


void Blob::setContourColor(float red, float green, float blue) const
{
    if (contour_ == NULL || !contourIsUpToDate_)
    {
        computeContour_();
        contourIsUpToDate_ = true;
    }
    contour_->setColor(red, green, blue);
}

void Blob::setContourColor(const float theColor[]) const
{
    if (contour_ == NULL || !contourIsUpToDate_)
    {
        computeContour_();
        contourIsUpToDate_ = true;
    }
    contour_->setColor(theColor);
}


void Blob::setContourColor(unsigned char red, unsigned char green, unsigned char blue) const
{
    if (contour_ == NULL || !contourIsUpToDate_)
    {
        computeContour_();
        contourIsUpToDate_ = true;
    }
    contour_->setColor(red, green, blue);
}

void Blob::setContourColor(const unsigned char theColor[]) const
{
    if (contour_ == NULL || !contourIsUpToDate_)
    {
        computeContour_();
        contourIsUpToDate_ = true;
    }
    contour_->setColor(theColor);
}

void Blob::setDrawContour(bool drawContour)
{
    drawContour_ = drawContour;
    if (drawContour_ && (contour_ == NULL || !contourIsUpToDate_))
    {
        computeContour_();
        contourIsUpToDate_ = true;
    }
        
}


#if 0
#pragma mark -
#endif

void Blob::deleteSegments_(void)
{
    LineSegDeque::iterator lineIter, lastLine = segDeque_.end();
    for (lineIter = segDeque_.begin(); lineIter != lastLine; lineIter++)
    {
        list<HorizontalSegment*> *segList = (*lineIter);
        list<HorizontalSegment*>::iterator  segIter, lastSeg = segList->end();
        for (segIter = segList->begin(); segIter != lastSeg; segIter++)
            delete (*segIter);
            
        segList->clear();
        delete segList;
    }
    segDeque_.clear();
}

