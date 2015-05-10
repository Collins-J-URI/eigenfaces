/*  NAME:
        Registration2D_LPCD.cpp

    DESCRIPTION:
        implementation of the uriVisionLib Registration2D_LPCD class

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "Registration2D_LPCD.h"

using namespace std;
using namespace uriVL;


Registration2D_LPCD::Registration2D_LPCD()
{
}

Registration2D_LPCD::~Registration2D_LPCD(void){
}

#if 0
#pragma mark -
#endif


Transformation2D *Registration2D_LPCD::registerPoints(const Contour *refPts, 
                                                     const Contour *dataPts,
                                                     const Transformation2D *guessTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Registration not implemented");

    Transformation2D    *transf = NULL;
    
    return transf;
}

Transformation2D *Registration2D_LPCD::registerPoints(const vector<const ImagePoint*> *refPtList, 
                                                     const vector<const ImagePoint*> *ptList,
                                                     const Transformation2D *guessTransf)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Registration not implemented");

    FAIL_CONDITION( refPtList == NULL,
                    kNullParameterError,
                    "null list of points passed to Registration2D object");
    FAIL_CONDITION( ptList == NULL,
                    kNullParameterError,
                    "null list of points passed to Registration2D object");
                    
    Transformation2D    *transf = NULL;
    
    return transf;
}

#if 0
#pragma mark -
#endif

Transformation2D *Registration2D_LPCD::registerPoints_lin_(int nbRefPts, 
                                                           const double* const refPtX, 
                                                           const double* const  refPtY, int nbPts, 
                                                           const double* const  ptX, 
                                                           const double* const  ptY)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Registration not implemented");

    Transformation2D    *transf = NULL;
    
    return transf;
}

Transformation2D *Registration2D_LPCD::registerPoints_nonlin_(int nbRefPts, 
                                                               const double* const refPtX, 
                                                               const double* const  refPtY, int nbPts, 
                                                               const double* const  ptX, 
                                                               const double* const  ptY)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Registration not implemented");

    Transformation2D    *transf = NULL;
    
    return transf;
}

Transformation2D *Registration2D_LPCD::registerPoints_aff_(int nbRefPts, 
                                                           const double* const refPtX, 
                                                           const double* const  refPtY, int nbPts, 
                                                           const double* const  ptX, 
                                                           const double* const  ptY)
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


float Registration2D_LPCD::costFunc(int nbRefPts, const double* const refPtX, 
                                    const double* const refPtY, 
                                    int nbPts, const double* const ptX, 
                                    const double* const ptY,
                                    const Transformation2D *transf,
                                    const int* match)
{
    return 0.f;
}

