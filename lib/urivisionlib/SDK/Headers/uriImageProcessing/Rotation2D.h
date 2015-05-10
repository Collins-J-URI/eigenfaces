/*  NAME:
        Rotation2D.h
 
    DESCRIPTION:
        Rotation2D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ROTATION_2D_H
#define URIVL_ROTATION_2D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RigidTransformation2D.h"

namespace uriVL
{

    /** Class that implements 2D rotation transformations.
     * 
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT Rotation2D : public RigidTransformation2D
    {
        public:

            /**
             *  Constructor.
             *  Creates an identity rotation.
             */
            Rotation2D(void);

            /** Copy constructor.
             *  Creates a 2D rotation identical to the one received as parameter
             *  @param  theRot      2D rotation to clone
             */
            Rotation2D(Rotation2D *theRot);

            /** Destructor
             */          
            ~Rotation2D(void);

            
    };
}   

#endif  //  ROTATION_2D_H
