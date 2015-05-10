/*  NAME:
        BackgroundRemover_static.h
 
    DESCRIPTION:
        BackgroundRemover_static public header.
        (Abstract) Base class: BackgroundRemover
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_BACKGROUND_REMOVER_STATIC_H
#define URIVL_BACKGROUND_REMOVER_STATIC_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "BackgroundRemover.h"
#include "RasterImage_gray_F.h"


namespace uriVL
{

    /**	Image operator that computes differences between images.
     *
     *	The BackgroundRemover_static class operates either in static mode (compute the
     *	difference between to raster images) or in "instance" mode" (compute the
     *	difference between an image and a "background" image stored by the
     *	BackgroundRemover_static object.
	 *	<p>
	 *	Note that image differences are always positive <code>unsigned char</code>
	 *
	 *	The basic rules for image differences are as follows
	 *	<ol>
	 *		<li> The output image is a <code>RasterImage_gray</code> (possibly a
	 *				<code>RasterImage_gray_F</code>);
	 *		<li> If one of the two images has a "<code>float</code>" raster as reference,
	 *				then that raster will be use to compute the difference;
	 *		<li> If the difference function must <i>return</i> an image (i.e. the output
	 *				image is not passed as a parameter), then if the difference function
	 *				calculates a "<code>float</code>" difference, the raster returned will
	 *				be of type <code>RasterImage_gray_F</code>;
	 *		<li> If one of the two input images is a gray raster, the other image will be
	 *				converted to gray before computing the difference;
	 *		<li> If one image is RGB and the other RGBa, then the former will be converted
	 *				to RGBA before computing the difference.
	 *	</ol>
	 *
	 *	<b>Very Important Note Regarding "Float" Images"</b><br>
	 *  If an image has a float raster as reference, then that raster will be used to 
	 *	calculate the image difference.  This can pose problems for float images that
	 *	are not using the range 0-255.  There are two workarounds for that problem:
	 *	<ul>
	 *		<li> rescale the raster's values to the desired range using
	 *				<code>RasterImage::scaleValues(minVal, maxVal)</code>
	 *		<li> call in sequence
	 *			<ul><li><code>unsigned char** gray2D = myFloatImage->getGray2D()</code> (forces float to char conversion)
	 *			<li><code>unsigned char** gray2D = myFloatImage->setGWorldAsReference(true)</code>
	 *			</ul>
	 *	</ul>
	 *	Using the first solution, a <code>float</code> difference will be calculated while
	 *  the second solution will give an "integer" difference.
	 *
	 *	@author		jyh and Fang Han
	 *	@version	1.1
	 *
	 *	(Abstract)Base class: BackgroundRemover
     */
	class URIVL_EXPORT BackgroundRemover_static : public BackgroundRemover
	{
		public:

			/**	Default constructor
			 */
			BackgroundRemover_static(void);

			/**	Destructor
			 */
			virtual ~BackgroundRemover_static(void);


			/**	Overided virtual func
			 *
			 *	Stores a copy of img to use as background for future image difference
			 *	computations
			 *
			 *	@param	img		raster image to store as background image
			 */			
			void setBackground(RasterImage* img);
			
	};
}

#endif	//	BACKGROUND_REMOVER_STATIC_H
