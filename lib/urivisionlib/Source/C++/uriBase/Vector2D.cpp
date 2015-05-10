/*  NAME:
        Vector2D.cpp

    DESCRIPTION:
        implementation of the uriVisionLib Vector2D class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Vector2D.h"

using namespace uriVL;


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors, destructors, operators
//----------------------------------------------------------
#endif

Vector2D::Vector2D(void)
		:	x_(0),
			y_(0)
{
}


Vector2D::Vector2D(int theX, int theY)
		:	x_(theX),
			y_(theY)
{
}

Vector2D::Vector2D(const int xy[])
	try	:	x_(xy[0]),
			y_(xy[1])
{
}
catch(...) {
    FAIL_CONDITION( xy == NULL,
                    kNullParameterError,
                    "null coordinate array passed to Vector2D constructor");
}

Vector2D::Vector2D(const Vector2D* theVect)
	try	:	x_(theVect->x_),
			y_(theVect->y_)
{
}
catch(...) {
    FAIL_CONDITION( theVect == NULL,
                    kNullParameterError,
                    "null Vector2D passed to copy constructor constructor");
}


Vector2D::Vector2D(const ImagePoint* pt1, const ImagePoint* pt2)
	try	:	x_(pt2->getX() - pt1->getX()),
			y_(pt2->getY() - pt1->getY())
{
}
catch(...) {
    FAIL_CONDITION( (pt1 == NULL) || (pt2 == NULL),
                    kNullParameterError,
                    "null Point2D passed to Vector2D constructor constructor");
}


Vector2D::~Vector2D(void)
{
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark access functions
//----------------------------------------------------------
#endif


int Vector2D::getX(void) const
{
    return x_;
}

int Vector2D::getY(void) const
{
    return y_;
}

void Vector2D::setX(int theX)
{
    x_ = theX;
}

void Vector2D::setY(int theY)
{
    y_ = theY;
}

void Vector2D::setCoordinates(int valX, int valY)
{
    x_ = valX;
    y_ = valY;
}

void Vector2D::setCoordinates(const int xy[])
{
    FAIL_CONDITION( xy == NULL,
                    kNullParameterError,
                    "null coordinate array passed to Vector2D::setCoordinates");

    x_ = xy[0];
    y_ = xy[1];
}

void Vector2D::setCoordinates(const Vector2D* theVect)
{
    FAIL_CONDITION( theVect == NULL,
                    kNullParameterError,
                    "null Vector2D passed to Vector2D::setCoordinates");

    x_ = theVect->x_;
    y_ = theVect->y_;
}

void Vector2D::setCoordinates(const ImagePoint* pt1, const ImagePoint* pt2)
{
    FAIL_CONDITION( (pt1 == NULL) || (pt2 == NULL),
                    kNullParameterError,
                    "null Point2D passed to Vector2D::setCoordinates");
    x_ = pt2->getX() - pt1->getX();
    y_ = pt2->getY() - pt1->getY();
}



#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark geometric transformations
//----------------------------------------------------------
#endif


void Vector2D::rotate(float angle)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


void Vector2D::applyRotation(const Rotation2D* theRot)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}



