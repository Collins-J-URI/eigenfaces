/*  NAME:
        ImagePoint_F.cpp

    DESCRIPTION:
        implementation of the uriVisionLib ImagePoint_F class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <typeinfo>
#include <cmath>
//
#include "ImagePoint_F.h"

using namespace uriVL;


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

ImagePoint_F::ImagePoint_F(void)
	:	ImagePoint(true),
		//
		xf_(0.f),
		yf_(0.f)
{
}


ImagePoint_F::ImagePoint_F(float theX, float theY)
	:	ImagePoint(static_cast<int>(theX), static_cast<int>(theY), true),
		//
		xf_(theX),
		yf_(theY)
{
}

ImagePoint_F::ImagePoint_F(const float* xy)
	try	:	ImagePoint(static_cast<int>(xy[0]), static_cast<int>(xy[1]), true),
			//
			xf_(xy[0]),
			yf_(xy[1])
{
}
catch(...) {
	FAIL_CONDITION( xy == NULL,
					kNullParameterError,
					"null coordinate array passed to ImagePoint_F constructor.");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid coordinate array passed to ImagePoint_F constructor.");
}


ImagePoint_F::ImagePoint_F(const ImagePoint_F* thePt)
	try	:	ImagePoint(thePt),
			//
			xf_(thePt->xf_),
			yf_(thePt->yf_)
{
}
catch(ErrorReport& e) {
	e.appendToMessage("called by ImagePoint_F constructor.");
	throw e;
}
catch(...) {
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid  ImagePoint pointer passed to ImagePoint_F constructor.");
}


ImagePoint_F::ImagePoint_F(const ImagePoint* thePt)
	try	:	ImagePoint(thePt),
			//
			xf_(thePt->hasSubpixelPrecision() ? static_cast<const ImagePoint_F*>(thePt)->getXF() : thePt->getX()),
			yf_(thePt->hasSubpixelPrecision() ? static_cast<const ImagePoint_F*>(thePt)->getYF() : thePt->getY())
{
}
catch(ErrorReport& e) {
	e.appendToMessage("called by ImagePoint_F constructor.");
	throw e;
}
catch(...) {
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid  ImagePoint pointer passed to ImagePoint_F constructor.");
}


ImagePoint_F::ImagePoint_F(const ImagePoint_F& thePt)
	try	:	ImagePoint(thePt),
			//
			xf_(thePt.xf_),
			yf_(thePt.yf_)
{
}
catch(ErrorReport& e) {
	e.appendToMessage("called by ImagePoint_F constructor.");
	throw e;
}
catch(...) {
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid  ImagePoint reference passed to ImagePoint_F constructor.");
}

ImagePoint_F::~ImagePoint_F(void)
{
}


#if 0
//----------------------------------------
#pragma mark setCoordinates
//----------------------------------------
#endif


void ImagePoint_F::setCoordinates(const float* xy)
{
    FAIL_CONDITION( xy == NULL,
                    kNullParameterError,
                    "null coordinate array passed to ImagePoint::setCoordinates");

	try {
		xf_ = xy[0];
		yf_ = xy[1];
	}
	catch(...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid coordinate pointer passed to ImagePoint_F::setCoordinates.");
	}

    x_ = static_cast<int>(round(xf_));
    y_ = static_cast<int>(round(yf_)); 
}


void ImagePoint_F::setCoordinates(float valX, float valY)
{
    xf_ = valX;
    yf_ = valY;
    x_=static_cast<int>(round(xf_));
    y_=static_cast<int>(round(yf_));
}


void ImagePoint_F::setCoordinates(int valX, int valY)
{
    x_ = valX;
    y_ = valY;
    xf_ = valX;
    yf_ = valY;
}

void ImagePoint_F::setCoordinates(const int* xy)
{
    FAIL_CONDITION( xy == NULL,
                    kNullParameterError,
                    "null coordinate array passed to ImagePoint::setCoordinates");

	try {
		x_ = xy[0];
		y_ = xy[1];
		xf_ = xy[0];
		yf_ = xy[1];
	}
	catch(...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid coordinate pointer passed to ImagePoint_F::setCoordinates.");
	}
}

void ImagePoint_F::setCoordinates(const ImagePoint* thePt)
{
    FAIL_CONDITION( thePt == NULL,
                    kNullParameterError,
                    "null ImagePoint passed to ImagePoint::setCoordinates");

	try {
		if (typeid(*thePt) == typeid(uriVL::ImagePoint_F))
		{
			const ImagePoint_F	*thePtF = (ImagePoint_F*) thePt;
			xf_ = thePtF->xf_;
			yf_ = thePtF->yf_;
			ImagePoint::setX(thePt->getX());
			ImagePoint::setY(y_ = thePt->getY());
		}
		else
		{
			int x = thePt->getX(),
				y = thePt->getY();
			ImagePoint::setX(x);
			ImagePoint::setY(y);
			xf_ = x;
			yf_ = y;
		}
	}
	catch(...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid coordinate pointer passed to ImagePoint_F::setCoordinates.");
	}
}

void ImagePoint_F::setCoordinates(const ImagePoint& thePt)
{
	try {
		if (typeid(thePt) == typeid(uriVL::ImagePoint_F))
		{
			const ImagePoint_F	*thePtF = (ImagePoint_F*) &thePt;
			xf_ = thePtF->xf_;
			yf_ = thePtF->yf_;
			ImagePoint::setX(thePt.getX());
			ImagePoint::setY(y_ = thePt.getY());
		}
		else
		{
			int x = thePt.getX(),
				y = thePt.getY();
			ImagePoint::setX(x);
			ImagePoint::setY(y);
			xf_ = x;
			yf_ = y;
		}
	}
	catch(...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid coordinate pointer passed to ImagePoint_F::setCoordinates.");
	}
}

#if 0
//----------------------------------------
#pragma mark individual setters and getters
//----------------------------------------
#endif


float ImagePoint_F::getXF(void) const
{
    return xf_;
}

float ImagePoint_F::getYF(void) const
{
    return yf_;
}

void ImagePoint_F::setX(float theX)
{
    x_ = static_cast<int>(round(theX));
    xf_ = theX;
}

void ImagePoint_F::setX(int theX)
{
    xf_ = theX;
	x_ = theX;
}

void ImagePoint_F::setY(int theY)
{
    y_ = theY;
    yf_ = theY;
}

void ImagePoint_F::setY(float theY)
{
    y_ = static_cast<int>(round(theY));
    yf_ = theY;
}

#if 0
//----------------------------------------
#pragma mark Geometric transformations
//----------------------------------------
#endif


void ImagePoint_F::shiftX(int dx)
{
    x_ += dx;
    xf_ += dx;
}

void ImagePoint_F::shiftX(float dx)
{
    xf_ += dx;
    x_=static_cast<int>(round(xf_));
}

void ImagePoint_F::shiftY(float dy)
{
    yf_ += dy;
    y_=static_cast<int>(round(yf_));
}

void ImagePoint_F::shiftY(int dy)
{
    y_ += dy;
    yf_ += dy;
}

void ImagePoint_F::translate(int dx, int dy)
{
    x_ += dx;
    y_ += dy;
    xf_ += dx;
    yf_ += dy;
}

void ImagePoint_F::translate(float dx, float dy)
{
    xf_ += dx;
    yf_ += dy;
    x_ = static_cast<int>(round(xf_));
    y_ = static_cast<int>(round(yf_));
}


void ImagePoint_F::rotate(float angle, const ImagePoint* rotCenter)
{
    const float cT = cosf(angle),
                sT = sinf(angle);

    //  if the rotation center is null we rotate around the origin
    if (rotCenter == NULL)
    {
        const float x = cT*xf_ - sT*yf_;
        yf_ = sT*xf_ + cT*yf_;
        xf_ = x;
    }
    else
    {
		float xcf, ycf;
    	if (typeid(*rotCenter) == typeid(uriVL::ImagePoint_F))
        {
        	const ImagePoint_F	*rotCenterF = (ImagePoint_F*) rotCenter;
        	xcf = rotCenterF->xf_;
    		ycf = rotCenterF->yf_;
        }
        else
        {
        	xcf = rotCenter->getX();
    		ycf = rotCenter->getY();
        }
		const float dx = x_ - xcf;
		const float dy = y_ - ycf;
		xf_ = xcf + cT*dx - sT*dy;
		yf_ = ycf + sT*dx + cT*dy;
    }

    x_ = static_cast<int>(round(xf_));
    y_ = static_cast<int>(round(yf_));
}


void ImagePoint_F::applyTransformation(const Transformation2D* theTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


