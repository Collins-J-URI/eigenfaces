/*  NAME:
        ReferenceFrame3D.h
 
    DESCRIPTION:
        ReferenceFrame3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_REFERENCE_FRAME_3D_H
#define URIVL_REFERENCE_FRAME_3D_H

#include <list>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RigidTransformation3D.h"


namespace uriVL
{
	/** Enum type that define shortcuts for accessing particular reference frames
	 *	from an Entity3D
	 */
	typedef enum ReferenceFrameShortcut {
									WORLD_FRAME = 0,
									VIEWER_FRAME = 1,	//	current GL camera
									PARENT_FRAME
	} ReferenceFrameShortcut;
	

    class Entity3D;
    
	/** A ReferenceFrame3D is the object relative to which any Entity3D is defined.
	 *
	 *  The notion of ReferenceFrame3D is closely related to that of RigidTransformation3D.
	 *  Any ReferenceFrame3D is defined relative to another ReferenceFrame3D, down to
	 *  the "world" ReferenceFrame3D, which is a static variable of this class.  A
	 *  ReferenceFrame3D is related to its reference ReferenceFrame3D through a 
	 *  RigidTransformation3D object.
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT ReferenceFrame3D
    {
        public:

        	/**
        	 *	Constructor.
        	 *	Creates a 3D reference frame defined relative to an external
        	 *  reference frame with which it initially coincides.
        	 *	@param	theRef		external frame relative to which this one is defined
        	 */
            ReferenceFrame3D(const ReferenceFrame3D* theRef);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D reference frame defined relative to the global reference frame.
        	 *	@param	theTransf	rigid transformation from the global reference frame to this one
        	 */
            ReferenceFrame3D(const RigidTransformation3D *theTransf);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D reference frame defined relative to an external
        	 *  reference frame.
        	 *	@param	theRef		external frame relative to which this one is defined
        	 *	@param	theTransf	rigid transformation from the external frame to this one
        	 */
            ReferenceFrame3D(const ReferenceFrame3D *theRef, const RigidTransformation3D *theTransf);


            /**
             *  Returns the reference frame relative to which the 3D entity is defined.
             */
            const ReferenceFrame3D* getParentFrame(void);


            /**
             *  Called by a 3D entity that reports itself to this reference frame to
             *  tell it that it uses it as a reference.
             *
             *  @param  theEntity   Entity3D that reports using this frame as reference
             */
            void notifyUseAsReference(const Entity3D* theEntity) const;
            

            /**
             *  Called by a 3D entity that reports itself to this reference frame to
             *  tell it that it does not use it anymore as a reference.
             *
             *  @param  theEntity   Entity3D that reports not using this frame as reference
             *                      anymore
             */
            void notifyReleaseReference(const Entity3D* theEntity) const;


            /**
             *  Sets a new transformation to this frame from the one it is defined
             *  relative to.
             *
             *  @param  thePose     new rigid transformation between this reference frame and the
             *                      one it is defined relative to
             */
            void setTransformation(const RigidTransformation3D* thePose);


            /**
             *  Applies a rigid transformation to this reference frame, to change it pose
             *  relative to the reference frame it is defined relative to.  The transformation
             *  to apply must be defined relative to the current/moving frame.
             *  @param  theTransf   the transformation to apply to this reference frame
             */
            void applyTransformation(const RigidTransformation3D* theTransf);
            
            
            /**
             *  Applies a rotation to this reference frame, to change its pose
             *  relative to the reference frame it is defined relative to.  The rotation
             *  to apply must be defined relative to the current/moving frame.
             *  @param  theRot   the rotation to apply to this reference frame
             */
            void applyTransformation(const Rotation3D* theRot);
            
            
            /**
             *  Applies a translation to this reference frame, to change its pose
             *  relative to the reference frame it is defined relative to.  The translation
             *  to apply must be defined relative to the current/moving frame.
             *  @param  theTrans   the translation to apply to this reference frame
             */
            void applyTransformation(const Translation3D* theTrans);


            /** Calculates and returns the rigid transformation from this reference
             *  frame to the one given by the shortcut.
			 *	@param	ref		indicates the frame relative to which we want the transformation
             *  @return the rigid trasnformation from this reference frame to the one
             *				identified by the shortcut
             */
            const RigidTransformation3D* getTransformation(ReferenceFrameShortcut ref) const;


            /** Calculates and returns the rigid transformation from this reference
             *  frame to the one received as parameter.  Returns NULL if such a
             *  transformation cannot be calculated.
             *  @param  theFrame    the frame whose pose relative to this frame
             *                      must be calculated
             *  @return the rigid trasnformation from this reference frame to theFrame
             */
            const RigidTransformation3D* getTransformation(const ReferenceFrame3D *theFrame) const;


            /** Calculates and returns the transformation from this reference
             *  frame to the one received as parameter.  Returns NULL if such a
             *  transformation cannot be calculated.
             *  @param  startFrame  the frame relative to which the pose is calculated
             *  @param  endFrame    the frame whose pose relative to startFrame
             *                      must be calculated
             *  @return the rigid transformation from startFrame to endFrame
             */
            static const RigidTransformation3D* getTransformation(const ReferenceFrame3D *startFrame,
                                                       const ReferenceFrame3D *endFrame);

            /** Returns the root "global" reference frame
             *  @return     the root "global" reference frame
             */
            static const ReferenceFrame3D* getGlobalReferenceFrame(void);

			/**	Sets the current viewer reference frame
			 *	@param	the current viewer frame for the application
			 */
			static void setViewerFrame(const ReferenceFrame3D* viewerFrame);
            
            /** Attempts to delete the ReferenceFrame3D object passed as parameter but first check if the
             *  ReferenceFrame3D still has Entity3D objects attached to it.  If it does, the 
             *  deletion attempt fails and the function returns falsel otherwise the object is
             *  properly freed and the function returnd true;
             *  @param  ref     the ReferenceFrame3D object we want to delete
             *  @return     true of the cal succeeded, false otherwise
             */
            static bool deleteFrame(ReferenceFrame3D* ref);
			
            
        private:

            /** A ReferenceFrame3D has its own unique id
             */
            int id_;
            
        	/**
        	 *	The reference frame relative to which this one is defined.
        	 */
            const ReferenceFrame3D* ref_;
            
        	/**
        	 *	The rigid transformation between this reference frame and the one it is
        	 *  defined relative to.  If this variable is NULL this corresponds to
             *  an identity transformation.
        	 */
            RigidTransformation3D* transf_;
            
        	/**
        	 *	List of all 3D entities that are defined relative to this 
        	 */
            mutable std::list<const Entity3D*> entitiesReferenced_;
            
            
            /** Counter for all the ReferenceFrame3D objects in the application.
             *  Used to generate id_ for a new ReferenceFrame3D object
             */
            static int frameCounter_;
            
            /**
             *  the root (global) reference frame
             */
            static ReferenceFrame3D* rootFrame_;
                        
            /** Current viewer frame. At a given time, there can be only one viewer frame
			 *	for the entire application. Access to this frame is protected by a mutex lock.
			 */
			static const ReferenceFrame3D* viewerFrame_;
			
            /**
             *	Destructor
             */          
            ~ReferenceFrame3D(void);

            /**
             *  Private constructor for the root (global) reference frame.
             */
            ReferenceFrame3D(void);

        	/**
        	 *	Copy Constructor. Disabled
        	 *	@param	theRef		external frame relative to copy
        	 */
            ReferenceFrame3D(const ReferenceFrame3D& theRef);
        	/**
        	 *	Copy Operator. Disabled
        	 *	@param	theRef		external frame relative to copy
        	 */
            const ReferenceFrame3D& operator = (const ReferenceFrame3D& theRef);

             
    };
    
        
}   

#endif  //  REFERENCE_FRAME_3D_H

