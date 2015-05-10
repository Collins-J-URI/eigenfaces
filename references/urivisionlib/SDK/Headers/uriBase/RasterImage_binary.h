/*  NAME:
        RasterImage_binary.h

    DESCRIPTION:
        RasterImage_binary public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef     URIVL_RASTER_IMAGE_BINARY_H
#define     URIVL_RASTER_IMAGE_BINARY_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage.h"
#include    "RasterImage_gray.h"

namespace uriVL
{
    /** Base binary image class.
     *
     *  RasterImage_binary objects are used in image processing as the input and output
     *  of a number of operators such as edge detection or morphological operators.
     *
     *  RasterImage_binary implements or overrides a number of methods of the base
     *  RasterImageClass, and in particular the mostly useless convertTo<Gray,RGB,RGBa>
     *  methods.
     *
     *  This class is special in the sense that it must maintain two copies of its
     *  raster: the "packed binary" base raster (8 pixels per byte) that is used to
     *  display the image, and the 1-byte-per pixel raster that is used to manipulate
     *  pixels.  The latter is stored as a RasterImage_gray object.  Any nonzero
     *  value in this raster will be coded as a 1 in the binary for on the image.
     *
     *  At this point, I don't see any reason why anyone would want to subclass
     *  RasterImage_binary, so I won't waste time thinking/writing about what should
     *  and what should not be overridden.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RasterImage_binary : public RasterImage
    {
        public:

            /**
             *  Allocates a binary raster image at the specified dimensions.
             *
             *  Allocates a binary GraphWorld at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      nbRows     number of rows of the binary raster image
             *  @param      nbCols     number of columns of the binary raster image
             */
            RasterImage_binary(int nbRows, int nbCols);

            /**
             *  Allocates an 8-bit binary raster image at the specified dimensions.
             *
             *  Allocates an 8-bit binary raster image at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      theRect     ImageREct specifying the image's bounding rectangle
             */
            RasterImage_binary(const ImageRect* theRect);

            /**
             *  Initializes an 8-bit binary raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit binary GraphWorld at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      nbRows     number of rows of the binary raster image
             *  @param      nbCols     number of columns of the binary raster image
             *  @param      theBW      values to which the image should be initialized
             */
            RasterImage_binary(int nbRows, int nbCols, const bool* theBw);

            /**
             *  Initializes an 8-bit binary raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit binary GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the GraphWorld received
             *  as parameter.
             *
             *  @param      gworld    the GraphWorld to use for dimensions and values
             */
            RasterImage_binary(const GraphWorld* gworld);

            /**
             *  Initializes an 8-bit binary raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit binary GraphWorld at the specified dimensions and
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
            RasterImage_binary(GraphWorld* gworld, bool keepThisGraphWorld);

            /**
             *  Initializes an 8-bit binary raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit binary GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img    the RasterImage to use for dimensions and values
             */
            RasterImage_binary(const RasterImage* img);


            /**
             *  Initializes an 8-bit binary raster image at the specified dimensions and values.
             *
             *  Allocates a 32-bit color GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img     the RasterImage to use for dimensions and values
             *  @param      theRect ImageRect specifying the image's bounding rectangle
             */
            RasterImage_binary(const RasterImage* img, const ImageRect* theRect);


            /**
             *  Class destructor.
             *
             *  Standard destructor (nothing fancy).
             *
             */
            ~RasterImage_binary(void);

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
             *  Returns the base raster as a read-only 1D array.
             *
             *  The implementation of this function for the "binary" image class differs
             *  from that for the "gray" and various "color classes in two important
             *  aspects:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  @return     1D base raster for this image
             *  @see     setGraphWorldAsReference
             */
            const unsigned char* getRaster(void) const;

            /**
             *  Returns the base raster as a read-write 1D array.
             *
             *  The implementation of this function for the "binary" image class differs
             *  from that for the "gray" and various "color classes in two important
             *  aspects:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D base raster for this image
             *  @see     setGraphWorldAsReference
             */
            unsigned char* getRaster(ReadWriteStatus rw);

            /**
             *  Returns the base raster as a read-only 2D array.
             *
             *  The implementation of this function for the "binary" image class differs
             *  from that for the "gray" and various "color classes in two important
             *  aspects:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  @return     2D base raster for this image
             *  @see     setGraphWorldAsReference
             */
            const unsigned char* const* getRaster2D(void) const;

            /**
             *  Returns the base raster as a read-write 2D array.
             *
             *  The implementation of this function for the "binary" image class differs
             *  from that for the "gray" and various "color classes in two important
             *  aspects:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     2D base raster for this image
             *  @see     setGraphWorldAsReference
             */
            unsigned char* const* getRaster2D(ReadWriteStatus rw);

            /**
             *  Returns the shifted base raster as a read-only 2D array.
             *
             *  The implementation of this function for the "binary" image class differs
             *  from that for the "gray" and various "color classes in two important
             *  aspects:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  @return     shifted 2D base raster for this image
             *  @see     setGraphWorldAsReference
             */
            const unsigned char* const* getShiftedRaster2D(void) const;

            /**
             *  Returns the shifted base raster as a read-write 2D array.
             *
             *  The implementation of this function for the "binary" image class differs
             *  from that for the "gray" and various "color classes in two important
             *  aspects:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     shifted 2D base raster for this image
             *  @see     setGraphWorldAsReference
             */
            unsigned char* const* getShiftedRaster2D(ReadWriteStatus rw);

            /**
             *  Returns the 1-bit binary base raster as a read-only 1D array.
             *
             *  You must be aware of the following when you use this function:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  @return     1D base raster for this image
             *  @see    getRaster
             */
            const unsigned char* getBw(void) const;

            /**
             *  Returns the 1-bit binary base raster as a read-write 1D array.
             *
             *  You must be aware of the following when you use this function:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     1D base raster for this image
             *  @see    getRaster
             */
            unsigned char* getBw(ReadWriteStatus rw);

            /**
             *  Returns the 1-bit binary "unpacked" base raster as a read-only 2D array.
             *
             *  You must be aware of the following when you use this function:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  @return     2D base raster for this image
             *  @see    getRaster2D
             */
            const unsigned char* const* getBw2D(void) const;

            /**
             *  Returns the 1-bit binary "unpacked" base raster as a read-write 2D array.
             *
             *  You must be aware of the following when you use this function:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     2D base raster for this image
             *  @see    getRaster2D
             */
            unsigned char* const* getBw2D(ReadWriteStatus rw);

            /**
             *  Returns the shifted 1-bit binary "unpacked" base raster as a read-only 2D array.
             *
             *  You must be aware of the following when you use this function:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  @return     shifted 2D base raster for this image
             *  @see    getShiftedRaster2D
             */
            const unsigned char* const* getShiftedBw2D(void) const;


            /**
             *  Returns the shifted 1-bit binary "unpacked" base raster as a read-write 2D array.
             *
             *  You must be aware of the following when you use this function:
             *      - First, it does not really return the image's graphworld's private
             *          raster, which is a "packed" binary raster (8 pixels per byte)
             *          but an "unpacked" copy of that raster (1 byte per pixel).
             *      - The raster that is returned is now assumed to be the reference raster
             *          for the image.  In other words, <code>gworldIsReference_</code> is
             *          set to <code>false</code> and <code>referenceHasChanged_</code> is
             *          set to <code>true</code>.  This will results in a slight performance
             *          penalty when rendering the image (if the "unpacked" raster was in fact
             *          not modified).  It is possible to avoid that performance hit by setting
             *          the graphworld back as the reference: <code>setGraphWorldAsReference</code>
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return     shifted 2D base raster for this image
             *  @see    getShiftedRaster2D
             */
            unsigned char* const* getShiftedBw2D(ReadWriteStatus rw);


            /** Returns the gray-level form of this image
             *  @return     private gray-level RasterImage form
             */
            RasterImage_gray* getGrayRasterImage(void) const;


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
             *  Converts (copies) a 1D gray-level raster into a 1D gray-level raster
             *
             *  @param      theSource       starting point of the raster to convert
             *  @param      theDestination  starting point of the destination raster
             *  @param      nbPixels        number of pixels to convert
             *  @param      convWeight      normalized weights to apply for the conversion
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
             *  @param      myGrayConv      the gray conversion mode desired
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

			/**	Indicates whether the "unpacked" raster (rather than the "packed" binary raster)
			 *	is the reference
			 *	@return		true if the "unpacked" raster (rather than the "packed" binary raster)
			 *	is the reference
			 */
			bool unpackedRasterIsReference(void);


        protected:

			/**	Alocates the GraphWorld for this object at the set dimensions
             *  @param      nbRows     number of rows of the gray-level raster image
             *  @param      nbCols     number of columns of the gray-level raster image
			 */
			void allocateGraphWorld_(int nbRows, int nbCols);

			/**	Sets the "unpacked" raster (rather than the "packed" binary raster)
			 *	tpo be the reference
			 */
			void setUnpackedRasterAsReference(void) const;

				
		private:
		
            /** "Unpacked" gray-level version of this object.  This member is mutable because we 
			 *	can "change" it by allocating the object w/o really changing this binary image.
             */
            mutable RasterImage_gray* grayForm_;

//            /**
//             *  2D array mapped on the GraphWorld's 1D raster.
//             */
//            unsigned char** bw2D_;
//
//            /**
//             *  "shifted" 2D array mapped on the GraphWorld's 1D raster.
//             */
//            unsigned char** shiftedBw2D_;
//
			/**	Indicates whether the "unpacked" raster (rather than the "packed" binary raster)
			 *	is the reference
			 */
			mutable bool unpackedRasterIsReference_;
			

            /**
             *  Default constructor.
             *
             *  This constructor simply sets all instance variables to 0, NULL, or the
             *  appropriate default value.
             *
             *  This constructor should only be invoked from subclasses
             */
            RasterImage_binary(void);

             /** Creates a new raster image based on the one received as parameter.
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  img   pointer to the RasterImage_binary object to copy
             */
            RasterImage_binary(const RasterImage_binary& img);
			
			/**
             *  Equal operator.
             *  Created so that gcc will stop complaining. This RasterImage_binary object will
			 *	retain its own id.
             *  @param  thePt       reference to the RasterImage_binary object to copy
             */
            const RasterImage_binary& operator = (const RasterImage_binary& thePt);



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
             *  Sets a new bounding rectangle for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a RasterImage could be defined as a subimage of another
             *  image.  This operation may require a reallocation of the image.
             *
             *  @param  x           horizontal coordinate of upper left corner
             *  @param  y           vertical coordinate of upper left corner
             *  @param  theWidth    desired width of the image
             *  @param  theHeight   desired height of the image
             */
            void setBoundRect_(int x, int y, int theWidth, int theHeight);


            /**
             *  Sets a new valid data rectangle for the raster image.
             *
             *  A raster image occupies a rectangle within the global image space.
             *  For example, a raster image could be defined as a subpart of another.
             *  Only part of the image may contain valid data (for example after application
             *  of a filter).
             *
             *  This function is called by the public function setValidRect of the root
             *  parent class <code.DrawableObject2D</code>.  All data validity tests were
             *  performed there.  This protected function only has to take care of
             *  class-specific issues such as the handling of non-base rasters.
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            void setValidRect_(int x, int y, int theWidth, int theHeight);


            /**
             *  Forces an update of the base gray raster if it is not the reference raster.
             *
             *  This virtual function should be overridden by all subclasses of RasterImage_gray.
             */
            void updateRaster_(void) const;


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

#endif
