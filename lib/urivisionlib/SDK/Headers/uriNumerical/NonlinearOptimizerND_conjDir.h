/*  NAME:
        NonlinearOptimizerND_conjDir.h
 
    DESCRIPTION:
        NonlinearOptimizerND_conjDir public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_MOMLINEAR_OPTIMIZER_ND_CONJ_DIR_H
#define URIVL_MOMLINEAR_OPTIMIZER_ND_CONJ_DIR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "OptimizerRecordND.h"
#include "NonlinearOptimizerND.h"
#include "FunctionNDSection1D.h"
#include "FunctionNDSection1D_F.h"
#include "NonlinearOptimizer1D.h"

namespace uriVL
{
    /** Codes for the various conjugate direction updating schemes supported
     */
    typedef enum DirectionUpdateMethod
    {
		/** implements coordinate descent
		 */
        NO_UPDATE = 0,

        /** Powell alg., new direction is displacement over a full cycle
		 */ 
		POWELL_UN_IS_XN_X0,

        /** Powell, alg., discard direction of largest decrease
		 */
		POWELL_DISCARD_LARGEST_DECREASE_DIR
		
    } DirectionUpdateMethod;


    /** Codes for the different ways to initialize/reset the search directions
     */
    typedef enum DirectionResetMode
    {
		/** canonical (coordinate) directions
		 */
        CANONICAL_DIRECTIONS = 0,	

		/** generate mutually orthogonal unit vectors
		 */
        RANDOM_ORTHOGONAL_DIRECTIONS,

		/** Uses directions set by the user.  If no directions were set, then canonical
		 *	directions will be used instead
		 */
        USER_SPECIFIED_DIRECTIONS
		
    } DirectionResetMode;
    

	/** Class that implements the conjugate directions line search optomization algorithm.
	 *	The class implements different algorithms for choosing and updating the search
	 *	directions.  At some point in the future, the library may add support for sme sort of
	 *	dynamic addition of a new update scheme, but for the time being the only way to
	 *	add an update scheme is to add it statically in this class.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT NonlinearOptimizerND_conjDir : public NonlinearOptimizerND
    {
        public:

        	/** Creates a new nD optimizer and specifies the dimension
			 *	@param	n				dimension of the optimizer
			 *	@param	updateMethod	scheme for updating the search directions
			 *	@param	reset			search directions are reset after how many n-direction cycles (n by default)
			 *	@param	resetMethod		how directions are reset
        	 */
            NonlinearOptimizerND_conjDir(int n, DirectionUpdateMethod updateMethod, 
										 int reset = -1, 
										 DirectionResetMode resetMethod = CANONICAL_DIRECTIONS);

        	/** Creates a new nD optimizer and specifies the function to optimize
			 *	@param	fn				n-dimensional function of the optimization problem
			 *	@param	updateMethod	scheme for updating the search directions
			 *	@param	reset			search directions are reset after how many n-direction cycles (n by default)
			 *	@param	resetMethod		how directions are reset
        	 */
            NonlinearOptimizerND_conjDir(const FunctionND* fn, DirectionUpdateMethod updateMethod, 
										 int reset = -1, 
										 DirectionResetMode resetMethod = CANONICAL_DIRECTIONS);

        	/** Creates a new nD optimizer and specifies the function to optimize
			 *	@param	fn				n-dimensional function of the optimization problem
			 *	@param	updateMethod	scheme for updating the search directions
			 *	@param	reset			search directions are reset after how many n-direction cycles (n by default)
			 *	@param	resetMethod		how directions are reset
        	 */
            NonlinearOptimizerND_conjDir(const FunctionND_F* fn, DirectionUpdateMethod updateMethod, 
										 int reset = -1, 
										 DirectionResetMode resetMethod = CANONICAL_DIRECTIONS);


            /** Destructor
             */          
            virtual ~NonlinearOptimizerND_conjDir(void);



        private:
		
			/**	Method used to update the search directions
			 */
			DirectionUpdateMethod updateMethod_;
			
			/**	Reset directions after how many n-direction cycles
			 */
			int reset_;
			
			/**	Method for reseting/initializing the directions
			 */
			DirectionResetMode resetMethod_;
			
			/**	Initial/reset directions defined by the user (if any)
			 */
			double** userDirections_;
			
            /** The 1D optimizer used by this nD optimizer to perform line minimization
             */
            NonlinearOptimizer1D* lineMinimizer_;
            
            /** The 1D optimizer record returned by the 1D optimizer.
             */
            OptimizerRecord1D* lineRecord_;
            
            /** 1D section of the function to minimize (float version)
             */
            FunctionNDSection1D* ffSec1D_;
            
            /** 1D section of the function to minimize (double version)
             */
            FunctionNDSection1D_F* fdSec1D_;
            
            /** Set of conjugate directions for the optimization problem.
             */
            double** u_;
            
            /** Origin of the search at the beginning of a cycle
             */
            double* P0_;
            
            /** Current origin of directional search
             */
            double* Pi_;
            
            /** Index of conjugate direction that produced the largest decrease of the function
             */
            int bestDirIndex_;
            

            /** Countes how many n-direction cycles the optimizer has gone through
             */
            int cycleCounter_;
			            

			/**	Disabled copy constructor
			 */
			NonlinearOptimizerND_conjDir(const NonlinearOptimizerND_conjDir& obj);
			
			/**	Disabled copy operator
			 */
			const NonlinearOptimizerND_conjDir& operator =(const NonlinearOptimizerND_conjDir& obj);
			
			
			
        	/** Called by this class's public function <i>prior</i> to changing the value of
			 *	the instance variable.  Any subclass that needs special allocation
			 *	or preprocessing when a new function is set should override this function.
			 *	@param f	the new function of the optimization problem
        	 */
            void setFunction_(const FunctionND* f);

        	/** Called by this class's public function <i>prior</i> to changing the value of
			 *	the instance variable.  Any subclass that needs special allocation
			 *	or preprocessing when a new function is set should override this function.
			 *	@param f	the new function of the optimization problem
        	 */
            void setFunction_(const FunctionND_F* f);


            /** Finds a (local) minimum for a function over a specific range.
             *
             *  @param  f   the function to optimize
             *  @param  startX  startting point of the search
             *  @param  xMin    n-sized array of lower bounds of the search domain
             *  @param  xMax    n-sized array of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
            void minimize_(const FunctionND* f,const double* startX, const double* xMin, 
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
            void minimize_(const FunctionND_F* f, const float* startX, const float* xMin, 
                          const float* xMax, float tol, OptimizerRecordND* optRec);
 
            /** Finds a (local) minimum for a function over a specific range.
             *
             *  @param  f   the function to optimize
             *  @param  startX  startting point of the search
             *  @param  xMin    n-sized vector of lower bounds of the search domain
             *  @param  xMax    n-sized vector of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
			void minimize_(const FunctionND* f, const std::vector<double>& startX, 
						   const std::vector<double>& xMin, 
                           const std::vector<double>& xMax, double tol, 
						   OptimizerRecordND* optRec);
           
            /** Finds a (local) minimum for a function over a specific range.
             *
             *  @param  f   the function to optimize
             *  @param  startX  startting point of the search
             *  @param  xMin    n-sized vector of lower bounds of the search domain
             *  @param  xMax    n-sized vector of upper bounds of the search domain
             *  @param  tol     tolerance of the search
             *  @param  optRec  result of the optimization
             */
			void minimize_(const FunctionND_F* f, const std::vector<float>& startX, 
						   const std::vector<float>& xMin, 
                           const std::vector<float>& xMax, float tol, 
						   OptimizerRecordND* optRec);
           

            /** Updates the set of conjugate directions of this optimizer.
             */
            void updateConjugateDirections_(void);
            
            /** (re)allocates internal arrays following a change of function
             *  @param  n   dimension of the new optimization problem
             */
            void allocateInstanceArrays_(const int n);


            /** free internal arrays, called from destructor or following a change of function
             *  @param  n   current dimension of the optimization problem
             */
            void deleteInstanceArrays_(const int n);

		//	parent class is the only friend
		friend class NonlinearOptimizerND;
		
		
    };
}   

#endif  //  MOMLINEAR_OPTIMIZER_ND_CONJ_DIR_H
