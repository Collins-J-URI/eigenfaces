/*  NAME:
        FeatureDetector.h
 
    DESCRIPTION:
        FeatureDetector public header.(Not implemented yet)
        FeatureDetector is an abstract class (with pure virtual member function)
        Inherited by: BlobFinder, SegmentDetector, CornerDetector...
        
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_FEATURE_DETECTOR_H
#define URIVL_FEATURE_DETECTOR_H

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
#include "ImageFeature.h"

namespace uriVL
{
    
    /** FeatureDetector class.
     *
     */
    class URIVL_EXPORT FeatureDetector
    {
        public:
        
            /** Constructor.
             *  Initializes the instance variables
             */
            FeatureDetector(void);

            /** Constructor.  Defines the sesrch rectangle.  This version of the constructor
			 *	may be needed for 
             *  Initializes the instance variables
             *  @param  rect       defines the dimensions of the search
             */
            FeatureDetector(const ImageRect* rect);


            /** Destructor
             */
            virtual ~FeatureDetector(void);


			/**	Returns a pointer to the ImageRect on which this object is operating
			 *	@return a pointer to the ImageRect on which this object is operating
			 */
			inline const ImageRect* getSearchRect(void) const
			{
				return searchRect_;
			}
			
			/**	Assigns a (new) search rectangle to this object.
			 *	@param	rect	new search rectangle for this object
			 */
			void setSearchRect(const ImageRect* rect);

			/**	Find *one* image feature near the designated point
			 *	@param	img				the image in which to search for features 
			 *	@param	searchStart		start point for the search
			 *	@param	searchRect		the search rectangle.  If NULL, the image's entire valid rect is used
			 *	@return		pointer to the image feature found (NULL if none)
			 */
			virtual ImageFeature* findFeatureAbout(const RasterImage* img, const ImagePoint* searchStart,
												   const ImageRect* searchRect = NULL) = 0;
											
			/**	Find all image feature within a search rectangle
			 *	@param	img				the image in which to search for features 
			 *	@param	searchStart		start point for the search
			 *	@param	searchRect		the search rectangle.  If NULL, the image's entire valid rect is used
			 *	@return		pointer to the image feature found (NULL if none)
			 */
			virtual FeatureList* findAllFeatures(const RasterImage* img,
												   const ImageRect* searchRect = NULL) = 0;
											
		private:


			/**	Search rectangle on which this object operates
			 */
			const ImageRect* searchRect_;
			
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.
             *
             *  @param  obj   reference of the object to copy
             */
            FeatureDetector(const FeatureDetector& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled..
             *
             *  @param  obj   reference of the object to copy
             */
            const FeatureDetector& operator =(const FeatureDetector& obj);

			


    };
}

#endif  //  FEATURE_DETECTOR_H
