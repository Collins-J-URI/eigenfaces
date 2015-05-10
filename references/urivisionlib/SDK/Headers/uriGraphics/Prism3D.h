/*  NAME:
        Prism3D.h

    DESCRIPTION:
        Prism3D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef	URIVL_PRISM3D_H
#define	URIVL_PRISM3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "GraphicComponent3D.h"
#include "GlUtils.h"


namespace uriVL
{
	/** Prism3D class.
	 *
	 *  A Prism3D can be closed or open, and smooth or not smooth.  A smooth prism has
	 *  normal vectors attached to the vertices of the mesh, whereas a non-smooth
	 *  prism has average normals attached to the faces of the mesh.<p>
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
    class Prism3D : public GraphicComponent3D
    {
    	public:
    		//
    		//	Constructor and destructor
    		//
    		Prism3D(int n, GLfloat** topFace, GLfloat** bottomFace, bool isClosed,
    					bool isSmooth);
    		Prism3D(int n, GLfloat** bottomFace, GLfloat* axis,
    					bool isClosed, bool isSmooth);
			/**	Destructor
			 */
    		~Prism3D(void);

			/**	Assigns a new drawing resolution to the object
			 *	@param	res		resolution to assign to the object
			 */
    		void setResolution(ResolutionLevel res);

			/**	Draw the object in the current graphic context
			 */
    		void draw(void);


    	private:

			/**	0-based  list of vertices
			 */
    		GLfloat***	vertex_;

			/**	0-based  list of normals
			 */
			GLfloat**	normalF_;

			/**	0-based  list of normals
			 */
			GLfloat***	normalV_;

			/**	Normal to the top face
			 */
    		GLfloat		normalTop_[3];
			
			/**	Normal to the bottom face
			 */
    		GLfloat		normalBottom_[3];

 			/**	Display resolution along slice
			 */
    		int		nbPtsPerSlice_;
					
			/**	Display resolution along a slab (main axis)
			 */
			int		slabStep_;
			
			/** Is the prism closed on the top and bottom faces?
			 */
    		bool	closedPrism_;
			
			/** Is the prism smooth (with vertex normals computed)?
			 */
    		bool	smoothPrism_;

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   pointer to the object to copy
             */
            Prism3D(const Prism3D& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const Prism3D& operator =(const Prism3D& obj);


    		void init_(GLfloat** topFace, GLfloat** bottomFace);

			/**	Delete all allocated arrays
			 */
			void deleteArrays_(void);

    };
}

#endif  //  URIVL_PRISM3D_H
