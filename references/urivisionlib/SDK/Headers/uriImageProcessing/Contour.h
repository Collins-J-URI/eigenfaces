/*  NAME:
        Contour.h
 
    DESCRIPTION:
        Contour public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CONTOUR_H
#define URIVL_CONTOUR_H

#include <cmath>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RigidTransformation2D.h"
#include "ImageFeature.h"
#include "ImagePoint_F.h"
#include "ImageRect.h"

const int DRAW_POINT_SIDES = 8;

namespace uriVL
{
	/**	Types of initial contour shapes for an active contour
	 */
	typedef enum ContourInitializer {
											ELLIPTIC_CONTOUR = 0,
											BOX_CONTOUR
	} ContourInitializer;
	
	
	
    class Contour_I;
    class Contour_F;
    
    /** Set of ImagePoint objects which form a closed curve
     *
	 *	@see Blob
	 *	@see Contour_I
	 *	@see Contour_F
	 *
     *  @author Clifford Manzanillo, jyh	Department of Computer Science and Statistics,
     *										University of Rhode Island
     */
    class URIVL_EXPORT Contour : public ImageFeature
    {
        public:

            /**  Destructor
             */
            virtual ~Contour(void);
                    

			/**	Indicates whether the image points defining this contour have subpixel precision 
			 *	coordinates
			 *	@return		true if this contour's points have subpixel precision
			 */
			virtual bool hasSubpixelPrecision(void) const = 0;


            /** Initializes the contour about a rectangular box
             *
			 *	@param	nbPts		number of points to create along the contour
			 *	@param	contourType	the type of contour to initialize this one to
             *  @param  rect		rectangular box 
             *  @param  padding		signed distance away from the box at which to initialize the
             *						contour (negative distance creates the contour inside the box)
             */
            void initialize(int nbPts, ContourInitializer contourType,
							const ImageRect* rect, int padding = 0);


            /** Initializes the contour along the edges of a rectangular box
             *
			 *	@param	nbPts		number of points to create along the contour
			 *	@param	contourType	the type of contour to initialize this one to
             *  @param  center		coordinates of the contour's center 
             *  @param  width		width of the box or ellipse
             *  @param  height		height of the box or ellipse
			 *	@param	angle		orientation of the contour
             */
            void initialize(int nbPts, ContourInitializer contourType,
							const ImagePoint& center, float width, float height, 
							float angle = 0.f);


            /** Copies this contour into the one received as parameter
             *  
             *  @param  contourOut  The contour to copy this one into
             */
            virtual void copyInto(Contour* contourOut) const = 0;
            
            
            /** Gives the number of points along the contour
             *
             *  @return     number of points along the contour
             */
            virtual int getNbPoints(void) const = 0;
 
			           
            /** Computes the distances between image points and returns it as a
             *  list of float numbers.
             *  Note that this function returns private data of the contour object.
             *  Do not modify it.
             *  @see    calculateInterPointDistances
             *  @see    ptDistance_
             */
            const std::list<float>* getInterPointDistances(void) const;
            

            /** Assigns a rendering color to the contour's vertices (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setVertexColor(float red, float green, float blue);


            /** Assigns a rendering color to the contour  (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setVertexColor(unsigned char red, unsigned char green, unsigned char blue);


            /** Assigns a rendering color to the contour (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setVertexColor(const float theColor[]);


            /** Assigns a rendering color to the contour (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setVertexColor(const unsigned char theColor[]);

            /** Sets whether to draw points at each vertex
             *  @param  drawPointCircle 
             */
            void setDrawPoints(bool drawPointCircle);


            /** Checks whether the point passed as parameter is inside the contour
             *  @param  pt  the point to check
             *  @return <code>true</code> if pt is inside this contour
             */
            virtual bool contains(const ImagePoint* pt) const = 0;

            /** Checks whether the point passed as parameter is inside the contour
             *  @param  pt  the point to check
             *  @return <code>true</code> if pt is inside this contour
             */
            virtual bool contains(const ImagePoint_F* pt) const = 0;

            /** Checks whether the point passed as parameter is inside the contour
             *  @param  x   x coordinate of the point to check
             *  @param  y   y coordinate of the point to check
             *  @return <code>true</code> if pt is inside this contour
             */
            virtual bool contains(int x, int y) const = 0;

            /** Checks whether the point passed as parameter is inside the contour
             *  @param  x   x coordinate of the point to check
             *  @param  y   y coordinate of the point to check
             *  @return <code>true</code> if pt is inside this contour
             */
            virtual bool contains(float x, float y) const = 0;

            /** Checks whether this contour is completely inside the one passed as parameter.
             *  @param  contour  the contour to check against
             *  @return <code>true</code> if this contour is inside theContour
             */
            virtual bool isInside(const Contour_I* pt) const = 0;

            /** Checks whether this contour is completely inside the one passed as parameter.
             *  @param  contour  the contour to check against
             *  @return <code>true</code> if this contour is inside theContour
             */
            virtual bool isInside(const Contour_F* pt) const = 0;

//            /** Checks whether this contour completely contains the one passed as parameter.
//             *  @param  contour  the contour to check against
//             *  @return <code>true</code> if this contour contains theContour
//             */
//            virtual bool contains(const Contour_I* pt) const = 0;
//
//            /** Checks whether this contour completely contains the one passed as parameter.
//             *  @param  contour  the contour to check against
//             *  @return <code>true</code> if this contour contains theContour
//             */
//            virtual bool contains(const Contour_F* pt) const = 0;

            /** Set the coordinates of the contour's vertex whose index is provided.
             *  Fails if the index is larger that the number of vertices of the contour.
             *  @param  index   index of the vertex to modify
             *  @param  x   new x coordinate of the vertex
             *  @param  y   new y coordinate of the vertex
             */
            virtual void setPoint(int index, float x, float y) = 0;
            
            /** To be called by a user who has modified the list of points or the
             *  points themselves.  This forces recomputation of some of the summary data.
             */
            void pointsHaveMoved(void);




		protected:
		
			/**	Radius of the circular dot used to render a vertex
			 */
			static float VERTEX_DOT_RADIUS;

            /** Constructor
			 *	@param	isFloatingPoint		true if the points on this contour have floating point coordinates
             */
            Contour(bool isFloatingPoint = false);

			/**	Copy Constructor
			 *	@param	ct		the object to copy
			 */
			Contour(const Contour& ct);
			
			/**	Allows a child class whose content has been changed to force recomputation of
			 *	the inter-point distances
			 *	@param	isUpToDate	are inter-point distances up to date?
			 */
			void setInterPointDistancesAreUpToDate_(void) const;

			/**	Allows a child class whose content has been changed to force recomputation of
			 *	the edge vectors
			 *	@param	isUpToDate	are edge vectors up to date?
			 */
			void setEdgeVectorsAreUpToDate_(void) const;


			
        private:
        
            /** The list of inter-point distances along the contour.
             *  distance [i] = || p[i] p[i+1] ||
             */
            mutable std::list <float>  ptDistance_;

            /** The contour's rendering color
             */
            float       color_[4];
			
            /** The rendering color for the contour's points
             */
            float       vertexColor_[4];
			
			/**	Indicates whether the points on this contour have floating
			 *	point coordinates
			 */
			mutable bool isFloatingPointContour_;
			
            /** Indicates whether the inter-point distance list is up to date
             */
            mutable bool interPointDistancesAreUpToDate_;
            
            
            /** Indicates whether the edge list is up to date
             */
            mutable bool edgeVectorsAreUpToDate_;
			
			/** Indicates whether individual points are drawn
			*/
			bool drawPoints_;
        

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const Contour& operator =(const Contour& obj);


            /** Initializes the contour along the edges of a rectangular box
             *
             *  @param  rect    rectangular box 
             *  @param  padding distance away from the box at which to initialize the
             *                  contour
             */
            virtual void initializeBox_(const ImageRect& rect, int padding = 0) = 0;



           /** Initializes the contour about a rectangular box
             *
			 *	@param	nbPts		number of points to create along the contour
			 *	@param	contourType	the type of contour to initialize this one to
             *  @param  rect		rectangular box 
             *  @param  padding		signed distance away from the box at which to initialize the
             *						contour (negative distance creates the contour inside the box)
             */
            virtual void initializeBox_(int nbPts, const ImagePoint& upperLeft,
										const ImagePoint& upperRight, const ImagePoint& lowerLeft,
										const ImagePoint& lowerRight) = 0;


            /** Initializes the contour along the edges of a oval
             *
			 *	@param	nbPts		number of points to create along the contour
			 *	@param	contourType	the type of contour to initialize this one to
             *  @param  center		coordinates of the contour's center 
             *  @param  width		width of the box or ellipse
             *  @param  height		height of the box or ellipse
			 *	@param	angle		orientation of the contour
             */
            virtual void initializeOval_(int nbPts, const ImagePoint& center, float width, float height, 
										 float angle = 0.f) = 0;



             /** Computes the distances between image points.
             *  @see    getInterPointDistances
             *  @see    ptDistance_
             */
            virtual void computeInterPointDistances_(void) const = 0;

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
            virtual void computeEdgeVectorList_(void) const = 0;
            
            /**
             *  Sets a new bounding rectangle for the drawable object.
             *
             *  A drawable object occupies a rectangle within the global image space.
             *  For example, a drawable object could be defined as a subimage of another
             *  image.  This operation may require a reallocation of the image.
             *
             *  This function should be overridden by child classes that have their
             *  own raster (eg.  HSV, gray_F, etc.)
             *
             *  @param  x       X coordinate of the image's origin
             *  @param  y       Y coordinate of the image's origin
             *  @param  theWidth    desired width of the object
             *  @param  theHeight   desired height of the object
             */
            virtual void setBoundRect_(int x, int y, int theWidth, int theHeight);

            /**
             *  Sets a new valid data rectangle for the contour.
             *
             *  Not sure yet what this function should do
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            void setValidRect_(int x, int y, int theWidth, int theHeight);
            
			
		friend class Contour_F;
		friend class Contour_I;

    };
}

#endif  //  CONTOUR_H
