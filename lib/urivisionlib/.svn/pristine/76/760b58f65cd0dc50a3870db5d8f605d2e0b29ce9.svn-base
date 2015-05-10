/*  NAME:
        DepthMap.cpp
 
    DESCRIPTION:
        implementation of the uriVisionLib DepthMap class
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#include <iostream>
#include <cstdlib>
//
#include "DepthMap.h"

using namespace std;
using namespace uriVL;


DepthMap::DepthMap(DepthMap* theMap)
	try	:	RasterImage_gray_F(theMap),
			GraphicComponent3D(),
			//
			origin_(new Point3D(theMap->origin_)),
			nRes_(theMap->nRes_),
			nbPts_(theMap->nbPts_),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleZ_(theMap->scaleZ_),
			cameraParams_(new IntrinsicParameters_pinhole(theMap->cameraParams_))				
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
					
	setFloatConversionMode(kPositiveFloat);
}
catch (ErrorReport& e) {
	e.appendToMessage("called by DepthMap constructor");
	throw e;
}


DepthMap::DepthMap(RasterImage_gray* depth, IntrinsicParameters_pinhole* camParams, 
					float theScaleZ)
	try	:	RasterImage_gray_F(depth),
			GraphicComponent3D(),
			//
			origin_(new Point3D(0.f, 0.f, 0.f)),
			nRes_(1),
			nbPts_(0),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleZ_(theScaleZ),
			cameraParams_(new IntrinsicParameters_pinhole(camParams))
{
	FAIL_CONDITION(	camParams == NULL,
					kNullParameterError,
					"NULL camera parameters object received in DepthMap constructor");

	setFloatConversionMode(kPositiveFloat);

	//	Store a clone of the camera parameters
	cameraParams_ = new IntrinsicParameters_pinhole(camParams);
	
    //  set arbitrary rendering parameters
    useVertexNormals_ = false;

	//	first, I allocate my 2D array of vertices --> a trimesh
	allocateDenseMesh_();

	//	then we calculate the points' coordinates				
	scaleZ_ = theScaleZ;
	origin_ = NULL;
	updateMesh();
}
catch (ErrorReport& e) {
	e.appendToMessage("called by DepthMap constructor");
	throw e;
}


DepthMap::DepthMap(const float* depth, const IntrinsicParameters_pinhole* camParams, float theScaleZ)
	try	:	RasterImage_gray_F(),
			GraphicComponent3D(),
			//
			origin_(new Point3D(0.f, 0.f, 0.f)),
			nRes_(1),
			nbPts_(0),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleZ_(theScaleZ),
			cameraParams_(new IntrinsicParameters_pinhole(camParams))
{
	FAIL_CONDITION(	camParams == NULL,
					kNullParameterError,
					"NULL camera parameters object received in DepthMap constructor");
	FAIL_CONDITION(	camParams == NULL,
					kNullRasterError,
					"NULL depth map raster received in DepthMap constructor");

	//	Store a clone of the camera parameters
	cameraParams_ = new IntrinsicParameters_pinhole(camParams);
	
	setFloatConversionMode(kPositiveFloat);

    //  set arbitrary rendering parameters
    useVertexNormals_ = false;

	scaleZ_ = theScaleZ;

    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "DepthMap constructor not implemented yet");
}
catch (ErrorReport& e) {
	e.appendToMessage("called by DepthMap constructor");
	throw e;
}


//	If all we have is a set of vertices, then we can triangulate this set of vertices
//	to create a triangular mesh, and use OpenGL to interpolate depth on the
//	triangular faces.
DepthMap::DepthMap(int nbPts, const float** thePts)
		:	RasterImage_gray_F(),
			GraphicComponent3D(),
			//
			origin_(NULL),
			nRes_(1),
			nbPts_(nbPts),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleZ_(1.f),
			cameraParams_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "sparse DepthMap constructor not implemented yet");

}

//	If all we have is a set of vertices, then we can triangulate this set of vertices
//	to create a triangular mesh, and use OpenGL to interpolate depth on the
//	triangular faces.
DepthMap::DepthMap(int nbPts, const Point3D*const* thePoint)
		:	RasterImage_gray_F(),
			GraphicComponent3D(),
			//
			origin_(NULL),
			nRes_(1),
			nbPts_(nbPts),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleZ_(1.f),
			cameraParams_(NULL)
{
    FAIL_CONDITION( true,
                    kFunctionNotImplemented,
                    "sparse DepthMap constructor not implemented yet");
}


DepthMap::DepthMap(const DepthMap& obj)
		:	RasterImage_gray_F(),
			GraphicComponent3D(),
			//
			origin_(NULL),
			nRes_(1),
			nbPts_(0),
			meshGraph_(NULL),
			isWireframe_(false),
			triMesh_(NULL),
			useVertexNormals_(false),
			scaleZ_(1.f),
			cameraParams_(NULL)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"DepthMap copy constructor not implemented");
}


DepthMap::~DepthMap(void)
{
	const int	nbRows = getHeight();
			
	for (int i=0; i<nbRows; i++)
		delete []triMesh_[i];
    delete []triMesh_;
}


const DepthMap& DepthMap::operator = (const DepthMap& obj)
{
	FAIL_CONDITION( true,
					kFunctionNotImplemented,
					"DepthMap copy operator not implemented");
	return *this;
}

#if 0
#pragma mark -
#endif

//---------------------------------------------------------------------------
//	Mesh calculations
//---------------------------------------------------------------------------

void DepthMap::updateMesh(void)
{
	//	If the depth raster is the reference we recalculate the mesh
	DrawableInformationType dType = getDrawableInformationType();
	if ((dType == kDense) || (dType == kDenseUncertain))
	{
		const ImageRect* vRect = getValidRect();
		const int iLow  = vRect->getTop(),
				  iHigh = vRect->getBottom(),
				  jLow = vRect->getLeft(),
				  jHigh = vRect->getRight();
    	const float	*const* depth = getShiftedGrayF2D();
		const double fx = cameraParams_->getFocalLengthX();
		const double fy = cameraParams_->getFocalLengthY();
		double x, y;
		float	Z;
		
		//  First pass: determine the origin of the depth map.
		//  We place it a the middle of the map's "bottom"
		float bottomZ = 0.f;
        for (int i=iLow; i<=iHigh; i++)
            for (int j=jLow; j<=jHigh; j++)
            {
				//	If there is a distortion function, it is applied here
				cameraParams_->pixelToMetric(j, i, &x, &y);

            	Z = -scaleZ_ * depth[i][j];
            	if (Z < bottomZ)
            	    bottomZ = Z;

            	triMesh_[i][j][0] = static_cast<float>(-x * Z / fx);
            	triMesh_[i][j][1] = static_cast<float>(y * Z / fy);
            	triMesh_[i][j][2] = static_cast<float>(Z);
			}		

        //  We can now produce the origin's coordinates
        //  For this we need the inverse projection of the image's center
		cameraParams_->pixelToMetric((jLow + jHigh)/2, (iLow + iHigh)/2, &x, &y);
        float   origX = static_cast<float>(-x * bottomZ / fx),
                origY = static_cast<float>(-y * bottomZ / fy);
        if (origin_ != NULL)
            delete origin_;
        origin_ = new Point3D(origX, origY, bottomZ);
        
        //  Second pass, we transform the mesh's points' cooordinates to be 
        //  relative to the mesh's origin    		
        for (int i=iLow; i<=iHigh; i++)
            for (int j=jLow; j<=jHigh; j++)
            {
            	triMesh_[i][j][0] -= origX;
            	triMesh_[i][j][1] -= origY;
            	triMesh_[i][j][2] -= bottomZ;
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

void DepthMap::allocateDenseMesh_(void)
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
#endif

//---------------------------------------------------------------------------
//	Rendering methods
//---------------------------------------------------------------------------

void DepthMap::draw(void)
{
	glPushMatrix();
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
	glPopMatrix();
}

void DepthMap::drawDenseMesh_(void)
{
	const ImageRect* vRect = getValidRect();
    const int iLow  = vRect->getTop(),
              iHigh = vRect->getBottom(),
              jLow = vRect->getLeft(),
              jHigh = vRect->getRight();

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
    

    for (int i=iLow; i<=iHigh-nRes_; i+= nRes_)
	{
		glBegin(GL_TRIANGLE_STRIP);
        for (int j=jLow; j<=jHigh; j+= nRes_)
		{
			glVertex3fv(triMesh_[i+nRes_][j]);
			glVertex3fv(triMesh_[i][j]);
		}
		glEnd();

	}
}


void DepthMap::drawDenseMeshSmooth_(void)
{
/*	
	glBegin(GL_TRIANGLE_STRIP);
		
	glEnd();
*/
}

void DepthMap::drawSparseMesh_(void)
{
}

void DepthMap::drawSparseMeshSmooth_(void)
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
void DepthMap::setResolution(ResolutionLevel res)
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
void DepthMap::setWireframe(bool wfState)
{
	isWireframe_ = wfState;
}

