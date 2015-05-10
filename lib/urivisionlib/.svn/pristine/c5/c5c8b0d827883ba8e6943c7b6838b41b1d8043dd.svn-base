/*  NAME:
        ImageRect.cpp

    DESCRIPTION:
        implementation of the uriVisionLib ImageRect class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ImageRect.h"

using namespace uriVL;

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Constructors and destructor
//--------------------------------------------------------------------------
#endif

ImageRect::ImageRect(void)
		:	rect_(URIVL_Rect()),
			upperLeft_(0, 0),
			lowerRight_(-1, -1)
{
	rect_.left = rect_.top = rect_.right = rect_.bottom = static_cast<short>(0);
}


ImageRect::ImageRect(int theLeft, int theTop, int theWidth, int theHeight)
		:	rect_(URIVL_Rect()),
			upperLeft_(ImagePoint(theLeft, theTop)),
			lowerRight_(ImagePoint(theLeft + theWidth - 1, theTop + theHeight - 1))
	
{
	rect_.left = static_cast<short>(theLeft);
	rect_.top = static_cast<short>(theTop);
	rect_.right = static_cast<short>(rect_.left + theWidth);
	rect_.bottom = static_cast<short>(rect_.top + theHeight);
}

ImageRect::ImageRect(const ImagePoint* theUpperLeft, const ImagePoint* theLowerRight)
	try	:	rect_(),
			upperLeft_(ImagePoint(theUpperLeft)),
			lowerRight_(ImagePoint(theLowerRight))
{
	try {
		rect_.left = static_cast<short>(theUpperLeft->getX());
		rect_.top = static_cast<short>(theUpperLeft->getY());
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid UL ImagePoint pointer passed to ImageRect constructor.");		
	}
	
    try {
		rect_.right = static_cast<short>(theLowerRight->getX() + 1);
		rect_.bottom = static_cast<short>(theLowerRight->getY() + 1);
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid LR ImagePoint pointer passed to ImageRect constructor.");		
	}
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageRect constructor.");
	throw e;
}

ImageRect::ImageRect(const ImagePoint* theUpperLeft, int theWidth, int theHeight)
	try	:	rect_(),
			upperLeft_(ImagePoint(theUpperLeft)),
			lowerRight_(ImagePoint(theUpperLeft))
{
	try {
		rect_.left = static_cast<short>(theUpperLeft->getX());
		rect_.top = static_cast<short>(theUpperLeft->getY());
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid UL ImagePoint pointer passed to ImageRect constructor.");		
	}

    rect_.right = static_cast<short>(rect_.left + theWidth);
    rect_.bottom = static_cast<short>(rect_.top + theHeight);

    lowerRight_.translate(theWidth-1, theHeight-1);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageRect constructor.");
	throw e;
}


ImageRect::ImageRect(const ImageRect* theRect)
	try	:	rect_(theRect->rect_),
			upperLeft_(theRect->upperLeft_),
			lowerRight_(theRect->lowerRight_)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageRect constructor.");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( theRect == NULL,
					kNullParameterError,
					"null ImageRect pointer passed to ImageRect constructor.");		
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid ImageRect pointer passed to ImageRect constructor.");		
}


ImageRect::ImageRect(const ImageRect& theRect)
	try	:	rect_(theRect.rect_),
			upperLeft_(theRect.upperLeft_),
			lowerRight_(theRect.lowerRight_)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageRect constructor.");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid ImageRect reference passed to ImageRect constructor.");		
}




ImageRect::ImageRect(const URIVL_Rect qtRect)
	try	:	rect_(qtRect),
			upperLeft_(new ImagePoint(qtRect.left, qtRect.top)),
			lowerRight_(new ImagePoint(qtRect.right - 1, qtRect.bottom - 1))
			
{
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid ImageRect pointer passed to ImageRect constructor.");		
}

ImageRect::~ImageRect(void)
{
}



#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Copy and Translation
//--------------------------------------------------------------------------
#endif

const ImageRect& ImageRect::operator = (const ImageRect& theRect)
{
	rect_ = theRect.rect_;
	upperLeft_.setCoordinates(theRect.upperLeft_);
	lowerRight_.setCoordinates(theRect.lowerRight_);
	return *this;
}



void ImageRect::setOrigin(int x, int y)
{
    rect_.right += static_cast<short>(x - rect_.left);
    rect_.left = static_cast<short>(x);
    rect_.bottom += static_cast<short>(y - rect_.top);
    rect_.top = static_cast<short>(y);

    upperLeft_.setCoordinates(rect_.left, rect_.top);
    lowerRight_.setCoordinates(rect_.right - 1, rect_.bottom - 1);
}


void ImageRect::setCenter(int cX, int cY)
{
	const int myWidth = rect_.right - rect_.left;
	const int myHeight = rect_.bottom - rect_.top;

    rect_.left = static_cast<short>(cX - myWidth/2);
    rect_.right = static_cast<short>(rect_.left + myWidth);
    rect_.top = static_cast<short>(cY - myHeight/2);
    rect_.bottom += static_cast<short>(rect_.top + myHeight);

    upperLeft_.setX(rect_.left);
    upperLeft_.setY(rect_.top);

    lowerRight_.setX(rect_.right - 1);
    lowerRight_.setY(rect_.bottom - 1);
}


void ImageRect::translate(int dx, int dy)
{
    rect_.right += dx;
    rect_.left += dx;
    rect_.bottom += dy;
    rect_.top += dy;

    upperLeft_.setCoordinates(rect_.left, rect_.top);

    lowerRight_.setCoordinates(rect_.right - 1, rect_.bottom - 1);
}

void ImageRect::setRect(const ImageRect* theRect)
{
    rect_.left = theRect->rect_.left;
    rect_.top = theRect->rect_.top;
    rect_.right = theRect->rect_.right;
    rect_.bottom = theRect->rect_.bottom;

//    upperLeft_ = new ImagePoint(theRect->upperLeft_);
//    lowerRight_ = new ImagePoint(theRect->lowerRight_);
	upperLeft_.setCoordinates(rect_.left, rect_.top);
	lowerRight_.setCoordinates(rect_.right-1, rect_.bottom - 1);
}

void ImageRect::setRect(int theLeft, int theTop, int theWidth, int theHeight)
{
    rect_.left = static_cast<short>(theLeft);
    rect_.top = static_cast<short>(theTop);
//    upperLeft_ = new ImagePoint(rect_.left, rect_.top);

    rect_.right = static_cast<short>(rect_.left + theWidth);
    rect_.bottom = static_cast<short>(rect_.top + theHeight);

//    lowerRight_ = new ImagePoint(rect_.right - 1, rect_.bottom - 1);

    upperLeft_.setCoordinates(rect_.left, rect_.top);

    lowerRight_.setCoordinates(rect_.right - 1, rect_.bottom - 1);
}

void ImageRect::setRect(const ImagePoint*  topLeft, const ImagePoint*  bottomRight)
{
    rect_.left = static_cast<short>(topLeft->getX());
    rect_.top = static_cast<short>(topLeft->getY());
    rect_.right = static_cast<short>(bottomRight->getX());
    rect_.bottom = static_cast<short>(bottomRight->getY());

    upperLeft_.setCoordinates(rect_.left, rect_.top);
    lowerRight_.setCoordinates(rect_.right - 1, rect_.bottom - 1);
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Access methods
//--------------------------------------------------------------------------
#endif


const URIVL_Rect *ImageRect::getRectQT_(void) const
{
    return &rect_;
}

int ImageRect::getWidth(void) const
{
    return rect_.right - rect_.left;
}
int ImageRect::getHeight(void) const
{
    return  rect_.bottom - rect_.top;
}

int	ImageRect::getLeft(void) const
{
    return upperLeft_.getX( );
}

int ImageRect::getRight(void) const
{
    return lowerRight_.getX( );
}

int ImageRect::getTop(void) const
{
    return upperLeft_.getY( );
}

int ImageRect::getBottom(void) const 
{
    return lowerRight_.getY( );
}

const ImagePoint* ImageRect::getUpperLeft() const
{
	return &upperLeft_;
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Boolean operations: tests between rectangles
//--------------------------------------------------------------------------
#endif


bool ImageRect::coincidesWith(const ImageRect* theRect) const
{
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in ImageRect::coincidesWith");

    return  (theRect->getTop() == getTop()) &&
            (theRect->getLeft() == getLeft()) &&
            (theRect->getBottom() == getBottom()) &&
            (theRect->getRight() == getRight());
}

bool ImageRect::coincidesWith(int theLeft, int theTop, int theWidth, int theHeight) const
{
    return  (theTop == getTop()) &&
            (theLeft == getLeft()) &&
            (theTop + theHeight == getBottom()) &&
            (theLeft + theWidth == getRight());
}

bool ImageRect::overlaps(const ImageRect* theRect) const
{
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in ImageRect::overlaps");

    return !(
               (theRect->getBottom() < getTop())   ||    //  completely above
               (theRect->getTop() > getBottom())   ||    //  completely below
               (theRect->getRight() < getLeft())   ||    //  completely to the left
               (theRect->getLeft() > getRight())         //  completely to the right
           );
}

bool ImageRect::overlaps(int theLeft, int theTop, int theWidth, int theHeight) const
{
    return !(
               (theTop + theHeight < getTop())   ||    //  completely above
               (theTop > getBottom())   ||    //  completely below
               (theLeft + theWidth < getLeft())   ||    //  completely to the left
               (theLeft > getRight())         //  completely to the right
           );
}

bool ImageRect::contains(const ImageRect* theRect, int padding) const
{
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in ImageRect::contains");

    return  (theRect->getTop() >= (getTop()+padding)) &&
            (theRect->getLeft() >= (getLeft()+padding)) &&
            (theRect->getBottom() <= (getBottom()-padding)) &&
            (theRect->getRight() <= (getRight()-padding));
}

bool ImageRect::contains(int theLeft, int theTop, int theWidth, int theHeight, 
                         int padding) const
{
    return  (theTop >= (getTop()+padding)) &&
            (theLeft >= (getLeft()+padding)) &&
            (theTop + theHeight <= (getBottom()-padding)) &&
            (theLeft + theWidth <= (getRight()-padding));
}

bool ImageRect::isContainedBy(const ImageRect* theRect) const
{
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in ImageRect::isContainedBy");

    return  (theRect->getTop() <= getTop()) &&
            (theRect->getLeft() <= getLeft()) &&
            (theRect->getBottom() >= getBottom()) &&
            (theRect->getRight() >= getRight());
}

bool ImageRect::isContainedBy(int theLeft, int theTop, int theWidth, int theHeight) const
{
    return  (theTop <= getTop()) &&
            (theLeft <= getLeft()) &&
            (theTop + theHeight >= getBottom()) &&
            (theLeft + theWidth >= getRight());
}

bool ImageRect::contains(const ImagePoint* q, int  padding) const
{
    FAIL_CONDITION( q == NULL,
                    kNullParameterError,
                    "Null ImagePoint passed to ImageRect::isInside");

    return contains(q->getX(), q->getY(), padding);
}

bool ImageRect::contains(int x, int y, int padding) const
{
//    return (x>=getLeft()) && (x<=getRight()) && (y>=getTop()) && (y<=getBottom());
    return (x>=(getLeft()+padding)) && (x<=(getRight()-padding))
    		 && (y>=(getTop()+padding)) && (y<=(getBottom()-padding));
}

bool ImageRect::upperLeftIsAtOrigin(void) const
{
    return (getTop() == 0) && (getLeft() == 0);
}

#if 0
#pragma mark -
//--------------------------------------------------------------------------
#pragma mark Rectangle operations
//--------------------------------------------------------------------------
#endif

void ImageRect::addAllAround(int n)
{
    rect_.left -= n;
    rect_.top -= n;
    rect_.right += n;
    rect_.bottom += n;

    upperLeft_.setCoordinates(rect_.left, rect_.top);
    lowerRight_.setCoordinates(rect_.right - 1, rect_.bottom - 1);
}

void ImageRect::removeAllAround(int n)
{
    FAIL_CONDITION( (getWidth() <= 2*n) || (getHeight() <= 2*n),
                    kInvalidRectangleError,
                    "attempt to remove too much from an ImageRect");

    rect_.left += n;
    rect_.top += n;
    rect_.right -= n;
    rect_.bottom -= n;

    upperLeft_.setCoordinates(rect_.left, rect_.top);
    lowerRight_.setCoordinates(rect_.right - 1, rect_.bottom - 1);
}


ImageRect* ImageRect::intersection(const ImageRect* rect1, const ImageRect* rect2)
{
    FAIL_CONDITION( rect1 == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in ImageRect::intersection");
    FAIL_CONDITION( rect2 == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in ImageRect::intersection");

    ImageRect* inter = NULL;

    //	if the rectangles don't overlap, we return NULL
    if (rect1->overlaps(rect2))
    {
        const int	xMin1 = rect1->getLeft(),
					xMax1 = rect1->getRight(),
					yMin1 = rect1->getTop(),
					yMax1 = rect1->getBottom();
        const int	xMin2 = rect2->getLeft(),
					xMax2 = rect2->getRight(),
					yMin2 = rect2->getTop(),
					yMax2 = rect2->getBottom();
		const int xMin = xMin1 > xMin2 ? xMin1 : xMin2;
		const int yMin = yMin1 > yMin2 ? yMin1 : yMin2;
		const int w = xMax1 > xMax2 ? xMax2 - xMin + 1: xMax1 - xMin + 1;
		const int h = yMax1 > yMax2 ? yMax2 - yMin + 1: yMax1 - yMin +1;

        inter = new ImageRect(xMin, yMin, w, h);
    }

    return inter;
}
