/*  NAME:
        SimpleGraphicObject2D.h

    DESCRIPTION:
        SimpleGraphicObject2D public header.

    COPYRIGHT:
        (c) 2013-2014, 3D Group for Interactive Visualization
					University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef	URIVL_SIMPLE_GRAPHIC_OBJECT_2D_H
#define	URIVL_SIMPLE_GRAPHIC_OBJECT_2D_H

#include "GlUtils.h"
#include "DrawableObject2D.h"

namespace uriVL
{
	/** A SimpleGraphicObject2D is essentially a DrawableObject2D madeup of a single
	 *	2D shape.  A GraphicObject2D is either a SimpleGraphicObject2D or a set
	 *	(implemented as a list) of GraphicObject2D.
	 *
	 *	Version history
	 *		- 2013-06-28 [jyh]		created the class
	 *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class SimpleGraphicObject2D : public DrawableObject2D
    {
    	public:
    		/**	Constructor
			 */
    		SimpleGraphicObject2D(void);
    		
			/**	Destructor
			 */
			~SimpleGraphicObject2D(void);
    		

    	private:

//            /**
//             *  Renders the drawable object at the depth and location specified.
//             *
//             *  A drawable object can be rendered with its valid data rectangle's upper-left
//             *  corner in the upper-left corner of the rendering window (shifted=false and
//             *  origin=NULL), or using the absolute coordinates of its valid data rectangle's
//             *  upper-left corner (shited=true and origin=NULL), or by setting its upper-left
//             *  corner at the location specified (origin != NULL).
//			 *
//			 *	All render functions are declared to be const.  The basic idea behind that is that
//			 *	rendering just gives a snapshot of the current state of the object but should not 
//			 *	really change the object.  According to this view, anything that needs to be 
//			 *	changed/updated at render time is really mutable.
//             *
//             *  This function must be overridden by child classes.
//             *
//             *  @param  scaledLeft  x coordinate of the object's upper-left corner when rendered scaled
//			 *	@param	scaledTop	y coordinate of the object's upper-left corner when rendered scaled
//			 *	@param	scaledRight x coordinate of the object's lower-right corner when rendered scaled
//			 *	@param	scaledBottom y coordinate of the object's lower-right corner when rendered scaled
//             *  @param  theZ    Depth at which this object is to be rendered.
//             *  @param  shifted     true if the object should be rendered at its true ImageRect
//             *                      position, false if its upper left corner should be at 0,0.
//             *  @param  origin      location of the object's upper-left corner in the rendering
//             */
//            virtual void render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
//								 float theZ, bool shifted, const ImagePoint* origin) const = 0;
//
//			
    };
}

#endif  //  URIVL_SIMPLE_GRAPHIC_OBJECT_2D_H
