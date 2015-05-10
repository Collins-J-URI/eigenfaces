/*  NAME:
        RasterImage_gray_F.cpp

    DESCRIPTION:
        implementation of the uriVisionLib RasterImage_gray_F class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstring>
//
#include "RasterImage_gray_F.h"
#include "RasterImage_RGBa_F.h"

using namespace uriVL;

//----------------------------------------------------------
//  static members
//----------------------------------------------------------

ConversionFromFloatMode RasterImage_gray_F::defaultFloatConv_ = kSignedFloat;

#if 0
//----------------------------------
#pragma mark Constructors and destructor
//----------------------------------
#endif

RasterImage_gray_F::RasterImage_gray_F(void)
        :   RasterImage_gray( ),
			//
			fltGray_(NULL),
			fltGray2D_(NULL),
			shiftedFltGray2D_(NULL),
			//
			hasIndivFloatConvMode_(false),
			floatConv_(kUndefFloatConv),
			valMin_(+HUGE_VALF),
			valMax_(-HUGE_VALF),
			convScale_(1.f)			
{
}


RasterImage_gray_F::RasterImage_gray_F(int nbRows, int nbCols)
		try:	RasterImage_gray(nbRows, nbCols),
				//
				fltGray_(NULL),
				fltGray2D_(NULL),
				shiftedFltGray2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				convScale_(1.f)			
{
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);

    setGraphWorldAsReference_();
    setReferenceHasChanged_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray_F constructor");
	throw e;
}


RasterImage_gray_F::RasterImage_gray_F(int nbRows, int nbCols, const unsigned char* theGray)
		try	:   RasterImage_gray(nbRows, nbCols, theGray),
				//
				fltGray_(NULL),
				fltGray2D_(NULL),
				shiftedFltGray2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kPositiveFloat),	//  all the values in this raster are positive
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				convScale_(1.f)			
{
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray_F constructor");
	throw e;
}


RasterImage_gray_F::RasterImage_gray_F(int nbRows, int nbCols, const float* theGray)
		try	:   RasterImage_gray(nbRows, nbCols),
				//
				fltGray_(NULL),
				fltGray2D_(NULL),
				shiftedFltGray2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				convScale_(1.f)			
{
    FAIL_CONDITION( theGray == NULL,
                    kNullRasterError,
                    "null raster as parameter in RasterImage_gray_F constructor");

    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);

    long    lSource = 0;
    for (int i=0; i<nbRows; i++)
        for (int j=0; j<nbCols; j++)
            fltGray2D_[i][j] = theGray[lSource++];

	setValidRect(0, 0, nbCols, nbRows);
    setFloatRasterAsReference_();
    setReferenceHasChanged_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray_F constructor");
	throw e;
}



RasterImage_gray_F::RasterImage_gray_F(const ImageRect* theRect)
    try	:	RasterImage_gray(theRect),
			//
			fltGray_(NULL),
			fltGray2D_(NULL),
			shiftedFltGray2D_(NULL),
			hasIndivFloatConvMode_(false),
			floatConv_(kUndefFloatConv),
			valMin_(+HUGE_VALF),
			valMax_(-HUGE_VALF),
			convScale_(1.f)			
{
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(), theRect->getTop(), theRect->getLeft());

    setGraphWorldAsReference_();
    setReferenceHasChanged_(false);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray constructor");
	throw e;
}


RasterImage_gray_F::RasterImage_gray_F(const ImageRect* theRect, const unsigned char* theGray)
		try	:   RasterImage_gray(theRect, theGray),
				//
				fltGray_(NULL),
				fltGray2D_(NULL),
				shiftedFltGray2D_(NULL),
				hasIndivFloatConvMode_(false),
				floatConv_(kPositiveFloat),	//  all the values in this raster are positive
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				convScale_(1.f)			
{
//    const int   iLow = theRect->getTop(),
//        		iHigh = theRect->getBottom(),
//        		jLow = theRect->getLeft(),
//        		jHigh = theRect->getRight();

    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(), 
							 theRect->getTop(), theRect->getLeft());

//	try {
//		long    lSource = 0;
//		for (int i=iLow; i<=iHigh; i++)
//			for (int j=jLow; j<=jHigh; j++)
//				shiftedFltGray2D_[i][j] = theGray[lSource++];
//	}
//	catch(...) {
//		FAIL_CONDITION( theGray == NULL,
//						kNullRasterError,
//						"null raster as parameter in RasterImage_gray_F constructor");
//		FAIL_CONDITION( true,
//						kMemoryCopyError,
//						"Data copy error in RasterImage_gray_F constructor");
//	}
	
    setHasFloatRaster_(true);
//    setFloatRasterAsReference_();
//    setReferenceHasChanged_(true);
//	setValidRect(theRect);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray_F constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( theRect == NULL,
					kNullRectangleError,
					"null ImageRect as parameter in RasterImage_gray_F constructor");
}

RasterImage_gray_F::RasterImage_gray_F(const ImageRect* theRect, const float* theGray)
		try	:	RasterImage_gray(theRect),
				//
				fltGray_(NULL),
				fltGray2D_(NULL),
				shiftedFltGray2D_(NULL),
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				convScale_(1.f)			
{
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in RasterImage_gray_F constructor");
    FAIL_CONDITION( theGray == NULL,
                    kNullRasterError,
                    "null raster as parameter in RasterImage_gray_F constructor");

    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(), theRect->getTop(), theRect->getLeft());

    const int   iLow = theRect->getTop(),
        		iHigh = theRect->getBottom(),
        		jLow = theRect->getLeft(),
        		jHigh = theRect->getRight();

    long    lSource = 0;
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
            shiftedFltGray2D_[i][j] = theGray[lSource++];

	setValidRect(theRect);
    setFloatRasterAsReference_();
    setReferenceHasChanged_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray_F constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( theRect == NULL,
					kNullRectangleError,
					"null ImageRect as parameter in RasterImage_gray_F constructor");
}


RasterImage_gray_F::RasterImage_gray_F(const GraphWorld* gworld)
        :   RasterImage_gray( ),
			//
			fltGray_(NULL),
			fltGray2D_(NULL),
			shiftedFltGray2D_(NULL),
			hasIndivFloatConvMode_(false),
			floatConv_(kUndefFloatConv),
			valMin_(+HUGE_VALF),
			valMax_(-HUGE_VALF),
			convScale_(1.f)			
{
    FAIL_CONDITION( true, kFunctionNotImplemented,
                    "RasterImage_gray GraphWorld clone constructor is not operative yet");
}

RasterImage_gray_F::RasterImage_gray_F(GraphWorld* gworld, const bool keepThisGraphWorld)
        :   RasterImage_gray( ),
			//
			fltGray_(NULL),
			fltGray2D_(NULL),
			shiftedFltGray2D_(NULL),
			hasIndivFloatConvMode_(false),
			floatConv_(kUndefFloatConv),
			valMin_(+HUGE_VALF),
			valMax_(-HUGE_VALF),
			convScale_(1.f)			
{
    FAIL_CONDITION( true, kFunctionNotImplemented,
                    "RasterImage_gray GraphWorld clone constructor is not operative yet");
}

//  this constructor has to be changed.  So far, I needlessly allocate
//  the base raster when in fact I should write directly into the
//  float raster.
RasterImage_gray_F::RasterImage_gray_F(const RasterImage* img)
		try	:   RasterImage_gray(img),
				//
				fltGray_(NULL),
				fltGray2D_(NULL),
				shiftedFltGray2D_(NULL),
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				convScale_(1.f)			
{
    //  Although technically the float raster does not exist yet,
    //  we do this to force the writing to take place in the float
    //  raster rather than in the GraphWorld.
    setHasFloatRaster_(true);
    setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

	try {
		img->copyInto(this, img->getValidRect());
	}
	catch(ErrorReport& e) {
		e.appendToMessage("called by RasterImage_gray_F constructor");
		throw e;
	}


    //  if img was a float raster image, then in the copyInto function
    //  it already attributed "me" its float to gray conversion mode.
    //  If img does not have a float raster, then all its values
    //  are positive
    if (!img->hasFloatRaster())
        setFloatConversionMode(kPositiveFloat);

}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray_F constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( img == NULL,
					kNullRectangleError,
					"null RasterImage pointer as parameter in RasterImage_gray_F constructor");
}

//  this constructor has to be changed.  So far, I needlessly allocate
//  the base raster when in fact I should write directly into the
//  float raster.
RasterImage_gray_F::RasterImage_gray_F(const RasterImage* img, const ImageRect* theRect)
		try	:	RasterImage_gray(img, theRect),
				//
				fltGray_(NULL),
				fltGray2D_(NULL),
				shiftedFltGray2D_(NULL),
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				convScale_(1.f)			
{
    //  Although technically the float raster does not exist yet,
    //  we do this to force the writing to take place in the float
    //  raster rather than in the GraphWorld.
    setHasFloatRaster_(true);
    setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

	try {
		img->copyInto(this, theRect);
	}
	catch(ErrorReport& e) {
		e.appendToMessage("called by RasterImage_gray_F constructor");
		throw e;
	}

    //  if img was a float raster image, then in the copyInto function
    //  it already attributed "me" its float to gray conversion mode.
    //  If img does not have a float raster, then all its values
    //  are positive
    if (!img->hasFloatRaster())
        setFloatConversionMode(kPositiveFloat);

    setHasFloatRaster_(true);
    setFloatRasterAsReference_();
    setReferenceHasChanged_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_gray_F constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( img == NULL,
					kNullRectangleError,
					"null RasterImage pointer as parameter in RasterImage_gray_F constructor");
	FAIL_CONDITION( theRect == NULL,
					kNullRectangleError,
					"null ImageRect as parameter in RasterImage_gray_F constructor");
}


RasterImage_gray_F::RasterImage_gray_F(const RasterImage_gray_F& img)
        :   RasterImage_gray( ),
			//
			fltGray_(NULL),
			fltGray2D_(NULL),
			shiftedFltGray2D_(NULL),
			hasIndivFloatConvMode_(false),
			floatConv_(kUndefFloatConv),
			valMin_(+HUGE_VALF),
			valMax_(-HUGE_VALF),
			convScale_(1.f)			
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_gray_F copy constructor not implemented");
}


RasterImage_gray_F::~RasterImage_gray_F(void)
{
    deleteShiftedRaster2D_(getTop());
    deleteRaster2D_();
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Access to the rasters
//------------------------------------------------------
#endif


const unsigned char* RasterImage_gray_F::getGray(void) const
{
    if ( graphWorldIsNull_() ||
            (!graphWorldIsReference() && referenceHasChanged()) )
        updateRaster_();

    return RasterImage_gray::getRaster();
}


unsigned char* RasterImage_gray_F::getGray(ReadWriteStatus rw)
{
    if ( graphWorldIsNull_() ||
            (!graphWorldIsReference() && referenceHasChanged()) )
        updateRaster_();

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    setGraphWorldAsReference_();
    setReferenceHasChanged_(true);

    return RasterImage_gray::getRaster(R_W_ACCESS);
}


const unsigned char* const* RasterImage_gray_F::getGray2D(void) const
{
    if ( graphWorldIsNull_() ||
            (!graphWorldIsReference() && referenceHasChanged()) )
        updateRaster_();

    return RasterImage_gray::getGray2D();
}

unsigned char*const* RasterImage_gray_F::getGray2D(ReadWriteStatus rw)
{
    if ( graphWorldIsNull_() ||
            (!graphWorldIsReference() && referenceHasChanged()) )
        updateRaster_();

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    setGraphWorldAsReference_();
    setReferenceHasChanged_(true);

    return RasterImage_gray::getGray2D(R_W_ACCESS);
}

const unsigned char* const* RasterImage_gray_F::getShiftedGray2D(void) const
{
    if ( graphWorldIsNull_() ||
            (!graphWorldIsReference() && referenceHasChanged()) )
        updateRaster_();

    setReferenceHasChanged_(false);

    return RasterImage_gray::getShiftedGray2D();
}


unsigned char*const* RasterImage_gray_F::getShiftedGray2D(ReadWriteStatus rw)
{
    if ( graphWorldIsNull_() ||
            (!graphWorldIsReference() && referenceHasChanged()) )
        updateRaster_();

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the reference is marked as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    return RasterImage_gray::getShiftedGray2D(R_W_ACCESS);
}


const float* RasterImage_gray_F::getGrayF(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    return fltGray_;
}

float* RasterImage_gray_F::getGrayF(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getRasterF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    return fltGray_;
}

const float* const* RasterImage_gray_F::getGrayF2D(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    return fltGray2D_;
}

float*const* RasterImage_gray_F::getGrayF2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getRasterF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    return fltGray2D_;
}

const float* const* RasterImage_gray_F::getShiftedGrayF2D(void) const
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return fltGray2D_;
    else
        return shiftedFltGray2D_;

}
float* const* RasterImage_gray_F::getShiftedGrayF2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getRasterF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return fltGray2D_;
    else
        return shiftedFltGray2D_;

}


//     Should update the non-reference raster, using the values stored in the
//      reference raster.
void RasterImage_gray_F::updateRaster_(void) const
{
	const ImageRect* validRect = getValidRect();
    const int   iLow = validRect->getTop(),
                iHigh = validRect->getBottom(),
                jLow = validRect->getLeft(),
                jHigh = validRect->getRight();
    float *const* fltGray;
    unsigned char *const* gray;

	//	Before we call functions of the parent class with side effects on the
	//	"reference" flags, we copy their values to know what actually needs
	//	to be updated
	bool isCharToFloatUpdate = graphWorldIsReference();
	
	//	Not the prettiest solution, but gets the job done to preserve logical constness
	RasterImage_gray_F* updateableThis = const_cast<RasterImage_gray_F*>(this);

    //  if the GWorld has not been allocated (typically
    //  the first time we render a RasterImage_gray_F object) then we must allocate
    //  it at the same dimensions (bounding rectangle) as the float raster
    if (graphWorldIsNull_())
    {
        updateableThis->allocateGraphWorld_(getHeight(), getWidth());
        FAIL_CONDITION( graphWorldIsNull_(),
                        kGWorldAllocationError,
                        "GraphWorld allocation failed in RasterImage_gray_F::updateRaster_");
        updateableThis->RasterImage_gray::allocateRaster2D_(getHeight(), getWidth());
        updateableThis->RasterImage_gray::allocateShiftedRaster2D_(getHeight(), getWidth(), getTop(), getLeft());
    }

    //  if the bounding rectangle has its origin at (0, 0) then the shifted
    //  raster was never allocated or it is equal to the unshifted raster
    if ((getTop()==0) && (getLeft()==0))
    {
        gray = updateableThis->RasterImage_gray::getGray2D(R_W_ACCESS);
        fltGray = fltGray2D_;
    }
    //  otherwise I use the shifted raster
    else
    {
        gray = updateableThis->RasterImage_gray::getShiftedGray2D(R_W_ACCESS);
        fltGray = shiftedFltGray2D_;
    }

    //  First find out which raster is the reference to update from
    if (isCharToFloatUpdate)
    {
        for (int i=iLow; i<=iHigh; i++)
            for (int j=jLow; j<=jHigh; j++)
                fltGray[i][j] = gray[i][j];
    }
    else
    {
        ConversionFromFloatMode convMode;
        if (hasIndivFloatConvMode_)
            convMode = floatConv_;
        else
            convMode = defaultFloatConv_;

		if ((convMode==kSignedFloat) || (convMode==kPositiveFloat) || (convMode==kRangeFloat))
			calculateFloatConversionParams_( );
        long nbCols = jHigh - jLow + 1;
        for (int i=iLow; i<=iHigh; i++)
            updateableThis->convertToGray_(fltGray[i]+jLow, gray[i]+jLow, nbCols, valMin_, convScale_, convMode);
            
    }

	if (isCharToFloatUpdate)
		setGraphWorldAsReference_();
	else
		setFloatRasterAsReference_();
		
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Copy and Write
//------------------------------------------------------
#endif

const RasterImage_gray_F& RasterImage_gray_F::operator = (const RasterImage_gray_F& img) {
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "RasterImage_gray_F copy operator not implemented");
	return *this;
}
 

void RasterImage_gray_F::copyInto(RasterImage* imgOut) const
{
    copyInto(imgOut, getValidRect(), NULL);
}


void RasterImage_gray_F::copyInto(RasterImage* imgOut, const ImageRect* theRect) const
{
    copyInto(imgOut, theRect, NULL);
}


void RasterImage_gray_F::copyInto(RasterImage* imgOut, const ImageRect* theRect, 
                                  const ImagePoint* origin) const
{
    //  If this image's float raster is not the reference, what we are
    //  trying to copy is just a "normal" gray level image.
    if (!floatRasterIsReference())
        RasterImage_gray::copyInto(imgOut, theRect, origin);

	//	If the float raster is the reference, then we use this class's copy mechanism
    else
    {
        //  All the rectangle testing and resizing is done in the parent class's function
        RasterImage::copyInto_(imgOut, theRect, origin);

        writeInto_(imgOut, theRect, origin);
    }
}

void RasterImage_gray_F::writeInto_(RasterImage* imgOut, const ImageRect* copyRect, 
                                    const ImagePoint* origin) const
{
    //  If the float raster is not the reference then we really should not be here.
    if (graphWorldIsReference() && referenceHasChanged()) {
        RasterImage_gray::writeInto_(imgOut, copyRect, origin);
	}
    //  Converting from the float raster
    else
    {
		//  The GWorld only works with unshifted rasters so we need to compute
		//  the "true" copy rectangles first
		ImageRect*  sourceRect = new ImageRect(copyRect->getLeft() - getLeft(),
											   copyRect->getTop() - getTop(),
											   copyRect->getWidth(), copyRect->getHeight());
		ImageRect*  destRect =  origin == NULL ?
								new ImageRect(copyRect->getLeft() - imgOut->getLeft(),
											  copyRect->getTop() - imgOut->getTop(),
											  copyRect->getWidth(), copyRect->getHeight())
								:
								new ImageRect(origin->getX() - imgOut->getLeft(),
											  origin->getY() - imgOut->getTop(),
											  copyRect->getWidth(), copyRect->getHeight());

        const float* const* graySource = getShiftedGrayF2D();
        const int   iLowSource = sourceRect->getTop(),
                    iLowDest = destRect->getTop(),
                    iHighSource = sourceRect->getBottom(),
                    jLowSource = sourceRect->getLeft(),
                    jLowDest = destRect->getLeft();
		const int	nbRows = sourceRect->getHeight(),
					nbCols = sourceRect->getWidth();

        //  For the time being, we only handle regular dense rasters
        imgOut->setDrawableInformationType(kDense);

        switch( imgOut->getBaseRasterType() )
        {
            case kBinaryRaster:
                imgOut->setOtherRasterAsReference_();
                imgOut->setReferenceHasChanged_(true);
                FAIL_CONDITION( true,
                                kFunctionNotImplemented,
                                "gray_F to binary copies not handled yet by RasterImage_gray_F::writeInto_");
                break;
                
            case kGrayRaster:
                //  if we are dealing with a plain gray raster image
                if (!imgOut->hasFloatRaster())
                {
                    unsigned char*const* grayDest = ((RasterImage_gray* )imgOut)->getShiftedGray2D(R_W_ACCESS);
                    ConversionFromFloatMode floatConvMode =  hasIndivFloatConvMode_ ? floatConv_ : defaultFloatConv_;

					if ((floatConvMode==kSignedFloat) || (floatConvMode==kPositiveFloat) || 
						(floatConvMode==kRangeFloat))
						calculateFloatConversionParams_( );
                    for (int i=iLowSource, iDest = iLowDest; i<=iHighSource; i++, iDest++)
                        convertToGray_(graySource[i]+jLowSource, grayDest[iDest]+jLowDest, 
                                       nbCols, valMin_, convScale_, floatConvMode);

                    imgOut->setGraphWorldAsReference_();
                    imgOut->setReferenceHasChanged_(true);
                }
                //
                //  if imgOut has a float raster, we had better use it
                else
                {
                    float* const* fGrayDest = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);

					//	This call may have been made from within the RasterImage_gray_F constructor of imgOut,
					//	in which case grayDest has not been allocated yet
					if (fGrayDest == NULL) 
					{
						imgOut->allocateRaster2D_(nbRows, nbCols);
						imgOut->allocateShiftedRaster2D_(nbRows, nbCols, iLowDest, jLowDest);
						fGrayDest = (static_cast<RasterImage_gray_F*>(imgOut))->getShiftedGrayF2D(R_W_ACCESS);
					}

                    int     byteShiftSource = static_cast<int>(sizeof(float)*jLowSource),
                            byteShiftDest = static_cast<int>(sizeof(float)*jLowDest);
                    long    bytesPerRow = static_cast<long>(sizeof(float)*nbCols);
                    for (int i=iLowSource, iDest = iLowDest; i<=iHighSource; i++, iDest++)
                        memcpy(fGrayDest[iDest] + byteShiftDest, 
                               graySource[i] + byteShiftSource, static_cast<unsigned long>(bytesPerRow));

                    imgOut->setGraphWorldAsReference_();
                    imgOut->setReferenceHasChanged_(true);

                    //  assign to imgOut the same float->gray conversion as this image
                    ((RasterImage_gray_F *)imgOut)->setFloatConversionMode(
                        this->getFloatConversionMode());
                    imgOut->setFloatRasterAsReference_();
                    imgOut->setReferenceHasChanged_(true);
                }

                break;

            case kRGB24Raster:
                //  if we are dealing with a plain RGB raster image, just copy the bytes
                FAIL_CONDITION( true,
                                kFunctionNotImplemented,
                                "Gray_F to RGB copies not handled yet");
                imgOut->setOtherRasterAsReference_();
                imgOut->setReferenceHasChanged_(true);
                break;

			case kaRGB16Raster:
					//  if we are dealing with a plain aRGB16 raster image, just copy the bytes
					FAIL_CONDITION( true,
									kFunctionNotImplemented,
									"Gray_F to aRGB16 copies not handled yet");
					imgOut->setGraphWorldAsReference_();
					imgOut->setReferenceHasChanged_(true);
				break;

			case kRGBa32Raster:
				//  if we are dealing with a plain gray raster image, just copy the bytes
				if (!imgOut->hasFloatRaster())
				{
					/*
					unsigned char** rgbaDest = ((RasterImage_RGBa* )imgOut)->getRGBa2D();
					for (int i2=0; i2<nbRows; i2++)
						convertToRGBa_(graySource[i2], rgbaDest[i2], nbCols);
					*/
					FAIL_CONDITION( true,
									kFunctionNotImplemented,
									"Gray_F to RGBa copies not handled yet");
					imgOut->setGraphWorldAsReference_();
					imgOut->setReferenceHasChanged_(true);
				}
				//
				//  if imgOut has a float raster, we had better use it
				else
				{
					float*const* fRedOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
					float*const* fGreenOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
					float*const* fBlueOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);

					//	This call may have been made from within the RasterImage_gray_F constructor of imgOut,
					//	in which case grayDest has not been allocated yet
					if ((fRedOut == NULL) || (fGreenOut == NULL)  || (fBlueOut == NULL))
					{
						imgOut->allocateRaster2D_(nbRows, nbCols);
						imgOut->allocateShiftedRaster2D_(nbRows, nbCols, iLowDest, jLowDest);
						fRedOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
						fGreenOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
						fBlueOut = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);
					}

                    int     byteShiftSource = static_cast<int>(sizeof(float)*jLowSource),
                            byteShiftDest = static_cast<int>(sizeof(float)*jLowDest);
                    long    bytesPerRow = static_cast<long>(sizeof(float)*nbCols);
                    for (int i=iLowSource, iDest = iLowDest; i<=iHighSource; i++, iDest++)
					{
                        memcpy(fRedOut[iDest] + byteShiftDest, 
                               graySource[i] + byteShiftSource, static_cast<unsigned long>(bytesPerRow));
                        memcpy(fGreenOut[iDest] + byteShiftDest, 
                               graySource[i] + byteShiftSource, static_cast<unsigned long>(bytesPerRow));
                        memcpy(fBlueOut[iDest] + byteShiftDest, 
                               graySource[i] + byteShiftSource, static_cast<unsigned long>(bytesPerRow));

					}

					imgOut->setFloatRasterAsReference_();
					imgOut->setReferenceHasChanged_(true);
						//  assign to imgOut the same float conversion as this image
						((RasterImage_RGBa_F *)imgOut)->setFloatConversionMode(this->getFloatConversionMode());
				}
				break;

            default:
                FAIL_CONDITION( true,
                                kRasterImageError,
                                "Invalid image type in RasterImage_gray::copyInto");
                break;

        }

        if (sourceRect != NULL)
            delete sourceRect;
        if (destRect != NULL)
            delete destRect;

    }

}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Float conversion
//------------------------------------------------------
#endif

void RasterImage_gray_F::setDefaultFloatConversionMode(ConversionFromFloatMode theMode)
{
    defaultFloatConv_ = theMode;
}

void RasterImage_gray_F::setFloatConversionMode(ConversionFromFloatMode theMode)
{
    floatConv_ = theMode;
    hasIndivFloatConvMode_ = true;
}

ConversionFromFloatMode RasterImage_gray_F::getFloatConversionMode(void) const
{
    if (hasIndivFloatConvMode_)
        return floatConv_;
    else
        return defaultFloatConv_;
}


void RasterImage_gray_F::calculateFloatConversionParams_(void) const
{
    const float* const* theGray;
	const ImageRect* validRect = getValidRect();
    int     iLow = validRect->getTop(),
            iHigh = validRect->getBottom(),
            jLow = validRect->getLeft(),
            jHigh = validRect->getRight();

    if ((getTop()==0) && (getLeft()==0))
        theGray = fltGray2D_;
    else
        theGray = shiftedFltGray2D_;


    ConversionFromFloatMode convType;
    if (hasIndivFloatConvMode_)
        convType = floatConv_;
    else
        convType = defaultFloatConv_;

    valMin_ = valMax_ = fltGray2D_[iLow][jLow];
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
            if (theGray[i][j] > valMax_)
                valMax_ = theGray[i][j];
            else if (theGray[i][j] < valMin_)
                valMin_ = theGray[i][j];

    switch (convType)
    {
            // We don't care about the MIN, simply rescale the MAX to 255
        case kPositiveFloat:
            valMin_ = 0.f;
            convScale_ = 255.0f/valMax_;
            break;

            //  We rescale to 0 255, with 0 at 128
        case kSignedFloat:
            if (valMax_ > -valMin_)
                valMin_ = -valMax_;
            else
                valMax_ = -valMin_;

            convScale_ = 127.5f/valMax_;
            break;

        case kRangeFloat:
            convScale_ = 255.f/(valMax_ - valMin_);
            break;

        default:
            break;
    }
}


void RasterImage_gray_F::convertToGray_(const float* theSource,
                                        unsigned char* theDestination,
                                        long nbPixels,
                                        float valMin, float convScale,
										ConversionFromFloatMode myFloatConv)
{
    const float     *grayIn = theSource;
    unsigned char   *grayOut = theDestination;

    switch (myFloatConv)
    {
        case kSignedFloat:
        case kPositiveFloat:
        case kRangeFloat:
            for (long l=0; l<nbPixels; l++)
                *(grayOut++) = static_cast<unsigned char>(convScale*(*(grayIn++)-valMin));
            break;


        case kSaturatedPositive:
            for (long l=0; l<nbPixels; l++)
            {
                float val = *(grayIn++);
                if (val >= 0.f)
                    if (val <= 255.f)
                        *(grayOut++) = static_cast<unsigned char>(val);
                    else
                        *(grayOut++) = static_cast<unsigned char>(255);
                else
                    *(grayOut++) = static_cast<unsigned char>(0);
            }
            break;


        case kSaturatedSigned:
            for (long l=0; l<nbPixels; l++)
            {
                float val = *(grayIn++);

                if (val >= -128.f)
                    if (val <= 127.f)
                        *(grayOut++) = static_cast<unsigned char>(val + 128);
                    else
                        *(grayOut++) = static_cast<unsigned char>(255);
                else
                    *(grayOut++) = static_cast<unsigned char>(0);
            }
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Unknown float to gray conversion mode");
            break;
    }
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocations
//------------------------------------------------------
#endif



void RasterImage_gray_F::allocateRaster2D_(int nbRows, int nbCols)
{
    fltGray_ = new float[nbRows*nbCols];
    FAIL_CONDITION(fltGray_ == NULL, kRasterAllocationFailure,
                   "Could not allocate fltGray_ array");

    fltGray2D_ = new float*[nbRows];
    FAIL_CONDITION(fltGray2D_ == NULL, kRasterAllocationFailure,
                   "Could not allocate fltGray2D_ array");

    fltGray2D_[0] = fltGray_;
    for (int i=1; i<nbRows; i++)
        fltGray2D_[i] = fltGray2D_[i-1] + nbCols;

    setHasFloatRaster_(true);
}

void RasterImage_gray_F::deleteRaster2D_(void)
{
    if (fltGray2D_ != NULL)
    {
        delete []fltGray2D_;
        fltGray2D_ = NULL;
    }
    if (fltGray_ != NULL)
    {
        delete []fltGray_;
        fltGray_ = NULL;
    }

    setHasFloatRaster_(false);
}

void RasterImage_gray_F::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
    {
        const int   iHigh = iLow + nbRows - 1;

        shiftedFltGray2D_ = new float*[nbRows];
        FAIL_CONDITION( shiftedFltGray2D_ == NULL,
                        kRasterAllocationFailure,
                        "2D float gray-level  raster allocation failed");

        shiftedFltGray2D_ -= iLow;
        //
        shiftedFltGray2D_[iLow] = getGrayF(R_W_ACCESS) - jLow;
        for (int i = iLow; i < iHigh; i++)
            shiftedFltGray2D_[i+1] = shiftedFltGray2D_[i] + nbCols;

    }
    else
        shiftedFltGray2D_ = NULL;

}

void RasterImage_gray_F::deleteShiftedRaster2D_(int iLow)
{
    //  delete the shifted float raster
    if (shiftedFltGray2D_ != NULL)
    {
        shiftedFltGray2D_ += iLow;
        delete []shiftedFltGray2D_;
        shiftedFltGray2D_ = NULL;
    }
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Set Rectangle
//------------------------------------------------------
#endif


void RasterImage_gray_F::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( propertiesAreLocked_,
                    kAccessToLockedData,
                    "Attempt to resize a RasterImage with locked properties");

    //  if width and height have not changed, then the operation is just a translation.
    //  All we need to do is update the "shifted" array (no re-allocation needed)
    //  since the parent class will take care of the rectangle itself
    if ((theWidth == getWidth()) && (theHeight == getHeight()))
    {
        if ((x != getLeft()) || (y != getTop()))
        {
            //  Since the rasters have the same dimensions, there is no need
            //  to delete and reallocate the shifted rasters.  Just apply a
            //  relative shift to them
            const int   iLow  = getTop(),
                        iHigh = getBottom(),
                        jLow = getLeft(),
                        rowShift = y - iLow,
                        colShift = x - jLow;

            for (int i=iLow; i<=iHigh; i++)
                shiftedFltGray2D_[i] += colShift;
            shiftedFltGray2D_ += rowShift;

            //  if the image has an integer raster, then we let the parent class do
            //  the rectangle resizing
			if (!graphWorldIsNull_())
                RasterImage_gray::setBoundRect(x, y, theWidth, theHeight);

        }
        //  if nothing has changed, there is nothing to do
    }
    //  if the dimensions of the rectangles are different, then we must delete
    //  and reallocate the 0-based 2D raster.

    else
    {
        deleteShiftedRaster2D_(getTop());
        deleteRaster2D_();

        //  if the image has an integer raster, then we let the parent class do
        //  the rectangle resizing
        if (!graphWorldIsNull_())
            RasterImage_gray::setBoundRect_(x, y, theWidth, theHeight);

        allocateRaster2D_(theHeight, theWidth);
        allocateShiftedRaster2D_(theHeight, theWidth, y, x);
    }
}

