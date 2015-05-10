/*  NAME:
        MedianFilter_F.h
 
    DESCRIPTION:
        MedianFilter_F public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_MEDIAN_FILTER_F_H
#define URIVL_MEDIAN_FILTER_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "MedianFilter.h"

namespace uriVL
{
    /** MedianFilter class
     *
     *  A median filter is an image operator that replaces the value at each
     *  pixel by the median value of a rectangular window centered at that pixel
     */
    class URIVL_EXPORT MedianFilter_F : public MedianFilter
    {
        public:

            /** Defines a median filter with a specified width.
             *  The filter will be centered at the pixel where it is applied.
             *  @param  theWidth   desired width of the median filter
             */
            MedianFilter_F(int theWidth);

            /** Defines a median filter with a specified width and height.
             *  The filter will be centered at the pixel where it is applied.
             *  @param  theWidth   desired width of the median filter
             *  @param  theHeight   desired width of the median filter
             */
            MedianFilter_F(int theWidth, int theHeight);

            /** Defines a median filter with a dimensions rectangle.  It is up
             *  to the user to make sure that the dimensions of the filter make sense
             *  (the "left" and "top" values should be negative, opposite of "right"
             *  and bottom")
             *  @param  theRect desired dimensions of the median filter
             */
            MedianFilter_F(const ImageRect* theRect);
            
            /** Destructor
             */
            ~MedianFilter_F(void);

            /** Applies this operator to an image, returns the result as a RasterImage.
             *
             *  The RasterImage returned is of the "float" form of the input image.
             *  For example, if imgIn was RasterImage_Gray, then the image
             *  returned should be RasterImage_Gray_F.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            RasterImage* applyTo(const RasterImage* imgIn);

            /** Applies this operator to an image  within the rectangle specified and
             *  returns the result as a RasterImage.
             *
             *  The RasterImage returned is of the "float" form of the input image.
             *  For example, if imgIn was RasterImage_Gray, then the image
             *  returned should be RasterImage_Gray_F.
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


        private:                
            
			/**	Private data array for median computation
			 */
            float* data_;
            
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            MedianFilter_F(const MedianFilter_F& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const MedianFilter_F& operator =(const MedianFilter_F& obj);
    };
}

#endif  //  MEDIAN_FILTER_F_H