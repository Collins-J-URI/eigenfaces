/*  NAME:
        EdgeDetector_Canny_F.h
        
    DESCRIPTION:
        EdgeDetector_Canny_F public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_EDGE_DETECTOR_CANNY_F_H
#define URIVL_EDGE_DETECTOR_CANNY_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "EdgeDetector.h"
#include "VectorField_F.h"
#include "ImageGradient_Gaussian_F.h"

namespace uriVL
{
    /** EdgeDetector_Canny_F class.
     *
     *
     */
    class URIVL_EXPORT EdgeDetector_Canny_F : public EdgeDetector
    {
        public:
            EdgeDetector_Canny_F(double theScale, float hThresh, float lThresh);
            EdgeDetector_Canny_F(int theWidth, float hThresh, float lThresh);
            EdgeDetector_Canny_F(double theScale,  ImageRect*  theRect, float hThresh, float lThresh);
            EdgeDetector_Canny_F(int theWidth,  ImageRect*  theRect, float hThresh, float lThresh);
            
            virtual ~EdgeDetector_Canny_F();
            
            LabelRaster *applyTo(RasterImage* imgIn);
            LabelRaster *applyTo(RasterImage* imgIn, char* params, bool connectNeighbor);
            LabelRaster *applyTo(RasterImage* imgIn, ImageRect* theRect);
            LabelRaster *applyTo(RasterImage* imgIn, char* params, ImageRect* theRect, bool connectNeighbor);
            
            void applyInto(RasterImage* imgIn, LabelRaster *labelOut);
            void applyInto(RasterImage* imgIn, char* params, LabelRaster *labelOut, bool connectNeighbor);
            void applyInto(RasterImage* imgIn, ImageRect* theRect,  LabelRaster *labelOut);
            void applyInto(RasterImage* imgIn, char* params, ImageRect* theRect,  LabelRaster *labelOut, bool connectNeighbor);
            
            void applyInto(RasterImage* imgIn, RasterImage* imgOut);
            void applyInto(RasterImage* imgIn, char* params, RasterImage* imgOut);
            void applyInto(RasterImage* imgIn, ImageRect* theRect, RasterImage* imgOut);
            void applyInto(RasterImage* imgIn, char* params, ImageRect* theRect, RasterImage* imgOut);
            
            /*
             * Returns a reference to the private data member lastVect_ which is in fact the non-maximum suppressed
             * gradient of the image this object last manipulated.  Keep in mind that since this is a private data reference,
             * repeated use of a particular instance of this class will change the data, so it would be wise to create a new
             * VectorField_F out of it before it is lost.
             *
             * @return a reference to the private member data lastVect_
             */
            VectorField_F* getLastVect();

        private:
            ImageGradient_Gaussian_F* cGrad_;
            
            float hThresh_;
            float lThresh_;

      VectorField_F* interVect_;
            VectorField_F* lastVect_;
    };
}
#endif  //  EDGE_DETECTOR_CANNY_F_H