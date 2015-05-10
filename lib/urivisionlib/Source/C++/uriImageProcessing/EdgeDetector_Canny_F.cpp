#include "EdgeDetector_Canny_F.h"
#include "NonMaximumSuppressor.h"

#error Work in progress: do not use this class yet

using namespace uriVL;

EdgeDetector_Canny_F::EdgeDetector_Canny_F(double theScale, float hThresh, float lThresh)
{
  cGrad_ = new ImageGradient_Gaussian_F(theScale);
  interVect_ = NULL;
  lastVect_ = NULL;
  hThresh_ = hThresh;
  lThresh_ = lThresh;
}

EdgeDetector_Canny_F::EdgeDetector_Canny_F(int theWidth, float hThresh, float lThresh)
{
  cGrad_ = new ImageGradient_Gaussian_F(theWidth);
  interVect_ = NULL;
  lastVect_ = NULL;
  hThresh_ = hThresh;
  lThresh_ = lThresh;
}

EdgeDetector_Canny_F::EdgeDetector_Canny_F(double theScale,  ImageRect*  theRect, float hThresh, float lThresh)
{
  cGrad_ = new ImageGradient_Gaussian_F(theScale);
  interVect_ = new VectorField_F(theRect);
  lastVect_ = new VectorField_F(theRect);
  hThresh_ = hThresh;
  lThresh_ = lThresh;
}

EdgeDetector_Canny_F::EdgeDetector_Canny_F(int theWidth,  ImageRect*  theRect, float hThresh, float lThresh)
{
  cGrad_ = new ImageGradient_Gaussian_F(theWidth);
  interVect_ = new VectorField_F(theRect);
  lastVect_ = new VectorField_F(theRect);
  hThresh_ = hThresh;
  lThresh_ = lThresh;
}
			
EdgeDetector_Canny_F::~EdgeDetector_Canny_F(void)
{
  delete cGrad_;
  
  if (interVect_ != NULL)
  {
    delete interVect_;
    delete lastVect_;
    interVect_ = NULL;
    lastVect_ = NULL;
  }  
}
			
LabelRaster *EdgeDetector_Canny_F::applyTo(RasterImage* imgIn)
{
    FAIL_CONDITION(imgIn == NULL,
                   kNullRasterImageError,
                   "NULL input raster image passed as parameter in EdgeDetector_Canny_F::applyTo");

    return applyTo(imgIn, imgIn->getValidRect());
}

LabelRaster *EdgeDetector_Canny_F::applyTo(RasterImage* imgIn, char* params, bool connectNeighbor)
{
  return applyTo(imgIn);
}

LabelRaster *EdgeDetector_Canny_F::applyTo(RasterImage* imgIn, ImageRect* theRect)
{
  FAIL_CONDITION(imgIn == NULL,
                 kNullRasterImageError,
                 "NULL input raster image passed as parameter in EdgeDetector_Canny_F::applyTo");

  FAIL_CONDITION(theRect == NULL, 
                 kNullRectangleError, 
                 "NULL ImageRect parameter in EdgeDetector_Canny_F::applyTo");

  ImageRect* usableRect = new ImageRect(imgIn->getValidRect());
  ImageRect* destRect = ImageRect::intersection(usableRect, theRect);

  FAIL_CONDITION(destRect == NULL, 
                 kNullRectangleError, 
                 "Destination rectangle does not intersect valid data rectangle in EdgeDetector_Canny_F::applyTo");

  LabelRaster *labelOut = new LabelRaster(destRect);

  applyInto(imgIn, theRect, labelOut);

  return labelOut;
}

LabelRaster *EdgeDetector_Canny_F::applyTo(RasterImage* imgIn, char* params, ImageRect* theRect, bool connectNeighbor)
{
  return applyTo(imgIn, theRect);
}

void EdgeDetector_Canny_F::applyInto(RasterImage* imgIn, LabelRaster *labelOut)
{
  FAIL_CONDITION(imgIn == NULL,
                kNullRasterImageError,
                "NULL input raster image passed as parameter in EdgeDetector_Canny_F::applyInto");

  applyInto(imgIn, imgIn->getValidRect(), labelOut);
}

void EdgeDetector_Canny_F::applyInto(RasterImage* imgIn, char* params, LabelRaster *labelOut, bool connectNeighbor)
{
  applyInto(imgIn, labelOut);
}

void EdgeDetector_Canny_F::applyInto(RasterImage* imgIn, ImageRect* theRect,  LabelRaster *labelOut)
{
  //----------------------------------------------------------------
  //	STEP 0:		Test for null or invalid type parameters
  //----------------------------------------------------------------
  FAIL_CONDITION(imgIn == NULL,
                kNullRasterImageError,
                "NULL input raster image passed as parameter in EdgeDetector_Canny_F::applyInto");

  FAIL_CONDITION(theRect == NULL, 
                kNullRectangleError, 
                "NULL ImageRect parameter in EdgeDetector_Canny_F::applyInto");

  FAIL_CONDITION(labelOut == NULL,
                kNullLabelRasterError,
                "NULL output raster label passed as parameter in EdgeDetector_Canny_F::applyInto");


  //----------------------------------------------------------------
  //	STEP 1:		Test for rectangle intersection
  //----------------------------------------------------------------
  //  determine the output rectangle that the input image can produce.

  ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
  ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);

  delete usableRect;
  FAIL_CONDITION(destRect == NULL, 
                kNullRectangleError, 
                "Destination rectangle does not intersect valid data rectangle");

  /*
   * I know that I have to be bale to resize my label raster as in the gaussian filter class, but how exactly does one do that?
   */
  //if (!imgOut->contains(destRect))
  //  imgOut->setBoundRect(destRect);

	//----------------------------------------------------------------
	//	STEP 2:		Operate only on a gray-level image
	//----------------------------------------------------------------
  bool localGray;
	RasterImage_gray	*grayImg = RasterImage_gray::localRasterGray(imgIn, &localGray);

	//----------------------------------------------------------------
	//	STEP 3:		Perform the calculations
	//----------------------------------------------------------------
  //	allocate a 2D raster for intermediate calculations if needed
  
  bool retainTempRaster = (interVect_ != NULL);

  if (!retainTempRaster)
  {
    interVect_ = new VectorField_F(imgIn->getValidRect());
    lastVect_ = new VectorField_F(imgIn->getValidRect());
  }
  // make sure the user is sending in images that agree with preallocated arrays
  else if (!interVect_->contains(theRect))
	{
  	if (localGray)
 	    delete grayImg;
  	    
  	delete destRect;
    
    if (!retainTempRaster)
    {
      delete interVect_;
      delete lastVect_;
      interVect_ = NULL;
      lastVect_ = NULL;
    }
    
    FAIL_CONDITION(true, 
                   kInvalidRectangleError, 
                   "Input rect does not agree with pre-allocated dimensions!");
	}

  // get the image gradient
  cGrad_->applyInto(grayImg, interVect_);
 
  // perform non-maximum suppression on the image gradient
  NonMaximumSuppressor::applyInto(interVect_, lastVect_);

  // copy the norm of the non-maximum suppressed vector field and operate on that
  const int   iLow = destRect->getTop(),
              iHigh = destRect->getBottom(),
              jLow = destRect->getLeft(),
              jHigh = destRect->getRight();

  float** vLast = ((RasterImage_gray_F *)(lastVect_->getNormAsRasterImage()))->getShiftedGrayF2D();
  char** lOut = labelOut->getShiftedLabel2D();

  for (int i = iLow; i <= iHigh; i++)
    for (int j = jLow; j <= jHigh; j++)
      lOut[i][j] = vLast[i][j];
      
  // perform hysterisis operation
  int highColor = 1;

  for (int i = iLow; i <= iHigh; i++)
    for (int j = jLow; j <= jHigh; j++)
    {
      if ((vLast[i][j] > hThresh_) || (lOut[i][j] == highColor))
      {
        lOut[i][j] = highColor;
        
        if ((i > iLow) && (j > jLow))
          if (vLast[i-1][j-1] > lThresh_)
            lOut[i-1][j-1] = highColor;
        
        if (j > jLow)  
          if (vLast[i][j-1] > lThresh_)
            lOut[i][j-1] = highColor;
            
        if ((i < iHigh) && (j > jLow))
          if (vLast[i+1][j-1] > lThresh_)
            lOut[i+1][j-1] = highColor;

        if (i > iLow)          
          if (vLast[i-1][j] > lThresh_)
            lOut[i-1][j] = highColor;

        if (i < iHigh)          
          if (vLast[i+1][j] > lThresh_)
            lOut[i+1][j] = highColor;

        if ((i > iLow)&&(j < jHigh))
          if (vLast[i-1][j+1] > lThresh_)
            lOut[i-1][j+1] = highColor;

        if (j < jHigh)
          if (vLast[i][j+1] > lThresh_)
            lOut[i][j+1] = highColor;
          
        if ((i > iHigh)&&(j < jHigh))
          if (vLast[i+1][j+1] > lThresh_)
            lOut[i+1][j+1] = highColor;
      }
    }

	//----------------------------------------------------------------
	//	STEP 4:		Set rectangle
	//----------------------------------------------------------------
	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
	labelOut->setValidRect(destRect);

	//----------------------------------------------------------------
	//	STEP 5:		Free temporary storage (if needed)
	//----------------------------------------------------------------
	if (localGray)
    delete grayImg;
	    
	delete destRect;
  
  if (!retainTempRaster)
  {
    delete interVect_;
    delete lastVect_;
    interVect_ = NULL;
    lastVect_ = NULL;
  }
}

void EdgeDetector_Canny_F::applyInto(RasterImage* imgIn, char* params, ImageRect* theRect,  LabelRaster *labelOut, bool connectNeighbor)
{
  applyInto(imgIn, theRect, labelOut);
}

void EdgeDetector_Canny_F::applyInto(RasterImage* imgIn, RasterImage* imgOut)
{
  FAIL_CONDITION(imgIn == NULL,
                 kNullRasterImageError,
                 "NULL input raster image passed as parameter in EdgeDetector_Canny_F::applyTo");
  applyInto(imgIn,imgIn->getValidRect(),imgOut);
}

void EdgeDetector_Canny_F::applyInto(RasterImage* imgIn, char* params, RasterImage* imgOut)
{
  applyInto(imgIn, imgOut);
}

void EdgeDetector_Canny_F::applyInto(RasterImage* imgIn, ImageRect* theRect, RasterImage* imgOut)
{
  //----------------------------------------------------------------
  //	STEP 0:		Test for null or invalid type parameters
  //----------------------------------------------------------------
  FAIL_CONDITION(imgIn == NULL,
		            kNullRasterImageError,
    		        "NULL input raster image passed as parameter in EdgeDetector_Canny_F::applyInto");

  FAIL_CONDITION(theRect == NULL, 
                kNullRectangleError, 
                "NULL ImageRect parameter in EdgeDetector_Canny_F::applyInto");

  FAIL_CONDITION(imgOut == NULL,
                kNullRasterImageError,
                "NULL output raster image passed as parameter in EdgeDetector_Canny_F::applyInto");

	//----------------------------------------------------------------
	//	STEP 1:		Test for rectangle intersection
	//----------------------------------------------------------------
	                
	ImageRect	*usableRect = new ImageRect(imgIn->getValidRect());
	ImageRect	*destRect = ImageRect::intersection(usableRect, theRect);

	delete usableRect;

  FAIL_CONDITION(destRect == NULL, 
                 kNullRectangleError, 
                 "Destination rectangle does not intersect valid data rectangle");

  // if the output image does not contain the destination rect, then the user is a fool
  // and we fail
  if (!imgOut->contains(destRect))
  {
    delete destRect;
    
    FAIL_CONDITION(true, 
                   kInvalidRectangleError, 
                   "Destination rectangle is not contained by output image");
  }
	
	//----------------------------------------------------------------
	//	STEP 2:		Operate only on a gray-level image
	//----------------------------------------------------------------
  bool localGray;
	RasterImage_gray	*grayImg = RasterImage_gray::localRasterGray(imgIn, &localGray);

	//----------------------------------------------------------------
	//	STEP 3:		Perform the calculations
	//----------------------------------------------------------------
  //	allocate a 2D raster for intermediate calculations if needed
  
  bool retainTempRaster = (interVect_ != NULL);

  if (!retainTempRaster)
  {
    interVect_ = new VectorField_F(imgIn->getValidRect());
    lastVect_ = new VectorField_F(imgIn->getValidRect());
  }
  // make sure the user is sending in images that agree with preallocated arrays
  else if (!interVect_->contains(theRect))
	{
  	if (localGray)
 	    delete grayImg;
  	    
  	delete destRect;
    
    if (!retainTempRaster)
    {
      delete interVect_;
      delete lastVect_;
      interVect_ = NULL;
      lastVect_ = NULL;
    }
    
    FAIL_CONDITION(true, 
                   kInvalidRectangleError, 
                   "Input rect does not agree with pre-allocated dimensions!");
	}

  // get the image gradient
  cGrad_->applyInto(grayImg, interVect_);
 
  // perform non-maximum suppression on the image gradient
  NonMaximumSuppressor::applyInto(interVect_, lastVect_);

  // copy the norm of the non-maximum suppressed vector field and operate on that
  const int   iLow = destRect->getTop(),
              iHigh = destRect->getBottom(),
              jLow = destRect->getLeft(),
              jHigh = destRect->getRight();

  float** vLast = ((RasterImage_gray_F *)(lastVect_->getNormAsRasterImage()))->getShiftedGrayF2D();
  float** lOut = ((RasterImage_gray_F *) imgOut)->getShiftedGrayF2D();

  for (int i = iLow; i <= iHigh; i++)
    for (int j = jLow; j <= jHigh; j++)
      lOut[i][j] = vLast[i][j];
      
  // perform hysterisis operation
  int highColor = 255;
  int lowColor = 0;

  for (int i = iLow; i <= iHigh; i++)
    for (int j = jLow; j <= jHigh; j++)
    {
      if (lOut[i][j] > hThresh_)
      {
        lOut[i][j] = highColor;
        
        if ((i > iLow) && (j > jLow))
          if (lOut[i-1][j-1] > lThresh_)
            lOut[i-1][j-1] = highColor;
        
        if (j > jLow)  
          if (lOut[i][j-1] > lThresh_)
            lOut[i][j-1] = highColor;
            
        if ((i < iHigh) && (j > jLow))
          if (lOut[i+1][j-1] > lThresh_)
            lOut[i+1][j-1] = highColor;

        if (i > iLow)          
          if (lOut[i-1][j] > lThresh_)
            lOut[i-1][j] = highColor;

        if (i < iHigh)          
          if (lOut[i+1][j] > lThresh_)
            lOut[i+1][j] = highColor;

        if ((i > iLow)&&(j < jHigh))
          if (lOut[i-1][j+1] > lThresh_)
            lOut[i-1][j+1] = highColor;

        if (j < jHigh)
          if (lOut[i][j+1] > lThresh_)
            lOut[i][j+1] = highColor;
          
        if ((i > iHigh)&&(j < jHigh))
          if (lOut[i+1][j+1] > lThresh_)
            lOut[i+1][j+1] = highColor;
      }
    }
    
  for (int i = iLow; i <= iHigh; i++)
    for (int j = jLow; j <= jHigh; j++)
      if (lOut[i][j] < hThresh_)
        lOut[i][j] = lowColor;

	//----------------------------------------------------------------
	//	STEP 4:		Set rectangle
	//----------------------------------------------------------------
	//	Very important:  At the end of the computation, don't forget to set the
	//	output's valid data rectangle.  In "safe" mode, the library has no way to guess
	//	what the valid data rectangle should be and sets it to its minimum value... empty
	imgOut->setValidRect(destRect);
	imgOut->setFloatRasterAsReference();
	
	// uh, what does this do?
//	((RasterImage_gray_F *) imgOut)->setFloatConversionMode(floatConversionMode_);

	//----------------------------------------------------------------
	//	STEP 5:		Free temporary storage (if needed)
	//----------------------------------------------------------------
	if (localGray)
    delete grayImg;
	    
	delete destRect;
  
  if (!retainTempRaster)
  {
    delete interVect_;
    delete lastVect_;
    interVect_ = NULL;
    lastVect_ = NULL;
  }
}

void EdgeDetector_Canny_F::applyInto(RasterImage* imgIn, char* params, ImageRect* theRect, RasterImage* imgOut)
{
  applyInto(imgIn, theRect, imgOut);
}