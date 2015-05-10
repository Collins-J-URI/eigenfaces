/*  NAME:
        Pattern2DMatchRecord_part.h
 
    DESCRIPTION:
        Pattern2DMatchRecord_part public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_2D_MATCH_RECORD_PART_H
#define URIVL_PATTERN_2D_MATCH_RECORD_PART_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Pattern2DMatchRecord.h"


namespace uriVL
{
    /** Holds the records of a 2D matching result (matching performed by a "partial
     *  match" matcher).
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT Pattern2DMatchRecord_part : public Pattern2DMatchRecord
    {
        public:

            /** Default constructor
             */
            Pattern2DMatchRecord_part(void);

            /** 
             *  Destructor
             */         
            ~Pattern2DMatchRecord_part(void);

            /** Returns the matchng record of a specific quadrant
             *  @param  quad    index of the quadrant of which we want the matching record
             */
            Pattern2DMatchRecord* getMatchRecord(QuadrantIndex quad);
            
            
            /** Returns the score of the best match found
             *  @param  quad    index of the quadrant of which we want the best match's score
             */
            float getScore(QuadrantIndex quad);

            /** Returns the location of the best match
             *  @param  quad    index of the quadrant of which we want the best match's score
             */
            ImagePoint* getBestMatch(QuadrantIndex quad);

            /** Sets the score
             *  @param  quad    index of the quadrant for which to set the best match's score
             *  @param  score   the new score for this record
             */
            void setScore(QuadrantIndex quad, float score);

            /** Sets the coordinates of the best match
             *  @param  quad    index of the quadrant for which to set the best match
             *  @param  x   new x coordinate for this record's best match point
             *  @param  y   new y coordinate for this record's best match point
             */
            void setBestMatch(QuadrantIndex quad, int x, int y);

            /** Sets the coordinates of the best match
             *  @param  quad    index of the quadrant for which to set the best match
             *  @param  bestMatch   new best match point for this record
             */
            void setBestMatch(QuadrantIndex quad, ImagePoint* bestMatch);


        protected:
        
            /** Matching records for the 4 quadrants
             */
            Pattern2DMatchRecord* *quadMatch_;
        
    };
}

#endif  //  PATTERN_2D_MATCH_RECORD_PART_H
