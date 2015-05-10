/*  NAME:
        ImageHistogram.h

    DESCRIPTION:
        ImageHistogram public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef URIVL_IMAGE_HISTOGRAM_H
#define URIVL_IMAGE_HISTOGRAM_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Histogram.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"

namespace uriVL
{
    /** Implements the ImageHistogram class.
     *
     *  This class computes, stores, and returns a 1D array of <code>long</code>
     *  storing the histogram of a given raster image.  It can also render this
     *  histogram in a window.  You should use this class if you want to display
     *  an image's histogram (for verification purposes) or want to do your own
     *  calculations with the histogram array.  Otherwise, the
     *  <code>ImageHistogramCalculator</code> class provides a number of functions.
     *  such as median computation and the generation of gray thresholding regions
     *
     *  So far the class does not provide any means to set the values of the various range
     *  parameters because I haven't quite decided yet what the API for this should be.
     *  So, for the time being the class only really works with RasterImage_gray objects
     *  (converts to that format any other type of image) and uses the 256 values in the
     *  range 0-255 to calculate the histogram.
     *
     *  Version history
     *      - 2007/03/16 [jyh]      first implementation.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ImageHistogram : public Histogram
    {
        public:

            /** Constructor taking in a RasterImage and possibly an ImageRect over which
             *  to compute the histogram.  Determines whether a floating point or unsigned
             *  char histogram should be used, and possibly allocates a local gray raster
             *  if theimage sent in was a color image.  Uses default values for the range
             *  [0, 256[ and the number of values of the histogram.
             *  <p>Users who know what they are doing should instead one of the next two
             *  constructors.
             *
             *  @param  img   The image of which to calculate the histogram
             *  @param  rect  Partial rectangle over which to compute the histogram.
             *                  When NULL, the image's entire validRect is used
             */
            ImageHistogram(RasterImage* img, const ImageRect* rect=NULL);


            /** Constructor taking in a RasterImage_gray, range values, and possibly an 
             *  ImageRect over which to compute the histogram.
             *
             *  @param  img         The gray-level image of which to calculate the histogram
             *  @param  rect        Partial rectangle over which to compute the histogram.
             *                          When NULL, the image's entire validRect is used
             *  @param  nbVals      The number of values/slots in the histogram
             *  @param  rangeMin    The lower bound of values over the image
             *  @param  rangeMax    The upper bound of values over the image
             */
            ImageHistogram(RasterImage_gray* img, 
                           const ImageRect* rect=NULL,
                           int nbVals = HIST_DEFAULT_NB_VALS,
                           unsigned char rangeMin = HIST_DEFAULT_RANGE_MIN_UC,
                           unsigned char rangeMax = HIST_DEFAULT_RANGE_MAX_UC);


            /** Constructor taking in a RasterImage_gray_F, range values, and possibly an 
             *  ImageRect over which to compute the histogram.
             *
             *  @param  img         The gray-level image of which to calculate the histogram
             *  @param  rect        Partial rectangle over which to compute the histogram.
             *                          When NULL, the image's entire validRect is used
             *  @param  nbVals      The number of values/slots in the histogram
             *  @param  rangeMin    The lower bound of values over the image
             *  @param  rangeMax    The upper bound of values over the image
             */
            ImageHistogram(RasterImage_gray_F *img, 
                           const ImageRect* rect=NULL,
                           int nbVals = HIST_DEFAULT_NB_VALS,
                           float rangeMin = HIST_DEFAULT_RANGE_MIN,
                           float rangeMax = HIST_DEFAULT_RANGE_MAX);

            /**
             *  Destructor
             */
            ~ImageHistogram(void);

            /** returns the histogram values cast to a pointer to char.  The histogram
             *  can be a single array of long (gray image) or a 3 1D arrays of long.  The
             *  user should call getType() befefore using this function.  You are probably
             *  better off calling getGrayHistogram or getColorHistogram;
             *  @return     histogram values
             */
            long *getHistogram(void) const;


            /** Defines the rendering mode for the image histogram.
             *  In my opinion a polar rendering makes no sense for a gray-level image, 
             *  so we only accept LIN_LIN and LIN_LOG modes.  Other mode selections are
             *  simply ignored.
             *  @param  renderMode  rendering mode
             *  @see renderMode_
             */
            void setRenderMode(HistogramRenderMode mode);
            

            /** Assigns a new RasterImage and possibly an ImageRect over which
             *  to compute the histogram.  Determines whether a floating point or unsigned
             *  char histogram should be used, and possibly allocates a local gray raster
             *  if theimage sent in was a color image.  Uses default values for the range
             *  [0, 256[ and the number of values of the histogram.
             *  <p>Users who know what they are doing should instead one of the next two
             *  constructors.
             *
             *  @param  img   The image of which to calculate the histogram
             *  @param  rect  Partial rectangle over which to compute the histogram.
             *                  When NULL, the image's entire validRect is used
             */
            void setImage(RasterImage* img, const ImageRect* rect=NULL);


            /** Assigns a new RasterImage_gray and possibly an ImageRect, and range values 
             *   over which to compute the histogram.
             *
             *  @param  img         The gray-level image of which to calculate the histogram
             *  @param  rect        Partial rectangle over which to compute the histogram.
             *                          When NULL, the image's entire validRect is used
             *  @param  nbVals      The number of values/slots in the histogram
             *  @param  rangeMin    The lower bound of values over the image
             *  @param  rangeMax    The upper bound of values over the image
             */
            void setImage(RasterImage_gray* img, 
                          const ImageRect* rect=NULL,
                          int nbVals = HIST_DEFAULT_NB_VALS,
                          unsigned char rangeMin = HIST_DEFAULT_RANGE_MIN_UC,
                          unsigned char rangeMax = HIST_DEFAULT_RANGE_MAX_UC);


            /** Assigns a new RasterImage_gray and possibly an ImageRect, and range values 
             *   over which to compute the histogram.
             *
             *  @param  img         The gray-level image of which to calculate the histogram
             *  @param  rect        Partial rectangle over which to compute the histogram.
             *                          When NULL, the image's entire validRect is used
             *  @param  nbVals      The number of values/slots in the histogram
             *  @param  rangeMin    The lower bound of values over the image
             *  @param  rangeMax    The upper bound of values over the image
             */
            void setImage(RasterImage_gray_F *img, 
                          const ImageRect* rect=NULL,
                          int nbVals = HIST_DEFAULT_NB_VALS,
                          float rangeMin = HIST_DEFAULT_RANGE_MIN,
                          float rangeMax = HIST_DEFAULT_RANGE_MAX);


        protected:

            /** Default constructor.
             */
            ImageHistogram(void);

            /** The function that does all the work for a regular (unsigned char) gray-level
             *  image, using the default histogram parameters (no scaling)
             */
            void computeFastHistogram_(RasterImage_gray* img, const ImageRect* rect);

            /** The function that does all the work for a regular (unsigned char) gray-level
             *  image, using custom histogram parameters (involving scaling and shifting)
             */
            void computeHistogram_(RasterImage_gray* img, const ImageRect* rect);

            /** The function that does all the work for a float gray-level
             *  image, using custom histogram parameters (involving scaling and shifting)
             */
            void computeFloatHistogram_(RasterImage_gray_F *img, const ImageRect* rect);


    };
}   //  namespace uriVL

#endif  //  IMAGE_HISTOGRAM_H
