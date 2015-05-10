/*  NAME:
        ColorMedianFilter.h
 
    DESCRIPTION:
        ColorMedianFilter public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_COLOR_MEDIAN_FILTER_H
#define URIVL_COLOR_MEDIAN_FILTER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageOperator.h"

namespace uriVL
{
    /** ColorMedianFilter class
     *
     *  A median filter is an image operator that replaces the value at each
     *  pixel by the median value of a rectangular window centered at that pixel
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ColorMedianFilter : public ImageOperator
    {
        public:


            /** Destructor
             */
            ~ColorMedianFilter(void);

            
            
        protected:

//            int   nbData_;
            
            /** Defines a median filter without a specified width and height.
             *  @param  mode    indicates the expected range of values for the output of this
             *                  operator.  This will be the mode assigned by default to all
             *                  "float" raster images produced by the operator.
			 *	@param	isIntOperator	true if this operator produces only integer output
             */
            ColorMedianFilter(void);

            /** Defines a median filter with a specified width and height.
             *  The filter will be centered at the pixel where it is applied.
             *  @param  theWidth   desired width of the median filter
             *  @param  theHeight   desired width of the median filter
             *  @param  mode    indicates the expected range of values for the output of this
             *                  operator.  This will be the mode assigned by default to all
             *                  "float" raster images produced by the operator.
			 *	@param	isIntOperator	true if this operator produces only integer output
             */
            ColorMedianFilter(int theWidth, int theHeight, bool isIntOperator = false, 
						 ConversionFromFloatMode mode = kSignedFloat);


            /** Defines a median filter with a dimensions rectangle.  It is up
             *  to the user to make sure that the dimensions of the filter make sense
             *  (the "left" and "top" values should be negative, opposite of "right"
             *  and bottom")
             *  @param  theRect desired dimensions of the median filter
             *  @param  mode    indicates the expected range of values for the output of this
             *                  operator.  This will be the mode assigned by default to all
             *                  "float" raster images produced by the operator.
			 *	@param	isIntOperator	true if this operator produces only integer output
             */
            ColorMedianFilter(const ImageRect* theRect, bool isIntOperator = false, 
						 ConversionFromFloatMode mode = kSignedFloat);
            
    };
}

#endif  //  COLOR_MEDIAN_FILTER_H
