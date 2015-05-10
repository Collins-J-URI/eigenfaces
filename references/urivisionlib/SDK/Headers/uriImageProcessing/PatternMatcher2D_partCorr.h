/*  NAME:
        PatternMatcher2D_partCorr.h
 
    DESCRIPTION:
        PatternMatcher2D_partCorr public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_PATTERN_MATCHER_2D_PART_CORR_H
#define URIVL_PATTERN_MATCHER_2D_PART_CORR_H

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

namespace uriVL
{
    /** Hopefully occlusion-resistant subclass of PatternMatcher2D.   
     *  This is not going to be the final version of this class.  If this partial
     *  matching business works, I intend to create a MatchingParams class
     *  that would store all sorts of parameters specifying conditions for
     *  the match process (e.g., number of division of the partial match,, mask
     *  for occlusion by other tracked objects, etc.).  So this one is just
     *  a quick coding with a hard-set 2x2 division into quadrants.
     *  
     *  As I was about half way through implementing this class, Meng and I 
     *  did a lot of changes into the "regular" correlation matcher.  The current
     *  coding of this class is a Q&D attempt at somehow matching the structure of
     *  the other class.  I am not particularly fond of the difference between
     *  parameter signatures for RGBa and gray patterns.  I also regret using the
     *  color as the first index of my expected values and covariance arrays
     *  [0=red, 1=green, 2=blue], because that means that the decomposition of the
     *  pattern into quadrants [5 indices: 0=whole, 1=UL, 2=UR, 3=LL, 4=LR] is
     *  explicitly stated in the API.  
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class PatternMatcher2D_partCorr : public PatternMatcher2D 
    {
        public:
        
            /** Default constructor
             */
            PatternMatcher2D_partCorr(void);
            
            /** Destructor. Does nothing
             */         
            ~PatternMatcher2D_partCorr(void);


               /** Computes the correlation score between a rectangular pattern and an image at
             *  the location indicated
             *  @param  pattern    the rectangular pattern we are seaking
             *  @param  patExp     expected values of the pattern's  R, G, B components
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals for the pattern's
             *                          R, G, B components (first index) for the whole pattern 
             *                      and each of its quadrants (second index: whole, UL, UR, LL, LR)
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            static void covariance(const RasterImage_RGBa* patImg, float patExp[][5], 
                                    const RasterImage_RGBa* img, int x, int y, float cov[][5], 
                                    float corrScale[][5]);

            /** Computes the correlation score between a rectangular pattern and an image at
             *  the location indicated
             *  @param  pattern    the rectangular pattern we are seaking
             *  @param  patExp     expected values of the pattern's intensity
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals for the whole pattern 
             *                          and each of its quadrants (whole, UL, UR, LL, LR)
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            static void covariance(const RasterImage_gray* patImg, float* patExp, 
								   const RasterImage_gray* img, 
                                   int x, int y, float* cov, float* corrScale);

            /** Computes the correlation score between a blob pattern and an image at
             *  the location indicated
             *  @param  patImg     the rectangular image pattern to search for
             *                     in img.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patExp     expected values of the pattern's  R, G, B components
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals for the pattern's
             *                          R, G, B components (first index) for the whole pattern 
             *                      and each of its quadrants (second index: whole, UL, UR, LL, LR)
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            static void covariance(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                    float patExp[][5], const RasterImage_RGBa* img, int x, int y, 
                                    float cov[][5], float corrScale[][5]);

            /** Computes the correlation score between a blob pattern and an image at
             *  the location indicated
             *  @param  patImg     the rectangular image pattern to search for
             *                     in img.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patExp     expected values of the pattern's intensity
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals for the whole pattern 
             *                          and each of its quadrants (whole, UL, UR, LL, LR)
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            static void covariance(const RasterImage_gray* patImg, const Blob* patBlob, 
                                    float* patExp, const RasterImage_gray* img, int x, int y, 
                                    float* cov, float* corrScale);

            /** Computes the correlation scaling factor and expected color intensity values
             *  for the pattern.  The expected values are to be sent to the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the rectangular image pattern to compute the scale factor for.
             *  @param  patExp     expected values of the pattern's  R, G, B components
             *                      (first index) for the whole pattern and each of its 
             *                      quadrants (second index: whole, UL, UR, LL, LR)
             *  @param  patCorrScale  the correlation scaling factors for the pattern's
             *                          R, G, B components (first index) for the whole pattern 
             *                      and each of its quadrants (second index: whole, UL, UR, LL, LR)
             */             
            static void patternCorrScale(const RasterImage_RGBa* patImg, float patExp[][5], 
                                                float patCorrScale[][5]);

            /** Computes the correlation scaling factor and expected gray intensity value
             *  for the pattern.  The expected value is to be sent to the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the rectangular image pattern to compute the scale factor for.
             *  @param  patExp     expected values of the pattern's intensity for the whole 
             *                     pattern and each of its quadrants (whole, UL, UR, LL, LR)
             *  @param  patCorrScale  the correlation scaling factor for the whole pattern 
             *                          and each of its quadrants (whole, UL, UR, LL, LR)
             */             
            static void patternCorrScale(const RasterImage_gray* patImg, float* patExp, 
                                                float* patCorrScale);

            /** Computes the correlation scaling factor and expected color intensity values
             *  for the pattern.  The expected values are to be sent to the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the blob image pattern to compute the scale factor for.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patExp     expected values of the pattern's  R, G, B components
             *                      (first index) for the whole blob and each of its 
             *                      quadrants (second index: whole, UL, UR, LL, LR)
             *  @param  patCorrScale  the correlation scaling factors for the pattern
             */
            static void patternCorrScale(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                             float patExp[][5], float patCorrScale[][5]);

            /** Computes the correlation scaling factor and expected gray intensity value
             *  for the pattern.  The expected value is to be sent to the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the blob image pattern to compute the scale factor for.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patExp     expected values of the pattern's intensity for the whole 
             *                     pattern and each of its quadrants (whole, UL, UR, LL, LR)
             *  @param  patCorrScale  the correlation scaling factor for the whole pattern 
             *                          and each of its quadrants (whole, UL, UR, LL, LR)
             */             
             static void patternCorrScale(const RasterImage_gray* patImg, const Blob* patBlob,
                                             float* patExp, float* patCorrScale);


    protected:
        
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
            void match_(const RasterImage_RGBa* patImg, const Blob* patBlob, const ImageRect* searchRect,
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
            void match_(const RasterImage_gray* patImg, const Blob* patBlob, const ImageRect* searchRect,
                        const RasterImage_gray* img, Pattern2DMatchRecord* matchRecord);

            /** Computes the correlation score between a rectangular pattern and an image at
             *  the location indicated
             *  @param  pattern    the rectangular pattern we are seaking
             *  @param  patExp     expected values of the pattern's  R, G, B components
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals for the pattern's
             *                          R, G, B components (first index) for the whole pattern 
             *                      and each of its quadrants (second index: whole, UL, UR, LL, LR)
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            static void covariance_(const RasterImage_RGBa* patImg, float patExp[][5], 
                                    const RasterImage_RGBa* img, int x, int y, float cov[][5], 
                                    float corrScale[][5]);

            /** Computes the correlation score between a rectangular pattern and an image at
             *  the location indicated
             *  @param  pattern    the rectangular pattern we are seaking
             *  @param  patExp     expected values of the pattern's intensity
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals for the whole pattern 
             *                          and each of its quadrants (whole, UL, UR, LL, LR)
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            static void covariance_(const RasterImage_gray* patImg, float* patExp, 
									const RasterImage_gray* img, 
									int x, int y, float* cov, float* corrScale);

            /** Computes the correlation score between a blob pattern and an image at
             *  the location indicated
             *  @param  patImg     the rectangular image pattern to search for
             *                     in img.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patExp     expected values of the pattern's  R, G, B components
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals for the pattern's
             *                          R, G, B components (first index) for the whole pattern 
             *                      and each of its quadrants (second index: whole, UL, UR, LL, LR)
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            static void covariance_(const RasterImage_RGBa* patImg, const Blob* patBlob, 
                                    float patExp[][5], const RasterImage_RGBa* img, int x, int y, 
                                    float cov[][5], float corrScale[][5]);

            /** Computes the correlation score between a blob pattern and an image at
             *  the location indicated
             *  @param  patImg     the rectangular image pattern to search for
             *                     in img.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patExp     expected values of the pattern's intensity
             *  @param  img        the image in which to search for a best match
             *  @param  x          horizontal coordinate of where to compute a score
             *  @param  y          vertical  coordinate of where to compute a score
             *  @param  cov        covariance of the two signals for the whole pattern 
             *                          and each of its quadrants (whole, UL, UR, LL, LR)
             *  @param  corrScale  scaling factor for the image region.  The covariance should
             *                      be mutiplied by this number and by the scaling factor
             *                      for the pattern returned by patternCorrScale
             */
            static void covariance_(const RasterImage_gray* patImg, const Blob* patBlob, 
                                    float* patExp, const RasterImage_gray* img, int x, int y, 
                                    float* cov, float* corrScale);

            /** Computes the correlation scaling factor and expected color intensity values
             *  for the pattern.  The expected values are to be sent to the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the rectangular image pattern to compute the scale factor for.
             *  @param  patExp     expected values of the pattern's  R, G, B components
             *                      (first index) for the whole pattern and each of its 
             *                      quadrants (second index: whole, UL, UR, LL, LR)
             *  @param  patCorrScale  the correlation scaling factors for the pattern's
             *                          R, G, B components (first index) for the whole pattern 
             *                      and each of its quadrants (second index: whole, UL, UR, LL, LR)
             */             
            static void patternCorrScale_(const RasterImage_RGBa* patImg, float patExp[][5], 
										  float patCorrScale[][5]);

            /** Computes the correlation scaling factor and expected gray intensity value
             *  for the pattern.  The expected value is to be sent to the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the rectangular image pattern to compute the scale factor for.
             *  @param  patExp     expected values of the pattern's intensity for the whole 
             *                     pattern and each of its quadrants (whole, UL, UR, LL, LR)
             *  @param  patCorrScale  the correlation scaling factor for the whole pattern 
             *                          and each of its quadrants (whole, UL, UR, LL, LR)
             */             
            static void patternCorrScale_(const RasterImage_gray* patImg, float* patExp, 
										  float* patCorrScale);

            /** Computes the correlation scaling factor and expected color intensity values
             *  for the pattern.  The expected values are to be sent to the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the blob image pattern to compute the scale factor for.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patExp     expected values of the pattern's  R, G, B components
             *                      (first index) for the whole blob and each of its 
             *                      quadrants (second index: whole, UL, UR, LL, LR)
             *  @param  patCorrScale  the correlation scaling factors for the pattern
             */
            static void patternCorrScale_(const RasterImage_RGBa* patImg, const Blob* patBlob, 
										  float patExp[][5], float patCorrScale[][5]);

            /** Computes the correlation scaling factor and expected gray intensity value
             *  for the pattern.  The expected value is to be sent to the covariance
             * computation function.  The scale factor should be used to multiply the
             * result of the covariance if the correlation coefficient is desired.
             *  
             *  @param  patImg     the blob image pattern to compute the scale factor for.
             *  @param  patBlob    the blob that defines the actual geometry of the
             *                              pattern
             *  @param  patExp     expected values of the pattern's intensity for the whole 
             *                     pattern and each of its quadrants (whole, UL, UR, LL, LR)
             *  @param  patCorrScale  the correlation scaling factor for the whole pattern 
             *                          and each of its quadrants (whole, UL, UR, LL, LR)
             */             
             static void patternCorrScale_(const RasterImage_gray* patImg, const Blob* patBlob,
										   float* patExp, float* patCorrScale);

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


    };
}

#endif  //  PATTERN_MATCHER_2D_CORR_H
