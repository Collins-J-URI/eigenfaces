/*  NAME:
        Registration2D_ICP.h

    DESCRIPTION:
        Registration2D_ICP public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_REGISTRATION_2D_ICP_H
#define URIVL_REGISTRATION_2D_ICP_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImagePoint.h"
#include "Registration2D.h"

namespace uriVL
{
    /** Class for objects that register two sets of image points using the Iterated Closest
     *  Point (ICP) algorithm of Besl and McKay.  
     *
     *  @author jean-yves herve', 3D Group for Interactive Visualization, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT  Registration2D_ICP : public Registration2D
    {
        public:
        
            /** Constructor
             */
            Registration2D_ICP(void);

            /** Destructor
             */
            ~Registration2D_ICP(void);

            /** Registers two image contours.  The contours are expected to be listed in
             *  counterclockwise order.
             *  @param  refPtList   the set of reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  guessTransf optional approximation to the solution
             *  @return     the transformation to apply to ptList to best fit refPtList
             */
            Transformation2D *registerPoints(const Contour *refPts, 
                                             const Contour *dataPts,
                                             const Transformation2D *guessTransf = NULL);

            /** Registers two sets of 2D image points.
             *  @param  refPtList   the set of reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  guessTransf optional approximation to the solution
             *  @return     the transformation to apply to ptList to best fit refPtList
             */
            Transformation2D *registerPoints(const std::vector<const ImagePoint*> *refPtList, 
                                             const std::vector<const ImagePoint*> *ptList,
                                             const Transformation2D *guessTransf = NULL);

        protected:

            /** Performs rigid linear registration of two sets of 2D image points.
             *  @param  nbRefPts    the number of of reference image points
             *  @param  refPtX      the array of x coordinates of the reference image points
             *  @param  refPtY      the array of y coordinates of the reference image points
             *  @param  transfRefX  work array for x coordinates of the transformed reference image points
             *  @param  transfRefY  work array for y coordinates of the transformed reference image points
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @param  match       array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  matchX      array of x coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @param  matchY      array of y coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @return   the transformation to apply to the reference to best fit the data
             */
            Transformation2D *registerPoints_lin_(int nbRefPts, const double* const refPtX, 
                                                  const double* refPtY,
												  double* transfRefX, double* transfRefY,
												  int nbPts, const double* ptX, const double* ptY,
												  bool *match, double* matchX, double* matchY);

            /** Performs rigid nonlinear registration of two sets of 2D image points.
             *  @param  nbRefPts    the number of of reference image points
             *  @param  refPtX      the array of x coordinates of the reference image points
             *  @param  refPtY      the array of y coordinates of the reference image points
             *  @param  transfRefX  work array for x coordinates of the transformed reference image points
             *  @param  transfRefY  work array for y coordinates of the transformed reference image points
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @param  match       array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  matchX      array of x coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @param  matchY      array of y coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @return   the transformation to apply to the reference to best fit the data
             */
            Transformation2D *registerPoints_nonlin_(int nbRefPts, const double* refPtX, const double* refPtY,
													 double* transfRefX, double* transfRefY,
													 int nbPts, const double* ptX, const double* ptY,
													 bool *match, double* matchX, double* matchY);

            /** Performs affine registration of two sets of 2D image points.
             *  @param  nbRefPts    the number of of reference image points
             *  @param  refPtX      the array of x coordinates of the reference image points
             *  @param  refPtY      the array of y coordinates of the reference image points
             *  @param  transfRefX  work array for x coordinates of the transformed reference image points
             *  @param  transfRefY  work array for y coordinates of the transformed reference image points
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @param  match       array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  matchX      array of x coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @param  matchY      array of y coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @return   the transformation to apply to the reference to best fit the data
             */
            Transformation2D *registerPoints_aff_(int nbRefPts, const double* refPtX, const double* refPtY,
												  double* transfRefX, double* transfRefY,
												  int nbPts, const double* ptX, const double* ptY,
												  bool *match, double* matchX, double* matchY);

            /** Determines for each data point which transformed reference point it is closest to.
             *  @param  nbRefPts    the number of of reference image points
             *  @param  refPtX      the array of x coordinates of the reference image points
             *  @param  refPtY      the array of y coordinates of the reference image points
             *  @param  transfRefX  work array for x coordinates of the transformed reference image points
             *  @param  transfRefY  work array for y coordinates of the transformed reference image points
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @param  transf      the 2D transformation for which to calculate the cost
             *  @param  match       output: array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  matchX      output: array of x coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @param  matchY      output: array of y coordinates of the "best match" point (in reference set) 
             *                          for each data point
             */
            void findBestMatch_(int nbRefPts, const double* refPtX, const double* refPtY,
                                double* transfRefX, double* transfRefY,
                                int nbPts, const double* ptX, const double* ptY,
                                Transformation2D *transf, bool *match, 
                                double* matchX, double* matchY);


            /** Determines for each contour data point which transformed reference point it is closest to.
             *  The distance used in this calculation is simply the Euclidean distance between two points.
             *  @param  refPtList   the set of reference image points
             *  @param  transfRefX  work array for x coordinates of the transformed reference image points
             *  @param  transfRefY  work array for y coordinates of the transformed reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  transf      the 2D transformation for which to calculate the cost
             *  @param  match       output: array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  matchX      output: array of x coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @param  matchY      output: array of y coordinates of the "best match" point (in reference set) 
             *                          for each data point
             */
            void findBestMatch_Vertex_(Contour *refPts, double* transfRefX, double* transfRefY,
                                       Contour *dataPts, Transformation2D *transf, 
                                       bool *match, double* matchX, double* matchY);

            /** Determines for each contour data point which transformed reference point it is closest to.
             *  The distance used in this calculation is measured along the normal vector at the data point.
             *  @param  refPtList   the set of reference image points
             *  @param  transfRefX  work array for x coordinates of the transformed reference image points
             *  @param  transfRefY  work array for y coordinates of the transformed reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  transf      the 2D transformation for which to calculate the cost
             *  @param  match       output: array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  matchX      output: array of x coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @param  matchY      output: array of y coordinates of the "best match" point (in reference set) 
             *                          for each data point
             */
            void findBestMatch_VertexNormal_(Contour *refPts, double* transfRefX, double* transfRefY,
                                             Contour *dataPts, Transformation2D *transf, 
                                             bool *match, double* matchX, double* matchY);

            /** Determines for each contour data point which point on a transformed reference edge (not
             *  The distance used in this calculation is simply the Euclidean distance between the data point
             *  and the closest point on the closest transformed edge.
             *  necessarily a vertex) it is closest to.
             *  @param  refPtList   the set of reference image points
             *  @param  transfRefX  work array for x coordinates of the transformed reference image points
             *  @param  transfRefY  work array for y coordinates of the transformed reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  transf      the 2D transformation for which to calculate the cost
             *  @param  match       output: array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  matchX      output: array of x coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @param  matchY      output: array of y coordinates of the "best match" point (in reference set) 
             *                          for each data point
             */
            void findBestMatch_Edge_(Contour *refPts, double* transfRefX, double* transfRefY,
                                     Contour *dataPts, Transformation2D *transf, 
                                     bool *match, double* matchX, double* matchY);


            /** Determines for each contour data point which point on a transformed reference edge (not
             *  necessarily a vertex) it is closest to.
             *  The distance used in this calculation is the Euclidean distance along the normal to the 
             *  data point to the closest transformed edge.
             *  @param  refPtList   the set of reference image points
             *  @param  transfRefX  work array for x coordinates of the transformed reference image points
             *  @param  transfRefY  work array for y coordinates of the transformed reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  transf      the 2D transformation for which to calculate the cost
             *  @param  match       output: array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  matchX      output: array of x coordinates of the "best match" point (in reference set) 
             *                          for each data point
             *  @param  matchY      output: array of y coordinates of the "best match" point (in reference set) 
             *                          for each data point
             */
            void findBestMatch_EdgeNormal_(Contour *refPts, double* transfRefX, double* transfRefY,
                                           Contour *dataPts, Transformation2D *transf, 
                                           bool *match, double* matchX, double* matchY);


            /** Computes the least-squares transformation for a list of matched points
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @param  match       array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  refPtX      the array of x coordinates of the "best match" reference image points
             *  @param  refPtY      the array of y coordinates of the "best match" reference image points
             *  @param  transf      output: the corresponding least squares 2D transformation
             *  @return the squared norm of the residual
             */
            double llsSolutionTransformation(int nbPts, const double* ptX, const double* ptY, const bool *match,
                                const double* refPtX, const double* refPtY, Transformation2D *transf);
            
            /** Evaluates the "weighted closest point" Sum of Squared Differences cost function 
             *  between the data points pt and the model refPt to which has been applied the 
             *  transformation
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @param  match       array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  refPtX      the array of x coordinates of the "best match" reference image points
             *  @param  refPtY      the array of y coordinates of the "best match" reference image points
             *  @param  transf      the 2D transformation for which to calculate the cost
             *  @return the cost value for the reference and data points and input transformation
             */
            float costFunc_SSD_(int nbPts, const double* ptX, const double* ptY, const bool *match,
                                const double* refPtX, const double* refPtY, Transformation2D *transf);

            /** Evaluates the "weighted closest point" Sum of Absolute Differences cost function 
             *  between the data points pt and the model refPt to which has been applied the transformation
             *  @param  nbPts       the number of of data image points
             *  @param  ptX         the array of x coordinates of the data image points
             *  @param  ptY         the array of y coordinates of the data image points
             *  @param  match       array indicating for each data point whether it has a match in the 
             *                          reference set
             *  @param  refPtX      the array of x coordinates of the "best match" reference image points
             *  @param  refPtY      the array of y coordinates of the "best match" reference image points
             *  @param  transf      the 2D transformation for which to calculate the cost
             *  @return the cost value for the reference and data points and input transformation
             */
            float costFunc_SAD_(int nbPts, const double* ptX, const double* ptY, const bool *match,
                                const double* refPtX, const double* refPtY, Transformation2D *transf);

    };
}

#endif  //  REGISTRATION_2D_ICP_H
