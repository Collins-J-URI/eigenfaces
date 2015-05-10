/*  NAME:
        Box3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Box3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstdlib>
//
#include "Box3D.h"

using namespace uriVL;

//==================================================================================
//	Box3D constructors and destructor
//==================================================================================

/*----------------------------------------------------------------------------------+
|	Constructor:	Box3D(GLfloat* origin, GLfloat** axes, bool isClosed)			|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					axes		coordinates of the major axis, minor axis, and 		|
|									direction axis in a 3x3 0-based array.			|
|					isClosed	should the top and bottom faces be drawn?			|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					modify() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
Box3D::Box3D(GLfloat* origin, GLfloat** axes, bool isClosed)
		:	GraphicComponent3D(),
			//
			vertex_(GlUtils::glfMatrix(0, 7, 0, 2)),
			normal_(isClosed ? GlUtils::glfMatrix(0, 5, 0, 2) : GlUtils::glfMatrix(0, 3, 0, 2)),
			closedBox_(isClosed)

{
	try {
		init_(origin, axes);
	}
	catch (ErrorReport& e) {
		deleteArrays_();
		e.appendToMessage("called from Box3D constructor.");
		throw e;
	}
}

Box3D::Box3D(const Box3D& theObj)
		:	GraphicComponent3D(),
			//
			vertex_(NULL),
			normal_(NULL),
			closedBox_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Box3D copy constructor not implemented.");
}



/*----------------------------------------------------------------------------------+
|	Destructor:		~Box3D(void)													|
|																					|
|	Parameters:		None															|
|																					|
|	Description:	Frees all the arrays that were allocated at the creation.		|																|
+----------------------------------------------------------------------------------*/
Box3D::~Box3D(void)
{
	deleteArrays_();
}

const Box3D& Box3D::operator = (const Box3D& theObj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Box3D copy operator not implemented.");

	return *this;
}



//==================================================================================
//	Box3D public methods
//==================================================================================
#if 0
#pragma mark -
#endif

/*----------------------------------------------------------------------------------+
|	Function:		void setResolution(ResolutionLevel res)							|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		res:		desired resolution									|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Adjusts private variables for proper display of the box			|
|																					|
+----------------------------------------------------------------------------------*/
void Box3D::setResolution(ResolutionLevel res)
{
//	setResolution(res, res);
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
|	Description:	Draws the box													|
+----------------------------------------------------------------------------------*/
void Box3D::draw(void)
{
	int	i;
	
	GraphicComponent3D::draw();
	
	glBegin(GL_QUADS);
		glNormal3fv(normal_[0]);
		glVertex3fv(vertex_[4]);
		glVertex3fv(vertex_[0]);
		glVertex3fv(vertex_[1]);
		glVertex3fv(vertex_[5]);
		//
		glNormal3fv(normal_[1]);
		glVertex3fv(vertex_[5]);
		glVertex3fv(vertex_[1]);
		glVertex3fv(vertex_[2]);
		glVertex3fv(vertex_[6]);
		//
		glNormal3fv(normal_[2]);
		glVertex3fv(vertex_[6]);
		glVertex3fv(vertex_[2]);
		glVertex3fv(vertex_[3]);
		glVertex3fv(vertex_[7]);
		//
		glNormal3fv(normal_[3]);
		glVertex3fv(vertex_[7]);
		glVertex3fv(vertex_[3]);
		glNormal3fv(normal_[0]);
		glVertex3fv(vertex_[0]);
		glVertex3fv(vertex_[4]);
	glEnd();
	
	if (closedBox_)
	{
		glBegin(GL_QUADS);
			glNormal3fv(normal_[4]);
			for (i=4; i<8; i++)
				glVertex3fv(vertex_[i]);
			//
			glNormal3fv(normal_[5]);
			for (i=3; i>=0; i--)
				glVertex3fv(vertex_[i]);
		glEnd();
	}
			
	
}

//==================================================================================
//	Box3D private methods
//==================================================================================
#if 0
#pragma mark -
#endif

/*----------------------------------------------------------------------------------+
|	Function:		void init(GLfloat* origin, GLfloat** a)							|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		origin		coordinates of the origin							|
|					a			coordinates of the major axis, minor axis, and 		|
|									direction axis in a 3x3 0-based array.			|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the array of vertex and normal coordinates			|
+----------------------------------------------------------------------------------*/
void Box3D::init_(GLfloat* origin, GLfloat** a)
{
	int		k;
	
	FAIL_CONDITION( (origin == NULL) || (a == NULL),
					kNullParameterError,
					"null array passed to Box3D::init_");
	
	try {
		commonNormalVector(a[0], a[2], normal_[0]);
		commonNormalVector(a[1], a[2], normal_[1]);
			
		for (k=0; k<3; k++)
		{
			vertex_[0][k] = origin[k];
			vertex_[1][k] = origin[k] + a[0][k];
			vertex_[3][k] = origin[k] + a[1][k];
			vertex_[2][k] = vertex_[1][k] + a[1][k];
			//
			vertex_[4][k] = origin[k] + a[2][k];
			vertex_[5][k] = vertex_[4][k] + a[0][k];
			vertex_[7][k] = vertex_[4][k] + a[1][k];
			vertex_[6][k] = vertex_[5][k] + a[1][k];		
		
			normal_[2][k] = -normal_[0][k];
			normal_[3][k] = -normal_[1][k];
		}

		//	if this is a closed box, then we must also compute the normal
		//	vector for the top and bottom faces.
		if (closedBox_)
		{
			commonNormalVector(a[0], a[1], normal_[4]);
		
			for (k=0; k<3; k++)
				normal_[5][k] = - normal_[4][k];

		}
	}
	catch(...) {
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"invalid array passed to Box3D::init_");
	}
}


void Box3D::deleteArrays_(void)
{
	if (vertex_!=NULL)
	{
		GlUtils::free_glfMatrix(vertex_, 0, 7, 0, 2);
		vertex_ = NULL;
	}
	if (normal_!=NULL)
	{
		if (closedBox_)
			GlUtils::free_glfMatrix(normal_, 0,  5, 0, 2);
		else
			GlUtils::free_glfMatrix(normal_, 0,  3, 0, 2);
			

	}
}



