/*  NAME:
        PatternMatcher2D.h
 
    DESCRIPTION:
        PatternMatcher2D public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_MATCHER_2D_H
#define URIVL_PATTERN_MATCHER_2D_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Blob.h"
#include "RasterImage.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray.h"
#include "Pattern2D.h"
#include "ImagePoint.h"
#include "ImageRect.h"
#include "Pattern2DMatchRecord.h"


namespace uriVL
{
    /** Defines the basic types of pattern matching supported by the library.  Other, more
     *  sophisticated pattern matching techniques (e.g. warping-based matching) may be
     *  based on one of these types
     */
    typedef enum BasePatternMatcher2DType {
                    PATTERN_MATCHER_2D_CORR,
                    PATTERN_MATCHER_2D_PART_CORR,
                    PATTERN_MATCHER_2D_MEDIAN,
                    PATTERN_MATCHER_2D_SCALED_DIFF,
                    //
                    PATTERN_MATCHER_2D_NON_BASIC
     } BasePatternMatcher2DType;
     

    /** Parent class for all 2D pattern matchers.  Typically computer vision matchers
     *  such as stereo matchers of motion matchers will have as one of their instance
     *  variables an instance of a subclass of PatternMatcher2D.
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class PatternMatcher2D 
    {
        public:
        
            /** Default constructor
             */
            PatternMatcher2D(void);

            /** Destructor. Does nothing
             */         
            virtual ~PatternMatcher2D(void);

            /** If the matcher is one of the basic types, indicates which
             */
            BasePatternMatcher2DType getBaseMatcherType(void) const;
             
            /** Indicates whether the matcher returns an integer of floating point
             *  (subpixel) best match
             *  @return     <code>true</code> if this matcher returns a floating point,
             *              subpixel best match
             */
            bool    isSubpixelMatcher(void);

            /** Called to define the pattern used for subsequent covariance calculations.
             *  This function is called by the match_ function.  It (and setImage) should
             *  be called prior to a call to covariance().
             *  Currently only RGBa and gray-level patterns are accepted.
             *  @param  patImg     the rectangular pattern we are seaking
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             */
            virtual void setPattern(const RasterImage* patImg, const Blob* patBlob=NULL);


            /** Called to define the pattern used for subsequent covariance calculations
             *  This function is called by the match_ function.  It (and setImage) should
             *  be called prior to a call to covariance().
             *  Currently only RGBa and gray-level patterns are accepted.
             *  @param  pattern    the pattern we are seaking
             */
            virtual void setPattern(const Pattern2D* pattern);


            /** Called to define the image used for subsequent covariance calculations.
             *  Currently, the image's base raster type should match that of the pattern
             *  (which implies that the pattern should have been set before this function
             *  is called)
             *  @param  img     the image in which we seek a best match for the pattern
             *  @param  matchPoint  origin of the search
             *  @see setPattern
             */
            virtual void setImage(const RasterImage* img, const ImagePoint* matchPoint=NULL);
            

            /** Determines the Pattern2DMatchRecord of the best match for a target
             *  pattern in an image.  The search region is specified by an image
             *  rectangle.<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patternSought   the pattern to search for in <code>img</code>.
             *                              This pattern may store a blob or be simply
             *                              rectangular
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @return     the Pattern2DMatchRecord of the best match found
             */
            Pattern2DMatchRecord* match(const Pattern2D* patternSought, const ImageRect* searchRect, 
                                        const RasterImage* img);

            /** Determines the Pattern2DMatchRecord of the best match for a target
             *  pattern in an image.  The search region is specified by an image
             *  rectangle.<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patternSought   the pattern to search for in <code>img</code>.
             *                              This pattern may store a blob or be simply
             *                              rectangular
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  occluder        list of blob patterns that occlude this one
             *  @return     the Pattern2DMatchRecord of the best match found
             */
            Pattern2DMatchRecord* match(const Pattern2D* patternSought, const ImageRect* searchRect, 
                                        const RasterImage* img, std::list<Blob*> *occluder);

            /** Determines the Pattern2DMatchRecord of the best match for a target
             *  pattern in an image.  The search region is specified by an image
             *  rectangle.<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patternSought   the pattern to search for in <code>img</code>.
             *                              This pattern may store a blob or be simply
             *                              rectangular
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             */
            void match(const Pattern2D* patternSought, const ImageRect* searchRect,
                       const RasterImage* img, Pattern2DMatchRecord* matchRecord);
                                
            /** Determines the Pattern2DMatchRecord of the best match for a target
             *  pattern in an image.  The search region is specified by an image
             *  rectangle.<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patternSought   the pattern to search for in <code>img</code>.
             *                              This pattern may store a blob or be simply
             *                              rectangular
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             *  @param  occluder        list of blob patterns that occlude this one
             */
            void match(const Pattern2D* patternSought, const ImageRect* searchRect, 
					   const RasterImage* img, 
                       Pattern2DMatchRecord* matchRecord, std::list<Blob*> *occluder);
                                


        protected:
        
            /** If the matcher is one of the basic types, indicates which
             */
            BasePatternMatcher2DType baseMatcherType_;
            
    
            /** Indicates whether the matcher returns an integer of floating point
             *  (subpixel) best match
             */
            bool    isSubpixelMatcher_;
        
            /** Blob information of the pattern we are seeking
             */
            const Blob* patBlob_;
            
            /** Raster type of the pattern sought
             */
            BaseRasterType patRasterType_;
            
            /** Pointer to the pattern's raster.
             */
            const RasterImage* patImg_;
            
            /** Pointer to the search image's raster.
             */
            const RasterImage* img_;          
            
            /** copy of a gray-level pattern's raster
             */
            float** grayPat_;
            
            /** copy of a gray-level image region's raster
             */
            float** grayImg_;
            
            /** copy of a color rgba pattern's raster
             */
            float*** rgbaPat_;
            
            /** copy of a color rgba image region's raster
             */
            float*** rgbaImg_;
            
            /** mask corresponding to the part of the pattern's bounding box to be used 
             *  for matching calculations.  Currently, the mask simply corresponds to the
             *  pattern's blob.  In the future, this will also take into account the
             *  geometry of other blobs that are expected to occlude the pattern sought.
             *  Note that this mask is only used for blob mattching.
             */
            unsigned char** patMask_;
            
            /** Width of the temporary rasters currently alocated
             */
            int patWidth_;
            
            /** Height of the temporary rasters currently alocated
             */
            int patHeight_;
                        
            /** Determines the location and score of the best match for a target
             *  pattern in an rgba image.  The search region is specified by an image
             *  rectangle.  The pattern is a (small) rectangular image patch<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patImg          the rectangluar image pattern to search for
             *                              in <code>img</code>.
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             */
            virtual void match_(const RasterImage_RGBa* patImg, const ImageRect* searchRect, 
								const RasterImage_RGBa* img, Pattern2DMatchRecord* matchRecord) = 0;

            /** Determines the location and score of the best match for a target
             *  pattern in a gray-level image.  The search region is specified by an image
             *  rectangle.  The pattern is a (small) rectangular image patch<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patImg          the rectangluar image pattern to search for
             *                              in <code>img</code>.
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             */
            virtual void match_(const RasterImage_gray* patImg, const ImageRect* searchRect, 
								const RasterImage_gray* img, Pattern2DMatchRecord* matchRecord) = 0;

            /** Determines the location and score of the best match for a target
             *  pattern in an rgba image.  The search region is specified by an image
             *  rectangle.  The pattern is a blob defined over a (small) rectangular
             *  image patch<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patImg          the rectangluar image pattern to search for
             *                              in <code>img</code>.
             *  @param  patBlob         the blob that defines the actual geometry of the
             *                              pattern
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             */
            virtual void match_(const RasterImage_RGBa* patImg, const Blob* patBlob, 
								const ImageRect* searchRect,
								const RasterImage_RGBa* img, Pattern2DMatchRecord* matchRecord) = 0;

            /** Determines the location and score of the best match for a target
             *  pattern in a gray-level image.  The search region is specified by an image
             *  rectangle.  The pattern is a blob defined over a (small) rectangular
             *  image patch<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of a
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patImg          the rectangluar image pattern to search for
             *                              in <code>img</code>.
             *  @param  patBlob         the blob that defines the actual geometry of the
             *                              pattern
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             */
            virtual void match_(const RasterImage_gray* patImg, const Blob* patBlob, 
								const ImageRect* searchRect,
								const RasterImage_gray* img, Pattern2DMatchRecord* matchRecord) = 0;


            /** Checks whether the Pattern2DMatchRecord sent has the proper subtype for the
             *  matcher.
             *  @param  matchRecord     Pattern2DMatchRecord to check
             */
            virtual bool isProperMatchRecordType_(const Pattern2DMatchRecord* matchRecord) const = 0;
            
            /** Allocates a Pattern2DMatchRecord that has the proper subtype for the
             *  matcher.
             *  @return  a match recordthat has the proper subtype for the matcher
             */
            virtual Pattern2DMatchRecord* newPattern2DMatchRecord_(void) = 0;

            /** Allocates private computation rasters for the pattern and the image 
             *  @param  patImg  pointer to the pattern's image to allocate rasters for
             *  @param  patBlob pointer to the pattern's blob (possibly NULL)
             */
            virtual void allocateRasters_(const RasterImage* patImg, const Blob* patBlob);

            /** Deletes the private computation rasters
             */
            virtual void deleteRasters_(void);


    };
}

#endif  //  PATTERN_MATCHER_2D_H
