/*  NAME:
        Translation2D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Translation2D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Translation2D.h"

using namespace uriVL;

Translation2D::Translation2D(void) 
        :   RigidTransformation2D( )
{
}

Translation2D::Translation2D(const float theTransl[]) 
        :   RigidTransformation2D( )
{
	setTranslation(theTransl);
}

Translation2D::Translation2D(float tx, float ty) 
        :   RigidTransformation2D( )
{
	setTranslation(tx, ty);
}

Translation2D::Translation2D(const Translation2D* theTransl) 
	try	:   RigidTransformation2D(theTransl)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by Translation2D constructor.");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid parameter passed to Translation2D constructor.");		
}

Translation2D::~Translation2D(void)
{}


