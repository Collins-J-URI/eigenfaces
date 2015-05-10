/*  NAME:
        NonMaximumSuppressor.h
 
    DESCRIPTION:
        NonMaximumSuppressor public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_NON_MAXIMUM_SUPPRESSOR_H
#define URIVL_NON_MAXIMUM_SUPPRESSOR_H

#include <list>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageOperator.h"
#include "VectorField.h"
#include "VectorField_F.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGBa_F.h"


namespace uriVL
{
    /** Implementation of the NonMaximumSuppressor class.
     *
     *  This class is implemented as an instantiable static class.  It is instantiable
     *  because it may be convenient at some point to case a NonMaximumSuppressor to
     *  an ImageOperator
     *  The nonmaximum suppressor eliminates (sets to zero) the pixels that
     *  are not ridge or peak pixels
     */
    class URIVL_EXPORT NonMaximumSuppressor : public ImageOperator
    {
        public:

            /** Creates a NonMaximumSuppressor operator
             */
            NonMaximumSuppressor(void);

            /** Destructor
             */
            virtual ~NonMaximumSuppressor(void);

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
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
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
                    
            /** Applies this operator to a VectorField, returns the result as a VectorField.
             *
             *  The VectorField returned is of the same base type as the input field.
             *  For example, if vectIn was VectorField_F, then so is the vector field
             *  returned.  
             *
             *  @param  vectIn      the VectorField to which the operator is applied
             *  @return             the VectorField obtained by applying the operator to
             *                      vectIn
             */ 
            VectorField *applyTo(const VectorField *vectIn);

            /** Applies this operator to a VectorField  within the rectangle specified and
             *  returns the result as a VectorField.
             *
             *  The VectorField returned is of the same base type as the input field.
             *  For example, if vectIn was VectorField_F, then so is the vector field
             *  returned.  
             *
             *  @param  vectIn      the VectorField to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @return             the VectorField obtained by applying the operator to
             *                      imgIn
             */ 
            VectorField *applyTo(const VectorField *vectIn, const ImageRect* theRect);

            /** Applies this operator to a VectorField and writes the result into a second
             *  RasterImage received as parameter.
             *
             *  @param  vectIn      the RasterImage to which the operator is applied
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyInto(const VectorField *vectIn, VectorField *vectOut);
            
            /** Applies this operator to a RasterImage within the rectangle specified
             *  and writes the result into the RasterImage received as parameter.
             *
             *  @param  vectIn      the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            void applyInto(const VectorField *vectIn, const ImageRect* theRect, VectorField *vectOut);


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
            static RasterImage* applyTo_st(const RasterImage* imgIn);

            /** Applies this operator to an image  within the rectangle specified and
             *  returns the result as a RasterImage.
             *
             *  The RasterImage returned is of the same form as the input image.
             *  For example, if imgIn was RasterImage_Gray_F, then so is the image
             *  returned.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @return             the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            static RasterImage* applyTo_st(const RasterImage* imgIn, const ImageRect* theRect);

            /** Applies this operator to a RasterImage and writes the result into a second
             *  RasterImage received as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            static void applyInto_st(const RasterImage* imgIn, RasterImage* imgOut);
            
            /** Applies this operator to a RasterImage within the rectangle specified
             *  and writes the result into the RasterImage received as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            static void applyInto_st(const RasterImage* imgIn, const ImageRect* theRect, RasterImage* imgOut);
                    
            /** Applies this operator to a VectorField, returns the result as a VectorField.
             *
             *  The VectorField returned is of the same base type as the input field.
             *  For example, if vectIn was VectorField_F, then so is the vector field
             *  returned.  
             *
             *  @param  vectIn      the VectorField to which the operator is applied
             *  @return             the VectorField obtained by applying the operator to
             *                      vectIn
             */ 
            static VectorField *applyTo_st(const VectorField *vectIn);

            /** Applies this operator to a VectorField  within the rectangle specified and
             *  returns the result as a VectorField.
             *
             *  The VectorField returned is of the same base type as the input field.
             *  For example, if vectIn was VectorField_F, then so is the vector field
             *  returned.  
             *
             *  @param  vectIn      the VectorField to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @return             the VectorField obtained by applying the operator to
             *                      imgIn
             */ 
            static VectorField *applyTo_st(const VectorField *vectIn, const ImageRect* theRect);

            /** Applies this operator to a VectorField and writes the result into a second
             *  RasterImage received as parameter.
             *
             *  @param  vectIn      the VectorField to which the operator is applied
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            static void applyInto_st(const VectorField *vectIn, VectorField *vectOut);
            
            /** Applies this operator to a RasterImage within the rectangle specified
             *  and writes the result into the RasterImage received as parameter.
             *
             *  @param  vectIn      the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            static void applyInto_st(const VectorField *vectIn, const ImageRect* theRect, VectorField *vectOut);

                    
        private:
                
            /** Applies this operator to a RasterImage_gray_F within the rectangle specified
             *  and writes the result into the RasterImage_gray_F received as parameter.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            static void applyInto_grayF_(const RasterImage_gray_F *imgIn, const ImageRect* theRect,
                                                RasterImage_gray_F *imgOut);

            /** Applies this operator to a VectorField_F within the rectangle specified
             *  and writes the result into the VectorField_F received as parameter.
             *
             *  @param  vectIn      the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            static void applyInto_vectF_(const VectorField_F *vectIn, const ImageRect* theRect,
                                                VectorField_F *vectOut);

        
    };
}

#endif  //  NON_MAXIMUM_SUPPRESSOR_H
