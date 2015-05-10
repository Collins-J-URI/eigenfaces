/*  NAME:
        ElevationMap.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib ElevationMap class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <cstdlib>
//
#include "ElevationMap.h"

using namespace uriVL;

#if 0
#pragma mark -
//---------------------------------------------------------------------------
#pragma mark Constructors & destructor
//---------------------------------------------------------------------------
#endif

ElevationMap::ElevationMap(ElevationMap* theMap)
	try	:	RasterImage_gray_F(theMap),
			GraphicComponent3D(),
			//
			nRes_(theMap->nRes_),
			nbPts_(theMap->nbPts_),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleX_(theMap->scaleX_),
			scaleY_(theMap->scaleY_),
			scaleZ_(theMap->scaleZ_)				
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
	e.appendToMessage("called by ElevationMap constructor");
	throw e;
}

ElevationMap::ElevationMap(RasterImage_gray* elevationMap, 
							float theScaleX, float theScaleY, float theScaleZ)
	try	:	RasterImage_gray_F(elevationMap),
			GraphicComponent3D(),
			//
			nRes_(1),
			nbPts_(elevationMap->getWidth()*elevationMap->getHeight()),
			meshGraph_(NULL),
			isWireframe_(true),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleX_(theScaleX),
			scaleY_(theScaleY),
			scaleZ_(theScaleZ)				
{
	//	first, I allocate my 2D array of vertices --> a trimesh
	allocateDenseMesh_();
	
	updateMesh();
	
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ElevationMap constructor");
	throw e;
}


//	If we have a dense elevation map and camera parameters, then we can compute
//	the "exact" shape of the surface (in the correct world units)
ElevationMap::ElevationMap(const ImageRect* theRect, const float* elevation, 
							float theScaleX, float theScaleY, float theScaleZ)
	try	:	RasterImage_gray_F(theRect, elevation),
			GraphicComponent3D(),
			//
			nRes_(1),
			nbPts_(theRect->getWidth()*theRect->getHeight()),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleX_(theScaleX),
			scaleY_(theScaleY),
			scaleZ_(theScaleZ)				
{
	FAIL_CONDITION(	elevation == NULL,
					kNullParameterError,
					"NULL depth raster passed to ElevationMap constructor");

//	int		iLow  = theRect->getTop(),
//			jLow = theRect->getLeft(),
//			nbRows = getHeight(),
//			nbCols = getWidth();
    
	//	first, I allocate my 2D array of vertices --> a trimesh
	allocateDenseMesh_();

	updateMesh();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ElevationMap constructor");
	throw e;
}

//	If we only have a dense elevation map, but not the camera parameters, then we
//	can compute a relative shape of the surface (up to a scale factor)
ElevationMap::ElevationMap(int theNbRows, int theNbCols, const float* elevation, 
							float theScaleX, float theScaleY, float theScaleZ)
	try	:	RasterImage_gray_F(theNbRows, theNbCols, elevation),
			GraphicComponent3D(),
			//
			nRes_(1),
			nbPts_(theNbRows*theNbCols),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleX_(theScaleX),
			scaleY_(theScaleY),
			scaleZ_(theScaleZ)				
{
	//	should be caught by parent constructor
	FAIL_CONDITION(	elevation == NULL,
					kNullParameterError,
					"NULL depth raster passed to ElevationMap constructor");
					
	//	first, I allocate my 2D array of vertices --> a trimesh
	allocateDenseMesh_();
			
	updateMesh();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ElevationMap constructor");
	throw e;
}

//	If we only have a dense elevation map, but not the camera parameters, then we
//	can compute a relative shape of the surface (up to a scale factor)
ElevationMap::ElevationMap(int theNbRows, int theNbCols, 
						   float theScaleX, float theScaleY, float theScaleZ)
	try	:	RasterImage_gray_F(theNbRows, theNbCols),
			GraphicComponent3D(),
			//
			nRes_(1),
			nbPts_(theNbRows*theNbCols),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleX_(theScaleX),
			scaleY_(theScaleY),
			scaleZ_(theScaleZ)				
{
	//	first, I allocate my 2D array of vertices --> a trimesh
	allocateDenseMesh_();
			
	//	we have no values to push yet

}
catch (ErrorReport& e) {
	e.appendToMessage("called by ElevationMap constructor");
	throw e;
}


//	If all we have is a set of vertices, then we can triangulate this set of vertices
//	to create a triangular mesh, and use OpenGL to interpolate depth on the
//	triangular faces.
ElevationMap::ElevationMap(int nbPts, const float*const* thePt)
	try	:	RasterImage_gray_F(),
			GraphicComponent3D(),
			//
			nRes_(1),
			nbPts_(0),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleX_(1.f),
			scaleY_(1.f),
			scaleZ_(1.f)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "sparse ElevationMap constructor not implemented yet");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ElevationMap constructor");
	throw e;
}

//	If all we have is a set of vertices, then we can triangulate this set of vertices
//	to create a triangular mesh, and use OpenGL to interpolate depth on the
//	triangular faces.
ElevationMap::ElevationMap(int nbPts, const Point3D*const* thePoint)
	try	:	RasterImage_gray_F(),
			GraphicComponent3D(),
			//
			nRes_(1),
			nbPts_(0),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleX_(1.f),
			scaleY_(1.f),
			scaleZ_(1.f)				
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "sparse ElevationMap constructor not implemented yet");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ElevationMap constructor");
	throw e;
}

ElevationMap::ElevationMap(const ElevationMap& obj)
	try	:	RasterImage_gray_F(),
			GraphicComponent3D(),
			//
			nRes_(1),
			nbPts_(0),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleX_(1.f),
			scaleY_(1.f),
			scaleZ_(1.f)				
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"StereoPair copy constructor not implemented");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by ElevationMap constructor");
	throw e;
}


ElevationMap::~ElevationMap(void)
{
	int	nbRows = getHeight();
// 			nbCols = getWidth();   // unused var. -CJC
			
	for (int i=0; i<nbRows; i++)
		delete []triMesh_[i];
    delete []triMesh_;
}

const ElevationMap& ElevationMap::operator = (const ElevationMap& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"ElevationMap copy operator not implemented");
	return *this;
}


#if 0
#pragma mark -
//---------------------------------------------------------------------------
#pragma mark Mesh calculations
//---------------------------------------------------------------------------
#endif


void ElevationMap::updateMesh(void)
{
	//	If the elevation raster is the reference we recalculate the mesh
	DrawableInformationType dType = getDrawableInformationType();
	if ((dType == kDense) || (dType == kDenseUncertain))
	{
		const ImageRect* vRect = getValidRect();
		const int	iLow = vRect->getTop(),
					jLow = vRect->getLeft(),
					nbRows = getHeight(),
					nbCols = getWidth();
					
		const float* elevation = getGrayF();
					
		//	Then we compute the XYZ coordinates and store them in the triMesh array
		long ptIndex = 0L;
		for (int i=0, iSh = iLow; i<nbRows; i++, iSh++)
			for (int j=0, jSh=jLow; j<nbCols; j++, jSh++)
			{
				triMesh_[i][j][0] = scaleX_ * jSh;
				triMesh_[i][j][1] = scaleY_ * iSh;
				triMesh_[i][j][2] = scaleZ_ * elevation[ptIndex++];
			}
	}
	//	else I don't know yet what to do
	else
	{
	}
}

void ElevationMap::allocateDenseMesh_(void)
{
    const int	nbRows = getHeight(),
				nbCols = getWidth();

	triMesh_ = new float**[nbRows];
	for (int i=0; i<nbRows; i++)
	{
		triMesh_[i] = new float*[nbCols];
		for (int j=0; j<nbCols; j++)
			triMesh_[i][j] = new float[3];
	}
}

#if 0
#pragma mark -
//---------------------------------------------------------------------------
#pragma mark Rendering methods
//---------------------------------------------------------------------------
#endif

void ElevationMap::draw(void)
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


void ElevationMap::drawDenseMesh_(void)
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
	
    setReflectionParameters_( );

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


void ElevationMap::drawDenseMeshSmooth_(void)
{
	
	glBegin(GL_TRIANGLE_STRIP);
		
	glEnd();
}

void ElevationMap::drawSparseMesh_(void)
{
}

void ElevationMap::drawSparseMeshSmooth_(void)
{
}

#if 0
#pragma mark -
//---------------------------------------------------------------------------
#pragma mark Configuration access methods
//---------------------------------------------------------------------------
#endif


//	Sets the desired resolution (as in "draw ever n points").  This method
//	has no effect on a sparse depth map
void ElevationMap::setResolution(ResolutionLevel res)
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
void ElevationMap::setWireframe(bool wfState)
{
	isWireframe_ = wfState;
}

