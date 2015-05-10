/*  NAME:
        ImagePoint_F.h

    DESCRIPTION:
        ImagePoint_F public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_POINT_F_H
#define URIVL_IMAGE_POINT_F_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

#include "ImagePoint.h"

namespace uriVL
{
    /** Implements the ImagePoint_F class.
     *
     *  An ImagePoint_F object represents a dimensionless mathematical point.  This class
     *  should be used for all applications requiring subpixel precision.
     *
     *  Version history
     *      - 2004/11/20 [jyh]      first implementation.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ImagePoint_F : public ImagePoint
    {
        public:

            /**
             *  Default Constructor.
             *  Creates an image point (pixel) using the pixel-as-point convention.
             */
            ImagePoint_F(void);

            /**
             *  Constructor.
             *  Creates an image point (pixel) using the pixel-as-point convention.
             *  @param  theX        x coordinate of the pixel
             *  @param  theY        y coordinate of the pixel
             */
            ImagePoint_F(float theX, float theY);

            /**
             *  Constructor.
             *  Creates an image point (pixel) using the pixel-as-point convention.
             *  @param  theXY       coordinate array of the pixel
             */
            ImagePoint_F(const float theXY[]);

            /**
             *  Copy constructor.
             *  Creates a clone of the ImagePoint object passed as parameter.
             *  @param  thePt       pointer to the ImagePoint_F object to clone
             */
            ImagePoint_F(const ImagePoint_F* thePt);

            /**
             *  Copy constructor.
             *  Creates a clone of the ImagePoint object passed as parameter.
             *  @param  thePt       pointer to the ImagePoint object to clone
             */
            ImagePoint_F(const ImagePoint* thePt);

            /**
             *  Copy constructor.
             *  Creates a clone of the ImagePoint object passed as parameter.
             *  @param  thePt       reference to the ImagePoint object to copy
             */
            ImagePoint_F(const ImagePoint_F& thePt);

            /**
             *  Destructor
             */
            ~ImagePoint_F(void);

            /**
             *  Equal operator.
             *  Created so that gcc will stop complaining.
             *  @param  thePt       reference to the ImagePoint object to copy
             */
            const ImagePoint_F& operator = (const ImagePoint_F& thePt);

			/**	Indicates that the image point has subpixel precision 
			 *	coordinates
			 *	@return		true if image point has subpixel precision
			 */
			bool hasSubpixelPrecision(void) const
			{
				return true;
			}
			

            /**
             *  Returns the pixel's x coordinate
             *  @return     x coordinate of the pixel
             */
            float getXF(void) const;

            /**
             *  Returns the pixel's y coordinate
             *  @return     y coordinate of the pixel
             */
            float getYF(void) const;

            /**
             *  Assigns new x coordinate to the pixel
             *  @param  val     new x coordinate for the pixel
             */
            void setX(int val);

            /**
             *  Assigns new x coordinate to the pixel
             *  @param  val     new x coordinate for the pixel
             */
            void setX(float val);

            /**
             *  Assigns new y coordinate to the pixel
             *  @param  val     new y coordinate for the pixel
             */
            void setY(int val);

            /**
             *  Assigns new y coordinate to the pixel
             *  @param  val     new y coordinate for the pixel
             */
            void setY(float val);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  valY    new x coordinate for the pixel
             *  @param  valY    new y coordinate for the pixel
             */
            void setCoordinates(int valX, int valY);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  theXY       coordinate array of the pixel
             */
            void setCoordinates(const int* xy);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  valY    new x coordinate for the pixel
             *  @param  valY    new y coordinate for the pixel
             */
            void setCoordinates(float valX, float valY);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  theXY       coordinate array of the pixel
             */
            void setCoordinates(const float* xy);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  thePt       pointer to the ImagePoint object to copy
             */
            void setCoordinates(const ImagePoint* thePt);

            /**
             *  Assigns new coordinates to the pixel
             *  @param  thePt       reference to the ImagePoint object to copy
             */
            void setCoordinates(const ImagePoint& thePt);

            /**
             *  Shifts the existing x and y coordinates by the amount specified
             *  @param  dx     a positive or negative x displacement
             *  @param  dy     a positive or negative y displacement
             */
            void translate(int dx, int dy);

            /**
             *  Shifts the existing x and y coordinate by the amount specified
             *  @param  dx     a positive or negative x displacement
             *  @param  dy     a positive or negative y displacement
             */
            void translate(float dx, float dy);

            /**
             *  Shifts the existing x coordinate by the amount specified
             *  @param  dx     a positive or negative x displacement
             */
            void shiftX(int dx);

            /**
             *  Shifts the existing x coordinate by the amount specified
             *  @param  dx     a positive or negative x displacement
             */
            void shiftX(float dx);

            /**
             *  Shifts the existing y coordinate by the amount specified
             *  @param  dy     a positive or negative y displacement
             */
            void shiftY(float dy);


            /**
             *  Shifts the existing y coordinate by the amount specified
             *  @param  dy     a positive or negative y displacement
             */
            void shiftY(int dy);

            /** Applies a Transformation2D to this object
             *  @param  theTransf   the Transformation2D to apply to this object
             */
            void applyTransformation(const Transformation2D* theTransf);

            /**	Rotates the object by the angle specified (in radians)
             *	If rotationCenter is null, then the centroid of the object is used
             *	as center of rotation
             *	@param	angle	the angle of the rotation (in radians)
             *	@param	rotCenter	the center of the rotation
             */
            void rotate(float angle, const ImagePoint* rotCenter = NULL);


        private:
            /**
             *  The pixel's x coordinate
             */
            float   xf_;

            /**
             *  The pixel's y coordinate
             */
            float   yf_;
    };
	
	typedef	std::list<ImagePoint*>	ImagePoint_FList;
	typedef	std::list<const ImagePoint*>	ConstImagePoint_FList;


}   //  namespace uriVL

#endif  //  IMAGE_POINT_F_H
