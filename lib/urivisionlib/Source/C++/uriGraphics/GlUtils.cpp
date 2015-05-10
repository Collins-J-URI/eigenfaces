/*  NAME:
        GlUtils.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib GlUtils utility class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#include "GlUtils.h"

using namespace uriVL;


/*----------------------------------------------------------------------------------+
| Allocates a GLfloat vector with range [nl..nh]									|																		| 							               			        				
+----------------------------------------------------------------------------------*/
GLfloat* GlUtils::glfVector(int nl,int nh)
{
	GLfloat* v;

	v = new GLfloat[nh-nl+1];
	FAIL_CONDITION( v == NULL,
	                kOpenGLNumAllocError,
	                "allocation failure in glfVector");
	return v-nl;
}

/*----------------------------------------------------------------------------------+
| Allocates a GLdouble vector with range [nl..nh]									|																		| 							               			        				
+----------------------------------------------------------------------------------*/
GLdouble* GlUtils::gldVector(int nl,int nh)
{
	GLdouble* v;

	v = new GLdouble[nh-nl+1];
	FAIL_CONDITION( v == NULL,
	                kOpenGLNumAllocError,
	                "allocation failure in gldVector");
	return v-nl;
}

/*----------------------------------------------------------------------------------+
| Allocates a GLuint vector with range [nl..nh]										|																		| 							               			        				
+----------------------------------------------------------------------------------*/
GLuint* GlUtils::gluiVector(int nl, int nh)
{
	GLuint	*v;

	v = new GLuint[nh-nl+1];
	FAIL_CONDITION( v == NULL,
	                kOpenGLNumAllocError,
	                "allocation failure in gluiVector");
	return v-nl;
}

/*----------------------------------------------------------------------------------+
| Allocates a GLint vector with range [nl..nh]										|																		| 							               			        				
+----------------------------------------------------------------------------------*/
GLint* GlUtils::gliVector(int nl, int nh)
{
	GLint	*v;

	v = new GLint[nh-nl+1];
	FAIL_CONDITION( v == NULL,
	                kOpenGLNumAllocError,
	                "allocation failure in gliVector");
	return v-nl;
}

/*----------------------------------------------------------------------------------+
| Allocates a GLfloat matrix with range [nrl..nrh][ncl..nch]						|																		| 							               			        				
+----------------------------------------------------------------------------------*/
GLfloat** GlUtils::glfMatrix(int nrl, int nrh, int ncl, int nch)
{
	int	i;
	GLfloat** m;

	m = new GLfloat*[nrh-nrl+1];
	FAIL_CONDITION( m == NULL,
	                kOpenGLNumAllocError,
	                "level 1 allocation failure in glfMatrix");
	m -= nrl;

	for(i=nrl;i<=nrh;i++)
	{
		m[i]= new GLfloat[nch-ncl+1];
    	FAIL_CONDITION( m[i] == NULL,
    	                kOpenGLNumAllocError,
    	                "level 2 allocation failure in glfMatrix");
		m[i] -= ncl;
	}
	return m;
}

/*----------------------------------------------------------------------------------+
| Allocates a GLdouble matrix with range [nrl..nrh][ncl..nch]						|																		| 							               			        				
+----------------------------------------------------------------------------------*/
GLdouble** GlUtils::gldMatrix(int nrl, int nrh, int ncl, int nch)
{
	int	 i;
	GLdouble** m;

	m = new GLdouble*[nrh-nrl+1];
	FAIL_CONDITION( m == NULL,
	                kOpenGLNumAllocError,
	                "level 1 allocation failure in gldMatrix");
	m -= nrl;

	for(i=nrl;i<=nrh;i++)
	{
		m[i]= new GLdouble[nch-ncl+1];
    	FAIL_CONDITION( m[i] == NULL,
    	                kOpenGLNumAllocError,
    	                "level 2 allocation failure in gldMatrix");
		m[i] -= ncl;
	}
	return m;
}


/*----------------------------------------------------------------------------------+
| Frees a GLfloat vector allocated by glfvector()									|																		| 							               			        				
+----------------------------------------------------------------------------------*/
void GlUtils::free_glfVector(GLfloat* v, int nl, int nh)
{
	if ( v!= NULL)
	{
		v += nl;
		delete []v;
	}
}

/*----------------------------------------------------------------------------------+
| Frees a GLdouble vector allocated by glfvector()									|																		| 							               			        				
+----------------------------------------------------------------------------------*/
void GlUtils::free_gldVector(GLdouble* v, int nl, int nh)
{
	if ( v!= NULL)
	{
		v += nl;
		delete []v;
	}
}

/*----------------------------------------------------------------------------------+
| Frees a GLuint vector allocated by gluiVector()									|																		| 							               			        				
+----------------------------------------------------------------------------------*/
void GlUtils::free_gluiVector(GLuint* v, int nl, int nh)
{
	if ( v!= NULL)
	{
		v += nl;
		delete [] v;
	}
}

/*----------------------------------------------------------------------------------+
| Frees a GLint vector allocated by gliVector()										|																		| 							               			        				
+----------------------------------------------------------------------------------*/
void GlUtils::free_gliVector(GLint* v, int nl, int nh)
{
	if ( v!= NULL)
	{
		v += nl;
		delete [] v;
	}
}

/*----------------------------------------------------------------------------------+
| Frees a GLfloat matrix allocated by glfMatrix()									|																		| 							               			        				
+----------------------------------------------------------------------------------*/
void GlUtils::free_glfMatrix(GLfloat** m, int nrl, int nrh, int ncl,  int nch)
{
	int	i;

	if ( m!= NULL)
	{
		for(i=nrh;i>=nrl;i--)
		{
			m[i] += ncl;
			delete [] m[i];
		}
		m += nrl;
		delete [] m;
	}
}

/*----------------------------------------------------------------------------------+
| Frees a GLfloat matrix allocated by glfMatrix()									|																		| 							               			        				
+----------------------------------------------------------------------------------*/
void GlUtils::free_gldMatrix(GLdouble** m, int nrl, int nrh, int ncl,  int nch)
{
	int	i;

	if ( m!= NULL)
	{
		for(i=nrh;i>=nrl;i--)
		{
			m[i] += ncl;
			delete [] m[i];
		}
		m += nrl;
		delete [] m;
	}
}
