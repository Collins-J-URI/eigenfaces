/*  NAME:
        ColorImageGradient.h
 
    DESCRIPTION:
        ColorImageGradient public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_COLOR_IMAGE_GRADIENT_H
#define URIVL_COLOR_IMAGE_GRADIENT_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "VectorOperator.h"
#include "RasterImage_RGBa_F.h"

namespace uriVL
{
    /** Diffferent ways of computing a color gradient
     */
    typedef enum ColorGradientMode {
                    GRAD_AVE_OF_COLOR_COMPONENTS = 0,   //  average red, green, blue grads
                    GRAD_LARGEST_COLOR_COMPONENT        //  r-g-b grad with largest norm
    } ColorGradientMode;
    
    /** Pure virtual parent class for an operator that computes the gradient of a color raster image
     *
     *  @author jean-yves herve', 3D Group, URI
     */
    class URIVL_EXPORT ColorImageGradient : public VectorOperator
    {
        public:

            /** Destructor. Does nothing
             */
            virtual ~ColorImageGradient(void);

            /**
             *  Returns the width of the filter.
             *
             *  @return     width of the filter
             */
            int getWidth(void) const;

            /**
             *  Returns the height of the filter.
             *
             *  @return     height of the filter
             */
            int getHeight(void) const;
                    
			/**	Returns the scale of the operator
			 *	@return		scale of the operator
			 */
			virtual float getScale(void) const = 0;
			 
			 
            /** Sets the Computation mode for the color gradient
             *  @param  colorGradientMode
             */
            void setColorGradientMode(ColorGradientMode colorGradientMode);
                    
            /** Returns the Computation mode for the color gradient
             *  @return  the Computation mode for the color gradient
             */
            ColorGradientMode getColorGradientMode(void) const;
                    
                    
        protected:
        
            /** Creates a ColorImageGradient operator with the specified width.
             *
             *  @param  theWidth   desired width of the Gaussian filter
			 *	@param	isIntOperator	true if this operator produces only integer output
			 *	@param	theMode			how the r g b components should be used to compute the gradient
             */
            ColorImageGradient(int theWidth, bool isIntOperator, 
								ColorGradientMode theMode = GRAD_AVE_OF_COLOR_COMPONENTS);
            
            /** Creates a ColorImageGradient operator with the specified width and 
			 *	a temporary raster at the specified dimensions.
             *
             *  This constructor is useful when processing a set of images that have
             *  the same dimensions.  It allows reusing the same temporary
             *  raster without losing time re-allocating and deleting each 
             *  at each call.
             *  @param  theWidth   desired width of the ImageGradient
             *  @param theRect  bounding rectangle for the temporary raster
			 *	@param	isIntOperator	true if this operator produces only integer output
			 *	@param	theMode			how the r g b components should be used to compute the gradient
             */
            ColorImageGradient(int theWidth, const ImageRect* theRect, bool isIntOperator, 
								ColorGradientMode theMode = GRAD_AVE_OF_COLOR_COMPONENTS);


            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ColorImageGradient(const ColorImageGradient& obj);


			/**	Allocates the temporary raster associated with this object.
			 *	If the raster was already allocated, the function only reallocates it if it 
			 *	was smaller than the new rect.
			 *	@param	rect	dimensions of the temp rect to allocate
			 *	@return true	if a new raster was allocated or the old one was resized
			 */
			bool allocateTempRasters_(const ImageRect* rect);

			/**	Frees the temporary raster associated with this object.
			 */
			void freeTempRasters_(void);
			
			/**	Gives writing access to the shifted 2D red temp raster
			 *  @param   rw     R_W_ACCESS is the only acceptable value
			 *	@return shifted 2D gray temp raster
			 */
			float* const* getShiftedTempRedF2D_(ReadWriteStatus rw);

			/**	Gives writing access to the shifted 2D green temp raster
			 *  @param   rw     R_W_ACCESS is the only acceptable value
			 *	@return shifted 2D gray temp raster
			 */
			float* const* getShiftedTempGreenF2D_(ReadWriteStatus rw);

			/**	Gives writing access to the shifted 2D blue temp raster
			 *  @param   rw     R_W_ACCESS is the only acceptable value
			 *	@return shifted 2D gray temp raster
			 */
			float* const* getShiftedTempBlueF2D_(ReadWriteStatus rw);

			/**	Gives writing access to the shifted 2D alternate red temp raster
			 *  @param   rw     R_W_ACCESS is the only acceptable value
			 *	@return shifted 2D gray temp raster
			 */
			float* const* getShiftedAltTempRedF2D_(ReadWriteStatus rw);

			/**	Gives writing access to the shifted 2D alternate green temp raster
			 *  @param   rw     R_W_ACCESS is the only acceptable value
			 *	@return shifted 2D gray temp raster
			 */
			float* const* getShiftedAltTempGreenF2D_(ReadWriteStatus rw);

			/**	Gives writing access to the shifted 2D alternate blue temp raster
			 *  @param   rw     R_W_ACCESS is the only acceptable value
			 *	@return shifted 2D gray temp raster
			 */
			float* const* getShiftedAltTempBlueF2D_(ReadWriteStatus rw);


        private:
        
            /** Computation mode for the color gradient.   default mode is
             *  GRAD_AVE_OF_COLOR_COMPONENTS
             */
            ColorGradientMode   colorGradientMode_;
                
            /** Bounding rectangle  for pre-allocated temporary raster.
             *
             *  This is useful when processing a set of images that have
             *  the same dimensions.  It allows reusing the same temporary
             *  raster without losing time re-allocating and deleting each 
             *  at each call.
             */
            ImageRect* boundRect_;
            
            /** Temporary raster.
             *
             *  This is useful when processing a set of images that have
             *  the same dimensions.  It allows reusing the same temporary
             *  raster without losing time re-allocating and deleting each 
             *  at each call.
             */
            RasterImage_RGBa_F* tempRaster_;
            
            /** Second temporary raster for computing the gradient.
             *
             *  This is useful when processing a set of images that have
             *  the same dimensions.  It allows reusing the same temporary
             *  raster without losing time re-allocating and deleting each 
             *  at each call.
             */
            RasterImage_RGBa_F*	altTempRaster_;
            
            
            /** the scale sigma of this filter
             */
            double      scale_;

             /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ColorImageGradient& operator =(const ColorImageGradient& obj);
    };
}

#endif  //  COLOR_IMAGE_GRADIENT_H
