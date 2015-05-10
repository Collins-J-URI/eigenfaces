/*  NAME:
        optimization_main.cpp
 
    DESCRIPTION:
        demonstration for the numerical optimization classes
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/


/*--------------------------------------------------------------------------+
|																			|
|	Author:	jean-yves herve', 3D Group for Interactive Visualization,       |
|                             Department of Computer Science and Statistics	|
|							  University of Rhode Island					|	
+--------------------------------------------------------------------------*/

#include <iostream>
#include <cstdlib>
//
#include "SimpleFunction1D.h"
#include "SimpleFunctionND.h"
#include "NotSoSimpleFunctionND.h"
#include "NonlinearOptimizer1D_gss.h"
#include "NonlinearOptimizer1D_parab.h"
#include "NonlinearOptimizerND_conjDir.h"

using namespace std;
using namespace uriVL;



//----------------------------------------------
//	Function prototypes
//----------------------------------------------

//----------------------------------------------
//	File-level global variables
//----------------------------------------------
const float TOL = 1.0E-6;
const float TOL_F = 0.0001f;


//=============================================================================
//	Function Implementations
//=============================================================================



int main(int argc, char** argv)
{
    SimpleFunction1D* f = new SimpleFunction1D();
    
    const int NB_PTS = 15;
    
    float   x[] = {-42.f, -35.f, -16.f, -8.f, -3.f, -2.5f, 0.f, 0.5f, 4.0f, 
					8.5f, 22.f, 33.f, 45.f, 48.f, 55.f};
    float   y;
    
    for (int i=0; i<NB_PTS; i++)
    {
        y = f->func(x[i]);
        cout << "pt " << i << ": <" << x[i] << ", " << y << ">" << endl;
    }
    
    //======================================================================
    //  1-D Optimization: GSS
    //======================================================================
    NonlinearOptimizer1D_gss *opt = new NonlinearOptimizer1D_gss();
    OptimizerRecord1D   *optRec = new OptimizerRecord1D();
    string  resultStr;
    cout << "1D Optimization: Golden section search" << endl;
    opt->minimize(f, -25.f, -30.f, 50.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
    
    opt->minimize(f, -25.f, -30.f, 75.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
        
    opt->minimize(f, 25.f, -30.f, 75.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
        
    opt->minimize(f, -50.f, -70.f, -41.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;        
    
    opt->minimize(f, 0.f, -30.f, 50.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
    
    opt->minimize(f, -10.f, -25.f, 25.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;        
    
    opt->minimize(f, -10.f, -30.f, 50.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
    

    //======================================================================
    //  1-D Optimization: Parabolic
    //======================================================================
    NonlinearOptimizer1D_parab *optP = new NonlinearOptimizer1D_parab();
    
    cout << endl << "1D Optimization: Parabolic interpolation method" << endl;
    optP->minimize(f, -25.f, -30.f, 50.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
    
    optP->minimize(f, -25.f, -30.f, 75.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
        
    optP->minimize(f, 25.f, -30.f, 75.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
        
    optP->minimize(f, -50.f, -70.f, -41.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;        
    
    optP->minimize(f, 0.f, -30.f, 50.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
    
    optP->minimize(f, -10.f, -25.f, 25.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;        
    
    optP->minimize(f, -10.f, -30.f, 50.f, TOL_F, optRec);
    resultStr = optRec->isSuccess ? "minimum found: " : "minimum not found: ";
    cout << resultStr<< "<"  << optRec->optX << ", " << optRec->optVal << "> in " 
         << optRec->nbIters << " steps" << endl;    
    

    //======================================================================
    //  n-D Optimization    
    //======================================================================
    SimpleFunctionND* fn1 = new SimpleFunctionND();

	//	I am going to re-use the same OptimizerRecordND for several consecutive searches
    OptimizerRecordND* optRecN = new OptimizerRecordND(2);

	//	creates an optimizer that performs coordinate search for fn1 (cycles optimization long
	//	the canonical directions)
    NonlinearOptimizerND* optN1 = new NonlinearOptimizerND_conjDir(fn1, NO_UPDATE);
	
    double x0[2], xMax[2], xMin[2];

    cout << endl << endl;
    cout << "n-dimensional optimization" << " (no direction update)" << endl;
	cout << "---------------------------" << endl;
    x0[0] = 0;
    x0[1] = 0;
    xMax[0] = 10;
    xMax[1] = 10;
    xMin[0] = -10;
    xMin[1] = -10;
    optN1->minimize(fn1, x0, xMin, xMax, TOL, optRecN);    
    cout << "xOpt = <" << optRecN->optX[0] << ", " << optRecN->optX[1] << ">" << endl;
    cout << "optVal = " << optRecN->optVal << endl;
    cout << optRecN->nbOuterIters << " outer iterations" << endl;
    cout << optRecN->nbInnerIters << " inner iterations" << endl;
    

	//	creates an optimizer to perform true conjugate directions optimization on fn1,
	//	introducing total-displacement-over-cycle as new search direction. Search
	//	directions are reset to canonical every 4 cycles.
    NonlinearOptimizerND* optN2 = new NonlinearOptimizerND_conjDir(fn1, POWELL_UN_IS_XN_X0, 4, CANONICAL_DIRECTIONS);
	//
    cout << endl << endl;
    cout << "n-dimensional optimization" << " (UN_IS_XN_X0)" << endl;
	cout << "---------------------------" << endl;
    x0[0] = 0;
    x0[1] = 0;
    xMax[0] = 10;
    xMax[1] = 10;
    xMin[0] = -10;
    xMin[1] = -10;
    optN2->minimize(fn1, x0, xMin, xMax, TOL, optRecN);    
    cout << "xOpt = <" << optRecN->optX[0] << ", " << optRecN->optX[1] << ">" << endl;
    cout << "optVal = " << optRecN->optVal << endl;
    cout << optRecN->nbOuterIters << " outer iterations" << endl;
    cout << optRecN->nbInnerIters << " inner iterations" << endl;
    
/*
    NonlinearOptimizerND* optN3 = new NonlinearOptimizerND_conjDir(RESET_AFTER_N_CYCLES);
    cout << endl << endl;
    cout << "n-dimensional optimization"  << " (RESET_AFTER_N_CYCLES)" << endl;
	cout << "---------------------------"<< endl;
    x0[0] = 0;
    x0[1] = 0;
    xMax[0] = 10;
    xMax[1] = 10;
    xMin[0] = -10;
    xMin[1] = -10;
    optN3->minimize(fn1, x0, xMin, xMax, TOL, optRecN);    
    cout << "xOpt = <" << optRecN->optX[0] << ", " << optRecN->optX[1] << ">" << endl;
    cout << "optVal = " << optRecN->optVal << endl;
    cout << optRecN->nbOuterIters << " outer iterations" << endl;
    cout << optRecN->nbInnerIters << " inner iterations" << endl;
    

    NotSoSimpleFunctionND* fn2 = new NotSoSimpleFunctionND();

    cout << endl << endl;
    cout << "n-dimensional optimization"  << " (no direction update)" << endl;
	cout << "---------------------------"<< endl;
    x0[0] = 0;
    x0[1] = 0;
    xMax[0] = 10;
    xMax[1] = 10;
    xMin[0] = -10;
    xMin[1] = -10;
    optN1->minimize(fn2, x0, xMin, xMax, TOL, optRecN);    
    cout << "xOpt = <" << optRecN->optX[0] << ", " << optRecN->optX[1] << ">" << endl;
    cout << "optVal = " << optRecN->optVal << endl;
    cout << optRecN->nbOuterIters << " outer iterations" << endl;
    cout << optRecN->nbInnerIters << " inner iterations" << endl;
    
    cout << endl << endl;
    cout << "n-dimensional optimization" << " (UN_IS_XN_X0)" << endl;
	cout << "---------------------------"<< endl;
    x0[0] = 0;
    x0[1] = 0;
    xMax[0] = 10;
    xMax[1] = 10;
    xMin[0] = -10;
    xMin[1] = -10;
    optN2->minimize(fn2, x0, xMin, xMax, TOL, optRecN);    
    cout << "xOpt = <" << optRecN->optX[0] << ", " << optRecN->optX[1] << ">" << endl;
    cout << "optVal = " << optRecN->optVal << endl;
    cout << optRecN->nbOuterIters << " outer iterations" << endl;
    cout << optRecN->nbInnerIters << " inner iterations" << endl;
    
    cout << endl << endl;
    cout << "n-dimensional optimization" << " (RESET_AFTER_N_CYCLES)" << endl;
	cout << "---------------------------"<< endl;
    x0[0] = 0;
    x0[1] = 0;
    xMax[0] = 10;
    xMax[1] = 10;
    xMin[0] = -10;
    xMin[1] = -10;
    optN3->minimize(fn2, x0, xMin, xMax, TOL, optRecN);    
    cout << "xOpt = <" << optRecN->optX[0] << ", " << optRecN->optX[1] << ">" << endl;
    cout << "optVal = " << optRecN->optVal << endl;
    cout << optRecN->nbOuterIters << " outer iterations" << endl;
    cout << optRecN->nbInnerIters << " inner iterations" << endl;
*/
    return 0;
}
