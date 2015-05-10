/*  NAME:
        RandomGenerator_C.h
 
    DESCRIPTION:
        RandomGenerator_C public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef	URIVL_RANDOM_GENERATOR_C_H
#define	URIVL_RANDOM_GENERATOR_C_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RandomGenerator.h"


namespace uriVL {

    /** Pseudorandom number generator wrapper class for the awful standard C function.
     *  <p>Needless to say, nobody should be using that one, I just coded it for debugging
     *  purposes (and to have a lower bound, quality-wise, to compare the other
     *  generators against).</p>
     *  <p>There can only be one object of this class, therefore the class stores that object
     *  as a priveate member and only allows static calls to the generation functions.</p>
     *  <p>Note that if you want to use the seed as a way to get repeatable pseudorandom
     *  sequences, it is important that you don't call directly the rand or srand functions.
     *  Make all calls through this class. </p>
     *  @author     jean-yves herve', 3D Group for Interactive Visualization,
     *                                Dept. of Computer Science and Statistics,
     *                                University of Rhode Island.
     */
    class RandomGenerator_C : public RandomGenerator
    {
    	public:
    	    
    	    /** number of full outputs of the C generator fitting in a long
    	     */
    		static const int FULL_IN_ULONG;
    		
    	    /** number of residual bits of a output of the C generator to dd to make a long
    	     */
    		static const int RES_TO_ULONG;
    		
    	    /** number of full outputs of the C generator fitting in an int
    	     */
    		static const int FULL_IN_UINT;
    		
    	    /** number of residual bits of a output of the C generator to dd to make an int
    	     */
    		static const int RES_TO_UINT;
    		
    		/** Destructor
    		 */ 
    	    ~RandomGenerator_C(void);
    		
    		/** Returns a new uniformly-distributed unsigned long pseudo-random number in the
    		 *  full range of the data type.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new unsigned long pseudo-random value
    		 */
    		unsigned long nextULong(void);

    		/** Returns a new uniformly distributed double pseudo-random number in the range specified.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (exclusive) of the generated value range
    		 *  @return new double pseudo-random value in the range [lowVal, highVal[
    		 */
    		double nextDouble(double lowVal, double highVal);

    		/** Returns a new uniformly-distributed double pseudo-random number in the range [0, 1[.
    		 *  @return new double pseudo-random value in the range [0, 1[
    		 */
    		double nextDouble(void);

            /** Sets a new seed for this generator
             *  @param seed     new seed value for this generator
             */
       		void setSeed(long theSeed);
       		
    		/** Returns a new uniformly-distributed unsigned int pseudo-random number in the
    		 *  full range of the data type.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new unsigned int pseudo-random value
    		 */
    		static unsigned int nextUInt_st(void);

    		/** Returns a new uniformly-distributed int pseudo-random number in the range specified.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new int pseudo-random value in the range [lowVal, highVal]
    		 */
    		static int	nextInt_st(int lowVal, int highVal);

    		/** Returns a new uniformly-distributed unsigned long pseudo-random number in the
    		 *  full range of the data type.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new unsigned long pseudo-random value
    		 */
    		static unsigned long nextULong_st(void);

    		/** Returns a new uniformly-distributed long int pseudo-random number in the range specified.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new int pseudo-random value in the range [lowVal, highVal]
    		 */
    		static long nextLong_st(long lowVal, long highVal);

    		/** Returns a new uniformly-distributed float pseudo-random number in the range [0, 1[.
    		 *  @return new float pseudo-random value in the range [0, 1[
    		 */
    		static float nextFloat_st(void);

    		/** Returns a new uniformly-distributed float pseudo-random number in the range specified.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (exclusive) of the generated value range
    		 *  @return new float pseudo-random value in thhe range [lowVal, highVal[
    		 */
    		static float nextFloat_st(float lowVal, float highVal);

    		/** Returns a new uniformly-distributed double pseudo-random number in the range [0, 1[.
    		 *  @return new double pseudo-random value in the range [0, 1[
    		 */
    		static double nextDouble_st(void);

    		/** Returns a new uniformly distributed double pseudo-random number in the range specified.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (exclusive) of the generated value range
    		 *  @return new double pseudo-random value in the range [lowVal, highVal[
    		 */
    		static double nextDouble_st(double lowVal, double highVal);

    		/** Returns a new double pseudo-random number in the range [0, 1[ "normally" distributed
    		 *  (mean 0.0 and standard deviation sigma).
    		 *  @param  sigma   standard deviation of the distribution
    		 *  @return new normally-distributed double pseudo-random value in the range [0, 1[
    		 */
    		static double nextGaussian_st(double sigma=1.L);


            /** Sets a new seed for this generator
             *  @param seed     new seed value for this generator
             */
       		static void setSeed_st(long theSeed);
       		
       		       		
    		
    	private:

    		/** The unique object of this class, stored as a static member
    		 */
    		static RandomGenerator_C  *rg_;
    		
            /** the generator's seed
             */
            unsigned int seed_;

    	    /** Creates a new RandomGenerator object and uses the current time stamp to
             *  generate a seed.
    	     */
    		RandomGenerator_C(void);
    		
    };
}

#endif  //  RANDOM_GENERATOR_C_H
