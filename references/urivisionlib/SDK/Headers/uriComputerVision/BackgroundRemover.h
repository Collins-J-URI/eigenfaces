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
#ifndef URIVL_BACKGROUND_REMOVER_H
#define URIVL_BACKGROUND_REMOVER_H

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
#include "ImageArithmetics.h"


namespace uriVL
{
    /**	BackgroundRemover is an abstract class (with pure virtual member function)
     *	Inherited by BackgroundRemover_static, BackgroundRemover_dynamic
     */
	class URIVL_EXPORT BackgroundRemover
	{
		public:

			/**	Default constructor
			 */
			BackgroundRemover(void);

			/**	Destructor
			 */
			virtual ~BackgroundRemover(void);
			
			
			/**	Different implementations between BackgroundRemover_static & BackgroundRemover_dynamic
			 *
			 *	Stores a copy of img to use as background for future image difference
			 *	computations
			 *	
			 *	@param	img		raster image to store as background image
			 */			
			virtual void setBackground(RasterImage* img) = 0;

			/**	Computes the difference between this raster image and the one stored
			 *	as background (using the default mode if the input images are color images)
			 *	@param	imgIn	input image
			 *	@return		the difference |imgIn - background|
			 */
			RasterImage_gray* removeBackground(RasterImage* imgIn);

			/**	Computes the difference between this raster image and the one stored
			 *	as background
			 *	@param	imgIn	input image
			 *	@param	mode	indicates the algorithm used to compute the difference between two
			 *					color values
			 *	@return		the difference |imgIn - background|
			 */
			RasterImage_gray* removeBackground(RasterImage* imgIn, ColorDifferenceMode mode);

			/**	Computes the difference between this raster image and the one stored
			 *	as background (using the default mode if the input images are color images)
			 *	into a gray-level raster image received as parameter
			 *	@param	imgIn	input image
			 *	@param	imgOut	the difference |imgIn - background|
			 */
			void removeBackground(RasterImage* imgIn, RasterImage_gray* imgOut);

			/**	Computes the difference between this raster image and the one stored
			 *	as background into a gray-level raster image received as parameter
			 *	@param	imgIn	input image
			 *	@param	mode	indicates the algorithm used to compute the difference between two
			 *					color values
			 *	@param	imgOut	the difference |imgIn - background|
			 */
			void removeBackground(RasterImage* imgIn, RasterImage_gray* imgOut, 
			                        ColorDifferenceMode mode);
			
			/**	Sets the default color difference mode for this object
			 *	@see mode_
			 */
			void setColorDifferenceMode(ColorDifferenceMode mode);

			/**
			 *	get background_;
			 */
			RasterImage* getBackground(void);

            /**Reports what is the color difference mode for this object
             *
             *  @return     the color difference mode for this object
             */
			ColorDifferenceMode getColorDifferenceMode(void);


        protected:
        
            void assignBackground_(RasterImage* img);
            
            
		private:
			
			/**	Stores the raster image used as background
			 */
			RasterImage			*background_;
			
			/**	Default color difference mode for this object
			 */
			ColorDifferenceMode mode_;
            
            /** Copy constructor disabled
             *  @param  obj     the object to copy
             */
            BackgroundRemover(const BackgroundRemover& obj);


            /** Copy operator disabled
             *  @param  obj     the object to copy
             */
            const BackgroundRemover& operator = (const BackgroundRemover& obj);


			
	};
}

#endif	//	BACKGROUND_REMOVER_H
