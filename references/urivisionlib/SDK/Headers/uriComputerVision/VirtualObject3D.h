/*  NAME:
        VirtualObject3D.h
 
    DESCRIPTION:
        VirtualObject3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VIRTUALOBJECT_3D_H
#define URIVL_VIRTUALOBJECT_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Entity3D.h"
#include "Camera.h"
#include "RasterImage.h"
#include "VirtualPart3D.h"


namespace uriVL
{

	/** The base virtual class for 3D objects.
	 * 
	 *  A 3D object is characterized by its pose (position <b>and</b> orientation)
	 *  relative to a given external reference frame and contains its own
	 *  internal reference frame as well.  This means that 3D entities suchs as
	 *  points and segments are not 3D objects (no complete internal reference frame).
	 *
     *  @author Fang Han, 3D Group, 
     *                    Department of Computer Science and Statistics,
     *                    University of Rhode Island
	 */
	class URIVL_EXPORT VirtualObject3D : public Entity3D
	{

		public:

            /** Default contructor.
             *
             *  This constructor simply sets to 0 or NULL the basic instance variables
             */
			VirtualObject3D( void);

            /** Default contructor.
             *
             *  This constructor simply sets to 0 or NULL the basic instance variables
             */
			VirtualObject3D(ReferenceFrame3D *theFrame);

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			virtual ~VirtualObject3D(void);
			
            /** renders each parts of the object basing on camera's reference frame
             *  @param  cam    the camera
             */
			void render(Camera *cam);
            
            /** Adds a part to this virtual object.  In future versions we will have to
             *  add parameters to define relative to what the part is defined, and the
             *  corresponding transformation.
             *  @param  thePart reference to the part to add
             *
             */
            void addPart(VirtualPart3D *thePart);

            /**
             *  Returns     parts which belong to object
             *  @return     the vector of virtual 3D parts
             */
            std::vector<VirtualPart3D *> *getParts(void);

            /**
			 *	Assigns new transformation 
        	 *	@param	knownFrames		the frames what we already know
        	 *	@param	framePaths		the path of reference frames
        	 *	@param	framesTransfs	the transformation of frames
			 */
            void setCameras(std::vector<Camera *> knownCameras);
/*            , 
                                        vector<vector <ReferenceFrame3D *>*> framePaths,
            					        vector<RigidTransformation3D *> framesTransfs);
*/            

//			/**	returns the transformation matrix Aco (from this object's reference frame to
//			 *	the camera's reference frame) for the camera specified
//			 *	@param	cam	the camera for which we want the transformation matrix
//			 *	@return	transformatioin matrix Aco
//			 */
//			HomogeneousMatrix *getTransformationMatrix(Camera *cam);

			/** Returns the read-only homogeneous matrix of the transformation from
			 *	the world reference frame to this object frame.
			 *	@return	the matrix of the world-to local frame for this object
			 */
			const HomogeneousMatrix* getWorldToObjectMatrix(void) const;

			/** Returns the index in the known cameras vector of the camera passed as parameter.
			 *  This information is used by a stereo interposer to set the visibility status
			 *  of a VirtualPoint3D.  Alternatively, we could have the VP3D ask for this 
             *  information itself (rioght now, we do the former).
			 *  @param cam  the camera we want the index of
			 *  @return     index of the camera in the "known cameras" vector
			 */
			int getCameraIndex(Camera *cam);
			
			void setDrawReferenceFrame(bool drawFrame);

			void setDrawVertices(bool drawVertices);
			
			static void setRenderMode(VirtualRenderMode mode);
			
        protected:
        
        
            /**
             *  texture mapped on the object
             */
        	RasterImage* texture_;
        	
            /**
             *  the vector of virtual 3D parts of the object
             */
        	std::vector<VirtualPart3D *> part_;
        	
            /**
             *  the vector of known cameras
             */
        	std::vector<Camera *> knownCameras_;
			
            /**
             *  the vector of reference frame paths 
             *  for each of path it's made of vector of reference frames
             */
        	std::vector<std::vector<ReferenceFrame3D*>*> framePaths_;
			
            /**
             *  the vector of rigid transformation frames
             */  
        	std::vector<RigidTransformation3D*> framesTransfs_;
        	
			
        	static VirtualRenderMode renderMode_;
        	
           /**
             *  Copy Constructor. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            VirtualObject3D(const VirtualObject3D& theObj);


            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const VirtualObject3D& operator = (const VirtualObject3D& theObj);
        	
	};
	
}
#endif  //  VIRTUALOBJECT_3D_H
