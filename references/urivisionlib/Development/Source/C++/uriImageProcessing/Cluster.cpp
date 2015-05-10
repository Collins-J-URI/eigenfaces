/*  NAME:
        Cluster.cpp
 
    DESCRIPTION:
        Cluster source file.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
//
#include "Cluster.h"

#error Work in progress: do not use this class yet

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------

/*
Cluster::Cluster(void)
{
	blobList_.clear();
}
*/

Cluster::Cluster(Blob* blob)
{
/*
	blobList_.clear();
	blobList_.push_back(blob);
*/
	blobList_ = new Blob*[1];
	blobList_[0] = blob;
	nbBlobs_ = 1;
}

Cluster::~Cluster(void)
{
/*
	if(!blobList_.empty())
		blobList_.erase(blobList_.begin(), blobList_.end());
*/
	delete blobList_;
}

Cluster::Cluster(const Cluster& theObj)
		:	GraphicComponent3D(),
			//
			vertex_(NULL),
			normal_(NULL),
			closedBox_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Cluster copy constructor not implemented.");
}



const Cluster& Cluster::operator = (const Cluster& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Cluster copy operator not implemented.");

	return *this;
}


//----------------------------------------------------------
//  public functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

void Cluster::join(Cluster *cl)
{
/*
cout << "joining to a list with " << blobList_.size() << "elements "
     << "a list with " << cl->getBlobList().size() << " elements" << endl;
int i=0;
      
	BlobList::iterator iter	= (cl->getBlobList()).begin();
	for( ; (iter != (cl->getBlobList()).end()) && 
			(i < (cl->getBlobList()).size()); iter++)
	{
		cout << "in join" << " i = " << i++ << endl;
		blobList_.push_back(*iter);
	}
*/
	Blob** temp = new Blob*[nbBlobs_ + cl->nbBlobs_];
	for (int i=0; i<nbBlobs_; i++)
		temp[i] = blobList_[i];
	for (int i=0; i<cl->nbBlobs_; i++)
		temp[i + nbBlobs_] = cl->blobList_[i];
	nbBlobs_ += cl->nbBlobs_;
	delete []blobList_;
	blobList_ = temp;
}

double Cluster::distance(Cluster *cl)
{
/*
	BlobList::iterator iter1	= cl->getBlobList().begin(),
						iter2	= blobList_.begin();
	
	float	avg1 = 0.f;
	for( ; iter1 != cl->getBlobList().end(); iter1++){
		float avg2 = 0.f;
		for( ; iter2 != blobList_.end(); iter2++){
			avg2 += sqrt(  ((*iter1)->getCentroid()->getX() - (*iter2)->getCentroid()->getX())
							* ((*iter1)->getCentroid()->getX() - (*iter2)->getCentroid()->getX())
						 + ((*iter1)->getCentroid()->getY() - (*iter2)->getCentroid()->getY())
						 	* ((*iter1)->getCentroid()->getY() - (*iter2)->getCentroid()->getY()));
		}
		avg2 /=	blobList_.size();
		avg1 += avg2;
	}
	avg1 /= cl->getBlobList().size();
*/

	float	avg1 = 0.f;
	for(int i=0; i<cl->nbBlobs_; i++) {
		float avg2 = 0.f;
		for (int j=0; j<nbBlobs_; j++) {
			avg2 += sqrt(  (cl->blobList_[i]->getCentroid()->getX() - blobList_[j]->getCentroid()->getX())
							* (cl->blobList_[i]->getCentroid()->getX() - blobList_[j]->getCentroid()->getX())
						 + (cl->blobList_[i]->getCentroid()->getY() - blobList_[j]->getCentroid()->getY())
						 	* (cl->blobList_[i]->getCentroid()->getY() - blobList_[j]->getCentroid()->getY()));
		}
		avg2 /=	nbBlobs_;
		avg1 += avg2;
	}
	avg1 /= cl->nbBlobs_;

	
	return avg1;
}


/*BlobList Cluster::getBlobList()
{
	return blobList_;
}
*/

Blob** Cluster::getBlobList(void)
{
	return blobList_;
}
int Cluster::getNbBlobs(void)
{
	return nbBlobs_;
}

//----------------------------------------------------------
//  private and protected functions
//----------------------------------------------------------

#if 0
#pragma mark -
#endif

