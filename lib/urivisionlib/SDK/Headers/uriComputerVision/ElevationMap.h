/*  NAME:
        ElevationMap.h
 
    DESCRIPTION:
        ElevationMap public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ELEVATION_MAP_H
#define URIVL_ELEVATION_MAP_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImagePoint_F.h"
#include "RasterImage_gray_F.h"
#include "Point3D.h"
#include "GraphicComponent3D.h"


namespace uriVL
{
    /** ElevationMap class
     *  An elevation map stores information relative to a surface of the form Z(X, Y),
     *	where X, Y, Z, are the coordinates of 3D points. An ElevationMap object can be
     *	rendered as a raster image object or drawn as a 3D mesh.
     *
     *	An ElevationMap object stores an elevation map:
     *	<ul>
     *		<li> for a dense depth map, as a 2D raster Z[Y, X] (the row index of a raster
     *				corresponds to the Y coordinate, the column corresponds to X);
     *		<li> for a sparse depth map, it is assumed that the 3D points form an elevation
     *				surface along the Z direction
  	 *	</ul>
  	 *
  	 *	To be properly defined, the elevation map requires scaling parameters for the
	 *	X, Y, and Z axes to transform raster indices into coordinates (the Z scaling
	 *	parameter could in theory be dispensed with, since the elevation is normally
	 *	stored in the float raster, but it is there for the sake of symmetry).
	 *
	 *	So far the only way to create a <code>DepthMap</code>, <code>RangeMap</code>, or
	 *  <code>ElevationMap</code> object is to send it either a raster or a list of 
	 *	points to initialize it with.  In other words, there is no "create now and
	 *	allocate/initialize later" constructor.
     *
     *  Version history:
     *      - v. 1.0		12/16/2004	[jyh]
     *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
	class URIVL_EXPORT ElevationMap : public RasterImage_gray_F,
	                                  public GraphicComponent3D

	{
		public:

			/**	Cloner constructor
			 *
			 *	@param	theMap		elevation map to clone
			 */
			ElevationMap(ElevationMap *theMap);

			/**	Transforms a range raster into a elevation map
			 *
			 *	@param	elevationMap	elevation raster image (gives elevation map
			 *								dimensions)
			 *	@param	theScaleX		length of a raster pixel along X axis
			 *	@param	theScaleY		length of a raster pixel along Y axis
			 *	@param	theScaleZ		scale to apply to Z value stored in the raster
			 */
			ElevationMap(RasterImage_gray* elevationMap, 
						 float theScaleX, float theScaleY, float theScaleZ);

			/**	transforms an elevation raster into a dense elevation map object.
			 *	
			 *	The input rectangle defines the origin and dimensions of the map
			 *
			 *	@param	theRect		bounding rectangle of the map
			 *	@param	theScaleX	length of a raster pixel along X axis
			 *	@param	theScaleY	length of a raster pixel along Y axis
			 *	@param	theScaleZ	scale to apply to Z value stored in the raster
			 *	@param	elevation	1D elevation raster
			 */			 
			ElevationMap(const ImageRect* theRect, const float* elevation,
						 float theScaleX, float theScaleY, float theScaleZ);
			
			/**	transforms an elevation raster into a dense elevation map object.
			 *	
			 *	The elevation map's origin is assumed to be its lower left corner
			 *
			 *	@param	theNbRows	height of the input raster
			 *	@param	theNbCols	width of the input raster
			 *	@param	elevation	1D elevation raster
			 *	@param	theScaleX	length of a raster pixel along X axis
			 *	@param	theScaleY	length of a raster pixel along Y axis
			 *	@param	theScaleZ	scale to apply to Z value stored in the raster
			 */			 
			ElevationMap(int theNbRows, int theNbCols, const float* elevation,
						 float theScaleX, float theScaleY, float theScaleZ);
			
			/**	Allocates space (and mesh) for an elevation the data of which will
			 *	only be known later.
			 *	
			 *	The elevation map's origin is assumed to be the center of its bottom row
			 *
			 *	@param	theNbRows	height of the input raster
			 *	@param	theNbCols	width of the input raster
			 *	@param	theScaleX	length of a raster pixel along X axis
			 *	@param	theScaleY	length of a raster pixel along Y axis
			 *	@param	theScaleZ	scale to apply to Z value stored in the raster
			 */			 
			ElevationMap(int theNbRows, int theNbCols, float theScaleX, float theScaleY, 
			             float theScaleZ);
			
			
			/*	Creates a sparse elevation map from a list of point coordinates
			 *
			 *	@param	nbPts	number of points
			 *	@param	thePt	list of point coordinates
			 */
			ElevationMap(int nbPts, const float*const* thePt);

			/*	Creates a sparse elevation map from a list of 3D point
			 *
			 *	@param	nbPts	number of points
			 *	@param	thePt	list of 3D points
			 */
			ElevationMap(int nbPts, const Point3D*const* thePoint);
			
			/**	Destructor
			 */
			~ElevationMap(void);

			/**	Recalculate the coordinates of the mesh points following an update of the
			 *	base raster or of the reference 3D points
			 */
			void updateMesh(void);
			
			/**	Renders the elevation map as a meshed surface
			 */
			void draw(void);
			
			/**	Sets the resolution for the rendering of the mesh.
			 *
			 *	Calling this function only makes sense for a dense (raster) map.
			 *
			 *	@param	res		resolution level for rendering the mesh
			 */
			void setResolution(ResolutionLevel res);

			/**	Indicates whether or not the elevation map should be rendered in wireframe.
			 *
			 *	@param	wfState		true if the elevation map should be rendered in wireframe
			 *
			 *	@see	isWireframe_
			 */
			void setWireframe(bool wfState);
			
			/**	Indicates whether or not the elevation map should be rendered using vertex 
			 *	normals for a "smooth" result.
			 *
			 *	@param	theUseNormals	true if the elevation map should be rendered using
			 *								vertex normals
			 *
			 *	@see	useVertexNormals_
			 */
			void setUseVertexNormals(bool theUseNormals);
			

		protected:
		
           /**
             *  Copy Constructor. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            ElevationMap(const ElevationMap& theObj);


		private:
		
			/**	Rendering resolution for a dense elevation map
			 *
			 *	@see	setResolution
			 */
			int		nRes_;
			
			/**	Number of points of a sparse elevation map
			 */
			int		nbPts_;
			
			/**	Stores the graph of a sparse elevationmap
			 */
			int		**meshGraph_;

			/**	Should the elevation map be rendered in wireframe?  
			 *	Default is <code>true</code>.
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
			
			
			/**	length of a raster pixel along X axis (in m)
			 */
			float		scaleX_;

			/**	length of a raster pixel along Y axis (in m)
			 */
			float		scaleY_;

			/**	scale to apply to Z value stored in the raster (in m)
			 */
			float		scaleZ_;


            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const ElevationMap& operator = (const ElevationMap& theObj);

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

#endif  //  ELEVATION_MAP_H
