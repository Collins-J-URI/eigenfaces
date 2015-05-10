/*  NAME:
        OptimizerRecordND.h
 
    DESCRIPTION:
        OptimizerRecordND public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_OPTIMIZER_RECORD_ND_H
#define URIVL_OPTIMIZER_RECORD_ND_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

namespace uriVL
{
	/** A simple structure to hold the results of a nD optimization process.  All variables
	 *  are public.  
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT OptimizerRecordND
    {
        public:

			/** Default constructor
			 */
			OptimizerRecordND(void);
			
        	/** Creates a new nD optimizer record
			 *	@param	n	dimension of the search space
        	 */
            OptimizerRecordND(const int n);

        	/** Creates a new nD optimizer record (copy constructor)
			 *	@param	optRec	pointer to the object to copy
        	 */
            OptimizerRecordND(const OptimizerRecordND* optRec);

        	/** Creates a new nD optimizer record (copy constructor)
			 *	@param	optRec	the object to copy
        	 */
            OptimizerRecordND(const OptimizerRecordND& optRec);

            /** Destructor
             */          
            ~OptimizerRecordND(void);

			
        	/** Copy operator
			 *	@param	optRec	the object to copy
        	 */
            const OptimizerRecordND operator =(const OptimizerRecordND& optRec);


			/**	Set dimension of the record
			 */
			void setDimension(const int n);


            /** Dimension of the optimization space
             */
            int nDim;
            
            /** Location of the optimum found (n-sized array)
             */
            double* optX;
            
            /** Value at the optimum found
             */
            double optVal;
            
            /** Number of outer loop iterations required
             */
            int nbOuterIters;
            
            /** Number of inner loop iterations required
             */
            int nbInnerIters;
            
            /** Indicates whether the search was successfull
             */
            bool isSuccess;
			
			
		private:
		
			
            
    };
}   

#endif  //  OPTIMIZER_RECORD_ND_H
