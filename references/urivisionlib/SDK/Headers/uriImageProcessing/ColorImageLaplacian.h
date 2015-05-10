/*  NAME:
        ColorImageLaplacian.h
 
    DESCRIPTION:
        ColorImageLaplacian public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_COLOR_IMAGE_LAPLACIAN_H
#define URIVL_COLOR_IMAGE_LAPLACIAN_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageOperator.h"

namespace uriVL
{
    /** Operator that computes the Laplacian of an RGBa color image
     *
     *  <b>This class has not been implemented yet</b>
     *
     *  @author jean-yves herve', 3D Group, URI
     */
    class URIVL_EXPORT ColorImageLaplacian : public ImageOperator
    {
        public:
            /**	Default constructor
			 */
            ColorImageLaplacian(void);

			/**	Destructor
			 */
            ~ColorImageLaplacian(void);

            /**
             *  Returns the width of the operator.
             *
             *  @return     width of the operator
             */
            int getWidth(void);

            /**
             *  Returns the height of the operator.
             *
             *  @return     height of the operator
             */
            int getHeight(void);
                    
        private:
                
            /** the width of the filter
             *
             *  The width is the smallest integer larger or equal to 5*scale
             */
            int       width_;
        
    };
}

#endif  //  COLOR_IMAGE_LAPLACIAN_H
