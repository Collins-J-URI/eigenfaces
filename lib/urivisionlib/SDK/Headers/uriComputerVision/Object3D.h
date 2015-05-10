/*  NAME:
        Object3D.h
 
    DESCRIPTION:
        Object3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_OBJECT_3D_H
#define URIVL_OBJECT_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Entity3D.h"

namespace uriVL
{
	/** The base virtual class for 3D objects.
	 * 
	 *  A 3D object is characterized by its pose (position <b>and</b> orientation)
	 *  relative to a given external reference frame and contains its own
	 *  internal reference frame as well.  This means that 3D entities suchs as
	 *  points and segments are not 3D objects (no complete internal reference frame).
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
	class URIVL_EXPORT Object3D : public Entity3D
	{

		public:

            /** Default contructor.
             *
             *  This constructor simply sets to 0 or NULL the basic instance variables
             */
			Object3D(void);

            /** Default contructor.
             *
             *  This constructor simply sets to 0 or NULL the basic instance variables
             */
			Object3D(ReferenceFrame3D* theFrame);

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			virtual ~Object3D(void);


        protected:
        
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


            /** Internal reference frame of this object.
             */
            ReferenceFrame3D  *internalFrame_;
        
            
	}
}
#endif	//	OBJECT_3D_H
