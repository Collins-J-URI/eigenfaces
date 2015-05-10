/*  NAME:
        Cluster.h
 
    DESCRIPTION:
        Cluster public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CLUSTER_H
#define URIVL_CLUSTER_H

#include <list>
#include <string>
#include <cmath>

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "DrawableObject2D.h"
#include "ImageRect.h"
#include "ImagePoint_F.h"
#include "Blob.h"

namespace uriVL
{
    //---------------------------------------------------------------------
    //  Data structures
    //---------------------------------------------------------------------

    /** Cluster using average distance
     */
    class URIVL_EXPORT Cluster
    {
        public:
        
            /** Default constructor
             */
//          Cluster(void);

            /** Constructor
             */
            Cluster(Blob* blob);

            /** Destructor
             */
            ~Cluster(void);

            

            /**
             *
             */
            void join(Cluster *cluster);

            /**
             *
             */
            double distance(Cluster *cluster);

            /**
             *
             */
//            BlobList getBlobList(void);
            Blob** getBlobList(void);
            int getNbBlobs(void);

        private:

            /**
             *
             */
//          BlobList    blobList_;
            Blob** blobList_;
            int  nbBlobs_;
            
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            Cluster(const Cluster& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const Cluster& operator =(const Cluster& obj);


    };

    typedef std::list<Cluster*>  ClusterList;
}

#endif  //  CLUSTER_H
