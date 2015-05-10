/*  NAME:
        PatternMatcher2D_warp.h

    DESCRIPTION:
        PatternMatcher2D_warp public header
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef URIVL_PATTERN_MATCHER_2D_WARP_H
#define URIVL_PATTERN_MATCHER_2D_WARP_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "PatternMatcher2D.h"
#include "WarpMatchScoreFunctionND.h"
#include "NonlinearOptimizerND_conjDir.h"
#include "Pattern2DMatchRecord_warp.h"


namespace uriVL
{
    /** Pattern matcher that uses an image warper to compute a best match for a
     *  deformable template.  
     */
    class PatternMatcher2D_warp : public PatternMatcher2D
    {
        public:
        
            /** Creates a new warping-based pattern matcher.
             *  @param  matcherType  type of basic matcher this one is based on
             *  @param  powellCode   type of conjugate direction algorithm used
             *  @param  warperType   type of image warper used by this object
             *  @param  gridRows    number of rows of the warping grid
             *  @param  gridCols    number of columns of the warping grid
             *  @param  innerMode    grid inner warping mode for this warper
             *  @param  outerMode    grid outer warping mode for this warper
             */
            PatternMatcher2D_warp(BasePatternMatcher2DType matcherType,
                                  PowellMethodCode code,
                                  ImageWarperType warperType,
                                  int gridRows, 
                                  int gridCols,
                                  InnerWarpingMode innerMode = INNER_FREE, 
                                  BorderWarpingMode outerMode = BORDER_FREE);

            /** Creates a new warping-based pattern matcher.
             *  @param  predefined basic pattern matcher (e.g. correlation) to use
             *  @param  powellCode   type of conjugate direction algorithm used
             *  @param  warperType   type of image warper used by this object
             *  @param  gridRows    number of rows of the warping grid
             *  @param  gridCols    number of columns of the warping grid
             *  @param  innerMode    grid inner warping mode for this warper
             *  @param  outerMode    grid outer warping mode for this warper
             */
            PatternMatcher2D_warp(PatternMatcher2D *matcher,
                                  PowellMethodCode code,
                                  ImageWarperType warperType,
                                  int gridRows, 
                                  int gridCols,
                                  InnerWarpingMode innerMode = INNER_FREE, 
                                  BorderWarpingMode outerMode = BORDER_FREE);

            /** Creates a new warping-based pattern matcher.
             *  @param  matcherType  type of basic matcher this one is based on
             *  @param  powellCode   type of conjugate direction algorithm used
             *  @param  predefined image warper (e.g. GL-based) to use
             */
            PatternMatcher2D_warp(BasePatternMatcher2DType matcherType,
                                  PowellMethodCode code,
                                  ImageWarper *warper);

            /** Creates a new warping-based pattern matcher.
             *  @param  predefined basic pattern matcher (e.g. correlation) to use
             *  @param  powellCode   type of conjugate direction algorithm used
             *  @param  predefined image warper (e.g. GL-based) to use
             */
            PatternMatcher2D_warp(PatternMatcher2D *matcher,
                                  PowellMethodCode code,
                                  ImageWarper *warper);


            /** Destructor
             */         
            ~PatternMatcher2D_warp(void);

            /** Called to define the pattern used for subsequent covariance calculations.
             *  This function is called by the match_ function.  It (and setImage) should
             *  be called prior to a call to covariance().
             *  Currently only RGBa and gray-level patterns are accepted.
             *  @param  patImg     the rectangular pattern we are seaking
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             */
            void setPattern(const RasterImage* patImg, const Blob* patBlob = NULL);


            /** Called to define the pattern used for subsequent covariance calculations
             *  This function is called by the match_ function.  It (and setImage) should
             *  be called prior to a call to covariance().
             *  Currently only RGBa and gray-level patterns are accepted.
             *  @param  pattern    the pattern we are seaking
             */
            void setPattern(const Pattern2D* pattern);


            /** Called to define the image used for subsequent covariance calculations.
             *  Currently, the image's base raster type should match that of the pattern
             *  (which implies that the pattern should have been set before this function
             *  is called)
             *  @param  img     the image in which we seek a best match for the pattern
             *  @param  matchPoint  origin of the search
             *  @see setPattern
             */
            void setImage(const RasterImage* img, const ImagePoint* matchPoint);
            

            /** Computes the image dimensions that the warper can afford that are closest to
             *  the ones sent by the user.
             *  <p>Some warpers require specific image dimensions/ratios
             *  (e.g. a GL warper only takes 2^n x 2^n images).  Furthermore, QuickTime
             *  GraphWorld rasters are padded on some platforms.  The user should therefore
             *  first call imageEffectiveDimensions with desired image dimensions, and then 
             *  create the image at the corresponding "effective" dimensions specified
             *  by that function.</p>
             *  <p>If you want to warp an image, it is therefore recommended that you check
             *  what are the closest suitable dimensions for the type of warper you are using.</p>
             *
             *  @param  imgWidth        input image width
             *  @param  imgHeight       input image height
             *  @param  lowImgWidth     closest lower suitable width
             *  @param  lowImgHeight    closest lower suitable height
             *  @param  highImgWidth    closest higher suitable width
             *  @param  highImgHeight   closest higher suitable height
             */
             void getImageSuitableDimensions(int imgWidth, int imgHeight,
                                            int* lowImgWidth, int* lowImgHeight,
                                            int* highImgWidth, int* highImgHeight);



            /**  Returns the "optimal" warped image corresponding to the best match
             */
            RasterImage* getWarpedImage(void);
             
            /** Returns a pointer to the array of initial grid coordinates
             */
            float*** const getInitGrid(void);


            /**
             *  own match function
             */
            Pattern2DMatchRecord* match(const Pattern2D* patt2D,
                                        const ImagePoint* matchPoint,
                                        const RasterImage* img);

            /**
             *  own match function
             */
            void match(const Pattern2D* patt2D,
                       const ImagePoint* matchPoint,
                       const RasterImage* img,
                       Pattern2DMatchRecord* matchRecord);

            /**
             *  own match function
             */
            Pattern2DMatchRecord* match(const RasterImage* pat,
                                        const ImagePoint* matchPoint,
                                        const RasterImage* img);

            /**
             *  own match function
             */
            void match(const RasterImage* pat,
                       const ImagePoint* matchPoint,
                       const RasterImage* img,
                       Pattern2DMatchRecord* matchRecord);

            
        protected:
            
            /** an embedded minimizer
             */
            NonlinearOptimizerND_conjDir_Powell *optN_;

            /** an embedded WarpCorrFunctionND
             */
            WarpMatchScoreFunctionND    *wcf_;
            
            /** OptimizerRecordND
             */
            OptimizerRecordND   *optRecND_;
            
            /** dimension of the NonlinearOptimizerND_conjDir_Powell
             */
            int     nDim_;
            
            /** nb of rows of grid in warper
             */
            int     gridRows_;
            
            /** nb of rows of grid in warper
             */
            int     gridCols_;
            
            /** starting ND int array
             */
            float   *xyStart_;
            
            /** min ND int array
             */
            float   *xyMin_;
            
            /** max ND int array
             */
            float   *xyMax_;

            /** initial points of warper_->originalPoints_
             *  computed right after the allocation of wcf_
             */
            float*** gridInit_;

            /** reverse warped version of initPoints
             */
            float*** gridRev_;
    
            /** contour after reverse warping
             */
            Contour_I *rwContour_;
            
            
            /** Tolerance of numerical optimization process
             */
            static const float OPT_TOL;
            
            
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
             *  @param  matchRecord     matching record of the best match found
             */
            void match_(const RasterImage_RGBa* patImg, const ImageRect* searchRect,
                        const RasterImage_RGBa* img, Pattern2DMatchRecord* matchRecord);

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
             *  @param  matchRecord     matching record of the best match found
             */
            void match_(const RasterImage_gray* patImg, const ImageRect* searchRect,
                        const RasterImage_gray* img, Pattern2DMatchRecord* matchRecord);

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
             *  @param  matchRecord     matching record of the best match found
             */
            void match_(const RasterImage_RGBa* patImg, const Blob* patBlob, const ImageRect* searchRect,
                        const RasterImage_RGBa* img, Pattern2DMatchRecord* matchRecord);

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
             *  @param  matchRecord     matching record of the best match found
             */
            void match_(const RasterImage_gray* patImg, const Blob* patBlob, const ImageRect* searchRect,
                        const RasterImage_gray* img, Pattern2DMatchRecord* matchRecord);            

            /**
             *  reverse warping & get reverse warped contour
             *
             *  @param  patt2D      reference 2D pattern
             *  @param  matchPoint  matching point
             */
            void reverseWarp_(const Pattern2D* patt2D, const ImagePoint* matchPoint);

            /** Checks whether the Pattern2DMatchRecord sent has the proper subtype for the
             *  matcher.
             *  @param  matchRecord     Pattern2DMatchRecord to check
             */
            bool isProperMatchRecordType_(const Pattern2DMatchRecord* matchRecord) const;

            /** Allocates a Pattern2DMatchRecord that has the proper subtype for the
             *  matcher.
             *  @return  a match recordthat has the proper subtype for the matcher
             */
            Pattern2DMatchRecord* newPattern2DMatchRecord_(void);

            /** deletes the grids and arrays of warp point coordinates stored by this
             *  object
             */
            void deleteGrids_(void);
            
            /** Initializes the min and max arrays of xy values
             *  This function should be moved to the ImageWarper class
             */
            void computeGridBounds_(void);


    };
}

#endif  //  PATTERN_MATCHER_2D_H

