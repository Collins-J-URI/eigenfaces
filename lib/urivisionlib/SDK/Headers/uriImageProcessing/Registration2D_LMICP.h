/*  NAME:
        Registration2D_LMICP.h

    DESCRIPTION:
        Registration2D_LMICP public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_REGISTRATION_2D_LMICP_H
#define URIVL_REGISTRATION_2D_LMICP_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImagePoint.h"
#include "Registration2D.h"


namespace uriVL
{
    /** Class for objects that register two sets of image points using the Levenberg-Marquardt
     *  formulation of the Iterated Closest Point (ICP) algorithm, as proposed by 
     *  Fitzgibbon (2001)  
     *
     *  @author jean-yves herve', 3D Group for Interactive Visualization, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT  Registration2D_LMICP : public Registration2D
    {
        public:
        
            /** Constructor
             */
            Registration2D_LMICP(void);

            /** Destructor
             */
            ~Registration2D_LMICP(void);

            /** Registers two image contours.  The contours are expected to be listed in
             *  counterclockwise order.
             *  @param  refPtList   the set of reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  guessTransf optional approximation to the solution
             *  @return     the transformation to apply to ptList to best fit refPtList
             */
            Transformation2D *registerPoints(Contour *refPts, 
                                             Contour *dataPts,
                                             Transformation2D *guessTransf = NULL);

            /** Registers two sets of 2D image points.
             *  @param  refPtList   the set of reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  guessTransf optional approximation to the solution
             *  @return     the transformation to apply to ptList to best fit refPtList
             */
            Transformation2D *registerPoints(const std::vector<ImagePoint*> *refPtList, 
                                             const std::vector<ImagePoint*> *ptList,
                                             Transformation2D *guessTransf = NULL);


        protected:

            /** Performs rigid linear registration of two sets of 2D image points.
             *  @param  nbRefPts    the number of of reference image points
             *  @param  refPtX      the array of x coordinates of the reference image points
             *  @param  refPtY      the array of y coordinates of the reference image points
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @return   the transformation to apply to the reference to best fit the data
             */
            Transformation2D *registerPoints_lin_(int nbRefPts, const double* refPtX, 
                                                  const double* refPtY, int nbPts, 
                                                  const double* ptX, const double* ptY);

            /** Performs rigid nonlinear registration of two sets of 2D image points.
             *  @param  nbRefPts    the number of of reference image points
             *  @param  refPtX      the array of x coordinates of the reference image points
             *  @param  refPtY      the array of y coordinates of the reference image points
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @return   the transformation to apply to the reference to best fit the data
             */
            Transformation2D *registerPoints_nonlin_(int nbRefPts, const double* refPtX, 
                                                     const double* refPtY, int nbPts, 
                                                     const double* ptX, const double* ptY);

            /** Performs affine registration of two sets of 2D image points.
             *  @param  nbRefPts    the number of of reference image points
             *  @param  refPtX      the array of x coordinates of the reference image points
             *  @param  refPtY      the array of y coordinates of the reference image points
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @return   the transformation to apply to the reference to best fit the data
             */
            Transformation2D *registerPoints_aff_(int nbRefPts, const double* refPtX, 
                                                  const double* refPtY, int nbPts, 
                                                  const double* ptX, const double* ptY);


            /** Evaluates the "weighted closest point" cost function between the data points pt
             *  and the model refPt to which has been applied the transformation
             *  @param  nbRefPts    the number of of reference image points
             *  @param  refPtX      the array of x coordinates of the reference image points
             *  @param  refPtY      the array of y coordinates of the reference image points
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @param  transf      the 2D transformation for which to calculate the cost
             *  @return the cost value for the reference and data points and input transformation
             */
            static float costFunc(int nbRefPts, const double* refPtX, const double* refPtY, 
                                  int nbPts, const double* ptX, const double* ptY,
                                  const Transformation2D *transf,
                                  const int* match);
    };
}

#endif  //  REGISTRATION_2D_LMICP_H
