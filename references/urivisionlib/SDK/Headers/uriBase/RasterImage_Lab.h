/*  NAME:
        RasterImage_Lab.h

    DESCRIPTION:
        RasterImage_Lab public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RASTER_IMAGE_LAB_H
#define     URIVL_RASTER_IMAGE_LAB_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage.h"

namespace uriVL
{
    class URIVL_EXPORT RasterImage_Lab : public RasterImage
    {
        public:
            RasterImage_Lab( short nbRows, short nbCols );
            RasterImage_Lab( RasterImage*  img );
            ~RasterImage_Lab();
            float** getRaster();
            float* getL();
            float* getA();
            float* getB();

        private:
            float   *L_;
            float   *a_;
            float   *b_;
            float   **L2D_;
            float   **a2D_;
            float   **b2D_;

            void convertFromRGB_();
            void convertFromRGB_( ImageRect*  theRect );
            void convertToRGB_( ImageRect*  theRect );
            void convertToRGB_();

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

    };
}

#endif
