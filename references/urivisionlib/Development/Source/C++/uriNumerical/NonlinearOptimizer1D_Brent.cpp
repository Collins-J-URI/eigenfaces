/*  NAME:
        NonlinearOptimizer1D_Brent.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib NonlinearOptimizer1D_Brent class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "NonlinearOptimizer1D_Brent.h"

using namespace std;
using namespace uriVL;

const double TAU = 0.618033988749895L;
//
const double TAU1 = 1.L - TAU;
const double INV_TAU = 1.L / TAU;
const float TAU_F = static_cast<float>(TAU);
const float TAU1_F = static_cast<float>(TAU1);
const float INV_TAU_F = static_cast<float>(INV_TAU);
//
const double SMALL_STEP_FRAC = 0.01L;
const double STEP_GAIN = 1.1L;
//
const float SMALL_STEP_FRAC_F = static_cast<float>(SMALL_STEP_FRAC);
const float STEP_GAIN_F = static_cast<float>(STEP_GAIN);
//
const int MAX_NB_ITERS = 1000;


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructor
//-------------------------------------------------------------------
#endif

NonlinearOptimizer1D_Brent::NonlinearOptimizer1D_Brent(void)
    :   NonlinearOptimizer1D()
{

}

NonlinearOptimizer1D_Brent::NonlinearOptimizer1D_Brent(const Function1D* f)
		try	:   NonlinearOptimizer1D(f)
{

}
catch(ErrorReport& e)
{
	e.appendToMessage("called by NonlinearOptimizer1D_Brent constructor");
	throw e;
}	

NonlinearOptimizer1D_Brent::NonlinearOptimizer1D_Brent(const Function1D_F* f)
		try	:   NonlinearOptimizer1D(f)
{

}
catch(ErrorReport& e)
{
	e.appendToMessage("called by NonlinearOptimizer1D_gss constructor");
	throw e;
}	


NonlinearOptimizer1D_Brent::~NonlinearOptimizer1D_Brent(void)
{
}


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark minimization private functions
//-------------------------------------------------------------------
#endif

void NonlinearOptimizer1D_Brent::minimize_(const Function1D_F *f, float xStart, float xMin,
              float xMax, float tol, OptimizerRecord1D* optRec)
{
    //  work on the intersection of the domain of definition and of the search interval
    //  specified by the user
    float  funcXMin = f->getLowerBound(),
           funcXMax = f->getUpperBound();
    if (xMin > funcXMin)
       funcXMin = xMin;
    if (xMax < funcXMax)
        funcXMax = xMax;

    //  determine an initial search bracket        
// unused vars. -CJC
//     float   a, b, x,
//             fA, fB, fX;
//     int     nbIters = 0;
             
// unused var. -CJC
//     bool foundBracket = initSearchBracket_(f, xStart, funcXMin, funcXMax, &a, &b, &x, 
//                                             &fA, &fB, &fX, &nbIters);

    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}


void NonlinearOptimizer1D_Brent::minimize_(const Function1D* f, double xStart, double xMin,
              double xMax, double tol, OptimizerRecord1D* optRec)
{
    //  work on the intersection of the domain of definition and of the search interval
    //  specified by the user
    double funcXMin = f->getLowerBound(),
           funcXMax = f->getUpperBound();
    if (xMin > funcXMin)
       funcXMin = xMin;
    if (xMax < funcXMax)
        funcXMax = xMax;

    //  determine an initial search bracket        
// unused vars. -CJC
//     double  a, b, x,
//             fA, fB, fX;
//     int     nbIters = 0;
             
// unused var. -CJC
//     bool foundBracket = initSearchBracket_(f, xStart, funcXMin, funcXMax, &a, &b, &x, 
//                                             &fA, &fB, &fX, &nbIters);

    FAIL_CONDITION(	true,
    				kFunctionNotImplemented,
    				"not implemented yet");

}


bool NonlinearOptimizer1D_Brent::initSearchBracket_(const Function1D_F* f, float xStart, 
                                    float xMin, float xMax, float* theA, 
                                    float* theB, float* theX, float* theFA, float* theFB, 
                                    float* theFX, int* nbIters)
{
    float   a, b, c,
            fA, fB, fC;
    
    a = xStart;
    fA = f->func(xStart);
    //  I make a small step (arbitrarily, 1/100th of the search interval) on the right side
    //  of a to see which way the function is going down.
    if ((xMin != -HUGE_VALF) && (xMax != HUGE_VALF))
        b = xStart + (xMax - xMin)*SMALL_STEP_FRAC_F;
    else if (xStart == 0)
        b = xStart + SMALL_STEP_FRAC_F;
    else if (xStart > 0)
        b = xStart * (1.0L + SMALL_STEP_FRAC_F);
    else
        b = xStart * (1.0L - SMALL_STEP_FRAC_F);
    
    fB = f->func(static_cast<float>(b));

    //  if the function is going down on the right side -> positive steps
    //  if the function is going down on the left side -> negative steps
    const bool  LEFT_TO_RIGHT = (fB < fA);
    if (!LEFT_TO_RIGHT)
    {
        //  swap a and b
        c = a; fC = fA;
        a = b; fA = fB;
        b = c; fB = fC;
    }

    *nbIters = 0;    
    c = a + INV_TAU_F * (b-a);
    fC = f->func(static_cast<float>(c));  
    
    if (LEFT_TO_RIGHT)
    {
        while ((fB > fC) && ((*nbIters)<=MAX_NB_ITERS))
        {
            (*nbIters)++;
            
            //  perform the parabolic interpolation
            float	bmc = b-c, cpb = c+b,
					cma = c-a, apc = a+c,
                    amb = a-b, bpa = b+a,
                    bmcfa = bmc*fA, cmafb = cma*fB, ambfc = amb*fC;
            float	d = bmcfa + cmafb + ambfc;
            
            //  We can use the cubic interpolation if the function  goes down L2R and d<0
            //  or the function goes down R2L and d>0
            if (d < 0) 
            {
                float e = cpb*bmcfa + apc*cmafb + bpa*ambfc;
                float x = 0.5f * e / d;
                
                //  if x < c, then it can only be between b and c
                if (x < c)
                {
                    float fX = f->func(x);
                    
                    //  we have found a bracket, the search is over
                    if (fX < fC)
                    {
                        *theA = b; *theFA = fB;
                        *theX = x; *theFX = fX;
                        *theB = c; *theFB = fC;
                        return true;
                    }
                    //  otherwise, we have to do one step of golden extrapolation
                    else
                    {
                        a = x; fA = fX;
                        b = c; fB = fB;
                        c = a + INV_TAU_F * (b-a);
                        if (c > xMax)
                            c = xMax;
                        fC = f->func(c);
                    }
                }
                //  if x >= c
                else if (x < xMax)
                {
                    float fX = f->func(x);
                    
                    //  we have found a bracket, the search is over
                    if (fX > fC)
                    {
                        *theA = b; *theFA = fB;
                        *theX = c; *theFX = fC;
                        *theB = x; *theFB = fX;
                        return true;
                    }
                    //  otherwise, we have to do one step of golden extrapolation
                    //  of c-x
                    else
                    {
                        a = c; fA = fC;
                        b = x; fB = fX;
                        c = a + INV_TAU_F * (b-a);
                        if (c > xMax)
                            c = xMax;
                        fC = f->func(c);
                    }
                }
                //  x is too far on the right
                else
                {
                    a = b; fA = fB;
                    b = c; fB = fC;
                    c = b + INV_TAU_F * (b-a);
                    if (c > xMax)
                        c = xMax;
                    fC = f->func(c);
                }
               
            }
            //  if d has the wrong sign, we have to do one step of golden extrapolation
            else
            {
                a = b; fA = fB;
                b = c; fB = fC;
                c = b + INV_TAU_F * (b-a);
                if (c > xMax)
                    c = xMax;
                fC = f->func(c);
            }
        }            

        if ((*nbIters) > MAX_NB_ITERS)
            return false;
            
        else
        {
            *theA = a;
            *theFA = fA;
            *theX = b;
            *theFX = fB;
            *theB = c;
            *theFB = fC;
            return true;
        }
    }
    
    //  if we are going right to left
    else
    {
        while ((fB > fC) && ((*nbIters)<=MAX_NB_ITERS))
        {
            (*nbIters)++;
            
            //  perform the parabolic interpolation
            float	bmc = b-c, cpb = c+b,
                    cma = c-a, apc = a+c,
                    amb = a-b, bpa = b+a,
                    bmcfa = bmc*fA, cmafb = cma*fB, ambfc = amb*fC;
            float	d = bmcfa + cmafb + ambfc;
            
            if (d > 0) 
            {
                float   e = cpb*bmcfa + apc*cmafb + bpa*ambfc;
                float   x = 0.5f * e / d;
                
                //  if x > c, then it can only be between b and c
                if (x > c)
                {
                    float   fX = f->func(x);
                    
                    //  we have found a bracket, the search is over
                    if (fX < fC)
                    {
                        *theA = c; *theFA = fC;
                        *theX = x; *theFX = fX;
                        *theB = b; *theFB = fB;
                        return true;
                    }
                    //  otherwise, we have to do one step of golden extrapolation
                    else
                    {
                        a = x; fA = fX;
                        b = c; fB = fB;
                        c = a + INV_TAU_F * (b-a);
                        fC = f->func(c);
                    }
                }
                //  if x <= c
                else if (x > xMin)
                {
                    float   fX = f->func(x);
                    
                    //  we have found a bracket, the search is over
                    if (fX > fC)
                    {
                        *theA = x; *theFA = fX;
                        *theX = c; *theFX = fC;
                        *theB = b; *theFB = fB;
                        return true;
                    }
                    //  otherwise, we have to do one step of golden extrapolation
                    else
                    {
                        a = c; fA = fC;
                        b = x; fB = fX;
                        c = a + INV_TAU_F * (b-a);
                        if (c < xMin)
                            c = xMin;
                        fC = f->func(c);
                    }
                }
                //  x too far on the left
                else
                {
                    a = b; fA = fB;
                    b = c; fB = fC;
                    c = b + INV_TAU_F * (b-a);
                    if (c < xMin)
                        c = xMin;
                    fC = f->func(c);
                }
            }
            //  if d has the wrong sign, we have to do one step of golden extrapolation
            else
            {
                a = b; fA = fB;
                b = c; fB = fC;
                c = b + INV_TAU_F * (b-a);
                if (c < xMin)
                    c = xMin;
                fC = f->func(c);
            }
        }            

        if ((*nbIters) > MAX_NB_ITERS)
            return false;
            
        else
        {
            *theA = c;
            *theFA = fC;
            *theX = b;
            *theFX = fB;
            *theB = a;
            *theFB = fA;
            return true;
        }
    }    
}



bool NonlinearOptimizer1D_Brent::initSearchBracket_(const Function1D* f, double xStart, 
													double xMin, double xMax, double* theA,
													double* theB, double* theX, double* theFA, double* theFB, 
													double* theFX, int* nbIters)
{
    double  a, b, c,
           fA, fB, fC;
    
    a = xStart;
    fA = f->func(xStart);
    //  I make a small step (arbitrarily, 1/100th of the search interval) on the right side
    //  of a to see which way the function is going down.
    if ((xMin != -HUGE_VAL) && (xMax != HUGE_VAL))
        b = xStart + (xMax - xMin)*SMALL_STEP_FRAC;
    else if (xStart == 0)
        b = xStart + SMALL_STEP_FRAC;
    else if (xStart > 0)
        b = xStart * (1.0L + SMALL_STEP_FRAC);
    else
        b = xStart * (1.0L - SMALL_STEP_FRAC);
    
    fB = f->func(b);

    //  if the function is going down on the right side -> positive steps
    //  if the function is going down on the left side -> negative steps
    const bool  LEFT_TO_RIGHT = (fB < fA);
    if (!LEFT_TO_RIGHT)
    {
        //  swap a and b
        c = a; fC = fA;
        a = b; fA = fB;
        b = c; fB = fC;
    }

    *nbIters = 0;    
    c = a + INV_TAU * (b-a);
    fC = f->func(c);  
    
    if (LEFT_TO_RIGHT)
    {
        while ((fB > fC) && ((*nbIters)<=MAX_NB_ITERS))
        {
            (*nbIters)++;
            
            //  perform the parabolic interpolation
            double   bmc = b-c, cpb = c+b,
                    cma = c-a, apc = a+c,
                    amb = a-b, bpa = b+a,
                    bmcfa = bmc*fA, cmafb = cma*fB, ambfc = amb*fC;
            double   d = bmcfa + cmafb + ambfc;
            
            //  We can use the cubic interpolation if the function  goes down L2R and d<0
            //  or the function goes down R2L and d>0
            if (d < 0) 
            {
                double   e = cpb*bmcfa + apc*cmafb + bpa*ambfc;
                double   x = 0.5 * e / d;
                
                //  if x < c, then it can only be between b and c
                if (x < c)
                {
                    double   fX = f->func(x);
                    
                    //  we have found a bracket, the search is over
                    if (fX < fC)
                    {
                        *theA = b; *theFA = fB;
                        *theX = x; *theFX = fX;
                        *theB = c; *theFB = fC;
                        return true;
                    }
                    //  otherwise, we have to do one step of golden extrapolation
                    else
                    {
                        a = x; fA = fX;
                        b = c; fB = fB;
                        c = a + INV_TAU * (b-a);
                        if (c > xMax)
                            c = xMax;
                        fC = f->func(c);
                    }
                }
                //  if x >= c
                else if (x < xMax)
                {
                    double   fX = f->func(x);
                    
                    //  we have found a bracket, the search is over
                    if (fX > fC)
                    {
                        *theA = b; *theFA = fB;
                        *theX = c; *theFX = fC;
                        *theB = x; *theFB = fX;
                        return true;
                    }
                    //  otherwise, we have to do one step of golden extrapolation
                    //  of c-x
                    else
                    {
                        a = c; fA = fC;
                        b = x; fB = fX;
                        c = a + INV_TAU * (b-a);
                        if (c > xMax)
                            c = xMax;
                        fC = f->func(c);
                    }
                }
                //  x is too far on the right
                else
                {
                    a = b; fA = fB;
                    b = c; fB = fC;
                    c = b + INV_TAU * (b-a);
                    if (c > xMax)
                        c = xMax;
                    fC = f->func(c);
                }
               
            }
            //  if d has the wrong sign, we have to do one step of golden extrapolation
            else
            {
                a = b; fA = fB;
                b = c; fB = fC;
                c = b + INV_TAU * (b-a);
                if (c > xMax)
                    c = xMax;
                fC = f->func(c);
            }
        }            

        if ((*nbIters) > MAX_NB_ITERS)
            return false;
            
        else
        {
            *theA = a;
            *theFA = fA;
            *theX = b;
            *theFX = fB;
            *theB = c;
            *theFB = fC;
            return true;
        }
    }
    
    //  if we are going right to left
    else
    {
        while ((fB > fC) && ((*nbIters)<=MAX_NB_ITERS))
        {
            (*nbIters)++;
            
            //  perform the parabolic interpolation
            double   bmc = b-c, cpb = c+b,
                    cma = c-a, apc = a+c,
                    amb = a-b, bpa = b+a,
                    bmcfa = bmc*fA, cmafb = cma*fB, ambfc = amb*fC;
            double   d = bmcfa + cmafb + ambfc;
            
            if (d > 0) 
            {
                double   e = cpb*bmcfa + apc*cmafb + bpa*ambfc;
                double   x = 0.5 * e / d;
                
                //  if x > c, then it can only be between b and c
                if (x > c)
                {
                    double   fX = f->func(x);
                    
                    //  we have found a bracket, the search is over
                    if (fX < fC)
                    {
                        *theA = c; *theFA = fC;
                        *theX = x; *theFX = fX;
                        *theB = b; *theFB = fB;
                        return true;
                    }
                    //  otherwise, we have to do one step of golden extrapolation
                    else
                    {
                        a = x; fA = fX;
                        b = c; fB = fB;
                        c = a + INV_TAU * (b-a);
                        fC = f->func(c);
                    }
                }
                //  if x <= c
                else if (x > xMin)
                {
                    double   fX = f->func(x);
                    
                    //  we have found a bracket, the search is over
                    if (fX > fC)
                    {
                        *theA = x; *theFA = fX;
                        *theX = c; *theFX = fC;
                        *theB = b; *theFB = fB;
                        return true;
                    }
                    //  otherwise, we have to do one step of golden extrapolation
                    else
                    {
                        a = c; fA = fC;
                        b = x; fB = fX;
                        c = a + INV_TAU * (b-a);
                        if (c < xMin)
                            c = xMin;
                        fC = f->func(c);
                    }
                }
                //  x too far on the left
                else
                {
                    a = b; fA = fB;
                    b = c; fB = fC;
                    c = b + INV_TAU * (b-a);
                    if (c < xMin)
                        c = xMin;
                    fC = f->func(c);
                }
            }
            //  if d has the wrong sign, we have to do one step of golden extrapolation
            else
            {
                a = b; fA = fB;
                b = c; fB = fC;
                c = b + INV_TAU * (b-a);
                if (c < xMin)
                    c = xMin;
                fC = f->func(c);
            }
        }            

        if ((*nbIters) > MAX_NB_ITERS)
            return false;
            
        else
        {
            *theA = c;
            *theFA = fC;
            *theX = b;
            *theFX = fB;
            *theB = a;
            *theFB = fA;
            return true;
        }
    }    
}
