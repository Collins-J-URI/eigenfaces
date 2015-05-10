/*  NAME:
        ReferenceFrame3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ReferenceFrame3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <pthread.h>
//
#include "ReferenceFrame3D.h"

using namespace std;
using namespace uriVL;

//------------------------------
//  Static members
//------------------------------

int ReferenceFrame3D::frameCounter_ = 0;
ReferenceFrame3D *ReferenceFrame3D::rootFrame_ = new ReferenceFrame3D();
const ReferenceFrame3D *ReferenceFrame3D::viewerFrame_ = NULL;

static pthread_mutex_t viewerFrameLock[1] = {PTHREAD_MUTEX_INITIALIZER};


#if 0
//----------------------------------------
#pragma mark Constructors and destructor
//----------------------------------------
#endif

ReferenceFrame3D::ReferenceFrame3D(void)
        :   id_(0),
            ref_(rootFrame_),
            transf_(NULL),
            entitiesReferenced_(list<const Entity3D*>())
{
    frameCounter_++;
}
 


ReferenceFrame3D::ReferenceFrame3D(const ReferenceFrame3D* theRef)
        :   id_(frameCounter_),
            ref_(theRef),
            transf_(NULL),
            entitiesReferenced_(list<const Entity3D*>())
{
    frameCounter_++;
}

ReferenceFrame3D::ReferenceFrame3D(const RigidTransformation3D* theTransf)
        :   id_(frameCounter_),
            ref_(rootFrame_),
            transf_(new RigidTransformation3D(theTransf)),
            entitiesReferenced_(list<const Entity3D*>())
{
    frameCounter_++;
}

ReferenceFrame3D::ReferenceFrame3D(const ReferenceFrame3D* theRef, const RigidTransformation3D *theTransf)
        :   id_(frameCounter_),
            ref_(theRef),
            transf_(new RigidTransformation3D(theTransf)),
            entitiesReferenced_(list<const Entity3D*>())
{
    frameCounter_++;
}

ReferenceFrame3D::ReferenceFrame3D(const ReferenceFrame3D& theRef)
        :   id_(0),
            ref_(NULL),
            transf_(NULL),
            entitiesReferenced_(list<const Entity3D*>())
{
    FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"ReferenceFrame3D copy constructor is disabled");;
}

const ReferenceFrame3D& ReferenceFrame3D::operator = (const ReferenceFrame3D& theRef)
{
    FAIL_CONDITION (true,
					kFunctionNotImplemented,
					"ReferenceFrame3D copy operator is disabled");;
	return *this;
}

bool ReferenceFrame3D::deleteFrame(ReferenceFrame3D* ref)
{
    if (ref->entitiesReferenced_.empty()) {
        delete ref;
        return true;
    }
    else
        return false;
        
}

ReferenceFrame3D::~ReferenceFrame3D(void)
{
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Dealings with parent frame
//------------------------------------------------------
#endif


const ReferenceFrame3D* ReferenceFrame3D::getParentFrame(void)
{
    return ref_;
}

void ReferenceFrame3D::notifyUseAsReference(const Entity3D* theEntity) const
{
    entitiesReferenced_.push_back(theEntity);
}

void ReferenceFrame3D::notifyReleaseReference(const Entity3D* theEntity) const
{
	entitiesReferenced_.remove(theEntity);
}


#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Transformations
//------------------------------------------------------
#endif


void ReferenceFrame3D::setTransformation(const RigidTransformation3D* thePose)
{
    if (transf_ != NULL)
        delete transf_;
    transf_ = new RigidTransformation3D(thePose);
}

void ReferenceFrame3D::applyTransformation(const RigidTransformation3D* theTransf)
{
    if (transf_ != NULL) {
        ((RigidTransformation3D*) transf_)->postComposeTransformation(theTransf);
    }
    else {
        transf_ = new RigidTransformation3D(theTransf);
    }
}

void ReferenceFrame3D::applyTransformation(const Rotation3D* theRot)
{
    if (transf_ != NULL) {
        ((RigidTransformation3D*) transf_)->postComposeTransformation(theRot);
    }
    else {
        transf_ = new RigidTransformation3D(theRot);
    }
}

void ReferenceFrame3D::applyTransformation(const Translation3D* theTrans)
{
    if (transf_ != NULL) {
        ((RigidTransformation3D*) transf_)->postComposeTransformation(theTrans);
    }
    else {
        transf_ = new RigidTransformation3D(theTrans);
    }
}

const RigidTransformation3D* ReferenceFrame3D::getTransformation(ReferenceFrameShortcut ref) const
{
	switch (ref)
	{
		case WORLD_FRAME:
			return getTransformation(rootFrame_, this);
			break;
			
		case VIEWER_FRAME:
			return getTransformation(viewerFrame_, this);
			break;
			
		case PARENT_FRAME:
		default:
			return transf_;
			break;
			
	}
}

const RigidTransformation3D* ReferenceFrame3D::getTransformation(const ReferenceFrame3D* theFrame) const
{
    return getTransformation(theFrame, this);
}

const RigidTransformation3D* ReferenceFrame3D::getTransformation(const ReferenceFrame3D* startFrame,
                                                            const ReferenceFrame3D* endFrame)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "function not implemented yet"); 
					
	//	Build the descendance list from rootFrame_ to endFrame
	
	//	now start building the ascendance list from the start frame.  When we find an element that is
	//	on the previous list we stop and compute the complete transformation
	
    return NULL;
}

#if 0
#pragma mark -
//------------------------------------------------------
#pragma mark Root and Viewer frame
//------------------------------------------------------
#endif


const ReferenceFrame3D *ReferenceFrame3D::getGlobalReferenceFrame(void)
{
    return rootFrame_;
}

void ReferenceFrame3D::setViewerFrame(const ReferenceFrame3D* viewerFrame)
{
	pthread_mutex_lock(viewerFrameLock);
	viewerFrame_ = viewerFrame;
	pthread_mutex_unlock(viewerFrameLock);
}
