/*  NAME:
        ImageTransformation.h

    DESCRIPTION:
        ImageTransformation public header.

    COPYRIGHT:
        (c) 2013-2014, 3D Group for Interactive Visualization
					University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_TRANSFORMATION_H
#define URIVL_IMAGE_TRANSFORMATION_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"
#include "ImageRect.h"

namespace uriVL
{
	/**	This class only contains static functions to rotate, scale, skew, and translate images.
	 *	The constructors and = operator are private and should not be implemented.
	 */
    class URIVL_EXPORT ImageTransformation
	{
		public:
		
			/**	Produces a new RasterImage object that is obtained by translating the in put
			 *	image by the amounts specified.  In practice, in RI this is done by translating
			 *	the bounding and valid data rects of the destination image.
			 *	@param	imgIn	the image to apply the translation to
			 *	@param	dx		amount of translation along the horizontal axis
			 *	@param	dy		amount of translation along the vertical axis
			 *	@return		pointer to the translated image produced;
			 */
			static RasterImage* translate(const RasterImage* imgIn, int dx, int dy);

			/**	Applies in-place translation to the image received as parameter
			 *	by the amounts specified.  In practice, in RI this is done by translating
			 *	the bounding and valid data rects of the image.
			 *	@param	imgIn	the image to apply the translation to
			 *	@param	dx		amount of translation along the horizontal axis
			 *	@param	dy		amount of translation along the vertical axis
			 */
			static void translateInPlace(RasterImage* imgIn, int dx, int dy);

			/**	Produces a new RasterImage object that is obtained by scaling the input
			 *	image by the amounts specified and from the origin specified.  Note that:
			 *	<ul>
			 *		<li> If no origin is pecified, or if a NULL pointer is passed for the origin, 
			 *			then the scaling is done from (0, 0).</li>
			 *		<li> Negative scaling along a direction will result in flipping the image
			 *			along that direction.</li>
			 *		<li> Dimensions of the scaled image are converted to nearest &le; integer,
			 *			so that any image produced by this function will be at least 1 x 1.</li>
			 *	</ul>
			 *
			 *	@param	imgIn	the image to apply the scaling to
			 *	@param	sx		amount of scaling along the horizontal axis
			 *	@param	sy		amount of scaling along the vertical axis
			 *	@param  origin	pointer to the point from which the scaling will be perfomed
			 *	@return		pointer to the scaled image produced;
			 */
			static RasterImage* scale(const RasterImage* imgIn, float sx, float sy, const ImagePoint* origin = NULL);

			/**	Produces a new RasterImage object that is obtained by rotating the input
			 *	image by the angle specified and around the centerspecified.  Note that:
			 *	<ul>
			 *		<li> If no center is pecified, or if a NULL pointer is passed for the center, 
			 *			then the rotatio is performed around the geometric center of the 
			 *			image's valid data rectangle.</li>
			 *		<li> Rotation angles are measured counterclockwise in degree.</li>
			 *		<li> Dimensions of the rotated image are converted to nearest &le; integer.</li>
			 *	</ul>
			 *	Note that for For 1-bit images (such as RasterImage_binary objects), the only 
			 *	admissible values for the rotation angle are multiple of 90° (e.g. –90, 90,
			 *	180, 270). A NULL value is returned for other angles.
			 *
			 *	@param	imgIn	the image to apply the rotation to
			 *	@param	angle	counterclockwise rotation angle in degree
			 *	@param  center	pointer to the point around which the rotation will be perfomed
			 *	@return		pointer to the rotated image produced;
			 */
			static RasterImage* rotate(const RasterImage* imgIn, float angle, const ImagePoint* center = NULL);


		
		private:

			/**	 Disabled default constructor
			 */
			ImageTransformation(void) { }
			
			/**	 Disabled copy constructor
			 *	@param	obj		the object <b>not</b> to copy
			 */
			ImageTransformation(const ImageTransformation& obj) { }
			
			~ImageTransformation(void) { }
			
			/**	 Disabled copy operator
			 *	@param	obj		the object <b>not</b> to copy
			 *	@return	should not
			 */
			const ImageTransformation& operator = (const ImageTransformation& obj) { return *this; }
			
	};
}


#endif	//	URIVL_IMAGE_TRANSFORMATION_H
