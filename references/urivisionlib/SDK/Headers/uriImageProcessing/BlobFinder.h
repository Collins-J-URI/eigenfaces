/*  NAME:
        BlobFinder.h

    DESCRIPTION:
        BlobFinder public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_BLOB_FINDER_H
#define URIVL_BLOB_FINDER_H

#include <list>
#include <string>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ColorRegions.h"
#include "RasterImage.h"
#include "LabelRaster.h"
#include "Blob.h"
#include "FeatureDetector.h"

namespace uriVL
{
    //---------------------------------------------------------------------
    //  Constants and macros
    //---------------------------------------------------------------------
    #define     kStackInitSize  100
    #define     kStackSizeIncr   50
    //

    /** Displacement directions along the search spiral
     */
    typedef enum MoveDirection
    {
        MOVE_LEFT = 0,
        MOVE_DOWN,
        MOVE_RIGHT,
        MOVE_UP
    } MoveDirection;


    /** Data structure that defines the conditions of a blob search
     */
    typedef struct BlobSearchParams
    {
        /** number of indices sought
         */
        int           nbIndicesSought;

        /** list of label indices sought
         */
        unsigned char   *index;

        /** minimum blob size (in number of pixels)
         */
        int           blobMinSize;

    } BlobSearchParams;


    /** Blob Finder class.
     *
     *  A blob finder detects in a label raster (or part of such a raster) connnected
     *  regions that share a common lbel index.
     *  A blob finder can either detect all blobs in a given rectangle or return the
     *  first target blob detected near a starting position, using a spiral search.
     *
     *  v. 1.0  06/09/2003  C++ rewriting of the fingertip detection C code developed
     *  by Fabienne Lathuiliere (GRPR, EP Montreal) for her hand/finger reconstruction
     *  project [1998-99]
     *  @author     jean-yves herve' and Fang Han , 3D Group, URI
     *
     *  @modified:  Jun, 2005       inherited from FeatureDetector
     */
    class URIVL_EXPORT BlobFinder : public FeatureDetector
    {
        public:

            /** Constructor.
             *  Initializes the instance variables
             */
            BlobFinder(void);

//            /** Constructor.
//             *  Initializes the instance variables
//             *  @param  label       defines the dimensions of the search
//             */
//            BlobFinder(const LabelRaster* label);
//
//            /** Constructor.
//             *  Initializes the instance variables
//             *  @param  rect       defines the dimensions of the search
//             */
//            BlobFinder(const ImageRect* rect);

            /** Destructor
             */
            ~BlobFinder(void);

            /** Detect all the blobs in the input label raster from a list of target indices
             *
             *  This methods modifes the values stored in the label raster rather than
             *  storing a copy.  If this poses a problem, the user should clone the
             *  input label raster and send a copy.
             *
             *  @param  label   the LabelRaster to seek the blobs into
             *  @param  n           the number of indices to look for
             *  @param  target      the list of these indices
             *  @param  blobMinSize     minimum size for the blob
             *  @return     an array of list of blobs (one list per index)
             */
            BlobList* findAllBlobs(const LabelRaster* label, int n, const char target[], long blobMinSize,
                                   bool fillHoles = false);


            /** Detect all the blobs in the input label raster from a list of target indices
             *
             *  This methods modifes the values stored in the label raster rather than
             *  storing a copy.  If this poses a problem, the user should clone the
             *  input label raster and send a copy.
             *
             *  @param  label       the LabelRaster to seek the blobs into
             *  @param  n           the number of indices to look for
             *  @param  target      the list of these indices
             *  @param  blobMinSize     minimum size for the blobs
             *  @param  rect        search rectangle
             *  @param  fillHoles   when true, holes within the blob are filled
             *  @return     an array of list of blobs (one list per index)
             */
            BlobList* findAllBlobs(const LabelRaster* label, int n, const char target[],
                                   long blobMinSize, const ImageRect* mySearchRect,
                                   bool fillHoles = false);

            /** Detect all the blobs in the input label raster from a list of target indices
             *
             *  This methods modifes the values stored in the label raster rather than
             *  storing a copy.  If this poses a problem, the user should clone the
             *  input label raster and send a copy.
             *
             *  @param  label   the LabelRaster to seek the blobs into
             *  @param  n           the number of indices to look for
             *  @param  target      the list of these indices
             *  @param  blobMinSize     array of minimum blob sizes per target color
             *  @param  blobMaxSize     array of maximum blob sizes per target color
             *  @param  rect    search rectangle
             *  @param  fillHoles   when true, holes within the blob are filled
             *  @return     an array of list of blobs (one list per index)
             */
            BlobList* findAllBlobs(const LabelRaster* label, int n, const char target[],
								   const long* blobMinSize, const long* blobMaxSize, 
								   const ImageRect* mySearchRect, 
                                   bool fillHoles = false);

			/** Detect all the blobs with the target color inside of the blobs of a list.
			 *
			 *	
			 *	@param	label		the LabelRaster to seek the blobs into
			 *	@param	blobL		a list of blobs to find blobs in
			 *	@param	target		list of target labels
			 *	@return		an array of lists of inner blobs found (one list for each input blob)
			 */
			BlobList** findAllBlobsInside(const LabelRaster* label, BlobList *blobL, 
										  const char target[], long blobMinSize);


			/** Detect all the blobs with the target color inside of a given blob.
			 *
			 *	
			 *	@param	label		the LabelRaster to seek the blobs into
			 *	@param	blob		the  blobs to find blobs in
			 *	@param	nbTargets	number of values in the target array
			 *	@param	target		list of target labels
			 *	@return	the list of all inner blobs found within the input blob
			 */
			BlobList* findAllBlobsInside(const LabelRaster* label, Blob* blob, int nbTargets, 
										 const char target[], long blobMinSize);


            /** Detect blob with the target color about the designated location.
             *
             *  Detect the blob of sufficient size (number of pixels) and target index
             *  nearest to the designated location.  The search for the blob is done
             *  along a spiral centered at the designated location.  Small blobs
             *  encountered on the way are discarded until a blob of sufficient size
             *  is finally encountered or the spiral search has run out of the
             *  label raster's valid data rectangle.
             *
             *  This methods modifes the values stored in the label raster rather than
             *  storing a copy.  If this poses a problem, the user should clone the
             *  input label raster and send a copy.
             *
             *  @param  label       the LabelRaster to seek the blobs into
             *  @param  pos         the image point to start the search from
             *  @param  target      the index sought
             *  @param  blobMinSize     minimum size for the blob
			 *	@param	extendOut	when true, blob filling continues out of the search rect
             *  @param  fillHoles   when true, holes within the blob are filled
             *  @return     the blob found
             */
            Blob* findBlobAbout(const LabelRaster* label, const ImagePoint* pos, char target,
                                long blobMinSize,
								bool extendOut = false,
                                bool fillHoles = false);

            /** Detect blob with the target color about the designated location.
             *
             *  Detect the blob of sufficient size (number of pixels) and target index
             *  nearest to the designated location.  The search for the blob is done
             *  along a spiral centered at the designated location.  Small blobs
             *  encountered on the way are discarded until a blob of sufficient size
             *  is finally encountered or the spiral search has run out of the
             *  search rectangle.
             *
             *  This methods modifes the values stored in the label raster rather than
             *  storing a copy.  If this poses a problem, the user should clone the
             *  input label raster and send a copy.
             *
             *  @param  label       the LabelRaster to seek the blobs into
             *  @param  pos         the image point to start the search from
             *  @param  target      the index sought
             *  @param  searchRect  search rectangle for the initial spiral search
             *  @param  blobMinSize     minimum size for the blob
			 *	@param	extendOut	when true, blob filling continues out of the search rect
             *  @param  fillHoles   when true, holes within the blob are filled
             *  @return     the blob found
             */
            Blob* findBlobAbout(const LabelRaster* label, const ImagePoint* pos, char target,
                                long blobMinSize, const ImageRect* searchRect,
								bool extendOut = false,
                                bool fillHoles = false);

            /** Detect blob with the target color about the designated location.
             *
             *  Detect the blob of sufficient size (number of pixels) and target index
             *  nearest to the designated location.  The search for the blob is done
             *  along a spiral centered at the designated location.  Small blobs
             *  encountered on the way are discarded until a blob of sufficient size
             *  is finally encountered or the spiral search has run out of the
             *  search rectangle.
             *
             *  This methods modifes the values stored in the label raster rather than
             *  storing a copy.  If this poses a problem, the user should clone the
             *  input label raster and send a copy.
             *
             *  @param  label       the LabelRaster to seek the blobs into
             *  @param  pos         the image point to start the search from
             *  @param  target      the index sought
             *  @param  blobMinSize     minimum size for the blob
             *  @param  blobMaxSize     maximum size for the blobs
             *  @param  searchRect  search rectangle for the initial spiral search
			 *	@param	extendOut	when true, blob filling continues out of the search rect
             *  @param  fillHoles   when true, holes within the blob are filled
             *  @return     the blob found
             */
            Blob* findBlobAbout(const LabelRaster* label, const ImagePoint* pos, char target,
                                long blobMinSize, long blobMaxSize, const ImageRect* searchRect,
								bool extendOut = false, bool fillHoles = false);


			/**	Find *one* image feature near the designated point
			 *	@param	img				the image in which to search for features 
			 *	@param	searchStart		start point for the search
			 *	@param	searchRect		the search rectangle.  If NULL, the image's entire valid rect is used
			 *	@return		pointer to the image feature found (NULL if none)
			 */
			ImageFeature* findFeatureAbout(const RasterImage* img, const ImagePoint* searchStart,
												   const ImageRect* searchRect = NULL);
											
			/**	Find all image feature within a search rectangle
			 *	@param	img				the image in which to search for features 
			 *	@param	searchStart		start point for the search
			 *	@param	searchRect		the search rectangle.  If NULL, the image's entire valid rect is used
			 *	@return		pointer to the image feature found (NULL if none)
			 */
			FeatureList* findAllFeatures(const RasterImage* img,
												   const ImageRect* searchRect = NULL);
											

        private:

            /** Default max size of a blob (basically, infinity)
             */
            static const long MAX_LONG_COUNT;

            /** Shifted raster array of the label raster being worked on
             */
            char**	label_;

//            /** Valid data rectangle of the label raster being worked on
//             */
//            ImageRect*	validRect_;


            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            BlobFinder(const BlobFinder& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const BlobFinder& operator =(const BlobFinder& obj);


			/**	Called when a Blob "with holes filled" is sought.  Fills again the elements of the
			 *	private label_ member at location occupied by the Blob
			 *	@param	blob		the location of pixels to refill
			 *	@param	fillValue	the value to assign at these locations
			 *	@see label_
			 */
            void refillBlobLocation_(Blob* theBlob, char fillValue);

            /** Searches left from x to find the first good pixel.
             *
             *  Starting from (x, y), find the left endpoint of the segment
             *  with the target color.  The point at (x, y) is assumed to have
             *  the target color (no checking made)
             *
             *  @param  pos         the image point to start the leftward search from
             *  @param  target      the index shought
             *  @param  limLeft     lower bound for the search
             *  @return     x coordinate of the left endpoint (&le x)
             */
            int findLeft_(const ImagePoint* pos, char target, int limLeft);


            /** Searches right from x to find the last good pixel.
             *
             *  Starting from (x, y), find the right endpoint of the segment
             *  with the target color.  The point at (x, y) is assumed to have
             *  the target color (no checking made)
             *
             *  @param  pos         the image point to start the rightward search from
             *  @param  target      the index shought
             *  @param  limRight    upper bound for the search
             *  @return     x coordinate of the right endpoint (&ge x)
             */
            int findRight_(const ImagePoint* pos, char target, int limRight);


            /**
             *  Checks the pixel at (leftX, y).  If it has the target color, then
             *  seeks leftwards to find the first pixel with the target index.
             *  If does not have the target index, it seeks rightwards, up to and
             *  including the specified upper bound, for the first pixel with the
             *  target index
             *
             *  @param  leftX           lower bound for the search
             *  @param  rightX          upper bound for the search
             *  @param  y               y coordinate of the segment
             *  @param  labelSought     the index sought
             *  @param  limRight        upper bound for the search
             *  @return     x coordinate of pixel found
             */
            int findFirstLeft_(int leftX, int rightX, int y, char labelSought,
                                    int limRight);

            /** Fills the horizontal segment and adds it to the blob
             *  Fills in the input label raster the horizontal segment defined by the three
             *  coordinates with a "no match" (-1) index and adds the segment to the blob
             *
             *  @param  leftX       left endpoint of the horizontal segment
             *  @param  right       right endpoint of the horizontal segment
             *  @param  y           y coordinate of the horizontal segment
             *  @param  theBlob     the blob to add the segment to
             *  @param  fillValue   the value to write in the label raster at the location
             *                          occupied by the blob
             */
            void fillSegAndAdd_(int leftX, int rightX, int y, Blob* theBlob,
                                char fillValue);

            /** Fills and returns the blob at the designated location.
             *
             *  Applies the data-recursive filling algorithm at the designated
             *  location.  The search uses the value of the label index at the
             *  starting point as the target for this search.  The target label
             *  is assumed to be strictly positive (valid label).
             *
             *  @param  pos         starting point of the search
             *  @param  searchRect    search rectangle
             *  @param  fillValue   the value to write in the label raster at the location
             *                          occupied by the blob
             *  @return     the blob found at the dsignated location
             */
            Blob* fillThisBlob_(const ImagePoint* pos, const ImageRect* searchRect, char fillValue);

            /** Implements the spiral search to detect the first pixel of a blob.
             *
             *  Implements the spiral search, starting from the point specified, to
             *  locate a blob with the target index.  The function modifies the point received
             *  as input to store the coordinates of the blob's first point encountered.<br>
             *  Original code by Djibril N'Diaye [EPM 1998], revized by Fabienne Lathuiliere
             *  [EPM 1999], revised by jyh [URI 2005]
             *
             *  @param  pos         starting point for the search and point found
             *  @param  target      the index sought
             *  @param  searchRect  bounding rectangle of the search
             *  @return     true if a blob was encountered, false otherwise
             */
            bool spiralSearch_(ImagePoint* pos, char target, const ImageRect* mySearchRect);

    };
}

#endif  //  BLOB_FINDER_H
