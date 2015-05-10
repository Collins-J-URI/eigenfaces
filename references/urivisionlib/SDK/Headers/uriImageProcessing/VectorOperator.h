/*  NAME:
        VectorOperator.h
 
    DESCRIPTION:
        VectorOperator public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VECTOR_OPERATOR_H
#define URIVL_VECTOR_OPERATOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageRect.h"
#include "VectorField.h"
#include "RasterImage.h"
#include "RasterImage_RGBa.h"


namespace uriVL
{
    /** Virtual parent class for all vector operators (operators that are applied to a
     *  raster image and produce a vector field as their output).
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT VectorOperator
    {
        public:

            /**
             *  Destructor
             */          
            virtual ~VectorOperator(void);

            /**
             *  returns true if the filter is an "integer" operator
             *  @return     true if the filter is an "integer" operator
             */
            bool isIntegerOperator(void) const;
            
            /**
             *  returns true if the filter is a "float" operator
             *  @return     true if the filter is a "float" operator
             */
            bool isFloatOperator(void) const;
            
            /** Applies this operator to an image, returns the result as a VectorField.
             *
             *  This function will be overloaded by subclasses of VectorOperator.  These 
             *  subclasses should follow the following rules:
             *  <ul>
             *      <li> if the operator is an "integer" operator, then the RasterImage
             *              returned should have the same type as the input image
             *      <li> if the operator is a "float" or "double" operator, then the 
             *              RasterImage returned should be of the "float" form of the input image.
             *              For example, if imgIn was RasterImage_Gray, then the image
             *              returned should be RasterImage_Gray_F.
             *  </ul>
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @return             the VectorField obtained by applying the operator to
             *                      imgIn
             */ 
             virtual VectorField* applyTo(const RasterImage* imgIn) = 0;

            /** Applies this operator to an image  within the rectangle specified and
             *  returns the result as a VectorField.
             *
             *  This function will be overloaded by subclasses of VectorOperator.  These 
             *  subclasses should follow the following rules:
             *  <ul>
             *      <li> if the operator is an "integer" operator, then the RasterImage
             *              returned should have the same type as the input image
             *      <li> if the operator is a "float" or "double" operator, then the 
             *              RasterImage returned should be of the "float" form of the input image.
             *              For example, if imgIn was RasterImage_Gray, then the image
             *              returned should be RasterImage_Gray_F.
             *  </ul>
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @return             the VectorField obtained by applying the operator to
             *                      imgIn
             */ 
             virtual VectorField* applyTo(const RasterImage* imgIn, const ImageRect* theRect) = 0;

            /** Applies this operator to a RasterImage and writes the result into a 
             *  VectorField received as parameter.
             *
             *  This function will be overloaded by subclasses of VectorOperator.  These 
             *  subclasses should follow the following rules:
             *  <ul>
             *      <li> if the operator is an "integer" operator, then the RasterImage
             *              returned should have the same type as the input image
             *      <li> if the operator is a "float" or "double" operator, then the 
             *              RasterImage returned should be of the "float" form of the input image.
             *              For example, if imgIn was RasterImage_Gray, then the image
             *              returned should be RasterImage_Gray_F.
             *  </ul>
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            virtual void applyInto(const RasterImage* imgIn, VectorField *vectOut) = 0;
            
            /** Applies this operator to a RasterImage within the rectangle specified
             *  and writes the result into the VectorField received as parameter.
             *
             *  This function will be overloaded by subclasses of VectorOperator.  These 
             *  subclasses should follow the following rules:
             *  <ul>
             *      <li> if the operator is an "integer" operator, then the RasterImage
             *              returned should have the same type as the input image
             *      <li> if the operator is a "float" or "double" operator, then the 
             *              RasterImage returned should be of the "float" form of the input image.
             *              For example, if imgIn was RasterImage_Gray, then the image
             *              returned should be RasterImage_Gray_F.
             *  </ul>
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  theRect     the rectangle within which to apply the filter
             *  @param  vectOut     the VectorField into which the output is written
             */ 
            virtual void applyInto(const RasterImage* imgIn, const ImageRect* theRect, 
                                    VectorField *vectOut) = 0;


			/**	Returns a pointer to this operator's operation rectangle
			 *	@return		a pointer to this operator's operation rectangle
			 */
			const ImageRect* getRect(void) const;

            /**
             *  Returns the width of this operator's operation rectangle.
             *  @return     width of this operator's operation rectangle
             */
            int getWidth(void) const;

            /**
             *  Returns the height of this operator's operation rectangle.
             *  @return     height of this operator's operation rectangle
             */
            int getHeight(void) const;
                    
			

        protected:

            /** 
             *  Constructor.
             *
             *  Except for the conversion mode, this constructor simply sets all instance 
             *  variables to 0, NULL, or the appropriate default value.  
             *
      		 *	@param	isIntOperator	true if this operator produces only integer output
             */
            VectorOperator(bool isIntOperator = false);
  

            /** Sets the image rectangle this operator will work on
             *  @param  theRect image rectangle this operator will work on
			 *	@param	isIntOperator	true if this operator produces only integer output
			 */
            VectorOperator(const ImageRect* theRect, bool isIntOperator = false);


            /** Sets the image rectangle this operator will work on
             *  @param  theLeft     x coordinate of left column pixels
             *  @param  theTop      y coordinate of top row pixels
             *  @param  theWidth    number of pixels along a row (pixel-as-point)
             *  @param  theHeight   number of pixels along a column (pixel-as-point)
			 *	@param	isIntOperator	true if this operator produces only integer output
              */
            VectorOperator(int theLeft, int theTop, int theWidth, int theHeight, 
						   bool isIntOperator = false);


            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            VectorOperator(const VectorOperator& obj);


			/**	Sets the rectangle on which this operator uh... operates.
             *  @param  theRect image rectangle this operator will work on
			 */
			void setRect(const ImageRect* theRect);


			/**	Sets the rectangle on which this operator uh... operates.
             *  @param  theLeft     x coordinate of left column pixels
             *  @param  theTop      y coordinate of top row pixels
             *  @param  theWidth    number of pixels along a row (pixel-as-point)
             *  @param  theHeight   number of pixels along a column (pixel-as-point)
			 */
			void setRect(int theLeft, int theTop, int theWidth, int theHeight);


        private:

            /**
             *  Neighborhood processing rectangle centered at the pixel to evaluate
			 *	the operator at.
             */
            ImageRect*	rect_;

            /**
             *  true if the filter is an "integer" operator
             */
            bool    isIntegerOperator_;
                                
			/** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const VectorOperator& operator =(const VectorOperator& obj);
    };
}

#endif  //  VECTOR_OPERATOR_H
