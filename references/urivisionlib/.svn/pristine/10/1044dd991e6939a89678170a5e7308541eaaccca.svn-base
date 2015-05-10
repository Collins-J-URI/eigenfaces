/*  NAME:
        ActiveContourDetector_GVF.cpp

    DESCRIPTION:
        implementation of the uriVisionLib ActiveContourDetector_GVF class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <vector>
//
#include "ActiveContourDetector_GVF.h"
#include "ImagePoint_F.h"

using namespace std;
using namespace uriVL;

//---------------------------------------------
//	DISABLED for the time being
//---------------------------------------------
#if 0

//----------------------------------------------------------
//  static members & constants
//----------------------------------------------------------
const float GVF_FORCE_DEFAULT_COLOR[] = {0.f, 0.f, 1.f};

#if 0
//----------------------------------------
#pragma mark -
#pragma mark Constructors and destructor
//----------------------------------------
#endif


ActiveContourDetector_GVF::ActiveContourDetector_GVF(int nbContourPts, const ImageRect* imgInRect, float tol, float timeStep)
		try	:	ActiveContourDetector(imgInRect),
				//
				gvfForce_(new ContourForce()),
				contourProc_(NULL),
				gvfVector_(new VectorField_F(imgInRect)),
				c_(new VectorField_F(imgInRect)),
				gvfVectorPlus1_(new VectorField_F(imgInRect))
{
	gvfForce_->setNeedleColor(GVF_FORCE_DEFAULT_COLOR);
}
catch (ErrorReport& e)
{
	e.appendToMessage("called by ActiveContourDetector_GVF constructor");
	throw e;
}
catch (...)
{
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"In ActiveContourDetector_GVF constructor");
}


ActiveContourDetector_GVF::ActiveContourDetector_GVF(const ActiveContourDetector_GVF& theObj)
        :	ActiveContourDetector(theObj),
			//
			gvfForce_(NULL),
			contourProc_(NULL),
			gvfVector_(NULL),
			c_(NULL),
			gvfVectorPlus1_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ActiveContourDetector copy constructor not implemented.");
}


ActiveContourDetector_GVF::~ActiveContourDetector_GVF(void)
{
	delete	gvfForce_;
	delete	contourProc_;
	delete	c_;
	delete	gvfVector_;
	delete	gvfVectorPlus1_;

}





const ActiveContourDetector_GVF& ActiveContourDetector_GVF::operator = (const ActiveContourDetector_GVF& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ActiveContourDetector_GVF copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//-------------------------------------------
#pragma mark Public find countour functions
//-------------------------------------------
#endif


Contour_F ActiveContourDetector_GVF::findContour(const RasterImage* imgIn,const ImageRect* boundRect)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ActiveContourDetector_GVF::findContour function not implemented.");

	return NULL;
}


IterationStatus ActiveContourDetector_GVF::findContour(const RasterImage* imgIn, Contour_F* contour, const ImageRect* boundRect)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage passed as input to ActiveContourDetector_GVF");
    FAIL_CONDITION( contour == NULL,
                    kNullParameterError,
                    "null Contour passed as input to ActiveContourDetector_GVF");

	const ImageRect* procRect = getRect();
    imageGradient_ 	= new VectorField_F(procRect);
    normSqrdOfGrad_ = new RasterImage_gray_F(procRect);
    gradOfNormSqrdOfGrad_ = new VectorField_F(procRect);
    if(imgIn->getBaseRasterType() == kRGBa32Raster){
    	colorImageGradientProcessor_ = new ColorImageGradient_Gaussian_F(DEFAULT_GAUSSIAN_FILTER_SCALE);
	    colorImageGradientProcessor_->setColorGradientMode(GRAD_LARGEST_COLOR_COMPONENT);
	    colorImageGradientProcessor_->applyInto(imgIn, imageGradient_);
    }else{
    	grayImageGradientProcessor_ = new ImageGradient_Gaussian_F(DEFAULT_GAUSSIAN_FILTER_SCALE);
	    grayImageGradientProcessor_->applyInto(imgIn, imageGradient_);
    }

    imageGradient_->getNormSquaredAsRasterImage(normSqrdOfGrad_);
    grayImageGradientProcessor_->applyInto(normSqrdOfGrad_, gradOfNormSqrdOfGrad_);
    //
    gradientsAreLocal_ = true;

    //	All the computation will be done inside contourProc_, which is an alias of contourOut
    //	and whose points are initialized as that of contourIn.
    contourProc_ = contourOut;
    contourIn->copyInto(contourProc_);

    return findContour_();
}

IterationStatus ActiveContourDetector_GVF::findContour(const RasterImage* imgIn, VectorField_F* grad, RasterImage_gray_F* normSqrd,
													  VectorField_F* gradNormSqrd, Contour_F* contour)
{

    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage passed as input to ActiveContourDetector_GVF");
    FAIL_CONDITION( grad == NULL,
                    kNullVectorFieldError,
                    "null gradient passed as input to ActiveContourDetector_GVF");
    FAIL_CONDITION( normSqrd == NULL,
                    kNullRasterImageError,
                    "null  gradient norm image passed as input to ActiveContourDetector_GVF");
    FAIL_CONDITION( gradNormSqrd == NULL,
                    kNullVectorFieldError,
                    "null grad-of-norm2 passed as input to ActiveContourDetector_GVF");
    FAIL_CONDITION( contourIn == NULL,
                    kNullParameterError,
                    "null Contour passed as input to ActiveContourDetector_GVF");

    //	We don't need to allocate new gradient objects.  We just keep references to the ones
    //	computed by the caller.
    imageGradient_ = grad;
    normSqrdOfGrad_ = normSqrd;
    gradOfNormSqrdOfGrad_ = gradNormSqrd;
    gradientsAreLocal_ = false;

    //	All the computation will be done inside contourProc_, which is an alias of contourOut
    //	and whose points are initialized as that of contourIn.
    contourProc_ = contourOut;
    contourIn->copyInto(contourProc_);

    return findContour_();

}

#if 0
#pragma mark -
//-------------------------------------------
#pragma mark Private calculation functions
//-------------------------------------------
#endif

const IterationStatus& ActiveContourDetector_GVF::findContour_(void)
{
    gvfForce_->mapToContour(contourProc_);
    internalForce_->mapToContour(contourProc_);
    computeGradientVectorFlow_();

    //	at this point the setup is complete.  We could proceed either step by step if the user
    //	asked us to do so.
    setupCompleted_ = true;

//jyh --> unused
//    bool	converged = false;
//    IterationStatus	*iterationStatus=NULL;
    /*
    	if (stepByStep_)
    	{
    		iterationStatus = new IterationStatus;
    		oneStep_(iterationStatus);
    	}
    	else
    		iterationStatus = iterate(MAX_GVF_ITERATIONS);
    */
    return getStatus();
}

void ActiveContourDetector_GVF::oneStep_(IterationStatus* stepStatus)
{
    calculateInternalForce_();
    //calculateGvfForce_(contourProc_, gradOfNormSqrdOfGrad_, edgeForce_);
    gvfForce_->applyForceToContour();

    nbIterations_++;
}

void ActiveContourDetector_GVF::computeGradientVectorFlow_(void)
{

    //floats and remove underscore
    float	**pGradXYSqrd = (float**)normSqrdOfGrad_->getShiftedGrayF2D(),
            **pGradientX = (float**)imageGradient_->getShiftedRasterX2D(),
            **pGradientY = (float**)imageGradient_->getShiftedRasterY2D(),
            **gvfVectorU, **gvfVectorV,	**gvfUPlus1, **gvfVPlus1,
            **c1 = (float**)c_->getShiftedRasterX2D(),
            **c2 = (float**)c_->getShiftedRasterY2D(),
            normUV	= 0.0f,
            tempAlpha, tempBeta;

    //jyh
    //	The norm of the gradient should be the drawable object with the smallest
    //	valid data rectangle.  Check on that though, if you use the gradient of
    //	that image later....
    const ImageRect	*workRect = normSqrdOfGrad_->getValidRect();
    int     iLow = workRect->getTop(),
            iHigh = workRect->getBottom(),
            jLow = workRect->getLeft(),
            jHigh = workRect->getRight();
    int     gvfIterationMax   = 2;

    VectorField_F	*tempVec = new VectorField_F(workRect);
//jyh --> unused
//    float** tempVecU = (float** ) tempVec->getShiftedRasterX2D();
//    float** tempVecV = (float** ) tempVec->getShiftedRasterY2D();


    for(int i = iLow; i < iHigh; i++)
    {
        for(int j = jLow; j < jHigh; j++)
        {
            c1[i][j] = pGradXYSqrd[i][j] * pGradientX[i][j];
            c2[i][j] = pGradXYSqrd[i][j] * pGradientY[i][j];
        }
    }

    tempBeta = (MU * timeStep_);
//    theTimeStep_ = theTimeStep_;

    iLow++;
    iHigh--;
    jLow++;
    jHigh--;
    for(int iterationNum = 0; iterationNum < gvfIterationMax; iterationNum++)
    {
        gvfVectorU	 = (float** )gvfVector_->getShiftedRasterX2D();
        gvfVectorV	 = (float** )gvfVector_->getShiftedRasterY2D();
        gvfUPlus1 = (float** )gvfVectorPlus1_->getShiftedRasterX2D();
        gvfVPlus1 = (float** )gvfVectorPlus1_->getShiftedRasterY2D();

        for(int i = iLow; i <= iHigh; i++)
        {
            for(int j = jLow; j <= jHigh; j++)
            {

                tempAlpha = (1 - pGradXYSqrd[i][j] * timeStep_);

                gvfUPlus1[i][j] = (tempAlpha * gvfVectorU[i][j])
                                  + (tempBeta
                                     * (gvfVectorU[i+1][j] + gvfVectorU[i][j+1]
                                        + gvfVectorU[i-1][j] + gvfVectorU[i][j-1]
                                        - (4 * gvfVectorU[i][j])))
                                  + c1[i][j] * timeStep_;
                gvfVPlus1[i][j] = (tempAlpha * gvfVectorV[i][j])
                                  + (tempBeta
                                     * (gvfVectorV[i+1][j] + gvfVectorV[i][j+1]
                                        + gvfVectorV[i-1][j] + gvfVectorV[i][j-1]
                                        - (4 * gvfVectorV[i][j])))
                                  + c2[i][j] * timeStep_;
                normUV = 0.f;
                normUV = sqrtf((gvfUPlus1[i][j] * gvfUPlus1[i][j])
                               + (gvfVPlus1[i][j] * gvfVPlus1[i][j]));

                if(normUV < 0.00001f)
                {
                    gvfUPlus1[i][j] = 0.f;
                    gvfVPlus1[i][j] = 0.f;
                }
                else
                {
                    gvfUPlus1[i][j] = gvfUPlus1[i][j] / normUV;
                    gvfVPlus1[i][j] = gvfVPlus1[i][j] / normUV;
                }
            }
        }

        float	**swapPointer;

        swapPointer = gvfVectorU;
        gvfVectorU = gvfUPlus1;
        gvfUPlus1 = swapPointer;

        swapPointer = gvfVectorV;
        gvfVectorV = gvfVPlus1;
        gvfVPlus1 = swapPointer;
    }

    delete tempVec;

    ImageRect	*outRect = new ImageRect(workRect);
    outRect->removeAllAround(1);
    gvfVector_->setValidRect(outRect);
    gvfVector_->setRasterAsReference();
    gvfVector_->forceRasterUpdate();
    delete outRect;
}

void ActiveContourDetector_GVF::calculateGvfForce_(VectorField_F *theGrad, VectorField_F *theGvf)
{
    calculateGvfForce_(theGrad, theGrad->getValidRect(), theGvf);
}

void ActiveContourDetector_GVF::calculateGvfForce_(VectorField_F *grad, const ImageRect* theRect, VectorField_F *gvf)
{
/*
	//	determine processing rectangle, possibly resize gvf if its dimnsions
	//	don't match (see gradient class
	ImageRect	*outRect;	//	intersection of 2 rectangles
	//	fail if outRect is null
	
	//	get the gradient's rasters
	float	**gradX = (float** )grad->getShiftedRasterX2D();
	float	**gradY = (float** )grad->getShiftedRasterY2D();
	float	**gvfX = (float** )gvf->getShiftedRasterX2D();
	float	**gvfY = (float** )gvf->getShiftedRasterY2D();
	
    int   iLow = theRect->getTop(),
            iHigh = outRect->getBottom(),
            jLow = theRect->getLeft(),
            jHigh = theRect->getRight();
 
    
    //  compute convolution over the processing rectangle
    int   im1 = iLow-1,
    		ip1 = iLow+1;
    for (int i=iLow; i<=iHigh; i++)
    {
        int	jm1 = jLow-1, 
        		jp1 = jLow+1;
 
        for (int j=jLow; j<=jHigh; j++)
        {
 
			//calculate alpha's u
			gvfX[i][j] = tempAlpha + ((tempGamma - tempAlpha) * a) + (tempBetaMinusAlpha * b)
						+ ((tempDelta - tempGamma - tempBetaMinusAlpha) * a * b);
			
			
			//recalculate alpha's for v
			
			gvfY[i][j] = tempAlpha + ((tempGamma - tempAlpha) * a) + (tempBetaMinusAlpha * b)
						+ ((tempDelta - tempGamma - tempBetaMinusAlpha) * a * b);
			
            
            //  shift the j indexes (j itself is taken care of in the loop
            //  statement)
            jm1 = j;
            jp1++;
        }
        
		//  shift the i indexes (i itself is taken care of in the loop
		//  statement)
		im1 = i;
		ip1++;
	}
    }
*/	
}

#if 0
//-------------------------------------------
#pragma mark -
#pragma mark Rendering functions
//-------------------------------------------
#endif

void ActiveContourDetector_GVF::renderForces(void)
{
    gvfVector_->render();
    //gvfForce_->setColor(0.f, 0.f, 0.f);
    //gvfForce_->render();
}

#endif
