/*  NAME:
        ManualStitcher.h
 
    DESCRIPTION:
        ManualStitcher public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_MANUAL_STITCHER_H
#define URIVL_MANUAL_STITCHER_H

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


    /** A ManualStitcher uses user input to position correctly relative to one another
     *  a set of RasterImage objects.
     *  I wrote that one quickly in July 2003 to help my student Baikang Pei stitch 
     *  together images of histological slices of a rat's brain (about 4 to 12 noisy, poorly
     *  color-equalized images per slice).  I initially wrote an automatic stitcher
     *  that did not always (ever?) work, hence this quick and dirty job.  Both classes
     *  need some serioous rewriting, and I doubt either one even compiles anymore.  It is
     *  just here as a place holder until I find the time to fix them.
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class ManualStitcher
    {
        public:
            ManualStitcher(void);
            virtual ~ManualStitcher(void);
            //
            void initStitch(RasterImage** image, StitchPattern *thePattern);
            RasterImage* stitchImage(RasterImage** image);
            void equalizeImages(RasterImage** image);
            void restoreImages(RasterImage** image, StitchPattern *thePattern);
            void translateImage(RasterImage** image, short i, short dx, short dy);

        private:
            StitchPattern   *stitchPat;
            bool            *intersect,
                            modifiedImages;
            short           *coordX, *coordY,
                            *xMin, *xMax,
                            *yMin, *yMax,
                            nbImages;
            float           **scaleRatios,
                            *scaleRatio;
            //
            void allocateStitchArrays(void);
            void deleteStitchArrays(void);
            void computeIntersections(RasterImage** image, short i);
            void computeAverages(RasterImage** image, short i, short j, float avg[]);
            void interpolateImages(RasterImage** image, short i, short j);

    };
}

#endif  //  MANUAL_STITCHER_H