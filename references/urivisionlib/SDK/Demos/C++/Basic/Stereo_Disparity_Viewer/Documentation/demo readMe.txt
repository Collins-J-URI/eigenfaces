This project demonstrates the use of dense DepthMap objects (and therefore
that of the RangeMap and ElevationMap classes which work pretty much
the same way).

The three classes are derived from RasterImage_gray_F and can therefore
store a float or integer 2D raster of depth/elevation/range values.

I have also put in place "hooks" for the later implementation of sparse
maps, defined by a few discrete points.  THis will be implemented later,
right now I have need for the dense maps, hence the priority attribution.

A dense ElevationMap object stores in a 2D raster discrete values for a function
		Z(X, Y),	where X, Y, Z are the Cartesian coordinates of a 3D point

To be properly defined, the elevation map requires scaling parameters for the
X, Y, and Z axes to transform raster indices into coordinates (the Z scaling
parameter could in theory be dispensed with, since the elevation is normally
stored in the float raster, but it is there for the sake of symmetry).




A dense DepthMap object stores in a 2D raster discrete values for a function
		Z(x, y),	where x, y are the coordinates of an image point 
						  Z is the "depth" Cartesian coordinate (along optical axis)
						  	of the corresponding 3D point
		
The depth map (for me at least) is pretty much indisossiable from the
notion of perspective projection.  Therefore, all constructors for this class
require an IntrinsicParameters_pinhole object (which will provide a focal
length and scaling parameters for the inverse projection.  If the camera
used in orthographic, then the raster does not really store a depth map but
an elevation map.
		


		
A dense RangeMap object stores in a 2D raster discrete values for a function
		r(lat, long),	where 	lat, long, r are the spherical coordinates
								(latitude, longitude, radius) of a 3D point
		
A range map is what is produced by range finders such as scan lasers or radar.
To be properly defined, the range map requires scaling parameters for the
longitude, latitude, and range to transform raster indices into coordinates
(the r scaling parameter could in theory be dispensed with, since the elevation
is normally stored in the float raster, but it is there for the sake of symmetry).
		