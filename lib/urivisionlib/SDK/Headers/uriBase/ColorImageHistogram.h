/*  NAME:
        ColorImageHistogram.h

    DESCRIPTION:
        ColorImageHistogram public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef URIVL_COLOR_IMAGE_HISTOGRAM_H
#define URIVL_COLOR_IMAGE_HISTOGRAM_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageHistogram.h"
#include "RasterImage_HSV.h"

namespace uriVL
{
    /** Enumerated types for all kinds of histogram computations supported by this
     *  class
     *      <ul><li>RGB_HISTOGRAM - three separate histogram for r, g, b components
     *          <li>HSV_HISTOGRAM - three separate histograms for hue, sat, val
     *      </ul>
     */
    typedef enum ColorImageHistogramType {
                                    DEFAULT_TYPE = 100,
                                    RGB_HISTOGRAM,
                                    HSV_HISTOGRAM
    } ColorImageHistogramType;
    

    /** So far only a place holder for the ColorImageHistogram class.
     *
     *  This class <i>should/will</i> compute, store, and return arrays of <code>long</code>
     *  storing the histograms of a given color raster image.  It can also render this
     *  histogram in a window.  You should use this class if you want to display
     *  an image's histogram (for verification purposes) or want to do your own
     *  calculations with the histogram array.
     *
     *  I haven't quite figured out whether this call should be derived from ImageHistogram, Histogram,
     *  or DrawableObject2D.  I haven't decided either whether I will be using iheritance or composition
     *  to implement histogram calculations in different color spaces.  Inheritance seems more flexible
     *  in this regard.     
     *
     *  Version history
     *      - 2007/03/21 [jyh]      first implementation.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ColorImageHistogram //   : public Histogram
    {
        public:
            /** Constructor.
             *  Takes in the image whose histogram is needed.
             *
             *  @param  img   The image of which to calculate the histogram
             *  @param  histType  The type of histogram.  By default, the image's "natural"
             *                  histogram type is selected.
             */
            ColorImageHistogram(RasterImage* img, ColorImageHistogramType histType=DEFAULT_TYPE);

            /** Constructor.
             *  Takes in the image whose histogram is needed.
             *
             *  @param  img   The image of which to calculate the histogram
             *  @param  rect  Partial rectangle over which to compute the histogram.
             *                  When NULL, the image's entire validRect is used
             *  @param  histType  The type of histogram.  By default, the image's "natural"
             *                  histogram type is selected.
             */
            ColorImageHistogram(RasterImage* img, const ImageRect* rect, 
                                ColorImageHistogramType histType=DEFAULT_TYPE);


            /**
             *  Destructor
             */
            ~ColorImageHistogram(void);

            /** Returns the type of this histogram
             *  @return type of the histogram
             */
            ColorImageHistogramType getType(void) const;
            

        protected:

            /** Type of the histogram
             */
            ColorImageHistogramType histType_;
            
            
            /** Default constructor.
             */
            ColorImageHistogram(void);


    };
}   //  namespace uriVL

#endif  //  COLOR_IMAGE_HISTOGRAM_H
