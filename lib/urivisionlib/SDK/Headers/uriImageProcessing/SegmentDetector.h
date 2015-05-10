/*  NAME:
        SegmentDetector.h
 
    DESCRIPTION:
        SegmentDetector public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_SEGMENT_DETECTOR_H
#define URIVL_SEGMENT_DETECTOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include <vector.h>

#include "RasterImage.h"


namespace uriVL
{

    typedef enum SegmentDetectorType
    {
            kVertical = 0,
            kHorizontal
    } SegmentDetectorType;

    /** This provides fast detection of horizontal or vertical line segment in the image.
     *  The input image should have been preprocessed using an edge detection technique 
     *  and should be a binary image. For fast processing, a block local approch for line
     *  detection will be applied.
     *  @author Jingjing Hu, 3D Group, 
     *                       Department of Computer Science and Statistics,
     *                       University of Rhode Island
     */
    class SegmentDetector
    {
        public:


            //
            //  Constructor and destructor  
            //
            SegmentDetector(SegmentDetectorType theType,
                            int blockWidth, 
                            int blockHeight, 
                            int windowSize, 
                            int voteThresh);
            
            ~SegmentDetector(void);
            //
            //  Public methods
            //
            void detect(RasterImage* inputImage, vector <int> *v);

            
        protected:
            SegmentDetectorType segType;
            int bWidth;
            int bHeight;
            int wSize;
            int threshold;
            
            //
            //  private methods
            //

    };
}

#endif  //  SEGMENT_DETECTOR_H