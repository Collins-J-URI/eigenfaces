/*  NAME:
        IntrinsicParameters_DLT.h
 
    DESCRIPTION:
        IntrinsicParameters_DLT public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_INTRINSIC_PARAMETERS_DLT_H
#define URIVL_INTRINSIC_PARAMETERS_DLT_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "IntrinsicParameters_CCD.h"
#include "IntrinsicParameters_pinhole.h"

namespace uriVL
{
	/** Implements the IntrinsicParameters_DLT class.
	 * 
	 *  A IntrinsicParameters_DLT object stores the intrinsic parameters
	 *  of a perspective camera as modelled by the Direct Linear Transformation method.
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT IntrinsicParameters_DLT :	public IntrinsicParameters_CCD,
    												public IntrinsicParameters_pinhole

    {
        public:

			/**	Copy constructor
			 *
			 *	@param	theParams	object to clone
			 */
            IntrinsicParameters_DLT(const IntrinsicParameters_DLT *theParams);


			/**	Creates intrinsic parameter object for an ideal pinhole camera.
			 *	The camera created has no distortion, has the same dimensions for its
			 *	sensing device and digitizable area, its sensor elements are (arbitrarily)
			 *  one micrometer wide squares, and it has its projection center at
			 *	at the center of the digitizable area.
			 *
			 *	@param	theWidth	width of digitizable area (in pixels)
			 *	@param	theHeight	height of digitizable area (in pixels)
			 *	@param	theFx		horizontal focal length (in pixel)
			 *	@param	theFy		vertical focal length (in pixel)
			 */
            IntrinsicParameters_DLT(int theWidth, int theHeight, double theFx, double theFy);


			/**	Creates intrinsic parameter object for a real, using nominal
			 *	(constructor) values for the parameters.  The camera is assumed to
			 *	be distortion-free (origin at the center, square pixels, etc.).
			 *	This is the constructor used for an uncalibrated camera.  After
			 *	Calibration, the parameters could either be set using the 
			 *	<code>setXYZ</code> access functions or a new object could be created
			 *	using one of the next two constructors.
			 *
			 *	@param	theWidth	width of digitizable area (in pixels)
			 *	@param	theHeight	height of digitizable area (in pixels)
			 *	@param	nbSensorsX	number of sensor elements horizontally
			 *	@param	nbSensorsY	number of sensor elements vertically
			 *	@param	dx			width of a sensor element (in m)
			 *	@param	dy			height of a sensor element (in m)
			 *	@param	theFx		horizontal focal length (in pixel)
			 *	@param	theFy		vertical focal length (in pixel)
			 */
            IntrinsicParameters_DLT(int theWidth, int theHeight, int nbSensorsX, 
								    int nbSensorsY, double dx, double dy, double theFx,
									double theFy);


			/**	Creates intrinsic parameter object for a real, calibrated camera.
			 *	This constructor would typically be used after calibration. It uses
			 *	an initial "pre-calibration" intrinsic parameters object and corrected,
			 *	"post-calibration" values for the distortion parameters.
			 *
			 *	@param	theParams	parameters pre-calibration (nominal camera)
			 *	@param	originX		x coordinate of the projection center (in pixels)
			 *	@param	originY		y coordinate of the projection center (in pixels)
			 *	@param	theFx		horizontal focal length (in pixel)
			 *	@param	theFy		vertical focal length (in pixel)
			 *	@param	theSigma	horizontal skewing parameter
			 *	@param	theK1		1st order radial distortion parameter
			 */
            IntrinsicParameters_DLT(const IntrinsicParameters_DLT *theParams, double originX,
									double originY, double theFx, double theFy, double theSigma, 
									double theK1);

			/**	Creates intrinsic parameter object for a real, calibrated camera.
			 *
			 *	@param	theWidth	width of digitizable area (in pixels)
			 *	@param	theHeight	height of digitizable area (in pixels)
			 *	@param	nbSensorsX	number of sensor elements horizontally
			 *	@param	nbSensorsY	number of sensor elements vertically
			 *	@param	dx			width of a sensor element (in m)
			 *	@param	dy			height of a sensor element (in m)
			 *	@param	originX		x coordinate of the projection center (in pixels)
			 *	@param	originY		y coordinate of the projection center (in pixels)
			 *	@param	theFx		horizontal focal length (in pixel)
			 *	@param	theFy		vertical focal length (in pixel)
			 *	@param	theSigma	horizontal skewing parameter
			 *	@param	theK1		1st order radial distortion parameter
			 */
            IntrinsicParameters_DLT(int theWidth, int theHeight, int nbSensorsX, 
            							int nbSensorsY, double dx, double dy, double originX,
            							double originY, double theFx, double theFy, double theSx,
										double theK1);

			/**	Destructor
			 */
            ~IntrinsicParameters_DLT(void);

			/**	Sets the value of the horizontal skewing parameter.
			 *
			 *	@param	theSigma	horizontal skewing parameter
			 */
			void setSkewing(double theSigma);
    			
			/**	Returns the value of the horizontal skewing parameter.
			 *
			 *	@return	the vlue of the horizontal skewing parameter
			 */
			double getSkewing(void) const;
    			
			/**	Sets the value of the horizontal focal length.
			 *
			 *	@param	theFx		horizontal focal length (in pixel)
			 */
			void setFocalLengthX(double theFx);
    			
			/**	Sets the value of the vertical focal length.
			 *
			 *	@param	theFy		horizontal focal length (in pixel)
			 */
			void setFocalLengthY(double theFy);
    			
			/**	Returns the focal length along the x axis (in pixel)
			 *
			 *	@return	the focal length along the x axis (in pixel)
			 */
			double getFocalLengthX(void) const;
    			
			/**	Returns the focal length along the y axis (in pixel)
			 *
			 *	@return	the focal length along the y axis (in pixel)
			 */
			double getFocalLengthY(void) const;
    			
			/**	Sets the value of the 1st order radial distortion parameter.
			 *
			 *	@param	theK1		1st order radial distortion parameter
			 */
			void setKappa1(double theK1);
    			
			/**	Returns the value of the 1st order radial distortion parameter.
			 *
			 *	@return	the 1st order radial distortion parameter
			 */
			double getKappa1(void) const;
    			
			/**	Applies distortion to an image point (pixels)
			 *
			 *	@param	ux	x coordinate (pixel) of the input undistorted point
			 *	@param	uy	y coordinate (pixel) of the input undistorted point
			 *	@param	dx	x coordinate (pixel) of the output distorted point
			 *	@param	dy	y coordinate (pixel) of the output distorted point
			 *
			 *	@see	hasDistortion
			 */
			void distortPixels(double ux, double uy, double *dx, double *dy) const;
			
			/**	Applies distortion to an image point (m)
			 *
			 *	@param	ux	x coordinate (m) of the input undistorted point
			 *	@param	uy	y coordinate (m) of the input undistorted point
			 *	@param	dx	x coordinate (m) of the output distorted point
			 *	@param	dy	y coordinate (m) of the output distorted point
			 *
			 *	@see	hasDistortion
			 */
			void distortMetric(double ux, double uy, double *dx, double *dy) const;
			
			/**	Applies undistortion to an image point (pixels)
			 *
			 *	@param	dx	x coordinate (pixel) of the input distorted point
			 *	@param	dy	y coordinate (pixel) of the input distorted point
			 *	@param	ux	x coordinate (pixel) of the output undistorted point
			 *	@param	uy	y coordinate (pixel) of the output undistorted point
			 *
			 *	@see	hasUndistortion
			 */
			void undistortPixel(double dx, double dy, double *ux, double *uy) const;
			
			/**	Applies undistortion to an image point (m)
			 *
			 *	@param	dx	x coordinate (m) of the input distorted point
			 *	@param	dy	y coordinate (m) of the input distorted point
			 *	@param	ux	x coordinate (m) of the output undistorted point
			 *	@param	uy	y coordinate (m) of the output undistorted point
			 *
			 *	@see	hasUndistortion
			 */
			void undistortMetric(double dx, double dy, double *ux, double *uy) const;
			
            /** Transforms metric coordinates into pixel coordinates.
             *
             *  @param  mx  metrix x coordinate of a point
             *  @param  my  metrix y coordinate of a point
             *  @param  px  pixel x coordinate of that point
             *  @param  py  pixel y coordinate of that point
             */
            void metricToPixel(double mx, double my, double *px, double *py) const;
            
            /** Transforms pixel coordinates into metric coordinates.
             *
             *  @param  px  pixel x coordinate of a point
             *  @param  py  pixel y coordinate of a point
             *  @param  mx  metrix x coordinate of that point
             *  @param  my  metrix y coordinate of that point
             */
            void pixelToMetric(double px, double py, double *mx, double *my) const;
			
			
        protected:
        
        	/**	  horizontal skewing parameter
        	 */
    		double   sigma_;

        	/**	1st order radial distortion parameter.
        	 */
    		double   kappa1_;
    		
    };
}

#endif  // INTRINSIC_PARAMETERS_DLT_H
