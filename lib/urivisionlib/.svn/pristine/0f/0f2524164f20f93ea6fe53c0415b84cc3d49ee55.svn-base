/*  NAME:
        ImageStitcher.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ImageStitcher class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstdlib>
#include <cmath>
#include <cstdio>
//
#include "ImageStitcher.h"

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
ImageStitcher::ImageStitcher(void)
{
    stitchPat = NULL;
}


/*------------------------------------------------------------------------------+
|	Destructor for this class													|
+------------------------------------------------------------------------------*/
ImageStitcher::~ImageStitcher(void)
{}


/*------------------------------------------------------------------------------+
|	Stitching function: This one takes as parameters an array of images and 	|
|	a stitching pattern structure that defines the number of images to stitch,	|
|	the coarse relative position of these images, and the range of these		|
|	relative positions.															|
+------------------------------------------------------------------------------*/
RasterImage* ImageStitcher::stitch(RasterImage** image, sStitchPattern *thePattern)
{
    RasterImage	*outImage = NULL;
    bool		converged = false,
                      moved;

    //------------
    //	Step 0: Allocation
    //------------
    stitchPat = thePattern;
    nbImages = stitchPat->nbImages;
    allocateStitchArrays();

    //------------
    //	Step 1: Initialization
    //------------
    //
    //	We "lock" the position of the first image.
    image[0]->setOrigin(stitchPat->coordX[0], stitchPat->coordY[0]);
    displState[0] = stitchPat->coordX[0];
    displState[1] = stitchPat->coordY[0];
    //
    for (short j=1; j<nbImages; j++)
    {
        //	X coordinate
        displState[2*j] = stitchPat->coordX[j];
        qMin[2*j] = stitchPat->xMin[j];
        qMax[2*j] = stitchPat->xMax[j];
        //	Y coordinate
        displState[2*j+1] = stitchPat->coordY[j];
        qMin[2*j+1] = stitchPat->yMin[j];
        qMax[2*j+1] = stitchPat->yMax[j];
        //
        image[j]->setOrigin(stitchPat->coordX[j], stitchPat->coordY[j]);
    }

    //------------
    //	Step 2: Optimization loop
    //------------
    /*	while (!converged)
    	{
    		moved = false;
    		
    		//	for each direction in the configuration space (except for the first 2 (locked)
    		for (short k=2; k<2*nbImages; k++)
    		{
    			if (optimizeDirection(k, image))
    				moved = true;;
    		}
    		
    		converged = !moved;
    	}
    */

    //------------
    //	Step 3: De-allocation
    //------------
    //	deleteStitchArrays();
    //	stitchPat = NULL;

    //------------
    //	Step 4: Get outta here
    //------------
    //	return outImage;
}


bool ImageStitcher::optimizeDirection(short k, RasterImage** image)
{
    float		score, bestScoreSoFar = 0;
    short		currDispl,
    bestMoveSoFar;
    bool		converged = false,
                      moved;

    //	store current displacement along that direction and initialize a bogus
    //	best score.
    bestMoveSoFar = currDispl = displState[k];
    bestScoreSoFar = -100.f;

    //	look for the minimum along this direction
    for (short l=qMin[k]; l<=qMax[k]; l++)
    {
        displState[k] = l;

        computeIntersections(image, k/2);

        score = computeScoreRGB(image, k/2);

        if (score > bestScoreSoFar)
        {
            bestScoreSoFar = score;
            bestMoveSoFar = l;
        }
    }

    //char buf[256];
    if ((k%2)==0)
        printf("Move for image %d, X direction : %d to %d.\n", k/2, bestMoveSoFar - currDispl, bestMoveSoFar);
    else
        printf("Move for image %d, Y direction : %d to %d.\n", k/2, bestMoveSoFar - currDispl, bestMoveSoFar);

    //	If we found a better state, we move there
    if (bestMoveSoFar != currDispl)
    {
        displState[k] = bestMoveSoFar;
        image[k/2]->setOrigin(displState[2*(k/2)], displState[2*(k/2) + 1]);
        moved = true;
    }

    return moved;
}

//------------------------------------------------------------------
//	Updates the intersections array for *1* image
//------------------------------------------------------------------
void ImageStitcher::computeIntersections(RasterImage** image, short i)
{
    short	li, ri, lj, rj,		//	left, right, top, and botttom bounds of the 2 images
    ti, tj, bi, bj;
    //
    bool	condHi, condHj, condVi, condVj;

    //	Compute absolute left, right, top & bottom coordinates of both images
    li = displState[2*i];
    ti = displState[2*i+1];
    ri = li + image[i]->getNbCols() - 1;
    bi = ti + image[i]->getNbRows() - 1;

    //	For each image Ij other than the reference image
    for (short j=0, l=0; j<nbImages; j++)
    {
        if (i != j)
        {
            lj = displState[2*j];
            tj = displState[2*j+1];
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


float ImageStitcher::computeScoreRGB(RasterImage** image, short i)
{
    float	score = 0.f, corrRed, corrGreen, corrBlue,
                  avg[6], sumTerm[9];

    //	For each pair of images, Ii and Ij
    for (short j=0; j<nbImages; j++)
    {
        if ( intersect[j] )
        {
            computeCorrTerms(image, i, j, avg, sumTerm);

            //	If none of the terms at the denominator was anywhere near zero, we compute
            //	correlation coefficients for each component
            if (sumTerm[kSumDenRed_i]*sumTerm[kSumDenRed_j]*
                    sumTerm[kSumDenGreen_i]*sumTerm[kSumDenGreen_j]*
                    sumTerm[kSumDenBlue_i]*sumTerm[kSumDenBlue_j] != 0.f)
            {
                corrRed = sumTerm[kSumNumRed] /
                          sqrtf(sumTerm[kSumDenRed_i]*sumTerm[kSumDenRed_j]);
                corrGreen = sumTerm[kSumNumGreen] /
                            sqrtf(sumTerm[kSumDenGreen_i]*sumTerm[kSumDenGreen_j]);
                corrBlue = sumTerm[kSumNumBlue] /
                           sqrtf(sumTerm[kSumDenBlue_i]*sumTerm[kSumDenBlue_j]);

                score += corrRed + corrGreen + corrBlue;
            }

            //	What if one of the terms that should be at the denominator is null?
        }

    }

    return score;
}

void ImageStitcher::equalizeImages(RasterImage** image)
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

void ImageStitcher::restoreImages(RasterImage** image, sStitchPattern *thePattern)
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


void ImageStitcher::interpolateImages(RasterImage** image, short i, short j)
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
void ImageStitcher::computeIntersections(RasterImage** image, short i)
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


void ImageStitcher::computeAverages(RasterImage** image, short i, short j, float avg[])
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



void ImageStitcher::computeCorrTerms(RasterImage** image, short i, short j, float avg[],
                                     float sumTerm[])
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
    k_i = yMin[j]-displState[2*i+1];
    k_j = yMin[j]-displState[2*j+1];

    //	x and y are defined in the "global" image space, whereas k_i, k_j, and
    //	l_i, l_j are defined in the local spaces of images k and l respectively
    for (short y=yMin[j]; y<=yMax[j]; y++, k_i++, k_j++)
    {
        l_i = 3*(xMin[j]-displState[2*i]);
        l_j = 3*(xMin[j]-displState[2*j]);

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

    //	Now we do a second pass to compute the correlation coefficient
    sumTerm[kSumDenRed_i] = sumTerm[kSumDenGreen_i] = sumTerm[kSumDenBlue_i] = 0.f;
    sumTerm[kSumDenRed_j] = sumTerm[kSumDenGreen_j] = sumTerm[kSumDenBlue_j] = 0.f;
    sumTerm[kSumNumRed] = sumTerm[kSumNumGreen] = sumTerm[kSumNumBlue] = 0.f;
    k_i = yMin[j]-displState[2*i+1];
    k_j = yMin[j]-displState[2*j+1];

    //	x and y are defined in the "global" image space, whereas k_i, k_j, and
    //	l_i, l_j are defined in the local spaces of images k and l respectively
    for (short y=yMin[j]; y<=yMax[j]; y++, k_i++, k_j++)
    {
        l_i = 3*(xMin[j]-displState[2*i]);
        l_j = 3*(xMin[j]-displState[2*j]);

        for (short x=xMin[j]; x<=xMax[j]; x++)
        {
            termRed_i = rgb_i[k_i][l_i++] - avg[kAvgRed_i];
            termGreen_i = rgb_i[k_i][l_i++] - avg[kAvgGreen_i];
            termBlue_i = rgb_i[k_i][l_i++] - avg[kAvgBlue_i];
            //
            termRed_j = rgb_j[k_j][l_j++] - avg[kAvgRed_j];
            termGreen_j = rgb_j[k_j][l_j++] - avg[kAvgGreen_j];
            termBlue_j = rgb_j[k_j][l_j++] - avg[kAvgBlue_j];

            /*			sumTerm[kSumDenRed_i] += ABS(termRed_i);
            			sumTerm[kSumDenGreen_i] += ABS(termGreen_i); 
            			sumTerm[kSumDenBlue_i] += ABS(termBlue_i);
            			//
            			sumTerm[kSumDenRed_j] += ABS(termRed_j); 
            			sumTerm[kSumDenGreen_j] += ABS(termGreen_j); 
            			sumTerm[kSumDenBlue_j] += ABS(termBlue_j);
            			//
            */
            sumTerm[kSumDenRed_i] += termRed_i*termRed_i;
            sumTerm[kSumDenGreen_i] += termGreen_i*termGreen_i;
            sumTerm[kSumDenBlue_i] += termBlue_i*termBlue_i;
            //
            sumTerm[kSumDenRed_j] += termRed_j*termRed_j;
            sumTerm[kSumDenGreen_j] += termGreen_j*termGreen_j;
            sumTerm[kSumDenBlue_j] += termBlue_j*termBlue_j;
            //
            sumTerm[kSumNumRed] += 	termRed_i * termRed_j;
            sumTerm[kSumNumGreen] += termGreen_i * termGreen_j;
            sumTerm[kSumNumBlue] += termBlue_i * termBlue_j;
        }
    }
}


#pragma mark -


//-------------------------------------------------------------------
//	data allocation and de-allocation functions
//-------------------------------------------------------------------

void ImageStitcher::allocateStitchArrays(void)
{
    intersect = (bool *) calloc(nbImages, sizeof(bool));

    xMin = (short *) calloc(nbImages, sizeof(short));
    xMax = (short *) calloc(nbImages, sizeof(short));
    yMin = (short *) calloc(nbImages, sizeof(short));
    yMax = (short *) calloc(nbImages, sizeof(short));

    displState = (short *) calloc(2*nbImages, sizeof(short));
    qMin = (short *) calloc(2*nbImages, sizeof(short));
    qMax = (short *) calloc(2*nbImages, sizeof(short));
}

void ImageStitcher::deleteStitchArrays(void)
{
    if (intersect != NULL)
    {
        free((char* ) intersect);
        intersect = NULL;
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
    if (qMin != NULL)
    {
        free((char* ) qMin);
        qMin = NULL;
    }
    if (qMax != NULL)
    {
        free((char* ) qMax);
        qMax = NULL;
    }
    if (displState != NULL)
    {
        free((char* ) displState);
        displState = NULL;
    }
}