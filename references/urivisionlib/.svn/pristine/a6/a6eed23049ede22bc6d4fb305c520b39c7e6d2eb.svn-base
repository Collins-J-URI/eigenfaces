/*  NAME:
        ImageGradient_Gaussian_F.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageGradient_Gaussian_F class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
//

#include "RasterImage_gray_F.h"
#include "ImageGradient_Gaussian_F.h"
#include "VectorField_F.h"

using namespace uriVL;

#define	kMaxHardCodedWidth	13


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Constructors and destructor
//------------------------------------------------------
#endif


ImageGradient_Gaussian_F::ImageGradient_Gaussian_F(double theScale)
		try	:	ImageGradient(2*(((int ) (5*theScale))/2) + 1, false),
				//
				scale_(theScale),
				fg_(NULL),
				fgD_(NULL)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a Gaussian Gradient operator is 0.8");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Gaussian_F constructor");
	throw e;
}



ImageGradient_Gaussian_F::ImageGradient_Gaussian_F(double theScale, const ImageRect* theRect)
		try	:	ImageGradient(2*(((int ) (5*theScale))/2) + 1, false),
				//
				scale_(theScale),
				fg_(NULL),
				fgD_(NULL)				
{
    FAIL_CONDITION( theScale < 0.8f,
                    kFilterAllocationError,
                    "The smallest scale admissible for a G Gradient filter is 0.8");
	try {
//		theRect->getWidth()*theRect->getHeight();
	}
	catch (ErrorReport& e) {
		e.appendToMessage("called by ImageGradient_Gaussian_F constructor");
		throw e;
	}
	catch (...) {
		FAIL_CONDITION( theRect == NULL,
						kNullParameterError,
						"Null pointer passed to ImageGradient_Gaussian_F constructor");
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid parameter passed to ImageGradient_Gaussian_F constructor");
	}

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Gaussian_F constructor");
	throw e;
}



ImageGradient_Gaussian_F::ImageGradient_Gaussian_F(int theWidth)
		try	:	ImageGradient(theWidth, false),
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
	e.appendToMessage("called by ImageGradient_Gaussian_F constructor");
	throw e;
}


			
ImageGradient_Gaussian_F::ImageGradient_Gaussian_F(int  theWidth, const ImageRect*  theRect)
		try	:	ImageGradient(theWidth, theRect, false),
				//
				scale_(0.2L*getWidth()),
				fg_(NULL),
				fgD_(NULL)				
{
    FAIL_CONDITION( theWidth < 3,
                    kFilterAllocationError,
                    "The smallest width admissible for a G Gradient filter is 3");

	initializeFilter_();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Gaussian_F constructor");
	throw e;
}


ImageGradient_Gaussian_F::ImageGradient_Gaussian_F(const ImageGradient_Gaussian_F& theObj)
		try	:	ImageGradient(theObj),
				//
				scale_(theObj.scale_),
				fg_(NULL),
				fgD_(NULL)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient_Gaussian_F copy constructor not implemented.");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ImageGradient_Gaussian_F constructor");
	throw e;
}


ImageGradient_Gaussian_F::~ImageGradient_Gaussian_F(void)
{
    delete []fg_;
    delete []fgD_;
}


const ImageGradient_Gaussian_F& ImageGradient_Gaussian_F::operator = (const ImageGradient_Gaussian_F& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "ImageGradient_Gaussian_F copy operator not implemented.");

	return *this;
}
			
#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Public getters
//------------------------------------------------------
#endif


float ImageGradient_Gaussian_F::getScale(void) const
{
    return static_cast<float>(scale_);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark applyTo-applyInto functions
//------------------------------------------------------
#endif


VectorField* ImageGradient_Gaussian_F::applyTo(const RasterImage* imgIn)
{
    FAIL_CONDITION( imgIn == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as input to ImageGradient_Gaussian_F::applyInto");
	
	return applyTo(imgIn, imgIn->getValidRect());
}


VectorField* ImageGradient_Gaussian_F::applyTo(const RasterImage* imgIn, const ImageRect* theRect)
{
    FAIL_CONDITION( imgIn == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as input to ImageGradient_Gaussian_F::applyInto");
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in ImageGradient_Gaussian_F::applyInto");

	ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
	const int	m = getWidth() / 2;
	usableRect->removeAllAround(m);
	ImageRect* destRect = ImageRect::intersection(usableRect, theRect);
    FAIL_CONDITION( destRect == NULL, 
                    kNullRectangleError, 
                    "Destination rectangle does not intersect valid data rectangle");

	VectorField	*vectOut = new VectorField_F(destRect);
	applyInto(imgIn, theRect, vectOut);

	delete destRect;
	delete usableRect;

	return vectOut;
}

void ImageGradient_Gaussian_F::applyInto(const RasterImage* imgIn, VectorField* vectOut)
{
    FAIL_CONDITION( imgIn == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage passed as input to ImageGradient_Gaussian_F::applyInto");
    FAIL_CONDITION( vectOut == NULL, 
                    kNullVectorFieldError, 
                    "null VectorField passed as destination to ImageGradient_Gaussian_F::applyInto");

    applyInto(imgIn, imgIn->getValidRect(), vectOut);
}


void ImageGradient_Gaussian_F::applyInto(const RasterImage* imgIn, const ImageRect* theRect, VectorField* vectOut)
{
	//----------------------------------------------------------------
	//	STEP 0:		Test for null or invalid type parameters
	//----------------------------------------------------------------
    FAIL_CONDITION(	imgIn == NULL,
    				kNullRasterImageError,
    				"NULL input raster image passed as parameter");

    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "NULL ImageRect parameter in ImageGradient_Gaussian_F::applyInto");

    FAIL_CONDITION( vectOut == NULL, 
                    kNullVectorFieldError, 
                    "null VectorField passed as destination to ImageGradient_Gaussian_F::applyInto");

    //  so far I only accept a floating point vector field as output
	FAIL_CONDITION( !vectOut->hasFloatRaster(),
	                kInvalidRasterType,
	                "ImageGradient_Gaussian_F::applyInto only accepts float vector fields as destination");
    
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
	//	STEP 2:		Operate only on a gray-level image
	//----------------------------------------------------------------
    bool localGray = false;
	const RasterImage_gray*	grayImg = RasterImage_gray::localRasterGray(imgIn, &localGray);

	//----------------------------------------------------------------
	//	STEP 3:		Perform the calculations
	//----------------------------------------------------------------
    //	allocate a 2D raster for intermediate calculations if needed
    bool newTempRaster = allocateTempRaster_(imgIn->getValidRect());
	
    float*const* fvx = (float*const*) (((VectorField_F *) vectOut)->getShiftedRasterX2D());
    float*const* fvy = (float*const*) (((VectorField_F *) vectOut)->getShiftedRasterY2D());

    switch (getWidth())
    {
        case 3:
            applyInto_3x3_X_(grayImg, destRect, fvx);
            applyInto_3x3_Y_(grayImg, destRect, fvy);
            break;

        case 5:
            applyInto_5x5_X_(grayImg, destRect, fvx);
            applyInto_5x5_Y_(grayImg, destRect, fvy);
            break;

        case 7:
            applyInto_7x7_X_(grayImg, destRect, fvx);
            applyInto_7x7_Y_(grayImg, destRect, fvy);
            break;

        case 9:
            applyInto_9x9_X_(grayImg, destRect, fvx);
            applyInto_9x9_Y_(grayImg, destRect, fvy);
            break;

        case 11:
            applyInto_11x11_X_(grayImg, destRect, fvx);
            applyInto_11x11_Y_(grayImg, destRect, fvy);
            break;

        case 13:
            applyInto_13x13_X_(grayImg, destRect, fvx);
            applyInto_13x13_Y_(grayImg, destRect, fvy);
            break;

        default:
            applyInto_X_(grayImg, destRect, fvx);
            applyInto_Y_(grayImg, destRect, fvy);
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
	if (localGray)
	    delete grayImg;
	delete destRect;
    if (newTempRaster)
		freeTempRaster_();

}


#if 0
#pragma mark -
#endif
//-------------------------------------------------------------------------------
//  Hard-coded private methods that implement the convolution
//-------------------------------------------------------------------------------


void ImageGradient_Gaussian_F::applyInto_3x3_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    int 	iLowM1 = iLow - 1, iHighP1 = iHigh+1;
    int 	j, jm1, jp1;
    int 	i, im1, ip1;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1;
        jp1 = jLow+1;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM1; i<=iHighP1; i++)
				fTemp[i][j] = 	fgD_[1]*(iGray[i][jm1] - iGray[i][jp1]);

            //  shift the j indexes (j itself is taken care of in the loop
            //  statement)
            jm1 = j;
            jp1++;
		}
			
		//-------------------------------------------------------
		//	then apply the 1D filter along the y direction
		//-------------------------------------------------------
        im1 = iLow-1;
		ip1 = iLow+1;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im1 = i;
			ip1++;
		}
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1;
        jp1 = jLow+1;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM1; i<=iHighP1; i++)
				fTemp[i][j] = 	fgD_[1]*(fGray[i][jm1] - fGray[i][jp1]);
					
            //  shift the j indexes (j itself is taken care of in the loop
            //  statement)
            jm1 = j;
            jp1++;
		}
			
		//-------------------------------------------------------
		//	then apply the 1D filter along the y direction
		//-------------------------------------------------------
        im1 = iLow-1;
		ip1 = iLow+1;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im1 = i;
			ip1++;
		}
    }
}

void ImageGradient_Gaussian_F::applyInto_3x3_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM1 = iLow - 1, iHighP1 = iHigh + 1;
    int 	j, jm1, jp1;
    int 	i, im1, ip1;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1;
        jp1 = jLow+1;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM1; i<=iHighP1; i++)
				fTemp[i][j] = 	fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
								fg_[0]* iGray[i][j];
					
            //  shift the j indexes (j itself is taken care of in the loop
            //  statement)
            jm1 = j;
            jp1++;
		}
			
		//-------------------------------------------------------
		//	then apply the 1D filter along the y direction
		//-------------------------------------------------------
        im1 = iLow-1;
		ip1 = iLow+1;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);
            
			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im1 = i;
			ip1++;
		}
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1;
        jp1 = jLow+1;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM1; i<=iHighP1; i++)
				fTemp[i][j] = 	fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
								fg_[0]* fGray[i][j];
					
            //  shift the j indexes (j itself is taken care of in the loop
            //  statement)
            jm1 = j;
            jp1++;
		}
			
		//-------------------------------------------------------
		//	then apply the 1D filter along the y direction
		//-------------------------------------------------------
        im1 = iLow-1;
		ip1 = iLow+1;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);
            
			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im1 = i;
			ip1++;
		}
    }
}

void ImageGradient_Gaussian_F::applyInto_5x5_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    int     iLow = theRect->getTop(),
            iHigh = theRect->getBottom(),
            jLow = theRect->getLeft(),
            jHigh = theRect->getRight();
    int 	iLowM2 = iLow - 2, iHighP2 = iHigh + 2;
    int 	i, im1, im2, ip1, ip2;
    int 	j, jm1, jm2, jp1, jp2;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2;
        jp1 = jLow+1; jp2 = jLow+2;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM2; i<=iHighP2; i++)
				fTemp[i][j] = 	fgD_[2]*(iGray[i][jm2] - iGray[i][jp2]) +
								fgD_[1]*(iGray[i][jm1] - iGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2;
		ip1 = iLow+1; ip2 = iLow+2;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im2 = im1;
			im1 = i;
			ip1 = ip2;
			ip2++;
		}
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2;
        jp1 = jLow+1; jp2 = jLow+2;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM2; i<=iHighP2; i++)
				fTemp[i][j] = 	fgD_[2]*(fGray[i][jm2] - fGray[i][jp2]) +
								fgD_[1]*(fGray[i][jm1] - fGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2;
		ip1 = iLow+1; ip2 = iLow+2;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im2 = im1;
			im1 = i;
			ip1 = ip2;
			ip2++;
		}
    }
}

void ImageGradient_Gaussian_F::applyInto_5x5_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    int 	    iLowM2 = iLow - 2, iHighP2 = iHigh + 2;
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    int 	i, im1, im2, ip1, ip2;
    int 	j, jm1, jm2, jp1, jp2;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; 
        jp1 = jLow+1; jp2 = jLow+2;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM2; i<=iHighP2; i++)
				fTemp[i][j] = 	fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
								fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
								fg_[0]* iGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2;
		ip1 = iLow+1; ip2 = iLow+2;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);

			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im2 = im1;
			im1 = i;
			ip1 = ip2;
			ip2++;
		}
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; 
        jp1 = jLow+1; jp2 = jLow+2;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM2; i<=iHighP2; i++)
				fTemp[i][j] = 	fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
								fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
								fg_[0]* fGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2;
		ip1 = iLow+1; ip2 = iLow+2;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);
            
			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im2 = im1;
			im1 = i;
			ip1 = ip2;
			ip2++;
		}
    }
}

void ImageGradient_Gaussian_F::applyInto_7x7_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM3 = iLow - 3, iHighP3 = iHigh + 3;
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    int 	i, im1, im2, im3, ip1, ip2, ip3;
    int 	j, jm1, jm2, jm3, jp1, jp2, jp3;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 

        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM3; i<=iHighP3; i++)
				fTemp[i][j] = 	fgD_[3]*(iGray[i][jm3] - iGray[i][jp3]) +
								fgD_[2]*(iGray[i][jm2] - iGray[i][jp2]) +
								fgD_[1]*(iGray[i][jm1] - iGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im3 = im2;
			im2 = im1;
			im1 = i;
			ip1 = ip2;
			ip2 = ip3;
			ip3++;
		}
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM3; i<=iHighP3; i++)
				fTemp[i][j] = 	fgD_[3]*(fGray[i][jm3] - fGray[i][jp3]) +
								fgD_[2]*(fGray[i][jm2] - fGray[i][jp2]) +
								fgD_[1]*(fGray[i][jm1] - fGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im3 = im2;
			im2 = im1;
			im1 = i;
			ip1 = ip2;
			ip2 = ip3;
			ip3++;
		}
    }
}

void ImageGradient_Gaussian_F::applyInto_7x7_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM3 = iLow - 3, iHighP3 = iHigh + 3;
    int 	i, im1, im2, im3, ip1, ip2, ip3;
    int 	j, jm1, jm2, jm3, jp1, jp2, jp3;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 

        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM3; i<=iHighP3; i++)
				fTemp[i][j] = 	fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
								fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
								fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
								fg_[0]* iGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);

			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im3 = im2;
			im2 = im1;
			im1 = i;
			ip1 = ip2;
			ip2 = ip3;
			ip3++;
		}
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM3; i<=iHighP3; i++)
				fTemp[i][j] = 	fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
								fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
								fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
								fg_[0]* fGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);

			//  shift the i indexes (i itself is taken care of in the loop
			//  statement)
			im3 = im2;
			im2 = im1;
			im1 = i;
			ip1 = ip2;
			ip2 = ip3;
			ip3++;
		}
    }
}

void ImageGradient_Gaussian_F::applyInto_9x9_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM4 = iLow - 4, iHighP4 = iHigh + 4;
    int 	j, jm1, jm2, jm3, jm4, jp1, jp2, jp3, jp4;
    int 	i, im1, im2, im3, im4, ip1, ip2, ip3, ip4;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM4; i<=iHighP4; i++)
				fTemp[i][j] = 	fgD_[4]*(iGray[i][jm4] - iGray[i][jp4]) +
								fgD_[3]*(iGray[i][jm3] - iGray[i][jp3]) +
								fgD_[2]*(iGray[i][jm2] - iGray[i][jp2]) +
								fgD_[1]*(iGray[i][jm1] - iGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
								fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

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
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM4; i<=iHighP4; i++)
				fTemp[i][j] = 	fgD_[4]*(fGray[i][jm4] - fGray[i][jp4]) +
								fgD_[3]*(fGray[i][jm3] - fGray[i][jp3]) +
								fgD_[2]*(fGray[i][jm2] - fGray[i][jp2]) +
								fgD_[1]*(fGray[i][jm1] - fGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
								fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

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
    }
}

void ImageGradient_Gaussian_F::applyInto_9x9_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM4 = iLow - 4, iHighP4 = iHigh + 4;
    int 	j, jm1, jm2, jm3, jm4, jp1, jp2, jp3, jp4;
    int 	i, im1, im2, im3, im4, ip1, ip2, ip3, ip4;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM4; i<=iHighP4; i++)
				fTemp[i][j] = 	fg_[4]*(iGray[i][jm4] + iGray[i][jp4]) +
								fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
								fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
								fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
								fg_[0]* iGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[4]*(fTemp[im4][j] - fTemp[ip4][j]) +
								fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);

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
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM4; i<=iHighP4; i++)
				fTemp[i][j] = 	fg_[4]*(fGray[i][jm4] + fGray[i][jp4]) +
								fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
								fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
								fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
								fg_[0]* fGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[4]*(fTemp[im4][j] - fTemp[ip4][j]) +
								fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);
            
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
    }
}

void ImageGradient_Gaussian_F::applyInto_11x11_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM5 = iLow - 5, iHighP5 = iHigh + 5;
    int 	j, jm1, jm2, jm3, jm4, jm5, jp1, jp2, jp3, jp4, jp5;
    int 	i, im1, im2, im3, im4, im5, ip1, ip2, ip3, ip4, ip5;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM5; i<=iHighP5; i++)
				fTemp[i][j] = 	fgD_[5]*(iGray[i][jm5] - iGray[i][jp5]) +
								fgD_[4]*(iGray[i][jm4] - iGray[i][jp4]) +
								fgD_[3]*(iGray[i][jm3] - iGray[i][jp3]) +
								fgD_[2]*(iGray[i][jm2] - iGray[i][jp2]) +
								fgD_[1]*(iGray[i][jm1] - iGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[5]*(fTemp[im5][j] + fTemp[ip5][j]) +
								fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
								fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];
            
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
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM5; i<=iHighP5; i++)
				fTemp[i][j] = 	fgD_[5]*(fGray[i][jm5] - fGray[i][jp5]) +
								fgD_[4]*(fGray[i][jm4] - fGray[i][jp4]) +
								fgD_[3]*(fGray[i][jm3] - fGray[i][jp3]) +
								fgD_[2]*(fGray[i][jm2] - fGray[i][jp2]) +
								fgD_[1]*(fGray[i][jm1] - fGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[5]*(fTemp[im5][j] + fTemp[ip5][j]) +
								fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
								fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

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
    }
}

void ImageGradient_Gaussian_F::applyInto_11x11_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM5 = iLow - 5, iHighP5 = iHigh + 5;
    int 	j, jm1, jm2, jm3, jm4, jm5, jp1, jp2, jp3, jp4, jp5;
    int 	i, im1, im2, im3, im4, im5, ip1, ip2, ip3, ip4, ip5;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM5; i<=iHighP5; i++)
				fTemp[i][j] = 	fg_[5]*(iGray[i][jm5] + iGray[i][jp5]) +
								fg_[4]*(iGray[i][jm4] + iGray[i][jp4]) +
								fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
								fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
								fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
								fg_[0]* iGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[5]*(fTemp[im5][j] - fTemp[ip5][j]) +
								fgD_[4]*(fTemp[im4][j] - fTemp[ip4][j]) +
								fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);

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
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM5; i<=iHighP5; i++)
				fTemp[i][j] = 	fg_[5]*(fGray[i][jm5] + fGray[i][jp5]) +
								fg_[4]*(fGray[i][jm4] + fGray[i][jp4]) +
								fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
								fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
								fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
								fg_[0]* fGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[5]*(fTemp[im5][j] - fTemp[ip5][j]) +
								fgD_[4]*(fTemp[im4][j] - fTemp[ip4][j]) +
								fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);
            
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
    }
}

void ImageGradient_Gaussian_F::applyInto_13x13_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
     const int 	iLowM6 = iLow - 6, iHighP6 = iHigh + 6;
    int 	j, jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    int 	i, im1, im2, im3, im4, im5, im6, ip1, ip2, ip3, ip4, ip5, ip6;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM6; i<=iHighP6; i++)
				fTemp[i][j] = 	fgD_[6]*(iGray[i][jm6] - iGray[i][jp6]) +
								fgD_[5]*(iGray[i][jm5] - iGray[i][jp5]) +
								fgD_[4]*(iGray[i][jm4] - iGray[i][jp4]) +
								fgD_[3]*(iGray[i][jm3] - iGray[i][jp3]) +
								fgD_[2]*(iGray[i][jm2] - iGray[i][jp2]) +
								fgD_[1]*(iGray[i][jm1] - iGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
            {
				fvx[i][j] = 	fg_[6]*(fTemp[im6][j] + fTemp[ip6][j]) +
								fg_[5]*(fTemp[im5][j] + fTemp[ip5][j]) +
								fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
								fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

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
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM6; i<=iHighP6; i++)
				fTemp[i][j] = 	fgD_[6]*(fGray[i][jm6] - fGray[i][jp6]) +
								fgD_[5]*(fGray[i][jm5] - fGray[i][jp5]) +
								fgD_[4]*(fGray[i][jm4] - fGray[i][jp4]) +
								fgD_[3]*(fGray[i][jm3] - fGray[i][jp3]) +
								fgD_[2]*(fGray[i][jm2] - fGray[i][jp2]) +
								fgD_[1]*(fGray[i][jm1] - fGray[i][jp1]);
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvx[i][j] = 	fg_[6]*(fTemp[im6][j] + fTemp[ip6][j]) +
								fg_[5]*(fTemp[im5][j] + fTemp[ip5][j]) +
								fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
								fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

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
    }
}

void ImageGradient_Gaussian_F::applyInto_13x13_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                                float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowM6 = iLow - 6, iHighP6 = iHigh + 6;
    int 	j, jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    int 	i, im1, im2, im3, im4, im5, im6, ip1, ip2, ip3, ip4, ip5, ip6;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM6; i<=iHighP6; i++)
				fTemp[i][j] = 	fg_[6]*(iGray[i][jm6] + iGray[i][jp6]) +
								fg_[5]*(iGray[i][jm5] + iGray[i][jp5]) +
								fg_[4]*(iGray[i][jm4] + iGray[i][jp4]) +
								fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
								fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
								fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
								fg_[0]* iGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[6]*(fTemp[im6][j] - fTemp[ip6][j]) +
								fgD_[5]*(fTemp[im5][j] - fTemp[ip5][j]) +
								fgD_[4]*(fTemp[im4][j] - fTemp[ip4][j]) +
								fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);

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
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowM6; i<=iHighP6; i++)
				fTemp[i][j] = 	fg_[6]*(fGray[i][jm6] + fGray[i][jp6]) +
								fg_[5]*(fGray[i][jm5] + fGray[i][jp5]) +
								fg_[4]*(fGray[i][jm4] + fGray[i][jp4]) +
								fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
								fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
								fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
								fg_[0]* fGray[i][j];
					
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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
				fvy[i][j] = 	fgD_[6]*(fTemp[im6][j] - fTemp[ip6][j]) +
								fgD_[5]*(fTemp[im5][j] - fTemp[ip5][j]) +
								fgD_[4]*(fTemp[im4][j] - fTemp[ip4][j]) +
								fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);
            
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
    }
}

void ImageGradient_Gaussian_F::applyInto_X_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                          float*const* fvx)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int    m = getWidth()/2;
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowMm = iLow - m, iHighPm = iHigh + m;
    int 	j, jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    int 	i, im1, im2, im3, im4, im5, im6, ip1, ip2, ip3, ip4, ip5, ip6;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
        const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowMm; i<=iHighPm; i++)
            {
				fTemp[i][j] = 	fgD_[6]*(iGray[i][jm6] - iGray[i][jp6]) +
								fgD_[5]*(iGray[i][jm5] - iGray[i][jp5]) +
								fgD_[4]*(iGray[i][jm4] - iGray[i][jp4]) +
								fgD_[3]*(iGray[i][jm3] - iGray[i][jp3]) +
								fgD_[2]*(iGray[i][jm2] - iGray[i][jp2]) +
								fgD_[1]*(iGray[i][jm1] - iGray[i][jp1]);
					
				for (int  l= kMaxHardCodedWidth+1; l<=m; l++)
					fTemp[i][j] += fgD_[l] * (iGray[i][j-l] - iGray[i][j+l] );

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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
            {
				fvx[i][j] = 	fg_[6]*(fTemp[im6][j] + fTemp[ip6][j]) +
								fg_[5]*(fTemp[im5][j] + fTemp[ip5][j]) +
								fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
								fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

				for (int  k= kMaxHardCodedWidth+1; k<=m; k++)
						fvx[i][j] += fg_[k] * (fTemp[i-k][j] + fTemp[i+k][j] );
		
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
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowMm; i<=iHighPm; i++)
            {
				fTemp[i][j] = 	fgD_[6]*(fGray[i][jm6] - fGray[i][jp6]) +
								fgD_[5]*(fGray[i][jm5] - fGray[i][jp5]) +
								fgD_[4]*(fGray[i][jm4] - fGray[i][jp4]) +
								fgD_[3]*(fGray[i][jm3] - fGray[i][jp3]) +
								fgD_[2]*(fGray[i][jm2] - fGray[i][jp2]) +
								fgD_[1]*(fGray[i][jm1] - fGray[i][jp1]);
					
				for (int  l= kMaxHardCodedWidth+1; l<=m; l++)
						fTemp[i][j] += fgD_[l] * (fGray[i][j-l] - fGray[i][j+l] );

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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
            {
				fvx[i][j] = 	fg_[6]*(fTemp[im6][j] + fTemp[ip6][j]) +
								fg_[5]*(fTemp[im5][j] + fTemp[ip5][j]) +
								fg_[4]*(fTemp[im4][j] + fTemp[ip4][j]) +
								fg_[3]*(fTemp[im3][j] + fTemp[ip3][j]) +
								fg_[2]*(fTemp[im2][j] + fTemp[ip2][j]) +
								fg_[1]*(fTemp[im1][j] + fTemp[ip1][j]) +
								fg_[0]* fTemp[i][j];

				for (int  k= kMaxHardCodedWidth+1; k<=m; k++)
						fvx[i][j] += fg_[k] * (fTemp[i-k][j] + fTemp[i+k][j] );

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
    }
}

void ImageGradient_Gaussian_F::applyInto_Y_(const RasterImage_gray* imgIn, const ImageRect* theRect,
                                          float*const* fvy)
{
    //  This private function is called after all data validity tests have been performed
    //  We know that the images are valid and that the rectangle fits both the input and
    //  the output images.
    float* const* fTemp = getShiftedTempGrayF2D_(R_W_ACCESS);
    const int   m = getWidth()/2;
    const int   iLow = theRect->getTop(),
                iHigh = theRect->getBottom(),
                jLow = theRect->getLeft(),
                jHigh = theRect->getRight();
    const int 	iLowMm = iLow - m, iHighPm = iHigh + m;
    int 	j, jm1, jm2, jm3, jm4, jm5, jm6, jp1, jp2, jp3, jp4, jp5, jp6;
    int 	i, im1, im2, im3, im4, im5, im6, ip1, ip2, ip3, ip4, ip5, ip6;
    
    //  case of an "integer" gray level image
    if (!imgIn->hasFloatRaster())
    {
       const unsigned char* const* iGray = imgIn->getShiftedRaster2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowMm; i<=iHighPm; i++)
            {
				fTemp[i][j] = 	fg_[6]*(iGray[i][jm6] + iGray[i][jp6]) +
								fg_[5]*(iGray[i][jm5] + iGray[i][jp5]) +
								fg_[4]*(iGray[i][jm4] + iGray[i][jp4]) +
								fg_[3]*(iGray[i][jm3] + iGray[i][jp3]) +
								fg_[2]*(iGray[i][jm2] + iGray[i][jp2]) +
								fg_[1]*(iGray[i][jm1] + iGray[i][jp1]) +
								fg_[0]* iGray[i][j];

				for (int  l= kMaxHardCodedWidth+1; l<=m; l++)
					fTemp[i][j] += fg_[l] * (iGray[i][j-l] + iGray[i][j+l] );

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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
            {
				fvy[i][j] = 	fgD_[6]*(fTemp[im6][j] - fTemp[ip6][j]) +
								fgD_[5]*(fTemp[im5][j] - fTemp[ip5][j]) +
								fgD_[4]*(fTemp[im4][j] - fTemp[ip4][j]) +
								fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);

 				for (int  k= kMaxHardCodedWidth+1; k<=m; k++)
						fvy[i][j] += fgD_[k] * (fTemp[i-k][j] + fTemp[i+k][j] );

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
    }
    //  float
    else
    {
        const float* const* fGray = ((RasterImage_gray_F *) imgIn)->getShiftedGrayF2D();

		//-------------------------------------------------------
		//	first apply the 1D filter along the x direction
		//-------------------------------------------------------
		//	Note that we have to apply this filter out of the destination rect since
		//	some of these elements will be needed we we apply a filter along the
		//	other direction.
        jm1 = jLow-1; jm2 = jLow-2; jm3 = jLow-3; 
        jm4 = jLow-4; jm5 = jLow-5; jm6 = jLow-6;
        jp1 = jLow+1; jp2 = jLow+2; jp3 = jLow+3; 
        jp4 = jLow+4; jp5 = jLow+5; jp6 = jLow+6;
        for (j=jLow; j<=jHigh; j++)
        {
            for (i=iLowMm; i<=iHighPm; i++)
            {
				fTemp[i][j] = 	fg_[6]*(fGray[i][jm6] + fGray[i][jp6]) +
								fg_[5]*(fGray[i][jm5] + fGray[i][jp5]) +
								fg_[4]*(fGray[i][jm4] + fGray[i][jp4]) +
								fg_[3]*(fGray[i][jm3] + fGray[i][jp3]) +
								fg_[2]*(fGray[i][jm2] + fGray[i][jp2]) +
								fg_[1]*(fGray[i][jm1] + fGray[i][jp1]) +
								fg_[0]* fGray[i][j];
					
				for (int  l= kMaxHardCodedWidth+1; l<=m; l++)
						fTemp[i][j] += fg_[l] * (fGray[i][j-l] + fGray[i][j+l] );

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
        im1 = iLow-1; im2 = iLow-2; im3 = iLow-3; 
		im4 = iLow-4; im5 = iLow-5; im6 = iLow-6;
		ip1 = iLow+1; ip2 = iLow+2; ip3 = iLow+3; 
		ip4 = iLow+4; ip5 = iLow+5; ip6 = iLow+6;
        for (i=iLow; i<=iHigh; i++)
        {
            for (j=jLow; j<=jHigh; j++)
            {
				fvy[i][j] = 	fgD_[6]*(fTemp[im6][j] - fTemp[ip6][j]) +
								fgD_[5]*(fTemp[im5][j] - fTemp[ip5][j]) +
								fgD_[4]*(fTemp[im4][j] - fTemp[ip4][j]) +
								fgD_[3]*(fTemp[im3][j] - fTemp[ip3][j]) +
								fgD_[2]*(fTemp[im2][j] - fTemp[ip2][j]) +
								fgD_[1]*(fTemp[im1][j] - fTemp[ip1][j]);

				for (int  k= kMaxHardCodedWidth+1; k<=m; k++)
						fvy[i][j] += fgD_[k] * (fTemp[i-k][j] - fTemp[i+k][j] );

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
    }
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Initialization
//------------------------------------------------------
#endif


void ImageGradient_Gaussian_F::initializeFilter_(void)
{
    const int  m = getWidth()/2;
    const double	sigma2Scale = 1.L / (2.L * scale_ * scale_),
					piSigma = 1.L / (sqrt(2*PI) * scale_),
					piSigma2 = piSigma/scale_;

    fg_ = new float [m+1];
    FAIL_CONDITION( fg_ == NULL,
                    kFilterAllocationError,
                    "filter allocation failed in ImageGradient_Gaussian_F::initializeFilter_");
    fgD_ = new float [m+1];
    FAIL_CONDITION( fgD_ == NULL,
                    kFilterAllocationError,
                    "filter allocation failed in ImageGradient_Gaussian_F::initializeFilter_");

    for (int  r=0; r<=m; r++)
    {
    	//	fg_ is a regular 1D Gaussian
    	fg_[r] = (float) (piSigma * exp(-r*r*sigma2Scale));

    	//	fg1Dx_ is a first derivative of a 1D Gaussian
    	fgD_[r] = (float) (-r * piSigma2 * exp(-r*r*sigma2Scale));

	}
}
