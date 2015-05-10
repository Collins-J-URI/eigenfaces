/*  NAME:
        RandomGenerator.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RandomGenerator class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstdlib>
#include <cmath>
//
#include "RandomGenerator.h"

using namespace uriVL;

//---------------------------------------------------------------------------
//  Redeclaration of static member variables
//---------------------------------------------------------------------------

const bool RandomGenerator::INT_IS_LONG = (sizeof(int) == sizeof(long));


RandomGenerator::RandomGenerator(void)
{ }

RandomGenerator::~RandomGenerator(void)
{ }

unsigned int RandomGenerator::nextUInt(void)
{
    if (INT_IS_LONG)
        return  static_cast<unsigned int>(nextULong());
    else
        //  otherwise return most significant part of the unsigned long
        return static_cast<unsigned int>(nextULong() >> 16);
}

int RandomGenerator::nextInt(int lowVal, int highVal)
{
    return static_cast<int>(nextDouble() * (highVal - lowVal + 1)) + lowVal;
}

long RandomGenerator::nextLong(long lowVal, long highVal)
{
    return static_cast<long>(nextDouble() * (highVal - lowVal + 1)) + lowVal;
}

float RandomGenerator::nextFloat(void)
{
    return static_cast<long>(nextDouble());
}

float RandomGenerator::nextFloat(float lowVal, float highVal)
{
    return static_cast<float>(nextDouble() * (highVal - lowVal)) + lowVal;
}

double RandomGenerator::nextGaussian(double sigma)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Function not implemented yet");
    return 0.L;
}
