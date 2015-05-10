/*  NAME:
        ColorSegParams.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorSegParams class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColorSegParams.h"

using namespace uriVL;


ColorSegParams::ColorSegParams(void)
		:	    colorMode(false),
				nbModes(0),
				noMatchVal(static_cast<char>(-1)),
				colorReg(NULL),
				grayReg(NULL),
				fillMode(kIdentityFill)
{
}


ColorSegParams::ColorSegParams(const ColorSegParams& param)
		:	    colorMode(false),
				nbModes(0),
				noMatchVal(static_cast<char>(-1)),
				colorReg(NULL),
				grayReg(NULL),
				fillMode(kIdentityFill)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"ColorSegParams copy constructor not implemented");
}
			
ColorSegParams::~ColorSegParams(void)
{
	if (colorReg != NULL)
		delete []colorReg;
		
	if (grayReg != NULL)
		delete []grayReg;
}



const ColorSegParams& ColorSegParams::operator = (const ColorSegParams& param)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"ColorSegParams copy operator not implemented");
	return *this;
}
			
