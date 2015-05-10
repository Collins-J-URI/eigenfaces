/*  NAME:
        BlobFinder.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib BlobFinder class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <stack>
#include <iostream>
#include <cstdlib>
//
#include "BlobFinder.h"

using namespace std;
using namespace uriVL;


//------------------------------------------------------------------------------
//  Redefinition of constants
//------------------------------------------------------------------------------

const long BlobFinder::MAX_LONG_COUNT = 1L << (8*sizeof(long) - 2);


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


BlobFinder::BlobFinder(void) 
        :   //FeatureDetector(),
            //
			label_(NULL)
{
}

//BlobFinder::BlobFinder(const LabelRaster* labelRaster) 
//		try	:   FeatureDetector(),
//				//
//				label_(NULL),
//				validRect_(NULL)
//{
//}
//
//BlobFinder::BlobFinder(const ImageRect* rect) 
//		try	:   FeatureDetector(),
//				//
//				label_(NULL),
//				validRect_(NULL)
//{
//}


BlobFinder::BlobFinder(const BlobFinder& theObj)
        :   //FeatureDetector(),
            //
			label_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "BlobFinder copy constructor not implemented.");
}


BlobFinder::~BlobFinder(void)
{
}


const BlobFinder& BlobFinder::operator = (const BlobFinder& theBox)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "BlobFinder copy operator not implemented.");

	return *this;
}

#if 0
#pragma mark -
//------------------------------------------------------------------------------
#pragma mark Implementation of FeatureDetector search functions
//------------------------------------------------------------------------------
#endif

ImageFeature* BlobFinder::findFeatureAbout(const RasterImage* img, const ImagePoint* searchStart,
										   const ImageRect* searchRect)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "BlobFinder::findFeatureAbout not implemented.");
	return NULL;
}

FeatureList* BlobFinder::findAllFeatures(const RasterImage* img, const ImageRect* searchRect)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "BlobFinder::findAllFeatures not implemented.");
	return NULL;
}
											




#if 0
#pragma mark -
//------------------------------------------------------------------------------
#pragma mark public filling/search functions
//------------------------------------------------------------------------------
#endif

// Detect all the blobs in the input label raster from a list of target indices
//
BlobList* BlobFinder::findAllBlobs(const LabelRaster* label, int n, const char target[], 
                                   long theMinSize, bool fillHoles)
{
    FAIL_CONDITION( n <=0,
                    kInvalidParameterError,
                    "number of targets has to be greater than 0 in BlobFinder::findAllBlobs");

    BlobList* blobL = NULL;
	long*	blobMinSize = new long[n];
	long*	blobMaxSize = new long[n];
	for (int i=0; i<n; i++)
	{
		blobMinSize[i] = theMinSize;
		blobMaxSize[i] = MAX_LONG_COUNT;
	}

    try
	{
		blobL = findAllBlobs(label, n, target, blobMinSize, blobMaxSize, label->getValidRect(), fillHoles);
	
		delete []blobMaxSize;
		delete []blobMinSize;
	}
	catch (ErrorReport& e) 
	{
		e.appendToMessage("called by BlobFinder::findAllBlobs.");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( label == NULL,
						kNullRasterImageError,
						"null LabelRaster as parameter in BlobFinder::findAllBlobs");
		FAIL_CONDITION( true,
						kNoCode,
						"Unknown error encountered in BlobFinder::findAllBlobs");
	}

    return blobL;
}


// Detect all the blobs in the input label raster from a list of target indices
//
BlobList* BlobFinder::findAllBlobs(const LabelRaster* label, int n, const char target[],
                                   long theMinSize, const ImageRect* mySearchRect,
								   bool fillHoles)
{
    FAIL_CONDITION( n <=0,
                    kInvalidParameterError,
                    "number of targets has to be greater than 0 in BlobFinder::findAllBlobs");

    BlobList* blobL = NULL;
	long*	blobMinSize = new long[n];
	long*	blobMaxSize = new long[n];
	for (int i=0; i<n; i++)
	{
		blobMinSize[i] = theMinSize;
		blobMaxSize[i] = MAX_LONG_COUNT;
	}

    try
	{
		blobL = findAllBlobs(label, n, target, blobMinSize, blobMaxSize, mySearchRect, fillHoles);
	
		delete []blobMaxSize;
		delete []blobMinSize;
	}
	catch (ErrorReport& e) 
	{
		e.appendToMessage("called by BlobFinder::findAllBlobs.");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( label == NULL,
						kNullRasterImageError,
						"null LabelRaster as parameter in BlobFinder::findAllBlobs");
		FAIL_CONDITION( true,
						kNoCode,
						"Unknown error encountered in BlobFinder::findAllBlobs");
	}
			
    return blobL;
}


// Detect all the blobs in the input label raster from a list of target indices
//
BlobList* BlobFinder::findAllBlobs(const LabelRaster* label, int n, const char target[],
                                   const long* blobMinSize, const long* blobMaxSize, const ImageRect* theRect,
								   bool fillHoles)
{
    FAIL_CONDITION( label == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in BlobFinder::findAllBlobs");
    FAIL_CONDITION( target == NULL,
                    kNullArrayError,
                    "null target list in BlobFinder::findAllBlobs");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in BlobFinder::findAllBlobs");
    FAIL_CONDITION( n <=0,
                    kInvalidParameterError,
                    "number of targets has to be greater than 0 in BlobFinder::findAllBlobs");


    ImageRect* searchRect = ImageRect::intersection(label->getValidRect(), theRect);
    FAIL_CONDITION( searchRect == NULL,
                    kNullRectangleError,
                    "search rect out of valid data bounds in BlobFinder::findAllBlobs");

    BlobList* blobL = new BlobList[n];

	try {
	
		const int   iLow = searchRect->getTop(),
					iHigh = searchRect->getBottom(),
					jLow = searchRect->getLeft(),
					jHigh = searchRect->getRight();

		label_ = label->getShiftedLabel2D(R_W_ACCESS);
		ImagePoint	pos;

		for (int i=iLow; i<=iHigh; i++)
		{
			for (int j=jLow; j<=jHigh; j++)
			{
				if (label_[i][j] > 0)
				{
					for (int k=0; k<n; k++)
						if (label_[i][j] == target[k])
						{
							pos.setCoordinates(j, i);

							Blob* myBlob = fillThisBlob_(&pos, searchRect, (char) -1);

							if ((myBlob->getNbPixels() >= blobMinSize[k]) && 
								(myBlob->getNbPixels() <= blobMaxSize[k]))
							{
								(blobL[k]).push_back(myBlob);
							}
							else
							{
								delete myBlob;
							}
						}
				}
			}
		}
    }
	catch (...)
	{
		if (searchRect != NULL)
			delete searchRect;
			
		FAIL_CONDITION( true,
						kImageProcessingError,
						"Error encountered while searching for blobs in BlobFinder::findAllBlobs");
	}

	try {
		if (fillHoles)
		{
			for (int k=0; k<n; k++)
			{
				BlobList::iterator endIter = blobL[k].end();
				for (BlobList::iterator iter = blobL[k].begin(); iter != endIter; iter++)
				{
					(*iter)->fillHoles();
					refillBlobLocation_(*iter, (char) -1);
				}
			}
		}
    }
	catch (...)
	{
		if (searchRect != NULL)
			delete searchRect;
			
		FAIL_CONDITION( true,
						kImageProcessingError,
						"Error encountered while filling holes in BlobFinder::findAllBlobs");
	}

	if (searchRect != NULL)
		delete searchRect;

    return blobL;
}

BlobList** BlobFinder::findAllBlobsInside(const LabelRaster* label, BlobList *blobL, const char target[], long blobMinSize)
{
	//Some fail conditions for this function
	FAIL_CONDITION( label == NULL,
					kNullRasterImageError,
					"null LabelRaster as a parameter to BlobFinder::findAllBlobsInside." );
	FAIL_CONDITION( blobL == NULL,
					kNullParameterError,
					"null BlobList as a parameter to BlobFinder::findAllBlobsInside." );
	
	/* Step One:
	 *		Find all blobs in the image that are of the given target.  This 'target' is
	 *		actually a ColorSegParams->noMatchVal which represents a fill number in the
	 *		LabelRaster that is a number other than the BlobList members.  By default,
	 *		when the ColorThresholdLabeler is applied -1's are applied to non-Blob pixels;
	 *		however, when this function is used a number other than -1 should have filled
	 *		the LabelRaster's non-Bloblist member's pixels.
	 */
	BlobList *foundBlobs = findAllBlobs(label, 1, target, blobMinSize, false);	//gather all the blobs of our target
	BlobList::iterator foundIter, bListIter;	//iterators for the two BlobLists we're dealing with
	BlobList **returnedBlobs = new BlobList*[foundBlobs->size()];
	

	for(unsigned int i = 0; i < foundBlobs->size(); i++ )
	{
		BlobList *L = new BlobList;
		returnedBlobs[i] = L;
	}

	bool found;

	int i;
	
	for( foundIter = foundBlobs->begin(); foundIter != foundBlobs->end(); foundIter++ )
	{
		i = 0;
		found = false;
		for( bListIter = blobL->begin(); bListIter != blobL->end(); bListIter++ )
		{
			if( (*bListIter)->getBoundRect()->contains((*foundIter)->getBoundRect()) )
			{
				returnedBlobs[i]->push_back( (*foundIter) );
				found = true;
			}
			i++;
		}

		//after all the blobs have been iterated through all those remaining in the foundBlobs
		//list need to be deleted.
		if( !found )
			delete (*foundIter);
	}
	
	return returnedBlobs;
}


BlobList* BlobFinder::findAllBlobsInside(const LabelRaster* label, Blob* blob, int nbTargets, 
										 const char target[], long blobMinSize)
{
	//Some fail conditions for this function
	FAIL_CONDITION( label == NULL,
					kNullRasterImageError,
					"null LabelRaster as a parameter to BlobFinder::findAllBlobsInside." );
	FAIL_CONDITION( blob == NULL,
					kNullParameterError,
					"null BlobList as a parameter to BlobFinder::findAllBlobsInside." );
	
	/* Step One:
	 *		Find all blobs in the image that are of the given target.  This 'target' is
	 *		actually a ColorSegParams->noMatchVal which represents a fill number in the
	 *		LabelRaster that is a number other than the BlobList members.  By default,
	 *		when the ColorThresholdLabeler is applied -1's are applied to non-Blob pixels;
	 *		however, when this function is used a number other than -1 should have filled
	 *		the LabelRaster's non-Bloblist member's pixels.
	 */
	BlobList *foundBlobs = findAllBlobs(label, nbTargets, target, blobMinSize, false);	//gather all the blobs of our target
	BlobList::iterator foundIter;
	BlobList *returnedBlobs = new BlobList;
	
	bool found;

	for( foundIter = foundBlobs->begin(); foundIter != foundBlobs->end(); foundIter++ )
	{
		found = false;
		if( blob->getBoundRect()->contains((*foundIter)->getBoundRect()) )
		{
			returnedBlobs->push_back( (*foundIter) );
			found = true;
		}
		else
			delete (*foundIter);

	}
	
	return returnedBlobs;
}


//	Find the blob with the target label that is the closest to the designated
//	starting position (using a spiral search), within the designated search
//	rectangle.
//
Blob* BlobFinder::findBlobAbout(const LabelRaster* label, const ImagePoint* pos, char target,
                                long blobMinSize, bool extendOut, bool fillHoles)
{
    FAIL_CONDITION( label == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in BlobFinder::findBlobAbout");
    return findBlobAbout(label, pos, target, blobMinSize, MAX_LONG_COUNT, label->getValidRect(), 
                         extendOut, fillHoles);
}

//	Find [within the search rectangle] the blob with the target label that is the
//	closest to the designated starting position (using a spiral search).
//
Blob* BlobFinder::findBlobAbout(const LabelRaster* label, const ImagePoint* pos, char target,
                                long blobMinSize, const ImageRect* theRect, 
								bool extendOut, bool fillHoles)
{
    return findBlobAbout(label, pos, target, blobMinSize, MAX_LONG_COUNT, theRect, 
						 extendOut, fillHoles);
}


//	Find [within the search rectangle] the blob with the target label that is the
//	closest to the designated starting position (using a spiral search).
Blob* BlobFinder::findBlobAbout(const LabelRaster* label, const ImagePoint* pos, char target,
                                long blobMinSize, long blobMaxSize, const ImageRect* theRect, 
								bool extendOut, bool fillHoles)
{
    FAIL_CONDITION( label == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in BlobFinder::findBlobAbout");
    FAIL_CONDITION( pos == NULL,
                    kNullImagePointError,
                    "null ImagePoint as startting point in BlobFinder::findBlobAbout");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in BlobFinder::findBlobAbout");

    ImageRect* spiralSearchRect = ImageRect::intersection(label->getValidRect(), theRect);
    FAIL_CONDITION( spiralSearchRect == NULL,
                    kNullRectangleError,
                    "search rect out of valid data bounds in BlobFinder::findBlobAbout");

    Blob* myBlob = NULL;
    
    if (spiralSearchRect->contains(pos))
    {
        label_ = label->getShiftedLabel2D(R_W_ACCESS);
        ImagePoint	firstPoint(pos);
        bool	searchIsOver = false;

        while (!searchIsOver)
        {
            firstPoint.setX(pos->getX());
            firstPoint.setY(pos->getY());

            bool	foundTarget  = spiralSearch_(&firstPoint, target, spiralSearchRect);

            if (foundTarget)
            {
				if (extendOut)
					myBlob = fillThisBlob_(&firstPoint, label->getValidRect(), (char) -1);
                else
					myBlob = fillThisBlob_(&firstPoint, theRect, (char) -1);

                //	only keep the blob if it large enough
                if ((myBlob->getNbPixels() >= blobMinSize) && 
                    (myBlob->getNbPixels() <= blobMaxSize))
                    searchIsOver = true;
                else
                {
                    delete myBlob;
                    myBlob = NULL;
                }
            }
            else
                searchIsOver = true;

        }
        delete spiralSearchRect;
        
        
        if (fillHoles)
        {
            myBlob->fillHoles();
            refillBlobLocation_(myBlob, (char) -1);
        }
    }
	
	if (spiralSearchRect != NULL)
		delete spiralSearchRect;
    
    return myBlob;
}

#if 0
#pragma mark -
//------------------------------------------------------------------------------
#pragma mark private search functions
//------------------------------------------------------------------------------
#endif

/*------------------------------------------------------------------------------+
|	searches left from x to find the first good pixel							|
+------------------------------------------------------------------------------*/
int BlobFinder::findLeft_(const ImagePoint* pos, char target, int limLeft)
{
    int	x = pos->getX(),
        y = pos->getY();

    while ( (x>=limLeft) && (label_[y][x] == target) )
        x--;

    return (x+1);
}

/*------------------------------------------------------------------------------+
|	searches right from x to find the last good pixel							|
+------------------------------------------------------------------------------*/
int BlobFinder::findRight_(const ImagePoint* pos, char target, int limRight)
{
    int	x = pos->getX(),
        y = pos->getY();

    while ( (x<=limRight) && (label_[y][x] == target) )
        x++;

    return (x-1);
}


/*------------------------------------------------------------------------------+
|	checks the pixel at leftX,y.  If it's fillable, then seeks leftfwards  to	|
|	find the first	fillable pixel.  If it's not fillable, it seeks rightwards,	|
|	up to and including 'rightX', for the first fillable pixel 					|
+------------------------------------------------------------------------------*/
int BlobFinder::findFirstLeft_(int leftX, int rightX, int y, char target,
                               int limLeft)
{
    int	x = leftX;

    if ( leftX > rightX )
        return (leftX);

    if ( label_[y][x] == target )
    {
        while ( (x>=limLeft) && (label_[y][x] == target) )
            x--;

        return (x+1);
    }
    else
    {
        while ( (x<=rightX) && (label_[y][x] != target) )
            x++;

        return (x);
    }
}


/*------------------------------------------------------------------------------+
|    fill all the pixels of the segment											|
+------------------------------------------------------------------------------*/
void BlobFinder::fillSegAndAdd_(int leftX, int rightX, int y, Blob* theBlob,
                                char fillValue)
{
    for (int x=leftX; x<=rightX; x++)
        label_[y][x] = fillValue;

    theBlob->addSegment(leftX, rightX, y);
}

/*------------------------------------------------------------------------------+
|    refill all the pixels at location occupied by the blob						|
+------------------------------------------------------------------------------*/
void BlobFinder::refillBlobLocation_(Blob* theBlob, char fillValue)
{
	const LineSegDeque& segDeque = theBlob->getSegDeque();
	LineSegDeque::const_iterator endLineIter = segDeque.end();
	for (LineSegDeque::const_iterator lineIter = segDeque.begin(); lineIter != endLineIter; lineIter++)
	{
		list<HorizontalSegment*>::iterator segIter = (*lineIter)->begin();
		int y = (*segIter)->y;

		for ( ; segIter != (*lineIter)->end(); segIter++)
		{
		    for (int x=(*segIter)->leftX; x<=(*segIter)->rightX; x++)
		        label_[y][x] = fillValue;

		}
	}
}


/*------------------------------------------------------------------------------+
|	Fills a blob from point pos.												|
|		Looks for all pixels with the same label that are connected to the		|
|		original pixel.															|
+------------------------------------------------------------------------------*/
Blob* BlobFinder::fillThisBlob_(const ImagePoint* pos, const ImageRect* searchRect, 
                                char fillValue)
{

    const int   searchLeft = searchRect->getLeft(),
                searchRight = searchRect->getRight(),
                searchTop = searchRect->getTop(),
                searchBottom = searchRect->getBottom();
    Blob*	myBlob = new Blob();
    ImagePoint	pos2(0, 0);
    HorizontalSegment	seg;
    stack<HorizontalSegment>	segStack;
    int	leftX, rightX,
        x = pos->getX(),
        y = pos->getY(),
        r;
    char 	target = label_[y][x];

    leftX=findLeft_(pos, target, searchLeft);
    rightX=findRight_(pos, target, searchRight);

    fillSegAndAdd_(leftX, rightX, y, myBlob, fillValue);

    if (y > searchTop)
    {
        /*push*/
        seg.leftX = leftX;
        seg.rightX = rightX;
        seg.y = y - 1;
        segStack.push(seg);
    }
    if(y < searchBottom)
    {
        /*push*/
        seg.leftX = leftX;
        seg.rightX = rightX;
        seg.y = y + 1;
        segStack.push(seg);
    }
    while(!segStack.empty())
    {
        /*pop*/
        seg = segStack.top();
        segStack.pop();
        leftX = seg.leftX;
        rightX = seg.rightX;
        y = seg.y;
        leftX=findFirstLeft_(leftX, rightX, y, target, searchLeft);

        while(leftX<=rightX)
        {
            pos2.setCoordinates(leftX, y);

            r = findRight_(&pos2, target, searchRight);

            fillSegAndAdd_(leftX, r, y, myBlob, fillValue);

            if (leftX <r)
            {
                if(y > searchTop)
                {
                    /*push*/
                    seg.leftX = leftX;
                    seg.rightX = r;
                    seg.y = y - 1;
                    segStack.push(seg);
                }
                if(y < searchBottom)
                {
                    /*push*/
                    seg.leftX = leftX;
                    seg.rightX = r;
                    seg.y = y + 1;
                    segStack.push(seg);
                }
            }

            if (r+2 <= rightX)
                leftX=findFirstLeft_(r+2, rightX, y, target, searchLeft);
            else
                leftX = rightX+1;

        }

    }

    return myBlob;
}


/*--------------------------------------------------------------------------+
|	Starting from pos spiral out until the target is found or we get out	|
|	of the search rectangle.												|
+---------------------------------------------------------------------------*/

bool BlobFinder::spiralSearch_(ImagePoint* pos, char target, const ImageRect* searchRect)
{
    int	i = pos->getY(),
              j = pos->getX();
    int	iLow = searchRect->getTop(),
		iHigh = searchRect->getBottom(),
		jLow = searchRect->getLeft(),
		jHigh = searchRect->getRight();

    int	iStart[4] = {0, 0, 0, 0},
		iEnd[4] = {0, 0, 0, 0},
		jStart[4] = {0, 0, 0, 0},
		jEnd[4] = {0, 0, 0, 0};
    int	lengthMove = 1;
    int	startIndex,
    endIndex;
    bool	found = false;
    bool	stillWithinRect[4] = {true, true, true, true};

    iStart[MOVE_LEFT] = iEnd[MOVE_LEFT] = i;
    jStart[MOVE_LEFT] = j;
    jEnd[MOVE_LEFT] = j - lengthMove;

    while (!found)
    {
        //-----------------------------------------
        //	move left
        //-----------------------------------------
        //	don't start outside of the box
        if (jStart[MOVE_LEFT] > jHigh)
            startIndex = jHigh;
        else
            startIndex = jStart[MOVE_LEFT];

        //	if this move brings us outside the box, the next one will be skipped
        if (jEnd[MOVE_LEFT] < jLow)
        {
            stillWithinRect[MOVE_DOWN] = false;

            //	if all directions are to be skipped, stop the search
            if ( !(	stillWithinRect[MOVE_LEFT] || stillWithinRect[MOVE_DOWN] ||
                    stillWithinRect[MOVE_RIGHT] || stillWithinRect[MOVE_UP]) )
                return false;

            endIndex = jLow;
        }
        else
            endIndex = jEnd[MOVE_LEFT];

        if (stillWithinRect[MOVE_LEFT])
        {
            for (j=startIndex; (j>=endIndex) && !found; j--)
                if (label_[i][j] == target)
                {
                    pos->setX(j);
                    pos->setY(i);
                    return true;
                }

            j++;
        }
        else
            j = endIndex;

        iStart[MOVE_DOWN] = iEnd[MOVE_LEFT]+1;
        iEnd[MOVE_DOWN] = iEnd[MOVE_LEFT] + lengthMove++;
        jStart[MOVE_DOWN] = jEnd[MOVE_DOWN] = jEnd[MOVE_LEFT];

        //-----------------------------------------
        //	move down
        //-----------------------------------------
        //	don't start outside of the box
        if (iStart[MOVE_DOWN] < iLow)
            startIndex = iLow;
        else
            startIndex = iStart[MOVE_DOWN];

        //	if this move brings us outside the box, the next one will be skipped
        if (iEnd[MOVE_DOWN] > iHigh)
        {
            stillWithinRect[MOVE_RIGHT] = false;
            //	if all directions are to be skipped, stop the search
            if ( !(	stillWithinRect[MOVE_LEFT] || stillWithinRect[MOVE_DOWN] ||
                    stillWithinRect[MOVE_RIGHT] || stillWithinRect[MOVE_UP]) )
                return false;

            endIndex = iHigh;
        }
        else
            endIndex = iEnd[MOVE_DOWN];

        if (stillWithinRect[MOVE_DOWN])
        {
            for (i=startIndex; (i<=endIndex) && !found; i++)
                if (label_[i][j] == target)
                {
                    pos->setX(j);
                    pos->setY(i);
                    return true;
                }

            i--;
        }
        else
            i = endIndex;

        iStart[MOVE_RIGHT] = iEnd[MOVE_RIGHT] = iEnd[MOVE_DOWN];
        jStart[MOVE_RIGHT] = jEnd[MOVE_DOWN]+1;
        jEnd[MOVE_RIGHT] = jEnd[MOVE_DOWN] + lengthMove;


        //-----------------------------------------
        //	move right
        //-----------------------------------------
        //	don't start outside of the box
        if (jStart[MOVE_RIGHT] < jLow)
            startIndex = jLow;
        else
            startIndex = jStart[MOVE_RIGHT];

        //	if this move brings us outside the box, the next one will be skipped
        if (jEnd[MOVE_RIGHT] > jHigh)
        {
            stillWithinRect[MOVE_UP] = false;
            //	if all directions are to be skipped, stop the search
            if ( !(	stillWithinRect[MOVE_LEFT] || stillWithinRect[MOVE_DOWN] ||
                    stillWithinRect[MOVE_RIGHT] || stillWithinRect[MOVE_UP]) )
                return false;

            endIndex = jHigh;
        }
        else
            endIndex = jEnd[MOVE_RIGHT];

        if (stillWithinRect[MOVE_RIGHT])
        {
            for (j=startIndex; (j<=endIndex) && !found; j++)
                if (label_[i][j] == target)
                {
                    pos->setX(j);
                    pos->setY(i);
                    return true;
                }

            j--;
        }
        else
            j = endIndex;

        iStart[MOVE_UP] = iStart[MOVE_RIGHT]-1;
        iEnd[MOVE_UP] = iEnd[MOVE_RIGHT] - lengthMove++;
        jStart[MOVE_UP] = jEnd[MOVE_UP] = jEnd[MOVE_RIGHT];


        //-----------------------------------------
        //	move up
        //-----------------------------------------
        //	don't start outside of the box
        if (iStart[MOVE_UP] > iHigh)
            startIndex = iHigh;
        else
            startIndex = iStart[MOVE_UP];

        //	if this move brings us outside the box, the next one will be skipped
        if (iEnd[MOVE_UP] < iLow)
        {
            stillWithinRect[MOVE_LEFT] = false;
            //	if all directions are to be skipped, stop the search
            if ( !(	stillWithinRect[MOVE_LEFT] || stillWithinRect[MOVE_DOWN] ||
                    stillWithinRect[MOVE_RIGHT] || stillWithinRect[MOVE_UP]) )
                return false;

            endIndex = iLow;
        }
        else
            endIndex = iEnd[MOVE_UP];

        if (stillWithinRect[MOVE_UP])
        {
            for (i=startIndex; (i>=endIndex) && !found; i--)
                if (label_[i][j] == target)
                {
                    pos->setX(j);
                    pos->setY(i);
                    return true;
                }

            i++;
        }
        else
            i = endIndex;

        iStart[MOVE_LEFT] = iEnd[MOVE_LEFT] = iEnd[MOVE_UP];
        jStart[MOVE_LEFT] = jEnd[MOVE_UP]-1;
        jEnd[MOVE_LEFT] = jEnd[MOVE_UP] - lengthMove;

    }

    return found;
}
