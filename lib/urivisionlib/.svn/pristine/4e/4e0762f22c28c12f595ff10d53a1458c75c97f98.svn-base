/*  NAME:
        Entity3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Entity3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include "Entity3D.h"

using namespace uriVL;

Entity3D::Entity3D(void)
        //  an entity defined without a reference frame explicitly defined
        //  (not sure if this should even be allowed) is automatically attached
        //  to the root "global" reference frame.
        :   frame_(new ReferenceFrame3D(ReferenceFrame3D::getGlobalReferenceFrame( )))
{
    frame_->notifyUseAsReference(this);
}

Entity3D::Entity3D(const ReferenceFrame3D* theFrame)
        :   frame_(theFrame)
{
    frame_->notifyUseAsReference(this);
}

Entity3D::Entity3D(const Entity3D& theFrame)
		:	frame_(NULL)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Entity3D copy constructor is disabled.");					
}

const Entity3D& Entity3D::operator = (const Entity3D& theFrame)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"Entity3D copy operator is disabled.");					
	return *this;
}


Entity3D::~Entity3D(void)
{
    frame_->notifyReleaseReference(this);
}

void Entity3D::setReferenceFrame(const ReferenceFrame3D *ref)
{
    frame_->notifyReleaseReference(this);
    frame_ = ref;
    frame_->notifyUseAsReference(this);
}
 

const ReferenceFrame3D* Entity3D::getReferenceFrame(void) const
{
    return frame_;
}


void Entity3D::addToGroup(const Entity3D* group)
{
}

void Entity3D::addMember(const Entity3D* member)
{
}
