/*  NAME:
        PolynomialFunction1D_D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib PolynomialFunction1D_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "PolynomialFunction1D_F.h"

using namespace uriVL;

PolynomialFunction1D_F::PolynomialFunction1D_F(int n, const float *a)
    :   Function1D_F()
{
    initCoeffArray_(n, a);
}


PolynomialFunction1D_F::PolynomialFunction1D_F(int n, const float *a, const float xMin, const float xMax)
    :   Function1D_F(xMin, xMax)
{
    initCoeffArray_(n, a);
}

PolynomialFunction1D_F::~PolynomialFunction1D_F(void)
{
}

#if 0
#pragma mark -
#endif

float PolynomialFunction1D_F::func(float x) const
{
    FAIL_CONDITION( !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in PolynomialFunction1D_F::dfunc");
    
    float val = a_[0][n_];
    for (int i=n_-1; i>=0; i--)                
        val = val*x + a_[0][i];
		
    return val;
}

float PolynomialFunction1D_F::dfunc(float x) const
{
    FAIL_CONDITION( !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in PolynomialFunction1D_F::dfunc");

	int mdeg = n_ - 1;
	float val = a_[1][mdeg];
	for (int i=mdeg-1; i>=0; i--)                
		val = val*x + a_[1][i];

	return val;
}


float PolynomialFunction1D_F::d2func(float x) const
{
    FAIL_CONDITION( !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in PolynomialFunction1D_F::dfunc2");

	int mdeg = n_ - 2;
	float val = a_[2][mdeg];
	for (int i=mdeg-1; i>=0; i--)                
		val = val*x + a_[2][i];
		
	return val;
}


float PolynomialFunction1D_F::dnfunc(float x, int m) const
{
    FAIL_CONDITION( !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in PolynomialFunction1D::dfunc");

    FAIL_CONDITION( m < 0,
                    kInvalidParameterError,
                    "Negative derivative order passed to PolynomialFunction1D::dfunc");
    
	float val = 0.L;
    if (m <= n_) 
    {
        int mdeg = n_ - m;
        val = a_[m][mdeg];
        for (int i=mdeg-1; i>=0; i--)                
            val = val*x + a_[m][i];
    }

	return val;        
}



#if 0
#pragma mark -
#endif

void PolynomialFunction1D_F::initCoeffArray_(int n, const float* a)
{
    FAIL_CONDITION( a == NULL,
                    kNullParameterError,
                    "Null coefficient array passed to PolynomialFunction1D constructor");
    
    FAIL_CONDITION( n < 0,
                    kInvalidParameterError,
                    "Negative degree passed to PolynomialFunction1D constructor");
    
    n_ = n;
    a_ = new float*[n+1];
    for (int i=0; i<=n; i++)
    {
         if (i==0) 
		{
			a_[0] = new float[n + 1];

            for (int j=0; j<=n; j++)
                a_[0][j] = a[j];
        }
		else
        {
			int nd = n - i + 1;
			a_[i] = new float[nd];

		    for (int j=0; j<nd; j++)
                a_[i][j] = (j+1)*a_[i-1][j+1];
        }        
    }    
}
