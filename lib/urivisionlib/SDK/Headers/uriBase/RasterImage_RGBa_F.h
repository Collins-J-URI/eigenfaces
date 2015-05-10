/*  NAME:
        RasterImage_RGBa_F.h

    DESCRIPTION:
        RasterImage_RGBa_F public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RASTER_IMAGE_RGBA_FLOAT_H
#define     URIVL_RASTER_IMAGE_RGBA_FLOAT_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage_RGBa.h"

namespace uriVL
{
    /** RasterImage_RGBa_F class
     *  @author jean-yves herve', 3D Group,
     *                            Department of Computer Science and Statistics,
     *                            University of Rhode Island
     */
    class URIVL_EXPORT RasterImage_RGBa_F : public RasterImage_RGBa
    {
        public:

			/**
			 *  Allocates a float RGBa color raster image at the dimensions specified.
			 *
			 *  Allocates a float RGBa color raster at the specified dimensions and
			 *  initializes this raster to zero values (black). Row and column indices 
			 *	of the new raster start at 0
			 *
			 *  @param  nbRows		number of rows of the raster
			 *	@param	nbCols		number of columns of the raster
			 */
			RasterImage_RGBa_F(int nbRows, int nbCols);

            /**
             *  Allocates a float RGBa color raster image at the specified dimensions
			 *	and initializes it.
             *
             *  Allocates a float RGBa color raster at the specified dimensions and
			 *  initializes this raster using the values stored in the 1D raster 
			 *	received as parameter. 
			 *
			 *  @param  nbRows		number of rows of the raster
			 *	@param	nbCols		number of columns of the raster
			 *	@param	theRGBa		1D RGBa raster (4 bytes/pixel) to unitialize the
			 *							new raster image with
             */
			RasterImage_RGBa_F(int nbRows, int nbCols, const unsigned char* theRGBa);


            /**
             *  Allocates a float RGBa color raster image at the specified dimensions
			 *	and initializes it.
             *
             *  Allocates a float RGBa color raster at the specified dimensions and
			 *  initializes this raster using the values stored in the 1D raster 
			 *	received as parameter. 
			 *
			 *  @param  nbRows		number of rows of the raster
			 *	@param	nbCols		number of columns of the raster
			 *	@param	theRed		1D raster (1 bytes/pixel) to unitialize the
			 *							new image's red channel with
			 *	@param	theGreen	1D raster (1 bytes/pixel) to unitialize the
			 *							new image's green channel with
			 *	@param	theBlue     1D raster (1 bytes/pixel) to unitialize the
			 *							new image's blue channel with
             */
            RasterImage_RGBa_F(int nbRows, int nbCols, const float* theRed,
                               const float* theGreen, const float* theBlue);


            /**
             *  Allocates a float RGBa color raster image at the specified dimensions.
             *
             *  Allocates a float RGBa color raster at the specified dimensions and
			 *  initializes this raster to zero values (black).
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             */
            RasterImage_RGBa_F(const ImageRect* theRect);
			
            /**
			 *  Allocates a float RGBa color raster image at the specified dimensions
			 *	and initializes it.
             *
             *  Allocates a float RGBa color raster at the specified dimensions and
			 *  initializes this raster using the values stored in the 1D raster 
			 *	received as parameter. 
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
			 *	@param		theRGBa		1D RGBa raster (4 bytes/pixel) to unitialize the
			 *							new raster image with
             */
			RasterImage_RGBa_F(const ImageRect* theRect, const unsigned char* theRGBa);

            /**
             *  Allocates a float RGBa color raster image at the specified dimensions
			 *	and initializes it.
             *
             *  Allocates a float RGBa color raster at the specified dimensions and
			 *  initializes this raster using the values stored in the 1D raster 
			 *	received as parameter. 
			 *
             *  @param  theRect     ImageRect specifying the image's bounding rectangle
			 *	@param	theRed		1D raster (1 bytes/pixel) to unitialize the
			 *							new image's red channel with
			 *	@param	theGreen	1D raster (1 bytes/pixel) to unitialize the
			 *							new image's green channel with
			 *	@param	theBlue     1D raster (1 bytes/pixel) to unitialize the
			 *							new image's blue channel with
             */
            RasterImage_RGBa_F(const ImageRect* theRect, const float* theRed,
                               const float* theGreen, const float* theBlue);

            /**
             *  Allocates a float RGBa color raster image at the dimensions and with the
			 *	values specified by the GraphWorld object received as parameter
             *
             *  @param  gworld  The GraphWolrd to initialize this image with
             */
            RasterImage_RGBa_F(const GraphWorld* gworld);

            /**
             *  Assigns to this raster image object the GraphWorld object received as 
             *  parameter.  There is no copying of the data here, only of the address.
             *
             *  @param  gworld  The GraphWolrd to initialize this image with
             */
            RasterImage_RGBa_F(GraphWorld* gworld, bool keepThisGraphWorld);

            /**
             *  Allocates a float RGBa color raster image at the dimensions and with the
			 *	values specified by the RasterImage object received as parameter.  If the
             *  image is not of type RGBa32, does a conversion.
             *
             *  @param  img  The raster image to initialize this image with
             */
            RasterImage_RGBa_F(const RasterImage* img);
            
            /**
             *  Allocates a float RGBa color raster image at the dimensions and with the
			 *	values specified by the RasterImage object received as parameter.  If the
             *  image is not of type RGBa32, does a conversion.
             *
             *  @param      img     the RasterImage to use for dimensions and values
             *  @param      theRect ImageRect specifying the image's bounding rectangle
             */
            RasterImage_RGBa_F(const RasterImage* img, const ImageRect* theRect);

            /** Destructor
             */
            ~RasterImage_RGBa_F(void);

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
             *  Overrides the method of the parent class RasterImage_RGBa.
             *
             *  @return     1D base raster for this image
             */
            const unsigned char* getRGBa(void) const;

            /**
             *  Returns the 32-bit rgba base raster as a read-write 1D array.
             *
             *  Overrides the method of the parent class RasterImage_RGBa.
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D base raster for this image
             */
            unsigned char* getRGBa(ReadWriteStatus rw);

            /**
             *  Returns the 0-based 32-bit rgba base raster as a read-only 2D array.
             *
             *  Overrides the method of the parent class RasterImage_RGBa.
             *
             *  @return     2D base raster for this image
             */
            const unsigned char* const* getRGBa2D(void) const;


            /**
             *  Returns the 0-based 32-bit rgba base raster as a read-write 2D array.
             *
             *  Overrides the method of the parent class RasterImage_RGBa.
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     2D base raster for this image
             */
            unsigned char* const* getRGBa2D(ReadWriteStatus rw);


            /**
             *  Returns the shifted 32-bit rgba base raster as a read-only 2D array.
             *
             *  Overrides the method of the parent class RasterImage_RGBa.
             *
             *  @return     shifted 2D base raster for this image
             */
            const unsigned char* const* getShiftedRGBa2D() const;


            /**
             *  Returns the shifted 32-bit rgba base raster as a read-write 2D array.
             *
             *  Overrides the method of the parent class RasterImage_RGBa.
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     shifted 2D base raster for this image
             */
            unsigned char* const* getShiftedRGBa2D(ReadWriteStatus rw);


			/**	Returns the base address of this objet's private 1D raster for the
			 *	red channel
			 */
            const float* getRedF(void) const;

			/**	Returns the base address of this objet's private 1D raster for the
			 *	red channel
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
			 */
            float* getRedF(ReadWriteStatus rw);

			/**	Returns the base address of this objet's private 1D raster for the
			 *	red channel
			 */
			const float* getGreenF(void) const;

			/**	Returns the base address of this objet's private 1D raster for the
			 *	red channel
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
			 */
			float* getGreenF(ReadWriteStatus rw);

			/**	Returns the base address of this objet's private 1D raster for the
			 *	blue channel
			 */
			const float* getBlueF(void) const;

			/**	Returns the base address of this objet's private 1D raster for the
			 *	blue channel
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
			 */
			float* getBlueF(ReadWriteStatus rw);

			/**	Returns the base address of this objet's private 2D raster for the
             *  red channel
             */
			const float* const* getRedF2D(void) const;

			/**	Returns the base address of this objet's private 2D raster for the
             *  red channel
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             */
			float* const* getRedF2D(ReadWriteStatus rw);

			/**	Returns the base address of this objet's private 1D raster for the
			 *	green channel
			 */
            const float* const* getGreenF2D(void) const;

			/**	Returns the base address of this objet's private 1D raster for the
			 *	green channel
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
			 */
            float* const* getGreenF2D(ReadWriteStatus rw);

			/**	Returns the base address of this objet's private 2D raster for the
             *	blue channel
             */
            const float* const* getBlueF2D(void) const;

			/**	Returns the base address of this objet's private 2D raster for the
             *	blue channel
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             */
            float* const* getBlueF2D(ReadWriteStatus rw);

			/**	Returns the base address of this objet's private shifted 2D raster for the
             *	red channel
             */
            const float* const* getShiftedRedF2D(void) const;

			/**	Returns the base address of this objet's private shifted 2D raster for the
             *	red channel
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             */
            float* const* getShiftedRedF2D(ReadWriteStatus rw);

			/**	Returns the base address of this objet's private shifted 2D raster for the
             *	green channel
             */
            const float* const* getShiftedGreenF2D(void) const;

			/**	Returns the base address of this objet's private shifted 2D raster for the
             *	green channel
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             */
            float* const* getShiftedGreenF2D(ReadWriteStatus rw);

			/**	Returns the base address of this objet's private shifted 2D raster for the
             *	blue channel
             */
            const float* const* getShiftedBlueF2D(void) const;

			/**	Returns the base address of this objet's private shifted 2D raster for the
             *	blue channel
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             */
            float* const* getShiftedBlueF2D(ReadWriteStatus rw);

            /** Sets an individual conversion mode from float to gray for this image.
             *
             *  @param  theMode     individual convcersion mode for this image
             *
             *  @see    getFloatConversionMode
             *  @see    setDefaultFloatConversionMode
             *  @see    hasIndivFloatConvMode_
             *  @see    defaultFloatConv_
             *  @see    floatConv_
             */
            void setFloatConversionMode(ConversionFromFloatMode theMode);

            /** Returns the image's conversion mode from float to gray.
             *
             *  @return     conversion mode for this image
             *
             *  @see    setFloatConversionMode
             *  @see    setDefaultFloatConversionMode
             *  @see    hasIndivFloatConvMode_
             *  @see    defaultFloatConv_
             *  @see    floatConv_
             */
            ConversionFromFloatMode getFloatConversionMode(void) const;

            /** Sets the default conversion mode from float to gray for this class.
             *
             *  @param  theMode     individual convcersion mode for the class
             *
             *  @see    getFloatConversionMode
             *  @see    setFloatConversionMode
             *  @see    hasIndivFloatConvMode_
             *  @see    defaultFloatConv_
             *  @see    floatConv_
             */
            static void setDefaultFloatConversionMode(ConversionFromFloatMode theMode);

            /**
             *  Converts a 1D 24-bit RGB raster into a 1D gray-level raster
             *
             *  @param      redSource       starting point of the red raster to convert
             *  @param      greenSource     starting point of the green raster to convert
             *  @param      blueSource      starting point of the blue raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             *  @param      convWeight      normalized weights to apply for the conversion
             *  @param      myFloatConv     the conversion mode from float desired
             */
            static void convertToGray(const float* redSource, const float* greenSource, 
                                      const float* blueSource, unsigned char* theDestination,
                                      long nbPixels, const float convWeight[],
                                      ConversionFromFloatMode myFloatConv = kUndefFloatConv);

            /**
             *  Converts a 1D 24-bit RGB raster into a 1D float gray-level raster
             *
             *  @param      redSource       starting point of the red raster to convert
             *  @param      greenSource     starting point of the green raster to convert
             *  @param      blueSource      starting point of the blue raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             *  @param      convWeight      normalized weights to apply for the conversion
             *  @param      myFloatConv     the conversion mode from float desired
             */
            static void convertToGray(const float* redSource, const float* greenSource, 
                                      const float* blueSource, float* theDestination,
                                      long nbPixels, const float convWeight[],
                                      ConversionFromFloatMode myFloatConv = kUndefFloatConv);

        private:

            /** private 1D raster for the red channel
             */
            float* red_;

            /** private 1D raster for the green channel
             */
            float* green_;

            /** private 1D raster for the blue channel
             */
            float* blue_;

            /** private 2D raster for the red channel
             */
            float** red2D_;

            /** private 2D raster for the green channel
             */
            float** green2D_;

            /** private 2D raster for the blue channel
             */
            float** blue2D_;

            /** private shifted 2D raster for the red channel
             */
            float** shiftedRed2D_;

            /** private shifted 2D raster for the green channel
             */
            float** shiftedGreen2D_;

            /** private shifted 2D raster for the blue channel
             */
            float** shiftedBlue2D_;

            /**
             *  True if an algorithm for conversion from float to gray <b>other</b> than the default
             *  class-wide algorithm identified in RasterImage::defaultFloatConv_.
             *
             *  @see    floatConv_
             */
            bool    hasIndivFloatConvMode_;

            /**
             *  Code for the algorithm for conversion from float to gray <b>other</b>
             *  than the default
             *  class-wide algorithm identified in RasterImage::defaultFloatConv_.
             *  Only relevant if hasIndivFloatConvMode_ is true.
             *
             *  @see    hasIndivFloatConvMode_
             */
            ConversionFromFloatMode    floatConv_;

            /** Scale factor to apply in a float to gray conversion
             */
            mutable float   convScale_;

            /** Scale factor to apply in a float to red conversion
             */
            mutable float   convScaleRed_;

            /** Scale factor to apply in a float to green conversion
             */
            mutable float   convScaleGreen_;

            /** Scale factor to apply in a float to blue conversion
             */
            mutable float   convScaleBlue_;

            /** Minimum value over the raster (r-g-b combined)
             */
            mutable float   valMin_;

            /** Minimum value over the raster (r-g-b combined)
             */
            mutable float   valMax_;

            /** Minimum value over the red raster
             */
            mutable float   valMinRed_;

            /** Maximum value over the red raster
             */
            mutable float   valMaxRed_;

            /** Minimum value over the green raster
             */
            mutable float   valMinGreen_;

            /** Maximum value over the green raster
             */
            mutable float   valMaxGreen_;

            /** Minimum value over the blue raster
             */
            mutable float   valMinBlue_;

            /** Maximum value over the blue raster
             */
            mutable float   valMaxBlue_;

            /**
             *  Code for the default algorithm for conversion from float to gray.  An image instance
             *  can override this default mode by setting its floatConv_ field to an other
             *  value.
             *
             *  @see    hasIndivFloatConvMode_
             */
            static ConversionFromFloatMode defaultFloatConv_;



			/**	Creates an undimensioned RGBa float color raster image
		     */
            RasterImage_RGBa_F(void);

            /**
             *  Allocates a float RGBa color raster image at the dimensions and with the
			 *	values specified by the RasterImage object received as parameter.  If the
             *  image is not of type RGBa32, does a conversion.
			 *	This copy contructor is not operative, and was only declared to eliminate compiler
			 *	warnings.
             *
             *  @param  img  The raster image to initialize this image with
             */
            RasterImage_RGBa_F(const RasterImage_RGBa_F& img);
            

            /**
             *  This operator has not been implemented.
             *
             *  @param  img  The raster image to initialize this image with
             */
            const RasterImage_RGBa_F& operator = (const RasterImage_RGBa_F& img);
            

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
            void writeInto_(RasterImage* imgOut, const ImageRect* rect, const ImagePoint* origin) const;

            /**
             *  Sets a new bounding rectangle for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation may require a reallocation of the image.
             *
             *  This function may be called or overridden by child classes that have their
             *  own raster
             *
             *  @param  x           horizontal coordinate of upper left corner
             *  @param  y           vertical coordinate of upper left corner
             *  @param  theWidth    desired width of the image
             *  @param  theHeight   desired height of the image
             */
            void setBoundRect_(int x, int y, int theWidth, int theHeight);


            /** Allocates the object's private rasters
             */
			void allocateRaster_(void);

            /** Deletes the object's private rasters
             */
			void deleteRaster_(void);

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


            /**
             *  Updates the image's base raster (if it is not the reference).
             *
             *  Overrides the virtual method of the RasterImage class.
             */
            void updateRaster_(void) const;

            /**
             *  Calculates the parameters of a conversion to (unsigned char) RGBa.
             */
            void calculateFloatConversionParams_(void) const;


            /**
             *  Converts a 1D float RGB raster into a 1D gray-level raster
             *
             *  @param      redSource       starting point of the red raster to convert
             *  @param      greenSource     starting point of the green raster to convert
             *  @param      blueSource      starting point of the blue raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
			 *	@param		valMin			min value of the float raster
			 *	@param		convScale		scaling factor from float to char
             *  @param      myGrayConv      the gray conversion mode desired
             *  @param      myFloatConv     the conversion mode from float desired
             */
            static void convertToGray_(const float* redSource, const float* greenSource, 
									   const float* blueSource, unsigned char* theDestination,
									   long nbPixels, float valMin, float convScale,
									   ConversionToGrayMode myGrayConv = kAverageConversion,
									   ConversionFromFloatMode myFloatConv = kUndefFloatConv);

            /**
             *  Converts a 1D float RGB raster into a 1D float gray-level raster
             *
             *  @param      redSource       starting point of the red raster to convert
             *  @param      greenSource     starting point of the green raster to convert
             *  @param      blueSource      starting point of the blue raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
			 *	@param		valMin			min value of the float raster
			 *	@param		convScale		scaling factor from float to char
             *  @param      myGrayConv      the gray conversion mode desired
             *  @param      myFloatConv     the conversion mode from float desired
             */
            static void convertToGray_(const float* redSource, const float* greenSource, 
									   const float* blueSource, float* theDestination,
									   long nbPixels, float valMin, float convScale,
									   ConversionToGrayMode myGrayConv = kAverageConversion,
									   ConversionFromFloatMode myFloatConv = kUndefFloatConv);

            /**
             *  Converts a 1D float RGB raster into a 1D 32-bit rgba raster.
             *
             *  @param      redSource       starting point of the red raster to convert
             *  @param      greenSource     starting point of the green raster to convert
             *  @param      blueSource      starting point of the blue raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
			 *	@param		valMin			min value of the float raster
			 *	@param		convScale		scaling factor from float to char
             *  @param      myFloatConv     the conversion mode from float desired
             */
            static void convertToRGBa_(const float* redSource, const float* greenSource, 
                                const float* blueSource, unsigned char* theDestination,
								long nbPixels, float valMin, float convScale,
                                ConversionFromFloatMode myFloatConv = kUndefFloatConv);

            /**
             *  Converts a 1D float RGB raster into a 1D 24-bit rgb raster.
             *
             *  @param      redSource       starting point of the red raster to convert
             *  @param      greenSource     starting point of the green raster to convert
             *  @param      blueSource      starting point of the blue raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
			 *	@param		valMin			min value of the float raster
			 *	@param		convScale		scaling factor from float to char
             *  @param      myFloatConv     the conversion mode from float desired
             */
            static void convertToRGB_(const float* redSource, const float* greenSource, 
									  const float* blueSource, unsigned char* theDestination,
									  long nbPixels, float valMin, float convScale,
									  ConversionFromFloatMode myFloatConv = kUndefFloatConv);


            /**
             *  Converts a 1D float RGB raster into a 1D 16-bit argb raster.
             *
             *  @param      redSource       starting point of the red raster to convert
             *  @param      greenSource     starting point of the green raster to convert
             *  @param      blueSource      starting point of the blue raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
			 *	@param		valMin			min value of the float raster
			 *	@param		convScale		scaling factor from float to char
             *  @param      myFloatConv     the conversion mode from float desired
             */
            static void convertToARGB16_(const float* redSource, const float* greenSource, 
										 const float* blueSource, unsigned char* theDestination,
										 long nbPixels, float valMin, float convScale,
										 ConversionFromFloatMode myFloatConv = kUndefFloatConv);


	//	temp hack
	friend class RasterImage_RGBa;

    };
	
	
}

#endif  //  RASTER_IMAGE_RGBA_FLOAT_H
