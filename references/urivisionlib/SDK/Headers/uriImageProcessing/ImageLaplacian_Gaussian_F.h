/*  NAME:
        ImageLaplacian_Gaussian_F.h
 
    DESCRIPTION:
        ImageLaplacian_Gaussian_F public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_LAPLACIAN_GAUSSIAN_F_H
#define URIVL_IMAGE_LAPLACIAN_GAUSSIAN_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageLaplacian.h"

namespace uriVL
{
    /**  Image operator that computes the Laplacian of a raster image using a
     *   "Laplacian of Gaussian" filter
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ImageLaplacian_Gaussian_F : public ImageLaplacian
    {
        public:

            /** Defines a Laplacian-of-Gaussian Filter with a specified scale
             *  The relations between scale, radius and width of the filter are
             *      width = (int)(5*scale)  and  width = 2*radius+1
             *
             *  @param  theScale    scale parameter sigma of the 2D Laplacian filter
             */
            ImageLaplacian_Gaussian_F(double theScale);

            /** Defines a Laplacian-of-Gaussian Filter with a specified "width".
             *  The scale of the filter is the square root of the variance of the
             *  associated distrinution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).<p>
             *  The actual width of the filter = 2*radius + 1<p>
             *  it is set to the smallest odd number greater or equal to the parameter
             *  received.
             *  @param  theWidth   desired width of the Laplacian filter
             */
            ImageLaplacian_Gaussian_F(int theWidth);

            /**
             *  Destructor
             */          
            ~ImageLaplacian_Gaussian_F(void);


			/**	Returns the scale of the operator
			 *	@return		scale of the operator
			 */
			float getScale(void) const;
			 
                    
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

            /** the scale sigma of this filter
             */
            double      scale_;

            /** Stores into a 1D array (radial distances squared) the filter's coefficients.
             */
            float       *fL_;
            
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ImageLaplacian_Gaussian_F(const ImageLaplacian_Gaussian_F& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ImageLaplacian_Gaussian_F& operator =(const ImageLaplacian_Gaussian_F& obj);


            /** Initializes the coefficients' array
             *  @see    fL_
             */
            void initializeFilter_(void);

            /** Hard-coded function that implements a 5x5 Laplacian of Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_5x5_(const RasterImage* imgIn, const ImageRect* theRect,
                                RasterImage* imgOut);

            /** Hard-coded function that implements a 7x7 Laplacian of Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_7x7_(const RasterImage* imgIn, const ImageRect* theRect,
                                RasterImage* imgOut);

            /** Hard-coded function that implements a 9x9 Laplacian of Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_9x9_(const RasterImage* imgIn, const ImageRect* theRect,
                                RasterImage* imgOut);

            /** Hard-coded function that implements a 11x11 Laplacian of Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_11x11_(const RasterImage* imgIn, const ImageRect* theRect,
                                RasterImage* imgOut);

            /** Hard-coded function that implements a 13x13 Laplacian of Gaussian filter.
             *
             *  @param  imgIn   the input image (must be gray-level so far)
             *  @param  imgOut  the output image (must be gray_F so far)
             *  @param  theRect the rectangle over which the filter must be applied
             *                  (it must overlap imgIn's valid data rectangle)
             */
            void applyInto_13x13_(const RasterImage* imgIn, const ImageRect* theRect,
                                  RasterImage* imgOut);

            /** Function that implements a generic (width greater than 13) Laplacian of
             *  Gaussian filter.
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

#endif  //  IMAGE_LAPLACIAN_GAUSSIAN_F_H
