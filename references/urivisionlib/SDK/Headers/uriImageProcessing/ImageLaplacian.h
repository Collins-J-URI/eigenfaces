/*  NAME:
        ImageLaplacian.h
 
    DESCRIPTION:
        ImageLaplacian public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_LAPLACIAN_H
#define URIVL_IMAGE_LAPLACIAN_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageOperator.h"

namespace uriVL
{
    /** Operator that computes the Laplacian of a raster image
     *
     *  @author jean-yves herve', 3D Group, URI
     */
    class URIVL_EXPORT ImageLaplacian : public ImageOperator
    {
        public:
            
			/**	Destructor
			 */
            ~ImageLaplacian(void);

			/**	Returns the scale of the operator
			 *	@return		scale of the operator
			 */
			virtual float getScale(void) const = 0;
			 
            
		protected:
		
			/**	Constructor.
			 *	@param	theWidth	width of the Laplacian filter
			 *	@param	theHeight	height of the Laplacian filter
			 *	@param	isIntOperator	true if this operator produces only integer output
			 */
            ImageLaplacian(int theWidth, int theHeight, bool isIntOperator);

		
        private:
                
        
    };
}

#endif  //  IMAGE_LAPLACIAN_H
