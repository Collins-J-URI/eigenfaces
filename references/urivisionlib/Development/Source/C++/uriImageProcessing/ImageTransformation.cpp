/*
 *  ImageTransformation.cpp
 *  uriVisionLib
 *
 *  Created by jyh Local on 2013-06-05.
 *  Copyright 2013 3DG, Univ. of Rhode Island. All rights reserved.
 *
 */

#include "ImageTransformation.h"
//
#if URIVL_USE_QUICKTIME
	#include "_ImageTransformation_QT.h"
#elif URIVL_USE_FREEIMAGE
	#include "_ImageTransformation_FreeImage.h"
#else
	//	???
#endif

using namespace uriVL;


RasterImage* ImageTransformation::translate(const RasterImage* imgIn, int dx, int dy)
{
	RasterImage* imgOut = NULL;
	
	try
	{
		imgOut = RasterImage::createCopy(imgIn);
		imgOut->translate(dx, dy);
	}
	catch (ErrorReport& e)
	{
		if (imgOut != NULL)
			delete imgOut;
		imgOut = NULL;
		
		e.appendToMessage("called by ImageTransformation::translate");
		throw e;
	}
	catch (...)
	{
		if (imgOut != NULL)
			delete imgOut;
		imgOut = NULL;
		
		FAIL_CONDITION( true,
						kImageProcessingError,
						"Unexpected error encountered in ImageTransformation::translate");
	}
		
	return imgOut;
}

void ImageTransformation::translateInPlace(RasterImage* imgIn, int dx, int dy)
{
	try
	{
		imgIn->translate(dx, dy);
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageTransformation::translateInPlace");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kImageProcessingError,
						"Unexpected error encountered in ImageTransformation::translateInPlace");
	}
}

RasterImage* ImageTransformation::scale(const RasterImage* imgIn, float sx, float sy, 
										const ImagePoint* origin)
{
	RasterImage* imgOut = NULL;
	
	try
	{

		#if URIVL_USE_QUICKTIME
		
		
		#elif URIVL_USE_FREEIMAGE
		
		
		#else
			FAIL_CONDITION( true,
							kFunctionNotImplemented,
							"Only the QuickTime and FreeImage versions implement ImageTransformation::scale");
		#endif
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageTransformation::scale");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kImageProcessingError,
						"Unexpected error encountered in ImageTransformation::scale");
	}
		
	return imgOut;
}

RasterImage* ImageTransformation::rotate(const RasterImage* imgIn, float angle, 
										 const ImagePoint* center)
{
	RasterImage* imgOut = NULL;
	
	try
	{
		//	First, treat the case of an attempt to rotate a binary image by an
		//	angle that is not a multiple of 90 degree --> return NULL
		BaseRasterType rasterType = imgIn->getBaseRasterType();
		if (rasterType == kBinaryRaster)
		{
			const float angleDiv90 = angle/90.f;
			if (floorf(angleDiv90) != angleDiv90)
				return NULL;

		}
		
		//	All other cases
		#if URIVL_USE_QUICKTIME
		
			return _ImageTransformation_QT::rotate_(imgIn, angle, center);
		
		#elif URIVL_USE_FREEIMAGE
		
			return _ImageTransformation_FreeImage::rotate_(imgIn, angle, center);
		
		#else
			FAIL_CONDITION( true,
							kFunctionNotImplemented,
							"Only the QuickTime and FreeImage versions implement ImageTransformation::rotate");
		#endif		
	}
	catch (ErrorReport& e)
	{
		e.appendToMessage("called by ImageTransformation::rotate");
		throw e;
	}
	catch (...)
	{
		FAIL_CONDITION( true,
						kImageProcessingError,
						"Unexpected error encountered in ImageTransformation::rotate");
	}
		
	return imgOut;
}
