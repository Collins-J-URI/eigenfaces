/*  NAME:
        ImageGradient.h
 
    DESCRIPTION:
        ImageGradient public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_GRADIENT_H
#define URIVL_IMAGE_GRADIENT_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "VectorOperator.h"

namespace uriVL
{
    /** Pure virtual parent class for an operator that computes the gradient of a raster image.
     *
     *  @author jean-yves herve', 3D Group, URI
     */
    class URIVL_EXPORT ImageGradient : public VectorOperator
    {
        public:

            /** Destructor. Does nothing
             */
            virtual ~ImageGradient(void);

			/**	Returns the scale of the operator
			 *	@return		scale of the operator
			 */
			virtual float getScale(void) const = 0;
			 
                    
        protected:
        
            /** Creates an ImageGradient operator with the specified width.
             *
             *  @param  theWidth   desired width of the Gaussian filter
			 *	@param	isIntOperator	true if this operator produces only integer output
             */
            ImageGradient(int theWidth, bool isIntOperator);
            
            /** Creates an ImageGradient operator with the specified width and 
			 *	a temporary raster at the specified dimensions.
             *
             *  This constructor is useful when processing a set of images that have
             *  the same dimensions.  It allows reusing the same temporary
             *  raster without losing time re-allocating and deleting each 
             *  at each call.
             *  @param  theWidth   desired width of the ImageGradient
             *  @param theRect  bounding rectangle for the temporary raster
			 *	@param	isIntOperator	true if this operator produces only integer output
             */
            ImageGradient(int theWidth, const ImageRect* theRect, bool isIntOperator);


            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ImageGradient(const ImageGradient& obj);


			/**	Allocates the temporary raster associated with this object.
			 *	If the raster was already allocated, the function only reallocates it if it 
			 *	was smaller than the new rect.
			 *	@param	rect	dimensions of the temp rect to allocate
			 *	@return true	if a new raster was allocated or the old one was resized
			 */
			bool allocateTempRaster_(const ImageRect* rect);

			/**	Frees the temporary raster associated with this object.
			 */
			void freeTempRaster_(void);
			
			/**	Gives writing access to the shifted 2D gray temp raster
			 *  @param   rw     R_W_ACCESS is the only acceptable value
			 *	@return shifted 2D gray temp raster
			 */
			float* const* getShiftedTempGrayF2D_(ReadWriteStatus rw);


		private:

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
            RasterImage_gray_F* tempRaster_;
            
            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ImageGradient& operator =(const ImageGradient& obj);

    };
}

#endif  //  IMAGE_GRADIENT_H
