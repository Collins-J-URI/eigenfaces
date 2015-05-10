/*  NAME:
        RasterImage_RGBa_F.cpp

    DESCRIPTION:
        implementation of the uriVisionLib RasterImage_RGBa_F class

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
using namespace std;

//----------------------------------------------------------
//  static members
//----------------------------------------------------------

ConversionFromFloatMode RasterImage_RGBa_F::defaultFloatConv_ = kSaturatedPositive;


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

RasterImage_RGBa_F::RasterImage_RGBa_F(void)
			:	RasterImage_RGBa(),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
}

RasterImage_RGBa_F::RasterImage_RGBa_F(int nbRows, int nbCols)
		try	:	RasterImage_RGBa(nbRows, nbCols),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
	//
    setFloatRasterAsReference_();
    setReferenceHasChanged_(false);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}


RasterImage_RGBa_F::RasterImage_RGBa_F(int nbRows, int nbCols, const unsigned char* theRGBa)
		try	:	RasterImage_RGBa(nbRows, nbCols, theRGBa),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kPositiveFloat),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}


RasterImage_RGBa_F::RasterImage_RGBa_F(int nbRows, int nbCols, const float* theRed,
                                       const float* theGreen, const float* theBlue)
		try	:	RasterImage_RGBa(nbRows, nbCols),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    allocateRaster2D_(nbRows, nbCols);
    allocateShiftedRaster2D_(nbRows, nbCols, 0, 0);

    long lSource = 0;
    for (int i=0; i<nbRows; i++) {
        for (int j=0; j<nbCols; j++)
        {
            red2D_[i][j] = theRed[lSource];
            green2D_[i][j] = theGreen[lSource];
            blue2D_[i][j] = theBlue[lSource];
        }
	}

	setValidRect(0, 0, nbCols, nbRows);
    setFloatRasterAsReference_();
    setReferenceHasChanged_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}


RasterImage_RGBa_F::RasterImage_RGBa_F(const ImageRect* theRect)
		try	:	RasterImage_RGBa(theRect),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(),
							 theRect->getTop(), theRect->getLeft());

    setGraphWorldAsReference_();
    setReferenceHasChanged_(false);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}



RasterImage_RGBa_F::RasterImage_RGBa_F(const ImageRect* theRect, const unsigned char* theRGBa)
		try	:	RasterImage_RGBa(theRect, theRGBa),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
//    const int   iLow = theRect->getTop(),
//        		iHigh = theRect->getBottom(),
//        		jLow = theRect->getLeft(),
//        		jHigh = theRect->getRight();

//	allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(),
							 theRect->getTop(), theRect->getLeft());

//	try {
//		long lSource = 0;
//		for (int i=iLow; i<=iHigh; i++)
//			for (int j=jLow; j<=jHigh; j++) 
//			{
//				red2D_[i][j] = theRed[lSource];
//				green2D_[i][j] = theGreen[lSource];
//				blue2D_[i][j] = theBlue[lSource];
//			}
//	}
//	catch(...) {
//		FAIL_CONDITION( true,
//						kMemoryCopyError,
//						"Data copy error in RasterImage_RGBa_F constructor");
//	}

//	setValidRect(theRect);
    setHasFloatRaster_(true);
//    setFloatRasterAsReference_();
//    setReferenceHasChanged_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}



RasterImage_RGBa_F::RasterImage_RGBa_F(const ImageRect* theRect, const float* theRed,
                                       const float* theGreen, const float* theBlue)
		try	:	RasterImage_RGBa(theRect),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    FAIL_CONDITION( theRed == NULL,
                    kNullRasterError,
                    "null raster as parameter in RasterImage_RGBa_F constructor");
    FAIL_CONDITION( theGreen == NULL,
                    kNullRasterError,
                    "null raster as parameter in RasterImage_RGBa_F constructor");
    FAIL_CONDITION( theBlue == NULL,
                    kNullRasterError,
                    "null raster as parameter in RasterImage_RGBa_F constructor");

//	allocateGraphWorld_(theRect->getHeight(), theRect->getWidth());
    allocateRaster2D_(theRect->getHeight(), theRect->getWidth());
    allocateShiftedRaster2D_(theRect->getHeight(), theRect->getWidth(),
							 theRect->getTop(), theRect->getLeft());


	const ImageRect* validRect = getValidRect();
    const int   iLow = validRect->getTop(),
                iHigh = validRect->getBottom(),
                jLow = validRect->getLeft(),
                jHigh = validRect->getRight();

	try {
		long lSource = 0;
		for (int i=iLow; i<=iHigh; i++)
			for (int j=jLow; j<=jHigh; j++) 
			{
				red2D_[i][j] = theRed[lSource];
				green2D_[i][j] = theGreen[lSource];
				blue2D_[i][j] = theBlue[lSource];
			}
	}
	catch(...) {
		FAIL_CONDITION( true,
						kMemoryCopyError,
						"Data copy error in RasterImage_RGBa_F constructor");
	}

	setValidRect(theRect);
    setHasFloatRaster_(true);
    setFloatRasterAsReference_();
    setReferenceHasChanged_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}



RasterImage_RGBa_F::RasterImage_RGBa_F(const GraphWorld* gworld)
		try	:	RasterImage_RGBa(),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    FAIL_CONDITION( true, kRasterImageError,
                    "RasterImage_RGBa_F GraphWorld clone constructor is not operative yet");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}


RasterImage_RGBa_F::RasterImage_RGBa_F(GraphWorld* gworld, bool keepThisGraphWorld)
		try	:	RasterImage_RGBa(),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    FAIL_CONDITION( true, kRasterImageError,
                    "RasterImage_RGBa_F GraphWorld clone constructor is not operative yet");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}


RasterImage_RGBa_F::RasterImage_RGBa_F(const RasterImage* img)
		try	:	RasterImage_RGBa(img),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kUndefFloatConv),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_RGBa_F constructor");

    //  Although technically the float raster does not exist yet,
    //  we do this to force the writing to take place in the float
    //  raster rather than in the GraphWorld.
    setHasFloatRaster_(true);

	try {
		img->copyInto(this, img->getValidRect());
	}
	catch(ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGBa_F constructor");
		throw e;
	}

    setHasFloatRaster_(true);
    setFloatRasterAsReference_();
    setReferenceHasChanged_(true);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}



RasterImage_RGBa_F::RasterImage_RGBa_F(const RasterImage* img, const ImageRect* theRect)
		try	:	RasterImage_RGBa(img, theRect),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kPositiveFloat),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    FAIL_CONDITION( img == NULL,
                    kNullRasterImageError,
                    "Attempt to clone a null RasterImage in RasterImage_RGBa_F constructor");

    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in RasterImage_RGBa_F constructor");

    //  Although technically the float raster does not exist yet,
    //  we do this to force the writing to take place in the float
    //  raster rather than in the GraphWorld.
    setHasFloatRaster_(true);

	try {
		img->copyInto(this, theRect);
	}
	catch(ErrorReport& e) {
		e.appendToMessage("called by RasterImage_RGBa_F constructor");
		throw e;
	}
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}



RasterImage_RGBa_F::RasterImage_RGBa_F(const RasterImage_RGBa_F& img)
		try	:	RasterImage_RGBa(),
				//
				red_(NULL),
				green_(NULL),
				blue_(NULL),
				red2D_(NULL),
				green2D_(NULL),
				blue2D_(NULL),
				shiftedRed2D_(NULL),
				shiftedGreen2D_(NULL),
				shiftedBlue2D_(NULL),
				//
				hasIndivFloatConvMode_(false),
				floatConv_(kPositiveFloat),
				//
				convScale_(1.f),		
				convScaleRed_(1.f),		
				convScaleGreen_(1.f),			
				convScaleBlue_(1.f),		
				//
				valMin_(+HUGE_VALF),
				valMax_(-HUGE_VALF),
				valMinRed_(+HUGE_VALF),
				valMaxRed_(-HUGE_VALF),
				valMinGreen_(+HUGE_VALF),
				valMaxGreen_(-HUGE_VALF),
				valMinBlue_(+HUGE_VALF),
				valMaxBlue_(-HUGE_VALF)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "copy consyructor not implemented in RasterImage_RGBa_F");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RasterImage_RGBa_F constructor");
	throw e;
}



RasterImage_RGBa_F::~RasterImage_RGBa_F(void)
{
    deleteShiftedRaster2D_(getTop());
    deleteRaster2D_();
}

#if 0
#pragma mark -
#endif
//------------------------------------------------------
//  Copy and Write
//------------------------------------------------------

const RasterImage_RGBa_F& RasterImage_RGBa_F::operator = (const RasterImage_RGBa_F& img)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "copy operator not implemented in RasterImage_RGBa_F");
	return *this;
}


void RasterImage_RGBa_F::copyInto(RasterImage* imgOut) const
{
    copyInto(imgOut, getValidRect(), NULL);
}


void RasterImage_RGBa_F::copyInto(RasterImage* imgOut, const ImageRect* theRect) const
{
    copyInto(imgOut, theRect, NULL);
}


void RasterImage_RGBa_F::copyInto(RasterImage* imgOut, const ImageRect* theRect, 
                                  const ImagePoint* origin) const
{
    //  If this image's float raster is not the reference, what we are
    //  trying to copy is just a "normal" RGBa level image.
    if (!floatRasterIsReference())
        RasterImage_RGBa::copyInto(imgOut, theRect, origin);

	//	If the float raster is the reference, then we use this class's copy mechanism
    else
    {
        //  All the rectangle testing and resizing is done in the parent class's function
        RasterImage::copyInto_(imgOut, theRect, origin);

        writeInto_(imgOut, theRect, origin);
    }
}

void RasterImage_RGBa_F::writeInto_(RasterImage* imgOut, const ImageRect* copyRect, 
                                    const ImagePoint* origin) const
{
    //  If the float raster is not the reference then we really should not be here.
    if (graphWorldIsReference() && referenceHasChanged())
	{
        RasterImage_RGBa::writeInto_(imgOut, copyRect, origin);
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

		const float* const* redSource = getShiftedRedF2D();
		const float* const* greenSource = getShiftedGreenF2D();
		const float* const* blueSource = getShiftedBlueF2D();
		const int   iLowSource = sourceRect->getTop(),
					iLowDest = destRect->getTop(),
					iHighSource = sourceRect->getBottom(),
					jLowSource = sourceRect->getLeft(),
					jLowDest = destRect->getLeft();
		const int	nbRows = sourceRect->getHeight(),
					nbCols = sourceRect->getWidth();

		switch( imgOut->getBaseRasterType() )
		{
			case kBinaryRaster:
				imgOut->setOtherRasterAsReference_();
				imgOut->setReferenceHasChanged_(true);
				FAIL_CONDITION( true,
								kFunctionNotImplemented,
								"rgba_F to binary copies not handled yet by RasterImage_RGBa_F::writeInto_");
				break;
				
			case kGrayRaster:
				//  if we are dealing with a plain gray raster image
				if (!imgOut->hasFloatRaster())
				{
					unsigned char* const* grayDest = ((RasterImage_gray* )imgOut)->getShiftedGray2D(R_W_ACCESS);
					ConversionFromFloatMode floatConvMode;
					if (hasIndivFloatConvMode_)
						floatConvMode = floatConv_;
					else
						floatConvMode = defaultFloatConv_;

					if ((floatConvMode==kSignedFloat) || (floatConvMode==kPositiveFloat) || 
						(floatConvMode==kRangeFloat))
						calculateFloatConversionParams_( );
					for (int i=iLowSource, iDest = iLowDest; i<=iHighSource; i++, iDest++)
						convertToGray_(redSource[i]+jLowSource, 
									   greenSource[i]+jLowSource, 
									   blueSource[i]+jLowSource, 
									   grayDest[iDest]+jLowDest, 
									   nbCols, valMin_, convScale_, defaultGrayConv_, floatConvMode);

					imgOut->setGraphWorldAsReference_();
					imgOut->setReferenceHasChanged_(true);
				}

				//
				//  if imgOut has a float raster, we had better use it
				else
				{
					float* const* fGrayDest = ((RasterImage_gray_F *)imgOut)->getShiftedGrayF2D(R_W_ACCESS);
					
					//	This call may have been made from within the RasterImage_gray_F constructor of imgOut,
					//	in which case grayDest has not been allocated yet
					if (fGrayDest == NULL) 
					{
						imgOut->allocateRaster2D_(nbRows, nbCols);
						imgOut->allocateShiftedRaster2D_(nbRows, nbCols, iLowDest, jLowDest);
						fGrayDest = ((RasterImage_gray_F *)imgOut)->getShiftedGrayF2D(R_W_ACCESS);
					}

					for (int i=iLowSource, iDest = iLowDest; i<=iHighSource; i++, iDest++)
						convertToGray_(redSource[i]+jLowSource, 
									   greenSource[i]+jLowSource, 
									   blueSource[i]+jLowSource, 
									   fGrayDest[iDest]+jLowDest,
									   nbCols, valMin_, convScale_, 
									   defaultGrayConv_);

					imgOut->setFloatRasterAsReference_();
					imgOut->setReferenceHasChanged_(true);
				}
				break;

			case kRGB24Raster:
				//  if we are dealing with a plain RGB raster image, just copy the bytes
				FAIL_CONDITION( true,
								kRasterImageError,
								"RGBA_F to RGB copies not handled yet");
				imgOut->setGraphWorldAsReference_();
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
				//  if we are dealing with a RGBa raster image, convert float to uc
				if (!imgOut->hasFloatRaster())
				{
					unsigned char*const* rgbaDest = static_cast<RasterImage_RGBa* >(imgOut)->getShiftedRGBa2D(R_W_ACCESS);
					ConversionFromFloatMode floatConvMode =  hasIndivFloatConvMode_ ? floatConv_ : defaultFloatConv_;

					if ((floatConvMode==kSignedFloat) || (floatConvMode==kPositiveFloat) || 
						(floatConvMode==kRangeFloat))
						calculateFloatConversionParams_( );
					for (int i=iLowSource, iDest = iLowDest; i<=iHighSource; i++, iDest++)
						convertToRGBa_(redSource[i]+jLowSource, greenSource[i]+jLowSource, 
									   blueSource[i]+jLowSource, rgbaDest[iDest]+jLowDest, 
									   nbCols, valMin_, convScale_, floatConvMode);

					imgOut->setGraphWorldAsReference_();
					imgOut->setReferenceHasChanged_(true);
				}
				//
				//  if imgOut has a float raster, we had better use it
				else
				{
					float*const* fRedDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
					float*const* fGreenDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
					float*const* fBlueDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);

					//	This call may have been made from within the RasterImage_RGBa_F constructor of imgOut,
					//	in which case grayDest has not been allocated yet
					if ((fRedDest == NULL) || (fGreenDest == NULL)  || (fBlueDest == NULL))
					{
						imgOut->allocateRaster2D_(nbRows, nbCols);
						imgOut->allocateShiftedRaster2D_(nbRows, nbCols, iLowDest, jLowDest);
						fRedDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedRedF2D(R_W_ACCESS);
						fGreenDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedGreenF2D(R_W_ACCESS);
						fBlueDest = (static_cast<RasterImage_RGBa_F*>(imgOut))->getShiftedBlueF2D(R_W_ACCESS);
					}

					int     byteShiftSource = static_cast<int>(sizeof(float)*jLowSource),
							byteShiftDest = static_cast<int>(sizeof(float)*jLowDest);
					long    bytesPerRow = static_cast<long>(sizeof(float)*nbCols);
					for (int i=iLowSource, iDest = iLowDest; i<=iHighSource; i++, iDest++)
					{
						memcpy(fRedDest[iDest] + byteShiftDest, 
							   redSource[i] + byteShiftSource, static_cast<unsigned long>(bytesPerRow));
						memcpy(fGreenDest[iDest] + byteShiftDest, 
							   greenSource[i] + byteShiftSource, static_cast<unsigned long>(bytesPerRow));
						memcpy(fBlueDest[iDest] + byteShiftDest, 
							   blueSource[i] + byteShiftSource, static_cast<unsigned long>(bytesPerRow));

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
	}
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark  Get Raster methods
//------------------------------------------------------
#endif


const unsigned char* RasterImage_RGBa_F::getRGBa(void) const
{
    if ( (gworld_ == NULL) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    return gworld_->getRaster();
}


unsigned char* RasterImage_RGBa_F::getRGBa(ReadWriteStatus rw)
{
    if ( (gworld_ == NULL) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return gworld_->getRaster();
}


const unsigned char* const* RasterImage_RGBa_F::getRGBa2D(void) const
{
    if ( (gworld_ == NULL) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    return RasterImage_RGBa::getRGBa2D();
}

unsigned char* const* RasterImage_RGBa_F::getRGBa2D(ReadWriteStatus rw)
{
    if ( (gworld_ == NULL) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return RasterImage_RGBa::getRGBa2D(R_W_ACCESS);
}

const unsigned char* const* RasterImage_RGBa_F::getShiftedRGBa2D(void) const
{
    if ( (gworld_ == NULL) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    return RasterImage_RGBa::getShiftedRGBa2D();
}

unsigned char* const* RasterImage_RGBa_F::getShiftedRGBa2D(ReadWriteStatus rw)
{
    if ( (gworld_ == NULL) ||
            (!graphWorldIsReference() && referenceHasChanged_) )
    {
        //  force an update
        referenceHasChanged_ = true;
        updateRaster_();
    }

    //  A side-effect of calling the read-write version of a getRaster function is
    //  that the base (GraphWorld) raster is set to be the reference and is marked
    //  as 'changed'
    gworldIsReference_= true;
    referenceHasChanged_ = true;

    return RasterImage_RGBa::getShiftedRGBa2D(R_W_ACCESS);
}


const float*  RasterImage_RGBa_F::getRedF(void) const
{
    return red_;
}

float*  RasterImage_RGBa_F::getRedF(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getXXXF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    return red_;
}

const float*  RasterImage_RGBa_F::getGreenF(void) const
{
    return green_;
}

float*  RasterImage_RGBa_F::getGreenF(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getXXXF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    return green_;
}

const float*  RasterImage_RGBa_F::getBlueF(void) const
{
    return blue_;
}

float*  RasterImage_RGBa_F::getBlueF(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getXXXF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    return blue_;
}

const float* const* RasterImage_RGBa_F::getRedF2D(void) const
{
    return red2D_;
}

float* const* RasterImage_RGBa_F::getRedF2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getXXXF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    return red2D_;
}

const float* const* RasterImage_RGBa_F::getGreenF2D(void) const
{
    return green2D_;
}

float* const* RasterImage_RGBa_F::getGreenF2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getXXXF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    return green2D_;
}

const float* const* RasterImage_RGBa_F::getBlueF2D(void) const
{
    return blue2D_;
}

float* const* RasterImage_RGBa_F::getBlueF2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getXXXF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    return blue2D_;
}

const float* const* RasterImage_RGBa_F::getShiftedRedF2D(void) const
{
//    return shiftedRed2D_;

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return red2D_;
    else
        return shiftedRed2D_;
}

float* const* RasterImage_RGBa_F::getShiftedRedF2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getXXXF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return red2D_;
    else
        return shiftedRed2D_;
}

const float* const* RasterImage_RGBa_F::getShiftedGreenF2D(void) const
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return green2D_;
    else
        return shiftedGreen2D_;
}

float* const* RasterImage_RGBa_F::getShiftedGreenF2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getXXXF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return green2D_;
    else
        return shiftedGreen2D_;
}

const float* const* RasterImage_RGBa_F::getShiftedBlueF2D(void) const
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return blue2D_;
    else
        return shiftedBlue2D_;
}

float* const* RasterImage_RGBa_F::getShiftedBlueF2D(ReadWriteStatus rw)
{
    if (graphWorldIsReference() && referenceHasChanged())
        updateRaster_();

    //  A side-effect of calling the read-write version of a getXXXF function is
    //  that the float raster is set to be the reference and is marked
    //  as 'changed'
	setFloatRasterAsReference_();
    setReferenceHasChanged_(true);

    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop()==0) && (getLeft()==0))
        return blue2D_;
    else
        return shiftedBlue2D_;
}



void RasterImage_RGBa_F::updateRaster_(void) const
{
	const ImageRect* validRect = getValidRect();
    const int   iLow = validRect->getTop(),
                iHigh = validRect->getBottom(),
                jLow = validRect->getLeft(),
                jHigh = validRect->getRight();
    unsigned char *const* rgba;
    float *const* fRed;
    float *const* fGreen;
    float *const* fBlue;

	//	Before we call functions of the parent class with side effects on the
	//	"reference" flags, we copy their values to know what actually needs
	//	to be updated
	bool isCharToFloatUpdate = graphWorldIsReference();

	//	Not the prettiest solution, but gets the job done to preserve logical constness
	RasterImage_RGBa_F* updateableThis = const_cast<RasterImage_RGBa_F*>(this);

    //  if the GWorld  has not been allocated (typically
    //  the first time we render a RasterImage_RGBa_F object) then we must allocate
    //  it at the same dimensions (bounding rectangle) as the float raster
    if (graphWorldIsNull_())
    {
        updateableThis->allocateGraphWorld_(getHeight(), getWidth());
        FAIL_CONDITION( graphWorldIsNull_(),
                        kGWorldAllocationError,
                        "GraphWorld allocation failed in RasterImage_RGBa_F::updateRaster_");
        updateableThis->RasterImage_RGBa::allocateRaster2D_(getHeight(), getWidth());
        updateableThis->RasterImage_RGBa::allocateShiftedRaster2D_(getHeight(), getWidth(), getTop(), getLeft());
    }

    //  if the bounding rectangle has its origin at (0, 0) then the shifted
    //  raster was never allocated or it is equal to the unshifted raster
    if ((getTop()==0) && (getLeft()==0))
    {
        rgba = updateableThis->RasterImage_RGBa::getRGBa2D(R_W_ACCESS);
        fRed = red2D_;
        fGreen = green2D_;
        fBlue = blue2D_;
    }
    //  otherwise I use the shifted raster
    else
    {
        rgba = updateableThis->RasterImage_RGBa::getRGBa2D(R_W_ACCESS);
        fRed = shiftedRed2D_;
        fGreen = shiftedGreen2D_;
        fBlue = shiftedBlue2D_;
    }

    //  First find out which raster is the reference to update from
    if (isCharToFloatUpdate)
    {
        for (int i=iLow; i<=iHigh; i++)
            for (int j=jLow, k=4*jLow; j<=jHigh; j++)
            {
                fRed[i][j] = rgba[i][k++];
                fGreen[i][j] = rgba[i][k++];
                fBlue[i][j] = rgba[i][k++];
            }
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
            updateableThis->convertToRGBa_(fRed[i]+jLow, fGreen[i]+jLow, fBlue[i]+jLow, 
										   rgba[i]+4*jLow, nbCols, valMin_, convScale_, convMode);
            
    }

	if (isCharToFloatUpdate)
		setGraphWorldAsReference_();
	else
		setFloatRasterAsReference_();
		
}



#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark  Conversion from float preferences & calculations
//------------------------------------------------------
#endif


void RasterImage_RGBa_F::setDefaultFloatConversionMode(ConversionFromFloatMode theMode)
{
    defaultFloatConv_ = theMode;
}

void RasterImage_RGBa_F::setFloatConversionMode(ConversionFromFloatMode theMode)
{
    floatConv_ = theMode;
    hasIndivFloatConvMode_ = true;
}

ConversionFromFloatMode RasterImage_RGBa_F::getFloatConversionMode(void) const
{
    if (hasIndivFloatConvMode_)
        return floatConv_;
    else
        return defaultFloatConv_;
}

void RasterImage_RGBa_F::calculateFloatConversionParams_(void) const
{
	const ImageRect* validRect = getValidRect();
    const int iLow = validRect->getTop(),
              iHigh = validRect->getBottom(),
              jLow = validRect->getLeft(),
              jHigh = validRect->getRight();

    const float* const* redSource = getShiftedRedF2D();
    const float* const* greenSource = getShiftedGreenF2D();
    const float* const* blueSource = getShiftedBlueF2D();
    if ((getTop()==0) && (getLeft()==0))
    {
        redSource = red2D_;
        greenSource = green2D_;
        blueSource = blue2D_;
    }
    else
    {
        redSource = shiftedRed2D_;
        greenSource = shiftedGreen2D_;
        blueSource = shiftedBlue2D_;
    }

    ConversionFromFloatMode convType;
    if (hasIndivFloatConvMode_)
        convType = floatConv_;
    else
        convType = defaultFloatConv_;

    valMin_ = valMax_ = redSource[iLow][jLow];
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            if (redSource[i][j] > valMax_)
                valMax_ = redSource[i][j];
            else if (redSource[i][j] < valMin_)
                valMin_ = redSource[i][j];
            if (greenSource[i][j] > valMax_)
                valMax_ = greenSource[i][j];
            else if (greenSource[i][j] < valMin_)
                valMin_ = greenSource[i][j];
            if (blueSource[i][j] > valMax_)
                valMax_ = blueSource[i][j];
            else if (blueSource[i][j] < valMin_)
                valMin_ = blueSource[i][j];
        }
        
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

#if 0
#pragma mark -
#endif

void RasterImage_RGBa_F::convertToRGBa_(const float* redSource, const float* greenSource, 
                                        const float* blueSource, unsigned char* theDestination,
                                        long nbPixels,
									    float valMin, float convScale,
                                        ConversionFromFloatMode myFloatConv)
{
    const float     *redIn = redSource, 
                    *greenIn = greenSource, 
                    *blueIn = blueSource;
    unsigned char   *rgbaOut = theDestination;

    switch (myFloatConv)
    {
        case kSignedFloat:
        case kPositiveFloat:
        case kRangeFloat:
//            calculateFloatConversionParams_( );

            for (long l=0; l<nbPixels; l++)
            {
                *(rgbaOut++) = static_cast<unsigned char>(convScale*(*(redIn++)-valMin));
                *(rgbaOut++) = static_cast<unsigned char>(convScale*(*(greenIn++)-valMin));
                *(rgbaOut++) = static_cast<unsigned char>(convScale*(*(blueIn++)-valMin));
                *(rgbaOut++) = static_cast<unsigned char>(255);
            }
            break;


        case kSaturatedPositive:
            for (long l=0; l<nbPixels; l++)
            {
                //  Red channel
                float val = *(redIn++);
                if (val >= 0.f)
                    if (val <= 255.f)
                        *(rgbaOut++) = static_cast<unsigned char>(val);
                    else
                        *(rgbaOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbaOut++) = static_cast<unsigned char>(0);

                //  Green channel
                val = *(greenIn++);
                if (val >= 0.f)
                    if (val <= 255.f)
                        *(rgbaOut++) = static_cast<unsigned char>(val);
                    else
                        *(rgbaOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbaOut++) = static_cast<unsigned char>(0);

                //  Blue channel
                val = *(blueIn++);
                if (val >= 0.f)
                    if (val <= 255.f)
                        *(rgbaOut++) = static_cast<unsigned char>(val);
                    else
                        *(rgbaOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbaOut++) = static_cast<unsigned char>(0);

                //  Alpha channel
                *(rgbaOut++) = static_cast<unsigned char>(255);
            }
            break;


        case kSaturatedSigned:
            for (long l=0; l<nbPixels; l++)
            {
                //  Red channel
                float val = *(redIn++);
                if (val >= -128.f)
                    if (val <= 127.f)
                        *(rgbaOut++) = static_cast<unsigned char>(val + 128);
                    else
                        *(rgbaOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbaOut++) = static_cast<unsigned char>(0);

                //  Green channel
                val = *(greenIn++);
                if (val >= -128.f)
                    if (val <= 127.f)
                        *(rgbaOut++) = static_cast<unsigned char>(val + 128);
                    else
                        *(rgbaOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbaOut++) = static_cast<unsigned char>(0);

                //  Blue channel
                val = *(blueIn++);
                if (val >= -128.f)
                    if (val <= 127.f)
                        *(rgbaOut++) = static_cast<unsigned char>(val + 128);
                    else
                        *(rgbaOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbaOut++) = static_cast<unsigned char>(0);

                //  Alpha channel
                *(rgbaOut++) = static_cast<unsigned char>(255);
            }
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Unknown float to gray conversion mode");
            break;
    }
}

void RasterImage_RGBa_F::convertToRGB_(const float* redSource, const float* greenSource, 
                                       const float* blueSource, unsigned char* theDestination,
									   long nbPixels, float valMin, float convScale,
                                       ConversionFromFloatMode myFloatConv)
{
    const float     *redIn = redSource, 
                    *greenIn = greenSource, 
                    *blueIn = blueSource;
    unsigned char   *rgbOut = theDestination;

    switch (myFloatConv)
    {
        case kSignedFloat:
        case kPositiveFloat:
        case kRangeFloat:
//            calculateFloatConversionParams_( );

            for (long l=0; l<nbPixels; l++)
            {
                *(rgbOut++) = static_cast<unsigned char>(convScale*(*(redIn++)-valMin));
                *(rgbOut++) = static_cast<unsigned char>
                                    (convScale*(*(greenIn++)-valMin));
                *(rgbOut++) = static_cast<unsigned char>(convScale*(*(blueIn++)-valMin));
            }
            break;


        case kSaturatedPositive:
            for (long l=0; l<nbPixels; l++)
            {
                //  Red channel
                float val = *(redIn++);
                if (val >= 0.f)
                    if (val <= 255.f)
                        *(rgbOut++) = static_cast<unsigned char>(val);
                    else
                        *(rgbOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbOut++) = static_cast<unsigned char>(0);

                //  Green channel
                val = *(greenIn++);
                if (val >= 0.f)
                    if (val <= 255.f)
                        *(rgbOut++) = static_cast<unsigned char>(val);
                    else
                        *(rgbOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbOut++) = static_cast<unsigned char>(0);

                //  Blue channel
                val = *(blueIn++);
                if (val >= 0.f)
                    if (val <= 255.f)
                        *(rgbOut++) = static_cast<unsigned char>(val);
                    else
                        *(rgbOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbOut++) = static_cast<unsigned char>(0);

            }
            break;


        case kSaturatedSigned:
            for (long l=0; l<nbPixels; l++)
            {
                //  Red channel
                float val = *(redIn++);
                if (val >= -128.f)
                    if (val <= 127.f)
                        *(rgbOut++) = static_cast<unsigned char>(val + 128);
                    else
                        *(rgbOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbOut++) = static_cast<unsigned char>(0);

                //  Green channel
                val = *(greenIn++);
                if (val >= -128.f)
                    if (val <= 127.f)
                        *(rgbOut++) = static_cast<unsigned char>(val + 128);
                    else
                        *(rgbOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbOut++) = static_cast<unsigned char>(0);

                //  Blue channel
                val = *(blueIn++);
                if (val >= -128.f)
                    if (val <= 127.f)
                        *(rgbOut++) = static_cast<unsigned char>(val + 128);
                    else
                        *(rgbOut++) = static_cast<unsigned char>(255);
                else
                    *(rgbOut++) = static_cast<unsigned char>(0);

            }
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Unknown float to gray conversion mode");
            break;
    }
}

void RasterImage_RGBa_F::convertToARGB16_(const float* redSource, const float* greenSource, 
										  const float* blueSource, unsigned char* theDestination,
										  long nbPixels, float valMin, float convScale,
										  ConversionFromFloatMode myFloatConv)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet");

}


#if 0
#pragma mark -
#endif

//----------------------------------------------------------
//  Conversion to gray calculations
//----------------------------------------------------------

void RasterImage_RGBa_F::convertToGray_(const float* sourceRed, const float* sourceGreen,
                                        const float* sourceBlue, unsigned char* theDestination,
										long nbPixels, float valMin, float convScale,
                                        ConversionToGrayMode myGrayConv,
                                        ConversionFromFloatMode myFloatConv)
{
    const float div3 = 1.f/3.f;
    float   sum;
    float   val;
    const float* redIn = sourceRed, 
                *greenIn = sourceGreen, 
                *blueIn = sourceBlue;
    unsigned char* dest = theDestination;
	const float* convWeight = getDefaultConversionWeights();
    
    //  How do we convert from float to unsigned char?
    switch (myFloatConv)
    {
        case kSignedFloat:
        case kPositiveFloat:
        case kRangeFloat:
//            calculateFloatConversionParams_( );
            
            //  How do we convert from RGBa to gray?
            switch (myGrayConv)
            {
                case kMaxFieldConversion:

                    for (long j=0; j<nbPixels; j++)
                    {
                        //  optimization "in the small" left for a more clever programmer
                        float   red = *(redIn++),
                                green = *(greenIn++),
                                blue = *(blueIn++);
                        val = MAX(red, MAX(green, blue));
                        *(dest++) = static_cast<unsigned char>(convScale*(val-valMin));
                    }
                    break;

                case kAverageConversion:

                    for (long j=0; j<nbPixels; j++)
                    {
                        sum = *(redIn++) + *(greenIn++) + *(blueIn++);
                        val = sum * div3;
                        *(dest++) = static_cast<unsigned char>(convScale*(val-valMin));
                    }
                    break;

                case kWeightedAverageConversion:

                    for (long j=0; j<nbPixels; j++)
                    {
                        val = convWeight[0] ** (redIn++) + 
                              convWeight[1] ** (greenIn++) + 
                              convWeight[2] ** (blueIn++);
                        *(dest++) = static_cast<unsigned char>(convScale*(val - valMin));
                    }
                    break;

                default:
                    FAIL_CONDITION( true, kRasterImageError,
                                    "unknown conversion to gray mode encountered");
                    break;

            }
            break;


        case kSaturatedPositive:
            //  How do we convert from RGBa to gray?
            switch (myGrayConv)
            {
                case kMaxFieldConversion:

                    for (long j=0; j<nbPixels; j++)
                    {
                        //  optimization "in the small" left for a more clever programmer
                        float   red = *(redIn++),
                                green = *(greenIn++),
                                blue = *(blueIn++);
                        val = MAX(red, MAX(green, blue));
                        if (val >= 0.f)
                            if (val <= 255.f)
                                *(dest++) = static_cast<unsigned char>(val);
                            else
                                *(dest++) = static_cast<unsigned char>(255);
                        else
                            *(dest++) = static_cast<unsigned char>(0);
                    }
                    break;

                case kAverageConversion:

                    for (long j=0; j<nbPixels; j++)
                    {
                        sum = *(redIn++) + *(greenIn++) + *(blueIn++);
                        val = sum * div3;
                        if (val >= 0.f)
                            if (val <= 255.f)
                                *(dest++) = static_cast<unsigned char>(val);
                            else
                                *(dest++) = static_cast<unsigned char>(255);
                        else
                            *(dest++) = static_cast<unsigned char>(0);
                    }
                    break;

                case kWeightedAverageConversion:

                    for (long j=0; j<nbPixels; j++)
                    {
                        val = convWeight[0] ** (redIn++) + 
                              convWeight[1] ** (greenIn++) + 
                              convWeight[2] ** (blueIn++);
                        if (val >= 0.f)
                            if (val <= 255.f)
                                *(dest++) = static_cast<unsigned char>(val);
                            else
                                *(dest++) = static_cast<unsigned char>(255);
                        else
                            *(dest++) = static_cast<unsigned char>(0);
                    }
                    break;

                default:
                    FAIL_CONDITION( true, kRasterImageError,
                                    "unknown conversion to gray mode encountered");
                    break;

            }
            break;


        case kSaturatedSigned:

            //  How do we convert from RGBa to gray?
            switch (myGrayConv)
            {
                case kMaxFieldConversion:

                    for (long j=0; j<nbPixels; j++)
                    {
                        //  optimization "in the small" left for a more clever programmer
                        float   red = *(redIn++),
                                green = *(greenIn++),
                                blue = *(blueIn++);
                        val = MAX(red, MAX(green, blue));
                        if (val >= -128.f)
                            if (val <= 127.f)
                                *(dest++) = static_cast<unsigned char>(val + 128);
                            else
                                *(dest++) = static_cast<unsigned char>(255);
                        else
                            *(dest++) = static_cast<unsigned char>(0);
                    }
                    break;

                case kAverageConversion:

                    for (long j=0; j<nbPixels; j++)
                    {
                        sum = *(redIn++) + *(greenIn++) + *(blueIn++);
                        val = sum * div3;
                        if (val >= -128.f)
                            if (val <= 127.f)
                                *(dest++) = static_cast<unsigned char>(val + 128);
                            else
                                *(dest++) = static_cast<unsigned char>(255);
                        else
                            *(dest++) = static_cast<unsigned char>(0);
                    }
                    break;

                case kWeightedAverageConversion:

                    for (long j=0; j<nbPixels; j++)
                    {
                        val = convWeight[0] ** (redIn++) + 
                              convWeight[1] ** (greenIn++) + 
                              convWeight[2] ** (blueIn++);
                         if (val >= -128.f)
                            if (val <= 127.f)
                                *(dest++) = static_cast<unsigned char>(val + 128);
                            else
                                *(dest++) = static_cast<unsigned char>(255);
                        else
                            *(dest++) = static_cast<unsigned char>(0);
                    }
                    break;

                default:
                    FAIL_CONDITION( true, kRasterImageError,
                                    "unknown conversion to gray mode encountered");
                    break;

            }
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Unknown float to gray conversion mode");
            break;
    }
}


void RasterImage_RGBa_F::convertToGray_(const float* sourceRed, const float* sourceGreen,
                                        const float* sourceBlue, float* theDestination, 
										long nbPixels, float valMin, float convScale,
										ConversionToGrayMode myGrayConv,
                                        ConversionFromFloatMode myFloatConv)
{
    const float div3 = 1.f/3.f;
    const float* redIn = sourceRed, 
                *greenIn = sourceGreen, 
                *blueIn = sourceBlue;
    float   *dest = theDestination;
	const float* convWeight = getDefaultConversionWeights();
    float   sum;

    //  how do we convert from rgb to gray?
    switch (myGrayConv)
    {
        case kMaxFieldConversion:

            for (long j=0; j<nbPixels; j++)
            {
                //  optimization "in the small" left for a more clever programmer
                float   red = *(redIn++),
                        green = *(greenIn++),
                        blue = *(blueIn++);
                *(dest++) = MAX(red, MAX(green, blue));
            }
            break;

        case kAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                sum = *(redIn++) + *(greenIn++) + *(blueIn++);
                *(dest++) = sum * div3;
            }
            break;

        case kWeightedAverageConversion:

            for (long j=0; j<nbPixels; j++)
            {
                *(dest++) = convWeight[0] ** (redIn++) + 
                            convWeight[1] ** (greenIn++) + 
                            convWeight[2] ** (blueIn++);
            }
            break;

        default:
            FAIL_CONDITION( true, kRasterImageError,
                            "unknown conversion to gray mode encountered");
            break;

    }
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Allocations
//------------------------------------------------------
#endif



#if 0
#pragma mark -
#endif


void RasterImage_RGBa_F::allocateRaster2D_(int nbRows, int nbCols)
{
    red_ = new float[nbRows*nbCols];
    FAIL_CONDITION(red_ == NULL, kRasterAllocationFailure,
                   "Could not allocate red_ array");
    green_ = new float[nbRows*nbCols];
    FAIL_CONDITION(green_ == NULL, kRasterAllocationFailure,
                   "Could not allocate green_ array");
    blue_ = new float[nbRows*nbCols];
    FAIL_CONDITION(blue_ == NULL, kRasterAllocationFailure,
                   "Could not allocate blue_ array");

    red2D_ = new float*[nbRows];
    FAIL_CONDITION(red2D_ == NULL, kRasterAllocationFailure,
                   "Could not allocate red2D_ array");
    green2D_ = new float*[nbRows];
    FAIL_CONDITION(green2D_ == NULL, kRasterAllocationFailure,
                   "Could not allocate green2D_ array");
    blue2D_ = new float*[nbRows];
    FAIL_CONDITION(blue2D_ == NULL, kRasterAllocationFailure,
                   "Could not allocate blue2D_ array");

    red2D_[0] = red_;
    green2D_[0] = green_;
    blue2D_[0] = blue_;
    for (int i=1; i<nbRows; i++){
        red2D_[i] = red2D_[i-1] + nbCols;
        green2D_[i] = green2D_[i-1] + nbCols;
        blue2D_[i] = blue2D_[i-1] + nbCols;
    }

    setHasFloatRaster_(true);
}


void RasterImage_RGBa_F::deleteRaster2D_(void)
{
    if(red_ != NULL)
        delete []red_;
    if(green_ != NULL)
        delete []green_;
    if(blue_ != NULL)
        delete []blue_;

    if(red2D_ != NULL)
        delete []red2D_;
    if(green2D_ != NULL)
        delete []green2D_;
    if(blue2D_ != NULL)
        delete []blue2D_;

    setHasFloatRaster_(false);
}


void RasterImage_RGBa_F::allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow)
{
    //  if the image has its upper left corner at the origin, then
    //  the shifted raster is just a copy of the unshifted one.
    //  no point wasting space.
    if ((getTop() != 0) || (getLeft() != 0))
    {
        const int   iHigh = iLow + nbRows - 1;

        deleteShiftedRaster2D_(iLow);
        shiftedRed2D_ = new float*[nbRows];
        FAIL_CONDITION( shiftedRed2D_ == NULL,
                        kRasterAllocationFailure,
                        "shiftedRed2D_ allocation failed");
        shiftedGreen2D_ = new float*[nbRows];
        FAIL_CONDITION( shiftedGreen2D_ == NULL,
                        kRasterAllocationFailure,
                        "shiftedGreen2D_ allocation failed");
        shiftedBlue2D_ = new float*[nbRows];
        FAIL_CONDITION( shiftedBlue2D_ == NULL,
                        kRasterAllocationFailure,
                        "shiftedBlue2D_ allocation failed");
                        
        shiftedRed2D_ -= iLow;
        shiftedGreen2D_ -= iLow;
        shiftedBlue2D_ -= iLow;
        //
        shiftedRed2D_[iLow] = getRedF(R_W_ACCESS) - jLow;
        shiftedGreen2D_[iLow] = getGreenF(R_W_ACCESS) - jLow;
        shiftedBlue2D_[iLow] = getBlueF(R_W_ACCESS) - jLow;
        for (int i = iLow; i < iHigh; i++){
            shiftedRed2D_[i+1] = shiftedRed2D_[i] + nbCols;
            shiftedGreen2D_[i+1] = shiftedGreen2D_[i] + nbCols;
            shiftedBlue2D_[i+1] = shiftedBlue2D_[i] + nbCols;
        }
    }
    else{
        shiftedRed2D_ = NULL;
        shiftedGreen2D_ = NULL;
        shiftedBlue2D_ = NULL;
    }
}


void RasterImage_RGBa_F::deleteShiftedRaster2D_(int iLow)
{
    //  delete the shifted float raster s
    if (shiftedRed2D_ != NULL)
    {
        shiftedRed2D_ += iLow;
        delete []shiftedRed2D_;
        shiftedRed2D_ = NULL;
    }
    if (shiftedGreen2D_ != NULL)
    {
        shiftedGreen2D_ += iLow;
        delete []shiftedGreen2D_;
        shiftedGreen2D_ = NULL;
    }
    if (shiftedBlue2D_ != NULL)
    {
        shiftedBlue2D_ += iLow;
        delete []shiftedBlue2D_;
        shiftedBlue2D_ = NULL;
    }
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Set Rects
//------------------------------------------------------
#endif

void RasterImage_RGBa_F::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( propertiesAreLocked(),
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
            const int	iLow  = getTop(),
						iHigh = getBottom(),
						jLow = getLeft(),
						rowShift = y - iLow,
						colShift = x - jLow;

//jyh	is this really needed?
if (shiftedRed2D_)
{
            for (int i=iLow; i<=iHigh; i++)
            {
                shiftedRed2D_[i] += colShift;
                shiftedGreen2D_[i] += colShift;
                shiftedBlue2D_[i] += colShift;
            }
            shiftedRed2D_ += rowShift;
            shiftedGreen2D_ += rowShift;
            shiftedBlue2D_ += rowShift;
}

            //  if the image has an integer raster, then we let the parent class do
            //  the rectangle resizing
        if (!graphWorldIsNull_())
                RasterImage_RGBa::setBoundRect(x, y, theWidth, theHeight);

        }
        //  if nothing has changed, there is nothing to do
    }
    //  if the dimensions of the rectangles are different, then we must delete
    //  and reallocate the 0-based 2D raster.

    else
    {
        deleteShiftedRaster2D_(y);
        deleteRaster2D_();

        //  if the image has an integer raster, then we let the parent class do
        //  the rectangle resizing
        if (!graphWorldIsNull_())
            RasterImage_RGBa::setBoundRect_(x, y, theWidth, theHeight);

		allocateRaster2D_(theHeight, theWidth);
		allocateShiftedRaster2D_(theHeight, theWidth, y, x);
    }
}

