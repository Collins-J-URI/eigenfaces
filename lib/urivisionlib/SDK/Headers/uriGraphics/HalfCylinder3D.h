/*  NAME:
        HalfCylinder3D.h

    DESCRIPTION:
        HalfCylinder3D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef	URIVL_HALF_CYLINDER3D_H
#define	URIVL_HALF_CYLINDER3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "GraphicComponent3D.h"
#include "GlUtils.h"
#include "TubularSection3D.h"


namespace uriVL
{
	/** HalfCylinder3D class.
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
    class HalfCylinder3D : public TubularSection3D
    {
    	public:

    		/**	Constructor.  Creates a half-cylinder from the location of its origin, the
			 *	directions of the 2 axes of its elliptic bottom face, and the direction of its main
			 *	axis.
			 *	@param	origin			coordinates of the cone's origin (at one corner of the box)
			 *	@param	axes			the 3 axes of the half-cylinder
			 *	@param	lengthIsClosed	true if the half-cylinder's length face should be drawn
			 *	@param	sideClosed		true if the half-cylinder's bottom face should be drawn
			 */
    		HalfCylinder3D(GLfloat* origin, GLfloat** axes, bool LengthSideIsClosed,
    						bool sidesAreClosed);

    		/**	Constructor.  Creates a half-cylinder from the loation of its origin,
			 *	direction of main axis, and radii of top and bottom faces.
			 *	@param	origin			coordinates of the box's origin (at one corner of the box)
			 *	@param	dirAxis			the direction of the half-cylinder's main axis
			 *	@param	bottomRadius	the radius of the half-cylinder's circular bottom face
			 *	@param	topRadius		the radius of the half-cylinder's circular top face
			 *	@param	lengthIsClosed	true if the half-cylinder's length face should be drawn
			 *	@param	sidesAreClosed	true if the half-cylinder's top and bottom faces should be drawn
			 */
    		HalfCylinder3D(GLfloat* origin, GLfloat* dirAxis, GLfloat radius,
    						bool LengthSideIsClosed, bool sidesAreClosed);
			/**	Destructor
			 */
    		~HalfCylinder3D(void);

			/**	Draw the object in the current graphic context
			 */
    		void draw(void);


    	private:

			/**	Normal to the top face
			 */
    		GLfloat		normalTop_[3];
			
			/**	Normal to the bottom face
			 */
    		GLfloat		normalBottom_[3];
    		
			/**	Normal to the length-side face
			 */
    		GLfloat		normalLength_[3];
    		

			/** Is the length-side face drawn?
			 */
    		bool	closedLength_;

			/** Are the top and bottom faces drawn?
			 */
    		bool	closedSides_;

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            HalfCylinder3D(const HalfCylinder3D& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const HalfCylinder3D& operator =(const HalfCylinder3D& obj);

			/**	Initialize the half-cylinder.
			 *	@param	axes		the 3 axes of the half-cylinder
			 */
    		void init_(GLfloat** axes);
    };
}

#endif  //  HALF_CYLINDER3D_H
