/*  NAME:
        ImageLaplacian_Sobel.h
 
    DESCRIPTION:
        ImageLaplacian_Sobel public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_LAPLACIAN_SOBEL_H
#define URIVL_IMAGE_LAPLACIAN_SOBEL_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageLaplacian.h"

namespace uriVL
{
    /** Forms of the "Sobel" Laplacian operators implemented in the ImageLaplacian_Sobel 
     *  class.
     *
     *  Note that currently the scaling parameter (1/2 for LAPLACIAN_SOBEL_FORM_2,
     *  1/6 for LAPLACIAN_SOBEL_FORM_3) is not applied because the main use we have for
     *  the Laplacian in our applications is the determination of edge pixels.  This
     *  may be changed in the future if a good case can be made for such a change.
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    typedef enum LaplacianSobelForm
    {
                                        //      0    1   0
        LAPLACIAN_SOBEL_FORM_1 = 1,     //      1   -4   1
                                        //      0    1   0
        
                                        //      1    0   1
        LAPLACIAN_SOBEL_FORM_2 = 2,     //      0   -4   0    *   1/2
                                        //      1    0   1
        
                                        //      1    4   1
        LAPLACIAN_SOBEL_FORM_3 = 3      //      4   -20  4    *   1/6
                                        //      1    4   1
    } LaplacianSobelForm;
    

    /** ImageLaplacian_Sobel class.
     *
     *  There are (so far) 3 possible implementations of the ImageLaplacian_Sobel
     *  class.  I know that by calling them so I abuse the name "Sobel" but I just
     *  call a "Sobel" operator any linear operator that corresponds to a 3x3
     *  covariance mask.
     *
     *  @see LaplacianSobelForm
     */
    class URIVL_EXPORT ImageLaplacian_Sobel : public ImageLaplacian
    {
        public:

            /** 
             *  Default constructor.
             *
             *  This constructor simply sets all instance variables to 0, NULL, or the 
             *  appropriate default value.  
             *
             */
            ImageLaplacian_Sobel(LaplacianSobelForm form);

            /**
             *  Destructor
             */          
            ~ImageLaplacian_Sobel(void);


			/**	Implements the pure virtual function defined in the parent class to return
			 *	the value 0.6f (or width/5 = 3/5.f) even though this is not really the
			 *	"scale" of anything, just an analogy to the Gaussian version.
			 *	@return		0.6f
			 */
			float getScale(void) const;
			 

            /** Applies this operator to an image, returns the result as a RasterImage.
             *
             *  The RasterImage returned is always of type RasterImage_Gray_F
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            RasterImage* applyTo(const RasterImage* imgIn);

            /** Applies this operator to an image  within the rectangle specified and
             *  returns the result as a RasterImage.
             *
             *  The RasterImage returned is always of type RasterImage_Gray_F
             *
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            RasterImage* applyTo(const RasterImage* imgIn, const ImageRect* theRect);

            /** Applies this operator to a RasterImage and writes the result into a second
             *  RasterImage received as parameter.  That destination image must be of
             *  type RasterImage_Gray_F.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            void applyInto(const RasterImage* imgIn, RasterImage* imgOut);
            
            /** Applies this operator to a RasterImage within the rectangle specified
             *  and writes the result into the RasterImage received as parameter.
             *  That destination image must be of type RasterImage_Gray_F.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            void applyInto(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut);
            

        protected:
        
            /** Mask form of this operator
             */
            LaplacianSobelForm  form_;
            
            /** Applies the "form 1" Laplacian to a RasterImage within the rectangle specified
             *  and writes the result into the RasterImage received as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            void applyInto_form1_(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut);
            
            /** Applies the "form 2" Laplacian to a RasterImage within the rectangle specified
             *  and writes the result into the RasterImage received as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            void applyInto_form2_(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut);
            
            /** Applies the "form 3" Laplacian to a RasterImage within the rectangle specified
             *  and writes the result into the RasterImage received as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            void applyInto_form3_(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut);
            
    };
}

#endif  //  IMAGE_LAPLACIAN_SOBEL_H
