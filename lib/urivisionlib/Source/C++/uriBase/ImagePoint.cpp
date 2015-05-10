/*  NAME:
        ImagePoint.cpp

    DESCRIPTION:
        implementation of the uriVisionLib ImagePoint class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <typeinfo>
//
#include "ImagePoint.h"
#include "ImagePoint_F.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//	Initizalization of class variables
//----------------------------------------------------------

int ImagePoint::ptCounter_ = 0;


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark constructors, destructors, operators
//----------------------------------------------------------
#endif


ImagePoint::ImagePoint(bool hasSubpixelPrec)
		:	x_(0),
			y_(0),
			id_(ptCounter_++)
{
}


ImagePoint::ImagePoint(int theX, int theY, bool hasSubpixelPrec)
		:	x_(theX),
			y_(theY),
			id_(ptCounter_++)
{
}

ImagePoint::ImagePoint(const int* xy, bool hasSubpixelPrec)
	try	:	x_(xy[0]),
			y_(xy[1]),
			id_(ptCounter_++)
{
}
catch(...) {
	FAIL_CONDITION( xy == NULL,
					kNullParameterError,
					"null coordinate array passed to ImagePoint constructor.");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid coordinate array passed to ImagePoint constructor.");
}


ImagePoint::ImagePoint(const ImagePoint* thePt)
	try :	x_(thePt->x_),
			y_(thePt->y_),
			id_(ptCounter_++)
{
}
catch(...) {
	FAIL_CONDITION( thePt == NULL,
					kNullParameterError,
					"null ImagePoint pointer passed to ImagePoint constructor.");
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid  ImagePoint pointer passed to ImagePoint constructor.");
}


ImagePoint::ImagePoint(const ImagePoint& thePt)
	try :	x_(thePt.x_),
			y_(thePt.y_),
			id_(ptCounter_++)
{
}
catch(...) {
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid  ImagePoint reference passed to ImagePoint copy constructor.");
}

ImagePoint::~ImagePoint(void)
{
}

const ImagePoint& ImagePoint::operator = (const ImagePoint& thePt)
{
	try {
		x_ = thePt.x_;
		y_ = thePt.y_;
	}
	catch(...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid  ImagePoint reference passed to ImagePoint = operator.");
	}
	return *this;
}

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark access functions
//----------------------------------------------------------
#endif

bool ImagePoint::hasSubpixelPrecision(void) const
{
	return false;
}


int ImagePoint::getX(void) const
{
    return x_;
}

int ImagePoint::getY(void) const
{
    return y_;
}

int ImagePoint::getID(void) const
{
	return id_;
}

void ImagePoint::setX(int theX)
{
    x_ = theX;
}

void ImagePoint::setY(int theY)
{
    y_ = theY;
}

void ImagePoint::setCoordinates(int valX, int valY)
{
    x_ = valX;
    y_ = valY;
}

void ImagePoint::setCoordinates(const int xy[])
{
    FAIL_CONDITION( xy == NULL,
                    kNullParameterError,
                    "null coordinate array passed to ImagePoint::setCoordinates");

    x_ = xy[0];
    y_ = xy[1];
}

void ImagePoint::setCoordinates(const ImagePoint* thePt)
{
    FAIL_CONDITION( thePt == NULL,
                    kNullParameterError,
                    "null ImagePoint passed to ImagePoint::setCoordinates");

    x_ = thePt->x_;
    y_ = thePt->y_;
}

void ImagePoint::setCoordinates(const ImagePoint& thePt)
{
	try {
		x_ = thePt.x_;
		y_ = thePt.y_;
	}
	catch(...) {
	    FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid ImagePoint reference passed to ImagePoint::setCoordinates");


	}
}

#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark geometric transformations
//----------------------------------------------------------
#endif

void ImagePoint::shiftX(int dx)
{
    x_ += dx;
}

void ImagePoint::shiftY(int dy)
{
    y_ += dy;
}

void ImagePoint::translate(int dx, int dy)
{
    x_ += dx;
    y_ += dy;
}

void ImagePoint::rotate(float angle, const ImagePoint* rotCenter)
{
    const float cT = cosf(angle),
                sT = sinf(angle);

    //  if the rotation center is null we rotate around the origin
    if (rotCenter == NULL)
    {
        const int x = static_cast<int>(cT*x_ - sT*y_);
        y_ = static_cast<int>(sT*x_ + cT*y_);
        x_ = x;
    }
    else
    {
    	if (typeid(*rotCenter) == typeid(uriVL::ImagePoint_F))
        {
        	const ImagePoint_F	*rotCenterF = (ImagePoint_F*) rotCenter;
        	const float xcf = rotCenterF->getXF();
    		const float ycf = rotCenterF->getYF();
    		const float dx = x_ - xcf;
    		const float dy = y_ - ycf;
    		x_ = static_cast<int>(xcf + cT*dx - sT*dy);
    		y_ = static_cast<int>(ycf + sT*dx + cT*dy);
        }
        else
        {
        	const int xc = rotCenter->x_;
    		const int yc = rotCenter->y_;
    		const float dx = x_ - xc;
    		const float dy = y_ - yc;
    		x_ = xc + static_cast<int>(cT*dx - sT*dy);
    		y_ = yc + static_cast<int>(sT*dx + cT*dy);
        }
    }
}


void ImagePoint::applyTransformation(const Transformation2D* theTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
}


float ImagePoint::distance(const ImagePoint* pt1, const ImagePoint* pt2)
{
//	try 
//	{
		
		float x1, x2, y1, y2;

		if (typeid(*pt1) == typeid(uriVL::ImagePoint_F))
		{
			const ImagePoint_F* pt1F = static_cast<const ImagePoint_F*>(pt1);
			
			x1 = pt1F->getXF();
			y1 = pt1F->getYF();
		}
		else 
		{
			x1 = pt1->getX();
			y1 = pt1->getY();
		}

		if (typeid(*pt2) == typeid(uriVL::ImagePoint_F))
		{
			const ImagePoint_F* pt2F = static_cast<const ImagePoint_F*>(pt2);
			
			x2 = pt2F->getXF();
			y2 = pt2F->getYF();
		}
		else 
		{
			x2 = pt2->getX();
			y2 = pt2->getY();
		}

		float dx = x1 - x2, dy = y1 - y2;
		
		return sqrtf(dx*dx + dy*dy);
//	}
//	catch(...) 
//	{
//		
//	}
}


#if 0
//----------------------------------------------------------
#pragma mark -
#pragma mark Free functions drawing a point
//----------------------------------------------------------
#endif

void uriVL::drawPoint_disk(float cx, float cy, float rad, const float* color)
{
//	// draw a circle
//    float nPolygonAngle = 0.f;
//    float nRotationAmount = 360.f / DRAW_POINT_SIDES;
//    float** circlePts = new float*[DRAW_POINT_SIDES];
//
//    for(int i = 0; i < DRAW_POINT_SIDES; i++ )
//    {
//        circlePts[i] = new float[2];
//        circlePts[i][0] = 1.5f * cosf(nPolygonAngle);
//        circlePts[i][1] = 1.5f * sinf(nPolygonAngle);
//        nPolygonAngle += static_cast<float>((nRotationAmount*PI)/180.f);
//    }

}
	
	 
void uriVL::drawPoint_square(float cx, float cy, float rad, const float* color)
{
}
	 
void uriVL::drawPoint_diamond(float cx, float cy, float rad, const float* color)
{
}


