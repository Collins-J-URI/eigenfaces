/*  NAME:
        RandomGenerator_R250_R521.h
 
    DESCRIPTION:
        RandomGenerator_R250_R521 public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef	URIVL_RANDOM_GENERATOR_R250_521_H
#define	URIVL_RANDOM_GENERATOR_R250_521_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RandomGenerator.h"


namespace uriVL {

    static const int R250_LEN = 250;
    static const int R521_LEN = 521;

    /** Random number generator class that uses the combined R250-R521 algorithm implemented 
     *  by Michael Brundage  <http://www.qbrundage.com/michaelb/index.html>.  
     *  I have tried as much as possible to preserve the original code and comments.
     *  The main change is the implementation of setSeed.
     */
    class RandomGenerator_R250_R521 : public RandomGenerator
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
    		
    	    /** Constructor
    	     */
    		RandomGenerator_R250_R521(void);
    		
    	    /** Creates a new RandomGenerator object seeded with the value received as parameter
             *  @param theSeed     new seed value for this generator
    	     */
    		RandomGenerator_R250_R521(long theSeed);
    		
    		/** Destructor
    		 */ 
    	    ~RandomGenerator_R250_R521(void);
    		
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
    		static RandomGenerator_R250_R521  *rg_;
    		
            /** index of the R250 algorrithm
             */
            int r250_index_; 

            /** index of the R521 algorrithm
             */
            int r521_index_;
            
            /** buffer of the R250 algorrithm
             */
            unsigned long r250_buffer_[R250_LEN];

            /** buffer of the R521 algorrithm
             */
            unsigned long r521_buffer_[R521_LEN];


    };
}

#endif  //  RANDOM_GENERATOR_R250_521_H
