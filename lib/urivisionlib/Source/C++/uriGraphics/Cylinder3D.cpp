/*  NAME:
        Cylinder3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib Cylinder3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstdlib>
//
#include "Cylinder3D.h"

using namespace uriVL;


//	Redefinition of the class' static elements
//
bool	Cylinder3D::initCtSt_ = false;
GLfloat Cylinder3D::ct_[kVeryHighDefCircPts],
		Cylinder3D::st_[kVeryHighDefCircPts];



//==================================================================================
//	Cylinder3D constructors and destructor
//==================================================================================

/*----------------------------------------------------------------------------------+
|	Constructor:	Cylinder3D(GLfloat* origin, GLfloat** axes, bool isClosed)		|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					axes		coordinates of the major axis, minor axis, and 		|
|									direction axis in a 3x3 0-based array.			|
|					isClosed	should the top and bottom faces be drawn?			|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
Cylinder3D::Cylinder3D(GLfloat* origin, GLfloat** axes, bool isClosed)
		:	GraphicComponent3D(),
			//
			vertex_(new GLfloat** [kVeryHighDefSlabs+1]),
			normal_(GlUtils::glfMatrix(0, kVeryHighDefCircPts-1, 0, 2)),
			circStep_(kFineResolution),
			slabStep_(kMediumResolution),
			closedCyl_(isClosed)
{
	if (!initCtSt_)
		initStatic_();
		
	try {
		init_(origin, axes);
	}
	catch (ErrorReport& e) {
		deleteArrays_();
		e.appendToMessage("called from Cylinder3D constructor.");
		throw e;
	}
}

/*----------------------------------------------------------------------------------+
|	Constructor:	Cylinder3D(GLfloat* origin, GLfloat* dirAxis, GLfloat radius,	|
|									bool isClosed)									|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					dirAxis		coordinates of the cylinder's axis.					|
|					radius		the cylinder's radius								|
|					isClosed	should the top and bottom faces be drawn?			|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
Cylinder3D::Cylinder3D(GLfloat* origin, GLfloat* dirAxis, GLfloat radius,	bool isClosed)
		:	GraphicComponent3D(),
			//
			vertex_(new GLfloat** [kVeryHighDefSlabs+1]),
			normal_(GlUtils::glfMatrix(0, kVeryHighDefCircPts-1, 0, 2)),
			circStep_(kFineResolution),
			slabStep_(kMediumResolution),
			closedCyl_(isClosed)
{
	if (!initCtSt_)
		initStatic_();

	GLfloat		**axes, w[3];
	
	axes = GlUtils::glfMatrix(0, 2, 0, 2);

	try {
		//	dirAxis is the direction axis of the cylinder
		axes[2][0] = dirAxis[0];
		axes[2][1] = dirAxis[1];
		axes[2][2] = dirAxis[2]; 

		//	For the major axis, we need to build a vector orthogonal to dirAxis.
		if ((dirAxis[0]!= 0.f) || (dirAxis[2]!= 0.f))	//	this is not the local Y axis
		{
			w[0] = 0.f; w[1] = 1.f; w[2] = 0.f;
			commonNormalVector(w, dirAxis, axes[0]);		//	"X" = "Y" x "Z"
			commonNormalVector(dirAxis, axes[0], axes[1]); 	//	"Y" = "Z" x "X"
			//
			//	and now we can multiply by the radius
			for (int i=0; i<3; i++)
			{
				axes[0][i] *= radius;
				axes[1][i] *= radius;
			}
		}
		//	if dirAxis was the local Y axis, then I can take Z and X as major and minor
		else
		{
			axes[0][0] = axes[0][1] = axes[1][1] = axes[1][2] = 0.f;
			axes[0][2] = axes[1][0] = radius;
		}
	}
	catch (...)
	{
		deleteArrays_();
		GlUtils::free_glfMatrix(axes, 0, 2, 0, 2);
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid array passed to Cylinder3D constructor");
	}
	
	for (int i=0; i<=kVeryHighDefSlabs; i++)
		vertex_[i] = GlUtils::glfMatrix(0, kVeryHighDefCircPts-1, 0, 2);
	
	try {
		init_(origin, axes);
	}
	catch (...)
	{
		deleteArrays_();
		GlUtils::free_glfMatrix(axes, 0, 2, 0, 2);
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid array passed to Cylinder3D constructor");
	}
	
	GlUtils::free_glfMatrix(axes, 0, 2, 0, 2);
}


Cylinder3D::Cylinder3D(const Cylinder3D& obj)
		:	GraphicComponent3D(),
			//
			vertex_(NULL),
			normal_(NULL),
			circStep_(kCoarseResolution),
			slabStep_(kCoarseResolution),
			closedCyl_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Cylinder3D copy constructor not implemented.");
}



/*----------------------------------------------------------------------------------+
|	Destructor:		Cylinder3D(void)												|
|																					|
|	Parameters:		None															|
|																					|
|	Description:	Frees all the arrays that were allocated at the creation.		|																|
+----------------------------------------------------------------------------------*/
Cylinder3D::~Cylinder3D(void)
{
	deleteArrays_();
}

const Cylinder3D& Cylinder3D::operator = (const Cylinder3D& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "Cylinder3D copy operator not implemented.");

	return *this;
}


//==================================================================================
//	Cylinder3D public methods
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
|	Description:	Adjusts private variables for proper display of the cylinder	|
|																					|
+----------------------------------------------------------------------------------*/
void Cylinder3D::setResolution(ResolutionLevel res)
{
	setResolution(res, res);
}



/*----------------------------------------------------------------------------------+
|	Function:		void setResolution(ResolutionLevel circRes,						|
|										ResolutionLevel slabRes)					|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		circRes:	desired nb of points on a circle					|
|					slabRes:	desired slab resolution level						|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Adjusts private variables for proper display of the cylinder	|
|																					|
+----------------------------------------------------------------------------------*/
void Cylinder3D::setResolution(ResolutionLevel circRes, ResolutionLevel slabRes)
{
	switch (circRes)
	{
		case kVeryHighResolution:
			circStep_ = 1;
			break;
			
		case kFineResolution:
			circStep_ = kFineDefCircScale;
			break;
			
		case kMediumResolution:
			circStep_ = kMediumDefCircScale;
			break;
			
		case kCoarseResolution:
			circStep_ = kCoarseDefCircScale;
			break;
			
		case kVeryLowResolution:
			circStep_ = kVeryLowDefCircScale;
			break;
			
		default:
			circStep_ = kMediumDefCircScale;
			break;
	}
	
	switch (slabRes)
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
			slabStep_ = kMediumDefSlabScale;
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
|	Description:	Draws the cylinder												|
|																					|
+----------------------------------------------------------------------------------*/
void Cylinder3D::draw(void)
{
	GraphicComponent3D::draw();
	
	//	I draw the cylinders as a series of one-slab-high crowns
	//	Each crown is defined as a quad strip.
	//	It might be a better idea to draw then the other way around (as a ring
	//	of linear quad strips).  --> check 
	for (int i=0; i<=kVeryHighDefSlabs-slabStep_; i+=slabStep_)
	{
		glBegin(GL_TRIANGLE_STRIP);
			for (int j=0; j<kVeryHighDefCircPts; j+=circStep_)
			{
				glNormal3fv(normal_[j]);
				glVertex3fv(vertex_[i+slabStep_][j]);
				glNormal3fv(normal_[j]);
				glVertex3fv(vertex_[i][j]);
			}
			glNormal3fv(normal_[0]);
			glVertex3fv(vertex_[i+slabStep_][0]);
			glNormal3fv(normal_[0]);
			glVertex3fv(vertex_[i][0]);
			
		glEnd();
	}
	
	//	If this is a closed cylinder, then we also draw the top and bottom
	//	sides at the same resolution as the rest)
	if (closedCyl_)
	{
		glNormal3fv(normalTop_);
		glBegin(GL_POLYGON);
			for (int j=0; j<kVeryHighDefCircPts; j+=circStep_)
				glVertex3fv(vertex_[kVeryHighDefSlabs][j]);
		glEnd();
		//
		glNormal3fv(normalBottom_);
		glBegin(GL_POLYGON);
			for (int j=kVeryHighDefCircPts-circStep_; j>=0; j-=circStep_)
				glVertex3fv(vertex_[0][j]);
		glEnd();
	}
}


//==================================================================================
//	Cylinder3D private methods
//==================================================================================
#if 0
#pragma mark -
#endif

/*----------------------------------------------------------------------------------+
|	Function:		void init(GLfloat* origin, GLfloat** a)							|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					a			coordinates of the major axis, minor axis, and 		|
|									direction axis in a 3x3 0-based array.			|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the array of vertex and normal coordinates			|
|																					|
+----------------------------------------------------------------------------------*/
void Cylinder3D::init_(GLfloat* origin, GLfloat** a)
{
	GLfloat	dirStep = 1.f/kVeryHighDefSlabs,
			dir[kVeryHighDefSlabs+1][3],
			//	X, Y, and Z coordinates of the radial and normal directions.
			radial_j[3],
			majCross[3], minCross[3];
			
	for (int i=0; i<=kVeryHighDefSlabs; i++)
		vertex_[i] = GlUtils::glfMatrix(0, kVeryHighDefCircPts-1, 0, 2);
	
	//	majCross = a[0] x a[2] (major axis x direction)
	crossProduct(a[0], a[2], majCross);
	//	minCross = a[1] x a[2] (minor axis x direction)
	crossProduct(a[1], a[2], minCross);

	//	First, we precompute the graduation steps for the slabs along
	//	the "direction" axis (a[2]).
	for (int i=0; i<=kVeryHighDefSlabs; i++)
	{
		dir[i][0] = i*dirStep*a[2][0];
		dir[i][1] = i*dirStep*a[2][1];
		dir[i][2] = i*dirStep*a[2][2];
	}
			
	for (int j=0; j< kVeryHighDefCircPts; j++)
	{
		radial_j[0] = ct_[j]*a[0][0] + st_[j]*a[1][0];
		normal_[j][0] =	ct_[j]*minCross[0] - st_[j]*majCross[0];
		//
		radial_j[1] = ct_[j]*a[0][1] + st_[j]*a[1][1];
		normal_[j][1] =	ct_[j]*minCross[1] - st_[j]*majCross[1];
		//
		radial_j[2] = ct_[j]*a[0][2] + st_[j]*a[1][2];
		normal_[j][2] =	ct_[j]*minCross[2] - st_[j]*majCross[2];

		normalizeVector(normal_[j]);
			
		for (int i=0; i<=kVeryHighDefSlabs; i++)
		{
				//	M = origin + cos[u] major + sin[u] minor + v direction
				vertex_[i][j][0] = origin[0] + radial_j[0] + dir[i][0];
				vertex_[i][j][1] = origin[1] + radial_j[1] + dir[i][1];
				vertex_[i][j][2] = origin[2] + radial_j[2] + dir[i][2];
		}		
	}
	
	//	if this is a closed cylinder, then we must also compute the normal
	//	vector for the top and bottom faces.
	if (closedCyl_)
	{
		//	topNormal = major x minor / ||major x minor||
		commonNormalVector(a[0], a[1], normalTop_);
		normalBottom_[0] = -normalTop_[0];
		normalBottom_[1] = -normalTop_[1];
		normalBottom_[2] = -normalTop_[2];
	}
}

void Cylinder3D::deleteArrays_(void)
{
	if (vertex_!=NULL)
	{
		for (int i=0; i<=kVeryHighDefSlabs; i++)
			GlUtils::free_glfMatrix(vertex_[i], 0,  kVeryHighDefCircPts-1, 0, 2);
		delete [] vertex_;
		vertex_ = NULL;
	}
	if (normal_!=NULL)
	{
		GlUtils::free_glfMatrix(normal_, 0,  kVeryHighDefCircPts-1, 0, 2);
		normal_ = NULL;
	}
}


/*----------------------------------------------------------------------------------+
|	Function:		void initStatic(void)											|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		None															|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the class (static) sine and cosine value arrays		|
|																					|
+----------------------------------------------------------------------------------*/
void Cylinder3D::initStatic_(void)
{
	GLfloat	dTheta = static_cast<GLfloat>(2.f*PI/kVeryHighDefCircPts);

	for (int j=0; j<kVeryHighDefCircPts; j++)
	{
		ct_[j] = cosf(j*dTheta);
		st_[j] = sinf(j*dTheta);
	}
	
	initCtSt_ = true;
}

