/*  NAME:
        EdgeDetector.h
 
    DESCRIPTION:
        EdgeDetector public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_EDGE_DETECTOR_H
#define URIVL_EDGE_DETECTOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Labeler.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGBa.h"

namespace uriVL
{
    /** EdgeDetector class.
     *
     *  For now, we don't know exactly what each edge detector will share in common as functions,
     *  so we'll keep it to what we know - the destructor will be virtual, the constructor protected
     *  and thus our edgedetector class is instantiable.
     *
     */
    class URIVL_EXPORT EdgeDetector : public Labeler
    {
        public:
            virtual ~EdgeDetector();
        
        protected:
            EdgeDetector();
            
    };
}
#endif  //  EDGE_DETECTOR_H
