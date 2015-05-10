/*  NAME:
        RasterImage_gray.h

    DESCRIPTION:
        RasterImage_gray public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef     URIVL_RASTER_IMAGE_GRAY_H
#define     URIVL_RASTER_IMAGE_GRAY_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage.h"

namespace uriVL
{
    /** Base gray-level image class.
     *
     *  RasterImage_gray is the base class for all gray-level image types.  This class is built
     *  around an 8-bit gray-level QuickTime GWorld buffer, which is itself wrapped into a
     *  GraphWorld object.
     *
     *  RasterImage_gray implements or overrides a number of methods of the base RasterImageClass,
     *  and in particular the convertTo<Gray,RGB,RGBa> methods.
     *
     *  <b>Important:</b>  All subclasses of RasterImage_gray <b>must</b> override the following
     *  methods:
     *      - getGray, which resturns the 1D base raster (8-bit gray level)
     *      - getGray2D, which resturns the 2D base raster (8-bit gray level).
     *  These methods must be overloaded to force a conversion from the reference type to the
     *  base raster type (and possibly an allocation of the base raster) if the base raster is not
     *  the reference.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RasterImage_gray : public RasterImage
    {
        public:

            /**
             *  Allocates an 8-bit gray-level raster image at the specified dimensions.
             *
             *  Allocates an 8-bit gray-level GraphWorld at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
             */
            RasterImage_gray(int nbRows, int nbCols);

            /**
             *  Initializes an 8-bit gray-level raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit gray-level GraphWorld at the specified dimensions and
             *  initializes it with the contents of the raster received as parameter.
             *
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
             *  @param      theGray    values to which the image should be initialized
             */
            RasterImage_gray(int nbRows, int nbCols, const unsigned char* theGray);

            /**
             *  Allocates an 8-bit gray-level raster image at the specified dimensions.
             *
             *  Allocates an 8-bit gray-level raster image at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             */
            RasterImage_gray(const ImageRect* theRect);

            /**
             *  Initializes an 8-bit gray-level raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit gray-level GraphWorld at the specified dimensions and
             *  initializes it with the contents of the raster received as parameter.
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             *  @param      theGray    values to which the image should be initialized
             */
            RasterImage_gray(const ImageRect* theRect, const unsigned char* theGray);

            /**
             *  Initializes an 8-bit gray-level raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit gray-level GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the GraphWorld received
             *  as parameter.
             *
             *  @param      gworld    the GraphWorld to use for dimensions and values
             */
            RasterImage_gray(const GraphWorld* gworld);

            /**
             *  Initializes an 8-bit gray-level raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit gray-level GraphWorld at the specified dimensions and
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
            RasterImage_gray(GraphWorld* gworld, bool keepThisGraphWorld);

            /**
             *  Initializes an 8-bit gray-level raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit gray-level GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img    the RasterImage to use for dimensions and values
             */
            RasterImage_gray(const RasterImage* img);

            /**
             *  Initializes an 8-bit gray-level raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit gray-level GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img     the RasterImage to use for dimensions and values
             *  @param      theRect ImageRect specifying the image's bounding rectangle
             */
            RasterImage_gray(const RasterImage* img, const ImageRect* theRect);

            /**
             *  Class destructor.
             *
             *  Standard destructor (nothing fancy).
             *
             */
            virtual ~RasterImage_gray(void);

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
             *      - kGrayRaster, since this image is based on the 8-bit gray level format
             *
             *  @return     the image's base raster type
             */
            BaseRasterType getBaseRasterType(void) const;

            /**
             *  Returns the 8-bit gray-level base raster as a read-only 1D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_gray.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  @return     1D base raster for this image
             */
            virtual const unsigned char* getGray(void) const;

            /**
             *  Returns the 8-bit gray-level base raster as a read-write 1D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_gray.  If the base
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
            virtual unsigned char* getGray(ReadWriteStatus rw);

            /**
             *  Returns the 8-bit gray-level base raster as a read-only 2D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_gray.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  @return     2D base raster for this image
             */
            virtual const unsigned char* const* getGray2D(void) const;

            /**
             *  Returns the 8-bit gray-level base raster as a read-write 2D array.
             *
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_gray.  If the base
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
            virtual unsigned char*const* getGray2D(ReadWriteStatus rw);

            /**
             *  Returns the shifted 8-bit gray-level base raster as a read-only 2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_gray.  If the base
             *  raster has not already been allocated, then it should be allocated and
             *  initialized using the reference information for the image (for example,
             *  HSV data should be converted into gray-level data in the 1D base raster).
             *
             *  @return     shifted 2D base raster for this image
             */
            virtual const unsigned char* const* getShiftedGray2D(void) const;


            /**
             *  Returns the shifted 8-bit gray-level base raster as a read-write 2D array.
             *
             *  The shifted raster has index bounds defined by the image's rectangle.
             *  This method is virtual and should be overridden by all subclasses
             *  of RasterImage_gray.  If the base
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
            virtual unsigned char* const* getShiftedGray2D(ReadWriteStatus rw);


            /**
             *  Converts a 1D gray-level raster into a 1D binary raster
             *
             *  Any nonzero value converts to 1, zero to 0.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToBinary(const unsigned char* theSource,
                                        unsigned char* theDestination,
                                        long nbPixels);

            /**
             *  Converts a 1D gray-level raster into a 1D gray-level raster
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToGray(const unsigned char* theSource,
                                        unsigned char* theDestination,
                                        long nbPixels);

            /**
             *  Converts a 1D gray-level raster into a 1D float gray-level raster
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToGray(const unsigned char* theSource,
                                        float* theDestination,
                                        long nbPixels);

            /**
             *  Converts a 1D gray-level raster into a 1D 32-bit rgba raster.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToRGBa(const unsigned char* theSource,
                                        unsigned char* theDestination,
                                        long nbPixels);

            /**
             *  Converts a 1D gray-level raster into a 1D 24-bit rgb raster.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToRGB(const unsigned char* theSource,
                                        unsigned char* theDestination,
                                        long nbPixels);

            /**
             *  Converts a 1D gray-level raster into a 1D 16-bit argb raster.
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             */
            static void convertToARGB16(const unsigned char* theSource,
                                        unsigned char* theDestination,
                                        long nbPixels);


            /** Allocates a read-only gray-level copy of the image received as parameter if that
             *  image is not already some form of gray-level raster.
             *  This function should be used as follows:
             *  <code>bool  localGray;<br>
             *  const RasterImage_gray  *imgGray = RasterImage_gray::localRasterGray(imgIn, &localGray);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localGray)<br>
             *  delete imgGray;</code>
             *
             *  @param  img        the image of which to make a local gray copy
             *  @param  localGray  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return	pointer to a read-only gray-level copy of the input image
             */
            static const RasterImage_gray* localRasterGray(const RasterImage* imgIn, bool* localGray);

            /** Allocates a read-only gray-level copy of the image received as parameter if that
             *  image is not already some form of gray-level raster.
             *  This function should be used as follows:
             *  <code>bool  localGray;<br>
             *  const RasterImage_gray  *imgGray = RasterImage_gray::localRasterGray(imgIn, &localGray);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localGray)<br>
             *  delete imgGray;</code>
             *
             *  @param  img        the image of which to make a local gray copy
             *  @param  rect       the rectangle over which we want a gray-level image
             *  @param  localGray  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return	pointer to a read-only gray-level copy of the input image
             */
            static const RasterImage_gray* localRasterGray(const RasterImage* imgIn, const ImageRect* rect,
                                                     bool *localGray);

            /** Allocates a writable gray-level copy of the image received as parameter if that
             *  image is not already some form of gray-level raster.
             *  This function should be used as follows:
             *  <code>bool  localGray;<br>
             *  RasterImage_gray  *imgGray = RasterImage_gray::localRasterGray(imgIn, &localGray);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localGray)<br>
             *  delete imgGray;</code>
             *
             *  @param  img        the image of which to make a local gray copy
             *  @param  localGray  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return	pointer to a gray-level copy of the input image
             */
            static RasterImage_gray* localRasterGray(RasterImage* imgIn, bool* localGray);

            /** Allocates a gray-level copy of the image received as parameter if that
             *  image is not already some form of gray-level raster.
             *  This function should be used as follows:
             *  <code>bool  localGray;<br>
             *  RasterImage_gray  *imgGray = RasterImage_gray::localRasterGray(imgIn, &localGray);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localGray)<br>
             *  delete imgGray;</code>
             *
             *  @param  img        the image of which to make a local gray copy
             *  @param  rect       the rectangle over which we want a gray-level image
             *  @param  localGray  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return	pointer to a gray-level copy of the input image
             */
            static RasterImage_gray* localRasterGray(RasterImage* imgIn, const ImageRect* rect,
                                                     bool *localGray);

		protected:
		
            /**
             *  Default constructor.
             *
             *  This constructor simply sets all instance variables to 0, NULL, or the
             *  appropriate default value.
             *
             *  This constructor should only be invoked from subclasses
             *  (RasterImage_gray_F, RasterImage_gray_D)
             */
            RasterImage_gray(void);

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
            virtual void writeInto_(RasterImage* imgOut, const ImageRect* rect, const ImagePoint* origin) const;


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
             *  @param  theWidth    number of columns of the image
             *  @param  theHeight   number of rows of the image
             */
            virtual void setBoundRect_(int x, int y, int theWidth, int theHeight);

			
			/**	Alocates the GraphWorld for this object at the set dimensions
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
			 */
			void allocateGraphWorld_(int nbRows, int nbCols);



            /**
             *  Forces an update of the base gray raster if it is not the reference raster.
             *
             *  This virtual function should be overridden by all subclasses of RasterImage_gray.
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

            /**
             *  Allocates the 2D gray-level base raster.
             *  @param  nbRows   number of rows of the image
             *  @param  nbCols   number of columns of the image
             */
            void allocateRaster2D_(int nbRows, int nbCols);

            /**
             *  Allocates the 2D shifted base raster.
             *  @param  jLow       X coordinate of the image's origin
             *  @param  iLow       Y coordinate of the image's origin
             *  @param  nbRows		number of rows of the image
             *  @param  nbCols		number of columns of the image
			 */
            void allocateShiftedRaster2D_(int nbRows, int nbCols, int iLow, int jLow);


        private:

            /**
             *  2D array mapped on the GraphWorld's 1D raster.
             */
            unsigned char** gray2D_;

            /**
             *  "shifted" 2D array mapped on the GraphWorld's 1D raster.
             */
            unsigned char** shiftedGray2D_;

            /** Creates a new raster image based on the one received as parameter.
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  img   pointer to the RasterImage_gray object to copy
             */
            RasterImage_gray(const RasterImage_gray& img);
			
			/**
             *  Equal operator.
             *  Created so that gcc will stop complaining. This RasterImage_gray object will
			 *	retain its own id.
             *  @param  theImg       reference to the RasterImage_gray object to copy
             */
            const RasterImage_gray& operator = (const RasterImage_gray& theImg);


            #if !URIVL_USE_QUICKTIME
                friend class _GraphWorld_gen;
            #endif
            
    };
}

#endif
