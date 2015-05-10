/*  NAME:
        ManualStitcher.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ManualStitcher class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <string>
#include <cstdlib>
#include <cmath>
#include <cstring>
//
#include "ManualStitcher.h"

using namespace uriVL;

#error Work in progress: do not use this class yet

enum eComponentAverages
{
    kAvgRed_i = 0,
    kAvgGreen_i,
    kAvgBlue_i,
    kAvgRed_j,
    kAvgGreen_j,
    kAvgBlue_j
};

enum eSummationTerms
{
    kSumDenRed_i =0,
    kSumDenGreen_i,
    kSumDenBlue_i,
    kSumDenRed_j,
    kSumDenGreen_j,
    kSumDenBlue_j,
    kSumNumRed,
    kSumNumGreen,
    kSumNumBlue
};

//----------------------------------------------
//	Macros
//----------------------------------------------
#ifndef ABS
#define ABS(a)	(a >= 0 ? a : -(a))
#endif


/*------------------------------------------------------------------------------+
|	Default constructor for this class											|
+------------------------------------------------------------------------------*/
ManualStitcher::ManualStitcher(void)
{
    stitchPat = NULL;
    modifiedImages = false;
}


/*------------------------------------------------------------------------------+
|	Destructor for this class													|
+------------------------------------------------------------------------------*/
ManualStitcher::~ManualStitcher(void)
{}


/*------------------------------------------------------------------------------+
|	Stitching function: This one takes as parameters an array of images and 	|
|	a stitching pattern structure that defines the number of images to stitch,	|
|	the coarse relative position of these images, and the range of these		|
|	relative positions.															|
+------------------------------------------------------------------------------*/
void ManualStitcher::initStitch(RasterImage** image, sStitchPattern *thePattern)
{
    short	dx = 0, dy = 0;

    //------------
    //	Step 0: Allocation
    //------------
    stitchPat = thePattern;
    nbImages = stitchPat->nbImages;
    allocateStitchArrays();

    //------------
    //	Step 2: Initialization
    //------------
    for (short i=0; i<nbImages; i++)
    {
        if (stitchPat->coordX[i] < dx)
            dx = stitchPat->coordX[i];
        if (stitchPat->coordY[i] < dy)
            coordY[i] = stitchPat->coordY[i];

    }

    for (short i=0; i<nbImages; i++)
    {
        coordX[i] = stitchPat->coordX[i] - dx;
        coordY[i] = stitchPat->coordY[i] - dy;
        image[i]->setOrigin(coordX[i], coordY[i]);
    }

}

void ManualStitcher::translateImage(RasterImage** image, short i, short dx, short dy)
{
    coordX[i] += dx;
    coordY[i] += dy;

    if ( (coordX[i] >= 0) && (coordY[i] >= 0) )
        image[i]->translate(dx, dy);
    else
    {
        if  (coordX[i] < 0)
        {
            for (short j=0; j<nbImages; j++)
                if (i!=j)
                    coordX[j] -= coordX[i];
            coordX[i] = 0;
        }
        if  (coordY[i] < 0)
        {
            for (short j=0; j<nbImages; j++)
                if (i!=j)
                    coordY[j] -= coordY[i];
            coordY[i] = 0;
        }
        for (short j=0; j<nbImages; j++)
            image[j]->setOrigin(coordX[j], coordY[j]);

    }
}

RasterImage* ManualStitcher::stitchImage(RasterImage** image)
{
    RasterImage	*outImage = NULL;
    short	leftBound, rightBound, topBound, bottomBound,
    shiftH, shiftV,
    nbCols, nbRows;
    unsigned char	**dest, **source;

    //	get the bounding box of the stitched image
    leftBound = coordX[0];
    rightBound = coordX[0] + image[0]->getNbCols() - 1;
    topBound = coordY[0];
    bottomBound = coordY[0] + image[0]->getNbRows() - 1;
    for (short k=1; k<nbImages; k++)
    {
        if (leftBound > coordX[k])
            leftBound = coordX[k];
        if (rightBound < coordX[k] + image[k]->getNbCols() - 1)
            rightBound = coordX[k] + image[k]->getNbCols() - 1;
        if (topBound > coordY[k])
            topBound = coordY[k];
        if (bottomBound < coordY[k] + image[k]->getNbRows() - 1)
            bottomBound = coordY[k] + image[k]->getNbRows() - 1;
    }

    nbRows = bottomBound - topBound + 1;
    nbCols = rightBound - leftBound + 1;

    outImage = new RasterImage(nbRows, nbCols, kRgbColorImg);
    dest = outImage->getRGB2D();

    for (short k=0; k<nbImages; k++)
    {
        //	compute hor and vert shift between the two images
        shiftH = 3*(coordX[k] - leftBound);
        shiftV = coordY[k] - topBound;

        //	I want to copy all the lines of image i into the destination raster
        source = image[k]->getRGB2D();
        for (short i=0; i<image[k]->getNbRows(); i++)
        {
            memcpy(dest[i+shiftV]+shiftH, source[i], 3*image[k]->getNbCols());
        }
    }
    return outImage;
}


void ManualStitcher::equalizeImages(RasterImage** image)
{
    float	averages[6], ratio[3],
    scale_i[3], scale_j[3];
    unsigned char	minI[3], maxI[3], minJ[3], maxJ[3],
    *dummy;

    //---------------------------
    //	Step 0:
    //	First, make sure that we preserve a copy of the image into its own RGBA raster
    //---------------------------
    for (short i=0; i<nbImages; i++)
        dummy = image[i]->getRGBA();

    //---------------------------
    //	Step 1:	Equalize the average intensity over the intersections by scaling the
    //			entire images
    //---------------------------
    for (short i=1; i<nbImages; i++)
    {
        computeIntersections(image, i);

        for (short j=0; j<i; j++)
        {
            //	if images i and j intersect
            if (intersect[j])
            {
                //	min-max must be recomputed each time since we modify them.
                image[i]->getMinMax(minI, maxI);
                image[j]->getMinMax(minJ, maxJ);

                computeAverages(image, i, j, averages);

                //	something better (i.e. least squares optimization) should be done
                //	in the future, but this will have to do.
                for (short k=0; k<3; k++)
                {
                    //	image i is brighter than image j for the k component (r-g-b)
                    if (averages[k] > averages[k+3])
                    {
                        //	if image i is "too bright" we scale it down
                        if (averages[k] > 200)
                        {
                            scale_i[k] = averages[k+3]/averages[k];
                            scale_j[k] = 1.f;
                        }
                        //	otherwise, scale up j
                        else
                        {
                            scale_i[k] = 1.f;
                            scale_j[k] = averages[k]/averages[k+3];
                        }
                    }
                    //	image i is darker than image j for the k component (r-g-b)
                    else
                    {
                        //	if image j is "too bright" we scale it down
                        if (averages[k+3] > 200)
                        {
                            scale_i[k] = 1.f;
                            scale_j[k] = averages[k]/averages[k+3];
                        }
                        //	otherwise, scale up i
                        else
                        {
                            scale_i[k] = averages[k+3]/averages[k];
                            scale_j[k] = 1.f;
                        }
                    }
                }
                image[i]->applyScale(scale_i);
                image[j]->applyScale(scale_j);
            }
        }
    }


    //---------------------------
    //	Step 2:	Interpolate the images oover their intersection region
    //---------------------------
    for (short i=1; i<nbImages; i++)
    {
        computeIntersections(image, i);

        for (short j=0; j<i; j++)
        {
            //	if images i and j intersect
            if (intersect[j])
            {
                //	interpolate over the intersection area
                interpolateImages(image, i, j);
            }
        }
    }

    modifiedImages = true;
}

void ManualStitcher::restoreImages(RasterImage** image, sStitchPattern *thePattern)
{
    short	dx = 0, dy = 0;

    if (modifiedImages)
    {
        //	ugly and slow, but simple
        for (short i=0; i<nbImages; i++)
        {
            //	first copy RGBA back into RGB
            image[i]->setType(kRgbaColorImg);
            //	then RGB  into RGBA
            image[i]->setType(kRgbColorImg);
        }
    }

    for (short i=0; i<nbImages; i++)
    {
        if (stitchPat->coordX[i] < dx)
            dx = stitchPat->coordX[i];
        if (stitchPat->coordY[i] < dy)
            dy = stitchPat->coordY[i];

    }

    for (short i=0; i<nbImages; i++)
    {
        coordX[i] = stitchPat->coordX[i] - dx;
        coordY[i] = stitchPat->coordY[i] - dy;
        image[i]->setOrigin(coordX[i], coordY[i]);
    }
}


void ManualStitcher::interpolateImages(RasterImage** image, short i, short j)
{
    float	s;
    short	k_i, l_i,		//	row and column indexes in image i
    k_j, l_j,		//	row and column indexes in image j
    h_i, h_j,
    r_i, r_j,
    b_i, b_j,
    H_i, H_j,
    V_i, V_j;
    short	w_i, w_j,		//	"weight" of the intensity of images i and j in interpolation
    cols_i = image[i]->getNbCols(),
             cols_j = image[j]->getNbCols(),
                      rows_i = image[i]->getNbRows(),
                               rows_j = image[j]->getNbRows();

    //
    unsigned char	**rgb_i, **rgb_j,
    r, g, b;

    rgb_i = image[i]->getRGB2D();
    rgb_j = image[j]->getRGB2D();

    k_i = yMin[j]-coordY[i];
    k_j = yMin[j]-coordY[j];
    //	x and y are defined in the "global" image space, whereas k_i, k_j, and
    //	l_i, l_j are defined in the local spaces of images k and l respectively
    for (short y=yMin[j]; y<=yMax[j]; y++, k_i++, k_j++)
    {
        b_i = rows_i - k_i - 1;
        b_j = rows_j - k_j - 1;
        V_i = MIN(k_i, b_i);
        V_j = MIN(k_j, b_j);

        h_i = xMin[j]-coordX[i];	//	distance to left border of the image
        h_j = xMin[j]-coordX[j];
        //
        for (short x=xMin[j]; x<=xMax[j]; x++, h_i++, h_j++)
        {
            l_i = 3*h_i;
            l_j = 3*h_j;

            r_i = cols_i - h_i - 1;
            r_j = cols_j - h_j - 1;
            H_i = MIN(h_i, r_i);
            H_j = MIN(h_j, r_j);

            //	w_i is the "inf-distance" to the border of image i
            w_i = MIN(H_i, V_i);
            w_j = MIN(H_j, V_j);
            //
            if (w_i + w_j != 0)
            {
                s = 1.f / (w_i + w_j);

                r = (unsigned char) (s *(w_i*rgb_i[k_i][l_i] + w_j*rgb_j[k_j][l_j]));
                g = (unsigned char) (s *(w_i*rgb_i[k_i][l_i+1] + w_j*rgb_j[k_j][l_j+1]));
                b = (unsigned char) (s *(w_i*rgb_i[k_i][l_i+2] + w_j*rgb_j[k_j][l_j+2]));

                rgb_i[k_i][l_i] = rgb_j[k_j][l_j] = r;
                rgb_i[k_i][l_i+1] = rgb_j[k_j][l_j+1] = g;
                rgb_i[k_i][l_i+2] = rgb_j[k_j][l_j+2] = b;
            }
            //	else do nothing
        }

    }
}




//------------------------------------------------------------------
//	Updates the intersections array for *1* image
//------------------------------------------------------------------
void ManualStitcher::computeIntersections(RasterImage** image, short i)
{
    short	li, ri, lj, rj,		//	left, right, top, and botttom bounds of the 2 images
    ti, tj, bi, bj;
    //
    bool	condHi, condHj, condVi, condVj;

    //	Compute absolute left, right, top & bottom coordinates of both images
    li = coordX[i];
    ti = coordY[i];
    ri = li + image[i]->getNbCols() - 1;
    bi = ti + image[i]->getNbRows() - 1;

    //	For each image Ij other than the reference image
    for (short j=0, l=0; j<nbImages; j++)
    {
        if (i != j)
        {
            lj = coordX[j];
            tj = coordY[j];
            rj = lj + image[j]->getNbCols() - 1;
            bj = tj + image[j]->getNbRows() - 1;

            //-----------------------------------------------------------------
            //	check if images i and j (in their current position) intersect
            //-----------------------------------------------------------------
            condHi = ((li >= lj) && (li < rj));	//	i's L between's j's L & R
            condHj = ((lj >= li) && (lj < ri));	//	j's L between's i's L & R
            condVi = ((ti >= tj) && (ti < bj));	//	i's T between's j's T & B
            condVj = ((tj >= ti) && (tj < bi));	//	j's T between's i's T & B

            if ( (condHi || condHj) && (condVi || condVj) )
            {
                intersect[j] = true;

                xMin[j] = MAX(li, lj);
                xMax[j] = MIN(ri, rj);
                yMin[j] = MAX(ti, tj);
                yMax[j] = MIN(bi, bj);

            }
            else
                intersect[j] = false;

        }
        else
            intersect[j] = false;

    }
}


void ManualStitcher::computeAverages(RasterImage** image, short i, short j, float avg[])
{
    long	nbPixelsInter;
    float	score = 0.f,
                  termRed_i, termGreen_i, termBlue_i,
                  termRed_j, termGreen_j, termBlue_j;
    short	k_i, l_i,		//	row and column indexes in image i
    k_j, l_j;		//	row and column indexes in image j
    //
    unsigned char	**rgb_i, **rgb_j;
    rgb_i = image[i]->getRGB2D();
    rgb_j = image[j]->getRGB2D();

    //	number of pixels in the intersection region
    nbPixelsInter = (yMax[j]-yMin[j]+1)*(xMax[j]-xMin[j]+1);

    //	compute the average intensity for images i and j over the
    //	intersection region.
    avg[kAvgRed_i] = avg[kAvgGreen_i] = avg[kAvgBlue_i] = 0.f;
    avg[kAvgRed_j] = avg[kAvgGreen_j] = avg[kAvgBlue_j] = 0.f;
    k_i = yMin[j]-coordY[i];
    k_j = yMin[j]-coordY[j];

    //	x and y are defined in the "global" image space, whereas k_i, k_j, and
    //	l_i, l_j are defined in the local spaces of images k and l respectively
    for (short y=yMin[j]; y<=yMax[j]; y++, k_i++, k_j++)
    {
        l_i = 3*(xMin[j]-coordX[i]);
        l_j = 3*(xMin[j]-coordX[j]);

        for (short x=xMin[j]; x<=xMax[j]; x++)
        {
            avg[kAvgRed_i] +=rgb_i[k_i][l_i++];
            avg[kAvgGreen_i] += rgb_i[k_i][l_i++];
            avg[kAvgBlue_i] += rgb_i[k_i][l_i++];
            //
            avg[kAvgRed_j] += rgb_j[k_j][l_j++];
            avg[kAvgGreen_j] += rgb_j[k_j][l_j++];
            avg[kAvgBlue_j] += rgb_j[k_j][l_j++];
        }
    }
    avg[kAvgRed_i] /= nbPixelsInter;
    avg[kAvgGreen_i] /= nbPixelsInter;
    avg[kAvgBlue_i] /= nbPixelsInter;
    avg[kAvgRed_j] /= nbPixelsInter;
    avg[kAvgGreen_j] /= nbPixelsInter;
    avg[kAvgBlue_j] /= nbPixelsInter;
}


#pragma mark -


//-------------------------------------------------------------------
//	data allocation and de-allocation functions
//-------------------------------------------------------------------

void ManualStitcher::allocateStitchArrays(void)
{
    intersect = (bool *) calloc(nbImages, sizeof(bool));
    assert(intersect != NULL);

    scaleRatio = (float* ) calloc(nbImages, sizeof(float));
    assert(scaleRatio != NULL);

    scaleRatios = (float** ) calloc(nbImages, sizeof(float* ));
    assert(scaleRatios != NULL);
    for (short i=1; i<nbImages; i++)
        scaleRatios[i] = (float* ) calloc(3, sizeof(float));

    coordX = (short *) calloc(nbImages, sizeof(short));
    coordY = (short *) calloc(nbImages, sizeof(short));
    //
    xMin = (short *) calloc(nbImages, sizeof(short));
    xMax = (short *) calloc(nbImages, sizeof(short));
    yMin = (short *) calloc(nbImages, sizeof(short));
    yMax = (short *) calloc(nbImages, sizeof(short));
}

void ManualStitcher::deleteStitchArrays(void)
{
    if (intersect != NULL)
    {
        free((char* ) intersect);
        intersect = NULL;
    }
    if (coordX != NULL)
    {
        free((char* ) coordX);
        coordX = NULL;
    }
    if (coordY != NULL)
    {
        free((char* ) coordY);
        coordY = NULL;
    }
    if (xMin != NULL)
    {
        free((char* ) xMin);
        xMin = NULL;
    }
    if (xMax != NULL)
    {
        free((char* ) xMax);
        xMax = NULL;
    }
    if (yMin != NULL)
    {
        free((char* ) yMin);
        yMin = NULL;
    }
    if (yMax != NULL)
    {
        free((char* ) yMax);
        yMax = NULL;
    }
    if (scaleRatio != NULL)
    {
        free((char* ) scaleRatio);
        scaleRatio = NULL;
    }
    if (scaleRatios != NULL)
    {
        for (short i=0; i<nbImages; i++)
            free((char* ) scaleRatios[i]);

        free((char** ) scaleRatios);
        scaleRatios = NULL;
    }

}