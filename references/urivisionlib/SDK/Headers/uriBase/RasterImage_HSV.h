/*  NAME:
        RasterImage_HSV.h

    DESCRIPTION:
        RasterImage_HSV public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RASTER_IMAGE_HSV
#define     URIVL_RASTER_IMAGE_HSV

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage_gray.h"
#include    "RasterImage_RGB.h"
#include    "RasterImage_RGBa.h"
#include    "RasterImage_aRGB16.h"

namespace uriVL
{
    /** A RasterImage_HSV is a RasterImage_RGBa that also stores the hue, saturation,
     *  and value for each pixel.
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RasterImage_HSV : public RasterImage_RGBa
    {
        public:

            /** Creates a RasterImage_HSV object at the specified dimensions
             *  @param      nbRows     number of rows of the hsv raster image
             *  @param      nbCols     number of columns of the hsv raster image
             */
            RasterImage_HSV(int theRows, int theCols);

            /** Allocates an HSV raster image at the specified dimensions.
             *
             *  Allocates an HSV color raster at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             */
            RasterImage_HSV(const ImageRect* theRect);

            /** Creates and initializes a raster image using the specified dimensions and
             *  values
             *
             *  @param  nbRows     number of rows of the hsv raster image
             *  @param  nbCols     number of columns of the hsv raster image
             *  @param  hue         1D raster storing hue values in the range [0, 359]
             *  @param  sat         1D raster storing saturation values in the range [0, 100]
             *  @param  val         1D raster storing the value in the range [0, 255]
             */
            RasterImage_HSV(int theRows, int theCols, const unsigned int* hue,
                            const unsigned char* sat, const unsigned char* val);

            /** Cloner constructor
             *  @param  img the image to clone
             */
            RasterImage_HSV(const RasterImage* img);

            /**
             *  Initializes an HSV raster image at the specified dimensions and values.
             *
             *  Allocates an 8-bit gray-level GraphWorld at the specified dimensions and
             *  initializes this raster to the values specified by the RasterImage received
             *  as parameter.
             *
             *  @param      img     the RasterImage to use for dimensions and values
             *  @param      theRect ImageRect specifying the image's bounding rectangle
             */
            RasterImage_HSV(const RasterImage* img, const ImageRect* theRect);


			/**	Copy constructor. Not implemented
			 *	@param	obj	the object to copy
			 */
			RasterImage_HSV(const RasterImage_HSV& obj);
			
			
            /** Destructor
             */
            virtual ~RasterImage_HSV(void);

			/**	Copy operator. Not implemented
			 *	@param	obj	the object to copy
			 */
			const RasterImage_HSV& operator = (const RasterImage_HSV& obj);
			

            /** Returns the hue as a read-only 1D raster
             *  @return hue as a 1D raster
             */
            const int* getHue(void) const;

            /** Returns the hue as a read-write 1D raster
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return hue as a 1D raster
             */
            int* getHue(ReadWriteStatus rw);

            /** Returns the hue as a read-only 2D raster
             *  @return hue as a 2D raster
             */
            const int* const* getHue2D(void) const;

            /** Returns the hue as a read-write 2D raster
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return hue as a 2D raster
             */
            int* const* getHue2D(ReadWriteStatus rw);

            /** Returns the hue as a read-only shifted 2D raster
             *  @return hue as a shifted 2D raster
             */
            const int* const* getShiftedHue2D(void) const;

            /** Returns the hue as a read-write shifted 2D raster
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return hue as a shifted 2D raster
             */
            int* const* getShiftedHue2D(ReadWriteStatus rw);

            /** Returns the saturation as a read-only 1D raster
             *  @return saturation as a 1D raster
             */
            const unsigned char* getSat(void) const;

            /** Returns the saturation as a read-write 1D raster
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @return saturation as a 1D raster
             */
            unsigned char* getSat(ReadWriteStatus rw);

            /** Returns the saturation as a read-only 2D raster
             *  @return saturation as a 2D raster
             */
            const unsigned char* const* getSat2D(void) const;

            /** Returns the saturation as a read-write 2D raster
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return saturation as a 2D raster
             */
            unsigned char* const* getSat2D(ReadWriteStatus rw);

            /** Returns the saturation as a read-only shifted 2D raster
             *  @return saturation as a shifted 2D raster
             */
            const unsigned char* const* getShiftedSat2D(void) const;

            /** Returns the saturation as a read-write shifted 2D raster
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return saturation as a shifted 2D raster
             */
            unsigned char* const* getShiftedSat2D(ReadWriteStatus rw);

            /** Returns the value as a read-only 1D raster
             *  @return the value a 1D raster
             */
            const unsigned char* getVal(void) const;

            /** Returns the value as a read-write  1D raster
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return the value a 1D raster
             */
            unsigned char* getVal(ReadWriteStatus rw);

            /** Returns the value as a read-only 2D raster
             *  @return the value a 2D raster
             */
            const unsigned char* const* getVal2D(void) const;

            /** Returns the value as a read-write  2D raster
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return the value a 2D raster
             */
            unsigned char* const* getVal2D(ReadWriteStatus rw);

            /** Returns the value as a read-only shifted 2D raster
             *  @return the value a shifted 2D raster
             */
            const unsigned char* const* getShiftedVal2D(void) const;

            /** Returns the value as a read-write  shifted 2D raster
             *
             *  A side-effect of calling the read-write version of a getRaster function is
             *  that the base (GraphWorld) raster is set to be the reference and is marked
             *  as 'changed'
             *  @param   rw     R_W_ACCESS is the only acceptable value
             *  @return the value a shifted 2D raster
             */
            unsigned char* const* getShiftedVal2D(ReadWriteStatus rw);

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
            const unsigned char* const* getShiftedRGBa2D(void) const;


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


            /** Converts an rgb value to hue, saturation, and value
             *
             *  @param  r       red component of the color to convert
             *  @param  g       green component of the color to convert
             *  @param  b       blue component of the color to convert
             *  @param  hue     corresponding hue in the range [0, 359]
             *  @param  sat     corresponding saturation in the range [0, 100]
             *  @param  val     corresponding value in the range [0, 255]
             */
            static void rgb2HSV(unsigned char r, unsigned char g, unsigned char b,
                                int* hue, unsigned char* sat, unsigned char* val);

            /** Converts an rgb value to hue, saturation, and value
             *
             *  @param  rgb     the color to convert
             *  @param  hue     corresponding hue in the range [0, 359]
             *  @param  sat     corresponding saturation in the range [0, 100]
             *  @param  val     corresponding value in the range [0, 255]
             */
            static void rgb2HSV(const unsigned char rgb[],
                                int* hue, unsigned char* sat, unsigned char* val);

            /** Sets this image's HSV rasters as the reference
             *  @see setGraphWorldAsReference
             */
            virtual void setHsvAsReference(void);


            /** Forces a conversion of the image's gworld rasters into a saturated
             *  image based on curent hue raster.
             */
            void forceConversionToSaturatedRGBa(void);

            /** Allocates a read-only HSV copy of the image received as parameter if that
             *  image is not already some forn of HSV raster.
             *  This function should be used as follows:
             *  <code> = bool  localHSV;<br>
             *  const RasterImage_HSV* imgHSV = RasterImage_HSV::localRasterHSV_(imgIn, &localHSV);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localHSV)<br>
             *  delete imgHSV;</code>
             *
             *  @param  img        the image of which to make a local gray copy
             *  @param  localGray  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return		pointer to read-only HSV image
             */
            static const RasterImage_HSV* localRasterHSV(const RasterImage* imgIn, bool* localHSV);

            /** Allocates a read-only HSV copy of the image received as parameter if that
             *  image is not already some forn of HSV raster.
             *  This function should be used as follows:
             *  <code> = bool  localHSV;<br>
             *  const RasterImage_HSV* imgHSV = RasterImage_HSV::localRasterHSV(imgIn, &localHSV);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localHSV)<br>
             *  delete imgHSV;</code>
             *
             *  @param  img        the image of which to make a local gray copy
             *  @param  rect       the rectangle over which we want a gray-level image
             *  @param  localGray  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return		pointer to read-only HSV image
             */
            static const RasterImage_HSV* localRasterHSV(const RasterImage* imgIn, const ImageRect* rect,
														 bool* localHSV);


            /** Allocates a writable HSV copy of the image received as parameter if that
             *  image is not already some forn of HSV raster.
             *  This function should be used as follows:
             *  <code> = bool  localHSV;<br>
             *  RasterImage_HSV  *imgHSV = RasterImage_HSV::localRasterHSV_(imgIn, &localHSV);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localHSV)<br>
             *  delete imgHSV;</code>
             *
             *  @param  img        the image of which to make a local gray copy
             *  @param  localGray  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return		pointer to writable HSV image
             */
            static RasterImage_HSV* localRasterHSV(RasterImage* imgIn, bool* localHSV);

            /** Allocates a writable HSV copy of the image received as parameter if that
             *  image is not already some forn of HSV raster.
             *  This function should be used as follows:
             *  <code> = bool  localHSV;<br>
             *  RasterImage_HSV  *imgHSV = RasterImage_HSV::localRasterHSV(imgIn, &localHSV);<code><p>
             *      ... lots of high-quality, efficient code ...<p>
             *  <code>if (localHSV)<br>
             *  delete imgHSV;</code>
             *
             *  @param  img        the image of which to make a local gray copy
             *  @param  rect       the rectangle over which we want a gray-level image
             *  @param  localGray  will be <code>true</code> if a local raster was allocated,
             *                      <code>true</code> if the image returned is just 
             *                      <code>img</code> itself
			 *	@return		pointer to writable HSV image
             */
            static RasterImage_HSV* localRasterHSV(RasterImage* imgIn, const ImageRect* rect,
												   bool* localHSV);


        private:

            /** 1D raster storing the hue of this image (range [0, 359])
             *  @see getHue
             */
            int* hue_;

            /** 1D raster storing the saturation of this image (range [0, 100])
             *  @see getSat
             */
            unsigned char* sat_;

            /** 1D raster storing the value of this image (range [0, 255])
             *  @see getVal
             */
            unsigned char* val_;

            /** unshifted 2D raster storing the hue of this image (range [0, 359])
             *  @see getHue2D
             */
            int** hue2D_;

            /** unshifted 2D raster storing the saturation of this image (range [0, 100])
             *  @see getSat
             */
            unsigned char** sat2D_;

            /** unshifted 2D raster storing the value of this image (range [0, 255])
             *  @see getVal
             */
            unsigned char** val2D_;

            /** shifted 2D raster storing the hue of this image (range [0, 359])
             *  @see getHue2D
             */
            int** shiftedHue2D_;

            /** shifted 2D raster storing the saturation of this image (range [0, 100])
             *  @see getSat
             */
            unsigned char** shiftedSat2D_;

            /** shifted 2D raster storing the value of this image (range [0, 255])
             *  @see getVal
             */
            unsigned char** shiftedVal2D_;

            /** <code>true</code> when the HSV rasters define the reference for this image
             */
            bool hsvIsReference_;


			#if URIVL_LUT_RGB_TO_HSV
				static int* hueFromRGB_;
				static unsigned char* satFromRGB_;
				static unsigned char* valFromRGB_;
			#endif

			#if URIVL_LUT_HSV_TO_RGB
				static int*** rgbaFromHSV_;
			#endif

			/**	Creates an undimensioned RGBa float color raster image
		     */
            RasterImage_HSV(void);

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

            /** Deletes the raster image's private hue, saturation and value rasters
             */
            void deleteHSV_(void);

            /** Updates the RGBa raster of this raster image to match the HSV rasters
             */
            void convertToRGBa_(void);

            /** Updates the RGBa raster of this raster image to match the HSV rasters
             *  on the image rectangle specified
             *  @param  theRect     the image rectangle on which to do the conversion
             */
            void convertToRGBa_(const ImageRect* theRect);

            /** Updates the RGBa raster of this raster image to match a saturated version
             *  the HSV rasters (where a value of 100 has been used for the saturation)
             */
            void convertToSaturatedRGBa_(void);

            /** Updates the RGBa raster of this raster image to match  a saturated version
             *  the HSV rasters (where a value of 100 has been used for the saturation)
             *  on the image rectangle specified
             *  @param  theRect     the image rectangle on which to do the conversion
             */
            void convertToSaturatedRGBa_(const ImageRect* theRect);

            /** Calculates HSV values for the input gray-level image received as parameter.
             *  This function is rather trivial.  It arbitrarily set the hue to 0.
             *  The saturation is unambiguously set to 0 and the val to the gray level.
             *  @param  img gray-level image to base the HSV values on
             */
            void convertFromGray_(const RasterImage_gray* imgIn);

            /** Calculates HSV values for the input gray-level image received as parameter.
             *  This function is rather trivial.  It arbitrarily set the hue to 0.
             *  The saturation is unambiguously set to 0 and the val to the gray level.
             *  @param  img gray-level image to base the HSV values on
             *  @param  theRect image rectangle on which to do the conversion
             */
            void convertFromGray_(const RasterImage_gray* imgIn, const ImageRect* theRect);

            /** Calculates HSV values for the input RGBa image received as parameter.
             *  @param  img RGBa image to base the HSV values on
             */
            void convertFromRGBa_(const RasterImage_RGBa* imgIn);

            /** Calculates HSV values for the input RGBa image received as parameter.
             *  @param  img RGBa image to base the HSV values on
             *  @param  theRect image rectangle on which to do the conversion
             */
            void convertFromRGBa_(const RasterImage_RGBa* imgIn, const ImageRect* theRect);

            /** Calculates HSV values for the input aRGB16 image received as parameter.
             *  @param  img RGBa image to base the HSV values on
             */
            void convertFromARGB16_(const RasterImage_aRGB16* imgIn);

            /** Calculates HSV values for the input aRGB16 image received as parameter.
             *  @param  img RGBa image to base the HSV values on
             *  @param  theRect image rectangle on which to do the conversion
             */
            void convertFromARGB16_(const RasterImage_aRGB16* imgIn, const ImageRect* theRect);

            /** Calculates HSV values for the input RGB image received as parameter.
             *  @param  img RGB image to base the HSV values on
             */
            void convertFromRGB_(const RasterImage_RGB* imgIn);

            /** Calculates HSV values for the input RGB image received as parameter.
             *  @param  img RGB image to base the HSV values on
             *  @param  theRect image rectangle on which to do the conversion
             */
            void convertFromRGB_(const RasterImage_RGB* imgIn, const ImageRect* theRect);

            /**
             *  Forces an update of the base rgb raster if it is not the reference raster.
             */
            void updateRaster_(void) const;

	
			//------------
			// Friends
			//------------
			#if URIVL_LUT_RGB_TO_HSV
				friend bool uriVL::initializeForwardLUT_(void);
			#endif

			#if URIVL_LUT_HSV_TO_RGB
				friend bool uriVL::initializeBackwardLUT_(void);
			#endif

			friend class RasterImage_RGBa;

    };
}

#endif
