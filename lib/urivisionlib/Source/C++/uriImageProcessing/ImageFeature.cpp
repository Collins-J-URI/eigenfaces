/*  NAME:
        ImageFeature.cpp
 
    DESCRIPTION:
        ImageFeature source file.
        ImageFeature is an abstract class (with pure virtual member function)
        Inherited by: Blob, Segment, Corner...
        Base class: DrawableObject2D
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ImageFeature.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

ImageFeature::ImageFeature(void)
		try	: DrawableObject2D()
{

}
catch (ErrorReport& e)
{
	e.appendToMessage("called by ImageFeature constructor");
	throw e;
}


ImageFeature::ImageFeature(const ImageFeature& obj)
		try	: DrawableObject2D(obj)
{

}
catch (ErrorReport& e)
{
	e.appendToMessage("called by ImageFeature constructor");
	throw e;
}


ImageFeature::~ImageFeature(void)
{

}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif


//----------------------------------------------------------
//  private functions
//----------------------------------------------------------

const ImageFeature& ImageFeature::operator = (const ImageFeature& obj)
{
	return *this;
}



#if 0
#pragma mark -
#endif

