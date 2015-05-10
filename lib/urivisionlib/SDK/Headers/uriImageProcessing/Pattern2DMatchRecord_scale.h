/*  NAME:
        Pattern2DMatchRecord_scale.h
 
    DESCRIPTION:
        Pattern2DMatchRecord_scale public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_2D_MATCH_RECORD_SCALE_H
#define URIVL_PATTERN_2D_MATCH_RECORD_SCALE_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Blob.h"
#include "RasterImage.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray.h"
#include "Pattern2D.h"
#include "ImagePoint.h"
#include "ImageRect.h"
#include "Pattern2DMatchRecord.h"


namespace uriVL
{
    typedef struct ScaleParameter
    {
        float scaleFactor;
        // ...
        
    } ScaleParameter;


    /** holds the records of 2D matching result
     */
    class Pattern2DMatchRecord_scale 
            : public Pattern2DMatchRecord
    {
        friend class PatternMatcher2D;
    
        public:
        
            /** 
             *  constructor
             */
            Pattern2DMatchRecord_scale(const ImagePoint* bestMatch, float score,
                                       const ScaleParameter *scaleParam);

            /** 
             *  Destructor
             */         
            ~Pattern2DMatchRecord_scale(void);



        protected:
        
            /** scaling parameter
             *  @see ScaleParameter
             */
            ScaleParameter *scaleParam_;
        
    };
}

#endif  //  PATTERN_2D_MATCH_RECORD_SCALE_H
