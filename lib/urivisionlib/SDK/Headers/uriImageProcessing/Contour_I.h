/*  NAME:
        Contour_I.h
 
    DESCRIPTION:
        Contour_I public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CONTOUR_I_H
#define URIVL_CONTOUR_I_H

#include <cmath>
#include <list>
//
#include "ImagePoint.h"
#include "RigidTransformation2D.h"
#include "Contour.h"

namespace uriVL
{
	//	Forward declaration
	class Contour_F;
	

    /** A Countour_I is a set of ImagePoint objects (intetger coordinates) which compose a closed curve.
     *
     *  @author Clifford Manzanillo, jyh	Department of Computer Science and Statistics,
     *										University of Rhode Island
     */
    class URIVL_EXPORT Contour_I : public Contour
    {
        public:
        
            /** default constructor
             */
            Contour_I(void);
            
			/**
             *  Copy Constructor.
             *
             *  @param      theObj   reference to the object to copy
             */
            Contour_I(const Contour_I& ct);

            /** Creates a contour with the number of vertices specified
             *  @param  theNumPts   number of points of the contour to create
             */
            Contour_I(int theNumPts);

            /** Creates a contour and initializes it with the points recived as parameters
             *  @param  n   number of points of the contour to create
             *  @param  pts array of points to initiakize thed contour with
             */
            Contour_I(int n, const ImagePoint*const* pts);

            /** Creates a contour and initializes it with the points recived as parameters
             *  @param  ptsList ImagePointList
             */
            Contour_I(const ImagePointList* ptsList);

            /** Destructor
             */
            ~Contour_I(void);
                    
            /** Copies this contour into the one received as parameter
             *  
             *  @param  contourOut  The contour to copy this one into
             */
            void copyInto(Contour* contourOut) const;
            
            /** Creates a new imagepoint at the iterator position
             *	@param	xVal	x coordinate of the new point
             *	@param	yVal	y coordinate of the new point
             */
            void insertPoint(int position, float xVal, float yVal);

			/**	Indicates that the image point has subpixel precision 
			 *	coordinates
			 *	@return		true if image point has subpixel precision
			 */
			bool hasSubpixelPrecision(void) const
			{
				return false;
			}

            /** Gives the number of points along the contour
             *
             *  @return     number of points along the contour
             */
            int getNbPoints(void) const;
            
            /** Returns the contour's private list of image points
             */
            const std::list<ImagePoint*>* getPoints(void) const;

            /** Returns a writable version of the contour's private list of image points
             *  @param   rw     R_W_ACCESS is the only acceptable value
             */
            std::list<ImagePoint*>* getPoints(ReadWriteStatus rw);


            /** Computes the edges along the closed contour and returns it as a
             *  list of image points.
             *  <ul><li> Admittedly, it is not pretty to use an ImagePoint object
             *          to store a vector.  This will have to be changed in the 
             *          not-too-distant future;
             *      <li> This function returns private data of the contour object.
             *  Do not modify it.
             *  </ul>
             *  @see    computeEdgeVectorList
             *  @see    edgeList_
             */
            const std::list <ImagePoint*> *getEdgeList(void);


            /** Checks whether the point passed as parameter is inside the contour
             *  @param  x   x coordinate of the point to check
             *  @param  y   y coordinate of the point to check
             *  @return <code>true</code> if pt is inside this contour
             */
            bool contains(int x, int y) const;

            /** Checks whether the point passed as parameter is inside the contour
             *  @param  pt  the point to check
             *  @return <code>true</code> if pt is inside this contour
             */
            bool contains(const ImagePoint_F* pt) const;

            /** Checks whether the point passed as parameter is inside the contour
             *  @param  pt  the point to check
             *  @return <code>true</code> if pt is inside this contour
             */
            bool contains(const ImagePoint* pt) const;

            /** Checks whether the point passed as parameter is inside the contour
             *  @param  x   x coordinate of the point to check
             *  @param  y   y coordinate of the point to check
             *  @return <code>true</code> if pt is inside this contour
             */
            bool contains(float x, float y) const;

            /** Checks whether this contour is completely inside the one passed as parameter.
             *  @param  contour  the contour to check against
             *  @return <code>true</code> if this contour is inside theContour
             */
            bool isInside(const Contour_I* pt) const;

            /** Checks whether this contour is completely inside the one passed as parameter.
             *  @param  contour  the contour to check against
             *  @return <code>true</code> if this contour is inside theContour
             */
            bool isInside(const Contour_F* pt) const;

            /** Checks whether this contour completely contains the one passed as parameter.
             *  @param  contour  the contour to check against
             *  @return <code>true</code> if this contour contains theContour
             */
            bool contains(const Contour_I* pt) const;

            /** Checks whether this contour completely contains the one passed as parameter.
             *  @param  contour  the contour to check against
             *  @return <code>true</code> if this contour contains theContour
             */
            bool contains(const Contour_F* pt) const;

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

            /** Translates the contour by the amounts specified
             *  @param  dx  x translation to apply
             *  @param  dy  y translation to apply
             */
            void translate(int dx, int dy);

            /** Translates the contour by the amounts specified
             *  @param  dx  x translation to apply
             *  @param  dy  y translation to apply
             */
            void translate(float dx, float dy);

            /** Set the coordinates of the contour's vertex whose index is provided.
             *  Fails if the index is larger that the number of vertices of the contour.
             *  @param  index   index of the vertex to modify
             *  @param  x   new x coordinate of the vertex
             *  @param  y   new y coordinate of the vertex
             */
            void setPoint(int index, int x, int y);
            
            /** Set the coordinates of the contour's vertex whose index is provided.
             *  Fails if the index is larger that the number of vertices of the contour.
             *  @param  index   index of the vertex to modify
             *  @param  x   new x coordinate of the vertex
             *  @param  y   new y coordinate of the vertex
             */
            void setPoint(int index, float x, float y);

        private:
        
            /** The contour proper, stored as a list of image points
             */
            std::list <ImagePoint*>  contourList_;
                    
            /** List of edges of the contour
             */
            mutable std::list <ImagePoint* >  edgeList_;

            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const Contour_I& operator = (const Contour_I& theObj);


            /** Initializes the contour along the edges of a rectangular box
             *
             *  @param  rect    rectangular box 
             *  @param  padding distance away from the box at which to initialize the
             *                  contour (positive: outer padding; negative: inner padding)
             */
            void initializeBox_(const ImageRect& rect, int padding = 0);



            /** Initializes the contour about a rectangular box specified by a Rectangle .  This function wil be called by
			 *	the parent class Contour and it can be safely assumed that the 
             *
			 *	@param	nbPts		number of points to create along the contour
             *  @param  upperLeft	coordinates of the box's upper-left corner
             */
            void initializeBox_(int nbPts, const ImagePoint& upperLeft,
								const ImagePoint& upperRight, const ImagePoint& lowerLeft,
								const ImagePoint& lowerRight);




            /** Initializes the contour along the edges of a oval
             *
			 *	@param	nbPts		number of points to create along the contour
			 *	@param	contourType	the type of contour to initialize this one to
             *  @param  center		coordinates of the contour's center 
             *  @param  width		width of the box or ellipse
             *  @param  height		height of the box or ellipse
			 *	@param	angle		orientation of the contour
             */
            void initializeOval_(int nbPts, const ImagePoint& center, float width, float height, 
								 float angle = 0.f);


            /** Computes the distances between image points.
             *  @see    getInterPointDistances
             *  @see    ptDistance_
             */
            void computeInterPointDistances_(void) const;

            /** Computes the edges along the closed contour and returns it as a
             *  list of image points.
             *  <ul><li> Admittedly, it is not pretty to use an ImagePoint object
             *          to store a vector.  This will have to be changed in the 
             *          not-too-distant future;
             *      <li> This function returns private data of the contour object.
             *  Do not modify it.
             *  </ul>
             *  @see    getEdgeList
             *  @see    edgeList_
             */
            void computeEdgeVectorList_(void) const;
            
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
             *  Renders the contour at the depth and location specified.
             *
             *  A drawable object can be rendered with its valid data rectangle's upper-left
             *  corner in the upper-left corner of the rendering window (shifted=false and
             *  origin=NULL), or using the absolute coordinates of its valid data rectangle's
             *  upper-left corner (shited=true and origin=NULL), or by setting its upper-left
             *  corner at the location specified (origin != NULL).
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


    };
}

#endif  //  CONTOUR_I_H
