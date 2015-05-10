/*  NAME:
        Pattern2D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Pattern2D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "Pattern2D.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  static members
//----------------------------------------------------------

Pattern2DUpdateMode Pattern2D::defaultUpdateMode_ = REPLACE_PATTERN;


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif


Pattern2D::Pattern2D(void)
		:   DrawableObject2D(),
			//
			blob_(NULL),
			pattern_(NULL),
			hasIndivUpdateMode_(false),
			updateMode_(REPLACE_PATTERN),
			drawContour_(false),
			patImgIsLocal_(false),
			patBlobIsLocal_(false)
{
}

Pattern2D::Pattern2D(const Blob* blob, const ImageRect* rect, const RasterImage* img, bool safe)
	try	:   DrawableObject2D(*blob),
			//
			blob_(NULL),
			pattern_(NULL),
			hasIndivUpdateMode_(false),
			updateMode_(REPLACE_PATTERN),
			drawContour_(false),
			patImgIsLocal_(false),
			patBlobIsLocal_(false)
{
    FAIL_CONDITION(	rect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect passed as parameter to Pattern2D constructor");

    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image passed as parameter to Pattern2D constructor");
   
    FAIL_CONDITION(	blob == NULL,
                    kNullParameterError,
                    "NULL blob passed as parameter to Pattern2D constructor");

	const ImageRect* blobRect = blob->getValidRect();

    FAIL_CONDITION(	(blobRect->getLeft() != rect->getLeft())  || 
                    (blobRect->getTop() != rect->getTop())    ||
		            (blobRect->getRight() > rect->getRight()) ||
                    (blobRect->getBottom() > rect->getBottom()),
                    kNullParameterError,
                    "blob and rect must have the same upperLeft and rect must cover larger area in Pattern2D constructor");
	
	if (safe)
	{	
        ImagePoint  origin(0, 0);
        pattern_ = img->grab(rect, &origin);
        blob_ = new Blob(blob, &origin);    
    	patImgIsLocal_ = patBlobIsLocal_ = true;
    }
    else
    {
        pattern_ = img;
        blob_ = blob;
    	patImgIsLocal_ = patBlobIsLocal_ = false;
    }
            
    drawContour_ = false;
    hasIndivUpdateMode_ = false;
}
catch(ErrorReport& e) {
	e.appendToMessage("called by Pattern2D constructor.");
	throw e;
}
catch(...) {
	FAIL_CONDITION( blob,
					kNullParameterError,
					"Null pointer passed to Pattern2D constructor.");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid  parameter passed to Pattern2D constructor.");
}


	  
Pattern2D::Pattern2D(const Blob* blob, const RasterImage* img, bool safe)
	try	:   DrawableObject2D(*blob),
			//
			blob_(NULL),
			pattern_(NULL),
			hasIndivUpdateMode_(false),
			updateMode_(REPLACE_PATTERN),
			drawContour_(false),
			patImgIsLocal_(false),
			patBlobIsLocal_(false)
{
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image passed as parameter to Pattern2D constructor");

    const ImageRect* rect;
    //  if we have a blob we use its valid rect.
    if (blob != NULL)   
        rect = blob->getBoundRect();
    //  otherwise that of the image will do
    else
        rect = img->getValidRect();
        
	if (safe)
	{	
        ImagePoint  origin(0, 0);
        pattern_ = img->grab(rect, &origin);
        blob_ = new Blob(blob, &origin);    
    	patImgIsLocal_ = patBlobIsLocal_ = true;
    }
    else
    {
//temp
        ImagePoint  origin(0, 0);
        pattern_ = img->grab(rect, &origin);
        blob_ = new Blob(blob, &origin);    
    	patImgIsLocal_ = patBlobIsLocal_ = false;
    }

/*
    ImagePoint  origin(0, 0);
//jyh
    pattern_ = img->grab(rect, &origin);
//jyh

    if (blob != NULL)
        blob_ = new Blob(blob, &origin);
    else
        blob_ = NULL;
*/

	patImgIsLocal_ = patBlobIsLocal_ = true;
    drawContour_ = false;
    hasIndivUpdateMode_ = false;
}
catch(ErrorReport& e) {
	e.appendToMessage("called by Pattern2D constructor.");
	throw e;
}
catch(...) {
	FAIL_CONDITION( blob,
					kNullParameterError,
					"Null pointer passed to Pattern2D constructor.");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Invalid  parameter passed to Pattern2D constructor.");
}

    
Pattern2D::Pattern2D(const ImageRect* rect, const RasterImage* img)
		try	:   DrawableObject2D(rect),
				//
			//
			blob_(NULL),
			pattern_(NULL),
			hasIndivUpdateMode_(false),
			updateMode_(REPLACE_PATTERN),
			drawContour_(false),
			patImgIsLocal_(false),
			patBlobIsLocal_(false)				
{
    FAIL_CONDITION(	img == NULL,
                    kNullRasterImageError,
                    "NULL raster image passed as parameter to Pattern2D constructor");
    FAIL_CONDITION(	rect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect passed as parameter to Pattern2D constructor");

    ImagePoint  origin(0, 0);

    pattern_ = img->grab(rect, &origin);

    blob_ = NULL;

	patImgIsLocal_ = true;
	patBlobIsLocal_ = false;
    drawContour_ = false;
    hasIndivUpdateMode_ = false;
}
catch(ErrorReport& e) {
	e.appendToMessage("called by Pattern2D constructor.");
	throw e;
}
catch(...) {
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid  ImagePoint pointer passed to Pattern2D constructor.");
}


Pattern2D::Pattern2D(BaseRasterType rasterType, int nbRows, int nbCols)
		try	:   DrawableObject2D(nbRows, nbCols),
				//
				blob_(NULL),
				pattern_(NULL),
				hasIndivUpdateMode_(false),
				updateMode_(REPLACE_PATTERN),
				drawContour_(false),
				patImgIsLocal_(false),
				patBlobIsLocal_(false)
{
    FAIL_CONDITION(	nbRows < 2,
                    kInvalidParameterError,
                    "A Pattern2D must be at least 2x2");
    FAIL_CONDITION(	nbCols < 2,
                    kInvalidParameterError,
                    "A Pattern2D must be at least 2x2");

    switch (rasterType)
    {
        case kBinaryRaster:
            pattern_ = new RasterImage_binary(nbRows, nbCols);
            break;

        case kGrayRaster:
            pattern_ = new RasterImage_gray(nbRows, nbCols);
            break;

        case kRGBa32Raster:
            pattern_ = new RasterImage_RGBa(nbRows, nbCols);
            break;

        default:
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in Pattern2D constructor");
            break;

    }

    blob_ = NULL;

	patImgIsLocal_ = true;
	patBlobIsLocal_ = false;
    drawContour_ = false;
    hasIndivUpdateMode_ = false;
}
catch(ErrorReport& e) {
	e.appendToMessage("called by Pattern2D constructor.");
	throw e;
}
catch(...) {
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid  ImagePoint pointer passed to Pattern2D constructor.");
}


Pattern2D::Pattern2D(const Pattern2D& obj)
		:   DrawableObject2D(),
			//
			blob_(NULL),
			pattern_(NULL),
			hasIndivUpdateMode_(false),
			updateMode_(REPLACE_PATTERN),
			drawContour_(false),
			patImgIsLocal_(false),
			patBlobIsLocal_(false)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Pattern2D copy constructor not implemented");
}



Pattern2D::~Pattern2D(void)
{
    if (patBlobIsLocal_ && (blob_ != NULL))
        delete blob_;
        
    if (patImgIsLocal_ && (pattern_ != NULL))
        delete pattern_;
}


const Pattern2D& Pattern2D::operator =(const Pattern2D& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Pattern2D copy operator not implemented");
	return *this;
}




#if 0
//----------------------------------------
#pragma mark Setters and getters
//----------------------------------------
#endif


void Pattern2D::setPattern(const Pattern2D* newPattern)
{
    setPattern(newPattern->getPattern(), newPattern->getBlob());
}

void Pattern2D::setPattern(const RasterImage* patImg, const Blob* patBlob)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL raster image passed as parameter to Pattern2D::setPattern");

    const ImageRect* rect;
    //  if we have a blob we use its valid rect.
    if (patBlob != NULL)   
        rect = patBlob->getBoundRect();
    //  otherwise that of the image will do
    else
        rect = patImg->getValidRect();

	if (patImgIsLocal_ && (pattern_ != NULL))
		delete pattern_;
	if (patBlobIsLocal_ && (blob_ != NULL))
		delete blob_;

	ImagePoint  origin(0, 0);

	pattern_ = patImg->grab(rect, &origin);

	if (patBlob != NULL)
	{
		blob_ = new Blob(patBlob, &origin);
		patBlobIsLocal_ = true;
	}
	else
	{
		blob_ = NULL;
		patBlobIsLocal_ = false;
	}
}


void Pattern2D::assignPattern(const RasterImage* patImg, const Blob* patBlob)
{
    FAIL_CONDITION(	patImg == NULL,
                    kNullRasterImageError,
                    "NULL raster image passed as parameter to Pattern2D::assignPattern");

    if (patImgIsLocal_ && (pattern_ != NULL))
        delete pattern_;
    pattern_ = patImg;
	patImgIsLocal_ = false;
       	
    if (patBlobIsLocal_ && (blob_ != NULL))
	    delete blob_;
	            	
    blob_ = patBlob;
	patBlobIsLocal_ = false;
}


const RasterImage* Pattern2D::getPattern(void) const
{
    return pattern_;
}


const Blob* Pattern2D::getBlob(void) const
{
    return blob_;
}

const ImageRect* Pattern2D::getBoundRect(void) const
{
    return getValidRect();
}

const ImageRect* Pattern2D::getValidRect(void) const
{
    if (blob_ != NULL)
        return blob_->getValidRect();
    else
        return pattern_->getValidRect();
}


void Pattern2D::setBoundRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}


void Pattern2D::setValidRect_(int x, int y, int theWidth, int theHeight)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}

long Pattern2D::getNbPixels(void) const
{
    if (blob_ == NULL)
        return pattern_->getWidth() * pattern_->getHeight();
    else
        return blob_->getNbPixels();
        
}

#if 0
#pragma mark -
#endif

void Pattern2D::writeInto(RasterImage* imgOut, const ImagePoint* origin)
{
	FAIL_CONDITION( imgOut == NULL,
					kNullRasterImageError,
					"Null rater image as parameter in Pattern2D::writeInto");

    if (blob_ == NULL)
        pattern_->writeInto(imgOut, origin);
    else
    {
// JYH:TOCHECK
// I don't know if getValidRect() has a side effect, so I'm leaving the call
// there.  But the variable 'rect' was going unused thus generating a compiler
// warning. -CJC
//     	ImageRect* rect = getValidRect();
        getValidRect();

    	BaseRasterType	rasterType = pattern_->getBaseRasterType();
    	if (rasterType == kGrayRaster)
    	{
			FAIL_CONDITION( true,
							kFunctionNotImplemented,
							"gray blob pattern wrinteInto not implemented yet");
    		
    	}
    	else if (rasterType == kRGBa32Raster)
    	{
    		const unsigned char* const* rgbaPat = 
    		                        (static_cast<const RasterImage_RGBa*>(pattern_))->getShiftedRGBa2D();
    		unsigned char* const* rgbaImg = 
    		            (static_cast<RasterImage_RGBa*>(imgOut))->getShiftedRGBa2D(R_W_ACCESS);
    		if (origin == NULL)
    		{
				FAIL_CONDITION( true,
								kFunctionNotImplemented,
								"gray blob pattern wrinteInto not implemented yet");
    		
    		}
    		else	
    		{
                const LineSegDeque segDeque = blob_->getSegDeque();
                LineSegDeque::const_iterator lastLine = segDeque.end();
                int iI = origin->getY();
                for (LineSegDeque::const_iterator lineIter = segDeque.begin(); lineIter != lastLine; lineIter++, iI++)
                {
                    list<HorizontalSegment*>::iterator  segIter, lastSeg = (*lineIter)->end();
                    for (segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
                    {
                        HorizontalSegment *theSeg = (*segIter);
                        int iP = theSeg->y;

                        for (int jP = theSeg->leftX, fourJP = 4*jP, jI = jP + origin->getX(), fourJI=4*jI; 
                                                    jP<=theSeg->rightX; jP++, jI++, fourJP++, fourJI++)
                        {
/*
                            rgbaImg[iI][fourJI++] = rgbaPat[iP][fourJP++];
                            rgbaImg[iI][fourJI++] = rgbaPat[iP][fourJP++];
                            rgbaImg[iI][fourJI++] = rgbaPat[iP][fourJP++];
*/
//
                            unsigned char x, y, z;
                            x = rgbaPat[iP][fourJP++];
                            y = rgbaPat[iP][fourJP++];
                            z = rgbaPat[iP][fourJP++];
//
                            rgbaImg[iI][fourJI++] = x;
                            rgbaImg[iI][fourJI++] = y;
                            rgbaImg[iI][fourJI++] = z;
                        }
                    }
                }
    		}		  
    	}
		else 
			FAIL_CONDITION( true,
							kNoCode,
							"invalid raster type in Pattern2D::writeInto");
    }
}


/*
void Pattern2D::update(Pattern2D* newPattern, bool updateBlob)
{
    Pattern2DUpdateMode theMode;
    if (hasIndivUpdateMode_)
        theMode = updateMode_;
    else
        theMode = defaultUpdateMode_;
        
    switch (theMode)
    {
        case REPLACE_PATTERN:
        	newPattern->getPattern()->copyInto(pattern_);
       	
        	//	update blob_
        	if (updateBlob)
        	{
	        	if (blob_ != NULL)
	            	delete blob_;
	            	
	            Blob    *newBlob = newPattern->getBlob();
	            if (newBlob != NULL)
	        	    blob_ = new Blob(newBlob);
	        	else
	        	    blob_ = NULL;
        	}
        	
            break;
        
        case UNION_BLOBS_REPLACE_IMG:
            break;
        
        case UNION_BLOBS_AVG_IMG:
            break;
        
        case INTERSECT_BLOBS_REPLACE_IMG:
            break;
        
        case INTERSECT_BLOBS_AVG_IMG:
            break;
        
        case AVERAGE_PATTERNS:
            break;
        
        default:
            break;
    }
}
*/

void Pattern2D::update(const Pattern2D* newPattern, bool updateBlob)
{
    FAIL_CONDITION(	newPattern == NULL,
                    kNullParameterError,
                    "NULL pattern passed as parameter to Pattern2::update");

    update(newPattern->getPattern(), newPattern->getBlob(), updateBlob);
}

void Pattern2D::update(const RasterImage* newImg, const Blob* newBlob, bool updateBlob)
{
    FAIL_CONDITION(	newImg == NULL,
                    kNullRasterImageError,
                    "NULL image passed as parameter to Pattern2::update");

    Pattern2DUpdateMode theMode;
    if (hasIndivUpdateMode_)
        theMode = updateMode_;
    else
        theMode = defaultUpdateMode_;
        
    ImagePoint  origin(0, 0);
    const ImageRect* rect;
    //  if we have a blob we use its valid rect.
    if (newBlob != NULL)   
        rect = newBlob->getBoundRect();
    //  otherwise that of the image will do
    else
        rect = newImg->getValidRect();

    switch (theMode)
    {
        case REPLACE_PATTERN:
		
		//	at this point:  Incomptible with setPattern/assignPattern distinction
//        	newImg->copyInto(pattern_, rect, &origin);
//       	
//        	//	update blob_
//        	if (updateBlob)
//        	{
//	        	if (blob_ != NULL)
//	            	delete blob_;
//	            	
//	            if (newBlob != NULL)
//	        	    blob_ = new Blob(newBlob, &origin);
//	        	else
//	        	    blob_ = NULL;
//        	}
        	
            break;
        
        case UNION_BLOBS_REPLACE_IMG:
            break;
        
        case UNION_BLOBS_AVG_IMG:
            break;
        
        case INTERSECT_BLOBS_REPLACE_IMG:
            break;
        
        case INTERSECT_BLOBS_AVG_IMG:
            break;
        
        case AVERAGE_PATTERNS:
            break;
        
        default:
            break;
    }
}

void Pattern2D::setDefaultUpdateMode(Pattern2DUpdateMode defaultUpdateMode)
{
    defaultUpdateMode_ = defaultUpdateMode;
}

void Pattern2D::setUpdateMode(Pattern2DUpdateMode updateMode)
{
    updateMode_ = updateMode;
    hasIndivUpdateMode_ = true;
}

#if 0
#pragma mark -
#endif

void Pattern2D::grabInto(RasterImage* img, const ImageRect* rect,
					Pattern2D* pattern)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}
					
Pattern2D* Pattern2D::grab(RasterImage* imgIn, const ImageRect* rect)
{
    return new Pattern2D(rect, imgIn);
}


void Pattern2D::grabInto(RasterImage* imgIn, Blob* blob,
					Pattern2D* pattern)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}
					
Pattern2D* Pattern2D::grab(RasterImage* imgIn, Blob* blob)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "not implemented yet");
}

bool Pattern2D::contains(const ImagePoint* q)
{
    FAIL_CONDITION( q == NULL,
                    kNullParameterError,
                    "Null ImagePoint passed to Blob::isInside");

    return contains(q->getX(), q->getY());
}

bool Pattern2D::contains(int x, int y)
{
    if (blob_ == NULL)
        return getBoundRect()->contains(x, y);
    else
        return blob_->contains(x, y);
        
}


#if 0
#pragma mark -
#endif

void Pattern2D::rotate(float angle, const ImagePoint* rotCenter)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


void Pattern2D::applyTransformation(const Transformation2D* theTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}



#if 0
#pragma mark -
#endif

void Pattern2D::setBlob(const Blob* blob, const ImagePoint* point)
{
    blob_ = new Blob(blob, point);
}

void Pattern2D::setContourColor(float red, float green, float blue)
{
    if (blob_ != NULL)
        blob_->setContourColor(red, green, blue);

    //  if this is a rectangular pattern we interpret the call as meaning "draw frame" 
    else
        setFrameColor(red, green, blue);    

}

void Pattern2D::setContourColor(const float theColor[])
{
    if (blob_ != NULL)
        blob_->setContourColor(theColor);

    //  if this is a rectangular pattern we interpret the call as meaning "draw frame" 
    else
        setFrameColor(theColor);    

}


void Pattern2D::setContourColor(unsigned char red, unsigned char green, unsigned char blue)
{
    if (blob_ != NULL)
        blob_->setContourColor(red, green, blue);

    //  if this is a rectangular pattern we interpret the call as meaning "draw frame" 
    else
        setFrameColor(red, green, blue);    

}

void Pattern2D::setContourColor(const unsigned char theColor[])
{
    if (blob_ != NULL)
        blob_->setContourColor(theColor);

    //  if this is a rectangular pattern we interpret the call as meaning "draw frame" 
    else
        setFrameColor(theColor);    

}

void Pattern2D::setDrawContour(bool drawContour)
{
    if (blob_ != NULL)
    {
        drawContour_ = drawContour;
        if (drawContour_) {
            //  force contour calculation


// JYH:TOCHECK
// I don't know if getContour() has a side effect, so I'm leaving the call
// there.  But the variable 'rect' was going unused thus generating a compiler
// warning. -CJC
//             Contour *contour = blob_->getContour();
            blob_->getContour();
        }
    }
    //  if this is a rectangular pattern we interpret the call as meaning "draw frame" 
    else
        setDrawFrame(drawContour);    
}

void Pattern2D::setRenderingScale(float scale) const
{
    pattern_->setRenderingScale(scale, scale);
    blob_->setRenderingScale(scale, scale);
}


void Pattern2D::setRenderingScale(float scaleX, float scaleY) const
{
    pattern_->setRenderingScale(scaleX, scaleY);
    blob_->setRenderingScale(scaleX, scaleY);
}


#if 0
#pragma mark -
#endif

void Pattern2D::render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
						float theZ, bool shifted, const ImagePoint* origin) const
{
    if (blob_ == NULL)
        pattern_->render(theZ, shifted, origin);
    else
        renderBlobPattern_(scaledLeft, scaledTop, scaledRight, scaledBottom, theZ, shifted, origin);
        
}

void Pattern2D::renderBlobPattern_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
								   float theZ, bool shifted, const ImagePoint* origin) const
{
    //--------------------------------------------------------------
    //	Part 1: make sure that what we draw is up to date
    //--------------------------------------------------------------
    //  Binary images are a special case.  OpenGL cannot draw QD binary
    //  raster, so we need to use the gray raster instead.
    bool    binaryRaster = (pattern_->getBaseRasterType() == kBinaryRaster);
    if (binaryRaster)
    {
        //  if the gworld is the reference and has been modified, or 
        //  the gray raster has been modified, update
        /// the gray raster (re-binarize it).
        if (pattern_->graphWorldIsReference() || pattern_->referenceHasChanged())
            pattern_->requestRasterUpdate( );

    }
    else
        //  For other image types, the base raster is the one to draw, so if it
        //  is not up-to-date, update it.
        if ( !pattern_->graphWorldIsReference() && pattern_->referenceHasChanged() )
            pattern_->requestRasterUpdate( );



    //--------------------------------------------------------------
    //	Part 2: render the blob pattern
    //--------------------------------------------------------------
    //	I apply a -1 scaling vertical scaling to my image so that it gets displayed
    //	with the right side up.
    GLenum  rasterType;
    int     pixelDepth;
    switch (pattern_->getBaseRasterType())
    {
        case kBinaryRaster:
        case kGrayRaster:
            rasterType = GL_LUMINANCE;
            pixelDepth = 1;
            break;
        
        case kRGB24Raster:
            rasterType = GL_RGB;
            pixelDepth = 3;
            break;
        
        case kRGBa32Raster:
            rasterType = GL_RGBA;
            pixelDepth = 4;
            break;

        case kUndefRasterType:
        case kLongGrayRaster:
        case kaRGB16Raster:
        case kaRGB32Raster:
            FAIL_CONDITION(true, kNoCode, "Unhandled case in a switch statement");
    }
    const unsigned char* const* raster = pattern_->getRaster2D();
    
	const float SCALE_X = getRenderingScaleX(), 
				SCALE_Y = getRenderingScaleY();
	
    glPushMatrix();
    glLoadIdentity();
    glPixelZoom(SCALE_X, -SCALE_Y);

    const LineSegDeque segDeque = blob_->getSegDeque();
    LineSegDeque::const_iterator lastLine = segDeque.end();
    for (LineSegDeque::const_iterator lineIter = segDeque.begin(); lineIter != lastLine; lineIter++)
    {
        list<HorizontalSegment*>::iterator lastSeg = (*lineIter)->end();
        for (list<HorizontalSegment*>::iterator segIter = (*lineIter)->begin(); segIter != lastSeg; segIter++)
        {
            HorizontalSegment   *theSeg;
            theSeg = (*segIter);

            glRasterPos3f(scaledLeft + SCALE_X*theSeg->leftX, 
            				scaledTop - SCALE_Y*theSeg->y, theZ);
            glDrawPixels(theSeg->rightX - theSeg->leftX + 1, 1, 
                         rasterType, GL_UNSIGNED_BYTE,
                         raster[theSeg->y] + pixelDepth*theSeg->leftX);
        }
    }
    glPopMatrix();

    //--------------------------------------------------------------
    //	Part 3: if needed, draw the blob's contour
    //--------------------------------------------------------------
    if (drawContour_)
        blob_->renderContour(theZ,  shifted, origin);

}


void Pattern2D::computeBoundingRect_(ImageRect& rect) const
{
}


void Pattern2D::computeValidRect_(ImageRect& rect) const
{
}


void Pattern2D::computeCentroid_(ImagePoint_F& cent) const
{
}


void Pattern2D::computeCenterOfMass_(ImagePoint_F& cent) const
{
}


