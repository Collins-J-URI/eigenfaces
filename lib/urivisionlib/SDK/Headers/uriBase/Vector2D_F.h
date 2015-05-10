/*  NAME:
        Vector2D_F.h

    DESCRIPTION:
        Vector2D_F public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VECTOR_2D_F_H
#define URIVL_VECTOR_2D_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImagePoint_F.h"
#include "Vector2D.h"

namespace uriVL
{
    /** Implements the Vector2D_F class.
     *
     *  A Vector2D_F should typically store an image displacement or an edge of a contour
     *
     *  Version history
     *      - 2007/11/02 [jyh]      first implementation.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT Vector2D_F : public Vector2D
    {
        public:

            /**
             *  Default Constructor.
             *  Creates an image point (pixel) using the pixel-as-point convention.
             */
            Vector2D_F(void);

            /**
             *  Constructor.
             *  Creates and initializes an image displacement.
             *  @param  dx        x coordinate of the vector
             *  @param  dy        y coordinate of the vector
             */
            Vector2D_F(float dx, float dy);

            /**
             *  Constructor.
             *  Creates and initializes an image displacement.
             *  @param  dxy       coordinate array of the vector
             */
            Vector2D_F(const float dxy[]);

            /**
             *  Copy constructor.
             *  Creates a clone of the Vector2D object passed as parameter.
             *  @param  vec       pointer to the Vector2D object to clone
             */
            Vector2D_F(const Vector2D* vec);

            /**
             *  Constructor.
             *  Creates and initializes an image displacement joining two image points.
             *  @param  pt1       start point of the vector
             *  @param  pt2       end point of the vector
             */
            Vector2D_F(const ImagePoint* pt1, const ImagePoint* pt2);

            /**
             *  Destructor
             */
            ~Vector2D_F(void);

            /**
             *  Returns the vector's x coordinate
             *  @return     x coordinate of the vector
             */
            float getXF(void) const;

            /**
             *  Returns the vector's y coordinate
             *  @return     y coordinate of the vector
             */
            float getYF(void) const;

            /**
             *  Assigns new x coordinate to the vector
             *  @param  val     new x coordinate for the vector
             */
            void setX(int val);

            /**
             *  Assigns new x coordinate to the vector
             *  @param  val     new x coordinate for the vector
             */
            void setX(float val);

            /**
             *  Assigns new y coordinate to the vector
             *  @param  val     new y coordinate for the vector
             */
            void setY(int val);

            /**
             *  Assigns new y coordinate to the vector
             *  @param  val     new y coordinate for the vector
             */
            void setY(float val);

            /**
             *  Assigns new coordinates to the vector
             *  @param  valY    new x coordinate for the vector
             *  @param  valY    new y coordinate for the vector
             */
            void setCoordinates(int valX, int valY);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  theXY       coordinate array of the pixel
             */
            void setCoordinates(const int xy[]);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  valY    new x coordinate for the vector
             *  @param  valY    new y coordinate for the vector
             */
            void setCoordinates(float valX, float valY);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  theXY       coordinate array of the vector
             */
            void setCoordinates(const float xy[]);

            /**
             *  Assigns new coordinates to the vector
             *  @param  theVec       pointer to the Vector2D object to copy
             */
            void setCoordinates(const Vector2D* theVec);

            /**
             *  Assigns new coordinates to the vector
             *  @param  pt1       start point of the vector
             *  @param  pt2       end point of the vector
             */
            void setCoordinates(const ImagePoint* pt1, const ImagePoint* pt2);


			/**	Indicates that this object has subpixel precision (overrides parent class's
			 *	virtual function).
			 */
			bool hasSubpixelPrecision(void) const
			{
				return true;
			}

            /** Applies a Rotation2D to this object
             *  @param  theTransf   the Transformation2D to apply to this object
             */
            void applyRotation(const Rotation2D* theRot);

            /**	Rotates the object by the angle specified (in radians)
             *	@param	angle	the angle of the rotation (in radians)
             */
            void rotate(float angle);


        private:
            /**
             *  The vector's x coordinate
             */
            float   xf_;

            /**
             *  The vector's y coordinate
             */
            float   yf_;
    };

}   //  namespace uriVL

#endif  //  VECTOR_2D_F_H
