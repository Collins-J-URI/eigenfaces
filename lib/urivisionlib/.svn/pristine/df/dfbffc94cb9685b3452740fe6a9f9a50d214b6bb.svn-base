/*  NAME:
        BackgroundRemover_static.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib BackgroundRemover_static class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "BackgroundRemover_static.h"
//
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


BackgroundRemover_static::BackgroundRemover_static(void)
        :   BackgroundRemover()
{
    setColorDifferenceMode(AVERAGE_DIFFERENCE);
}

BackgroundRemover_static::~BackgroundRemover_static(void)
{
}


//	overided pure virtual func
void BackgroundRemover_static::setBackground(RasterImage* img)
{
    try {
        assignBackground_(img);   
    }
    catch (ErrorReport& e) {
        e.appendToMessage("called by BackgroundRemover_static::setBackground.");
        throw e;
    }
}


