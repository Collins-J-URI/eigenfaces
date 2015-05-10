/*  NAME:
        NonlinearOptimizer1D_Brent.h
 
    DESCRIPTION:
        NonlinearOptimizer1D_Brent public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_NONLINEAR_OPTIMIZER_1D_BRENT_H
#define URIVL_NONLINEAR_OPTIMIZER_1D_BRENT_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "OptimizerRecord1D.h"
#include "NonlinearOptimizer1D.h"

namespace uriVL
{
	/** 1-dimensional optimizer that implements the Brent optimization algorithm
	 *  (combining parabolic and golden section searches).
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT NonlinearOptimizer1D_Brent : public NonlinearOptimizer1D
    {
        public:

        	/** Creates a new 1D GSS optimizer
        	 */
            NonlinearOptimizer1D_Brent(void);

        	/** Creates a new 1D optimizer for a specific function.  This is the version of
			 *	the constructor to invoke if the same function will be used over and over for
			 *	optimization.
			 *	@param	f	the function to optimize
        	 */
            NonlinearOptimizer1D_Brent(const Function1D* f);

        	/** Creates a new 1D optimizer for a specific function.  This is the version of
			 *	the constructor to invoke if the same function will be used over and over for
			 *	optimization.
			 *	@param	f	the function to optimize
        	 */
            NonlinearOptimizer1D_Brent(const Function1D_F* f);
			
            /** Destructor
             */          
            virtual ~NonlinearOptimizer1D_Brent(void);


        private:

            /** Finds a (local) minimum for a function over a specific range.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize_(const Function1D_F* f, float startX, float xMin, 
                           float xMax, float tol, OptimizerRecord1D* optRec);
            
            
            /** Finds a (local) minimum for a function over a specific range.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize_(const Function1D* f, double startX, double xMin, 
                           double xMax, double tol, OptimizerRecord1D* optRec);
            
            
            /** Finds a GS search bracket about the starting point.
             *  @param  f   the function to optimize
             *  @param  xStart  starting point for the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  a   left endpoint of the search bracket found 
             *  @param  b   right endpoint of the search bracket found 
             *  @param  x   internal point of the search bracket found 
             *  @param  fA  value of the function at a
             *  @param  fB  value of the function at b
             *  @param  fX value of the function at x
             *  @param  nbIters number of iterations steps required to find the bracket
             *  @return <code>true</code> if a search bracket was found
             */
            static bool initSearchBracket_(const Function1D_F* f, float xStart, 
											float xMin, float xMax, float* theA, 
											float* theB, float* theX, float* theFA, float* theFB, 
											float* theFX, int* nbIters);
            
            /** Finds a GS search bracket about the starting point.
             *  @param  f   the function to optimize
             *  @param  xStart  starting point for the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  a   left endpoint of the search bracket found 
             *  @param  b   right endpoint of the search bracket found 
             *  @param  x   internal point of the search bracket found 
             *  @param  fA  value of the function at a
             *  @param  fB  value of the function at b
             *  @param  fX value of the function at x
             *  @param  nbIters number of iterations steps required to find the bracket
             *  @return <code>true</code> if a search bracket was found
             */
            static bool initSearchBracket_(const Function1D* f, double xStart, 
											double xMin, double xMax, double* a,
											double* b, double* x, double* fA, double* fB, 
											double* fX, int* nbIters);
            
    };
}   

#endif  //  NONLINEAR_OPTIMIZER_1D_BRENT_H
