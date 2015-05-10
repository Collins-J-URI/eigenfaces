/*  NAME:
        Pattern2DMatchRecord.h
 
    DESCRIPTION:
        Pattern2DMatchRecord public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_2D_MATCH_RECORD_H
#define URIVL_PATTERN_2D_MATCH_RECORD_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include <string>
//
#include "Blob.h"
#include "RasterImage.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray.h"
#include "Pattern2D.h"
#include "ImagePoint.h"
#include "ImageRect.h"

namespace uriVL
{

    /** Holds the records of a 2D matching result.
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class Pattern2DMatchRecord 
    {
        friend class PatternMatcher2D;
    
        public:

            /** Default constructor
             */
            Pattern2DMatchRecord(void);

            /** Creates a new match record  with the best matching point and score
             *  specified.
             *  @param  bestMatch   location of the best match found
             *  @param  score       score of the best match
             */
            Pattern2DMatchRecord(const ImagePoint* bestMatch, float score);

            /** Creates a new match record  with the best matching point and score
             *  specified.
             *  @param  x   x coordinate for this record's best match point
             *  @param  y   y coordinate for this record's best match point
             *  @param  score       score of the best match
             */
            Pattern2DMatchRecord(int x, int y, float score);

            /** 
             *  Destructor
             */         
            ~Pattern2DMatchRecord(void);

            /** Returns the score of the best match found
             */
            float getScore(void);

            /** Returns the location of the best match
             */
            ImagePoint* getBestMatch(void);

            /** Sets the score
             *  @param  score   the new score for this record
             */
            void setScore(float score);

            /** Sets the coordinates of the best match
             *  @param  x   new x coordinate for this record's best match point
             *  @param  y   new y coordinate for this record's best match point
             */
            void setBestMatch(int x, int y);

            /** Sets the coordinates of the best match
             *  @param  bestMatch   new best match point for this record
             */
            void setBestMatch(const ImagePoint* bestMatch);

            /** Temporary solution until I understand why polymorphism doesn't seem to be 
             *  working with the Pattern2DMatchRecord class hierarchy
             */
            const std::string getRecordType(void) const;


        protected:
        
            /** best matching point
             */
            ImagePoint* bestMatch_;
        
            /** confidence score
             */
            float score_;

            /** Indicates whether the match was calculated with subpixel
             *  accuracy (false by default)
             */
            bool isSubpixelMatch_;
            
            /** Temporary solution until I understand why polymorphism doesn't seem to be 
             *  working with the Pattern2DMatchRecord class hierarchy
             */
            std::string recordType_;
        
    };
}

#endif  //  PATTERN_2D_MATCH_RECORD_H
