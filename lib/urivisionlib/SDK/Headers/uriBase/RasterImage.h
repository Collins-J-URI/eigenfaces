/*  NAME:
        RasterImage.h

    DESCRIPTION:
        RasterImage public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RASTER_IMAGE_H
#define     URIVL_RASTER_IMAGE_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageRect.h"
#include "DrawableObject2D.h"
#include "GraphWorld.h"

namespace uriVL
{

    /** Enumerated type of modes for color to gray conversion
     *
     *  Enumerated type of modes for color to gray conversion.  Currently three
     *  conversion modes are supported:
     *
     *      - kMaxFieldConversion:          gray = max(red, green, blue)
     *      - kAverageConversion:           gray = (red + green + blue) / 3
     *      - kWeightedAverageConversion:   gray = (wr*red + wg*green + wb*blue)
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    typedef enum    ConversionToGrayMode {
                                            kMaxFieldConversion = 0,
                                            kAverageConversion,
                                            kWeightedAverageConversion
    } ConversionToGrayMode;


    /** Enumerated type of modes for float to unsigned char gray conversion
     *
     *  Enumerated type of modes for float to unsigned char conversion.
     *  Currently five conversion modes are supported:
     *
     *      - kPositiveFloat:     [0, MAX] --> [0, 255]
     *      - kSignedFloat:       [MIN, MAX] --> [-MAX', MAX'] --> [0, 255], 0 at 128
     *      - kRangeFloat:        [MIN, MAX] --> [0, 255]
     *      - kSaturatedPositive  v < 0 set to 0, v > 255 set to 255, v otherwise
     *      - kSaturatedSigned    0 at 128, v < -128 set to 0, v > 127 set to 127,
     *                            v otherwise
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    typedef enum ConversionFromFloatMode {
                                            kUndefFloatConv = -1,
                                            kSignedFloat = 0,
                                            kPositiveFloat,
                                            kRangeFloat,
                                            kSaturatedPositive,
                                            kSaturatedSigned

    } ConversionFromFloatMode;


    /** Virtual base image class.
     *
     *  RasterImage is the base class for all image types.  This class is build around
     *  a QuickTime GWorld buffer, which is itself wrapped into a GraphWorld object.
     *
     *  Each RasterImage object has a base raster type (8 bit gray, 16 bit argb color,
     *  24 bit rgb color, and 32 bit rgba color).  The GraphWorld object corresponding to
     *  this base type is not always allocated.  It only needs to be so when the raster
     *  must be displayed.
     *
     *  The RasterImage class contains a number of virtual methods that must be implemented
     *  by the subclasses.
     *
     *  The base subclasses of RasterImage correspond to the base raster types.  They are:
     *      - RasterImage_gray for 8-bit gray level images
     *      - RasterImage_rgb16 for 16-bit [1 alpha, 5 red, 5 green, 5 blue] color images
     *      - RasterImage_RGB for 24-bit [8 red, 8 green, 8 blue] color images
     *      - RasterImage_RGBa for 32-bit [8 alpha, 8 red, 8 green, 8 blue] color images
     *  These base subclasses can be instanciated and are the ones most commonly used for
     *  image processing and computer vision purposes.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RasterImage : public DrawableObject2D
    {
        public:
            /**
             *  Destructor
             */
            virtual ~RasterImage(void);

            /**
             *  Returns the base raster as a read-only 1D array.
             *
             *  This method is implemented by the base subclasses of RasterImage,
             *  and should be overridden by these classes' own subclasses.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @return     1D base raster for this image
             */
            virtual const unsigned char* getRaster(void) const;

            /**
             *  Returns the base raster as a read-write 1D array.
             *
             *  This method is implemented by the base subclasses of RasterImage,
             *  and should be overridden by these classes' own subclasses.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D base raster for this image
             */
            virtual unsigned char* getRaster(ReadWriteStatus rw);

            /**
             *  Returns the base raster as a read-only 2D array.
             *
             *  This method is implemented by the base subclasses of RasterImage,
             *  and should be overridden by these classes' own subclasses.  The 2D base
             *  raster array is an array of rows that point to parts of the base 1D
             *  raster.  If the base raster has not already been allocated, then it should
             *  be allocated and initialized using the reference information for the image
             *  (for example, HSV data should be converted into RGBA data in the 1D base
             *  raster).
             *
             *  @return     2D base raster for this image
             */
            virtual const unsigned char*const* getRaster2D(void) const;

            /**
             *  Returns the base raster as a read-write 2D array.
             *
             *  This method is implemented by the base subclasses of RasterImage,
             *  and should be overridden by these classes' own subclasses.  The 2D base
             *  raster array is an array of rows that point to parts of the base 1D
             *  raster.  If the base raster has not already been allocated, then it should
             *  be allocated and initialized using the reference information for the image
             *  (for example, HSV data should be converted into RGBA data in the 1D base
             *  raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     2D base raster for this image
             */
            virtual unsigned char*const* getRaster2D(ReadWriteStatus rw);

            /**
             *  Returns the shifted base raster as a read-only 2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  This method simply calls getShiftedRGB2D().
             *
             *  @return     shifted 2D base raster for this image
             */
            virtual const unsigned char*const* getShiftedRaster2D(void) const;

            /**
             *  Returns the shifted base raster as a read-write 2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  This method simply calls getShiftedRGB2D().
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     shifted 2D base raster for this image
             */
            virtual unsigned char*const* getShiftedRaster2D(ReadWriteStatus rw);

            /**
             *  Copies this image into the one received as parameter.
             *
             *  Copies (rather than cloning) this image into the destination image received
             *  as parameter.  Depending on the types of this image and of the destination
             *  image, the copy may require some data conversion (for example, HSV to Lab).
             *  If the raster(s) of the destination image does not have the same dimensions
             *  as the source, then the destination image's raster are re-allocated.
             *  This function must be overridden (and called by the overriding function)
             *  by subclasses of RasterImage.
             *
             *  @param      imgOut  pointer to the destination image
             */
            virtual void copyInto(RasterImage* imgOut) const = 0;

            /**
             *  Copies this image into the one received as parameter.
             *
             *  Copies (rather than clones) this image into the destination image received
             *  as parameter.  Depending on the types of this image and of the destination
             *  image, the copy may require some data conversion (for example, HSV to Lab).
             *  If the raster(s) of the destination image does not have the same dimensions
             *  as the source, then the destination image's raster are re-allocated.
             *  This function must be overridden (and called by the overriding function)
             *  by subclasses of RasterImage.
             *
             *  @param      imgOut  pointer to the destination image
             *  @param      theRect region of the image to copy
             */
            virtual void copyInto(RasterImage* imgOut, const ImageRect* theRect) const = 0;

            /**
             *  Copies this image into the one received as parameter.
             *
             *  Copies (rather than clones) this image into the destination image received
             *  as parameter.  Depending on the types of this image and of the destination
             *  image, the copy may require some data conversion (for example, HSV to Lab).
             *  If the raster(s) of the destination image does not have the same dimensions
             *  as the source, then the destination image's raster are re-allocated.
             *  This function must be overridden (and called by the overriding function)
             *  by subclasses of RasterImage.
             *
             *  @param      imgOut  pointer to the destination image
             *  @param      theRect region of the image to copy
             *  @param  origin  the point to use as origin for this image's valid data rectangle's
             *                  upper-left corner.
             */
            virtual void copyInto(RasterImage* imgOut, const ImageRect* theRect, 
                                  const ImagePoint* origin) const = 0;


            /**	Writes another image over part of this image. Contrary to <code>copyInto</code>
             *  this function does not resize the destination image if it is smaller than the
             *  source.  It does not even modify the valid data rectangle of the destination
             *  image.  It just writes in the intersection of the images' valid data rectangles.

             *  @param  imgIn   the image to write over (part of) this image
             */
            void overlay(const RasterImage* imgIn);


            /**	Writes another image over part of this image, using the point specified as origin
             *  for this image's valid rectangle's upper-left corner.
             *  Contrary to <code>copyInto</code> this function does not resize the
             *  destination image if it is smaller than the source.  It does not even modify
             *  the valid data rectangle of the destination image.  It just writes in the
             *  intersection of the destination rectangle and of this image's valid data
             *  rectangle.  Fails if the destination rectangle is not contained by the
             *  source image.
             *
             *  @param  imgIn   the image to write into
             *  @param  origin  the point to use as origin for this image's valid data rectangle's
             *                  upper-left corner.
             */
            void overlay(const RasterImage* imgIn, const ImagePoint* origin);


            /**	Writes over this image the part of another image specified by a rectangle.
             *  Contrary to <code>copyInto</code> this function does not resize the
             *  destination image if it is smaller than the source.  It does not even modify
             *  the valid data rectangle of the destination image.  It just writes in the
             *  intersection of the destination rectangle and of this image's valid data
             *  rectangle.  Fails if the destination rectangle is not contained by the
             *  source image.
             *
             *  @param  imgIn   the image to write into
             *  @param  rect    specifies the part of the source to write into the destination
             */
            void overlay(const RasterImage* imgIn, const ImageRect* rect);

            /**	Writes over this image the part of another image specified by a rectangle,
             *  using the point specified as origin for this image's valid rectangle's
             *  upper-left corner.
             *  Contrary to <code>copyInto</code> this function does not resize the
             *  destination image if it is smaller than the source.  It does not even modify
             *  the valid data rectangle of the destination image.  It just writes in the
             *  intersection of the destination rectangle and of this image's valid data
             *  rectangle.  Fails if the destination rectangle is not contained by the
             *  source image.
             *
             *  @param  imgIn   the image to write into
             *  @param  rect    specifies the part of the source to write into the destination
             *  @param  origin  the point to use as origin for this image's valid data rectangle's
             *                  upper-left corner.
             */
            void overlay(const RasterImage* imgIn, const ImageRect* rect, const ImagePoint* origin);

            /**	Writes this image into another image. Contrary to <code>copyInto</code>
             *  this function does not resize the destination image if it is smaller than the
             *  source.  It does not even modify the valid data rectangle of the destination
             *  image.  It just writes in the intersection of the images' valid data rectangles.

             *  @param  imgOut  the image to write into
             */
            void writeInto(RasterImage* imgOut) const;


           /**	Writes into another image the part of this image specified by a rectangle.
            *   Contrary to <code>copyInto</code> this function does not resize the
            *   destination image if it is smaller than the source.  It does not even modify
            *   the valid data rectangle of the destination image.  It just writes in the
            *   intersection of the destination rectangle and of this image's valid data
            *   rectangle.  Fails if the destination rectangle is not contained by the
            *   source image.
            *
            *   @param  imgOut  the image to write into
            *   @param  rect    specifies the part of the source to write into the destination
            */
            void writeInto(RasterImage* imgOut, const ImageRect* rect) const;


            /**	Writes this image into another image using the point specified as origin for
             *  this image's valid rectangle's upper-left corner. Contrary to <code>copyInto</code>
             *  this function does not resize the destination image if it is smaller than the
             *  source.  It does not even modify the valid data rectangle of the destination
             *  image.  It just writes in the intersection of the images' valid data rectangles.
             *
             *  @param  imgOut  the image to write into
             *  @param  origin  the point to use as origin for this image's valid data rectangle's
             *                  upper-left corner.
             */
            void writeInto(RasterImage* imgOut, const ImagePoint* origin) const;


           /**	Writes into another image the part of this image specified by a rectangle, using
            *   the point specified as origin for this image's valid rectangle's upper-left corner.
            *   Contrary to <code>copyInto</code> this function does not resize the
            *   destination image if it is smaller than the source.  It does not even modify
            *   the valid data rectangle of the destination image.  It just writes in the
            *   intersection of the destination rectangle and of this image's valid data
            *   rectangle.  Fails if the destination rectangle is not contained by the
            *   source image.
            *
            *   @param  imgOut  the image to write into
            *   @param  rect    specifies the part of the source to write into the destination
            *   @param  origin  the point to use as origin for this image's valid data rectangle's
            *                   upper-left corner.
            */
            void writeInto(RasterImage* imgOut, const ImageRect* rect, const ImagePoint* origin) const;


           /**	Writes into a new raster image the part of this image specified by a
            *   rectangle. Returns <code>NULL</code> if the destination rectangle is not
            *   contained by the source image.
            *
            *   @param  rect    specifies the part of the source to grab from
            *   @return the part of this image specified by rect
            */
            RasterImage* grab(const ImageRect* rect) const;


           /**	Writes into a new raster image the part of this image specified by a
            *   rectangle, and at the location specified by the image point. Returns
            *   <code>NULL</code> if the destination rectangle is not
            *   contained by the source image.  If the origin point is <code>NULL</code>,
            *   just uses the origin of the grabbing rectangle.
            *
            *   @param  rect    specifies the part of the source to grab from
            *   @param  origin  the point to use as origin for the new image's valid data
            *                       rectangle's upper-left corner.
            *   @return the part of this image specified by rect
            */
            RasterImage* grab(const ImageRect* rect, const ImagePoint* origin) const;


           /**	Writes into a new raster image the part of the input image specified by a
            *   rectangle. Returns <code>null</code> if the destination rectangle is not
            *   contained by the source image.
            *
            *   @param  imgIn  the image to grab from
            *   @param  rect    specifies the part of the source to write into the destination
            *   @return the part of the input image specified by rect
            */
            static RasterImage* grab(const RasterImage* imgIn, const ImageRect* rect);


           /**	Writes into a new raster image the part of the input image specified by a
            *   rectangle. Returns <code>null</code> if the destination rectangle is not
            *   contained by the source image.
            *
            *   @param  imgIn  the image to grab from
            *   @param  rect    specifies the part of the source to write into the destination
            *   @param  origin  the point to use as origin for the new image's valid data
            *                       rectangle's upper-left corner.
            *   @return the part of the input image specified by rect
            */
            static RasterImage* grab(const RasterImage* imgIn, const ImageRect* rect, const ImagePoint* origin);


           /**	Creates a new raster image that is a copy of this image.
		    *	Note that this function only copies the valid data of this image.
            *   This function is just a simplified version of the grab function, which it calls.
            *
            *   @return a copy of this image
             *  @see    grab
            */
            RasterImage* createCopy(void) const;


           /**	Creates a new raster image that is a copy of the image received as parameter.
		    *	Note that this function only copies the valid data of the input image.
            *   This function is just a simplified version of the grab function, which it calls.
            *
            *
            *   @param  imgIn  the image to copy
            *   @return a pointer to a new copy of img
             *  @see    grab
            */
            static RasterImage* createCopy(const RasterImage* imgIn);


            /**
             *  Returns the image's base raster type.
             *
             *  Returns one of the following values:
             *
             *      - kGrayRaster, for any image based on the 8-bit gray level format
             *      - kaRGB16Raster, for any image based on the 16-bit argb color format
             *      - kRGB24Raster, for any image based on the 24-bit rgb color format
             *      - kaRGB32Raster, for any image based on the 32-bit argb color format
             *      - kRGBa32Raster, for any image based on the 32-bit rgba color format
             *
             *  This method is virtual and should be implemented by the base raster
             *  subclasses
             *
             *  @return     the image's base raster type
             */
            virtual BaseRasterType getBaseRasterType(void) const = 0;

            /**
             *  Returns the number of bytes on each row of the image.
             *
             *  Beware:     Base rasters have <b>padded</b> rows.  You cannot rely on the
             *              fact that the number of bytes in a row is equal to the number of
             *              columns multiplied by by the number of bytes per pixel.
             *
             *  @return     number of bytes on a row of the image
             */
            int getBytesPerRow(void) const;

            /**
             *  Returns the number of total number of bytes in the image.
             *
             *  Beware:     Base rasters have <b>padded</b> rows.  You cannot rely on the
             *              fact that the number of bytes in a row is equal to the number of
             *              columns multiplied by by the number of bytes per pixel.
             *
             *  @return     number of bytes in the image
             */
            long getTotalByteCount(void) const;

            /**
             *  Returns the number of bytes per pixel (pixel depth)
             *
             *  @return     pixel depth
             */
            int getPixelDepth(void) const;


            /** Get the time stamp of this image.  The time stamp only really makes sense when the 
             *  image is coming from a movie file or frame grabber.
             *  @return  the time stamp of this image
             *  @see    setTimeStamp
             */
            long getTimeStamp(void) const;

            /** Sets the time stamp of this image.  The time stamp only really makes sense when the 
             *  image is coming from a movie file or frame grabber.
             *  @param  timeStamp   the new time stamp of this image
             *  @see    getTimeStamp
             */
            void setTimeStamp(long timeStamp);

			/**	Gives this image's unique id
			 *	@rteturn	this image's id
			 */
			unsigned int getID(void);
			
            /**
             *  Forces a refresh of nonreference raster according to the reference raster.
             *
             *  A RasterImage object can be store more than one raster.  For example, a
             *  RasterImage_HSV will store hue, saturation, and value rasters, and possibly
             *  an RGBa raster as well.  One of the rasters (HSV or RGBa) must be
             *  designated as the <i>reference raster</i>.  It is possible that one of
             *  the image's rasters be modified by some image processing operation.
             *  Whenever the user requests a pointer to a non-reference raster,
             *  we must verify if this raster must be updated according to the values
             *  of the reference raster.
             */
            void requestRasterUpdate(void) const;

            /** Locks the geometry and pixel depth of the image.  The object that calls this
             *  function is meant to pass its own adress as parameter so that it can unlock the
             *  image later.
             *  @param  key address/key that must be passed later to unlock the image
             */
            void lockProperties(void *key);

            /** Unlocks the geometry and pixel depth of the image.  Only the address/key that 
             *  was used to lock the image can unlock it.
             *  @param  key address/key that earlier locked the image
             */
            void unlockProperties(void *key);

			/**	Indicates whether the RasterImage object's properties are locked
			 *	@return		true if the RasterImage object's properties are locked
			 */
			bool propertiesAreLocked(void) const;
			
            /** Locks the pixel map of the image so that calls to the read-write version of
             *  getRaster will fail until the image is unlocked.  The object that calls this
             *  function is meant to pass its own adress as parameter so that it can unlock the
             *  image later.
             *  @param  key address/key that must be passed later to unlock the image
             */
            void lockPixels(void *key);

            /** Unlocks the pixel map of the image.  Only the address/key that 
             *  was used to lock the image can unlock it.
             *  @param  key address/key that earlier locked the image
             */
            void unlockPixels(void *key);

			/**	Indicates whether the RasterImage object's pixmap is locked
			 *	@return		true if the RasterImage object's pixmap is locked
			 */
			bool pixelsAreLocked(void) const;
			

            /**
             *  Sets this RasterImage as destination for global blitting/drawing.
             *
             *  QuickDraw (and therefore QuickTime) draws into whatever was defined as the
             *  "current GWorld".  This member function defines its RasterImage's base raster
             *  as the destination for such operations.
             *
             *  This function keeps a copy of the previous GraphWorld, so that it can restore
             *  it later on.
             *
             *  @see   restoreGraphWorld
             */
           void setAsCurrentGraphWorld(void);


            /**
             *  Sets this RasterImage as destination for movie playing
             *
             *  When a movie is played or live video is grabbed, a GWorld must be set as the
             *  destination for the video stream.  This member function allows to define the
             *  RasterImage's base raster as the destination for the video stream, so as to
             *  avoid unnecessary blitting between buffers.
             *
             *  This function keeps a copy of the previous GraphWorld, so that it can restore
             *  it later on.
             *  @param  theMovie the movie that will play into this object
             *
             *  @see   restoreGraphWorld
             */
            void setAsMovieGraphWorld(Movie theMovie);

//            /**
//             *  Sets this RasterImage as destination for a video capture.
//             *
//             *  When a movie is played or live video is grabbed, a GWorld must be set as the
//             *  destination for the video stream.  This member function allows to define the
//             *  RasterImage's base raster as the destination for the video stream, so as to
//             *  avoid unnecessary blitting between buffers.
//             *
//             *  This function keeps a copy of the previous GraphWorld, so that it can restore
//             *  it later on.
//             *  @param  grabber the FrameGrabber that will play into this object
//             *  @return	error code (0 if all OK)
//             *  @see   restoreGraphWorld
//             */
////            ErrorCode setAsGrabberGraphWorld(FrameGrabber *grabber);

            /**
             *  Sets this RasterImage as destination for a graphic importer.
             *
             *  When data is read from an image file, a GWorld must be set as the
             *  destination for the graphic importer.  This member function allows to 
             *  define the RasterImage's base raster as the destination for the importer,
             *  so as to avoid unnecessary blitting between buffers.
             *
             *  This function keeps a copy of the previous GraphWorld, so that it can restore
             *  it later on.
             *
             *  @param  importer the graphic importer that will draw into this object
             *  @see   restoreGraphWorld
             */
            void setAsImporterGraphWorld(GraphicsImportComponent importer);


//#if URIVL_USE_QUICKTIME
//            /** Adds this image to the video digitizer record of a buffer ring
//             *  @param  bufferRec   buffer record to add information to
//             */
////            void addToBufferRing(VdigBufferRec *bufferRec);
//#endif

            /**
             *  Restores the previous "current GraphWorld".
             *
             *  QuickDraw (and therefore QuickTime) draws into whatever was defined as the
             *  "current GWorld."   This class function resores the "current GWorld" to its
             *  previous value".
             *
             *  @see   setAsCurrentGraphWorld
             *  @see   setAsMovieGraphWorld
             */
            void restoreGraphWorld(void);

            /** Indicates whether or not the raster stored in the <code>GraphWorld</code>
             *  (if it exists) contains the reference values for this image.  Could be false
             *  for a raster image that stores its own raster outside of the
             *  <code>GraphWorld<code>, eg
             *  <code>RasterImage_HSV</code> or <code>RasterImage_gray_F</code>
             *
             *  @return     true if the raster stored in the <code>GraphWorld</code> stores
             *              the reference
             *
             *  @see    floatRasterIsReference
             */
            bool graphWorldIsReference(void) const;

            /** Indicates whether or not the reference raster for this image has been modified.
             *
             *  When the reference raster is modified, other rasters stored by the image object
             *  my have to be modified.  For example, for a RasterImage_HSV image, if the HSV rasters
             *  are the reference and are modified, the RGBa raster would have to be updated prior to
             *  rendering the image.
             *
             *  @return     true if the reference raster has changed.
             *
             *  @see    referenceHasChanged_
             */
            bool referenceHasChanged(void) const;

			
            /** Indicates whether or not the raster image stores a float raster (eg this would
             *  be true for a <code>Raster_Image_gray_F</code> object)
             *
             *  @return     true if the raster images stores a float raster
             *
             *  @see    floatRasterIsReference
             */
            bool hasFloatRaster(void) const;

            /** Indicates whether or not the float raster (for a raster that does have
             *  a float raster, eg <code>Raster_Image_gray_F</code> stores the reference
             *  values for the raster image
             *
             *  @return     true if the float raster stores the reference values
             *
             *  @see    hasFloatRaster
             */
            bool floatRasterIsReference(void) const;


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


			/**	Returns the to-gray conversion coefficients for this image.  For some unknown reason
			 *	(must have made sense at the time) the conversion coefficients as instance
			 *	variables for a particular image are gone (along with companion access functions).
			 *	I Still implement this finction as instance in case these members make it back
			 *	@return		array of coefficients of the conversion to gray
			 */
			const float* getConversionWeights(void) const;

			/**	Returns the default to-gray conversion coefficients for this image. 
			 *	@return		array of default coefficients of the conversion to gray
			 */
			static const float* getDefaultConversionWeights(void);

            /** Sets the default mode for color to gray conversion for the entire RasterImage
             *  class.
             *  @param  theMode     new default conversion mode for the class
             */
            static void setDefaultGrayConversionMode(const ConversionToGrayMode theMode);

            /** When the default gray conversion mode for the class is
             *  <code>kWeightedAverageConversion</code>, this function allows to set the
             *  weights of the three color components.  The function uses the relative values of
             *  these weights and sets normalized [0..1] weights.
             *
             *  @param  redW    weight of the red component
             *  @param  greenW  weight of the green component
             *  @param  blueW   weight of the blue component
             */
            static void setDefaultConversionWeights(const float redW, const float greenW, const float blueW);


		protected:
			
			/**	Called by a derived class to indicate whether one of its instances
			 *	has a float raster (false by default)
			 *	@param hasRaster	true if this RasterImage has a float raster
			 */
			void setHasFloatRaster_(bool hasRaster);
			

			/**	Called by a derived class to indicate whether one of its instances
			 *	has another (other than float) raster, e.g. binary or HSV (false by default)
			 *	@param hasRaster	true if this RasterImage has another raster
			 */
			void setHasOtherRaster_(bool hasRaster);
			

            /** Sets this image's float raster as the reference (fails if it was NULL)
             */
            void setFloatRasterAsReference_(void) const;


            /** Sets this image's gworld as the reference (and allocates if it was NULL)
             *
             *  Should be overridden by subclasses that allocate and maintain their own
             *  raster(s), e.g. <code>RasterImage_HSV</code>
			 *	@see graphWorldIsReference
             */
            void setGraphWorldAsReference_(void) const;

			/**	Called by a subclass that provides another type of raster (for example,
			 *	RasterImage_binary), when that other raster becomes the reference.
			 */
			void setOtherRasterAsReference_(void) const;
			

			/**	To be called by a child class that has modified the graphworld or one
			 *	of the secondary rasters, or updated a secondary raster
			 *	@param	hasChanged	new value to assign to referenceHasChanged_
			 */
			void setReferenceHasChanged_(bool hasChanged) const;
			
			/**	Indicates whether the GraphWorld was allocated for this object
			 *	@return		true if the GraphWorld for this object is null
			 */
			bool graphWorldIsNull_(void) const;

			/**	Alocates the GraphWorld for this object at the set dimensions
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
			 */
			virtual void allocateGraphWorld_(int nbRows, int nbCols) = 0;


        private:

			/**	Unique index of this RasterImage object
			 */
			unsigned int id_;
			
            /**
             *  Stores the base raster for this image.  Not necessarily allocated.
             */
            GraphWorld * gworld_;

            /**
             *  True when the base raster stored in the GraphWorld object is the reference
             *  raster for this image.  This is not always the case.  For example, color
             *  image processing may affect the HSV fields of a RasterImage_HSV, and these
             *  changes will only be applied to the base RGBa raster if requestRasterUpdate
             *  is called.
             */
            mutable bool    gworldIsReference_;

            /**
             *  True when the RasterImage stores a float raster.
             */
            bool    hasFloatRaster_;

            /**
             *  True when the RasterImage stores another raster (other than a float raster),
			 *	e.g. HSV raster or binary raster
             */
            bool    hasOtherRaster_;

            /**
             *  True when the RasterImage stores a float raster and that raster is the reference.
             */
            mutable bool    floatRasterIsReference_;

            /**
             *  True when the base raster stored in the GraphWorld object the reference
             *  raster for this image.  This is not always the case.  For example, color
             *  image processing may affect the HSV fields of a RasterImage_HSV, and these
             *  changes will only be applied to the base RGBa raster if requestRasterUpdate
             *  is called.a frame should be drawn around the images, when it is rendered.
             *  This frame has the color stored in frameColor_ or black if frameColor_
             *  is null.
             */
            mutable bool    referenceHasChanged_;

             /** Time stamp for this image.  The time stamp only really makes sense when the 
             *  image is coming from a movie file or frame grabber.
             *  @see    setTimeStamp
             *  @see    getTimeStamp
             */
            long timeStamp_;
            
           /** True when the image's dimensions and pixel depth are locked
             */
            bool propertiesAreLocked_;
            
            /** True when the image's pixel map is locked
             */
            bool pixmapIsLocked_;
            
            /** Key for the image's dimensions and pixel depth
             */
            void* propertiesKey_;
            
            /** Key for the image's pixel map
             */
            void* pixmapKey_;
            
            
 			/**	Counter for RasterImage objects created by the application
			 */
			static unsigned int imgCount_;
			

            /**
             *  Code for the default algorithm for conversion to gray.  An image instance
             *  can override this default mode by setting its grayConv_ field to an other
             *  value.
             *
             *  @see    hasIndivGrayConvMode_
             */
            static ConversionToGrayMode defaultGrayConv_;

            /**
             *  Weights used for the red, green, and blue channels if the default conversion
             *  algorithm for this image is kWeightedAverageConversion.
             *
             *  @see    defaultGrayConv_
             */
            static float    defaultConvWeight_[3];


            /**
             *  Default constructor.
             *
             *  This constructor simply sets all instance variables to 0, NULL, or the
             *  appropriate default value.
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
             *
             */
            RasterImage(void);

            /**
             *  Allocates a raster image at the specified dimensions.
             *
             *  Initializes some instance variables (in particular the ImageRect).
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
             *
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
             */
            RasterImage(int nbRows, int nbCols);

            /**
             *  Allocates a raster image at the specified dimensions.
             *
             *  Initializes some instance variables (in particular the ImageRect).
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
             *
             *  @param      theRect   image rectangle indicating the desired dimensions
             */
            RasterImage(const ImageRect* theRect);

            /**
             *  Allocates a raster image at the specified dimensions.
             *
             *  Initializes some instance variables (in particular the ImageRect).
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
             *
             *  @param      theRect   image rectangle indicating the desired dimensions
             */
            RasterImage(const ImageRect& theRect);

            /** Creates a new raster image based on the GraphWorld received as parameter.
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
             *
             *  The GraphWorld received as parameter is cloned into the new image and
             *  gworldIsReference_ is set to true (can be overridden by a subclass).
             *
             *  @see    gworldIsReference_
             *
             *  @param  gworld   pointer to the GraphWorld to clone
             */
            RasterImage(const GraphWorld* gworld);


            /** Creates a new raster image based on the GraphWorld received as parameter.
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
             *
             *  The GraphWorld received as parameter is <b>kept</b> rather than being
             *  cloned if the boolean parameter is true.
             *  gworldIsReference_ is set to true (can be overridden by a subclass).
             *
             *  @see    gworldIsReference_
             *
             *  @param  gworld   pointer to the GraphWorld to clone or keep
             *  @param  keepThisGWorld   true if the GraphWorld should be kept rather
             *                          than cloned
             */
            RasterImage(GraphWorld* gworld, bool keepThisGWorld);


            /** Creates a new raster image based on the one received as parameter.
             *
             *  This constructor should only be invoked from one of the base raster subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa)
             *
             *  <ul>
             *      <li> if that image did not not have its GraphWorld allocated or if the
             *          GraphWorld was not the reference, then only copy image bounds info
             *      <li> if that image had a valid GraphWorld as its reference, then clone
             *          that GraphWorld for the new image
             *  </ul>
			 *	Note that it is not possible w/o massive recoding and headaches to offer a version
			 *	of this constructor that gharantees the constness of the parameter.  If the actual
			 *	types of the two images do not match, then a conversion must take place, and either
			 *	we have to code all possible combinations of such conversions or we do it through
			 *	the GWorld, in which case the GWorld of the source may have to be updated, hence
			 *	non-constness.  Making the GWorld mutable has a cascading effect (after all, this is the
			 *	central data structure for this class), which amounts to making "const" meaningless.
             *
             *  @param  img   pointer to the RasterImage object to copy
             */
            RasterImage(const RasterImage* img);

            /** Creates a new raster image based on the one received as parameter.
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  img   the RasterImage object to copy
             */
            RasterImage(const RasterImage& img);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  img   pointer to the RasterImage object to copy
             */
            const RasterImage& operator =(const RasterImage& img);


            /**
             *  Copies this image into the one received as parameter.
             *
             *  Copies (rather than clones) this image into the destination image received
             *  as parameter.  Depending on the types of this image and of the destination
             *  image, the copy may require some data conversion (for example, HSV to Lab).
             *  If the raster(s) of the destination image does not have the same dimensions
             *  as the source, then the destination image's raster are re-allocated.
             *  This function must be overridden (and called by the overriding function)
             *  by subclasses of VectorField.
             *
             *  @param      imgOut  pointer to the destination image
             *  @param      theRect region of the image to copy
             *  @param      origin  the point to use as origin for this image's valid data
             *                  rectangle's upper-left corner
             */
            virtual void copyInto_(RasterImage* imgOut, const ImageRect* theRect, const ImagePoint* origin) const;


            /**	Writes into another image the part of this image specified by a rectangle, using
             *   the point specified as origin for this image's valid rectangle's upper-left corner.
             *   Contrary to <code>copyInto</code> this function does not resize the
             *   destination image if it is smaller than the source.  It does not even modify
             *   the valid data rectangle of the destination image.  It just writes in the
             *   intersection of the destination rectangle and of this image's valid data
             *   rectangle.  Fails if the destination rectangle is not contained by the
             *   source image.
             *
             *   @param  imgOut  the image to write into
             *   @param  rect    specifies the part of the source to write into the destination
             *   @param  origin  the point to use as origin for this image's valid data rectangle's
             *                   upper-left corner
             */
            virtual void writeInto_(RasterImage* imgOut, const ImageRect* rect, const ImagePoint* origin) const = 0;


            /**
             *  Sets a new valid data rectangle for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a raster image could be defined as a subpart of another.
             *  Only part of the image may contain valid data (for example after application
             *  of a filter).
             *  The function call will fail if the new valid data rect is not contained
             *  by the vector field's bounding rectangle
             *
             *  May be overridden by child classes that have other rasters than their base raster.
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            virtual void setValidRect_(int x, int y, int theWidth, int theHeight);


            /**
             *  Renders the RasterImage's base raster at the depth and location specified.
             *
             *  Renders the RasterImage's base raster, using OpenGL's glDrawPixels function.
             *  The rendering is done at depth specified.  The latter should be chosen so
             *  that the image lies within the frustum and vector graphics can be overlaid.
             *  The rendering is done at the location specified.  This means that the image's
             *  bounding rectangle's upper left corner will appear at the location specified
             *  by the point received as parameter.
             *
             *  @param  scaledLeft  x coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledTop	y coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledRight x coordinate of the object's lower-right corner when rendered scaled
			 *	@param	scaledBottom y coordinate of the object's lower-right corner when rendered scaled
             *  @param  theZ    Depth at which this object is to be rendered.
             *  @param  shifted     true if the image should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @param  origin      location of the object's upper-left corner in the rendering
             */
            virtual void render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
								 float theZ, bool shifted, const ImagePoint* origin) const;


            /**
             *  Updates the image's base raster (if it is not the reference).
             *
             *  This method is virtual and is implemented by the base subclasses
             *  (RasterImage_gray, RasterImage_rgb16, RasterImage_RGB, RasterImage_RGBa).
             *  Updates the image's base raster stored in the GraphWorld of this RasterImage
             *  object, if the base raster was not the reference raster.  If the base
             *  raster had not been allocated it will be allocated and initialized.
             */
            virtual void updateRaster_(void) const = 0;


            /**
             *  Allocates the 2D  base raster.
             *  This pure virtual function must be implemented by child classes.
             *  @param  nbRows   number of rows of the image
             *  @param  nbCols		number of columns of the image
             */
            virtual void allocateRaster2D_(int nbRows, int nbCols) = 0;

            /**
             *  Deletes the 0-based 2D base raster.
             *  This virtual function must be implemented by child classes.
             */
            virtual void deleteRaster2D_(void) = 0;


            /**
             *  Allocates the 2D  shifted base raster.
             *  This pure virtual function must be implemented by child classes.
             *  @param  nbRows		number of rows of the image
             *  @param  nbCols		number of columns of the image
             *  @param  iLow       Y coordinate of the image's origin
             *  @param  jLow       X coordinate of the image's origin
             */
            virtual void allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow) = 0;


            /**
             *  Deletes the 2D  shifted base raster.
             *  @param  iLow       Y coordinate of the image's origin
             *  This pure virtual function must be implemented by child classes.
             */
            virtual void deleteShiftedRaster2D_(int iLow) = 0;


            /** Calculates the object's bounding rect
			 *	@param	rect	the bounding box to update
             */
            virtual void computeBoundingRect_(ImageRect& rect) const;


            /** Calculates the object's valid box
			 *	@param	rect	the bounding box to update
             */
            virtual void computeValidRect_(ImageRect& rect) const;


            /** Computes the contour's centroid.
			 *	@param	cent	the centroid point to update
             */
			virtual void computeCentroid_(ImagePoint_F& cent) const;

            /** Computes the object's center of mass by calculating the  
             *  average point
			 *	@param	cent	the center of mass point to update
             */
            virtual void computeCenterOfMass_(ImagePoint_F& cent) const;



//trying to get rid of all these friends
		friend class DrawableObject2D;

		friend class RasterImage_binary;
		friend class RasterImage_gray;
		friend class RasterImage_gray_F;
		friend class RasterImage_RGB;
		friend class RasterImage_RGBa;
		friend class RasterImage_RGBa_F;
		friend class RasterImage_HSV;
		friend class RasterImage_aRGB16;
		
		friend class Pattern2D;

		friend class VideoSource;
		
		#if URIVL_USE_QUICKTIME
			friend class _FrameGrabber_QT;
		#else
			friend class _FrameGrabber_libvidcap;
		#endif
			
    };
}

#endif
