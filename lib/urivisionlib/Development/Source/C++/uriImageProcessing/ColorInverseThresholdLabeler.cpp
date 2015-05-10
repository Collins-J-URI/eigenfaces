/*  NAME:
        ColorInverseThresholdLabeler.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorInverseThresholdLabeler class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColorInverseThresholdLabeler.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray.h"


//using namespace uriBase;
//using namespace uriImageProcessing;
using namespace std;
using namespace uriVL;

/*------------------------------------------------------------------------------+
|	Default constructor for this class											|
+------------------------------------------------------------------------------*/
ColorInverseThresholdLabeler::ColorInverseThresholdLabeler(void)
{}

ColorInverseThresholdLabeler::ColorInverseThresholdLabeler(int theWidth, int theHeight)
						: Labeler(theWidth, theHeight)
{
}


/*------------------------------------------------------------------------------+
|	Destructor for this class													|
+------------------------------------------------------------------------------*/
ColorInverseThresholdLabeler::~ColorInverseThresholdLabeler(void)
{}


#if 0
#pragma mark -
#endif

/*------------------------------------------------------------------------------+
|	applies color thresholds to an image.										|
|	Writes the results of the thresholding in image's RGBA raster				|
+------------------------------------------------------------------------------*/

LabelRaster *ColorInverseThresholdLabeler::applyTo(const RasterImage* imgIn, const char* params, 
                                            bool connectNeighbor)
{
    return applyTo(imgIn, (const ColorSegParams *) params, connectNeighbor);
}

LabelRaster *ColorInverseThresholdLabeler::applyTo(const RasterImage* imgIn, const char* params,
                                            const ImageRect* theRect, bool connectNeighbor)
{
    return applyTo(imgIn, (const ColorSegParams *) params, theRect, connectNeighbor);
}

LabelRaster *ColorInverseThresholdLabeler::applyTo(const RasterImage* imgIn, const ColorSegParams *params, 
                                            bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorInverseThresholdLabeler::applyTo");

    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "output LabelRaster  allocation failed in ColorInverseThresholdLabeler::applyTo");

    applyInto_(imgIn, params, imgIn->getValidRect(), labelOut, connectNeighbor);

    return labelOut;
}

LabelRaster *ColorInverseThresholdLabeler::applyTo(const RasterImage* imgIn, const ColorSegParams *params,
                                            const ImageRect* theRect, bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ColorInverseThresholdLabeler::applyTo");

    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "output LabelRaster  allocation failed in GrayThresholdLabeler::applyTo");

    applyInto_(imgIn, params, theRect, labelOut, connectNeighbor);

    return labelOut;
}


#if 0
#pragma mark -
#endif

void ColorInverseThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                      LabelRaster *labelOut, bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorInverseThresholdLabeler::applyTo");

    applyInto(imgIn, params, imgIn->getValidRect(), labelOut, connectNeighbor);
}

void ColorInverseThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                      const ImageRect* theRect, LabelRaster *labelOut,
                                      bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ColorInverseThresholdLabeler::applyTo");

    applyInto_(imgIn, params, theRect, labelOut, connectNeighbor);
}


void ColorInverseThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                      LabelRaster *labelOut, bool connectNeighbor)
{
    applyInto(imgIn, (const ColorSegParams *) params, labelOut, connectNeighbor);
}


void ColorInverseThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                      const ImageRect* theRect, LabelRaster *labelOut,
                                      bool connectNeighbor)
{
    applyInto(imgIn, (const ColorSegParams *) params, theRect, labelOut, connectNeighbor);
}


void ColorInverseThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                      RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output RasterImage as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorInverseThresholdLabeler::applyTo");

    FAIL_CONDITION( imgOut->getBaseRasterType() != kRGBa32Raster,
                    kRasterImageError,
                    "only RGBa images accepted in ColorInverseThresholdLabeler::applyInto so far");

    applyInto_(imgIn, params, imgIn->getValidRect(), (RasterImage_RGBa* ) imgOut);
}

void ColorInverseThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                      const ImageRect* theRect, RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output RasterImage as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorInverseThresholdLabeler::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ColorInverseThresholdLabeler::applyTo");

    FAIL_CONDITION( imgOut->getBaseRasterType() != kRGBa32Raster,
                    kRasterImageError,
                    "only RGBa images accepted in ColorInverseThresholdLabeler::applyInto so far");

    applyInto_(imgIn, params, theRect, (RasterImage_RGBa* ) imgOut);
}

void ColorInverseThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                      RasterImage* imgOut)
{
    applyInto(imgIn, (const ColorSegParams *) params, imgOut);
}


void ColorInverseThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                      const ImageRect* theRect, RasterImage* imgOut)
{
    applyInto(imgIn, (const ColorSegParams *) params, theRect, imgOut);
}



#if 0
#pragma mark -
#endif
//---------------------------
//  Private and protected functions
//---------------------------

void ColorInverseThresholdLabeler::applyInto_(const RasterImage* imgIn, const ColorSegParams *params,
											   const ImageRect* theRect, LabelRaster *labelOut,
											   bool connectNeighbor)
{
    bool 			localHSV;
    const RasterImage_HSV* hsvImg = RasterImage_HSV::localRasterHSV(imgIn, &localHSV);

    ColorRegion 	*colorSought = params->colorReg;
    const unsigned char* const* sat2D = hsvImg->getShiftedSat2D(),
                             *const* val2D = hsvImg->getShiftedVal2D();
    char 			**label = labelOut->getShiftedLabel2D(R_W_ACCESS);
    int 			nbColors = params->nbModes;
    const int		*const* hue2D = hsvImg->getShiftedHue2D();
    int			nbRows = hsvImg->getHeight(),
                nbCols = hsvImg->getWidth(),
                iLow = theRect->getTop( ),
                iHigh = theRect->getBottom( ),
                jLow = theRect->getLeft( ),
                jHigh = theRect->getRight( );
	char		noMatchVal = params->noMatchVal;
    bool			matchFound;


    FAIL_CONDITION( (iLow < 0) || (iHigh > nbRows) || (jLow < 0) || (jHigh > nbCols),
                    kRasterDimensionsDontMatch,
                    "Invalid rectangle dimensions for RGB to HSV conversion");

    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            matchFound = false;
            for (int k=0; (k<nbColors) && !matchFound; k++)
                if (goodColor_(hue2D[i][j], sat2D[i][j], val2D[i][j], colorSought+k))
                {
                    label[i][j] = noMatchVal;
                    matchFound = true;
                }

            if (!matchFound)
            {
                label[i][j] = 1;
            }

        }
    }
    
    //	do connect-neighbor operation
    if (connectNeighbor)
    {
    	bool tempLabelIsLocal = false;
    	if (tempLabel_ == NULL)
    	{
    		tempLabel_ = new LabelRaster(theRect);
    		tempLabelIsLocal = true;    		
    	}
    	
    	connectNeighbor_(theRect, nbColors, labelOut, tempLabel_, noMatchVal);
    	
    	if (tempLabelIsLocal)
    	{
    		delete tempLabel_;
    		tempLabel_ = NULL;
    	}
    }

    //  cleanup if needed
    //	if the HSV rasters were a local copy, delete the image
    if (localHSV)
        delete hsvImg;

    labelOut->setValidRect(theRect);
}

void ColorInverseThresholdLabeler::applyInto_(const RasterImage* imgIn, const ColorSegParams *params,
                                       const ImageRect* theRect, RasterImage_RGBa* imgOut)
{
    bool 			localHSV;
    const RasterImage_HSV* hsvImg = RasterImage_HSV::localRasterHSV(imgIn, &localHSV);

    ColorRegion* colorSought = params->colorReg;
    const unsigned char* const* sat2D = hsvImg->getShiftedSat2D(),
						*const* val2D = hsvImg->getShiftedVal2D();
    unsigned char	*const* rgbaOut = imgOut->getShiftedRGBa2D(R_W_ACCESS);
    const unsigned char	*const* rasterIn = imgIn->getShiftedRaster2D();
    const BaseRasterType inType = imgIn->getBaseRasterType();
    const int   inDepth = imgIn->getPixelDepth()/8;

    int 		nbColors = params->nbModes;
    const int	*const* hue2D = hsvImg->getHue2D();
    const int	nbRows = hsvImg->getHeight(),
                nbCols = hsvImg->getWidth(),
                iLow = theRect->getTop( ),
                iHigh = theRect->getBottom( ),
                jLow = theRect->getLeft( ),
                jHigh = theRect->getRight( );
    bool		matchFound;
    int         i, j, k, l, m;
    FillingMode fillMode = params->fillMode;

    FAIL_CONDITION( (iLow < 0) || (iHigh > nbRows) || (jLow < 0) || (jHigh > nbCols),
                    kRasterDimensionsDontMatch,
                    "Invalid rectangle dimensions for RGB to HSV conversion");

    switch (fillMode)
    {
        //	The output is an unmodified copy of the input
        case kIdentityFill:
            //jyh   fix later
            //imgIn->copyInto(imgOut, imgIn->getType());
            imgIn->copyInto(imgOut);
            break;

            //	Simply overlay thresholded regions on the original image
        case kOrigColorFill:

            for (i=iLow; i<=iHigh; i++)
            {
                //  j is the pixel index in the hsv rasters
                //  l is the byte index in the output image
                //  m is the byte index in the input image
                for (j=jLow, l=4*jLow, m=inDepth*jLow; j<=jHigh; j++)
                {
                    matchFound = false;
                    for (k=0; (k<nbColors) && !matchFound; k++)
                        if (goodColor_(hue2D[i][j], sat2D[i][j], val2D[i][j], colorSought+k))
                        {
                            rgbaOut[i][l++] = colorSought[k].fillColor[0];  //  red
                            rgbaOut[i][l++] = colorSought[k].fillColor[1];  //  green
                            rgbaOut[i][l++] = colorSought[k].fillColor[2];  //  blue
                            rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                            m+=inDepth;
                            matchFound = true;
                        }

                    if (!matchFound)
                    {
                        switch (inType)
                        {
                            case kGrayRaster:
                                // I reworked this because it generates GCC
                                // compiler warnings, for good reasons:
                                // http://c-faq.com/expr/evalorder1.html
//                                 rgbaOut[i][l++] = rgbaOut[i][l++] =
//                                                       rgbaOut[i][l++] = rasterIn[i][m++];
//                                 rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                                {
                                    unsigned char c = rasterIn[i][m++];
                                    rgbaOut[i][l++] = c;
                                    rgbaOut[i][l++] = c;
                                    rgbaOut[i][l++] = c;
                                }
                                break;
                            case kRGB24Raster:
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = (unsigned char) 255;  //  alpha

                                break;

                            case kRGBa32Raster:
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];

                                break;

                            default:
                                break;

                        }
                    }

                }
            }
            break;

            //	Display the thresholded regions over a black background
        case kBlackFill:
            for (i=iLow; i<=iHigh; i++)
            {
                //  j is the pixel index in the hsv rasters
                //  l is the byte index in the output image
                for (j=jLow, l=4*jLow; j<=jHigh; j++)
                {
                    matchFound = false;
                    for (k=0; (k<nbColors) && !matchFound; k++)
                        if (goodColor_(hue2D[i][j], sat2D[i][j], val2D[i][j], colorSought+k))
                        {
                            rgbaOut[i][l++] = colorSought[k].fillColor[0];  //  red
                            rgbaOut[i][l++] = colorSought[k].fillColor[1];  //  green
                            rgbaOut[i][l++] = colorSought[k].fillColor[2];  //  blue
                            rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                            matchFound = true;
                        }
                    if (!matchFound)
                    {
                        rgbaOut[i][l++] = 0;
                        rgbaOut[i][l++] = 0;
                        rgbaOut[i][l++] = 0;
                        rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                    }
                }
            }

            break;

            //	Display the thresholded regions over a white background
        case kWhiteFill:

            for (i=iLow; i<=iHigh; i++)
            {
                //  j is the pixel index in the hsv rasters
                //  l is the byte index in the output image
                for (j=jLow, l=4*jLow; j<=jHigh; j++)
                {
                    matchFound = false;
                    for (k=1; (k<nbColors) && !matchFound; k++)
                        if (goodColor_(hue2D[i][j], sat2D[i][j], val2D[i][j], colorSought+k))
                        {
                            rgbaOut[i][l++] = colorSought[k].fillColor[0];  //  red
                            rgbaOut[i][l++] = colorSought[k].fillColor[1];  //  green
                            rgbaOut[i][l++] = colorSought[k].fillColor[2];  //  blue
                            rgbaOut[i][l++] = (unsigned char) 0;  //  alpha
                            matchFound = true;
                        }

                    if (!matchFound)
                    {
                        rgbaOut[i][l++] = (unsigned char) 255;
                        rgbaOut[i][l++] = (unsigned char) 255;
                        rgbaOut[i][l++] = (unsigned char) 255;
                        rgbaOut[i][l++] = (unsigned char) 255;
                    }
                }
            }
            break;

            //	Display the thresholded regions over a medium gray background
        case kMediumGrayFill:

            for (i=iLow; i<=iHigh; i++)
            {
                //  j is the pixel index in the hsv rasters
                //  l is the byte index in the output image
                for (j=jLow, l=4*jLow; j<=jHigh; j++)
                {
                    matchFound = false;
                    for (k=0; (k<nbColors) && !matchFound; k++)
                        if (goodColor_(hue2D[i][j], sat2D[i][j], val2D[i][j], colorSought+k))
                        {
                            rgbaOut[i][l++] = colorSought[k].fillColor[0];  //  red
                            rgbaOut[i][l++] = colorSought[k].fillColor[1];  //  green
                            rgbaOut[i][l++] = colorSought[k].fillColor[2];  //  blue
                            rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                            matchFound = true;
                        }


                    if (!matchFound)
                    {
                        rgbaOut[i][l++] = (unsigned char) 127;
                        rgbaOut[i][l++] = (unsigned char) 127;
                        rgbaOut[i][l++] = (unsigned char) 127;
                        rgbaOut[i][l++] = (unsigned char) 255;
                    }
                }
            }
            break;

            //	Display the thresholded regions over the original gray-level image
        case kOrigGrayFill:

            for (i=iLow; i<=iHigh; i++)
            {
                //  j is the pixel index in the hsv rasters
                //  l is the byte index in the output image
                //  m is the byet index in the input image
                for (j=jLow, l=4*jLow, m=inDepth*jLow; j<=jHigh; j++)
                {
                    matchFound = false;
                    for (k=0; (k<nbColors) && !matchFound; k++)
                        if (goodColor_(hue2D[i][j], sat2D[i][j], val2D[i][j], colorSought+k))
                        {
                            rgbaOut[i][l++] = colorSought[k].fillColor[0];  //  red
                            rgbaOut[i][l++] = colorSought[k].fillColor[1];  //  green
                            rgbaOut[i][l++] = colorSought[k].fillColor[2];  //  blue
                            rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                            matchFound = true;
                        }

                    if (!matchFound)
                    {
                        rgbaOut[i][l++] = val2D[i][j];
                        rgbaOut[i][l++] = val2D[i][j];
                        rgbaOut[i][l++] = val2D[i][j];
                        rgbaOut[i][l++] = (unsigned char) 255;
                    }
                }
            }

            break;

        default:
            break;

    }


    //  cleanup if needed
    //	if the HSV rasters were a local copy, delete the image
    if (localHSV)
        delete hsvImg;

    imgOut->setValidRect(theRect);
}



/*------------------------------------------------------------------------------+
|   Checks if the color (in HSV space) belongs to the color domain sought		|
+------------------------------------------------------------------------------*/
bool ColorInverseThresholdLabeler::goodColor_(int hue, unsigned char sat, unsigned char val,
                                       const ColorRegion *theColor)
{
    int	hueDiff = hue - theColor->hueRef;

    //--------------------------------
    //	first check the hue
    //--------------------------------
    //	If we get a large positive difference, it might be due to a modulus problem
    if ( hueDiff > theColor->deltaHue )
    {
        //	we are out of the hue wedge sought
        if ( (360 - hueDiff) > theColor->deltaHue )
            return false;
    }
    else if (hueDiff < -theColor->deltaHue )
    {
        //	we are out of the hue wedge sought
        if ( (360 + hueDiff) > theColor->deltaHue )
            return false;
    }

    //--------------------------------
    //	We passed the the hue test, now we check the saturation
    //--------------------------------
    if ( (sat < theColor->satMin) || (sat > theColor->satMax) )
        return false;

    //--------------------------------
    //	Finally we check the value
    //--------------------------------
    if ( (val < theColor->valMin) || (val > theColor->valMax) )
        return false;

    //	We passed all the tests
    return true;
}


void ColorInverseThresholdLabeler::connectNeighbor_(const ImageRect* destRect, int nbRegions,
											 LabelRaster *label, LabelRaster *tempLabel, char noMatchVal)
{
	const int	iLow = destRect->getTop( ),
                iHigh = destRect->getBottom( ),
                jLow = destRect->getLeft( ),
                jHigh = destRect->getRight( );
	const int	hT = 5,					// high threshold on number of neighbors
		        lT = 3;					// low threshold on number of neighbors

	char	**label2D = label->getShiftedLabel2D(R_W_ACCESS),
			**tempLabel2D = tempLabel->getShiftedLabel2D(R_W_ACCESS);
			
	// array of label values
	char	*labelArray = new char[nbRegions];
	int		*vote = new int[nbRegions];		// number of each pixel's same neighbor
	int		maxIndex = 0;					// index of vote that has max votes
	for (int i=0; i<nbRegions; i++)
	{
		labelArray[i] = static_cast<char>(i+1);				// label values begins from 1
		vote[i] = 0;
	}
	
	// peripheral points are not processed in this operation, so these values
	// are kept same between label2D and tempLabel2D
	for (int j=jLow; j<=jHigh; j++)
	{
		tempLabel2D[iLow][j] = label2D[iLow][j]; 
		tempLabel2D[iHigh][j] = label2D[iHigh][j]; 
	}
	for (int i=iLow; i<=iHigh; i++)
	{
		tempLabel2D[i][jLow] = label2D[i][jLow]; 
		tempLabel2D[i][jHigh] = label2D[i][jHigh]; 
	}

	// process internal points
	for (int i=iLow+1; i<iHigh; i++)
		for (int j=jLow+1; j<jHigh; j++)
		{
			for (int k=0; k<nbRegions; k++)
			{
				if (labelArray[k] == label2D[i-1][j-1])
					vote[k]++;
				if (labelArray[k] == label2D[i-1][j])
					vote[k]++;
				if (labelArray[k] == label2D[i-1][j+1])
					vote[k]++;
				if (labelArray[k] == label2D[i][j-1])
					vote[k]++;
				if (labelArray[k] == label2D[i][j+1])
					vote[k]++;
				if (labelArray[k] == label2D[i+1][j-1])
					vote[k]++;
				if (labelArray[k] == label2D[i+1][j])
					vote[k]++;
				if (labelArray[k] == label2D[i+1][j+1])
					vote[k]++;
					
				if (vote[k]>vote[maxIndex])
					maxIndex = k;
			}
			
			// decide the pixel value based upon vote value
			if (vote[maxIndex] >= hT)
				tempLabel2D[i][j] = labelArray[maxIndex];	// adapt it to majority
			else if (vote[maxIndex] >= lT)
				tempLabel2D[i][j] = label2D[i][j];			// keep its value
			else
				tempLabel2D[i][j] = noMatchVal;						// set to -1
				
			// initialize vote back to 0 for next loop
			for (int k=0; k<nbRegions; k++)
				vote[k] = 0;				
		}

	// copy tempLabel2D into label2D
	for (int i=iLow; i<=iHigh; i++)
		for (int j=jLow; j<=jHigh; j++)
		{
			label2D[i][j] = tempLabel2D[i][j];
		}
}
