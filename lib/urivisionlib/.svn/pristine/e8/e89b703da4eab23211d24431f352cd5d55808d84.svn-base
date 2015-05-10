/*  NAME:
        HalfCylinder3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib HalfCylinder3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstdlib>
//
#include "HalfCylinder3D.h"

using namespace uriVL;

//==================================================================================
//	HalfCylinder3D constructors and destructor
//==================================================================================

/*----------------------------------------------------------------------------------+
|	Constructor:	HalfCylinder3D(GLfloat* origin, GLfloat** axes,					|
|						bool lengthIsClosed, bool sidesAreClosed)					|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					axes		coordinates of the major axis, minor axis, and 		|
|									direction axis in a 3x3 0-based array.			|
|					lengthIsClosed	should the length face be drawn?				|
|					sidesAreClosed	should the side faces be drawn?					|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
HalfCylinder3D::HalfCylinder3D(GLfloat* origin, GLfloat** axes, bool lengthIsClosed,
						bool sidesAreClosed)
		:	TubularSection3D(origin, axes, 0.f, PI_F),
			//
			closedLength_(lengthIsClosed),
			closedSides_(sidesAreClosed)

{
	try {
		init_(axes);
	}
	catch (ErrorReport& e) {
//		deleteArrays_();
		e.appendToMessage("called from Cone constructor.");
		throw e;
	}
}

/*----------------------------------------------------------------------------------+
|	Constructor:	HalfCylinder3D(GLfloat* origin, GLfloat* dirAxis,					|
|						GLfloat radius,	bool lengthClosed, bool sidesClosed)		|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					dirAxis		coordinates of the cylinder's axis.					|
|					radius		the cylinder's radius								|
|					lengthIsClosed	should the length face be drawn?				|
|					sidesAreClosed	should the side faces be drawn					|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
HalfCylinder3D::HalfCylinder3D(GLfloat* origin, GLfloat* dirAxis, GLfloat radius,
					bool lengthIsClosed, bool sidesAreClosed)
		:	TubularSection3D(origin, dirAxis, radius, 0.f, PI_F),
			//
			closedLength_(lengthIsClosed),
			closedSides_(sidesAreClosed)
{
	GLfloat		**axes = GlUtils::glfMatrix(0, 2, 0, 2);
	//	
	//	dirAxis is the direction axis of the cylinder
	axes[2][0] = dirAxis[0];
	axes[2][1] = dirAxis[1];
	axes[2][2] = dirAxis[2]; 
	//
	//	For the major axis, we need to build a vector orthogonal to dirAxis.
	if ((dirAxis[0]!= 0.f) || (dirAxis[2]!= 0.f))	//	this is not the local Y axis
	{
		GLfloat		w[3] = {0.f, 1.f, 0.f};
		commonNormalVector(w, dirAxis, axes[0]);		//	"X" = "Y" x "Z"
		commonNormalVector(dirAxis, axes[0], axes[1]); 	//	"Y" = "Z" x "X"
		//
		//	and now we can multiply by the radius
		axes[0][0] *= radius;
		axes[1][0] *= radius;
		//
		axes[0][1] *= radius;
		axes[1][1] *= radius;
		//
		axes[0][2] *= radius;
		axes[1][2] *= radius;
	}
	//	if dirAxis was the local Y axis, then I can take Z and X as major and minor
	else
	{
		axes[0][0] = axes[0][1] = axes[1][1] = axes[1][2] = 0.f;
		axes[0][2] = axes[1][0] = radius;
	}
		
	init_(axes);
	
	GlUtils::free_glfMatrix(axes, 0, 2, 0, 2);
}

HalfCylinder3D::HalfCylinder3D(const HalfCylinder3D& obj)
		: TubularSection3D(NULL, NULL, 0.f, 0.f, 0.f),
			//
			closedLength_(false),
			closedSides_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "HalfCylinder3D copy constructor not implemented.");
}


const HalfCylinder3D& HalfCylinder3D::operator = (const HalfCylinder3D& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "HalfCylinder3D copy operator not implemented.");

	return *this;
}


/*----------------------------------------------------------------------------------+
|	Destructor:		HalfCylinder3D(void)												|
|																					|
|	Parameters:		None															|
|																					|
|	Description:	Frees all the arrays that were allocated at the creation.		|																|
+----------------------------------------------------------------------------------*/
HalfCylinder3D::~HalfCylinder3D(void)
{
/*	int i;
	
	if (vertex_!=NULL)
	{
		for (i=0; i<=kVeryHighDefSlabs; i++)
			free_glfMatrix(vertex_[i], 0,  kVeryHighDefCircPts-1, 0, 2);
		delete [] vertex_;
		vertex_ = NULL;
	}
	if (normal!=NULL)
	{
		free_glfMatrix(normal_, 0,  kVeryHighDefCircPts-1, 0, 2);
		normal_ = NULL;
	}
*/
}


//==================================================================================
//	HalfCylinder3D public methods
//==================================================================================
#if 0
#pragma mark -
#endif


/*----------------------------------------------------------------------------------+
|	Function:		void draw(void)													|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		None															|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Draws the cylinder												|
|																					|
+----------------------------------------------------------------------------------*/
void HalfCylinder3D::draw(void)
{
	TubularSection3D::draw();

/*	if (closedSides_)
	{
		glNormal3fv(normalTop_);
		glBegin(GL_POLYGON);
			for (int j=0; j<=nbCircSegs; j+=circStep_)
				glVertex3fv(vertex_[kVeryHighDefSlabs][j]);
		glEnd();
		//
		glNormal3fv(normalBottom_);
		glBegin(GL_POLYGON);
			for (int j=nbCircSegs; j>=0; j-=circStep_)
				glVertex3fv(vertex_[0][j]);
		glEnd();
	}
	
	if (closedLength_)
	{
		glNormal3fv(normalLength_);
		glBegin(GL_QUADS);
			glVertex3fv(vertex_[0][0]);
			glVertex3fv(vertex_[kVeryHighDefSlabs][0]);
			glVertex3fv(vertex_[kVeryHighDefSlabs][nbCircSegs]);
			glVertex3fv(vertex_[0][nbCircSegs]);
		glEnd();
	}
*/
}


//==================================================================================
//	HalfCylinder3D private methods
//==================================================================================
#if 0
#pragma mark -
#endif


/*----------------------------------------------------------------------------------+
|	Function:		void init(GLfloat** a)											|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		a			coordinates of the major axis, minor axis, and 		|
|									direction axis in a 3x3 0-based array.			|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the array of vertex and normal coordinates			|
|																					|
+----------------------------------------------------------------------------------*/
void HalfCylinder3D::init_(GLfloat** a)
{
	GLfloat	dirStep = 1.f/kVeryHighDefSlabs,
			dir[kVeryHighDefSlabs+1][3],
			majCross[3], minCross[3];
			
	//	majCross = a[0] x a[2] (major axis x direction)
	crossProduct(a[0], a[2], majCross);
	//	minCross = a[1] x a[2] (minor axis x direction)
	crossProduct(a[1], a[2], minCross);

	//	First, we precompute the graduation steps for the slabs along
	//	the "direction" axis (a[2]).
	for (int i=0; i<=kVeryHighDefSlabs; i++)
		for (int k=0; k<3; k++)
			dir[i][k] = i*dirStep*a[2][k];
			
	
	//	if the top and bottom sides are filled, normal vectors must be computed
	//	for these sides
	if (closedSides_)
	{
		//	topNormal = major x minor / ||major x minor||
		commonNormalVector(a[0], a[1], normalTop_);
		normalBottom_[0] = -normalTop_[0];
		normalBottom_[1] = -normalTop_[1];
		normalBottom_[2] = -normalTop_[2];
	}
	//	Same thing for the length side
	if (closedLength_)
		//	normalLength = major x direction / ||major x direction||
		commonNormalVector(a[0], a[2], normalLength_);
}

