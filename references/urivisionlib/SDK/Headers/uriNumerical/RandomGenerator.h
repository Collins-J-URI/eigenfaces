/*  NAME:
        RandomGenerator.h
 
    DESCRIPTION:
        RandomGenerator public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef	URIVL_RANDOM_GENERATOR_H
#define	URIVL_RANDOM_GENERATOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif


namespace uriVL {

    /** Pure virtual parent class for pseudorandom number generators.  The API for this 
     *  class is largely inspired from that of the <code>Random</code> class of Java.
     *  <p>This class class was initially created just as a way to simply create random
     *  matrices and vectors to test the classes of uriVL.  Then I decided to 
     *  implement several different algorithms (basically just embedding into my classes 
     *  public domain or other open-source code posted online by people who know alo more
     *  about random generators.</p>
     *  <p>If you are really concerned about performance of the random generator, then you
     *  should probably use directly the original code rather than the wrapper classes
     *  of this hierarchy.</p>
     *  @author     jean-yves herve', 3D Group for Interactive Visualization,
     *                                Dept. of Computer Science and Statistics,
     *                                University of Rhode Island.
     */
    class RandomGenerator
    {
    	public:
    	    
    		/** Destructor
    		 */ 
    		virtual ~RandomGenerator(void);
    		
    		/** Returns a new uniformly-distributed unsigned int pseudo-random number in the
    		 *  full range of the data type.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new unsigned int pseudo-random value
    		 */
    		virtual unsigned int nextUInt(void);

    		/** Returns a new uniformly-distributed int pseudo-random number in the range specified.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new int pseudo-random value in the range [lowVal, highVal]
    		 */
    		virtual int	nextInt(int lowVal, int highVal);

    		/** Returns a new uniformly-distributed unsigned long pseudo-random number in the
    		 *  full range of the data type.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new unsigned long pseudo-random value
    		 */
    		virtual unsigned long nextULong(void) = 0;

    		/** Returns a new uniformly-distributed long int pseudo-random number in the range specified.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new int pseudo-random value in the range [lowVal, highVal]
    		 */
    		virtual long nextLong(long lowVal, long highVal);

    		/** Returns a new uniformly-distributed float pseudo-random number in the range [0, 1[.
    		 *  @return new float pseudo-random value in the range [0, 1[
    		 */
    		virtual float nextFloat(void);

    		/** Returns a new uniformly-distributed float pseudo-random number in the range specified.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (exclusive) of the generated value range
    		 *  @return new float pseudo-random value in thhe range [lowVal, highVal[
    		 */
    		virtual float nextFloat(float lowVal, float highVal);

    		/** Returns a new uniformly-distributed double pseudo-random number in the range [0, 1[.
    		 *  @return new double pseudo-random value in the range [0, 1[
    		 */
    		virtual double nextDouble(void) = 0;

    		/** Returns a new uniformly distributed double pseudo-random number in the range specified.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (exclusive) of the generated value range
    		 *  @return new double pseudo-random value in the range [lowVal, highVal[
    		 */
    		virtual double nextDouble(double lowVal, double highVal) = 0;

    		/** Returns a new double pseudo-random number in the range [0, 1[ "normally" distributed
    		 *  (mean 0.0 and standard deviation sigma).
    		 *  @param  sigma   standard deviation of the distribution
    		 *  @return new normally-distributed double pseudo-random value in the range [0, 1[
    		 */
    		virtual double nextGaussian(double sigma=1.L);

            /** Sets a new seed for this generator
             *  @param theSeed     new theSeed value for this generator
             */
       		virtual void setSeed(long theSeed) = 0;
       		

        protected:
        
            /** Indicates whether an int has the same length as a long (should be true non most
             *  platforms
             */
            static const bool INT_IS_LONG;
            
    	    /** Creates a new RandomGenerator object
    	     */
    		RandomGenerator(void);
    		

    };
}

#endif  //  RANDOM_GENERATOR_H
