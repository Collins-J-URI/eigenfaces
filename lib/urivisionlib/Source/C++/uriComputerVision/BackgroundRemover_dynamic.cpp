/*  NAME:
        BackgroundRemover_dynamic.cpp
 
    DESCRIPTION:
        BackgroundRemover_dynamic source file
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "BackgroundRemover_dynamic.h"
//
using namespace std;
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


BackgroundRemover_dynamic::~BackgroundRemover_dynamic(void)
{
	delete firstBackground_;
}

//	overidden virtual func
void BackgroundRemover_dynamic::setBackground(RasterImage* bkgd)
{
    try {
        assignBackground_(bkgd);
    }
    catch (ErrorReport& e) {
        e.appendToMessage("called by BackgroundRemover_dynamic::setBackground.");
        throw e;
    }
    
    firstBackground_	= new RasterImage_gray(getBackground());
}




BackgroundRemover_dynamic::BackgroundRemover_dynamic(void)
            :   BackgroundRemover(),
                //
                firstBackground_(NULL)
{
}

RasterImage* BackgroundRemover_dynamic::getFirstBackground_(void)
{
    return firstBackground_;
}


BackgroundRemover_dynamic::BackgroundRemover_dynamic(const BackgroundRemover_dynamic& obj)
            :   BackgroundRemover(),
                //
                firstBackground_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "BackgroundRemover_dynamic copy constructor disabled");
}




const BackgroundRemover_dynamic& BackgroundRemover_dynamic::operator = (const BackgroundRemover_dynamic& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "BackgroundRemover_dynamic copy operator disabled");
    return *this;
}

