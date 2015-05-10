/*  NAME:
        VirtualPoint3D.h
 
    DESCRIPTION:
        VirtualPoint3D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VIRTUALPOINT_3D_H
#define URIVL_VIRTUALPOINT_3D_H

#include <vector>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Camera.h"
#include "Entity3D.h"


namespace uriVL
{
    class VirtualPart3D;
    class VirtualObject3D;
     
    typedef enum VisibilityCode
    {
            UNKNOWN = -1,
            VISIBLE,
            INVISIBLE,
            CONTACT
            
    } VisibilityCode;

   	typedef enum VirtualRenderMode
    {
            INTERPOSE = 1,
            GEOMETRY_TEST = 2
   
            
    } VirtualRenderMode;

   	typedef struct VisibilityStatus
    {
            VisibilityCode	code;
            float			confidence;
            
    } VisibilityStatus;

     
   
	/** Implements the VirtualPoint3D class.  A VirtualPoint3D belongs to a VirtualPart3D
     *  to be embedded into a real scene for 1-n cameras. A VirtualPoint3D object stores 
     *  the 3D coordinates of a point as well as information about normal directions, 
     *  visibility relative ot cameras, etc.
	 *
     *  @author Fang Han, 3D Group, 
     *                    Department of Computer Science and Statistics,
     *                    University of Rhode Island
	 */
    class URIVL_EXPORT VirtualPoint3D : public Entity3D
    {
        public:

       	/**
        	 *	Constructor.
        	 *	Creates a 3D point belonging to the VirtualPart3D passed as parameter
        	 *	@param	X		X coordinate of the point
        	 *	@param	Y		Y coordinate of the point
        	 *	@param	Z		Z coordinate of the point
        	 *  @param  thePart  the part of a virtual object this point is created relative to
        	 */
            VirtualPoint3D(double X, double Y, double Z, VirtualPart3D *thePart);

        	/**
        	 *	Constructor.
        	 *	Creates a 3D point in the reference frame received as parameter
        	 *	@param	theXYZ		coordinate array of the point
        	 *  @param  thePart  the part of a virtual object this point is created relative to
        	 */
            VirtualPoint3D(const double theXYZ[], VirtualPart3D* thePart);

        	/**
        	 *	Cloner constructor.
        	 *	Creates a clone of the VirtualPoint3D object passed as parameter (including that
        	 *  point's reference frame).
        	 *	@param	thePt		pointer to the Point3D object to clone
        	 */
            VirtualPoint3D(const Point3D* thePt);
 
 
            /**
             *	Destructor
             */          
            ~VirtualPoint3D(void);


			/**
			 *	Returns the point's X coordinate
			 *	@return		X coordinate of the point
			 */
            double getX(void) const;

			/**
			 *	Returns the point's Y coordinate
			 *	@return		Y coordinate of the point
			 */
            double getY(void) const;

			/**
			 *	Returns the point's Z coordinate
			 *	@return		Z coordinate of the point
			 */
            double getZ(void) const;
            
            /**
             *  Returns the point's coordinates vector.
             *  @return     a pointer to the object's private coordinate vector.
             */
            const double* getXYZ(void) const;

			/**
			 *	Assigns new x coordinate to the point
			 *	@param	XYZ 	new coordinate array for the 3D point
			 */
            void setPosition(const double XYZ[]);

			/**
			 *	Assigns new x coordinate to the point
        	 *	@param	X		X coordinate of the point
        	 *	@param	Y		Y coordinate of the point
        	 *	@param	Z		Z coordinate of the point
			 */
            void setPosition(double X, double Y, double Z);

            /**
             *  Returns the distance between this point and the one received as parameter.
             *  @param  p   the point to which the distance must be computed
             *  @return the distnace between this point and p
             */
            double distanceTo(const VirtualPoint3D *p) const;

            /** Reports the visibility status of the virtual point relatve to a specific
             *  camera
             *  @param  camIndex   index of the camera rwlative to which we inquire about visibility
             *  @return     visibility status relative to this camera
             */
            const VisibilityStatus* getVisibility(int camIndex) const;
            
            /** Sets the visibility status of the virtual point relatve to a specific
             *  camera
             *  @param  camIndex   index of the camera rwlative to which we inquire about visibility
             *  @param  vis        visibility status relative to this camera
             */
            void setVisibility(int camIndex, const VisibilityStatus* vis);
            
            void setColor(float theRed, float theGreen, float theBlue);
            
            void setColor(const float rgb[]);
			
			/**
			 *	Adds neighbor points to the 3D point
			 *	@param	pt 	the 3D point
			 */
			void addNeighbor(VirtualPoint3D* pt);


            /** Adds information about a virtual face the point belongs to
             *  @param  faceIndex       index of the face we are adding information about
             *  @param  uvIndex         uh???  not sure anymore
             *  @param  normalIndex     same here
             */
            void addFaceInfo(int faceIndex, int uvIndex, int normalIndex);
          	
            /** Called to select/deselect a VirtualPoint3D 
             *  @param  selected    tells whether the point should be selected
             */
          	void setSelected(bool selected);
          	
          	bool isSelected(void) const;

            /** Called to select/deselect a VirtualPoint3D 
             *  @param  selected
             */
          	void setSelectedNeighbor(bool selectedNeighbor);
          	
          	
          	static void setRenderMode(VirtualRenderMode mode);
            
            /** Assigns a new rendering scale to this virtual point
             *  @param  renderScale new rendering scale for this virtual point
             */
            void setRenderScale(GLfloat renderScale);
            

        private:

        	/**
        	 *	The Point3D this object is based on
        	 */
            Point3D* point_;
            
            /** Reference to the VirtualPart3D (of a VirtualObject3D) that the point
             *  belongs to.
             */
            VirtualPart3D* part_;
            
            /**
        	 *	Stores visibility status relatrive to each known camera
        	 */
        	VisibilityStatus** visibility_;
        	
        	std::vector<int> normalIndex_;
        	std::vector<int> uvIndex_;
        	std::vector<VirtualPoint3D*> neighbors_;
        	
        	static VirtualPoint3D* currentSelected_;
            
        	/**  the point's RGB color
        	 */
        	GLfloat* color_;
        	
         	bool isSelected_;

        	bool isSelectedNeighbor_;
        	
            /** Rendering scale for this virtual point
             */
            GLfloat renderScale_;
            
            /** Does this virtual point have its own rendering scale?
             */
            bool hasIndividualRenderScale_;
        	
           /** Default scale to use to render the Point3D as a small cube
             */
            static const GLfloat defaultRenderScale_;
            
        	static const GLfloat defaultColor_[4];
        	
        	static VirtualRenderMode renderMode_;


            /** Copy constructor. Disabled.
             *  @param obj  reference to the object to copy
             */
            VirtualPoint3D(const VirtualPoint3D& obj);
            
            /** Copy operator. Disabled.
             *  @param obj  reference to the object to copy
             */
            const VirtualPoint3D& operator = (const VirtualPoint3D& obj);


        	void setNbCameras_(int n);
			
			void render_(int camIndex) const;
            
            
			
        friend class VirtualPart3D;
        friend class VirtualObject3D;
                    
    };
}   

#endif  //  VIRTUALPOINT_3D_H
