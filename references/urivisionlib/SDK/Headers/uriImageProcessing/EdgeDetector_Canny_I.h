/*  NAME:
        EdgeDetector_Canny_I.h
        
    DESCRIPTION:
        EdgeDetector_Canny_I public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_EDGE_DETECTOR_CANNY_I_H
#define URIVL_EDGE_DETECTOR_CANNY_I_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "EdgeDetector.h"

namespace uriVL
{
    /** EdgeDetector_Canny_I
     *
     */
    class URIVL_EXPORT EdgeDetector_Canny_I : public EdgeDetector
    {
        public:
            EdgeDetector_Canny_I(void);
            ~EdgeDetector_Canny_I(void);

    };
}
#endif  //  EDGE_DETECTOR_CANNY_I_H