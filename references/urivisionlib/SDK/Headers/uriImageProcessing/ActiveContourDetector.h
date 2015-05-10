/*  NAME:
        ActiveContourDetector.h

    DESCRIPTION:
        ActiveContourDetector public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ACTIVE_CONTOUR_DETECTOR_H
#define URIVL_ACTIVE_CONTOUR_DETECTOR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageRect.h"
#include "Contour_F.h"
#include "RasterImage.h"
#include "VectorField_F.h"
#include "RasterImage_gray_F.h"
#include "ContourForce.h"
#include "ContourDetector.h"
#include "ImageGradient_Gaussian_F.h"
#include "ColorImageGradient_Gaussian_F.h"
#include "ImageGradient_Gaussian_F.h"
#include "IterationStatus.h"


namespace uriVL
{
	/** Parent class for all implementations of active contour algorithms for seeking closed contours
     *  in an image.	
	 *
	 *	In uriVL there is no such thing as an "active contour object."  There is a Contour class
	 *	(a Contour object being made up of a list of ImagePoint objects) and then there is a 
	 *	Hierarchy of ActiveContourDetector classes, each of which implements a specific algorithm for
	 *	actively adjusting the vertices of a Contour object to fit the contour of a region in an image.
	 *	All ActiveContourDetector classes proceed by summing up diverse "force" components---implemented
	 *	as ContourForce objects mapped to the current Contour---such as "edge force," "internal force," etc.,
	 *	to produce the "total force" that is applied to the vertices of the current contour.  The fitting of
	 *	the active contour is therefore modeled as the convergence of a diffusion equation, which requires
	 *	a "time step" constant, a criterion for convergence (average displacement, maximum displacement,
	 *	etc.), and a numerical threshold associated to that criterion.
     *
	 *	@author Clifford Manzanillo, Department of Computer Science and Statistics, URI
	 *	@author Chris Allen, 3DG, URI [major revision 2009]
	 *	@author jyh, 3DG, URI [major uriVL overhaul 2013]
	 */
	class URIVL_EXPORT  ActiveContourDetector : public ContourDetector
	{
		public:
		
			/**	Constructor.  If a non-null ImageRect pointer is passed, the ActiveContourDetector
			 *	will allocate accessory object (RasterImage and VectorField) at the dimensions
			 *	specified by the ImageRect.
			 *	<br></br>
			 *	Whether or not to pass a non-null ImageRect pointer to this constructor
			 *	depends on the way this object will be used:
			 *	<ul>
			 *		<li>If it's a one-shot use, then it does not make much difference, except
			 *			maybe to allow to allocate all accessory objects in advance.</li>
			 *		<li>If the ActiveContourDetector will be used multiple times, then
			 *		<ul>
			 *			<li>If the accessory objects are computed outside of the ActiveContourDetector,
			 *			(for example as part of preprocessing), then pass a null pointer
			 *			to this constructor;</li>
			 *			<li>Otherwise, send a pointer to a valid ImageRect</li>
			 *		</ul></li>
			 *	</ul>
			 *	@param	imgInRect	the rectangle to work within. If a rectangle is specified, then
			 *	this object will preallocate 
			 */
			ActiveContourDetector(const ImageRect* imgInRect = NULL);
								
			/**	Destructor. Note that the destructor does not free the Contour_F object it has
			 *	a pointer to, as that object was created outside of this class and a pointer to
			 *	the Contour_F passed as destination for the contour finding computation.
			 */
			virtual ~ActiveContourDetector(void);
			

			/**	Finds <b>one</b> contour in the input image, starting with the initial contour specified, which
			 *	will be modified and provide the output of the function
			 *	@param	imgIn		image in which to look for the contour
			 *	@param	contourIn	initial contour, and also output contour
			 */
			virtual IterationStatus findContour(const RasterImage* imgIn, Contour_F* contour,
												const ImageRect* searchRect = NULL) = 0;
			
			
			/**	 Finds <b>all</b> contours within the specified search rectangle of the input image.
			 *	To be implemented by subclasses.
			 *	@param	imgIn	the image to process
			 *	@param	rect	the rectangle within which to look for a contour.
			 *	@return	a list of pointers to the contour objects found in the image
			 */
            std::list<Contour_F*> * findAllContours(const RasterImage* imgIn, const ImageRect* rect=NULL);


			/**	Returns a read-only pointer to the contour being computed (when operating in step-by-step mode).
			 *	@return read-only pointer to the contour being computed
			 */
			const Contour_F* getContour(void) const;

			/**	Returns the current iteration status without computing an iteration step
			 *	@return		reference to the current iteration status
			 */
			IterationStatus getStatus(void) const;


			/**	Renders the different forces applied to the contour's vertices
			 */
			virtual void renderForces(void) = 0;
			
			/**	Allows the user to change the mode in which the detector operate: "discrete" mode
			 *	(n steps at a time) or "full" mode (runs to convergence or max number iterations).
			 *
			 *	@param	sbs		true if step-by-step mode is selected by the user
			 *
			 *	@see	stepByStep_
			 */
			void setStepByStep(bool sbs);
			

			/**	When in step-by-step mode, sets the number of iterations run at each "step"
			 *	@param	size	size of a step (number of regular iterations)
			 */
			void setStepSize(int size);
			

			/**	Aplies n iterations of the algorithm (or fewer if convergence occurs before).
			 *	Can only be called when the detector operates in "step-by-step" mode and
			 *	after one step of calculation or more has been performed (by a call to
			 *	<code>findContour</code>)
			 *
			 *	@param	n	number of iteration steps to process
			 *	@return		an iteration status record
			 *
			 *	@see	setStepByStep
			 *	@see	findContour
			 *	@see	stepByStep_
			 *	@see	setupCompleted_
			 */
			IterationStatus iterate(int n);
			
			/**	Reports whether this object "owns" the various gradient rasters and vector fields
			 *	and should delete them in the destructor
			 *	@return true if this object "owns" the various gradient rasters and vector fields
			 */
			inline bool gradientsAreLocal(void) const
			{
				return gradientsAreLocal_;
			}

			/**	Returns the value of the "time step" that scales the total force to produce a displacement
			 *	@see timeStep_
			 *	@return value of the "time step"
			 */
			float getTimeStep(void) const;
			

		protected:


			/**	Default scale of the gradient of Gaussian.  In the Xu & Price paper it is 1.5f
			 */
			static const float DEFAULT_GAUSSIAN_FILTER_SCALE;
			
			/**	Default value for the maximimum magnitude of the internal force
			 */
			static const float DEFAULT_MAX_INTERNAL_FORCE;
			
			/**	Default value for the internal force coefficient
			 */
			static const float DEFAULT_INTERNAL_COEFF;
			
			/**	Default time step in the iteration equation, arbitrarily set
			 */
			static const float DEFAULT_TIME_STEP;

			/**	Default time step in the iteration equation, arbitrarily set
			 */
			static const float DEFAULT_TOLERANCE;

			/** Default value for the energy contributed by the distance between control points.
			 */
			static const float DEFAULT_ALPHA_1ST_DERIV;
			
			/** Default value for the energy contributed by the curvature of the snake.
			 */
			static const float DEFAULT_BETA_2ND_DERIV;


			/**	Calculates the internal force that preserves the quality of the contour
			 *	(points regularly spread out on the contour, minimization of curvature)
			 *
			 */
			void calculateInternalForce_(void);

			/**	Called by a subclass to set the coeefficient of the internal force in the
			 *	computation of the total force
			 *	@param	internalCoeff	value to assign to the coefficient of the internal force
			 */
			void setInternalCoeff_(float internalCoeff);

			/**	This function should be called for video-rate applications that must re-use
			 *	the same rasters for performance optimization purposes. In that case, the image and vector
			 *	rasters are not local to the 
			 *	@param	grad			image gradient vector field
			 *	@param	normSqrd		norm of gradient raster
			 */
			void setGradientRasters_(VectorField_F* grad, RasterImage_gray_F* normSqrd);

			/**	Assigns a contour object, typically, to start an iteration.  This object will now use that contour 
			 *	object for its search and modify it while doing so.
			 *	@param	contour	the initial contour for the search
			 */
			void setContour_(Contour_F* contour);

            /** Returns a writable pointer to the contour force's geometric contour (list of points)
             *
             *  @return     a writable pointer to the contour force's geometric contour
             */
			Contour_F* getContour_(ReadWriteStatus rw);

//		private:

			/**	Stores the gradient of the image being operated on
			 */
			VectorField_F* imageGradient_;
			
			/**	Stores as a gray-level image the norm squared of gradient of the image being operated on
			 */
			RasterImage_gray_F* normSqrdOfGrad_;
			
			/**	Used when processing a color image
			 */
			ColorImageGradient_Gaussian_F* colorImageGradientProcessor_;
			
			/**	Used when processing a gray-level image
			 */
			ImageGradient_Gaussian_F* grayImageGradientProcessor_;

			/**	Pointer to the Contour_F object being worked on by this object.  The Contour_F
			 *	object is not "owned" by this object and will not be freed by the destructor.
			 */
			Contour_F* contour_;
			
			/**	Sum of all forces applied to each vertex of the active contour
			 */
			ContourForce* totalForce_;
			
			/**	Internal, "goodness" component of the force applied to each vertex of the active contour
			 */
			ContourForce* internalForce_;
			
			/**	Status of the current iteration
			 */
			IterationStatus status_;

			/**	Coefficient of the internal force in the computation of the total force
			 */
			float internalCoeff_;
			
			/**	Scale of the gradient of Gaussian.  In the Xu & Price paper it is 1.5f
			 */
			float gaussianGradientScale_;
			
			/**	Energy contributed by the distance between control points to the internal force
			 */
			float alphaFirstDeriv_;
			
			/** Energy contributed by the curvature of the snake  to the internal force.  Larger
			 *	values of beta cause bends in the snake to have a high cost
			 * and lead to smoother snakes.
			 */
			float betaSecondDeriv_;

			/**	Maximimum magnitude of the internal force
			 */
			float maxInternalForce_;
			
			/**	Time "step" in the integration of the differential equation
			 */
			float timeStep_;
			
			/**	Convergence threshold, for whatever convergence criterion is being used
			 */
			float tolerance_;

			/**	Number of iterations done so far
			 */
			int nbIterations_;

			/**	Indicates whether the contour detector allocated its own gradient, norm
			 *	of gradient, etc. objects
			 */
			bool gradientsAreLocal_;

			/**	Indicates whether the contour detector operates in "discrete" mode (n steps at
			 *	a time) or "full" mode (runs to convergence or max number iterations).
			 *
			 *	@see	setStepByStep
			 */
			bool stepByStep_ ;

			/**	How many integration steps to apply when in "step-by"step" mode
			 */
			int stepSize_;

			/**	True after the detector has run at least one iteration step, so that the
			 *  <code>iterate</code> function can be called to iterate one or more steps.
			 *	This boolean is set true after one step of calculation or more has been
			 *	performed (by a call to <code>findContour</code>)
			 *
			 *	@see iterate
			 */
			bool setupCompleted_;

			
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ActiveContourDetector(const ActiveContourDetector& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ActiveContourDetector& operator =(const ActiveContourDetector& obj);


			/**	Applies one iteration of the contour determination algorithm
			 *
			 *	@return	contour detection iteration status structure
			 */
			virtual IterationStatus oneStep_(void) = 0;


	};
}

#endif	//	URIVL_ACTIVE_CONTOUR_DETECTOR_H
