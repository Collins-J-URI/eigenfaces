/*  NAME:
        ActiveContourDetector_GVF.h

    DESCRIPTION:
        ActiveContourDetector_GVF public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ACTIVE_CONTOUR_DETECTOR_GVF_H
#define URIVL_ACTIVE_CONTOUR_DETECTOR_GVF_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Contour_F.h"
#include "RasterImage.h"
#include "VectorField.h"
#include "RasterImage_gray_F.h"
#include "ActiveContourDetector.h"
#include "ContourForce.h"
#include "ImageRect.h"

//coefficient for GVF stability (theTimeStep_ * MU <= .25 for stability)
// theTimeStep_ must also be <= (1 / (4*MU))
// MU can be be anything < 12 but not equal to 12
const float MU = 0.2f;

namespace uriVL{

	/** A Function Class which searches for a closed contour in a given image
     *  by applying the GVF (gradient vector field) active contour algorithm
     *  of Xu & Price.
	 *
	 *	@author Clifford Manzanillo, Department of Computer Science and Statistics, URI
	 *	@author jyh, 3DG, URI [major uriVL overhaul 2011]
	 */
	class URIVL_EXPORT ActiveContourDetector_GVF : public ActiveContourDetector
	{
		public:
		
			/**	Creates a new detector that will operate within the image rectangle specified
			 *	@param	nbContourPts	number of points of initial contours
			 *	@param	imgRect			rectangle within which to operate
			 *	@param	tol				tolerance of the search
			 *	@param	time			search time [???]
			 */
			ActiveContourDetector_GVF(int nbContourPts, const ImageRect* imgRect, float tol, float time);
			
			/**	Destructor
			 */
			~ActiveContourDetector_GVF(void);
			

			/**	Finds a contour in the input image, starting with the initial contour specified, which
			 *	will be modified and provide the output of the function
			 *	@param	imgIn		image in which to look for the contour
			 *	@param	contour		initial contour, and also output contour
			 */
			IterationStatus findContour(const RasterImage* imgIn, Contour_F* contour);
			
			
			/**	Finds a contour in the input image, starting with the initial contour specified, which
			 *	will be modified and provide the output of the function
			 *	@param	imgIn		image in which to look for the contour
			 *	@param	imgIn		image in which to look for the contour
			 *	@param	contour		initial contour, and also output contour
			 */
			IterationStatus findContour(const RasterImage* imgIn, VectorField_F* grad, 
										RasterImage_gray_F* normSqrd, VectorField_F* gradNormSqrd, 
										Contour_F* contour);
								

			void renderForces(void);

			static const int MAX_GVF_ITERATIONS = 1;


		private:
			ContourForce	*gvfForce_;
			Contour_F		*contourProc_;

			VectorField_F	*c_,
							*gvfVector_,
							*gvfVectorPlus1_;


            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ActiveContourDetector_GVF(const ActiveContourDetector_GVF& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ActiveContourDetector_GVF& operator =(const ActiveContourDetector_GVF& obj);


			const IterationStatus& findContour_(void);


			void computeGradientVectorFlow_(void);
			void calculateGvfForce_(VectorField_F* theGrad, VectorField_F* theGvf);
			void calculateGvfForce_(VectorField_F* grad, const ImageRect* theRect, VectorField_F* gvf);

			/**	Applies one iteration of the balloon algorithm
			 *
			 *	@return	contour detection iteration status structure
			 */
			void oneStep_(IterationStatus* iterationStatus);
	};
}

#endif	//	URIVL_ACTIVE_CONTOUR_DETECTOR_GVF_H

