/*  NAME:
        RasterImage_aRGB16.h

    DESCRIPTION:
        RasterImage_aRGB16 public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef     URIVL_RASTER_IMAGE_ARGB16_H
#define     URIVL_RASTER_IMAGE_ARGB16_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage.h"

namespace uriVL
{
    /** Base 16-bit argb color image class.
     *
     *  RasterImage_aRGB16 is the base class for all 16-bit rgba color image types.  This class is built
     *  around a 16-bit color QuickTime GWorld buffer, which is itself wrapped into a
     *  GraphWorld object.
     *
     *  This class has been primarily created for real-time color thresholding operations.
     *
     *  RasterImage_aRGB16 implements or overrides a number of methods of the base RasterImageClass,
     *  and in particular the convertTo<Gray,RGB,RGBa> methods.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RasterImage_aRGB16 : public RasterImage
    {
        public:

            /**
             *  Allocates a 16-bit aRGB color raster image at the specified dimensions.
             *
             *  @param      nbRows     number of rows of the argb16 raster image
             *  @param      nbCols     number of columns of the argb16 raster image
             */
            RasterImage_aRGB16(int nbRows, int nbCols);

            /**
             *  Allocates a 16-bit aRGB color raster image at the specified dimensions.
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             */
            RasterImage_aRGB16(const ImageRect* theRect);

           /**
             *  Allocates a 16-bit aRGB color raster image at the specified dimensions.
             *
             *  @param      nbRows     number of rows of the rgba raster image
             *  @param      nbCols     number of columns of the rgba raster image
             *  @param      theRGBa    values to which the image should be initialized
             */
            RasterImage_aRGB16(int nbRows, int nbCols, const unsigned char* theRGBa);

            /**
             *  Allocates a 16-bit aRGB color raster image at the specified dimensions.
             *
             *  Allocates a 16-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the GraphWorld received
             *  as parameter.
             *
             *  @param      gworld    the GraphWorld to use for dimensions and values
             *  @param      keepThisGraphWorld  if true, then this new image raster object will
             *                  simply store a reference to a pre-allocated GWorld instead of
             *                  allocating its own (beware!)
             */
            RasterImage_aRGB16(GraphWorld* gworld, bool keepThisGraphWorld = false);

            /**
             *  Allocates a 16-bit aRGB color raster image at the specified dimensions.
             *
             *  Allocates a 16-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img    the RasterImage to use for dimensions and values
             */
            RasterImage_aRGB16(RasterImage* img);

            /**
             *  Allocates a 16-bit aRGB color raster image at the specified dimensions.
             *
             *  Allocates a 16-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img     the RasterImage to use for dimensions and values
             *  @param      theRect ImageRect specifying the image's bounding rectangle
             */
            RasterImage_aRGB16(RasterImage* img, const ImageRect* theRect);

            /**
             *  Class destructor.
             *
             *  Standard destructor (nothing fancy).
             *
             */
            ~RasterImage_aRGB16();

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
             *  Returns the 16-bit argb base raster as a read-only 1D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_aRGB.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @return     1D base raster for this image
             */
            virtual const unsigned char* getARGB(void) const;


            /**
             *  Returns the 16-bit argb base raster as a read-write 1D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_aRGB.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D base raster for this image
             */
            virtual unsigned char* getARGB(ReadWriteStatus rw);


            /**
             *  Returns the 0-based 16-bit argb base raster as a read-only 2D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_aRGB.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @return     2D base raster for this image
             */
            virtual const unsigned char* const* getARGB2D(void) const;


            /**
             *  Returns the 0-based 16-bit argb base raster as a read-write 2D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_aRGB.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     2D base raster for this image
             */
            virtual unsigned char*const* getARGB2D(ReadWriteStatus rw);


            /**
             *  Returns the shifted 16-bit argb base raster as a read-only 2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_aRGB.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @return     shifted 2D base raster for this image
             */
            virtual const unsigned char* const* getShiftedARGB2D(void) const;


            /**
             *  Returns the shifted 16-bit argb base raster as a read-write 2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_aRGB.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into RGBA data in the 1D base raster).
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     shifted 2D base raster for this image
             */
            virtual unsigned char* const* getShiftedARGB2D(ReadWriteStatus rw);


            /**
             *  Converts this image's base raster into a 1D gray-level raster
             *
             *  The raster returned is not a clone of this image's base raster since it does
             *  not contain any padding of the rows.
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
             *  Converts this image's base raster into a 1D gray-level raster
             *
             *  The raster returned is not a clone of this image's base raster since it does
             *  not contain any padding of the rows.
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
             *  Converts this image's base raster into a 1D float gray-level raster
             *
             *  Initializes a 1D raster array to store the gray levels
             *  corresponding to this image's data.
             *  Will be called by convertToGray and copyInto.
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
             *  Converts this image's base raster into a 1D float gray-level raster
             *
             *  Initializes a 1D raster array to store the gray levels
             *  corresponding to this image's data.
             *  Will be called by convertToGray and copyInto.
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
             *  Converts this image's base raster into a 1D 32-bit rgba raster.
             *
             *  Initializes a 1D raster array to store the gray levels
             *  corresponding to this image's data.
             *  Will be called by convertToRGBa and copyInto.
             *
             *  The raster returned is not a clone of this image's base raster since it does
             *  not contain any padding of the rows.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToRGBa(const unsigned char* theSource,
                                      unsigned char* theDestination,
                                      long nbPixels);

            /**
             *  Converts this image's base raster into a 1D 24-bit rgb raster.
             *
             *  Initializes a 1D raster array to store the gray levels
             *  corresponding to this image's data.
             *  Will be called by convertToRGB and copyInto.
             *
             *  The raster returned is not a clone of this image's base raster since it does
             *  not contain any padding of the rows.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToRGB(const unsigned char* theSource,
                                     unsigned char* theDestination,
                                     long nbPixels);


            /** Allocates an aRGBa16 copy of the image received as parameter if that
             *  image is not already some form of aRGB16 raster.
             *  This function should be used as follows:
             *  <code>bool  localaRGB16;<br>
             *  localARGB16  *imgARGB16 = RasterImage_aRGB16::localRasterARGB16(imgIn, &localARGB16);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localaRGB16)<br>
             *  delete imgARGB16;</code>
             *
             *  @param  img        the image of which to make a local 16-bit aRGB copy
             *  @param  localARGB16  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
             */
            static RasterImage_aRGB16* localRasterARGB16(RasterImage* imgIn, bool* localARGB16);

            /** Allocates an aRGBa16 copy of the image received as parameter if that
             *  image is not already some form of aRGB16 raster.
             *  This function should be used as follows:
             *  <code>bool  localaRGB16;<br>
             *  localARGB16  *imgARGB16 = RasterImage_aRGB16::localRasterARGB16(imgIn, &localARGB16);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localaRGB16)<br>
             *  delete imgARGB16;</code>
             *
             *  @param  img        the image of which to make a local 16-bit aRGB copy
             *  @param  rect       the rectangle over which we want a gray-level image
             *  @param  localARGB16  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
             */
            static RasterImage_aRGB16* localRasterARGB16(RasterImage* imgIn, const ImageRect* rect,
                                                      bool* localARGB16);


			/**	Initialializes the lookup table for conversion to grayF
			 */
			static void initializeLutGray(void);
			
			/**	Initialializes the lookup tables for conversion to HSV
			 */
			static void initializeLutHSV(void);
			 
			 
			 
        protected:

            /**
             *  Protected default constructor.
             *
             *  Called by subclasses that don't need to allocate the base raster.
             */
            RasterImage_aRGB16(void);

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


            /**
             *  Forces an update of the base rgba raster if it is not the reference raster.
             *
             *  This virtual function should be overridden by all subclasses of RasterImage_RGBa.
             */
            virtual void updateRaster_(void) const;


        private:

            /**
             *  2D array mapped on the GraphWorld's 1D raster.
             */
            unsigned char** argb2D_;

            /**
             *  "shifted" 2D array (based on the image's rectangle bounds) mapped on the GraphWorld's
             *  1D raster.
             */
            unsigned char** shiftedArgb2D_;

			/**	lookup table that converts aRGB16 color values into float gray-level values. 
			 *	This table is only allocated and initialized by a call to initializeLutGray or
			 *	by the first attempt to convert an aRGB16 image to gray_F.
			 *	@see initializeLutGray
			 */
			static float* lutGrayF_;
			
			/**	lookup table that provides Hue values for aRGB16 color values. 
			 *	This table is only allocated and initialized by a call to initializeLutHSV or
			 *	by the first attempt to convert an aRGB16 image to HSV.
			 *	@see initializeLutHSV
			 */
			static short* lutHue_;
			
			/**	lookup table that provides Saturation values for aRGB16 color values. 
			 *	This table is only allocated and initialized by a call to initializeLutHSV or
			 *	by the first attempt to convert an aRGB16 image to HSV.
			 *	@see initializeLutHSV
			 */
			static unsigned char* lutSat_;
			
			/**	lookup table that provides Value values for aRGB16 color values. 
			 *	This table is only allocated and initialized by a call to initializeLutHSV or
			 *	by the first attempt to convert an aRGB16 image to HSV.
			 *	@see initializeLutHSV
			 */
			static unsigned char* lutVal_;
			
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


			/**	Copy constructor. Disabled.
			 *	@param	obj		the object to copy
			 */
			RasterImage_aRGB16(const RasterImage_aRGB16& obj);
			
			
			/**	Copy operator. Disabled.
			 *	@param	obj		the object to copy
			 */
			const RasterImage_aRGB16& operator = (const RasterImage_aRGB16& obj);
			

			/**	Alocates the GraphWorld for this object at the set dimensions
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
			 */
			void allocateGraphWorld_(int nbRows, int nbCols);


    };
}

#endif  //  RASTER_IMAGE_ARGB16_H
