/*  NAME:
        Cone3D.h

    DESCRIPTION:
        Cone3D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef	URIVL_CONE3D_H
#define	URIVL_CONE3D_H

#include "GraphicComponent3D.h"
#include "GlUtils.h"


namespace uriVL
{
	/** Cone3D class.
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
    class Cone3D : public GraphicComponent3D
    {
    	public:

    		/**	Constructor.  Creates a pointed cone from the location of its origin, the
			 *	directions of the 2 axes of its elliptic bottom face, and the direction of its main
			 *	axis.
			 *	@param	origin		coordinates of the cone's origin (at one corner of the box)
			 *	@param	axes		the 3 axes of the cone
			 *	@param	isClosed		true if the cone's bottom face should be drawn
			 */
    		Cone3D(GLfloat* origin, GLfloat** axes, bool isClosed);

    		/**	Constructor.  Creates a truncated cone from the location of its origin, the
			 *	directions of the 2 axes of its elliptic bottom face, that of its elliptic top
			 *	face, and the direction of its main
			 *	axis.
			 *	@param	origin		coordinates of the cone's origin (at one corner of the box)
			 *	@param	botAxes		the 2 axes of the cone bottom face
			 *	@param	dirAxis		the direction of the cone's main axis
			 *	@param	topAxes		the 2 axes of the cone's top face
			 *	@param	isClosed		true if the cone's top and bottom faces should be drawn
			 */
    		Cone3D(GLfloat* origin, GLfloat** botAxes, GLfloat* dirAxis, GLfloat** topAxes,
    						bool isClosed);

    		/**	Constructor.  Creates a truncated circular cone from the loation of its origin,
			 *	direction of main axis, and radii of top and bottom faces.
			 *	@param	origin			coordinates of the cone's origin (at one corner of the box)
			 *	@param	dirAxis			the direction of the cone's main axis
			 *	@param	bottomRadius	the radius of the cone's circular bottom face
			 *	@param	topRadius		the radius of the cone's circular top face
			 *	@param	isClosed		true if the cone's top and bottom faces should be drawn
			 */
    		Cone3D(GLfloat* origin, GLfloat* dirAxis, GLfloat bottomRadius, GLfloat topRadius,
    						bool isClosed);

			/**	Destructor
			 */
    		~Cone3D(void);

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
            Cone3D(const Cone3D& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const Cone3D& operator =(const Cone3D& obj);

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
    };
}

#endif  //  CONE3D_H
