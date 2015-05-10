/*  NAME:
        Pattern2D.h
 
    DESCRIPTION:
        Pattern2D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_2D_H
#define URIVL_PATTERN_2D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Blob.h"
#include "RasterImage_binary.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGBa.h"
#include "RasterImage.h"
#include "Pattern2D.h"
#include "ImageRect.h"


namespace uriVL
{
    /** Standard update modes for a Pattern2D
     */
    typedef enum Pattern2DUpdateMode {
                                        REPLACE_PATTERN = 0,
                                        UNION_BLOBS_REPLACE_IMG,
                                        UNION_BLOBS_AVG_IMG,
                                        INTERSECT_BLOBS_REPLACE_IMG,
                                        INTERSECT_BLOBS_AVG_IMG,
                                        AVERAGE_PATTERNS
    } Pattern2DUpdateMode;
    
    
    /** A Pattern2D is defined as a connected set of pixels.
     *  A Pattern2D can be stored either as a <code>Blob</code> or as a rectangular region.
     *  In either case, image information is stored for each pixel of the Pattern.  
     *  The indices of the pixels of the blob and of the image stored have been shifted so
     *  that the pattern's upper-left corner is at (0, 0).
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class Pattern2D : public DrawableObject2D
    {
        public:
        
            /** A new empty Pattern2D
             */
            Pattern2D(void);


            /** A new Pattern2D is defined by a Blob (connected set of pixels), an ImageRect
             *  and a RasterImage to get pixel data from. (blob and rect must have the same upperleft)
             * (blob's rect and rect must have the same upperleft and rect cover larger area).
             * <p>If you you decide to use the "fast", usafe mode, then you cannot delete the object
             *  blob refers to before this Pattern2D object has been deleted.</p>
             *  @param  blob    list of connected pixels that define the pattern's geometry
             *  @param  img     RasterImage where to get pixel data from
			 *	@param	safe	when true, a copy of the blob will be created and stored
             */
            Pattern2D(const Blob* blob, const ImageRect* rect, const RasterImage* img, bool safe=false);

            /** A new Pattern2D is defined by a Blob (connected set of pixels) and
             *  a RasterImage to get pixel data from.
             * <p>If you you decide to use the "fast", usafe mode, then you cannot delete the object
             *  blob refers to before this Pattern2D object has been deleted.</p>
             *  @param  blob    list of connected pixels that define the pattern's geometry
             *  @param  img     RasterImage where to get pixel data from
			 *	@param	safe	when true, a copy of the blob will be created and stored
             */
            Pattern2D(const Blob* blob, const RasterImage* img, bool safe=false);
            
            /** A new Pattern2D is defined by an image rectangle and
             *  a RasterImage to get pixel data from.
             *  @param  rect    image rectangle that define the pattern's geometry
             *  @param  img     RasterImage where to get pixel data from
             */
            Pattern2D(const ImageRect* rect, const RasterImage* img);
            

            /** A new rectangular Pattern2D is defined by its dimensions
             *  @param  rasterType  what kind of image raster the pattern stores
             *  @param  nbRows  number of rows of the pattern
             *  @param  nbCols  number of columns of the pattern
             */
            Pattern2D(BaseRasterType rasterType, int nbRows, int nbCols);
            

            /**  Destructor
             */
            virtual ~Pattern2D(void);
            
            /** sets new raster and blob info for this pattern. This function actually copies
             *  the data.  This is equivalent to the "safe" mode of the constructor.

             */
            void setPattern(const Pattern2D* newPattern);

            /** sets new raster and blob info for this pattern. This function actually copies
             *  the data.  This is equivalent to the "safe" mode of the constructor
             */
            void setPattern(const RasterImage* patImg, const Blob* patBlob);

            /** Assigns new raster and blob info for this pattern.  This function does not
             *  copy the data, just references to the original objects.  This is equivalent to the
			 *	"unsafe" mode of the constructor
             *  @see patImgIsLocal_
             *  @see patBlobIsLocal_
             */
            void assignPattern(const RasterImage* patImg, const Blob* patBlob);

            /** Returns a reference to the pattern's pixel data
             *  @return a reference to the pattern's pixel data
             */
            const RasterImage* getPattern(void) const;
            
            /** Returns a reference to the pattern's blob data
             *  @return a reference to the pattern's blob data
             */
            const Blob* getBlob(void) const;
            
            /**
             *  Returns the pattern's bounding rectangle.
             *
             *  Returns a pointer to the pattern's bounding rectangle, which is either
             *  the of the blob or (if the blob is NULL) that of the RasterImage pattern.
             *  Note that this rectangle always has its upper-left corner at (0,0).
             *  
             *  @return     pointer to the pattern's bounding rectangle
             *
             *  @see    validRect_
             *  @see    boundRect_
             *  @see    getValidRect
             */
            const ImageRect* getBoundRect() const;
            
            /**
             *  Returns the pattern's valid data rectangle.
             *
             *  Returns a pointer to the pattern's valid data rectangle, which is either
             *  the of the blob or (if the blob is NULL) that of the RasterImage pattern.
             *  Note that this rectangle always has its upper-left corner at (0,0).
             *  
             *  @return     pointer to the pattern's valid data rectangle
             *
             *  @see    validRect_
             *  @see    boundRect_
             *  @see    getBoundRect
             */
            const ImageRect* getValidRect() const;
            
            
            /** Returns the total number of pixels in the pattern
             *  @return     total number of pixels in the pattern
             */
            long getNbPixels() const;


            /** Writes this pattern into a raster image using the point specified as origin for
             *  this pattern's valid rectangle's upper-left corner. Contrary to <code>copyInto</code>
             *  this function does not resize the destination image if it is smaller than the
             *  source.  It does not even modify the valid data rectangle of the destination
             *  image.  It just writes in the intersection of the images' valid data rectangles.
             
             *  @param  imgOut  the image to write into
            *   @param  origin  the point to use as origin for this pattern's valid data rectangle's
            *                   upper-left corner.  
             */
            void writeInto(RasterImage* imgOut, const ImagePoint* origin);


            /** Update the state of this pattern using the new measurement received as parameter,
             *  using the selected pattern update mode.
             *  @param  newPattern  the new measurement with which to update the state of this pattern
             *  @param  updateBlob  whether update blob inside the pattern2D
             *  @see    Pattern2DUpdateMode
             *  @see    setDefaultUpdateMode
             *  @see    setUpdateMode
             */
            void update(const Pattern2D* newPattern, bool updateBlob = false);
            
            /** Update the state of this pattern using the new measurement received as parameter,
             *  using the selected pattern update mode.
             *  @param  newImage  raster image of the new measurement with which to update the state of this pattern
             *  @param  newBlob  blob info of the new measurement with which to update the state of this pattern
             *  @param  updateBlob  whether update blob inside the pattern2D
             *  @see    Pattern2DUpdateMode
             *  @see    setDefaultUpdateMode
             *  @see    setUpdateMode
             */
            void update(const RasterImage* newImg, const Blob* newBlob, bool updateBlob);
            
            /** Sets a new default pattern update mode for this class
             *  @see    Pattern2DUpdateMode
             *  @see    defaultUpdateMode_
             *  @see    hasIndivUpdateMode_
             */
            static void setDefaultUpdateMode(Pattern2DUpdateMode defaultUpdateMode);
            
            /** Sets a new Individual update mode for this pattern (hasIndivUpdateMode_ is now true).
             *  @see    Pattern2DUpdateMode
             *  @see    setDefaultUpdateMode
             *  @see    setUpdateMode
             *  @see    defaultUpdateMode_
             *  @see    hasIndivUpdateMode_
             */
            void setUpdateMode(Pattern2DUpdateMode updateMode);

            /** Grabs a rectangular pattern in the input image and assign this value to the
             *  pattern object received as parameter
             *  @param  imgIn   the image to grab the pattern into
             *  @param  rect    the rectangle within within to grab the pattern
             *  @param  pattern the rectangular Pattern2D object in which to store the data grabbed
             */
            static void grabInto(RasterImage* img, const ImageRect* rect,
                                Pattern2D* pattern);
                                
            /** Grabs a rectangular pattern in the input image and returns it as a new
             *  pattern object.
             *  @param  imgIn   the image to grab the pattern into
             *  @param  rect    the rectangle within within to grab the pattern
             *  @return     a new rectangular Pattern2D object
             */
            static Pattern2D* grab(RasterImage* imIng, const ImageRect* rect);
            

            /** Grabs a 2D pattern in the input image at the location specified by the blob
             *  received as parameter and assign this value to the
             *  pattern object received as parameter
             *  @param  imgIn   the image to grab the pattern into
             *  @param  blob    the blob template within within to grab the pattern
             *  @param  pattern the Pattern2D object in which to store the data grabbed
             */
            static void grabInto(RasterImage* imgIn, Blob* blob,
                                Pattern2D* pattern);
                                
            /** Grabs a 2D pattern in the input image at the location specified by the blob
             *  returns it as a new pattern object.
             *  @param  imgIn   the image to grab the pattern into
             *  @param  blob    the blob template within within to grab the pattern
             *  @return     a new Pattern2D object
             */
            static Pattern2D* grab(RasterImage* imgIn, Blob* blob);
            
            /** Checks whether the point passed as parameter belongs to the pattern.
             *  @param  pt  the point to check
             *  @return <code>true</code> if pt is inside this pattern's contour
             */
            bool contains(const ImagePoint* pt);

            /** Checks whether the point passed as parameter is inside the pattern
             *  @param  x   x coordinate of the point to check
             *  @param  y   y coordinate of the point to check
             *  @return <code>true</code> if pt is inside this pattern
             */
            bool contains(int x, int y);


            /** Tells the blob whether its contour should be drawn when the pattern is 
             *  rendered
             *  @param  drawContour <code>true</code> if the blob's contour is to be drawn
             */
            void setDrawContour(bool drawContour);
            
            /** Assigns a rendering color to the contour (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setContourColor(float red, float green, float blue);


            /** Assigns a rendering color to the contour  (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setContourColor(unsigned char red, unsigned char green, unsigned char blue);


            /** Assigns a rendering color to the contour (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setContourColor(const float theColor[]);


            /** Assigns a rendering color to the contour (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setContourColor(const unsigned char theColor[]);

            /** Assigns a reference to the pattern's blob data
             *  @param  blob    a reference to a blob
             *  @param  point   a reference to an ImagePoint
             */
            void setBlob(const Blob* blob, const ImagePoint* point);
            
        
            /** Applies a Transformation2D to this object
             *  @param  theTransf   the Transformation2D to apply to this object
             */
            void applyTransformation(const Transformation2D* theTransf);

            /**	Rotates the object by the angle specified (in radians)
             *	If rotationCenter is null, then the centroid of the object is used
             *	as center of rotation
             *	@param	angle	the angle of the rotation (in radians)
             *	@param	rotCenter	the center of the rotation
             */
            void rotate(float angle, const ImagePoint* rotCenter = NULL);


            /**
             *  Sets a uniform scale at which this image should be rendered.
             *
             *  An image can be rendered at different scales (smaller if the
             *  scale is less than 1, larger is the scale is greater than 1). This
             *  function specifies the same scale for the horizontal and vertical
             *  directions (uniform scaling).
             *
             *  @param  scale    scale for the horizontal and vertical directions
             */
            void setRenderingScale(float scale) const;

            /**
             *  Sets horizontal and vertical scales at which this image should be rendered.
             *
             *  An image can be rendered at different scales (smaller if the
             *  scale is less than 1, larger is the scale is greater than 1). This
             *  function specifies separate scales for the horizontal and vertical
             *  directions.
             *
             *  @param  scaleX    scale for the horizontal direction
             *  @param  scaleY    scale for the vertical direction
             */
            void setRenderingScale(float scaleX, float scaleY) const;



        private:
        
            /** Reference to the blob that lists the pixels making up the pattern
             */
            const Blob* blob_;
            
            /** The pixel value of the pattern over the blob's bounding rectangle
             */
            const RasterImage* pattern_;
        
            /** True if this pattern has an "individual" (non-default) update mode
             *  @see    Pattern2DUpdateMode
             *  @see    setDefaultUpdateMode
             *  @see    setUpdateMode
             *  @see    defaultUpdateMode_
             *  @see    updateMode_
             */
            bool hasIndivUpdateMode_;

            /** Individual update mode for this pattern (if hasIndivUpdateMode_ is true).
             *  @see    Pattern2DUpdateMode
             *  @see    setDefaultUpdateMode
             *  @see    setUpdateMode
             *  @see    defaultUpdateMode_
             *  @see    hasIndivUpdateMode_
             */
            Pattern2DUpdateMode updateMode_;
            
            /** When rendering the pattern, draw its contour
             */
            bool    drawContour_;
            
            /** True when the pattern image is local to the object
             */
            bool    patImgIsLocal_;

            /** True when the pattern's blob is local to the object
             */
            bool    patBlobIsLocal_;

            /** Default pattern update mode for this class
             *  @see    Pattern2DUpdateMode
             *  @see    setDefaultUpdateMode
             *  @see    setUpdateMode
             *  @see    hasIndivUpdateMode_
             *  @see    updateMode_
             */
            static  Pattern2DUpdateMode defaultUpdateMode_;
            
			
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            Pattern2D(const Pattern2D& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const Pattern2D& operator =(const Pattern2D& obj);


            /**
             *  Sets a new bounding rectangle for the pattern.
             *
             *  At this point I am not quite sure what this function should do.  It has to
             *  be implemented because a Pattern2D is a DrawableObject2D.
             *
             *  @param  x       X coordinate of the image's origin
             *  @param  y       Y coordinate of the image's origin
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            void setBoundRect_(int x, int y, int theWidth, int theHeight);


            /**
             *  Sets a new valid data rectangle for the pattern.
             *
             *  At this point I am not quite sure what this function should do.  It has to
             *  be implemented because a Pattern2D is a DrawableObject2D.
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            void setValidRect_(int x, int y, int theWidth, int theHeight);

            /**
             *  Renders the pattern at the depth and location specified.
             *
             *  A drawable object can be rendered with its valid data rectangle's upper-left
             *  corner in the upper-left corner of the rendering window (shifted=false and
             *  origin=NULL), or using the absolute coordinates of its valid data rectangle's
             *  upper-left corner (shited=true and origin=NULL), or by setting its upper-left
             *  corner at the location specified (origin != NULL).
             *
             *  @param  scaledLeft  x coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledTop	y coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledRight x coordinate of the object's lower-right corner when rendered scaled
			 *	@param	scaledBottom y coordinate of the object's lower-right corner when rendered scaled
             *  @param  theZ    Depth at which this object is to be rendered.
             *  @param  shifted     true if the object should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @param  origin      location of the object's upper-left corner in the rendering
             */
            void render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
								 float theZ, bool shifted, const ImagePoint* origin) const;


            /**
             *  Renders a blob pattern.
             *
             *  Renders a blob pattern at the depth and location specified by drawing the 
             *  value of the image at the pixels that belong to the blob.
             *
             *  @param  scaledLeft  x coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledTop	y coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledRight x coordinate of the object's lower-right corner when rendered scaled
			 *	@param	scaledBottom y coordinate of the object's lower-right corner when rendered scaled
             *  @param  theZ    Depth at which this object is to be rendered.
             *  @param  shifted     true if the object should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @param  origin      location of the object's upper-left corner in the rendering
             */
            void renderBlobPattern_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
									float theZ, bool shifted, const ImagePoint* origin) const;


            /** Calculates the object's bounding rect
			 *	@param	rect	the bounding box to update
             */
            void computeBoundingRect_(ImageRect& rect) const;


            /** Calculates the object's valid box
			 *	@param	rect	the bounding box to update
             */
            void computeValidRect_(ImageRect& rect) const;


            /** Computes the contour's centroid.
			 *	@param	cent	the centroid point to update
             */
			void computeCentroid_(ImagePoint_F& cent) const;

            /** Computes the object's center of mass by calculating the  
             *  average point
			 *	@param	cent	the center of mass point to update
             */
            void computeCenterOfMass_(ImagePoint_F& cent) const;

    };
    
    typedef std::list<Pattern2D* > Pattern2DList;
                                    
}

#endif  //  PATTERN_2D_H
