/*  NAME:
        BackgroundRemover.h
 
    DESCRIPTION:
        BackgroundRemover public header.
		BackgroundRemover is an abstract class (with pure virtual member function)
     	Inherited by BackgroundRemover_static, BackgroundRemover_dynamic 
     	
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_ARITHMETICS_H
#define URIVL_IMAGE_ARITHMETICS_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_RGBa_F.h"
#include "RasterImage_HSV.h"

namespace uriVL
{
	/**	Defines the three possible modes for computing image differences between
	 *	color images:
	 *	<ul>
	 *		<li><code>AVERAGE_DIFFERENCE</code> -- average of the differences in the
	 *					r, g, and b channels
	 *		<li><code>MAX_DIFFERENCE</code> -- maximum of the differences in the
	 *					r, g, and b channels
	 *		<li><code>HUE_DIFFERENCE</code> -- hue difference rescaled to 0-255
	 *	</ul>
	 *	Other modes will be added as more image types are implemented (e.g. L*a*b).
	 *	<p>
	 *	Note that image differences are always positive <code>unsigned char</code>
	 */
    typedef enum    ColorDifferenceMode{
    										AVERAGE_DIFFERENCE,
        									MAX_DIFFERENCE,
        									HUE_DIFFERENCE
    } ColorDifferenceMode;	
    
    
    /** ImageArithmetics is a class made up of static functions
     */
	class URIVL_EXPORT ImageArithmetics
	{
		public:

			/**	Computes the difference between two raster images
			 *	@param	img1	first input image
			 *	@param	img2	second input image
			 *	@param	mode	indicates the algorithm used to compute the difference between two
			 *					color values
			 *	@return		the difference |img1 - img2|
			 */
			static RasterImage_gray* difference(RasterImage* img1, RasterImage* img2, 
			                             	    ColorDifferenceMode mode = MAX_DIFFERENCE);
		
			/**	Computes the difference between two raster images into a third one received
			 *	as parameter
			 *	@param	img1	first input image
			 *	@param	img2	second input image
			 *	@param	mode	indicates the algorithm used to compute the difference between two
			 *					color values
			 *	@param	imgOut	the difference |img1 - img2|
			 */
			static void difference(RasterImage* img1, RasterImage* img2, RasterImage_gray* imgOut, 
				                	ColorDifferenceMode mode = MAX_DIFFERENCE);

        private:
            ImageArithmetics(void);
            
            ~ImageArithmetics(void);

			/**	Computes the difference between two "integer" gray-level images into an
			 *	"integer" gray-level image.  If the destination has a float raster,
			 *	it will not be the reference anymore
			 *	@param	img1	first input image
			 *	@param	img2	second input image
			 *	@param	imgOut	destination for the difference |img1 - img2|
			 */
			static void difference_(RasterImage_gray* img1, RasterImage_gray* img2, 
									RasterImage_gray* imgOut);

			/**	Computes the difference between two "float" gray-level images into an 
			 *	"integer" gray-level image.
			 *	@param	img1	first input image
			 *	@param	img2	second input image
			 *	@param	imgOut	destination for the difference |img1 - img2|
			 */
			static void difference_(RasterImage_gray_F *img1, RasterImage_gray_F *img2, 
									RasterImage_gray* imgOut);

			/**	Computes the difference between two "float" gray-level images into a "float"
			 *	gray-level image
			 *	@param	img1	first input image
			 *	@param	img2	second input image
			 *	@param	imgOut	destination for the difference |img1 - img2|
			 */
			static void difference_(RasterImage_gray_F *img1, RasterImage_gray_F *img2, 
									RasterImage_gray_F *imgOut);

			/**	Computes the difference between "float" and an "integer" gray-level images
			 *	into a "float" gray-level image
			 *	@param	img1	first "float" input image
			 *	@param	img2	second "integer" input image
			 *	@param	imgOut	destination for the difference |img1 - img2|
			 */
			static void difference_(RasterImage_gray_F *img1, RasterImage_gray* img2, 
									RasterImage_gray_F *imgOut);

			/**	Computes the difference between "float" and an "integer" gray-level images
			 *	into an "integer" gray-level image
			 *	@param	img1	first "float" input image
			 *	@param	img2	second "integer" input image
			 *	@param	imgOut	destination for the difference |img1 - img2|
			 */
			static void difference_(RasterImage_gray_F *img1, RasterImage_gray* img2, 
									RasterImage_gray* imgOut);

			/**	Computes the difference between two RGB color images into an
			 *	"integer" gray-level image.  If the destination has a float raster,
			 *	it will not be the reference anymore
			 *	@param	img1	first input image
			 *	@param	img2	second input image
			 *	@param	imgOut	destination for the difference |img1 - img2|
			 *	@param	mode	indicates the algorithm used to compute the difference between two
			 *					color values
			 */
			static void difference_(RasterImage_RGB* img1, RasterImage_RGB* img2, 
									RasterImage_gray* imgOut, 
									ColorDifferenceMode mode);

			/**	Computes the difference between two RGBa color images into an
			 *	"integer" gray-level image.  If the destination has a float raster,
			 *	it will not be the reference anymore
			 *	@param	img1	first input image
			 *	@param	img2	second input image
			 *	@param	imgOut	destination for the difference |img1 - img2|
			 *	@param	mode	indicates the algorithm used to compute the difference between two
			 *					color values
			 */
			static void difference_(RasterImage_RGBa* img1, RasterImage_RGBa* img2, 
									RasterImage_gray* imgOut, 
									ColorDifferenceMode mode);	

    };			
}

#endif  //  IMAGE_ARITHMETICS_H
