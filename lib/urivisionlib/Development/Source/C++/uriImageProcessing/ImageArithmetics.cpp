/*  NAME:
        ImageArithmetics.cpp
 
    DESCRIPTION:
        ImageArithmetics source file
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ImageArithmetics.h"

using namespace uriVL;


RasterImage_gray* ImageArithmetics::difference(RasterImage* img1, RasterImage* img2,
                                                ColorDifferenceMode mode)

{
    FAIL_CONDITION( img1 == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ImageArithmetics::difference");
    FAIL_CONDITION( img2 == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ImageArithmetics::difference");

    //	The difference will be calculated over the the overlap of the two images'
    //	valid data rectangles.
    RasterImage_gray* imgDiff = NULL;
    ImageRect* diffRect = ImageRect::intersection(img1->getValidRect(), img2->getValidRect());
    FAIL_CONDITION(	diffRect == NULL,
                    kRasterDimensionsDontMatch,
                    "the input images in ImageArithmetics::difference don't overlap");

    //	Allocate a gray-level raster image at the output dimensions
    //	If one of the two input images has a gray-level raster as its reference, then the
    //	output image is of type RasterImage_gray_F, else RasterImage_gray
    if ( (img1->hasFloatRaster() && img1->floatRasterIsReference()) ||
            (img2->hasFloatRaster() && img2->floatRasterIsReference()) )
        imgDiff	= new RasterImage_gray_F(diffRect);
    else
        imgDiff	= new RasterImage_gray(diffRect);

    difference(img1, img2, imgDiff, mode);
    delete diffRect;
    return imgDiff;
}


void ImageArithmetics::difference(RasterImage* img1, RasterImage* img2, RasterImage_gray* imgOut,
                                   ColorDifferenceMode mode)
{
    FAIL_CONDITION( img1 == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ImageArithmetics::difference");
    FAIL_CONDITION( img2 == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ImageArithmetics::difference");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ImageArithmetics::difference");

    //	The difference will be calculated over the the overlap of the two images'
    //	valid data rectangles.
    ImageRect* diffRect = ImageRect::intersection(img1->getValidRect(), img2->getValidRect());
    FAIL_CONDITION(	diffRect == NULL,
                    kRasterDimensionsDontMatch,
                    "the input images in ImageArithmetics::difference don't overlap");
    //	if imgOut does not contain the diffRect, we resize imgOut
    if (!imgOut->contains(diffRect))
        imgOut->setBoundRect(diffRect);

    //  I set the valid data rect before actually calculating the difference because this rectangle
    //  will be used to determined the bounds of the difference calculation
    imgOut->setValidRect(diffRect);

    //	Get now all the information about the type of our input and output
    int		type1 = img1->getBaseRasterType(),
                 type2 = img2->getBaseRasterType();
    bool	inFloat1 = img1->hasFloatRaster(),
            inFloatRef1 = img1->floatRasterIsReference(),
            inFloat2 = img2->hasFloatRaster(),
            inFloatRef2 = img2->floatRasterIsReference(),
            outFloat = imgOut->hasFloatRaster();

    //	If img1 and img2 have the same base raster type, keep it that way
    if (type1 == type2)
    {
        switch (type1)
        {
            case kGrayRaster:
                //	If one of the input images has a float raster as reference *and* the
                //	output image has a float raster, then use the floating point difference
                if (outFloat && ((inFloat1 && inFloatRef1) || (inFloat2 && inFloatRef2)))
                {
                    //	if image 1 has a float raster as reference
                    if (inFloat1 && inFloatRef1)
                        if (inFloat2 && inFloatRef2)
                            difference_((RasterImage_gray_F	*) img1,
                                        (RasterImage_gray_F *) img2,
                                        (RasterImage_gray_F *) imgOut);

                    //	img2 is an integer raster
                        else
                            difference_((RasterImage_gray_F	*) img1,
                                        (RasterImage_gray* )   img2,
                                        (RasterImage_gray_F *) imgOut);

                    //	else image2 must have a float raster as reference
                    else
                        difference_((RasterImage_gray_F	*) img2,
                                    (RasterImage_gray* )   img1,
                                    (RasterImage_gray_F *) imgOut);

                }

                //	Otherwise we cast all images involved to RasterImage_gray
                else
                {
                    difference_((RasterImage_gray* ) img1, (RasterImage_gray* ) img2, imgOut);
                }
                break;

            case kRGB24Raster:
                difference_((RasterImage_RGB* ) img1, (RasterImage_RGB* ) img2, imgOut, mode);
                break;

            case kRGBa32Raster:
                difference_((RasterImage_RGBa* ) img1, (RasterImage_RGBa* ) img2, imgOut, mode);
                break;

            default:
                FAIL_CONDITION( true,
                                kRasterImageError,
                                "Invalid image type in ImageArithmetics::difference_");
                break;

        }
    }
    else
    {
        //	if one is a RasterImage_gray, convert the other to RasterImage_gray
        if (type1 == kGrayRaster)
        {
            //	we create a gray-level copy of img2 and call this function again with
            //	the new images.

            if (inFloat1 && inFloatRef1)
            {
                RasterImage_gray_F	*flGray2 = new RasterImage_gray_F(img2);

                if (outFloat)
                    difference_((RasterImage_gray_F *) img1, flGray2,
                                (RasterImage_gray_F *) imgOut);
                else
                    difference_((RasterImage_gray_F *) img1, flGray2, imgOut);

                delete flGray2;
            }
            else
            {
                RasterImage_gray	*gray2 = new RasterImage_gray(img2);

                difference_((RasterImage_gray* ) img1, gray2, imgOut);
                delete gray2;
            }
        }
        else if (type2 == kGrayRaster)
        {
            //	we create a gray-level copy of img1 and call this function again with
            //	the new images.

            if (inFloat2 && inFloatRef2)
            {
                RasterImage_gray_F	*flGray1 = new RasterImage_gray_F(img1);

                if (outFloat)
                    difference_(flGray1, (RasterImage_gray_F *) img2,
                                (RasterImage_gray_F *) imgOut);
                else
                    difference_(flGray1, (RasterImage_gray_F *) img2, imgOut);

                delete flGray1;
            }
            else
            {
                RasterImage_gray	*gray1 = new RasterImage_gray(img1);
                difference_(gray1, (RasterImage_gray* ) img2, imgOut);
                delete gray1;
            }
        }
        //	else if one is RGB and the other RGBa, convert the former to RGBa
        else if (type1 == kRGBa32Raster)
        {
            RasterImage_RGBa	*rgba2 = new RasterImage_RGBa(img2);
            difference_((RasterImage_RGBa* ) img1, rgba2, imgOut, mode);
            delete rgba2;
        }
        else if (type2 == kRGBa32Raster)
        {
            RasterImage_RGBa	*rgba1 = new RasterImage_RGBa(img1);
            difference_(rgba1, (RasterImage_RGBa* ) img2, imgOut, mode);
            delete rgba1;
        }
        else	//	this should not happen
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "Invalid image type in ImageArithmetics::difference_");

    }

    delete diffRect;
}


//---------------------------------
//  Private and protected functions
//---------------------------------
#if 0
#pragma mark -
#endif



void ImageArithmetics::difference_(RasterImage_gray* img1, RasterImage_gray* img2,
                                    RasterImage_gray* imgOut)
{
    const ImageRect* diffRect = imgOut->getValidRect();
    const unsigned char* const* im1 = img1->getShiftedGray2D();
    const unsigned char* const* im2 = img2->getShiftedGray2D();
    unsigned char*const* diff = imgOut->getShiftedGray2D(R_W_ACCESS);

    const int   iLow  = diffRect->getTop(),
                iHigh = diffRect->getBottom(),
                jLow = diffRect->getLeft(),
                jHigh = diffRect->getRight();
    int pDiff;
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            if ((pDiff = im2[i][j] - im1[i][j]) < 0)
                diff[i][j] = static_cast<unsigned char>(-pDiff);
            else
                diff[i][j] = static_cast<unsigned char>(pDiff);
        }

    imgOut->setValidRect(diffRect);
}


void ImageArithmetics::difference_(RasterImage_gray_F *img1, RasterImage_gray* img2,
                                    RasterImage_gray_F *imgOut)
{
    const ImageRect* diffRect = imgOut->getValidRect();
    const float* const* im1 = img1->getShiftedGrayF2D();
    const unsigned char* const* im2 = img2->getShiftedGray2D();
    float* const* diff = imgOut->getShiftedGrayF2D(R_W_ACCESS);

    const int   iLow  = diffRect->getTop(),
                iHigh = diffRect->getBottom(),
                jLow = diffRect->getLeft(),
                jHigh = diffRect->getRight();
    float pDiff;
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            if ((pDiff = im2[i][j] - im1[i][j]) < 0)
                diff[i][j] = -pDiff;
            else
                diff[i][j] = pDiff;
        }

    imgOut->setFloatConversionMode(kPositiveFloat);
}

void ImageArithmetics::difference_(RasterImage_gray_F *img1, RasterImage_gray* img2,
                                    RasterImage_gray* imgOut)
{
    const ImageRect* diffRect = imgOut->getValidRect();
    const float* const* im1 = img1->getShiftedGrayF2D();
    const unsigned char* const* im2 = img2->getShiftedRaster2D();
	unsigned char* const* diff = imgOut->getShiftedGray2D(R_W_ACCESS);

    const int   iLow  = diffRect->getTop(),
                iHigh = diffRect->getBottom(),
                jLow = diffRect->getLeft(),
                jHigh = diffRect->getRight();

    float pDiff;
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            if ((pDiff = im2[i][j] - im1[i][j]) < 0)
                diff[i][j] = static_cast<unsigned char>(-pDiff);
            else
                diff[i][j] = static_cast<unsigned char>(pDiff);
        }
}

void ImageArithmetics::difference_(RasterImage_gray_F *img1, RasterImage_gray_F *img2,
                                    RasterImage_gray_F *imgOut)
{
    const ImageRect* diffRect = imgOut->getValidRect();
    const float	*const* im1 = img1->getShiftedGrayF2D();
    const float	*const* im2 = img2->getShiftedGrayF2D();
    float* const* diff = imgOut->getShiftedGrayF2D(R_W_ACCESS);

    const int   iLow  = diffRect->getTop(),
                iHigh = diffRect->getBottom(),
                jLow = diffRect->getLeft(),
                jHigh = diffRect->getRight();

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            float	pDiff = (im2[i][j] - im1[i][j]);
            if (pDiff < 0)
                diff[i][j] = -pDiff;
            else
                diff[i][j] = pDiff;
        }
    imgOut->setFloatConversionMode(kPositiveFloat);
}

void ImageArithmetics::difference_(RasterImage_gray_F *img1, RasterImage_gray_F *img2,
                                    RasterImage_gray* imgOut)
{
    const ImageRect* diffRect = imgOut->getValidRect();
    const float	*const* im1 = img1->getShiftedGrayF2D();
    const float	*const* im2 = img2->getShiftedGrayF2D();
    unsigned char* const* diff = imgOut->getShiftedGray2D(R_W_ACCESS);

    const int   iLow  = diffRect->getTop(),
                iHigh = diffRect->getBottom(),
                jLow = diffRect->getLeft(),
                jHigh = diffRect->getRight();

    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
        {
            char	pDiff = static_cast<char> (im2[i][j] - im1[i][j]);
            diff[i][j] = (pDiff < 0) ? static_cast<unsigned char>(-pDiff) : static_cast<unsigned char>(pDiff);
        }
}



void ImageArithmetics::difference_(RasterImage_RGB* img1, RasterImage_RGB* img2, 
                                    RasterImage_gray* imgOut, ColorDifferenceMode mode)
{
    const ImageRect* diffRect = imgOut->getValidRect();
  
    const unsigned char* const* im1 = img1->getShiftedRGB2D();
    const unsigned char* const* im2 = img2->getShiftedRGB2D();
    unsigned char* const* diff = imgOut->getShiftedGray2D(R_W_ACCESS);
				    
    int         i, j, k, avg, temp;
    const int   iLow	= diffRect->getTop(),
                iHigh	= diffRect->getBottom(),
                jLow	= diffRect->getLeft(),
                jHigh	= diffRect->getRight();

    switch (mode)
    {
        case AVERAGE_DIFFERENCE:
        
        	//	get the average value of R,G,B for each pixel
		    for (i=iLow; i<=iHigh; i++)
		        for (j=jLow, k=3*jLow; j<=jHigh; j++, k++)
		        {
		            temp = im1[i][k] - im2[i][k];      //  red
		        	avg = temp>0 ? temp : -temp;     // absolute value
                    
		            k++;
		            temp = im1[i][k] - im2[i][k];    //  green
		        	avg += temp>0 ? temp : -temp;    // absolute value
                    
		            k++;
		            temp = im1[i][k] - im2[i][k];    //  blue
		        	avg += temp>0 ? temp : -temp;    // absolute value
                    
		        	diff[i][j] = static_cast<unsigned char>((avg+1)/3);
		        }
		    break;


        case MAX_DIFFERENCE:
        	//	get the max value of R,G,B for each pixel
		    for (i=iLow; i<=iHigh; i++)
		        for (j=jLow, k=3*jLow; j<=jHigh; j++, k++)
		        {
		            int maxv = 0;  

		            temp = im1[i][k] - im2[i][k];      //  red
		        	if (temp < 0)
		        	    temp = -temp;   // absolute value
	        		if(temp > maxv)
	        			maxv = temp;
                    
		            k++;
		            temp = im1[i][k] - im2[i][k];      //  green
		        	if (temp < 0)
		        	    temp = -temp;   // absolute value
	        		if(temp > maxv)
	        			maxv = temp;
                    
		            k++;
		            temp = im1[i][k] - im2[i][k];      //  blue
		        	if (temp < 0)
		        	    temp = -temp;   // absolute value
	        		if(temp > maxv)
	        			maxv = temp;
					
		        	diff[i][j] = static_cast<unsigned char>(maxv);
		        }
		    break;


        case HUE_DIFFERENCE:
            {

                RasterImage_HSV* hsv1 = new RasterImage_HSV(img1),
                                *hsv2 = new RasterImage_HSV(img2);
                FAIL_CONDITION( hsv1 == NULL,
                                kRasterAllocationFailure,
                                "HSV raster allocation failed in ImageArithmetics::difference");
                FAIL_CONDITION( hsv2 == NULL,
                                kRasterAllocationFailure,
                                "HSV raster allocation failed in ImageArithmetics::difference");

                //  calculate hue difference for each pixel, scale to [0, 255], store in imgOut
				const int* const* hue1 = hsv1->getShiftedHue2D();
                const int* const* hue2 = hsv2->getShiftedHue2D();
				
				float scale = 256.f / 360.f;
				
			    for (i=iLow; i<=iHigh; i++)
			        for (j=jLow; j<=jHigh; j++)
			        {
			            temp = (hue1[i][j] - hue2[i][j]);
			            if (temp < 0)
                            temp = -temp;
			            if (temp > 180)
                            temp = 360-temp;
			            diff[i][j] = static_cast<unsigned char>(scale*temp);
			        }

                delete hsv1;
                delete hsv2;
            }
            break;

        default:    //	none of the above
        	FAIL_CONDITION(true,
        					kInvalidParameterError,
        					"Invalid mode in ImageArithmetics::difference_");
            break;
    }
}

void ImageArithmetics::difference_(RasterImage_RGBa* img1, RasterImage_RGBa* img2, RasterImage_gray* imgOut,
                                    ColorDifferenceMode mode)
{
    const ImageRect* diffRect = imgOut->getValidRect();
  
    const unsigned char* const* im1 = img1->getShiftedRGBa2D();
    const unsigned char* const* im2 = img2->getShiftedRGBa2D();
    unsigned char* const* diff	= imgOut->getShiftedGray2D(R_W_ACCESS);

    int         i, j, k, avg, temp;
    const int   iLow	= diffRect->getTop(),
                iHigh	= diffRect->getBottom(),
                jLow	= diffRect->getLeft(),
                jHigh	= diffRect->getRight();

    switch (mode)
    {
        case AVERAGE_DIFFERENCE:
        
        	//	get the average value of R,G,B for each pixel
		    for (i=iLow; i<=iHigh; i++)
		        for (j=jLow, k=4*jLow; j<=jHigh; j++, k+=2)
		        {
					temp = im1[i][k] - im2[i][k];      //  red
		        	avg = temp>0 ? temp : -temp;     // absolute value
                    
		            k++;
		            temp = im1[i][k] - im2[i][k];    //  green
		        	avg += temp>0 ? temp : -temp;    // absolute value
                    
		            k++;
		            temp = im1[i][k] - im2[i][k];    //  blue
		        	avg += temp>0 ? temp : -temp;    // absolute value
					
		        	diff[i][j] = (unsigned char) ((avg+1)/3);
		        }
		    break;


        case MAX_DIFFERENCE:
        	//	get the max value of R,G,B for each pixel
		    for (i=iLow; i<=iHigh; i++)
		        for (j=jLow, k=4*jLow; j<=jHigh; j++, k+=2)
		        {
		            int maxv = 0;  

		            temp = im1[i][k] - im2[i][k];      //  red
		        	if (temp < 0)
		        	    temp = -temp;   // absolute value
	        		if(temp > maxv)
	        			maxv = temp;
                    
		            k++;
		            temp = im1[i][k] - im2[i][k];      //  green
		        	if (temp < 0)
		        	    temp = -temp;   // absolute value
	        		if(temp > maxv)
	        			maxv = temp;
                    
		            k++;
		            temp = im1[i][k] - im2[i][k];      //  blue
		        	if (temp < 0)
		        	    temp = -temp;   // absolute value
	        		if(temp > maxv)
	        			maxv = temp;

		        	diff[i][j] = static_cast<unsigned char>(maxv);
		        }
		    break;


        case HUE_DIFFERENCE:
            {

                RasterImage_HSV* hsv1 = new RasterImage_HSV(img1),
                                *hsv2 = new RasterImage_HSV(img2);
                FAIL_CONDITION( hsv1 == NULL,
                                kRasterAllocationFailure,
                                "HSV raster allocation failed in ImageArithmetics::difference");
                FAIL_CONDITION( hsv2 == NULL,
                                kRasterAllocationFailure,
                                "HSV raster allocation failed in ImageArithmetics::difference");

                //  calculate hue difference for each pixel, scale to [0, 255], store in imgOut
				const int* const* hue1 = hsv1->getShiftedHue2D();
			    const int* const* hue2 = hsv2->getShiftedHue2D();
				unsigned char* const* sat1 = hsv1->getShiftedSat2D(R_W_ACCESS),
							  *const* sat2 = hsv2->getShiftedSat2D(R_W_ACCESS);
				float scale = 256 / 360.f;
				
			    for (i=iLow; i<=iHigh; i++)
			        for (j=jLow; j<=jHigh; j++)
			        {
			        	if ((sat1[i][j] > 30) && (sat2[i][j] > 30))
			        	{
				            temp = (hue1[i][j] - hue2[i][j]);
				            temp = temp > 0 ? temp : -temp;
				            temp = temp > 180 ? (360-temp) : temp;
				            //temp2 *= scale;
				        	diff[i][j] = static_cast<unsigned char>(temp * scale);
			        	}
			        	else
			        	{
			        		temp = sat1[i][j] - sat2[i][j];
			        		diff[i][j] = static_cast<unsigned char>(temp > 0 ? temp : -temp);
			        	}
			        }

                delete hsv1;
                delete hsv2;
                break;
            }

        default:    //	none of the above
        	FAIL_CONDITION(true,
        					kInvalidParameterError,
        					"Invalid mode in ImageArithmetics::difference_");
            break;
    }
}
