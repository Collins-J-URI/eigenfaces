/*  NAME:
        Registration2D.h

    DESCRIPTION:
        Registration2D public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_REGISTRATION_2D_H
#define URIVL_REGISTRATION_2D_H

#include <vector>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImagePoint.h"
#include "Contour.h"
#include "Transformation2D.h"
#include "LinearSolver_QR.h"
#include "LinearSolver_SVD.h"


namespace uriVL
{
    /** Basic types of 2D registrations supported by this class
     */
    typedef enum Registration2DType
    {
        RIGID_LINEAR_REG_2D = 0,        //  rigid (Euclidean), using cos(theta) sin(theta) as independent variables
        RIGID_NONLINEAR_REG_2D,         //  rigid (Euclidean), using theta variable
        AFFINE_REG_2D                   //  affine registration (6 parameters of transf. matrix as variables)
        //  WARPING_REG_2D
        
    } Registration2DType;
    

    /** Defines the different data point selection modes supported
     */
    typedef enum Point2DSelectionMode
    {
        SELECT_ALL_POINTS = 0,          //  all data points are used
        SELECT_RANDOM_POINT_SAMPLE,     //  a different random sample of points is used at each iteration
        MAX_GRADIENT_DISTRIBUTION       //  points chosen so that the distrution of normal vector directions
                                        //      is as large as possible

    } Point2DSelectionMode;
        

    /** Defines the different matching metric types supported
     */
    typedef enum Point2DMatchingMetric
    {
        POINT_POINT_DISTANCE,           //  Euclidean distance between data point and transformed ref point
        POINT_POINT_NORMAL_DISTANCE,    //  Euclidean distance between data point and transformed ref point
                                        //      measured along normal at data point (only for contour reg)
        POINT_POINT_DISTANCE_SQ,        //  Square of Euclidean distance between data point and transformed ref point
        POINT_POINT_NORMAL_DISTANCE_SQ, //  Square of Euclidean distance between data point and transformed ref point
                                        //      measured along normal at data point (only for contour reg)
        POINT_EDGE_DISTANCE,            //  Euclidean distance between data point and transformed edge
                                        //      (only for contour reg)
        POINT_EDGE_NORMAL_DISTANCE,     //  Euclidean distance between data point and transformed edge
                                        //      measured along normal at data point (only for contour reg)
        POINT_EDGE_DISTANCE_SQ,         //  Square of Euclidean distance between data point and transformed edge
                                        //      (only for contour reg)
        POINT_EDGE_NORMAL_DISTANCE_SQ   //  Square of Euclidean distance between data point and transformed edge
                                        //      measured along normal at data point (only for contour reg)
        

    } Point2DMatchingMetric;
        


    /** Defines the matching pair rejection modes supported by the registration algorithm.
     */
    typedef enum Point2DPairRejectMode
    {
        RETAIN_ALL_PAIRS,               //  retain all match pairs
        THRESHOLD_BASED_REJECT,         //  reject pair (p, q) if d(p, q) > D
        PERCENTAGE_REJECT,              //  reject n% worst matches
        STANDARD_DEV_REJECT,            //  reject (p, q) id d(p, q) > s StandardDev(d(p', q'))
        NEIGHBOR_INCONSISTENCY_REJECT   //  reject (p, q) if for more than n out m match pairs (p', q')
                                        //     |d(p, p') - d(q,q')| > s . max(d(p, p'), d(q,q'))
        
    } Point2DPairRejectMode;
    
     

    /** Defines the weighting schemes for pairs of matching points that the registration 
     *  algorithm supports.
     */
    typedef enum MatchPairWeightingMode
    {
        EQUAL_WEIGHT = 0,           //  all retained pairs weigh 1
        DISTANCE_BASED_WEIGHT,      //  1 - d(p, q) / dMax
        ANGLE_BASED_WEIGHT,         //  1 - normal(p) . normal(q)
        COLOR_BASED_WEIGHT          //  When a color or gray level was assigned to each point,
                                    //      1 - |col(p) - col(q)|/range(col)
        
    } MatchPairWeightingMode;
    

    
    /** Virtual parent class for objects that register two sets of image points or (later)
     *  two image regions.  A Registration2D object produces a Transformation2D object.
     *  The type of transformation will depend on the type of algorithm used.
     *
     *  @author jean-yves herve', 3D Group for Interactive Visualization, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT  Registration2D
    {
        public:
        
            /** Constructor
             */
            Registration2D(void);

            /** Destructor
             */
            virtual ~Registration2D(void);

            /** Registers two image contours.  The contours are expected to be listed in
             *  counterclockwise order.
             *  @param  refPtList   the set of reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  guessTransf optional approximation to the solution
             *  @return     the transformation to apply to ptList to best fit refPtList
             */
            virtual Transformation2D *registerPoints(const Contour *refPts, 
                                                     const Contour *dataPts,
                                                     const Transformation2D *guessTransf = NULL) = 0;

            /** Registers two sets of 2D image points.
             *  @param  refPtList   the set of reference image points
             *  @param  ptList      the set of points to register to the reference
             *  @param  guessTransf optional approximation to the solution
             *  @return     the transformation to apply to ptList to best fit refPtList
             */
            virtual Transformation2D *registerPoints(const std::vector<const ImagePoint*> *refPtList, 
                                                     const std::vector<const ImagePoint*> *ptList,
                                                     const Transformation2D *guessTransf = NULL) = 0;

            /** Sets the type of registration calculations to perform.
             *  @param  regType    type of registration calculations to perform
             */
            void setRegistrationType(Registration2DType regType);

            /** Sets the criterion for point selection
             *  @param  ptSelectMode    new criterion for point selection
             */
            void setPointSelectionMode(Point2DSelectionMode ptSelectMode);

            /** Sets the matching metric
             *  @param  matchingMetric    new matching metric
             */
            void setMatchingMetric(Point2DMatchingMetric matchingMetric);

            /** Sets the criterion for rejecting matching pairs
             *  @param  pairRejectionMode    new criterion for rejecting matching pairs
             */
            void setPairRejectionMode(Point2DPairRejectMode pairRejectionMode);

            /** Sets the weigthing scheme for combining the scores of matching pairs
             *  @param  pairWeightingMode    new weigthing scheme
             */
            void setPairWeightingMode(MatchPairWeightingMode pairWeightingMode);

            /** Sets the distance beyond which we consider that a data point finds no match
             *  in the reference set
             *  @param  noMatchThreshold    new "no match" threshold for interpoint distance
             */
            void setNoMatchThreshold(float noMatchThreshold);

            /** Sets the penalty for a "no match" in the global cost function
             *  @param  noMatchPenalty    new penalty for a "no match"
             */
            void setNoMatchPenalty(float noMatchPenalty);


        protected:

            /** Default threshold for "no match distance"
             */
            static const float DEFAULT_NO_MATCH_THRESHOLD;
            
            /** Default penalty for a no-match
             */
            static const float DEFAULT_NO_MATCH_PENALTY;
            
            /** Default percentage of pairs to reject
             */
            static const float DEFAULT_PAIR_REJECT_PERCENT;
            
            /** Default standard deviation multiplier for STANDARD_DEV_REJECT mode
             */
            static const float DEFAULT_STANDARD_DEV_REJECT_MULT;
            
            /** Default number of "neighbor" pairs to compare to in NEIGHBOR_INCONSISTENCY_REJECT mode
             */
            static const int DEFAULT_NUMBER_NEIGHBOR_COMP;
            
            /** Default number of neighbors that must fail the test to reject the pair
             *  in NEIGHBOR_INCONSISTENCY_REJECT mode
             */
            static const int DEFAULT_MIN_NUMBER_FAIL_NEIGHBOR;

            /** Default scaling factor to reject the pair in NEIGHBOR_INCONSISTENCY_REJECT mode
             */
            static const float DEFAULT_FAIL_NEIGHBOR_SCALE;

            /** Linear solver used to compute the best-fit parameters
             */
            LinearSolver *solver_;

            /** what kind of registration to perform
             */
            Registration2DType regType_;

            /** How data points to match are selected
             */
            Point2DSelectionMode ptSelectMode_;
            
            /** The metric used to evaluate quality of match
             */
            Point2DMatchingMetric matchingMetric_;
            
            /** Criterion used to reject matching pairs
             */
            Point2DPairRejectMode pairRejectionMode_;
            
            /** weighting scheme used to combine pairs' matching scores
             */
            MatchPairWeightingMode pairWeightingMode_;

            /** threshold for "no match distance" with THRESHOLD_BASED_REJECT
             */
            float noMatchThreshold_;
            
            /** Penalty for a no-match (match rejected)
             */
            float noMatchPenalty_;

    };            
}

#endif  //  REGISTRATION_2D_H
