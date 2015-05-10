/*  NAME:
        Labeler.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Labeler class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "RasterImage_gray.h"
#include "RasterImage_HSV.h"
#include "Labeler.h"

using namespace uriVL;


/*------------------------------------------------------------------------------+
|	Default constructor								                            |
+------------------------------------------------------------------------------*/
Labeler::Labeler(void)
		:	tempLabel_(NULL)
{
}

Labeler::Labeler(int theWidth, int theHeight)
	try	:	tempLabel_(new LabelRaster(theHeight, theWidth))
{
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by Labeler constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kNoCode,
					"In Labeler constructor");
}


Labeler::Labeler(const Labeler& obj)
		:	tempLabel_(NULL)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Label copy constructor not implemented");
}
			

/*------------------------------------------------------------------------------+
|	Destructor											                        |
+------------------------------------------------------------------------------*/
Labeler::~Labeler(void)
{
    if (tempLabel_ != NULL)
        delete tempLabel_;
        
}


const Labeler& Labeler::operator = (const Labeler& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Label copy constructor not implemented");
	return *this;
}


