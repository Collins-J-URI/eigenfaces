/*  NAME:
        PolyMesh3D.h

    DESCRIPTION:
        PolyMesh3D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef	URIVL_POLY_MESH3D_H
#define	URIVL_POLY_MESH3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "GraphicComponent3D.h"
#include "GlUtils.h"


namespace uriVL
{
	/** The base PolyMesh3D class.
	 *
	 *  A PolyMesh3D is a polygonal mesh defined as a set of polygonal faces.<p>
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
    class PolyMesh3D : public GraphicComponent3D
    {
    	public:
    		//
    		//	Constructor and destructor
    		//
    		PolyMesh3D(int nVertices, int nFaces, const GLfloat *const* theVertex,
    								const GLuint *const* theFace, bool isSmooth);
			/**	Destructor
			 */
    		~PolyMesh3D(void);

			/**	Assigns a new drawing resolution to the object
			 *	@param	res		resolution to assign to the object
			 */
    		void setResolution(ResolutionLevel res);

			/**	Draw the object in the current graphic context
			 */
    		void draw(void);


    	private:

			/**	Total number of vertices of the mesh
			 */
    		GLuint nbVert_;

			/**	Total number of faces of the mesh
			 */
    		GLuint nbFaces_;

			/**	Number of triangles of the mesh
			 */
    		GLuint nbTriangles_;

			/**	Number of quads of the mesh
			 */
    		GLuint nbQuads_;

			/**	Number of faces with more than 4 vertices
			 */
    		GLuint nbPolygons_;
			
			/**	Is the poly mesh smooth (vertices attached to vertices rather than to faces)
			 */
    		bool smoothPoly_;

			/**	0-based flat list of vertex coordinates for the polygonal mesh
			 *	first index: vertex index; second index  0 --> X, 1 --> Y, 2 --> Z
			 */
    		GLfloat** vertex_;

			/**	0-based flat list of vertex normal coordinates for a smooth polygonal mesh
			 *	first index: vertex index; second index  0 --> X, 1 --> Y, 2 --> Z
			 */
    		GLfloat** normalV_;		//	one normal per vertex for a smooth polyhedron

			/**	0-based flat list of face normal coordinates for the polygonal mesh
			 *	first index: vertex index; second index  0 --> X, 1 --> Y, 2 --> Z
			 */
    		GLfloat** normalF_;

			/**	List of the indices all vertices making up all faces of the poly mesh
			 *	first index: index of the face
			 *	second index: number n of vertices of the face; indices 1-n: indices of the vertices, listed counterclockwise
			 */
    		GLuint** face_;

			/**	List of the indices all vertices making up all triangular faces of the poly mesh
			 *	first index: index of the face
			 *	second index: 0-2 indices of the vertices, listed counterclockwise
			 */
    		GLuint** triangle_;

			/**	List of the indices all vertices making up all quad faces of the poly mesh
			 *	first index: index of the face
			 *	second index: 0-3 indices of the vertices, listed counterclockwise
			 */
    		GLuint** quad_;

			/**	List of the indices all vertices making up all faces of the poly mesh with more than 4 vertices
			 *	first index: index of the face
			 *	second index: number n of vertices of the face; indices 1-n: indices of the vertices, listed counterclockwise
			 */
    		GLuint** polygon_;
			
			/**	List of the indices all vertices making up all faces of the poly mesh with more than 4 vertices
			 *	first index: index of the face
			 *	second index: number n of vertices of the face; indices 1-n: indices of the vertices, listed counterclockwise
			 */
    		GLuint** facesForVertex_;

			

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            PolyMesh3D(const PolyMesh3D& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const PolyMesh3D& operator =(const PolyMesh3D& obj);

			/** Initializes the arrays of vertex coordiates and faces
			 */
    		void init_(const GLfloat *const* theVert, const GLuint *const* theFace);

    };
}

#endif
