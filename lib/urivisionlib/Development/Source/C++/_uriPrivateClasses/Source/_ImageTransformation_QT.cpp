/*  NAME:
		_ImageTransformation_QT.cpp

		DESCRIPTION:
			implementation of the uriVisionLib private _ImageTransformation_QT class

		COPYRIGHT:
			(c) 2013-2014, 3D Group for Interactive Visualization
						University of Rhode Island.

		Licensed under the Academic Free License - v. 2.1
		For more information see http://opensource.org/licenses/afl-2.1.php
*/

// safeguard the loading of this source file
#if URIVL_USE_QUICKTIME

#include "_ImageTransformation_QT.h"

using namespace std;
using namespace uriVL;



RasterImage* _ImageTransformation_QT::scale_(const RasterImage* imgIn, float sx, float sy, 
											 const ImagePoint* origin)
{
	return NULL;
}


RasterImage* _ImageTransformation_QT::rotate_(const RasterImage* imgIn, float angle, 
											  const ImagePoint* center)
{
	return NULL;
}

#endif	//	URIVL_USE_QUICKTIME
