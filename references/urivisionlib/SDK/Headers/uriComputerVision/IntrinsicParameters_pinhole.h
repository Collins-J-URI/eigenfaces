/*  NAME:
        IntrinsicParameters_pinhole.h
 
    DESCRIPTION:
        IntrinsicParameters_pinhole public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_INTRINSIC_PARAMETERS_PINHOLE_H
#define URIVL_INTRINSIC_PARAMETERS_PINHOLE_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "IntrinsicParameters.h"

namespace uriVL
{
	/** Implements the IntrinsicParameters_pinhole class.
	 * 
	 *  A IntrinsicParameters_pinhole object stores the intrinsic parameters
	 *	of an ideal pinhole camera.
	 *	A pinhole camera's projection center is at the center of the image and has
	 *	no radial distortion.  However, the pinhole can have vertical/horizontal ratio
	 *	distortion by setting different values for the focal length (in pixels) along
	 *	the x and y axes
	 *
	 *	Note that I only allow the setting of values for the horizontal and vertical
	 *	focal length at the creation of a IntrinsicParameters_pinhole object.  There
	 *	is no <code>set</code> function for these parameters because this would make
	 *	it possible to screw up a <code>IntrinsicParameters_TsaiW</code> object.
	 *
	 *	Note that unless dimensions of pixels have been defined 
	 *	(<code>IntrinsicParameters_CCD</code> and its subclasses), pixel and "metric"
	 *	units are identical.
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 *
	 *	@see		IntrinsicParameters_TsaiW
	 */
    class URIVL_EXPORT IntrinsicParameters_pinhole : public virtual IntrinsicParameters
    {
        public:
        
        	/**	Cloner constructor
          	 *	@param theParams	object to clone
        	 */
            IntrinsicParameters_pinhole(const IntrinsicParameters_pinhole *theParams);

			/**	Creates a pinhole camera at the dimensions specified.
			 *
			 *	@param	theWidth	width of digitizable area (in pixels)
			 *	@param	theHeight	height of digitizable area (in pixels)
			 *	@param	originX		x coordinate of the projection center (in pixels)
			 *	@param	originY		y coordinate of the projection center (in pixels)
			 *	@param	theFx		focal length along x axis (in pixels)
			 *	@param	theFy		focal length along y axis (in pixels)
			 */	
            IntrinsicParameters_pinhole(int theWidth, int theHeight, double originX,
            							double originY, double theFx, double theFy);
            
            /**	Creates a symmetric pinhole camera at the dimensions specified.
             *	The projection center is assumed to be at the center of the digitizable
             *	area.
             *
			 *	@param	theWidth	width of digitizable area (in pixels)
			 *	@param	theHeight	height of digitizable area (in pixels)
			 *	@param	theFx		focal length along x axis (in pixels)
			 *	@param	theFy		focal length along y axis (in pixels)
             */
            IntrinsicParameters_pinhole(int theWidth, int theHeight, double theFx,
            								double theFy);

            /**	Destructor
             */
            virtual ~IntrinsicParameters_pinhole(void);
            
			/**	Returns the value of the focal length along the x axis (in pixel).
			 *
			 *	@return	thefocal length along the x axis (in pixel)
			 */
			double getFocalLengthX(void) const;
    			
			/**	Returns the value of the focal length along the y axis (in pixel).
			 *
			 *	@return	thefocal length along the y axis (in pixel)
			 */
			double getFocalLengthY(void) const;
    			
            /*	Returns the x coordinate of the origin in the same reference frame
             *	as that in which the bounding and valida data rectangles are defined
             *	@return		x coordinate of the origin
             */
            double getOriginX(void) const;
            
            /*	Returns the y coordinate of the origin in the same reference frame
             *	as that in which the bounding and valida data rectangles are defined
             *	@return		y coordinate of the origin
             */
            double getOriginY(void) const;


    		/**	Indicates whether this object has a forward distortion calculation
    		 *	function.  Must be implemented by subclasses.
    		 *
    		 *	@return	true if this object has a forward distortion calculation function
    		 *	@see	distortMetric
    		 *	@see	distortPixel
    		 */
			virtual bool hasDistortion(void) const;
			
    		/**	Indicates whether this object has an undistortion calculation
    		 *	function.  Must be implemented by subclasses.
    		 *
    		 *	@return	true if this object has ann undistortion calculation function
    		 *	@see	undistortMetric
    		 *	@see	undistortPixel
    		 */
			virtual bool hasUndistortion(void) const;


            /** Transforms metric coordinates into pixel coordinates.
             *  Should be overridden by subclasses that introduce the notion of
             *  pixel dimensions, for example, <code>IntrinsicParameters_CCD</code>
             *
             *  @param  mx  metrix x coordinate of a point
             *  @param  my  metrix y coordinate of a point
             *  @param  px  pixel x coordinate of that point
             *  @param  py  pixel y coordinate of that point
             */
            virtual void metricToPixel(double mx, double my, double* px, double* py) const;
            
            /** Transforms pixel coordinates into metric coordinates.
             *  Should be overridden by subclasses that introduce the notion of
             *  pixel dimensions, for example, <code>IntrinsicParameters_CCD</code>
             *
             *  @param  px  pixel x coordinate of a point
             *  @param  py  pixel y coordinate of a point
             *  @param  mx  metrix x coordinate of that point
             *  @param  my  metrix y coordinate of that point
             */
            virtual void pixelToMetric(double px, double py, double* mx, double* my) const;


        protected:
        

			/**	X coordinate of the origin
			 *
			 *	@see	getOriginX
			 */
            double   Ox_;

			/**	Y coordinate of the origin
			 *
			 *	@see	getOriginY
			 */
            double   Oy_;

            /** Focal length along the x axis (in pixel)
             *
             */
            double   fx_;
            
            /** Focal length along the y axis (in pixel)
             *
             */
            double   fy_;
            
        	/**	Default constructor called by some child classes
         	 */
            IntrinsicParameters_pinhole(void);

            
    };
}

#endif  // INTRINSIC_PARAMETERS_PINHOLE_H
