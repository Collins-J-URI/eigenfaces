/*  NAME:
        PatternMatcher2D_corr.h
 
    DESCRIPTION:
        PatternMatcher2D_corr public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_MATCHER_2D_CORR_H
#define URIVL_PATTERN_MATCHER_2D_CORR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Blob.h"
#include "RasterImage.h"
#include "Pattern2D.h"
#include "ImagePoint.h"
#include "ImageRect.h"
#include "PatternMatcher2D.h"

using namespace std;

namespace uriVL
{
    /** One of the simplest possible subclasses of PatternMatcher2D.  Selects the
     *  best match based on the correlation coefficient of the pattern and
     *  of the image
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class PatternMatcher2D_corr : public PatternMatcher2D 
    {
        public:
        
            /** Default constructor.
             *  This constructor does not allocate temporary storage rasters.
             */
            PatternMatcher2D_corr(void);
            
            /** Destructor. Does nothing
             */         
            ~PatternMatcher2D_corr(void);

            /** Called to define the pattern used for subsequent covariance calculations.
             *  This function is called by the match_ function.  It (and setImage) should
             *  be called prior to a call to covariance().
             *  Currently only RGBa and gray-level patterns are accepted.
             *  @param  patImg     the rectangular pattern we are seaking
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             */
            void setPattern(const RasterImage* patImg, const Blob* patBlob=NULL);


            /** Called to define the pattern used for subsequent covariance calculations
             *  This function is called by the match_ function.  It (and setImage) should
             *  be called prior to a call to covariance().
             *  Currently only RGBa and gray-level patterns are accepted.
             *  @param  pattern    the pattern we are seaking
             */
            void setPattern(const Pattern2D* pattern);


            /** Computes the correlation score between the pattern stored and 
             *  an image at the location indicated
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            void covariance(const RasterImage_RGBa* img, int x, int y, float* cov, 
                            float* corrScale);

            /** Computes the correlation score between the pattern stored and 
             *  an image at the location indicated
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            void covariance(const RasterImage_gray* img, int x, int y, float* cov, 
                            float* corrScale);

            /** Computes the correlation scaling factor and expected color intensity values
             *  for the pattern.  The expected values are kept stored in this object and will
             *  be used to compute the variance.  The scale factor should be used to 
             *  multiply the result of the covariance if the correlation coefficient is 
             *  desired.
             *  
             *  @param  patImg     the rectangular image pattern to compute the scale factor for.
             *  @param  patCorrScale  the correlation scaling factors for the pattern
             */             
            void patternCorrScale(const RasterImage_RGBa* patImg, float* patCorrScale);

            /** Computes the correlation scaling factor and expected gray intensity value
             *  for the pattern.  The expected value is kept stored in this object and will
             *  be used to compute the variance.  The scale factor should be used to 
             *  multiply the result of the covariance if the correlation coefficient is 
             *  desired.
             *  
             *  @param  patImg     the rectangular image pattern to compute the scale factor for.
             *  @param  patCorrScale  the correlation scaling factor for the pattern
             */             
            void patternCorrScale(const RasterImage_gray* patImg, float* patCorrScale);

            /** Computes the correlation scaling factor and expected color intensity values
             *  for the pattern.  The expected values are kept stored in this object and will
             *  be used to compute the variance.  The scale factor should be used to 
             *  multiply the result of the covariance if the correlation coefficient is 
             *  desired.
             *  
             *  @param  patImg     the blob image pattern to compute the scale factor for.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patCorrScale  the correlation scaling factors for the pattern
             */
            void patternCorrScale(const RasterImage_RGBa* patImg, const Blob* patBlob,  
                                  float* patCorrScale);

            /** Computes the correlation scaling factor and expected gray intensity value
             *  for the pattern.  The expected value is kept stored in this object and will
             *  be used to compute the variance.  The scale factor should be used to 
             *  multiply the result of the covariance if the correlation coefficient is 
             *  desired.
             *  
             *  @param  patImg     the blob image pattern to compute the scale factor for.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patCorrScale  the correlation scaling factor for the pattern
             */             
             void patternCorrScale(const RasterImage_gray* patImg, const Blob* patBlob,
                                   float* patCorrScale);

        protected:
        
            /** Expected value(s) of the pattern raster.  If the pattern has a gray-level image,
             *  the array sotres asingle element
             */
            float* patExp_;
            
            /** Determines the location and score of the best match for a target
             *  pattern in an rgba image.  The correlation computed is the average 
             *  of that of the three color components.  The search region is specified by 
             *  an image rectangle.  The pattern is a (small) rectangular image patch<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patImg          the rectangular image pattern to search for
             *                              in <code>img</code>.
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             */
            void match_(const RasterImage_RGBa* patImg, const ImageRect* searchRect, 
                        const RasterImage_RGBa* img, Pattern2DMatchRecord* matchRecord);

            /** Determines the location and score of the best match for a target
             *  pattern in a gray-level image.  The search region is specified by 
             *  an image rectangle.  The pattern is a (small) rectangular image patch<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patImg          the rectangular image pattern to search for
             *                              in <code>img</code>.
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             */
            void match_(const RasterImage_gray* patImg, const ImageRect* searchRect, 
                        const RasterImage_gray* img, Pattern2DMatchRecord* matchRecord);

            /** Determines the location and score of the best match for a target
             *  pattern in an rgba image.  The correlation computed is the average 
             *  of that of the three color components.  The search region is specified by 
             *  an image rectangle.  The pattern is a blob defined over a (small) rectangular
             *  image patch<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patImg          the rectangular image pattern to search for
             *                              in <code>img</code>.
             *  @param  patBlob         the blob that defines the actual geometry of the
             *                              pattern
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             */
            void match_(const RasterImage_RGBa* patImg, const Blob* patBlob, 
						const ImageRect* searchRect,
                        const RasterImage_RGBa* img, Pattern2DMatchRecord* matchRecord);

            /** Determines the location and score of the best match for a target
             *  pattern in a gray-level image.  The search region is specified by an image
             *  rectangle.  The pattern is a blob defined over a (small) rectangular
             *  image patch<p>
             *  If the search region selected extends out of the image's valid
             *  data rectangle, then a smaller effective search region is used instead.<p>
             *  Note for developers of stereo or motion matchers: the origin of
             *  pattern is in its upper-left corner. You will need to translate the
             *  best match found by half the dimensions of the target pattern if you
             *  want to do point-to-point matching (disparity)
             *
             *  @param  patImg          the rectangular image pattern to search for
             *                              in <code>img</code>.
             *  @param  patBlob         the blob that defines the actual geometry of the
             *                              pattern
             *  @param  searchRect      the region of the search
             *  @param  img             the image in which to search for a best match
             *  @param  matchRecord     Pattern2DMatchRecord of the best match found
             */
            void match_(const RasterImage_gray* patImg, const Blob* patBlob, 
						const ImageRect* searchRect,
                        const RasterImage_gray* img, Pattern2DMatchRecord* matchRecord);


        private:
                     
            /** Computes the correlation score between a rectangular pattern and an image at
             *  the location indicated
             *  @param  rgbaImg    2D RGBa raster of the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            void covarianceRGBa_(const unsigned char* const* rgbaImg, int x, int y, float* cov, 
                                float* corrScale);

            /** Computes the correlation score between a rectangular pattern and an image at
             *  the location indicated
             *  @param  patRows    height of the pattern
             *  @param  patCols    width of the pattern
             *  @param  grayImg    2D gray raster of the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            void covarianceGray_(const unsigned char* const* grayImg, int x, int y, float* cov, 
                                float* corrScale);

            /** Computes the correlation score between a blob pattern and an image at
             *  the location indicated
             *  @param  rgbaImg    2D RGBa raster of the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            void covarianceRGBaBlob_(const unsigned char* const* rgbaImg, int x, int y, float* cov, 
                                     float* corrScale);

            /** Computes the correlation score between a blob pattern and an image at
             *  the location indicated
             *  @param  grayImg    2D gray raster of the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            void covarianceGrayBlob_(const unsigned char* const* grayImg, int x, int y, float* cov, 
                                     float* corrScale);

            /** Computes the correlation scaling factor and expected color intensity values
             *  for the pattern.  The expected values are to be used by the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the rectangular image pattern to compute the scale factor for.
             *  @param  patCorrScale  the correlation scaling factors for the pattern
             */             
            void patternCorrScale_(const RasterImage_RGBa* patImg, float* patCorrScale);

            /** Computes the correlation scaling factor and expected gray intensity value
             *  for the pattern.  The expected value is to be used by the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the rectangular image pattern to compute the scale factor for.
             *  @param  patCorrScale  the correlation scaling factor for the pattern
             */             
            void patternCorrScale_(const RasterImage_gray* patImg, float* patCorrScale);

            /** Computes the correlation scaling factor and expected color intensity values
             *  for the pattern.  The expected values are to be used by the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the blob image pattern to compute the scale factor for.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patCorrScale  the correlation scaling factors for the pattern
             */
            void patternCorrScale_(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                   float* patCorrScale);

            /** Computes the correlation scaling factor and expected gray intensity value
             *  for the pattern.  The expected value is to be used by the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the blob image pattern to compute the scale factor for.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patCorrScale  the correlation scaling factor for the pattern
             */             
             void patternCorrScale_(const RasterImage_gray* patImg, const Blob* patBlob,
                                    float* patCorrScale);

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

            /** Allocates private computation rasters for the pattern and the image 
             *  @param  patImg  pointer to the pattern's image to allocate rasters for
             *  @param  patBlob pointer to the pattern's blob (possibly NULL)
             */
            void allocateRasters_(const RasterImage* patImg, const Blob* patBlob);

            /** Deletes the private computation rasters
             */
            void deleteRasters_(void);


    };
}
#endif  //  PATTERN_MATCHER_2D_CORR_H
