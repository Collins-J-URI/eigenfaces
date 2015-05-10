/*  NAME:
        RectifiedStereoMatch.h
 
    DESCRIPTION:
        RectifiedStereoMatch public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RECTIFIED_STEREO_MATCH_H
#define     URIVL_RECTIFIED_STEREO_MATCH_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "StereoMatch.h"

namespace uriVL
{
    class URIVL_EXPORT RectifiedStereoMatch : public StereoMatch 
    {
        private:        
    		float			d_;       
    };
}
#endif //	URIVL_RECTIFIED_STEREO_MATCH_H
