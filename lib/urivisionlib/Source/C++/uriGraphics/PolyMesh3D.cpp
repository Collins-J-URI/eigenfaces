/*  NAME:
        PolyMesh3D.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib PolyMesh3D class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cmath>
#include <cstdlib>
//
#include "PolyMesh3D.h"

using namespace uriVL;

//==================================================================================
//	PolyMesh3D constructors and destructor
//==================================================================================

/*----------------------------------------------------------------------------------+
|	Constructor:	PolyMesh3D(int theNbVert, int theNbFaces,				        |
|							GLfloat** theVert, GLuint** theFace, bool isSmooth)		|
|																					|
|	Parameters:		theNbVert		number of vertices of the polyhedron			|
|					theNbFaces		number of polygonal faces of the polyhedron		|
|					theVert			array of vertices' coordinates					|
|					theFace			array of vertices indexes						|
|					isSmooth		are the sides smooth or faceted?				|
|																					|
|	Description:	Allocates the vertex and normal arrays, then invokes the 		|
|					init() method	to initialize these arrays.						|
+----------------------------------------------------------------------------------*/
PolyMesh3D::PolyMesh3D(int theNbVert, int theNbFaces, const GLfloat *const* theVert,
							const GLuint *const* theFace, bool theIsSmooth)
	try	:	GraphicComponent3D(),
			//
			nbVert_(theNbVert),
			nbFaces_(theNbFaces),
			nbTriangles_(0),
			nbQuads_(0),
			nbPolygons_(0),
			smoothPoly_(theIsSmooth),
			vertex_(GlUtils::glfMatrix(0, theNbVert - 1, 0, 2)),
			normalV_(theIsSmooth ? GlUtils::glfMatrix(0, theNbVert-1, 0, 2) : NULL),
			normalF_(GlUtils::glfMatrix(0, theNbFaces-1, 0, 2)),
			face_(new GLuint*[nbFaces_ + 1]),
			triangle_(NULL),
			quad_(NULL),
			polygon_(NULL),
			facesForVertex_(new GLuint*[theNbVert])
{
	try
	{
		init_(theVert, theFace);
	}
	catch (ErrorReport& e)
	{
			e.appendToMessage("called by PolyMesh3D constructor");
			throw e;
	}
}
catch (ErrorReport& e)
{
		e.appendToMessage("called by PolyMesh3D constructor");
		throw e;
}
catch (...)
{
	FAIL_CONDITION( face_ == NULL,
	                kOpenGLNumAllocError, 
	                "Error while allocating array of GL numbers");
	FAIL_CONDITION( facesForVertex_ == NULL,
	                kOpenGLNumAllocError, 
	                "Error while allocating array of GL numbers");
}

PolyMesh3D::PolyMesh3D(const PolyMesh3D& obj)
		:	GraphicComponent3D(),
			//
			nbVert_(0),
			nbFaces_(0),
			nbTriangles_(0),
			nbQuads_(0),
			nbPolygons_(0),
			smoothPoly_(0),
			vertex_(NULL),
			normalV_(NULL),
			normalF_(NULL),
			face_(NULL),
			triangle_(NULL),
			quad_(NULL),
			polygon_(NULL),
			facesForVertex_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "PolyMesh3D copy constructor not implemented.");
}


/*----------------------------------------------------------------------------------+
|	Destructor:		~PolyMesh(void)											|
|																					|
|	Parameters:		None															|
|																					|
|	Description:	Frees all the arrays that were allocated at the creation.		|																|
+----------------------------------------------------------------------------------*/
PolyMesh3D::~PolyMesh3D(void)
{
	int	i;
	
	if (vertex_!=NULL)
	{
		GlUtils::free_glfMatrix(vertex_, 0,  nbVert_-1, 0, 2);
		vertex_ = NULL;
	}

	for (i=0; i< static_cast<int>(nbFaces_); i++)
		delete [] face_[i];
	delete [] face_;
	
	for (i=0; i< static_cast<int>(nbVert_); i++)
		delete [] facesForVertex_[i];
	delete [] facesForVertex_;
	
	if (smoothPoly_)
		if (normalV_!=NULL)
			GlUtils::free_glfMatrix(normalV_, 0,  nbVert_-1, 0, 2);
	
	if (normalF_!=NULL)
		GlUtils::free_glfMatrix(normalF_, 0,  nbFaces_-1, 0, 2);

}

const PolyMesh3D& PolyMesh3D::operator = (const PolyMesh3D& obj)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "PolyMesh3D copy operator not implemented.");

	return *this;
}



//==================================================================================
//	PolyMesh3D public methods
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
|	Description:	Adjusts private variables for proper display of the polyhedron	|
|																					|
+----------------------------------------------------------------------------------*/
void PolyMesh3D::setResolution(ResolutionLevel res)
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
|	Description:	Draws the prism													|
|																					|
+----------------------------------------------------------------------------------*/
void PolyMesh3D::draw(void)
{
	int	i, iQ, iP, k,
			j, j1, j2, j3, j4;
	
	GraphicComponent3D::draw();
	
	if (smoothPoly_)
	{
		//first draw all the triangles
		glBegin(GL_TRIANGLES);
			for (i=0; i< static_cast<int>(nbTriangles_); i++)
			{
				j1 = triangle_[i][1]; j2 = triangle_[i][2]; j3 = triangle_[i][3];
				//
				glNormal3fv(normalV_[j1]);
				glVertex3fv(vertex_[j1]);
				//
				glNormal3fv(normalV_[j2]);
				glVertex3fv(vertex_[j2]);
				//
				glNormal3fv(normalV_[j3]);
				glVertex3fv(vertex_[j3]);
			}
		glEnd();
		
		//	next draw all the quads
		glBegin(GL_QUADS);
			for (iQ=0; iQ< static_cast<int>(nbQuads_); iQ++)
			{
				j1 = quad_[iQ][1]; j2 = quad_[iQ][2]; j3 = quad_[iQ][3]; j4 = quad_[iQ][4];
				//
				glNormal3fv(normalV_[j1]);
				glVertex3fv(vertex_[j1]);
				//
				glNormal3fv(normalV_[j2]);
				glVertex3fv(vertex_[j2]);
				//
				glNormal3fv(normalV_[j3]);
				glVertex3fv(vertex_[j3]);
				//
				glNormal3fv(normalV_[j4]);
				glVertex3fv(vertex_[j4]);
			}
		glEnd();
		
		//	finally draw all the remaining polygons
		for (iP=0; iP< static_cast<int>(nbPolygons_); iP++)
		{
			glBegin(GL_POLYGON);
				for (k=1; k<= static_cast<int>(polygon_[i][0]); k++)
				{
					j = polygon_[i][k];
					//
					glNormal3fv(normalV_[j]);
					glVertex3fv(vertex_[j]);
				}
				
			glEnd();
		}
	}
	else
	{
		//first draw all the triangles
		glBegin(GL_TRIANGLES);
			for (i=0; i< static_cast<int>(nbTriangles_); i++)
			{
				glNormal3fv(normalF_[i]);
				//
				j1 = triangle_[i][1]; j2 = triangle_[i][2]; j3 = triangle_[i][3];
				//
				glVertex3fv(vertex_[j1]);
				glVertex3fv(vertex_[j2]);
				glVertex3fv(vertex_[j3]);
			}
		glEnd();
		
		//	next draw all the quads
		glBegin(GL_QUADS);
			for (iQ=0, i= static_cast<int>(nbTriangles_); iQ< static_cast<int>(nbQuads_); iQ++,i++)
			{
				glNormal3fv(normalF_[i]);
				//
				j1 = quad_[iQ][1]; j2 = quad_[iQ][2]; j3 = quad_[iQ][3]; j4 = quad_[iQ][4];
				//
				glVertex3fv(vertex_[j1]);
				glVertex3fv(vertex_[j2]);
				glVertex3fv(vertex_[j3]);
				glVertex3fv(vertex_[j4]);
			}
		glEnd();
		
		//	finally draw all the remaining polygons
		for (iP=0, i=static_cast<int>(nbTriangles_+nbQuads_); iP<static_cast<int>(nbPolygons_); iP++,i++)
		{
			glBegin(GL_POLYGON);
				glNormal3fv(normalF_[i]);
				//
				for (k=1; k<=static_cast<int>(polygon_[iP][0]); k++)
				{
					j = polygon_[iP][k];
					//
					glVertex3fv(vertex_[j]);
				}
				
			glEnd();
		}
	}
	
}

//==================================================================================
//	PolyMesh3D private methods
//==================================================================================
#if 0
#pragma mark -
#endif


/*----------------------------------------------------------------------------------+
|	Function:	void init(GLfloat** theVert, GLuint** theFace)						|
|																					|
|	Status:			Private															|
|																					|
|	Parameters:		theVert			array of vertices' coordinates					|
|					theFace			array of vertices indexes						|
|																					|
|	Returns:		Nothing															|
|																					|
|	Description:	Initializes the array of vertex and normal coordinates			|
|																					|
+----------------------------------------------------------------------------------*/
void PolyMesh3D::init_(const GLfloat *const* theVert, const GLuint *const* theFace)
{
	int	i, j,
			j1, j2, j3, j4, k,
			iT, iQ, iP, n,
			nF, nF1, nF2, nF3, nF4;
	GLuint	*fCount;
	
	//	fCount will store for each vertex the number of adjacent faces
	fCount = new GLuint [nbVert_];
	
	//	copy the array of vertices
	for (i=0; i<static_cast<int>(nbVert_); i++)
	{
		vertex_[i][0] = theVert[i][0];
		vertex_[i][1] = theVert[i][1];
		vertex_[i][2] = theVert[i][2];
	}
	
	//	For the faces, we would like to re-order the array so that triangles are
	//	listed first, then quads, then general polygons.  This will take 4 passes.
	//
	//	1.	go through the list of faces, count the number of triangles and quads,
	//		and the number of adjacent faces for each vertex;
	nbTriangles_ = nbQuads_ = 0;
	for (i=0; i< static_cast<int>(nbFaces_); i++)
	{
		n = theFace[i][0];	//	nb of vertices of that face_
		switch (n)
		{
			case 3:
				nbTriangles_++;
				break;
			case 4:
				nbQuads_++;
				break;
			default:
				break;
		}
		for (j=1; j<=n; j++)
			fCount[theFace[i][j]]++;
	}
	
	//	2.	Now we can allocate some of the arrays.
	for (j=0; j< static_cast<int>(nbVert_); j++)
	{
		facesForVertex_[j] = new GLuint[fCount[j]];
		facesForVertex_[j][0] = 0;
	}
	//
	triangle_ = face_;
	quad_ = face_ + nbTriangles_;
	polygon_ = face_ + nbTriangles_ + nbQuads_;
	nbPolygons_ = nbFaces_ - nbTriangles_ - nbQuads_;
	//
	for (i=0; i< static_cast<int>(nbTriangles_); i++)
	{
		triangle_[i] = new GLuint[3];
		triangle_[i][0] = 3;
	}
	for (i=0; i< static_cast<int>(nbQuads_); i++)
	{
		quad_[i] = new GLuint[4];
		quad_[i][0] = 4;
	}
	
	//	3.	Second pass through the list of faces.  This time, we update all the 
	//		remaining counters and arrays.
	for (i=0, iT=0, iQ=0, iP=0; i< static_cast<int>(nbFaces_); i++)
	{
		n = theFace[i][0];	//	nb of vertices of that face_
		switch (n)
		{
			case 3:
				j1 = triangle_[iT][1] = theFace[i][1];
				j2 = triangle_[iT][2] = theFace[i][2];
				j3 = triangle_[iT][3] = theFace[i][3];
				nF1 = 1 + facesForVertex_[j1][0]++;
				nF2 = 1 + facesForVertex_[j2][0]++;
				nF3 = 1 + facesForVertex_[j3][0]++;
				facesForVertex_[j1][nF1] = iT;
				facesForVertex_[j2][nF2] = iT;
				facesForVertex_[j3][nF3] = iT;
				iT++;
				break;
			//
			case 4:
				j1 = quad_[iQ][1] = theFace[i][1];
				j2 = quad_[iQ][2] = theFace[i][2];
				j3 = quad_[iQ][3] = theFace[i][3];
				j4 = quad_[iQ][4] = theFace[i][4];
				nF1 = 1 + facesForVertex_[j1][0]++;
				nF2 = 1 + facesForVertex_[j2][0]++;
				nF3 = 1 + facesForVertex_[j3][0]++;
				nF4 = 1 + facesForVertex_[j4][0]++;
				facesForVertex_[j1][nF1] = iQ + nbTriangles_;
				facesForVertex_[j2][nF2] = iQ + nbTriangles_;
				facesForVertex_[j3][nF3] = iQ + nbTriangles_;
				facesForVertex_[j4][nF4] = iQ + nbTriangles_;
				iQ++;
				break;
			//
			default:
				//	first allocate the array of vertex indexes for that face_
				polygon_[iP] = new GLuint[n];
				polygon_[iP][0] = n;
				for (k=1; k<=n; k++)
				{
					j = polygon_[iP][k] = theFace[i][k];
					nF = 1 + facesForVertex_[j][0]++;
					facesForVertex_[j][nF] = iP + nbTriangles_ + nbQuads_;
				}
				iP++;
				break;
		}
	}

	//	4.	Finally we can compute the normal vectors.  We must first compute
	//		face_ normals (whether the polyhedron is smooth or not).
	//	Triangles first
	for (i=0; i< static_cast<int>(nbTriangles_); i++)
		faceExactNormal(vertex_[triangle_[i][1]], vertex_[triangle_[i][2]],
						vertex_[triangle_[i][3]], normalF_[i]);
	//	Then quads
	for (iQ=0, i= static_cast<int>(nbTriangles_); i< static_cast<int>(nbQuads_); iQ++,i++)
		faceAverageNormal(vertex_[quad_[iQ][1]], vertex_[quad_[iQ][2]],
						vertex_[quad_[iQ][3]], vertex_[quad_[i][4]], normalF_[i]);
	//	Finally generic polygons
	for (iP=0, i= static_cast<int>(nbTriangles_+nbQuads_); iP< static_cast<int>(nbPolygons_); iP++,i++)
		faceAverageNormal(polygon_[iP], vertex_, normalF_[i]);
	//
	//	If we want a smooth polyhedron, then we must compute the average normal 
	//	for each vertex.
	if (smoothPoly_)
		for (j=0; j< static_cast<int>(nbVert_); j++)
			for (k=0; k< static_cast<int>(facesForVertex_[j][0]); k++)
			{
				i = facesForVertex_[j][k];
				normalV_[j][0] += normalF_[i][0];
				normalV_[j][1] += normalF_[i][1];
				normalV_[j][2] += normalF_[i][2];
				normalizeVector(normalV_[j]);
			}
	
	delete [] fCount;
}
