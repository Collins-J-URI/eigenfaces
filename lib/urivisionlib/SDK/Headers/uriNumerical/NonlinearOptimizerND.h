/*  NAME:
        NonlinearOptimizerND.h
 
    DESCRIPTION:
        NonlinearOptimizerND public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_MOMLINEAR_OPTIMIZER_ND_H
#define URIVL_MOMLINEAR_OPTIMIZER_ND_H

#include <vector>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "OptimizerRecordND.h"
#include "FunctionND.h"
#include "FunctionND_F.h"

namespace uriVL
{
	
	/** Pure virtual parent class for all n-dimensional optimizers.  Subclasses of
	 *	NonlinearOptimizerND will actually implement an optimization algorithm, 
	 *	i.e. Powells conjugate gradients.
	 *
	 *	I have flip-flopped for a while on a couple of questions regarding optimizers:
	 *	<ul>
	 *		<li>Inheritance:  Is there any point at all to using inheritance for these
	 *			classes, and wouldn't it be simpler to simple propose a flat list of
	 *			implementations with no common parent class?  All the more so since most of
	 *			the functions in these classes could very well be static.  
	 *			I opted for a class hierarchy because this allows me in the future to define
	 *			a "processing pipeline" with a generic nd optimizer, and then pick the
	 *			particular optimizer subclass to use w/o having to redefine the pipeline.</li>
	 *		<li>Should an optimizer store a reference to the nd function of the optimization
	 *			problem, since all my minimize_ functions take the nd function asone of their
	 *			parameters?  The answer is yes, because setting the function (and dimension)
	 *			allows to preallocate matrices and setup 1D "line" optimizers (if needed).
	 *			Now, if the user wants to change the nd funtion each time they solve a 
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
    class URIVL_EXPORT NonlinearOptimizerND
    {
        public:

        	/** Creates a new nD optimizer
        	 */
            NonlinearOptimizerND(void);

        	/** Creates a new n-dimensional nD optimizer
			 *	@param n	dimension of the optimization problem
        	 */
            NonlinearOptimizerND(int n);

        	/** Creates a new n-dimensional nD optimizer for a specific function
			 *	@param f	the function of the optimization problem
        	 */
            NonlinearOptimizerND(const FunctionND* f);

        	/** Creates a new n-dimensional nD optimizer for a specific function
			 *	@param f	the function of the optimization problem
        	 */
            NonlinearOptimizerND(const FunctionND_F* f);

            /** Destructor
             */          
            virtual ~NonlinearOptimizerND(void);

        	/** Assigns a new nD function to this optimizer.  Note that an optimizer cannot
			 *	have a double <i>and</i> float function assigned to it.  The last function 
			 *	assigned simply replaces the previous one, regardless of type.
			 *	@param f	the function of the optimization problem
        	 */
            void setFunction(const FunctionND* f);

        	/** Assigns a new nD function to this optimizer.  Note that an optimizer cannot
			 *	have a double <i>and</i> float function assigned to it.  The last function 
			 *	assigned simply replaces the previous one, regardless of type.
			 *	@param f	the function of the optimization problem
        	 */
            void setFunction(const FunctionND_F* f);


			/**	Checks if the function passed as parameter is the one set for this optimization 
			 *	problem.  Why this function:  Because the library (still) admits double and
			 *	float versions of the Function1D class, we can't have a getFunction function,
			 *	and I don't like the idea of using protected variables much.
			 *	@return true if f is the function of this optimizaton problem
			 */
			bool isOptimizationFunction(const FunctionND* f);
			
			/**	Checks if the function passed as parameter is the one set for this optimization 
			 *	problem.  Why this function:  Because the library (still) admits double and
			 *	float versions of the Function1D class, we can't have a getFunction function,
			 *	and I don't like the idea of using protected variables much.
			 *	@return true if f is the function of this optimizaton problem
			 */
			bool isOptimizationFunction(const FunctionND_F* f);
			
			/**	Gives the dimension of this optimizer (if one has been set)
			 *	@return		dimension of this optimizer
			 */
			int getDimension(void) const;
			

            /** Finds a (local) minimum for a function over a specific domain of R^n.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    n-sized array of lower bounds of the search domain
             *  @param  xMax    n-sized array of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @return result of the minimization in an OptimizerRecordND
             */
            const OptimizerRecordND* minimize(const FunctionND* f, const double* startX, 
											  const double* xMin, const double* xMax, 
											  double tol);
            

            /** Finds a (local) minimum for a function over a specific domain of R^n.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    n-sized array of lower bounds of the search domain
             *  @param  xMax    n-sized array of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @return result of the minimization in an OptimizerRecordND
             */
            const OptimizerRecordND* minimize(const FunctionND_F* f, const float* startX, 
											  const float* xMin, const float* xMax, 
											  float tol);
            
            /** Finds a (local) minimum for a function over a specific domain of R^n.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    n-sized vector of lower bounds of the search domain
             *  @param  xMax    n-sized vector of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @return result of the minimization in an OptimizerRecordND
             */
            const OptimizerRecordND* minimize(const FunctionND* f, const std::vector<double>& startX, 
											  const std::vector<double>& xMin, 
											  const std::vector<double>& xMax, double tol);
            
            /** Finds a (local) minimum for a function over a specific domain of R^n.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    n-sized vector of lower bounds of the search domain
             *  @param  xMax    n-sized vector of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @return result of the minimization in an OptimizerRecordND
             */
            const OptimizerRecordND* minimize(const FunctionND_F* f, const std::vector<float>& startX, 
											  const std::vector<float>& xMin, 
											  const std::vector<float>& xMax, float tol);
            
            
            /** Finds a (local) minimum for a function over a specific range.
             *
             *  @param  f   the function to optimize
             *  @param  startX  startting point of the search
             *  @param  xMin    n-sized array of lower bounds of the search domain
             *  @param  xMax    n-sized array of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize(const FunctionND* f, const double* startX, const double* xMin, 
                          const double* xMax, double tol, OptimizerRecordND* optRec);
 

            /** Finds a (local) minimum for a function over a specific range.
             *
             *  @param  f   the function to optimize
             *  @param  startX  startting point of the search
             *  @param  xMin    n-sized array of lower bounds of the search domain
             *  @param  xMax    n-sized array of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize(const FunctionND_F* f, const float* startX, const float* xMin, 
                          const float* xMax, float tol, OptimizerRecordND* optRec);
 

            /** Finds a (local) minimum for a function over a specific range.
             *
             *  @param  f   the function to optimize
             *  @param  startX  startting point of the search
             *  @param  xMin    n-sized array of lower bounds of the search domain
             *  @param  xMax    n-sized array of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize(const FunctionND* f, const std::vector<double>& startX, 
						  const std::vector<double>& xMin, const std::vector<double>& xMax, 
						  double tol, OptimizerRecordND* optRec);

            /** Finds a (local) minimum for a function over a specific range.
             *
             *  @param  f   the function to optimize
             *  @param  startX  startting point of the search
             *  @param  xMin    n-sized array of lower bounds of the search domain
             *  @param  xMax    n-sized array of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize(const FunctionND_F* f, const std::vector<float>& startX, 
						  const std::vector<float>& xMin, const std::vector<float>& xMax, 
						  float tol, OptimizerRecordND* optRec);
 

 

        private:
		
			/**	Stores the dimension of the minimization problem (if it has been defined)
			 */
			int nDim_;
			
			/**	Stores a pointer to the nD function of the minimization problem (if it has been defined)
			 */
			const FunctionND* fND_;

			/**	Stores a pointer to the nD function of the minimization problem (if it has been defined)
			 */
			const FunctionND_F* fNF_;

			
			/** Disabled copy constructor
        	 */
            NonlinearOptimizerND(const NonlinearOptimizerND& obj);

        	/** Disabled copy operator
        	 */
            const NonlinearOptimizerND& operator =(const NonlinearOptimizerND& obj);

        	/** Called by this class's public function <i>prior</i> to changing the value of
			 *	the instance variable.  Any subclass that needs special allocation
			 *	or preprocessing when a new function is set should override this function.
			 *	@param f	the new function of the optimization problem
        	 */
            virtual void setFunction_(const FunctionND* f)
			{
				//	nothing.  Override if needed
			}

        	/** Called by this class's public function <i>prior</i> to changing the value of
			 *	the instance variable.  Any subclass that needs special allocation
			 *	or preprocessing when a new function is set should override this function.
			 *	@param f	the new function of the optimization problem
        	 */
            virtual void setFunction_(const FunctionND_F* f)
			{
				//	nothing.  Override if needed
			}


			/** Finds a (local) minimum for a double function over a specific range.
             *
             *  Should be implemented by subclasses.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    n-sized array of lower bounds of the search domain
             *  @param  xMax    n-sized array of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            virtual void minimize_(const FunctionND* f, const double* startX, 
								   const double* xMin, const double* xMax, 
								   double tol, OptimizerRecordND* optRec) = 0;


            /** Finds a (local) minimum for a float function over a specific range.
             *
             *  Should be implemented by subclasses.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    n-sized array of lower bounds of the search domain
             *  @param  xMax    n-sized array of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            virtual void minimize_(const FunctionND_F* f, const float* startX, 
								   const float* xMin, const float* xMax, 
								   float tol, OptimizerRecordND* optRec) = 0;
 

			/** Finds a (local) minimum for a double function over a specific range.
             *
             *  Should be implemented by subclasses.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    n-sized vector of lower bounds of the search domain
             *  @param  xMax    n-sized vector of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            virtual void minimize_(const FunctionND* f, 
								   const std::vector<double>& startX, 
								   const std::vector<double>& xMin,
								   const std::vector<double>& xMax, double tol, 
								   OptimizerRecordND* optRec) = 0;
 
            /** Finds a (local) minimum for a float function over a specific range.
             *
             *  Should be implemented by subclasses.
             *
             *  @param  f   the function to optimize
             *  @param  startX  starting point of the search
             *  @param  xMin    n-sized vector of lower bounds of the search domain
             *  @param  xMax    n-sized vector of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            virtual void minimize_(const FunctionND_F* f, 
								   const std::vector<float>& startX, 
								   const std::vector<float>& xMin,
								   const std::vector<float>& xMax, 
								   float tol, 
								   OptimizerRecordND* optRec) = 0;
 

			 
    };	
}   

#endif  //  MOMLINEAR_OPTIMIZER_ND_H
