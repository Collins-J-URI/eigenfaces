/*  NAME:
        NormalFlowCalculator.h
 
    DESCRIPTION:
        NormalFlowCalculator public header.
 
    COPYRIGHT:
        uriVisionLib Copyright  2004, uriVisionLib Developers.
 
 
 
        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2 of the License, or (at your option) any later version.
 
        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Lesser General Public License for more details.
 
        You should have received a copy of the GNU Lesser General Public
        License along with this library; if not, write to the Free Software
        Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
#ifndef URIVL_NORMAL_FLOW_CALCULATOR_H
#define URIVL_NORMAL_FLOW_CALCULATOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"
#include "RasterImage_gray.h"
#include "RasterImage_RGB.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray_F.h"
#include "VectorField.h"
#include "VectorField_F.h"
#include "ImageGradient.h"
#include "ImageGradient_Gaussian_F.h"
//#include "ImageGradient_Sobel.h"

namespace uriVL
{
	class URIVL_EXPORT NormalFlowCalculator
	{
		public:

			/** Creates a NormalFlowCalculator
			 *	@param	rect			image's bound rect
			 *	@param	gradScale		gradient scale
			 *	@param	gradThreshold	gradient threshold
			 */
			NormalFlowCalculator(const ImageRect* rect,
								 const float gradScale,
								 const float gradThreshold);
								  
			/** Creates a NormalFlowCalculator
			 *	@param	gradScale		gradient scale
			 *	@param	gradThreshold	gradient threshold
			 */
			NormalFlowCalculator(const float gradScale,
								 const float gradThreshold);
								  
			/**
			 *	Destructor
			 */								  
			virtual ~NormalFlowCalculator();
			
			
			
			/**
			 *	Call applyInto()
			 *	Compute the normal flow
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@return					computed normal flow as a RasterImage_gray_F
			 */			
			RasterImage_gray_F* applyTo(const RasterImage* currImg,
									    const RasterImage* prevImg);


			/**
			 *	Call applyInto()
			 *	Compute the normal flow within the rectangle specified
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *  @param  theRect         rectangle within which to calculate the normal flow
			 *	@return					computed normal flow as a RasterImage_gray_F
			 */			
			RasterImage_gray_F* applyTo(const RasterImage* currImg,
									    const RasterImage* prevImg,
        							    const ImageRect* theRect);


			/**
			 *	Call applyInto()
			 *	Compute the normal flow
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@param	grad			gradient as a VectorField_F
			 *	@return					computed normal flow as a RasterImage_gray_F
			 */			
			RasterImage_gray_F* applyTo(const RasterImage* currImg,
									    const RasterImage* prevImg,
										const VectorField_F* grad);


			/**
			 *	Call applyInto()
			 *	Compute the normal flow within the rectangle specified
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@param	grad			gradient as a VectorField_F
			 *  @param  theRect         rectangle within which to calculate the normal flow
			 *	@return					computed normal flow as a RasterImage_gray_F
			 */			
			RasterImage_gray_F* applyTo(const RasterImage* currImg,
									    const RasterImage* prevImg,
										const VectorField_F* grad,
        							    const ImageRect* theRect);


			/**
			 *	Call applyInto()
			 *	Compute the normal flow
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@param	normalFlow		normal flow as a RasterImage_gray_F
			 */			
			void applyInto(const RasterImage* currImg,
						   const RasterImage* prevImg,
						   RasterImage_gray_F* normalFlow);


			/**
			 *	Call applyInto()
			 *	Compute the normal flow within the rectangle specified
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *  @param  theRect         rectangle within which to calculate the normal flow
			 *	@param	normalFlow		normal flow as a RasterImage_gray_F
			 */			
			void applyInto(const RasterImage* currImg,
						   const RasterImage* prevImg,
        				   const ImageRect* theRect,
						   RasterImage_gray_F* normalFlow);


			/**
			 *	Call applyInto()
			 *	Compute the normal flow
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@param	grad			gradient as a VectorField_F
			 *	@param	normalFlow		normal flow as a RasterImage_gray_F
			 */			
			void applyInto(const RasterImage* currImg,
						   const RasterImage* prevImg,
						   const VectorField_F* grad,
						   RasterImage_gray_F* normalFlow);


			/**
			 *	Call applyInto_()
			 *	Compute the normal flow within the rectangle specified
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@param	grad			gradient as a VectorField_F
			 *  @param  theRect         rectangle within which to calculate the normal flow
			 *	@param	normalFlow		normal flow as a RasterImage_gray_F
			 */			
			void applyInto(const RasterImage* currImg,
						   const RasterImage* prevImg,
						   const VectorField_F* grad,
        				   const ImageRect* theRect,
						   RasterImage_gray_F* normalFlow);


		protected:
		
			/**
			 *	Threshold used to compute normal flow
			 */								  
			float threshold_;
			
			/**
			 *	Gradient calculator
			 */								  
			//ImageGradient_Sobel *gaussGrad_;
			ImageGradient_Gaussian_F *gaussGrad_;


           /**
             *  Copy Constructor. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            NormalFlowCalculator(const NormalFlowCalculator& theObj);


            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const NormalFlowCalculator& operator = (const NormalFlowCalculator& theObj);


			/**
			 *	Called by all public functions
			 *	Compute the normal flow
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@param	grad			gradient as a VectorField_F
			 *  @param  theRect         rectangle within which to calculate the normal flow
			 *	@param	normalFlow		normal flow as a RasterImage_gray_F
			 */			
			void applyInto_(const RasterImage_gray* currImg,
							const RasterImage_gray* prevImg,
							const VectorField_F* grad,
							const ImageRect* theRect,
							RasterImage_gray_F* normalFlow);


			/**
			 *	Called by all public functions
			 *	Compute the normal flow
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@param	grad			gradient as a VectorField_F
			 *  @param  theRect         rectangle within which to calculate the normal flow
			 *	@param	normalFlow		normal flow as a RasterImage_gray_F
			 */			
			void applyInto_(const RasterImage_gray_F* currImg,
							const RasterImage_gray_F* prevImg,
							const VectorField_F* grad,
							const ImageRect* theRect,
							RasterImage_gray_F* normalFlow);


//			/**
//			 *	Called by all public functions
//			 *	Compute the normal flow
//			 *	@param	currImg			current image
//			 *	@param	prevImg			previous image
//			 *	@param	grad			gradient as a VectorField_F
//			 *  @param  theRect         rectangle within which to calculate the normal flow
//			 *	@param	normalFlow		normal flow as a RasterImage_gray_F
//			 */			
//			void applyInto_(const RasterImage_gray* currImg,
//							const RasterImage_gray_F* prevImg,
//							const VectorField_F* grad,
//							const ImageRect* theRect,
//							RasterImage_gray_F* normalFlow);
	

			/**
			 *	Called by all public functions
			 *	Compute the normal flow
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@param	grad			gradient as a VectorField_F
			 *  @param  theRect         rectangle within which to calculate the normal flow
			 *	@param	normalFlow		normal flow as a RasterImage_gray_F
			 */			
			void applyInto_(const RasterImage_RGBa* currImg,
							const RasterImage_RGBa* prevImg,
							const VectorField_F* grad,
							const ImageRect* theRect,
							RasterImage_gray_F* normalFlow);


			/**
			 *	Called by all public functions
			 *	Compute the normal flow
			 *	@param	currImg			current image
			 *	@param	prevImg			previous image
			 *	@param	grad			gradient as a VectorField_F
			 *  @param  theRect         rectangle within which to calculate the normal flow
			 *	@param	normalFlow		normal flow as a RasterImage_gray_F
			 */			
			void applyInto_(const RasterImage_RGB* currImg,
							const RasterImage_RGB* prevImg,
							const VectorField_F* grad,
							const ImageRect* theRect,
							RasterImage_gray_F* normalFlow);
	};
}

#endif		//  NORMAL_FLOW_CALCULATOR_H
