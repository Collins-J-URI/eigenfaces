/*  NAME:
        ImageStitcher.h
 
    DESCRIPTION:
        ImageStitcher public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_STITCHER_H
#define URIVL_IMAGE_STITCHER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"

namespace uriVL
{
    typedef struct StitchPattern
    {
        short   nbImages;
        short   *coordX,
                *coordY,
                *xMin,
                *xMax,
                *yMin,
                *yMax;
    } StitchPattern;


    /** An ImageStitcher positions correctly relative to one another a set of
     *  RasterImage objects (translation only).
     *  I wrote that one quickly in July 2003 to help my student Baikang Pei stitch 
     *  together images of histological slices of a rat's brain (about 4 to 12 noisy, poorly
     *  color-equalized images per slice).  Since it did not always (ever?) work, I also
     *  wrote a quick and dirty "manual" stitcher.  Both classes need some serious rewriting,
     *  and I doubt either one even compiles anymore.  It is
     *  just here as a place holder until I find the time to fix them.
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class ImageStitcher
    {
        public:
            ImageStitcher(void);
            ~ImageStitcher(void);
            //
            RasterImage* stitch(RasterImage** image, StitchPattern *thePattern);
            bool optimizeDirection(short k, RasterImage** image);
            void equalizeImages(RasterImage** image);
            void restoreImages(RasterImage** image, StitchPattern *thePattern);
            void translateImage(RasterImage** image, short i, short dx, short dy);

        private:
            StitchPattern   *stitchPat;
            bool            *intersect,
                            modifiedImages;
            short           *displState,
                            *qMin, *qMax,       //  Bounds for generalized coordinates.
                            *xMin, *xMax,       //  Bounds for intersection regions
                            *yMin, *yMax,       //      (for correlation computation).
                            nbImages;
            //
            void allocateStitchArrays(void);
            void deleteStitchArrays(void);
            void computeIntersections(RasterImage** image, short i);
            float computeScoreGray(RasterImage** image, short i);
            float computeScoreRGB(RasterImage** image, short i);
            float computeScoreRGBA(RasterImage** image, short i);
            void computeCorrTerms(RasterImage** image, short k, short l, float avg[],
                                            float sumTerm[]);
            void computeAverages(RasterImage** image, short i, short j, float avg[]);
            void interpolateImages(RasterImage** image, short i, short j);

    };
}

#endif  //  IMAGE_STITCHER_H