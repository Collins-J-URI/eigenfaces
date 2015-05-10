/*  NAME:
        RandomGenerator_C.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RandomGenerator_C class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstdlib>
#include <ctime>
#include <cmath>
//
#include "RandomGenerator_C.h"

using namespace uriVL;

static const double INV_MAX_ULONG = 1.L / pow((double)2, (double)(8*sizeof(long))); 
static const int MAX_BITS_IN_RAND = static_cast<int>(log((double)RAND_MAX)/log(2.L) + 0.1L);

//---------------------------------------------------------------------------
//  Redeclaration of static member variables
//---------------------------------------------------------------------------
const int RandomGenerator_C::FULL_IN_ULONG = ((8*sizeof(unsigned long)) / MAX_BITS_IN_RAND);
const int RandomGenerator_C::RES_TO_ULONG = ((8*sizeof(unsigned long)) - 
                                             FULL_IN_ULONG*MAX_BITS_IN_RAND);
const int RandomGenerator_C::FULL_IN_UINT = ((8*sizeof(unsigned int)) / MAX_BITS_IN_RAND);
const int RandomGenerator_C::RES_TO_UINT = ((8*sizeof(unsigned int)) - 
                                            FULL_IN_UINT*MAX_BITS_IN_RAND);
RandomGenerator_C  *RandomGenerator_C::rg_ = new RandomGenerator_C();


//----------------------------------------------------------------------------
//  instance calls for the private generator
//----------------------------------------------------------------------------

RandomGenerator_C::RandomGenerator_C(void)
    :   RandomGenerator(),
		//
		seed_(0)
{
    setSeed(time(NULL));
}


RandomGenerator_C::~RandomGenerator_C(void)
{
}

void RandomGenerator_C::setSeed(long theSeed)
{
    if (INT_IS_LONG)
        seed_ = static_cast<unsigned int>(theSeed);
    else
    {
        //  keep the most significant bits of the time stamp to be the seed
        seed_ = static_cast<unsigned int>(theSeed >> (sizeof(long)-sizeof(int)));
    }

    srand(seed_);
}

unsigned long RandomGenerator_C::nextULong(void)
{
    unsigned long rl = rand();
    for (int i=1; i<FULL_IN_ULONG; i++)
        rl = (rl << MAX_BITS_IN_RAND) | rand();

    if (RES_TO_ULONG)
    {
        unsigned long lsb = static_cast<unsigned long>(rand()) >> 
                                       (MAX_BITS_IN_RAND - RES_TO_ULONG);
        rl = (rl << RES_TO_ULONG) | lsb;                             
    }    

    return rl;
}

double RandomGenerator_C::nextDouble(double lowVal, double highVal)
{
    return INV_MAX_ULONG * nextULong() * (highVal - lowVal) + lowVal;
}


double RandomGenerator_C::nextDouble(void)
{
    return INV_MAX_ULONG * nextULong();
}

//----------------------------------------------------------------------------
//  static calls (passed to the private generator)
//----------------------------------------------------------------------------

void RandomGenerator_C::setSeed_st(long theSeed)
{
    rg_->setSeed(theSeed);
}

unsigned int RandomGenerator_C::nextUInt_st(void)
{
    return rg_->nextUInt();
}

int RandomGenerator_C::nextInt_st(int lowVal, int highVal)
{
    return rg_->nextInt(lowVal, highVal);
}

unsigned long RandomGenerator_C::nextULong_st(void)
{
    return rg_->nextULong();
}

long RandomGenerator_C::nextLong_st(long lowVal, long highVal)
{
    return rg_->nextLong(lowVal, highVal);
}

float RandomGenerator_C::nextFloat_st(void)
{
    return rg_->nextFloat();
}

float RandomGenerator_C::nextFloat_st(float lowVal, float highVal)
{
    return rg_->nextFloat(lowVal, highVal);
}

double RandomGenerator_C::nextDouble_st(void)
{
    return rg_->nextDouble();
}

double RandomGenerator_C::nextDouble_st(double lowVal, double highVal)
{
    return rg_->nextDouble(lowVal, highVal);
}

double RandomGenerator_C::nextGaussian_st(double sigma)
{
    return rg_->nextGaussian(sigma);
}
