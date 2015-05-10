/*  NAME:
        GraphicComponent3D.h

    DESCRIPTION:
        GraphicComponent3D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef	URIVL_GRAPHIC_COMPONENT_3D_H
#define	URIVL_GRAPHIC_COMPONENT_3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "GlUtils.h"



//	Levels of radial resolution for cylinders and cones
#define		kVeryHighDefCircPts		48
#define		kVeryHighDefCircPtsHalf	24
//
#define		kFineDefCircScale		 2
#define		kMediumDefCircScale		 3
#define		kCoarseDefCircScale		 4
#define		kVeryLowDefCircScale	 6
//
//	Degrees of resolution along the axis ("slabs") for cylinders, prisms, cones
#define		kVeryHighDefSlabs		16
#define		kFineDefSlabScale		 2
#define		kMediumDefSlabScale		 4
#define		kCoarseDefSlabScale		 8
#define		kVeryLowDefSlabScale	16


namespace uriVL
{
    //	Different levels of resolution for the 3D objects
    //
    typedef enum ResolutionLevel
    {
        kVeryHighResolution,
        kFineResolution,
        kMediumResolution,
        kCoarseResolution,
        kVeryLowResolution
    }
    ResolutionLevel;

    /** Basic types of graphical components
     *
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
     *	Comments:	The normal computation members of this class are only called when
     *				the objects are created or if/when their shape is modified (rare).
     *				Optimization is therefore not a major issue, which explains why I
     *				went overboard into decomposing the computation	into very small
     *				pieces, each of which is defined as a function.  Even if the
     *				compiler screws up the inlining, the penalty won't be too heavy.<p>
	 *	Version history
	 *      - 2001/05/01 [jyh]      last version of the old telerobotics code
	 *		- 2005/04/27 [jyh]		adapted and brought into the uriVisionLib
	 *		- 2010/07/2x [jyh]		massive cleanup of C++ code, with 0-warning objective
	 *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    typedef enum GraphicalComponentType
    {
    	kOpenBoxObject = 0,
    	kClosedBoxObject,
    	kOpenCylinderObject,
    	kClosedCylinderObject,
    	kTubularSectionObject,
    	kOpenHalfCylinderObject,
    	kClosedHalfCylinderObject,
    	kOpenPrismObject,
    	kClosedPrismObject,
    	kOpenConeObject,
    	kClosedConeObject,
    	kOpenHalfConeObject,
    	kClosedHalfConeObject,
    	kPolyhedronObject,
    	//
    	kNbComponentTypes
    }
    GraphicalComponentType;


    /** This is the parent class for all basic 3D shapes (boxes, cylinders, quad and
     *  triangle meshes, generic polyhedra.  Virtual objects are made up of one or
     *  more GraphicComponent3D objects.<b>
	 *	<b>Important:</b> A GraphicComponent3D is <i>not</i> a 3D object in the sense that it
	 *	does not have a ReferenceFrame3D.  A GraphicComponent3D is simply something that will
	 *	be attached (has-a) to an Entity3D that need to be rendered.
     *
	 *  This code is derived from the one I wrote for my personal "multiple-site" version
	 *  of the SMART telerobotics project I did while at Ecole Poytechnique de Montreal.
	 *  I co-wrote (with the interns I supervised) the initial OpenGL C code, then rewrote
	 *  the whole thing as C++ classes and defined my own 3D format (mostly useful to define
	 *  models of robots) to match these classes.  There is absolutely nothing left of the
	 *  original stuff, except for the file format and object loader, which were mine to
	 *  begin with.<p>
	 *
	 *  Older versions of this code have been used (and will continued to be used) as demos
	 *  for my computer graphics courses.
	 *
	 *	Version history
	 *		- 2005/04/27 [jyh]		adapted and brought into the uriVisionLib
	 *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class GraphicComponent3D
    {
    	public:

    		/** Destructor
    		 */
    		virtual ~GraphicComponent3D(void);

			/**	Draw the object in the current graphic context
			 */
    		virtual void draw(void);

    		void setMaterial(float params[]);
    		void setMaterial(GLfloat ambRed, GLfloat ambGreen, GLfloat ambBlue, GLfloat difRed,
    						GLfloat difGreen, GLfloat difBlue, GLfloat specRed, GLfloat specGreen,
    						GLfloat specBlue, GLfloat shine);
			/**	Assigns a new drawing resolution to the object
			 *	@param	res		resolution to assign to the object
			 */
    		virtual void setResolution(ResolutionLevel res) = 0;
    		//
    		static GLfloat norm(GLfloat* V);
    		static void normalizeVector(GLfloat* V);
    		static GLfloat dotProduct(GLfloat* e1, GLfloat* e2);
    		static void crossProduct(GLfloat* e1, GLfloat* e2, GLfloat* nv);
    		static void commonNormalVector(GLfloat* e1, GLfloat* e2, GLfloat* nv);
    		static void faceExactNormal(GLfloat* v0, GLfloat* v1, GLfloat* v2, GLfloat* nVect);
    		static void faceAverageNormal(GLuint n, GLfloat** v, GLfloat* nVect);
    		static void faceAverageNormal(GLuint* index, GLfloat** v, GLfloat* nVect);
    		static void faceAverageNormal(GLfloat* v0, GLfloat* v1, GLfloat* v2, GLfloat* v3,
    								GLfloat* nVect);
    		static void vertexAverageNormal(GLfloat* v0, GLfloat* v1, GLfloat* v2, GLfloat* d,
    								GLfloat* nVect);

    	protected:

			/** stores "ambient" light material property for this object
			 */
    		GLfloat		ambientMat_[3];

			/** stores "diffuse" light material property for this object
			 */
    		GLfloat		diffuseMat_[3];

			/** stores "specular" light material property for this object
			 */
    		GLfloat		specularMat_[3];

			/** stores "shininess" light material property for this object
			 */
    		GLfloat		shiny_;

            /** Default constructor
             */
    		GraphicComponent3D(void);


    		void setReflectionParameters_(void);
    };
}

#endif  //  GRAPHIC_COMPONENT_3D_H
