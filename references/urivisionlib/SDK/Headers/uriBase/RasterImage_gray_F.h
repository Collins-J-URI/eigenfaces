/*  NAME:
        RasterImage_gray_F.h

    DESCRIPTION:
        RasterImage_gray_F public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RASTER_IMAGE_GRAY_FLOAT_H
#define     URIVL_RASTER_IMAGE_GRAY_FLOAT_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage_gray.h"

namespace uriVL
{
    /** Floating point gray-level image class.  This class is heavily used by the
     *  image operator classes.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RasterImage_gray_F : public RasterImage_gray
    {

        public:

            /**
             *  Allocates a float gray-level raster image at the specified dimensions.
             *
             *  Allocates a float gray-level raster at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
             */
            RasterImage_gray_F(int nbRows, int nbCols);

            /**
             *  Allocates a float gray-level raster image at the specified dimensions.
             *
             *  Allocates a float gray-level raster at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             */
            RasterImage_gray_F(const ImageRect* theRect);


            /**
             *  Initializes a float raster image at the specified dimensions and values.
             *
             *  Allocates a float raster at the specified dimensions and
             *  initializes it with the contents of the raster received as parameter.
             *
             *  @param      nbRows     number of rows of the rgb raster image
             *  @param      nbCols     number of columns of the rgb raster image
             *  @param      theGray    values to which the image should be initialized
             */
            RasterImage_gray_F(int nbRows, int nbCols, const unsigned char* theGray);

            /**
             *  Initializes a float raster image at the specified dimensions and values.
             *
             *  Allocates a float raster at the specified dimensions and
             *  initializes it with the contents of the raster received as parameter.
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             *  @param      theGray    values to which the image should be initialized
             */
            RasterImage_gray_F(const ImageRect* theRect, const unsigned char* theGray);

            /**
             *  Initializes a float raster image at the specified dimensions and values.
             *
             *  Allocates a float raster at the specified dimensions and
             *  initializes it with the contents of the raster received as parameter.
             *
             *  @param      nbRows     number of rows of the rgb raster image
             *  @param      nbCols     number of columns of the rgb raster image
             *  @param      theGray    values to which the image should be initialized
             */
            RasterImage_gray_F(int nbRows, int nbCols, const float* theGray);

            /**
             *  Initializes a float raster image at the specified dimensions and values.
             *
             *  Allocates a float raster at the specified dimensions and
             *  initializes it with the contents of the raster received as parameter.
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             *  @param      theGray    values to which the image should be initialized
             */
            RasterImage_gray_F(const ImageRect* theRect, const float* theGray);

            /**
             *  Initializes a float raster image at the specified dimensions and values.
             *
             *  Allocates a float raster at the specified dimensions and
             *  initializes this raster to the values specified by the GraphWorld received
             *  as parameter.
             *
             *  @param      gworld    the GraphWorld to use for dimensions and values
             */
            RasterImage_gray_F(const GraphWorld* gworld);

            /**
             *  Initializes a float raster image at the specified dimensions and values.
             *
             *  Allocates a float raster at the specified dimensions and
             *  initializes this raster to the values specified by the GraphWorld received
             *  as parameter.
             *  Possibly keeps the GraphWorld received as parameter rather than making a copy
             *  of it.
             *
             *  @param      gworld    the GraphWorld to use for dimensions and values
             *  @param      keepThisGraphWorld  if true, then this new image raster object will
             *                  simply store a reference to a pre-allocated GWorld instead of
             *                  allocating its own (beware!)
             */
            RasterImage_gray_F(GraphWorld* gworld, bool keepThisGraphWorld);


            /**
             *  Initializes a float raster image at the specified dimensions and values.
             *
             *  Allocates a float raster at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img    the RasterImage to use for dimensions and values
             */
            RasterImage_gray_F(const RasterImage* img);

            /**
             *  Initializes a float raster image at the specified dimensions and values.
             *
             *  Allocates a float raster at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img     the RasterImage to use for dimensions and values
             *  @param      theRect ImageRect specifying the image's bounding rectangle
             */
            RasterImage_gray_F(const RasterImage* img, const ImageRect* theRect);

            /** Destructor
             */
            ~RasterImage_gray_F(void);

            /**
             *  Copies this image into the one received as parameter.
             *
             *  Copies (rather than clones) this image into the destination image received
             *  as parameter.  Depending on the type of the destination
             *  image, the copy may require some data conversion.
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
             *  as parameter.  Depending on the type of the destination
             *  image, the copy may require some data conversion.
             *  If the raster(s) of the destination image does not have the same dimensions
             *  as the source, then the destination image's raster are re-allocated.
             *
             *  @param      imgOut  pointer to the destination image
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
             *  Returns the float gray-level raster for this image as a read-only 1D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  @return     1D float raster for this image
             */
            const float* getGrayF(void) const;

            /**
             *  Returns the float gray-level raster for this image as a read-write 1D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D float raster for this image
             */
            float* getGrayF(ReadWriteStatus rw);

            /**
             *  Returns the float gray-level raster for this image as a read-only 2D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  @return     1D float raster for this image
             */
            const float* const* getGrayF2D(void) const;

            /**
             *  Returns the float gray-level raster for this image as a read-write 2D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D float raster for this image
             */
            float*const* getGrayF2D(ReadWriteStatus rw);

            /**
             *  Returns the shifted float gray-level raster for this image as a read-only 2D 
             *  array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  @return     1D float raster for this image
             */
            const float* const* getShiftedGrayF2D(void) const;

            /**
             *  Returns the shifted float gray-level raster for this image as a read-write 
             *  2D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D float raster for this image
             */
            float* const* getShiftedGrayF2D(ReadWriteStatus rw);

            /**
             *  Returns the 8-bit gray-level base raster as a read-only 1D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  @return     1D base raster for this image
             */
            const unsigned char* getGray(void) const;

            /**
             *  Returns the 8-bit gray-level base raster as a read-write 1D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D base raster for this image
             */
            unsigned char* getGray(ReadWriteStatus rw);

            /**
             *  Returns the 8-bit gray-level base raster as a read-only 2D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  @return     2D base raster for this image
             */
            const unsigned char* const* getGray2D(void) const;

            /**
             *  Returns the 8-bit gray-level base raster as a read-write 2D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     2D base raster for this image
             */
            unsigned char* const* getGray2D(ReadWriteStatus rw);

            /**
             *  Returns the shifted 8-bit gray-level base raster as a read-only 2D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  @return     shifted 2D base raster for this image
             */
            const unsigned char* const* getShiftedGray2D(void) const;


            /**
             *  Returns the shifted 8-bit gray-level base raster as a read-write 2D array.
             *
             *  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     shifted 2D base raster for this image
             */
            unsigned char* const* getShiftedGray2D(ReadWriteStatus rw);


            /** Sets an individual conversion mode from float to gray for this image.
             *  @param  theMode     individual convcersion mode for this image
             *
             *  @see    getFloatConversionMode
             *  @see    setDefaultFloatConversionMode
             *  @see    hasIndivFloatConvMode_
             *  @see    defaultFloatConv_
             *  @see    floatConv_
             */
            void setFloatConversionMode(const ConversionFromFloatMode theMode);

            /** Returns the image's conversion mode from float to gray.
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
             *  @param  theMode     individual convcersion mode for the class
             *
             *  @see    getFloatConversionMode
             *  @see    setFloatConversionMode
             *  @see    hasIndivFloatConvMode_
             *  @see    defaultFloatConv_
             *  @see    floatConv_
             */
            static void setDefaultFloatConversionMode(const ConversionFromFloatMode theMode);

		
		protected:
		
            /**
             *  Default constructor.
             *
             *  This constructor simply sets all instance variables to 0, NULL, or the
             *  appropriate default value.
             *
             *  This constructor should only be invoked from subclasses of Raster_image_gray_F
             */
            RasterImage_gray_F(void);

		

        private:

            /**
             *  The float gray-level raster of this image.
             */
            mutable float*	fltGray_;

            /**
             *  The 0-based 2D float gray-level raster mapped over fltGray_.
             */
            mutable float**	fltGray2D_;

            /**
             *  The shifted 2D float gray-level raster mapped over fltGray_.
             */
            mutable float**	shiftedFltGray2D_;

            /**
             *  True if an algorithm for conversion from float to gray <b>other</b>
             *  than the default class-wide algorithm identified in 
             *  RasterImage::defaultFloatConv_.
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

            /**
             *  Code for the default algorithm for conversion from float to gray.  An image instance
             *  can override this default mode by setting its floatConv_ field to an other
             *  value.
             *
             *  @see    hasIndivFloatConvMode_
             */
            static ConversionFromFloatMode defaultFloatConv_;


            /** Minimum value over the raster
             */
            mutable float   valMin_;

            /** Minimum value over the raster
             */
            mutable float   valMax_;

            /** Scale factor to apply in a float to gray conversion
             */
            mutable float   convScale_;


            /** Creates a new raster image based on the one received as parameter.
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  img   pointer to the RasterImage_gray_F object to copy
             */
            RasterImage_gray_F(const RasterImage_gray_F& img);
			
			/**
             *  Equal operator.
             *  Created so that gcc will stop complaining. This RasterImage_gray object will
			 *	retain its own id and info.
             *  @param  theImg       reference to the RasterImage_gray_F object to copy
             */
            const RasterImage_gray_F& operator = (const RasterImage_gray_F& theImg);


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
             *  own raster
             *
             *  @param  x           horizontal coordinate of upper left corner
             *  @param  y           vertical coordinate of upper left corner
             *  @param  theWidth    desired width of the image
             *  @param  theHeight   desired height of the image
             */
            virtual void setBoundRect_(int x, int y, int theWidth, int theHeight);


            /**
             *  Converts this image's base raster into a 1D gray-level raster
             *
             *  Initializes a 1D raster array to store the gray levels
             *  corresponding to this image's data.
             *  Will be called by convertToGray and copyInto.
             *
             *  The raster returned is not a clone of this image's base raster since it does
             *  not contain any padding of the rows.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
			 *	@param		valMin			min value of the float raster
			 *	@param		convScale		scaling factor from float to char
             *  @param      myFloatConv     the conversion mode from float desired
             */
            static void convertToGray_(const float* theSource,
                                unsigned char* theDestination,
                                long nbPixels,
								float valMin, float convScale,
                                ConversionFromFloatMode myFloatConv = kUndefFloatConv);

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
             *  @param      myFloatConv     the conversion mode from float desired
             */
            void convertToRGBa_(const float* theSource,
                                unsigned char* theDestination,
                                long nbPixels,
                                ConversionFromFloatMode myFloatConv = kUndefFloatConv);

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
             *  @param      myFloatConv     the conversion mode from float desired
             */
            void convertToRGB_(const float* theSource,
                               unsigned char* theDestination,
                               long nbPixels,
                               ConversionFromFloatMode myFloatConv = kUndefFloatConv);


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
             *  Calculates the parameters of a conversion to gray.
             */
            void calculateFloatConversionParams_(void) const;


            friend class RasterImage_gray;

/*
            friend class RasterImage;
            friend class RasterImage_RGB;
            friend class RasterImage_RGBa;
            friend class RasterImage_aRGB16;
*/
    };
}

#endif  //  RASTER_IMAGE_GRAY_FLOAT_H
