/*  NAME:
        RandomGenerator_MT.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RandomGenerator_MT class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstdlib>
#include <cmath>
#include <ctime>
//
#include "RandomGenerator_MT.h"

using namespace uriVL;

static const double INV_MAX_ULONG = 1.L / pow((double)2, (double)(8*sizeof(long))); 
static const int MAX_BITS_IN_RAND = static_cast<int>(log((double)RAND_MAX)/log(2.L) + 0.1L);

//---------------------------------------------------------------------------
//  Redeclaration of static member variables
//---------------------------------------------------------------------------
RandomGenerator_MT  *RandomGenerator_MT::rg_ = new RandomGenerator_MT();


//----------------------------------------------------------------------------
//  instance calls for the private generator
//----------------------------------------------------------------------------

RandomGenerator_MT::RandomGenerator_MT(void)
    :   RandomGenerator(),
		//
		seed_(time(NULL))
{
}


RandomGenerator_MT::~RandomGenerator_MT(void)
{
}

void RandomGenerator_MT::setSeed(long theSeed)
{
	seed_ = theSeed;
}

unsigned long RandomGenerator_MT::nextULong(void)
{
    return 0L;
}

double RandomGenerator_MT::nextDouble(double lowVal, double highVal)
{
    return INV_MAX_ULONG * nextULong() * (highVal - lowVal) + lowVal;
}


double RandomGenerator_MT::nextDouble(void)
{
    return INV_MAX_ULONG * nextULong();
}

//----------------------------------------------------------------------------
//  static calls (passed to the private generator)
//----------------------------------------------------------------------------

void RandomGenerator_MT::setSeed_st(long theSeed)
{
    rg_->setSeed(theSeed);
}

unsigned int RandomGenerator_MT::nextUInt_st(void)
{
    return rg_->nextUInt();
}

int RandomGenerator_MT::nextInt_st(int lowVal, int highVal)
{
    return rg_->nextInt(lowVal, highVal);
}

unsigned long RandomGenerator_MT::nextULong_st(void)
{
    return rg_->nextULong();
}

long RandomGenerator_MT::nextLong_st(long lowVal, long highVal)
{
    return rg_->nextLong(lowVal, highVal);
}

float RandomGenerator_MT::nextFloat_st(void)
{
    return rg_->nextFloat();
}

float RandomGenerator_MT::nextFloat_st(float lowVal, float highVal)
{
    return rg_->nextFloat(lowVal, highVal);
}

double RandomGenerator_MT::nextDouble_st(void)
{
    return rg_->nextDouble();
}

double RandomGenerator_MT::nextDouble_st(double lowVal, double highVal)
{
    return rg_->nextDouble(lowVal, highVal);
}

double RandomGenerator_MT::nextGaussian_st(double sigma)
{
    return rg_->nextGaussian(sigma);
}
