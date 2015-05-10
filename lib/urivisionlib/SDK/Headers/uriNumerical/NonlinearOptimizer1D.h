/*  NAME:
        NonlinearOptimizer1D.h
 
    DESCRIPTION:
        NonlinearOptimizer1D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_NONLINEAR_OPTIMIZER_1D_H
#define URIVL_NONLINEAR_OPTIMIZER_1D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "OptimizerRecord1D.h"
#include "Function1D.h"
#include "Function1D_F.h"

namespace uriVL
{
	/** Pure virtual parent class for all 1-dimensional optimizers  Its subclasses
	 *  will actually implement an optimization algorithm, i.e. golden section search.
	 *
	 *	I have flip-flopped for a while on a couple of questions regarding optimizers:
	 *	<ul>
	 *		<li>Inheritance:  Is there any point at all to using inheritance for these
	 *			classes, and wouldn't it be simpler to simple propose a flat list of
	 *			implementations with no common parent class?  All the more so since most of
	 *			the functions in these classes could very well be static.  
	 *			I opted for a class hierarchy because this allows me in the future to define
	 *			a "processing pipeline" with a generic 1d optimizer, and then pick the
	 *			particular optimizer subclass to use w/o having to redefine the pipeline.</li>
	 *		<li>Should an optimizer store a reference to the 1d function of the optimization
	 *			problem, since all my minimize_ functions take the 1d function asone of their
	 *			parameters?  The answer is yes, because setting the function allows to 
	 *			preallocate whatever arrays/structures may be needed by the specific algorithm.
	 *			Now, if the user wants to change the 1d funtion each time they solve a 
	 *			minimization problem, this is their problem.</li>
	 *		<li>What about a set of "maximize" functions?  No time, and not convinced that
	 *			it's worth it.  Since the user has to define & implement the function to
	 *			optimize, nothing prevents implenting -f  or bigNumber - f and run through
	 *			minimization to get the maximum of +f.</li>
	 *		</ul>
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT NonlinearOptimizer1D
    {
        public:

        	/** Creates a new 1D optimizer
        	 */
            NonlinearOptimizer1D(void);

        	/** Creates a new 1D optimizer for a specific function.  This is the version of
			 *	the constructor to invoke if the same function will be used over and over for
			 *	optimization.
			 *	@param	f	the function to optimize
        	 */
            NonlinearOptimizer1D(const Function1D* f);

        	/** Creates a new 1D optimizer for a specific function.  This is the version of
			 *	the constructor to invoke if the same function will be used over and over for
			 *	optimization.
			 *	@param	f	the function to optimize
        	 */
            NonlinearOptimizer1D(const Function1D_F* f);


            /** Destructor
             */          
            virtual ~NonlinearOptimizer1D(void);


        	/** Assigns a new double 1D function to this optimizer.  Note that an optimizer cannot
			 *	have a double <i>and</i> float function assigned to it.  The last function 
			 *	assigned simply replaces the previous one, regardless of type.
			 *	@param f	the function of the optimization problem
        	 */
            void setFunction(const Function1D* f);

        	/** Assigns a new float 1D function to this optimizer.  Note that an optimizer cannot
			 *	have a double <i>and</i> float function assigned to it.  The last function 
			 *	assigned simply replaces the previous one, regardless of type.
			 *	@param f	the function of the optimization problem
        	 */
            void setFunction(const Function1D_F* f);

			/**	Checks if the function passed as parameter is the one set for this optimization 
			 *	problem.  Why this function:  Because the library (still) admits double and
			 *	float versions of the Function1D class, we can't have a getFunction function,
			 *	and I don't like the idea of using protected variables much.
			 *	@return true if f is the function of this optimizaton problem
			 */
			bool isOptimizationFunction(const Function1D* f);
			
			/**	Checks if the function passed as parameter is the one set for this optimization 
			 *	problem.  Why this function:  Because the library (still) admits double and
			 *	float versions of the Function1D class, we can't have a getFunction function,
			 *	and I don't like the idea of using protected variables much.
			 *	@return true if f is the function of this optimizaton problem
			 */
			bool isOptimizationFunction(const Function1D_F* f);
			

            /** Finds and returns a (local) minimum for a double function over a specific 
             *  range.
             *  Note that the rule of thumb for the (relative) tolerance is that typically 
             *  it should not be smaller than sqrt(machine's floating point prevision),
             *  that is, 1.e-04 for float, and 3.e-08 for double (so sez Numerical Recipes)
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  tol     tolerance of the search
             *  @return   result of the optimization
             */
            const OptimizerRecord1D* minimize(const Function1D* f, double startX, double xMin, 
											  double xMax, double tol);
            
            /** Finds and returns a (local) minimum for a float function over a specific range.
             *  Note that the rule of thumb for the (relative) tolerance is that typically 
             *  it should not be smaller than sqrt(machine's floating point prevision),
             *  that is, 1.e-04 for float, and 3.e-08 for double (so sez Numerical Recipes)
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  tol     tolerance of the search
             *  @return   result of the optimization
             */
            const OptimizerRecord1D* minimize(const Function1D_F* f, float startX, float xMin, 
											  float xMax, float tol);
            
            /** Finds a (local) minimum for a double function over a specific range.
             *  Note that the rule of thumb for the (relative) tolerance is that typically 
             *  it should not be smaller than sqrt(machine's floating point prevision),
             *  that is, 1.e-04 for float, and 3.e-08 for double (so sez Numerical Recipes)
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize(const Function1D* f, double startX, double xMin, 
                          double xMax, double tol, OptimizerRecord1D* optRec);
            
            /** Finds a (local) minimum for a float function over a specific range.
             *  Note that the rule of thumb for the (relative) tolerance is that typically 
             *  it should not be smaller than sqrt(machine's floating point prevision),
             *  that is, 1.e-04 for float, and 3.e-08 for double (so sez Numerical Recipes)
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    lower bound of the search interval
             *  @param  xMax    upper bound of the search interval
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize(const Function1D_F* f, float startX, 
						  float xMin, float xMax, 
						  float tol, OptimizerRecord1D* optRec);
            
        private:

			/**	Stores a pointer to the nD function of the minimization problem (if it has been defined)
			 */
			const Function1D* fD_;

			/**	Stores a pointer to the nD function of the minimization problem (if it has been defined)
			 */
			const Function1D_F* fF_;


        	/** Disabled copy constructor
        	 */
            NonlinearOptimizer1D(const NonlinearOptimizer1D& obj);

        	/** Disabled copy operator
        	 */
            const NonlinearOptimizer1D& operator =(const NonlinearOptimizer1D& obj);


        	/** Called by this class's public function <i>prior</i> to changing the value of
			 *	the instance variable.  Any subclass that needs special allocation
			 *	or preprocessing when a new function is set should override this function.
			 *	@param f	the new function of the optimization problem
        	 */
            virtual void setFunction_(const Function1D* f)
			{
				//	nothing.  Override if needed
			}

        	/** Called by this class's public function <i>prior</i> to changing the value of
			 *	the instance variable.  Any subclass that needs special allocation
			 *	or preprocessing when a new function is set should override this function.
			 *	@param f	the new function of the optimization problem
        	 */
            virtual void setFunction_(const Function1D_F* f)
			{
				//	nothing.  Override if needed
			}


            /** Finds a (local) minimum for a double function over a specific range.
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
            virtual void minimize_(const Function1D* f, double startX, 
								   double xMin, double xMax, 
								   double tol, OptimizerRecord1D* optRec) = 0;
            
            /** Finds a (local) minimum for a double function over a specific range.
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
            virtual void minimize_(const Function1D_F* f, float startX, 
								   float xMin, float xMax, 
								   float tol, OptimizerRecord1D* optRec) = 0;
            
            
    };
}   

#endif  //  NONLINEAR_OPTIMIZER_1D_H
