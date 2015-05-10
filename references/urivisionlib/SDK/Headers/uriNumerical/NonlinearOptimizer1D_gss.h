/*  NAME:
        NonlinearOptimizer1D_gss.h
 
    DESCRIPTION:
        NonlinearOptimizer1D_gss public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_NONLINEAR_OPTIMIZER_1D_GSS_H
#define URIVL_NONLINEAR_OPTIMIZER_1D_GSS_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "OptimizerRecord1D.h"
#include "NonlinearOptimizer1D.h"

namespace uriVL
{
	/** 1-dimensional optimizerimplementing the golden section search.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT NonlinearOptimizer1D_gss : public NonlinearOptimizer1D
    {
        public:

        	/** Creates a new 1D GSS optimizer
        	 */
            NonlinearOptimizer1D_gss(void);

        	/** Creates a new 1D optimizer for a specific function.  This is the version of
			 *	the constructor to invoke if the same function will be used over and over for
			 *	optimization.
			 *	@param	f	the function to optimize
        	 */
            NonlinearOptimizer1D_gss(const Function1D* f);

        	/** Creates a new 1D optimizer for a specific function.  This is the version of
			 *	the constructor to invoke if the same function will be used over and over for
			 *	optimization.
			 *	@param	f	the function to optimize
        	 */
            NonlinearOptimizer1D_gss(const Function1D_F* f);
			
			
            /** Destructor
             */          
            ~NonlinearOptimizer1D_gss(void);


        private:

            /** Finds a GS search bracket about the starting point.
             *  @param  f   the function to optimize
             *  @param  xStart  starting point for the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  a   left endpoint of the search bracket found 
             *  @param  b   right endpoint of the search bracket found 
             *  @param  x1  left internal point of the search bracket found 
             *  @param  x2  right internal point of the search bracket found 
             *  @param  fA  value of the function at a
             *  @param  fB  value of the function at b
             *  @param  fX1 value of the function at x1
             *  @param  fX2 value of the function at x2
             *  @param  nbIters number of iterations steps required to find the bracket
             *  @return <code>true</code> if a search bracket was found
             */
            static bool initSearchBracket_(const Function1D_F* f, float xStart, 
											float xMin, float xMax, float* a,
											float* b, float* x1, float* x2, float* fA, 
											float* fB, float* fX1, float* fX2, int* nbIters);
            
            /** Finds a GS search bracket about the starting point.
             *  @param  f   the function to optimize
             *  @param  xStart  starting point for the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  a   left endpoint of the search bracket found 
             *  @param  b   right endpoint of the search bracket found 
             *  @param  x1  left internal point of the search bracket found 
             *  @param  x2  right internal point of the search bracket found 
             *  @param  fA  value of the function at a
             *  @param  fB  value of the function at b
             *  @param  fX1 value of the function at x1
             *  @param  fX2 value of the function at x2
             *  @param  nbIters number of iterations steps required to find the bracket
             *  @return <code>true</code> if a search bracket was found
             */
            static bool initSearchBracket_(const Function1D* f, double xStart, 
											double xMin, double xMax, double* a,
											double* b, double* x1, double* x2, double* fA, 
											double* fB, double* fX1, double* fX2, int* nbIters);
            
            /** Finds a (local) minimum for a function over a specific range.
             *
             *  Should be implemented by subclasses.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize_(const Function1D_F* f, const float startX, const float xMin, 
                          const float xMax, const float tol, OptimizerRecord1D* optRec);
            
            
            /** Finds a (local) minimum for a function over a specific range.
             *
             *  Should be implemented by subclasses.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize_(const Function1D* f, const double startX, const double xMin, 
                          const double xMax, const double tol, OptimizerRecord1D* optRec);
            
            
    };
}   

#endif  //  NONLINEAR_OPTIMIZER_1D_GSS_H
