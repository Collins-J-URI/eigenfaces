/*  NAME:
        ColorImageGradient_Gaussian_I.h
 
    DESCRIPTION:
        ColorImageGradient_Gaussian_I public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_COLOR_IMAGE_GRADIENT_GAUSSIAN_I_H
#define URIVL_COLOR_IMAGE_GRADIENT_GAUSSIAN_I_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ColorImageGradient.h"

namespace uriVL
{
    /** Vector operator that computes the gradient of a raster image using
     *  a "gradient of Gaussian" filter.
     *
     *  This class has not been implemented yet
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ColorImageGradient_Gaussian_I : public ColorImageGradient
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
            ColorImageGradient_Gaussian_I(double theScale);

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
            ColorImageGradient_Gaussian_I(int theWidth);

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
            ColorImageGradient_Gaussian_I(double theScale, const ImageRect* theRect);

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
            ColorImageGradient_Gaussian_I(int theWidth, const ImageRect* theRect);

            /**
             *  Destructor
             */          
            ~ColorImageGradient_Gaussian_I(void);


			/**	Returns the scale of the operator
			 *	@return		scale of the operator
			 */
			float getScale(void) const;
			 

            /** Applies this operator to an image, returns the result as a VectorField.
             *
             *  Since the operator is an "integer" operator, then the VectorField
             *              returned should have the type matching that of the input image
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the VectorField obtained by applying the operator to
             *                      imgIn
             */ 
            VectorField* applyTo(const RasterImage* imgIn);

            /** Applies this operator to an image  within the rectangle specified and
             *  returns the result as a VectorField.
             *
             *
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the VectorField obtained by applying the operator to
             *                      imgIn
             */ 
            VectorField* applyTo(const RasterImage* imgIn, const ImageRect* theRect);

            /** Applies this operator to a RasterImage and writes the result into a
             *  VectorField received as second parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyInto(const RasterImage* imgIn, VectorField* vectOut);

            /** Applies this operator to a RasterImage within the rectangle specified
             *  and writes the result into the VectorField received as parameter.
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

            /** Stores into a 1D array (radial distances squared) the filter's coefficients.
             */
            float*		ig_;
            
            /** Stores into a 1D array (radial distances squared) the filter's coefficients.
             */
            float*		igD_;
            
                        
            /** denumerator by which to divide the weighted summation computed using the
             *  coefficients of ig_
             */
            int     iDenum_;

            /** denumerator by which to divide the weighted summation computed using the
             *  coefficients of igD_
             */
            int     iDevDenum_;

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ColorImageGradient_Gaussian_I(const ColorImageGradient_Gaussian_I& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ColorImageGradient_Gaussian_I& operator =(const ColorImageGradient_Gaussian_I& obj);

                
//            void applyInto_3x3_(RasterImage* imgIn, const ImageRect* theRect,
//                                VectorField* vectOut);
//
//            void applyInto_5x5_(RasterImage* imgIn, const ImageRect* theRect,
//                                VectorField* vectOut);
//
//            void applyInto_7x7_(RasterImage* imgIn, const ImageRect* theRect,
//                                VectorField* vectOut);
//
//            void applyInto_9x9_(RasterImage* imgIn, const ImageRect* theRect,
//                                VectorField* vectOut);
//
//            void applyInto_11x11_(RasterImage* imgIn, const ImageRect* theRect,
//                                VectorField* vectOut);
//
//            void applyInto_13x13_(RasterImage* imgIn, const ImageRect* theRect,
//                                  VectorField* vectOut);
//
//            void applyInto_(RasterImage* imgIn, const ImageRect* theRect,
//                            VectorField* vectOut);

            /** Initializes the coefficients' array
             *  @see    fg_
             *  @see    fgD_
             */
            void initializeFilter_(void);
    };
}

#endif  //  COLOR_IMAGE_GRADIENT_GAUSSIAN_I_H
