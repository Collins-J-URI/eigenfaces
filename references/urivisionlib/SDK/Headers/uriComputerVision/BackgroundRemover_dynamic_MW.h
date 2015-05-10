/*  NAME:
        BackgroundRemover_dynamic_MW.h
 
    DESCRIPTION:
        BackgroundRemover_dynamic_MW public header.
        Inherited from: BackgroundRemover_dynamic
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_BACKGROUND_REMOVER_DYNAMIC_MW_H
#define URIVL_BACKGROUND_REMOVER_DYNAMIC_MW_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "BackgroundRemover_dynamic.h"
#include "Blob.h"
#include "LinearSolver_QR.h"
#include "ColumnVector.h"


namespace uriVL
{
    /**	
     *	Inherited from: BackgroundRemover_dynamic
     *	Deal with changing background using a certain linear model
     */
	class URIVL_EXPORT BackgroundRemover_dynamic_MW : public BackgroundRemover_dynamic
	{
		friend class BackgroundRemoverTester;
		
		public:

			/**	Default constructor
			 */
			BackgroundRemover_dynamic_MW(void);

			/**	Destructor
			 */
			virtual ~BackgroundRemover_dynamic_MW(void);

			/**	Overidden virtual func
			 *
			 *	Stores a copy of img to use as firstBackground_(NOT background_) for future
			 *	image difference computations
			 *
			 *	@param	bkgd		raster image to store as firstBackground_
			 */			
			void setBackground(RasterImage* bkgd);
				
			/**	Overloaded func
			 *	Update background_
			 *
			 *	@param	img			raster image to be processed to produce new background
			 *	@param	RectList	a list of rectangles containing moving objects
			 */
			void updateBackground(RasterImage* img, ConstRectList rectList);
			
			/**	Returns the fitting coeffients as a 2D array
			 *	@return fitting coeffients as a 2D array
			 */
			double** getCoefficients(void);
			
		private:
			
			/**	square size of the area considered to be part of background
			 */
			short size_;
			 
			/** unit of horizontal squares
			 */
			int xUnit_;

			/** unit of vertical squares
			 */
			int yUnit_;
			
			/**	this variable is to control the initialization which can be either done
			 *	or this program or the BackgroundRemoverTester
			 */	
			bool initializedByTester_;
			
			/**	whether we do smoothing or not
			 *	BackgroundRemoverTester.testcase21 requires no smoothing
			 */
			bool noSmoothing_;

			/**	2D array to store a0_ ~ a9_ (share the same allocation)
			 *	which makes retrieval easier
			 */
			double** a_;
			
			/**	arrays (for R,G,B separately) estimated(using best fitting)
			 *	value of coefficients in equation:
			 *		Bt(i, j)	=	(a0_*i*i*i + a1_*j*j*j + a2_*i*i*j + a3_*i*j*j
			 *						+ a4_*i*i + a5_*j*j + a6_*i*j
			 *						+ a7_*i + a8_*j + a9_) * B0(i, j)
			 *		a_(t)		=	w*a(t) + (1-w)*a_(t-1)
			 *	these values are computed each time after background smoothing
			 */
			double* a0_, *a1_, *a2_, *a3_, *a4_, *a5_, *a6_, *a7_, *a8_, *a9_;
			
			/**	array (for R,G,B separately) parameter in equation:
			 *	a_(t) = w*a(t) + (1-w)*a_(t-1)
			 */
			double w_[3];
			
			/** 2D boolean array to recognize good squares (not containing foreground)
			 */
			bool **goodSquare_;
			
			/**	multi-dim arrays (for R,G,B separately) to store overall intensities of
			 *	a square (center at (x,y)) of pixels
			 */
			double*** intensityBack_;
			

			/**	arrays of X (same for R,G,B) values and B (for R,G,B separately) values
			 *	in SLE AX=B
			 */
			double	*list0_, *list1_, *list2_, *list3_, *list4_, *list5_,
					*list6_, *list7_, *list8_, **list9_;
			
			/**	SLE solver for solving linear matrix equation
			 */
			LinearSolver_QR *lls_;
			
			/**	Overloaded func
			 *	Update background_
			 *
			 *	@param	img			raster image to be processed to produce new background
			 *	@param	rectList	a list of rectangles containing moving objects
			 */
			void updateBackground_(RasterImage_gray_F *img, ConstRectList rectList);

			/**	Overloaded func
			 *	Update background_
			 *
			 *	@param	img			raster image to be processed to produce new background
			 *	@param	rectList	a list of rectangles containing moving objects
			 */
			void updateBackground_(RasterImage_gray* img, ConstRectList rectList);

			/**	Overloaded func
			 *	Update background_
			 *
			 *	@param	img			raster image to be processed to produce new background
			 *	@param	rectList	a list of rectangles containing moving objects
			 */
			void updateBackground_(RasterImage_RGBa_F *img, ConstRectList rectList);

			/**	Overloaded func
			 *	Update background_
			 *
			 *	@param	img			raster image to be processed to produce new background
			 *	@param	rectList	a list of rectangles containing moving objects
			 */
			void updateBackground_(RasterImage_RGBa* img, ConstRectList rectList);

			/**
			 *	Stores a copy of img to use as firstBackground_(NOT background_)
			 *	for future image difference computations
			 *
			 *	@param	bkgd		RasterImage_gray_F to store as firstBackground_
			 */			
			void setBackground_(RasterImage_gray_F *bkgd);

			/**
			 *	Stores a copy of img to use as firstBackground_(NOT background_)
			 *	for future image difference computations
			 *
			 *	@param	bkgd		RasterImage_gray to store as firstBackground_
			 */			
			void setBackground_(RasterImage_gray* bkgd);

			/**
			 *	Stores a copy of img to use as firstBackground_(NOT background_)
			 *	 for future image difference computations
			 *
			 *	@param	bkgd		RasterImage_RGBa_F to store as firstBackground_
			 */			
			void setBackground_(RasterImage_RGBa_F *bkgd);

			/**
			 *	Stores a copy of img to use as firstBackground_(NOT background_)
			 *	for future image difference computations
			 *
			 *	@param	bkgd		RasterImage_RGBa to store as firstBackground_
			 */			
			void setBackground_(RasterImage_RGBa* bkgd);
            
            /** Frees all space allocated for instance variables
             */
            void deleteAll_(void);

	};
}

#endif	//	BACKGROUND_REMOVER_DYNAMIC_MW_H
