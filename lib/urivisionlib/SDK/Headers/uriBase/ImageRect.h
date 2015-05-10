/*  NAME:
        ImageRect.h

    DESCRIPTION:
        ImageRect public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef URIVL_IMAGE_RECT_H
#define URIVL_IMAGE_RECT_H

#include <list>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "uriVisionLib.h"
#include "ImagePoint.h"

namespace uriVL
{
    #if URIVL_USE_QUICKTIME
    
        typedef Rect URIVL_Rect;
        
    #else
    
        /**  For platforms not supported by Quicktime, we need to define Rect.
         *  QuickTime sees a pixel as a little square that hangs between the points
         *  (xLeft, yTop) and (xLeft+1, yTop+1).  This pixel occuppies the QuickTime Rect
         *  {xLeft, yTop, xLeft+1, yTop+1}.  So the width and height of a QuickTime Rect
         *  are simply the differences between xBottom - xLeft and yBottom - yTop.
         */
        typedef struct URIVL_Rect
        {
            short   top;
            short   left;
            short   bottom;
            short   right;
            
        } URIVL_Rect; 

    #endif


    /** Implements the ImageRect class.
     *
     *  Wrapper class for an image rectangle structure.
     *
     *  Beware!  The ImageRect class and the Rect structure of QuickTime (and QuickDraw)
     *  are based on different notions of a pixel.
     *
     *  QuickTime sees a pixel as a little square that hangs between the points
     *  (xLeft, yTop) and (xLeft+1, yTop+1).  This pixel occuppies the QuickTime Rect
     *  {xLeft, yTop, xLeft+1, yTop+1}.  So the width and height of a QuickTime Rect
     *  are simply the differences between xBottom - xLeft and yBottom - yTop.
     *
     *  An ImageRect rectangle is based on the notion of a pixel as a point.  In this
     *  model, the rectangle {xLeft, yTop, xLeft+1, yTop+1} contains 4 points, not one!
     *  This makes more sense for image processing, but one should be very careful
     *  when going from Rect to ImageRect and back.
     *
     *  Important: The PixMap stored by the GWorld pads the rows of the raster to
     *  round their length in bytes to a multiple of 32.  Make sure not to confuse
     *  the nominal and effective bounding rectangles of the GraphWorld.
     *
     *  Version history
     *      - 2007/04/14 [jyh]      minor revision (added a bunch of const modifiers).
     *      - 2004/04/27 [jyh]      fixed the pixel-as-point vs. pixel-as-rectangle
     *                              issue.
     *      - 2004/02/20 [jyh]      first implementation.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ImageRect
    {
        public:

			/**	Default Constructor.  Creates an image rectangle with its upper-left
			 *	corner at (0, 0) and a width and height of 0 pixel
			 */
			ImageRect(void);
			
			
            /**
             *  Constructor.
             *  Creates an image rectangle using the pixel-as-point convention.
             *  @param  theLeft     x coordinate of left column pixels
             *  @param  theTop      y coordinate of top row pixels
             *  @param  theWidth    number of pixels along a row (pixel-as-point)
             *  @param  theHeight   number of pixels along a column (pixel-as-point)
             */
            ImageRect(int theLeft, int theTop, int theWidth, int theHeight);

            /**
             *  Constructor.
             *  Creates an image rectangle using the pixel-as-point convention.
             *  @param  theUpperLeft    pointer to the upper-left pixel
             *  @param  theLowerRight   pointer to the lower-right pixel (inclusive)
             */
            ImageRect(const ImagePoint* theUpperLeft, const ImagePoint* theLowerRight);

            /**
             *  Constructor.
             *  Creates an image rectangle using the pixel-as-point convention.
             *  @param  theUpperLeft    pointer to the upper-left pixel
             *  @param  theWidth        number of pixels along a row (pixel-as-point)
             *  @param  theHeight       number of pixels along a column (pixel-as-point)
             */
            ImageRect(const ImagePoint* theUpperLeft, int theWidth, int theHeight);

            /**
             *  Copy constructor.
             *  Creates an image rectangle using the pixel-as-point convention.
             *  @param  theRect     pointer to ImageRect to copy
             */
            ImageRect(const ImageRect* theRect);


            /**
             *  Copy constructor.
             *  Creates an image rectangle using the pixel-as-point convention.
             *  @param  theRect     reference to ImageRect object to copy
             */
            ImageRect(const ImageRect& theRect);


            /**
             *  Cross-convention constructor.
             *  Creates a pixel-as-point image rectangle using a pixel-as-rectangle
             *  rectangle (QuickTime rectangle).
             *  @param  theRect     pointer to a QuickTime rectangle
             */
            ImageRect(const URIVL_Rect qtRect);

 
			/**
             *  Destructor
             */
            ~ImageRect(void);
			
			
            /**
             *  Equal operator.  
			 *	Mostly defined so that gcc will stop bugging me about it.
             *  @param  theRect     reference to ImageRect object to copy
             */
			const ImageRect& operator = (const ImageRect& theRect);
			

            /**
             *  Returns the width of the rectangle.
             *  @return     width of the rectangle (pixels per row)
             */
            int getWidth(void) const;

            /**
             *  Returns the height of the rectangle.
             *  @return     height of the rectangle (pixels per column)
             */
            int getHeight(void) const;

            /**
             *  Returns the minimum x coordinate of the rectangle
             *  @return     x coordinate of the upper-left pixel
             */
            int getLeft(void) const;

            /**
             *  Returns the maximum x coordinate of the rectangle
             *  @return     x coordinate of the upper-right pixel
             */
            int getRight(void) const;

            /**
             *  Returns the minimum y coordinate of the rectangle
             *  @return     y coordinate of the upper-left pixel
             */
            int getTop(void) const;

            /**
             *  Returns the maximum y coordinate of the rectangle
             *  @return     y coordinate of the lower-right pixel
             */
            int getBottom(void) const;

            /**
             * returns the coordinates of the upper left corner of the object
             * 
             * @return		coordinates of the upper left corner
             */
             const ImagePoint*  getUpperLeft(void) const;

            /**
             *  Returns the area of the rectangle
             *  @return     area (number of pixels) of the rectangle
             */
            long getArea(void) const;


            /**
             *  Sets the origin of the rectangle.
             *  A move-to operation.
             *  @param  x       new x coordinate asigned to the upper-left pixel
             *  @param  y       new y coordinate asigned to the upper-left pixel
             */
            void setOrigin(int x, int y);

			/**	Sets the center of the rectangle.  The rectangle's width and height are preserved
			 *	but it is translated so that its center is at the coordinates indicated.  The rectangle's
			 *	upper-left corner is moved to (cx - width/2, cy - height/2)
			 *	@param	cX	new x coordinate of the rectangle's center
			 *	@param	cY	new y coordinate of the rectangle's center
			 */
			void setCenter(int cX, int cY);
			
            /**
             *  Sets the origin of the rectangle.
             *  A move-by operation.
             *  @param  dx      x displacement asigned to the upper-left pixel
             *  @param  dy      y displacement asigned to the upper-left pixel
             */
            void translate(int dx, int dy);

           /**
            *   Sets the origin and size of the rectangle.
            *   @param  theRect ImageRect object to copy the information from
            */
            void setRect(const ImageRect* theRect);

            /**
             *   Sets the origin and size of the rectangle.
             *   @param  left    new x coordinate asigned to the upper-left pixel
             *   @param  top     new y coordinate asigned to the upper-left pixel
             *   @param  width   new width asigned to the rectangular
             *   @param  height  new height asigned to the rectangular
             */
            void setRect(int theLeft, int theTop, int theWidth, int theHeight);

            /**
             *   Sets the origin and size of the rectangle.
             *   @param  topLeft     new ImagePoint asigned to the upper-left pixel
             *   @param  bottomRight new ImagePoint asigned to the bottom-right pixel
             */
            void setRect(const ImagePoint*  topLeft, const ImagePoint*  bottomRight);

            /**
             *   Sets the origin and size of the rectangle.
             *   @param  topLeft     new ImagePoint asigned to the upper-left pixel
             *   @param  width   new width asigned to the rectangular
             *   @param  height  new height asigned to the rectangular
             */
            void setRect(const ImagePoint*  topLeft, int theWidth, int theHeight);

            /**
             *  Indicates whether this rectangle and the one received as parameter overlap.
             *  @param   theRect    the rectangle to test agains this rectangle
             *  @return  true if the two rectangles overlap, false otherwise
             */
            bool overlaps(const ImageRect* theRect) const;

            /**
             *  Indicates whether this rectangle and the one received as parameter overlap.
             *  @param  theLeft     x coordinate of left column pixels
             *  @param  theTop      y coordinate of top row pixels
             *  @param  theWidth    number of pixels along a row (pixel-as-point)
             *  @param  theHeight   number of pixels along a column (pixel-as-point)
             *  @return  true if the two rectangles overlap, false otherwise
             */
            bool overlaps(int theLeft, int theTop, int theWidth, int theHeight) const;

            /**
             *  Indicates whether this rectangle and the one received as parameter coincide.
             *  @param   theRect    the rectangle to test agains this rectangle
             *  @return  true if the two rectangles coincide, false otherwise
             */
            bool coincidesWith(const ImageRect* theRect) const;

            /**
             *  Indicates whether this rectangle and the one received as parameter coincide.
             *  @param  theLeft     x coordinate of left column pixels
             *  @param  theTop      y coordinate of top row pixels
             *  @param  theWidth    number of pixels along a row (pixel-as-point)
             *  @param  theHeight   number of pixels along a column (pixel-as-point)
             *  @return  true if the two rectangles coincide, false otherwise
             */
            bool coincidesWith(int theLeft, int theTop, int theWidth, int theHeight) const;

            /**
             *  Indicates whether this rectangle contains the one received as parameter.
             *  @param   theRect    the rectangle to test agains this rectangle
             *  @param   padding    the difference between the rectangle and search rectangle
             *  @return  true if theRect is contained within this rectangle, false otherwise
             */
            bool contains(const ImageRect* theRect, int padding=0) const;

            /**
             *  Indicates whether this rectangle contains the one received as parameter.
             *  @param   theRect    the rectangle to test agains this rectangle
             *  @param  theLeft     x coordinate of left column pixels
             *  @param  theTop      y coordinate of top row pixels
             *  @param  theWidth    number of pixels along a row (pixel-as-point)
             *  @param  theHeight   number of pixels along a column (pixel-as-point)
             *  @param   padding    the difference between the rectangle and search rectangle
             *  @return  true if theRect is contained within this rectangle, false otherwise
             */
            bool contains(int theLeft, int theTop, int theWidth, int theHeight, int padding=0) const;

            /** Checks whether the point passed as parameter belongs to the rectangle.
             *  @param  pt  		the point to check
             *  @param  padding    	the difference between the rectangle and search rectangle
             *  @return <code>true</code> if pt is inside this rectangle
             */
			bool contains(const ImagePoint* pt, int padding=0) const;

            /** Checks whether the point passed as parameter belongs to the rectangle.
             *  @param  x   		x coordinate of the point to check
             *  @param  y   		y coordinate of the point to check
             *  @param  padding    	the difference between the rectangle and search rectangle
             *  @return <code>true</code> if pt is inside this rectangle
             */
			bool contains(int x, int y, int padding=0) const;

            /**
             *  Indicates whether this rectangle is contained by the one received as parameter.
             *  @param   theRect    the rectangle to test agains this rectangle
             *  @return  true if theRect contains this rectangle, false otherwise
             */
            bool isContainedBy(const ImageRect* theRect) const;

            /**
             *  Indicates whether this rectangle is contained by the one received as parameter.
             *  @param  theLeft     x coordinate of left column pixels
             *  @param  theTop      y coordinate of top row pixels
             *  @param  theWidth    number of pixels along a row (pixel-as-point)
             *  @param  theHeight   number of pixels along a column (pixel-as-point)
             *  @return  true if theRect contains this rectangle, false otherwise
             */
            bool isContainedBy(int theLeft, int theTop, int theWidth, int theHeight) const;

            /**
             *  Indicates whether this rectangle has its upper left corner at (0, 0)
             *  @return  true if this rectangle has its upper left corner at (0, 0)
             */
            bool upperLeftIsAtOrigin(void) const;


            /**
             *  Adds n elements all around the rectangle.
             *  After this operation, the rowHigh and colHigh have been incremented by n
             *  while the rowLow and colLow have been decremented by n.
             *
             *  @param  n   amount to add all around
             */
            void addAllAround(int n);


            /**
             *  Removes n elements all around the rectangle.
             *  After this operation, the rowHigh and colHigh have been decremented by n
             *  while the rowLow and colLow have been incremented by n.  The operation
             *  fails (throws an exception) if either dimension of the resized rectangle
             *  is negative.
             *
             *  @param  n   amount to add all around
             */
            void removeAllAround(int n);


            /**
             *  Returns the intersection of the two rectangles received as parameters
             *  @param  rect1   first input rectangle
             *  @param  rect2   second input rectangle
             *  @return new rectangle  = intersection of rect1 and rect2
             */
            static ImageRect* intersection(const ImageRect* rect1, const ImageRect* rect2);


        private:
            /**
             *  QuickTime rectangle.
             *  Underlying QuickTime rectangle, using the pixel-as-rectangle convention.
             */
            URIVL_Rect rect_;

            /**
             *  Upper-left point of the rectangle.
             *  Redundant information, using the pixel-as-point convention.
             */
            ImagePoint  upperLeft_;


            /**
             *  Lower-right point of the rectangle.
             *  Redundant information, using the pixel-as-point convention.
             */
            ImagePoint  lowerRight_;

            /** Returns a reference to the private QuickTime rectangle structure of this
             *  ImageRect objet.
             *  This function is private, with GraphWorld as sole friend class, because
             *  I can't see any reason why anyone should want to access this data except
             *  for a CopyBits.
             *  @return     reference to private Rect structure
             */
            const URIVL_Rect *getRectQT_(void) const;


        friend class GraphWorld;
        #if URIVL_USE_QUICKTIME
            friend class _GraphWorld_QT;
        #else
            friend class _GraphWorld_gen;
        #endif
        
    };

    typedef std::list<ImageRect*> RectList;
    typedef std::list<const ImageRect*> ConstRectList;

}   //  namespace uriVL

#endif  //  IMAGE_RECT_H
