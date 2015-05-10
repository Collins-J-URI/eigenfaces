/*  NAME:
        ZeroCrossingDetector.h
 
    DESCRIPTION:
        ZeroCrossingDetector public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ZERO_CROSSING_DETECTOR_H
#define URIVL_ZERO_CROSSING_DETECTOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Labeler.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGBa.h"


namespace uriVL
{
    /** ZeroCrossingDetector class.
     *
     *  This class is implemented as an instantiable static class.  It is instantiable
     *  because it may be useful at some point to typecast a <code>ZeroCrossingDetector</code>
     *  object as a <code>labeler</code>.
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ZeroCrossingDetector : public Labeler
    {
        public:

            /** 
             *  Default constructor.
             *
             */
            ZeroCrossingDetector(void);


            /**
             *  Destructor
             */          
            ~ZeroCrossingDetector(void);


            /** Detects the zero crossings in the input image, returns the result as a 
             *  LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      ignored by this function
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            LabelRaster* applyTo(const RasterImage* imgIn, const char* params);


            /** Detects the zero crossings in the input image, returns the result as a 
             *  LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            LabelRaster *applyTo(const RasterImage* imgIn);


            /** Detects the zero crossings in the input image, returns the result as a 
             *  LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      ignored by this function
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            LabelRaster* applyTo(const RasterImage* imgIn, const char* params, const ImageRect* theRect);


            /** Detects the zero crossings in the input image, returns the result as a 
             *  LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      ignored by this function
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            LabelRaster* applyTo(const RasterImage* imgIn, const ImageRect* theRect);


            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      ignored by this function
              * @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const char* params, LabelRaster* labelOut);


            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
              * @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            void applyInto(const RasterImage* imgIn, LabelRaster* labelOut);

            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      ignored by this function
             *  @param  theRect     the rectangle within which to apply the labeler
              * @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const char* params, const ImageRect* theRect, 
                                    LabelRaster *labelOut);

            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the labeler
              * @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const ImageRect* theRect,  LabelRaster* labelOut);

            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      ignored by this function
             *  @param  imgOut      a RasterImage version of the label raster obtained by 
             *                      applying the operator to imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const char* params, RasterImage* imgOut);

            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  imgOut      a RasterImage version of the label raster obtained by 
             *                      applying the operator to imgIn
             */ 
            void applyInto(const RasterImage* imgIn, RasterImage* imgOut);

            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      ignored by this function
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  imgOut      a RasterImage version of the label raster obtained by 
             *                      applying the operator to imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const char* params, const ImageRect* theRect, 
                                    RasterImage* imgOut);

            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  imgOut      a RasterImage version of the label raster obtained by 
             *                      applying the operator to imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut);


            /** Detects the zero crossings in the input image, returns the result as a 
             *  LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            static LabelRaster* applyTo_st(const RasterImage* imgIn);


            /** Detects the zero crossings in the input image, returns the result as a 
             *  LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      ignored by this function
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            static LabelRaster* applyTo_st(const RasterImage* imgIn, const ImageRect* theRect);


            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
              * @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            static void applyInto_st(const RasterImage* imgIn, LabelRaster* labelOut);

            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the labeler
              * @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            static void applyInto_st(const RasterImage* imgIn, const ImageRect* theRect,  LabelRaster* labelOut);

            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  imgOut      a RasterImage version of the label raster obtained by 
             *                      applying the operator to imgIn
             */ 
            static void applyInto_st(const RasterImage* imgIn, RasterImage* imgOut);

            /** Detects the zero crossings in the input image, writes the result into the 
             *  LabelRaster provided as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  imgOut      a RasterImage version of the label raster obtained by 
             *                      applying the operator to imgIn
             */ 
            static void applyInto_st(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut);

            /** Displays the sign of the data in the input image under the form of a 
             *  color raster image (RGBa).  This is mostly a debugging function,
             *  to be applied to the entire image.  It is very specific in the type
             *  of its parameters.  The output image is colored
             *  <ul>
             *      <li> blue where imgIn is positive
             *      <li> green where imgIn is negative
             *      <li> red where imgIn is zero
             *  </ul>
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  imgOut      the color RasterImage indicating the sign of imgIn
             *  @param  colorChoice color attributions for positive, negative, zero
             */
            static void showSignAsColor(const RasterImage_gray_F* imgIn, RasterImage_RGBa* imgOut,
                                        const unsigned char colorChoice[][3]);

            
        private:

            /** This is the function that actually does the job, trusting the caller
             *  functions to have done all the data type and validity checking.         
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  labelOut    the LabelRaster obtained by applying the operator to
             *                          imgIn
             */ 
            static void applyInto_(const RasterImage_gray* imgIn, const ImageRect* theRect, 
                            LabelRaster* labelOut);

            /** This is the function that actually does the job, trusting the caller
             *  functions to have done all the data type and validity checking.         
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  imgOut      a RasterImage version of the label raster obtained by 
             *                      applying the operator to imgIn
             */ 
            static void applyInto_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   RasterImage* imgOut);
            
    };
}

#endif  //  ZERO_CROSSING_DETECTOR_H
