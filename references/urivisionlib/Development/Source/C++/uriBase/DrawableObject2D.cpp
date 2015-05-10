/*  NAME:
        DrawableObject2D.cpp

    DESCRIPTION:
        implementation of the uriVisionLib DrawableObject2D class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "DrawableObject2D.h"

using namespace uriVL;

//----------------------------------------------------------
//  static members
//----------------------------------------------------------

int DrawableObject2D::defaultWindowWidth_ = -1;
int DrawableObject2D::defaultWindowHeight_ = -1;
bool DrawableObject2D::defaultDimensionsHaveChangedSinceLastRender_ = true;


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

DrawableObject2D::DrawableObject2D(DrawableInformationType drawableInfoType)
	:	    boundRect_(ImageRect()),
            validRect_(ImageRect()),			
            centroid_(ImagePoint_F()),			
            centerOfMass_(ImagePoint_F()),
			//
			boundingRectIsUpToDate_(false),
			validRectIsUpToDate_(false),
			centroidIsUpToDate_(false),
			centerOfMassIsUpToDate_(false),
			//			
			scaleX_(1.f),
			scaleY_(1.f),
			isRenderedScaled_(false),
			scaledLeft_(-1),
			scaledTop_(-1),
			scaledRight_(-1),
			scaledBottom_(-1),
			dimensionsHaveChangedSinceLastRender_(true),
			drawableInfoType_(drawableInfoType),
			//
			windowWidth_(-1),
			windowHeight_(-1),
			hasIndivWindowDimensions_(false),
			//
			renderZ_(-1.f),
			frameColor_(NULL),
			mustDrawFrame_(false),
			mustDrawValidDataFrame_(false)
{
	//	default color is black
    color_[0] = color_[1] = color_[2] = 0.f;
    color_[3] = 1.0f;
}

DrawableObject2D::DrawableObject2D(const ImageRect& rect, DrawableInformationType drawableInfoType)
	try	:	boundRect_(rect),
            validRect_(UriVL::validDataRectIsSafe() ? ImageRect() : rect),			
            centroid_(ImagePoint_F()),			
            centerOfMass_(ImagePoint_F()),
			//
			boundingRectIsUpToDate_(false),
			validRectIsUpToDate_(false),
			centroidIsUpToDate_(false),
			centerOfMassIsUpToDate_(false),
			//			
			scaleX_(1.f),
			scaleY_(1.f),
			isRenderedScaled_(false),
			scaledLeft_(-1),
			scaledTop_(-1),
			scaledRight_(-1),
			scaledBottom_(-1),
			dimensionsHaveChangedSinceLastRender_(true),
			drawableInfoType_(drawableInfoType),
			//
			windowWidth_(-1),
			windowHeight_(-1),
			hasIndivWindowDimensions_(false),
			//
			renderZ_(-1.f),
			frameColor_(NULL),
			mustDrawFrame_(false),
			mustDrawValidDataFrame_(false)
{
	//	default color is black
    color_[0] = color_[1] = color_[2] = 0.f;
    color_[3] = 1.0f;
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by DrawableObject2D constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid ImageRect parameter error in DrawableObject2D constructor");
}

DrawableObject2D::DrawableObject2D(int nbRows, int nbCols, DrawableInformationType drawableInfoType)
	try	:	boundRect_(ImageRect(0, 0, nbCols, nbRows)),
            validRect_(UriVL::validDataRectIsSafe() ? ImageRect() : ImageRect(0, 0, nbCols, nbRows)),			
            centroid_(ImagePoint_F()),			
            centerOfMass_(ImagePoint_F()),
			//
			boundingRectIsUpToDate_(false),
			validRectIsUpToDate_(false),
			centroidIsUpToDate_(false),
			centerOfMassIsUpToDate_(false),
			//			
			scaleX_(1.f),
			scaleY_(1.f),
			isRenderedScaled_(false),
			scaledLeft_(-1),
			scaledTop_(-1),
			scaledRight_(-1),
			scaledBottom_(-1),
			dimensionsHaveChangedSinceLastRender_(true),
			drawableInfoType_(drawableInfoType),
			//
			windowWidth_(-1),
			windowHeight_(-1),
			hasIndivWindowDimensions_(false),
			//
			renderZ_(-1.f),
			frameColor_(NULL),
			mustDrawFrame_(false),
			mustDrawValidDataFrame_(false)
{
    FAIL_CONDITION( (nbRows<0) || (nbCols<0),
                    kRasterInvalidDimensions,
                    "Invalid dimensions in DrawableObject2D constructor");

	//	default color is black
    color_[0] = color_[1] = color_[2] = 0.f;
    color_[3] = 1.0f;
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by DrawableObject2D constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid dimension parameter error in DrawableObject2D constructor");
}



DrawableObject2D::DrawableObject2D(const DrawableObject2D& theObj)
	try:	boundRect_(theObj.boundRect_),
			validRect_(theObj.validRect_),			
			centroid_(theObj.centroid_),			
			centerOfMass_(theObj.centerOfMass_),
			//
			boundingRectIsUpToDate_(theObj.boundingRectIsUpToDate_),
			validRectIsUpToDate_(theObj.validRectIsUpToDate_),
			centroidIsUpToDate_(theObj.centroidIsUpToDate_),
			centerOfMassIsUpToDate_(theObj.centerOfMassIsUpToDate_),
			//			
			scaleX_(theObj.scaleX_),
			scaleY_(theObj.scaleY_),
			isRenderedScaled_(theObj.isRenderedScaled_),
			scaledLeft_(-1),
			scaledTop_(-1),
			scaledRight_(-1),
			scaledBottom_(-1),
			dimensionsHaveChangedSinceLastRender_(true),
			drawableInfoType_(theObj.drawableInfoType_),
			//
			windowWidth_(theObj.windowWidth_),
			windowHeight_(theObj.windowHeight_),
			hasIndivWindowDimensions_(theObj.hasIndivWindowDimensions_),
			//
			renderZ_(theObj.renderZ_),
			frameColor_(NULL),
			mustDrawFrame_(theObj.mustDrawFrame_),
			mustDrawValidDataFrame_(theObj.mustDrawValidDataFrame_)
{
	if (theObj.frameColor_ != NULL)
	{
		frameColor_ = new GLfloat[4];
		frameColor_[0] = theObj.frameColor_[0];
		frameColor_[1] = theObj.frameColor_[1];
		frameColor_[2] = theObj.frameColor_[2];
		frameColor_[3] = theObj.frameColor_[3];
	}

	//	default color is black
    color_[0] = color_[1] = color_[2] = 0.f;
    color_[3] = 1.0f;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid reference passed to DrawableObject2D constructor.");		
}




DrawableObject2D::~DrawableObject2D(void)
{
    if (frameColor_ != NULL)
        delete []frameColor_;

}


const DrawableObject2D& DrawableObject2D::operator =(const DrawableObject2D& theObj)
{
	boundRect_ = new ImageRect(theObj.boundRect_);
	if (!theObj.validRect_.coincidesWith(&(theObj.boundRect_))) {
		validRect_ = new ImageRect(theObj.validRect_);
	}
	else {
		validRect_ = theObj.boundRect_;
	}
		
	scaleX_ = theObj.scaleX_;
	scaleY_ = theObj.scaleY_;
	isRenderedScaled_ = theObj.isRenderedScaled_;
	hasIndivWindowDimensions_ = theObj.hasIndivWindowDimensions_;
	//
	renderZ_ = theObj.renderZ_;
	if (frameColor_ != NULL) {
		frameColor_ = new float[3];
		for (int i=0; i<3; i++)
			frameColor_[i] = theObj.frameColor_[i];
	}
	mustDrawFrame_= theObj.mustDrawFrame_;
	mustDrawValidDataFrame_ = theObj.mustDrawValidDataFrame_;

	return *this;
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to geometric info
//------------------------------------------------------
#endif


const ImageRect* DrawableObject2D::getBoundRect() const
{
    if (!boundingRectIsUpToDate_) 
	{
		computeBoundingRect_(boundRect_);
		boundingRectIsUpToDate_ = true;
	}
	return &boundRect_;
}

const ImageRect* DrawableObject2D::getValidRect() const
{
    if (!validRectIsUpToDate_) 
	{
		computeValidRect_(boundRect_);
		validRectIsUpToDate_ = true;
	}
    return &validRect_;
}

bool DrawableObject2D::boundRectIsEmpty(void) const
{
	return ((boundRect_.getWidth() == 0) && (boundRect_.getHeight() == 0));
}

const ImagePoint_F* DrawableObject2D::getCentroid(void) const{
	if (centroidIsUpToDate_ != true)
	{
		computeCentroid_(centroid_);
		centroidIsUpToDate_ = true;
	}
	return &centroid_;
}


// Simple get function
const ImagePoint_F* DrawableObject2D::getCenterOfMass(void) const{
	if (centerOfMassIsUpToDate_ != true)
	{
		computeCenterOfMass_(centerOfMass_);
		centerOfMassIsUpToDate_ = true;
	}
	
	return &centerOfMass_;
}


void DrawableObject2D::setCentroidIsUpToDate_(bool isUpToDate) const
{
    centroidIsUpToDate_ = isUpToDate;
}

void DrawableObject2D::setCenterOfMassIsUpToDate_(bool isUpToDate) const
{
    centerOfMassIsUpToDate_ = isUpToDate;
}

void DrawableObject2D::setBoundingRectIsUpToDate_(bool isUpToDate) const
{
    boundingRectIsUpToDate_ = isUpToDate;
	if (!isUpToDate)
	{
		dimensionsHaveChangedSinceLastRender_ = true;
	}
}

void DrawableObject2D::setValidRectIsUpToDate_(bool isUpToDate) const
{
    validRectIsUpToDate_ = isUpToDate;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Rectangle handling functions
//------------------------------------------------------
#endif

bool DrawableObject2D::coincidesWith(DrawableObject2D* theObj) const
{
	try {
		return boundRect_.coincidesWith(theObj->getBoundRect());
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::coincidesWith.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::coincidesWith");
		//	for compiler
		return false;
	}
}

bool DrawableObject2D::coincidesWith(const ImageRect* theRect) const
{
	try {
		return boundRect_.coincidesWith(theRect);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::coincidesWith.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::coincidesWith");
		//	for compiler
		return false;
	}
}

bool DrawableObject2D::overlaps(DrawableObject2D* theObj) const
{
	try {
		return boundRect_.overlaps(theObj->getBoundRect());
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::overlaps.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::overlaps");
		//	for compiler
		return false;
	}
}

bool DrawableObject2D::overlaps(const ImageRect* theRect) const
{
	try {
		return boundRect_.overlaps(theRect);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::overlaps.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::overlaps");
		//	for compiler
		return false;
	}
}

bool DrawableObject2D::contains(const DrawableObject2D* theObj) const
{
	try {
		return boundRect_.contains(theObj->getBoundRect());
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::contains.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::contains");
		//	for compiler
		return false;
	}
}

bool DrawableObject2D::contains(const ImageRect* theRect) const
{
	try {
		return boundRect_.contains(theRect);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::contains.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::contains");
		//	for compiler
		return false;
	}
}

bool DrawableObject2D::contains(const ImagePoint* pt, int padding) const
{
	try {
		return boundRect_.contains(pt->getX(), pt->getY(), padding);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::contains.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::contains");
		//	for compiler
		return false;
	}
}

bool DrawableObject2D::contains(int x, int y, int padding) const
{
	try {
		return boundRect_.contains(x, y, padding);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::contains.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::contains");
		//	for compiler
		return false;
	}
}



bool DrawableObject2D::isContainedBy(DrawableObject2D* theObj) const
{
	try {
		return boundRect_.isContainedBy(theObj->getBoundRect());
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::isContainedBy.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::isContainedBy");
		//	for compiler
		return false;
	}
}

bool DrawableObject2D::isContainedBy(const ImageRect* theRect) const
{
	try {
		return boundRect_.isContainedBy(theRect);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by DrawableObject2D::isContainedBy.");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer passed to a DrawableObjec2D passed to DrawableObject2D::isContainedBy");
		//	for compiler
		return false;
	}
}

int DrawableObject2D::getHeight(void) const
{
    return boundRect_.getHeight( );
}

int DrawableObject2D::getWidth(void) const
{
    return boundRect_.getWidth( );
}

const ImagePoint* DrawableObject2D::getUpperLeft() const
{
	return boundRect_.getUpperLeft();
}

int DrawableObject2D::getTop(void) const
{
    return boundRect_.getTop( );
}

int DrawableObject2D::getBottom(void) const
{
    return boundRect_.getBottom( );
}

int DrawableObject2D::getLeft(void) const
{
    return boundRect_.getLeft( );
}

int DrawableObject2D::getRight(void) const
{
    return boundRect_.getRight( );
}


void DrawableObject2D::setOrigin(int x, int y)
{
    setBoundRect(x, y, getWidth(), getHeight());
}


void DrawableObject2D::translate(int dx, int dy)
{
    //  Needed to accomodate the child classes that don't maintain their bounding 
	//	rectangle at all times: forces recomputation for bounding rect if needed
    const ImageRect* theRect = getBoundRect();
    setBoundRect_(theRect->getLeft() + dx, theRect->getTop() + dy,
				 theRect->getWidth(), theRect->getHeight());
	boundRect_.translate(dx, dy);

    //  Same thing with valid rectangle
    theRect = getValidRect();
    setValidRect(theRect->getLeft() + dx, theRect->getTop() + dy,
				 theRect->getWidth(), theRect->getHeight());
}


void DrawableObject2D::translate(float dx, float dy)
{
	translate(static_cast<int>(dx+0.5f), static_cast<int>(dy+0.5f));
}



void DrawableObject2D::setBoundRect(const ImageRect* theRect)
{
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null rectangle received as parameter in DrawableObject2D::setRect");

    setBoundRect(theRect->getLeft(), theRect->getTop(), theRect->getWidth(), theRect->getHeight());
}


void DrawableObject2D::setBoundRect(int x, int y, int theWidth, int theHeight)
{
    //  Call the child class's function for some class-specific settings
    setBoundRect_(x, y, theWidth, theHeight);
	
	boundRect_.setRect(x, y, theWidth, theHeight);

	//	if we are in "safe mode" then the valid data rect is set to empty
	//	otherwise it is equal to the new boundrect
	if (UriVL::validDataRectIsSafe())
		setValidRect(x, y, 0, 0);
	else 
		setValidRect(x, y, theWidth, theHeight);			

}


void DrawableObject2D::setValidRect(const ImageRect* theRect)
{
    setValidRect(theRect->getLeft(), theRect->getTop(), theRect->getWidth(), theRect->getHeight());
}

void DrawableObject2D::setValidRect(int x, int y, int theWidth, int theHeight)
{
    //  Call the child class's function for some class-specific settings
    setValidRect_(x, y, theWidth, theHeight);

    validRect_.setRect(x, y, theWidth, theHeight);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Functions preparing for rendering
//------------------------------------------------------
#endif

void DrawableObject2D::setRenderDepth(float theZ)
{
    renderZ_ = theZ;
}

void DrawableObject2D::setFrameColor(float frameRed, float frameGreen, float frameBlue)
{
    if (frameColor_ == NULL)
        frameColor_ = new float[4];

    frameColor_[0] = frameRed;
    frameColor_[1] = frameGreen;
    frameColor_[2] = frameBlue;
    frameColor_[3] = 1.f;
}

void DrawableObject2D::setFrameColor(const float* theColor)
{
    if (frameColor_ == NULL)
        frameColor_ = new float[4];

    frameColor_[0] = theColor[0];
    frameColor_[1] = theColor[1];
    frameColor_[2] = theColor[2];
    frameColor_[3] = 1.f;
}


void DrawableObject2D::setFrameColor(unsigned char red, unsigned char green, unsigned char blue)
{
    if (frameColor_ == NULL)
        frameColor_ = new float[4];

    frameColor_[0] = red/255.f;
    frameColor_[1] = green/255.f;
    frameColor_[2] = blue/255.f;
    frameColor_[3] = 1.f;
}

void DrawableObject2D::setFrameColor(const unsigned char* theColor)
{
    if (frameColor_ == NULL)
        frameColor_ = new float[4];

    frameColor_[0] = theColor[0]/255.f;
    frameColor_[1] = theColor[1]/255.f;
    frameColor_[2] = theColor[2]/255.f;
    frameColor_[3] = 1.f;
}

void DrawableObject2D::setDrawFrame(bool drawFrame)
{
    mustDrawFrame_ = drawFrame;
}


void DrawableObject2D::setColor(float red, float green, float blue)
{
    color_[0] = red;
    color_[1] = green;
    color_[2] = blue;
    color_[3] = 1.f;
}

void DrawableObject2D::setColor(const float theColor[])
{
    color_[0] = theColor[0];
    color_[1] = theColor[1];
    color_[2] = theColor[2];
    color_[3] = 1.f;
}


void DrawableObject2D::setColor(unsigned char red, unsigned char green, unsigned char blue)
{
    color_[0] = red/255.f;
    color_[1] = green/255.f;
    color_[2] = blue/255.f;
    color_[3] = 1.f;
}

void DrawableObject2D::setColor(const unsigned char theColor[])
{
    color_[0] = theColor[0]/255.f;
    color_[1] = theColor[1]/255.f;
    color_[2] = theColor[2]/255.f;
    color_[3] = 1.f;
}

const float* DrawableObject2D::getColor(void) const
{
	return color_;
}


void DrawableObject2D::setDrawValidDataFrame(bool drawFrame)
{
    mustDrawValidDataFrame_ = drawFrame;
}

void DrawableObject2D::setRenderingScale(float scale) const
{
    scaleX_ = scaleY_ = scale;
    isRenderedScaled_ = ((scaleX_ != 1.f) || (scaleY_ != 1.f));
}


void DrawableObject2D::setRenderingScale(float scaleX, float scaleY) const
{
    scaleX_ = scaleX;
    scaleY_ = scaleY;
    isRenderedScaled_ = ((scaleX_ != 1.f) || (scaleY_ != 1.f));
}

float DrawableObject2D::getRenderingScaleX(void) const
{
    return scaleX_;
}

float DrawableObject2D::getRenderingScaleY(void) const
{
    return scaleY_;
}


void DrawableObject2D::computeScaledCorners_(int theLeft, int theTop, int theRight, int theBottom,
											 bool shifted, const ImagePoint* origin,
											 int& scaledLeft, int& scaledTop, 
											 int& scaledRight, int& scaledBottom) const
{
    const int renderWinHeight = getWindowHeight();
    const int renderWinWidth = getWindowWidth();

    if (origin != NULL)
    {
        FAIL_CONDITION( (renderWinHeight < 0) || (renderWinWidth < 0),
                        kInvalidRectangleError,
                        "The dimensions of the window must be known in the shifted mode");

        //  if we display at a specified location, then the global (0, 0) corner should be
        //  at the upper left corner of our window.
        scaledLeft = static_cast<int>(floorf(origin->getX()*scaleX_));
        scaledRight = static_cast<int>(floorf((origin->getX() + (theRight - theLeft))*scaleX_));
        scaledTop = renderWinHeight - static_cast<int>(floorf(origin->getY()*scaleY_));
        scaledBottom = renderWinHeight -
                        static_cast<int>(floorf(origin->getY() + (theBottom - theTop)*scaleY_));
    }
    else if (shifted)
    {
        FAIL_CONDITION( (renderWinHeight < 0) || (renderWinWidth < 0),
                        kInvalidRectangleError,
                        "The dimensions of the window must be known in the shifted mode");

        //  if we display shifted, then the global (0, 0) corner should be in
        //  the upper left corner of our window.
        scaledLeft = static_cast<int>(floorf(theLeft*scaleX_));
        scaledRight = static_cast<int>(floorf(theRight*scaleX_));
        scaledTop = renderWinHeight - static_cast<int>(floorf(theTop*scaleY_));
        scaledBottom = renderWinHeight - static_cast<int>(floorf(theBottom*scaleY_));
    }
    else
    {
        //  If we display unshifted, then we need to move to the upper-left
        //  corner of the image, and draw right and down from there.
        scaledLeft = 0;
        scaledRight = static_cast<int>(floorf(getWidth()*scaleX_));
        scaledTop = static_cast<int>(floorf(getHeight()*scaleY_));
        scaledBottom = 0;
    }
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Rendering functions
//------------------------------------------------------
#endif

void DrawableObject2D::render(void) const
{
    render(renderZ_, false, NULL);
}

void DrawableObject2D::render(bool shifted) const
{
    render(renderZ_, shifted, NULL);
}

void DrawableObject2D::render(float theZ) const
{
    render(theZ, false, NULL);
}

void DrawableObject2D::render(float theZ, bool shifted) const
{
    render(theZ, shifted, NULL);
}

void DrawableObject2D::render(const ImagePoint* origin) const
{
    render(renderZ_, true, origin);
}

void DrawableObject2D::render(float theZ, bool shifted, const ImagePoint* origin) const
{
	//	If needed, forces a recalculation of the bounding rectangle
	//	From this point until the end of the function, the bounding rect will not change,
	//	so, e.g., trueBoundRect->getLeft(), getLeft(), and getBoundRect()->getLeft() all return
	//	the same value.
	const ImageRect* trueBoundRect = getBoundRect();

    //--------------------------------------------------------------
    //	Part 1: update rendering location if needed
    //--------------------------------------------------------------
	if ((hasIndivWindowDimensions_ && dimensionsHaveChangedSinceLastRender_) ||
		defaultDimensionsHaveChangedSinceLastRender_)
	{
		computeScaledCorners_(getLeft(), getTop(), getRight(), getBottom(), shifted, origin,
							  scaledLeft_, scaledTop_, scaledRight_, scaledBottom_);
		dimensionsHaveChangedSinceLastRender_ = false;
	}
	
    //--------------------------------------------------------------
    //	Part 2: do subclass-specific rendering stuff
    //--------------------------------------------------------------
    render_(scaledLeft_, scaledTop_, scaledRight_, scaledBottom_, theZ, shifted, origin);


    //--------------------------------------------------------------
    //	Part 3: if required, indicate the bounding box by a colored frame
    //--------------------------------------------------------------
    if (mustDrawFrame_)
    {
        float	pRed, pGreen, pBlue;
        if (frameColor_ != NULL)
        {
            pRed = frameColor_[0];
            pGreen = frameColor_[1];
            pBlue = frameColor_[2];
        }
        else
            pRed = pGreen = pBlue = 0.f;

        //  The actual drawing
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(scaledLeft_, scaledTop_, theZ);
        glScalef(scaleX_, -scaleY_, 1.f);
        glColor3f(pRed, pGreen, pBlue);
        glBegin(GL_LINE_LOOP);
            glVertex3f(0, 0, 0);
            glVertex3f(trueBoundRect->getWidth(), 0, 0);
            glVertex3f(trueBoundRect->getWidth(), trueBoundRect->getHeight(), 0);
            glVertex3f(0, trueBoundRect->getHeight(), 0);
        glEnd();
        glPopMatrix();
    }

    //--------------------------------------------------------------
    //	Part 4: if required and necessary (not all data are valid),
    //	indicate the valid data rectangle by a colored frame
    //--------------------------------------------------------------
    if (mustDrawValidDataFrame_ && (!validRect_.coincidesWith(&boundRect_) || !mustDrawFrame_))
    {
        const ImageRect* trueValidRect = getValidRect();
		int shiftX = trueValidRect->getLeft() - getLeft();
		int shiftY = trueValidRect->getTop() - getTop();

        float	pRed, pGreen, pBlue;
        if (frameColor_ != NULL)
        {
            pRed = 1.f - frameColor_[0];
            pGreen = 1.f - frameColor_[1];
            pBlue = 1.f - frameColor_[2];
        }
        else
            pRed = pGreen = pBlue = 0.f;

        //  The actual drawing
        glPushMatrix();

		//	moved to the location of the bounding rect
        glTranslatef(scaledLeft_, scaledTop_, theZ);
		//	apply the rendering scale
        glScalef(scaleX_, -scaleY_, 1.f);
		//	then apply relative shift to the origin of the valid rect
        glTranslatef(shiftX, shiftY, 0.f);
		
        glColor3f(pRed, pGreen, pBlue);
        glBegin(GL_LINE_LOOP);
			glVertex3f(0, 0, 0);
			glVertex3f(trueValidRect->getWidth(), 0, 0);
			glVertex3f(trueValidRect->getWidth(), trueValidRect->getHeight(), 0);
			glVertex3f(0, trueValidRect->getHeight(), 0);
        glEnd();

        glPopMatrix();
    }
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Misc. access functions
//------------------------------------------------------
#endif

void DrawableObject2D::setDefaultWindowDimensions(int theWidth, int theHeight)
{
    FAIL_CONDITION( (theWidth < 0) || (theHeight < 0),
                    kRasterInvalidDimensions,
                    "Invalid window dimensions specified");

    defaultWindowWidth_ = theWidth;
    defaultWindowHeight_ = theHeight;
	defaultDimensionsHaveChangedSinceLastRender_ = true;
}

void DrawableObject2D::setWindowDimensions(int theWidth, int theHeight)
{
    FAIL_CONDITION( (theWidth < 0) || (theHeight < 0),
                    kRasterInvalidDimensions,
                    "Invalid window dimensions specified");

    windowWidth_ = theWidth;
    windowHeight_ = theHeight;
    hasIndivWindowDimensions_ = true;
	dimensionsHaveChangedSinceLastRender_ = true;
}

int DrawableObject2D::getWindowWidth(void) const
{
    if (hasIndivWindowDimensions_)
		return windowWidth_;
	else
		return defaultWindowWidth_;

    return windowHeight_;
}

int DrawableObject2D::getWindowHeight(void) const
{
    if (hasIndivWindowDimensions_)
		return windowHeight_;
	else
		return defaultWindowHeight_;

}

bool DrawableObject2D::hasIndividualWindowDimensions(void) const
{
	return hasIndivWindowDimensions_;
}

DrawableInformationType DrawableObject2D::getDrawableInformationType(void) const
{
    return drawableInfoType_;
}

void DrawableObject2D::setDrawableInformationType(DrawableInformationType type)
{
    drawableInfoType_ = type;
}


#if 0
#pragma mark -
#pragma mark Free functions drawing simple shapes
#endif

void uriVL::drawRect(float x, float y, float width, float height)
{
	glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x, y - height);
		glVertex2f(x + width, y - height);
		glVertex2f(x + width, y);		
	glEnd();
}

void uriVL::fillRect(float x, float y, float width, float height)
{
	glBegin(GL_POLYGON);
		glVertex2f(x, y);
		glVertex2f(x, y - height);
		glVertex2f(x + width, y - height);
		glVertex2f(x + width, y);		
	glEnd();
}


void uriVL::drawCenteredRect(float cx, float cy, float width, float height, float angle)
{
	glPushMatrix();
		glTranslatef(cx, cy, 0.f);
		glRotatef(angle, 0.f, 0.f, 1.f);
		glScalef(width, height, 1.f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.5f, -0.5f);
			glVertex2f(0.5f, 0.5f);
			glVertex2f(-0.5f, 0.5f);
		glEnd();
	glPopMatrix();
}


void uriVL::fillCenteredRect(float cx, float cy, float width, float height, float angle)
{
	glPushMatrix();
		glTranslatef(cx, cy, 0.f);
		glRotatef(angle, 0.f, 0.f, 1.f);
		glScalef(width, height, 1.f);
		glBegin(GL_POLYGON);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f(0.5f, -0.5f);
			glVertex2f(0.5f, 0.5f);
			glVertex2f(-0.5f, 0.5f);
		glEnd();
	glPopMatrix();
}


void uriVL::drawOval(float x, float y, float width, float height)
{
	const float HALF_WIDTH = 0.5f*width, HALF_HEIGHT = 0.5f*height;
	glPushMatrix();
		glTranslatef(x + HALF_WIDTH, y - HALF_HEIGHT, 0.f);
		glScalef(HALF_WIDTH, HALF_HEIGHT, 1.f);
		drawCircularDot();
	glPopMatrix();
}


void uriVL::fillOval(float x, float y, float width, float height)
{
	const float HALF_WIDTH = 0.5f*width, HALF_HEIGHT = 0.5f*height;
	glPushMatrix();
		glTranslatef(x + HALF_WIDTH, y - HALF_HEIGHT, 0.f);
		glScalef(HALF_WIDTH, HALF_HEIGHT, 1.f);
		fillCircularDot();
	glPopMatrix();
}


void uriVL::drawCenteredOval(float cx, float cy, float width, float height, float angle)
{
	const float HALF_WIDTH = 0.5f*width, HALF_HEIGHT = 0.5f*height;
	glPushMatrix();
		glTranslatef(cx, cy, 0.f);
		glRotatef(angle, 0.f, 0.f, 1.f);
		glScalef(HALF_WIDTH, HALF_HEIGHT, 1.f);
		drawCircularDot();
	glPopMatrix();
}


void uriVL::fillCenteredOval(float cx, float cy, float width, float height, float angle)
{
	const float HALF_WIDTH = 0.5f*width, HALF_HEIGHT = 0.5f*height;
	glPushMatrix();
		glTranslatef(cx, cy, 0.f);
		glRotatef(angle, 0.f, 0.f, 1.f);
		glScalef(HALF_WIDTH, HALF_HEIGHT, 1.f);
		fillCircularDot();
	glPopMatrix();
}


void uriVL::drawCircularDot(void)
{
	const int NB_PTS = 16;
	const float xy[NB_PTS][2] = {{1.f, 0.f}, {0.92388f, 0.382683f}, {0.707107f, 0.707107f}, 
								{0.382683f, 0.92388f}, {0.f, 1.f}, {-0.382683f, 0.92388f}, 
								{-0.707107f, 0.707107f}, {-0.92388f, 0.382683f}, 
								{-1.f, 0.f}, {-0.92388f, -0.382683f}, {-0.707107f, -0.707107f}, 
								{-0.382683f, -0.92388f}, {0.f, -1.f}, {0.382683f, -0.92388f}, 
								{0.707107f, -0.707107f}, {0.92388f, -0.382683f}};
	glBegin(GL_LINE_LOOP);
		for (int k=0; k<NB_PTS; k++)
			glVertex2fv(xy[k]);
	glEnd();
}


void uriVL::fillCircularDot(void)
{
	const int NB_PTS = 16;
	const float xy[NB_PTS][2] = {{1.f, 0.f}, {0.92388f, 0.382683f}, {0.707107f, 0.707107f}, 
								{0.382683f, 0.92388f}, {0.f, 1.f}, {-0.382683f, 0.92388f}, 
								{-0.707107f, 0.707107f}, {-0.92388f, 0.382683f}, 
								{-1.f, 0.f}, {-0.92388f, -0.382683f}, {-0.707107f, -0.707107f}, 
								{-0.382683f, -0.92388f}, {0.f, -1.f}, {0.382683f, -0.92388f}, 
								{0.707107f, -0.707107f}, {0.92388f, -0.382683f}};
	glBegin(GL_POLYGON);
		for (int k=0; k<NB_PTS; k++)
			glVertex2fv(xy[k]);
	glEnd();
}


void uriVL::drawSquareDot(void)
{
	glBegin(GL_LINE_LOOP);
		glVertex2f(-0.7071f, 0.7071f);
		glVertex2f(-0.7071f, -0.7071f);
		glVertex2f(0.7071f, -0.7071f);
		glVertex2f(0.7071f, 0.7071f);
	glEnd();
}


void uriVL::fillSquareDot(void)
{
	glBegin(GL_POLYGON);
		glVertex2f(-0.7071f, 0.7071f);
		glVertex2f(-0.7071f, -0.7071f);
		glVertex2f(0.7071f, -0.7071f);
		glVertex2f(0.7071f, 0.7071f);
	glEnd();
}


void uriVL::drawDiamondDot(void)
{
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.f, 1.f);
		glVertex2f(-1.f, 0.f);
		glVertex2f(0.f, -1.f);
		glVertex2f(1.f, 0.f);
	glEnd();
}


void uriVL::fillDiamondDot(void)
{
	glBegin(GL_POLYGON);
		glVertex2f(0.f, 1.f);
		glVertex2f(-1.f, 0.f);
		glVertex2f(0.f, -1.f);
		glVertex2f(1.f, 0.f);
	glEnd();
}


