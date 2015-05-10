/*  NAME:
        RasterImage_RGBa.h

    DESCRIPTION:
        RasterImage_RGBa public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RASTER_IMAGE_RGBA_H
#define     URIVL_RASTER_IMAGE_RGBA_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage.h"

namespace uriVL
{
    /** Base 32-bit rgba color image class.
     *
     *  RasterImage_RGBa is the base class for all 32-bit rgba color image types.  This class is built
     *  around a 32-bit color QuickTime GWorld buffer, which is itself wrapped into a
     *  GraphWorld object.
     *
     *  Since 24-bit color support in QuickTime is weak (only works for the
     *  Windows platform, and should not even do so, according to specifications), developers are
     *  encouraged to use RasterImage_RGBa rather than RasterImage_RGBa as the base class for image
     *  classes implementing other color spaces (HSL, HSV, Lab, etc.)
     *
     *  RasterImage_RGBa implements or overrides a number of methods of the base RasterImageClass,
     *  and in particular the convertTo<Gray,RGB,RGBa> methods.
     *
     *  <b>Important:</b>  All subclasses of RasterImage_RGBa <b>must</b> override the following
     *  methods:
     *      - getRGB, which resturns the 1D base raster (8-bit gray level)
     *      - getRGB2D, which resturns the 2D base raster (8-bit gray level).
     *  These methods must be overloaded to force a conversion from the reference type to the
     *  base raster type (and possibly an allocation of the base raster) if the base raster is not
     *  the reference.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RasterImage_RGBa : public RasterImage
    {
        public:

            /**
             *  Initializes an rgb raster image at the specified dimensions.
             *
             *  Allocates a 32-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      nbRows     number of rows of the rgba raster image
             *  @param      nbCols     number of columns of the rgba raster image
             */
            RasterImage_RGBa(int nbRows, int nbCols);

            /**
             *  Allocates a 32-bit RGBa color raster image at the specified dimensions.
             *
             *  Allocates a 32-bit RGBa color raster at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             */
            RasterImage_RGBa(const ImageRect* theRect);

            /**
             *  Initializes an rgb raster image at the specified dimensions and values.
             *
             *  Allocates a 32-bit color GraphWorld at the specified dimensions and
             *  initializes it with the contents of the raster received as parameter.
             *
             *  @param      nbRows     number of rows of the rgba raster image
             *  @param      nbCols     number of columns of the rgba raster image
             *  @param      theRGBa    values to which the image should be initialized
             */
            RasterImage_RGBa(int nbRows, int nbCols, const unsigned char* theRGBa);

            /**
             *  Initializes an rgb raster image at the specified dimensions and values.
             *
             *  Allocates a 32-bit RGBa color raster at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             *  @param      theRGBa    values to which the image should be initialized
             */
            RasterImage_RGBa(const ImageRect* theRect, const unsigned char* theRGBa);

            /**
             *  Initializes an rgb raster image at the specified dimensions and values.
             *
             *  Allocates a 32-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the GraphWorld received
             *  as parameter.
             *
             *  @param      gworld    the GraphWorld to use for dimensions and values
             *  @param      keepThisGraphWorld  if true, then this new image raster object will
             *                  simply store a reference to a pre-allocated GWorld instead of
             *                  allocating its own (beware!)
             */
            RasterImage_RGBa(GraphWorld* gworld, bool keepThisGraphWorld=false);

            /**
             *  Initializes an rgba raster image at the specified dimensions and values.
             *
             *  Allocates a 32-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img    the RasterImage to use for dimensions and values
             */
            RasterImage_RGBa(const RasterImage* img);

            /**
             *  Initializes an rgba raster image at the specified dimensions and values.
             *
             *  Allocates a 32-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img     the RasterImage to use for dimensions and values
             *  @param      theRect ImageRect specifying the image's bounding rectangle
             */
            RasterImage_RGBa(const RasterImage* img, const ImageRect* theRect);

            /**
             *  Class destructor.
             *
             *  Standard destructor (nothing fancy).
             *
             */
            virtual ~RasterImage_RGBa(void);

            /**
             *  Returns the image's base raster type.
             *
             *  Returns the following value:
             *
             *      - kRGBa32Raster, since this image is based on the 32-bit rgba color format
             *
             *  @return     the image's base raster type
             */
            BaseRasterType getBaseRasterType(void) const;

            /**
             *  Copies this image into the one received as parameter.
             *
             *  Copies (rather than clones) this image into the destination image received
             *  as parameter.  Depending on the types of this image and of the destination
             *  image, the copy may require some data conversion (for example, HSV to Lab).
             *  If the raster(s) of the destination image does not have the same dimensions
             *  as the source, then the destination image's raster are re-allocated.
             *
             *  @param      imgOut  pointer to the destination image
             */
            void copyInto(RasterImage* imgOut) const;


            /**
             *  Copies this image into the one received as parameter.
             *
             *  Copies (rather than clones) this image into the destination image received
             *  as parameter.  Depending on the types of this image and of the destination
             *  image, the copy may require some data conversion (for example, HSV to Lab).
             *
             *  @param      imgOut      pointer to the destination image
             *  @param      theRect region of the image to copy
             */
            void copyInto(RasterImage* imgOut, const ImageRect* theRect) const;


            /**
             *  Copies this image into the one received as parameter.
             *
             *  Copies (rather than clones) this image into the destination image received
             *  as parameter.  Depending on the types of this image and of the destination
             *  image, the copy may require some data conversion (for example, HSV to Lab).
             *  If the raster(s) of the destination image does not have the same dimensions
             *  as the source, then the destination image's raster are re-allocated.
             *
             *  @param      imgOut  pointer to the destination image
             *  @param      theRect region of the image to copy
             *  @param  origin  the point to use as origin for this image's valid data rectangle's
             *                  upper-left corner.
             */
            void copyInto(RasterImage* imgOut, const ImageRect* theRect, const ImagePoint* origin) const;


            /**
             *  Returns the 32-bit rgba base raster as a read-only 1D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_RGBa.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @return     1D base raster for this image
             */
            virtual const unsigned char* getRGBa(void) const;


            /**
             *  Returns the 32-bit rgba base raster as a read-write 1D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_RGBa.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D base raster for this image
             */
            virtual unsigned char* getRGBa(ReadWriteStatus rw);


            /**
             *  Returns the 0-based 32-bit rgba base raster as a read-only 2D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_RGBa.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @return     2D base raster for this image
             */
            virtual const unsigned char* const* getRGBa2D(void) const;


            /**
             *  Returns the 0-based 32-bit rgba base raster as a read-write 2D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_RGBa.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     2D base raster for this image
             */
            virtual unsigned char* const* getRGBa2D(ReadWriteStatus rw);


            /**
             *  Returns the shifted 32-bit rgba base raster as a read-only 2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_RGBa.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @return     shifted 2D base raster for this image
             */
            virtual const unsigned char* const* getShiftedRGBa2D(void) const;


            /**
             *  Returns the shifted 32-bit rgba base raster as a read-write 2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_RGBa.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     shifted 2D base raster for this image
             */
            virtual unsigned char* const* getShiftedRGBa2D(ReadWriteStatus rw);


            /**
             *  Converts a 1D RGBa32 raster into a 1D gray-level raster
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             *  @param      myGrayConv      the gray conversion mode desired
             */
            static void convertToGray(const unsigned char* theSource,
                                      unsigned char* theDestination,
                                      long nbPixels,
                                      ConversionToGrayMode myGrayConv = kAverageConversion);

            /**
             *  Converts a 1D RGBa32 raster into a 1D gray-level raster
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             *  @param      convWeight      normalized weights to apply for the conversion
             */
            static void convertToGray(const unsigned char* theSource,
                                      unsigned char* theDestination,
                                      long nbPixels,
                                      const float convWeight[]);

            /**
             *  Converts a 1D RGBa32 raster into a 1D float gray-level raster
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             *  @param      myGrayConv      the gray conversion mode desired
             */
            static void convertToGray(const unsigned char* theSource,
                                      float* theDestination,
                                      long nbPixels,
                                      ConversionToGrayMode myGrayConv = kAverageConversion);

            /**
             *  Converts a 1D RGBa32 raster into a 1D float gray-level raster
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             *  @param      convWeight      normalized weights to apply for the conversion
             */
            static void convertToGray(const unsigned char* theSource,
                                      float* theDestination,
                                      long nbPixels,
                                      const float convWeight[]);

            /**
             *  Copies a 1D RGBa32 raster into a 1D 32-bit rgba raster.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToRGBa(const unsigned char* theSource,
                                      unsigned char* theDestination,
                                      long nbPixels);

            /**
             *  Converts a 1D RGBa32 raster into a 1D 24-bit rgb raster.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToRGB(const unsigned char* theSource,
                                     unsigned char* theDestination,
                                     long nbPixels);


            /**
             *  Converts a 1D RGBa32 raster into a 1D 16-bit aRGB16 raster.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToARGB16(const unsigned char* theSource,
                                      unsigned char* theDestination,
                                      long nbPixels);

            /** Allocates a read-only RGBa32 copy of the image received as parameter if that
             *  image is not already some form of RGBa raster.
             *  This function should be used as follows:
             *  <code>bool  localRGBa;<br>
             *  const RasterImage_RGBa  *imgRGBa = RasterImage_RGBa::localRasterRGBa(imgIn, &localRGBa);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localRGBa)<br>
             *  delete imgRGBa;</code>
             *
             *  @param  img        the image of which to make a local 32-bit RGBa copy
             *  @param  localRGBa  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return		pointer to read-only RGBa image
             */
            static const RasterImage_RGBa* localRasterRGBa(const RasterImage* imgIn, bool* localRGBa);

            /** Allocates an RGBa32 copy of the image received as parameter if that
             *  image is not already some form of RGBa raster.
             *  This function should be used as follows:
             *  <code>bool  localRGBa;<br>
             *  const RasterImage_RGBa  *imgRGBa = RasterImage_RGBa::localRasterRGBa(imgIn, &localRGBa);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localRGBa)<br>
             *  delete imgRGBa;</code>
             *
             *  @param  img        the image of which to make a local 32-bit RGBa copy
             *  @param  rect       the rectangle over which we want a gray-level image
             *  @param  localRGBa  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return		pointer to read-only RGBa image
             */
            static const RasterImage_RGBa* localRasterRGBa(const RasterImage* imgIn, const ImageRect* rect,
                                                      bool* localRGBa);

            /** Allocates an RGBa32 copy of the image received as parameter if that
             *  image is not already some form of RGBa raster.
             *  This function should be used as follows:
             *  <code>bool  localRGBa;<br>
             *  RasterImage_RGBa  *imgRGBa = RasterImage_RGBa::localRasterRGBa(imgIn, &localRGBa);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localRGBa)<br>
             *  delete imgRGBa;</code>
             *
             *  @param  img        the image of which to make a local 32-bit RGBa copy
             *  @param  localRGBa  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
             */
            static RasterImage_RGBa* localRasterRGBa(RasterImage* imgIn, bool* localRGBa);

            /** Allocates an RGBa32 copy of the image received as parameter if that
             *  image is not already some form of RGBa raster.
             *  This function should be used as follows:
             *  <code>bool  localRGBa;<br>
             *  RasterImage_RGBa  *imgRGBa = RasterImage_RGBa::localRasterRGBa(imgIn, &localRGBa);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localRGBa)<br>
             *  delete imgRGBa;</code>
             *
             *  @param  img        the image of which to make a local 32-bit RGBa copy
             *  @param  rect       the rectangle over which we want a gray-level image
             *  @param  localRGBa  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
             */
            static RasterImage_RGBa* localRasterRGBa(RasterImage* imgIn, const ImageRect* rect,
                                                      bool* localRGBa);


		protected:
		
            /**
             *  Protected default constructor.
             *
             *  Called by subclasses that don't need to allocate the base raster.
             */
            RasterImage_RGBa(void);


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
            virtual void writeInto_(RasterImage* imgOut, const ImageRect* rect, 
                                    const ImagePoint* origin) const;


            /**
             *  Sets a new bounding rectangle for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation may require a reallocation of the image.
             *
             *  This function may be called or overridden by child classes that have their
             *  own raster (eg.  RGBa_F, HSV, etc.)
             *
             *  @param  x           horizontal coordinate of upper left corner
             *  @param  y           vertical coordinate of upper left corner
             *  @param  theWidth    desired width of the image
             *  @param  theHeight   desired height of the image
             */
            virtual void setBoundRect_(int x, int y, int theWidth, int theHeight);

		
			/**	Alocates the GraphWorld for this object at the set dimensions
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
			 */
			void allocateGraphWorld_(int nbRows, int nbCols);

            /**
             *  Allocates the 2D gray-level base raster.
             *  @param  nbCols		number of columns of the image
             *  @param  nbRows   number of rows of the image
             */
            void allocateRaster2D_(int nbRows, int nbCols);

            /**
             *  Allocates the 2D shifted base raster.
             *  @param  nbRows		number of rows of the image
             *  @param  nbCols		number of columns of the image
             *  @param  iLow       Y coordinate of the image's origin
             *  @param  jLow       X coordinate of the image's origin
             */
            void allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow);


		
        private:

            /**
             *  0-based 2D array mapped on the GraphWorld's 1D raster.
             */
            unsigned char** rgba2D_;

            /**
             *  "shifted" 2D array (based on the image's rectangle bounds) mapped on the GraphWorld's
             *  1D raster.
             */
            unsigned char** shiftedRgba2D_;

            /** Creates a new raster image based on the one received as parameter.
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the pervious one (constness).
             *
             *  @param  img   pointer to the RasterImage_RGBa object to copy
             */
            RasterImage_RGBa(RasterImage_RGBa& img);
			
			/**
             *  Equal operator.
             *  Created so that gcc will stop complaining. This RasterImage_RGBa object will
			 *	retain its own id.
             *  @param  theImg       reference to the RasterImage_RGBa object to copy
             */
            const RasterImage_RGBa& operator = (const RasterImage_RGBa& theImg);

            /**
             *  Forces an update of the base rgba raster if it is not the reference raster.
             *
             *  This virtual function should be overridden by all subclasses of RasterImage_RGBa.
             */
            virtual void updateRaster_(void) const;

			/**
             *  Deletes the 0-based 2D base raster.
             */
            void deleteRaster2D_(void);

            /**
             *  Deletes the 2D shifted base raster.
			 *	@param	iLow	start index for rows
             */
            void deleteShiftedRaster2D_(int iLow);

            #if !URIVL_USE_QUICKTIME
                friend class _GraphWorld_gen;
            #endif
            
    };
}

#endif  //  RASTER_IMAGE_RGBA_H
