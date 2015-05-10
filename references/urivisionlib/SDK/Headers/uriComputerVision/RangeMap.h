/*  NAME:
        RangeMap.h
 
    DESCRIPTION:
        RangeMap public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_RANGE_MAP_H
#define URIVL_RANGE_MAP_H

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
    /** RangeMap class
     *  A range map stores information relative to a surface of the form Z(X, Y),
     *	where X, Y, Z, are the coordinates of 3D points. An ElevationMap object can be
     *	rendered as a raster image object or drawn as a 3D mesh.
     *
     *	A RangeMap object stores a range map:
     *	<ul>
     *		<li> for a dense range map, as a 2D raster r[theta, phi] (the row index of the 
     *			 raster corresponds to the longitude theta, the column corresponds to the
     *			 latitude phi);
     *		<li> for a sparse range map, it is assumed that the 3D points form a valid range
     *				surface (i.e. no two points are aligned with the origin)
  	 *	</ul>
  	 *
  	 *	A range map is what is produced by range finders such as scan lasers or radar.
	 *	To be properly defined, the range map requires scaling parameters for the
	 *	longitude, latitude, and range to transform raster indices into coordinates
	 *	(the r scaling parameter could in theory be dispensed with, since the elevation
	 *	is normally stored in the float raster, but it is there for the sake of symmetry)
	 *
	 *	So far the only way to create a <code>DepthMap</code>, <code>RangeMap</code>, or
	 *  <code>ElevationMap</code> object is to send it either a raster or a list of 
	 *	points to initialize it with.  In other words, there is no "create now and
	 *	allocate/initialize later" constructor.
	 *
	 *  - v. 1.0	12/17/2004	[jyh]
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
	class URIVL_EXPORT RangeMap : public RasterImage_gray_F,
	                              public GraphicComponent3D

	{
		public:

			/**	Cloner constructor
			 *
			 *	@param	theMap		range map to clone
			 */
			RangeMap(RangeMap *theMap);

			/**	Transforms a range raster into a range map
			 *
			 *	@param	rangeMap			range raster image (gives depth map dimensions)
			 *	@param	topLatitude			latitude at top of range map [in degrees]
			 *	@param	bottomLatitude	    latitude at bottom of range map [in degrees]
			 *	@param	leftLongitude	    longitude at left of range map [in degrees]
			 *	@param	rightLongitude	    longitude at right of range map [in degrees]
			 *	@param	scaleRange	        scale to apply to range value stored in raster
			 */
			RangeMap(RasterImage_gray* rangeMap, float topLatitude, float bottomLatitude, 
					 float leftLongitude, float rightLongitude,
			         float scaleRange);

			/**	transforms a range raster into a dense range map object.
			 *	
			 *	The input rectangle defines the origin and dimensions of the map
			 *
			 *	@param	theRect				bounding rectangle of the map
			 *	@param	scaleLongitude	    angle step for the longitude [in degrees]
			 *	@param	scaleLatitude	    angle step for the latitude [in degrees]
			 *	@param	scaleRange	        scale to apply to range value stored in raster
			 *	@param	range				1D range raster
			 */			 
			RangeMap(ImageRect* theRect, float scaleLongitude, float scaleLatitude,
			            float scaleRange, const float* range);
			
			/**	Set up dimension for the range map.
			 *	
			 *	@param	theNbRows			height of the range map
			 *	@param	theNbCols			width of the range map
			 *	@param	startElevation	    the start Elevation angle [in degrees]
			 *	@param	elevationStep	    the increament of Elevation angle for each steps [in degrees]
			 *	@param	startAzimuth	    the start Azimuth angle [in degrees]
			 *	@param	AzimuthStep	        the increament of Azimuth angle for each steps [in degrees]
			 */			 
			RangeMap(int theNbRows, int theNbCols, float startElevation, 
				   float elevationStep, float startAzimuth, float AzimuthStep);
			
			/*	Creates a sparse range map from a list of point coordinates
			 *
			 *	@param	nbPts	number of points
			 *	@param	thePt	list of point coordinates
			 */
			RangeMap(int nbPts, const float** thePt);

			/*	Creates a sparse range map from a list of 3D point
			 *
			 *	@param	nbPts	number of points
			 *	@param	thePt	list of 3D points
			 */
			RangeMap(int nbPts, const Point3D **thePoint);
			
			/**	Destructor
			 */
			~RangeMap(void);

			
			/**	Recalculate the coordinates of the mesh points following an update of the
			 *	base raster or of the reference 3D points
			 */
			void updateMesh(void);
			

			/**	Renders the range map as a meshed surface
			 */
			void draw(void);
			
			/**	Sets the resolution for the rendering of the mesh.
			 *
			 *	Calling this function only makes sense for a dense (raster) map.
			 *
			 *	@param	res		resolution level for rendering the mesh
			 */
			void setResolution(ResolutionLevel res);

			/**	Indicates whether or not the range map should be rendered in wireframe.
			 *
			 *	@param	wfState		true if the range map should be rendered in wireframe
			 *
			 *	@see	isWireframe_
			 */
			void setWireframe(bool wfState);
		
		
		protected:
			
           /**
             *  Copy Constructor. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            RangeMap(const RangeMap& theObj);

			
			
		private:
		
			/**	Stores the graph of a sparse map
			 */
			int			**meshGraph_;

			/**	Triangular mesh of a dense map
			 */
			float		***triMesh_;
			
			/**	Rendering resolution for a dense range map
			 *
			 *	@see	setResolution
			 */
			int			nRes_;
			
			/**	Number of points of a sparse range map
			 */
			int			nbPts_;
			
			/**	Should the range map be rendered in wireframe
			 *
			 *	@see	setWireframe
			 */								
			bool		isWireframe_;

			/**	longitude at left of range map [in radians]
			 */
			float		startLongitude_;

			/**	angle step for the longitude [in radians]
			 */
			float		stepLongitude_;

			/**	latitude at top of range map [in radians]
			 */
			float		startLatitude_;

			/**	angle step for the latitude [in radians]
			 */
			float		stepLatitude_;

			/**	Scale to transform raster values into a range [in m]
			 */
			float		scaleRange_;

			/**	Store the value of cosine of longitude
			 */
			float*		cosLong_;

			/**	Store the value of sine of longitude
			 */			
			float*		sinLong_;

			/**	Store the value of cosine of latitude
			 */
			float*		cosLat_;

			/**	Store the value of sine of latitude
			 */			
			float*		sinLat_;

			/**	Indicates whether or not the elevation map should be rendered using vertex 
			 *	normals for a "smooth" result. Default is <code>false</code>.
			 *
			 *	@see	setUseVertexNormals
			 */
			bool useVertexNormals_;

            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const RangeMap& operator = (const RangeMap& theObj);

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

#endif  //  RANGE_MAP_H
