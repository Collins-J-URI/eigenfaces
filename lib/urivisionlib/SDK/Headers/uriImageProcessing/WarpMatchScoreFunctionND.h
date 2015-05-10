/*  NAME:
        WarpMatchScoreFunctionND.h
        
    DESCRIPTION:
        WarpMatchScoreFunctionND public header

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef URIVL_WARP_MATCH_SCORE_FUNCTION_ND_H
#define URIVL_WARP_MATCH_SCORE_FUNCTION_ND_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageWarper.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray.h"
#include "Blob.h"
#include "FunctionND.h"
#include "Pattern2D.h"
#include "PatternMatcher2D_corr.h"
#include "PatternMatcher2D_partCorr.h"
#include "PatternMatcher2D_scaledDiff.h"
#include "PatternMatcher2D_medianDiff.h"
#include "Pattern2DMatchRecord_part.h"


namespace uriVL
{
    /** Warping Matching Score Function nD.
     *
     *  This class exists for doing deformable (warping-based) pattern matching.
     *  It allows the user to define a reference pattern (image + blob) and a search
     *  image, and then compute the matching score for a given warping configuration
     *  (nD array of warping point coordinates) 
     */
    class WarpMatchScoreFunctionND : public FunctionND
    {
        public:

            /** Creates a new warping-based matching score function.'
             *  @param  matcherType  type of basic matcher this one is based on
             *  @param  warperType   type of image warper used by this object
             *  @param  gridRows    number of rows of the warping grid
             *  @param  gridCols    number of columns of the warping grid
             *  @param  innerMode    grid inner warping mode for this warper
             *  @param  outerMode    grid outer warping mode for this warper
             */
            WarpMatchScoreFunctionND(BasePatternMatcher2DType matcherType,
                                     ImageWarperType warperType,
                                     int gridRows, 
                                     int gridCols,
                                     InnerWarpingMode innerMode = INNER_FREE, 
                                     BorderWarpingMode outerMode = BORDER_FREE);

            /** Creates a new warping-based matching score function.'
             *  @param  predefined basic pattern matcher (e.g. correlation) to use
             *  @param  warperType   type of image warper used by this object
             *  @param  gridRows    number of rows of the warping grid
             *  @param  gridCols    number of columns of the warping grid
             *  @param  innerMode    grid inner warping mode for this warper
             *  @param  outerMode    grid outer warping mode for this warper
             */
            WarpMatchScoreFunctionND(PatternMatcher2D* matcher,
                                     ImageWarperType warperType,
                                     int gridRows, 
                                     int gridCols,
                                     InnerWarpingMode innerMode = INNER_FREE, 
                                     BorderWarpingMode outerMode = BORDER_FREE);

            /** Creates a new warping-based matching score function.'
             *  @param  matcherType  type of basic matcher this one is based on
             *  @param  predefined image warper (e.g. GL-based) to use
             */
            WarpMatchScoreFunctionND(BasePatternMatcher2DType matcherType,
                                     ImageWarper *warper);

            /** Creates a new warping-based matching score function.
             *  @param  predefined basic pattern matcher (e.g. correlation) to use
             *  @param  predefined image warper (e.g. GL-based) to use
             */
            WarpMatchScoreFunctionND(PatternMatcher2D* matcher,
                                     ImageWarper *warper);

            /**
             *  destructor
             */
            ~WarpMatchScoreFunctionND(void);
            

            /** Called to define the pattern used for subsequent matching score calculations
             *  @param  newPattern new pattern
             */
            void setPattern(const Pattern2D* newPattern);


            /** Called to define the pattern used for subsequent matching score calculations.
             *  @param  patImg     the rectangular pattern we are seaking
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             */
            void setPattern(const RasterImage* patImg, const Blob* patBlob = NULL);

            /** Called to define the image used for subsequent covariance calculations.
             *  Currently, the image's base raster type should match that of the pattern
             *  (which implies that the pattern should have been set before this function
             *  is called)
             *  @param  img         the image in which we seek a best match for the pattern
             *  @param  matchPoint  origin of the search
             *  @see setPattern
             */
            void setImage(const RasterImage* img, const ImagePoint* matchPoint);

            /** Returns a new grid of points allocated at the grid dimensions of this object's
             *  warper and initialized as a regular rectangular grid.  This function
             *  can only be called after the grid dimensions and input image have been
             *  set.  The user is responsible for deleting this grid when it is not 
             *  needed anymore.
             *  @return a new warping grid to be used with this warper
             */
            float*** const newWarpingGrid(void) const;

            /** Returns a this object's private grid of warping points.  This function
             *  should only be used for debugging purposes, to visualize the  allocated at the grid dimensions of this 
             *  warper and initialized as a regular rectangular grid.  This function
             *  can only be called after the grid dimensions and input image have been
             *  set.  The user is responsible for deleting this grid when it is not 
             *  needed anymore.
             *  @return a new warping grid to be used with this warper
             */
            float*** const getWarpingGrid(void) const;


            /** Returns a pointer to this object's warper
             *  @return     a pointer to this object's warper
             */
            ImageWarper *getWarper(void) const;
            
            /** Returns a pointer to the output image of the warper
             */
            RasterImage* getWarpedImage(void) const;


            /** Returns a new 1D array of float properly sized to serve as an argument
             *  for this nD function.  It is the user's responsibility to delete this
             *  array when it is not needed anymore.
             *  @return     a new array of float to pass as parameter to func
             */
            float*  newVarArray(void) const;
            
            
            void newVarArray(float* x) const;


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
                                            int* highImgWidth, int* highImgHeight) const;


            /** 
             *  Evaluates the function at the nD point specified.
             *
             *  @param  x   where the function should be evaluated
             *  @return     the value of the function at x
             */
            float func(const float* x);
            
            /** Evaluates the function at the nD point specified.
             *  @param  x   where the function should be evaluated
             *  @return     the value of the function at x
             */
           float func(const vector<float>& x);
           

        protected:

            /** Pattern matcher (of one of the basic types) used by this warping-based
             *  matcher
             *  @see BasePatternMatcher2DType
             */
            PatternMatcher2D* matcher_;

            /** Indicates whether _matcher is "owned" by this object or was passed to
             *  the constructor
             */
            bool matcherIsLocal_;
            
            /** warper
             */
            ImageWarper *warper_;
            
            /** Indicates whether _warper is "owned" by this object or was passed to
             *  the constructor
             */
            bool warperIsLocal_;


            /** Grid point coordinates used to pass warping commands to the warper.
             */
            float*** grid_;
            
            /** The warper's inner warping mode (stored for speedier func call)
             */
            InnerWarpingMode innerMode_; 
            
            /** The warper's border warping mode (stored for speedier func call)
             */
            BorderWarpingMode outerMode_;
                                         
                                                     
            /** number of rows of the grid (stored for speedier func call)
             */
            int gridRows_;
            
            /** number of cols of the grid (stored for speedier func call)
             */
            int gridCols_;
            

            /** input image (for warping)
             */
            const RasterImage* imgIn_;

            /** output image (for correlation)
             */
            RasterImage* imgOut_;

            /** pattern for correlation with img_
             */
            Pattern2D   *pat_;
            
            /** Base raster type of the pattern
             */
            BaseRasterType patRasterType_;
            
            
            /** Match record used for the results of the matcher
             */
            Pattern2DMatchRecord* matchRecord_;
            
            /** stores a 1D array of "pattern scaling factor" data needed by some
             *   pattern matchers
             */
            float* patScale1D_;
            
            /** stores a 1D array of "image scaling factor" data needed by some
             *   pattern matchers
             */
            float* imgScale1D_;
            
            /** stores a 2D array of "pattern scaling factor" data needed by some
             *   pattern matchers
             */
            float** patScale2D_;
            
            /** stores a 2D array of "image scaling factor" data needed by some
             *   pattern matchers
             */
            float** imgScale2D_;
            
            /** stores a 1D array of "image score" data needed by some
             *   pattern matchers
             */
            float* imgScore1D_;
            
            /** stores a 2D array of "image score" data needed by some
             *   pattern matchers
             */
            float** imgScore2D_;
            
            /** x coordinate of match point
             */
            int matchPointX_;

            /** y coordinate of match point
             */
            int matchPointY_;

            /** Deletes the grid of warping points stored by this object
             */
            void deleteGrid_(void);

            /** Computes the number of active grid point coordinates for the warping mode
             *  and grid dimensions chosen
             *
             */
            static int functionDim_(int gridRows, 
                                    int gridCols,
                                    InnerWarpingMode innerMode = INNER_FREE, 
                                    BorderWarpingMode outerMode = BORDER_FREE);
                                   
    };
}

#endif  //  WARP_MATCH_SCORE_FUNCTION_ND_H
