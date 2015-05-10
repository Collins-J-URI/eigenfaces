/*  NAME:
        Blob.h

    DESCRIPTION:
        Blob public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_BLOB_H
#define URIVL_BLOB_H

#include <list>
#include <deque>
#include <vector>
#include <string>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"
#include "ImageFeature.h"
#include "ColorRegions.h"
#include "ImageRect.h"
#include "ImagePoint_F.h"
#include "Contour_I.h"

namespace uriVL
{
    //---------------------------------------------------------------------
    //  Data structures
    //---------------------------------------------------------------------

    /** enumerated type for indices of quadrants.  This is probably not the
     *  best place for that declaration.
     */
    typedef enum QuadrantIndex
    {
        UPPER_LEFT_QUAD = 0,
        UPPER_RIGHT_QUAD,
        LOWER_LEFT_QUAD,
        LOWER_RIGHT_QUAD
    } QuadrantIndex;

    /** This is the data structure returned by the filling algorithm: a linked list of
     *  horizontal segments constitutes a blob
     */
    typedef struct HorizontalSegment
    {
        int leftX;
        int rightX;
        int y;
    } HorizontalSegment;

	/**	Iterator for a list of pointers to HorizontalSegment objects
	 */
	typedef std::list<HorizontalSegment*>::iterator HSegIterator;
	
	/**	Constant iterator for a list of pointers to HorizontalSegment objects
	 */
	typedef std::list<HorizontalSegment*>::const_iterator const_HSegIterator;
	
    /** The data structure used to hold of the segments of a blob:  A deque that store
     *  the data doe each line of the blob, sorted top to bottom.  The info for each line
     *  is a (pointer to a) list of pointers to HorizontalSegment structs.  The segments
     *  are sorted left to right on the line.  This allows for pretty fast access on the
     *  blob
     */
    typedef std::deque< std::list<HorizontalSegment*> *>  LineSegDeque;
	

    typedef struct Edge
    {
        int yMax;
        int x;
        float slope;
    } Edge;

//    /**
//     *  XPair is used in fillHoles(), indicating each covered segment
//     *  upstairs/downstairs for each line of HorizontalSegment
//     */
//    typedef struct XPair
//    {
//        int left;
//        int right;
//    } XPair;
//
//    /**
//     *  IndexPair is used in fillHoles(), indicating each pair of segments
//     *  to be connected in each line
//     */
//    typedef struct IndexPair
//    {
//        int left;
//        int right;
//    } IndexPair;
//
//    typedef deque<XPair *>  XPairDeque;
//    typedef deque<IndexPair *>  IndexPairDeque;
//

    /** A blob is a connected set of pixels that share a comon characteristic.
     *
     *  Typically a Blob is detected after a labelling information (color regions,
     *  motion detection, etc.).  The main data structure for this class is a deque 
	 *	(top to bottom) of ordered lists (left to right) of horizontal extents.<br></br>
	 *
	 *	<b>The vexing issue of Blob contour:</b> This issue sits at the itching point
	 *	of the depate between "the pixel as a point" and "the pixel as a little square".  
	 *	Even though I have string "pixel as point" leanings, I must concede that this view
	 *	leads into problems when we have to talk of the contour of a connected group of
	 *	pixels (a Blob, then) that is "one pixel think" in at least some places.  A such
	 *	locations, the "pixel as point" view gives us a contour that is self-intersecting,
	 *	which leads to all sorts of problems for modules that must interpret the contour.
	 *	Another--minor--side effect of such an approach is that the contour of a blob always
	 *	appears to be "one off" on the right-side borders and bottom-side borders of the blob.
	 *
	 *	As a workaround to both problem, the contour of a Blob is now defined for the
	 *	"pixel as a little square".  So, now, a single pixel can have a contour defined
	 *	buy its four "corners".  This solves the display and the self-intersection issues.
	 *	The contructor taking a Contour as input parameter and the function  that "fill in"
	 *	a contour both naturally take that into account so that blob and Blob(blob->contour())
	 *	refer to the same image region.
	 *	
	 *	@see Contour
     *
     *  @author     Fang Han and jean-yves herve'
     *                  3D Group for Interactive Visualization,
     *                  Dept, of Computer Science and Statistics,
     *                  University of Rhode Island
     *  @modified:  Jun, 2005       inherited from ImageFeature
     */
    class URIVL_EXPORT Blob : public ImageFeature
    {
        public:

            /** Default constructor
             */
            Blob(void);

            /**
             *  Constructor that produce a blob based on the contour
             *  @param  contour    the contour of this blob
             */
            Blob(const Contour_I* contour);

            /** Constructor that copies the structure of the blob received as parameter
             *  but translates it to the origin received as second parameter.  If that
             *  parameter is NULL, then the origin is not changed and this constructor
             *  behaves like a cloner.
             *  @param  blob    the blob to copy
             *  @param  origin  new origin for the blob
             */
            Blob(const Blob* blob, const ImagePoint* origin = NULL);


            /** Destructor
             */
            ~Blob(void);

            /** Adds an horizontal segment to the blob and updates the bounding
             *  rectangle.
             *
             *  @param  leftX   left endpoint of the segment
             *  @param  rightX  right endpoint of the segment
             *  @param  y       y coordinate of the segment
             */
            void addSegment(int leftX, int rightX, int y);


            /** Returns the total number of pixels in the blob
             *  @return     total number of pixels in the blob
             */
            long getNbPixels(void) const;


            /** Returns the total number of pixels in the blob's indicated quadrant
             *  @param      the quadrant for which we want the number of pixels
             *  @return     total number of pixels in the blob
             */
            long getNbPixels(QuadrantIndex quad) const;


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

            /**
             *  shift the blobs coordinates by the specified amount
             *  @param  dx  translation along the x axis
             *  @param  dy  translation along the y axis
             */
            void translate(int dx, int dy);

            /**
             *  shift the blobs coordinates by the specified amount
             *  @param  dx  translation along the x axis
             *  @param  dy  translation along the y axis
             */
            void translate(float dx, float dy);

            /**
             *  copy method.  Copies this blob into another one,
             *  @param  destBlob    the blob to copy this one into
             */
            void copyInto(Blob* destBlob);

            /** returns a read-only reference to this blob's private segments structure.
             *  @return a pointer to segments structure
             */
            const LineSegDeque& getSegDeque(void) const;

            /** Get the contour of the blob.
             *
             *  NOTICE: This is an approximate contour of the blob, since it is not
             *  that accurate when the blob has some inside contour
             *
             *  If the contour did not already exist, a new contour is calculated.
             *  The method returns a pointer to the object's private data.  Don't
             *  delete it.
             *
             *  @return     a pointer to this blob's private contour
             */
            const Contour_I* getContour(void) const;

            /** Checks whether the point passed as parameter belongs to the blob.
             *  @param  pt  the point to check
             *  @return <code>true</code> if pt is inside this blob's contour
             */
            bool contains(const ImagePoint* pt) const;

            /** Checks whether the point passed as parameter is inside the contour
             *  @param  x   x coordinate of the point to check
             *  @param  y   y coordinate of the point to check
             *  @return <code>true</code> if pt is inside this contour
             */
            bool contains(int x, int y) const;

            /** Tells the blob whether it's contour should be drawn when the blob is
             *  rendered
             *  @param  drawContour <code>true</code> if the blob's contour is to be drawn
             */
            void setDrawContour(bool drawContour);

            /** Assigns a rendering color to the contour (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setContourColor(float red, float green, float blue) const;


            /** Assigns a rendering color to the contour  (alpha is set to 1)
             *  @param  red     red component of the rendering color
             *  @param  green   green component of the rendering color
             *  @param  blue    blue component of the rendering color
             */
            void setContourColor(unsigned char red, unsigned char green, unsigned char blue) const;

            /** Assigns a rendering color to the contour (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setContourColor(const float theColor[]) const;


            /** Assigns a rendering color to the contour (alpha is set to 1)
             *  @param  theColor    rgb components of the rendering color
             */
            void setContourColor(const unsigned char theColor[]) const;


            /**
             *  Renders the blob's contour at the depth and location specified.
             *
             *  @param  theZ    Depth at which this blob is to be rendered.
             *  @param  shifted     true if the object should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @param  origin      location of the object's upper-left corner in the rendering
             */
			void renderContour(float theZ, bool shifted, const ImagePoint* origin) const;
			

			/**	Renders this blob into a RasterImage.  
			 *  Notes: <ul>
			 *		<li> Not all image formats supported at this point (this is yet-another project-driven feature).</li>
			 *		<li> If img is a color image, then the blob's rendering color is used for filling</li>
			 *		<li> If img is a gray-level image, then blob pixels are painted white</li>
			 *	</ul>
			 *	@param	img				the RasterImage object to render into
			 *	@param	clearFirst		if true, then set pixel values to zero first
			 */
			void renderInto(RasterImage* img, bool clearFirst=false);


            /**
             *  connect all Horizontal segments that have holes between each other
             *  The holes here are defined as spaces in closed (four directions) area
             */
            void fillHoles(void);

            /** Tests what fraction of a disk centered at the center of the blob's bounding
             *  box is actually made up of of blob pixels.  This can be used as a quick first
             *  way to select possibly elliptic blobs. 
             *  @param  rad radius of the test disk
             *  @return the fraction of disk pixels that belong to the blob (range [0, 1])
             */
            float diskCoverage(int radius);
            

        private:

            /** Structure that stores the segments making up the blob.
             */
            LineSegDeque segDeque_;

            /** Contour of the blob
             */
            mutable Contour_I* contour_;

            /** Stores the number of pixels for each quadrant of the blob
             */
            mutable long quadNbPixels_[4];


            /** Indicates whether the bonding box stored is valid
             */
            mutable bool contourIsUpToDate_;

            /** Lowest y value in the blob.  Updated for fast acces to the deque.
             */
            int top_;

            /** Largest y value in the blob.  Updated for fast acces to the deque.
             */
            int bottom_;

            /** Total number of HorizontalSegment in the blob
             */
            int nbHSeg_;

            /** Total number of pixels in the blob
             */
            mutable long nbPixels_;

            /** When rendering the blob, draw its contour
             */
            bool drawContour_;

            /** Indicates whether the blob has chhanged since the last time the
             *  numbers of pixels in the quadrants were calculated
             */
            mutable bool quadNbPixelsAreUpToDate_;


            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            Blob(const Blob& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const Blob& operator =(const Blob& obj);


            /** Computes the number of pixels in each quadrant
             */
            void computeQuadNbPixels_(void) const;

            /**
             *  helper func of fillHoles
             *  @param  indexDeque  a deque of lists of the indices (integer) of segment
             *  @param  rangeVect   pointer to a vector of integer that indicates both left and
             *                      right of the covered segments
             *  @param  currSegList current line (a list of segments)
             *  @param  topDown     topDown or bottomUp
             */
            void fillHolesOnePass_(std::deque<std::vector<int> *>* indexDeque,
                                   std::vector<int>** rangeVect,
                                   std::list<HorizontalSegment *>* currSegList,
                                   bool topDown);

            /**
             *  Renders the blob at the depth and location specified.
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


            /**
             *  Attempts (and fails ) to a new bounding rectangle for the blob.
             *
             *  This function must be implemented since a Blob is a DrawableObject2D,
             *  and it can be instantiated. In DrawableObject2D this method is pure virtual.
             *  However, it makes no sense forcing a change of bounding rectangle for
             *  a blob (I may rethink this later as a form of clipping, though).
             *
             *  @param  x           horizontal coordinate of upper left corner
             *  @param  y           vertical coordinate of upper left corner
             *  @param  theWidth    desired width of the image
             *  @param  theHeight   desired height of the image
             */
            virtual void setBoundRect_(int x, int y, int theWidth, int theHeight);


            /**
             *  Sets a new valid data rectangle for the blob.
             *
             *  Not sure yet what this function should do
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            void setValidRect_(int x, int y, int theWidth, int theHeight);


            /** Computes the contour of the blob.
             *
             *  NOTICE: This is an approximate contour of the blob, since it is not
             *  that accurate when the blob has some inside contour
             *
             *  If the contour did not already exist, a new contour is calculated.
             *  This method firstly calls sortArrayHSeg_() and then gets the contour
             */
            void computeContour_(void) const;


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


            /** Deletes the segments stored.  Called by the destructor and copyInto.
             */
            void deleteSegments_(void);


        friend class Pattern2D;
		friend class DrawableObject2D;


    };

	/**	A BlobList is a list of Blob pointers
	 */
    typedef std::list<Blob*> BlobList;
}

#endif  //  BLOB_H
