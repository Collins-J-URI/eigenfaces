/*  NAME:
        NonlinearOptimizer1D_gss.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib NonlinearOptimizer1D_gss class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <cmath>
//
#include "NonlinearOptimizer1D_gss.h"

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

const int MAX_NB_ITERS = 1000;


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark constructors and destructor
//-------------------------------------------------------------------
#endif

NonlinearOptimizer1D_gss::NonlinearOptimizer1D_gss(void)
			:   NonlinearOptimizer1D()
{

}

NonlinearOptimizer1D_gss::NonlinearOptimizer1D_gss(const Function1D* f)
		try	:   NonlinearOptimizer1D(f)
{

}
catch(ErrorReport& e)
{
	e.appendToMessage("called by NonlinearOptimizer1D_gss constructor");
	throw e;
}	

NonlinearOptimizer1D_gss::NonlinearOptimizer1D_gss(const Function1D_F* f)
		try	:   NonlinearOptimizer1D(f)
{

}
catch(ErrorReport& e)
{
	e.appendToMessage("called by NonlinearOptimizer1D_gss constructor");
	throw e;
}	

NonlinearOptimizer1D_gss::~NonlinearOptimizer1D_gss(void)
{
}


#if 0
//-------------------------------------------------------------------
#pragma mark -
#pragma mark minimization private functions
//-------------------------------------------------------------------
#endif

void NonlinearOptimizer1D_gss::minimize_(const Function1D_F* f, float xStart, float xMin,
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
    float   a, b, x1, x2,
            fA, fB, fX1, fX2,
            tolX;
    int     nbIters = 0;
/*
const int NB_TEST_PTS = 100;
float	testX[NB_TEST_PTS], testFunc[NB_TEST_PTS];
if ((xMin != -HUGE_VALF) && (xMax !=HUGE_VALF))
{
    cout << "{";
    for (int k=0; k<NB_TEST_PTS; k++)
    {
        testX[k] = xMin + k*(xMax-xMin) / 99.f;
        testFunc[k] = f->func(testX[k]);
        cout << "{" << testX[k] << "," << testFunc[k] << "}";
        if (k <NB_TEST_PTS-1)
            cout << ",";
    }	
    cout << "}" << endl;
}             
*/
    bool foundBracket = initSearchBracket_(f, xStart, funcXMin, funcXMax, &a, &b, &x1, &x2, 
                                            &fA, &fB, &fX1, &fX2, &nbIters);

    if (foundBracket)
    {
        tolX = 0.5f*tol*(fabs(x1) + fabs(x2));
        
        //  if the search bracket we get has a > b (this will happens if the function's slope is
        //  positive at the starting point we got), then we must invert the interval
        if (a > b)
        {
            float  t = a,
                   fT = fA;
            a = b;
            fA = fB;
            b = t;
            fB = fT;
            t = x1;
            fT = fX1;
            x1 = x2;
            fX1 = fX2;
            x2 = t;
            fX2 = fT;
        }
        
        while ((fabs(b-a) > tolX) && (nbIters<=MAX_NB_ITERS))
        {
            if (fX1 > fX2)
            {
                a = x1;
                fA = fX1;
                x1 = x2;
                fX1 = fX2;
                x2 = a + TAU_F*(b-a);
                fX2 = f->func(x2);
            }
            else
            {
                b = x2;
                fB = fX2;
                x2 = x1;
                fX2 = fX1;
                x1 = a + TAU1_F*(b-a);
                fX1 = f->func(x1);
            }
            
            tolX = 0.5f*tol*(fabsf(x1) + fabsf(x2));
            nbIters++;
        }
        
        if (fX1 > fX2)
        {
            optRec->optX = x2;
            optRec->optVal = fX2;
        }
        else    
        {
            optRec->optX = x1;
            optRec->optVal = fX1;
        }
        optRec->isSuccess = (nbIters<=MAX_NB_ITERS);
        optRec->nbIters = nbIters;
    }
    
    //  if we did not find a bracket, then we must report our abject failure
    else
    {
        //  still return the "best" among a, b, x1, x2
        double  x, fX;
        if (fA < fB) {
            x = a; fX = fA;
        }
        else {
            x = b; fX = fB;
        }
        if (fX1 < fX) {
            x = x1; fX = fX1;
        }
        if (fX2 < fX) {
            x = x2; fX = fX2;
        }
        optRec->optX = x;
        optRec->optVal = fX;        
        optRec->isSuccess = false;
        optRec->nbIters = nbIters;
    }
}


void NonlinearOptimizer1D_gss::minimize_(const Function1D *f, double xStart, double xMin,
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
    double  a, b, x1, x2,
            fA, fB, fX1, fX2,
            tolX;
    int     nbIters = 0;
             
    bool foundBracket = initSearchBracket_(f, xStart, funcXMin, funcXMax, &a, &b, &x1, &x2, &fA, 
                                            &fB, &fX1, &fX2, &nbIters);

    if (foundBracket)
    {
        tolX = 0.5f*tol*(fabs(x1) + fabs(x2));

        //  if the search bracket we get has a > b (this will happens if the function's slope is
        //  positive at the starting point we got), then we must invert the interval
        if (a > b)
        {
            double  t = a,
                    fT = fA;
            a = b;
            fA = fB;
            b = t;
            fB = fT;
            t = x1;
            fT = fX1;
            x1 = x2;
            fX1 = fX2;
            x2 = t;
            fX2 = fT;
        }
         
        while ((fabs(b-a) > tolX) && (nbIters<=MAX_NB_ITERS))
        {
            if (fX1 > fX2)
            {
                a = x1;
                fA = fX1;
                x1 = x2;
                fX1 = fX2;
                x2 = a + TAU*(b-a);
                fX2 = f->func(x2);
            }
            else
            {
                b = x2;
                fB = fX2;
                x2 = x1;
                fX2 = fX1;
                x1 = a + TAU1*(b-a);
                fX1 = f->func(x1);
            }
            
            tolX = 0.5*tol*(fabs(x1) + fabs(x2));
            nbIters++;
        }
        
        if (fX1 > fX2)
        {
            optRec->optX = x2;
            optRec->optVal = fX2;
        }
        else    
        {
            optRec->optX = x1;
            optRec->optVal = fX1;
        }
        optRec->isSuccess = (nbIters<=MAX_NB_ITERS);
        optRec->nbIters = nbIters;
    }
    
    //  if we did not find a bracket, then we must report our abject failure
    else
    {
        //  still return the "best" among a, b, x1, x2
        double  x, fX;
        if (fA < fB) {
            x = a; fX = fA;
        }
        else {
            x = b; fX = fB;
        }
        if (fX1 < fX) {
            x = x1; fX = fX1;
        }
        if (fX2 < fX) {
            x = x2; fX = fX2;
        }
        optRec->optX = x;
        optRec->optVal = fX;        
        optRec->isSuccess = false;
        optRec->nbIters = nbIters;
    }
}


bool NonlinearOptimizer1D_gss::initSearchBracket_(const Function1D_F* f, float xStart, 
                                    float xMin, float xMax, float* theA, 
                                    float* theB, float* theX1, float* theX2,
                                    float* theFA, float* theFB, float* theFX1, 
                                    float* theFX2, int* nbIters)
{
    float	a, b, c, lastC,
			fA, fB, fC, lastFC;
    
    a = xStart;
    fA = f->func(xStart);
    //  I make a small step (arbitrarily, 1/10th to 1/100th of the search interval) on the 
    //  right side of a to see which way the function is going down.
    if ((xMin != -HUGE_VALF) && (xMax != HUGE_VALF))
        c = xStart + (xMax - xStart)*SMALL_STEP_FRAC_F;
    else if (xStart == 0)
        c = xStart + SMALL_STEP_FRAC_F;
    else if (xStart > 0)
        c = xStart * (1.0f + SMALL_STEP_FRAC_F);
    else
        c = xStart * (1.0f - SMALL_STEP_FRAC_F);
    fC = f->func(c);

    //  if the function is going down on the right side -> positive steps
    //  if the function is going down on the left side -> negative steps
    const bool  LEFT_TO_RIGHT = (fC < fA);
    if (!LEFT_TO_RIGHT)
    {
        //  swap a and c
        b = a; fB = fA;
        a = c; fA = fC;
        c = b; fC = fB;
    }
    
    float tempSmallStep, tempBoundC;
    if ((xMin != -HUGE_VALF) && (xMax != HUGE_VALF))
    {
        if (LEFT_TO_RIGHT)
        {
            tempSmallStep = (xMax - xStart)*SMALL_STEP_FRAC_F;
            tempBoundC = a + TAU_F*(xMax - a);
        }
        else
        {
            tempSmallStep = (xMin - xStart)*SMALL_STEP_FRAC_F;
            tempBoundC = a + TAU_F * (xMin - a);
        }
    }
    else if (xStart != 0)
    {
        if (LEFT_TO_RIGHT)
        {
            tempSmallStep = fabsf(SMALL_STEP_FRAC_F * xStart);
            tempBoundC = HUGE_VALF;
        }
        else
        {
            tempSmallStep = -fabsf(SMALL_STEP_FRAC_F * xStart);
            tempBoundC = -HUGE_VALF;
        }
    }
    else
    {
        if (LEFT_TO_RIGHT)
        {
            tempSmallStep = SMALL_STEP_FRAC_F;
            tempBoundC = HUGE_VALF;
        }
        else
        {
            tempSmallStep = -SMALL_STEP_FRAC_F;
            tempBoundC = -HUGE_VALF;
        }
    }
    
    const float smallStep = tempSmallStep;
    const float boundC = tempBoundC;
    float step = smallStep;

    *nbIters = 0;    
    b = a + INV_TAU_F *(c-a);
    fB = f->func(b);  
    
    while ((fC > fB) && ((*nbIters)<=MAX_NB_ITERS))
    {
        lastC = c;
        lastFC = fC;
        
        c = c + step;
        (*nbIters)++;
        
        //  if c is beyond the bound, we have not found a bracket.  It is possible/likely
        //  that the function is strictly decreasing over the search interval.  So we 
        //  compute one more value at the end of the search interval
        if (LEFT_TO_RIGHT && (c>boundC))
        {
        	*theA = a; *theFA = fA;
        	*theX1 = c; *theFX1 = fC;
        	*theX2 = b; *theFX2 = fB;
        	*theB = xMax - SMALL_STEP_FRAC_F * (xMax-xMin); *theFB = f->func(*theB);
            return false;
        }
        else if (!LEFT_TO_RIGHT && (c<boundC))
        {
        	*theA = a; *theFA = fA;
        	*theX1 = c; *theFX1 = fC;
        	*theX2 = b; *theFX2 = fB;
        	*theB = xMin + SMALL_STEP_FRAC_F * (xMax-xMin); *theFB = f->func(*theB);
            return false;
        }
            
        fC = f->func(c);
        
        //  if f(c) is still ower than f(a) then we compute the corresponding b
        //  for the next round of evaluation
        if (fC < fA)
        {
            b = a + INV_TAU_F *(c-a);
            fB = f->func(b);
            step *= STEP_GAIN_F;
        }
        //  if fC > fA, we went too far, but it's good because  now we know that
        //  there is a bracket [a, lastC, c].  We do a sort of GSS until we have our 
        //  bracket.  This is not wasted time since this is going to reduce the 
        //  time of the actual minimum search by giving us a smaller bracket.
        else
        {
            //  we compute the golden section points for the segment a-c
            float x1, x2, 
                   fX1 = HUGE_VALF,
                   fX2 = HUGE_VALF;
            
            //  if either one of these two points is below f(a), then we have our bracket
            //  otherwise, we need to repeat the operation
            while (((*nbIters)<=MAX_NB_ITERS) && (fX1 > fA) && (fX2 > fA))
            {
                x1 = a + TAU1_F*(c-a);
                x2 = a + TAU_F*(c-a);
                fX1 = f->func(x1);
                fX2 = f->func(x2);
                (*nbIters)++;

                //  if lastC is between a and x1, repeat the operation between 
                //  a and x1
                if (lastC < x1)
                {
                    c = x1;
                    fC = fX1;
                }
                //  if lastC is between x1 and x2
                else if (lastC < x2)
                {
                    a = x1;
                    fA = fX1;
                    c = x2;
                    fC = fX2;
                }
                else
                {
                    a = x2;
                    fA = fX2;
                }
            }
            
            if ((*nbIters) > MAX_NB_ITERS)
                return false;
                
            else
            {
                *theA = a;
                *theFA = fA;
                *theX1 = x1;
                *theFX1 = fX1;
                *theX2 = x2;
                *theFX2 = fX2;
                *theB = c;
                *theFB = fC;
                return true;
            }
            
        }
    }
    
    *theA = a;
    *theFA = fA;
    *theX1 = a + TAU1_F*(b-a);
    *theFX1 = f->func(*theX1);
    *theX2 = c;
    *theFX2 = fC;
    *theB = b;
    *theFB = fB;
    
    return (*nbIters) <= MAX_NB_ITERS;

}



bool NonlinearOptimizer1D_gss::initSearchBracket_(const Function1D* f, double xStart, 
                                    double xMin, double xMax, double* theA, 
                                    double* theB, double* theX1, double* theX2,
                                    double* theFA, double* theFB, double* theFX1, 
                                    double* theFX2, int* nbIters)
{
    double   a, b, c, lastC,
             fA, fB, fC, lastFC;
    
    a = xStart;
    fA = f->func(xStart);
    //  I make a small step (arbitrarily, 1/100th of the search interval) on the right side
    //  of a to see which way the function is going down.
    if ((xMin != -HUGE_VALF) && (xMax != HUGE_VALF))
        c = xStart + (xMax - xMin)*SMALL_STEP_FRAC;
    else if (xStart == 0)
        c = xStart + SMALL_STEP_FRAC;
    else if (xStart > 0)
        c = xStart * (1.0f + SMALL_STEP_FRAC);
    else
        c = xStart * (1.0f - SMALL_STEP_FRAC);
    
    fC = f->func(c);

    //  if the function is going down on the right side -> positive steps
    //  if the function is going down on the left side -> negative steps
    const bool  LEFT_TO_RIGHT = (fC < fA);
    if (!LEFT_TO_RIGHT)
    {
        //  swap a and c
        b = a; fB = fA;
        a = c; fA = fC;
        c = b; fC = fB;
    }
    
    double   tempSmallStep, tempBoundC;
    if ((xMin != -HUGE_VALF) && (xMax != HUGE_VALF))
    {
        if (LEFT_TO_RIGHT)
        {
            tempSmallStep = (xMax - xMin)*SMALL_STEP_FRAC;
            tempBoundC = a + TAU*(xMax - a);
        }
        else
        {
            tempSmallStep = (xMin - xMax)*SMALL_STEP_FRAC;
            tempBoundC = a + TAU * (xMin - a);
        }
    }
    else if (xStart != 0)
    {
        if (LEFT_TO_RIGHT)
        {
            tempSmallStep = fabs(SMALL_STEP_FRAC * xStart);
            tempBoundC = HUGE_VALF;
        }
        else
        {
            tempSmallStep = -fabs(SMALL_STEP_FRAC * xStart);
            tempBoundC = -HUGE_VALF;
        }
    }
    else
    {
        if (LEFT_TO_RIGHT)
        {
            tempSmallStep = SMALL_STEP_FRAC;
            tempBoundC = HUGE_VALF;
        }
        else
        {
            tempSmallStep = -SMALL_STEP_FRAC;
            tempBoundC = -HUGE_VALF;
        }
    }
    
    const double smallStep = tempSmallStep;
    const double boundC = tempBoundC;
    double  step = smallStep;

    *nbIters = 0;    
    b = a + INV_TAU *(c-a);
    fB = f->func(b);  
    
    while ((fC > fB) && ((*nbIters)<=MAX_NB_ITERS))
    {
        lastC = c;
        lastFC = fC;
        
        c = c + step;
        (*nbIters)++;
        
        //  if c is beyond the bound, we have not found a bracket.  It is possible/likely
        //  that the function is strictly decreasing over the search interval.  So we 
        //  compute one more value at the end of the search interval
        if (LEFT_TO_RIGHT && (c>boundC))
        {
        	*theA = a; *theFA = fA;
        	*theX1 = c; *theFX1 = fC;
        	*theX2 = b; *theFX2 = fB;
        	*theB = xMax - SMALL_STEP_FRAC * (xMax-xMin); *theFB = f->func(*theB);
            return false;
        }
        else if (LEFT_TO_RIGHT && (c>boundC))
        {
        	*theA = a; *theFA = fA;
        	*theX1 = c; *theFX1 = fC;
        	*theX2 = b; *theFX2 = fB;
        	*theB = xMin + SMALL_STEP_FRAC * (xMax-xMin); *theFB = f->func(*theB);
            return false;
        }
            
        fC = f->func(c);
        
        //  if f(c) is still ower than f(a) then we compute the corresponding b
        //  for the next round of evaluation
        if (fC < fA)
        {
            b = a + INV_TAU *(c-a);
            fB = f->func(b);
            step *= STEP_GAIN;
        }
        //  if fC > fA, we went too far, but it's good because  now we know that
        //  there is a bracket [a, lastC, c].  We do a sort of GSS until we have our 
        //  bracket.  This is not wasted time since this is going to reduce the 
        //  time of the actual minimum search by giving us a smaller bracket.
        else
        {
            //  we compute the golden section points for the segment a-c
            double  x1, x2, 
                    fX1 = HUGE_VALF,
                    fX2 = HUGE_VALF;
            
            //  if either one of these two points is below f(a), then we have our bracket
            //  otherwise, we need to repeat the operation
            while (((*nbIters)<=MAX_NB_ITERS) && (fX1 > fA) && (fX2 > fA))
            {
                x1 = a + TAU1*(c-a);
                x2 = a + TAU*(c-a);
                fX1 = f->func(x1);
                fX2 = f->func(x2);
                (*nbIters)++;

                //  if lastC is between a and x1, repeat the operation between 
                //  a and x1
                if (lastC < x1)
                {
                    c = x1;
                    fC = fX1;
                }
                //  if lastC is between x1 and x2
                else if (lastC < x2)
                {
                    a = x1;
                    fA = fX1;
                    c = x2;
                    fC = fX2;
                }
                else
                {
                    a = x2;
                    fA = fX2;
                }
            }
            
            if ((*nbIters) > MAX_NB_ITERS)
                return false;
                
            else
            {
                *theA = a;
                *theFA = fA;
                *theX1 = x1;
                *theFX1 = fX1;
                *theX2 = x2;
                *theFX2 = fX2;
                *theB = c;
                *theFB = fC;
                return true;
            }
            
        }
    }
    
    if ((*nbIters) > MAX_NB_ITERS)
        return false;
        
    else
    {
        *theA = a;
        *theFA = fA;
        *theX1 = a + TAU1*(b-a);
        *theFX1 = f->func(*theX1);
        *theX2 = c;
        *theFX2 = fC;
        *theB = b;
        *theFB = fB;
        return true;
    }
}
