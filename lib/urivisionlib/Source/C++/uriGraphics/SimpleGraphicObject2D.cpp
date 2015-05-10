/*  NAME:
        SimpleGraphicObject2D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib SimpleGraphicObject2D class
 
    COPYRIGHT:
        (c) 2013-2014, 3D Group for Interactive Visualization
					University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstdlib>
//
#include "SimpleGraphicObject2D.h"

using namespace uriVL;


SimpleGraphicObject2D::SimpleGraphicObject2D(void)
		:	DrawableObject2D()
{
//	try {
//		init_(origin, axes);
//	}
//	catch (ErrorReport& e) {
//		deleteArrays_();
//		e.appendToMessage("called from SimpleGraphicObject2D constructor.");
//		throw e;
//	}
}

//SimpleGraphicObject2D::SimpleGraphicObject2D(const SimpleGraphicObject2D& theObj)
//		:	DrawableObject2D()
//{
//    FAIL_CONDITION( true,
//                    kFunctionNotImplemented,
//                    "SimpleGraphicObject2D copy constructor not implemented.");
//}


SimpleGraphicObject2D::~SimpleGraphicObject2D(void)
{
}


//const SimpleGraphicObject2D& SimpleGraphicObject2D::operator = (const SimpleGraphicObject2D& theObj)
//{
//    FAIL_CONDITION( true,
//                    kFunctionNotImplemented,
//                    "SimpleGraphicObject2D copy operator not implemented.");
//
//	return *this;
//}


