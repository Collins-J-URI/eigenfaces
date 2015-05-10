/*  NAME:
        FeatureDetector.cpp
 
    DESCRIPTION:
        FeatureDetector source file
        FeatureDetector is an abstract class (with pure virtual member function)
        Inherited by: BlobFinder, SegmentDetector, CornerDetector...
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "FeatureDetector.h"

using namespace std;
using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif

FeatureDetector::FeatureDetector(void)
	:	searchRect_(NULL)
{

}

FeatureDetector::FeatureDetector(const ImageRect* rect)
	:	searchRect_(NULL)
{
	try 
	{
		setSearchRect(rect);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FeatureDetector constructor");
		throw e;
	}
}


FeatureDetector::~FeatureDetector(void)
{
	if (searchRect_)
		delete searchRect_;

}




#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public functions
//------------------------------------------------------
#endif

void FeatureDetector::setSearchRect(const ImageRect* rect)
{
	try 
	{
		searchRect_ = new ImageRect(rect);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by FeatureDetector::setSearchRect");
		throw e;
	}
}
