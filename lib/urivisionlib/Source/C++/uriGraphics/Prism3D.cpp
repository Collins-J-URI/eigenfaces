/*  NAME:
        Prism3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Prism3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstdlib>

#include "Prism3D.h"

using namespace uriVL;

#ifndef 	MOD
#define		MOD(a, b)	((a) >= 0 ? (a)%(b) : (a)%(b) + (b))
#endif

//==================================================================================
//	Prism3D constructors and destructor
//==================================================================================

/*----------------------------------------------------------------------------------+
|	Constructor:	Prism3D(int n, GLfloat** topFace, GLfloat** bottomFace,			|
|					bool isClosed, bool isSmooth)									|
|																					|
|	Parameters:		n			number of vertices on the top and bottom faces		|
|					topFace		vertices of the top face (listed counterclockwise)	|
|					bottomFace	vertices of the bottom face (listed clockwise)		|
|					isClosed	should the top and bottom faces be drawn?			|
|					isSmooth	are the sides smooth or polygonal?					|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init_() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
Prism3D::Prism3D(int n, GLfloat** topFace, GLfloat** bottomFace, bool isClosed,
						bool isSmooth)
		:	GraphicComponent3D(),
    		vertex_(new GLfloat** [kVeryHighDefSlabs+1]),
			normalF_(NULL),
			normalV_(NULL),
			nbPtsPerSlice_(n),
			slabStep_(kCoarseResolution),
			closedPrism_(isClosed),
			smoothPrism_(isSmooth)
{
	init_(topFace, bottomFace);
}


/*----------------------------------------------------------------------------------+
|	Constructor:	Prism3D(int n, GLfloat** bottomFace, GLfloat* axis,				|
|					bool isClosed, bool isSmooth)									|
|																					|
|	Parameters:		n			number of vertices on the top and bottom faces		|
|					bottomFace	vertices of the bottom face (listed clockwise)		|
|					axis		coordinates of the regular prism's axis				|
|					isClosed	should the top and bottom faces be drawn?			|
|					isSmooth	are the sides smooth or polygonal?					|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
Prism3D::Prism3D(int n, GLfloat** bottomFace, GLfloat* axis, bool isClosed,
					bool isSmooth)
		:	GraphicComponent3D(),
    		vertex_(new GLfloat** [kVeryHighDefSlabs+1]),
			normalF_(),
			normalV_(),
			nbPtsPerSlice_(n),
			slabStep_(),
			closedPrism_(isClosed),
			smoothPrism_(isSmooth)
{
	//	Allocate the (local) array of top vertices
	GLfloat** topFace = GlUtils::glfMatrix(0, n-1, 0, 2);
	for (int i=0; i<n; i++)
		for (int k=0; k<3; k++)
			topFace[i][k] = bottomFace[i][k] + axis[k];
			
	//	---> NOW we are exactly in the same situation as with the other constructor
	
	//	settle for a low slab resolution initially
	setResolution(kCoarseResolution);
	init_(topFace, bottomFace);
	
	GlUtils::free_glfMatrix(topFace, 0, n-1, 0, 2);
}


Prism3D::Prism3D(const Prism3D& obj)
		:	GraphicComponent3D(),
    		vertex_(NULL),
			normalF_(NULL),
			normalV_(NULL),
			nbPtsPerSlice_(0),
			slabStep_(kCoarseResolution),
			closedPrism_(false),
			smoothPrism_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Prism3D copy constructor not implemented.");
}


const Prism3D& Prism3D::operator = (const Prism3D& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Prism3D copy operator not implemented.");

	return *this;
}

/*----------------------------------------------------------------------------------+
|	Destructor:		~Prism(void)													|
|																					|
|	Parameters:		None															|
|																					|
|	Description:	Frees all the arrays that were allocated at the creation.		|																|
+----------------------------------------------------------------------------------*/
Prism3D::~Prism3D(void)
{
	deleteArrays_();
}


//==================================================================================
//	Prism3D public methods
//==================================================================================
#if 0
#pragma mark -
#endif

/*----------------------------------------------------------------------------------+
|	Function:		void setResolution(ResolutionLevel theRes)						|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		theRes:		desired slab resolution level						|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Adjusts private variables for proper display of the prism		|
|																					|
+----------------------------------------------------------------------------------*/
void Prism3D::setResolution(ResolutionLevel theRes)
{
	switch (theRes)
	{
		case kVeryHighResolution:
			slabStep_ = 1;
			break;
			
		case kFineResolution:
			slabStep_ = kFineDefSlabScale;
			break;
			
		case kMediumResolution:
			slabStep_ = kMediumDefSlabScale;
			break;
			
		case kCoarseResolution:
			slabStep_ = kCoarseDefSlabScale;
			break;
			
		case kVeryLowResolution:
			slabStep_ = kVeryLowDefSlabScale;
			break;
			
		default:
			break;
	}
}



/*----------------------------------------------------------------------------------+
|	Function:		void draw(void)													|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		None															|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Draws the prism													|
|																					|
+----------------------------------------------------------------------------------*/
void Prism3D::draw(void)
{
	GraphicComponent3D::draw();
	
	if (smoothPrism_)
	{
		for (int i=0; i<=kVeryHighDefSlabs-slabStep_; i+=slabStep_)
		{
			glBegin(GL_TRIANGLE_STRIP);
				for (int j=0; j<nbPtsPerSlice_; j++)
				{
					glNormal3fv(normalV_[i+slabStep_][j]);
					glVertex3fv(vertex_[i+slabStep_][j]);
					glNormal3fv(normalV_[i][j]);
					glVertex3fv(vertex_[i][j]);
				}
				glNormal3fv(normalV_[i+slabStep_][0]);
				glVertex3fv(vertex_[i+slabStep_][0]);
				glNormal3fv(normalV_[i][0]);
				glVertex3fv(vertex_[i][0]);
				
			glEnd();
		}
	}
	else
	{
		for (int j=0; j<nbPtsPerSlice_-1; j++)
		{
			glNormal3fv(normalF_[j]);
				
			glBegin(GL_TRIANGLE_STRIP);
				for (int i=0; i<=kVeryHighDefSlabs; i+=slabStep_)
				{
					glVertex3fv(vertex_[i][j]);
					glVertex3fv(vertex_[i][j+1]);
				}
			glEnd();
		}
		glBegin(GL_TRIANGLE_STRIP);
			glNormal3fv(normalF_[nbPtsPerSlice_-1]);
			for (int i=0; i<=kVeryHighDefSlabs; i+=slabStep_)
			{
				glVertex3fv(vertex_[i][nbPtsPerSlice_-1]);
				glVertex3fv(vertex_[i][0]);
			}
		glEnd();
	}
	
	
	if (closedPrism_)
	{
		glNormal3fv(normalTop_);
		glBegin(GL_POLYGON);
			for (int j=0; j<nbPtsPerSlice_; j++)
				glVertex3fv(vertex_[kVeryHighDefSlabs][j]);
		glEnd();
		//
		glNormal3fv(normalBottom_);
		glBegin(GL_POLYGON);
			for (int j=nbPtsPerSlice_-1; j>=0; j--)
				glVertex3fv(vertex_[0][j]);
		glEnd();
	}
}

//==================================================================================
//	Prism3D private methods
//==================================================================================
#if 0
#pragma mark -
#endif


/*----------------------------------------------------------------------------------+
|	Function:		void init(GLfloat** topFace, GLfloat** bottomFace)				|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		topFace		vertices of the top face (listed counterclockwise)	|
|					bottomFace	vertices of the bottom face (listed clockwise)		|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the array of vertex and normal coordinates			|
|																					|
+----------------------------------------------------------------------------------*/
void Prism3D::init_(GLfloat** topFace, GLfloat** bottomFace)
{
	//	Allocate the arrays of vertices and normal vectors
	for (int i=0; i<=kVeryHighDefSlabs; i++)
		vertex_[i] = GlUtils::glfMatrix(0, nbPtsPerSlice_-1, 0, 2);
	

	//	if we have a smooth prism, then there is one normal vector per vertex
	if (smoothPrism_)
	{
		normalV_ = new GLfloat** [kVeryHighDefSlabs+1];
		FAIL_CONDITION( normalV_==NULL,
		                kOpenGLNumAllocError, 
		                "Error while allocating array of GL numbers");
		for (int i=0; i<=kVeryHighDefSlabs; i++)
			normalV_[i] = GlUtils::glfMatrix(0, nbPtsPerSlice_-1, 0, 2);
	}
	//	if the prism is not smooth, then there is one normal vector per face.
	else
		normalF_ = GlUtils::glfMatrix(0, nbPtsPerSlice_-1, 0, 2);

	GLfloat	dirStep = 1.f/kVeryHighDefSlabs,
			d[3];
	

	for (int j=0; j< nbPtsPerSlice_; j++)
	{
		d[0] = dirStep*(topFace[j][0] - bottomFace[j][0]);
		d[1] = dirStep*(topFace[j][1] - bottomFace[j][1]);
		d[2] = dirStep*(topFace[j][2] - bottomFace[j][2]);
		
		vertex_[0][j][0] = bottomFace[j][0]; 
		vertex_[0][j][1] = bottomFace[j][1]; 
		vertex_[0][j][2] = bottomFace[j][2]; 
		
		for (int i=1; i<=kVeryHighDefSlabs; i++)
		{
			vertex_[i][j][0] = vertex_[i-1][j][0] + d[0];
			vertex_[i][j][1] = vertex_[i-1][j][1] + d[1];
			vertex_[i][j][2] = vertex_[i-1][j][2] + d[2];
		}
	}
	
	if (smoothPrism_)
	{
		for (int j=0; j< nbPtsPerSlice_; j++)
			for (int i=0; i<=kVeryHighDefSlabs; i++)
				vertexAverageNormal(vertex_[i][j], vertex_[i][MOD(j-1, nbPtsPerSlice_)],
									vertex_[i][MOD(j+1, nbPtsPerSlice_)], d, normalV_[i][j]);
	}
	else
	{
		for (int j=0; j< nbPtsPerSlice_; j++)
			faceAverageNormal(vertex_[0][j], vertex_[0][MOD(j+1, nbPtsPerSlice_)],
								vertex_[kVeryHighDefSlabs][MOD(j+1, nbPtsPerSlice_)],
								vertex_[kVeryHighDefSlabs][j], normalF_[j]);
	}
	
	//	if this is a closed cylinder, then we must also compute the normal
	//	vector for the top and bottom faces.
	if (closedPrism_)
	{
		faceAverageNormal(nbPtsPerSlice_, vertex_[0], normalBottom_);
		//	don't forget that the bottom points are listed clockwise.
		normalBottom_[0] = -normalBottom_[0];
		normalBottom_[1] = -normalBottom_[1];
		normalBottom_[2] = -normalBottom_[2];
		
		faceAverageNormal(nbPtsPerSlice_, vertex_[kVeryHighDefSlabs], normalTop_);
	}
}

void Prism3D::deleteArrays_(void)
{
	if (vertex_!=NULL)
	{
		for (int i=0; i<=kVeryHighDefSlabs; i++)
			GlUtils::free_glfMatrix(vertex_[i], 0,  nbPtsPerSlice_-1, 0, 2);
		delete [] vertex_;
		vertex_ = NULL;
	}
	if (smoothPrism_)
	{
		if (normalV_!=NULL)
		{
			for (int i=0; i<=kVeryHighDefSlabs; i++)
				GlUtils::free_glfMatrix(normalV_[i], 0,  nbPtsPerSlice_-1, 0, 2);
			delete [] normalV_;
			normalV_ = NULL;
		}
	}
	else
	{
		if (normalF_!=NULL)
		{
			GlUtils::free_glfMatrix(normalF_, 0,  nbPtsPerSlice_-1, 0, 2);
			normalF_ = NULL;
		}
	}
}

