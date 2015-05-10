/*  NAME:
        DrawableObject2D.h

    DESCRIPTION:
        DrawableObject2D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_DRAWABLE_OBJECT_2D_H
#define     URIVL_DRAWABLE_OBJECT_2D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageRect.h"
#include "Transformation2D.h"

namespace uriVL
{
    const int IMAGE_SPACE_BOUND = 32767;


    /** Enumerated type that lists the type of data that could be found in a
     *  drawable object
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    typedef enum DrawableInformationType
    {
            kUndetermined = -1,
            kDense = 0,
            kSparse,
            kInterpolated,
            kDenseUncertain,
            kSparseUncertain,
            kInterpolatedUncertain

    } DrawableInformationType;



    /** Virtual base DrawableObject2D class.
     *
     *  DrawableObject2D is the base class for all classes of 2D objects that can
     *  be drawn in by OpenGL: RasterImage, VectorField, Blob, etc.
	 *
	 *	Any (direct or indirect) non-pure-virtual subclass of DrawableObject2D that does not 
	 *	inherit implementations of computeBoundingRect_, computeValidRect_, computeCentroid_, 
	 *	computeCenterOfMass_ along its inheritance chain <b>must</b> implement these functions
	 *	and declare DrawableObject2D as a friend class.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT DrawableObject2D
    {
        public:

            /**
             *  Destructor
             */
            virtual ~DrawableObject2D(void);

            /**
             *  Returns the object's bounding rectangle.
             *
             *  Returns a pointer to the object's bounding rectangle.
             *
             *  Reminder: the ImageRect class uses the pixel-as-rectangle convention.  
             *  Many subclasses will need to override this function (e.g RasterImage & 
             *  LabelRaster classes, Blob & Pattern2D classes).
             *
             *  @return     pointer to the object's bounding rectangle
             *
             *  @see    validRect_
             *  @see    boundRect_
             *  @see    getValidRect
             */
            virtual const ImageRect* getBoundRect(void) const;
			

			/**	Indicates whether the boinding rect is empty
			 *	@return		true if the bounding rect is empty
			 */
			bool boundRectIsEmpty(void) const;

            /**
             *  Returns the object's valid data rectangle.
             *
             *  Returns a pointer to the object's valid data rectangle.  Many subclasses will
             *  need to override this function (e.g RasterImage & LabelRaster classes, 
             *  Blob & Pattern2D classes)
             *
             *  Reminder: the ImageRect class uses the pixel-as-rectangle convention.
             *
             *  @return     pointer to the object's valid data rectangle
             *
             *  @see    validRect_
             *  @see    boundRect_
             *  @see    getBoundRect
             */
            virtual const ImageRect* getValidRect(void) const;

            /** Returns the object's centroid.
             *  @return the object centroid as a float image point
             */
            const ImagePoint_F* getCentroid(void)const ;
            
            /** Returns the object's center of mass.
             *  @return the object center of mass as a float image point
             */
            const ImagePoint_F* getCenterOfMass(void) const;

            /**
             *  Indicates whether this image's bounding rectangle and that of the
             *  drawable object received as parameter overlap.
             *  @param   img    the image to test agains this image
             *  @return  true if the two rectangles overlap, false otherwise
             */
            virtual bool overlaps(DrawableObject2D* obj) const;

            /**
             *  Indicates whether this image's bounding rectangle and the one
             *  received as parameter overlap.
             *  @param   rect    the rectangle to test agains that of this image
             *  @return  true if the two rectangles overlap, false otherwise
             */
            virtual bool overlaps(const ImageRect* rect) const;

            /**
             *  Indicates whether this image's bounding rectangle and that of the image
             *  received as parameter coincide.
             *  @param   img    the image to test agains this image
             *  @return  true if the two rectangles coincide, false otherwise
             */
            virtual bool coincidesWith(DrawableObject2D* obj) const;

            /**
             *  Indicates whether this image's bounding rectangle and the one
             *  received as parameter coincide.
             *  @param   rect    the rectangle to test agains that of this image
             *  @return  true if the two rectangles coincide, false otherwise
             */
            virtual bool coincidesWith(const ImageRect* rect) const;

            /**
             *  Indicates whether this image's bounding rectangle contains that of the
             *  drawable object received as parameter.
             *  @param   img    the image to test agains this image
             *  @return  true if theRect is contained within this rectangle, false otherwise
             */
            virtual bool contains(const DrawableObject2D* obj) const;

            /**
             *  Indicates whether this image's bounding rectangle contains the one
             *  received as parameter.
             *  @param   rect    the rectangle to test agains that of this image
             *  @return  true if theRect is contained within this rectangle, false otherwise
             */
            virtual bool contains(const ImageRect* rect) const;

			/** Checks whether the point passed as parameter belongs to the rectangle.
             *  @param  pt  		the point to check
             *  @param  padding    	the difference between the rectangle and search rectangle
             *  @return <code>true</code> if pt is inside this rectangle
             */
			virtual bool contains(const ImagePoint* pt, int padding=0) const;

            /** Checks whether the point passed as parameter belongs to the rectangle.
             *  @param  x   		x coordinate of the point to check
             *  @param  y   		y coordinate of the point to check
             *  @param  padding    	the difference between the rectangle and search rectangle
             *  @return <code>true</code> if pt is inside this rectangle
             */
			virtual bool contains(int x, int y, int padding=0) const;

            /**
             *  Indicates whether this image's bounding rectangle is contained by that
             *  of the drawable object received as parameter.
             *  @param   img    the image to test agains this image
             *  @return  true if this image contains img, false otherwise
             */
            virtual bool isContainedBy(DrawableObject2D* obj) const;

            /**
             *  Indicates whether this image's bounding rectangle is contained by the
             *  received as parameter.
             *  @param   rect    the rectangle to test agains that of this image
             *  @return  true if this image contains img, false otherwise
             */
            virtual bool isContainedBy(const ImageRect* rect) const;

            /**
             *  Returns the number of rows for the image.
             *
             *  @return     number of rows for the image
             */
            virtual int getHeight(void) const;

            /**
             *  Returns the number of columns for the image.
             *
             *  Beware:     Base rasters have <b>padded</b> rows.  You cannot rely on the
             *              fact that the number of bytes in a row is equal to the number of
             *              columns multiplied by by the number of bytes per pixel.
             *
             *  @return     number of columns for the image
             */
            virtual int getWidth(void) const;
            
            /**
             *  Returns the minimum x coordinate of the bounding rectangle
             *  @return     x coordinate of the upper-left pixel
             */
            int getLeft(void) const;

            /**
             *  Returns the maximum x coordinate of the bounding rectangle
             *  @return     x coordinate of the upper-right pixel
             */
            int getRight(void) const;

            /**
             *  Returns the minimum y coordinate of the bounding rectangle
             *  @return     y coordinate of the upper-left pixel
             */
            int getTop(void) const;

            /**
             *  Returns the maximum y coordinate of the bounding rectangle
             *  @return     y coordinate of the lower-right pixel
             */
            int getBottom(void) const;

            /**
             * returns the coordinates of the upper left corner of the object
             * 
             * @return		coordinates of the upper left corner
             */
             virtual const ImagePoint*  getUpperLeft(void) const;

            /**
             *  Sets a new origin for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation does not result in a reallocation of the image's
             *  base raster, but the sifted raster will be reallocated.
             *
             *  @param  x   X coordinate of the image's origin
             *  @param  y   Y coordinate of the image's origin
             */
            void setOrigin(int x = 0, int y = 0);

            /** Applies a Transformation2D to this object
             *  @param  theTransf   the Transformation2D to apply to this object
             */
            virtual void applyTransformation(const Transformation2D* theTransf) = 0;

            /**	Rotates the object by the angle specified (in radians)
             *	If rotationCenter is null, then the centroid of the object is used
             *	as center of rotation
             *	@param	angle	the angle of the rotation (in radians)
             *	@param	rotCenter	the center of the rotation
             */
            virtual void rotate(float angle, const ImagePoint* rotCenter = NULL) = 0;

            /**
             *  Translates the raster image by the specified amount.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation does not result in a reallocation of the image's
             *  base raster, but the sifted raster will be reallocated.
             *
             *  @param  dx   horizontal displacement of the image's origin
             *  @param  dy   vertical displacement of the image's origin
             */
           virtual void translate(int dx = 0, int dy = 0);

            /**
             *  Translates the raster image by the specified amount.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation does not result in a reallocation of the image's
             *  base raster, but the sifted raster will be reallocated.
             *
             *  @param  dx   horizontal displacement of the image's origin
             *  @param  dy   vertical displacement of the image's origin
             */
           virtual void translate(float dx = 0.f, float dy = 0.f);

            /**
             *  Sets a new bounding rectangle for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation may require a reallocation of the image.
             *
             *  This function should be overridden by child classes that have their
             *  own raster (eg.  HSV, gray_F, etc.)
             *
             *  @param  theRect new bounding rectangle for the image
             */
            void setBoundRect(const ImageRect* theRect);

            /**
             *  Sets a new bounding rectangle for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation may require a reallocation of the image.
             *
             *  This function should be overridden by child classes that have their
             *  own raster (eg.  HSV, gray_F, etc.)
             *
             *  @param  x           X coordinate of the image's origin
             *  @param  y           Y coordinate of the image's origin
             *  @param  theWidth    number of columns occupied by the drawable object
             *  @param  theHeight   number of rows occupied by the drawable object
             */
            virtual void setBoundRect(int x, int y, int theWidth, int theHeight);

            /**
             *  Sets a new valid data rectangle for the drawable object.
             *
             *  A raster image or vector field occupies a rectangle within the global
             *  image space.
             *  For example, a vector field could be defined as a subpart of another.
             *  Only part of the image may contain valid data (for example after application
             *  of a filter).
             *  The function call will fail if the new valid data rect is not contained
             *  by the vector field's bounding rectangle
             *
             *  @param  theRect new valid data rectangle for the vector field
             */
            void setValidRect(const ImageRect* theRect);

            /**
             *  Sets a new valid data rectangle for the drawable object.
             *
             *  A raster image or vector field occupies a rectangle within the global
             *  image space.
             *  For example, a vector field could be defined as a subpart of another.
             *  Only part of the image may contain valid data (for example after application
             *  of a filter).
             *  The function call will fail if the new valid data rect is not contained
             *  by the vector field's bounding rectangle
             *
             *  May be overridden by child classes that don't need to maintain their valid
             *  data rectangle at all times (e.g. Blob, Contour)
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            virtual void setValidRect(int x, int y, int theWidth, int theHeight);

            /**
             *  Renders the object at the default depth and (0, 0) location.
             *
             *  Renders the object, using OpenGL.
             *  The rendering is done at the preset "depth".
			 *
			 *	All render functions are declared to be const.  The basic idea behind that is that
			 *	rendering just gives a snapshot of the current state of the object but should not 
			 *	really change the object.  According to this view, anything that needs to be 
			 *	changed/updated at render time is really mutable.
             *
             *  @see    setRenderDepth
             */
            void render(void) const;

            /**
             *  Renders the "shifted" object at the default depth and (0, 0) location.
             *
             *  Renders the "shifted" object, using OpenGL.
             *  The rendering is done at the preset "depth".
			 *
			 *	All render functions are declared to be const.  The basic idea behind that is that
			 *	rendering just gives a snapshot of the current state of the object but should not 
			 *	really change the object.  According to this view, anything that needs to be 
			 *	changed/updated at render time is really mutable.
             *
             *  @param  shifted     true if the object should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @see    setRenderDepth
             */
            void render(bool shifted) const;

            /**
             *  Renders the object's base raster at the depth specified and (0, 0) location.
             *
             *  Renders the RasterImage's base raster, using OpenGL's glDrawPixels function.
             *  The rendering is done at depth specified.  The latter should be chosen so
             *  that the image lies within the frustum and vector graphics can be overlaid
			 *
			 *	All render functions are declared to be const.  The basic idea behind that is that
			 *	rendering just gives a snapshot of the current state of the object but should not 
			 *	really change the object.  According to this view, anything that needs to be 
			 *	changed/updated at render time is really mutable.
             *
             *  @param  theZ    Depth at which this object is to be rendered.
             */
            void render(float theZ) const;

            /**
             *  Renders the drawable object at the depth specified and (0, 0) location.
             *
             *  Renders the drawable object, using OpenGL at the depth specified.
			 *
			 *	All render functions are declared to be const.  The basic idea behind that is that
			 *	rendering just gives a snapshot of the current state of the object but should not 
			 *	really change the object.  According to this view, anything that needs to be 
			 *	changed/updated at render time is really mutable.
             *
             *  @param  theZ    Depth at which this object is to be rendered.
             *  @param  shifted     true if the image should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             */
            void render(float theZ, bool shifted) const;


            /**
             *  Renders the drawable object at the default depth and at the location specified.
             *
             *  A drawable object can be rendered with its bounding rectangle's upper-left
             *  corner in the upper-left corner of the rendering window (shifted=false and
             *  origin=NULL), or using the absolute coordinates of its valid data rectangle's
             *  upper-left corner (shited=true and origin=NULL), or by setting its upper-left
             *  corner at the location specified (origin != NULL).
			 *
			 *	All render functions are declared to be const.  The basic idea behind that is that
			 *	rendering just gives a snapshot of the current state of the object but should not 
			 *	really change the object.  According to this view, anything that needs to be 
			 *	changed/updated at render time is really mutable.
             *
             *  @param  origin      location of the object's upper-left corner in the rendering
             */
            void render(const ImagePoint* origin) const;


            /**
             *  Renders the drawable object at the depth and location specified.
             *
             *  A drawable object can be rendered with its bounding rectangle's upper-left
             *  corner in the upper-left corner of the rendering window (shifted=false and
             *  origin=NULL), or using the absolute coordinates of its valid data rectangle's
             *  upper-left corner (shited=true and origin=NULL), or by setting its upper-left
             *  corner at the location specified (origin != NULL).
			 *
			 *	All render functions are declared to be const.  The basic idea behind that is that
			 *	rendering just gives a snapshot of the current state of the object but should not 
			 *	really change the object.  According to this view, anything that needs to be 
			 *	changed/updated at render time is really mutable.
             *
             *  @param  theZ    Depth at which this object is to be rendered.
             *  @param  shifted     true if the image should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @param  origin      location of the object's upper-left corner in the rendering
             */
            void render(float theZ, bool shifted, const ImagePoint* origin) const;


            /**
             *  Specifies the depth at which the RasterImage's base raster should be rendered.
             *
             *  The rendering is done at depth specified.  The latter should be chosen so
             *  that the image lies within the frustum and vector graphics can be overlaid
             *
             *  @param  theZ    Depth at which this object is to be rendered.
             */
            void setRenderDepth(float theZ);


            /** Assigns a rendering color to the object (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            virtual void setColor(float red, float green, float blue);


            /** Assigns a rendering color to the object (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            virtual void setColor(unsigned char red, unsigned char green, unsigned char blue);


            /** Assigns a rendering color to the object (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            virtual void setColor(const float theColor[]);


            /** Assigns a rendering color to the object (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            virtual void setColor(const unsigned char theColor[]);


			/**	Returns a read-only pointer to the object's color data.
			 *	Typically this function should only be called by the child class's 
			 *	render_ function.
			 *	@return		read-only pointer to the object's color data
			 */
			const float* getColor(void) const;
			
            /**
             *  Specifies the a color for the frame of this object.
             *
             *  An image can be rendered with a colored surrounding frame (by default
             *  there is no such frame).  This function allows to specify the color
             *  for the frame.
             *
             *  @see    setDrawFrame
             *
             *  @param  frameRed    red value for the frame
             *  @param  frameGreen  green value for the frame
             *  @param  frameBlue   blue value for the frame
             */
            void setFrameColor(float frameRed = 0.f, float frameGreen = 0.f,
                                float frameBlue = 0.f);

			/**	Assigns a rendering color to the frame  (alpha is set to 1)
			 *	@param	red		red component of the rendering color
			 *	@param	green	green component of the rendering color
			 *	@param	blue	blue component of the rendering color
			 */
			void setFrameColor(unsigned char red, unsigned char green, unsigned char blue);


			/**	Assigns a rendering color to the frame (alpha is set to 1)
			 *	@param	theColor	rgb components of the rendering color
			 */
			void setFrameColor(const float* theColor);


			/**	Assigns a rendering color to the frame (alpha is set to 1)
			 *	@param	theColor	rgb components of the rendering color
			 */
			void setFrameColor(const unsigned char* theColor);


            /**
             *  Indicates whether this image's bounding rectangle should be
             *  indicated by a color frame.
             *
             *  An image can be rendered with a colored surrounding frame.  This
             *  function allows set on or off the drawing of the frame (off by default).
             *
             *  @see    setFrameColor
             *  @see    mustDrawFrame_
             *
             *  @param  drawFrame   true if a frame should be drawn to indicate the bounding
             *                      rectangle
             */
            void setDrawFrame(bool drawFrame);

            /**
             *  Indicates whether this image's valid data rectangle should be
             *  indicated by a color frame
             *
             *  An image can be rendered with a colored surrounding frame.  This
             *  function allows set on or off the drawing of the frame (off by default).
             *
             *  @see    setDrawFrame
             *  @see    mustDrawValidDataFrame_
             *
             *  @param  drawFrame   true if a frame should be drawn to indicate the valid
             *                      data rectangle
             */
            void setDrawValidDataFrame(bool drawFrame);

            /**
             *  Sets a uniform scale at which this object should be rendered.
             *
             *  An image can be rendered at different scales (smaller if the
             *  scale is less than 1, larger is the scale is greater than 1). This
             *  function specifies the same scale for the horizontal and vertical
             *  directions (uniform scaling).
             *
             *  @param  scale    scale for the horizontal and vertical directions
             */
            virtual void setRenderingScale(float scale) const;

            /**
             *  Sets horizontal and vertical scales at which this object should be rendered.
             *
             *  An image can be rendered at different scales (smaller if the
             *  scale is less than 1, larger is the scale is greater than 1). This
             *  function specifies separate scales for the horizontal and vertical
             *  directions.
             *
             *  @param  scaleX    scale for the horizontal direction
             *  @param  scaleY    scale for the vertical direction
             */
            virtual void setRenderingScale(float scaleX, float scaleY) const;

            /**
             *  Returns the horizontal rendering scale set for this object.
             *
             *  An image can be rendered at different scales (smaller if the
             *  scale is less than 1, larger is the scale is greater than 1). This
             *  function returns the scale set for the horizontal direction.
             *
             *  @return  horizontal rendering scale set for this image
             */
            float getRenderingScaleX(void) const;

            /**
             *  Returns the horizontal rendering scale set for this object.
             *
             *  An image can be rendered at different scales (smaller if the
             *  scale is less than 1, larger is the scale is greater than 1). This
             *  function returns the scale set for the vertical direction.
             *
             *  @return  vertical rendering scale set for this image
             */
            float getRenderingScaleY(void) const;

            /**
             *  Informs the class of the dimensions of the default graphic context
             *  used for rendering.  An individual VectorField can have its own
             *  dimensions assigned by calling setWindowDimensions.
             *  These dimensions are only needed if the user wants to display the
             *  VectorField "shifted" (boolean true passed to render).  If only the
             *  0-based rendering is used (default call to render), then this information
             *  is not needed.
             *  @param  theWidth    width of the window where rendering takes place
             *  @param  theHeight   height of the window where rendering takes place
             *
             *  @see    defaultWindowWidth_
             *  @see    defaultWindowHeight_
             *  @see    setWindowDimensions
             *  @see    hasIndivWindowDimensions_
             */
            static void setDefaultWindowDimensions(int theWidth, int theHeight);

            /**
             *  Informs the vector field object of the dimensions of graphic context
             *  used for its rendering.  These dimensions override the default settings
             *  for the class.
             *  These dimensions are only needed if the user wants to display the
             *  VectorField "shifted" (boolean true passed to render).  If only the
             *  0-based rendering is used (default call to render), then this information
             *  is not needed.
             *  @param  theWidth    width of the window where rendering takes place
             *  @param  theHeight   height of the window where rendering takes place
             *
             *  @see    defaultWindowWidth_
             *  @see    defaultWindowHeight_
             *  @see    setWindowDimensions
             *  @see    hasIndivWindowDimensions_
             *  @see    setDefaultWindowDimensions
             */
            virtual void setWindowDimensions(int theWidth, int theHeight);

			/**	Gives the window width for this object
			 *	@return		the window width for this object
			 */
			int getWindowWidth(void) const;

			/**	Gives the window height for this object
			 *	@return		the window height for this object
			 */
			int getWindowHeight(void) const;

			/**	Indicates whether this object has individual window dimensions (or uses the
			 *	default class values
			 *	@return		true if this object has individual window dimensions
			 */
			bool hasIndividualWindowDimensions(void) const;

            /** Returns the type of information stored in this object
             *
             *  @return     type of information stored in this object
             */
            DrawableInformationType getDrawableInformationType(void) const;
			
			
			/**	Defines the type of information stored in this object (dense, sparse, interpoalted, etc.)
			 *	@param	type	type of information	in this object
			 */
			void setDrawableInformationType(DrawableInformationType type);


		protected:

            /**
             *  This constructor simply sets all instance variables to 0, NULL, or the
             *  appropriate default value.
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
			 *	@param	drawableInfoType	what kind of information is this object made of
             *
             */
            DrawableObject2D(DrawableInformationType drawableInfoType = kUndetermined);


            /**
             *  This constructor also sets the bounding rectangle of the object.
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
			 *	@param	rect				the bounding rectangle for this object (will also be the
			 *									valid data rect in "unsafe" mode
			 *	@param	drawableInfoType	what kind of information is this object made of
             */
            DrawableObject2D(const ImageRect& rect, DrawableInformationType drawableInfoType = kUndetermined);


            /**
             *  This constructor also sets the bounding rectangle of the object.
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
			 *	@param	nbRows				number of rows of the object to create
			 *	@param	nbCols				number of columns of the object to create
			 *	@param	drawableInfoType	what kind of information is this object made of
             */
            DrawableObject2D(int nbRows, int nbCols, DrawableInformationType drawableInfoType = kUndetermined);


            /**
             *  Copy constructor.
             *  Creates a clone of the DrawableObject2D object passed as parameter.
             *  @param  thePt       reference to the DrawableObject2D object to copy
             */
            DrawableObject2D(const DrawableObject2D& thePt);


			/**	Allows a child class whose content has been changed to force recomputation of
			 *	the centroid
			 *	@param	isUpToDate	is centroid up to date?
			 */
			void setCentroidIsUpToDate_(bool isUpToDate) const;

			/**	Allows a child class whose content has been changed to force recomputation of
			 *	the center of mass
			 *	@param	isUpToDate	is center of mass up to date?
			 */
			void setCenterOfMassIsUpToDate_(bool isUpToDate) const;

			/**	Allows a child class whose content has been changed to force recomputation of
			 *	the bounding rectangle
			 *	@param	isUpToDate	is bounding rectangle up to date?
			 */
			void setBoundingRectIsUpToDate_(bool isUpToDate) const;

			/**	Allows a child class whose content has been changed to force recomputation of
			 *	the valid rectangle
			 *	@param	isUpToDate	is valid rectangle up to date?
			 */
			void setValidRectIsUpToDate_(bool isUpToDate) const;


        private:
		
            /**
             *  Bounding rectangle for this drawable object.
             *
             *  This rectangle defines the dimensions of the space actually allocated
             *  to store this raster image.  The upper left corner of the rectangle
             *  is not necessarily at (0,0).
             *
             *  @see    validRect_
             *  @see    getBoundRect
             *  @see    getValidRect
             *  @see    getShiftedRaster2D
             */
            mutable ImageRect boundRect_;

            /**
             *  Bounding rectangle of the valid data region for this drawable object.
             *
             *  This rectangle defines the subregion of the bounding rectangle (boundRect_)
             *  that contains valid data.  For example, when a gaussian filter is applied
             *  to a raster image, the processed image's valid data rectangle is smaller
             *  than that of the input image (we don't use padding for filtering operations).
             *  Rather than reallocating an image at the smaller dimensions, we simply
             *  update the validaRect_ rectangle.
             *
             *  @see    boundRect_
             *  @see    getBoundRect
             *  @see    getValidRect
             *  @see    getShiftedRaster2D
             */
            mutable ImageRect validRect_;


            /** The contour's centroid
             *  @see centroidIsUpToDate_
             */
            mutable ImagePoint_F centroid_;

            /** The contour's ccenter of mass
             *  @see centerOfMassIsUpToDate
             */
            mutable ImagePoint_F centerOfMass_;


            /** Indicates whether the bonding box stored is valid
             *
             *  This boolean is modified by the calculation of the bounding box (true),
             *  the addition of a new segment to the contour force, and some operations
             *  that modifes the contour force (e.g. erosion).
             */
            mutable bool  boundingRectIsUpToDate_;

            /** Indicates whether the bonding box stored is valid
             *
             *  This boolean is modified by the calculation of the bounding box (true),
             *  the addition of a new segment to the contour force, and some operations
             *  that modifes the contour force (e.g. erosion).
             */
            mutable bool  validRectIsUpToDate_;

            /** Indicates whether the centroid coordinates stored are valid
             *
             *  This boolean is modified by the calculation of the centroid (true),
             *  the addition of a new segment to the contour force, or any other operation
             *  that modifes the contour force (e.g. fill inner holes).
             */
            mutable bool centroidIsUpToDate_;
            
		
            /** Indicates whether the centroid coordinates stored are valid
             *
             *  This boolean is modified by the calculation of the centroid (true),
             *  the addition of a new segment to the contour force, or any other operation
             *  that modifes the contour force (e.g. fill inner holes).
             */
            mutable bool  centerOfMassIsUpToDate_;
            
            /**
             *  Horizontal scale at which this image is to be rendered.
             */
            mutable float scaleX_;

            /**
             *  Vertical scale at which this image is to be rendered.
             */
            mutable float scaleY_;

            /**
             *  True when the image should be rendered at a scale other than 1.
             *
             *  @see scaleX_
             *  @see scaleY_
             */
            mutable bool isRenderedScaled_;

			/**	x coordinate of the object's upper-left corner when rendered scaled
			 */
			mutable int scaledLeft_;
			
			/**	y coordinate of the object's upper-left corner when rendered scaled
			 */
			mutable int	scaledTop_;
			
			/**	x coordinate of the object's lower-right corner when rendered scaled
			 */
			mutable int scaledRight_;
			

			/**	y coordinate of the object's lower-right corner when rendered scaled
			 */
			mutable int scaledBottom_;

			/**	flag used to minimize the number of times the scaledXXX_ variables are recomputed
			 *	prior to a render
			 */
			mutable bool dimensionsHaveChangedSinceLastRender_;
			

            /** Stores the type of information (as opposed to "data type" stored in the
             *  drawable object.
             *  @see    DrawableInformationType
             */
            DrawableInformationType drawableInfoType_;

            /** Individual rendering window width for this object.
             *  Proper rendering (scaled in particular) requires knowledge of the dimensions
             *  of the rendering context.
             */
            int   windowWidth_;

            /** Individual rendering window height for this object.
             *  Proper rendering (scaled in particular) requires knowledge of the dimensions
             *  of the rendering context.
             */
            int   windowHeight_;

            /** Indicate whether individual rendering dimensions have been defined for
             *  this object
             */
            bool    hasIndivWindowDimensions_;


            /**
             *  Depth at which this object is to be rendered.  Since the image is rendered
             *  into a window by use of the OpenGL function glDrawPixels, under orthographic
             *  projection, we must set a depth for this rendering.  This way, we will be
             *  able to overlay vector graphics on the image (e.g. edge information,
             *  intensity gradient, contours, etc.)
             *
             *  @see    render
             *  @see    setRenderDepth
             */
            float	renderZ_;

            /**
             *  Color used for drawing a frame for the image when it is rendered.  This
             *  color is only used if mustDrawFrame_ is set to true.
             *
             *  @see setFrameColor
             *  @see setDrawFrame
             *  @see mustDrawFrame_
             */
            GLfloat*	frameColor_;

            /**
             *  True when a frame should be drawn around the image when it is rendered.
             *  This frame has the color stored in frameColor_ or black if frameColor_
             *  is null.
             *
             *  @see frameColor_
             *  @see mustDrawValidDataFrame_
             *  @see
             */
            bool    mustDrawFrame_;

            /** The object's rendering color
             */
            float color_[4];

            /**
             *  True when a frame should be drawn around the image's valid data rectangle
             *  when it is rendered.
             *  This frame has the inverse of the color stored in frameColor_ or white if
             *  frameColor_
             *  is null.
             *
             *  @see frameColor_
             */
            bool    mustDrawValidDataFrame_;


            /** Default rendering window height for all drawable objects.
             *  Proper rendering (scaled in particular) requires knowledge of the dimensions
             *  of the rendering context.  It is possible to assigne each DrawableObject2D
             *  individual rendering dimensions, otherwise this default height is used.
             */
            static int defaultWindowWidth_;

            /** Default rendering window height for all drowable objects.
             *  Proper rendering (scaled in particular) requires knowledge of the dimensions
             *  of the rendering context.  It is possible to assigne each DrawableObject2D
             *  individual rendering dimensions, otherwise this default height is used.
             */
            static int defaultWindowHeight_;


			/**	flag used to minimize the number of times the scaledXXX_ variables are recomputed
			 *	prior to a render
			 */
			static bool defaultDimensionsHaveChangedSinceLastRender_;
			

            /**
             *  Equal operator.
             *  Created so that gcc will stop complaining.
             *  @param  thePt       reference to the DrawableObject2D object to copy
             */
            const DrawableObject2D& operator = (const DrawableObject2D& thePt);

            /**
             *  Sets a new bounding rectangle for the drawable object.
             *
             *  A drawable object occupies a rectangle within the global image space.
             *  For example, a drawable object could be defined as a subimage of another
             *  image.  This operation may require a reallocation of the image.
             *
             *  This function must be overridden by child classes
             *
             *  @param  x       X coordinate of the image's origin
             *  @param  y       Y coordinate of the image's origin
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            virtual void setBoundRect_(int x, int y, int theWidth, int theHeight) = 0;


            /**
             *  Sets a new valid data rectangle for the drawable object.
             *
             *  A raster image or vector field occupies a rectangle within the global
             *  image space.
             *  For example, a vector field could be defined as a subpart of another.
             *  Only part of the image may contain valid data (for example after application
             *  of a filter).
             *  The function call will fail if the new valid data rect is not contained
             *  by the vector field's bounding rectangle
             *
             *  Must be overridden by child classes that don't need to maintain their valid
             *  data rectangle at all times (e.g. Blob, Contour)
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            virtual void setValidRect_(int x, int y, int theWidth, int theHeight) = 0;

            /** Calculates the object's bounding rect
			 *	@param	rect	the bounding box to update
             *  @see boundingRectIsUpToDate_
             *  @see boundRect_
             */
            virtual void computeBoundingRect_(ImageRect& rect) const = 0;


            /** Calculates the object's valid box
			 *	@param	rect	the bounding box to update
             *  @see boundingRectIsUpToDate_
             *  @see boundRect_
             */
            virtual void computeValidRect_(ImageRect& rect) const = 0;


           /** Computes the contour's centroid.
			 *	@param	cent	the centroid point to update
             */
			virtual void computeCentroid_(ImagePoint_F& cent) const = 0;

            /** Computes the object's center of mass by calculating the  
             *  average point
			 *	@param	cent	the center of mass point to update
             */
            virtual void computeCenterOfMass_(ImagePoint_F& cent) const = 0;


			/**	Computes updated values for the object's corners when rendered scaled
			 *	@param	theLeft		unscaled x coordinate of the object's upper-left corner when rendered scaled
			 *	@param	theTop		unscaled  y coordinate of the object's upper-left corner when rendered scaled
			 *	@param	theRight	unscaled  x coordinate of the object's lower-right corner when rendered scaled
			 *	@param	theBottom	unscaled  y coordinate of the object's lower-right corner when rendered scaled
             *  @param  shifted     true if the image should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @param  origin      location of the object's upper-left corner in the rendering
             *  @param  scaledLeft		computed x coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledTop		computed y coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledRight		computed x coordinate of the object's lower-right corner when rendered scaled
			 *	@param	scaledBottom	computed y coordinate of the object's lower-right corner when rendered scaled
			 */
			void computeScaledCorners_(int theLeft, int theTop, int theRight, int theBottom,
											 bool shifted, const ImagePoint* origin,
											 int& scaledLeft, int& scaledTop, 
											 int& scaledRight, int& scaledBottom) const;


            /**
             *  Renders the drawable object at the depth and location specified.
             *
             *  A drawable object can be rendered with its valid data rectangle's upper-left
             *  corner in the upper-left corner of the rendering window (shifted=false and
             *  origin=NULL), or using the absolute coordinates of its valid data rectangle's
             *  upper-left corner (shited=true and origin=NULL), or by setting its upper-left
             *  corner at the location specified (origin != NULL).
			 *
			 *	All render functions are declared to be const.  The basic idea behind that is that
			 *	rendering just gives a snapshot of the current state of the object but should not 
			 *	really change the object.  According to this view, anything that needs to be 
			 *	changed/updated at render time is really mutable.
             *
             *  This function must be overridden by child classes.
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
            virtual void render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
								 float theZ, bool shifted, const ImagePoint* origin) const = 0;

    };
	
	
	#if 0
	#pragma mark -
	#pragma mark Free functions drawing simple shapes
	#endif
	
	/**	Draws the contour of a rectangle specified by the coordinates of it upper-left corner,
	 *	width, and height.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 *
	 *	@param	x		x coordinate of the rectangle's upper-left corner
	 *	@param	y		y coordinate of the rectangle's upper-left corner
	 *	@param	width	width of the rectangle
	 *	@param	height	height of the rectangle
	 */
	void drawRect(float x, float y, float width, float height);
	

	/**	Fills a rectangle specified by the coordinates of it upper-left corner,
	 *	width, and height.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 *
	 *	@param	x		x coordinate of the rectangle's upper-left corner
	 *	@param	y		y coordinate of the rectangle's upper-left corner
	 *	@param	width	width of the rectangle
	 *	@param	height	height of the rectangle
	 */
	void fillRect(float x, float y, float width, float height);
	

	/**	Draws the contour of a rectangle specified by the coordinates of it center,
	 *	width, and height, and by a rotation angle (in degree).
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 *
	 *	@param	cx		x coordinate of the rectangle's center
	 *	@param	cy		y coordinate of the rectangle's center
	 *	@param	width	width of the rectangle
	 *	@param	height	height of the rectangle
	 *	@param	angle	orientation of the rectangle
	 */
	void drawCenteredRect(float cx, float cy, float width, float height, float angle=0.f);
	
	
	/**	Fills a rectangle specified by the coordinates of it center,
	 *	width, and height, and by a rotation angle (in degree).
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 *
	 *	@param	cx		x coordinate of the rectangle's center
	 *	@param	cy		y coordinate of the rectangle's center
	 *	@param	width	width of the rectangle
	 *	@param	height	height of the rectangle
	 *	@param	angle	orientation of the rectangle
	 */
	void fillCenteredRect(float cx, float cy, float width, float height, float angle=0.f);
	
	
	/**	Draws the contour of an ellipse specified by the coordinates of it upper-left corner,
	 *	and the width and height of its bounding rectangle (major and minor diameters of the ellipse).
	 *	We simply use the name "oval" for similaroty to the Java API.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 *
	 *	@param	x		x coordinate of the ellipse's bounding rectangle's upper-left corner
	 *	@param	y		y coordinate of the ellipse's bounding rectangle's upper-left corner
	 *	@param	width	the ellipse's major diameter
	 *	@param	height	the ellipse's minor diameter
	 */
	void drawOval(float x, float y, float width, float height);
	

	/**	Fills an ellipse specified by the coordinates of it upper-left corner,
	 *	and the width and height of its bounding rectangle (major and minor diameters of the ellipse).
	 *	We simply use the name "oval" for similaroty to the Java API.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 *
	 *	@param	x		x coordinate of the ellipse's bounding rectangle's upper-left corner
	 *	@param	y		y coordinate of the ellipse's bounding rectangle's upper-left corner
	 *	@param	width	the ellipse's major diameter
	 *	@param	height	the ellipse's minor diameter
	 */
	void fillOval(float x, float y, float width, float height);
	

	/**	Draws an ellipse specified by the coordinates of it center,
	 *	and the width and height of its bounding rectangle (major and minor diameters of the ellipse).
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 *
	 *	@param	cx		x coordinate of the ellipse's center
	 *	@param	cy		y coordinate of the ellipse's center
	 *	@param	width	the ellipse's major diameter
	 *	@param	height	the ellipse's minor diameter
	 *	@param	angle	orientation of the ellipse
	 */
	void drawCenteredOval(float cx, float cy, float width, float height, float angle=0.f);
	
	
	/**	Fills an ellipse specified by the coordinates of it center,
	 *	and the width and height of its bounding rectangle (major and minor diameters of the ellipse).
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 *
	 *	@param	cx		x coordinate of the ellipse's center
	 *	@param	cy		y coordinate of the ellipse's center
	 *	@param	width	the ellipse's major diameter
	 *	@param	height	the ellipse's minor diameter
	 *	@param	angle	orientation of the ellipse
	 */
	void fillCenteredOval(float cx, float cy, float width, float height, float angle=0.f);


	/**	Draws a unit circular "dot" at the current location.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 */
	void drawCircularDot(void);
	
	
	/**	Fills a unit circular "dot" at the current location.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 */
	void fillCircularDot(void);

	/**	Draws a unit square "dot" at the current location.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 */
	void drawSquareDot(void);
	
	
	/**	Fills a unit square "dot" at the current location.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 */
	void fillSquareDot(void);

	
	/**	Draws a unit diamond "dot" at the current location.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 */
	void drawDiamondDot(void);
	
	
	/**	Fills a unit diamond "dot" at the current location.
	 *	Like all the free functions defined here
	 *	<ul>
	 *		<li>this makes no pretention of being super-efficient.
	 *	It simply provides a simple way to draw a basic shape.  In any case, the main purpose of this 
	 *	library is image processing, not rendering.  The latter is generally only performed for
	 *	debugging/validation purposes.</li>
	 *		<li> It is up to the user to set the color <b>before</b> making the call to this function</li>
	 *	</ul>
	 */
	void fillDiamondDot(void);

}

#endif  //  DRAWABLE_OBJECT_2D_H
