/*  NAME:
        ConfidenceRecord.h
 
    DESCRIPTION:
        ConfidenceRecord public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef		URIVL_CONFIDENCE_RECORD_H
#define		URIVL_CONFIDENCE_RECORD_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

namespace uriVL
{
	class URIVL_EXPORT  ConfidenceRecord 
	{
		public:
			
			/**	Constructor
			 */
			ConfidenceRecord(void);

			/**	Destructor
			 */
			virtual ~ConfidenceRecord(void);

			
		protected:



	};
}
#endif	//	CONFIDENCE_RECORD_H
