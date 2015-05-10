/*  NAME:
        TubularSection3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib TubularSection3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstdlib>
//
#include "TubularSection3D.h"

using namespace uriVL;



//==================================================================================
//	TubularSection3D constructors and destructor
//==================================================================================

/*----------------------------------------------------------------------------------+
|	Constructor:	TubularSection3D(GLfloat* origin, GLfloat** axes,				|
|											GLfloat theta0, GLfloat thetaf)			|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					axes		coordinates of the major axis, minor axis, and 		|
|									direction axis in a 3x3 0-based array.			|
|					theta0		starting angle of the tubular section				|
|					thetaf		ending angle of the tubular section					|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
TubularSection3D::TubularSection3D(GLfloat* origin, GLfloat** axes, GLfloat theta0,
										GLfloat thetaf)
		:	GraphicComponent3D(),
			//
			vertex_(NULL),
			normal_(NULL),
			circStep_(kFineResolution),
			slabStep_(kMediumResolution),
			startAngle_(theta0),
			stopAngle_(thetaf),
			nbCircSegs_(0),
			tubeType_(FULL_TUBE)
{
	//	First determine how much of a fraction of a tube we actually must draw
	//	 Note: I only care about powers of two
	GLfloat dTheta = thetaf - theta0;
	FAIL_CONDITION( (dTheta < 0.L) || (dTheta > 2.L*PI),
		            kOpenGLNumAllocError, 
		            "incorrect tubular section");
	double thetaFrac = dTheta / (2.L*PI);
	//
	//	closer to the full tube
	if ( thetaFrac > 7.L/8.L )
	{
		nbCircSegs_ = kVeryHighDefCircPts;
		tubeType_ = NEARLY_FULL_TUBE;
	}
	//
	//	closer to the 3/4 tube
	else if ( thetaFrac > 5.L/8.L )
	{
		nbCircSegs_ = 3*kVeryHighDefCircPts/2;
		tubeType_ = ABOUT_3_QUARTER_TUBE;
	}
	//
	//	closer to the 1/2 tube
	else if ( thetaFrac > 3.L/8.L )
	{
		nbCircSegs_ = kVeryHighDefCircPts/2;
		tubeType_ = ABOUT_HALF_TUBE;
	}
	//
	//	closer to the quarter tube
	else if ( thetaFrac > 3.L/16.L )
	{
		nbCircSegs_ = 18;
		tubeType_ = ABOUT_QUARTER_CUBE;
	}
	//
	else //	about 1/8 tube or less
	{
		nbCircSegs_ = kVeryHighDefCircPts/4;
		tubeType_ = LESS_THAN_QUARTER_CUBE;
	}

	dTheta /= nbCircSegs_;

	init_(origin, axes, theta0, dTheta);
}

/*----------------------------------------------------------------------------------+
|	Constructor:	TubularSection3D(GLfloat* origin, GLfloat* dirAxis,				|
|									GLfloat radius,	GLfloat theta0, GLfloat thetaf)	|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					dirAxis		coordinates of the cylinder's axis.					|
|					theta0		starting angle of the tubular section				|
|					thetaf		ending angle of the tubular section					|
||																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
TubularSection3D::TubularSection3D(GLfloat* origin, GLfloat* dirAxis, GLfloat radius,
									GLfloat theta0, GLfloat thetaf)
		:	GraphicComponent3D(),
			//
			vertex_(NULL),
			normal_(NULL),
			circStep_(kFineResolution),
			slabStep_(kMediumResolution),
			startAngle_(theta0),
			stopAngle_(thetaf),
			nbCircSegs_(0),
			tubeType_(FULL_TUBE)
{	
	//	First determine how much of a fraction of a tube we actually must draw
	//	 Note: I only care about powers of two
	double dTheta = thetaf - theta0;
	FAIL_CONDITION( (dTheta < 0.L) || (dTheta > 2.L*PI),
		            kOpenGLNumAllocError, 
		            "incorrect tubular section");
	double thetaFrac = dTheta / (2.L*PI);
	//
	//	closer to the full tube
	if ( thetaFrac > 7.L/8.L )
	{
		nbCircSegs_ = kVeryHighDefCircPts;
		tubeType_ = NEARLY_FULL_TUBE;
	}
	//
	//	closer to the 3/4 tube
	else if ( thetaFrac > 5.L/8.L )
	{
		nbCircSegs_ = 3*kVeryHighDefCircPts/2;
		tubeType_ = ABOUT_3_QUARTER_TUBE;
	}
	//
	//	closer to the 1/2 tube
	else if ( thetaFrac > 3.L/8.L )
	{
		nbCircSegs_ = kVeryHighDefCircPts/2;
		tubeType_ = ABOUT_HALF_TUBE;
	}
	//
	//	closer to the quarter tube
	else if ( thetaFrac > 3.L/16.L )
	{
		nbCircSegs_ = 18;
		tubeType_ = ABOUT_QUARTER_CUBE;
	}
	//
	else //	about 1/8 tube or less
	{
		nbCircSegs_ = kVeryHighDefCircPts/4;
		tubeType_ = LESS_THAN_QUARTER_CUBE;
	}

	dTheta /= nbCircSegs_;
	
	GLfloat**	axes = GlUtils::glfMatrix(0, 2, 0, 2);

	try
	{
		//	dirAxis is the direction axis of the cylinder
		axes[2][0] = dirAxis[0];
		axes[2][1] = dirAxis[1];
		axes[2][2] = dirAxis[2]; 
		//
		//	For the major axis, we need to build a vector orthogonal to dirAxis.
		if ((dirAxis[0]!= 0.f) || (dirAxis[2]!= 0.f))	//	this is not the local Y axis
		{
			GLfloat w[3] = {0.f, 1.f, 0.f};
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
						"Invalid array passed to TubularSection3D constructor");
	}
	
	try
	{
		init_(origin, axes, theta0, static_cast<GLfloat>(dTheta));
	}
	catch (...)
	{
		deleteArrays_();
		GlUtils::free_glfMatrix(axes, 0, 2, 0, 2);
		FAIL_CONDITION( true,
						kInvalidParameterError,
						"Invalid array passed to TubularSection3D constructor");
	}
	
	GlUtils::free_glfMatrix(axes, 0, 2, 0, 2);
}


/*----------------------------------------------------------------------------------+
|	Destructor:		TubularSection3D(void)											|
|																					|
|	Parameters:		None															|
|																					|
|	Description:	Frees all the arrays that were allocated at the creation.		|																|
+----------------------------------------------------------------------------------*/
TubularSection3D::~TubularSection3D(void)
{
}


//==================================================================================
//	TubularSection3D public methods
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
void TubularSection3D::setResolution(ResolutionLevel res)
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
void TubularSection3D::setResolution(ResolutionLevel circRes, ResolutionLevel slabRes)
{
	//	 no coarse resolution for quarter tubes --> set to medium
	if ((tubeType_ == ABOUT_QUARTER_CUBE) && (circRes == kCoarseResolution))
		circRes = kMediumResolution;
	//	 no very low resolution for 1/8 tubes --> set to coarse
	else if ((tubeType_ == LESS_THAN_QUARTER_CUBE) && (circRes == kVeryLowResolution))
		circRes = kCoarseResolution;


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
void TubularSection3D::draw(void)
{
	int	i, j;
	
	GraphicComponent3D::draw();
	
	for (i=0; i<=kVeryHighDefSlabs-slabStep_; i+=slabStep_)
	{
		glBegin(GL_TRIANGLE_STRIP);
			for (j=0; j<=nbCircSegs_; j+=circStep_)
			{ 
				glNormal3fv(normal_[j]);
				glVertex3fv(vertex_[i+slabStep_][j]);
				glNormal3fv(normal_[j]);
				glVertex3fv(vertex_[i][j]);
			}
			
		glEnd();
	}
}


//==================================================================================
//	TubularSection3D private methods
//==================================================================================
#if 0
#pragma mark -
#endif


/*----------------------------------------------------------------------------------+
|	Function:		void init(GLfloat* origin, GLfloat** a, GLfloat theta0,			|
|									GLfloat thetaf)									|
|																					|
|	Status:			Public															|
|																					|
|	Parameters:		origin		coordinates of the origin (bottom' center)			|
|					a			coordinates of the major axis, minor axis, and 		|
|									direction axis in a 3x3 0-based array.			|
|					theta0		starting angle of the tubular section				|
|					dTheta		angle spread of the tubular section					|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the array of vertex and normal coordinates			|
|																					|
+----------------------------------------------------------------------------------*/
void TubularSection3D::init_(GLfloat* origin, GLfloat** a, GLfloat theta0, GLfloat dTheta)
{
	GLfloat	dirStep = 1.f/kVeryHighDefSlabs,
			dir[kVeryHighDefSlabs+1][3],
			//	X, Y, and Z coordinates of the radial and normal directions.
			radial_j[3],
			majCross[3], minCross[3];
			
	//	Allocate the arrays of vertices and normal vectors
	vertex_ = new GLfloat** [kVeryHighDefSlabs+1];
	normal_ = GlUtils::glfMatrix(0, nbCircSegs_, 0, 2);
	
	for (int i=0; i<=kVeryHighDefSlabs; i++)
		vertex_[i] = GlUtils::glfMatrix(0, nbCircSegs_, 0, 2);
		
	//	majCross = a[0] x a[2] (major axis x direction)
	crossProduct(a[0], a[2], majCross);
	//	minCross = a[1] x a[2] (minor axis x direction)
	crossProduct(a[1], a[2], minCross);

	//	First, we precompute the graduation steps for the slabs along
	//	the "direction" axis (a[2]).
	for (int i=0; i<=kVeryHighDefSlabs; i++)
		for (int k=0; k<3; k++)
			dir[i][k] = i*dirStep*a[2][k];
			
	GLfloat theta_j = theta0;
	for (int j=0; j<= nbCircSegs_; j++)
	{
		GLfloat cos_tj = cosf(theta_j);
		GLfloat sin_tj = sinf(theta_j);
		
		radial_j[0] = cos_tj*a[0][0] + sin_tj*a[1][0];
		normal_[j][0] =	cos_tj*minCross[0] - sin_tj*majCross[0];
		//
		radial_j[1] = cos_tj*a[0][1] + sin_tj*a[1][1];
		normal_[j][1] =	cos_tj*minCross[1] - sin_tj*majCross[1];
		//
		radial_j[2] = cos_tj*a[0][2] + sin_tj*a[1][2];
		normal_[j][2] =	cos_tj*minCross[2] - sin_tj*majCross[2];
		
		normalizeVector(normal_[j]);
			
		for (int i=0; i<=kVeryHighDefSlabs; i++)
		{
			//	M = origin + cos[u] major + sin[u] minor + v direction
			vertex_[i][j][0] = origin[0] + radial_j[0] + dir[i][0];
			vertex_[i][j][1] = origin[1] + radial_j[1] + dir[i][1];
			vertex_[i][j][2] = origin[2] + radial_j[2] + dir[i][2];
		}
		
		theta_j += dTheta;
	}
}


void TubularSection3D::deleteArrays_(void)
{
	if (vertex_!=NULL)
	{
		for (int i=0; i<=kVeryHighDefSlabs; i++)
			GlUtils::free_glfMatrix(vertex_[i], 0,  nbCircSegs_, 0, 2);
		delete [] vertex_;
		vertex_ = NULL;
	}
	if (normal_!=NULL)
	{
		GlUtils::free_glfMatrix(normal_, 0,  nbCircSegs_, 0, 2);
		normal_ = NULL;
	}
}
