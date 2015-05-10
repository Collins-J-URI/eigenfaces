/*  NAME:
        DepthMap.h
 
    DESCRIPTION:
        DepthMap public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_DEPTH_MAP_H
#define URIVL_DEPTH_MAP_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImagePoint_F.h"
#include "RasterImage_gray_F.h"
#include "IntrinsicParameters_pinhole.h"
#include "Point3D.h"
#include "GraphicComponent3D.h"


namespace uriVL
{
    /** depth map class
     *  A depth map is a form of raster image that instead of storing an image itensity
     *  at each image point stores a depth value.  A DepthMap object can be rendered as
     *  a raster image object or drawn as a 3D mesh.
     *
     *	A DepthMap object stores an elevation map:
     *	<ul>
     *		<li> for a dense depth map, as a 2D raster Z[Y, X] (the row index of a raster
     *				corresponds to the Y coordinate, the column corresponds to X);
     *		<li> for a sparse depth map, it is assumed that the 3D points form an elevation
     *				surface along the Z direction
  	 *	</ul>
  	 *
  	 *	The depth map (for me at least) is pretty much indisossiable from the notion of
  	 *	perspective projection.  Therefore, all constructors for this class require an 
  	 *	<code>IntrinsicParameters_pinhole</code> object (which will provide a focal
	 *	length and scaling parameters for the inverse projection.  If the camera used in
	 *	orthographic, then the raster does not really store a depth map but
	 *	an elevation map.
	 *
	 *	So far the only way to create a <code>DepthMap</code>, <code>RangeMap</code>, or
	 *  <code>ElevationMap</code> object is to send it either a raster or a list of 
	 *	points to initialize it with.  In other words, there is no "create now and
	 *	allocate/initialize later" constructor.
	 *
	 *	@see	ElevationMap
	 *	@see	RangeMap
     */
	class URIVL_EXPORT DepthMap : public RasterImage_gray_F,
	                              public GraphicComponent3D
	{
		public:

			/**	Cloner constructor
			 *
			 *	@param	theMap		depth map to clone
			 */
			DepthMap(DepthMap* theMap);

			/**	Transforms a range raster into a depth map
			 *
			 *	@param	camParams	intrinsic parameters of the camera
			 *	@param	theScaleZ	scale to apply to Z value stored in the raster
			 *	@param	range		range raster image (gives depth map dimensions)
			 */
			DepthMap(RasterImage_gray* range, IntrinsicParameters_pinhole* camParams, float theScaleZ);

			/**	Transforms a range raster into a dense depth map.
			 *	@param	depth	range raster image
			 *	@param	camParams	intrinsic parameters of the camera
			 *	@param	theScaleZ	scale to apply to Z value stored in the raster
			 */
			DepthMap(const float* depth, const IntrinsicParameters_pinhole*camParams, float theScaleZ);
			
			/*	Creates a sparse depth map from a list of point coordinates
			 *
			 *	@param	nbPts	number of points
			 *	@param	thePt	list of point coordinates
			 */
			DepthMap(int nbPts, const float** thePt);

			/*	Creates a sparse depth map from a list of 3D point
			 *
			 *	@param	nbPts	number of points
			 *	@param	thePt	list of 3D points
			 */
			DepthMap(int nbPts, const Point3D*const* thePoint);
			

			/**	Destructor
			 */
			~DepthMap(void);

			
			/**	Recalculate the coordinates of the mesh points following an update of the
			 *	base raster or of the reference 3D points
			 */
			void updateMesh(void);
			

			/**	Renders the depth map as a meshed surface
			 */
			void draw(void);
			
			/**	Sets the resolution for the rendering of the mesh.
			 *
			 *	Calling this function only makes sense for a dense (raster) map.
			 *
			 *	@param	res		resolution level for rendering the mesh
			 */
			void setResolution(ResolutionLevel res);

			/**	Indicates whether or not the depth map should be rendered in wireframe.
			 *
			 *	@param	wfState		true if the depth map should be rendered in wireframe
			 *
			 *	@see	isWireframe_
			 */
			void setWireframe(bool wfState);
			
			
			/**	Indicates whether or not the depth map should be rendered using vertex 
			 *	normals for a "smooth" result.
			 *
			 *	@param	theUseNormals	true if the depth map should be rendered using
			 *								vertex normals
			 *
			 *	@see	useVertexNormals_
			 */
			void setUseVertexNormals(bool theUseNormals);
			
			
		private:
		
		    /** Origin of the depth map.  We set the origin at the center of the
		     *  map's "bottom"
		     */
		    Point3D     *origin_;
		    
			/**	Rendering resolution for a dense depth map
			 *
			 *	@see	setResolution
			 */
			int		nRes_;
			
			/**	Number of points of a sparse depth map
			 */
			int		nbPts_;
			
			/**	Stores the graph of a sparse map
			 */
			int		**meshGraph_;

			/**	Should the elevation map be rendered in wireframe?  
			 *	Default is <code>true</code>.
			 *
			 *
			 *	@see	setWireframe
			 */								
			bool		isWireframe_;

			/**	Triangular mesh of a dense map
			 */
			float		***triMesh_;

			/**	Indicates whether or not the elevation map should be rendered using vertex 
			 *	normals for a "smooth" result. Default is <code>false</code>.
			 *
			 *	@see	setUseVertexNormals
			 */
			bool useVertexNormals_;
			
			/**	scale to apply to Z value stored in the raster (in m)
			 */
			float	scaleZ_;

			/**	Stores the distortion parameters of the camera that produced a dense depth
			 *	map.
			 */
			IntrinsicParameters_pinhole		*cameraParams_;
			
           /**
             *  Copy Constructor. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            DepthMap(const DepthMap& theObj);

            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const DepthMap& operator = (const DepthMap& theObj);

			/**	Allocates the coordinate array of the dense trimesh
			 */
			void allocateDenseMesh_(void);
			
			/**	Draws the triangular mesh of a dense map
			 */
			void drawDenseMesh_(void);

			/**	Draws the smoothed triangular mesh of a dense map
			 */
			void drawDenseMeshSmooth_(void);

			/**	Draws the mesh of a sparse map
			 */
			void drawSparseMesh_(void);

			/**	Draws the smoothed mesh of a sparse map
			 */
			void drawSparseMeshSmooth_(void);


	};
}

#endif  //  DEPTH_MAP_H
