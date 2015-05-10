/*  NAME:
        ColorThresholdLabeler.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ColorThresholdLabeler class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "ColorThresholdLabeler.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray.h"


using namespace uriVL;
using namespace std;

#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

/*------------------------------------------------------------------------------+
|	Default constructor for this class											|
+------------------------------------------------------------------------------*/
ColorThresholdLabeler::ColorThresholdLabeler(void)
{
	#if URIVL_LUT_COLOR_SEGM
		allocateSegmentationLUT_();	
	#endif
}

ColorThresholdLabeler::ColorThresholdLabeler(int theWidth, int theHeight)
						: Labeler(theWidth, theHeight)
{
	#if URIVL_LUT_COLOR_SEGM
		allocateSegmentationLUT_();	
	#endif
}


/*------------------------------------------------------------------------------+
|	Destructor for this class													|
+------------------------------------------------------------------------------*/
ColorThresholdLabeler::~ColorThresholdLabeler(void)
{
	#if URIVL_LUT_COLOR_SEGM
		delete []segmentationLUT_;
	#endif
}


#if 0
//----------------------------------------
#pragma mark -
#pragma mark applyTo functions
//----------------------------------------
#endif

/*------------------------------------------------------------------------------+
|	applies color thresholds to an image.										|
|	Writes the results of the thresholding in image's RGBA raster				|
+------------------------------------------------------------------------------*/

LabelRaster *ColorThresholdLabeler::applyTo(const RasterImage* imgIn, const char* params, 
                                            bool connectNeighbor)
{
    return applyTo(imgIn, (const ColorSegParams *) params, connectNeighbor);
}

LabelRaster *ColorThresholdLabeler::applyTo(const RasterImage* imgIn, const char* params,
                                            const ImageRect* theRect, bool connectNeighbor)
{
    return applyTo(imgIn, (const ColorSegParams *) params, theRect, connectNeighbor);
}

LabelRaster *ColorThresholdLabeler::applyTo(const RasterImage* imgIn, const ColorSegParams *params, 
                                            bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorThresholdLabeler::applyTo");

    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "output LabelRaster  allocation failed in ColorThresholdLabeler::applyTo");

    applyInto_(imgIn, params, imgIn->getValidRect(), labelOut, connectNeighbor);

    return labelOut;
}

LabelRaster *ColorThresholdLabeler::applyTo(const RasterImage* imgIn, const ColorSegParams *params,
                                            const ImageRect* theRect, bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ColorThresholdLabeler::applyTo");

    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "output LabelRaster  allocation failed in GrayThresholdLabeler::applyTo");

    applyInto_(imgIn, params, theRect, labelOut, connectNeighbor);

    return labelOut;
}


#if 0
//----------------------------------------
#pragma mark -
#pragma mark applyInto functions
//----------------------------------------
#endif

void ColorThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                      LabelRaster *labelOut, bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorThresholdLabeler::applyTo");

    applyInto(imgIn, params, imgIn->getValidRect(), labelOut, connectNeighbor);
}



void ColorThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                      const ImageRect* theRect, LabelRaster *labelOut,
                                      bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ColorThresholdLabeler::applyTo");

    applyInto_(imgIn, params, theRect, labelOut, connectNeighbor);
}



void ColorThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                      LabelRaster *labelOut, bool connectNeighbor)
{
    applyInto(imgIn, (const ColorSegParams *) params, labelOut, connectNeighbor);
}



void ColorThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                      const ImageRect* theRect, LabelRaster *labelOut,
                                      bool connectNeighbor)
{
    applyInto(imgIn, (const ColorSegParams *) params, theRect, labelOut, connectNeighbor);
}



void ColorThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams* params,
                                      RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output RasterImage as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorThresholdLabeler::applyTo");

    FAIL_CONDITION( imgOut->getBaseRasterType() != kRGBa32Raster,
                    kRasterImageError,
                    "only RGBa images accepted in ColorThresholdLabeler::applyInto so far");

    applyInto_(imgIn, params, imgIn->getValidRect(), (RasterImage_RGBa* ) imgOut);
}



void ColorThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                      const ImageRect* theRect, RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output RasterImage as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in ColorThresholdLabeler::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in ColorThresholdLabeler::applyTo");

    FAIL_CONDITION( imgOut->getBaseRasterType() != kRGBa32Raster,
                    kRasterImageError,
                    "only RGBa images accepted in ColorThresholdLabeler::applyInto so far");

    applyInto_(imgIn, params, theRect, (RasterImage_RGBa* ) imgOut);
}



void ColorThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                      RasterImage* imgOut)
{
    applyInto(imgIn, (const ColorSegParams *) params, imgOut);
}


void ColorThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                      const ImageRect* theRect, RasterImage* imgOut)
{
    applyInto(imgIn, (const ColorSegParams *) params, theRect, imgOut);
}



#if 0
#pragma mark -
//------------------------------------------------
#pragma mark Private and protected functions
//------------------------------------------------
#endif

void ColorThresholdLabeler::applyInto_(const RasterImage* imgIn, const ColorSegParams* params,
                                       const ImageRect* theRect, LabelRaster *labelOut,
                                       bool connectNeighbor)
{
    ImageRect* destRect = ImageRect::intersection(imgIn->getValidRect(), theRect);
    FAIL_CONDITION(	destRect == NULL,
                    kInvalidRectangleError,
                    "input image valid date rect and output rectangles must overlap");
    const int	iLow = destRect->getTop( ),
                iHigh = destRect->getBottom( ),
                jLow = destRect->getLeft( ),
                jHigh = destRect->getRight( );
	delete destRect;
	
    bool localHSV;
    const RasterImage_HSV* hsvImg = RasterImage_HSV::localRasterHSV(imgIn, &localHSV);

    ColorRegion* colorSought = params->colorReg;
    const unsigned char* const* sat2D = hsvImg->getShiftedSat2D();
    const unsigned char* const*val2D = hsvImg->getShiftedVal2D();
    char** label = labelOut->getShiftedLabel2D(R_W_ACCESS);
    const int* const* hue2D = hsvImg->getShiftedHue2D();
    const int 	nbColors = params->nbModes;
	const char	noMatchVal = params->noMatchVal;
    bool		matchFound;

    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            matchFound = false;
            for (int k=0; (k<nbColors) && !matchFound; k++)
                if (goodColor_(hue2D[i][j], sat2D[i][j], val2D[i][j], colorSought+k))
                {
                    label[i][j] = colorSought[k].label;
                    matchFound = true;
                }

            if (!matchFound)
            {
                label[i][j] = noMatchVal;
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
    	
    	connectNeighbors_(theRect, nbColors, labelOut, tempLabel_, noMatchVal);
    	
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

void ColorThresholdLabeler::applyInto_(const RasterImage* imgIn, const ColorSegParams *params,
                                       const ImageRect* theRect, RasterImage_RGBa* imgOut)
{
    ImageRect* destRect = ImageRect::intersection(imgIn->getValidRect(), theRect);
    FAIL_CONDITION(	destRect == NULL,
                    kInvalidRectangleError,
                    "input image valid date rect and output rectangles must overlap");
    const int	iLow = destRect->getTop( ),
                iHigh = destRect->getBottom( ),
                jLow = destRect->getLeft( ),
                jHigh = destRect->getRight( );
	delete destRect;
	
    bool localHSV;
    const RasterImage_HSV* hsvImg = RasterImage_HSV::localRasterHSV(imgIn, &localHSV);

    int nbColors = params->nbModes;
    ColorRegion* colorSought = params->colorReg;
    unsigned char*const* rgbaOut = imgOut->getShiftedRGBa2D(R_W_ACCESS);
    const unsigned char	*const* rasterIn = imgIn->getShiftedRaster2D();
    const BaseRasterType inType = imgIn->getBaseRasterType();
    const int   inDepth = imgIn->getPixelDepth()/8;
    const unsigned char* const* sat2D = hsvImg->getShiftedSat2D();
    const unsigned char* const*val2D = hsvImg->getShiftedVal2D();
    const int* const* hue2D = hsvImg->getShiftedHue2D();
	bool		matchFound;
    int         i, j, k, l, m;
    FillingMode fillMode = params->fillMode;

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
bool ColorThresholdLabeler::goodColor_(int hue, unsigned char sat, unsigned char val,
                                       const ColorRegion *theColor)
{
    int	hueDiff = hue - theColor->hueRef;
	if (hueDiff < 0)
		hueDiff = - hueDiff;

    //--------------------------------
    //	first check the hue
    //--------------------------------
    //	If we get a large difference, it might be due to a modulus problem
    if ( hueDiff > theColor->deltaHue )
    {
        //	we are out of the hue wedge sought
        if ( (360 - hueDiff) > theColor->deltaHue )
            return false;
    }
	//	otherwise we might still be good


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


void ColorThresholdLabeler::connectNeighbors_(const ImageRect* destRect, int nbRegions,
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

#if URIVL_LUT_COLOR_SEGM

	void ColorThresholdLabeler::allocateSegmentationLUT_(void)
	{
		const int SIZE_LUT = 256*256*256;

		segmentationLUT_ = new char[SIZE_LUT];
	}

	void ColorThresholdLabeler::initializeSegmentationLUT_(void)
	{

	}
	
#endif
