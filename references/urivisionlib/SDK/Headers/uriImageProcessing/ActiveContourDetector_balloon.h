/*  NAME:
        ActiveContourDetector_balloon.h

    DESCRIPTION:
        ActiveContourDetector_balloon public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ACTIVE_CONTOUR_DETECTOR_BALLOON_H
#define URIVL_ACTIVE_CONTOUR_DETECTOR_BALLOON_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Contour_F.h"
#include "RasterImage.h"
#include "VectorField_F.h"
#include "ImageRect.h"
#include "ActiveContourDetector.h"
#include "ContourForce.h"

const int MAX_BALLOON_ITERATIONS = 300;
const float EDGECOEFFICIENT = 15.f;
const float SHRINKCOEFFICIENT = 5.f;
// variable says it all
const int BRESENHAM_STEP_SIZE = 1;
const int MIN_DIST_BTWN_PTS = BRESENHAM_STEP_SIZE + 2;//static_cast<int>(1.2 * BRESENHAM_STEP_SIZE);
// this is the threshold val in the insertionPoint method
const float GRAD_THRESHOLD = 250.0f;


namespace uriVL{

	/**	A balloon can be inflating or contracting depending on the sign of the "shrink force" applied
	 *	to its vertices.
	 */
	typedef enum BalloonType {
								EXPANDING_BALLOON = 0,
								CONTRACTING_BALLOON
	} BalloonType;
	
								
	/** A Function Class which searches for a closed contour in a given image
     *  by applying the "balloon" active contour algorithm of L. Cohen.
	 *	The balloon's vertices are connected by linear splines.
	 *
	 *	@author Clifford Manzanillo, Department of Computer Science and Statistics, URI
	 *	@author Chris Allen, 3DG, URI [major revision 2009]
	 *	@author jyh, 3DG, URI [major uriVL overhaul 2013]
	 */
	class URIVL_EXPORT  ActiveContourDetector_balloon : public ActiveContourDetector
	{
		public:
		
//			/**	Constructor.
//			 *	@param	nbContourPts	initial number of points on a new contour
//			 *	@param	imgRect			the rectangle to work within
//			 *	@param	edgeCoeff		coefficient of the edge force in total force calculation
//			 *	@param	shrinkCoeff		coefficient of the shrink/inflate force in total force calculation
//			 *	@param	tol				tolerance of the search
//			 *	@param	timeStep		the "time step" of the active contour search
//			 */
//			ActiveContourDetector_balloon(int nbContourPts, const ImageRect* imgRect,
//										float edgeCoeff, float shrinkCoeff, float tol,
//										float timeStep);
			/**	Constructor.
			 *	@param	type			does the balloon expand or contract?
			 *	@param	imgRect			the rectangle to work within
			 *	@param	crit			what convergence criterion 
			 *	@param	tol				tolerance of the search
			 *	@param	edgeCoeff		coefficient of the edge force in total force calculation
			 *	@param	shrinkCoeff		coefficient of the shrink/inflate force in total force calculation
			 *	@param	timeStep		the "time step" of the active contour search
			 */
			ActiveContourDetector_balloon(BalloonType type, const ImageRect* imgRect, 
										  ConvergenceCriterion crit, float tol,
										  float internalCoeff, float edgeCoeff, float shrinkCoeff,
										  float timeStep = DEFAULT_TIME_STEP);
			/**	Destructor
			 */
			~ActiveContourDetector_balloon(void);
			
			/**	Sets the coefficients of the detector's various force components. Thie coefficients received
			 *	are normalized by the function (so that their sum is 1).
			 *	@param	internalCoeff	coefficient of the internal, "goodness" force in total force calculation
			 *	@param	edgeCoeff		coefficient of the edge force in total force calculation
			 *	@param	shrinkCoeff		coefficient of the shrink/inflate force in total force calculation
			 */
			void setCoeffs(float internalCoeff, float edgeCoeff, float shrinkCoeff);
			
			
			/**	This function should be called for video-rate applications that must re-use
			 *	the same rasters for performance optimization purposes. In that case, the image and vector
			 *	rasters are not local to the 
			 *	@param	grad			image gradient vector field
			 *	@param	normSqrd		norm of gradient raster
			 *	@param	gradNormSqrd	gradient of norm of gradient squared raster
			 *	@param	contourOut		the contour object into which to compute the results
			 */
			void setImageProperties(VectorField_F* grad, RasterImage_gray_F* normSqrd, 
									VectorField_F* gradNormSqrd, Contour_F* contourOut);

		
			/**	Finds a contour in the input image, starting with the initial contour specified, which
			 *	will be modified and provide the output of the function
			 *	@param	imgIn		image in which to look for the contour
			 *	@param	contour		initial contour, and also output contour
			 *	@return		status of the search
			 */
			IterationStatus findContour(const RasterImage* imgIn, Contour_F* contour);
			
			
			/**	Finds a contour in the input image, starting with the initial contour specified, which
			 *	will be modified and provide the output of the function
			 *	@param	imgIn		image in which to look for the contour
			 *	@param	contour		initial contour, and also output contour
			 *	@return		status of the search
			 */
			IterationStatus findContour(VectorField_F* grad, RasterImage_gray_F* normSqrd, 
										 VectorField_F* gradNormSqrd, Contour_F* contour);
								

			/**	Renders the different forces that compose the balloon total force
			 */
			void renderForces(void);

		
		private:
			
			/**	Force that attracts the contour to the closest/strongest edge
			 */
			ContourForce* edgeForce_;
			
			/**	Force that makes the balloon contract/expand
			 */
			ContourForce* shrinkForce_;
			
			/**	Stores the gradient of norm squared of gradient of the image being operated on
			 */
			VectorField_F* gradOfNormSqrdOfGrad_;
			
			/**	Coefficient of the "edge force" in the total force computation
			 */
			float edgeCoeff_;
			
			/**	Coefficient of the "expand/contract force" in the total force computation
			 */
			float shrinkCoeff_;
			

            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ActiveContourDetector_balloon(const ActiveContourDetector_balloon& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ActiveContourDetector_balloon& operator =(const ActiveContourDetector_balloon& obj);


			/**	Depending on the value of step
			 */
			const IterationStatus findContour_(void);

			/** Uses bresenham's alg to calculate all the points
			*	in between each contour pt and inserts
			*	a new point if an edge is found beyond a certain
			*	threshold
			*/
			void searchAndInsertPoints_(Contour_F *contour, const RasterImage_gray_F *normSqrdOfGrad);

			/** Calculate the edge force for each pt on the contour
			 *	@param	vectorfield which is the normSquaredOfTheGrad
			 *	@param	contour
			 *	@param	contourforce
			*/
			void calculateEdgeForce_(Contour_F* contour, const VectorField_F* gradNorm2,
									ContourForce* edge);

			/** Calculate the shrink force for each pt on the contour
			 *	@param	contour
			 *	@param	contourforce
			*/
			void calculateShrinkForce_(Contour_F* contourIn, ContourForce* shrink);

			/** Set up the contour equaly spacing them around the image
			 *	@param	contour
			*/
			void initializeContour_(Contour_F* contour);

			/**	Applies one iteration of the balloon algorithm
			 *
			 *	@return	contour detection iteration status structure
			 */
			IterationStatus oneStep_(void);
			
//			/** Attempts to find an edge by going a fixed distance from a 
//			 *	a particular point
//			 *
//			 *	@param	start		an ImagePoint_F that represents the starting point
//			 *	@param	maxDist		a float representing the maximum distance from 
//			 *						the starting point that should be searched
//			 *	@param	dx			the displacement in the x
//			 *	@param	dy			the displacement in the y
//			 *	@param	normSquaredOfGrad	a RasterImage_gray_F representing the
//			 *								norm squared of the gradient of the
//			 *								points in the VectorField.
//			 *	@returns	true	if an edge is detected
//			 */
//			bool scout_(ImagePoint_F* start, float maxDist, float dx, float dy, 
//						const RasterImage_gray_F* normSquaredOfGrad);
//		

			/**	Private helper method designed to facilitate using our
			 *	modified Bresenham's Algorithm in searchAndInsertPoints() method
			 *	
			 *	@param	dx	the initially calculated change in the x-values
			 *	@param	dy	the initially calculated change in the y-values
			 *	@param	x	an array of size two representing the pair of
			 *				x-values (x_0, x_f)
			 *	@param	y	an array of size two representing the pair of
			 *				y-values (y_0, y_f)
			 *	@param	octant	an interger value representing what octant the
			 *					pair of points lies in.
			 *	@param	swapped0F	a boolean value used in conjunction with
			 *						octant to determine the order that points
			 *						are inserted onto the contour.
			 */
			void transToBresenhamSpace_(int &dx, int &dy, int x[], int y[], int& octant, bool& swapped0F);
			
			/** This is a private helper method designed to facilitate using
			 *	our modified Bresenham's Algorithm to transform the computed
			 *	point from the space of the Bresenham's Algorithm to its 
			 *	appropriate reflection about the various axes. 
			 *
			 *	@param	xVal	the computed x-value
			 *	@param	yVal	the computed y-value
			 *	@param	octant	the octant that the point is supposed to be in
			 */
			void transFromBresenhamSpace_(int& xVal, int& yVal, int octant);
		
			/** Swaps the values of X-pairs and Y-pairs 
			 *	(x0,x1)->(y0,y1)
			 *	
			 *	@param	X	a pointer to the array representing the pair
			 *				of x values.
			 *	@param	Y	a pointer to the array representing the pair
			 *				of y values.
			 */
			void inline swapXY_(int x[], int y[]);
			 
			/** Swaps the values of X and Y
			 *
			 *	 @param	X	a reference to the X value.
			 *	 @param	Y	a reference to the Y value.
			 */
			void inline swapXY_(int &x, int &y);
			 
//			/** Inverts the Y-pair vaules (y0,y1)->(-y0,-y1)
//			 *
//			 *	 @param	Y	a pointer to the array representing the pair
//			 *				of y values.
//			 */
//			void inline invertY_(int* y);
//			  
//			/** Inverts the Y value y -> -y
//			 *
//			 *  @param	Y	a reference to the Y value.
//			 */
//			void inline invertY_(int &y);
			  
			/** Swaps the initial point's values with the final point's values
			 *
			 *	@param	x	an array representing the pair of x-values
			 *	@param	y	an array representing the pair of y-values
			 */
			void inline swap0F_(int x[], int y[]);
			
	};
}

#endif	//	URIVL_ACTIVE_CONTOUR_DETECTOR_BALLOON_H
