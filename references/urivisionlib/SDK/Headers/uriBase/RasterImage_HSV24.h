/*  NAME:
        RasterImage_HSV24.h

    DESCRIPTION:
        RasterImage_HSV24 public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RASTER_IMAGE_HSV24
#define     URIVL_RASTER_IMAGE_HSV24

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage_RGBa.h"

namespace uriVL
{
    /** A RasterImage_HSV24 is a RasterImage_RGBa that also stores the hue, saturation,
     *  and value for each pixel using only an <code>unsigned char</code> to store the
     *  hue, which forces us to scale it from [0, 359] to [0, 255].
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT RasterImage_HSV24 : public RasterImage_RGBa
    {
        public:
            /** Creates a RasterImage_HSV24 object at the specified dimensions
             *  @param  theRows     number of rows of the raster image to create
             *  @param  theCols     number of columns of the raster image to create
             */
            RasterImage_HSV24(int theRows, int theCols);

            /**
             *  Allocates an HSV24 raster image at the specified dimensions.
             *
             *  Allocates an HSV24 color raster at the specified dimensions and
             *  initializes this raster to zero values (black).
             *
             *  @param      theRect     ImageRect specifying the image's bounding rectangle
             */
            RasterImage_HSV24(ImageRect* theRect);

            /** Creates and initializes a raster image using the specified dimensions and
             *  values
             *
             *  @param  theRows     number of rows of the raster image to create
             *  @param  theCols     number of columns of the raster image to create
             *  @param  hue         1D raster storing hue values in the range [0, 359]
             *  @param  sat         1D raster storing saturation values in the range [0, 100]
             *  @param  val         1D raster storing the value in the range [0, 255]
             */
            RasterImage_HSV24(int theRows, int theCols, unsigned char* hue,
                            unsigned char* sat, unsigned char* val);

            /** Clonor constructor
             *  @param  img the image to clone
             */
            RasterImage_HSV24(RasterImage* img);

            /** Destructor
             */
            virtual ~RasterImage_HSV24(void);

            unsigned char* getHue(void);
            unsigned char* getSat(void);
            unsigned char* getVal(void);
            unsigned char** getHue2D(void);
            unsigned char** getSat2D(void);
            unsigned char** getVal2D(void);
            unsigned char** getShiftedHue2D(void);
            unsigned char** getShiftedSat2D(void);
            unsigned char** getShiftedVal2D(void);
            //
            static void rgb2HSV(unsigned char r, unsigned char g, unsigned char b,
                                unsigned char* hue, unsigned char* sat, unsigned char* val);
            static void rgb2HSV(unsigned char rgb[],
                                unsigned char* hue, unsigned char* sat, unsigned char* val);



        protected:
            unsigned char   *hue_;
            unsigned char   *sat_;
            unsigned char   *val_;
            unsigned char   **hue2D_;
            unsigned char   **sat2D_;
            unsigned char   **val2D_;
            unsigned char   **shiftedHue2D_;
            unsigned char   **shiftedSat2D_;
            unsigned char   **shiftedVal2D_;

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


            void convertFromRGBa_(void);
            void convertFromRGBa_(ImageRect* theRect);
//            void convertToRGBa_(void);
//            void convertToRGBa_(ImageRect* theRect);


        private:
            void allocateHSV_(int theRows, int theCols);
            void deleteHSV_(void);

    };
}

#endif  //  #ifndef     RASTER_IMAGE_HSV24

