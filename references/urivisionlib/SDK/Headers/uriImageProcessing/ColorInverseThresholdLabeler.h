/*  NAME:
        ColorInverseThresholdLabeler.h
 
    DESCRIPTION:
        ColorInverseThresholdLabeler public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_COLOR_THRESHOLD_LABELER_H
#define URIVL_COLOR_THRESHOLD_LABELER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ColorSegParams.h"
#include "RasterImage.h"
#include "RasterImage_HSV.h"
#include "Labeler.h"
#include "LabelRaster.h"

namespace uriVL
{
    /** A ColorInverseThresholdLabeler segments an image into regions based on
     *  a set of predefined color threshold-based modes
     *
     *  Note:  So far this class does not do much in the way of data validation.
     *  For example, if the user defines gray level or color domains I simply
     *  assume that these domains do not overlap, without checking for it.
     *  @author jean-yves herve' and Fang Han, 3D Group, 
     *                                  Department of Computer Science and Statistics,
     *                                  University of Rhode Island
     */
    class URIVL_EXPORT ColorInverseThresholdLabeler : public Labeler
    {
        public:
            ColorInverseThresholdLabeler(void);


            /** constructor
             *
             *  @param  theWidth   number of columns
             *  @param  theHeight  number of rows
             */
            ColorInverseThresholdLabeler(int theWidth, int theHeight);
            
            /**
             *  Destructor
             */          
            ~ColorInverseThresholdLabeler(void);

            /** Applies this operator to an image, returns the result as a LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            LabelRaster *applyTo(const RasterImage* imgIn, const ColorSegParams *params,
                                 bool connectNeighbor = false);

            /** Applies this operator to an image, returns the result as a LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            LabelRaster *applyTo(const RasterImage* imgIn, const ColorSegParams *params, 
                                 const ImageRect* theRect, bool connectNeighbor = false);

            /** Applies this operator to an image, returns the result as a LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to char)
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             */ 
            LabelRaster *applyTo(const RasterImage* imgIn, const char* params, bool connectNeighbor = false);

            /** Applies this operator to an image, returns the result as a LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to void)
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             */ 
            LabelRaster *applyTo(const RasterImage* imgIn, const char* params, const ImageRect* theRect,
                                 bool connectNeighbor = false);
            

            /** Applies this operator to an image, returns the result as a LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation
             *  @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             */ 
            void applyInto(const RasterImage* imgIn, const ColorSegParams *params, LabelRaster *labelOut,
                            bool connectNeighbor = false);

            /** Applies this operator to an image, returns the result as a LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             */ 
            void applyInto(const RasterImage* imgIn, const ColorSegParams *params, const ImageRect* theRect, 
                            LabelRaster *labelOut, bool connectNeighbor = false);

            /** Applies this operator to an image, returns the result as a LabelRaster.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to char)
             *  @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             */ 
            void applyInto(const RasterImage* imgIn, const char* params, LabelRaster *labelOut,
                            bool connectNeighbor = false);

            
            /** Applies this operator to an image, writes the result into a RasterImage.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to char)
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  ingOut      the RasterImage obtained by applying the operator to
             *                      imgIn
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             */ 
            void applyInto(const RasterImage* imgIn, const char* params, const ImageRect* theRect, 
                           LabelRaster *labelOut, bool connectNeighbor = false);

            /** Applies this operator to an image, writes the result into a RasterImage.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation
             *  @param  ingOut      the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const ColorSegParams *params, RasterImage* imgOut);

            /** Applies this operator to an image, writes the result into a RasterImage.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  ingOut      the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const ColorSegParams *params, const ImageRect* theRect, 
                            RasterImage* imgOut);

            /** Applies this operator to an image, writes the result into a RasterImage.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation
             *  @param  ingOut      the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const char* params, RasterImage* imgOut);

            /** Applies this operator to an image, writes the result into a RasterImage.
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to char)
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  ingOut      the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            void applyInto(const RasterImage* imgIn, const char* params, const ImageRect* theRect, 
                                RasterImage* imgOut);

        private:

            /** Private function that applies this operator to an image, writes the result
             *  into a LabelRaster.
             *
             *  @param  imgIn           the RasterImage to which the operator is applied
             *  @param  params          parameters for the labeling operation
             *  @param  theRect         the rectangle within which to apply the labeler
             *  @param  labelOut        the LabelRaster obtained by applying the operator to
             *                          imgIn
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             */ 
            void applyInto_(const RasterImage* imgIn, const ColorSegParams *colorParams, 
                            const ImageRect* theRect, LabelRaster *labelOut,
                            bool connectNeighbor);

            /** Private function that applies this operator to an image, writes the result
             *  into an RGBA raster image.
             *
             *  @param  imgIn           the RasterImage to which the operator is applied
             *  @param  params          parameters for the labeling operation
             *  @param  theRect         the rectangle within which to apply the labeler
             *  @param  labelOut        the LabelRaster obtained by applying the operator to
             *                          imgIn
             *  @param  connectNeighbor when <code>true</code>, indicates that an 8-neighbor
             *                          connection process should be applied to the results
             *                          of the thresholding to eliminate isolated pixels.
             */ 
            void applyInto_(const RasterImage* imgIn, const ColorSegParams *Params, 
                            const ImageRect* theRect, RasterImage_RGBa* imgOut);


                                            
            /**
             *  connect neighbor pixels based on each pixel's 8 label values
             *
             *  @param  destRect    destination bounding rectangle
             *  @param  nbRegions   number of regions
             *  @param  label       original LabelRaster
             *  @param  tempLabel   temporary LabelRaster for storing label values
             */
            void connectNeighbor_(const ImageRect* destRect, int nbRegions,
                                  LabelRaster *label, LabelRaster *tempLabel, char noMatchVal);



            bool goodColor_(int hue, unsigned char sat, unsigned char val, 
                            const ColorRegion *theColor);
    };
}

#endif  //  COLOR_THRESHOLD_LABELER_H
