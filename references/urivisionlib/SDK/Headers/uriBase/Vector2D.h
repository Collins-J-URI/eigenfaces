/*  NAME:
        Vector2D.h

    DESCRIPTION:
        Vector2D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VECTOR_2D_H
#define URIVL_VECTOR_2D_H

#include <list>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImagePoint.h"

namespace uriVL
{
    class Rotation2D;    
    
    /** Implements the Vector2D class.
     *
     *  A Vector2D should typically store an image displacement or an edge of a contour
     *
     *  Version history
     *      - 2007/11/02 [jyh]      first implementation.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT Vector2D
    {
        public:

            /**
             *  Default Constructor.
             *  Creates a (0, 0) image displacement.
             */
            Vector2D(void);

            /**
             *  Constructor.
             *  Creates and initializes an image displacement.
             *  @param  dX        x displacement
             *  @param  dY        y displacement
             */
            Vector2D(int dx, int dy);

            /**
             *  Constructor.
             *  Creates and initializes an image displacement.
             *  @param  dxy       displacement vector
             */
            Vector2D(const int dxy[]);

            /**
             *  Copy constructor.
             *  Creates a clone of the Vector2D object passed as parameter.
             *  @param  displ       pointer to the Vector2D object to copy
             */
            Vector2D(const Vector2D* displ);

            /**
             *  Constructor.
             *  Creates and initializes an image displacement joining two image points.
             *  @param  pt1       start point of the vector
             *  @param  pt2       end point of the vector
             */
            Vector2D(const ImagePoint* pt1, const ImagePoint* pt2);

            /**
             *  Destructor
             */
            virtual ~Vector2D(void);


            /**
             *  Returns the pixel's x coordinate
             *  @return     x coordinate of the pixel
             */
            int getX(void) const;

            /**
             *  Returns the pixel's y coordinate
             *  @return     y coordinate of the pixel
             */
            int getY(void) const;


            /**
             *  Assigns new x coordinate to the vector
             *  @param  val     new x coordinate for the pixel
             */
            virtual void setX(int val);

            /**
             *  Assigns new y coordinate to the vector
             *  @param  val     new y coordinate for the pixel
             */
            virtual void setY(int val);

            /**
             *  Assigns new coordinates to the vector
             *  @param  valY    new x coordinate for the pixel
             *  @param  valY    new y coordinate for the pixel
             */
            virtual void setCoordinates(int valX, int valY);

            /**
             *  Assigns new coordinates to the vector
             *  @param  dxy       coordinate array of the pixel
             */
            virtual void setCoordinates(const int dxy[]);

            /**
             *  Assigns new coordinates to the vector
             *  @param  theVect       pointer to the Vector2D object to copy
             */
            virtual void setCoordinates(const Vector2D* theVect);

            /**
             *  Assigns new coordinates to the vector
             *  @param  pt1       start point of the vector
             *  @param  pt2       end point of the vector
             */
            virtual void setCoordinates(const ImagePoint* pt1, const ImagePoint* pt2);


			/**	Indicates whether this object has subpixel precision.  Should be overridden
			 *	by a subclass that has floating point precision
			 */
			virtual bool hasSubpixelPrecision(void) const
			{
				return false;
			}

            /** Applies a Rotation2D to this object
             *  @param  theTransf   the Transformation2D to apply to this object
             */
            virtual void applyRotation(const Rotation2D* theRot);

            /**	Rotates the object by the angle specified (in radians)
             *	@param	angle	the angle of the rotation (in radians)
             */
            virtual void rotate(float angle);



        protected:
            /**
             *  The vector's x coordinate
             */
            int   x_;

            /**
             *  The vector's y coordinate
             */
            int   y_;

        
    };

	typedef	std::list<Vector2D*>	Vector2DList;
	typedef	std::list<const Vector2D*>	ConstVector2DList;


}   //  namespace uriVL

#endif  //  VECTOR_2D_H
