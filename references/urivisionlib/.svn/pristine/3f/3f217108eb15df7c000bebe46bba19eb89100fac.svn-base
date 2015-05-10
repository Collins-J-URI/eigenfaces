/*  NAME:
        HalfCone3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib HalfCone3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstdlib>
//
#include "HalfCone3D.h"

using namespace uriVL;

//	Redefinition of the class' static elements
//
bool	HalfCone3D::initCtSt_ = false;
GLfloat HalfCone3D::ct_[kVeryHighDefCircPts],
		HalfCone3D::st_[kVeryHighDefCircPts];


//==================================================================================
//	HalfCone3D constructors and destructor
//==================================================================================

/*----------------------------------------------------------------------------------+
|	Constructor:	HalfCone3D(GLfloat* origin, GLfloat** axes,					|
|								bool lengthIsClosed, bool sidesAreClosed)			|
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
HalfCone3D::HalfCone3D(GLfloat* origin, GLfloat** axes, bool lengthIsClosed,
						bool sideIsClosed)
		:	GraphicComponent3D(),
			//
			vertex_(new GLfloat** [kVeryHighDefSlabs+1]),
			normal_(GlUtils::glfMatrix(0, kVeryHighDefCircPtsHalf, 0, 2)),
			circStep_(kFineResolution),
			slabStep_(kMediumResolution),
			closedBottom_(sideIsClosed),
			closedTop_(false),
			closedLength_(sideIsClosed)
{
	if (!initCtSt_)
		initStatic_();
		
	try {
		init_(origin, axes);
	}
	catch (ErrorReport& e) {
		deleteArrays_();
		e.appendToMessage("called from HalfCone3D constructor.");
		throw e;
	}
}


/*----------------------------------------------------------------------------------+
|	Constructor:	HalfCone3D(GLfloat* origin,GLfloat** botAxes, GLfloat* dirAxis,	|
|							GLfloat** topAxes, bool lengthIsClosed,					|
|							bool sidesAreClosed)									|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					botAxes		major and minor axes of the bottom face				|
|					dirAxis		coordinates of the cylinder's axis.					|
|					topAxes		major and minor axes of the top face				|
|					lengthIsClosed	should the length face be drawn?				|
|					sidesAreClosed	should the side faces be drawn?					|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
HalfCone3D::HalfCone3D(GLfloat* origin, GLfloat** botAxes, GLfloat* dirAxis,
					GLfloat** topAxes, bool lengthIsClosed, bool sidesAreClosed)
		:	GraphicComponent3D(),
			//
			vertex_(new GLfloat** [kVeryHighDefSlabs+1]),
			normal_(GlUtils::glfMatrix(0, kVeryHighDefCircPtsHalf, 0, 2)),
			circStep_(kFineResolution),
			slabStep_(kMediumResolution),
			closedBottom_(sidesAreClosed),
			closedTop_(false),
			closedLength_(lengthIsClosed)
{
	if (!initCtSt_)
		initStatic_();

	try {
		init_(origin, botAxes, dirAxis, topAxes);
	}
	catch (ErrorReport& e) {
		deleteArrays_();
		e.appendToMessage("called from HalfCone3D constructor.");
		throw e;
	}
}


/*----------------------------------------------------------------------------------+
|	Constructor:	HalfCone3D(GLfloat* origin,GLfloat* dirAxis,GLfloat bottomRad,	|
|								GLfloat topRad, bool lengthIsClosed,				|
|								bool sidesAreClosed)								|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					dirAxis		coordinates of the cylinder's axis.					|
|					bottomRad	the cone's bottom radius							|
|					topRad	the cone's top radius									|
|					lengthIsClosed	should the length face be drawn?				|
|					sidesAreClosed	should the side faces be drawn?					|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
HalfCone3D::HalfCone3D(GLfloat* origin, GLfloat* dirAxis, GLfloat bottomRad,
						GLfloat topRad, bool lengthIsClosed, bool sidesAreClosed)
		:	GraphicComponent3D(),
			//
			vertex_(new GLfloat** [kVeryHighDefSlabs+1]),
			normal_(GlUtils::glfMatrix(0, kVeryHighDefCircPts-1, 0, 2)),
			circStep_(kFineResolution),
			slabStep_(kMediumResolution),
			closedBottom_(sidesAreClosed),
			closedTop_(sidesAreClosed),
			closedLength_(lengthIsClosed)
{
	if (!initCtSt_)
		initStatic_();
		
	GLfloat		**botAxes, **topAxes, w[3];
	
	botAxes = GlUtils::glfMatrix(0, 2, 0, 2);
	topAxes = GlUtils::glfMatrix(0, 2, 0, 2);

	try {
		//	For the major axis, we need to build a vector orthogonal to dirAxis.
		if ((dirAxis[0]!= 0.f) || (dirAxis[2]!= 0.f))	//	this is not the local Y axis
		{
			w[0] = 0.f; w[1] = 1.f; w[2] = 0.f;
			commonNormalVector(w, dirAxis, botAxes[0]);				//	"X" = "Y" x "Z"
			commonNormalVector(dirAxis, botAxes[0], botAxes[1]); 	//	"Y" = "Z" x "X"
			//
			//	and now we can multiply by the radius
			for (int i=0; i<3; i++)
			{
				topAxes[0][i] = botAxes[0][i]*topRad;
				topAxes[1][i] = botAxes[1][i]*topRad;
				//
				botAxes[0][i] *= bottomRad;
				botAxes[1][i] *= bottomRad;
			}
		}
		//	if dirAxis was the local Y axis, then I can take Z and X as major and minor
		else
		{
			topAxes[0][0] = topAxes[0][1] = topAxes[1][1] = topAxes[1][2] = 0.f;
			botAxes[0][0] = botAxes[0][1] = botAxes[1][1] = botAxes[1][2] = 0.f;
			topAxes[0][2] = topAxes[1][0] = topRad;
			botAxes[0][2] = botAxes[1][0] = bottomRad;
		}
	}
	catch (...)
	{
		deleteArrays_();
		GlUtils::free_glfMatrix(topAxes, 0, 2, 0, 2);
		GlUtils::free_glfMatrix(botAxes, 0, 2, 0, 2);
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid array passed to Cone constructor");
	}
	
	try {
		init_(origin, botAxes, dirAxis, topAxes);
	}
	catch (ErrorReport& e) {
		deleteArrays_();
		GlUtils::free_glfMatrix(topAxes, 0, 2, 0, 2);
		GlUtils::free_glfMatrix(botAxes, 0, 2, 0, 2);
		e.appendToMessage("called from Cone constructor.");
		throw e;
	}
	
	GlUtils::free_glfMatrix(topAxes, 0, 2, 0, 2);
	GlUtils::free_glfMatrix(botAxes, 0, 2, 0, 2);
}


HalfCone3D::HalfCone3D(const HalfCone3D& theBox)
		:	GraphicComponent3D(),
			//
			vertex_(NULL),
			normal_(NULL),
			circStep_(kCoarseResolution),
			slabStep_(kCoarseResolution),
			closedBottom_(false),
			closedTop_(false),
			closedLength_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "HalfCone3D copy constructor not implemented.");
}


/*----------------------------------------------------------------------------------+
|	Destructor:		~HalfCone(void)													|
|																					|
|	Parameters:		None															|
|																					|
|	Description:	Frees all the arrays that were allocated at the creation.		|																|
+----------------------------------------------------------------------------------*/
HalfCone3D::~HalfCone3D(void)
{
	deleteArrays_();
}


const HalfCone3D& HalfCone3D::operator = (const HalfCone3D& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "HalfCone3D copy operator not implemented.");

	return *this;
}



//==================================================================================
//	HalfCone3D public methods
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
|	Description:	Adjusts private variables for proper display of the 1/2 cone	|
|																					|
+----------------------------------------------------------------------------------*/
void HalfCone3D::setResolution(ResolutionLevel res)
{
	setResolution(res, res);
}


/*----------------------------------------------------------------------------------+
|	Function:		void setResolution(ResolutionLevel circRes,					|
|										ResolutionLevel slabRes)					|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		circRes:	desired nb of points on a circle					|
|					slabRes:	desired slab resolution level						|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Adjusts private variables for proper display of the tube		|
|																					|
+----------------------------------------------------------------------------------*/
void HalfCone3D::setResolution(ResolutionLevel circRes, ResolutionLevel slabRes)
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
|	Description:	Draws the tubular section										|
|																					|
+----------------------------------------------------------------------------------*/
void HalfCone3D::draw(void)
{
	GraphicComponent3D::draw();
	
	for (int i=0; i<=kVeryHighDefSlabs-slabStep_; i+=slabStep_)
	{
		glBegin(GL_TRIANGLE_STRIP);
			for (int j=0; j<=kVeryHighDefCircPtsHalf; j+=circStep_)
			{
				glNormal3fv(normal_[j]);
				glVertex3fv(vertex_[i+slabStep_][j]);
				glNormal3fv(normal_[j]);
				glVertex3fv(vertex_[i][j]);
			}
			
		glEnd();
	}

	if (closedTop_)
	{
		glNormal3fv(normalTop_);
		glBegin(GL_POLYGON);
			for (int j=0; j<=kVeryHighDefCircPtsHalf; j+=circStep_)
				glVertex3fv(vertex_[kVeryHighDefSlabs][j]);
		glEnd();
	}
	if (closedBottom_)
	{
		glNormal3fv(normalBottom_);
		glBegin(GL_POLYGON);
			for (int j=kVeryHighDefCircPtsHalf; j>=0; j-=circStep_)
				glVertex3fv(vertex_[0][j]);
		glEnd();
	}
	
	if (closedLength_)
	{
		glNormal3fv(normalLength_);
		glBegin(GL_QUADS);
			glVertex3fv(vertex_[0][0]);
			glVertex3fv(vertex_[kVeryHighDefSlabs][0]);
			glVertex3fv(vertex_[kVeryHighDefSlabs][kVeryHighDefCircPtsHalf]);
			glVertex3fv(vertex_[0][kVeryHighDefCircPtsHalf]);
		glEnd();
		
	}
}


//==================================================================================
//	HalfCone3D private methods
//==================================================================================
#if 0
#pragma mark -
#endif


/*----------------------------------------------------------------------------------+
|	Function:		void init(GLfloat* origin, GLfloat** a)							|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					a			coordinates of the major axis, minor axis, and 		|
|									direction axis in a 3x3 0-based array.			|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the array of vertex and normal coordinates			|
|																					|
|	Comments:		This constructor implements the "pointed" cone, where the point	|
|					is at origin + axes[2]											|
|																					|
+----------------------------------------------------------------------------------*/
void HalfCone3D::init_(GLfloat* origin, GLfloat** a)
{
	GLfloat	dirStep = 1.f/kVeryHighDefSlabs,
			d[3],
			//	The summit of the cone is = origin + axes[2]
			summit[3] = {	origin[0] + a[2][0],
							origin[1] + a[2][1],
							origin[2] + a[2][2]},
							
			//	X, Y, and Z coordinates of the radial and normal directions.
			radial_j[3],
			majCross[3], minCross[3];
			
	//	majCross = a[0] x a[2] (major axis x direction)
	crossProduct(a[0], a[2], majCross);
	//	minCross = a[1] x a[2] (minor axis x direction)
	crossProduct(a[1], a[2], minCross);
	
	for (int j=0; j<=kVeryHighDefCircPtsHalf; j++)
	{
		for (int k=0; k<3; k++)
		{
			//	coordinates of the radial vector for that angle
			radial_j[k] = ct_[j]*a[0][k] + st_[j]*a[1][k];

			//	coordinates of the bottom point for that angle
			vertex_[0][j][k] = origin[k] + radial_j[k]; 
			
			//	the "top vertex" is at the summit of the cone
			vertex_[kVeryHighDefSlabs][j][k] = summit[k];
			
			//	coordinates of the "step displacement towards the summit.
			d[k] = dirStep*(a[2][k] - radial_j[k]);
			
			//	coordinates of the normal vector for that angle
			normal_[j][k] =	ct_[j]*minCross[k] - st_[j]*majCross[k];

		}
		normalizeVector(normal_[j]);
				

		for (int i=1; i<kVeryHighDefSlabs; i++)
		{
			vertex_[i][j][0] = vertex_[i-1][j][0] + d[0];
			vertex_[i][j][1] = vertex_[i-1][j][1] + d[1];
			vertex_[i][j][2] = vertex_[i-1][j][2] + d[2];
		}
	}
	
	//	if this is a closed cone, then we must also compute the normal
	//	vector for the bottom face.
	//	normalBottom = -major x minor / ||major x minor||
	//
	if (closedBottom_)
		commonNormalVector(a[1], a[0], normalBottom_);

	//	Same thing for the length side
	if (closedLength_)
		//	normalLength_ = major x direction / ||major x direction||
		commonNormalVector(a[0], a[2], normalLength_);

}


/*----------------------------------------------------------------------------------+
|	Function:		void init(GLfloat* origin, GLfloat** botAxes, GLfloat* dirAxis,	|
|										GLfloat** topAxes)							|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					botAxes		coordinates of the bottom major axis & minor axes	|
|					dirAxis		coordinates of the cylinder's axis.					|
|					topAxes		coordinates of the top major axis & minor axes		|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the array of vertex and normal coordinates			|
|																					|
+----------------------------------------------------------------------------------*/
void HalfCone3D::init_(GLfloat* origin, GLfloat** botAxes, GLfloat* dirAxis,
					GLfloat** topAxes)
{
	GLfloat	dirStep = 1.f/kVeryHighDefSlabs,
			//	X, Y, and Z coordinates of the radial and normal directions.
			radialTop_j[3], radialBot_j[3],
			d[3],
			majCross[3], minCross[3];
	GLfloat origTop[3] = {	origin[0] + dirAxis[0], 
							origin[1] + dirAxis[1],
							origin[2] + dirAxis[2]};

	for (int i=0; i<=kVeryHighDefSlabs; i++)
		vertex_[i] = GlUtils::glfMatrix(0, kVeryHighDefCircPts-1, 0, 2);
	
	for (int j=0; j<=kVeryHighDefCircPtsHalf; j++)
	{
		for (int k=0; k<3; k++)
		{
			radialTop_j[k] = ct_[j]*topAxes[0][k] + st_[j]*topAxes[1][k];
			radialBot_j[k] = ct_[j]*botAxes[0][k] + st_[j]*botAxes[1][k];

		
			vertex_[0][j][k] = origin[k] + radialBot_j[k];
			vertex_[kVeryHighDefSlabs][j][k] = origTop[k] + radialTop_j[k];
			d[k] = dirStep*(vertex_[kVeryHighDefSlabs][j][k] - vertex_[0][j][k]);
		}
		
		//	Ready to compute the coordinates of the cones "inner" vertices
		for (int i=1; i<kVeryHighDefSlabs; i++)
		{
			vertex_[i][j][0] = vertex_[i-1][j][0] + d[0];
			vertex_[i][j][1] = vertex_[i-1][j][1] + d[1];
			vertex_[i][j][2] = vertex_[i-1][j][2] + d[2];
		}
		
		//	 We can also compute the normal vector for each angular section of the cone
		crossProduct(botAxes[0], d, majCross);
		crossProduct(botAxes[1], d, minCross);
		for (int k=0; k<3; k++)
			normal_[j][k] =	ct_[j]*minCross[k] - st_[j]*majCross[k];
		//
		normalizeVector(normal_[j]);
	}
	
	//	if this is a closed cone, then we must also compute the normal
	//	vector for the top and bottom faces.
	if (closedTop_)
		//	normalTop = major x minor / ||major x minor||
		commonNormalVector(topAxes[0], topAxes[1], normalTop_);
	//
	if (closedBottom_)
		//	normalBottom = -major x minor / ||major x minor||
		commonNormalVector(botAxes[1], botAxes[0], normalBottom_);

	//	Same thing for the length side
	if (closedLength_)
		//	normalLength = major x direction / ||major x direction||
		commonNormalVector(botAxes[0], dirAxis, normalLength_);

}

void HalfCone3D::deleteArrays_(void)
{
	if (vertex_!=NULL)
	{
		for (int i=0; i<=kVeryHighDefSlabs; i++)
			GlUtils::free_glfMatrix(vertex_[i], 0,  kVeryHighDefCircPtsHalf, 0, 2);
		delete [] vertex_;
		vertex_ = NULL;
	}
	if (normal_!=NULL)
	{
		GlUtils::free_glfMatrix(normal_, 0,  kVeryHighDefCircPtsHalf, 0, 2);
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
void HalfCone3D::initStatic_(void)
{
	GLfloat	dTheta = static_cast<GLfloat>(2.f*PI/kVeryHighDefCircPts);

	for (int j=0; j<kVeryHighDefCircPts; j++)
	{
		ct_[j] = cosf(j*dTheta);
		st_[j] = sinf(j*dTheta);
	}
	
	initCtSt_ = true;
}


