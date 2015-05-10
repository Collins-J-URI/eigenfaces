/*  NAME:
        Pattern2DMatchRecord_warp.h
 
    DESCRIPTION:
        Pattern2DMatchRecord_warp public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_2D_MATCH_RECORD_WARP_H
#define URIVL_PATTERN_2D_MATCH_RECORD_WARP_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Pattern2DMatchRecord.h"
#include "OptimizerRecordND.h"
#include "Contour_I.h"


namespace uriVL
{
    /** Holds the records of a 2D warp-based matching result.
     *  Note that in the current implementation an object of this class does not store a copy
     *  of the data but just references to the members of the PatternMatcher_warp
     */
    class Pattern2DMatchRecord_warp : public Pattern2DMatchRecord
    {
        public:

            /** Default constructor
             */
            Pattern2DMatchRecord_warp(void);

            /** 
             *  Destructor
             */         
            ~Pattern2DMatchRecord_warp(void);

            /** Returns the gridRows_ x gridCols_ array of 2D grid points corresponding 
             *  to the best warp-based match
             */
            float   ***const getWarpingGrid(void);
            
//          /** Returns the gridRows_ x gridCols_ array of 2D grid points corresponding 
//           *  to the reverse of the best match warp
//           */
//          float   ***const getReverseWarpingGrid(void);
            
            /** Returns the contour of the reverse-warped pattern.  Used to determine the 
             *  new location of the pattern in the current image.
             */
            Contour_I *getReverseWarpContour(void);
            
            /**
             *  @return OptimizerRecordND   the matching result record
             */
            OptimizerRecordND *getOptimizerRecordND(void);
            
            /** set grid_
             */
            void setWarpingGrid(float   ***const grid);
            
//          /** set revGrid_
//           */
//          void setReverseWarpingGrid(float    ***const revGrid);
            
            /** set revContour_
             */
            void setReverseWarpContour(Contour_I *revContour);
            


        protected:
        
            Contour_I *revContour_;
            
            /** Record returned by the nD optimizer at the end of the matching calculations
             */
            OptimizerRecordND   *optRecND_;
            
            /** number of rows of the warping grid
             */
            int gridRows_;
            
            /** number of cols of the warping grid
             */
            int gridCols_;
            
            /** gridRows_ x gridCols_ array of 2D grid points corresponding to the best
             *  warp-based match
             */
            float   ***grid_;
            
//          /** Grid of the reverse of the optimal match (to apply to the pattern for 
//           *  its update)
//           */
//          float   ***revGrid_;
        
        friend class PatternMatcher2D_warp;
    };
}

#endif  //  PATTERN_2D_MATCH_RECORD_WARP_H
