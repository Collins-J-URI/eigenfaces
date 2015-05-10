/*  NAME:
        IntrinsicParameters.h
 
    DESCRIPTION:
        IntrinsicParameters public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_INTRINSIC_PARAMETERS_H
#define URIVL_INTRINSIC_PARAMETERS_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

namespace uriVL
{
	/**	Parent class for all descriptors of a camera's intrinsic parameters.
	 *
	 *	A "camera" is a device (physical or virtual) that digitizes data to 
	 *	produce a raster.  One could select to produce a raster for a small 
	 *	subpart of the available surface, but this would be a characteristic
	 *	of the raster image produced, not of the camera.
	 *
	 *	Since calculating the intrinsic parameters of a camera is an operation
	 *	that is performed unfrequently, I have decided not to provide any
	 *	"setVariable" method.  The use desiring modify the value of a particular
	 *	parameter should therefore create a new <code>IntrinsicParameter</code>
	 *	object initialized with the proper values for all the parameters.<p>
	 *	
	 *	The basic geometric properties of a camera are the dimensions of the
	 *	digitizable rectangle.   Subclasses will add information relative to the
	 *	optics of the camera (focal length, coordinates of the origin, 
	 *	distortion) and the details of the digitizing device (number and size
	 *	of sensor elements).
	 *
	 *	I have hesitated a long time about it, moving instance variables and functions
	 *	relative to distortion and transformations between "pixel" and "world"
	 *	coordinates back and forth between this class and its subclassses.  I have finally
	 *	decided to define at the root class the following functions:
	 *	<ul>
	 *		<li> <code>pixelToMetric</code>
	 *		<li> <code>metricToPixel</code>
	 *		<li> <code>distortMetric</code>
	 *		<li> <code>distortPixel</code>
	 *		<li> <code>undistortMetric</code>
	 *		<li> <code>undistortPixel</code>
	 *	</ul>
	 *
	 *	Users of these functions should however be aware that
	 *	<ul>
	 *		<li> unless coordinates of the image center have been defined
	 *				(<code>IntrinsicParameters_pinhole</code> and its subclasses),
	 *				the image center is assumed to be at (0, 0).
	 *		<li> unless dimensions of pixels have been defined
	 *				(<code>IntrinsicParameters_CCD</code> and its subclasses),
	 *				pixel and "metric" units are identical
	 *		</ul>
	 *				
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT IntrinsicParameters
    {
        public:
        
        	/**	Cloner constructor
        	 *	@param	theParams	object to clone
         	 */
            IntrinsicParameters(const IntrinsicParameters *theParams);

        	/**	Constructor
			 *	@param	theWidth	width of digitizable area (in pixels)
			 *	@param	theHeight	height of digitizable area (in pixels)
        	 */
            IntrinsicParameters(int theWidth, int theHeight);

        	/**	Destructor
        	 */
            virtual ~IntrinsicParameters(void);
            
            /** Returns the width (in pixels) of the raster digitized
             *	@return		width (in pixels) of the raster digitized
             */
            int getWidth(void) const;
            
            /** Returns the height (in pixels) of the raster digitized
             *	@return		height (in pixels) of the raster digitized
             */
            int getHeight(void) const;

    		
    		/**	Indicates whether this object has a forward distortion calculation
    		 *	function.  Must be implemented by subclasses.
    		 *
    		 *	@return	true if this object has a forward distortion calculation function
    		 *	@see	distortMetric
    		 *	@see	distortPixel
    		 */
			virtual bool hasDistortion(void) const = 0;
			
    		/**	Indicates whether this object has an undistortion calculation
    		 *	function.  Must be implemented by subclasses.
    		 *
    		 *	@return	true if this object has ann undistortion calculation function
    		 *	@see	undistortMetric
    		 *	@see	undistortPixel
    		 */
			virtual bool hasUndistortion(void) const = 0;

			/**	Applies distortion to an image point (pixels).
			 *
			 *	Should be overridden by classes that have distortion parameters
			 *
			 *	@param	ux	x coordinate (pixel) of the input undistorted point
			 *	@param	uy	y coordinate (pixel) of the input undistorted point
			 *	@param	dx	x coordinate (pixel) of the output distorted point
			 *	@param	dy	y coordinate (pixel) of the output distorted point
			 *
			 *	@see	hasDistortion
			 */
			virtual void distortPixels(double ux, double uy, double* dx, double* dy) const;
			
			/**	Applies distortion to an image point (m)
			 *
			 *	Should be overridden by classes that have distortion parameters
			 *
			 *	@param	ux	x coordinate (m) of the input undistorted point
			 *	@param	uy	y coordinate (m) of the input undistorted point
			 *	@param	dx	x coordinate (m) of the output distorted point
			 *	@param	dy	y coordinate (m) of the output distorted point
			 *
			 *	@see	hasDistortion
			 */
			virtual void distortMetric(double ux, double uy, double* dx, double* dy) const;
			
			/**	Applies undistortion to an image point (pixels)
			 *
			 *	Should be overridden by classes that have distortion parameters
			 *
			 *	@param	dx	x coordinate (pixel) of the input distorted point
			 *	@param	dy	y coordinate (pixel) of the input distorted point
			 *	@param	ux	x coordinate (pixel) of the output undistorted point
			 *	@param	uy	y coordinate (pixel) of the output undistorted point
			 *
			 *	@see	hasunDistortion
			 */
			virtual void undistortPixel(double dx, double dy, double* ux, double* uy) const;
			
			/**	Applies undistortion to an image point (m)
			 *
			 *	Should be overridden by classes that have distortion parameters
			 *
			 *	@param	dx	x coordinate (m) of the input distorted point
			 *	@param	dy	y coordinate (m) of the input distorted point
			 *	@param	ux	x coordinate (m) of the output undistorted point
			 *	@param	uy	y coordinate (m) of the output undistorted point
			 *
			 *	@see	hasunDistortion
			 */
			virtual void undistortMetric(double dx, double dy, double* ux, double* uy) const;

            /** Transforms metric coordinates into pixel coordinates.
             *  Should be overridden by subclasses that introduce the notion of
             *  a projection center, for example, <code>IntrinsicParameters_TsaiW</code>,
             *  or pixel dimensions, for example, <code>IntrinsicParameters_CCD</code>.
             *
             *  @param  mx  metrix x coordinate of a point
             *  @param  my  metrix y coordinate of a point
             *  @param  px  pixel x coordinate of that point
             *  @param  py  pixel y coordinate of that point
             */
            virtual void metricToPixel(double mx, double my, double* px, double* py) const;
            
            /** Transforms pixel coordinates into metric coordinates.
             *  Should be overridden by subclasses that introduce the notion of
             *  a projection center, for example, <code>IntrinsicParameters_TsaiW</code>,
             *  or pixel dimensions, for example, <code>IntrinsicParameters_CCD</code>.
             *
             *  @param  px  pixel x coordinate of a point
             *  @param  py  pixel y coordinate of a point
             *  @param  mx  metrix x coordinate of that point
             *  @param  my  metrix y coordinate of that point
             */
            virtual void pixelToMetric(double px, double py, double* mx, double* my) const;


        protected:
        
        	/**	number of frame grabber elements horizontally
        	 */
    		int	Nfx_;

        	/**	number of frame grabber elements vertically
        	 */
    		int	Nfy_;
    		

    };
}

#endif  // INTRINSIC_PARAMETERS_H
