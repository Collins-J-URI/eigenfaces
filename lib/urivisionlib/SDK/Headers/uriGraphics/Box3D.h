/*  NAME:
        Box3D.h

    DESCRIPTION:
        Box3D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef	URIVL_BOX3D_H
#define	URIVL_BOX3D_H

#include "GraphicComponent3D.h"
#include "GlUtils.h"


namespace uriVL
{
	/** Box3D class.
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
    class Box3D : public GraphicComponent3D
    {
    	public:
    		/**	Constructor
			 *	@param	origin		coordinates of the box's origin (at one corner of the box)
			 *	@param	axes		the 3 axes of the box
			 *	@param	isClosed	true if the box's top and bottom faces should be drawn
			 */
    		Box3D(GLfloat* origin, GLfloat** axes, bool isClosed);
    		
			/**	Destructor
			 */
			~Box3D(void);
    		
			/**	Inoperative for this type of object (for the time being)
			 *	@param	res		resolution to assign to the object
			 */
    		void setResolution(ResolutionLevel res);
			
			/**	Draw the object in the current graphic context
			 */
    		void draw(void);


    	private:

			/**	0-based  list of vertices
			 */
    		GLfloat**	vertex_;

			/**	0-based  list of normals
			 */
			GLfloat**	normal_;

			
			/**	Is the box closed? (top and bottom faces are drawn)
			 */
    		bool	closedBox_;
			
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            Box3D(const Box3D& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const Box3D& operator =(const Box3D& obj);


			/**	Initialize the box 
			 *	@param	origin		coordinates of the box's origin (at one corner of the box)
			 *	@param	axes		the 3 axes of the box
			 */
    		void init_(GLfloat* origin, GLfloat** axes);
			
			/**	Delete all allocated arrays
			 */
			void deleteArrays_(void);
			
    };
}

#endif  //  BOX3D_H
