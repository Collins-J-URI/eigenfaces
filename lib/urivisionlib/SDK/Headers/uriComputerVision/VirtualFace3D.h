/*  NAME:
        VirtualFace3D.h
 
    DESCRIPTION:
        VirtualFace3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VIRTUAL_FACE_3D_H
#define URIVL_VIRTUAL_FACE_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Entity3D.h"
#include "VirtualPoint3D.h"


namespace uriVL
{
    class VirtualObject3D;
    

	/** Implements the VirtualFace3D class.
	 * 
	 *  A VirtualFace3D object stores reference frame of 3D object 
	 *
     *  @author Fang Han, 3D Group, 
     *                    Department of Computer Science and Statistics,
     *                    University of Rhode Island
	 */
	class URIVL_EXPORT VirtualFace3D : public Entity3D
	{
		public:

            /** Contructor.
             *
             *  @param  obj the VirtualObject3D this is a part of
             */
			VirtualFace3D();

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			virtual ~VirtualFace3D(void);
			
            /**
             *  Adds a new vertex to this face
             *	@param	vertIndex	index (in the part's master vector) of the vertex to add
             */
			void addVertex(int vertIndex);            
            
           
        protected:
       
        	std::vector<int> vertexIndex_;
        	
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
        	
        friend class VirtualPart3D;
        friend class VirtualObject3D;
                    
        	
	};
	
}
#endif  //  URIVL_VIRTUAL_FACE_3D_H
