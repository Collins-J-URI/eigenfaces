/*  NAME:
        Translation2D.h
 
    DESCRIPTION:
        Translation2D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_TRANSLATION_2D_H
#define URIVL_TRANSLATION_2D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RigidTransformation2D.h"

namespace uriVL
{

    /** Virtual base class for all 2D transformation that can be represented by
     *  a 4 by 4 homogenous matrix.
     * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT Translation2D : public RigidTransformation2D
    {
        public:

            /**
             *  Constructor.
             *  Creates a zero translation.
             */
            Translation2D(void);

            /**
             *  Constructor.
             *  Creates a 2D transformation based on the translation vector received
             *  as parameter
             *  @param  theTransl   a 2x1 coordinate vector
             */
            Translation2D(const float theTransl[]);

            /**
             *  Constructor.
			 *	@param	tx	x translation component
			 *	@param	ty	x translation component
			 */
			Translation2D(float tx, float ty);
                        

           /**
             *  Copy donstructor.
             *  Creates a 2D translation identical to the one received as parameter
             *  @param  theTransl   translation to clone
             */
            Translation2D(const Translation2D *theTransl);

            /**
             *  Destructor
             */          
            ~Translation2D(void);

            


    };
}   

#endif  //  TRANSLATION_2D_H
