/*  NAME:
        Labeler.h
 
    DESCRIPTION:
        Labeler public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_LABELER_H
#define URIVL_LABELER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ColorRegions.h"
#include "LabelRaster.h"
#include "RasterImage.h"
#include "RasterImage_HSV.h"

namespace uriVL
{
    /** A Labeler object assigns a label to each pixel of its input image
     *
     *  The main difference between a Labeler and an ImageOperator (and the
     *  reason why the former is not a subclass of the latter) is that a
     *  Labeler's standard output is a LabelRaster object rather than a RasterImage.
     *  Labeler classes provide support for RasterImage objects as output, but
     *  really only as a help for development/debugging.
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT Labeler
    {
        public:
            /**
             *  Destructor
             */          
            virtual ~Labeler(void);

            /** Applies this operator to an image, returns the result as a LabelRaster.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to void)
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             *  @param  connectNeighbor boolean variable to decide whether
             *                          do the connect-neighbor operation
             */ 
            virtual LabelRaster *applyTo(const RasterImage* imgIn, const char* params,
                                            bool connectNeighbor) = 0;

            /** Applies this operator to an image, returns the result as a LabelRaster.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to void)
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @return             the LabelRaster obtained by applying the operator to
             *                      imgIn
             *  @param  connectNeighbor boolean variable to decide whether
             *                          do the connect-neighbor operation
             */ 
            virtual LabelRaster *applyTo(const RasterImage* imgIn, const char* params,
                                         const ImageRect* theRect, bool connectNeighbor) = 0;

            /** Applies this operator to an image, writes the result into a LabelRaster
             *  provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to void)
             *  @param  labelOut    the LabelRaster obtained by applying the operator to
             *                      imgIn
             *  @param  connectNeighbor boolean variable to decide whether
             *                          do the connect-neighbor operation
             */ 
            virtual void applyInto(const RasterImage* imgIn, const char* params, LabelRaster *labelOut,
                                    bool connectNeighbor)=0;

            /** Applies this operator to an image, writes the result into a LabelRaster
             *  provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn           the RasterImage to which the operator is applied
             *  @param  params          parameters for the labeling operation (cast as a 
             *                          pointer to void)
             *  @param  theRect         the rectangle within which to apply the labeler
             *  @param  labelOut        the LabelRaster obtained by applying the operator to
             *                          imgIn
             *  @param  connectNeighbor boolean variable to decide whether
             *                          do the connect-neighbor operation
             */ 
            virtual void applyInto(const RasterImage* imgIn, const char* params, const ImageRect* theRect, 
                                    LabelRaster *labelOut, bool connectNeighbor)=0;

            /** Applies this operator to an image, writes the result into a RasterImage
             *  provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to void)
             *  @param  imgOut      a RasterImage version of the label raster obtained by 
             *                      applying the operator to imgIn
             */ 
            virtual void applyInto(const RasterImage* imgIn, const char* params, RasterImage* imgOut) = 0;

            /** Applies this operator to an image, writes the result into a RasterImage
             *  provided as parameter.
             *
             *  This function will be overloaded by subclasses of Labeler
             *
             *  @param  imgIn       the RasterImage to which the operator is applied
             *  @param  params      parameters for the labeling operation (cast as a 
             *                      pointer to void)
             *  @param  theRect     the rectangle within which to apply the labeler
             *  @param  imgOut      a RasterImage version of the label raster obtained by 
             *                      applying the operator to imgIn
             */ 
            virtual void applyInto(const RasterImage* imgIn, const char* params, const ImageRect* theRect, 
                                    RasterImage* imgOut) = 0;


        protected:

            LabelRaster *tempLabel_;

            /**  default constructor
             */
            Labeler(void);
            
            /**
             *  constructor
             *
             *  @param  theWidth   number of columns
             *  @param  theHeight  number of rows
             */
            Labeler(int theWidth, int theHeight);
			
		private:
		
			/**	Copy constructor.  Disabled.
			 *	@param	obj		object to copy
			 */
			Labeler(const Labeler& obj);
			
			/**	Copy operator.  Disabled.
			 *	@param	obj		object to copy
			 */
			const Labeler& operator = (const Labeler& obj);
			
			

    };
}

#endif  //  LABELER_H
