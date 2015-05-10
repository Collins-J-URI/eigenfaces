/*  NAME:
        RandomGenerator_R250_R521.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RandomGenerator_R250_R521 class
 
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
#include "RandomGenerator_R250_521.h"

using namespace uriVL;

#define R250_IA  (sizeof(unsigned long)*103)
#define R250_IB  (sizeof(unsigned long)*R250_LEN - R250_IA)
#define R521_IA  (sizeof(unsigned long)*168)
#define R521_IB  (sizeof(unsigned long)*R521_LEN - R521_IA)
//
static const double INV_MAX_ULONG = 1.L / pow((double)2, (double)(8*sizeof(long))); 
static const int MAX_BITS_IN_RAND = static_cast<int>(log((double)RAND_MAX)/log(2.L) + 0.1L);

//---------------------------------------------------------------------------
//  Redeclaration of static member variables
//---------------------------------------------------------------------------
const int RandomGenerator_R250_R521::FULL_IN_ULONG = ((8*sizeof(unsigned long)) / MAX_BITS_IN_RAND);
const int RandomGenerator_R250_R521::RES_TO_ULONG = ((8*sizeof(unsigned long)) - 
                                             FULL_IN_ULONG*MAX_BITS_IN_RAND);
const int RandomGenerator_R250_R521::FULL_IN_UINT = ((8*sizeof(unsigned int)) / MAX_BITS_IN_RAND);
const int RandomGenerator_R250_R521::RES_TO_UINT = ((8*sizeof(unsigned int)) - 
                                            FULL_IN_UINT*MAX_BITS_IN_RAND);
RandomGenerator_R250_R521  *RandomGenerator_R250_R521::rg_ = new RandomGenerator_R250_R521();

//----------------------------------------------------------------------------
//  Private free functions
//----------------------------------------------------------------------------
unsigned long lrand(void);


//----------------------------------------------------------------------------
//  instance calls for the private generator
//----------------------------------------------------------------------------

RandomGenerator_R250_R521::RandomGenerator_R250_R521(void)
    :   RandomGenerator(),
		//
		r250_index_(0),
		r521_index_(0)
{
    setSeed(time(NULL));
}


RandomGenerator_R250_R521::~RandomGenerator_R250_R521(void)
{
}

//jyh I am not sure to what extent what I do degrades the quality of the generator.
//Because RAND_MAX can be very low (32767) the values stored in the two buffers might
//be very small as well. So I make sure that all 32 bits of the buffers's elements 
//are used.
void RandomGenerator_R250_R521::setSeed(long theSeed)
{
    //jyh
    //I use a seed to initialize the C random generator
    unsigned int mySeed;
    if (INT_IS_LONG)
        mySeed = static_cast<unsigned int>(theSeed);
    else
    {
        //  keep the most significant bits of the time stamp to be the seed
        mySeed = static_cast<unsigned int>(theSeed >> (sizeof(long)-sizeof(int)));
    }
    srand(mySeed);

    int i = R521_LEN;
    unsigned long mask1 = 1;
    unsigned long mask2 = 0xFFFFFFFF;
	
	//jyh   
    while (i-- > R250_LEN) {
        r521_buffer_[i] = lrand();
    }
    while (i-- > 31) {
        r250_buffer_[i] = lrand();
        r521_buffer_[i] = lrand();
    }
    
    /*Michael Brundage
    Establish linear independence of the bit columns
    by setting the diagonal bits and clearing all bits above
    */
    while (i-- > 0) {
        r250_buffer_[i] = (lrand() | mask1) & mask2;
        r521_buffer_[i] = (lrand() | mask1) & mask2;
        mask2 ^= mask1;
        mask1 >>= 1;
    }
    r250_buffer_[0] = mask1;
    r521_buffer_[0] = mask2;
    r250_index_ = 0;
    r521_index_ = 0;
    
    //jyh
    //I call nextULong to get rid of the first number in the generator, which is
    //always an extremely large one (would make a nextDouble of 0.99x)
    nextULong();
}

unsigned long RandomGenerator_R250_R521::nextULong(void)
{
    /*Michael Brundage
    I prescale the indices by sizeof(unsigned long) to eliminate
    four shlwi instructions in the compiled code.  This minor optimization
    increased perf by about 12%.
    
    I also carefully arrange index increments and comparisons to minimize
    instructions.  gcc 3.3 seems a bit weak on instruction reordering. The
    j1/j2 branches are mispredicted, but nevertheless these optimizations
    increased perf by another 10%.
    */
    
    int i1 = r250_index_;
    int i2 = r521_index_;
    unsigned char*  b1 = (unsigned char*)r250_buffer_;
    unsigned char*  b2 = (unsigned char*)r521_buffer_;
    unsigned long * tmp1, * tmp2;
    unsigned long r, s;
    int j1, j2;
    
    j1 = i1 - R250_IB;
    if (j1 < 0)
        j1 = i1 + R250_IA;
    j2 = i2 - R521_IB;
    if (j2 < 0)
        j2 = i2 + R521_IA;
    
    tmp1 = (unsigned long *)(b1 + i1);
    r = (*(unsigned long *)(b1 + j1)) ^ (*tmp1);
    *tmp1 = r;
    tmp2 = (unsigned long *)(b2 + i2);
    s = (*(unsigned long *)(b2 + j2)) ^ (*tmp2);
    *tmp2 = s;
    
    i1 = (i1 != sizeof(unsigned long)*(R250_LEN-1)) ? (i1 + sizeof(unsigned long)) : 0;
    r250_index_ = i1;
    i2 = (i2 != sizeof(unsigned long)*(R521_LEN-1)) ? (i2 + sizeof(unsigned long)) : 0;
    r521_index_ = i2;
        
    return r ^ s;
}

double RandomGenerator_R250_R521::nextDouble(double lowVal, double highVal)
{
    return INV_MAX_ULONG * nextULong() * (highVal - lowVal) + lowVal;
}


double RandomGenerator_R250_R521::nextDouble(void)
{
    return INV_MAX_ULONG * static_cast<double>(nextULong());
}

//----------------------------------------------------------------------------
//  static calls (passed to the private generator)
//----------------------------------------------------------------------------

void RandomGenerator_R250_R521::setSeed_st(long theSeed)
{
    rg_->setSeed(theSeed);
}

unsigned int RandomGenerator_R250_R521::nextUInt_st(void)
{
    return rg_->nextUInt();
}

int RandomGenerator_R250_R521::nextInt_st(int lowVal, int highVal)
{
    return rg_->nextInt(lowVal, highVal);
}

unsigned long RandomGenerator_R250_R521::nextULong_st(void)
{
    return rg_->nextULong();
}

long RandomGenerator_R250_R521::nextLong_st(long lowVal, long highVal)
{
    return rg_->nextLong(lowVal, highVal);
}

float RandomGenerator_R250_R521::nextFloat_st(void)
{
    return rg_->nextFloat();
}

float RandomGenerator_R250_R521::nextFloat_st(float lowVal, float highVal)
{
    return rg_->nextFloat(lowVal, highVal);
}

double RandomGenerator_R250_R521::nextDouble_st(void)
{
    return rg_->nextDouble();
}

double RandomGenerator_R250_R521::nextDouble_st(double lowVal, double highVal)
{
    return rg_->nextDouble(lowVal, highVal);
}

double RandomGenerator_R250_R521::nextGaussian_st(double sigma)
{
    return rg_->nextGaussian(sigma);
}


unsigned long lrand(void)
{
    unsigned long rl = rand();
    for (int i=1; i<RandomGenerator_R250_R521::FULL_IN_ULONG; i++)
        rl = (rl << MAX_BITS_IN_RAND) | rand();

    if (RandomGenerator_R250_R521::RES_TO_ULONG)
    {
        unsigned long lsb = static_cast<unsigned long>(rand()) >> 
                            (MAX_BITS_IN_RAND - RandomGenerator_R250_R521::RES_TO_ULONG);
        rl = (rl << RandomGenerator_R250_R521::RES_TO_ULONG) | lsb;                             
    }    

    return rl;
}
