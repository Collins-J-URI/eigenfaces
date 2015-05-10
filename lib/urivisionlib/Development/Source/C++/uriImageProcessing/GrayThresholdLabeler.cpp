/*  NAME:
        GrayThresholdLabeler.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib GrayThresholdLabeler class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "GrayThresholdLabeler.h"
#include "RasterImage_gray.h"

using namespace uriVL;
using namespace std;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


GrayThresholdLabeler::GrayThresholdLabeler(void)
        :   Labeler()
{
}

GrayThresholdLabeler::GrayThresholdLabeler(int theWidth, int theHeight)
        :   Labeler(theWidth, theHeight)
{
}


GrayThresholdLabeler::~GrayThresholdLabeler(void)
{
}


//----------------------------------------------------------
//  Public funcs
//----------------------------------------------------------

LabelRaster *GrayThresholdLabeler::applyTo(const RasterImage* imgIn, const char* params, bool connectNeighbor)
{
    return applyTo(imgIn, (const ColorSegParams *)params, connectNeighbor);
}


LabelRaster *GrayThresholdLabeler::applyTo(const RasterImage* imgIn, const char* params,
                                           const ImageRect* theRect, bool connectNeighbor)
{
    return applyTo(imgIn, (const ColorSegParams*)params, theRect, connectNeighbor);
}


LabelRaster *GrayThresholdLabeler::applyTo(const RasterImage* imgIn, const ColorSegParams *params,
										   bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in GrayThresholdLabeler::applyTo");

    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION(	labelOut == NULL,
                    kRasterAllocationFailure,
                    "output LabelRaster  allocation failed in GrayThresholdLabeler::applyTo");

    applyInto_(imgIn, params, imgIn->getValidRect(), labelOut, connectNeighbor);

    return labelOut;
}


LabelRaster *GrayThresholdLabeler::applyTo(const RasterImage* imgIn, const ColorSegParams *params,
        								   const ImageRect* theRect, bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in GrayThresholdLabeler::applyTo");

    LabelRaster		*labelOut = new LabelRaster(imgIn->getValidRect());
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "output LabelRaster  allocation failed in GrayThresholdLabeler::applyTo");

    applyInto_(imgIn, params, theRect, labelOut, connectNeighbor);

    return labelOut;
}

/*------------------------------------------------------------------------------+
|	applies gray-level thresholds to an image.									|
|	Writes the results of the thresholding into another image's RGBA raster		|
+------------------------------------------------------------------------------*/

void GrayThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams* params,
                                     LabelRaster *labelOut, bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in GrayThresholdLabeler::applyTo");

    applyInto(imgIn, params, imgIn->getValidRect(), labelOut, connectNeighbor);
}


void GrayThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                     const ImageRect* theRect, LabelRaster *labelOut,
                                     bool connectNeighbor)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( labelOut == NULL,
                    kNullRasterImageError,
                    "null LabelRaster as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in GrayThresholdLabeler::applyTo");

    applyInto_(imgIn, params, theRect, labelOut, connectNeighbor);
}


void GrayThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                     LabelRaster *labelOut, bool connectNeighbor)
{
    applyInto(imgIn, (const ColorSegParams *) params, labelOut, connectNeighbor);
}


void GrayThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                     const ImageRect* theRect, LabelRaster *labelOut,
                                     bool connectNeighbor)
{
    applyInto(imgIn, (const ColorSegParams *) params, theRect, labelOut, connectNeighbor);
}


void GrayThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                     RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output RasterImage as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in GrayThresholdLabeler::applyTo");

    //  we accept RGBa images as destination
    if (imgOut->getBaseRasterType() == kRGBa32Raster)
        applyInto_(imgIn, params, imgIn->getValidRect(), (RasterImage_RGBa* ) imgOut);

    //  ...as well as gray-level images when a single mode has been specified
    //  for the search [the test for the number of modes is done in the function called,
    //  to produce distinct error messages].
    else
        if (imgOut->getBaseRasterType() == kGrayRaster)
            applyInto_(imgIn, params, imgIn->getValidRect(), (RasterImage_gray* ) imgOut);

    //  otherwise we just refuse to deal with the image
        else
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "only RGBa and (some) gray outputs accepted in GrayThresholdLabeler::applyInto so far");

}

void GrayThresholdLabeler::applyInto(const RasterImage* imgIn, const ColorSegParams *params,
                                     const ImageRect* theRect, RasterImage* imgOut)
{
    FAIL_CONDITION( imgIn == NULL,
                    kNullRasterImageError,
                    "null RasterImage as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( imgOut == NULL,
                    kNullRasterImageError,
                    "null output RasterImage as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( params == NULL,
                    kNullParameterError,
                    "null ColorSegParams as parameter in GrayThresholdLabeler::applyTo");
    FAIL_CONDITION( theRect == NULL,
                    kNullRectangleError,
                    "null ImageRect as parameter in GrayThresholdLabeler::applyTo");

    //  we accept RGBa images as destination
    if (imgOut->getBaseRasterType() == kRGBa32Raster)
        applyInto_(imgIn, params, theRect, (RasterImage_RGBa* ) imgOut);

    //  ...as well as gray-level images when a single mode has been specified
    //  for the search [the test for the number of modes is done in the function called,
    //  to produce distinct error messages].
    else
        if (imgOut->getBaseRasterType() == kGrayRaster)
            applyInto_(imgIn, params, theRect, (RasterImage_gray* ) imgOut);

    //  otherwise we just refuse to deal with the image
        else
            FAIL_CONDITION( true,
                            kRasterImageError,
                            "only RGBa and (some) gray outputs accepted in GrayThresholdLabeler::applyInto so far");

}


void GrayThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                     RasterImage* imgOut)
{
    applyInto(imgIn, (const ColorSegParams *) params, imgOut);
}



void GrayThresholdLabeler::applyInto(const RasterImage* imgIn, const char* params,
                                     const ImageRect* theRect, RasterImage* imgOut)
{
    applyInto(imgIn, (const ColorSegParams *) params, theRect, imgOut);
}


#if 0
#pragma mark -
#endif


/*------------------------------------------------------------------------------+
|	applies gray-level thresholds to an image									|
+------------------------------------------------------------------------------*/

void GrayThresholdLabeler::applyInto_(const RasterImage* imgIn, const ColorSegParams* grayParams,
                                      const ImageRect* theRect, LabelRaster* labelOut,
                                      bool connectNeighbor)
{
    ImageRect* destRect = ImageRect::intersection(imgIn->getValidRect(), theRect);
    FAIL_CONDITION(	destRect == NULL,
                    kInvalidRectangleError,
                    "input image valid date rect and output rectangles must overlap");

    bool localGray;
    const RasterImage_gray* grayImg = RasterImage_gray::localRasterGray(imgIn, &localGray);
    GrayRegion		*graySought = grayParams->grayReg;
    const unsigned char* const* gray = grayImg->getShiftedGray2D();
    char** label = labelOut->getShiftedLabel2D(R_W_ACCESS);

    const int   nbRegions = grayParams->nbModes;
    const int   iLow = destRect->getTop( ),
                iHigh = destRect->getBottom( ),
                jLow = destRect->getLeft( ),
                jHigh = destRect->getRight( );
    bool	matchFound;
	char	noMatchVal = grayParams->noMatchVal;

    for (int i=iLow; i<=iHigh; i++)
    {
        for (int j=jLow; j<=jHigh; j++)
        {
            matchFound = false;
            for (int k=0; (k<nbRegions) && !matchFound; k++)
                if ((gray[i][j]>=graySought[k].vMin) &&
                        (gray[i][j]<=graySought[k].vMax))
                {

                    label[i][j] = graySought[k].label;
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
    	
    	connectNeighbors_(theRect, nbRegions, labelOut, tempLabel_, noMatchVal);
    	
    	if (tempLabelIsLocal)
    	{
    		delete tempLabel_;
    		tempLabel_ = NULL;
    	}
    }

    //  cleanup if needed

    if (localGray)
        delete grayImg;
    delete destRect;

    labelOut->setValidRect(theRect);
}


void GrayThresholdLabeler::applyInto_(const RasterImage* imgIn, const ColorSegParams *grayParams,
                                      const ImageRect* theRect, RasterImage_RGBa* imgOut)
{
    ImageRect* destRect = ImageRect::intersection(imgIn->getValidRect(), theRect);
    FAIL_CONDITION(	destRect == NULL,
                    kInvalidRectangleError,
                    "input image valid date rect and output rectangles must overlap");

    bool localGray;
    const RasterImage_gray* grayImg = RasterImage_gray::localRasterGray(imgIn, &localGray);

    GrayRegion 	*graySought = grayParams->grayReg;
    unsigned char* const* rgbaOut = imgOut->getShiftedRGBa2D(R_W_ACCESS);
    const unsigned char* const* gray = grayImg->getShiftedGray2D();
    const unsigned char* const* rasterIn = imgIn->getShiftedRaster2D();

    const BaseRasterType inType = imgIn->getBaseRasterType();
    const int inDepth = imgIn->getPixelDepth()/8;

    const int   nbRegions = grayParams->nbModes;
    const int	iLow = destRect->getTop( ),
                iHigh = destRect->getBottom( ),
                jLow = destRect->getLeft( ),
                jHigh = destRect->getRight( );
    bool	matchFound;
    FillingMode fillMode = grayParams->fillMode;

    switch (fillMode)
    {
        //	The output is an unmodified copy of the input
        case kIdentityFill:
            //jyh   fix later
            //imgIn->copyInto(imgOut, imgIn->getType());
            //imgIn->copyInto(imgOut);
            break;

        //	Simply overlay thresholded regions on the original image
        case kOrigColorFill:

            switch (inType)
            {
                case kGrayRaster:

                    for (int i=iLow; i<=iHigh; i++)
                    {
                        //  j is the pixel index in the hsv rasters
                        //  l is the byte index in the output image
                        //  m is the byte index in the input image
                        for (int j=jLow, l=4*jLow, m=inDepth*jLow; j<=jHigh; j++)
                        {
                            matchFound = false;
                            for (int k=0; (k<nbRegions) && !matchFound; k++)
                                if ((gray[i][j]>=graySought[k].vMin) &&
                                        (gray[i][j]<=graySought[k].vMax))
                                {
                                    rgbaOut[i][l++] = graySought[k].fillColor[0];  //  red
                                    rgbaOut[i][l++] = graySought[k].fillColor[1];  //  green
                                    rgbaOut[i][l++] = graySought[k].fillColor[2];  //  blue
                                    rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                                    m+=inDepth;
                                    matchFound = true;
                                }

                            if (!matchFound)
                            {
                                unsigned char   val = rasterIn[i][m++];
                                rgbaOut[i][l++] = val;
                                rgbaOut[i][l++] = val;
                                rgbaOut[i][l++] = val;
                                rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                            }
                        }
                    }
                    break;
                    
                 case kRGB24Raster:
                 
                    for (int i=iLow; i<=iHigh; i++)
                    {
                        //  j is the pixel index in the hsv rasters
                        //  l is the byte index in the output image
                        //  m is the byte index in the input image
                        for (int j=jLow, l=4*jLow, m=inDepth*jLow; j<=jHigh; j++)
                        {
                            matchFound = false;
                            for (int k=0; (k<nbRegions) && !matchFound; k++)
                                if ((gray[i][j]>=graySought[k].vMin) &&
                                        (gray[i][j]<=graySought[k].vMax))
                                {
                                    rgbaOut[i][l++] = graySought[k].fillColor[0];  //  red
                                    rgbaOut[i][l++] = graySought[k].fillColor[1];  //  green
                                    rgbaOut[i][l++] = graySought[k].fillColor[2];  //  blue
                                    rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                                    m+=inDepth;
                                    matchFound = true;
                                }

                            if (!matchFound)
                            {
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                            }
                        }
                    }
                    break;
                                                
                case kRGBa32Raster:
                    for (int i=iLow; i<=iHigh; i++)
                    {
                        //  j is the pixel index in the hsv rasters
                        //  l is the byte index in the output image
                        //  m is the byte index in the input image
                        for (int j=jLow, l=4*jLow, m=inDepth*jLow; j<=jHigh; j++)
                        {
                            matchFound = false;
                            for (int k=0; (k<nbRegions) && !matchFound; k++)
                                if ((gray[i][j]>=graySought[k].vMin) &&
                                        (gray[i][j]<=graySought[k].vMax))
                                {
                                    rgbaOut[i][l++] = graySought[k].fillColor[0];  //  red
                                    rgbaOut[i][l++] = graySought[k].fillColor[1];  //  green
                                    rgbaOut[i][l++] = graySought[k].fillColor[2];  //  blue
                                    rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                                    m+=inDepth;
                                    matchFound = true;
                                }

                            if (!matchFound)
                            {
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];
                                rgbaOut[i][l++] = rasterIn[i][m++];
                            }
                        }
                    }

                default:
                    break;


            }
            break;

        //	Display the thresholded regions over a black background
        case kBlackFill:

            for (int i=iLow; i<=iHigh; i++)
            {
                //  j is the pixel index in the hsv rasters
                //  l is the byte index in the output image
                //  m is the byet index in the input image
                for (int j=jLow, l=4*jLow; j<=jHigh; j++)
                {
                    matchFound = false;
                    for (int k=0; (k<nbRegions) && !matchFound; k++)
                        if ((gray[i][j]>=graySought[k].vMin) &&
                                (gray[i][j]<=graySought[k].vMax))
                        {
                            rgbaOut[i][l++] = graySought[k].fillColor[0];  //  red
                            rgbaOut[i][l++] = graySought[k].fillColor[1];  //  green
                            rgbaOut[i][l++] = graySought[k].fillColor[2];  //  blue
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

            for (int i=iLow; i<=iHigh; i++)
            {
                //  j is the pixel index in the hsv rasters
                //  l is the byte index in the output image
                //  m is the byet index in the input image
                for (int j=jLow, l=4*jLow; j<=jHigh; j++)
                {
                    matchFound = false;
                    for (int k=0; (k<nbRegions) && !matchFound; k++)
                        if ((gray[i][j]>=graySought[k].vMin) &&
                                (gray[i][j]<=graySought[k].vMax))
                        {
                            rgbaOut[i][l++] = graySought[k].fillColor[0];  //  red
                            rgbaOut[i][l++] = graySought[k].fillColor[1];  //  green
                            rgbaOut[i][l++] = graySought[k].fillColor[2];  //  blue
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

            for (int i=iLow; i<=iHigh; i++)
            {
                //  j is the pixel index in the hsv rasters
                //  l is the byte index in the output image
                //  m is the byet index in the input image
                for (int j=jLow, l=4*jLow; j<=jHigh; j++)
                {
                    matchFound = false;
                    for (int k=0; (k<nbRegions) && !matchFound; k++)
                        if ((gray[i][j]>=graySought[k].vMin) &&
                                (gray[i][j]<=graySought[k].vMax))
                        {
                            rgbaOut[i][l++] = graySought[k].fillColor[0];  //  red
                            rgbaOut[i][l++] = graySought[k].fillColor[1];  //  green
                            rgbaOut[i][l++] = graySought[k].fillColor[2];  //  blue
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

            for (int i=iLow; i<=iHigh; i++)
            {
                //  j is the pixel index in the hsv rasters
                //  l is the byte index in the output image
                //  m is the byet index in the input image
                for (int j=jLow, l=4*jLow, m=inDepth*jLow; j<=jHigh; j++)
                {
                    matchFound = false;
                    for (int k=0; (k<nbRegions) && !matchFound; k++)
                        if ((gray[i][j]>=graySought[k].vMin) &&
                                (gray[i][j]<=graySought[k].vMax))
                        {
                            rgbaOut[i][l++] = graySought[k].fillColor[0];  //  red
                            rgbaOut[i][l++] = graySought[k].fillColor[1];  //  green
                            rgbaOut[i][l++] = graySought[k].fillColor[2];  //  blue
                            rgbaOut[i][l++] = (unsigned char) 255;  //  alpha
                            m++;
                            matchFound = true;
                        }

                    if (!matchFound)
                    {
                        unsigned char   val = gray[i][m++];
                        rgbaOut[i][l++] = val;
                        rgbaOut[i][l++] = val;
                        rgbaOut[i][l++] = val;
                        rgbaOut[i][l++] = (unsigned char) 255;
                    }
                }
            }
            break;

        default:
            break;
    }

    if (localGray)
        delete grayImg;

    delete destRect;

    imgOut->setValidRect(theRect);
}

void GrayThresholdLabeler::applyInto_(const RasterImage* imgIn, const ColorSegParams *grayParams,
                                      const ImageRect* theRect, RasterImage_gray* imgOut)
{
    ImageRect* destRect = ImageRect::intersection(imgIn->getValidRect(), theRect);
    FAIL_CONDITION(	destRect == NULL,
                    kInvalidRectangleError,
                    "input image valid date rect and output rectangles must overlap");

	//	should be computed only for theRect
    bool localGray;
    const RasterImage_gray* grayImg = RasterImage_gray::localRasterGray(imgIn, &localGray);

    //  When the output is a gray-level image, then we accept only a single mode for the
    //  thresholding
    int nbRegions = grayParams->nbModes;
    FAIL_CONDITION(	nbRegions != 1,
                    kInvalidParameterError,
                    "thresholding into a gray level image is only permitted ");


    GrayRegion 	    *graySought = grayParams->grayReg;
    unsigned char* const* grayOut = imgOut->getShiftedGray2D(R_W_ACCESS);
    const unsigned char* const* grayIn = grayImg->getShiftedGray2D();

    //  just trying to compensate for Nature's way of producing ever
    //  more performant idiots
    unsigned char   fillVal = static_cast<unsigned char>((graySought[0].fillColor[0] + 
                                                          graySought[0].fillColor[1] +
                                                          graySought[0].fillColor[1]) / 3);
    unsigned char   vMin = graySought[0].vMin,
                    vMax = graySought[0].vMax;

    const int   iLow = destRect->getTop( ),
                iHigh = destRect->getBottom( ),
                jLow = destRect->getLeft( ),
                jHigh = destRect->getRight( );
    FillingMode     fillMode = grayParams->fillMode;

    switch (fillMode)
    {
        //	The output is an unmodified copy of the input
        //  ???? Why ???  What exactly did I write this for????
        case kIdentityFill:
            //jyh   fix later
            //imgIn->copyInto(imgOut, imgIn->getType());
            imgIn->copyInto(imgOut);
            break;


            //	Display the thresholded regions over a black background [better hope that the
            //  filling color is not black too]
        case kBlackFill:

            for (int i=iLow; i<=iHigh; i++)
                for (int j=jLow; j<=jHigh; j++)
                    if ((grayIn[i][j] >= vMin) && (grayIn[i][j] <= vMax))
                        grayOut[i][j] = fillVal;
                    else
                        grayOut[i][j] = (unsigned char) 0;

            break;

        //	Display the thresholded regions over a white background [better hope that the
        //  filling color is not white too]
        case kWhiteFill:

            for (int i=iLow; i<=iHigh; i++)
                for (int j=jLow; j<=jHigh; j++)
                    if ((grayIn[i][j] >= vMin) && (grayIn[i][j] <= vMax))
                        grayOut[i][j] = fillVal;
                    else
                        grayOut[i][j] = (unsigned char) 255;

            break;


        //	Display the thresholded regions over a medium gray background
        case kMediumGrayFill:

            for (int i=iLow; i<=iHigh; i++)
                for (int j=jLow; j<=jHigh; j++)
                    if ((grayIn[i][j] >= vMin) && (grayIn[i][j] <= vMax))
                        grayOut[i][j] = fillVal;
                    else
                        grayOut[i][j] = (unsigned char) 127;

            break;


        //	Simply overlay thresholded regions on the original image
        case kOrigColorFill:
        case kOrigGrayFill:

            for (int i=iLow; i<=iHigh; i++)
                for (int j=jLow; j<=jHigh; j++)
                    if ((grayIn[i][j] >= vMin) && (grayIn[i][j] <= vMax))
                        grayOut[i][j] = fillVal;
                    else
                        grayOut[i][j] = grayIn[i][j];

            break;


        default:
            break;
    }

    if (localGray)
        delete grayImg;

    delete destRect;

    imgOut->setValidRect(theRect);
}


void GrayThresholdLabeler::connectNeighbors_(const ImageRect* destRect, int nbRegions,
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
