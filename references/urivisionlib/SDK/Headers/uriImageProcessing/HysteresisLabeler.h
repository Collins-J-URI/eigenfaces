/*  NAME:
        HysteresisLabeler.h
 
    DESCRIPTION:
        HysteresisLabeler public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_HYSTERESIS_LABELER_H
#define URIVL_HYSTERESIS_LABELER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageRect.h"
#include "LabelRaster.h"
#include "VectorField.h"
#include "RasterImage.h"
#include "VectorField_F.h"
#include "VectorField_I.h"


namespace uriVL
{
    /** A HysteresisLabeler is a component of the Canny edge detector.
     *
     *  @author Chris Allen, 
	 *							3D Group 
     *          Department of Computer Science and Statistics,
     *                  University of Rhode Island
     */
    class URIVL_EXPORT HysteresisLabeler
    {
        public:

            /**
             *  Destructor
             */          
            ~HysteresisLabeler(void);

            /** Applies this operator to a VectorField and returns the results 
			 *	as a LabelRaster.
             *
			 *	@param	high		high threshold (for norm of gradient)
			 *	@param	low			low threshold (for norm of gradient)
             *  @param  vecIn       the VectorField to which the operator is applied
             *  @return             the LabelRaster of edge pixels in the vector field
             */ 
             static LabelRaster *applyTo(float high, float low, VectorField *vecIn);

            /** Applies this operator to a VectorField and returns the result as
			 *	a LabelRaster. Additionally, this function writes output to the
			 *	vecOut VectorField given as a parameter.
			 *
			 *	@param	high		high threshold (for norm of gradient)
			 *	@param	low			low threshold (for norm of gradient)
             *  @param  vecIn       the VectorField to which the operator is applied
             *  @param  vectOut     the VectorField into which the output is written (sets to nul non edge vectors)
             *  @return             the LabelRaster of edge pixels in the vector field
             */ 
             static LabelRaster *applyTo(float high, float lo, VectorField *vecIn, VectorField *vecOut);


            /** Applies this operator to a VectorField and writes the results
			 *	into a LabelRaster received as a parameter.
			 *
			 *	@param	high		high threshold (for norm of gradient)
			 *	@param	low			low threshold (for norm of gradient)
             *  @param  vecIn       the VectorField to which the operator is applied
             *  @param  labelOut	the LabelRaster of edge pixels in the vector field
             */ 
             static void applyInto(float high, float low, VectorField *vecIn, LabelRaster *labelOut);

            /** Applies this operator to a Vector field and writes the results 
			 *	into a VectorField and a LabelRaster that are received as parameters.
             *
			 *	@param	high		high threshold (for norm of gradient)
			 *	@param	low			low threshold (for norm of gradient)
             *  @param  vecIn       the VectorField to which the operator is applied
             *  @param  vecOut		the VectorField into which the output is written (sets to nul non edge vectors)
             *  @param  labelOut	the LabelRaster of edge pixels in the vector field
             */ 
             static void applyInto(float high, float low, VectorField *vecIn, VectorField *vecOut, LabelRaster *labelOut);


        protected:
            /** 
             *  Default constructor.
             *
             *  This constructor simply sets all instance variables to 0, NULL, or the 
             *  appropriate default value.  
             *
			 *	c.allen(4.24.08): At the moment, this class is composed entirely
			 *		of static methods.  In the future this may change.
             */
            HysteresisLabeler(void);
  
    };
}

#endif  //  HYSTERESIS_LABELER_H
