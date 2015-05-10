/*  NAME:
        ColorImageGradient_Sobel.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorImageGradient_Sobel class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColorImageGradient_Sobel.h"
#include "VectorField_I.h"
#include "VectorField_F.h"

using namespace uriVL;

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif



ColorImageGradient_Sobel::ColorImageGradient_Sobel(void)
		try	:	ColorImageGradient(1, true)
{
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ColorImageGradient_Sobel constructor");
	throw e;
}


ColorImageGradient_Sobel::~ColorImageGradient_Sobel(void)
{
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters
//------------------------------------------------------
#endif


float ColorImageGradient_Sobel::getScale(void) const
{
    return 0.6f;
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif



VectorField *ColorImageGradient_Sobel::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");

    return applyTo(imgIn, imgIn->getValidRect());
}

VectorField *ColorImageGradient_Sobel::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "NULL ImageRect parameter in ColorImageGradient_Sobel::ApplyTo");

    ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
    usableRect->removeAllAround(1);
    ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION( destRect == NULL,
                    kNullRectangleError,
                    "Destination rectangle does not intersect valid data rectangle");

    VectorField	        *vectOut;
    if (imgIn->hasFloatRaster())
        vectOut = new VectorField_F(destRect);
    else
        vectOut = new VectorField_I(destRect);

    applyInto(imgIn, theRect, vectOut);

    delete destRect;
    delete usableRect;

    return vectOut;
}

void ColorImageGradient_Sobel::applyInto(const RasterImage* imgIn, VectorField* vectOut)
{
    FAIL_CONDITION(	imgIn == NULL,
                    kNullRasterImageError,
                    "NULL input raster image passed as parameter");
    FAIL_CONDITION( vectOut == NULL,
                    kNullVectorFieldError,
                    "null VectorField passed as destination to ColorImageGradient_Sobel::applyInto");

    applyInto(imgIn, imgIn->getValidRect(), vectOut);
}

void ColorImageGradient_Sobel::applyInto(const RasterImage* imgIn, const ImageRect* theRect,
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
                    "NULL ImageRect parameter in VectorField_F::copyInto");

    FAIL_CONDITION(	vectOut == NULL,
                    kNullVectorFieldError,
                    "NULL output vector field passed as parameter");


    //----------------------------------------------------------------
    //	STEP 1:		Test for rectangle intersection
    //----------------------------------------------------------------
    //  determine the output rectangle that the input image can produce.
    //		Here, because we apply a filter of width&height 3, we
    //		"lose 1 all around".
    ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
    usableRect->removeAllAround(1);
    //	... and determine its intersection with the user-specified output rectangle
    ImageRect	*destRect = ImageRect::intersection(theRect, usableRect);
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
    bool localRGBa;
	const RasterImage_RGBa* rgbaImg = RasterImage_RGBa::localRasterRGBa(imgIn, &localRGBa);


    //----------------------------------------------------------------
    //	STEP 3:		Perform the calculations
    //----------------------------------------------------------------
    //  There are 4 cases, depending on the presence of a float raster
    //  in the input and the destination
    if (rgbaImg->hasFloatRaster())
        if (vectOut->hasFloatRaster())
            applyIntoF_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, 
                        static_cast<VectorField_F*>(vectOut));
        else
            applyIntoF_(static_cast<const RasterImage_RGBa_F*>(rgbaImg), destRect, 
                        static_cast<VectorField_I*>(vectOut));
    else
        if (vectOut->hasFloatRaster())
            applyIntoI_(rgbaImg, destRect, static_cast<VectorField_F *>(vectOut));
        else
            applyIntoI_(rgbaImg, destRect, static_cast<VectorField_I*>(vectOut));


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
}

#if 0
#pragma mark -
#endif


void ColorImageGradient_Sobel::applyIntoF_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                     VectorField_F* vectOut)
{
    int iLow  = theRect->getTop(),
        iHigh = theRect->getBottom(),
        jLow = theRect->getLeft(),
        jHigh = theRect->getRight(),
        im1, ip1, jm1, jp1;

    //  We have already done all the dimensions checking.  In particular, we know that the
    //  input image extends one pixel out of theRect in all directions.  Hence we can
    //  always access pixels at im1, ip1, jm1, jp1
    const float* const* fRed = imgIn->getShiftedRedF2D();
    const float* const* fGreen = imgIn->getShiftedGreenF2D();
    const float* const* fBlue = imgIn->getShiftedBlueF2D();
    float   **fvx = static_cast<float**> (vectOut->getShiftedRasterX2D(R_W_ACCESS)),
            **fvy = static_cast<float**> (vectOut->getShiftedRasterY2D(R_W_ACCESS));
    float   fvxTemp, fvyTemp;
    const   float   ONE_THIRD = 1.f/3.f;
            
    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
           
            //  compute convolution over the processing rectangle
            im1 = iLow-1;
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; i++)
            {
                jm1 = jLow-1;
                jp1 = jLow+1;
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp =  0.25f*(fRed[i][jp1] - fRed[i][jm1]) +
                               0.125f*((fRed[im1][jp1] - fRed[im1][jm1]) +
                                       (fRed[ip1][jp1] - fRed[ip1][jm1]));

                    fvyTemp =  0.25f*(fRed[ip1][j] - fRed[im1][j]) +
                               0.125f*((fRed[ip1][jp1] - fRed[im1][jp1]) +
                                       (fRed[ip1][jm1] - fRed[im1][jm1]));

                    //  green component
                    fvxTemp += 0.25f*(fGreen[i][jp1] - fGreen[i][jm1]) +
                               0.125f*((fGreen[im1][jp1] - fGreen[im1][jm1]) +
                                       (fGreen[ip1][jp1] - fGreen[ip1][jm1]));

                    fvyTemp += 0.25f*(fGreen[ip1][j] - fGreen[im1][j]) +
                               0.125f*((fGreen[ip1][jp1] - fGreen[im1][jp1]) +
                                       (fGreen[ip1][jm1] - fGreen[im1][jm1]));

                    //  blue component
                    fvxTemp += 0.25f*(fBlue[i][jp1] - fBlue[i][jm1]) +
                               0.125f*((fBlue[im1][jp1] - fBlue[im1][jm1]) +
                                       (fBlue[ip1][jp1] - fBlue[ip1][jm1]));

                    fvyTemp += 0.25f*(fBlue[ip1][j] - fBlue[im1][j]) +
                               0.125f*((fBlue[ip1][jp1] - fBlue[im1][jp1]) +
                                       (fBlue[ip1][jm1] - fBlue[im1][jm1]));

                    fvx[i][j] = ONE_THIRD * fvxTemp;
                    fvy[i][j] = ONE_THIRD * fvyTemp;

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
            break;
        
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
           
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            im1 = iLow-1;
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; i++)
            {
                jm1 = jLow-1;
                jp1 = jLow+1;
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = 0.25f*(fRed[i][jp1] - fRed[i][jm1]) +
                              0.125f*((fRed[im1][jp1] - fRed[im1][jm1]) +
                                      (fRed[ip1][jp1] - fRed[ip1][jm1]));

                    fvyTemp = 0.25f*(fRed[ip1][j] - fRed[im1][j]) +
                              0.125f*((fRed[ip1][jp1] - fRed[im1][jp1]) +
                                      (fRed[ip1][jm1] - fRed[im1][jm1]));

                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;
                    
                    //  green component
                    fvxComp = 0.25f*(fGreen[i][jp1] - fGreen[i][jm1]) +
                              0.125f*((fGreen[im1][jp1] - fGreen[im1][jm1]) +
                                      (fGreen[ip1][jp1] - fGreen[ip1][jm1]));

                    fvyComp = 0.25f*(fGreen[ip1][j] - fGreen[im1][j]) +
                              0.125f*((fGreen[ip1][jp1] - fGreen[im1][jp1]) +
                                      (fGreen[ip1][jm1] - fGreen[im1][jm1]));

                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = 0.25f*(fBlue[i][jp1] - fBlue[i][jm1]) +
                              0.125f*((fBlue[im1][jp1] - fBlue[im1][jm1]) +
                                      (fBlue[ip1][jp1] - fBlue[ip1][jm1]));

                    fvyComp = 0.25f*(fBlue[ip1][j] - fBlue[im1][j]) +
                              0.125f*((fBlue[ip1][jp1] - fBlue[im1][jp1]) +
                                      (fBlue[ip1][jm1] - fBlue[im1][jm1]));

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
           break;

        //  this should never happen
        default:
            break;
            
    }
}

void ColorImageGradient_Sobel::applyIntoF_(const RasterImage_RGBa_F* imgIn, const ImageRect* theRect,
                                     VectorField_I* vectOut)
{

    const int   iLow  = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int im1, ip1, jm1, jp1;

    //  We have already done all the dimensions checking.  In particular, we know that the
    //  input image extends one pixel out of theRect in all directions.  Hence we can
    //  always access pixels at im1, ip1, jm1, jp1
    const float* const* fRed = imgIn->getShiftedRedF2D();
    const float* const* fGreen = imgIn->getShiftedGreenF2D();
    const float* const* fBlue = imgIn->getShiftedBlueF2D();
    float   fvxTemp, fvyTemp;
    const   float   ONE_THIRD = 1.f/3.f;
    int   **vx = static_cast<int**> (vectOut->getShiftedRasterX2D(R_W_ACCESS));
    int   **vy = static_cast<int**> (vectOut->getShiftedRasterY2D(R_W_ACCESS));

    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
           
            //  compute convolution over the processing rectangle
            im1 = iLow-1;
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; i++)
            {
                jm1 = jLow-1;
                jp1 = jLow+1;
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp =  0.25f*(fRed[i][jp1] - fRed[i][jm1]) +
                               0.125f*((fRed[im1][jp1] - fRed[im1][jm1]) +
                                       (fRed[ip1][jp1] - fRed[ip1][jm1]));

                    fvyTemp =  0.25f*(fRed[ip1][j] - fRed[im1][j]) +
                               0.125f*((fRed[ip1][jp1] - fRed[im1][jp1]) +
                                       (fRed[ip1][jm1] - fRed[im1][jm1]));

                    //  green component
                    fvxTemp += 0.25f*(fGreen[i][jp1] - fGreen[i][jm1]) +
                               0.125f*((fGreen[im1][jp1] - fGreen[im1][jm1]) +
                                       (fGreen[ip1][jp1] - fGreen[ip1][jm1]));

                    fvyTemp += 0.25f*(fGreen[ip1][j] - fGreen[im1][j]) +
                               0.125f*((fGreen[ip1][jp1] - fGreen[im1][jp1]) +
                                       (fGreen[ip1][jm1] - fGreen[im1][jm1]));

                    //  blue component
                    fvxTemp += 0.25f*(fBlue[i][jp1] - fBlue[i][jm1]) +
                               0.125f*((fBlue[im1][jp1] - fBlue[im1][jm1]) +
                                       (fBlue[ip1][jp1] - fBlue[ip1][jm1]));

                    fvyTemp += 0.25f*(fBlue[ip1][j] - fBlue[im1][j]) +
                               0.125f*((fBlue[ip1][jp1] - fBlue[im1][jp1]) +
                                       (fBlue[ip1][jm1] - fBlue[im1][jm1]));

                    vx[i][j] = static_cast<int> (ONE_THIRD * fvxTemp);
                    vy[i][j] = static_cast<int> (ONE_THIRD * fvyTemp);

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
            break;
        
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
           
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            im1 = iLow-1;
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; i++)
            {
                jm1 = jLow-1;
                jp1 = jLow+1;
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = 0.25f*(fRed[i][jp1] - fRed[i][jm1]) +
                              0.125f*((fRed[im1][jp1] - fRed[im1][jm1]) +
                                      (fRed[ip1][jp1] - fRed[ip1][jm1]));

                    fvyTemp = 0.25f*(fRed[ip1][j] - fRed[im1][j]) +
                              0.125f*((fRed[ip1][jp1] - fRed[im1][jp1]) +
                                      (fRed[ip1][jm1] - fRed[im1][jm1]));
                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;
                    
                    //  green component
                    fvxComp = 0.25f*(fGreen[i][jp1] - fGreen[i][jm1]) +
                              0.125f*((fGreen[im1][jp1] - fGreen[im1][jm1]) +
                                      (fGreen[ip1][jp1] - fGreen[ip1][jm1]));

                    fvyComp = 0.25f*(fGreen[ip1][j] - fGreen[im1][j]) +
                              0.125f*((fGreen[ip1][jp1] - fGreen[im1][jp1]) +
                                      (fGreen[ip1][jm1] - fGreen[im1][jm1]));
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = 0.25f*(fBlue[i][jp1] - fBlue[i][jm1]) +
                              0.125f*((fBlue[im1][jp1] - fBlue[im1][jm1]) +
                                      (fBlue[ip1][jp1] - fBlue[ip1][jm1]));

                    fvyComp = 0.25f*(fBlue[ip1][j] - fBlue[im1][j]) +
                              0.125f*((fBlue[ip1][jp1] - fBlue[im1][jp1]) +
                                      (fBlue[ip1][jm1] - fBlue[im1][jm1]));
                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        vx[i][j] = static_cast<int> (fvxComp);
                        vy[i][j] = static_cast<int> (fvyComp);
                    }
                    else
                    {
                        vx[i][j] = static_cast<int> (fvxTemp);
                        vy[i][j] = static_cast<int> (fvyTemp);
                    }

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
           break;

        //  this should never happen
        default:
            break;
            
    }
}

void ColorImageGradient_Sobel::applyIntoI_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                     VectorField_F* vectOut)
{
    const int   iLow  = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int im1, ip1, 
        fourjR, fourjG, fourjB,         //  4*j     red green blue
        fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
        fourjp1R, fourjp1G, fourjp1B;   //  4*(j+1) red green blue

    //  We have already done all the dimensions checking.  In particular, we know that the
    //  input image extends one pixel out of theRect in all directions.  Hence we can
    //  always access pixels at im1, ip1, jm1, jp1
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    float   **fvx = static_cast<float**> (vectOut->getShiftedRasterX2D(R_W_ACCESS)),
            **fvy = static_cast<float**> (vectOut->getShiftedRasterY2D(R_W_ACCESS));
    float   fvxTemp, fvyTemp;
    const   float   ONE_THIRD = 1.f/3.f;
            
    const int   FIRST_4JM1_R = 4*(jLow-1);
    const int   FIRST_4JM1_G = FIRST_4JM1_R + 1;
    const int   FIRST_4JM1_B = FIRST_4JM1_G + 1;
    const int   FIRST_4J_R = FIRST_4JM1_R + 4;
    const int   FIRST_4J_G = FIRST_4J_R + 1;
    const int   FIRST_4J_B = FIRST_4J_G + 1;
    const int   FIRST_4JP1_R = FIRST_4J_R + 4;
    const int   FIRST_4JP1_G = FIRST_4JP1_R + 1;
    const int   FIRST_4JP1_B = FIRST_4JP1_G + 1;

    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
           
            //  compute convolution over the processing rectangle
            im1 = iLow-1;
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; ip1++)
            {
                fourjm1R = FIRST_4JM1_R;
                fourjm1G = FIRST_4JM1_G;
                fourjm1B = FIRST_4JM1_B;
                fourjR = FIRST_4J_R;
                fourjG = FIRST_4J_G;
                fourjB = FIRST_4J_B;
                fourjp1R = FIRST_4JP1_R;
                fourjp1G = FIRST_4JP1_G;
                fourjp1B = FIRST_4JP1_B;
                //  
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp =  0.25f*(rgba[i][fourjp1R] - rgba[i][fourjm1R]) +
                               0.125f*((rgba[im1][fourjp1R] - rgba[im1][fourjm1R]) +
                                       (rgba[ip1][fourjp1R] - rgba[ip1][fourjm1R]));

                    fvyTemp =  0.25f*(rgba[ip1][fourjR] - rgba[im1][fourjR]) +
                               0.125f*((rgba[ip1][fourjp1R] - rgba[im1][fourjp1R]) +
                                       (rgba[ip1][fourjm1R] - rgba[im1][fourjm1R]));

                    //  green component
                    fvxTemp += 0.25f*(rgba[i][fourjp1G] - rgba[i][fourjm1G]) +
                               0.125f*((rgba[im1][fourjp1G] - rgba[im1][fourjm1G]) +
                                       (rgba[ip1][fourjp1G] - rgba[ip1][fourjm1G]));

                    fvyTemp += 0.25f*(rgba[ip1][fourjG] - rgba[im1][fourjG]) +
                               0.125f*((rgba[ip1][fourjp1G] - rgba[im1][fourjp1G]) +
                                       (rgba[ip1][fourjm1G] - rgba[im1][fourjm1G]));

                    //  blue component
                    fvxTemp += 0.25f*(rgba[i][fourjp1B] - rgba[i][fourjm1B]) +
                               0.125f*((rgba[im1][fourjp1B] - rgba[im1][fourjm1B]) +
                                       (rgba[ip1][fourjp1B] - rgba[ip1][fourjm1B]));

                    fvyTemp += 0.25f*(rgba[ip1][fourjB] - rgba[im1][fourjB]) +
                               0.125f*((rgba[ip1][fourjp1B] - rgba[im1][fourjp1B]) +
                                       (rgba[ip1][fourjm1B] - rgba[im1][fourjm1B]));

                    fvx[i][j] = ONE_THIRD * fvxTemp;
                    fvy[i][j] = ONE_THIRD * fvyTemp;

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

                //  shift the i indexes (ip1 is taken care of in the loop
                //  statement)
                im1 = i;
                i = ip1;
            }
            break;
        
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
           
            float   fvxComp, fvyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            im1 = iLow-1;
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; ip1++)
            {
                fourjm1R = FIRST_4JM1_R;
                fourjm1G = FIRST_4JM1_G;
                fourjm1B = FIRST_4JM1_B;
                fourjR = FIRST_4J_R;
                fourjG = FIRST_4J_G;
                fourjB = FIRST_4J_B;
                fourjp1R = FIRST_4JP1_R;
                fourjp1G = FIRST_4JP1_G;
                fourjp1B = FIRST_4JP1_B;
                //  
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    fvxTemp = 0.25f*(rgba[i][fourjp1R] - rgba[i][fourjm1R]) +
                              0.125f*((rgba[im1][fourjp1R] - rgba[im1][fourjm1R]) +
                                      (rgba[ip1][fourjp1R] - rgba[ip1][fourjm1R]));

                    fvyTemp = 0.25f*(rgba[ip1][fourjR] - rgba[im1][fourjR]) +
                              0.125f*((rgba[ip1][fourjp1R] - rgba[im1][fourjp1R]) +
                                      (rgba[ip1][fourjm1R] - rgba[im1][fourjm1R]));

                    normTemp = fvxTemp*fvxTemp + fvyTemp*fvyTemp;
                    
                    //  green component
                    fvxComp = 0.25f*(rgba[i][fourjp1G] - rgba[i][fourjm1G]) +
                              0.125f*((rgba[im1][fourjp1G] - rgba[im1][fourjm1G]) +
                                      (rgba[ip1][fourjp1G] - rgba[ip1][fourjm1G]));

                    fvyComp = 0.25f*(rgba[ip1][fourjG] - rgba[im1][fourjG]) +
                              0.125f*((rgba[ip1][fourjp1G] - rgba[im1][fourjp1G]) +
                                      (rgba[ip1][fourjm1G] - rgba[im1][fourjm1G]));

                    normComp = fvxComp*fvxComp + fvyComp*fvyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        fvxTemp = fvxComp;
                        fvyTemp = fvyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    fvxComp = 0.25f*(rgba[i][fourjp1B] - rgba[i][fourjm1G]) +
                              0.125f*((rgba[im1][fourjp1B] - rgba[im1][fourjm1G]) +
                                      (rgba[ip1][fourjp1B] - rgba[ip1][fourjm1G]));

                    fvyComp = 0.25f*(rgba[ip1][fourjB] - rgba[im1][fourjB]) +
                              0.125f*((rgba[ip1][fourjp1B] - rgba[im1][fourjp1B]) +
                                      (rgba[ip1][fourjm1B] - rgba[im1][fourjm1B]));

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

                    //  shift the j indices (j is taken care of in the loop statement)
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

                //  shift the i indices (ip1 is taken care of in the loop statement)
                im1 = i;
                i = ip1;
            }
           break;

        //  this should never happen
        default:
            break;
            
    }
}

void ColorImageGradient_Sobel::applyIntoI_(const RasterImage_RGBa* imgIn, const ImageRect* theRect,
                                     VectorField_I* vectOut)
{
    int iLow  = theRect->getTop(),
        iHigh = theRect->getBottom(),
        jLow = theRect->getLeft(),
        jHigh = theRect->getRight(),
        im1, ip1, 
        fourjR, fourjG, fourjB,         //  4*j     red green blue
        fourjm1R, fourjm1G, fourjm1B,   //  4*(j-1) red green blue
        fourjp1R, fourjp1G, fourjp1B;   //  4*(j+1) red green blue

    //  We have already done all the dimensions checking.  In particular, we know that the
    //  input image extends one pixel out of theRect in all directions.  Hence we can
    //  always access pixels at im1, ip1, jm1, jp1
    const unsigned char* const* rgba = imgIn->getShiftedRaster2D();
    int     **vx = static_cast<int**> (vectOut->getShiftedRasterX2D(R_W_ACCESS));
    int     **vy = static_cast<int**> (vectOut->getShiftedRasterY2D(R_W_ACCESS));
    int     vxTemp, vyTemp;

    const int   FIRST_4JM1_R = 4*(jLow-1);
    const int   FIRST_4JM1_G = FIRST_4JM1_R + 1;
    const int   FIRST_4JM1_B = FIRST_4JM1_G + 1;
    const int   FIRST_4J_R = FIRST_4JM1_R + 4;
    const int   FIRST_4J_G = FIRST_4J_R + 1;
    const int   FIRST_4J_B = FIRST_4J_G + 1;
    const int   FIRST_4JP1_R = FIRST_4J_R + 4;
    const int   FIRST_4JP1_G = FIRST_4JP1_R + 1;
    const int   FIRST_4JP1_B = FIRST_4JP1_G + 1;



    //  We have separate computation loops for each color gradient mode
    switch (getColorGradientMode())
    {
        //  the "color" gradient is the average of the red, green, and blue gradients
        case GRAD_AVE_OF_COLOR_COMPONENTS:
           
            //  compute convolution over the processing rectangle
            im1 = iLow-1;
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; ip1++)
            {
                fourjm1R = FIRST_4JM1_R;
                fourjm1G = FIRST_4JM1_G;
                fourjm1B = FIRST_4JM1_B;
                fourjR = FIRST_4J_R;
                fourjG = FIRST_4J_G;
                fourjB = FIRST_4J_B;
                fourjp1R = FIRST_4JP1_R;
                fourjp1G = FIRST_4JP1_G;
                fourjp1B = FIRST_4JP1_B;
                //  
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    vxTemp =  2*(rgba[i][fourjp1R] - rgba[i][fourjm1R]) +
                                (rgba[im1][fourjp1R] - rgba[im1][fourjm1R]) +
                                (rgba[ip1][fourjp1R] - rgba[ip1][fourjm1R]);

                    vyTemp =  2*(rgba[ip1][fourjR] - rgba[im1][fourjR]) +
                                 (rgba[ip1][fourjp1R] - rgba[im1][fourjp1R]) +
                                 (rgba[ip1][fourjm1R] - rgba[im1][fourjm1R]);

                    //  green component
                    vxTemp += 2*(rgba[i][fourjp1G] - rgba[i][fourjm1G]) +
                                (rgba[im1][fourjp1G] - rgba[im1][fourjm1G]) +
                                (rgba[ip1][fourjp1G] - rgba[ip1][fourjm1G]);

                    vyTemp += 2*(rgba[ip1][fourjG] - rgba[im1][fourjG]) +
                                (rgba[ip1][fourjp1G] - rgba[im1][fourjp1G]) +
                                (rgba[ip1][fourjm1G] - rgba[im1][fourjm1G]);

                    //  blue component
                    vxTemp += 2*(rgba[i][fourjp1B] - rgba[i][fourjm1B]) +
                                (rgba[im1][fourjp1B] - rgba[im1][fourjm1B]) +
                                (rgba[ip1][fourjp1B] - rgba[ip1][fourjm1B]);

                    vyTemp += 2*(rgba[ip1][fourjB] - rgba[im1][fourjB]) +
                                (rgba[ip1][fourjp1B] - rgba[im1][fourjp1B]) +
                                (rgba[ip1][fourjm1B] - rgba[im1][fourjm1B]);

                    vx[i][j] = vxTemp/24;
                    vy[i][j] = vyTemp/24;

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

                //  shift the i indexes (ip1 is taken care of in the loop
                //  statement)
                im1 = i;
                i = ip1;
            }
            break;
        
        //  the "color" gradient is the red, green, or blue gradient with the largest
        //  magnitude
        case GRAD_LARGEST_COLOR_COMPONENT:
           
            int   vxComp, vyComp, normTemp, normComp; 

            //  compute convolution over the processing rectangle
            im1 = iLow-1;
            ip1 = iLow+1;
            for (int i=iLow; i<=iHigh; ip1++)
            {
                fourjm1R = 4*(jLow-1);
                fourjm1G = fourjm1R + 1;
                fourjm1B = fourjm1G + 1;
                fourjR = 4*jLow;
                fourjG = fourjR + 1;
                fourjB = fourjG + 1;
                fourjp1R = fourjm1R + 8;
                fourjp1G = fourjp1R + 1;
                fourjp1B = fourjp1G + 1;
                for (int j=jLow; j<=jHigh; j++)
                {
                    //  red component
                    vxTemp =  2*(rgba[i][fourjp1R] - rgba[i][fourjm1R]) +
                                (rgba[im1][fourjp1R] - rgba[im1][fourjm1R]) +
                                (rgba[ip1][fourjp1R] - rgba[ip1][fourjm1R]);

                    vyTemp =  2*(rgba[ip1][fourjR] - rgba[im1][fourjR]) +
                                (rgba[ip1][fourjp1R] - rgba[im1][fourjp1R]) +
                                (rgba[ip1][fourjm1R] - rgba[im1][fourjm1R]);

                    normTemp = vxTemp*vxTemp + vyTemp*vyTemp;
                    
                    //  green component
                    vxComp  = 2*(rgba[i][fourjp1G] - rgba[i][fourjm1G]) +
                                (rgba[im1][fourjp1G] - rgba[im1][fourjm1G]) +
                                (rgba[ip1][fourjp1G] - rgba[ip1][fourjm1G]);

                    vyComp  = 2*(rgba[ip1][fourjG] - rgba[im1][fourjG]) +
                                (rgba[ip1][fourjp1G] - rgba[im1][fourjp1G]) +
                                (rgba[ip1][fourjm1G] - rgba[im1][fourjm1G]);


                    normComp = vxComp*vxComp + vyComp*vyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        vxTemp = vxComp;
                        vyTemp = vyComp;
                        normTemp = normComp;
                    }

                    //  blue component
                    vxComp  = 2*(rgba[i][fourjp1B] - rgba[i][fourjm1B]) +
                                (rgba[im1][fourjp1B] - rgba[im1][fourjm1B]) +
                                (rgba[ip1][fourjp1B] - rgba[ip1][fourjm1B]);

                    vyComp  = 2*(rgba[ip1][fourjB] - rgba[im1][fourjB]) +
                                (rgba[ip1][fourjp1B] - rgba[im1][fourjp1B]) +
                                (rgba[ip1][fourjm1B] - rgba[im1][fourjm1B]);

                    normComp = vxComp*vxComp + vyComp*vyComp;
                    //  retain the component with the largest gradient norm
                    if (normComp > normTemp)
                    {
                        vx[i][j] = vxComp/8;
                        vy[i][j] = vyComp/8;
                    }
                    else
                    {
                        vx[i][j] = vxTemp/8;
                        vy[i][j] = vyTemp/8;
                    }

                    //  shift the j indices (j is taken care of in the loop statement)
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

                //  shift the i indices (ip1 is taken care of in the loop statement)
                im1 = i;
                i = ip1;
            }
           break;

        //  this should never happen
        default:
            break;
            
    }
}


