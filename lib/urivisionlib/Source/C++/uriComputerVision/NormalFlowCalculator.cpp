/*  NAME:
        NormalFlowCalculator.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib NormalFlowCalculator class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/  

#include "NormalFlowCalculator.h"

using namespace std;
using namespace uriVL;


//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

NormalFlowCalculator::NormalFlowCalculator(const float gradScale,
										   const float gradThreshold)
	try	:	threshold_(gradThreshold),
			gaussGrad_(new ImageGradient_Gaussian_F(gradScale))
{
}
catch(ErrorReport& e) {
	e.appendToMessage("called by NormalFlowCalculator constructor.");
	throw e;
}
catch(...) {
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Unexpected error encountered in NormalFlowCalculator constructor.");
}


NormalFlowCalculator::NormalFlowCalculator(const ImageRect* theRect,
                                           const float gradScale,
										   const float gradThreshold)
	try	:	threshold_(gradThreshold),
			gaussGrad_(new ImageGradient_Gaussian_F(gradScale, theRect))
{
}
catch(ErrorReport& e) {
	e.appendToMessage("called by NormalFlowCalculator constructor.");
	throw e;
}
catch(...) {
	FAIL_CONDITION( true,
					kInvalidParameterError,
					"Unexpected error encountered in NormalFlowCalculator constructor.");
}

NormalFlowCalculator::NormalFlowCalculator(const NormalFlowCalculator& theObj)
		:	threshold_(0.f),
			gaussGrad_(NULL)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"NormalFlowCalculator copy constructor not implemented");
}


NormalFlowCalculator::~NormalFlowCalculator(void)
{
	if (gaussGrad_ != NULL)
		delete gaussGrad_;
}



const NormalFlowCalculator& NormalFlowCalculator::operator = (const NormalFlowCalculator& theObj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"NormalFlowCalculator copy operator not implemented");
	return *this;
}



//------------------------------------------------------------------------------
//	public functions
//------------------------------------------------------------------------------

#if 0
//----------------------------------------
#pragma mark public functions
//----------------------------------------
#endif


RasterImage_gray_F* NormalFlowCalculator::applyTo(const RasterImage* currImg,
												  const RasterImage* prevImg)
{
	RasterImage_gray_F* normalFlow = new RasterImage_gray_F(currImg->getHeight(), currImg->getWidth()); 

	applyInto(currImg, prevImg, normalFlow);
	return normalFlow;
}



RasterImage_gray_F* NormalFlowCalculator::applyTo(const RasterImage* currImg,
												  const RasterImage* prevImg,
												  const ImageRect* theRect)
{
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "null ImageRect as parameter in NormalFlowCalculator::applyTo");

	RasterImage_gray_F* normalFlow = new RasterImage_gray_F(theRect); 

	applyInto(currImg, prevImg, theRect, normalFlow);
	return normalFlow;
}



RasterImage_gray_F* NormalFlowCalculator::applyTo(const RasterImage* currImg,
												  const RasterImage* prevImg,
										  	 	  const VectorField_F* grad)
{
	RasterImage_gray_F* normalFlow = new RasterImage_gray_F(currImg->getHeight(), currImg->getWidth()); 

	applyInto(currImg, prevImg, grad, normalFlow);
	return normalFlow;
}



RasterImage_gray_F* NormalFlowCalculator::applyTo(const RasterImage* currImg,
												  const RasterImage* prevImg,
										  	 	  const VectorField_F* grad,
										  	 	  const ImageRect* theRect)
{
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "null ImageRect as parameter in NormalFlowCalculator::applyTo");

	RasterImage_gray_F* normalFlow = new RasterImage_gray_F(theRect); 

	applyInto(currImg, prevImg, grad, theRect, normalFlow);
	return normalFlow;
}



void NormalFlowCalculator::applyInto(const RasterImage* currImg,
									 const RasterImage* prevImg,
									 RasterImage_gray_F* normalFlow)
{
	applyInto(currImg, prevImg, currImg->getValidRect(), normalFlow);
}



void NormalFlowCalculator::applyInto(const RasterImage* currImg,
									 const RasterImage* prevImg,
									 const ImageRect* theRect,
									 RasterImage_gray_F* normalFlow)
{
	//  compute gradient
	const VectorField_F* grad = (const VectorField_F *) (gaussGrad_->applyTo(prevImg));
    FAIL_CONDITION( grad == NULL, 
                    kNullRasterImageError, 
                    "null VectorField in NormalFlowCalculator::applyInto");

	applyInto(currImg, prevImg, grad, theRect, normalFlow);

    //  clean up local allocations
	delete grad;
}



void NormalFlowCalculator::applyInto(const RasterImage* currImg,
									 const RasterImage* prevImg,
						 			 const VectorField_F* grad,
						 			 RasterImage_gray_F* normalFlow)
{
	applyInto(currImg, prevImg, grad, currImg->getValidRect(), normalFlow);
}



void NormalFlowCalculator::applyInto(const RasterImage* currImg,
									 const RasterImage* prevImg,
						 			 const VectorField_F* grad,
						 			 const ImageRect* theRect,
						 			 RasterImage_gray_F* normalFlow)
{
    FAIL_CONDITION( currImg == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage as parameter in NormalFlowCalculator::applyInto");
    FAIL_CONDITION( prevImg == NULL, 
                    kNullParameterError, 
                    "null RasterImage as parameter in NormalFlowCalculator::applyInto");
    FAIL_CONDITION( grad == NULL, 
                    kNullRasterImageError, 
                    "null VectorField in NormalFlowCalculator::applyInto");
    FAIL_CONDITION( theRect == NULL, 
                    kNullRectangleError, 
                    "null ImageRect as parameter in NormalFlowCalculator::applyInto");
    FAIL_CONDITION( normalFlow == NULL, 
                    kNullRasterImageError, 
                    "null RasterImage_gray_F in NormalFlowCalculator::applyInto");


    //	get type information
    const int type1 = currImg->getBaseRasterType(),
			  type2 = prevImg->getBaseRasterType();
    const bool	inFloat1 = currImg->hasFloatRaster(),
				inFloatRef1 = currImg->floatRasterIsReference(),
				inFloat2 = prevImg->hasFloatRaster(),
				inFloatRef2 = prevImg->floatRasterIsReference();


    //	if currImg and prevImg have the same base raster type
    if (type1 == type2)
    {
    	switch (type1)
    	{
    		case kGrayRaster:
                if(inFloat1 && inFloatRef1)
	                if(inFloat2 && inFloatRef2)
                		applyInto_((RasterImage_gray_F*)currImg, (RasterImage_gray_F*)prevImg, grad, theRect, normalFlow);
                	else
                		applyInto_((RasterImage_gray* )prevImg, (RasterImage_gray_F *)currImg, grad, theRect, normalFlow);
                else{
                	if(inFloat2 && inFloatRef2)
                		applyInto_((RasterImage_gray* )currImg, (RasterImage_gray_F *)prevImg, grad, theRect, normalFlow);
                	else
                		applyInto_((RasterImage_gray* )currImg, (RasterImage_gray* )prevImg, grad, theRect, normalFlow);
    			}
    			break;
    			
    		case kRGB24Raster:
    			applyInto_((RasterImage_RGB* )currImg, (RasterImage_RGB* )prevImg, grad, theRect, normalFlow);
 	            break;

            case kRGBa32Raster:
                applyInto_((RasterImage_RGBa* )currImg, (RasterImage_RGBa* )prevImg, grad, theRect, normalFlow);
                break;

            default:
                FAIL_CONDITION( true,
                                kRasterImageError,
                                "Invalid image type in NormalFlowCalculator::applyInto");
                break;
    	}
    }
    //	if type different, convert them to be the same
    else
    {
        //	if one is a RasterImage_gray, convert the other to RasterImage_gray
        if (type1 == kGrayRaster)
        {
            RasterImage_gray* gray2 = new RasterImage_gray(prevImg);
            if(inFloat1 && inFloatRef1)
            	applyInto_(gray2, (RasterImage_gray_F *)currImg, grad, theRect, normalFlow);
            else
            	applyInto_((RasterImage_gray* )currImg, gray2, grad, theRect, normalFlow);
            delete gray2;
        }
        else if (type2 == kGrayRaster)
        {
            RasterImage_gray	*gray1 = new RasterImage_gray(currImg);
            if(inFloat2 && inFloatRef2)
            	applyInto_(gray1, (RasterImage_gray_F *)prevImg, grad, theRect, normalFlow);
            else
            	applyInto_((RasterImage_gray* )prevImg, gray1, grad, theRect, normalFlow);
            delete gray1;
        }
        //	else if one is RGB and the other RGBa, convert the former to RGBa
        else if (type1 == kRGBa32Raster)
        {
            RasterImage_RGBa* rgba2 = new RasterImage_RGBa(prevImg);
            applyInto_((RasterImage_RGBa* )currImg, rgba2, grad, theRect, normalFlow);
            delete rgba2;
        }
        else if (type2 == kRGBa32Raster)
        {
            RasterImage_RGBa* rgba1 = new RasterImage_RGBa(currImg);
            applyInto_(rgba1, (RasterImage_RGBa* )prevImg, grad, theRect, normalFlow);
            delete rgba1;
        }
        else	//	this should not happen
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in NormalFlowCalculator::applyInto");
		
    }
}


//------------------------------------------------------------------------------
//	Private and protected functions
//------------------------------------------------------------------------------

#if 0
//----------------------------------------
#pragma mark private functions
//----------------------------------------
#endif



/*	the RasterImage_gray case	*/
void NormalFlowCalculator::applyInto_(const RasterImage_gray* currImg,
									  const RasterImage_gray* prevImg,
									  const VectorField_F* grad,
									  const ImageRect* theRect,
									  RasterImage_gray_F* normalFlow)
{
    const unsigned char*const*	im1	= (const unsigned char* const*) currImg->getShiftedRaster2D();
    const unsigned char*const*	im2	= (const unsigned char* const*) prevImg->getShiftedRaster2D();
	const float*const*	ng	= grad->getNormAsRasterImage()->getShiftedGrayF2D();
	float* const*	mv	= normalFlow->getShiftedGrayF2D(R_W_ACCESS);
	
	
	/*	get display rectangle	*/
	ImageRect* rect1		= ImageRect::intersection(currImg->getValidRect(), prevImg->getValidRect());
	ImageRect* rect2		= ImageRect::intersection(rect1, grad->getValidRect());
	ImageRect* destRect	= ImageRect::intersection(rect2, theRect);
	
	FAIL_CONDITION( destRect == NULL,
	                kNullRectangleError,
	                "Computation rectangle specified does not contain valid data");
	                
    //	if normalFlow does not contain the destRect, resize it
    if (!normalFlow->contains(destRect))
        normalFlow->setBoundRect(destRect);
    /*	end of get display rectangle	*/
	                
	                
    const int iLow	= destRect->getTop(),
              iHigh	= destRect->getBottom(),
			  jLow	= destRect->getLeft(),
              jHigh	= destRect->getRight();

	for(int i=iLow; i<=iHigh; i++)
		for(int j=jLow; j<=jHigh; j++)
		{
			if(ng[i][j] > threshold_)
			{
				float temp = (im1[i][j] - im2[i][j])/ng[i][j];
				mv[i][j] = (temp > 0 ? temp:-temp);
			}
			else
				mv[i][j] = 0;
		}
	
	normalFlow->requestRasterUpdate();
	normalFlow->setValidRect(destRect);
	normalFlow->setFloatConversionMode(kPositiveFloat);

    //  clean up local allocations
	delete rect1;
	delete rect2;
	delete destRect;
}

//
///*	the RasterImage_gray & RasterImage_gray_F case	*/
//void NormalFlowCalculator::applyInto_(const RasterImage_gray* currImg,
//									  const RasterImage_gray_F* prevImg,
//									  const VectorField_F *grad,
//									  const ImageRect* theRect,
//									  RasterImage_gray_F* normalFlow)
//{
//    const float	*const* im1	= currImg->getShiftedGray2D();
//    const float	*const* im2	= prevImg->getShiftedGray2D();
//	const float	*const* ng	= grad->getNormAsRasterImage()->getShiftedGrayF2D();
//	float* const* mv	= normalFlow->getShiftedGrayF2D(R_W_ACCESS);
//	
//
//    const unsigned char*const*	im1	= (unsigned char** ) currImg->getShiftedRaster2D();
//    const float*const*			im2	= (const float* const*) prevImg->getShiftedRaster2D();
//	const float*const*			ng	= (const float* const*) grad->getNormAsRasterImage()->getShiftedGrayF2D();
//	float*const*			mv	= (const float* const*) normalFlow->getShiftedGrayF2D(R_W_ACCESS);
//	
//	
//	/*	get display rectangle	*/
//	ImageRect   *rect1, *rect2, *destRect;
//	rect1		= ImageRect::intersection(currImg->getValidRect(), prevImg->getValidRect());
//	rect2		= ImageRect::intersection(rect1, grad->getValidRect());
//	destRect	= ImageRect::intersection(rect2, theRect);
//	
//	FAIL_CONDITION( destRect == NULL,
//	                kNullRectangleError,
//	                "Computation rectangle specified does not contain valid data");
//	                
//    //	if normalFlow does not contain the destRect, resize it
//    if (!normalFlow->contains(destRect))
//        normalFlow->setBoundRect(destRect);
//    /*	end of get display rectangle	*/
//	                
//	                
//    int   iLow	= destRect->getTop(),
//            iHigh	= destRect->getBottom(),
//            jLow	= destRect->getLeft(),
//            jHigh	= destRect->getRight();
//
//	for(int i=iLow; i<=iHigh; i++)
//		for(int j=jLow; j<=jHigh; j++)
//		{
//			if(ng[i][j] > threshold_)
//			{
//				float temp = (im1[i][j] - im2[i][j])/ng[i][j];
//				mv[i][j] = (temp > 0 ? temp:-temp);
//			}
//			else
//				mv[i][j] = 0;
//		}
//	
//	normalFlow->requestRasterUpdate();
//	normalFlow->setValidRect(destRect);
//	normalFlow->setFloatConversionMode(kPositiveFloat);
//
//    //  clean up local allocations
//	delete rect1;
//	delete rect2;
//	delete destRect;
//}


/*	the RasterImage_gray_F case	*/
void NormalFlowCalculator::applyInto_(const RasterImage_gray_F* currImg,
									  const RasterImage_gray_F* prevImg,
									  const VectorField_F *grad,
									  const ImageRect* theRect,
									  RasterImage_gray_F* normalFlow)
{
    const float	*const* im1	= currImg->getShiftedGrayF2D();
    const float	*const* im2	= prevImg->getShiftedGrayF2D();
	const float	*const* ng	= grad->getNormAsRasterImage()->getShiftedGrayF2D();
	float* const* mv	= normalFlow->getShiftedGrayF2D(R_W_ACCESS);
	
	
	/*	get display rectangle	*/
	ImageRect   *rect1, *rect2, *destRect;
	rect1		= ImageRect::intersection(currImg->getValidRect(), prevImg->getValidRect());
	rect2		= ImageRect::intersection(rect1, grad->getValidRect());
	destRect	= ImageRect::intersection(rect2, theRect);
	
	FAIL_CONDITION( destRect == NULL,
	                kNullRectangleError,
	                "Computation rectangle specified does not contain valid data");
	                
    //	if normalFlow does not contain the destRect, resize it
    if (!normalFlow->contains(destRect))
        normalFlow->setBoundRect(destRect);
    /*	end of get display rectangle	*/
	                
	                
    int   iLow	= destRect->getTop(),
            iHigh	= destRect->getBottom(),
            jLow	= destRect->getLeft(),
            jHigh	= destRect->getRight();

	for(int i=iLow; i<=iHigh; i++)
		for(int j=jLow; j<=jHigh; j++)
		{
			if(ng[i][j] > threshold_)
			{
				float temp = (im1[i][j] - im2[i][j])/ng[i][j];
				mv[i][j] = (temp > 0 ? temp:-temp);
			}
			else
				mv[i][j] = 0;
		}
	
	normalFlow->requestRasterUpdate();
	normalFlow->setValidRect(destRect);
	normalFlow->setFloatConversionMode(kPositiveFloat);

    //  clean up local allocations
	delete rect1;
	delete rect2;
	delete destRect;
}


/*	the RasterImage_RGBa case	*/
void NormalFlowCalculator::applyInto_(const RasterImage_RGBa* currImg,
									  const RasterImage_RGBa* prevImg,
									  const VectorField_F *grad,
									  const ImageRect* theRect,
									  RasterImage_gray_F* normalFlow)
{
    const unsigned char* const* im1	= currImg->getShiftedRaster2D(),
						*const* im2	= prevImg->getShiftedRaster2D();
	const float* const* ng	= grad->getNormAsRasterImage()->getShiftedGrayF2D();
	float* const* mv	= normalFlow->getShiftedGrayF2D(R_W_ACCESS);
	
	
	/*	get display rectangle	*/
	ImageRect   *rect1, *rect2, *destRect;
	rect1		= ImageRect::intersection(currImg->getValidRect(), prevImg->getValidRect());
	rect2		= ImageRect::intersection(rect1, grad->getValidRect());
	destRect	= ImageRect::intersection(rect2, theRect);
	
	FAIL_CONDITION( destRect == NULL,
	                kNullRectangleError,
	                "Computation rectangle specified does not contain valid data");
	                
    //	if normalFlow does not contain the destRect, resize it
    if (!normalFlow->contains(destRect))
        normalFlow->setBoundRect(destRect);
    /*	end of get display rectangle	*/
	                
	                
    const int   iLow	= destRect->getTop(),
                iHigh	= destRect->getBottom(),
                jLow	= destRect->getLeft(),
                jHigh	= destRect->getRight();

    //	use AVERAGE_DIFFERENCE as the default algorithm
	//	get the average value of R,G,B,a for each pixel
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
			if(ng[i][j] > threshold_)
			{
	            int avg = 0;  
	            for(int k=0; k<4; k++)  
	        	{
	        		int temp1 = im1[i][4*j+k] - im2[i][4*j+k];
	        		temp1 = temp1>0 ? temp1 : -temp1;		// normalize
	        		avg += temp1;
	        	}
	        	mv[i][j] = static_cast<unsigned char>(avg/(4*ng[i][j]));
	        }
			else
				mv[i][j] = 0;
        }
	
	normalFlow->requestRasterUpdate();
	normalFlow->setValidRect(destRect);
	normalFlow->setFloatConversionMode(kPositiveFloat);

    //  clean up local allocations
	delete rect1;
	delete rect2;
	delete destRect;
}


/*	the RasterImage_RGB case	*/
void NormalFlowCalculator::applyInto_(const RasterImage_RGB* currImg,
									  const RasterImage_RGB* prevImg,
									  const VectorField_F *grad,
									  const ImageRect* theRect,
									  RasterImage_gray_F* normalFlow)
{
    unsigned char	**im1	= (unsigned char** ) currImg->getShiftedRaster2D(),
            		**im2	= (unsigned char** ) prevImg->getShiftedRaster2D();
	float			**ng	= (float** ) grad->getNormAsRasterImage()->getShiftedGrayF2D(),
		 			**mv	= (float** ) normalFlow->getShiftedGrayF2D();
	
	
	/*	get display rectangle	*/
	ImageRect   *rect1, *rect2, *destRect;
	rect1		= ImageRect::intersection(currImg->getValidRect(), prevImg->getValidRect());
	rect2		= ImageRect::intersection(rect1, grad->getValidRect());
	destRect	= ImageRect::intersection(rect2, theRect);
	
	FAIL_CONDITION( destRect == NULL,
	                kNullRectangleError,
	                "Computation rectangle specified does not contain valid data");
	                
    //	if normalFlow does not contain the destRect, resize it
    if (!normalFlow->contains(destRect))
        normalFlow->setBoundRect(destRect);
    /*	end of get display rectangle	*/
	                
	                
    int   iLow	= destRect->getTop(),
            iHigh	= destRect->getBottom(),
            jLow	= destRect->getLeft(),
            jHigh	= destRect->getRight();

    //	use AVERAGE_DIFFERENCE as the default algorithm
	//	get the average value of R,G,B for each pixel
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
			if(ng[i][j] > threshold_)
			{
	            unsigned char avg = 0;  
	            for(int k=0; k<3; k++)  
	        	{
	        		int temp1 = im1[i][3*j+k] - im2[i][3*j+k];
	        		temp1 = temp1>0 ? temp1 : -temp1;		// normalize
	        		avg += temp1;
	        	}
	        	mv[i][j] = static_cast<unsigned char>(avg/(3*ng[i][j]));
	        }
			else
				mv[i][j] = 0;
        }
	
	normalFlow->requestRasterUpdate();
	normalFlow->setValidRect(destRect);
	normalFlow->setFloatConversionMode(kPositiveFloat);

    //  clean up local allocations
	delete rect1;
	delete rect2;
	delete destRect;
}
