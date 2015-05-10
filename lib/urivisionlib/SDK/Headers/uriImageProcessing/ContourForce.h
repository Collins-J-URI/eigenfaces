/*  NAME:
        ContourForce.h
 
    DESCRIPTION:
        ContourForce public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CONTOUR_FORCE_H
#define URIVL_CONTOUR_FORCE_H

#include <list>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "DrawableObject2D.h"
#include "ImagePoint_F.h"
#include "Vector2D_F.h"
#include "Contour_F.h"


namespace uriVL
{
	const float VECTORSCALE = 5.f;

    /** A contour force is a connected set of pixels that share a comon characteristic.
     *  Typically a contour force is detected after a labelling information (color regions,
     *  motion detection, etc.)
     *  @author Clifford Manzanillo Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ContourForce : public DrawableObject2D
    {
        public:
        
            /** Constructor
             */
            ContourForce(void);
            

            /** Destructor
             */
            ~ContourForce(void);

            /** Assigns a rendering color to the contour force's needles (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setNeedleColor(float red, float green, float blue);


            /** Assigns a rendering color to the contour force's needles (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setNeedleColor(unsigned char red, unsigned char green, unsigned char blue);


            /** Assigns a rendering color to the contour force's needles (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setNeedleColor(const float theColor[]);


            /** Assigns a rendering color to the contour force's needles (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setNeedleColor(const unsigned char theColor[]);

            /** Assigns a rendering color to the contour force's contour (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setColor(float red, float green, float blue);


            /** Assigns a rendering color to the contour force's contour (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setColor(unsigned char red, unsigned char green, unsigned char blue);


            /** Assigns a rendering color to the contour force's contour (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setColor(const float theColor[]);


            /** Assigns a rendering color to the contour force's contour (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setColor(const unsigned char theColor[]);
			
			
            /** Sets whether to draw a point at each vertex of the contour
             *  @param  drawPointCircle 
             */
            void setDrawPoints(bool drawPointCircle);


            /**
             *  Returns the contour's bounding rectangle.
             *
             *  Returns a pointer to the contour's bounding rectangle.
             *  
             *  Reminder: the ImageRect class uses the pixel-as-rectangle convention.
             *
             *  @return     pointer to the contour's bounding rectangle
             *
             *  @see    validRect_
             *  @see    boundRect_
             *  @see    getValidRect
             */
            const ImageRect* getBoundRect(void) const;

            /**
             *  Returns the contour's valid data rectangle.
             *
             *  Returns a pointer to the contour's valid data rectangle.
             *  
             *  Reminder: the ImageRect class uses the pixel-as-rectangle convention.
             *
             *  @return     pointer to the contour's valid data rectangle
             *
             *  @see    validRect_
             *  @see    boundRect_
             *  @see    getBoundRect
             */
            const ImageRect* getValidRect(void) const;
            

            /** Creates a new element in the force vector and assigns it to the values 
             *  @param  pointNumber The position that the new vector will have 
             *                          (other elements will be pushed up or down accordingly)
             *  @param  valueX  The value which will be assigned to forceX[pointNumber]  
             *  @param  valueY  The value which will be assigned to forceY[pointNumber]
            */
            void insertIntoForceVector(int pointIndex, float valueX, float valueY);
 
            /** Adds the force vector (element by element) to the class's force vector
             *  @param  cForce2  The force which will be added to the class's force vector
            */
            void addForce(ContourForce* cForce2);

            /** Sets the force's values to zero
             *  
             */
            void setForcesToZero(void);

            /** Adds the force vector to contourforce's mapped contour
             *  @return true or false based on whether contour has converged
             */
            bool applyForceToContour(void);
            
            /** Assigns a contour to which the force is currently applied to
             *  @param  contourIn A reference to the contour. 
             */
            void mapToContour(Contour_F *contourIn);

            /** Returns a read-only pointer to the contour force's geometric contour (list of points)
             *
             *  @return     a read-only pointer to the contour force's geometric contour
             */
            const Contour_F* getContour(void) const;

//            /** Returns a writable pointer to the contour force's geometric contour (list of points)
//             *
//             *  @return     a writable pointer to the contour force's geometric contour
//             */
//			Contour_F* getContour(ReadWriteStatus rw);

            /** Returns the X and Y component vector of the force
             *  @return force_ 
             */
            const std::list<Vector2D_F*>* getForce(void) const;
            
            /** Returns a writable pointer to the X and Y component vector of the force
             *  @return force_ 
             */
            std::list<Vector2D_F*>* getForce(ReadWriteStatus rw);

            
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
			
			/**	Indicates whether the contour should be rendered (when false, only the
			 *	needles are rendered). Used when several ContourForce objects
			 *	coincide on the same contour and only one needs to be rendered
			 *	@param	isRendered   true if the contour should be rendered
			 */
			void setRenderContour(bool isRendered);


        private:
            
            /** Stores a pointer to the contour which the force is mapped to 
             */
            Contour_F*	contourSubject2Force_;
            
            /** Stores the force which can be applied to counter points x and y coordinates
             */
            std::list<Vector2D_F*> force_;

            /** The rendering color for the force's "needles"
             */
            float needleColor_[4];
        
			/**	Should the contour be rendered.  Used when several ContourForce objects
			 *	coincide on the same contour and only one needs to be rendered
			 */
			bool renderContour_;

            /**
             *  Copy Constructor. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            ContourForce(const ContourForce& theObj);


            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const ContourForce& operator = (const ContourForce& theObj);

            /**
             *  Renders the contour at the depth and location specified.
             *
             *  A drawable object can be rendered with its valid data rectangle's upper-left
             *  corner in the upper-left corner of the rendering window (shifted=false and
             *  origin=NULL), or using the absolute coordinates of its valid data rectangle's
             *  upper-left corner (shited=true and origin=NULL), or by setting its upper-left
             *  corner at the location specified (origin != NULL).
             *
             *  This function must be overridden by child classes.
             *
             *  @param  scaledLeft  x coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledTop	y coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledRight x coordinate of the object's lower-right corner when rendered scaled
			 *	@param	scaledBottom y coordinate of the object's lower-right corner when rendered scaled
             *  @param  theZ    Depth at which this object is to be rendered.
             *  @param  shifted     true if the object should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @param  origin      location of the object's upper-left corner in the rendering
             */
            void render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
						 float theZ, bool shifted, const ImagePoint* origin) const;


            /** Calculates the object's bounding rect
			 *	@param	rect	the bounding box to update
             */
            void computeBoundingRect_(ImageRect& rect) const;


            /** Calculates the object's valid box
			 *	@param	rect	the bounding box to update
             */
            void computeValidRect_(ImageRect& rect) const;


            /** Computes the contour's centroid.
			 *	@param	cent	the centroid point to update
             */
			void computeCentroid_(ImagePoint_F& cent) const;

            /** Computes the object's center of mass by calculating the  
             *  average point
			 *	@param	cent	the center of mass point to update
             */
            void computeCenterOfMass_(ImagePoint_F& cent) const;


            /**
             *  Attempts (and fails ) to a new bounding rectangle for the contour force.
             *
             *  This function must be implemented since a contour force is a DrawableObject2D.
             *  However, it makes no sense forcing a change of bounding rectangle for
             *  a contour force (I may rethink this later as a form of clipping, though).
             *
             *  @param  x           horizontal coordinate of upper left corner
             *  @param  y           vertical coordinate of upper left corner
             *  @param  theWidth    desired width of the contour
             *  @param  theHeight   desired height of the contour
             */
            void setBoundRect_(int x, int y, int theWidth, int theHeight);


            /**
             *  Sets a new valid data rectangle for the contour force.
             *
             *  Not sure yet what this function should do
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            void setValidRect_(int x, int y, int theWidth, int theHeight);



//		friend class DrawableObject2D;


    };
}

#endif  //  contourSubject2Force_FORCE_H
