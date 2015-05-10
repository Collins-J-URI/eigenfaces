/*  NAME:
        Vector2D_F.cpp

    DESCRIPTION:
        implementation of the uriVisionLib Vector2D_F class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <typeinfo>
#include <cmath>
//
#include "Vector2D_F.h"

using namespace uriVL;



#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors, destructors, operators
//----------------------------------------------------------
#endif

Vector2D_F::Vector2D_F(void)
		:	Vector2D(),
			//
			xf_(0.f),
			yf_(0.f)
{
}


Vector2D_F::Vector2D_F(float theX, float theY)
		:	Vector2D(static_cast<int>(theX), static_cast<int>(theY)),
			//
			xf_(theX),
			yf_(theY)
{
}

Vector2D_F::Vector2D_F(const float xy[])
	try	:	Vector2D(static_cast<int>(xy[0]), static_cast<int>(xy[1])),
			//
			xf_(xy[0]),
			yf_(xy[1])
{
}
catch(...) {
    FAIL_CONDITION( xy == NULL,
                    kNullParameterError,
                    "null coordinate array passed to Vector2D_F constructor");
}


Vector2D_F::Vector2D_F(const Vector2D* theVec)
		:   Vector2D(),
			//
			xf_(0.f),
			yf_(0.f)
{
	try {
		setCoordinates(theVec);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by Vector2D_F constructor.");
		throw e;
	}
}


Vector2D_F::Vector2D_F(const ImagePoint* pt1, const ImagePoint* pt2)
		:   Vector2D(),
			//
			xf_(0.f),
			yf_(0.f)
{
	try {
		setCoordinates(pt1, pt2);
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by Vector2D_F constructor.");
		throw e;
	}
}

Vector2D_F::~Vector2D_F(void)
{
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark access functions
//----------------------------------------------------------
#endif

void Vector2D_F::setCoordinates(const float xy[])
{
    FAIL_CONDITION( xy == NULL,
                    kNullParameterError,
                    "null coordinate array passed to ImagePoint::setCoordinates");

    xf_ = xy[0];
    yf_ = xy[1];
    //
    x_ = static_cast<int>(round(xf_));
    y_ = static_cast<int>(round(yf_)); 
}

void Vector2D_F::setCoordinates(int valX, int valY)
{
    x_ = valX;
    y_ = valY;
    xf_ = valX;
    yf_ = valY;
}

void Vector2D_F::setCoordinates(const int xy[])
{
	try {
		x_ = xy[0];
		y_ = xy[1];
		xf_ = xy[0];
		yf_ = xy[1];
	}
	catch (...) {
		FAIL_CONDITION( xy == NULL,
						kNullParameterError,
						"null coordinate array passed to ImagePoint::setCoordinates");
		FAIL_CONDITION( true,
						kNullParameterError,
						"invalid pointer passed to ImagePoint::setCoordinates");
	}
}

void Vector2D_F::setCoordinates(float valX, float valY)
{
    xf_ = valX;
    yf_ = valY;
    x_=static_cast<int>(round(xf_));
    y_=static_cast<int>(round(yf_));
}


void Vector2D_F::setCoordinates(const Vector2D* theVec)
{
	FAIL_CONDITION( theVec == NULL,
					kNullParameterError,
					"null Vector2D pointer passed to ImagePoint::setCoordinates");

	try {
		if (typeid(*theVec) == typeid(uriVL::Vector2D_F))
		{
			const Vector2D_F	*theVecF = (Vector2D_F*) theVec;
			xf_ = theVecF->xf_;
			yf_ = theVecF->yf_;
			Vector2D::setX(theVec->getX());
			Vector2D::setY(theVec->getY());
		}
		else
		{
			int x = theVec->getX(),
				y = theVec->getY();
			Vector2D::setCoordinates(x, y);
			xf_ = x;
			yf_ = y;
		}
	}
	catch (...) {
		FAIL_CONDITION( true,
						kNullParameterError,
						"invalid Vector2D pointer passed to ImagePoint::setCoordinates");
	}
}

void Vector2D_F::setCoordinates(const ImagePoint* pt1, const ImagePoint* pt2)
{
    FAIL_CONDITION( (pt1 == NULL) || (pt2 == NULL),
                    kNullParameterError,
                    "null ImagePoint passed to Vector2D_F::setCoordinates");
    
    float dx=0.f, dy=0.f;
    
	try {
		if (typeid(*pt2) == typeid(uriVL::ImagePoint_F))
		{
			const ImagePoint_F	*pt2F = (ImagePoint_F*) pt2;
			dx = pt2F->getXF();
			dy = pt2F->getY();
		}
		else
		{
			dx = pt1->getX();
			dy = pt1->getY();
		}
		if (typeid(*pt1) == typeid(uriVL::ImagePoint_F))
		{
			const ImagePoint_F	*pt1F = (ImagePoint_F*) pt1;
			dx -= pt1F->getXF();
			dy -= pt1F->getY();
		}
		else
		{
			dx = pt1->getX();
			dy = pt1->getY();
		}
		
		xf_ = dx;
		yf_ = dy;
		Vector2D::setCoordinates(static_cast<int>(roundf(xf_)), static_cast<int>(roundf(yf_)));
	}
	catch (...) {
		FAIL_CONDITION( true,
						kNullParameterError,
						"invalid ImagePoint pointer passed to ImagePoint::setCoordinates");
	}	
}


#if 0
#pragma mark -
#endif

float Vector2D_F::getXF(void) const
{
    return xf_;
}

float Vector2D_F::getYF(void) const
{
    return yf_;
}

void Vector2D_F::setX(float theX)
{
    x_ = static_cast<int>(round(theX));
    xf_ = theX;
}

void Vector2D_F::setX(int theX)
{
    xf_ = theX;
	x_ = theX;
}

void Vector2D_F::setY(int theY)
{
    y_ = theY;
    yf_ = theY;
}

void Vector2D_F::setY(float theY)
{
    y_ = static_cast<int>(round(theY));
    yf_ = theY;
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark geometric transformations
//----------------------------------------------------------
#endif


void Vector2D_F::rotate(float angle)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


void Vector2D_F::applyRotation(const Rotation2D* theRot)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


