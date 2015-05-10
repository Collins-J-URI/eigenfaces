/*  NAME:
        ColorImageGradient_Gaussian_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorImageGradient_Gaussian_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//
#include "RasterImage_gray_F.h"
#include "ColorImageGradient_Gaussian_F.h"
#include "VectorField_F.h"

using namespace uriVL;

#define	kMaxHardCodedWidth	13


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ColorImageGradient_Gaussian_F::ColorImageGradient_Gaussian_F(double theScale)
		try	:	ColorImageGradient(2*(((int ) (5*theScale))/2) + 1, false),
				//
				scale_(theScale),
				fg_(NULL),
				fgD_(NULL)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a G Gradient filter is 0.8");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorImageGradient_Gaussian_F constructor");
	throw e;
}



ColorImageGradient_Gaussian_F::ColorImageGradient_Gaussian_F(double theScale, const ImageRect* theRect)
		try	:	ColorImageGradient(2*(((int ) (5*theScale))/2) + 1, false),
				//
				scale_(theScale),
				fg_(NULL),
				fgD_(NULL)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a G Gradient filter is 0.8");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorImageGradient_Gaussian_F constructor");
	throw e;
}



ColorImageGradient_Gaussian_F::ColorImageGradient_Gaussian_F(int theWidth)
		try	:	ColorImageGradient(theWidth, false),
				//
				scale_(0.2L*getWidth()),
				fg_(NULL),
				fgD_(NULL)				
{
    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest width admissible for a G gradient filter is 3");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorImageGradient_Gaussian_F constructor");
	throw e;
}

			
ColorImageGradient_Gaussian_F::ColorImageGradient_Gaussian_F(int theWidth, const ImageRect* theRect)
		try	:	ColorImageGradient(theWidth, theRect, false),
				//
				scale_(0.2L*getWidth()),
				fg_(NULL),
				fgD_(NULL)				
{
    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest width admissible for a G filter is 3");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorImageGradient_Gaussian_F constructor");
	throw e;
}

			
ColorImageGradient_Gaussian_F::ColorImageGradient_Gaussian_F(const ColorImageGradient_Gaussian_F& theObj)
		try	:	ColorImageGradient(theObj),
				//
				scale_(theObj.scale_),
				fg_(NULL),
				fgD_(NULL)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorImageGradient_Gaussian_F copy constructor not implemented.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorImageGradient_Gaussian_F constructor");
	throw e;
}




ColorImageGradient_Gaussian_F::~ColorImageGradient_Gaussian_F(void)
{
    delete []fg_;
    delete []fgD_;
}

const ColorImageGradient_Gaussian_F& ColorImageGradient_Gaussian_F::operator = (const ColorImageGradient_Gaussian_F& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ColorImageGradient_Gaussian_F copy operator not implemented.");

	return *this;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters
//------------------------------------------------------
#endif


float ColorImageGradient_Gaussian_F::getScale(void) const
{
    return static_cast<float>(scale_);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


VectorField *ColorImageGradient_Gaussian_F::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION( imgIn == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as input to ColorImageGradient_Gaussian_F::applyInto");
	
	return applyTo(imgIn, imgIn->getValidRect());
}

VectorField *ColorImageGradient_Gaussian_F::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION( imgIn == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as input to ColorImageGradient_Gaussian_F::applyInto");
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in ColorImageGradient_Gaussian_F::applyInto");

	ImageRect*	usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle");

	VectorField	*vectOut = new VectorField_F(destRect);
	applyInto(imgIn, theRect, vectOut);

	delete destRect;
	delete usableRect;

	return vectOut;
}

void ColorImageGradient_Gaussian_F::applyInto(const RasterImage* imgIn, VectorField* vectOut)
{
    FAIL_CONDITION( imgIn == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as input to ColorImageGradient_Gaussian_F::applyInto");
    FAIL_CONDITION( vectOut == NULL, 
                    kNullVectorFieldError, 
                    "null VectorField passed as destination to ColorImageGradient_Gaussian_F::applyInto");

    applyInto(imgIn, imgIn->getValidRect(), vectOut);
}

void ColorImageGradient_Gaussian_F::applyInto(const RasterImage* imgIn, const ImageRect* theRect, 
                                              VectorField* vectOut)
{
	//----------------------------------------------------------------
	//	STEP 0:		Test for null or invalid type parameters
	//----------------------------------------------------------------
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in GaussianFilter_F::applyInto");

    FAIL_CONDITION( vectOut == NULL, 
                    kNullVectorFieldError, 
                    "null VectorField passed as destination to ColorImageGradient_Gaussian_F::applyInto");

    //  so far I only accept a floating point vector field as output
	FAIL_CONDITION( !vectOut->hasFloatRaster(),
	                kInvalidRasterType,
	                "ColorImageGradient_Gaussian_F::applyInto only accepts float vector fields as destination");
    
	//----------------------------------------------------------------
	//	STEP 1:		Test for rectangle intersection
	//----------------------------------------------------------------
	//  determine the output rectangle that the input image can produce.
	//		Here, because we apply a filter of width&height (2m+1), we
	//		"lose m all around".
	ImageRect*	usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	//	... and determine its intersection with the user-specified output rectangle
	ImageRect*	destRect = ImageRect::intersection(usableRect, theRect);
	delete usableRect;
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle");
    //  if the output field does not contain the destination rect, it must be resized.
    if (!vectOut->contains(destRect))
    	vectOut->setBoundRect(destRect);
    

	//----------------------------------------------------------------
	//	STEP 2:		Operate only on an rgba color image
	//----------------------------------------------------------------
	bool localRGBa = false;
    const RasterImage_RGBa* rgbaImg = RasterImage_RGBa::localRasterRGBa(imgIn, &localRGBa);

	//----------------------------------------------------------------
	//	STEP 3:		Perform the calculations
	//----------------------------------------------------------------
    //	allocate a 2D raster for intermediate calculations if needed
    bool newTempRaster = allocateTempRasters_(imgIn->getValidRect());

    float*const* fvx = static_cast<float*const*>(static_cast<VectorField_F*>(vectOut)->getShiftedRasterX2D(R_W_ACCESS));
    float*const* fvy = static_cast<float*const*>(static_cast<VectorField_F*>(vectOut)->getShiftedRasterY2D(R_W_ACCESS));

    switch (getWidth())
    {
        case 3:
            if (!rgbaImg->hasFloatRaster())
            {
                applyIntoI_3x3_X_(rgbaImg, destRect, fvx);
                applyIntoI_3x3_Y_(rgbaImg, destRect, fvx, fvy);
            }
            else
            {
                applyIntoF_3x3_X_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx);
                applyIntoF_3x3_Y_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx, fvy);
            }
            break;

        case 5:
            if (!rgbaImg->hasFloatRaster())
            {
                applyIntoI_5x5_X_(rgbaImg, destRect, fvx);
                applyIntoI_5x5_Y_(rgbaImg, destRect, fvx, fvy);
            }
            else
            {
                applyIntoF_5x5_X_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx);
                applyIntoF_5x5_Y_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx, fvy);
            }
            break;


        case 7:
            if (!rgbaImg->hasFloatRaster())
            {
                applyIntoI_7x7_X_(rgbaImg, destRect, fvx);
                applyIntoI_7x7_Y_(rgbaImg, destRect, fvx, fvy);
            }
            else
            {
                applyIntoF_7x7_X_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx);
                applyIntoF_7x7_Y_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx, fvy);
            }
            break;

        case 9:
            if (!rgbaImg->hasFloatRaster())
            {
                applyIntoI_9x9_X_(rgbaImg, destRect, fvx);
                applyIntoI_9x9_Y_(rgbaImg, destRect, fvx, fvy);
            }
            else
            {
                applyIntoF_9x9_X_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx);
                applyIntoF_9x9_Y_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx, fvy);
            }
            break;

        case 11:
            if (!rgbaImg->hasFloatRaster())
            {
                applyIntoI_11x11_X_(rgbaImg, destRect, fvx);
                applyIntoI_11x11_Y_(rgbaImg, destRect, fvx, fvy);
            }
            else
            {
                applyIntoF_11x11_X_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx);
                applyIntoF_11x11_Y_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx, fvy);
            }
            break;

        case 13:
            if (!rgbaImg->hasFloatRaster())
            {
                applyIntoI_13x13_X_(rgbaImg, destRect, fvx);
                applyIntoI_13x13_Y_(rgbaImg, destRect, fvx, fvy);
            }
            else
            {
                applyIntoF_13x13_X_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx);
                applyIntoF_13x13_Y_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx, fvy);
            }
            break;

        default:
            if (!rgbaImg->hasFloatRaster())
            {
                applyIntoI_X_(rgbaImg, destRect, fvx);
                applyIntoI_Y_(rgbaImg, destRect, fvx, fvy);
            }
            else
            {
                applyIntoF_X_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx);
                applyIntoF_Y_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, fvx, fvy);
            }
            break;
    }
    
	//----------------------------------------------------------------
	//	STEP 4:		Set rectangle
	//----------------------------------------------------------------
	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
    vectOut->setValidRect(destRect);
	vectOut->setRasterAsReference();
    vectOut->forceRasterUpdate();

	//----------------------------------------------------------------
	//	STEP 5:		Free temporary storage (if needed)
	//----------------------------------------------------------------
	if (localRGBa)
	    delete rgbaImg;
	delete destRect;
    if (newTempRaster)
		freeTempRasters_();

}


#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Hard-coded private methods that implement the 3x3 convolution
//-------------------------------------------------------------------------------


void ColorImageGradient_Gaussian_F::applyIntoI_3x3_X_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                      float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int	iLowM1 = iLow - 1, iHighP1 = iHigh + 1;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM1; i<=iHighP1; i++)
        {
			redTemp[i][j] = 	fgD_[1]*(rgba[i][fourjm1R] - rgba[i][fourjp1R]);
			greenTemp[i][j] = 	fgD_[1]*(rgba[i][fourjm1G] - rgba[i][fourjp1G]);
			blueTemp[i][j] = 	fgD_[1]*(rgba[i][fourjm1B] - rgba[i][fourjp1B]);
        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R += 4;
        fourjp1G += 4;
        fourjp1B += 4;
	}

    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_3x3_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoI_3x3_Y_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM1 = iLow - 1, iHighP1 = iHigh+1;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM1; i<=iHighP1; i++)
        {
			redTemp[i][j] =   fg_[1]*(rgba[i][fourjm1R] + rgba[i][fourjp1R]) +
							  fg_[0]* rgba[i][fourjR];
			
			greenTemp[i][j] = fg_[1]*(rgba[i][fourjm1G] + rgba[i][fourjp1G]) +
							  fg_[0]* rgba[i][fourjG];
							
			blueTemp[i][j] =  fg_[1]*(rgba[i][fourjm1B] + rgba[i][fourjp1B]) +
							  fg_[0]* rgba[i][fourjB];
							
        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R += 4;
        fourjp1G += 4;
        fourjp1B += 4;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_3x3_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyIntoF_3x3_X_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                     float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM1 = iLow - 1, iHighP1 = iHigh+1;
    //
    int         jm1, jp1;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    jm1 = jLow-1;
    jp1 = jLow+1;
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM1; i<=iHighP1; i++)
        {
			redTemp[i][j] = 	fgD_[1]*(fRed[i][jm1] - fRed[i][jp1]);
			greenTemp[i][j] = 	fgD_[1]*(fGreen[i][jm1] - fGreen[i][jp1]);
			blueTemp[i][j] = 	fgD_[1]*(fBlue[i][jm1] - fBlue[i][jp1]);
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm1 = j;
        jp1++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_3x3_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoF_3x3_Y_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                      float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM1 = iLow - 1, iHighP1 = iHigh+1;
    //
    int 	jm1, jp1;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    jm1 = jLow-1;
    jp1 = jLow+1;
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM1; i<=iHighP1; i++)
        {
			redTemp[i][j] = 	fg_[1]*(fRed[i][jm1] + fRed[i][jp1]) +
							        fg_[0]* fRed[i][j];
			greenTemp[i][j] = 	fg_[1]*(fGreen[i][jm1] + fGreen[i][jp1]) +
							        fg_[0]* fGreen[i][j];
			blueTemp[i][j] = 	fg_[1]*(fBlue[i][jm1] + fBlue[i][jp1]) +
							        fg_[0]* fBlue[i][j];
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm1 = j;
        jp1++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_3x3_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}


void ColorImageGradient_Gaussian_F::applyInto_3x3_X_(int iLow, int iHigh, int jLow, 
                                                     int jHigh, float*const* redTemp, 
                                                     float*const* greenTemp, float*const* blueTemp, 
                                                     float*const* fvx)
{
    int im1 = iLow-1, ip1 = iLow+1; 
    
    //-------------------------------------------------------
    //	Apply the 1D filter along the y direction
    //-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {
			const   float   fgScaled[2] = {fg_[0]/3.f, fg_[1]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvx[i][j] =   fgScaled[1]*(redTemp[im1][j] + redTemp[ip1][j] +
                			                   greenTemp[im1][j] + greenTemp[ip1][j] +
                			                   blueTemp[im1][j] + blueTemp[ip1][j]) +
        						  fgScaled[0]* (redTemp[i][j] + greenTemp[i][j] + blueTemp[i][j]);

        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
        		im1 = i;
        		ip1++;
        	}
            break;
		}	
            
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);

        	//-------------------------------------------------------
        	//	then apply the 1D filter along the y direction
        	//-------------------------------------------------------
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			altRedTemp[i][j] =    fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
        						          fg_[0]* redTemp[i][j];

        			altGreenTemp[i][j] =  fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
        						          fg_[0]* greenTemp[i][j];

        			altBlueTemp[i][j]  =  fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
        						          fg_[0]* blueTemp[i][j];

        		}

        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
        		im1 = i;
        		ip1++;
        	}
            break;
        }
                        
        //  this should never happen
        default:
            break;
            
    }
}


void ColorImageGradient_Gaussian_F::applyInto_3x3_Y_(int iLow, int iHigh, int jLow,
                                                     int jHigh, float*const* redTemp,
                                                     float*const* greenTemp, float*const* blueTemp,
                                                     float*const* fvx, float*const* fvy)
{
    int im1 = iLow-1, ip1 = iLow+1; 
    float   fvxTemp, fvyTemp;        

	//-------------------------------------------------------
	//	Apply the 1D filter along the y direction
	//-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
            const   float   fgDScaled = fgD_[1]/3.f;

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvy[i][j] = fgDScaled*(redTemp[im1][j] - redTemp[ip1][j] +
								           greenTemp[im1][j] - greenTemp[ip1][j] +
								           blueTemp[im1][j] - blueTemp[ip1][j]);

        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
        		im1 = i;
        		ip1++;
        	}
        	break;
		}	

        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            for (int i=iLow; i<=iHigh; ip1++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = altRedTemp[i][j];
                    fvyTemp = fgD_[1]*(redTemp[im1][j] - redTemp[ip1][j]);
                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;

                    //  green component
                    fvxComp = altGreenTemp[i][j];
        			fvyComp = fgD_[1]*(greenTemp[im1][j] - greenTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = altBlueTemp[i][j];
        			fvyComp = fgD_[1]*(blueTemp[im1][j] - blueTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvx[i][j] = fvxComp;
                        fvy[i][j] = fvyComp;
                    }
                    else
                    {
                        fvx[i][j] = fvxTemp;
                        fvy[i][j] = fvyTemp;
                    }
                }

                //  shift the i indices (ip1 is taken care of in the loop statement)
                im1 = i;
                i = ip1;
            }
            break;
        }
        
        //  this should never happen
        default:
            break;
            
    }
}

#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Hard-coded private methods that implement the 5x5 convolution
//-------------------------------------------------------------------------------


void ColorImageGradient_Gaussian_F::applyIntoI_5x5_X_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                      float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int   iLowM2 = iLow - 2, iHighP2 = iHigh + 2;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM2; i<=iHighP2; i++)
        {
			redTemp[i][j] = 	fgD_[2]*(rgba[i][fourjm2R] - rgba[i][fourjp2R]) +
								fgD_[1]*(rgba[i][fourjm1R] - rgba[i][fourjp1R]);
			greenTemp[i][j] = 	fgD_[2]*(rgba[i][fourjm2G] - rgba[i][fourjp2G]) +
								fgD_[1]*(rgba[i][fourjm1G] - rgba[i][fourjp1G]);
			blueTemp[i][j] = 	fgD_[2]*(rgba[i][fourjm2B] - rgba[i][fourjp2B]) +
								fgD_[1]*(rgba[i][fourjm1B] - rgba[i][fourjp1B]);

        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R += 4;
        fourjp2G += 4;
        fourjp2B += 4;
	}

    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_3x3_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoI_5x5_Y_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                      float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM2 = iLow - 2, iHighP2 = iHigh + 2;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM2; i<=iHighP2; i++)
        {
			redTemp[i][j] =   fg_[2]*(rgba[i][fourjm2R] + rgba[i][fourjp2R]) +
								fg_[1]*(rgba[i][fourjm1R] + rgba[i][fourjp1R]) +
								fg_[0]* rgba[i][fourjR];
			
			greenTemp[i][j] = fg_[2]*(rgba[i][fourjm2G] + rgba[i][fourjp2G]) +
								fg_[1]*(rgba[i][fourjm1G] + rgba[i][fourjp1G]) +
								fg_[0]* rgba[i][fourjG];
							
			blueTemp[i][j] =  fg_[2]*(rgba[i][fourjm2B] + rgba[i][fourjp2B]) +
								fg_[1]*(rgba[i][fourjm1B] + rgba[i][fourjp1B]) +
								fg_[0]* rgba[i][fourjB];
							
        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R += 4;
        fourjp2G += 4;
        fourjp2B += 4;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_5x5_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyIntoF_5x5_X_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                      float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM2 = iLow - 2, iHighP2 = iHigh + 2;
    int 	jm1, jm2, jp1, jp2;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    jm1 = jLow-1; jm2 = jLow-2;
    jp1 = jLow+1; jp2 = jLow+2;
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM2; i<=iHighP2; i++)
        {
			redTemp[i][j] = 	fgD_[2]*(fRed[i][jm2] - fRed[i][jp2]) +
			                    fgD_[1]*(fRed[i][jm1] - fRed[i][jp1]);
			greenTemp[i][j] = 	fgD_[2]*(fGreen[i][jm2] - fGreen[i][jp2]) +
			                    fgD_[1]*(fGreen[i][jm1] - fGreen[i][jp1]);
			blueTemp[i][j] = 	fgD_[2]*(fBlue[i][jm2] - fBlue[i][jp2]) +
			                    fgD_[1]*(fBlue[i][jm1] - fBlue[i][jp1]);
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_5x5_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoF_5x5_Y_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                      float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM2 = iLow - 2, iHighP2 = iHigh + 2;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int jm1 = jLow-1, jm2 = jLow-2; 
    int jp1 = jLow+1, jp2 = jLow+2;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM2; i<=iHighP2; i++)
        {
			redTemp[i][j] = 	fg_[2]*(fRed[i][jm2] + fRed[i][jp2]) +
								fg_[1]*(fRed[i][jm1] + fRed[i][jp1]) +
								fg_[0]* fRed[i][j];
			greenTemp[i][j] = 	fg_[2]*(fGreen[i][jm2] + fGreen[i][jp2]) +
								fg_[1]*(fGreen[i][jm1] + fGreen[i][jp1]) +
								fg_[0]* fGreen[i][j];
			blueTemp[i][j] = 	fg_[2]*(fBlue[i][jm2] + fBlue[i][jp2]) +
								fg_[1]*(fBlue[i][jm1] + fBlue[i][jp1]) +
								fg_[0]* fBlue[i][j];
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_5x5_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}


void ColorImageGradient_Gaussian_F::applyInto_5x5_X_(int iLow, int iHigh, int jLow, 
                                                     int jHigh, float*const* redTemp,
                                                     float*const* greenTemp, float*const* blueTemp, 
                                                     float*const* fvx)
{
    int im1 = iLow-1, im2 = iLow-2; 
	int ip1 = iLow+1, ip2 = iLow+2; 
    
    //-------------------------------------------------------
    //	Apply the 1D filter along the y direction
    //-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
			const   float   fgScaled[3] = {fg_[0]/3.f, fg_[1]/3.f, fg_[2]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvx[i][j] =   fgScaled[2]*(redTemp[im2][j] + redTemp[ip2][j] +
                			                   greenTemp[im2][j] + greenTemp[ip2][j] +
                			                   blueTemp[im2][j] + blueTemp[ip2][j]) +
        			              fgScaled[1]*(redTemp[im1][j] + redTemp[ip1][j] +
                			                   greenTemp[im1][j] + greenTemp[ip1][j] +
                			                   blueTemp[im1][j] + blueTemp[ip1][j]) +
        						  fgScaled[0]* (redTemp[i][j] + greenTemp[i][j] + blueTemp[i][j]);

        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2++;
        	}
            break;
		}	
            
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);

        	//-------------------------------------------------------
        	//	then apply the 1D filter along the y direction
        	//-------------------------------------------------------
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			altRedTemp[i][j] =    fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
            							  fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
            							  fg_[0]* redTemp[i][j];

        			altGreenTemp[i][j] =  fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
            							  fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
            							  fg_[0]* greenTemp[i][j];

        			altBlueTemp[i][j]  =  fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
								          fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
								          fg_[0]* blueTemp[i][j];

        		}

        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2++;
        	}
            break;
        }
                        
        //  this should never happen
        default:
            break;
            
    }
}


void ColorImageGradient_Gaussian_F::applyInto_5x5_Y_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx, float*const* fvy)
{
    int im1 = iLow-1, im2 = iLow-2; 
	int ip1 = iLow+1, ip2 = iLow+2; 
    float   fvxTemp, fvyTemp;        

	//-------------------------------------------------------
	//	Apply the 1D filter along the y direction
	//-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
            const   float   fgDScaled[3] = {0, fgD_[1]/3.f, fgD_[2]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvy[i][j] = fgDScaled[2]*(redTemp[im2][j] - redTemp[ip2][j] +
								              greenTemp[im2][j] - greenTemp[ip2][j] +
								              blueTemp[im2][j] - blueTemp[ip2][j])+
							    fgDScaled[1]*(redTemp[im1][j] - redTemp[ip1][j] +
								              greenTemp[im1][j] - greenTemp[ip1][j] +
								              blueTemp[im1][j] - blueTemp[ip1][j]);
        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2++;
        	}
        	break;
		}	

        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = altRedTemp[i][j];
                    fvyTemp = fgD_[2]*(redTemp[im2][j] - redTemp[ip2][j]) +
								fgD_[1]*(redTemp[im1][j] - redTemp[ip1][j]);
                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;

                    //  green component
                    fvxComp = altGreenTemp[i][j];
        			fvyComp = fgD_[2]*(greenTemp[im2][j] - greenTemp[ip2][j]) +
								fgD_[1]*(greenTemp[im1][j] - greenTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = altBlueTemp[i][j];
        			fvyComp = fgD_[2]*(blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgD_[1]*(blueTemp[im1][j] - blueTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvx[i][j] = fvxComp;
                        fvy[i][j] = fvyComp;
                    }
                    else
                    {
                        fvx[i][j] = fvxTemp;
                        fvy[i][j] = fvyTemp;
                    }
                }

                //  shift the i indices (ip1 is taken care of in the loop statement)
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2++;
            }
           break;
        }
        
        //  this should never happen
        default:
            break;
            
    }
}

#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Hard-coded private methods that implement the 7x7 convolution
//-------------------------------------------------------------------------------


void ColorImageGradient_Gaussian_F::applyIntoI_7x7_X_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{

    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM3 = iLow - 3, iHighP3 = iHigh + 3;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm3R = 4*(jLow-3);      //  4*(j-3) red green blue
    int fourjm3G = fourjm3R + 1;
    int fourjm3B = fourjm3G + 1;
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    int fourjp3R = 4*(jLow+3);      //  4*(j+3) red green blue
    int fourjp3G = fourjp3R + 1;
    int fourjp3B = fourjp3G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM3; i<=iHighP3; i++)
        {
			redTemp[i][j] = 	fgD_[3]*(rgba[i][fourjm3R] - rgba[i][fourjp3R]) +
								fgD_[2]*(rgba[i][fourjm2R] - rgba[i][fourjp2R]) +
								fgD_[1]*(rgba[i][fourjm1R] - rgba[i][fourjp1R]);
								
			greenTemp[i][j] = 	fgD_[3]*(rgba[i][fourjm3G] - rgba[i][fourjp3G]) +
								fgD_[2]*(rgba[i][fourjm2G] - rgba[i][fourjp2G]) +
								fgD_[1]*(rgba[i][fourjm1G] - rgba[i][fourjp1G]);
								
			blueTemp[i][j] = 	fgD_[3]*(rgba[i][fourjm3B] - rgba[i][fourjp3B]) +
								fgD_[2]*(rgba[i][fourjm2B] - rgba[i][fourjp2B]) +
								fgD_[1]*(rgba[i][fourjm1B] - rgba[i][fourjp1B]);

        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R += 4;
        fourjp3G += 4;
        fourjp3B += 4;
	}

    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_7x7_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoI_7x7_Y_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM3 = iLow - 3, iHighP3 = iHigh + 3;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm3R = 4*(jLow-3);      //  4*(j-3) red green blue
    int fourjm3G = fourjm3R + 1;
    int fourjm3B = fourjm3G + 1;
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    int fourjp3R = 4*(jLow+3);      //  4*(j+3) red green blue
    int fourjp3G = fourjp3R + 1;
    int fourjp3B = fourjp3G + 1;
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM3; i<=iHighP3; i++)
        {
			redTemp[i][j] = 	fg_[3]*(rgba[i][fourjm3R] + rgba[i][fourjp3R]) +
								fg_[2]*(rgba[i][fourjm2R] + rgba[i][fourjp2R]) +
								fg_[1]*(rgba[i][fourjm1R] + rgba[i][fourjp1R]) +
								fg_[0]* rgba[i][fourjR];

			greenTemp[i][j] = 	fg_[3]*(rgba[i][fourjm3G] + rgba[i][fourjp3G]) +
								fg_[2]*(rgba[i][fourjm2G] + rgba[i][fourjp2G]) +
								fg_[1]*(rgba[i][fourjm1G] + rgba[i][fourjp1G]) +
								fg_[0]* rgba[i][fourjG];
								
			blueTemp[i][j] = 	fg_[3]*(rgba[i][fourjm3B] + rgba[i][fourjp3B]) +
								fg_[2]*(rgba[i][fourjm2B] + rgba[i][fourjp2B]) +
								fg_[1]*(rgba[i][fourjm1B] + rgba[i][fourjp1B]) +
								fg_[0]* rgba[i][fourjB];
								
        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R += 4;
        fourjp3G += 4;
        fourjp3B += 4;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_7x7_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyIntoF_7x7_X_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM3 = iLow - 3, iHighP3 = iHigh + 3;
    int 	jm1, jm2, jm3, jp1, jp2, jp3;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
    jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM3; i<=iHighP3; i++)
        {
			redTemp[i][j] = 	fgD_[3]*(fRed[i][jm3] - fRed[i][jp3]) +
								fgD_[2]*(fRed[i][jm2] - fRed[i][jp2]) +
								fgD_[1]*(fRed[i][jm1] - fRed[i][jp1]);

			greenTemp[i][j] = 	fgD_[3]*(fGreen[i][jm3] - fGreen[i][jp3]) +
								fgD_[2]*(fGreen[i][jm2] - fGreen[i][jp2]) +
								fgD_[1]*(fGreen[i][jm1] - fGreen[i][jp1]);

			blueTemp[i][j] = 	fgD_[3]*(fBlue[i][jm3] - fBlue[i][jp3]) +
								fgD_[2]*(fBlue[i][jm2] - fBlue[i][jp2]) +
								fgD_[1]*(fBlue[i][jm1] - fBlue[i][jp1]);
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_7x7_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoF_7x7_Y_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM3 = iLow - 3, iHighP3 = iHigh + 3;
    int 	jm1, jm2, jm3, jp1, jp2, jp3;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
    jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
    jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM3; i<=iHighP3; i++)
        {
			redTemp[i][j] = 	fg_[3]*(fRed[i][jm3] + fRed[i][jp3]) +
								fg_[2]*(fRed[i][jm2] + fRed[i][jp2]) +
								fg_[1]*(fRed[i][jm1] + fRed[i][jp1]) +
								fg_[0]* fRed[i][j];

			greenTemp[i][j] = 	fg_[3]*(fGreen[i][jm3] + fGreen[i][jp3]) +
								fg_[2]*(fGreen[i][jm2] + fGreen[i][jp2]) +
								fg_[1]*(fGreen[i][jm1] + fGreen[i][jp1]) +
								fg_[0]* fGreen[i][j];

			blueTemp[i][j] = 	fg_[3]*(fBlue[i][jm3] + fBlue[i][jp3]) +
								fg_[2]*(fBlue[i][jm2] + fBlue[i][jp2]) +
								fg_[1]*(fBlue[i][jm1] + fBlue[i][jp1]) +
								fg_[0]* fBlue[i][j];
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_7x7_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyInto_7x7_X_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx)
{
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3; 
    
    //-------------------------------------------------------
    //	Apply the 1D filter along the y direction
    //-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
			const   float   fgScaled[4] = {fg_[0]/3.f, fg_[1]/3.f, fg_[2]/3.f, fg_[3]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvx[i][j] =   fgScaled[3]*(redTemp[im3][j] + redTemp[ip3][j] +
                			                   greenTemp[im3][j] + greenTemp[ip3][j] +
                			                   blueTemp[im3][j] + blueTemp[ip3][j]) +
        			              fgScaled[2]*(redTemp[im2][j] + redTemp[ip2][j] +
                			                   greenTemp[im2][j] + greenTemp[ip2][j] +
                			                   blueTemp[im2][j] + blueTemp[ip2][j]) +
        			              fgScaled[1]*(redTemp[im1][j] + redTemp[ip1][j] +
                			                   greenTemp[im1][j] + greenTemp[ip1][j] +
                			                   blueTemp[im1][j] + blueTemp[ip1][j]) +
        						  fgScaled[0]* (redTemp[i][j] + greenTemp[i][j] + blueTemp[i][j]);
        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3++;
        	}
            break;
		}	
            
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);

        	//-------------------------------------------------------
        	//	then apply the 1D filter along the y direction
        	//-------------------------------------------------------
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			altRedTemp[i][j] =    fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
            								fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
            								fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
            								fg_[0]* redTemp[i][j];

        			altGreenTemp[i][j] =  fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
            								fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
            								fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
            								fg_[0]* greenTemp[i][j];

        			altBlueTemp[i][j]  =  fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
            								fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
            								fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
            								fg_[0]* blueTemp[i][j];

        		}

        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3++;
        	}
            break;
        }
                        
        //  this should never happen
        default:
            break;
            
    }
}


void ColorImageGradient_Gaussian_F::applyInto_7x7_Y_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx, float*const* fvy)
{
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3; 
    float   fvxTemp, fvyTemp;        

	//-------------------------------------------------------
	//	Apply the 1D filter along the y direction
	//-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
            const   float   fgDScaled[4] = {0, fgD_[1]/3.f, fgD_[2]/3.f, fgD_[3]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvy[i][j] = fgDScaled[3]*(redTemp[im3][j] - redTemp[ip3][j] +
								              greenTemp[im3][j] - greenTemp[ip3][j] +
								              blueTemp[im3][j] - blueTemp[ip3][j])+
							    fgDScaled[2]*(redTemp[im2][j] - redTemp[ip2][j] +
								              greenTemp[im2][j] - greenTemp[ip2][j] +
								              blueTemp[im2][j] - blueTemp[ip2][j])+
							    fgDScaled[1]*(redTemp[im1][j] - redTemp[ip1][j] +
								              greenTemp[im1][j] - greenTemp[ip1][j] +
								              blueTemp[im1][j] - blueTemp[ip1][j]);
        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3++;
        	}
        	break;
		}	

        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = altRedTemp[i][j];
                    fvyTemp = fgD_[3]*(redTemp[im3][j] - redTemp[ip3][j]) +
								fgD_[2]*(redTemp[im2][j] - redTemp[ip2][j]) +
								fgD_[1]*(redTemp[im1][j] - redTemp[ip1][j]);
                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;

                    //  green component
                    fvxComp = altGreenTemp[i][j];
        			fvyComp = fgD_[3]*(greenTemp[im3][j] - greenTemp[ip3][j]) +
								fgD_[2]*(greenTemp[im2][j] - greenTemp[ip2][j]) +
								fgD_[1]*(greenTemp[im1][j] - greenTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = altBlueTemp[i][j];
        			fvyComp = fgD_[3]*(blueTemp[im3][j] - blueTemp[ip3][j]) +
								fgD_[2]*(blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgD_[1]*(blueTemp[im1][j] - blueTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvx[i][j] = fvxComp;
                        fvy[i][j] = fvyComp;
                    }
                    else
                    {
                        fvx[i][j] = fvxTemp;
                        fvy[i][j] = fvyTemp;
                    }
                }

                //  shift the i indices (ip1 is taken care of in the loop statement)
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3++;
            }
           break;
        }
        
        //  this should never happen
        default:
            break;
            
    }
}

#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Hard-coded private methods that implement the 9x9 convolution
//-------------------------------------------------------------------------------

void ColorImageGradient_Gaussian_F::applyIntoI_9x9_X_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM4 = iLow - 4, iHighP4 = iHigh + 4;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm4R = 4*(jLow-4);      //  4*(j-4) red green blue
    int fourjm4G = fourjm4R + 1;
    int fourjm4B = fourjm4G + 1;
    int fourjm3R = 4*(jLow-3);      //  4*(j-3) red green blue
    int fourjm3G = fourjm3R + 1;
    int fourjm3B = fourjm3G + 1;
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    int fourjp3R = 4*(jLow+3);      //  4*(j+3) red green blue
    int fourjp3G = fourjp3R + 1;
    int fourjp3B = fourjp3G + 1;
    int fourjp4R = 4*(jLow+4);      //  4*(j+4) red green blue
    int fourjp4G = fourjp4R + 1;
    int fourjp4B = fourjp4G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM4; i<=iHighP4; i++)
        {
			redTemp[i][j] = 	fgD_[4]*(rgba[i][fourjm4R] - rgba[i][fourjp4R]) +
								fgD_[3]*(rgba[i][fourjm3R] - rgba[i][fourjp3R]) +
								fgD_[2]*(rgba[i][fourjm2R] - rgba[i][fourjp2R]) +
								fgD_[1]*(rgba[i][fourjm1R] - rgba[i][fourjp1R]);
								
			greenTemp[i][j] = 	fgD_[4]*(rgba[i][fourjm4G] - rgba[i][fourjp4G]) +
								fgD_[3]*(rgba[i][fourjm3G] - rgba[i][fourjp3G]) +
								fgD_[2]*(rgba[i][fourjm2G] - rgba[i][fourjp2G]) +
								fgD_[1]*(rgba[i][fourjm1G] - rgba[i][fourjp1G]);
								
			blueTemp[i][j] = 	fgD_[4]*(rgba[i][fourjm4B] - rgba[i][fourjp4B]) +
								fgD_[3]*(rgba[i][fourjm3B] - rgba[i][fourjp3B]) +
								fgD_[2]*(rgba[i][fourjm2B] - rgba[i][fourjp2B]) +
								fgD_[1]*(rgba[i][fourjm1B] - rgba[i][fourjp1B]);

        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm4R = fourjm3R;
        fourjm4G = fourjm3G;
        fourjm4B = fourjm3B;
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R  = fourjp4R;
        fourjp3G  = fourjp4G;
        fourjp3B  = fourjp4B;
        fourjp4R += 4;
        fourjp4G += 4;
        fourjp4B += 4;
	}

    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_9x9_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoI_9x9_Y_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM4 = iLow - 4, iHighP4 = iHigh + 4;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm4R = 4*(jLow-4);      //  4*(j-4) red green blue
    int fourjm4G = fourjm4R + 1;
    int fourjm4B = fourjm4G + 1;
    int fourjm3R = 4*(jLow-3);      //  4*(j-3) red green blue
    int fourjm3G = fourjm3R + 1;
    int fourjm3B = fourjm3G + 1;
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    int fourjp3R = 4*(jLow+3);      //  4*(j+3) red green blue
    int fourjp3G = fourjp3R + 1;
    int fourjp3B = fourjp3G + 1;
    int fourjp4R = 4*(jLow+4);      //  4*(j+4) red green blue
    int fourjp4G = fourjp4R + 1;
    int fourjp4B = fourjp4G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM4; i<=iHighP4; i++)
        {
			redTemp[i][j] = 	fg_[4]*(rgba[i][fourjm4R] + rgba[i][fourjp4R]) +
								fg_[3]*(rgba[i][fourjm3R] + rgba[i][fourjp3R]) +
								fg_[2]*(rgba[i][fourjm2R] + rgba[i][fourjp2R]) +
								fg_[1]*(rgba[i][fourjm1R] + rgba[i][fourjp1R]) +
								fg_[0]* rgba[i][fourjR];

			greenTemp[i][j] = 	fg_[4]*(rgba[i][fourjm4G] + rgba[i][fourjp4G]) +
								fg_[3]*(rgba[i][fourjm3G] + rgba[i][fourjp3G]) +
								fg_[2]*(rgba[i][fourjm2G] + rgba[i][fourjp2G]) +
								fg_[1]*(rgba[i][fourjm1G] + rgba[i][fourjp1G]) +
								fg_[0]* rgba[i][fourjG];
								
			blueTemp[i][j] = 	fg_[4]*(rgba[i][fourjm4B] + rgba[i][fourjp4B]) +
								fg_[3]*(rgba[i][fourjm3B] + rgba[i][fourjp3B]) +
								fg_[2]*(rgba[i][fourjm2B] + rgba[i][fourjp2B]) +
								fg_[1]*(rgba[i][fourjm1B] + rgba[i][fourjp1B]) +
								fg_[0]* rgba[i][fourjB];
								
        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm4R = fourjm3R;
        fourjm4G = fourjm3G;
        fourjm4B = fourjm3B;
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R  = fourjp4R;
        fourjp3G  = fourjp4G;
        fourjp3B  = fourjp4B;
        fourjp4R += 4;
        fourjp4G += 4;
        fourjp4B += 4;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_9x9_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyIntoF_9x9_X_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM4 = iLow - 4, iHighP4 = iHigh + 4;
    int 	jm1, jm2, jm3, jm4, jp1, jp2, jp3, jp4;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3, jm4 = jLow-4; 
    jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3, jp4 = jLow+4; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM4; i<=iHighP4; i++)
        {
			redTemp[i][j] = 	fgD_[4]*(fRed[i][jm4] - fRed[i][jp4]) +
								fgD_[3]*(fRed[i][jm3] - fRed[i][jp3]) +
								fgD_[2]*(fRed[i][jm2] - fRed[i][jp2]) +
								fgD_[1]*(fRed[i][jm1] - fRed[i][jp1]);

			greenTemp[i][j] = 	fgD_[4]*(fGreen[i][jm4] - fGreen[i][jp4]) +
								fgD_[3]*(fGreen[i][jm3] - fGreen[i][jp3]) +
								fgD_[2]*(fGreen[i][jm2] - fGreen[i][jp2]) +
								fgD_[1]*(fGreen[i][jm1] - fGreen[i][jp1]);

			blueTemp[i][j] = 	fgD_[4]*(fBlue[i][jm4] - fBlue[i][jp4]) +
								fgD_[3]*(fBlue[i][jm3] - fBlue[i][jp3]) +
								fgD_[2]*(fBlue[i][jm2] - fBlue[i][jp2]) +
								fgD_[1]*(fBlue[i][jm1] - fBlue[i][jp1]);
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm4 = jm3;
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3 = jp4;
        jp4++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_9x9_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}


void ColorImageGradient_Gaussian_F::applyIntoF_9x9_Y_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM4 = iLow - 4, iHighP4 = iHigh + 4;
    int 	jm1, jm2, jm3, jm4, jp1, jp2, jp3, jp4;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
    jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3, jm4 = jLow-4; 
    jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3, jp4 = jLow+4; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM4; i<=iHighP4; i++)
        {
			redTemp[i][j] = 	fg_[4]*(fRed[i][jm4] + fRed[i][jp4]) +
								fg_[3]*(fRed[i][jm3] + fRed[i][jp3]) +
								fg_[2]*(fRed[i][jm2] + fRed[i][jp2]) +
								fg_[1]*(fRed[i][jm1] + fRed[i][jp1]) +
								fg_[0]* fRed[i][j];

			greenTemp[i][j] = 	fg_[4]*(fGreen[i][jm4] + fGreen[i][jp4]) +
								fg_[3]*(fGreen[i][jm3] + fGreen[i][jp3]) +
								fg_[2]*(fGreen[i][jm2] + fGreen[i][jp2]) +
								fg_[1]*(fGreen[i][jm1] + fGreen[i][jp1]) +
								fg_[0]* fGreen[i][j];

			blueTemp[i][j] = 	fg_[4]*(fBlue[i][jm4] + fBlue[i][jp4]) +
								fg_[3]*(fBlue[i][jm3] + fBlue[i][jp3]) +
								fg_[2]*(fBlue[i][jm2] + fBlue[i][jp2]) +
								fg_[1]*(fBlue[i][jm1] + fBlue[i][jp1]) +
								fg_[0]* fBlue[i][j];
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm4 = jm3;
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3 = jp4;
        jp4++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_9x9_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyInto_9x9_X_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx)
{
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3, im4 = iLow-4; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3, ip4 = iLow+4; 
    
    //-------------------------------------------------------
    //	Apply the 1D filter along the y direction
    //-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
			const   float   fgScaled[5] = {fg_[0]/3.f, fg_[1]/3.f, fg_[2]/3.f, fg_[3]/3.f, fg_[4]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvx[i][j] =   fgScaled[4]*(redTemp[im4][j] + redTemp[ip4][j] +
                			                   greenTemp[im4][j] + greenTemp[ip4][j] +
                			                   blueTemp[im4][j] + blueTemp[ip4][j]) +
        			              fgScaled[3]*(redTemp[im3][j] + redTemp[ip3][j] +
                			                   greenTemp[im3][j] + greenTemp[ip3][j] +
                			                   blueTemp[im3][j] + blueTemp[ip3][j]) +
        			              fgScaled[2]*(redTemp[im2][j] + redTemp[ip2][j] +
                			                   greenTemp[im2][j] + greenTemp[ip2][j] +
                			                   blueTemp[im2][j] + blueTemp[ip2][j]) +
        			              fgScaled[1]*(redTemp[im1][j] + redTemp[ip1][j] +
                			                   greenTemp[im1][j] + greenTemp[ip1][j] +
                			                   blueTemp[im1][j] + blueTemp[ip1][j]) +
        						  fgScaled[0]* (redTemp[i][j] + greenTemp[i][j] + blueTemp[i][j]);
        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4++;
        	}
            break;
		}
        
            
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);

        	//-------------------------------------------------------
        	//	then apply the 1D filter along the y direction
        	//-------------------------------------------------------
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			altRedTemp[i][j] =      fg_[4]*(redTemp[im4][j] + redTemp[ip4][j]) +
            								fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
            								fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
            								fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
            								fg_[0]* redTemp[i][j];

        			altGreenTemp[i][j] =    fg_[4]*(greenTemp[im4][j] + greenTemp[ip4][j]) +
            								fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
            								fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
            								fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
            								fg_[0]* greenTemp[i][j];

        			altBlueTemp[i][j]  =    fg_[4]*(blueTemp[im4][j] + blueTemp[ip4][j]) +
            								fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
            								fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
            								fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
            								fg_[0]* blueTemp[i][j];

        		}

        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4++;
        	}
            break;
        }
                        
        //  this should never happen
        default:
            break;
            
    }
}


void ColorImageGradient_Gaussian_F::applyInto_9x9_Y_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx, float*const* fvy)
{
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3, im4 = iLow-4; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3, ip4 = iLow+4; 
    float   fvxTemp, fvyTemp;        

	//-------------------------------------------------------
	//	Apply the 1D filter along the y direction
	//-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
            const   float   fgDScaled[5] = {0, fgD_[1]/3.f, fgD_[2]/3.f, fgD_[3]/3.f, fgD_[4]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvy[i][j] = fgDScaled[4]*(redTemp[im4][j] - redTemp[ip4][j] +
								              greenTemp[im3][j] - greenTemp[ip4][j] +
								              blueTemp[im4][j] - blueTemp[ip4][j]) +
								fgDScaled[3]*(redTemp[im3][j] - redTemp[ip3][j] +
								              greenTemp[im3][j] - greenTemp[ip3][j] +
								              blueTemp[im3][j] - blueTemp[ip3][j]) +
								fgDScaled[2]*(redTemp[im2][j] - redTemp[ip2][j] +
								              greenTemp[im2][j] - greenTemp[ip2][j] +
								              blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgDScaled[1]*(redTemp[im1][j] - redTemp[ip1][j] +
								              greenTemp[im1][j] - greenTemp[ip1][j] +
								              blueTemp[im1][j] - blueTemp[ip1][j]);

        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4++;
        	}
        	break;
        }
        
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = altRedTemp[i][j];
                    fvyTemp =   fgD_[4]*(redTemp[im4][j] - redTemp[ip4][j]) +
								fgD_[3]*(redTemp[im3][j] - redTemp[ip3][j]) +
								fgD_[2]*(redTemp[im2][j] - redTemp[ip2][j]) +
								fgD_[1]*(redTemp[im1][j] - redTemp[ip1][j]);
                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;

                    //  green component
                    fvxComp = altGreenTemp[i][j];
        			fvyComp =   fgD_[4]*(greenTemp[im4][j] - greenTemp[ip4][j]) +
								fgD_[3]*(greenTemp[im3][j] - greenTemp[ip3][j]) +
								fgD_[2]*(greenTemp[im2][j] - greenTemp[ip2][j]) +
								fgD_[1]*(greenTemp[im1][j] - greenTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = altBlueTemp[i][j];
        			fvyComp =   fgD_[4]*(blueTemp[im4][j] - blueTemp[ip4][j]) +
								fgD_[3]*(blueTemp[im3][j] - blueTemp[ip3][j]) +
								fgD_[2]*(blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgD_[1]*(blueTemp[im1][j] - blueTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvx[i][j] = fvxComp;
                        fvy[i][j] = fvyComp;
                    }
                    else
                    {
                        fvx[i][j] = fvxTemp;
                        fvy[i][j] = fvyTemp;
                    }
                }

                //  shift the i indices (ip1 is taken care of in the loop statement)
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4++;
            }
           break;
        }
        
        //  this should never happen
        default:
            break;
            
    }
}

#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Hard-coded private methods that implement the 11x11 convolution
//-------------------------------------------------------------------------------


void ColorImageGradient_Gaussian_F::applyIntoI_11x11_X_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM5 = iLow - 5, iHighP5 = iHigh + 5;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm5R = 4*(jLow-5);      //  4*(j-5) red green blue
    int fourjm5G = fourjm5R + 1;
    int fourjm5B = fourjm5G + 1;
    int fourjm4R = 4*(jLow-4);      //  4*(j-4) red green blue
    int fourjm4G = fourjm4R + 1;
    int fourjm4B = fourjm4G + 1;
    int fourjm3R = 4*(jLow-3);      //  4*(j-3) red green blue
    int fourjm3G = fourjm3R + 1;
    int fourjm3B = fourjm3G + 1;
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    int fourjp3R = 4*(jLow+3);      //  4*(j+3) red green blue
    int fourjp3G = fourjp3R + 1;
    int fourjp3B = fourjp3G + 1;
    int fourjp4R = 4*(jLow+4);      //  4*(j+4) red green blue
    int fourjp4G = fourjp4R + 1;
    int fourjp4B = fourjp4G + 1;
    int fourjp5R = 4*(jLow+5);      //  4*(j+5) red green blue
    int fourjp5G = fourjp5R + 1;
    int fourjp5B = fourjp5G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM5; i<=iHighP5; i++)
        {
			redTemp[i][j] = 	fgD_[5]*(rgba[i][fourjm5R] - rgba[i][fourjp5R]) +
								fgD_[4]*(rgba[i][fourjm4R] - rgba[i][fourjp4R]) +
								fgD_[3]*(rgba[i][fourjm3R] - rgba[i][fourjp3R]) +
								fgD_[2]*(rgba[i][fourjm2R] - rgba[i][fourjp2R]) +
								fgD_[1]*(rgba[i][fourjm1R] - rgba[i][fourjp1R]);
								
			greenTemp[i][j] = 	fgD_[5]*(rgba[i][fourjm5G] - rgba[i][fourjp5G]) +
								fgD_[4]*(rgba[i][fourjm4G] - rgba[i][fourjp4G]) +
								fgD_[3]*(rgba[i][fourjm3G] - rgba[i][fourjp3G]) +
								fgD_[2]*(rgba[i][fourjm2G] - rgba[i][fourjp2G]) +
								fgD_[1]*(rgba[i][fourjm1G] - rgba[i][fourjp1G]);
								
			blueTemp[i][j] = 	fgD_[5]*(rgba[i][fourjm5B] - rgba[i][fourjp5B]) +
								fgD_[4]*(rgba[i][fourjm4B] - rgba[i][fourjp4B]) +
								fgD_[3]*(rgba[i][fourjm3B] - rgba[i][fourjp3B]) +
								fgD_[2]*(rgba[i][fourjm2B] - rgba[i][fourjp2B]) +
								fgD_[1]*(rgba[i][fourjm1B] - rgba[i][fourjp1B]);

        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm5R = fourjm4R;
        fourjm5G = fourjm4G;
        fourjm5B = fourjm4B;
        fourjm4R = fourjm3R;
        fourjm4G = fourjm3G;
        fourjm4B = fourjm3B;
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R  = fourjp4R;
        fourjp3G  = fourjp4G;
        fourjp3B  = fourjp4B;
        fourjp4R  = fourjp5R;
        fourjp4G  = fourjp5G;
        fourjp4B  = fourjp5B;
        fourjp5R += 4;
        fourjp5G += 4;
        fourjp5B += 4;
	}

    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_11x11_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoI_11x11_Y_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM5 = iLow - 5, iHighP5 = iHigh + 5;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm5R = 4*(jLow-5);      //  4*(j-5) red green blue
    int fourjm5G = fourjm5R + 1;
    int fourjm5B = fourjm5G + 1;
    int fourjm4R = 4*(jLow-4);      //  4*(j-4) red green blue
    int fourjm4G = fourjm4R + 1;
    int fourjm4B = fourjm4G + 1;
    int fourjm3R = 4*(jLow-3);      //  4*(j-3) red green blue
    int fourjm3G = fourjm3R + 1;
    int fourjm3B = fourjm3G + 1;
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    int fourjp3R = 4*(jLow+3);      //  4*(j+3) red green blue
    int fourjp3G = fourjp3R + 1;
    int fourjp3B = fourjp3G + 1;
    int fourjp4R = 4*(jLow+4);      //  4*(j+4) red green blue
    int fourjp4G = fourjp4R + 1;
    int fourjp4B = fourjp4G + 1;
    int fourjp5R = 4*(jLow+5);      //  4*(j+5) red green blue
    int fourjp5G = fourjp5R + 1;
    int fourjp5B = fourjp5G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM5; i<=iHighP5; i++)
        {
			redTemp[i][j] = 	fg_[5]*(rgba[i][fourjm5R] + rgba[i][fourjp5R]) +
								fg_[4]*(rgba[i][fourjm4R] + rgba[i][fourjp4R]) +
								fg_[3]*(rgba[i][fourjm3R] + rgba[i][fourjp3R]) +
								fg_[2]*(rgba[i][fourjm2R] + rgba[i][fourjp2R]) +
								fg_[1]*(rgba[i][fourjm1R] + rgba[i][fourjp1R]) +
								fg_[0]* rgba[i][fourjR];

			greenTemp[i][j] = 	fg_[5]*(rgba[i][fourjm5G] + rgba[i][fourjp5G]) +
								fg_[4]*(rgba[i][fourjm4G] + rgba[i][fourjp4G]) +
								fg_[3]*(rgba[i][fourjm3G] + rgba[i][fourjp3G]) +
								fg_[2]*(rgba[i][fourjm2G] + rgba[i][fourjp2G]) +
								fg_[1]*(rgba[i][fourjm1G] + rgba[i][fourjp1G]) +
								fg_[0]* rgba[i][fourjG];
								
			blueTemp[i][j] = 	fg_[5]*(rgba[i][fourjm5B] + rgba[i][fourjp5B]) +
								fg_[4]*(rgba[i][fourjm4B] + rgba[i][fourjp4B]) +
								fg_[3]*(rgba[i][fourjm3B] + rgba[i][fourjp3B]) +
								fg_[2]*(rgba[i][fourjm2B] + rgba[i][fourjp2B]) +
								fg_[1]*(rgba[i][fourjm1B] + rgba[i][fourjp1B]) +
								fg_[0]* rgba[i][fourjB];
								
        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm5R = fourjm4R;
        fourjm5G = fourjm4G;
        fourjm5B = fourjm4B;
        fourjm4R = fourjm3R;
        fourjm4G = fourjm3G;
        fourjm4B = fourjm3B;
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R  = fourjp4R;
        fourjp3G  = fourjp4G;
        fourjp3B  = fourjp4B;
        fourjp4R  = fourjp5R;
        fourjp4G  = fourjp5G;
        fourjp4B  = fourjp5B;
        fourjp5R += 4;
        fourjp5G += 4;
        fourjp5B += 4;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_11x11_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyIntoF_11x11_X_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM5 = iLow - 5, iHighP5 = iHigh + 5;
    int 	jm1, jm2, jm3, jm4, jm5, jp1, jp2, jp3, jp4, jp5;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3, jm4 = jLow-4, jm5 = jLow-5; 
    jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3, jp4 = jLow+4, jp5 = jLow+5; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM5; i<=iHighP5; i++)
        {
			redTemp[i][j] = 	fgD_[5]*(fRed[i][jm5] - fRed[i][jp5]) +
								fgD_[4]*(fRed[i][jm4] - fRed[i][jp4]) +
								fgD_[3]*(fRed[i][jm3] - fRed[i][jp3]) +
								fgD_[2]*(fRed[i][jm2] - fRed[i][jp2]) +
								fgD_[1]*(fRed[i][jm1] - fRed[i][jp1]);

			greenTemp[i][j] = 	fgD_[5]*(fGreen[i][jm5] - fGreen[i][jp5]) +
								fgD_[4]*(fGreen[i][jm4] - fGreen[i][jp4]) +
								fgD_[3]*(fGreen[i][jm3] - fGreen[i][jp3]) +
								fgD_[2]*(fGreen[i][jm2] - fGreen[i][jp2]) +
								fgD_[1]*(fGreen[i][jm1] - fGreen[i][jp1]);

			blueTemp[i][j] = 	fgD_[5]*(fBlue[i][jm5] - fBlue[i][jp5]) +
								fgD_[4]*(fBlue[i][jm4] - fBlue[i][jp4]) +
								fgD_[3]*(fBlue[i][jm3] - fBlue[i][jp3]) +
								fgD_[2]*(fBlue[i][jm2] - fBlue[i][jp2]) +
								fgD_[1]*(fBlue[i][jm1] - fBlue[i][jp1]);
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm5 = jm4;
        jm4 = jm3;
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3 = jp4;
        jp4 = jp5;
        jp5++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_11x11_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoF_11x11_Y_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM5 = iLow - 5, iHighP5 = iHigh + 5;
    int 	jm1, jm2, jm3, jm4, jm5, jp1, jp2, jp3, jp4, jp5;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
    jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3, jm4 = jLow-4, jm5 = jLow-5; 
    jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3, jp4 = jLow+4, jp5 = jLow+5; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM5; i<=iHighP5; i++)
        {
			redTemp[i][j] = 	fg_[5]*(fRed[i][jm5] + fRed[i][jp5]) +
								fg_[4]*(fRed[i][jm4] + fRed[i][jp4]) +
								fg_[3]*(fRed[i][jm3] + fRed[i][jp3]) +
								fg_[2]*(fRed[i][jm2] + fRed[i][jp2]) +
								fg_[1]*(fRed[i][jm1] + fRed[i][jp1]) +
								fg_[0]* fRed[i][j];

			greenTemp[i][j] = 	fg_[5]*(fGreen[i][jm5] + fGreen[i][jp5]) +
								fg_[4]*(fGreen[i][jm4] + fGreen[i][jp4]) +
								fg_[3]*(fGreen[i][jm3] + fGreen[i][jp3]) +
								fg_[2]*(fGreen[i][jm2] + fGreen[i][jp2]) +
								fg_[1]*(fGreen[i][jm1] + fGreen[i][jp1]) +
								fg_[0]* fGreen[i][j];

			blueTemp[i][j] = 	fg_[5]*(fBlue[i][jm5] + fBlue[i][jp5]) +
								fg_[4]*(fBlue[i][jm4] + fBlue[i][jp4]) +
								fg_[3]*(fBlue[i][jm3] + fBlue[i][jp3]) +
								fg_[2]*(fBlue[i][jm2] + fBlue[i][jp2]) +
								fg_[1]*(fBlue[i][jm1] + fBlue[i][jp1]) +
								fg_[0]* fBlue[i][j];
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm5 = jm4;
        jm4 = jm3;
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3 = jp4;
        jp4 = jp5;
        jp5++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_11x11_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyInto_11x11_X_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx)
{
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3, im4 = iLow-4, im5 = iLow-5; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3, ip4 = iLow+4, ip5 = iLow+5; 
    
    //-------------------------------------------------------
    //	Apply the 1D filter along the y direction
    //-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
			const   float   fgScaled[6] = {fg_[0]/3.f, fg_[1]/3.f, fg_[2]/3.f, fg_[3]/3.f, fg_[4]/3.f,
			                               fg_[5]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvx[i][j] =   fgScaled[5]*(redTemp[im5][j] + redTemp[ip5][j] +
                			                   greenTemp[im5][j] + greenTemp[ip5][j] +
                			                   blueTemp[im5][j] + blueTemp[ip5][j]) +
        			              fgScaled[4]*(redTemp[im4][j] + redTemp[ip4][j] +
                			                   greenTemp[im4][j] + greenTemp[ip4][j] +
                			                   blueTemp[im4][j] + blueTemp[ip4][j]) +
        			              fgScaled[3]*(redTemp[im3][j] + redTemp[ip3][j] +
                			                   greenTemp[im3][j] + greenTemp[ip3][j] +
                			                   blueTemp[im3][j] + blueTemp[ip3][j]) +
        			              fgScaled[2]*(redTemp[im2][j] + redTemp[ip2][j] +
                			                   greenTemp[im2][j] + greenTemp[ip2][j] +
                			                   blueTemp[im2][j] + blueTemp[ip2][j]) +
        			              fgScaled[1]*(redTemp[im1][j] + redTemp[ip1][j] +
                			                   greenTemp[im1][j] + greenTemp[ip1][j] +
                			                   blueTemp[im1][j] + blueTemp[ip1][j]) +
        						  fgScaled[0]* (redTemp[i][j] + greenTemp[i][j] + blueTemp[i][j]);
        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5++;
        	}
            break;
        }
            
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);

        	//-------------------------------------------------------
        	//	then apply the 1D filter along the y direction
        	//-------------------------------------------------------
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			altRedTemp[i][j] =      fg_[5]*(redTemp[im5][j] + redTemp[ip5][j]) +
            								fg_[4]*(redTemp[im4][j] + redTemp[ip4][j]) +
            								fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
            								fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
            								fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
            								fg_[0]* redTemp[i][j];

        			altGreenTemp[i][j] =    fg_[5]*(greenTemp[im5][j] + greenTemp[ip5][j]) +
            								fg_[4]*(greenTemp[im4][j] + greenTemp[ip4][j]) +
            								fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
            								fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
            								fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
            								fg_[0]* greenTemp[i][j];

        			altBlueTemp[i][j]  =    fg_[5]*(blueTemp[im5][j] + blueTemp[ip5][j]) +
            								fg_[4]*(blueTemp[im4][j] + blueTemp[ip4][j]) +
            								fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
            								fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
            								fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
            								fg_[0]* blueTemp[i][j];

        		}

        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5++;
        	}
            break;
        }
                        
        //  this should never happen
        default:
            break;
            
    }
}


void ColorImageGradient_Gaussian_F::applyInto_11x11_Y_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx, float*const* fvy)
{
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3, im4 = iLow-4, im5 = iLow-5; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3, ip4 = iLow+4, ip5 = iLow+5; 
    float   fvxTemp, fvyTemp;        

	//-------------------------------------------------------
	//	Apply the 1D filter along the y direction
	//-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
            const   float   fgDScaled[6] = {0, fgD_[1]/3.f, fgD_[2]/3.f, fgD_[3]/3.f, fgD_[4]/3.f,
                                            fgD_[5]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvy[i][j] = fgDScaled[5]*(redTemp[im5][j] - redTemp[ip5][j] +
								              greenTemp[im5][j] - greenTemp[ip5][j] +
    								          blueTemp[im5][j] - blueTemp[ip5][j]) +
								fgDScaled[4]*(redTemp[im4][j] - redTemp[ip4][j] +
								              greenTemp[im3][j] - greenTemp[ip4][j] +
								              blueTemp[im4][j] - blueTemp[ip4][j]) +
								fgDScaled[3]*(redTemp[im3][j] - redTemp[ip3][j] +
								              greenTemp[im3][j] - greenTemp[ip3][j] +
								              blueTemp[im3][j] - blueTemp[ip3][j]) +
								fgDScaled[2]*(redTemp[im2][j] - redTemp[ip2][j] +
								              greenTemp[im2][j] - greenTemp[ip2][j] +
								              blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgDScaled[1]*(redTemp[im1][j] - redTemp[ip1][j] +
								              greenTemp[im1][j] - greenTemp[ip1][j] +
								              blueTemp[im1][j] - blueTemp[ip1][j]);

        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5++;
        	}
        	break;
        }
        
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = altRedTemp[i][j];
                    fvyTemp =   fgD_[5]*(redTemp[im5][j] - redTemp[ip5][j]) +
								fgD_[4]*(redTemp[im4][j] - redTemp[ip4][j]) +
								fgD_[3]*(redTemp[im3][j] - redTemp[ip3][j]) +
								fgD_[2]*(redTemp[im2][j] - redTemp[ip2][j]) +
								fgD_[1]*(redTemp[im1][j] - redTemp[ip1][j]);
                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;

                    //  green component
                    fvxComp = altGreenTemp[i][j];
        			fvyComp =   fgD_[5]*(greenTemp[im5][j] - greenTemp[ip5][j]) +
								fgD_[4]*(greenTemp[im4][j] - greenTemp[ip4][j]) +
								fgD_[3]*(greenTemp[im3][j] - greenTemp[ip3][j]) +
								fgD_[2]*(greenTemp[im2][j] - greenTemp[ip2][j]) +
								fgD_[1]*(greenTemp[im1][j] - greenTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = altBlueTemp[i][j];
        			fvyComp =   fgD_[5]*(blueTemp[im5][j] - blueTemp[ip5][j]) +
								fgD_[4]*(blueTemp[im4][j] - blueTemp[ip4][j]) +
								fgD_[3]*(blueTemp[im3][j] - blueTemp[ip3][j]) +
								fgD_[2]*(blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgD_[1]*(blueTemp[im1][j] - blueTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvx[i][j] = fvxComp;
                        fvy[i][j] = fvyComp;
                    }
                    else
                    {
                        fvx[i][j] = fvxTemp;
                        fvy[i][j] = fvyTemp;
                    }
                }

                //  shift the i indices (ip1 is taken care of in the loop statement)
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5++;
            }
           break;
        }
        
        //  this should never happen
        default:
            break;
            
    }
}


#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Hard-coded private methods that implement the 13x13 convolution
//-------------------------------------------------------------------------------


void ColorImageGradient_Gaussian_F::applyIntoI_13x13_X_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM6 = iLow - 6, iHighP6 = iHigh + 6;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm6R = 4*(jLow-6);      //  4*(j-6) red green blue
    int fourjm6G = fourjm6R + 1;
    int fourjm6B = fourjm6G + 1;
    int fourjm5R = 4*(jLow-5);      //  4*(j-5) red green blue
    int fourjm5G = fourjm5R + 1;
    int fourjm5B = fourjm5G + 1;
    int fourjm4R = 4*(jLow-4);      //  4*(j-4) red green blue
    int fourjm4G = fourjm4R + 1;
    int fourjm4B = fourjm4G + 1;
    int fourjm3R = 4*(jLow-3);      //  4*(j-3) red green blue
    int fourjm3G = fourjm3R + 1;
    int fourjm3B = fourjm3G + 1;
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    int fourjp3R = 4*(jLow+3);      //  4*(j+3) red green blue
    int fourjp3G = fourjp3R + 1;
    int fourjp3B = fourjp3G + 1;
    int fourjp4R = 4*(jLow+4);      //  4*(j+4) red green blue
    int fourjp4G = fourjp4R + 1;
    int fourjp4B = fourjp4G + 1;
    int fourjp5R = 4*(jLow+5);      //  4*(j+5) red green blue
    int fourjp5G = fourjp5R + 1;
    int fourjp5B = fourjp5G + 1;
    int fourjp6R = 4*(jLow+6);      //  4*(j+6) red green blue
    int fourjp6G = fourjp6R + 1;
    int fourjp6B = fourjp6G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM6; i<=iHighP6; i++)
        {
			redTemp[i][j] = 	fgD_[6]*(rgba[i][fourjm6R] - rgba[i][fourjp6R]) +
								fgD_[5]*(rgba[i][fourjm5R] - rgba[i][fourjp5R]) +
								fgD_[4]*(rgba[i][fourjm4R] - rgba[i][fourjp4R]) +
								fgD_[3]*(rgba[i][fourjm3R] - rgba[i][fourjp3R]) +
								fgD_[2]*(rgba[i][fourjm2R] - rgba[i][fourjp2R]) +
								fgD_[1]*(rgba[i][fourjm1R] - rgba[i][fourjp1R]);
								
			greenTemp[i][j] = 	fgD_[6]*(rgba[i][fourjm6G] - rgba[i][fourjp6G]) +
								fgD_[5]*(rgba[i][fourjm5G] - rgba[i][fourjp5G]) +
								fgD_[4]*(rgba[i][fourjm4G] - rgba[i][fourjp4G]) +
								fgD_[3]*(rgba[i][fourjm3G] - rgba[i][fourjp3G]) +
								fgD_[2]*(rgba[i][fourjm2G] - rgba[i][fourjp2G]) +
								fgD_[1]*(rgba[i][fourjm1G] - rgba[i][fourjp1G]);
								
			blueTemp[i][j] = 	fgD_[6]*(rgba[i][fourjm6B] - rgba[i][fourjp6B]) +
								fgD_[5]*(rgba[i][fourjm5B] - rgba[i][fourjp5B]) +
								fgD_[4]*(rgba[i][fourjm4B] - rgba[i][fourjp4B]) +
								fgD_[3]*(rgba[i][fourjm3B] - rgba[i][fourjp3B]) +
								fgD_[2]*(rgba[i][fourjm2B] - rgba[i][fourjp2B]) +
								fgD_[1]*(rgba[i][fourjm1B] - rgba[i][fourjp1B]);

        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm6R = fourjm5R;
        fourjm6G = fourjm5G;
        fourjm6B = fourjm5B;
        fourjm5R = fourjm4R;
        fourjm5G = fourjm4G;
        fourjm5B = fourjm4B;
        fourjm4R = fourjm3R;
        fourjm4G = fourjm3G;
        fourjm4B = fourjm3B;
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R  = fourjp4R;
        fourjp3G  = fourjp4G;
        fourjp3B  = fourjp4B;
        fourjp4R  = fourjp5R;
        fourjp4G  = fourjp5G;
        fourjp4B  = fourjp5B;
        fourjp5R  = fourjp6R;
        fourjp5G  = fourjp6G;
        fourjp5B  = fourjp6B;
        fourjp6R += 4;
        fourjp6G += 4;
        fourjp6B += 4;
	}

    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_13x13_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoI_13x13_Y_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM6 = iLow - 6, iHighP6 = iHigh + 6;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm6R = 4*(jLow-6);      //  4*(j-6) red green blue
    int fourjm6G = fourjm6R + 1;
    int fourjm6B = fourjm6G + 1;
    int fourjm5R = 4*(jLow-5);      //  4*(j-5) red green blue
    int fourjm5G = fourjm5R + 1;
    int fourjm5B = fourjm5G + 1;
    int fourjm4R = 4*(jLow-4);      //  4*(j-4) red green blue
    int fourjm4G = fourjm4R + 1;
    int fourjm4B = fourjm4G + 1;
    int fourjm3R = 4*(jLow-3);      //  4*(j-3) red green blue
    int fourjm3G = fourjm3R + 1;
    int fourjm3B = fourjm3G + 1;
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    int fourjp3R = 4*(jLow+3);      //  4*(j+3) red green blue
    int fourjp3G = fourjp3R + 1;
    int fourjp3B = fourjp3G + 1;
    int fourjp4R = 4*(jLow+4);      //  4*(j+4) red green blue
    int fourjp4G = fourjp4R + 1;
    int fourjp4B = fourjp4G + 1;
    int fourjp5R = 4*(jLow+5);      //  4*(j+5) red green blue
    int fourjp5G = fourjp5R + 1;
    int fourjp5B = fourjp5G + 1;
    int fourjp6R = 4*(jLow+6);      //  4*(j+6) red green blue
    int fourjp6G = fourjp6R + 1;
    int fourjp6B = fourjp6G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM6; i<=iHighP6; i++)
        {
			redTemp[i][j] = 	fg_[6]*(rgba[i][fourjm6R] + rgba[i][fourjp6R]) +
								fg_[5]*(rgba[i][fourjm5R] + rgba[i][fourjp5R]) +
								fg_[4]*(rgba[i][fourjm4R] + rgba[i][fourjp4R]) +
								fg_[3]*(rgba[i][fourjm3R] + rgba[i][fourjp3R]) +
								fg_[2]*(rgba[i][fourjm2R] + rgba[i][fourjp2R]) +
								fg_[1]*(rgba[i][fourjm1R] + rgba[i][fourjp1R]) +
								fg_[0]* rgba[i][fourjR];

			greenTemp[i][j] = 	fg_[6]*(rgba[i][fourjm6G] + rgba[i][fourjp6G]) +
								fg_[5]*(rgba[i][fourjm5G] + rgba[i][fourjp5G]) +
								fg_[4]*(rgba[i][fourjm4G] + rgba[i][fourjp4G]) +
								fg_[3]*(rgba[i][fourjm3G] + rgba[i][fourjp3G]) +
								fg_[2]*(rgba[i][fourjm2G] + rgba[i][fourjp2G]) +
								fg_[1]*(rgba[i][fourjm1G] + rgba[i][fourjp1G]) +
								fg_[0]* rgba[i][fourjG];
								
			blueTemp[i][j] = 	fg_[6]*(rgba[i][fourjm6B] + rgba[i][fourjp6B]) +
								fg_[5]*(rgba[i][fourjm5B] + rgba[i][fourjp5B]) +
								fg_[4]*(rgba[i][fourjm4B] + rgba[i][fourjp4B]) +
								fg_[3]*(rgba[i][fourjm3B] + rgba[i][fourjp3B]) +
								fg_[2]*(rgba[i][fourjm2B] + rgba[i][fourjp2B]) +
								fg_[1]*(rgba[i][fourjm1B] + rgba[i][fourjp1B]) +
								fg_[0]* rgba[i][fourjB];
								
        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm6R = fourjm5R;
        fourjm6G = fourjm5G;
        fourjm6B = fourjm5B;
        fourjm5R = fourjm4R;
        fourjm5G = fourjm4G;
        fourjm5B = fourjm4B;
        fourjm4R = fourjm3R;
        fourjm4G = fourjm3G;
        fourjm4B = fourjm3B;
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R  = fourjp4R;
        fourjp3G  = fourjp4G;
        fourjp3B  = fourjp4B;
        fourjp4R  = fourjp5R;
        fourjp4G  = fourjp5G;
        fourjp4B  = fourjp5B;
        fourjp5R  = fourjp6R;
        fourjp5G  = fourjp6G;
        fourjp5B  = fourjp6B;
        fourjp6R += 4;
        fourjp6G += 4;
        fourjp6B += 4;
	}
		
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_13x13_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyIntoF_13x13_X_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM6 = iLow - 6, iHighP6 = iHigh + 6;
    int 	jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3, jm4 = jLow-4, jm5 = jLow-5, jm6 = jLow-6; 
    jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3, jp4 = jLow+4, jp5 = jLow+5, jp6 = jLow+6; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM6; i<=iHighP6; i++)
        {
			redTemp[i][j] = 	fgD_[6]*(fRed[i][jm6] - fRed[i][jp6]) +
								fgD_[5]*(fRed[i][jm5] - fRed[i][jp5]) +
								fgD_[4]*(fRed[i][jm4] - fRed[i][jp4]) +
								fgD_[3]*(fRed[i][jm3] - fRed[i][jp3]) +
								fgD_[2]*(fRed[i][jm2] - fRed[i][jp2]) +
								fgD_[1]*(fRed[i][jm1] - fRed[i][jp1]);

			greenTemp[i][j] = 	fgD_[6]*(fGreen[i][jm6] - fGreen[i][jp6]) +
								fgD_[5]*(fGreen[i][jm5] - fGreen[i][jp5]) +
								fgD_[4]*(fGreen[i][jm4] - fGreen[i][jp4]) +
								fgD_[3]*(fGreen[i][jm3] - fGreen[i][jp3]) +
								fgD_[2]*(fGreen[i][jm2] - fGreen[i][jp2]) +
								fgD_[1]*(fGreen[i][jm1] - fGreen[i][jp1]);

			blueTemp[i][j] = 	fgD_[6]*(fBlue[i][jm6] - fBlue[i][jp6]) +
								fgD_[5]*(fBlue[i][jm5] - fBlue[i][jp5]) +
								fgD_[4]*(fBlue[i][jm4] - fBlue[i][jp4]) +
								fgD_[3]*(fBlue[i][jm3] - fBlue[i][jp3]) +
								fgD_[2]*(fBlue[i][jm2] - fBlue[i][jp2]) +
								fgD_[1]*(fBlue[i][jm1] - fBlue[i][jp1]);
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm6 = jm5;
        jm5 = jm4;
        jm4 = jm3;
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3 = jp4;
        jp4 = jp5;
        jp5 = jp6;
        jp6++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_13x13_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoF_13x13_Y_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM6 = iLow - 6, iHighP6 = iHigh + 6;
    int 	jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3, jm4 = jLow-4, jm5 = jLow-5, jm6 = jLow-6; 
    jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3, jp4 = jLow+4, jp5 = jLow+5, jp6 = jLow+6; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowM6; i<=iHighP6; i++)
        {
			redTemp[i][j] = 	fg_[6]*(fRed[i][jm6] + fRed[i][jp6]) +
								fg_[5]*(fRed[i][jm5] + fRed[i][jp5]) +
								fg_[4]*(fRed[i][jm4] + fRed[i][jp4]) +
								fg_[3]*(fRed[i][jm3] + fRed[i][jp3]) +
								fg_[2]*(fRed[i][jm2] + fRed[i][jp2]) +
								fg_[1]*(fRed[i][jm1] + fRed[i][jp1]) +
								fg_[0]* fRed[i][j];

			greenTemp[i][j] = 	fg_[6]*(fGreen[i][jm6] + fGreen[i][jp6]) +
								fg_[5]*(fGreen[i][jm5] + fGreen[i][jp5]) +
								fg_[4]*(fGreen[i][jm4] + fGreen[i][jp4]) +
								fg_[3]*(fGreen[i][jm3] + fGreen[i][jp3]) +
								fg_[2]*(fGreen[i][jm2] + fGreen[i][jp2]) +
								fg_[1]*(fGreen[i][jm1] + fGreen[i][jp1]) +
								fg_[0]* fGreen[i][j];

			blueTemp[i][j] = 	fg_[6]*(fBlue[i][jm6] + fBlue[i][jp6]) +
								fg_[5]*(fBlue[i][jm5] + fBlue[i][jp5]) +
								fg_[4]*(fBlue[i][jm4] + fBlue[i][jp4]) +
								fg_[3]*(fBlue[i][jm3] + fBlue[i][jp3]) +
								fg_[2]*(fBlue[i][jm2] + fBlue[i][jp2]) +
								fg_[1]*(fBlue[i][jm1] + fBlue[i][jp1]) +
								fg_[0]* fBlue[i][j];
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm6 = jm5;
        jm5 = jm4;
        jm4 = jm3;
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3 = jp4;
        jp4 = jp5;
        jp5 = jp6;
        jp6++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_13x13_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyInto_13x13_X_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx)
{
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3, im4 = iLow-4, im5 = iLow-5, im6 = iLow-6; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3, ip4 = iLow+4, ip5 = iLow+5, ip6 = iLow+6; 
    
    //-------------------------------------------------------
    //	Apply the 1D filter along the y direction
    //-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
			const   float   fgScaled[7] = {fg_[0]/3.f, fg_[1]/3.f, fg_[2]/3.f, fg_[3]/3.f, fg_[4]/3.f,
			                               fg_[5]/3.f, fg_[6]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvx[i][j] =   fgScaled[6]*(redTemp[im6][j] + redTemp[ip6][j] +
                			                   greenTemp[im6][j] + greenTemp[ip6][j] +
                			                   blueTemp[im6][j] + blueTemp[ip6][j]) +
        			              fgScaled[5]*(redTemp[im5][j] + redTemp[ip5][j] +
                			                   greenTemp[im5][j] + greenTemp[ip5][j] +
                			                   blueTemp[im5][j] + blueTemp[ip5][j]) +
        			              fgScaled[4]*(redTemp[im4][j] + redTemp[ip4][j] +
                			                   greenTemp[im4][j] + greenTemp[ip4][j] +
                			                   blueTemp[im4][j] + blueTemp[ip4][j]) +
        			              fgScaled[3]*(redTemp[im3][j] + redTemp[ip3][j] +
                			                   greenTemp[im3][j] + greenTemp[ip3][j] +
                			                   blueTemp[im3][j] + blueTemp[ip3][j]) +
        			              fgScaled[2]*(redTemp[im2][j] + redTemp[ip2][j] +
                			                   greenTemp[im2][j] + greenTemp[ip2][j] +
                			                   blueTemp[im2][j] + blueTemp[ip2][j]) +
        			              fgScaled[1]*(redTemp[im1][j] + redTemp[ip1][j] +
                			                   greenTemp[im1][j] + greenTemp[ip1][j] +
                			                   blueTemp[im1][j] + blueTemp[ip1][j]) +
        						  fgScaled[0]* (redTemp[i][j] + greenTemp[i][j] + blueTemp[i][j]);
        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
        		im6 = im5;
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5 = ip6;
    			ip6++;
        	}
            break;
        }
            
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);

        	//-------------------------------------------------------
        	//	then apply the 1D filter along the y direction
        	//-------------------------------------------------------
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			altRedTemp[i][j] =      fg_[6]*(redTemp[im6][j] + redTemp[ip6][j]) +
								            fg_[5]*(redTemp[im5][j] + redTemp[ip5][j]) +
            								fg_[4]*(redTemp[im4][j] + redTemp[ip4][j]) +
            								fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
            								fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
            								fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
            								fg_[0]* redTemp[i][j];

        			altGreenTemp[i][j] =    fg_[6]*(greenTemp[im6][j] + greenTemp[ip6][j]) +
                                            fg_[5]*(greenTemp[im5][j] + greenTemp[ip5][j]) +
            								fg_[4]*(greenTemp[im4][j] + greenTemp[ip4][j]) +
            								fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
            								fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
            								fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
            								fg_[0]* greenTemp[i][j];

        			altBlueTemp[i][j]  =    fg_[6]*(blueTemp[im6][j] + blueTemp[ip6][j]) +
        			                        fg_[5]*(blueTemp[im5][j] + blueTemp[ip5][j]) +
            								fg_[4]*(blueTemp[im4][j] + blueTemp[ip4][j]) +
            								fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
            								fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
            								fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
            								fg_[0]* blueTemp[i][j];

        		}

        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
        		im6 = im5;
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5 = ip6;
    			ip6++;
        	}
            break;
        }
                        
        //  this should never happen
        default:
            break;
            
    }
}


void ColorImageGradient_Gaussian_F::applyInto_13x13_Y_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx, float*const* fvy)
{
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3, im4 = iLow-4, im5 = iLow-5, im6 = iLow-6; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3, ip4 = iLow+4, ip5 = iLow+5, ip6 = iLow+6; 
    float   fvxTemp, fvyTemp;        

	//-------------------------------------------------------
	//	Apply the 1D filter along the y direction
	//-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
            const   float   fgDScaled[7] = {0, fgD_[1]/3.f, fgD_[2]/3.f, fgD_[3]/3.f, fgD_[4]/3.f,
                                            fgD_[5]/3.f, fgD_[6]/3.f};

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvy[i][j] = fgDScaled[6]*(redTemp[im6][j] - redTemp[ip6][j] +
								              greenTemp[im6][j] - greenTemp[ip6][j] +
    								          blueTemp[im6][j] - blueTemp[ip6][j]) +
								fgDScaled[5]*(redTemp[im5][j] - redTemp[ip5][j] +
								              greenTemp[im5][j] - greenTemp[ip5][j] +
    								          blueTemp[im5][j] - blueTemp[ip5][j]) +
								fgDScaled[4]*(redTemp[im4][j] - redTemp[ip4][j] +
								              greenTemp[im3][j] - greenTemp[ip4][j] +
								              blueTemp[im4][j] - blueTemp[ip4][j]) +
								fgDScaled[3]*(redTemp[im3][j] - redTemp[ip3][j] +
								              greenTemp[im3][j] - greenTemp[ip3][j] +
								              blueTemp[im3][j] - blueTemp[ip3][j]) +
								fgDScaled[2]*(redTemp[im2][j] - redTemp[ip2][j] +
								              greenTemp[im2][j] - greenTemp[ip2][j] +
								              blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgDScaled[1]*(redTemp[im1][j] - redTemp[ip1][j] +
								              greenTemp[im1][j] - greenTemp[ip1][j] +
								              blueTemp[im1][j] - blueTemp[ip1][j]);

        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
        		im6 = im5;
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5 = ip6;
    			ip6++;
        	}
        	break;
        }
        
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = altRedTemp[i][j];
                    fvyTemp =   fgD_[6]*(redTemp[im6][j] - redTemp[ip6][j]) +
								fgD_[5]*(redTemp[im5][j] - redTemp[ip5][j]) +
								fgD_[4]*(redTemp[im4][j] - redTemp[ip4][j]) +
								fgD_[3]*(redTemp[im3][j] - redTemp[ip3][j]) +
								fgD_[2]*(redTemp[im2][j] - redTemp[ip2][j]) +
								fgD_[1]*(redTemp[im1][j] - redTemp[ip1][j]);
                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;

                    //  green component
                    fvxComp = altGreenTemp[i][j];
        			fvyComp =   fgD_[6]*(greenTemp[im6][j] - greenTemp[ip6][j]) +
								fgD_[5]*(greenTemp[im5][j] - greenTemp[ip5][j]) +
								fgD_[4]*(greenTemp[im4][j] - greenTemp[ip4][j]) +
								fgD_[3]*(greenTemp[im3][j] - greenTemp[ip3][j]) +
								fgD_[2]*(greenTemp[im2][j] - greenTemp[ip2][j]) +
								fgD_[1]*(greenTemp[im1][j] - greenTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = altBlueTemp[i][j];
        			fvyComp =   fgD_[6]*(blueTemp[im6][j] - blueTemp[ip6][j]) +
								fgD_[5]*(blueTemp[im5][j] - blueTemp[ip5][j]) +
								fgD_[4]*(blueTemp[im4][j] - blueTemp[ip4][j]) +
								fgD_[3]*(blueTemp[im3][j] - blueTemp[ip3][j]) +
								fgD_[2]*(blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgD_[1]*(blueTemp[im1][j] - blueTemp[ip1][j]);
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvx[i][j] = fvxComp;
                        fvy[i][j] = fvyComp;
                    }
                    else
                    {
                        fvx[i][j] = fvxTemp;
                        fvy[i][j] = fvyTemp;
                    }
                }

                //  shift the i indices (ip1 is taken care of in the loop statement)
        		im6 = im5;
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5 = ip6;
    			ip6++;
            }
           break;
        }
        
        //  this should never happen
        default:
            break;
            
    }
}


#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Private methods that implement the generic convolution (widht larger than 13)
//-------------------------------------------------------------------------------


void ColorImageGradient_Gaussian_F::applyIntoI_X_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int   m = getWidth()/2;
    const int	iLowMm = iLow - m, iHighPm = iHigh + m;
    //
    int 	fourjR, fourjG, fourjB,         //  4*j     red green blue
            fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
            fourjm2R, fourjm2G, fourjm2B,   //  4*(j-2) red green blue
            fourjm3R, fourjm3G, fourjm3B,   //  4*(j-3) red green blue
            fourjm4R, fourjm4G, fourjm4B,   //  4*(j-4) red green blue
            fourjm5R, fourjm5G, fourjm5B,   //  4*(j-5) red green blue
            fourjm6R, fourjm6G, fourjm6B,   //  4*(j-6) red green blue
            fourjp1R, fourjp1G, fourjp1B,   //  4*(j+1) red green blue
            fourjp2R, fourjp2G, fourjp2B,   //  4*(j+2) red green blue
            fourjp3R, fourjp3G, fourjp3B,   //  4*(j+3) red green blue
            fourjp4R, fourjp4G, fourjp4B,   //  4*(j+4) red green blue
            fourjp5R, fourjp5G, fourjp5B,   //  4*(j+5) red green blue
            fourjp6R, fourjp6G, fourjp6B;   //  4*(j+6) red green blue
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    fourjm6R = 4*(jLow-6);
    fourjm6G = fourjm6R + 1;
    fourjm6B = fourjm6G + 1;
    fourjm5R = 4*(jLow-5);
    fourjm5G = fourjm5R + 1;
    fourjm5B = fourjm5G + 1;
    fourjm4R = 4*(jLow-4);
    fourjm4G = fourjm4R + 1;
    fourjm4B = fourjm4G + 1;
    fourjm3R = 4*(jLow-3);
    fourjm3G = fourjm3R + 1;
    fourjm3B = fourjm3G + 1;
    fourjm2R = 4*(jLow-2);
    fourjm2G = fourjm2R + 1;
    fourjm2B = fourjm2G + 1;
    fourjm1R = 4*(jLow-1);
    fourjm1G = fourjm1R + 1;
    fourjm1B = fourjm1G + 1;
    fourjR   = 4*jLow;
    fourjG   = fourjR + 1;
    fourjB   = fourjG + 1;
    fourjp1R = 4*(jLow+1);
    fourjp1G = fourjp1R + 1;
    fourjp1B = fourjp1G + 1;
    fourjp2R = 4*(jLow+2);
    fourjp2G = fourjp2R + 1;
    fourjp2B = fourjp2G + 1;
    fourjp3R = 4*(jLow+3);
    fourjp3G = fourjp3R + 1;
    fourjp3B = fourjp3G + 1;
    fourjp4R = 4*(jLow+4);
    fourjp4G = fourjp4R + 1;
    fourjp4B = fourjp4G + 1;
    fourjp5R = 4*(jLow+5);
    fourjp5G = fourjp5R + 1;
    fourjp5B = fourjp5G + 1;
    fourjp6R = 4*(jLow+6);
    fourjp6G = fourjp6R + 1;
    fourjp6B = fourjp6G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowMm; i<=iHighPm; i++)
        {
			redTemp[i][j] = 	fgD_[6]*(rgba[i][fourjm6R] - rgba[i][fourjp6R]) +
								fgD_[5]*(rgba[i][fourjm5R] - rgba[i][fourjp5R]) +
								fgD_[4]*(rgba[i][fourjm4R] - rgba[i][fourjp4R]) +
								fgD_[3]*(rgba[i][fourjm3R] - rgba[i][fourjp3R]) +
								fgD_[2]*(rgba[i][fourjm2R] - rgba[i][fourjp2R]) +
								fgD_[1]*(rgba[i][fourjm1R] - rgba[i][fourjp1R]);
								
			greenTemp[i][j] = 	fgD_[6]*(rgba[i][fourjm6G] - rgba[i][fourjp6G]) +
								fgD_[5]*(rgba[i][fourjm5G] - rgba[i][fourjp5G]) +
								fgD_[4]*(rgba[i][fourjm4G] - rgba[i][fourjp4G]) +
								fgD_[3]*(rgba[i][fourjm3G] - rgba[i][fourjp3G]) +
								fgD_[2]*(rgba[i][fourjm2G] - rgba[i][fourjp2G]) +
								fgD_[1]*(rgba[i][fourjm1G] - rgba[i][fourjp1G]);
								
			blueTemp[i][j] = 	fgD_[6]*(rgba[i][fourjm6B] - rgba[i][fourjp6B]) +
								fgD_[5]*(rgba[i][fourjm5B] - rgba[i][fourjp5B]) +
								fgD_[4]*(rgba[i][fourjm4B] - rgba[i][fourjp4B]) +
								fgD_[3]*(rgba[i][fourjm3B] - rgba[i][fourjp3B]) +
								fgD_[2]*(rgba[i][fourjm2B] - rgba[i][fourjp2B]) +
								fgD_[1]*(rgba[i][fourjm1B] - rgba[i][fourjp1B]);

			for (int l=kMaxHardCodedWidth+1, /* fourl=4*(kMaxHardCodedWidth+1), unused var. -CJC */
			         fourjml = fourjR - 4*(kMaxHardCodedWidth+1),
			         fourjpl = fourjR + 4*(kMaxHardCodedWidth+1); l<=m; l++, fourjml--, fourjpl++)
            {
				redTemp[i][j]   += fgD_[l] * (rgba[i][fourjml--] + 
                                             rgba[i][fourjpl++]);
				greenTemp[i][j] += fgD_[l] * (rgba[i][fourjml--] + 
                                             rgba[i][fourjpl++]);
				blueTemp[i][j]  += fgD_[l] * (rgba[i][fourjml--] + 
                                             rgba[i][fourjpl++]);
            }
        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm6R = fourjm5R;
        fourjm6G = fourjm5G;
        fourjm6B = fourjm5B;
        fourjm5R = fourjm4R;
        fourjm5G = fourjm4G;
        fourjm5B = fourjm4B;
        fourjm4R = fourjm3R;
        fourjm4G = fourjm3G;
        fourjm4B = fourjm3B;
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R  = fourjp4R;
        fourjp3G  = fourjp4G;
        fourjp3B  = fourjp4B;
        fourjp4R  = fourjp5R;
        fourjp4G  = fourjp5G;
        fourjp4B  = fourjp5B;
        fourjp5R  = fourjp6R;
        fourjp5G  = fourjp6G;
        fourjp5B  = fourjp6B;
        fourjp6R += 4;
        fourjp6G += 4;
        fourjp6B += 4;
	}

    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoI_Y_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int   m = getWidth()/2;
    const int	iLowMm = iLow - m, iHighPm = iHigh + m;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int fourjm6R = 4*(jLow-6);      //  4*(j-6) red green blue
    int fourjm6G = fourjm6R + 1;
    int fourjm6B = fourjm6G + 1;
    int fourjm5R = 4*(jLow-5);      //  4*(j-5) red green blue
    int fourjm5G = fourjm5R + 1;
    int fourjm5B = fourjm5G + 1;
    int fourjm4R = 4*(jLow-4);      //  4*(j-4) red green blue
    int fourjm4G = fourjm4R + 1;
    int fourjm4B = fourjm4G + 1;
    int fourjm3R = 4*(jLow-3);      //  4*(j-3) red green blue
    int fourjm3G = fourjm3R + 1;
    int fourjm3B = fourjm3G + 1;
    int fourjm2R = 4*(jLow-2);      //  4*(j-2) red green blue
    int fourjm2G = fourjm2R + 1;
    int fourjm2B = fourjm2G + 1;
    int fourjm1R = 4*(jLow-1);      //  4*(j-1) red green blue
    int fourjm1G = fourjm1R + 1;
    int fourjm1B = fourjm1G + 1;
    int fourjR   = 4*jLow;          //  4*j     red green blue
    int fourjG   = fourjR + 1;
    int fourjB   = fourjG + 1;
    int fourjp1R = 4*(jLow+1);      //  4*(j+1) red green blue
    int fourjp1G = fourjp1R + 1;
    int fourjp1B = fourjp1G + 1;
    int fourjp2R = 4*(jLow+2);      //  4*(j+2) red green blue
    int fourjp2G = fourjp2R + 1;
    int fourjp2B = fourjp2G + 1;
    int fourjp3R = 4*(jLow+3);      //  4*(j+3) red green blue
    int fourjp3G = fourjp3R + 1;
    int fourjp3B = fourjp3G + 1;
    int fourjp4R = 4*(jLow+4);      //  4*(j+4) red green blue
    int fourjp4G = fourjp4R + 1;
    int fourjp4B = fourjp4G + 1;
    int fourjp5R = 4*(jLow+5);      //  4*(j+5) red green blue
    int fourjp5G = fourjp5R + 1;
    int fourjp5B = fourjp5G + 1;
    int fourjp6R = 4*(jLow+6);      //  4*(j+6) red green blue
    int fourjp6G = fourjp6R + 1;
    int fourjp6B = fourjp6G + 1;
    //
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowMm; i<=iHighPm; i++)
        {
			redTemp[i][j] = 	fg_[6]*(rgba[i][fourjm6R] + rgba[i][fourjp6R]) +
								fg_[5]*(rgba[i][fourjm5R] + rgba[i][fourjp5R]) +
								fg_[4]*(rgba[i][fourjm4R] + rgba[i][fourjp4R]) +
								fg_[3]*(rgba[i][fourjm3R] + rgba[i][fourjp3R]) +
								fg_[2]*(rgba[i][fourjm2R] + rgba[i][fourjp2R]) +
								fg_[1]*(rgba[i][fourjm1R] + rgba[i][fourjp1R]) +
								fg_[0]* rgba[i][fourjR];

			greenTemp[i][j] = 	fg_[6]*(rgba[i][fourjm6G] + rgba[i][fourjp6G]) +
								fg_[5]*(rgba[i][fourjm5G] + rgba[i][fourjp5G]) +
								fg_[4]*(rgba[i][fourjm4G] + rgba[i][fourjp4G]) +
								fg_[3]*(rgba[i][fourjm3G] + rgba[i][fourjp3G]) +
								fg_[2]*(rgba[i][fourjm2G] + rgba[i][fourjp2G]) +
								fg_[1]*(rgba[i][fourjm1G] + rgba[i][fourjp1G]) +
								fg_[0]* rgba[i][fourjG];
								
			blueTemp[i][j] = 	fg_[6]*(rgba[i][fourjm6B] + rgba[i][fourjp6B]) +
								fg_[5]*(rgba[i][fourjm5B] + rgba[i][fourjp5B]) +
								fg_[4]*(rgba[i][fourjm4B] + rgba[i][fourjp4B]) +
								fg_[3]*(rgba[i][fourjm3B] + rgba[i][fourjp3B]) +
								fg_[2]*(rgba[i][fourjm2B] + rgba[i][fourjp2B]) +
								fg_[1]*(rgba[i][fourjm1B] + rgba[i][fourjp1B]) +
								fg_[0]* rgba[i][fourjB];
								

			for (int l=kMaxHardCodedWidth+1, /*fourl=4*(kMaxHardCodedWidth+1), unused var. -CJC */
			         fourjml = fourjR - 4*(kMaxHardCodedWidth+1),
			         fourjpl = fourjR + 4*(kMaxHardCodedWidth+1); l<=m; l++, fourjml--, fourjpl++)
            {
				redTemp[i][j]   += fg_[l] * (rgba[i][fourjml--] + rgba[i][fourjpl++]);
				greenTemp[i][j] += fg_[l] * (rgba[i][fourjml--] + rgba[i][fourjpl++]);
				blueTemp[i][j]  += fg_[l] * (rgba[i][fourjml--] + rgba[i][fourjpl++]);
            }
        }
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        fourjm6R = fourjm5R;
        fourjm6G = fourjm5G;
        fourjm6B = fourjm5B;
        fourjm5R = fourjm4R;
        fourjm5G = fourjm4G;
        fourjm5B = fourjm4B;
        fourjm4R = fourjm3R;
        fourjm4G = fourjm3G;
        fourjm4B = fourjm3B;
        fourjm3R = fourjm2R;
        fourjm3G = fourjm2G;
        fourjm3B = fourjm2B;
        fourjm2R = fourjm1R;
        fourjm2G = fourjm1G;
        fourjm2B = fourjm1B;
        fourjm1R = fourjR;
        fourjm1G = fourjG;
        fourjm1B = fourjB;
        fourjR = fourjp1R;
        fourjG = fourjp1G;
        fourjB = fourjp1B;
        fourjp1R  = fourjp2R;
        fourjp1G  = fourjp2G;
        fourjp1B  = fourjp2B;
        fourjp2R  = fourjp3R;
        fourjp2G  = fourjp3G;
        fourjp2B  = fourjp3B;
        fourjp3R  = fourjp4R;
        fourjp3G  = fourjp4G;
        fourjp3B  = fourjp4B;
        fourjp4R  = fourjp5R;
        fourjp4G  = fourjp5G;
        fourjp4B  = fourjp5B;
        fourjp5R  = fourjp6R;
        fourjp5G  = fourjp6G;
        fourjp5B  = fourjp6B;
        fourjp6R += 4;
        fourjp6G += 4;
        fourjp6B += 4;
	}		
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyIntoF_X_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int   m = getWidth()/2;
    const int	iLowMm = iLow - m, iHighPm = iHigh + m;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int jm1 = jLow-1, jm2 = jLow-2, jm3 = jLow-3, jm4 = jLow-4, jm5 = jLow-5, jm6 = jLow-6; 
    int jp1 = jLow+1, jp2 = jLow+2, jp3 = jLow+3, jp4 = jLow+4, jp5 = jLow+5, jp6 = jLow+6; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowMm; i<=iHighPm; i++)
        {
			redTemp[i][j] = 	fgD_[6]*(fRed[i][jm6] - fRed[i][jp6]) +
								fgD_[5]*(fRed[i][jm5] - fRed[i][jp5]) +
								fgD_[4]*(fRed[i][jm4] - fRed[i][jp4]) +
								fgD_[3]*(fRed[i][jm3] - fRed[i][jp3]) +
								fgD_[2]*(fRed[i][jm2] - fRed[i][jp2]) +
								fgD_[1]*(fRed[i][jm1] - fRed[i][jp1]);

			greenTemp[i][j] = 	fgD_[6]*(fGreen[i][jm6] - fGreen[i][jp6]) +
								fgD_[5]*(fGreen[i][jm5] - fGreen[i][jp5]) +
								fgD_[4]*(fGreen[i][jm4] - fGreen[i][jp4]) +
								fgD_[3]*(fGreen[i][jm3] - fGreen[i][jp3]) +
								fgD_[2]*(fGreen[i][jm2] - fGreen[i][jp2]) +
								fgD_[1]*(fGreen[i][jm1] - fGreen[i][jp1]);

			blueTemp[i][j] = 	fgD_[6]*(fBlue[i][jm6] - fBlue[i][jp6]) +
								fgD_[5]*(fBlue[i][jm5] - fBlue[i][jp5]) +
								fgD_[4]*(fBlue[i][jm4] - fBlue[i][jp4]) +
								fgD_[3]*(fBlue[i][jm3] - fBlue[i][jp3]) +
								fgD_[2]*(fBlue[i][jm2] - fBlue[i][jp2]) +
								fgD_[1]*(fBlue[i][jm1] - fBlue[i][jp1]);

			for (int l= kMaxHardCodedWidth+1, jml=j-l, jpl=j+l; l<=m; l++, jpl++, jml--)
            {
				redTemp[i][j] += fgD_[l] * (fRed[i][jml] + fRed[i][jpl]);
				greenTemp[i][j] += fgD_[l] * (fGreen[i][jml] + fGreen[i][jpl]);
				blueTemp[i][j] += fgD_[l] * (fBlue[i][jml] + fBlue[i][jpl]);
            }
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm6 = jm5;
        jm5 = jm4;
        jm4 = jm3;
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3 = jp4;
        jp4 = jp5;
        jp5 = jp6;
        jp6++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_X_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx);
}

void ColorImageGradient_Gaussian_F::applyIntoF_Y_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                                float*const* fvx, float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const float*const* fRed = imgIn->getShiftedRedF2D();
    const float*const* fGreen = imgIn->getShiftedGreenF2D();
    const float*const* fBlue = imgIn->getShiftedBlueF2D();
    float* const* redTemp = getShiftedTempRedF2D_(R_W_ACCESS);
    float* const* greenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
    float* const* blueTemp = getShiftedTempBlueF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int   m = getWidth()/2;
    const int	iLowMm = iLow - m, iHighPm = iHigh + m;
    
	//-------------------------------------------------------
	//	first apply the 1D filter along the x direction
	//-------------------------------------------------------
	//	Note that we have to apply this filter out of the destination rect since
	//	some of these elements will be needed we we apply a filter along the
	//	other direction.
    int jm1 = jLow-1, jm2 = jLow-2, jm3 = jLow-3, jm4 = jLow-4, jm5 = jLow-5, jm6 = jLow-6; 
    int jp1 = jLow+1, jp2 = jLow+2, jp3 = jLow+3, jp4 = jLow+4, jp5 = jLow+5, jp6 = jLow+6; 
    for (int j=jLow; j<=jHigh; j++)
    {
        for (int i=iLowMm; i<=iHighPm; i++)
        {
			redTemp[i][j] = 	fg_[6]*(fRed[i][jm6] + fRed[i][jp6]) +
								fg_[5]*(fRed[i][jm5] + fRed[i][jp5]) +
								fg_[4]*(fRed[i][jm4] + fRed[i][jp4]) +
								fg_[3]*(fRed[i][jm3] + fRed[i][jp3]) +
								fg_[2]*(fRed[i][jm2] + fRed[i][jp2]) +
								fg_[1]*(fRed[i][jm1] + fRed[i][jp1]) +
								fg_[0]* fRed[i][j];

			greenTemp[i][j] = 	fg_[6]*(fGreen[i][jm6] + fGreen[i][jp6]) +
								fg_[5]*(fGreen[i][jm5] + fGreen[i][jp5]) +
								fg_[4]*(fGreen[i][jm4] + fGreen[i][jp4]) +
								fg_[3]*(fGreen[i][jm3] + fGreen[i][jp3]) +
								fg_[2]*(fGreen[i][jm2] + fGreen[i][jp2]) +
								fg_[1]*(fGreen[i][jm1] + fGreen[i][jp1]) +
								fg_[0]* fGreen[i][j];

			blueTemp[i][j] = 	fg_[6]*(fBlue[i][jm6] + fBlue[i][jp6]) +
								fg_[5]*(fBlue[i][jm5] + fBlue[i][jp5]) +
								fg_[4]*(fBlue[i][jm4] + fBlue[i][jp4]) +
								fg_[3]*(fBlue[i][jm3] + fBlue[i][jp3]) +
								fg_[2]*(fBlue[i][jm2] + fBlue[i][jp2]) +
								fg_[1]*(fBlue[i][jm1] + fBlue[i][jp1]) +
								fg_[0]* fBlue[i][j];


            for (int k= kMaxHardCodedWidth+1, imk=i-k, ipk=i+k; k<=m; k++, ipk++, imk--)
            {
                redTemp[i][j]   += fg_[k] * (fRed[imk][j] + fRed[ipk][j] );
                greenTemp[i][j] += fg_[k] * (fGreen[imk][j] + fGreen[ipk][j] );
                blueTemp[i][j]  += fg_[k] * (fBlue[imk][j] + fBlue[ipk][j] );
            }
		}		
        //  shift the j indexes (j itself is taken care of in the loop
        //  statement)
        jm6 = jm5;
        jm5 = jm4;
        jm4 = jm3;
        jm3 = jm2;
        jm2 = jm1;
        jm1 = j;
        jp1 = jp2;
        jp2 = jp3;
        jp3 = jp4;
        jp4 = jp5;
        jp5 = jp6;
        jp6++;
	}
		
    //-------------------------------------------------------
    //	then apply the 1D filter along the y direction
    //-------------------------------------------------------
    applyInto_Y_(iLow, iHigh, jLow, jHigh, redTemp, greenTemp, blueTemp, fvx, fvy);
}

void ColorImageGradient_Gaussian_F::applyInto_X_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx)
{
    const int   m = getWidth()/2;
    //
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3, im4 = iLow-4, im5 = iLow-5, im6 = iLow-6; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3, ip4 = iLow+4, ip5 = iLow+5, ip6 = iLow+6; 
    
    //-------------------------------------------------------
    //	Apply the 1D filter along the y direction
    //-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
            float   fvxTemp;        
            const   float   ONE_THIRD = 1.f/3.f;

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvxTemp =     fg_[6]*(redTemp[im6][j] + redTemp[ip6][j] +
                			              greenTemp[im6][j] + greenTemp[ip6][j] +
                			              blueTemp[im6][j] + blueTemp[ip6][j]) +
        			              fg_[5]*(redTemp[im5][j] + redTemp[ip5][j] +
                			              greenTemp[im5][j] + greenTemp[ip5][j] +
                			              blueTemp[im5][j] + blueTemp[ip5][j]) +
        			              fg_[4]*(redTemp[im4][j] + redTemp[ip4][j] +
                			             greenTemp[im4][j] + greenTemp[ip4][j] +
                			             blueTemp[im4][j] + blueTemp[ip4][j]) +
        			              fg_[3]*(redTemp[im3][j] + redTemp[ip3][j] +
                			              greenTemp[im3][j] + greenTemp[ip3][j] +
                			              blueTemp[im3][j] + blueTemp[ip3][j]) +
        			              fg_[2]*(redTemp[im2][j] + redTemp[ip2][j] +
                			              greenTemp[im2][j] + greenTemp[ip2][j] +
                			              blueTemp[im2][j] + blueTemp[ip2][j]) +
        			              fg_[1]*(redTemp[im1][j] + redTemp[ip1][j] +
                			              greenTemp[im1][j] + greenTemp[ip1][j] +
                			              blueTemp[im1][j] + blueTemp[ip1][j]) +
        						  fg_[0]* (redTemp[i][j] + greenTemp[i][j] + blueTemp[i][j]);


                    for (int k= kMaxHardCodedWidth+1, imk=i-k, ipk=i+k; k<=m; k++, ipk++, imk--)
                    {
                        fvxTemp   += fg_[k] * (redTemp[imk][j] + redTemp[ipk][j] +
                			                   greenTemp[imk][j] + greenTemp[ipk][j] +
                			                   blueTemp[imk][j] + blueTemp[ipk][j]);
                    }
                    
                    fvx[i][j] = fvxTemp * ONE_THIRD;
        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
        		im6 = im5;
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5 = ip6;
    			ip6++;
        	}
            break;
        }
            
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);

        	//-------------------------------------------------------
        	//	then apply the 1D filter along the y direction
        	//-------------------------------------------------------
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			altRedTemp[i][j] =      fg_[6]*(redTemp[im6][j] + redTemp[ip6][j]) +
								            fg_[5]*(redTemp[im5][j] + redTemp[ip5][j]) +
            								fg_[4]*(redTemp[im4][j] + redTemp[ip4][j]) +
            								fg_[3]*(redTemp[im3][j] + redTemp[ip3][j]) +
            								fg_[2]*(redTemp[im2][j] + redTemp[ip2][j]) +
            								fg_[1]*(redTemp[im1][j] + redTemp[ip1][j]) +
            								fg_[0]* redTemp[i][j];

        			altGreenTemp[i][j] =    fg_[6]*(greenTemp[im6][j] + greenTemp[ip6][j]) +
                                            fg_[5]*(greenTemp[im5][j] + greenTemp[ip5][j]) +
            								fg_[4]*(greenTemp[im4][j] + greenTemp[ip4][j]) +
            								fg_[3]*(greenTemp[im3][j] + greenTemp[ip3][j]) +
            								fg_[2]*(greenTemp[im2][j] + greenTemp[ip2][j]) +
            								fg_[1]*(greenTemp[im1][j] + greenTemp[ip1][j]) +
            								fg_[0]* greenTemp[i][j];

        			altBlueTemp[i][j]  =    fg_[6]*(blueTemp[im6][j] + blueTemp[ip6][j]) +
        			                        fg_[5]*(blueTemp[im5][j] + blueTemp[ip5][j]) +
            								fg_[4]*(blueTemp[im4][j] + blueTemp[ip4][j]) +
            								fg_[3]*(blueTemp[im3][j] + blueTemp[ip3][j]) +
            								fg_[2]*(blueTemp[im2][j] + blueTemp[ip2][j]) +
            								fg_[1]*(blueTemp[im1][j] + blueTemp[ip1][j]) +
            								fg_[0]* blueTemp[i][j];

                    for (int k= kMaxHardCodedWidth+1, imk=i-k, ipk=i+k; k<=m; k++, ipk++, imk--)
                    {
                        altRedTemp[i][j]   += fg_[k] * (redTemp[imk][j] + redTemp[ipk][j]);
                        altGreenTemp[i][j] += fg_[k] * (greenTemp[imk][j] + greenTemp[ipk][j]);
                		altBlueTemp[i][j]  += fg_[k] * (blueTemp[imk][j] + blueTemp[ipk][j]);
                    }
        		}

        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
        		im6 = im5;
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5 = ip6;
    			ip6++;
        	}
            break;
        }
                        
        //  this should never happen
        default:
            break;
            
    }
}


void ColorImageGradient_Gaussian_F::applyInto_Y_(int iLow, int iHigh, int jLow, 
                                                        int jHigh, float*const* redTemp,
                                                        float*const* greenTemp, float*const* blueTemp, 
                                                        float*const* fvx, float*const* fvy)
{
    const int   m = getWidth()/2;
    //
    int im1 = iLow-1, im2 = iLow-2, im3 = iLow-3, im4 = iLow-4, im5 = iLow-5, im6 = iLow-6; 
	int ip1 = iLow+1, ip2 = iLow+2, ip3 = iLow+3, ip4 = iLow+4, ip5 = iLow+5, ip6 = iLow+6; 
    float   fvxTemp, fvyTemp;        

	//-------------------------------------------------------
	//	Apply the 1D filter along the y direction
	//-------------------------------------------------------
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
        {   
            const   float   ONE_THIRD = 1.f/3.f;

            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
        			fvyTemp =   fgD_[6]*(redTemp[im6][j] - redTemp[ip6][j] +
        			                     greenTemp[im6][j] - greenTemp[ip6][j] +
        			                     blueTemp[im6][j] - blueTemp[ip6][j]) +
								fgD_[5]*(redTemp[im5][j] - redTemp[ip5][j] +
								         greenTemp[im5][j] - greenTemp[ip5][j] +
								         blueTemp[im5][j] - blueTemp[ip5][j]) +
								fgD_[4]*(redTemp[im4][j] - redTemp[ip4][j] +
								         greenTemp[im3][j] - greenTemp[ip4][j] +
								         blueTemp[im4][j] - blueTemp[ip4][j]) +
								fgD_[3]*(redTemp[im3][j] - redTemp[ip3][j] +
								         greenTemp[im3][j] - greenTemp[ip3][j] +
								         blueTemp[im3][j] - blueTemp[ip3][j]) +
								fgD_[2]*(redTemp[im2][j] - redTemp[ip2][j] +
								         greenTemp[im2][j] - greenTemp[ip2][j] +
								         blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgD_[1]*(redTemp[im1][j] - redTemp[ip1][j] +
								         greenTemp[im1][j] - greenTemp[ip1][j] +
								         blueTemp[im1][j] - blueTemp[ip1][j]);

                    for (int k= kMaxHardCodedWidth+1, imk=i-k, ipk=i+k; k<=m; k++, ipk++, imk--)
                    {
                        fvyTemp   += fg_[k] * (redTemp[imk][j] - redTemp[ipk][j] +
                			                   greenTemp[imk][j] - greenTemp[ipk][j] +
                			                   blueTemp[imk][j] - blueTemp[ipk][j]);
                    }
                    
                    fvy[i][j] = ONE_THIRD * fvyTemp;
        		}
        		//  shift the i indexes (i itself is taken care of in the loop
        		//  statement)
        		im6 = im5;
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5 = ip6;
    			ip6++;
        	}
        	break;
        }
        
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
        {   
            //  altRedTemp, altGreenTemp, altBlueTemp store the gradient's x component
            //  for the corresponding color chanel.
            float* const* altRedTemp = getShiftedAltTempRedF2D_(R_W_ACCESS);
            float* const* altGreenTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float* const* altBlueTemp = getShiftedTempGreenF2D_(R_W_ACCESS);
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            for (int i=iLow; i<=iHigh; i++)
            {
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = altRedTemp[i][j];
                    fvyTemp =   fgD_[6]*(redTemp[im6][j] - redTemp[ip6][j]) +
								fgD_[5]*(redTemp[im5][j] - redTemp[ip5][j]) +
								fgD_[4]*(redTemp[im4][j] - redTemp[ip4][j]) +
								fgD_[3]*(redTemp[im3][j] - redTemp[ip3][j]) +
								fgD_[2]*(redTemp[im2][j] - redTemp[ip2][j]) +
								fgD_[1]*(redTemp[im1][j] - redTemp[ip1][j]);
                    for (int k= kMaxHardCodedWidth+1, imk=i-k, ipk=i+k; k<=m; k++, ipk++, imk--)
                        fvyTemp += fgD_[k]*(redTemp[imk][j] - redTemp[ipk][j]);
                        
                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;

                    //  green component
                    fvxComp = altGreenTemp[i][j];
        			fvyComp =   fgD_[6]*(greenTemp[im6][j] - greenTemp[ip6][j]) +
								fgD_[5]*(greenTemp[im5][j] - greenTemp[ip5][j]) +
								fgD_[4]*(greenTemp[im4][j] - greenTemp[ip4][j]) +
								fgD_[3]*(greenTemp[im3][j] - greenTemp[ip3][j]) +
								fgD_[2]*(greenTemp[im2][j] - greenTemp[ip2][j]) +
								fgD_[1]*(greenTemp[im1][j] - greenTemp[ip1][j]);
                    for (int k= kMaxHardCodedWidth+1, imk=i-k, ipk=i+k; k<=m; k++, ipk++, imk--)
                        fvyComp += fgD_[k]*(greenTemp[imk][j] - greenTemp[ipk][j]);

                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = altBlueTemp[i][j];
        			fvyComp =   fgD_[6]*(blueTemp[im6][j] - blueTemp[ip6][j]) +
								fgD_[5]*(blueTemp[im5][j] - blueTemp[ip5][j]) +
								fgD_[4]*(blueTemp[im4][j] - blueTemp[ip4][j]) +
								fgD_[3]*(blueTemp[im3][j] - blueTemp[ip3][j]) +
								fgD_[2]*(blueTemp[im2][j] - blueTemp[ip2][j]) +
								fgD_[1]*(blueTemp[im1][j] - blueTemp[ip1][j]);
                    for (int k= kMaxHardCodedWidth+1, imk=i-k, ipk=i+k; k<=m; k++, ipk++, imk--)
                        fvyComp += fgD_[k]*(blueTemp[imk][j] - blueTemp[ipk][j]);

                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvx[i][j] = fvxComp;
                        fvy[i][j] = fvyComp;
                    }
                    else
                    {
                        fvx[i][j] = fvxTemp;
                        fvy[i][j] = fvyTemp;
                    }
                }

                //  shift the i indices (ip1 is taken care of in the loop statement)
        		im6 = im5;
    			im5 = im4;
    			im4 = im3;
    			im3 = im2;
    			im2 = im1;
    			im1 = i;
    			ip1 = ip2;
    			ip2 = ip3;
    			ip3 = ip4;
    			ip4 = ip5;
    			ip5 = ip6;
    			ip6++;
            }
           break;
        }
        
        //  this should never happen
        default:
            break;
            
    }
}


#if 0
#pragma mark -
#endif

void ColorImageGradient_Gaussian_F::initializeFilter_(void)
{
    const int   m = getWidth()/2;
	const double	scale = getScale();
    const double	sigma2Scale = 1.L / (2.L * scale * scale),
					piSigma = 1.L / (sqrt(2*PI) * scale),
					piSigma2 = piSigma/scale;

    fg_ = new float [m+1];
    FAIL_CONDITION( fg_ == NULL,
                    kFilterAllocationError,
                    "filter allocation failed");
    fgD_ = new float [m+1];
    FAIL_CONDITION( fgD_ == NULL,
                    kFilterAllocationError,
                    "filter allocation failed");
    for (int r=0; r<=m; r++)
    {
    	//	fg_ is a regular 1D Gaussian
    	fg_[r] = (float) (piSigma * exp(-r*r*sigma2Scale));

    	//	fg1Dx_ is a first derivative of a 1D Gaussian
    	fgD_[r] = (float) (-r * piSigma2 * exp(-r*r*sigma2Scale));
	}
}


