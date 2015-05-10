/*  NAME:
        RasterImage_RGB.h

    DESCRIPTION:
        RasterImage_RGB public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RASTER_IMAGE_RGB_H
#define     URIVL_RASTER_IMAGE_RGB_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage.h"

namespace uriVL
{
    /** Base rgb color image class.
     *
     *  RasterImage_RGB is the base class for all gray-level image types.  This class is built
     *  around an 8-bit gray-level QuickTime GWorld buffer, which is itself wrapped into a
     *  GraphWorld object.
     *
     *  RasterImage_RGB implements or overrides a number of methods of the base RasterImageClass,
     *  and in particular the convertTo<Gray,RGB,RGBa> methods.
     *
     *  Note to class developers: Do <b>not</b> subclass RasterImage_RGB but rather
     *  RasterImage_RGBa.  Support for RasterImage_RGB is now minimal.  Images of this
     *  type should not be relied upon for more than reading and displaying.  The reason
     *  for this is that QuickTime does not officially support 24-bits buffers.  So,
     *  some function work todays but may break down with a future update of QT.  Right
     *  now fixing the implementation of 24-bits images is a very low priority for me.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RasterImage_RGB : public RasterImage
    {
        public:
            /**
             *  Initializes an rgb raster image at the specified dimensions.
             *
             *  Allocates a 24-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      nbRows     number of rows of the rgb raster image
             *  @param      nbCols     number of columns of the rgb raster image
             */
            RasterImage_RGB(int nbRows, int nbCols);

            /**
             *  Allocates a 24-bit RGB color raster image at the specified dimensions.
             *
             *  Allocates a 24-bit RGB color raster at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             */
            RasterImage_RGB(const ImageRect* theRect);

            /**
             *  Initializes an rgb raster image at the specified dimensions and values.
             *
             *  Allocates a 24-bit color GraphWorld at the specified dimensions and
             *  initializes it using the raster received as parameter.
             *
             *  @param      nbRows     number of rows of the rgb raster image
             *  @param      nbCols     number of columns of the rgb raster image
             *  @param      theRGBA    values to which the image should be initialized
             */
            RasterImage_RGB(int nbRows, int nbCols, const unsigned char* theRGB);

            /**
             *  Initializes an rgb raster image at the specified dimensions and values.
             *
             *  Allocates a 24-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the GraphWorld received
             *  as parameter.
             *
             *  @param      gworld    the GraphWorld to use for dimensions and values
             *  @param      keepThisGraphWorld  if true, then this new image raster object will
             *                  simply store a reference to a pre-allocated GWorld instead of
             *                  allocating its own (beware!)
             */
            RasterImage_RGB(GraphWorld* gworld, bool keepThisGraphWorld);

            /**
             *  Initializes an rgb raster image at the specified dimensions and values.
             *
             *  Allocates a 24-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the GraphWorld received
             *  as parameter.
             *  Possibly keeps the GraphWorld received as parameter rather than making a copy
             *  of it.
             *
             *  @param      gworld    the GraphWorld to use for dimensions and values
             *  @param      keepThisGraphWorld  true if the pointer is to be used, false if
             *                                      data shouold be copied (clone)
             */
            RasterImage_RGB(const GraphWorld* gworld);

            /**
             *  Initializes an rgb raster image at the specified dimensions and values.
             *
             *  Allocates a 24-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img    the RasterImage to use for dimensions and values
             */
            RasterImage_RGB(const RasterImage* img);

            /**
             *  Initializes an rgb raster image at the specified dimensions and values.
             *
             *  Allocates a 24-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img     the RasterImage to use for dimensions and values
             *  @param      theRect ImageRect specifying the image's bounding rectangle
             */
            RasterImage_RGB(RasterImage* img, const ImageRect* theRect);

            /**
             *  Class destructor.
             *
             *  Standard destructor (nothing fancy).
             *
             */
            ~RasterImage_RGB(void);

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
             *  Returns the image's base raster type.
             *
             *  Returns the following value:
             *
             *      - kRGB24Raster, since this image is based on the 24-bit rgb color format
             *
             *  @return     the image's base raster type
             */
            BaseRasterType getBaseRasterType(void) const;

            /**
             *  Returns the 24-bit rgb base raster as a read-only 1D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_RGB.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGB data in the 1D base raster).
             *
             *  @return     1D base raster for this image
             */
            const unsigned char* getRGB(void) const;

            /**
             *  Returns the 24-bit rgb base raster as a read-write 1D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_RGB.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGB data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D base raster for this image
             */
            unsigned char* getRGB(ReadWriteStatus rw);

            /**
             *  Returns the 24-bit rgb base raster as a read-only 2D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGB data in the 1D base raster).
             *
             *  @return     2D base raster for this image
             */
            const unsigned char* const* getRGB2D(void) const;

            /**
             *  Returns the 24-bit rgb base raster as a read-write 2D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGB data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     2D base raster for this image
             */
            unsigned char* const* getRGB2D(ReadWriteStatus rw);

            /**
             *  Returns the shifted 24-bit rgba base raster as a constant2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @return     shifted 2D base raster for this image
             */
            const unsigned char* const* getShiftedRGB2D(void) const;

            /**
             *  Returns the shifted 24-bit rgba base raster as a read-write 2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     shifted 2D base raster for this image
             */
            unsigned char* const* getShiftedRGB2D(ReadWriteStatus rw);

            /**
             *  Converts a 1D 24-bit RGB raster into a 1D gray-level raster
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
             *  Converts a 1D 24-bit RGB raster into a 1D gray-level raster
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
             *  Converts a 1D 24-bit RGB raster into a 1D float gray-level raster
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
             *  Converts a 1D 24-bit RGB raster into a 1D float gray-level raster
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
             *  Converts a 1D 24-bit RGB raster into a 1D 32-bit rgba raster.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToRGBa(const unsigned char* theSource,
                                      unsigned char* theDestination,
                                      long nbPixels);

            /**
             *  Converts (copies) a 1D 24-bit RGB raster into a 1D 24-bit rgb raster.
             *
             *  The raster returned is not a clone of this image's base raster since it does
             *  not contain any padding of the rows.
             *
             *  Will be called by convertToRGB and copyInto.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToRGB(const unsigned char* theSource,
                                     unsigned char* theDestination,
                                     long nbPixels);

            /**
             *  Converts a 1D 24-bit RGB raster into a 1D 16-bit argb raster.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToARGB16(const unsigned char* theSource,
                                      unsigned char* theDestination,
                                      long nbPixels);


        private:

            /**
             *  2D array mapped on the GraphWorld's 1D raster.
             */
            unsigned char** rgb2D_;

            /**
             *  "shifted" 2D array (based on the image's rectangle bounds) mapped on the GraphWorld's
             *  1D raster.
             */
            unsigned char** shiftedRgb2D_;


           /**
             *  Protected default constructor.
             *
             *  Called by subclasses that don't need to allocate the base raster.
             */
            RasterImage_RGB(void);


             /** Creates a new raster image based on the one received as parameter.
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  img   pointer to the RasterImage_RGB object to copy
             */
            RasterImage_RGB(RasterImage_RGB& img);
			
			/**
             *  Equal operator.
             *  Created so that gcc will stop complaining. This RasterImage_RGB object will
			 *	retain its own id.
             *  @param  theImg       reference to the RasterImage_RGB object to copy
             */
            const RasterImage_RGB& operator = (const RasterImage_RGB& theImg);


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
             *  Sets a new Sets a new bounding rectangle for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a raster image could be defined as a subpart of another.
             *  This operation may require a reallocation of the raster image.
             *
             *  This function could be overridden by child classes
             *
             *  @param  x       X coordinate of the image's origin
             *  @param  y       Y coordinate of the image's origin
             *  @param  theWidth    desired width of the image
             *  @param  theHeight   desired height of the image
             */
            virtual void setBoundRect_(int x, int y, int theWidth, int theHeight);


            /**
             *  Forces an update of the base rgb raster if it is not the reference raster.
             *
             *  This virtual function should be overridden by all subclasses of RasterImage_RGB.
             */
            virtual void updateRaster_(void) const;

			/**	Alocates the GraphWorld for this object at the set dimensions
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
			 */
			void allocateGraphWorld_(int nbRows, int nbCols);

			/**
             *  Deletes the 0-based 2D base raster.
             */
            void deleteRaster2D_(void);

            /**
             *  Deletes the 2D shifted base raster.
			 *	@param	iLow	start index for rows
             */
            void deleteShiftedRaster2D_(int iLow);

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


             #if !URIVL_USE_QUICKTIME
                friend class _GraphWorld_gen;
            #endif
            
    };
}

#endif  //  RASTER_IMAGE_RGB_H
