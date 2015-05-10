/*  NAME:
        VirtualPoint3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib VirtualPoint3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <cmath>
#include <vector>
//
#include "VirtualPoint3D.h"
#include "VirtualPart3D.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  static members
//----------------------------------------------------------

const GLfloat VirtualPoint3D::defaultColor_[4] = {0.f, 1.f, 0.f, 1.f};
VirtualRenderMode VirtualPoint3D::renderMode_ = INTERPOSE;
VirtualPoint3D *VirtualPoint3D::currentSelected_ = NULL;
const GLfloat VirtualPoint3D::defaultRenderScale_ = 0.005f;

//----------------------------------------------------------
//  Constructors and destructor
//----------------------------------------------------------


VirtualPoint3D::VirtualPoint3D(double X, double Y, double Z, VirtualPart3D *thePart)
		try	:	Entity3D(thePart->getReferenceFrame()),
				//
				point_(new Point3D(X, Y, Z, thePart->getReferenceFrame())),
				part_(NULL),
				visibility_(NULL),
				normalIndex_(vector<int>(0)),
				uvIndex_(vector<int>(0)),
				neighbors_(vector<VirtualPoint3D*>(0)),
				color_(NULL),
				isSelected_(false),
				isSelectedNeighbor_(false),
				renderScale_(1.f),
				hasIndividualRenderScale_(false)				
{
    thePart->addVertex(this);
}
catch (...)
{
    FAIL_CONDITION( thePart==NULL,
                    kNullEntity3D,
                    "null VirtualPart3D passed to VirtualPoint3D constructor"); 
}

VirtualPoint3D::VirtualPoint3D(const double theXYZ[], VirtualPart3D *thePart)
		try	:	Entity3D(thePart->getReferenceFrame()),
				//
				point_(new Point3D(theXYZ, thePart->getReferenceFrame())),
				part_(NULL),
				visibility_(NULL),
				normalIndex_(vector<int>(0)),
				uvIndex_(vector<int>(0)),
				neighbors_(vector<VirtualPoint3D*>(0)),
				color_(NULL),
				isSelected_(false),
				isSelectedNeighbor_(false),
				renderScale_(1.f),
				hasIndividualRenderScale_(false)				
{
    thePart->addVertex(this);
}
catch (...)
{
    FAIL_CONDITION( thePart==NULL,
                    kNullEntity3D,
                    "null VirtualPart3D passed to VirtualPoint3D constructor"); 
}

VirtualPoint3D::VirtualPoint3D(const Point3D *thePt)
		try	:	Entity3D(thePt->getReferenceFrame()),
				//
				point_(new Point3D(thePt)),
				part_(NULL),
				visibility_(NULL),
				normalIndex_(vector<int>(0)),
				uvIndex_(vector<int>(0)),
				neighbors_(vector<VirtualPoint3D*>(0)),
				color_(NULL),
				isSelected_(false),
				isSelectedNeighbor_(false),
				renderScale_(1.f),
				hasIndividualRenderScale_(false)				
{
}
catch (...)
{
    FAIL_CONDITION( thePt==NULL,
                    kNullEntity3D,
                    "null Point3D passed to VirtualPoint3D constructor"); 
}


VirtualPoint3D::VirtualPoint3D(const VirtualPoint3D& obj)
			:	Entity3D(),
				//
				point_(NULL),
				part_(NULL),
				visibility_(NULL),
				normalIndex_(),
				uvIndex_(),
				neighbors_(),
				color_(NULL),
				isSelected_(false),
				isSelectedNeighbor_(false),
				renderScale_(1.f),
				hasIndividualRenderScale_(false)				
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "VirtualPoint3D copy constructor not implemented");
}
            

const VirtualPoint3D& VirtualPoint3D::operator = (const VirtualPoint3D& obj)
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "VirtualPoint3D copy operator not implemented");
    return *this;
}

VirtualPoint3D::~VirtualPoint3D(void)
{
	delete point_;
}

double VirtualPoint3D::getX(void) const
{
    return point_->getX();
}

double VirtualPoint3D::getY(void) const
{
    return point_->getY();
}

double VirtualPoint3D::getZ(void) const
{
    return point_->getZ();
}

const double* VirtualPoint3D::getXYZ(void) const
{
    return point_->getXYZ();
}

void VirtualPoint3D::setPosition(const double XYZ[])
{
    point_->setPosition(XYZ);
}

void VirtualPoint3D::setPosition(double X, double Y, double Z)
{
    point_->setPosition(X, Y, Z);
}

double VirtualPoint3D::distanceTo(const VirtualPoint3D *p) const
{
    FAIL_CONDITION( p==NULL,
                    kNullEntity3D,
                    "Attempt to calculate distance to a NULL Point3D"); 

    return point_->distanceTo(p->point_);
}

void VirtualPoint3D::setColor(float theRed, float theGreen, float theBlue)
{
    if (color_ == NULL)
        color_ = new GLfloat[4];
    
    color_[0] = theRed;
    color_[1] = theGreen;
    color_[2] = theBlue;
    color_[3] = 1.f;
}
            
void VirtualPoint3D::setColor(const float rgb[])
{
    if (rgb!=NULL)
        setColor(rgb[0], rgb[1], rgb[2]);
}       

void VirtualPoint3D::addNeighbor(VirtualPoint3D *pt)
{
	neighbors_.push_back(pt);	
}


void VirtualPoint3D::addFaceInfo(int faceIndex, int uvIndex, int normalIndex)
{
    //  if the face index vector is too small, resize it to be able to add an element
    //  at the new faceIndex
    if (faceIndex >= static_cast<int>(normalIndex_.size())) {
        for (int i=normalIndex_.size(); i<=faceIndex; i++)
        {
            normalIndex_.push_back(-1);
            uvIndex_.push_back(-1);
        }
    }
    
    vector<int>::iterator normal = normalIndex_.begin();
    vector<int>::iterator uv = uvIndex_.begin();
    normal[faceIndex] = normalIndex;
    uv[faceIndex] = uvIndex;
}

void VirtualPoint3D::setSelected(bool selected)
{
	if (selected == true){
		
		//  if a point was selected before, deselect it
		if (currentSelected_ != NULL)
		    currentSelected_->setSelected(false);
		
		//  this point is selected
		isSelected_ = true;
		currentSelected_ = this;
		
		//  all its neighbors are selected neighbors
		vector <VirtualPoint3D*>:: iterator nb, endNeighbor = neighbors_.end();
		for (nb = neighbors_.begin(); nb != endNeighbor; nb++)
		    (*nb)->setSelectedNeighbor(true);
		    
	}
	else{
	    //  deselect this point
		isSelected_ = false;
		
		//  deselect all neighbors as selected neighbors					
		//  all its neighbors are selected neighbors
		vector <VirtualPoint3D*>:: iterator nb, endNeighbor = neighbors_.end();
		for (nb = neighbors_.begin(); nb != endNeighbor; nb++)
		    (*nb)->setSelectedNeighbor(false);
	}
		
}
          	
void VirtualPoint3D::setSelectedNeighbor(bool selectedNeighbor)
{
	isSelectedNeighbor_ = selectedNeighbor;
} 
         	
bool VirtualPoint3D::isSelected(void) const
{
	return isSelected_;
}
          	
void VirtualPoint3D::setRenderMode(VirtualRenderMode mode)
{
	renderMode_ = mode;	
}

void VirtualPoint3D::setNbCameras_(int n)
{
	if (visibility_ != NULL)
		delete visibility_;
	visibility_ = new VisibilityStatus *[n];
	for (int i=0; i<n; i++)
		visibility_[i] = new VisibilityStatus();
}
            
const VisibilityStatus* VirtualPoint3D::getVisibility(int camIndex) const
{
    return visibility_[camIndex];
}

void VirtualPoint3D::setRenderScale(GLfloat renderScale)
{
    hasIndividualRenderScale_ = true;
    renderScale_ = renderScale;
}

void VirtualPoint3D::setVisibility(int camIndex, const VisibilityStatus* vis)
{
	visibility_[camIndex]->code = vis->code;
	visibility_[camIndex]->confidence = vis->confidence;
	
}

void VirtualPoint3D:: render_(int camIndex) const
{
	//  drawing a small square
	glPushMatrix();

	//  Move from the reference frame to this point
	glTranslatef(static_cast<float>(point_->getX()), 
				 static_cast<float>(point_->getY()), 
				 static_cast<float>(point_->getZ()));
	

	if (renderMode_ == INTERPOSE)
		if (color_ != NULL)        
			glColor4fv(color_);
		else
			glColor4fv(defaultColor_);
	else if (isSelected_)
			glColor4f(1.f, 0.f, 0.f, 1.f);
	else if (isSelectedNeighbor_)
		glColor4f(1.f, 1.f, 0.f, 1.f);
	else
		glColor4fv(defaultColor_);

	if (hasIndividualRenderScale_)
		glScalef(renderScale_, renderScale_, renderScale_);
	else
		glScalef(defaultRenderScale_, defaultRenderScale_, defaultRenderScale_);
	
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-1.f, -1.f, 1.f);
		glVertex3f(-1.f, -1.f, -1.f);
		glVertex3f(1.f, -1.f, 1.f);
		glVertex3f(1.f, -1.f, -1.f);
		glVertex3f(1.f, 1.f, 1.f);
		glVertex3f(1.f, 1.f, -1.f);
		glVertex3f(-1.f, 1.f, 1.f);
		glVertex3f(-1.f, 1.f, -1.f);
		glVertex3f(-1.f, -1.f, 1.f);
		glVertex3f(-1.f, -1.f, -1.f);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3f(-1.f, -1.f, 1.f);
		glVertex3f(1.f, -1.f, 1.f);
		glVertex3f(1.f, 1.f, 1.f);
		glVertex3f(-1.f, 1.f, 1.f);

		glVertex3f(-1.f, -1.f, -1.f);
		glVertex3f(-1.f, 1.f, -1.f);
		glVertex3f(1.f, 1.f, -1.f);
		glVertex3f(1.f, -1.f, -1.f);
	glEnd();
	
	glPopMatrix();	
}

