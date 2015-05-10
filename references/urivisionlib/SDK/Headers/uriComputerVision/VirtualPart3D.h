/*  NAME:
        VirtualPart3D.h
 
    DESCRIPTION:
        VirtualPart3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VIRTUAL_PART_3D_H
#define URIVL_VIRTUAL_PART_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Entity3D.h"
#include "VirtualPoint3D.h"
#include "VirtualFace3D.h"


namespace uriVL
{
    class VirtualObject3D;
    

	/** Implements the VirtualPart3D class.
	 * 
	 *  A VirtualPart3D object stores reference frame of 3D object 
	 *
     *  @author Fang Han, 3D Group, 
     *                    Department of Computer Science and Statistics,
     *                    University of Rhode Island
	 */
	class URIVL_EXPORT VirtualPart3D : public Entity3D
	{
		public:

            /** Contructor.
             *
             *  @param  obj the VirtualObject3D this is a part of
             */
			VirtualPart3D(VirtualObject3D *obj);

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			virtual ~VirtualPart3D(void);

            /**
             *  Returns the vector of virtual point3D vertices related to the virtual part
             */
            std::vector<VirtualPoint3D *> *getVertices(void);

            /** Adds a vertex to the part
             *  @param  thePoint    reference to the vertex to add
             */
            void addVertex(VirtualPoint3D *thePoint);
			
            /** Adds a normal vector to the part
             *  @param  norm    reference to the normal vector to add
             */
            void addNormalVector(NormalVector *norm);
            
            /** Adds a face to the part.  If the vertices that are passed as parameters
             *  do not alrteady belong to the frame, they are added to it (this means that
             *  their part reference is set to refer to this object.
             *
             *  @param  theFace    the face to add, stored as a vector of faces
             */
            void addFace(VirtualFace3D *theFace);
            
            /**
             */
            void addFaceVertexInfo(int faceIndex, int vertIndex, int uvIndex, int normalIndex);
          	
          	 /** drawing a face to be a polygon
             *
             *  @param  camIndex    the number of cameras
             */
          	void render(int camIndex);
          	
          	/** set number of cameras
             *
             *  @param  n    the number of camera
             */
          	void setNbCameras(int n);


			void setDrawReferenceFrame(bool drawFrame);
			
			void setDrawVertices(bool drawVertices);

          	static void setRenderMode(VirtualRenderMode mode);


        private:
       
        	std::vector<VirtualPoint3D *> vertices_;
        	
        	std::vector <VirtualFace3D *> faces_;
			
			std::vector<NormalVector *> normals_;
        	
//vector<uv storage tbd *> uv_;

        	//vector<vector<VirtualPoint3D *>*> faces_;
        	
        	/** Reference to the object this part belongs to
        	 */
        	VirtualObject3D *object_;
        	
        	bool drawFrame_;

        	bool drawVertices_;
        	
        	static VirtualRenderMode renderMode_;
        	
            /** Copy constructor. Disabled.
             *  @param obj  reference to the object to copy
             */
            VirtualPart3D(const VirtualPart3D& obj);
            
            /** Copy operator. Disabled.
             *  @param obj  reference to the object to copy
             */
            const VirtualPart3D& operator = (const VirtualPart3D& obj);


			void drawReferenceFrame_(void);

			void setMaterial_(GLfloat ambRed, GLfloat ambGreen, GLfloat ambBlue, GLfloat difRed,
				GLfloat difGreen, GLfloat difBlue, GLfloat specRed, GLfloat specGreen,
				GLfloat specBlue, GLfloat shine);

        	
	};
	
}
#endif  //  VIRTUAL_PART_3D_H
