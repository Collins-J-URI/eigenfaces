/*  NAME:
        IntrinsicParameters_CCD.h
 
    DESCRIPTION:
        IntrinsicParameters_CCD public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_INTRINSIC_PARAMETERS_CCD_H
#define URIVL_INTRINSIC_PARAMETERS_CCD_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "IntrinsicParameters.h"

namespace uriVL
{
	/** Implements the IntrinsicParameters_CCD class.
	 * 
	 *  An IntrinsicParameters_CCD object stores the intrinsic parameters
	 *  of a digitizing device such as a CDD or CMOS chip (or of a combination
	 *	of analog camera + film scanner).  Whereas the parent class was simply
	 *	concerned with the output side, that is, the dimensions of the digitizable
	 * 	area (in pixels), this class is concerned with the dimensions of the
	 *	digitizing device itself:  the number of elements and their dimensions.
	 *
	 *	Note that the number of elements of the device is typically not equal to
	 *	that of the output (pixel values are interpolated from the measurements
	 *	of sensor elements).
	 *
	 *	Also note that the characteristics of the CCD device are completely
	 *	independent from the type of optics mounted on the camera.
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT IntrinsicParameters_CCD : public virtual IntrinsicParameters
    {
        public:
        
        	/**	Cloner constructor
        	 *  @param  theParams   camera parameters object to clone
        	 */
            IntrinsicParameters_CCD(const IntrinsicParameters_CCD *theParams);
            
            /**	Creates an new intrisic parameters object for an "ideal" CCD camera.
             *	The CCD device has the same number of elements as the digitizable
             *	area and the sensor elements are (arbitrarily) one micrometer wide
             *	squares.
             *
			 *	@param	theWidth	width of digitizable area (in pixels)
			 *	@param	theHeight	height of digitizable area (in pixels)
			 */
            IntrinsicParameters_CCD(int theWidth, int theHeight);
            
            /**	Creates a new intrisic parameters object for a CCD camera
             *
			 *	@param	theWidth	width of digitizable area (in pixels)
			 *	@param	theHeight	height of digitizable area (in pixels)
			 *	@param	nbSensorsX	number of sensor elements horizontally
			 *	@param	nbSensorsY	number of sensor elements vertically
			 *	@param	dx			width of a sensor element (in m)
			 *	@param	dy			height of a sensor element (in m)
			 */
            IntrinsicParameters_CCD(int theWidth, int theHeight, int nbSensorsX, 
            							int nbSensorsY, double dx, double dy);
            
            
            /**	Destructor
             */
            virtual ~IntrinsicParameters_CCD(void);
            

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
             *  a projection center, for example, <code>IntrinsicParameters_TsaiW</code>
             *
             *  @param  mx  metrix x coordinate of a point
             *  @param  my  metrix y coordinate of a point
             *  @param  px  pixel x coordinate of that point
             *  @param  py  pixel y coordinate of that point
             */
            virtual void metricToPixel(double mx, double my, double* px, double* py) const;
            
            /** Transforms pixel coordinates into metric coordinates.
             *  Should be overridden by subclasses that introduce the notion of
             *  a projection center, for example, <code>IntrinsicParameters_TsaiW</code>
             *
             *  @param  px  pixel x coordinate of a point
             *  @param  py  pixel y coordinate of a point
             *  @param  mx  metrix x coordinate of that point
             *  @param  my  metrix y coordinate of that point
             */
            virtual void pixelToMetric(double px, double py, double* mx, double* my) const;
             


        protected:
        
        	/**	number of sensor elements horizontally
        	 */
    		int	Ncx_;

			/**	number of sensor elements vertically
			 */
    		int   Ncy_;
    		
			/**	horizontal dimension of a sensor element in m
			 */
    		double	dx_;

			/**	vertical dimension of a sensor element in m
			 */
    		double   dy_;
    		
			/**	horizontal dimension of a pixel element in m
			 */
    		double	dpx_;

			/**	vertical dimension of a pixel element in m
			 */
    		double   dpy_;
    		
            /** horizontal metric-to-pixel scaling factor 1/dpx stored for faster
             *  calculations
             */
            double   m2pixX_; 
               				
            /** vertical metric-to-pixel scaling factor 1/dpy stored for faster
             *  calculations
             */
            double   m2pixY_; 
               				

    };
}

#endif  // INTRINSIC_PARAMETERS_CCD_H
