/*  NAME:
        IterationStatus.h
 
    DESCRIPTION:
        IterationStatus public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef		URIVL_ITERATION_STATUS_H
#define		URIVL_ITERATION_STATUS_H

#include <string>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif


namespace uriVL
{
	/**	Criteria for deciding that an iteration has converged
	 */
	typedef enum ConvergenceCriterion {
										SUM_OF_ABSOLUTE_DISPLCT = 0,
										SUM_OF_SQUARED_DISPLCT,
										AVERAGE_DISPLCT,
										MAXIMUM_DISPLCT
	} ConvergenceCriterion;
	
	/**	Class that stores the status of an ongoing iteration and/or the results 
	 *	of a finished iteration
	 */
	class URIVL_EXPORT  IterationStatus 
	{
		public:
			
			/**	Constructor
			 */
			IterationStatus(void);

			/**	Destructor
			 */
			~IterationStatus(void);

			/**	Switches the convergence indicator to true (there is no way to switch
		     *	back to false)
			 *
			 *	@see	hasConverged_
			 */
			void setConverged(void);
			
			/**	Indicates whether the iteration has converged.
			 *
			 *	@see	hasConverged_
			 *	@see	setConverged
			 */
			bool hasConverged(void) const;
			
			
			/**	Allows the user to store in this object various measurements and
			 *	estimates (up to <code>MEASUREMENT_VECTOR_DIM</code>) about the
			 *	ongoing iteration (application-specific).
			 *
			 *	@param	meas	the array of measurement data
			 *
			 *	@see	getMeasurements
			 *	@see	measurements_
			 *	@see	MEASUREMENT_VECTOR_DIM
			 */
			void setMeasurements(const float* meas);

			/**	writes the measurements stored in this object into the array 
			 *  received as parameter.
			 *
			 *	@param	meas	the array in which to write measurement data
			 *
			 *	@see	setMeasurements
			 *	@see	measurements_
			 *	@see	MEASUREMENT_VECTOR_DIM
			 */
			void getMeasurements(float* meas) const;

			/**	returns the measurements stored in this object in a read-only array.
			 *
			 *	@return	an array of measurement data
			 *
			 *	@see	setMeasurements
			 *	@see	measurements_
			 *	@see	MEASUREMENT_VECTOR_DIM
			 */
			const float* getMeasurements(void) const;

			/**	returns a writable array of the he measurements stored in this object
			 *
			 *	@return	an array of measurement data
			 *
			 *	@see	setMeasurements
			 *	@see	measurements_
			 *	@see	MEASUREMENT_VECTOR_DIM
			 */
			float* getMeasurements(ReadWriteStatus rw);
			
			/**	Sets the message string for this object
			 *	@param	message		new message for this object
			 */
			void setMessage(const std::string& message);
			
			/**	Sets the message string for this object
			 *	@param	message		new message for this object
			 */
			const std::string& getMessage(void) const;

			/**	Dimension of the array used to store error and displacement
			 *	estimates.  The user can use this array to store various measurements
			 *	regarding the ongoing iteration
			 *
			 *	@see	setMeasurements
			 */
			static const int MEASUREMENT_VECTOR_DIM = 4;
			

		private:

			/**	Indicates whether the iteration has already converged 
			 *	(this is naturally application-specific)
			 *
			 *	@see	hasConverged
			 *	@see	setConverged
			 */
			bool hasConverged_;
			
			/**	Number of iterations done so far
			 */
			int	nbIterations_;
			
			/**	stores (application-specific) measurements
			 *
			 *	@see 	setMeasurements
			 *	@see	getMeasurements
			 *	@see 	DIMENSION_ERROR_VECTOR
			 */
			float measurements_[MEASUREMENT_VECTOR_DIM];
			
			/**	User-set string that provides info about the iteration (mostly
			 *	for display purposes)
			 */
			std::string message_;
	};
}
#endif
