/*  NAME:
        ColorRegion.h
 
    DESCRIPTION:
        ColorRegion public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_COLOR_REGIONS_H
#define URIVL_COLOR_REGIONS_H

#include <string>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

namespace uriVL
{
    /** A FillingMode specifies how part of an image that fall *outside* of color/gray
     *  regions should be filled in the output raster
     */
    typedef enum FillingMode
    {
        kIdentityFill = 0,
        kOrigColorFill,
        kOrigGrayFill,
        kBlackFill,
        kWhiteFill,
        kMediumGrayFill
    } FillingMode;


    /** A ColorRegion is defined as a rectangular domain in the HSV space
     */
    typedef struct ColorRegion
    {
        std::string     labelStr;
        int             hueRef,         //  the hue domain is specified by a "reference" hue
                        deltaHue;       //  and a max deviation from that reference
        //
        unsigned char   satMin,         //  the saturation and value domains are specified
                        satMax,         //  by a min and a max value.
                        valMin,
                        valMax;
        //
        char            label;
        unsigned char   fillColor[3];
    } ColorRegion;

    /** A GrayRegion is defined as a range of gray-level values
     */
    typedef struct GrayRegion
    {
        std::string     labelStr;
        unsigned char   vMin,
                        vMax;
        //
        char            label;
        unsigned char   fillColor[3];       //  filling color (for display)
    } GrayRegion;


}

#endif  //  COLOR_REGIONS_H
