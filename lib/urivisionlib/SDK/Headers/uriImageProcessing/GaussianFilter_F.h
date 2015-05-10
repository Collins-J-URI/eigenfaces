/*  NAME:
        GaussianFilter_F.h
 
    DESCRIPTION:
        GaussianFilter_F public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_GAUSSIAN_FILTER_F_H
#define URIVL_GAUSSIAN_FILTER_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "GaussianFilter.h"

namespace uriVL
{
    /** Gaussian filter class (float operator).
	 *	Note that in the current implementation of this class the same scale is
	 *	applied along the x and y directions.  Implementing a more general form
	 *	is a low-priority item at this point.
     */
    class URIVL_EXPORT GaussianFilter_F : public GaussianFilter
    {
        public:

            /** Defines a Gaussian filter with the specified scale
             *  The relations between scale, radius and width of the filter are
             *      width = (int)(5*scale)  and  width = 2*radius+1
             *
             *  @param  theScale    scale parameter sigma of the 2D Gaussian filter
             */
            GaussianFilter_F(double theScale);

            /** Defines a Gaussian filter with the specified width.
             *  The scale of the filter is the square root of the variance of the
             *  associated distrinution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).<p>
             *  The actual width of the filter = 2*radius + 1<p>
             *  it is set to the smallest odd number greater or equal to the parameter
             *  received.
             *  @param  theWidth   desired width of the Gaussian filter
             */
            GaussianFilter_F(int theWidth);
            
            /** Creates a Gaussian filter with the specified scale
             *
             *  The scale of the filter is the square root of the variance of the
             *  associated distribution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).
             *
             *  This constructor is useful when processing a set of images that have
             *  the same dimensions.  It allows reusing the same temporary
             *  raster without losing time re-allocating and deleting each 
             *  at each call.
             *  @param  scale   scale sigma of the Gaussian filter
             *  @param theRect  bounding rectangle for the temporary raster
             */
            GaussianFilter_F(double theScale, const ImageRect* theRect);

            /** Creates a Gaussian filter with the specified width
             *
             *  The scale of the filter is the square root of the variance of the
             *  associated distribution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).<p>
             *  The actual width of the filter = 2*radius + 1<p>
             *  it is set to the smallest odd number greater or equal to the parameter
             *  received.
             *
             *  This constructor is useful when processing a set of images that have
             *  the same dimensions.  It allows reusing the same temporary
             *  raster without losing time re-allocating and deleting each 
             *  at each call.
             *  @param  theWidth   desired width of the Gaussian filter
             *  @param theRect  bounding rectangle for the temporary raster
             */
            GaussianFilter_F(int theWidth, const ImageRect* theRect);

            /** Destructor
             */
            ~GaussianFilter_F(void);

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

            /** Stores into a 1D array (radial distances squared) the filter's coefficients.
             */
            float*	fg_;
            
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            GaussianFilter_F(const GaussianFilter_F& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const GaussianFilter_F& operator =(const GaussianFilter_F& obj);

            /** Initializes the coefficients' array
             *  @see    fg_
             */
            void initializeFilter_(void);

            /** Hard-coded function that implements a 3x3 Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_3x3_(const RasterImage* imgIn, const ImageRect* theRect,
                                RasterImage* imgOut);

            /** Hard-coded function that implements a 5x5 Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_5x5_(const RasterImage* imgIn, const ImageRect* theRect,
                                RasterImage* imgOut);

            /** Hard-coded function that implements a 7x7 Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_7x7_(const RasterImage* imgIn, const ImageRect* theRect,
                                RasterImage* imgOut);

            /** Hard-coded function that implements a 9x9 Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_9x9_(const RasterImage* imgIn, const ImageRect* theRect,
                                RasterImage* imgOut);

            /** Hard-coded function that implements a 11x11 Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_11x11_(const RasterImage* imgIn, const ImageRect* theRect,
                                RasterImage* imgOut);

            /** Hard-coded function that implements a 13x13 Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_13x13_(const RasterImage* imgIn, const ImageRect* theRect,
                                  RasterImage* imgOut);

            /** Function that implements a generic (width greater than 13) Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_(const RasterImage* imgIn, const ImageRect* theRect,
                            RasterImage* imgOut);

    };
}

#endif  //  GAUSSIAN_FILTER_F_H
