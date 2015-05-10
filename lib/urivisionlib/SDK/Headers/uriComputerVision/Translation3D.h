/*  NAME:
        Translation3D.h
 
    DESCRIPTION:
        Translation3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_TRANSLATION_3D_H
#define URIVL_TRANSLATION_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

namespace uriVL
{
	/** A Translation object is always stored as an array of three double numbers for the
	 *  X, Y, and Z translation components.
	 * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT Translation3D
    {
        public:

        	/**
        	 *	Constructor.
        	 *	Creates a zero translation.
        	 */
            Translation3D(void);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D transformation based on the transformation matrix received
        	 *  as parameter
        	 *	@param	theMat	a flat, GL-style transformation matrix
        	 */
            Translation3D(const double* theTransl);

        	/**
        	 *	Clone donstructor.
        	 *	Creates a 3D translation identical to the one received as parameter
        	 *	@param	theTransl	translation to clone
        	 */
            Translation3D(const Translation3D *theTransl);

            /**
             *	Destructor
             */          
            ~Translation3D(void);
             
            /**  Returns the translation's coordinates, in read-only form
             *  @return     translation's coordinates as double[3]
             */
            const double* getVector(void) const;

            /**  Returns the translation's coordinates, in writable form
             *  @return     translation coordinates as double[3]
             */
            double* getVector(ReadWriteStatus rwa);

        protected:

            /** Stores the translation components
             */
            double   tXYZ_[3];

//           /**
//             *  Copy Constructor. Disabled
//             *
//             *  @param      theObj   reference to the object to copy
//             */
//            DepthMap(const DepthMap& theObj);
//
//
//            /**
//             *  Copy operator. Disabled
//             *
//             *  @param      theObj   reference to the object to copy
//             */
//            const DepthMap& operator = (const DepthMap& theObj);
    };
}   

#endif  //  TRANSLATION_3D_H
