/*  NAME:
        VirtualPart3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib VirtualPart3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <vector>
#include <cmath>

#include "VirtualPart3D.h"

using namespace std;
using namespace uriVL;

//----------------------------------------------------------
//  static members
//----------------------------------------------------------

VirtualRenderMode VirtualPart3D::renderMode_ = INTERPOSE;


//----------------------------------
//  Constructors & destructor
//----------------------------------


VirtualPart3D::VirtualPart3D(VirtualObject3D *obj)
        :   Entity3D(),
            //
            vertices_(),
            faces_(),
            normals_(),
            object_(obj),
            //
            drawFrame_(false),
            drawVertices_(true)
{  
	//  setup the reference frames (look at VirtualPoint3D constructor)
}

VirtualPart3D::VirtualPart3D(const VirtualPart3D& obj)
        :   Entity3D(),
            //
            vertices_(),
            faces_(),
            normals_(),
            object_(NULL),
            //
            drawFrame_(false),
            drawVertices_(true)
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "VirtualPart3D copy constructor not implemented");
}
            

const VirtualPart3D& VirtualPart3D::operator = (const VirtualPart3D& obj)
{
    FAIL_CONDITION (true,
                    kFunctionNotImplemented,
                    "VirtualPart3D copy operator not implemented");
    return *this;
}


VirtualPart3D::~VirtualPart3D(void)
{
}

vector<VirtualPoint3D*> *VirtualPart3D::getVertices(void)
{
    return &vertices_;
}

void VirtualPart3D::addVertex(VirtualPoint3D *thePoint)
{
	vertices_.push_back(thePoint);
    thePoint->part_ = this;
    thePoint->setReferenceFrame(getReferenceFrame());
}

void VirtualPart3D::addNormalVector(NormalVector *norm)
{
	normals_.push_back(norm);
}

void VirtualPart3D::addFace(VirtualFace3D *theFace)
{
 /* vector<VirtualPoint3D*>    *tempList = new vector<VirtualPoint3D*>;
    vector<VirtualPoint3D*>::iterator  iter, endList = theFace.end();
    for (iter = theFace.begin(); iter != endList; iter++)
    {
        addVertex(*iter);
    		
        tempList->push_back(*iter);
    	faces_.push_back(tempList); 
    }
*/	
/*
	vector<VirtualFace3D*>::iterator  iter, endList = faces_.end();
    for (iter = faces_.begin(); iter != endList; iter++)
    {
        
        theFace->addNeighbor(*iter);
    }
*/
	faces_.push_back(theFace); 
}

void VirtualPart3D::addFaceVertexInfo(int faceIndex, int vertIndex, int uvIndex, int normalIndex)
{
   	vector<VirtualFace3D*>::iterator face = faces_.begin();
   	face[faceIndex]->addVertex(vertIndex);
   	
   	vector<VirtualPoint3D*>::iterator vertex = vertices_.begin();
   	vertex[vertIndex]->addFaceInfo(faceIndex, uvIndex, normalIndex);
   	
}


void VirtualPart3D::render(int camIndex)
{
    //  ask each of the part's points to render themselves
	vector<VirtualFace3D*>::iterator	faceIter = faces_.begin();
	vector<VirtualPoint3D*>::iterator	vertIter = vertices_.begin();
	vector<NormalVector*>::iterator	normIter = normals_.begin();
	int nFaces = faces_.size();
	if (renderMode_ == INTERPOSE)
	{
		//  for each face in the part
		for (int i=0; i<nFaces; i++)
		{
		    VirtualFace3D *face = faceIter[i];
		    
			glBegin(GL_POLYGON);
	            vector<int>::iterator faceVertIter = (face->vertexIndex_).begin();
	            int  nbFaceVerts = face->vertexIndex_.size();
	            for (int j=0; j<nbFaceVerts; j++) 
	            {
	                int vertIndex = faceVertIter[j];
	                VirtualPoint3D *vert = vertIter[vertIndex];
	                vector<int>::iterator faceVertNormIter = vert->normalIndex_.begin();
	                NormalVector *norm = normIter[faceVertNormIter[i]];
	                
	                glNormal3d(norm->nx, norm->ny, norm->nz);
	                
			    	if (vert->getVisibility(camIndex)->code == VISIBLE)
			    		glColor4f(.0f, .8f, 0.f, 1.f);
			    	else if (vert->getVisibility(camIndex)->code == INVISIBLE)
			    		glColor4f(0.f, 0.f, 1.f, 0.f);
			    	else if (vert->getVisibility(camIndex)->code == CONTACT)
			    		glColor4f(1.f, 0.f, 0.f, 0.f);
			    	else //  if (vert->getVisibility(camIndex)->code == UNKNOWN)
			    		glColor4f(0.f, 1.f, 1.f, 0.f);
			   		
		    		glVertex3dv(vert->getXYZ());
	            }
			glEnd();
		}
		    
		if (drawFrame_)
			drawReferenceFrame_();
			
		if (drawVertices_)
		{   
            //  virtual points are shaded
    		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	    	glDepthMask (GL_TRUE); 
	    	glDisable (GL_BLEND);
	        vector<VirtualPoint3D *>::iterator ptIter, endVertices = vertices_.end();
	        for (ptIter = vertices_.begin(); ptIter != endVertices; ptIter++)
	            (*ptIter)->render_(camIndex);

	    }
	}

    //  Rendering mode = GEOMETRY_TEST
	else
	{
	    //  face edges in wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//  for each face in the part
		for (int i=0; i<nFaces; i++)
		{
		    VirtualFace3D *face = faceIter[i];
		    
    		glColor4f(0.f, .8f, 0.f, 1.f);
			glBegin(GL_POLYGON);
	            vector<int>::iterator faceVertIter = (face->vertexIndex_).begin();
	            int  nbFaceVerts = face->vertexIndex_.size();
	            for (int j=0; j<nbFaceVerts; j++) 
	            {
	                int vertIndex = faceVertIter[j];
	                VirtualPoint3D *vert = vertIter[vertIndex];
	                
		    		glVertex3dv(vert->getXYZ());
	            }
			glEnd();
		}
		    
		if (drawFrame_)
			drawReferenceFrame_();
			
        //  virtual points are shaded
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    	glDepthMask (GL_TRUE); 
    	glDisable (GL_BLEND);
        vector<VirtualPoint3D *>::iterator ptIter, endVertices = vertices_.end();
        for (ptIter = vertices_.begin(); ptIter != endVertices; ptIter++)
            (*ptIter)->render_(camIndex);

	}	
}
void VirtualPart3D::setNbCameras(int n)
{
    vector<VirtualPoint3D *>::iterator ptIter, endVertices = vertices_.end();
		
    for (ptIter = vertices_.begin(); ptIter != endVertices; ptIter++)
        (*ptIter)->setNbCameras_(n);

}

void VirtualPart3D::setDrawReferenceFrame(bool drawFrame)
{
	drawFrame_ = drawFrame;
}

void VirtualPart3D::setDrawVertices(bool drawVertices)
{
    drawVertices_ = drawVertices;
}

//	Draws the three axes of the local reference frame 
void VirtualPart3D::drawReferenceFrame_(void)
{
	glBegin(GL_LINES);
		//	Z --> red.
		glColor3f(1.f, 0.f, 0.f);
//		setMaterial_(1.0, 0., 0., 0., 0., 0., 0., 0., 0., 0.);
		glVertex3f(0.f, 0.f, -0.1f);
		glVertex3f(0.f, 0.f, 0.5f);
		//	X --> Green
		glColor3f(0.f, 1.f, 0.f);
//		setMaterial_(0., 1.0, 0., 0., 0., 0., 0., 0., 0., 0.);
		glVertex3f(-0.1f, 0.f, 0.f);
		glVertex3f(0.5f, 0.f, 0.f);
		//	Y --> Blue		
		glColor3f(0.f, 0.f, 1.f);
//		setMaterial_(0., 0., 1.0, 0., 0., 0., 0., 0., 0., 0.);
		glVertex3f(0.f, -0.1f, 0.f);
		glVertex3f(0.f, 0.5f, 0.f);
	glEnd();
}


//	 Allows to define the reflectance properties of the current object's material 
void VirtualPart3D::setMaterial_(GLfloat ambRed, GLfloat ambGreen, GLfloat ambBlue, GLfloat difRed,
	GLfloat difGreen, GLfloat difBlue, GLfloat specRed, GLfloat specGreen,
	GLfloat specBlue, GLfloat shine)
{
  float mat[4];

	mat[0] = ambRed;
	mat[1] = ambGreen;
	mat[2] = ambBlue;
	mat[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	mat[0] = difRed;
	mat[1] = difGreen;
	mat[2] = difBlue;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
	mat[0] = specRed;
	mat[1] = specGreen;
	mat[2] = specBlue;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	glMaterialf(GL_FRONT, GL_SHININESS, shine * 128.0f);
}


void VirtualPart3D::setRenderMode(VirtualRenderMode mode)
{
	renderMode_ = mode;	
}
