/*  NAME:
        PolynomialFunction1D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib PolynomialFunction1D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "PolynomialFunction1D.h"

using namespace uriVL;

PolynomialFunction1D::PolynomialFunction1D(int n, const double* a)
    :   Function1D()
{
    initCoeffArray_(n, a);
}


PolynomialFunction1D::PolynomialFunction1D(int n, const double *a, const double xMin, const double xMax)
    :   Function1D(xMin, xMax)
{
    initCoeffArray_(n, a);
}

PolynomialFunction1D::~PolynomialFunction1D(void)
{
}

#if 0
#pragma mark -
#endif

double PolynomialFunction1D::func(double x) const
{
    FAIL_CONDITION( !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in PolynomialFunction1D::dfunc");
    
    double val = a_[0][n_];
    for (int i=n_-1; i>=0; i--)                
        val = val*x + a_[0][i];
    return val;
}

double PolynomialFunction1D::dfunc(double x) const
{
    FAIL_CONDITION( !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in PolynomialFunction1D::dfunc");

	int mdeg = n_ - 1;
	double val = a_[1][mdeg];
	for (int i=mdeg-1; i>=0; i--)                
		val = val*x + a_[1][i];
	return val;
}


double PolynomialFunction1D::d2func(double x) const
{
    FAIL_CONDITION( !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in PolynomialFunction1D::dfunc2");

	int mdeg = n_ - 2;
	double val = a_[2][mdeg];
	for (int i=mdeg-1; i>=0; i--)                
		val = val*x + a_[2][i];
	return val;
}


double PolynomialFunction1D::dnfunc(double x, int m) const
{
    FAIL_CONDITION( !isDefinedAt(x),
                    kDataRangeError,
                    "x not within domain of definition in PolynomialFunction1D::dfuncn");

    FAIL_CONDITION( m < 0,
                    kInvalidParameterError,
                    "Negative derivative order passed to PolynomialFunction1D::dfuncn");
    
	double val = 0.L;
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

void PolynomialFunction1D::initCoeffArray_(int n, const double *a)
{
    FAIL_CONDITION( a == NULL,
                    kNullParameterError,
                    "Null coefficient array passed to PolynomialFunction1D constructor");
    
    FAIL_CONDITION( n < 0,
                    kInvalidParameterError,
                    "Negative degree passed to PolynomialFunction1D constructor");
    
    n_ = n;
    a_ = new double*[n+1];
    for (int i=0; i<=n; i++)
    {
         if (i==0) 
		{
			a_[0] = new double[n + 1];

            for (int j=0; j<=n; j++)
                a_[0][j] = a[j];
        }
		else
        {
			int nd = n - i + 1;
			a_[i] = new double[nd];

		    for (int j=0; j<nd; j++)
                a_[i][j] = (j+1)*a_[i-1][j+1];
        }        
    }    
}

