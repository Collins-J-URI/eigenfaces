/*  NAME:
        GraphicComponent3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib GraphicComponent3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#include <cmath>

#include "GraphicComponent3D.h"

using namespace uriVL;


//==================================================================================
//	Box constructors and destructor
//==================================================================================

/*----------------------------------------------------------------------------------+
|	Constructor:	GraphicComponent3D(void)										|
|																					|
|	Parameters:		None															|
|																					|
|	Description:	does nothing													|
+----------------------------------------------------------------------------------*/
GraphicComponent3D::GraphicComponent3D(void)
	:	shiny_(0.f)
{
}

/*----------------------------------------------------------------------------------+
|	Destructor:		~GraphicComponent3D(void)										|
|																					|
|	Parameters:		None															|
|																					|
|	Description:	does nothing													|
+----------------------------------------------------------------------------------*/
GraphicComponent3D::~GraphicComponent3D(void)
{
}

//==================================================================================
//	GraphicComponent3D public methods
//==================================================================================
#if 0
#pragma mark -
#endif

/*----------------------------------------------------------------------------------+
|	Function:	void storeReflectParams(float params[])								|
|																					|
|	Status:			public															|
|																					|
|	Parameters:		params		Reflectance properties of the object's material		|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the reflectance properties of the object			|
|																					|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::setMaterial(float params[])
{
	ambientMat_[0] = params[0];
	ambientMat_[1] = params[1];
	ambientMat_[2] = params[2];
	//
	diffuseMat_[0] = params[3];
	diffuseMat_[1] = params[4];
	diffuseMat_[2] = params[5];
	//
	specularMat_[0] = params[6];
	specularMat_[1] = params[7];
	specularMat_[2] = params[8];
	//
	shiny_ = params[9];
}

void GraphicComponent3D::setMaterial(GLfloat ambRed, GLfloat ambGreen, GLfloat ambBlue, GLfloat difRed,
				GLfloat difGreen, GLfloat difBlue, GLfloat specRed, GLfloat specGreen,
				GLfloat specBlue, GLfloat shine)
{
	ambientMat_[0] = ambRed;
	ambientMat_[1] = ambGreen;
	ambientMat_[2] = ambBlue;
	//
	diffuseMat_[0] = difRed;
	diffuseMat_[1] = difGreen;
	diffuseMat_[2] = difBlue;
	//
	specularMat_[0] = specRed;
	specularMat_[1] = specGreen;
	specularMat_[2] = specBlue;
	//
	shiny_ = shine;
}


/*----------------------------------------------------------------------------------+
|	Function:	void setReflectParams(void)											|
|																					|
|	Status:			protected														|
|																					|
|	Parameters:		None															|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Sets the GL material properties according to the oarameters		|
|					stored for this object, prior to drawing the object				|
|																					|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::setReflectionParameters_(void)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMat_);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMat_);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMat_);
	glMaterialf(GL_FRONT, GL_SHININESS, shiny_);
}


void GraphicComponent3D::draw(void)
{
	setReflectionParameters_();
}


//==================================================================================
//	GraphicComponent3D public static methods
//==================================================================================
#if 0
#pragma mark -
#endif

/*----------------------------------------------------------------------------------+
|	Function:	GLfloat norm(GLfloat* v)											|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		V			3d vector to normalize								|
|																					|
|	Returns:		Norm of vector V												|
|																					|
|	Description:	Utility function that computes the norm of a 3d vector			|
|																					|
+----------------------------------------------------------------------------------*/
GLfloat GraphicComponent3D::norm(GLfloat* V)
{
	return sqrtf(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
}


/*----------------------------------------------------------------------------------+
|	Function:	void normalizeVector(GLfloat* V)									|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		V			3d vector to normalize								|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Utility function that normalizes a 3d vector					|
|																					|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::normalizeVector(GLfloat* V)
{
	GLfloat normScale = 1.0f / norm(V);
				
	V[0] *= normScale;
	V[1] *= normScale;
	V[2] *= normScale;
}


/*----------------------------------------------------------------------------------+
|	Function:	GLfloat dotProduct(GLfloat* e1, GLfloat e2)							|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		e1, e2		two 3d cordinate vectors							|
|																					|
|	Returns:		Norm of vector V												|
|																					|
|	Description:	Computes the dot product of two 3d vectors						|
|																					|
+----------------------------------------------------------------------------------*/
GLfloat GraphicComponent3D::dotProduct(GLfloat* e1, GLfloat* e2)
{
	return e1[0]*e2[0] + e1[1]*e2[1] + e1[2]*e2[2];
}


/*----------------------------------------------------------------------------------+
|	Function:	void crossProduct(GLfloat* e1, GLfloat* e2, GLfloat* nv)			|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		e1, e2		two 3d cordinate vectors							|
|					nv			their cross product.								|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Computes the cross product of two 3d vectors.					|
|																					|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::crossProduct(GLfloat* e1, GLfloat* e2, GLfloat* nv)
{
	nv[0] = e1[1]*e2[2]-e1[2]*e2[1];
	nv[1] = e1[2]*e2[0]-e1[0]*e2[2];
	nv[2] = e1[0]*e2[1]-e1[1]*e2[0];
}


/*----------------------------------------------------------------------------------+
|	Function:	void commonNormalVector(GLfloat* e1, GLfloat* e2, GLfloat* nv)		|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		e1, e2		two 3d cordinate vectors							|
|					nv			e1 and e2's normalized common normal vector			|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Computes the cross product of the 2 input vectors and then		|
|					normalizes the result.											|
|																					|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::commonNormalVector(GLfloat* e1, GLfloat* e2, GLfloat* nv)
{
	nv[0] = e1[1]*e2[2]-e1[2]*e2[1];
	nv[1] = e1[2]*e2[0]-e1[0]*e2[2];
	nv[2] = e1[0]*e2[1]-e1[1]*e2[0];

	normalizeVector(nv);
}


/*----------------------------------------------------------------------------------+
|	Function:	void faceExactNormal(GLfloat* v0, GLfloat* v1, GLfloat* v2,			|
|										GLfloat* v3, GLfloat* nVect)				|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		v0, v1, v2, v3		coordinates of a triangle's vertices		|
|					nVect				the triangle's  normal vector				|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Computes the average normal vector of a triangular face			|
|					specified by the coordinates of its three vertices				|
|																					|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::faceExactNormal(GLfloat* v0, GLfloat* v1, GLfloat* v2,
											GLfloat* nVect)
{
	GLfloat	e1[3] = {v1[0]-v0[0], v1[1]-v0[1], v1[2]-v0[2]},
			e2[3] = {v2[0]-v0[0], v2[1]-v0[1], v2[2]-v0[2]};
				
	commonNormalVector(e1, e2, nVect);
}


/*----------------------------------------------------------------------------------+
|	Function:	void faceAverageNormal(GLuint n, GLfloat** v, GLfloat* nVect)		|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		n			number of vertices of the polygonal face			|
|					v			array of vertices' coordinates for the polygon		|
|					nVect		the polygonal face's average normal vector			|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Computes the average normal vector of a polygonal face that is	|
|					specified by the coordinates of its vertices as a 2D array		|
|					The vertices are expected to be listed counterclockwise.		|
|																					|
|	Comment:		This function might as well be private, since it will rarely be	|
|					called directly.  One of the next 2 methods, which receive		|
|					among their parameters a 1D array of vertex indexes (for a		|
|					polyhedron object), will/should be called instead.				|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::faceAverageNormal(GLuint n, GLfloat** v, GLfloat* nVect)
{
	GLfloat	nv[3],
			e1[3], e2[3],
			theta,
			totalAngle;
	int	i, i1, i2, k;
	
	//	The difficulty in computing the average normal vector of a polygonal
	//	patch is that we can't just take any 3-tuple of vertices and compute a
	//	cross product because we don't know which way the computed normal vector
	//	will be pointing (i.o.w., do the 3 vertices form a convex or concave corner
	//	of the polygon?).  Here I am using the fact that going around the polygon
	//	counterclockwise really corresponds to doing a +2pi rotation around the 
	//	normal vector.  So here is how I proceed.  Start from the first 3 points,
	//	compute the cross product, normalize.  This is +/- the desired normal vector
	//	(+ some error), but I don't know which.
	//	For all v_i, v_i+1 v_i+2 3-tuples of vertices, compute the cross product,
	//	if it points in the same direction as the first one, increment the angle,
	//	else decrement it.  In any case, add up +/- computed normal vector for the
	//	average.  In the end, if the sum of angles is 2pi, we have the right normal
	//	vector, otherwise we have its opposite.
	
	//-------------------------------------------------------------------------
	//	Initialize the computation with the first 3 vertices
	//-------------------------------------------------------------------------
	//	1. Compute the edge vectors and normalize them
	for (k=0; k<3; k++)
	{
		e1[k] = v[1][k] - v[0][k];
		e2[k] = v[2][k] - v[0][k];
	}
	normalizeVector(e1);
	normalizeVector(e2);
	
	//	2. Compute the cross product.  Store its normalized version as the 
	//		first normal vector.  Compute the dot product and store its arcCos
	//		as the angle of the rotation.
	commonNormalVector(e1, e2, nVect);
	totalAngle = acosf(dotProduct(e1, e2));
	
	//-------------------------------------------------------------------------
	//	Go through the polygon, counterclockwise, and repeat the following
	//	step for all 3-tuples of consecutive vertices.
	//-------------------------------------------------------------------------
	for (i=1; i< static_cast<int>(n); i++)
	{
		i1 = (i+1)%n;
		i2 = (i+2)%n;
	
		//	1. Compute the edge vectors and normalize them
		for (k=0; k<3; k++)
		{
			e1[k] = v[i1][k] - v[i][k];
			e2[k] = v[i2][k] - v[i][k];
		}
		normalizeVector(e1);
		normalizeVector(e2);
	
		//	2. compute the cross product and the angle of the rotation
		commonNormalVector(e1, e2, nv);
		theta = acosf(dotProduct(e1, e2));
		
		//	3. If nv points in the same direction as the initial normal vector
		//		everything is added up.
		if (dotProduct(nv, nVect) > 0)
		{
			for (k=0; k<3; k++)
				nVect[k] += nv[k];
			
			totalAngle += theta;
		}
		//	if the vectors point in opposite directions, we substract.
		else
		{
			for (k=0; k<3; k++)
				nVect[k] -= nv[k];
			
			totalAngle -= theta;
		}
	}

	//-------------------------------------------------------------------------
	//	Once we have gone through all vertices, we check the sign of the total
	//	rotation we computed.  If it is > 0 (should be 2Pi), then we just have
	//	to normalize the normal vector.  If it is negative (should be -2Pi),
	//	then we must invert the normal vector before we return it.
	//-------------------------------------------------------------------------
	if (totalAngle < 0.f)
		for (k=0; k<3; k++)
			nVect[k] *= -1.f;
	
	normalizeVector(nVect);
}


/*----------------------------------------------------------------------------------+
|	Function:	void faceAverageNormal(GLuint* index, GLfloat** v, GLfloat* nVect)	|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		index		array of vertex indexes for the polygonal face		|
|									the 1st element of the array = nb vertices		|
|					v			array of vertices' coordinates for the polygon		|
|					nVect		the polygonal face's average normal vector			|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Computes the average normal vector of a polygonal face that is	|
|					specified by the coordinates of its vertices as a 2D array		|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::faceAverageNormal(GLuint* index, GLfloat** v, GLfloat* nVect)
{
	GLfloat		**vArray;
	unsigned int		i;
	
	vArray  = new GLfloat*[index[0]];
	for (i=0; i<index[0]; i++)
		vArray[i] = v[index[i+1]];
		
	faceAverageNormal(index[0], vArray, nVect);
	
	delete [] vArray;
}


/*----------------------------------------------------------------------------------+
|	Function:	void faceAverageNormal(GLfloat* v0, GLfloat* v1, GLfloat* v2,		|
|										GLfloat* v3, GLfloat* nVect)				|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		v0, v1, v2, v3		arrays of coordinates of a quad's vertices	|
|					nVect				the quad's average normal vector			|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Computes the average normal vector of a quad face specified by	|
|					the coordinates of its four vertices							|
|																					|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::faceAverageNormal(GLfloat* v0, GLfloat* v1, GLfloat* v2,
			GLfloat* v3, GLfloat* nVect)
{
	//	computes the norm at the vertex as the average of the norm of the three
	//	triangles that this vertex participates to: v0-v1-v2, v0-v1-v3, v0-v2-v3
	nVect[0] = 2.f * (v3[1]*v0[2] - v1[1]*v0[2] + v0[1]*v1[2] - v0[1]*v3[2]) - v2[1]*v1[2] - 
    				v3[1]*v1[2] + v1[1]*v2[2] - v3[1]*v2[2] + v1[1]*v3[2] + v2[1]*v3[2];
     
  	nVect[1] = 2.f * (v1[0]*v0[2] - v3[0]*v0[2] - v0[0]*v1[2] + v0[0]*v3[2]) + v2[0]*v1[2] +
  					v3[0]*v1[2] - v1[0]*v2[2] + v3[0]*v2[2] - v1[0]*v3[2] - v2[0]*v3[2];
    
    nVect[2] = 2.f * (v3[0]*v0[1] - v1[0]*v0[1] + v0[0]*v1[1] - v0[0]*v3[1]) - v2[0]*v1[1] -
    				v3[0]*v1[1] + v1[0]*v2[1] - v3[0]*v2[1] + v1[0]*v3[1] + v2[0]*v3[1];

	normalizeVector(nVect);	
}

/*----------------------------------------------------------------------------------+
|	Function:	void vertexAverageNormal(GLfloat* v0, GLfloat* vL, GLfloat* vR,		|
|										GLfloat* d, GLfloat* nVect)					|
|																					|
|	Status:			static public													|
|																					|
|	Parameters:		v0			the vertex where the normal should be computed		|
|					vL, vR		left and right neighbors of v0 along a slice		|
|					d			local direction of the slab							| 
|					nVect		the vertex' average normal vector					|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Computes the average normal vector at a vertex of a smooth		|
|					mesh.															|
|																					|
|	Comments:		This version is really more appropriate for a prism-like mesh	|
|					since it identifies a "direction" for the mesh.					|
|																					|
+----------------------------------------------------------------------------------*/
void GraphicComponent3D::vertexAverageNormal(GLfloat* v0, GLfloat* vL, GLfloat* vR,
						GLfloat* d, GLfloat* nVect)
{
	GLfloat		e[3] = {vR[0]-vL[0], vR[1]-vL[1], vR[2]-vL[2]};
	
	//	n (unscaled) = (v0 - vL) x d  +  (vR - v0) x d  =  (vR - vL) x d.
	commonNormalVector(e, d, nVect);
}



