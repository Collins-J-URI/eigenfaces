/*  NAME:
        TubularSection3D.h

    DESCRIPTION:
        TubularSection3D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef	URIVL_TUBULAR_SECTION3D_H
#define	URIVL_TUBULAR_SECTION3D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "GraphicComponent3D.h"
#include "GlUtils.h"


namespace uriVL
{
	/**	Enum that defines what kind of partial tube we are dealing with
	 */
	typedef enum TubeType {	
							FULL_TUBE = -1,
							//
							NEARLY_FULL_TUBE = 0,
							ABOUT_3_QUARTER_TUBE,
							ABOUT_HALF_TUBE,
							ABOUT_QUARTER_CUBE,
							LESS_THAN_QUARTER_CUBE
	} TubeType;
	
	
	/** TubularSection3D class.
	 *
	 *  A "tubular section" is a section of an elliptic cylinder defined for a range of angles.
	 *	
	 *	This code is derived from the one I wrote for my personal "multiple-site" version
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
    class TubularSection3D : public GraphicComponent3D
    {
    	public:

    		/**	Constructor.  Creates a tubular section from the location of its origin, the
			 *	directions of the 2 axes of its elliptic bottom face, and the direction of its main
			 *	axis.
			 *	@param	origin		coordinates of the tubular section's origin (at one corner of the box)
			 *	@param	axes		the 2 axes of the tubular section's base ellipse
			 *	@param	theta0		start angle of the tubular section
			 *	@param	thetaf		end angle of the tubular section
			 */
    		TubularSection3D(GLfloat* origin, GLfloat** axes, GLfloat theta0, GLfloat thetaf);

    		/**	Constructor.  Creates a circular tubular section from the location of its origin, the
			 *	directions of the 2 axes of its circular bottom face, and the direction of its main
			 *	axis.
			 *	@param	origin		coordinates of the tubular section's origin (at one corner of the box)
			 *	@param	dirAxis		the 2 axes of the tubular section's base ellipse
			 *	@param	theta0		start angle of the tubular section
			 *	@param	thetaf		end angle of the tubular section
			 */
    		TubularSection3D(GLfloat* origin, GLfloat* dirAxis, GLfloat radius, GLfloat theta0,
    							GLfloat thetaf);

			/**	Destructor
			 */
    		~TubularSection3D(void);

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

			/**	Normal to the top faced
			 */
    		GLfloat		normalTop_[3];
			
			/**	Normal to the bottom faced
			 */
    		GLfloat		normalBottom_[3];
    		
			/**	Display resolution along a circular/elliptic face
			 */
    		int			circStep_;
					
			/**	Display resolution along a slab (main axis)
			 */
			int			slabStep_;
			
			/**	Start angle of the tube
			 */
			GLfloat		startAngle_;
			
			/**	End angle of the tube
			 */
			GLfloat		stopAngle_;
 
			/**	Number of segments that defined a partial circle/ellipse
			 */
			int			nbCircSegs_;
			
			/**	What kind of tube we're dealing with here
			 */
			TubeType	tubeType_;


            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            TubularSection3D(const TubularSection3D& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const TubularSection3D& operator =(const TubularSection3D& obj);


			/**	Initialize the cone.
			 *	@param	origin		coordinates of the cone's origin (at one corner of the box)
			 *	@param	axes		the 3 axes of the cone
			 *	@param	theta0		start angle of the tubular section
			 *	@param	thetaf		end angle of the tubular section
			 *	@param	dTheta		angle increment for circular/ellptic segments
			 */
    		void init_(GLfloat* origin, GLfloat** axes, GLfloat theta0, GLfloat dTheta);

			/**	Delete all allocated arrays
			 */
			void deleteArrays_(void);

    };
}

#endif   //  TUBULAR_SECTION3D_H
