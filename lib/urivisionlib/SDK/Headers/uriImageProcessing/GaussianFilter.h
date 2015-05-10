/*  NAME:
        GaussianFilter.h
 
    DESCRIPTION:
        GaussianFilter public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_GAUSSIAN_FILTER_H
#define URIVL_GAUSSIAN_FILTER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageOperator.h"
#include "RasterImage_gray_F.h"
#include "RasterImage_RGBa_F.h"

namespace uriVL
{

    /** Pure Virtual parent class for the Gaussian filter classes _F and _I.
	 *	Note that in the current implementation of this class the same scale is
	 *	applied along the x and y directions.  Implementing a more general form
	 *	is a low-priority item at this point.
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT GaussianFilter : public ImageOperator
    {
        public:

			/**	Returns the scale of the filter
			 *	@return		scale of the filter
			 */
			float getScale(void) const;
			 
            /** Destructor
             */
            ~GaussianFilter(void);


        protected:

            /** Creates a Gaussian filter with the specified scale
             *
             *  The scale of the filter is the square root of the variance of the
             *  associated distribution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).
             *  @param  scale   scale sigma of the Gaussian filter
			 *	@param	isIntOperator	true if this operator produces only integer output
             */
            GaussianFilter(double theScale, bool isIntOperator);

            /** Creates a Gaussian filter with the specified width
             *
             *  The scale of the filter is the square root of the variance of the
             *  associated distribution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).<p>
             *  The actual width of the filter = 2*radius + 1<p>
             *  it is set to the smallest odd number greater or equal to the parameter
             *  received.
             *  @param  theWidth   desired width of the Gaussian filter
			 *	@param	isIntOperator	true if this operator produces only integer output
             */
            GaussianFilter(int theWidth, bool isIntOperator);
            
            /** Creates a Gaussian filter with the specified scale
             *
             *  The scale of the filter is the square root of the variance of the
             *  associated distribution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).
             *
             *  This constructor is useful when processing a set of images that have
             *  the same dimensions.  It allows reusing the same temporary
             *  raster without losing time re-allocating and deleting each 
             *  at each call.
             *  @param  scale   scale sigma of the Gaussian filter
             *  @param theRect  bounding rectangle for the temporary raster
			 *	@param	isIntOperator	true if this operator produces only integer output
             */
            GaussianFilter(double theScale, const ImageRect* theRect, bool isIntOperator);

            /** Creates a Gaussian filter with the specified width
             *
             *  The scale of the filter is the square root of the variance of the
             *  associated distribution.  It it also related to the width of the 
             *  filter by the relation
             *      width = ceil(5*scale).<p>
             *  The actual width of the filter = 2*radius + 1<p>
             *  it is set to the smallest odd number greater or equal to the parameter
             *  received.
             *
             *  This constructor is useful when processing a set of images that have
             *  the same dimensions.  It allows reusing the same temporary
             *  raster without losing time re-allocating and deleting each 
             *  at each call.
             *  @param  theWidth   desired width of the Gaussian filter
             *  @param theRect  bounding rectangle for the temporary raster
			 *	@param	isIntOperator	true if this operator produces only integer output
             */
            GaussianFilter(int theWidth, const ImageRect* theRect, bool isIntOperator);

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            GaussianFilter(const GaussianFilter& obj);


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
            const GaussianFilter& operator =(const GaussianFilter& obj);
                
    };
}

#endif  //  GAUSSIAN_FILTER_H
