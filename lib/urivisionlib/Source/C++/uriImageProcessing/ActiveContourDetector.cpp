/*  NAME:
ActiveContourDetector.cpp

DESCRIPTION:
implementation of the uriVisionLib ActiveContourDetector class

COPYRIGHT:
(c) 2003-2014, 3D Group for Interactive Visualization
University of Rhode Island.

Licensed under the Academic Free License - v. 2.1
For more information see http://opensource.org/licenses/afl-2.1.php
*/


#include <list>
#include <iostream>
//
#include "ActiveContourDetector.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  static members & constants
//----------------------------------------------------------
const float INTERNAL_FORCE_DEFAULT_COLOR[] = {0.f, 1.f, 0.f};
const float TOTAL_FORCE_DEFAULT_COLOR[] = {1.f, 0.f, 0.f};

const float ActiveContourDetector::DEFAULT_GAUSSIAN_FILTER_SCALE = 2.0f;
const float ActiveContourDetector::DEFAULT_INTERNAL_COEFF = 1.f;
const float ActiveContourDetector::DEFAULT_MAX_INTERNAL_FORCE = 5.1f;
const float ActiveContourDetector::DEFAULT_TIME_STEP = 0.15f;
const float ActiveContourDetector::DEFAULT_ALPHA_1ST_DERIV = 4.f;
const float ActiveContourDetector::DEFAULT_BETA_2ND_DERIV = 0.1f;
const float ActiveContourDetector::DEFAULT_TOLERANCE = (float)1E-5;


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif


ActiveContourDetector::ActiveContourDetector(const ImageRect* imgRect)
		try	:	ContourDetector(imgRect),
				//
				imageGradient_(NULL),
				normSqrdOfGrad_(NULL),
				colorImageGradientProcessor_(NULL),
				grayImageGradientProcessor_(NULL),
				//
				contour_(NULL),
				totalForce_(NULL),
				internalForce_(NULL),
				status_(IterationStatus()),
				//
				internalCoeff_(DEFAULT_INTERNAL_COEFF),
				gaussianGradientScale_(DEFAULT_GAUSSIAN_FILTER_SCALE),
				alphaFirstDeriv_(DEFAULT_ALPHA_1ST_DERIV),
				betaSecondDeriv_(DEFAULT_BETA_2ND_DERIV),
				maxInternalForce_(DEFAULT_MAX_INTERNAL_FORCE),
				timeStep_(DEFAULT_TIME_STEP),
				tolerance_(DEFAULT_TOLERANCE),
				//
				nbIterations_(0),
				gradientsAreLocal_(true),
				stepByStep_(false),
				stepSize_(1),
				setupCompleted_(false)
{
	totalForce_->setNeedleColor(TOTAL_FORCE_DEFAULT_COLOR);
	internalForce_->setNeedleColor(INTERNAL_FORCE_DEFAULT_COLOR);
	//
	totalForce_->setRenderContour(false);
	internalForce_->setRenderContour(false);
	//
	totalForce_->mapToContour(contour_);
	internalForce_->mapToContour(contour_);
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by ActiveContourDetector constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"In ActiveContourDetector constructor");
}



ActiveContourDetector::ActiveContourDetector(const ActiveContourDetector& theObj)
		:		ContourDetector(),
				//
				imageGradient_(NULL),
				normSqrdOfGrad_(NULL),
				colorImageGradientProcessor_(NULL),
				grayImageGradientProcessor_(NULL),
				//
				contour_(NULL),
				totalForce_(NULL),
				internalForce_(NULL),
				status_(IterationStatus()),
				//
				internalCoeff_(DEFAULT_INTERNAL_COEFF),
				gaussianGradientScale_(DEFAULT_GAUSSIAN_FILTER_SCALE),
				alphaFirstDeriv_(DEFAULT_ALPHA_1ST_DERIV),
				betaSecondDeriv_(DEFAULT_BETA_2ND_DERIV),
				maxInternalForce_(DEFAULT_MAX_INTERNAL_FORCE),
				timeStep_(DEFAULT_TIME_STEP),
				tolerance_(DEFAULT_TOLERANCE),
				//
				nbIterations_(0),
				gradientsAreLocal_(true),
				stepByStep_(false),
				stepSize_(1),
				setupCompleted_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ActiveContourDetector copy constructor not implemented.");
}



ActiveContourDetector::~ActiveContourDetector(void){
	if (colorImageGradientProcessor_)
		delete colorImageGradientProcessor_;
	if (grayImageGradientProcessor_)
		delete grayImageGradientProcessor_;
	if (internalForce_)
		delete internalForce_;
	if (totalForce_)
		delete totalForce_;
		
	//	only delete the gradient objects if they are local
	if (gradientsAreLocal()){
		if (imageGradient_)
			delete imageGradient_;
		if (normSqrdOfGrad_)
			delete normSqrdOfGrad_;
	}	
}




const ActiveContourDetector& ActiveContourDetector::operator = (const ActiveContourDetector& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ActiveContourDetector copy operator not implemented.");

	return *this;
}

#if 0
//----------------------------------------
#pragma mark Contour finding functions
//----------------------------------------
#endif



list<Contour_F*> * ActiveContourDetector::findAllContours(const RasterImage* imgIn, const ImageRect* rect)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ActiveContourDetector::findAllContours function not implemented.");
	return NULL;
}


#if 0
//----------------------------------------
#pragma mark Setters and getters
//----------------------------------------
#endif

void ActiveContourDetector::setInternalCoeff_(float internalCoeff)
{
	internalCoeff_ = internalCoeff;
}

void ActiveContourDetector::setGradientRasters_(VectorField_F* grad, RasterImage_gray_F* normSqrd)
{
	imageGradient_ = grad;
	normSqrdOfGrad_ = normSqrd;
	gradientsAreLocal_ = false;
}


const Contour_F* ActiveContourDetector::getContour(void) const
{
	return contour_;
}

Contour_F* ActiveContourDetector::getContour_(ReadWriteStatus rw)
{
	return contour_;
}

void ActiveContourDetector::setContour_(Contour_F*  contour)
{
	if (contour_ != NULL)
	{
		FAIL_CONDITION( true,
						kFunctionNotImplemented,
						"Not completely finished.  Unusure yet if contour will ever be local to an instance of the class");
	}
	
	contour_ = contour;
}

//Contour_F* ActiveContourDetector::getContour(ReadWriteStatus rw)
//{
//	return contour_;
//}

void ActiveContourDetector::setStepByStep(bool sbs)
{
	stepByStep_ = sbs;
}
void ActiveContourDetector::setStepSize(int size)
{
	stepSize_ = size;
}


IterationStatus ActiveContourDetector::getStatus(void) const
{
	return status_;
}

float ActiveContourDetector::getTimeStep(void) const
{
	return timeStep_;
}

//IterationStatus& ActiveContourDetector::getStatus(ReadWriteStatus rw)
//{
//	return status_;
//}

#if 0
//----------------------------------------
#pragma mark Iteration functions
//----------------------------------------
#endif


IterationStatus ActiveContourDetector::iterate(int nbSteps)
{
	//	if this balloon detector has been set to operate in "step by step" mode
	//	and has been properly configured (image and input contour sent in),
	//	execute the number of steps specified.
	if (setupCompleted_){
		bool converged = false;
		int count = 0;
		for(int i=0; (i < nbSteps) && !converged ; i++){
			IterationStatus stepStatus = oneStep_();
			converged = stepStatus.hasConverged();
			count++;
		}
		if(converged){
			cout<< count << " iterations (convergence)" << endl;
		}

	}

	return getStatus();
}


// v(s) = (x(s), y(s)) 
// elasticity --> .5 * INTEGRAL( alpha(s) |Vs|^2) with respect to ds where Vs = dv(s)/ds
// bending  --> .5 * INTEGRAL( beta(s) |Vss|^2 with respect to ds
void ActiveContourDetector::calculateInternalForce_(void)
{
	const list<Vector2D_F*> *internalForce = internalForce_->getForce();
	const Contour_F* contour = internalForce_->getContour();
	const list<ImagePoint_F*> *contourPts = contour->getPoints();

	list<ImagePoint_F*>::const_iterator itm2, itm1, itp1, itp2, contourEnd = contourPts->end();
	list<Vector2D_F*>::const_iterator intIter;
	float	w2 = DEFAULT_BETA_2ND_DERIV,
			w2Times4 = 4.f*w2,
			w2Times6 = 6.f*w2,
			w1 = DEFAULT_ALPHA_1ST_DERIV,
			w1Times2 = 2.f*w1;
	float	a, b, c;
	float	im2X, im1X, ip1X, ip2X, iX;
	float	im2Y, im1Y, ip1Y, ip2Y, iY;

	// begin by setting all the contour pts to the 
	// correct positions
	itm2 = contourPts->end();
	itm2--;
	itm1 = itm2;
	itm2--;
	itp2 = contourPts->begin();
	itp2++;
	itp1 = itp2;
	itp2++;
	//
	intIter = internalForce->begin();

	for(list<ImagePoint_F*>::const_iterator iter = contourPts->begin(); iter != contourEnd; iter++, intIter++){
		/* 	
			The following stems from the equations below.  
			We basically loop through each point call them C, and 
			compute the 2nd and 4th derivatives by the difference of points
			
			-(W1*Ci')' + (W2*Ci")" = AV
			-W1*Ci" + W2*Ci"" = AV
			Ci" = C(i+1) - 2 * Ci + C(i-1)
			Ci"" = C(i+2) - 4 * C(i+1) + 6 * Ci - 4C(i-1) + C(i-2)s
		*/

		a = -w2;
		b = w1 + w2Times4;
		c = -w1Times2 - w2Times6;

		im2X = (*itm2)->getX();
		im1X = (*itm1)->getX();
		iX   = (*iter)->getX();
		ip1X = (*itp1)->getX();
		ip2X = (*itp2)->getX();

		im2Y = (*itm2)->getY();
		im1Y = (*itm1)->getY();
		iY   = (*iter)->getY();
		ip1Y = (*itp1)->getY();
		ip2Y = (*itp2)->getY();

		float  vectX = a * (im2X + ip2X ) + b * (im1X + ip1X ) + c * iX,
			vectY = a * (im2Y + ip2Y ) + b * (im1Y + ip1Y ) + c * iY,
			normVect2 = vectX*vectX + vectY*vectY;

		if (normVect2 > DEFAULT_MAX_INTERNAL_FORCE) {
			float scale = static_cast<float>(DEFAULT_MAX_INTERNAL_FORCE / sqrt(normVect2));
			vectX *= scale;
			vectY *= scale;
		}

		(*intIter)->setX(timeStep_ * vectX);
		(*intIter)->setY(timeStep_ * vectY);

		itm2 = itm1;
		itm1 = iter;
		itp1 = itp2;
		itp2++;
		if (itp2 == contourPts->end()){
			itp2 = contourPts->begin();
		}
	}
}

