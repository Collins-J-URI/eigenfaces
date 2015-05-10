/*  NAME:
        RangeMap.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib RangeMap class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstdlib>
#include <iostream>
#include <cmath>
//
#include "RangeMap.h"

using namespace std;
using namespace uriVL;

#if 0
#pragma mark -
//---------------------------------------------------------------------------
#pragma mark Constructors & destructor
//---------------------------------------------------------------------------
#endif

RangeMap::RangeMap(RangeMap* theMap)
		try	:	RasterImage_gray_F(theMap),
				//
				meshGraph_(NULL),
				triMesh_(NULL),
				nRes_(1),
				nbPts_(0),
				isWireframe_(true),
				startLongitude_((theMap->startLongitude_)*DEGREE_TO_RADIAN),
				stepLongitude_((theMap->stepLongitude_)*DEGREE_TO_RADIAN),
				startLatitude_((theMap->startLatitude_)*DEGREE_TO_RADIAN),
				stepLatitude_((theMap->stepLatitude_)*DEGREE_TO_RADIAN),
				scaleRange_(theMap->scaleRange_),
				cosLong_(new float[theMap->getWidth()]),
				sinLong_(new float[theMap->getWidth()]),
				cosLat_(new float[theMap->getHeight()]),
				sinLat_(new float[theMap->getHeight()]),
				useVertexNormals_(false)
{
	//	I want to copy data from the map to clone
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();
                
	//	first, I allocate my 2D array of vertices --> a trimesh
	allocateDenseMesh_();
				
    for (int i=iLow; i<=iHigh; i++)
        for (int j=jLow; j<=jHigh; j++)
			for (int k=0; k<3; k++)
				triMesh_[i][j][k] = theMap->triMesh_[i][j][k];

}
catch (ErrorReport& e) {
	e.appendToMessage("called by RangeMap constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "invalid RangeMap passed as parameter to RangeMap constructor");
}

RangeMap::RangeMap(RasterImage_gray* rangeMap, float topLatitude, float bottomLatitude, 
					 float leftLongitude, float rightLongitude, float scaleRange)
		try:	RasterImage_gray_F(rangeMap),
				//
				meshGraph_(NULL),
				triMesh_(NULL),
				nRes_(1),
				nbPts_(0),
				isWireframe_(true),
				startLongitude_(leftLongitude*DEGREE_TO_RADIAN),
				stepLongitude_(DEGREE_TO_RADIAN*(rightLongitude - leftLongitude) / (rangeMap->getWidth() - 1)),
				startLatitude_(topLatitude*DEGREE_TO_RADIAN),
				stepLatitude_(DEGREE_TO_RADIAN*(bottomLatitude - topLatitude) / (rangeMap->getHeight() - 1)),
				scaleRange_(1.f),
				cosLong_(new float[rangeMap->getWidth()]),
				sinLong_(new float[rangeMap->getWidth()]),
				cosLat_(new float[rangeMap->getHeight()]),
				sinLat_(new float[rangeMap->getHeight()]),
				useVertexNormals_(false)
{
	//	first, I allocate my 2D array of vertices --> a trimesh
	allocateDenseMesh_();
	
	updateMesh();

}
catch (ErrorReport& e) {
	e.appendToMessage("called by RangeMap constructor");
	throw e;
}
catch (...) {
	FAIL_CONDITION( true,
                    kInvalidParameterError,
                    "invalid RasterImage passed as parameter to RangeMap constructor");
}


RangeMap::RangeMap(ImageRect* theRect, float scaleLongitude, float scaleLatitude, 
					float scaleRange, const float* range)
		try:	RasterImage_gray_F(theRect, range),
				//
				meshGraph_(NULL),
				triMesh_(NULL),
				nRes_(1),
				nbPts_(0),
				isWireframe_(true),
				startLongitude_(0*DEGREE_TO_RADIAN),
				stepLongitude_(scaleLongitude*DEGREE_TO_RADIAN),
				startLatitude_(0*DEGREE_TO_RADIAN),
				stepLatitude_(scaleLatitude*DEGREE_TO_RADIAN),
				scaleRange_(scaleRange),
				cosLong_(NULL),
				sinLong_(NULL),
				cosLat_(NULL),
				sinLat_(NULL),
				useVertexNormals_(false)
{
	//	first, I allocate my 2D array of vertices --> a trimesh
	allocateDenseMesh_();
			
	//	we have no values to push yet
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RangeMap constructor");
	throw e;
}


//	If we only have a dense range map, but not the camera parameters, then we
//	can compute a relative shape of the surface (up to a scale factor)
RangeMap::RangeMap(int theNbRows, int theNbCols, float startElevation, 
				   float elevationStep, float startAzimuth, float azimuthStep)
		try:	meshGraph_(NULL),
				triMesh_(NULL),
				nRes_(1),
				nbPts_(0),
				isWireframe_(true),
				startLongitude_(startElevation*DEGREE_TO_RADIAN),
				stepLongitude_(azimuthStep*DEGREE_TO_RADIAN),
				startLatitude_(startAzimuth*DEGREE_TO_RADIAN),
				stepLatitude_(elevationStep*DEGREE_TO_RADIAN),
				scaleRange_(1.f),
				cosLong_(NULL),
				sinLong_(NULL),
				cosLat_(NULL),
				sinLat_(NULL),
				useVertexNormals_(false)
{
	//	I allocate my 2D array of vertices --> a trimesh
	allocateDenseMesh_();

	//	we have no values to push yet
	
	
}
catch (ErrorReport& e) {
	e.appendToMessage("called by RangeMap constructor");
	throw e;
}


//	If all we have is a set of vertices, then we can triangulate this set of vertices
//	to create a triangular mesh, and use OpenGL to interpolate depth on the
//	triangular faces.
RangeMap::RangeMap(int nbPts, const float** thePts)
		:	RasterImage_gray_F(),
			//
			meshGraph_(NULL),
			triMesh_(NULL),
			nRes_(1),
			nbPts_(0),
			isWireframe_(true),
			startLongitude_(0*DEGREE_TO_RADIAN),
			stepLongitude_(0*DEGREE_TO_RADIAN),
			startLatitude_(0*DEGREE_TO_RADIAN),
			stepLatitude_(0*DEGREE_TO_RADIAN),
			scaleRange_(1.f),
			cosLong_(NULL),
			sinLong_(NULL),
			cosLat_(NULL),
			sinLat_(NULL),
			useVertexNormals_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "sparse RangeMap constructor not implemented yet");

}

//	If all we have is a set of vertices, then we can triangulate this set of vertices
//	to create a triangular mesh, and use OpenGL to interpolate depth on the
//	triangular faces.
RangeMap::RangeMap(int nbPts, const Point3D** thePoint)
		:	RasterImage_gray_F(),
			//
			meshGraph_(NULL),
			triMesh_(NULL),
			nRes_(1),
			nbPts_(0),
			isWireframe_(true),
			startLongitude_(1*DEGREE_TO_RADIAN),
			stepLongitude_(1*DEGREE_TO_RADIAN),
			startLatitude_(1*DEGREE_TO_RADIAN),
			stepLatitude_(1*DEGREE_TO_RADIAN),
			scaleRange_(1),
			cosLong_(NULL),
			sinLong_(NULL),
			cosLat_(NULL),
			sinLat_(NULL),
			useVertexNormals_(false)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "sparse RangeMap constructor not implemented yet");
}


RangeMap::RangeMap(const RangeMap& obj)
		:	RasterImage_gray_F(),
			//
			meshGraph_(NULL),
			triMesh_(NULL),
			nRes_(1),
			nbPts_(0),
			isWireframe_(true),
			startLongitude_(0),
			stepLongitude_(0),
			startLatitude_(0),
			stepLatitude_(0),
			scaleRange_(0),
			cosLong_(NULL),
			sinLong_(NULL),
			cosLat_(NULL),
			sinLat_(NULL),
			useVertexNormals_(false)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"RangeMap copy constructor not implemented");
}


RangeMap::~RangeMap(void)
{
	int	nbRows = getHeight();
// 			nbCols = getWidth();   // unused var. -CJC
			
	for (int i=0; i<nbRows; i++)
		delete []triMesh_[i];

    delete []triMesh_;
	delete []cosLong_;
	delete []sinLong_;
	delete []cosLat_;
	delete []sinLat_;
}

const RangeMap& RangeMap::operator = (const RangeMap& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"RangeMap copy operator not implemented");
	return *this;
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------
//	Mesh calculations
//---------------------------------------------------------------------------

void RangeMap::updateMesh(void)
{
	//	If the range raster is the reference we recalculate the mesh
	DrawableInformationType dType = getDrawableInformationType();
	if ((dType == kDense) || (dType == kDenseUncertain))
	{
		const ImageRect* vRect = getValidRect();
		const int iLow  = vRect->getTop(),
				  iHigh = vRect->getBottom(),
				  jLow = vRect->getLeft(),
				  jHigh = vRect->getRight();
				  
    	const float	*const* range = getShiftedGrayF2D();

		for (int i=iLow; i<=iHigh; i++)
		{	
			for (int j=jLow; j<=jHigh; j++)
			{
				float	r = scaleRange_*range[i][j];
				
				//	X
				triMesh_[i][j][0] = r * cosLong_[j] * cosLat_[i];

				//	Y
				triMesh_[i][j][1] = r * sinLat_[i];

				//	Z (z is in the horizontal direction)
				triMesh_[i][j][2] = -r * sinLong_[j] * cosLat_[i];

//dynamic array				
//cout << "(" << i << ", " << j << "), r = " << r << " --> { " << cosLat_[i] << ", " << sinLat_[i] << ", " << 
//		cosLong_[j] << ", " << sinLong_[j] << "} --> [" << triMesh_[i][j][0] 
//				<< ", " << triMesh_[i][j][1] << ", " << triMesh_[i][j][2] << "]" << endl;
			
			}
		}
	}
	//	else I don't know yet what to do
	else
	{
		FAIL_CONDITION(	true,
						kFunctionNotImplemented,
						"Not implemented yet");
	}
}

void RangeMap::allocateDenseMesh_(void)
{
	const ImageRect* vRect = getValidRect();
    const int	nbRows = vRect->getHeight(),
				nbCols = vRect->getWidth();
    const int	iLow  = vRect->getTop(),
				iHigh = vRect->getBottom(),
				jLow = vRect->getLeft(),
				jHigh = vRect->getRight();

	triMesh_ = new float**[nbRows];
	for (int i=0; i<nbRows; i++)
	{
		triMesh_[i] = new float*[nbCols];
		for (int j=0; j<nbCols; j++)
			triMesh_[i][j] = new float[3];
	}

	float longit = startLongitude_;
	for (int j=jLow; j<=jHigh; j++, longit += stepLongitude_)
	{
		cosLong_[j] = cosf(longit);
		sinLong_[j] = sinf(longit);
	}
	
	float lat = startLatitude_;
	for (int i=iLow; i<=iHigh; i++, lat += stepLatitude_)
	{
		cosLat_[i] = cosf(lat);
		sinLat_[i] = sinf(lat);
	}
}


#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------
//	Rendering methods
//---------------------------------------------------------------------------

void RangeMap::draw(void)
{
	switch(getDrawableInformationType())
	{
		case kDense:
			if (isWireframe_ || !useVertexNormals_)
				drawDenseMesh_();
			else
				drawDenseMeshSmooth_();
			break;
			
		case kSparse:
			if (isWireframe_ || !useVertexNormals_)
				drawSparseMesh_();
			else
				drawSparseMeshSmooth_();
			break;
			
		default:
			break;
		
	}
}

void RangeMap::drawDenseMesh_(void)
{
	const int	nbRows = getHeight(),
			    nbCols = getWidth();

	if (isWireframe_) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(ambientMat_[0], ambientMat_[1], ambientMat_[2]);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        setReflectionParameters_( );
    }

	for (int i=0; i<nbRows-1; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int j=0; j<nbCols; j++)
		{
			glVertex3fv(triMesh_[i+1][j]);
			glVertex3fv(triMesh_[i][j]);
		}
		glEnd();
	}
}


void RangeMap::drawDenseMeshSmooth_(void)
{
	
	glBegin(GL_TRIANGLE_STRIP);
		
	glEnd();
}

void RangeMap::drawSparseMesh_(void)
{
}

void RangeMap::drawSparseMeshSmooth_(void)
{
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------
//	Configuration access methods
//---------------------------------------------------------------------------

//	Sets the desired resolution (as in "draw ever n points").  This method
//	has no effect on a sparse depth map
void RangeMap::setResolution(ResolutionLevel res)
{
    switch (res)
    {
        case kVeryHighResolution:
            nRes_ = 1;
            break;
        case kFineResolution:
            nRes_ = 4;
            break;
        case kMediumResolution:
            nRes_ = 10;
            break;
        case kCoarseResolution:
            nRes_ = 25;
            break;
        case kVeryLowResolution:
            nRes_ = 50;
            break;
    }
}

//	Sets wireframe/filled rendering state
void RangeMap::setWireframe(bool wfState)
{
	isWireframe_ = wfState;
}

