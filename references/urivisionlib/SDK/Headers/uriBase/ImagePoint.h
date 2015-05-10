/*  NAME:
        ImagePoint.h

    DESCRIPTION:
        ImagePoint public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_POINT_H
#define URIVL_IMAGE_POINT_H

#include <list>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//

namespace uriVL
{
    class Transformation2D;  
    
    /** Implements the ImagePoint class.
     *
     *  The ImagePoint class is based on the notion of a pixel as a dimensionless point.
     *  By opposition, QuickTime sees a pixel as a little square that hangs between the
     *  points (xLeft, yTop) and (xLeft+1, yTop+1).
     *
     *  Version history
     *      - 2004/04/27 [jyh]      fixed the pixel-as-point vs. pixel-as-rectangle
     *                              issue.
     *      - 2004/02/20 [jyh]      first implementation.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ImagePoint
    {
        public:

            /**
             *  Default Constructor.
             *  Creates an image point (pixel) using the pixel-as-point convention.
			 *	@param	hasSubpixelPrecision = true if the image point has subpixel precision
             */
            ImagePoint(bool hasSubpixelPrecision = false);

            /**
             *  Constructor.
             *  Creates an image point (pixel) using the pixel-as-point convention.
             *  @param  x        x coordinate of the point
             *  @param  y        y coordinate of the point
			 *	@param	hasSubpixelPrecision = true if the image point has subpixel precision
             */
            ImagePoint(int x, int y, bool hasSubpixelPrecision = false);

            /**
             *  Constructor.
             *  Creates an image point (pixel) using the pixel-as-point convention.
             *  @param  xy       coordinate array of the point
			 *	@param	hasSubpixelPrecision = true if the image point has subpixel precision
             */
            ImagePoint(const int xy[], bool hasSubpixelPrecision = false);

            /**
             *  Copy constructor.
             *  Creates a clone of the ImagePoint object passed as parameter.
             *  @param  thePt       pointer to the ImagePoint object to copy
             */
            ImagePoint(const ImagePoint* thePt);

            /**
             *  Copy constructor.
             *  Creates a copy of the ImagePoint object passed as parameter.
			 *	Note that the copy will have a new id.
             *  @param  thePt       reference to the ImagePoint object to copy
             */
            ImagePoint(const ImagePoint& thePt);

            /**
             *  Destructor
             */
            virtual ~ImagePoint(void);


            /**
             *  Equal operator.
             *  Created so that gcc will stop complaining. This Point object will
			 *	retain its own id.
             *  @param  thePt       reference to the ImagePoint object to copy
             */
            const ImagePoint& operator = (const ImagePoint& thePt);


            /**
             *  Returns the point's x coordinate
             *  @return     x coordinate of the point
             */
            int getX(void) const;

            /**
             *  Returns the point's y coordinate
             *  @return     y coordinate of the point
             */
            int getY(void) const;
			
			/**
			 *	Returns the point's id
			 *	@return		id of the point
			 */
			int getID(void) const;


            /**
             *  Assigns new x coordinate to the pixel
             *  @param  val     new x coordinate for the point
             */
            virtual void setX(int val);

            /**
             *  Assigns new y coordinate to the pixel
             *  @param  val     new y coordinate for the point
             */
            virtual void setY(int val);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  valY    new x coordinate for the point
             *  @param  valY    new y coordinate for the point
             */
            virtual void setCoordinates(int valX, int valY);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  xy       coordinate array of the point
             */
            virtual void setCoordinates(const int xy[]);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  thePt       pointer to the ImagePoint object to copy
             */
            virtual void setCoordinates(const ImagePoint* thePt);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  thePt       reference to the ImagePoint object to copy
             */
            virtual void setCoordinates(const ImagePoint& thePt);

            /**
             *  Shifts the existing x coordinate by the amount specified
             *  @param  dx     a positive or negative x displacement
             */
            virtual void shiftX(int dx);

            /**
             *  Shifts the existing y coordinate by the amount specified
             *  @param  dy     a positive or negative y displacement
             */
            virtual void shiftY(int dy);

            /**
             *  Shifts the existing x and y coordinates by the amount specified
             *  @param  dx     a positive or negative x displacement
             *  @param  dy     a positive or negative y displacement
             */
            virtual void translate(int dx, int dy);

            /** Applies a Transformation2D to this object
             *  @param  theTransf   the Transformation2D to apply to this object
             */
            virtual void applyTransformation(const Transformation2D* theTransf);

            /**	Rotates the object by the angle specified (in radians)
             *	If rotationCenter is null, then the centroid of the object is used
             *	as center of rotation, which for a point does not make much sense.
             *	@param	angle	the angle of the rotation (in radians)
             *	@param	rotCenter	the center of the rotation
             */
            virtual void rotate(float angle, const ImagePoint* rotCenter = NULL);


			/**	Indicates whether the image point has subpixel precision 
			 *	coordinates (is in fact an ImagePoint_F object)
			 *	@return		true if image point has subpixel precision
			 */
			virtual bool hasSubpixelPrecision(void) const;
			
			
			/**	Computes the distance between this point and the one passed as parameter
			 *	@param	pt	pointer to the point we want the distance to
			 *	@return		distance to the point
			 */
			float distanceTo(const ImagePoint* pt) {
				return distance(this, pt);
			}
			
			/**	Computes the distance between two points.
			 *	@param	pt1	pointer to the first point
			 *	@param	pt2	pointer to the second point
			 *	@return		distance between the two points
			 */
			static float distance(const ImagePoint* pt1, const ImagePoint* pt2);
			

        private:
            /**
             *  The point's x coordinate
             */
            int   x_;

            /**
             *  The point's y coordinate
             */
            int   y_;
			

			/**
			 *	The point's id
			 */
			int	  id_;

			/**	Class variable used to attribute a unique id to each Point object
			 */
			static int ptCounter_;
        
		

		friend class ImagePoint_F;

    };

	typedef	std::list<ImagePoint*>	ImagePointList;
	typedef	std::list<const ImagePoint*>	ConstImagePointList;
	
	
	/**	Free function to draw a point as a disk.
	 *	@param	cx		x coordinate of the circle's center
	 *	@param	cy		y coordinate of the circle's center
	 *	@param	rad		the circle's radius
	 *	@param	color	the filling color of the disk
	 */
	void drawPoint_disk(float cx, float cy, float rad, const float* color);
	 
	/**	Free function to draw a point as a square.
	 *	@param	cx		x coordinate of the circle's center
	 *	@param	cy		y coordinate of the circle's center
	 *	@param	rad		the square's "radius" (half side)
	 *	@param	color	the filling color of the disk
	 */
	void drawPoint_square(float cx, float cy, float rad, const float* color);
	 
	/**	Free function to draw a point as a diamond.
	 *	@param	cx		x coordinate of the circle's center
	 *	@param	cy		y coordinate of the circle's center
	 *	@param	rad		the diamond's "radius" (half diagonal)
	 *	@param	color	the filling color of the disk
	 */
	void drawPoint_diamond(float cx, float cy, float rad, const float* color);
	 
	
}   //  namespace uriVL

#endif  //  IMAGE_POINT_H
