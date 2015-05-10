/*  NAME:
        Translation3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Translation3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Translation3D.h"

using namespace uriVL;

Translation3D::Translation3D(void) 
{
    tXYZ_[0] = tXYZ_[1] = tXYZ_[2] = 0.L;
}

Translation3D::Translation3D(const double* theTransl) 
{
    if (theTransl != NULL)
        for (int i=0; i<3; i++)
            tXYZ_[i] = theTransl[i];
    else
        for (int i=0; i<3; i++)
            tXYZ_[i] = 0;
}

Translation3D::Translation3D(const Translation3D *theTransl) 
{
    FAIL_CONDITION( theTransl == NULL,
                    kNullTransformation3D,
                    "Null Translation3D passed as argument to Translation3D cloner constructor");

    for (int i=0; i<3; i++)
        tXYZ_[i] = theTransl->tXYZ_[i];

}

Translation3D::~Translation3D(void)
{
}

const double* Translation3D::getVector(void) const
{
    return tXYZ_;
        
}

double* Translation3D::getVector(ReadWriteStatus rwa)
{
    return tXYZ_;
        
}
