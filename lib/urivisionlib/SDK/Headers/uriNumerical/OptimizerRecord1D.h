/*  NAME:
        OptimizerRecord1D.h
 
    DESCRIPTION:
        OptimizerRecord1D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_OPTIMIZER_RECORD_1D_H
#define URIVL_OPTIMIZER_RECORD_1D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

namespace uriVL
{
	/** A simple structure to hold the results of a 1D optimization process.  All variables
	 *  are public.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT OptimizerRecord1D
    {
        public:

        	/** Creates a new 1D optimizer record
        	 */
            OptimizerRecord1D(void);

            /** Destructor
             */          
            virtual ~OptimizerRecord1D(void);

            /** Location of the optimum found
             */
            double optX;
            
            /** Value at the optimum found
             */
            double optVal;
            
            /** Number of iterations required
             */
            int nbIters;
            
            /** Indicates whether the search was successfull
             */
            bool isSuccess;
            
    };
}   

#endif  //  OPTIMIZER_RECORD_1D_H
