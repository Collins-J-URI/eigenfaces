/*  NAME:
        Point3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Point3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "Point3D.h"

using namespace uriVL;


Point3D::Point3D(double X, double Y, double Z, const ReferenceFrame3D *theRef)
//        :   frame_(theR
{
    frame_ = theRef;
    frame_->notifyUseAsReference(this);
    XYZ_[0] = X;
    XYZ_[1] = Y;
    XYZ_[2] = Z;
}

Point3D::Point3D(const double theXYZ[], const ReferenceFrame3D *theRef)
			:	Entity3D(theRef)
{
    for (int i=0; i<3; i++)
        XYZ_[i] = theXYZ[i];
}

Point3D::Point3D(const Point3D *thePt)
		:	Entity3D()
{
    FAIL_CONDITION( thePt==NULL,
                    kNullEntity3D,
                    "Attempt to clone a NULL Point3D"); 

    for (int i=0; i<3; i++)
        XYZ_[i] = thePt->XYZ_[i];
}

Point3D::Point3D(double X, double Y, double Z)
{
    XYZ_[0] = X;
    XYZ_[1] = Y;
    XYZ_[2] = Z;
}

Point3D::Point3D(const double theXYZ[])
		:	Entity3D()
{
    for (int i=0; i<3; i++)
        XYZ_[i] = theXYZ[i];
}

Point3D::~Point3D(void)
{
}


const double Point3D::getX(void) const
{
    return XYZ_[0];
}

const double Point3D::getY(void) const
{
    return XYZ_[1];
}

const double Point3D::getZ(void) const
{
    return XYZ_[2];
}

const double* Point3D::getXYZ(void) const
{
    return XYZ_;
}

void Point3D::setPosition(const double XYZ[])
{
    for (int i=0; i<3; i++)
        XYZ_[i] = XYZ[i];
}

void Point3D::setPosition(double X, double Y, double Z)
{
    XYZ_[0] = X;
    XYZ_[1] = Y;
    XYZ_[2] = Z;
}

double Point3D::distanceTo(const Point3D* p) const
{
    FAIL_CONDITION( p==NULL,
                    kNullEntity3D,
                    "Attempt to calculate distance to a NULL Point3D"); 

    double   pXYZ[3] = {p->XYZ_[0], p->XYZ_[1], p->XYZ_[2]};
    
    //  the code for computing the transformation matrix between two
    //  arbitrary reference frames does not exist yet
    if (frame_ != p->frame_)
    {
        FAIL_CONDITION( true,
                        kInvalidTransformation3D,
                        "cannot calculate transformation between arbitrary frames (yet)"); 
        //  something would happen to pXYZ
    }
    
    for (int i=0; i<3; i++)
        pXYZ[i] -= XYZ_[i];
        
    return double(sqrt(pXYZ[0]*pXYZ[0] + pXYZ[1]*pXYZ[1] + pXYZ[2]*pXYZ[2]));
    
}
