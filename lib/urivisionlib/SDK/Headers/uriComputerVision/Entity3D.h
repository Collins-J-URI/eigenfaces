/*  NAME:
        Entity3D.h
 
    DESCRIPTION:
        Entity3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ENTITY_3D_H
#define URIVL_ENTITY_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ReferenceFrame3D.h"

namespace uriVL
{

	/** The base virtual Entity3D class.
	 * 
	 *  Entity3D is the base class for all geometric 3D objects in the library.
	 *  An Entity3D is always defined relative to an external reference frame.
	 *	An Entity3D could in fact 
	 *
	 *	Version history
	 *		- 2004/07/10 [jyh]		first "implementation"
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
	class URIVL_EXPORT Entity3D
	{

		public:

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			virtual ~Entity3D(void);

            /**
             *  Sets the reference frame relative to which the 3D entity is defined.
             */
            void setReferenceFrame(const ReferenceFrame3D* ref);
             

            /**
             *  Returns the reference frame relative to which the 3D entity is defined.
             */
            const ReferenceFrame3D* getReferenceFrame(void) const;

			/**	Indicates whether this object belongs to a group
			 *	@return		true if this object belongs to  a group
			 */
			bool isGrouped(void) const;
			
			/**	Function to invoke to add this Entity3D to a group.  This function and
			 *	the function addMember of the same class accomplish the same thing.
			 *	The function to call is a pure matter of preference
			 *	@param group	the group Entity3D to which to add this object
			 */
			void addToGroup(const Entity3D* group);
			
			/**	Function to invoke to add an Entity3D to this group.  This function and
			 *	the function addTo of the same class  accomplish the same thing. 
			 *	The function to call is a pure matter of preference
			 *	@param member	the Entity3D to add to this group object
			 */
			void addMember(const Entity3D* member);
			
			/**	This function releases this object from the group it belongs to. This object
			 *	is removed from the group's member list and gets its own reference frame that
			 *	coincides with that of the group.
			 */
			void leaveGroup(void);
			

        protected:
        
            /** Contructor.
             *
             *  This constructor simply sets to 0 or NULL the basic instance variables
			 *	@param	theFrame	the object to copy
             */
			Entity3D(const ReferenceFrame3D* theFrame);

            /** Default contructor.
             *
             *  This constructor simply sets to 0 or NULL the basic instance variables
             */
			Entity3D(void);


		private:
		
            /** The reference frame relative to which the entity's is defined.
             */
            const ReferenceFrame3D  *frame_;

            /** Copy contructor. Disabled
             *
			 *	@param	theFrame	the object to copy
             */
			Entity3D(const Entity3D& theFrame);

            /** Copy operator. Disabled
             *
			 *	@param	theFrame	the object to copy
             */
			const Entity3D& operator = (const Entity3D& theFrame);
            

        friend class Point3D;
		friend class Object3D;            
            
	};
}

#endif	//	ENTITY_3D_H
