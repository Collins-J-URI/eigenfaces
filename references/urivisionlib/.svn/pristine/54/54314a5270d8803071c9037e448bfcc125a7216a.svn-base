/*  NAME:
        Rotation2D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Rotation2D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "Rotation2D.h"

using namespace std;
using namespace uriVL;

Rotation2D::Rotation2D(void) 
        :   RigidTransformation2D( )
{
}


Rotation2D::Rotation2D(Rotation2D *theRot) 
        :   RigidTransformation2D( )
{
    FAIL_CONDITION( theRot == NULL,
                    kNullTransformation2D,
                    "Null Rotation2D passed as argument to Rotation2D copy constructor");

    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Rotation2D copy constructor not impemented (again)");


//    if (theRot->matrixIsReference_)
//    {
//        for (int i=0; i<2; i++)
//        {
//            for (int j=0; j<2; j++)
//                mat_[i][j] = theRot->mat_[i][j];
//
//            //  I know that theRot's matrix is supposed to be the reference,
//            //  and therefore valid, but why take chances?  I prefer to
//            //  initialize the translation component to zero myself.
//            mat_[i][2] = 0;
//        }
//        mat_[2][0] = mat_[2][1] = 0.f;
//        mat_[2][2] = 1.f;
//        matrixIsReference_ = true;
//        angleIsReference_ = false;
//    }
//    else //	if (theRot->angleIsReference_)
//    {
//        angle_ = theRot->getAngle();
//        //
//        matrixIsReference_ = false;
//        angleIsReference_ = true;
//    }
}

Rotation2D::~Rotation2D(void)
{
}

