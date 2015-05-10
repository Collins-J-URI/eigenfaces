/*  NAME:
        VirtualObject3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib VirtualObject3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <vector>
#include <cmath>

//
#include "VirtualObject3D.h"

using namespace std;
using namespace uriVL;

VirtualRenderMode VirtualObject3D::renderMode_;


VirtualObject3D::VirtualObject3D( void)
	:	Entity3D(),
		//
		texture_(NULL),
		part_(vector<VirtualPart3D *>()),
        knownCameras_(vector<Camera *>()),
		framePaths_(vector<vector<ReferenceFrame3D*>*>()),
		framesTransfs_(vector<RigidTransformation3D*>())
{    
}

VirtualObject3D::VirtualObject3D(ReferenceFrame3D* theFrame)
	:	Entity3D(),
		//
		texture_(NULL),
		part_(vector<VirtualPart3D *>()),
        knownCameras_(vector<Camera *>()),
		framePaths_(vector<vector<ReferenceFrame3D*>*>()),
		framesTransfs_(vector<RigidTransformation3D*>())
{ 
	 //TODO:  what do we do about theFrame???
}

VirtualObject3D::VirtualObject3D(const VirtualObject3D& theObj)
	:	Entity3D(),
		//
		texture_(NULL),
		part_(vector<VirtualPart3D *>()),
        knownCameras_(vector<Camera *>()),
		framePaths_(vector<vector<ReferenceFrame3D*>*>()),
		framesTransfs_(vector<RigidTransformation3D*>())
{
     FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "VirtualObject3D copy constructor not open for business yet");
}


VirtualObject3D::~VirtualObject3D(void)
{
	vector<VirtualPart3D *>::const_iterator endPart = part_.end();
	for (vector<VirtualPart3D *>::iterator partIter = part_.begin(); 
		 partIter != endPart; partIter++)
		 delete (*partIter);
	part_.clear();
	knownCameras_.clear();
	framesTransfs_.clear();
	
	vector<vector<ReferenceFrame3D*>*>::iterator endPathList = framePaths_.end();
	for (vector<vector<ReferenceFrame3D*>*>::iterator pathIter = framePaths_.begin(); 
		 pathIter != endPathList; pathIter++)
	{
		(*pathIter)->clear();
		delete (*pathIter);
	}
	framePaths_.clear();
	
}

const VirtualObject3D& VirtualObject3D::operator = (const VirtualObject3D& theObj)
{
     FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "VirtualObject3D copy constructor not open for business yet");
}


/*
HomogeneousMatrix *VirtualObject3D::getTransformationMatrix(Camera *cam)
{
	HomogeneousMatrix *theMatrix = NULL;
	vector<Camera *>::iterator  camIter, startCams = knownCameras_.begin(),
	                                    endCams = knownCameras_.end();
	vector<RigidTransformation3D *>::iterator transfIter = framesTransfs_.begin();

	bool noMatch = true;
	for(camIter = startCams; (camIter != endCams) && noMatch; camIter++, transfIter++)
	{
	    //  if we found a match on the reference frame
		if(cam == (*camIter))
		{
		    noMatch = false;

			theMatrix = (*transfIter)->getMatrix();	    
		}
	}
	
	return theMatrix;
}
*/

void VirtualObject3D::render(Camera *cam)
{
	//	if cam is in vector knownCameras [and it will be]
	vector<Camera *>::iterator  camIter, startCams = knownCameras_.begin(),
	                          endCams = knownCameras_.end();
	bool matchFound = false;
	int i=0, camIndex = -1;
	for(camIter = startCams; (camIter != endCams) && !matchFound; camIter++, i++)
	{
	    //  if we found a match on the reference frame
		if(cam == (*camIter))
		{
			matchFound = true;
			camIndex = i;
		}	
	}
	
	//  if we did not find the camera in our "known" vector, then we don't know the visibility
	//	of points/parts relative to the camera, so we don't draw anything
	if (matchFound)
	{
		//	get transformation camera to world
		const GLfloat* Acw = (cam->getCamToWorldTransf(R_W_ACCESS))->getGLMatrix();
		
		glPushMatrix();

		//  Move to the camera's reference frame
		glLoadIdentity();
		//  from there, move to the world reference frame, by applying A
		glMultMatrixf(Acw);
		
		//	 from there, move to the object reference frame by applying world-to-object matrix
//jyh 2010-05-05
//to fix later
//		const GLfloat* Awo = (frame_->getTransformation())->getGLMatrix();
//		glMultMatrixf(Awo);
		
	    //  ask each of the object's parts to render itself
	    vector<VirtualPart3D *>::const_iterator endPart = part_.end();
	    for (vector<VirtualPart3D *>::const_iterator partIter = part_.begin(); 
			 partIter != endPart; partIter++)
	    {
	     //   cout << "render part" << endl; 
	        (*partIter)->render(camIndex);
	    }
	    
		glPopMatrix();
	}
}

const HomogeneousMatrix* VirtualObject3D::getWorldToObjectMatrix(void) const 
{
//jyh 2010-05-05
//to fix later
//	return (getReferenceFrame->getTransformation())->getMatrix();
return NULL;
}


void VirtualObject3D::setCameras(vector<Camera *> knownCameras)
/* 
                                        vector<vector <ReferenceFrame3D *>*> framePaths,
            					        vector<RigidTransformation3D *> framesTransfs)
*/
{
    //  makes a copy of the vector of known reference frames
	vector<Camera *>::iterator camIter, endCams = knownCameras.end();
	for(camIter = knownCameras.begin(); camIter != endCams; camIter++)
		knownCameras_.push_back(*camIter);	

	// set the number of cameras 
	vector<VirtualPart3D *>::const_iterator endPart = part_.end();
	for (vector<VirtualPart3D *>::const_iterator partIter = part_.begin(); 
		 partIter != endPart; partIter++)
        (*partIter)->setNbCameras(knownCameras.size());
    

/*
	//  makes a copy of the reference frame paths
	vector< vector<ReferenceFrame3D*> *>::iterator pathIter, endPaths = framePaths.end();
    for (pathIter = framePaths.begin(); pathIter != endPaths; pathIter++)
    {
        vector<ReferenceFrame3D*>    *subList = (*pathIter),
                                    *tempList = new vector<ReferenceFrame3D*>;
        vector<ReferenceFrame3D*>::iterator  iter, endList = subList->end();
        for (iter = subList->begin(); iter != endList; iter++)
        {
	 		ReferenceFrame3D   *copyList = new ReferenceFrame3D(*iter);
           
        	tempList->push_back(copyList);
        }
        framePaths_.push_back(tempList);
    }		

    //  makes a copy of the vector of transformations between frames
	vector<RigidTransformation3D *>::iterator transfIter, endTransf = framesTransfs.end();
	for(transfIter = framesTransfs.begin(); transfIter != endTransf; transfIter++)
		framesTransfs_.push_back(*transfIter);

*/
}

void VirtualObject3D::addPart(VirtualPart3D *thePart)
{
    part_.push_back(thePart);
}

vector<VirtualPart3D *> *VirtualObject3D::getParts(void)
{
    return &part_;
}

int VirtualObject3D::getCameraIndex(Camera *cam)
{
    int cameraIndex = -1, i=0;
    //  makes a copy of the vector of known reference frames
	vector<Camera *>::const_iterator endCams = knownCameras_.end();
	for(vector<Camera *>::const_iterator camIter = knownCameras_.begin(); camIter != endCams; camIter++, i++)
		if (cam == (*camIter))
		    cameraIndex = i;	

    return cameraIndex;
}

void VirtualObject3D::setDrawReferenceFrame(bool drawFrame)
{
	// set the number of cameras 
	vector<VirtualPart3D *>::const_iterator endPart = part_.end();
	for (vector<VirtualPart3D *>::const_iterator partIter = part_.begin(); 
		 partIter != endPart; partIter++)
        (*partIter)->setDrawReferenceFrame(drawFrame);
}

void VirtualObject3D::setDrawVertices(bool drawVertices)
{
	// set the number of cameras 
	vector<VirtualPart3D *>::const_iterator endPart = part_.end();
	for (vector<VirtualPart3D *>::const_iterator partIter = part_.begin(); 
		 partIter != endPart; partIter++)
        (*partIter)->setDrawVertices(drawVertices);
}

void VirtualObject3D::setRenderMode(VirtualRenderMode mode)
{
    VirtualPart3D::setRenderMode(mode);
    VirtualPoint3D::setRenderMode(mode);
}
