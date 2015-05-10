/*  NAME:
        Registration2D_ICP.cpp

    DESCRIPTION:
        implementation of the uriVisionLib Registration2D_ICP class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "Registration2D_ICP.h"

#include "RandomGenerator_C.h"

using namespace std;
using namespace uriVL;

//const int NB_REF_PTS = 14;

Registration2D_ICP::Registration2D_ICP(void)
{
/*
//    const int NB_REF_PTS = 14;
    double refPtX[NB_REF_PTS] = {2, 4, 0, 3, 4, 0, 2, 5, 3, 4, 0, 2, 1, 4}, 
           refPtY[NB_REF_PTS] = {7, 7, 6, 6, 6, 4, 4, 4, 3, 3, 1, 1, 0, 0},
           transfRefX[NB_REF_PTS],
           transfRefY[NB_REF_PTS];
    
    //  My data points are points {10, 11, 12, 1, 7, 8, 4, 9, 0, 2, 5, 3, 6, 13} rotated by pi/2, with point 1 at 10, 10

//  all pts, no rot
//    const int NB_PTS = 14;
//    double ptX[NB_PTS] = {5, 7, 6, 9, 10, 8, 9, 9, 7, 5},//, 5, 8, 7, 9},
//           ptY[NB_PTS] = {3, 3, 2, 9,  6, 5, 8, 5, 9, 8};//, 6, 8, 6, 2},

// all pts, rot 90
//    const int NB_PTS = 14;
//    double ptX[NB_PTS] = {15, 15, 16,  9, 12, 13, 10, 13, 9, 10, 12, 10, 12, 16},
//           ptY[NB_PTS] = { 6,  8,  7, 10, 11,  9, 10, 10, 8,  6,  6,  9,  8, 10};


//  top pts, rot 90
    const int NB_PTS = 7;
    double ptX[NB_PTS] = {12, 10, 9, 10,  9, 10, 12},
           ptY[NB_PTS] = { 6,  6, 8,  9, 10, 10, 11};

    double matchX[NB_PTS],
           matchY[NB_PTS];
	bool match[NB_PTS];


    registerPoints_lin_(NB_REF_PTS, refPtX, refPtY, transfRefX, transfRefY,
                        NB_PTS, ptX, ptY, match, matchX, matchY);

*/
}

Registration2D_ICP::~Registration2D_ICP(void)
{
}


#if 0
#pragma mark -
#endif


Transformation2D *Registration2D_ICP::registerPoints(const Contour *refPts, 
                                                     const Contour *dataPts,
                                                     const Transformation2D *guessTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Registration not implemented");

    Transformation2D    *transf = NULL;
    
    return transf;
}

Transformation2D *Registration2D_ICP::registerPoints(const vector<const ImagePoint*> *refPtList, 
                                                     const vector<const ImagePoint*> *ptList,
                                                     const Transformation2D *guessTransf)
{
    FAIL_CONDITION( refPtList == NULL,
                    kNullParameterError,
                    "null list of points passed to Registration2D object");
    FAIL_CONDITION( ptList == NULL,
                    kNullParameterError,
                    "null list of points passed to Registration2D object");

    int nbRefPts = refPtList->size();
    FAIL_CONDITION( nbRefPts == 0,
                    kInvalidParameterError,
                    "empty list of reference points passed to Registration2D object");

    int nbPts = ptList->size();
    FAIL_CONDITION( nbRefPts == 0,
                    kInvalidParameterError,
                    "empty list of data points passed to Registration2D object");

	//	Arrays storing a opy of the reference points
    double  *refPtX = new double[nbRefPts],
            *refPtY = new double[nbRefPts];
    for (int i=0; i<nbRefPts; i++)
    {
        refPtX[i] = (*refPtList)[i]->getX();
        refPtY[i] = (*refPtList)[i]->getY();
    }

	//	Arays of coordinates of "transformed" reference points
    double  *transfRefX = new double[nbRefPts],
            *transfRefY = new double[nbRefPts];

	//	Arrays storing a copy of the data points
    double  *ptX = new double[nbPts],
            *ptY = new double[nbPts];
    for (int k=0; k<nbPts; k++)
    {
        ptX[k] = (*ptList)[k]->getX();
        ptY[k] = (*ptList)[k]->getY();
    }

	//	Arrays of coordinates of the "best match" reference point of each data point
    double  *matchX = new double[nbPts],
            *matchY = new double[nbPts];
	//	Bool array indicating the presence of a match
	bool	*match = new bool[nbPts];
	
    Transformation2D    *transf = NULL;
    switch(regType_)
    {
        case RIGID_LINEAR_REG_2D:
            transf = registerPoints_lin_(nbRefPts, refPtX, refPtY, transfRefX, transfRefY,
										 nbPts, ptX, ptY, match, matchX, matchY);
            break;
        
        case RIGID_NONLINEAR_REG_2D:
            transf = registerPoints_nonlin_(nbRefPts, refPtX, refPtY, transfRefX, transfRefY,
										    nbPts, ptX, ptY, match, matchX, matchY);
            break;

        case AFFINE_REG_2D:
            transf = registerPoints_aff_(nbRefPts, refPtX, refPtY, transfRefX, transfRefY,
										 nbPts, ptX, ptY, match, matchX, matchY);
            break;

    }
    
    delete []matchY;
    delete []matchX;
    delete []ptY;
    delete []ptX;
	delete []transfRefY;
	delete []transfRefX;
    delete []refPtY;
    delete []refPtX;
    
    return transf;
}


#if 0
#pragma mark -
#endif


Transformation2D *Registration2D_ICP::registerPoints_lin_(int nbRefPts, const double* refPtX,
                								        const double* refPtY, double* transfRefX, 
                										double* transfRefY, int nbPts, const double* ptX, 
                										const double* ptY, bool *match, double* matchX, 
                									    double* matchY)
{
    RigidTransformation2D *transf = new RigidTransformation2D();
    
	//----------------------------------------------------------------------------
	//	1.	Try to put the registration on the right track (we hope)
	//----------------------------------------------------------------------------
	//	1.1	We set the translation to bring the centroids to coincidence
	double	sumRefX = 0.L,
			sumRefY = 0.L;
	for (int i=0; i<nbRefPts; i++)
	{
		sumRefX += refPtX[i];
		sumRefY += refPtY[i];
	}
	double	sumPtX = 0.L,
			sumPtY = 0.L;
	for (int i=0; i<nbPts; i++)
	{
		sumPtX += ptX[i];
		sumPtY += ptY[i];
	}

double cx = sumPtX/nbPts,
       cRefX = sumRefX/nbRefPts,
       cy = sumPtY/nbPts,
       cRefY = sumRefY/nbRefPts,
       tx, ty;

    transf->setTranslation(	static_cast<float>(sumPtX/nbPts - sumRefX/nbRefPts), 
							static_cast<float>(sumPtY/nbPts - sumRefY/nbRefPts));

    float angle = 0.f;
    float bestAngle = 0.f, lowestCost = 10000000.f;
	const float angleIncr = static_cast<float> (2*PI_F/12);
	//
    do
    {
        tx = cx - cRefX*cosf(angle) + cRefY*sinf(angle);
        ty = cy - cRefY*cosf(angle) - cRefX*sinf(angle);
        transf->setAngle(angle);
        transf->setTranslation(tx, ty);
cout << endl << "angle = " << angle << endl;
        findBestMatch_(nbRefPts, refPtX, refPtY, transfRefX, transfRefY, nbPts, ptX, ptY,
                       transf, match, matchX, matchY);
        double cost = llsSolutionTransformation(nbPts, ptX, ptY, match, matchX, matchY, transf);
cout << endl << "\t--> transf = (" << transf->getTransX() << ", " << transf->getTransY() << " | " <<
                          transf->getAngle() << ")" << endl;
cout << "\tcost = " << cost << endl;
       
        if (cost < lowestCost)
        {
            lowestCost = cost;
            bestAngle = transf->getAngle();
        }
        angle += angleIncr;
    }
    while (angle < 2*PI_F);
    
	
    transf->setAngle(static_cast<float>(bestAngle));
    tx = cx - cRefX*cosf(bestAngle) + cRefY*sinf(bestAngle);
    ty = cy - cRefY*cosf(bestAngle) - cRefX*sinf(bestAngle);
    transf->setTranslation(static_cast<float>(tx), static_cast<float>(ty));
    const int NB_RUNS = 20;
    int k = 0;
    do
    {
cout << "Iteration " << k << endl << "------------" << endl;
        findBestMatch_(nbRefPts, refPtX, refPtY, transfRefX, transfRefY, nbPts, ptX, ptY,
                       transf, match, matchX, matchY);
        double cost = llsSolutionTransformation(nbPts, ptX, ptY, match, matchX, matchY, transf);
cout << "\ttransf = (" << transf->getTransX() << ", " << transf->getTransY() << " | " <<
                          transf->getAngle() << ")" << endl;
cout << "\tcost = " << cost << endl;
        k++;
    } while (k <NB_RUNS);
    
    
    return transf;
}

Transformation2D *Registration2D_ICP::registerPoints_nonlin_(int nbRefPts, const double* refPtX, 
											const double* refPtY,
											double* transfRefX, double* transfRefY,
											int nbPts, const double* ptX, const double* ptY,
											bool *match, double* matchX, double* matchY)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Registration not implemented");

    Transformation2D    *transf = NULL;
    
    return transf;
}

Transformation2D *Registration2D_ICP::registerPoints_aff_(int nbRefPts, const double* refPtX, 
											const double* refPtY,
											double* transfRefX, double* transfRefY,
											int nbPts, const double* ptX, const double* ptY,
											bool *match, double* matchX, double* matchY)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Registration not implemented");

    Transformation2D    *transf = NULL;
    
    return transf;
}


#if 0
#pragma mark -
#endif

void Registration2D_ICP::findBestMatch_(int nbRefPts, const double* refPtX, const double* refPtY,
                                        double* transfRefX, double* transfRefY,
                                        int nbPts, const double* ptX, const double* ptY,
                                        Transformation2D *transf, bool *match, double* matchX, 
                                        double* matchY)
{
//int bestMatch[NB_REF_PTS];

    const float*const*const mat = transf->getMatrix();

    //--------------------------------------------------------------------------
    //  1.  Apply the transformation to each point of the reference set
    //--------------------------------------------------------------------------
    if (transf->isRigidTransformation())
    {
        for (int i=0; i<nbRefPts; i++)
        {
            transfRefX[i] = mat[0][0]*refPtX[i] + mat[0][1]*refPtY[i] + mat[0][2];
            transfRefY[i] = mat[1][0]*refPtX[i] + mat[1][1]*refPtY[i] + mat[1][2];
        }
    }
    else
    {
        for (int i=0; i<nbRefPts; i++)
        {
            //  compute scaling term
            double s = mat[2][0]*refPtX[i] + mat[2][1]*refPtY[i] + mat[2][2];

            //  compute displacement between the data point and the transformed reference point
            transfRefX[i] = (mat[0][0]*refPtX[i] + mat[0][1]*refPtY[i] + mat[0][2])/s;
            transfRefX[i] = (mat[1][0]*refPtX[i] + mat[1][1]*refPtY[i] + mat[1][2])/s;
        }
    }

    //--------------------------------------------------------------------------
    //  2.  Iterate over data points and find closest transformed point
    //--------------------------------------------------------------------------
    for (int k=0; k<nbPts; k++)
    {
        double  smallestDist2 = 1000000000000000.L;    //  out of workspace
        
        for (int i=0; i<nbRefPts; i++)
        {
            double  dx = ptX[k] - transfRefX[i],
                    dy = ptY[k] - transfRefY[i];
            double  d2 = dx*dx + dy*dy;
            
            if (d2 < smallestDist2)
            {
                smallestDist2 = d2;
                matchX[k] = refPtX[i];
                matchY[k] = refPtY[i];
//bestMatch[k] = i;
            }
        }
        
        if (smallestDist2 > noMatchThreshold_*noMatchThreshold_)
//{
            match[k] = false;
//bestMatch[k] = -1;
//}
        else
            match[k] = true;
            
    }

//cout << "\tmatch: ";
//for (int k=0; k<nbPts; k++)
//    cout << "(" << k << " : " << bestMatch[k] << ")  ";
//cout << flush;
}


void Registration2D_ICP::findBestMatch_Vertex_(Contour *refPts, double* transfRefX, double* transfRefY,
                                               Contour *dataPts, Transformation2D *transf, 
                                               bool *match, double* matchX, double* matchY)
{
}

void Registration2D_ICP::findBestMatch_VertexNormal_(Contour *refPts, double* transfRefX, double* transfRefY,
                                               Contour *dataPts, Transformation2D *transf, 
                                               bool *match, double* matchX, double* matchY)
{
}

void Registration2D_ICP::findBestMatch_Edge_(Contour *refPts, double* transfRefX, double* transfRefY,
                                             Contour *dataPts, Transformation2D *transf, 
                                             bool *match, double* matchX, double* matchY)
{
}

void Registration2D_ICP::findBestMatch_EdgeNormal_(Contour *refPts, double* transfRefX, double* transfRefY,
                                             Contour *dataPts, Transformation2D *transf, 
                                             bool *match, double* matchX, double* matchY)
{
}


#if 0
#pragma mark -
#endif


double Registration2D_ICP::llsSolutionTransformation(int nbPts, const double* ptX, const double* ptY, 
                                                        const bool *match, const double* refPtX, 
                                                        const double* refPtY, Transformation2D *transf)
{
    //------------------------------------------------------------------------
    //  Do the basic summations of x & y coordinates of the reference and data
    //  points, dot products, and determinants
    //------------------------------------------------------------------------
	int	matchCount = 0;
	double	sumPtX = 0, sumPtY = 0, 
			sumRefX = 0, sumRefY = 0,
			sumDet = 0, sumDotProd = 0;
	for (int i=0; i<nbPts; i++)
	{
		if (match[i])
		{
			sumPtX += ptX[i];
			sumPtY += ptY[i];
			sumRefX += refPtX[i];
			sumRefY += refPtY[i];
			sumDet += ptX[i]*refPtY[i] - ptY[i]*refPtX[i];
			sumDotProd += ptX[i]*refPtX[i] + ptY[i]*refPtY[i];
			//
			matchCount++;
		}
	}
	
    //------------------------------------------------------------------------
    //  Compute the angle and translation of the rigid transformation
    //------------------------------------------------------------------------
	const double meanPtX = sumPtX / matchCount,
				 meanPtY = sumPtY / matchCount,
				 meanRefX = sumRefX / matchCount,
				 meanRefY = sumRefY / matchCount,
				 meanDet = sumDet / matchCount,
				 meanDotProd = sumDotProd / matchCount;
	const double dotProdMean = meanPtX*meanRefX + meanPtY*meanRefY,
				 detMean = meanPtX*meanRefY - meanPtY*meanRefX;

	const double A = detMean - meanDet,
				 B = dotProdMean - meanDotProd,
				 normAB = sqrt(A*A + B*B);

	const double cosTheta = -B / normAB,
				 sinTheta = A / normAB;
				 
	const double tx = meanPtX - cosTheta*meanRefX + sinTheta*meanRefY,
				 ty = meanPtY - sinTheta*meanRefX - cosTheta*meanRefY;
				 
    //------------------------------------------------------------------------
    //  Update the transformation's matrix
    //------------------------------------------------------------------------
	float* const* const mat = transf->getMatrix(R_W_ACCESS);
	mat[0][0] = mat[1][1] =  static_cast<float>(cosTheta);
	mat[0][1] = - (mat[1][0] = static_cast<float>(sinTheta));
	//
	mat[0][2] = static_cast<float>(tx);
	mat[1][2] = static_cast<float>(ty);
	//
	mat[2][0] = mat[2][1] = 0.f;
	mat[2][2] = 1.f;

    //------------------------------------------------------------------------
    //  And finally compute the squared norm of the residual
    //------------------------------------------------------------------------
	double  normSqRes = 0.L;
    for (int i=0; i<nbPts; i++)
	{
		if (match[i])
		{
            const double dx = ptX[i] - (cosTheta*refPtX[i] - sinTheta*refPtY[i] + tx),
                         dy = ptY[i] - (sinTheta*refPtX[i] + cosTheta*refPtY[i] + ty);

			normSqRes += dx*dx + dy*dy;
		}
	}

    return normSqRes;
}


#if 0
#pragma mark -
#endif


//  
float Registration2D_ICP::costFunc_SSD_(int nbPts, const double* ptX, const double* ptY, 
                                        const bool *match, const double* refPtX, 
                                        const double* refPtY, Transformation2D *transf)
{
    const float*const*const mat = transf->getMatrix();
    double costSum = 0.L;
    
    if (transf->isRigidTransformation())
    {
        for (int i=0; i<nbPts; i++)
        {
            //  did we find a match for pt[i]?
            if ( match[i])
            {
                const double refX = refPtX[i],
                             refY = refPtY[i];

                //  compute displacement between the data point and the transformed reference point
                const double dx = ptX[i] - (mat[0][0]*refX + mat[0][1]*refY + mat[0][2]);
                const double dy = ptY[i] - (mat[1][0]*refX + mat[1][1]*refY + mat[1][2]);
                
                costSum += dx*dx + dy*dy;
            }
            else
                costSum += noMatchPenalty_;
                
        }
    }
    else
    {
        for (int i=0; i<nbPts; i++)
        {
            //  did we find a match for pt[i]?
            if ( match[i])
            {
                const double refX = refPtX[i],
                             refY = refPtY[i];

                //  compute scaling term
                const double s = mat[2][0]*refX + mat[2][1]*refY + mat[2][2];

                //  compute displacement between the data point and the transformed reference point
                const double dx = ptX[i] - (mat[0][0]*refX + mat[0][1]*refY + mat[0][2])/s;
                const double dy = ptY[i] - (mat[1][0]*refX + mat[1][1]*refY + mat[1][2])/s;

                costSum += dx*dx + dy*dy;
            }
            else
                costSum += noMatchPenalty_;
                
        }
    }
    
    return static_cast<float>(costSum / nbPts);
}


float Registration2D_ICP::costFunc_SAD_(int nbPts, const double* ptX, const double* ptY, 
                                        const bool *match, const double* refPtX, 
                                        const double* refPtY, Transformation2D *transf)
{
    const float*const*const mat = transf->getMatrix();
    double costSum = 0.;
    
    if (transf->isRigidTransformation())
    {
        for (int i=0; i<nbPts; i++)
        {
            //  did we find a match for pt[i]?
            if ( match[i])
            {
                const double refX = refPtX[i],
                             refY = refPtY[i];

                //  compute displacement between the data point and the transformed reference point
                costSum += fabs(ptX[i] - (mat[0][0]*refX + mat[0][1]*refY + mat[0][2])) +
                           fabs(ptY[i] - (mat[1][0]*refX + mat[1][1]*refY + mat[1][2]));

            }
            else
                costSum += noMatchPenalty_;
                
        }
    }
    else
    {
        for (int i=0; i<nbPts; i++)
        {
            //  did we find a match for pt[i]?
            if ( match[i])
            {
                const double refX = refPtX[i],
                             refY = refPtY[i];

                //  compute scaling term
                const double s = mat[2][0]*refX + mat[2][1]*refY + mat[2][2];

                //  compute displacement between the data point and the transformed reference point
                costSum += fabs(ptX[i] - (mat[0][0]*refX + mat[0][1]*refY + mat[0][2])/s) +
                           fabs(ptY[i] - (mat[1][0]*refX + mat[1][1]*refY + mat[1][2])/s);

            }
            else
                costSum += noMatchPenalty_;
                
        }
    }
    
    return static_cast<float>(costSum / nbPts);
}

