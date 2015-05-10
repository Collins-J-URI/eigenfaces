/*  NAME:
        ActiveContourDetector_balloon.cpp
 
    DESCRIPTION:
        ActiveContourDetector_balloon source file.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#include <iostream>
#include <fstream>
#include <algorithm>
//
#include "ActiveContourDetector_balloon.h"
#include "ImagePoint_F.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  static members & constants
//----------------------------------------------------------
const float SHRINK_FORCE_DEFAULT_COLOR[] = {0.f, 0.f, 1.f};
const float EDGE_FORCE_DEFAULT_COLOR[] = {0.5f, 0.5f, 0.5f};

#if 0
//----------------------------------------
#pragma mark -
#pragma mark Constructors and destructor
//----------------------------------------
#endif


ActiveContourDetector_balloon::ActiveContourDetector_balloon(BalloonType type, const ImageRect* imgRect, 
															 ConvergenceCriterion crit, float tol,
															 float internalCoeff, float edgeCoeff, 
															 float shrinkCoeff, float timeStep)
		try			:	ActiveContourDetector(imgRect),
						//
						edgeForce_(new ContourForce()),
						shrinkForce_(new ContourForce()),
						gradOfNormSqrdOfGrad_(NULL),
						//
						edgeCoeff_(edgeCoeff),
						shrinkCoeff_(shrinkCoeff)			
{
	shrinkForce_->setNeedleColor(SHRINK_FORCE_DEFAULT_COLOR);
	edgeForce_->setNeedleColor(EDGE_FORCE_DEFAULT_COLOR);
}
catch (ErrorReport& e) 
{
	e.appendToMessage("called by ActiveContourDetector_balloon constructor.");
	throw e;
}
catch(...) 
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"invalid parameters sent to ActiveContourDetector_balloon constructor.");
}

ActiveContourDetector_balloon::ActiveContourDetector_balloon(const ActiveContourDetector_balloon& theObj)
			:	ActiveContourDetector(),
				//
				edgeForce_(NULL),
				shrinkForce_(NULL),
				gradOfNormSqrdOfGrad_(NULL),
				//
				edgeCoeff_(0.f),
				shrinkCoeff_(0.f)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ActiveContourDetector_balloon copy constructor not implemented.");
}


ActiveContourDetector_balloon::~ActiveContourDetector_balloon(void){
	delete edgeForce_;
	delete shrinkForce_;

	//	only delete the gradient objects if they are local
	if (gradientsAreLocal())
	{
		if (gradOfNormSqrdOfGrad_)
			delete gradOfNormSqrdOfGrad_;

	}	
}



const ActiveContourDetector_balloon& ActiveContourDetector_balloon::operator = (const ActiveContourDetector_balloon& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ActiveContourDetector_balloon copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//-------------------------------------------
#pragma mark Setters and getters
//-------------------------------------------
#endif


void ActiveContourDetector_balloon::setCoeffs(float internalCoeff, float edgeCoeff, float shrinkCoeff)
{
	//	Normalize coefficients sent by the user, just in case.
	const float coeffScale = 1.f / (internalCoeff + edgeCoeff + shrinkCoeff);
	
	ActiveContourDetector::setInternalCoeff_(coeffScale * internalCoeff);
	
	edgeCoeff_ = edgeCoeff * coeffScale;
	shrinkCoeff_ = shrinkCoeff * coeffScale;
}

void ActiveContourDetector_balloon::setImageProperties(VectorField_F* grad, RasterImage_gray_F* normSqrd, 
													   VectorField_F* gradNormSqrd, Contour_F* contourOut)
{
	//	We don't need to allocate new gradient objects.  We just keep references to the ones
	//	computed by the caller.
	//	The parent class's function also sets gradientsAreLocal_ to 'false'
	setGradientRasters_(grad, normSqrd);

	gradOfNormSqrdOfGrad_ = gradNormSqrd;

	//	All the computation will be done inside balloonContour_, which is an alias of contourOut
	//	and whose points are initialized as that of contourIn.
	setContour_(contourOut);
	
	edgeForce_->mapToContour(contourOut);
	shrinkForce_->mapToContour(contourOut);
	internalForce_->mapToContour(contourOut);
	totalForce_->mapToContour(contourOut);
}

#if 0
#pragma mark -
//-------------------------------------------
#pragma mark Public find countour functions
//-------------------------------------------
#endif


IterationStatus ActiveContourDetector_balloon::findContour(const RasterImage* imgIn, Contour_F* contour)
{
	FAIL_CONDITION( imgIn == NULL,
					kNullRasterImageError,
					"null RasterImage passed as input to ActiveContourDetector_balloon");
	FAIL_CONDITION( contour == NULL,
					kNullParameterError,
					"null Contour passed as input to ActiveContourDetector_balloon");


	const ImageRect* procRect = getSearchRect();
    imageGradient_ 	= new VectorField_F(procRect);
    normSqrdOfGrad_ = new RasterImage_gray_F(procRect);
    gradOfNormSqrdOfGrad_ = new VectorField_F(procRect);

	//  we only need a color gradient if the input image is a color image
	if(imgIn->getBaseRasterType() == kRGBa32Raster){
		colorImageGradientProcessor_ = new ColorImageGradient_Gaussian_F(DEFAULT_GAUSSIAN_FILTER_SCALE);
		colorImageGradientProcessor_->setColorGradientMode(GRAD_LARGEST_COLOR_COMPONENT);
		colorImageGradientProcessor_->applyInto(imgIn, imageGradient_);
	}
	else{
		grayImageGradientProcessor_ = new ImageGradient_Gaussian_F(DEFAULT_GAUSSIAN_FILTER_SCALE);
		grayImageGradientProcessor_->applyInto(imgIn, imageGradient_);
	}

	imageGradient_->getNormSquaredAsRasterImage(normSqrdOfGrad_);
	grayImageGradientProcessor_->applyInto(normSqrdOfGrad_, gradOfNormSqrdOfGrad_);
	//
	gradientsAreLocal_ = true;

	//	All the computation will be done inside balloonContour_, which is an alias of contourOut
	//	and whose points are initialized as that of contourIn.
	setContour_(contour);

	return findContour_( );
}

IterationStatus ActiveContourDetector_balloon::findContour(VectorField_F* grad, RasterImage_gray_F* normSqrd, 
														  VectorField_F* gradNormSqrd, Contour_F* contour)
{
	FAIL_CONDITION( grad == NULL,
					kNullVectorFieldError,
					"null gradient passed as input to ActiveContourDetector_balloon");
	FAIL_CONDITION( normSqrd == NULL,
					kNullRasterImageError,
					"null  gradient norm image passed as input to ActiveContourDetector_balloon");
	FAIL_CONDITION( gradNormSqrd == NULL,
					kNullVectorFieldError,
					"null grad-of-norm2 passed as input to ActiveContourDetector_balloon");
	FAIL_CONDITION( contour == NULL,
					kNullParameterError,
					"null Contour passed as input to ActiveContourDetector_balloon");

	//	We don't need to allocate new gradient objects.  We just keep references to the ones
	//	computed by the caller.
	imageGradient_ = grad;
	normSqrdOfGrad_ = normSqrd;
	gradOfNormSqrdOfGrad_ = gradNormSqrd;
	gradientsAreLocal_ = false;

	//	All the computation will be done inside balloonContour_, which is an alias of contourOut
	//	and whose points are initialized as that of contourIn.
	setContour_(contour);

	return findContour_();
}


#if 0
#pragma mark -
//-------------------------------------------
#pragma mark Private calculation functions
//-------------------------------------------
#endif


const IterationStatus ActiveContourDetector_balloon::findContour_(void)
{	
	Contour_F* contour = getContour_(R_W_ACCESS);
	
	edgeForce_->mapToContour(contour);
	shrinkForce_->mapToContour(contour);
	internalForce_->mapToContour(contour);
	totalForce_->mapToContour(contour);

	//	at this point the setup is complete.  We could proceed either step by step if the user
	//	asked us to do so.
	setupCompleted_ = true;
	
	IterationStatus iterationStatus;

	if (stepByStep_)
	{
		for(int i=0; (i < stepSize_) && !iterationStatus.hasConverged(); i++)
		{
			iterationStatus = oneStep_();
		}
	}
	else
	{
		iterationStatus = iterate(MAX_BALLOON_ITERATIONS);
	}

	return iterationStatus;
}

IterationStatus ActiveContourDetector_balloon::oneStep_(void){

	/* 	here i calc all the forces then add them to the total force
		then i apply the total force to the contour
		after this i set it back to zero for the next iteration.
	
		these forces stem from the equation
		Eint + Eimage = 0
		where 
			Eint = Internal Force
			Eimage = Balloon Force -> (-k1(vectorNormal2Curve) + k2(grad/normOfGrad))
	 */
	IterationStatus status;
	Contour_F* contour = getContour_(R_W_ACCESS);
	
	// pretty smooth.. just calc the forces on each point
	calculateInternalForce_();
	calculateEdgeForce_(contour, gradOfNormSqrdOfGrad_, edgeForce_);
	calculateShrinkForce_(contour, shrinkForce_);

	// add them up to be one total force
	totalForce_->setForcesToZero();
	totalForce_->addForce(internalForce_);
	totalForce_->addForce(edgeForce_);
	totalForce_->addForce(shrinkForce_);
	
	// apply the force and if it has converged then don't insert pts
	bool contourHasConverged = totalForce_->applyForceToContour();
	if (contourHasConverged)
	{ 
		status.setConverged();
	}
	else
	{
		// check to see if after the move we need to insert a point
		searchAndInsertPoints_(contour, normSqrdOfGrad_);
	}
	nbIterations_++;
	
	return status;
}


void ActiveContourDetector_balloon::calculateShrinkForce_(Contour_F* contourIn, ContourForce* shrink){

	const float TIMESTEP_MULT_EDGE_COEFF = edgeCoeff_ * getTimeStep();
	//const float NORM_SQRD_TOL = 250.f;

	float	shrinkTolerance = .001f,
			magnitude = 0.f,
			tVecYTimestVecX = 0.f,
			tVecXTimestVecY = 0.f,
			difference = 0.f,
			normedVecX, normedVecXp1, normedVecY, normedVecYp1; 

	const list<float>*	ptDistance = contourIn->getInterPointDistances();
	const list<ImagePoint_F*>*	tangentVec = contourIn->getEdgeList();
	list<Vector2D_F*>*	shrinkVec = shrink->getForce(R_W_ACCESS);
	const list<ImagePoint_F*>*	contour = contourIn->getPoints();

	list<Vector2D_F*>::const_iterator 	iterS = shrinkVec->begin(),
										iterSip1 = shrinkVec->begin();
	list<ImagePoint_F*>::const_iterator iterTanVec = tangentVec->begin(),
										iterTanVecIp1 = tangentVec->begin(),
										iter_0 = contour->begin(),
										iter_1 = contour->begin();
	
	
	iter_0 = contour->begin();
	for(list<float>::const_iterator iterPtDist = ptDistance->begin(), iterPtDistIp1 = ptDistance->begin(); 
	                            iterPtDist != ptDistance->end(); iterPtDist++)
								{
		// increment all teh points accordingly
		iterSip1++;
		iterPtDistIp1++;
		iterTanVecIp1++;

		if(iterSip1 == shrinkVec->end()){
			iterSip1 = shrinkVec->begin();
			iterPtDistIp1 = ptDistance->begin();
			iterTanVecIp1 = tangentVec->begin();
		}

		float   shrinkVecXiP1 = (*iterSip1)->getX();
		float   shrinkVecYiP1 = (*iterSip1)->getY();

		//  normalize edge vectors --> should not have to calculate the values for
		//  i, just get those from ip1 [from last iteration]
//jyh --> unused
//		float   tempVecX = (*iterTanVec)->getX();
//		float   tempVecY = (*iterTanVec)->getY();
//		float   tempDist = *iterPtDist;
//		float   tempVecXp1 = (*iterTanVecIp1)->getX();
//		float   tempVecYp1 = (*iterTanVecIp1)->getY();
//		float   tempDistP1 = *iterPtDistIp1;

		normedVecX = (*iterTanVec)->getX()/(*iterPtDist);
		normedVecY = (*iterTanVec)->getY()/(*iterPtDist);
		normedVecXp1 = (*iterTanVecIp1)->getX()/(*iterPtDistIp1);
		normedVecYp1 = (*iterTanVecIp1)->getY()/(*iterPtDistIp1);

		tVecYTimestVecX = normedVecYp1 * normedVecX;
		tVecXTimestVecY = normedVecXp1 * normedVecY;
		difference = tVecXTimestVecY - tVecYTimestVecX;

		//if contour is a straight line at Vi+1
		if(difference*difference < shrinkTolerance){
			shrinkVecXiP1 = -normedVecYp1;
			shrinkVecYiP1 =  normedVecXp1;
			magnitude = 1.0f;   //  edge vectors have been normalized
		}else if(difference < 0){//if contour is convex at Vi+1
			shrinkVecXiP1 = normedVecXp1 - normedVecX;
			shrinkVecYiP1 = normedVecYp1 - normedVecY;
			//	shrinking force is normalized
			magnitude = sqrtf(shrinkVecXiP1 * shrinkVecXiP1 + shrinkVecYiP1 * shrinkVecYiP1);
		}else{//contour is concave at Vi+1
			shrinkVecXiP1 = normedVecX - normedVecXp1;
			shrinkVecYiP1 = normedVecY - normedVecYp1;
			//  hack
			magnitude = sqrtf(shrinkVecXiP1 * shrinkVecXiP1 +
			shrinkVecYiP1 * shrinkVecYiP1);
		}

		//	shrinking force is normalized
		if(magnitude != 0.f){
			shrinkVecXiP1 /= magnitude;
			shrinkVecYiP1 /= magnitude;
		}

		(*iterSip1)->setX(shrinkVecXiP1 * TIMESTEP_MULT_EDGE_COEFF);
		(*iterSip1)->setY(shrinkVecYiP1 * TIMESTEP_MULT_EDGE_COEFF);
		iterS++;
		iterTanVec++;
		
	}
}


void ActiveContourDetector_balloon::calculateEdgeForce_(Contour_F* contourIn, const VectorField_F* gradNorm2, 
														ContourForce* edgeForce)
{
	const float TIMESTEP_MULT_EDGE_COEFF = edgeCoeff_ * getTimeStep();
	const float NORM_SQRD_TOL = 250.f;

	//gradient of image gradient magnitude at integer and non-integer position
	const float *const* dPx = (const float *const*) gradNorm2->getShiftedRasterX2D();
	const float *const* dPy = (const float *const*) gradNorm2->getShiftedRasterY2D();

	list <ImagePoint_F*>* contourList = contourIn->getPoints(R_W_ACCESS);
	const list <Vector2D_F*>* edgeForceList = edgeForce->getForce();

	list<Vector2D_F*>::const_iterator	iterE = edgeForceList->begin(),
										iterE2 = edgeForceList->begin();
	list<ImagePoint_F*>::iterator	iter2 = contourList->begin(),
									iterTemp = contourList->begin();

	list<ImagePoint_F*>::const_iterator	endContourList = contourList->end();
	
	for(list<ImagePoint_F*>::iterator iter = contourList->begin(); iter != endContourList; iter++, iterE++)
	{
		//rename x and y to IroundedDown, JroundedDown
		int	j = int((*iter)->getX()),
			i = int((*iter)->getY()),
			iPlus1 = i + 1,
			jPlus1 = j + 1;

		float	a = (*iter)->getX() - j,
				b = (*iter)->getY() - i;

		//if points are at the non-filtered positions,edge force is zero
		//bilinear interpolation
		float 	tempAlpha = dPx[i][j],
				tempBeta = dPx[iPlus1][j],
				tempGamma = dPx[i][jPlus1],
				tempDelta = dPx[iPlus1][jPlus1],
				tempBetaMinusAlpha = tempBeta - tempAlpha;

		float fx = tempAlpha + ((tempGamma - tempAlpha) * a) + (tempBetaMinusAlpha * b)
							 + ((tempDelta - tempGamma - tempBetaMinusAlpha) * a * b);

		tempAlpha = dPy[i][j];
		tempBeta = dPy[iPlus1][j];
		tempGamma = dPy[i][jPlus1];
		tempDelta = dPy[iPlus1][jPlus1];
		tempBetaMinusAlpha = tempBeta - tempAlpha;

		float fy = tempAlpha + ((tempGamma - tempAlpha) * a) + (tempBetaMinusAlpha * b)
							 + ((tempDelta - tempGamma - tempBetaMinusAlpha) * a * b);

		//if edge force is greater than smallest float value, get it
		//otherwise, set it to zero

		float normSquared = fx*fx + fy*fy;

		if(normSquared > NORM_SQRD_TOL)
		{
			float norm = sqrtf(normSquared);
			(*iterE)->setX((fx/norm) * TIMESTEP_MULT_EDGE_COEFF);
			(*iterE)->setY((fy/norm) * TIMESTEP_MULT_EDGE_COEFF);			
		}
		else
		{
			(*iterE)->setX(0.f);
			(*iterE)->setY(0.f);
		}
		
		iter2 = iter;
		iter2++;
		bool done = false;
//		bool swapped = false;
		while (!done)
		{
			if (iter2 == endContourList)
			{
				iter2 = contourList->begin();
			//	swapped = true;
			}
			
			if (iter2 == iter)
			{
				done = true;
				break;
			}
			
//			int iter2_x = (*iter2)->getX(),
//				iter2_y = (*iter2)->getY();
			
			float	deltaX = (*iter2)->getX() - (*iter)->getX(),
					deltaY = (*iter2)->getY() - (*iter)->getY();
			float diff = sqrtf(deltaX*deltaX + deltaY*deltaY);
				  
			if (diff < MIN_DIST_BTWN_PTS)
			{
				contourList->erase(iter2++);
			} 
			else 
			{
				//if (diff < MIN_DIST_BTWN_PTS*2)
//				{
//					int swap = 0;
//					for (iterTemp = iter; iterTemp != iter2; iterTemp++)
//					{
//						swap++;
//					}
//					
//					if (swap >= contourList->size()/2)
//					{
//						swapped = true;
//					}
//					
//					if (!swapped)
//					{	
//						
//						iterTemp = iter;
//						iterTemp++;
//						while (iterTemp != iter2)
//						{
//							contourList->erase(iterTemp++);
//						}
////						iterTemp++;
////						contourList->erase(iterTemp,iter2++);
//					} else {
//						iterTemp = iter2;
//						iterTemp++;
//						while(iterTemp != iter)
//						{
//							contourList->erase(iterTemp++);
//						}
//////						iter2++;
////////						iterTemp--;
////////						contourList->erase(iter2++,iterTemp);
//					}
//				} 
				iter2++;
			}
		}
		
	}
	
}

// CAllen: Attempting to rewrite the modified Bresenham Algorithm
void ActiveContourDetector_balloon::searchAndInsertPoints_(Contour_F* contour, const RasterImage_gray_F* normSquaredOfGrad)
{
	//get the array representing the norm squared of the gradient.
	const float *const*		normRaster = normSquaredOfGrad->getShiftedGrayF2D();	 
	
	//coordinate-related integer variables
	int	x[2] = {0,0},
		y[2] = {0,0};

	int	x0 = 0,	y0 = 0,	
		startX = 0, startY = 0,
		endX = 0, endY = 0,
		dx = 0, dy = 0, //difference in x and y
		currIndex = 0;
		
	int lastCx = 0,
		lastCy = 0,
		lastInsert = 0;
		
	//	iter_0 starts at the end of the list of contour points while iter_1
	//	starts at the beginning of the list.  This is so that the during the 
	//	first iteration, the edge E[n]->E[0] is handled first.  During this first
	//	iteration the value of the current index (the index representing iter_1)
	//	is not modified; however, every subsequent loop that points are inserted
	//	onto the contour edge, the value of current index must be incremented 
	//	accordingly.
	const list<ImagePoint_F *>*	contourList = contour->getPoints();		//a list of the ImagePoint_Fs representing the contour
	
	list<ImagePoint_F *>::const_iterator	iter_0 = contourList->end(),	//iterator representing element C_(i-1) of the contour list
											iter_1 = contourList->begin(),	//iterator representing element C_(i) of the contour list 
											endList = contourList->end(),
											debug_0 = contourList->begin(),
											debug_1 = contourList->begin();
	
	iter_0--;
	while (iter_1 != endList)
	{
		bool	swapped0F = false,
				edgeSection = false;
					
		// get this iterations initial and final values for each point
		x[0] = (*iter_0)->getX();
		x[1] = (*iter_1)->getX();
		y[0] = (*iter_0)->getY();
		y[1] = (*iter_1)->getY();
		
		//	if the two endpoints coincide
		if ((x[0] == x[1]) && (y[0] == y[1]))
		{
			//	and iter_1 is not the end of the list
			if ( iter_1 != endList )
			{
				iter_0 = iter_1;
				iter_1++;
				currIndex++;
				x[0] = (*iter_0)->getX();
				y[0] = (*iter_0)->getY();
				x[1] = (*iter_1)->getX();
				y[1] = (*iter_1)->getY();
 
			}
			
			// else do what???
		}
		

		//next we need to keep track of these initial and final values since
		//the x and y arrays will be modified by the transToBresenhamSpace 
		//method
		startX = x[0];
		startY = y[0];
		endX = x[1];
		endY = y[1];

		
		dx = x[1] - x[0];	//dx represents the difference in x values
		dy = y[1] - y[0];	//dy represents the difference in y values
		
		//rather than adapt Bresenham's Algorithm to handle all cases we exploit
		//the symmetry inherent in our coordinate system to transform the given
		//points into a space that will work with the unadapted Bresenham's 
		//algorithm
		int	octant = 0;	//octant represents the Octant that the current point lies in
		transToBresenhamSpace_(dx, dy, x, y, octant, swapped0F);
		
		int	sumX = x[0],
			sumY = y[0],
			ptCount = 1;
				
		//	Bressenham decision variable
		int bres_d = 2*dy - dx;	//initialize bres_d to be 2dy - dx, per the Bresenham Algorithm
		y0 = y[0];		//initialize y0 to be the initial point transformed to Bresenham Space

		lastCx = x[0];
		lastCy = y[0];
		lastInsert = currIndex;

		for (x0 = x[0]; x0 < x[1]; x0 += BRESENHAM_STEP_SIZE )
		{
			int	xn = x0,
				yn = y0;
						
			transFromBresenhamSpace_(xn, yn, octant);
			
			const float gradAtPoint = normRaster[yn][xn];
			 
			if (gradAtPoint > GRAD_THRESHOLD)
			{

				if (edgeSection)
				{
					sumX += xn;
					sumY += yn;
					ptCount++;
				} else {
					edgeSection = true;
					sumX = xn;
					sumY = yn;
					ptCount = 1;
					
				}
			} else {
				if (edgeSection)
				{
					edgeSection = false;
					
					float	Cx = sumX/ptCount,
							Cy = sumY/ptCount;
				
					float	distToEnd = sqrtf((Cx - endX)*(Cx - endX) + (Cy - endY)*(Cy - endY)),
							distToLast = sqrtf((Cx - lastCx)*(Cx - lastCx) + (Cy - lastCy)*(Cy - lastCy)),
							distToStart = sqrtf((Cx - startX)*(Cx - startX) + (Cy - startY)*(Cy - startY));
				
					if (distToEnd > MIN_DIST_BTWN_PTS && distToLast > MIN_DIST_BTWN_PTS && distToStart > MIN_DIST_BTWN_PTS)
					{		
						if (!swapped0F)
						{	
							lastCx = static_cast<int>(roundf(Cx));
							lastCy = static_cast<int>(roundf(Cy));
								
							contour->insertPoint(currIndex,Cx,Cy);
							edgeForce_->insertIntoForceVector(currIndex, 0.f, 0.f);
							internalForce_->insertIntoForceVector(currIndex, 0.f, 0.f);
							shrinkForce_->insertIntoForceVector(currIndex, 0.f, 0.f);
							totalForce_->insertIntoForceVector(currIndex, 0.f, 0.f);
						
							
							if (iter_1 != contourList->begin() || currIndex != 0)
							{
								currIndex++;
								lastInsert = currIndex;
							}
								
						} else {

							lastCx = static_cast<int>(roundf(Cx));
							lastCy = static_cast<int>(roundf(Cy));

							contour->insertPoint(currIndex,Cx,Cy);
							edgeForce_->insertIntoForceVector(lastInsert, 0.f, 0.f);
							internalForce_->insertIntoForceVector(lastInsert, 0.f, 0.f);
							shrinkForce_->insertIntoForceVector(lastInsert, 0.f, 0.f);
							totalForce_->insertIntoForceVector(lastInsert, 0.f, 0.f);
							if (iter_1 != contourList->begin() || currIndex != 0)
							{
								currIndex++;
								if (lastInsert != 0)
								{
									lastInsert--;
								} else {
									lastInsert = 0;
								}
							} 
						}
					} 
				}
			}

			bres_d += dy;
			if ( (bres_d << 1) >= dx )
			{
				y0 += BRESENHAM_STEP_SIZE;
				bres_d -= dx;
			}
		}
	
		iter_0 = iter_1;
		iter_1++;
		currIndex++;
	}// ;	//keep looping until we've visited all the contour points
	
}
 
void ActiveContourDetector_balloon::transToBresenhamSpace_(int &dx, int &dy, int* x, int* y, int &octant, bool &swapped0F)
{
	if (x[1] > x[0] && abs(dx) >= abs(dy) && y[1] >= y[0])
	{
		octant = 1;
		swapped0F = false;
	}
	else if (x[1] >= x[0] && abs(dx) < abs(dy) && y[1] > y[0]) 
	{
		octant = 2;
		swapXY_(x,y);
		swapped0F = false;
	}
	else if (x[1] < x[0] && abs(dx) < abs(dy) && y[1] > y[0])
	{
		octant = 3;
		swapXY_(x,y);
		*y = -(*y);
		swapped0F = false;
	} 
	else if (x[1] < x[0] && abs(dx) >= abs(dy) && y[1] > y[0])
	{
		octant = 4;
		*y = -(*y);
		swap0F_(x,y);
		swapped0F = true;
	}
	else if (x[1] < x[0] && abs(dx) >= abs(dy) && y[1] <= y[0])
	{
		octant = 5;
		swap0F_(x,y);
		swapped0F = true;
	} 
	else if (x[1] < x[0] && abs(dx) < abs(dy) && y[1] < y[0])
	{
		octant = 6;
		swap0F_(x,y);
		swapXY_(x,y);
		swapped0F = true;
	} 
	else if (x[1] >= x[0] && abs(dx) < abs(dy) && y[1] < y[0])
	{
		octant = 7;
		*y = -(*y);
		swapXY_(x,y);
		swapped0F = false;
	} 
	else if (x[1] > x[0] && abs(dx) >= abs(dy) && y[1] < y[0])
	{
		octant = 8;
		*y = -(*y);
		swapped0F = false;
	} 
	else 
	{
		cout << "There is an error in the logic of transforming the point to Bresenham Algorithm's space." << endl;
		cout << "dx: " << dx << "\tdy: " << dy << endl;
		cout << "x0: " << x[0] << "\tx1: " << x[1] << endl;
		cout << "y0: " << y[0] << "\ty1: " << y[1] << endl;
	}
	
	//recalculate the dx and dy
	dx = x[1] - x[0];	//dx represents the difference in x values
 	dy = y[1] - y[0];	//dy represents the difference in y values
}

void ActiveContourDetector_balloon::transFromBresenhamSpace_(int &x, int &y, int octant)
{
	switch(octant)
	{
		case 1:
			break;
		case 2:
			swapXY_(x,y);
			break;
		case 3:
			y = -y;
			swapXY_(x,y);
			break;
		case 4:
			y = -y;
			break;
		case 5:
			break;
		case 6:
			swapXY_(x,y);
			break;
		case 7:
			swapXY_(x,y);
			y = -y;
			break;
		case 8:
			y = -y;
			break;
		default:
			break;
	}
}
													

void inline ActiveContourDetector_balloon::swapXY_(int x[], int y[])
{
	int temp = x[0];
	x[0] = y[0];
	y[0] = temp;
	
	temp = x[1];
	x[1] = y[1];
	y[1] = temp;
}

void inline ActiveContourDetector_balloon::swapXY_(int &x, int &y)
{
	int temp = x;
	x = y;
	y = temp;
}

void inline ActiveContourDetector_balloon::swap0F_(int x[], int y[])
{
	int temp = x[0];
	x[0] = x[1];
	x[1] = temp;
	
	temp = y[0];
	y[0] = y[1];
	y[1] = temp;
}


#if 0
//-------------------------------------------
#pragma mark -
#pragma mark Rendering functions
//-------------------------------------------
#endif

void ActiveContourDetector_balloon::renderForces(void){

	glPushMatrix();
		shrinkForce_->render();
		internalForce_->render();
		edgeForce_->render();

		totalForce_->render();
	glPopMatrix();
}


