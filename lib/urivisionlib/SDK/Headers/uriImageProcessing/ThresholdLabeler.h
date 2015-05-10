/*  NAME:
        ThresholdLabeler.h
 
    DESCRIPTION:
        ThresholdLabeler public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_THRESHOLD_LABELER_H
#define URIVL_THRESHOLD_LABELER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ColorRegions.h"
#include "RasterImage.h"
#include "Labeler.h"


namespace uriVL
{
    /** Pure virtual parent class for objects that segment an image into regions based on
     *  a set of predefined threshold-based modes.
     *
     *  This whole family of classes is up for a major overhaul.  
     *
     *  Note:  So far this class does not do much in the way of data validation.
     *  For example, if the user defines gray level or color domains I simply
     *  assume that these domains do not overlap, without checking for it.
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ThresholdLabeler : public Labeler
    {
        public:
            ThresholdLabeler(void);
            
            /** Destructor
             */
            virtual ~ThresholdLabeler(void);

            /** Applies thresholding to the input image according to a set of parameters,
             *  and returns the result as a label raster
             */
            virtual LabelRaster *applyTo(RasterImage* imgIn, char* params) = 0;

            /** Applies thresholding to a rectangular region of the input image according
             *  to a set of parameters, and returns the result as a label raster
             */
            virtual LabelRaster *applyTo(RasterImage* imgIn, char* params,
                                            ImageRect* theRect) = 0;

            virtual void applyInto(RasterImage* imgIn, LabelRaster *labelOut, ColorSegParams *params)=0;
            virtual void applyInto(RasterImage* imgIn, LabelRaster *labelOut, ColorSegParams *params, 
                                            ImageRect* theRect)=0;
/*  Deactivated in v. 1
uh? why?
            virtual void applyInto(RasterImage* imgIn, RasterImage* imgOut, char* params) = 0;
            virtual void applyInto(RasterImage* imgIn, RasterImage* imgOut, char* params,
                            ImageRect* theRect) = 0;
*/
        private:
            //
/*          
            //
            void applyInto_color_(RasterImage* imgIn, RasterImage_RGBa* imgOut,
                                    ColorSegParams *colorParams);
            void applyInto_color_(RasterImage* imgIn, RasterImage_RGBa* imgOut,
                                    ColorSegParams *colorParams, ImageRect* theRect);
            void label_color_(RasterImage* imgIn,  char  *labelOut, ColorSegParams *colorParams);
            void label_color_(RasterImage* imgIn,  char  *labelOut, ColorSegParams *colorParams,
                                    ImageRect* theRect);
            //
            void applyInto_gray_(RasterImage* imgIn, RasterImage_gray* imgOut,
                                    ColorSegParams *colorParams);
            void applyInto_gray_(RasterImage* imgIn, RasterImage_gray* imgOut,
                                    ColorSegParams *colorParams, ImageRect* theRect);
            void label_gray_(RasterImage* imgIn,  char* labelOut, ColorSegParams *colorParams);
            void label_gray_(RasterImage* imgIn,  char* labelOut, ColorSegParams *colorParams,
                                    ImageRect* theRect);
*/
    };
}
#endif  //  THRESHOLD_LABELER_H
