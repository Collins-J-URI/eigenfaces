/*  NAME:
        Segment.h
 
    DESCRIPTION:
        Segment public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_SEGMENT_H
#define URIVL_SEGMENT_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageFeature.h"

namespace uriVL
{
    class URIVL_EXPORT Segment : public ImageFeature
    {
        public:
            Segment(void);
            ~Segment(void);
            ImageRect* getBoundRect(void) = 0;
            ImageRect* getValidRect(void) = 0;
            void setValidRect(short x, short y, short theWidth, short theHeight) = 0;

        protected:
            void setBoundRect_(short x, short y, short theWidth, short theHeight) = 0;
    }
}

#endif