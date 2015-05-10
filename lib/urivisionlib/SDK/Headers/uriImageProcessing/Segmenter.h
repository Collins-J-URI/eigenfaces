/*  NAME:
        Segmenter.h
 
    DESCRIPTION:
        Segmenter public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_SEGMENTER_H
#define URIVL_SEGMENTER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ColorRegions.h"
#include "RasterImage.h"


namespace uriVL
{
    /** Generic, abstract Segmenter class.
     *
     *  06/20/2003  first "working" version
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class Segmenter
    {
        public:
            Segmenter(void);
            virtual ~Segmenter(void);
            //

        protected:

    };
}

#endif  //  SEGMENTER_H
