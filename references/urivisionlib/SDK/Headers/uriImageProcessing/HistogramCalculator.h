/*  NAME:
        HistogramCalculator.h
 
    DESCRIPTION:
        HistogramCalculator public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_HISTOGRAM_CALCULATOR_H
#define URIVL_HISTOGRAM_CALCULATOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Histogram.h"
#include "RasterImage_gray.h"
#include "ColorSegParams.h"

namespace uriVL
{
    /** An HistogramCalculator computes the histogram of RasterImage objects.
     *  So far, only RasterImage_gray images are handled.   I have made the class
     *  non-instantiable because it is not derived from one of the major image
     *  processing classes (<code>ImageOperator</code>, <code>VectorOperator</code>,
     *  <code>Labeler</code>).  The <code>HistogramCalculator</code> class can
     *  produce an histogram either as an array of long ints or as a RasterImage to
     *  be rendered in a window.
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class HistogramCalculator
    {
        public:

            /** Default constructor
             */
            HistogramCalculator(void);
            
            /** Destructor
             */
            virtual ~HistogramCalculator(void);

            /** computes an image's histogram and stores it as an instance variable.
             *  @param  img   The image of which to calculate and store the histogram 
             *  @see    hist_
             */
            void computeHistogram(RasterImage_gray* img);

            
            /** Produces the segmentation parameter structure of a low-pass filter
             *  for the image whose histogram is stored as instance variable.  The
             *  threshold of the filter is specified by the percentage
             *  received as parameter.
             *  @param    threshRatio     percentage of 
             */
            ColorSegParams *getLowpassFilterParams(unsigned char threshRatio);

            /** Produces the segmentation parameter structure of a hysteresis thresholding
             *  filter (like the one of a Canny edge detector)
             *  for the image whose histogram is stored as instance variable.  The
             *  threshold of the filter is specified by the percentage
             *  received as parameter.  The "low" threshold value is automatically
             *  calculated as the midpoint of the "high" threshold and of the min value
             *  of the raster image.
             *  @param    threshRatio     percentage level for "high" value
             */
            ColorSegParams *getHysteresisFilterParams(unsigned char threshRatio);

            /** returns an image's histogram
             *  @param  img   The image of which to calculate the histogram 
             *  @return     histogram of the image
             */
            static Histogram *getHistogram(RasterImage_gray* img);

            
            /** writes the image's histogram into the object passed as parameter 
             *  @param  img         The image of which to calculate the histogram 
             *  @param  histOut     output histogram
             */
            static void getHistogram(RasterImage_gray* img, Histogram *histOut);

            

            /** returns the image's histogram with color settings for rendering
             *  @param  img   The image of which to calculate the histogram 
             *  @param  backgroundColor     rgb color for the image background
             *  @param  histogramColor      rgb color of the histogram
             *  @param  axesColor           rgb color of the histogram's axes
             *  @param  labelColor          rgb color of the histograms' axes
             *  @param  titleColor          rgb color of the histograms' title
             *  @return     histogram of the image
             */
            static Histogram *getHistogram(RasterImage_gray* img, const float* backgroundColor,
                                           const float* histogramColor, const float* axesColor,
                                           const float* labelColor, const float* titleColor);


            /** writes the image's histogram into the object passed as parameter,
             *  with color settings for rendering
             *  @param  img   The image of which to calculate the histogram 
             *  @param  backgroundColor     rgb color for the image background
             *  @param  histogramColor      rgb color of the histogram
             *  @param  axesColor           rgb color of the histogram's axes
             *  @param  labelColor          rgb color of the histograms' axes
             *  @param  titleColor          rgb color of the histograms' title
             *  @param  histOut             output histogram
             */
            static void getHistogram(RasterImage_gray* img, const float* backgroundColor,
                                     const float* histogramColor, const float* axesColor, 
                                     const float* labelColor, const float* titleColor, 
                                     Histogram *histOut);


            /** returns the histogram as a raster image, using default colors
             *  @param  img   The image of which to calculate the histogram 
             *  @return     histogram as a raster image
             */
            static RasterImage_RGBa* getHistogramAsRasterImage(RasterImage_gray* img);
            

            /** renders the input image's histogram into the image passed as parameter.
             *  using default colors
             *  @param  img   The image of which to calculate the histogram 
             *  @param  histOut             Output histogram
             */
            static void getHistogramAsRasterImage(RasterImage_gray* img, RasterImage_RGBa* histOut);
            

            /** returns the histogram as a raster image
             *  @param  img   The image of which to calculate the histogram 
             *  @param  backgroundColor     rgb color for the image background
             *  @param  histogramColor      rgb color of the histogram
             *  @param  axesColor           rgb color of the histogram's axes
             *  @return     histogram as a raster image
             */
            static RasterImage_RGBa* getHistogramAsRasterImage(RasterImage_gray* img,
                                                               const float* backgroundColor,
                                                               const float* histogramColor,
                                                               const float* axesColor);

            /** renders the input image's histogram into the image passed as parameter,
             *  using the redering parameters specified.
             *  @param  img   The image of which to calculate the histogram 
             *  @param  backgroundColor     rgb color for the image background
             *  @param  histogramColor      rgb color of the histogram
             *  @param  axesColor           rgb color of the histogram's axes
             *  @param  imgOut              Output image
             */
            static void getHistogramAsRasterImage(RasterImage_gray* img,
                                                  const float* backgroundColor,
                                                  const float* histogramColor,
                                                  const float* axesColor,
                                                  RasterImage_RGBa* imgOut);

            /** returns the histogram as a labeled raster image, that is, with
             *  text giving the range and scale of the histogram.
             *  @param  img   The image of which to calculate the histogram 
             *  @return     histogram as a raster image
             */
            static RasterImage_RGBa* getHistogramAsLabeledRasterImage(RasterImage_gray* img);
            
           
            /** renders the input image's histogra as a labeled raster image, 
             *  into the image passed as parameter.
             *  @param  img   The image of which to calculate the histogram 
             *  @param  imgOut              output image
             */
            static void getHistogramAsLabeledRasterImage(RasterImage_gray* img, 
                                                         RasterImage_RGBa* imgOut);
            
           
            /** returns the histogram as a labeled raster image, that is, with
             *  text giving the range and scale of the histogram.
             *  @param  img   The image of which to calculate the histogram 
             *  @param  backgroundColor     rgb color for the image background
             *  @param  histogramColor      rgb color to draw the histogram in
             *  @param  axesColor           rgb color to draw the histogram's axes in
             *  @param  textColor           rgb color to draw the text (range, values) in
             *  @return     histogram as a labeled raster image
             */
            static RasterImage_RGBa* getHistogramAsLabeledRasterImage(RasterImage_gray* img,
                                                              const float* backgroundColor,
                                                              const float* histogramColor,
                                                              const float* axesColor,
                                                              const float* labelColor,
                                                              const float* titleColor);

            /** returns the histogram as a labeled raster image, that is, with
             *  text giving the range and scale of the histogram.
             *  @param  img   The image of which to calculate the histogram 
             *  @param  backgroundColor     rgb color for the image background
             *  @param  histogramColor      rgb color to draw the histogram in
             *  @param  axesColor           rgb color to draw the histogram's axes in
             *  @param  textColor           rgb color to draw the text (range, values) in
             *  @param  imgOut              output image
             */
            static void getHistogramAsLabeledRasterImage(RasterImage_gray* img,
                                                         const float* backgroundColor,
                                                         const float* histogramColor,
                                                         const float* axesColor,
                                                         const float* labelColor,
                                                         const float* titleColor,
                                                         RasterImage_RGBa* imgOut);
            
        protected:
        
            /** Stores the <code>Histogram</code> object calculated through a call of
             *  <code>computeHistogram</code>.  This is the histogram that will be used to
             *  compute gray thresholding regions.<p>
             *  It goes without saying that you should keep track of your calls to
             *  <code>computeHistogram</code> so that this instance variable actually stores
             *  the histogram you think it stores (i.e. that of the right image).
             *  @see   computeHistogram
             */
            Histogram   *hist_;

    };
}

#endif  //  HISTOGRAM_CALCULATOR_H
