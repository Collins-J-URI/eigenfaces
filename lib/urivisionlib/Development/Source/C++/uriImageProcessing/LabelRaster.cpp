/*  NAME:
        LabelRaster.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib LabelRaster class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstring>
//
#include "LabelRaster.h"


using namespace uriVL;


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors & destructor
//------------------------------------------------------
#endif

LabelRaster::LabelRaster(int nbRows, int nbCols)
	try	:	label_(NULL),
			label2D_(NULL),
			shiftedLabel2D_(NULL),
			boundRect_ (new ImageRect(0, 0, nbCols, nbRows)),
			validRect_(boundRect_)
{

    allocateRasters_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by LabelRaster constructor");
	throw e;
}
catch(...) {
    FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "Failure in LabelRaster constructor");

}



LabelRaster::LabelRaster(const ImageRect* theRect)
		:	label_(NULL),
			label2D_(NULL),
			shiftedLabel2D_(NULL),
			boundRect_ (NULL),
			validRect_(NULL)
{
	try {
		boundRect_ = new ImageRect(theRect);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called from LabelRaster constructor.");
		throw e;
	}
    FAIL_CONDITION( boundRect_ == NULL,
                    kNullRectangleError,
                    "ImageRect creation failed in LabelRaster constructor");

    if (UriVL::validDataRectIsSafe())
    {
        validRect_ = new ImageRect(theRect->getLeft(), theRect->getTop(), 0, 0);
        FAIL_CONDITION( validRect_ == NULL,
                        kNullRectangleError,
                        "ImageRect creation failed in LabelRaster constructor");
    }
    else
        validRect_ = boundRect_;

    allocateRasters_();
}



LabelRaster::LabelRaster(const LabelRaster* label)
	try	:	label_(NULL),
			label2D_(NULL),
			shiftedLabel2D_(NULL),
			boundRect_ (new ImageRect(label->getBoundRect())),
			validRect_(NULL)
{
    FAIL_CONDITION( label == NULL,
                    kNullLabelRasterError,
                    "NULL LabelRaster parameter in LabelRaster constructor");

	try {
		boundRect_ = new ImageRect(label->getBoundRect());
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called from LabelRaster constructor.");
		throw e;
	}
	catch(...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid pointer to a LabelRaster passed to LabelRaster constructor");
	}
    FAIL_CONDITION( boundRect_ == NULL,
                    kNullRectangleError,
                    "ImageRect creation failed in LabelRaster constructor");

    if (UriVL::validDataRectIsSafe())
    {
		const ImageRect* rect = label->getBoundRect();
        validRect_ = new ImageRect(rect->getLeft(), rect->getTop(), 0, 0);
        FAIL_CONDITION( validRect_ == NULL,
                        kNullRectangleError,
                        "ImageRect creation failed in LabelRaster constructor");
    }
    else
        validRect_ = boundRect_;

    allocateRasters_();
	
	//	copy the data
	try {
		memcpy(label_, label->label_, getHeight()*getWidth());
	}
	catch (...) {
    FAIL_CONDITION( true,
                    kMemoryCopyError,
                    "Memory copy error in LabelRaster constructor");	
	}
}
catch (ErrorReport& e) {
	e.appendToMessage("called by LabelRaster copy constructor");
	throw e;
}
catch(...) {
    FAIL_CONDITION( label == NULL,
                    kNullParameterError,
                    "NULL LabelRaster parameter in LabelRaster constructor");
    FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "Failure in LabelRaster constructor");
}


LabelRaster::LabelRaster(const LabelRaster& label)
		:	label_(NULL),
			label2D_(NULL),
			shiftedLabel2D_(NULL),
			boundRect_ (NULL),
			validRect_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Copy contructor of LabelRaster class not implemented");

}


LabelRaster::~LabelRaster(void)
{
    if (label2D_ != NULL)
        delete []label2D_;

    if (shiftedLabel2D_ != NULL)
        delete []shiftedLabel2D_;

    delete []label_;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copying
//------------------------------------------------------
#endif

void LabelRaster::copyInto(LabelRaster *label) const
{
    FAIL_CONDITION( label == NULL,
                    kNullLabelRasterError,
                    "NULL LabelRaster parameter in LabelRaster::copyInto");

	try {
		label->setRect(boundRect_);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by LabelRaster::copyInto");
		throw e;
	}

	//	copy the data
	try {
		memcpy(label_, label->label_, getHeight()*getWidth());
	}
	catch (...) {
    FAIL_CONDITION( true,
                    kMemoryCopyError,
                    "Memory copy error in LabelRaster::copyInto");	
	}
}

const LabelRaster& LabelRaster::operator = (const LabelRaster& label)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Copy operator of LabelRaster class not implemented yet");
	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Setters and getters
//------------------------------------------------------
#endif


const ImageRect* LabelRaster::getBoundRect(void) const
{
    //    return new ImageRect(boundRect_);
    return boundRect_;
}

const ImageRect* LabelRaster::getValidRect(void) const
{
    //    return new ImageRect(validRect_);
    return validRect_;
}


bool LabelRaster::coincidesWith(const LabelRaster* img) const
{
    FAIL_CONDITION( img == NULL,
                    kNullLabelRasterError,
                    "Test of null LabelRaster in LabelRaster::coincidesWith");
    const ImageRect* theRect = img->getBoundRect();
    FAIL_CONDITION( (boundRect_ == NULL) || (theRect == NULL),
                    kNullRectangleError,
                    "Test of null ImageRect member in LabelRaster::coincidesWith");

    return  boundRect_->coincidesWith(theRect);
}

bool LabelRaster::overlaps(const LabelRaster* img) const
{
    FAIL_CONDITION( img == NULL,
                    kNullLabelRasterError,
                    "Test of null LabelRaster in LabelRaster::coincidesWith");
    const ImageRect* theRect = img->getBoundRect();
    FAIL_CONDITION( (boundRect_ == NULL) || (theRect == NULL),
                    kNullRectangleError,
                    "Test of null ImageRect member in LabelRaster::overlaps");

    return  boundRect_->overlaps(theRect);
}

bool LabelRaster::contains(const LabelRaster* img) const
{
    FAIL_CONDITION( img == NULL,
                    kNullLabelRasterError,
                    "Test of null LabelRaster in LabelRaster::coincidesWith");
    const ImageRect* theRect = img->getBoundRect();
    FAIL_CONDITION( (boundRect_ == NULL) || (theRect == NULL),
                    kNullRectangleError,
                    "Test of null ImageRect member in LabelRaster::contains");

    return  boundRect_->contains(theRect);
}

bool LabelRaster::isContainedBy(const LabelRaster* img) const
{
    FAIL_CONDITION( img == NULL,
                    kNullLabelRasterError,
                    "Test of null LabelRaster in LabelRaster::coincidesWith");
    const ImageRect* theRect = img->getBoundRect();
    FAIL_CONDITION( (boundRect_ == NULL) || (theRect == NULL),
                    kNullRectangleError,
                    "Test of null ImageRect member in LabelRaster::isContainedBy");

    return  boundRect_->isContainedBy(theRect);
}

int LabelRaster::getHeight(void) const
{
    return boundRect_->getHeight( );
}

int LabelRaster::getWidth(void) const
{
    return boundRect_->getWidth( );
}


void LabelRaster::setOrigin(int x, int y)
{
    boundRect_->setOrigin(x, y);
}

void LabelRaster::translate(int dx, int dy)
{
    boundRect_->translate(dx, dy);
}

void LabelRaster::setRect(const ImageRect* theRect)
{
    setRect(theRect->getLeft(), theRect->getTop(), theRect->getWidth(), theRect->getHeight());
}

void LabelRaster::setRect(int x, int y, int theWidth, int theHeight)
{
    //	if width and height have not changed, then all we need to do is update the
    //	ImageRect member and reallocate the "shifted" array.
    if ((theWidth == getWidth()) && (theHeight == getHeight()))
    {
        if ((x != boundRect_->getLeft()) || (y != boundRect_->getTop()))
        {
            if (shiftedLabel2D_ != NULL)
                delete []shiftedLabel2D_;

            //  if the valid rect was equal to the bounding rect, we simply
            //  reassign validRect_, otherwise we have to translate it first
            if (validRect_ != boundRect_)
                validRect_->translate(x - boundRect_->getLeft(), y - boundRect_->getRight());

            boundRect_->setRect(x, y, theWidth, theHeight);
            allocateShiftedRaster2D_();
        }
        //  if nothing has changed, there is nothing to do
    }
    //	if the dimensions of the rectangle are different, then we must delete
    //	and reallocate the GraphWorld as well as the 0-based 2D raster.

    else
    {
        //  First we delete the rasters
        if (label2D_ != NULL)
            delete []label2D_;

        if (shiftedLabel2D_ != NULL)
            delete []shiftedLabel2D_;

        delete []label_;

        //  if the valid rect was equal to the bounding rect, we simply
        //  reassign validRect_, otherwise we have to translate it first
        if (validRect_ != boundRect_)
            delete validRect_;
        validRect_ = boundRect_;

        boundRect_->setRect(x, y, theWidth, theHeight);

        //  then we allocate the rasters
        allocateRasters_();
    }
}

void LabelRaster::setValidRect(const ImageRect* theRect)
{
    setValidRect(theRect->getLeft(), theRect->getTop(), theRect->getWidth(), theRect->getHeight());
}

void LabelRaster::setValidRect(int x, int y, int theWidth, int theHeight)
{
    //	The new valid data rect must be contained by the bounding rectangle
    FAIL_CONDITION(	( x < boundRect_->getLeft() ) ||
                    ( x + theWidth - 1 > boundRect_->getRight() ) ||
                    ( y < boundRect_->getTop() ) ||
                    ( y + theHeight - 1 > boundRect_->getBottom() ),
                    kInvalidRectangleError,
                    "New valid data rectangle must be within the bounding rectangle");

    //	if the valid data rect used to coincide with the bounding rect, we must allocate
    //	a new rectangle
    if (validRect_ == boundRect_)
        validRect_ = new ImageRect(x, y, theWidth, theHeight);
    else
        validRect_->setRect(x, y, theWidth, theHeight);

}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to rasters
//------------------------------------------------------
#endif

const char* LabelRaster::getLabel(void) const
{
    return label_;
}


char* LabelRaster::getLabel(ReadWriteStatus rw) const
{
    return label_;
}


const char* const* LabelRaster::getLabel2D(void) const
{
    return label2D_;
}

char** LabelRaster::getLabel2D(ReadWriteStatus rw) const
{
    return label2D_;
}

const char* const* LabelRaster::getShiftedLabel2D(void) const
{

    //  if the label raster has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if (boundRect_->upperLeftIsAtOrigin())
        return label2D_;
    else
        return shiftedLabel2D_;
}

char** LabelRaster::getShiftedLabel2D(ReadWriteStatus rw) const
{

    //  if the label raster has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if (boundRect_->upperLeftIsAtOrigin())
        return label2D_;
    else
        return shiftedLabel2D_;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Raster allocation
//------------------------------------------------------
#endif


void LabelRaster::allocateRasters_(void)
{
    int	nbRows = boundRect_->getHeight(),
		nbCols = boundRect_->getWidth();

    //---------------
    //	allocate 1D raster
    //---------------
    label_ = new char[nbRows*nbCols];
    FAIL_CONDITION( label_ == NULL,
                    kRasterAllocationFailure,
                    "Label raster allocation failed in LabelRaster constructor");

    //---------------
    //	allocate 2D raster
    //---------------
    if (label2D_ != NULL)
        delete []label2D_;

    label2D_ = new char*[nbRows];
    FAIL_CONDITION(label2D_ == NULL, kRasterAllocationFailure,
                   "Could not allocate label2D_ array");

    label2D_[0] = label_;
    for (int i=1; i<nbRows; i++)
        label2D_[i] = label2D_[i-1] + nbCols;

    //---------------
    //	allocate shifted raster (if needed)
    //---------------
    allocateShiftedRaster2D_();
}

void LabelRaster::allocateShiftedRaster2D_(void)
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if (!boundRect_->upperLeftIsAtOrigin())
    {
        const int   iLow  = boundRect_->getTop(),
                    iHigh = boundRect_->getBottom(),
					jLow  = boundRect_->getLeft(),
                    nbRows = getHeight(),
                    nbCols = getWidth();

        if (shiftedLabel2D_ != NULL)
            delete shiftedLabel2D_;
        shiftedLabel2D_ = new char*[nbRows];
        FAIL_CONDITION(	shiftedLabel2D_ == NULL,
                        kRasterAllocationFailure,
                        "label  raster allocation failed");

        shiftedLabel2D_ -= iLow;
        shiftedLabel2D_[iLow] = label2D_[0] - jLow;
        for (int i = iLow; i < iHigh; i++)
            shiftedLabel2D_[i+1] = shiftedLabel2D_[i] + nbCols;

    }
    else
        shiftedLabel2D_ = NULL;

}


