/*  NAME:
        RandomGenerator_Knuth.h
 
    DESCRIPTION:
        RandomGenerator_Knuth public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef	URIVL_RANDOM_GENERATOR_KNUTH_H
#define	URIVL_RANDOM_GENERATOR_KNUTH_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RandomGenerator.h"


namespace uriVL {

    /** Random number generator class that implements Knuth's algorithm (by way of the
     *  Numerical Recipes in C).  
     *  
     */
    class RandomGenerator_Knuth : public RandomGenerator
    {
    	public:
    	    
    	    /** Creates a new RandomGenerator object and uses the current time stamp to
             *  generate a seed.
    	     */
    		RandomGenerator_Knuth(void);
    		
    	    /** Creates a new RandomGenerator object seeded with the value received as parameter
             *  @param theSeed     new seed value for this generator
    	     */
    		RandomGenerator_Knuth(long theSeed);
    		
    		/** Destructor
    		 */ 
    	    ~RandomGenerator_Knuth(void);
    		
    		/** Returns a new uniformly-distributed unsigned long pseudo-random number in the
    		 *  full range of the data type.
    		 *  @param  lowVal      lower bound of the generated value range
    		 *  @param  highVal     upper bound (inclusive) of the generated value range
    		 *  @return new unsigned long pseudo-random value
    		 */
    		unsigned long nextULong(void);

    		/** Returns a new uniformly-distributed double pseudo-random number in the range [0, 1[.
    		 *  @return new double pseudo-random value in the range [0, 1[
    		 */
    		double nextDouble(void);

            /** Sets a new seed for this generator
             *  @param theSeed     new seed value for this generator
             */
       		void setSeed(long theSeed);
       		
       		       		
    		
    	private:
    	    /** seed for this generator
    	     */
    		int		seed_;
    		
    		double	y_, maxran_, v_[98];

    };
}

#endif  //  RANDOM_GENERATOR_KNUTH_H

