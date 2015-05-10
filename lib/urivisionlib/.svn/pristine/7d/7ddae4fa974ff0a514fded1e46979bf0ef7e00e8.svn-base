/*  NAME:
        VectorField.cpp

    DESCRIPTION:
        implementation of the uriVisionLib Vector class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "VectorField.h"

using namespace uriVL;



#if 0
//----------------------------------------
#pragma mark Initialization of constants
//----------------------------------------
#endif
float VectorField::NEEDLE_DOT_RADIUS = 2.f;


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif


//	This version of the constructor creates an empty vector field
//  at the specified dimensions
VectorField::VectorField(int theNbRows, int theNbCols, bool hasFloatRast,
						DrawableInformationType drawableInfoType)
	try	:   DrawableObject2D(theNbRows, theNbCols, drawableInfoType),
			//
			//by default, I set the vectorial rendering resolution to 1 every 30 pixels
			//	and the color to red
			nVectRes_(30),
			scaleVect_(1.f),
			normMax_(0.f),
			//
			referenceHasChanged_(true),
			normNeedsUpdate_(false),
			normSquaredNeedsUpdate_(false),
			phaseNeedsUpdate_(false),
			hasFloatRaster_(hasFloatRast),
			baseRasterIsReference_(true),
			normAndPhaseIsReference_(false),
			normSquaredAndPhaseIsReference_(false),
			norm_(NULL),
			normSquared_(NULL),
			phase_(NULL)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid dimension parameter error in VectorField constructor");
}


//	This version of the constructor creates an empty vector field
//  at the specified dimensions
VectorField::VectorField(const ImageRect* theRect, bool hasFloatRast,
						DrawableInformationType drawableInfoType)
	try	:	DrawableObject2D(theRect, drawableInfoType),
			//
			//by default, I set the vectorial rendering resolution to 1 every 30 pixels
			//	and the color to red
			nVectRes_(30),
			scaleVect_(1.f),
			normMax_(0.f),
			//
			referenceHasChanged_(true),
			normNeedsUpdate_(false),
			normSquaredNeedsUpdate_(false),
			phaseNeedsUpdate_(false),
			hasFloatRaster_(hasFloatRast),
			baseRasterIsReference_(true),
			normAndPhaseIsReference_(false),
			normSquaredAndPhaseIsReference_(false),
			norm_(NULL),
			normSquared_(NULL),
			phase_(NULL)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid ImageRect parameter error in VectorField constructor");
}

//	This version of the constructor creates an empty vector field
//  at the specified dimensions
VectorField::VectorField(const ImageRect& theRect, bool hasFloatRast,
						DrawableInformationType drawableInfoType)
	try	:	DrawableObject2D(theRect, drawableInfoType),
			//
			//by default, I set the vectorial rendering resolution to 1 every 30 pixels
			//	and the color to red
			nVectRes_(30),
			scaleVect_(1.f),
			normMax_(0.f),
			//
			referenceHasChanged_(true),
			normNeedsUpdate_(false),
			normSquaredNeedsUpdate_(false),
			phaseNeedsUpdate_(false),
			hasFloatRaster_(hasFloatRast),
			baseRasterIsReference_(true),
			normAndPhaseIsReference_(false),
			normSquaredAndPhaseIsReference_(false),
			norm_(NULL),
			normSquared_(NULL),
			phase_(NULL)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid ImageRect parameter error in VectorField constructor");
}


//	This version of the constructor produces a clone of the VectorField object
//	received as parameter.
VectorField::VectorField(const VectorField* theField, bool hasFloatRast,
						DrawableInformationType drawableInfoType)
	try	:   DrawableObject2D(*theField),
			//
			//by default, I set the vectorial rendering resolution to 1 every 30 pixels
			//	and the color to red
			nVectRes_(30),
			scaleVect_(1.f),
			normMax_(0.f),
			//
			referenceHasChanged_(true),
			normNeedsUpdate_(false),
			normSquaredNeedsUpdate_(false),
			phaseNeedsUpdate_(false),
			hasFloatRaster_(hasFloatRast),
			baseRasterIsReference_(true),
			normAndPhaseIsReference_(false),
			normSquaredAndPhaseIsReference_(false),
			norm_(NULL),
			normSquared_(NULL),
			phase_(NULL)
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( theField == NULL,
					kInvalidParameterError,
					"null pointer error in VectorField copy constructor");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter error in VectorField copy constructor");
}

VectorField::VectorField(const VectorField& theField)
	try	:   DrawableObject2D(theField),
			//
			//by default, I set the vectorial rendering resolution to 1 every 30 pixels
			//	and the color to red
			nVectRes_(30),
			scaleVect_(1.f),
			normMax_(0.f),
			//
			referenceHasChanged_(true),
			normNeedsUpdate_(false),
			normSquaredNeedsUpdate_(false),
			phaseNeedsUpdate_(false),
			hasFloatRaster_(false),
			baseRasterIsReference_(true),
			normAndPhaseIsReference_(false),
			normSquaredAndPhaseIsReference_(false),
			norm_(NULL),
			normSquared_(NULL),
			phase_(NULL)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"VectorField copy constructor not implemented");
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by VectorField constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid parameter error in VectorField copy constructor");
}


VectorField::~VectorField(void)
{
	if (norm_ != NULL)
		delete norm_;
	if (normSquared_ != NULL)
		delete normSquared_;
	if (phase_ != NULL)
		delete phase_;
}

const VectorField& VectorField::operator = (const VectorField& theField)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"VectorField copy operator not implemented");
	return *this;
}

#if 0
#pragma mark -
#endif

bool VectorField::hasFloatRaster() const
{
    return hasFloatRaster_;
}

void VectorField::forceRasterUpdate(void)
{
    referenceHasChanged_ = true;
    if (baseRasterIsReference_)
    {
        normNeedsUpdate_ = normSquaredNeedsUpdate_ = phaseNeedsUpdate_ = true;
    }
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Raster getters and setters
//------------------------------------------------------
#endif


const RasterImage_gray_F* VectorField::getNormAsRasterImage(void) const
{
    computeNorm();
    return norm_;
}

RasterImage_gray_F* VectorField::getNormAsRasterImage(ReadWriteStatus rw)
{
    computeNorm();
    return norm_;
}

void VectorField::getNormAsRasterImage(RasterImage_gray* normOut)
{
    FAIL_CONDITION( normOut == NULL,
                    kNullRasterImageError,
                    "null raster image destination in VectorField::getNormAsRasterImage");

    computeNorm();
    norm_->copyInto(normOut);
}

const RasterImage_gray_F* VectorField::getNormSquaredAsRasterImage(void) const
{
    computeNormSquared();
    return normSquared_;
}

RasterImage_gray_F* VectorField::getNormSquaredAsRasterImage(ReadWriteStatus rw)
{
    computeNormSquared();
    return normSquared_;
}

void VectorField::getNormSquaredAsRasterImage(RasterImage_gray* norm2Out)
{
    FAIL_CONDITION( norm2Out == NULL,
                    kNullRasterImageError,
                    "null raster image destination in VectorField::computeNormSquaredAsRasterImage");

    computeNormSquared();
    normSquared_->copyInto(norm2Out);
}

const RasterImage_gray_F* VectorField::getPhaseAsRasterImage(void) const
{
    computePhase();
    return phase_;
}

RasterImage_gray_F* VectorField::getPhaseAsRasterImage(ReadWriteStatus rw)
{
    computePhase();
    return phase_;
}

void VectorField::getPhaseAsRasterImage(RasterImage_gray* phaseOut)
{
    FAIL_CONDITION( phaseOut == NULL,
                    kNullRasterImageError,
                    "null raster image destination in VectorField::getPhaseAsRasterImage");

    computePhase();
    phase_->copyInto(phaseOut);
}

void VectorField::getNormAndPhaseAsRasterImages(RasterImage_gray* normOut,
        RasterImage_gray* phaseOut)
{
    FAIL_CONDITION( normOut == NULL,
                    kNullRasterImageError,
                    "null raster image destination in VectorField::getNormAsRasterImage");
    FAIL_CONDITION( phaseOut == NULL,
                    kNullRasterImageError,
                    "null raster image destination in VectorField::getPhaseAsRasterImage");

    computeNormAndPhase();

    //  copy the local rasters into the destination images provided
    norm_->copyInto(normOut);
    phase_->copyInto(phaseOut);
}

void VectorField::getNormSquaredAndPhaseAsRasterImages(RasterImage_gray* norm2Out,
        RasterImage_gray* phaseOut)
{
    FAIL_CONDITION( norm2Out == NULL,
                    kNullRasterImageError,
                    "null raster image destination in VectorField::getNormAsRasterImage");
    FAIL_CONDITION( phaseOut == NULL,
                    kNullRasterImageError,
                    "null raster image destination in VectorField::getPhaseAsRasterImage");
    computeNormSquaredAndPhase();

    //  copy the local rasters into the destination images provided
    normSquared_->copyInto(norm2Out);
    phase_->copyInto(phaseOut);
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Norm and phase
//------------------------------------------------------
#endif



void VectorField::computeNorm(void) const
{
    bool mustComputeNorm = false;

    //  If the norm has never been allocated do so now
    if (norm_ == NULL)
    {
        norm_ = new RasterImage_gray_F(getBoundRect());
        mustComputeNorm = true;
    }
    //  otherwise check if the vector field has changed since last use
    else
        mustComputeNorm = normNeedsUpdate_;

    //  update the rasters that need it
    if (mustComputeNorm)
    {
        computeNorm_(norm_);
        norm_->requestRasterUpdate();
    }

    normNeedsUpdate_ = false;
    norm_->setValidRect(this->getValidRect());
    norm_->setFloatConversionMode(kPositiveFloat);
}

void VectorField::computeNormSquared(void) const
{
    bool    mustComputeNorm2 = false;

    //  If the norm has never been allocated do so now
    if (normSquared_ == NULL)
    {
        normSquared_ = new RasterImage_gray_F(getBoundRect());
        mustComputeNorm2 = true;
    }
    //  otherwise check if the vector field has changed since last use
    else
        mustComputeNorm2 = normSquaredNeedsUpdate_;


    //  update the rasters that need it
    if (mustComputeNorm2)
    {
        computeNormSquared_(normSquared_);
        normSquared_->referenceHasChanged();
    }

    normSquaredNeedsUpdate_ = false;
    normSquared_->setValidRect(this->getValidRect());
    normSquared_->setFloatConversionMode(kPositiveFloat);
}

void VectorField::computePhase(void) const
{
    bool    mustComputePhase = false;

    //  If the phase has never been allocated do so now
    if (phase_ == NULL)
    {
        phase_ = new RasterImage_gray_F(getBoundRect());
        mustComputePhase = true;
    }
    //  otherwise check if the vector field has changed since last use
    else
        mustComputePhase = phaseNeedsUpdate_;

    //  update the rasters that need it
    if (mustComputePhase)
    {
        computePhase_(phase_);
        phase_->referenceHasChanged();
    }

    phaseNeedsUpdate_ = false;
    phase_->setValidRect(getValidRect());
}

void VectorField::computeNormAndPhase(void) const
{
    bool    mustComputePhase = false,
                               mustComputeNorm = false;

    //  If the norm has never been allocated do so now
    if (norm_ == NULL)
    {
        norm_ = new RasterImage_gray_F(getBoundRect());
        mustComputeNorm = true;
    }
    //  otherwise check if the vector field has changed since last use
    else
        mustComputeNorm = normNeedsUpdate_;

    //  If the phase has never been allocated do so now
    if (phase_ == NULL)
    {
        phase_ = new RasterImage_gray_F(getBoundRect());
        mustComputePhase = true;
    }
    //  otherwise check if the vector field has changed since last use
    else
        mustComputePhase = phaseNeedsUpdate_;

    //  update the rasters that need it
    if (mustComputeNorm)
        if (mustComputePhase)
        {
            computeNormAndPhase_(norm_, phase_);
            norm_->referenceHasChanged();
            phase_->referenceHasChanged();
        }
        else
        {
            computeNorm_(norm_);
            norm_->referenceHasChanged();
        }
    else if (mustComputePhase)
    {
        computePhase_(phase_);
        phase_->referenceHasChanged();
    }

    normNeedsUpdate_ = false;
    phaseNeedsUpdate_ = false;
    norm_->setValidRect(getValidRect());
    phase_->setValidRect(getValidRect());
}

void VectorField::computeNormSquaredAndPhase(void)
{
    bool    mustComputePhase = false,
                               mustComputeNorm2 = false;

    //  If the norm has never been allocated do so now
    if (normSquared_ == NULL)
    {
        normSquared_ = new RasterImage_gray_F(getBoundRect());
        mustComputeNorm2 = true;
    }
    //  otherwise check if the vector field has changed since last use
    else
        mustComputeNorm2 = normSquaredNeedsUpdate_;

    //  If the phase has never been allocated do so now
    if (phase_ == NULL)
    {
        phase_ = new RasterImage_gray_F(getBoundRect());
        mustComputePhase = true;
    }
    //  otherwise check if the vector field has changed since last use
    else
        mustComputePhase = phaseNeedsUpdate_;

    //  update the rasters that need it
    if (mustComputeNorm2)
        if (mustComputePhase)
        {
            computeNormSquaredAndPhase_(normSquared_, phase_);
            normSquared_->referenceHasChanged();
            phase_->referenceHasChanged();
        }
        else
        {
            computeNormSquared_(normSquared_);
            normSquared_->referenceHasChanged();
        }
    else if (mustComputePhase)
    {
        computePhase_(phase_);
        phase_->referenceHasChanged();
    }

    normSquaredNeedsUpdate_ = false;
    phaseNeedsUpdate_ = false;
    normSquared_->setValidRect(this->getValidRect());
    phase_->setValidRect(this->getValidRect());
}

#if 0
#pragma mark -
#endif

void VectorField::setRasterAsReference(void)
{
    baseRasterIsReference_ = true;
    normAndPhaseIsReference_ = normSquaredAndPhaseIsReference_ = false;
}

void VectorField::setNormAndPhaseAsReference(void)
{
    normAndPhaseIsReference_ = true;
    baseRasterIsReference_ = normSquaredAndPhaseIsReference_ = false;
}

void VectorField::setNormSquaredAndPhaseAsReference(void)
{
    normSquaredAndPhaseIsReference_ = true;
    baseRasterIsReference_ = normAndPhaseIsReference_ = false;
}

bool VectorField::referenceHasChanged(void) const
{
    return referenceHasChanged_;
}

void VectorField::setReferenceHasChanged_(bool hasChanged) const
{
    referenceHasChanged_ = hasChanged;
}

void VectorField::setReferenceHasChanged(void) const
{
    referenceHasChanged_ = true;
}


#if 0
#pragma mark -
#endif

void VectorField::copyInto(VectorField* vectOut) const
{
    copyInto(vectOut, getValidRect());
}


void VectorField::copyInto(VectorField* vectOut, const ImageRect* theRect) const
{
    FAIL_CONDITION(	vectOut == NULL,
                    kNullVectorFieldError,
                    "null vector field passed as parameter");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in VectorField_F::copyInto");

    //	We only copy from the validRect_, so we need to calculate its intersection
    //	with theRect.
    ImageRect	*copyRect = ImageRect::intersection(getValidRect(), theRect);
    FAIL_CONDITION( copyRect == NULL,
                    kUriBaseError,
                    "the copy rectangle specified is not within valid data");

    //	if theRect was not already the bounding rectangle of the destination field,
    //	we make it so.
    if (!theRect->coincidesWith(vectOut->getBoundRect()))
    {
        vectOut->deleteShiftedRaster2D_();
        vectOut->deleteRaster2D_( );

        vectOut->setValidRect(getBoundRect());
        vectOut->setBoundRect(getBoundRect());
        vectOut->allocateRaster2D_(getHeight(), getWidth());
        vectOut->allocateShiftedRaster2D_(getHeight(), getWidth(), getTop(), getLeft());
    }

    //	set the destination field's new valid data rect.
    vectOut->setValidRect(copyRect);

	delete copyRect;
}



void VectorField::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
	//  The ImageRect member will be updated in DrawableObject2D::setBoundRect, so we only need
	//	to update the "shifted" array (no re-allocation needed)

    //	if width and height have not changed, then all we need to do is update the
    //	ImageRect member and translate the "shifted" array.
    if ((theWidth == getWidth()) && (theHeight == getHeight()))
    {
        if ((x != getLeft()) || (y != getTop()))
        {
            translateShiftedRaster2D_(y - getTop(), x - getLeft());
        }
    }
    //	if the dimensions of the rectangle are different, then we must delete
    //	and reallocate the 0-based and "shifted" 2D rasters.
    else
    {
        //	I am not sure if this is the right thing to do: should subclasses completely
        //	reimplement this or call this function if they have to override it?
        //	This is opting for the latter.
        deleteShiftedRaster2D_();
        deleteRaster2D_();

        allocateRaster2D_(theHeight, theWidth);
        allocateShiftedRaster2D_(theHeight, theWidth, y, x);
    }

    //  If the norm, norm squared, or phase have been allocated then their bounding
    //  data rect must be modified too.
    if (norm_ != NULL)
        norm_->setBoundRect(x, y, theWidth, theHeight);
    if (normSquared_ != NULL)
        normSquared_->setBoundRect(x, y, theWidth, theHeight);
    if (phase_ != NULL)
        phase_->setBoundRect(x, y, theWidth, theHeight);

}

void VectorField::setValidRect_(int x, int y, int theWidth, int theHeight)
{
    //  If the norm, norm squared, or phase have been allocated then their valid
    //  data rect must be modified too.
    if (norm_ != NULL)
        norm_->setValidRect(x, y, theWidth, theHeight);
    if (normSquared_ != NULL)
        normSquared_->setValidRect(x, y, theWidth, theHeight);
    if (phase_ != NULL)
        phase_->setValidRect(x, y, theWidth, theHeight);

}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Rendering functions
//------------------------------------------------------
#endif


void VectorField::render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
						  float theZ, bool shifted, const ImagePoint* origin) const
{
    //--------------------------------------------------------------
    //	Part 1: make sure that what we draw is up to date
    //--------------------------------------------------------------
    if (referenceHasChanged_)
	{
        prepareVectFieldForRendering_(nVectRes_, scaleVect_, normMax_);
		referenceHasChanged_ = false;
	}
	
    //--------------------------------------------------------------
    //	Part 2: render the vector field
    //--------------------------------------------------------------
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(scaledLeft, scaledTop, theZ);
    glColor4fv(getColor());
    glScalef(getRenderingScaleX(), -getRenderingScaleY(), 1.f);

    //	now let the child class take over
    renderNeedles_(nVectRes_, scaleVect_);

    glPopMatrix();

}

void VectorField::setVectResolution(int nPts)
{
    nVectRes_ = nPts;
    prepareVectFieldForRendering_(nVectRes_, scaleVect_, normMax_);
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Must implement these (useless) functions
//------------------------------------------------------
#endif


void VectorField::computeBoundingRect_(ImageRect& rect) const
{
	//	nothing to do
}

void VectorField::computeValidRect_(ImageRect& rect) const
{
	//	nothing to do
}

void VectorField::computeCentroid_(ImagePoint_F& cent) const
{
	//	nothing to do
}

void VectorField::computeCenterOfMass_(ImagePoint_F& cent) const
{
	//	nothing to do
}


