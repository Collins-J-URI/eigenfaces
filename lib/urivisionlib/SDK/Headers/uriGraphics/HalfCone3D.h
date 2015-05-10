/*  NAME:
        HalfCone3D.h

    DESCRIPTION:
        HalfCone3D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef	URIVL_HALF_CONE3D_H
#define	URIVL_HALF_CONE3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "GraphicComponent3D.h"
#include "GlUtils.h"


namespace uriVL
{
	/** HalfCone3D class.
	 *
	 *  This code is derived from the one I wrote for my personal "multiple-site" version
	 *  of the SMART telerobotics project I did while at Ecole Poytechnique de Montreal.
	 *  I co-wrote (with the interns I supervised) the initial OpenGL C code, then rewrote
	 *  the whole thing as C++ classes and defined my own 3D format (mostly useful to define
	 *  models of robots) to match these classes.  There is absolutely nothing left of the
	 *  original stuff, except for the file format and object loader, which were mine to
	 *  begin with.
	 *
	 *  Older versions of this code have been used (and will continued to be used) as demos
	 *  for my computer graphics courses.
	 *
	 *	Version history
	 *		- 2005/04/27 [jyh]		adapted and brought into the uriVisionLib
	 *		- 2010/07/2x [jyh]		massive cleanup of C++ code, with 0-warning objective
	 *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class HalfCone3D : public GraphicComponent3D
    {
    	public:

    		/**	Constructor.  Creates a pointed half-cone from the location of its origin, the
			 *	directions of the 2 axes of its elliptic bottom face, and the direction of its main
			 *	axis.
			 *	@param	origin			coordinates of the cone's origin (at one corner of the box)
			 *	@param	axes			the 3 axes of the cone
			 *	@param	lengthIsClosed	true if the cone's length face should be drawn
			 *	@param	sideClosed		true if the cone's bottom face should be drawn
			 */
    		HalfCone3D(GLfloat* origin, GLfloat** axes, bool lengthIsClosed,
    						bool sidesClosed);

    		/**	Constructor.  Creates a truncated half-cone from the location of its origin, the
			 *	directions of the 2 axes of its elliptic bottom face, that of its elliptic top
			 *	face, and the direction of its main
			 *	axis.
			 *	@param	origin			coordinates of the cone's origin (at one corner of the box)
			 *	@param	botAxes			the 2 axes of the cone bottom face
			 *	@param	dirAxis			the direction of the cones main axis
			 *	@param	topAxes			the 2 axes of the cone top face
			 *	@param	lengthIsClosed	true if the cone's length face should be drawn
			 *	@param	sidesAreClosed	true if the cone's top and bottom faces should be drawn
			 */
     		HalfCone3D(GLfloat* origin, GLfloat** botAxes, GLfloat* dirAxis, GLfloat** topAxes,
    						bool lengthIsClosed, bool sidesAreClosed);

    		/**	Constructor.  Creates a truncated circular half-cone from the loation of its origin,
			 *	direction of main axis, and radii of top and bottom faces.
			 *	@param	origin			coordinates of the box's origin (at one corner of the box)
			 *	@param	dirAxis			the direction of the cones main axis
			 *	@param	bottomRadius	the radius of the cone's circular bottom face
			 *	@param	topRadius		the radius of the cone's circular top face
			 *	@param	lengthIsClosed	true if the cone's length face should be drawn
			 *	@param	sidesAreClosed	true if the cone's top and bottom faces should be drawn
			 */
    		HalfCone3D(GLfloat* origin, GLfloat* dirAxis, GLfloat bottomRadius, GLfloat topRadius,
    						bool lengthIsClosed, bool sidesAreClosed);
			/**	Destructor
			 */
    		~HalfCone3D(void);

			/**	Assigns a new drawing resolution to the object
			 *	@param	res		resolution to assign to the object
			 */
    		void setResolution(ResolutionLevel res);

			/**	Assigns a new drawing resolution to the object
			 *	@param	circRes		resolution to assign along a circular/elliptic face
			 *	@param	slabRes		resolution to assign along the main axis
			 */
    		void setResolution(ResolutionLevel circRes, ResolutionLevel slabRes);

			/**	Draw the object in the current graphic context
			 */
    		void draw(void);

    	private:

			/**	0-based  list of vertices
			 */
    		GLfloat***	vertex_;

			/**	0-based  list of normals
			 */
			GLfloat**	normal_;

			/**	Normal to the top face
			 */
    		GLfloat		normalTop_[3];
			
			/**	Normal to the bottom face
			 */
    		GLfloat		normalBottom_[3];
    		
			/**	Normal to the length-side face
			 */
    		GLfloat		normalLength_[3];
    		
			/**	Display resolution along a circular/elliptic face
			 */
    		int		circStep_;
					
			/**	Display resolution along a slab (main axis)
			 */
			int		slabStep_;
			
			/** Is the bottom face drawn?
			 */
    		bool	closedBottom_;

			/** Is the top face drawn?
			 */
    		bool	closedTop_;

			/** Is the length-side face drawn?
			 */
    		bool	closedLength_;

			/**	Have the static variables been initialized?
			 */
    		static bool initCtSt_;
			
			/**	Precomputed array of cosine values
			 */
    		static GLfloat ct_[kVeryHighDefCircPts];

			/**	Precomputed array of sine values
			 */
    		static GLfloat st_[kVeryHighDefCircPts];

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            HalfCone3D(const HalfCone3D& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const HalfCone3D& operator =(const HalfCone3D& obj);

			/**	Initialize the cone.
			 *	@param	origin		coordinates of the cone's origin (at one corner of the box)
			 *	@param	axes		the 3 axes of the cone
			 */
    		void init_(GLfloat* origin, GLfloat** axes);

			/**	Initialize the cone.
			 *	@param	origin		coordinates of the cone's origin (at one corner of the box)
			 *	@param	botAxes		the 2 axes of the cone bottom face
			 *	@param	dirAxis		the direction of the cones main axis
			 *	@param	topAxes		the 2 axes of the cone top face
			 */
    		void init_(GLfloat* origin, GLfloat** botAxes, GLfloat* dirAxis, GLfloat** topAxes);

			/**	Delete all allocated arrays
			 */
			void deleteArrays_(void);

			/**	Initializes the classe's static variables
			 */
    		static void initStatic_(void);

//    		void init(GLfloat* origin, GLfloat** axes);
//    		void init(GLfloat* origin, GLfloat** botAxes, GLfloat* dirAxis, GLfloat** topAxes);

    };
}

#endif  //  HALF_CONE3D_H
