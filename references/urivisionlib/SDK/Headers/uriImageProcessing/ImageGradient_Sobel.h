/*  NAME:
        ImageGradient_Sobel.h
 
    DESCRIPTION:
        ImageGradient_Sobel public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_GRADIENT_SOBEL_H
#define URIVL_IMAGE_GRADIENT_SOBEL_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageGradient.h"
#include "VectorField_F.h"
#include "VectorField_I.h"

namespace uriVL
{
    /** Vector operator that computes the gradient of a raster image using
     *  Sobel-like masks.  Objects of this class only operate
     *  on gray-level images.  If a color image is sent as parameter, it gets
     *  converted to a gray-level image first.  For operation on color
     *  images, see <code>ColorImageGradient</code> subclasses.
     *
	 *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ImageGradient_Sobel : public ImageGradient
    {
        public:

            /** 
             *  Default constructor.
             *
             *  This constructor simply sets all instance variables to 0, NULL, or the 
             *  appropriate default value.  
             *
             */
            ImageGradient_Sobel(void);

            /**
             *  Destructor
             */          
            ~ImageGradient_Sobel(void);

			/**	Implements the pure virtual function defined in the parent class to return
			 *	the value 0.6f (or width/5 = 3/5.f) even though this is not really the
			 *	"scale" of anything, just an analogy to the Gaussian version.
			 *	@return		0.6f
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
             *  This function will be overloaded by subclasses of VectorOperator.  These 
             *  subclasses should follow the following rules:
             *  <ul>
             *      <li> if the operator is an "integer" operator, then the RasterImage
             *              returned should have the same type as the input image
             *      <li> if the operator is a "float" or "double" operator, then the 
             *              RasterImage returned should be of the "float" form of the input 
             *              image.
             *              For example, if imgIn was RasterImage_Gray, then the image
             *              returned should be RasterImage_Gray_F.
             *  </ul>
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
             *  If the input image was not a gray-level image, then a local grey-level
             *  copy is first made, then the operator is applied.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyInto(const RasterImage* imgIn, const ImageRect* theRect, VectorField* vectOut);


        private:
        
            /** Effectively implements the Sobel gradient mask on a float gray-level image
             *  and writes the results in a float vector field
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyIntoF_(const RasterImage_gray_F* imgIn, const ImageRect* theRect, VectorField_F *vectOut);

            /** Effectively implements the Sobel gradient mask on a float gray-level image
             *  and writes the results in an integer vector field
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyIntoF_(const RasterImage_gray_F* imgIn, const ImageRect* theRect, VectorField_I *vectOut);

            /** Effectively implements the Sobel gradient mask on an "integer" gray-level image
             *  and writes the results in a float vector field
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyIntoI_(const RasterImage_gray* imgIn, const ImageRect* theRect, VectorField_F *vectOut);

            /** Effectively implements the Sobel gradient mask on an "integer" gray-level image
             *  and writes the results in an integer vector field
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyIntoI_(const RasterImage_gray* imgIn, const ImageRect* theRect, VectorField_I *vectOut);

    };
}

#endif  //  IMAGE_GRADIENT_SOBEL_H
