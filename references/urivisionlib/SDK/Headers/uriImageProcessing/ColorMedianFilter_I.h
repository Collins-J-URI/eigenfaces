/*  NAME:
        ColorMedianFilter_I.h
 
    DESCRIPTION:
        ColorMedianFilter_I public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_COLOR_MEDIAN_FILTER_I_H
#define URIVL_COLOR_MEDIAN_FILTER_I_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ColorMedianFilter.h"

namespace uriVL
{
    /** ColorMedianFilter class
     *
     *  <b>This class has not been implemented yet</b>
     *
     *  A median filter is an image operator that replaces the value at each
     *  pixel by the median value of a rectangular window centered at that pixel
     */
    class URIVL_EXPORT ColorMedianFilter_I : public ColorMedianFilter
    {
        public:


            /** Defines a median filter with a specified width.
             *  The filter will be centered at the pixel where it is applied.
             *  @param  theWidth   desired width of the median filter
             */
            ColorMedianFilter_I(int theWidth);

            /** Defines a median filter with a specified width and height.
             *  The filter will be centered at the pixel where it is applied.
             *  @param  theWidth   desired width of the median filter
             *  @param  theHeight   desired width of the median filter
             */
            ColorMedianFilter_I(int theHeight, int theWidth);

            /** Defines a median filter with a dimensions rectangle.  It is up
             *  to the user to make sure that the dimensions of the filter make sense
             *  (the "left" and "top" values should be negative, opposite of "right"
             *  and bottom")
             *  @param  theRect desired dimensions of the median filter
             */
            ColorMedianFilter_I(const ImageRect* theRect);
            
            /** Destructor
             */
            ~ColorMedianFilter_I(void);

            /** Applies this operator to an image, returns the result as a RasterImage.
             *
             *  The RasterImage returned is of the same base type as the input image.
             *  For example, if imgIn was RasterImage_Gray_F, then so is the image
             *  returned.  
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            RasterImage* applyTo(const RasterImage* imgIn);

            /** Applies this operator to an image  within the rectangle specified and
             *  returns the result as a RasterImage.
             *
             *  The RasterImage returned is of the same form as the input image.
             *  For example, if imgIn was RasterImage_Gray_F, then so is the image
             *  returned.
             *
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            RasterImage* applyTo(const RasterImage* imgIn, const ImageRect* theRect);

            /** Applies this operator to a RasterImage and writes the result into a second
             *  RasterImage received as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            void applyInto(const RasterImage* imgIn, RasterImage* imgOut);
            
            /** Applies this operator to a RasterImage within the rectangle specified
             *  and writes the result into the RasterImage received as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            void applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut);
            
            
            
        protected:

            /** Local storage for red median computation
             */
            unsigned char* redData_;

            /** Local storage for green median computation
             */
            unsigned char* greenData_;

            /** Local storage for blue median computation
             */
            unsigned char* blueData_;
            
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ColorMedianFilter_I(const ColorMedianFilter_I& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ColorMedianFilter_I& operator =(const ColorMedianFilter_I& obj);
    };
}

#endif  //  COLOR_MEDIAN_FILTER_I_H