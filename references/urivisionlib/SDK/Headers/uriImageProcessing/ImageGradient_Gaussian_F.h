/*  NAME:
        ImageGradient_Gaussian_F.h
 
    DESCRIPTION:
        ImageGradient_Gaussian_F public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_GRADIENT_GAUSSIAN_F_H
#define URIVL_IMAGE_GRADIENT_GAUSSIAN_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageGradient.h"

namespace uriVL
{
    /** Vector operator that computes the gradient of a raster image using
     *  a "gradient of Gaussian" filter.  Objects of this class only operate
     *  on gray-level images.  If a color image is sent as parameter, it gets
     *  converted to a gray-level image first.  For operation on color
     *  images, see <code>ColorImageGradient</code> subclasses.
     *
	 *	Note that in the current implementation of this class the same scale is
	 *	applied along the x and y directions.  Implementing a more general form
	 *	is a low-priority item at this point.
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ImageGradient_Gaussian_F : public ImageGradient
    {
        public:

            /** Creates a gradient-of-Gaussian filter with the specified scale
             *
             *  The scale of the filter is the square root of the variance of the
             *  associated distribution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).
             *  @param  scale   scale sigma of the Gaussian filter
             */
            ImageGradient_Gaussian_F(double theScale);

            /** Creates a gradient-of-Gaussian filter with the specified width
             *
             *  The scale of the filter is the square root of the variance of the
             *  associated distribution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).<p>
             *  The actual width of the filter = 2*radius + 1<p>
             *  it is set to the smallest odd number greater or equal to the parameter
             *  received.
             *  @param  theWidth   desired width of the Gaussian filter
             */
            ImageGradient_Gaussian_F(int theWidth);
            
            /** Creates a gradient-of-Gaussian filter with the specified scale
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
            ImageGradient_Gaussian_F(double theScale, const ImageRect* theRect);

            /** Creates a gradient-of-Gaussian filter with the specified width
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
            ImageGradient_Gaussian_F(int theWidth, const ImageRect* theRect);

            /**
             *  Destructor
             */          
            ~ImageGradient_Gaussian_F(void);

			/**	Returns the scale of the operator
			 *	@return		scale of the operator
			 */
			float getScale(void) const;
			 
                    
            /** Applies this operator to a gray-level version of the input image
             *  and returns the result as a VectorField_F.
             *        
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the VectorField obtained by applying the operator to
             *                      imgIn
             */ 
            VectorField* applyTo(const RasterImage* imgIn);

            /** Applies this operator to a gray-level version of the input image
             *  within the rectangle specified as second parameter
             *  and returns the result as a VectorField_F.
             *        
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the VectorField obtained by applying the operator to
             *                      imgIn
             */ 
            VectorField* applyTo(const RasterImage* imgIn, const ImageRect* theRect);

            /** Applies this operator to a gray-level version of the input image
             *  and writes the result into a VectorField_F received as second parameter.
             *        
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyInto(const RasterImage* imgIn, VectorField* vectOut);

            /** Applies this operator to a gray-level version of the input image
             *  within the rectangle specified as second parameter
             *  and writes the result into a VectorField_F received as third parameter.
             *        
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyInto(const RasterImage* imgIn, const ImageRect* theRect, VectorField* vectOut);

        private:
        
            /** Scale parameter (sigma) of the filter
             */
            double	scale_;

            /** Stores into a 1D array the filter coefficients for the Gaussian.
             */
            float*	fg_;
            
            /** Stores into a 1D array the filter coefficients for the Gaussian's derivative.
             */
            float*	fgD_;
            
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ImageGradient_Gaussian_F(const ImageGradient_Gaussian_F& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ImageGradient_Gaussian_F& operator =(const ImageGradient_Gaussian_F& obj);

            /** Hard-coded function that computes the x component of a 3x3 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvx     x component of the output field
             */
            void applyInto_3x3_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvx);

            /** Hard-coded function that computes the x component of a 3x3 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvy     y component of the output field
             */
            void applyInto_3x3_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvy);

            /** Hard-coded function that computes the x component of a 5x5 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvx     x component of the output field
             */
            void applyInto_5x5_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvx);

            /** Hard-coded function that computes the y component of a 5x5 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvy     y component of the output field
             */
            void applyInto_5x5_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvy);

            /** Hard-coded function that computes the x component of a 7x7 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvx     x component of the output field
             */
            void applyInto_7x7_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvx);

            /** Hard-coded function that computes the y component of a 7x7 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvy     y component of the output field
             */
            void applyInto_7x7_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvy);

            /** Hard-coded function that computes the x component of a 9x9 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvx     x component of the output field
             */
            void applyInto_9x9_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvx);

            /** Hard-coded function that computes the y component of a 9x9 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvy     y component of the output field
             */
            void applyInto_9x9_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvy);

            /** Hard-coded function that computes the x component of an 11x11 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvx     x component of the output field
             */
            void applyInto_11x11_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvx);

            /** Hard-coded function that computes the y component of an 11x11 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvy     y component of the output field
             */
            void applyInto_11x11_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvy);

            /** Hard-coded function that computes the x component of a 13x13 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvx     x component of the output field
             */
            void applyInto_13x13_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvx);

            /** Hard-coded function that computes the y component of a 13x13 gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvy     y component of the output field
             */
            void applyInto_13x13_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvy);

            /** Function that computes the x component of a generic (width greater than 13)
             *  gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvx     x component of the output field
             */
            void applyInto_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                   float*const* fvx);

            /** Function that computes the y component of a generic (width greater than 13)
             *  gradient filter.
             *
             *  @param  imgIn   the input image
             *  @param  theRect the rectangle over which the filter must be applied
             *  @param  fvy     y component of the output field
             */
            void applyInto_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                            float*const* fvy);

            /** Initializes the coefficients' array
             *  @see    fg_
             *  @see    fgD_
             */
            void initializeFilter_(void);
                
    };
}

#endif  //  IMAGE_GRADIENT_GAUSSIAN_F_H
