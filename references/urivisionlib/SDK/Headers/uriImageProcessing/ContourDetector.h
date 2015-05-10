/*  NAME:
        ContourDetector.h
 
    DESCRIPTION:
        ContourDetector public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CONTOUR_DETECTOR_H
#define URIVL_CONTOUR_DETECTOR_H

#include <list>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "FeatureDetector.h"
#include "Contour_F.h"
#include "RasterImage.h"

namespace uriVL
{
    /** Virtual parent class for all contour detector objects
     *  @author Clifford Manzanillo, 
     *          Department of Computer Science and Statistics
     *          University of Rhode Island 
     */
    class URIVL_EXPORT ContourDetector : public FeatureDetector
    {
        public:

			/**	 Constructor.
			 */
            ContourDetector(void);

			/**	 Constructor.
			 *	@param	rect	the rectangle on which image processing may be needed
			 */
            ContourDetector(const ImageRect* rect);

			/**	 Destructor.
			 */
            virtual ~ContourDetector(void){}

			/**	 Finds <b>one</b> contour within the specified search rectangle of the input image.
			 *	To be implemented by subclasses.
			 *	@param	imgIn	the image to process
			 *	@param	rect	the rectangle within which to look for a contour. 
			 *					If null, look over the entire image.
			 *	@return	a pointer to the contour object found in the image
			 */
            virtual Contour_F* findContour(const RasterImage* imgIn, const ImageRect* rect=NULL) = 0;


			/**	 Finds <b>all</b> contours within the specified search rectangle of the input image.
			 *	To be implemented by subclasses.
			 *	@param	imgIn	the image to process
			 *	@param	rect	the rectangle within which to look for a contour. 
			 *					If null, look over the entire image.
			 *	@return	a list of pointers to the contour objects found in the image
			 */
            virtual std::list<Contour_F*> * findAllContours(const RasterImage* imgIn, const ImageRect* rect=NULL) = 0;



		private:

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ContourDetector(const ContourDetector& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ContourDetector& operator =(const ContourDetector& obj);

			

    };
}

#endif  //  CONTOUR_DETECTOR_H
