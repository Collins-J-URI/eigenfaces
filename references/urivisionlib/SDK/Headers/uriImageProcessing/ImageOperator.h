/*  NAME:
        ImageOperator.h
 
    DESCRIPTION:
        ImageOperator public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_OPERATOR_H
#define URIVL_IMAGE_OPERATOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageRect.h"
#include "RasterImage_HSV.h"

namespace uriVL
{
    /** Virtual parent class for all image operators (operators that are applied to a
     *  raster image and produce a raster image as their output).
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ImageOperator
    {
        public:

            /**
             *  Destructor
             */          
            virtual ~ImageOperator(void);

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
            
            /** Applies this operator to an image, returns the result as a RasterImage.
             *
             *  This function will be overloaded by subclasses of ImageOperator.  These 
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
             *  @return             the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            virtual RasterImage* applyTo(const RasterImage* imgIn) = 0;

            /** Applies this operator to an image, returns the result as a RasterImage.
             *
             *  This function will be overloaded by subclasses of ImageOperator.  These 
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
             *  @return             the RasterImage obtained by applying the operator to
             *                      imgIn
             */ 
            virtual RasterImage* applyTo(const RasterImage* imgIn, const ImageRect* theRect) = 0;

            /** Applies this operator to a RasterImage and writes the resukt into a second
             *  RasterImage received as parameter.
             *
             *  This function will be overloaded by subclasses of ImageOperator.  These 
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
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            virtual void applyInto(const RasterImage* imgIn, RasterImage* imgOut) = 0;
            
            /** Applies this operator to a RasterImage and writes the resukt into a second
             *  RasterImage received as parameter.
             *
             *  This function will be overloaded by subclasses of ImageOperator.  These 
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
             *  @param  imgOut      the RasterImage into which the output is written
             */ 
            virtual void applyInto(const RasterImage* imgIn, const ImageRect* theRect, 
                                    RasterImage* imgOut) = 0;
            
            /** Sets the default conversion mode for this ImageOperator.
             *  @param  theMode     conversion mode for this image operator
             *
             *  @see    getFloatConversionMode
             *  @see    floatConversionMode_
             */
            void setFloatConversionMode(ConversionFromFloatMode theMode);

            /** Returns the image operator's conversion mode.
             *  @return     conversion mode for this image operator
             *
             *  @see    setFloatConversionMode
             *  @see    floatConversionMode_
             */
            ConversionFromFloatMode getFloatConversionMode(void) const;
			
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
             *  @param  mode    indicates the expected range of values for the output of this
             *                  operator.  This will be the mode assigned by default to all
             *                  "float" raster images produced by the operator.
             */
            ImageOperator(bool isIntOperator = false, ConversionFromFloatMode mode = kSignedFloat);
  

            /** Sets the image rectangle this operator will work on
             *  @param  theRect image rectangle this operator will work on
			 *	@param	isIntOperator	true if this operator produces only integer output
             *  @param  mode    indicates the expected range of values for the output of this
             *                  operator.  This will be the mode assigned by default to all
             *                  "float" raster images produced by the operator.
             */
            ImageOperator(const ImageRect* theRect, bool isIntOperator = false,
						  ConversionFromFloatMode mode = kSignedFloat);


            /** Sets the image rectangle this operator will work on
             *  @param  theLeft     x coordinate of left column pixels
             *  @param  theTop      y coordinate of top row pixels
             *  @param  theWidth    number of pixels along a row (pixel-as-point)
             *  @param  theHeight   number of pixels along a column (pixel-as-point)
			 *	@param	isIntOperator	true if this operator produces only integer output
             *  @param  mode    indicates the expected range of values for the output of this
             *                  operator.  This will be the mode assigned by default to all
             *                  "float" raster images produced by the operator.
             */
            ImageOperator(int theLeft, int theTop, int theWidth, int theHeight, 
						  bool isIntOperator = false, ConversionFromFloatMode mode = kSignedFloat);


            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ImageOperator(const ImageOperator& obj);


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
             *  Image rectangle (with its origina at the image pixel) over which the 
             *  operator is calculated.
             */
            ImageRect*	rect_;

            /**
             *  true if the operator is an "integer" operator
             */
            bool    isIntegerOperator_;
                                        
            /** Indicates how the values calculated are to be converted into 
             *  <code>unsigned char</code> values.  This variable is set by the constructor.
             *  Default value is <code>kSignedFloat</code>
             *  @see    getFloatConversionMode
             *  @see    setFloatConversionMode
             */
            ConversionFromFloatMode floatConversionMode_;
                                        

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ImageOperator& operator =(const ImageOperator& obj);

    };
}

#endif  //  IMAGE_OPERATOR_H
