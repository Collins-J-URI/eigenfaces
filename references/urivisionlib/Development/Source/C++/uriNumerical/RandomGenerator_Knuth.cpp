/*  NAME:
        RandomGenerator_Knuth.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RandomGenerator_Knuth class
 
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
#include "RandomGenerator_Knuth.h"

using namespace uriVL;

RandomGenerator_Knuth::RandomGenerator_Knuth(void)
    :   RandomGenerator(),
		//
		seed_(0)	//	to remove warning
{
}


RandomGenerator_Knuth::RandomGenerator_Knuth(long theSeed)
    :   RandomGenerator(),
		//
		seed_(theSeed > 0 ? -theSeed : theSeed)
{
/*
    if (theSeed > 0)
        seed_ = -theSeed;
    else
        seed_ = theSeed;
*/        
}

RandomGenerator_Knuth::~RandomGenerator_Knuth(void)
{
}

void RandomGenerator_Knuth::setSeed(long theSeed)
{
	
}

unsigned long RandomGenerator_Knuth::nextULong(void)
{
    return 0;
}

double RandomGenerator_Knuth::nextDouble(void)
{
/*
	float		dum;
	int			j;
	unsigned	i, k;
    
	if (*iSeed<0 || iff==0)
	{
		iff = 1;
		i = 2;
	
		//	Version 1:
		//	If your compiler is ANSI-compliant (like CodeWarrior)	
		maxran = RAND_MAX + 1.0;
		//
		//	Version 2:
		//	Non-compliant compilers (most likely VC)
		//	do {
		//		k = i;
		//		i<< = 1;
		//		} while (i);
		//	maxran = k;

		srand(*iSeed);
		*iSeed = 1;
		for (j = 1; j<= 97; j++)
			dum = rand();
		for (j = 1; j<= 97; j++)
			v[j] = rand();

		y = rand();
	}

	j = 1+97.0 * y / maxran;

	//	if (j > 97 || j < 1) nrerror("RAN0: This cannot happen.");

	y = v[j];
	v[j] = rand();

	return y / maxran;
*/
    return 0.L;
}
