/*  NAME:
        Camera.h
 
    DESCRIPTION:
        Camera public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_CAMERA_H
#define URIVL_CAMERA_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Entity3D.h"
#include "Point3D.h"
#include "ImagePoint_F.h"
#include "IntrinsicParameters.h"
#include "RigidTransformation3D.h"


namespace uriVL
{

	/** The base virtual Camera class.
	 * 
	 *  A Camera object projects geometric 3D points onto the image plane to get 2D image
	 *  points.  This class provides a ridiculous number of functions performing this projection,
	 *  defined for different numbers and types of parameters.
	 *  A Camera object is characterized by a set of extrinsinc and intrinsic parameters.
	 *
	 *	Version history
	 *		- 2004/06/19 [jyh]		first "implementation"
	 *
     *  @author jean-yves herve', 3D Group, 
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
	class URIVL_EXPORT Camera : public Entity3D
	{

		public:

            /** Non-operative destructor.
             *
             *  The class destructor has nothing to do.
             */
			virtual ~Camera(void);

            /** Returns the width (in pixels) of the raster digitized
             *	@return		width (in pixels) of the raster digitized
             */
            int getWidth(void);
            
            /** Returns the height (in pixels) of the raster digitized
             *	@return		height (in pixels) of the raster digitized
             */
            int getHeight(void);

    		
            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  This function must be implemented by child classes.  All other "float" getImagePoint
             *  methods in this class call this method (and are therefore implemented in this class.
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  x   x coordinate of projected image point
             *  @param  y   y coordinate of projected image point
             */
			void getImagePoint(const float X, const float Y, const float Z, float* x, float* y);


            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  This functon must be implemented by child classes.  All other "double" getImagePoint
             *  methods in this class call this method (and are therefore implemented in this class.
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  x  address of x coordinate of projected image point
             *  @param  y  address of y coordinate of projected image point
             */
			void getImagePoint(const double X, const double Y, const double Z, double* x, double* y);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  XYZ   array of coordinates of 3D point to project
             *  @param  x  address of x coordinate of projected image point
             *  @param  y  address of y coordinate of projected image point
             */
			void getImagePoint(const float* XYZ, float* x, float* y);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  @param  XYZ   array of coordinates of 3D point to project
             *  @param  x  address of x coordinate of projected image point
             *  @param  y  address of y coordinate of projected image point
             */
			void getImagePoint(const double* XYZ, double* x, double* y);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  XYZ   array of coordinates of 3D point to project
             *  @param  xy    array of image coordinates of projected point
             */
			void getImagePoint(const float* XYZ, float* xy);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  @param  XYZ   array of coordinates of 3D point to project
             *  @param  xy    array of image coordinates of projected point
             */
			void getImagePoint(const double* XYZ, double* xy);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  xy    array of image coordinates of projected point
             */
	        void getImagePoint(const float X, const float Y, const float Z, float* xy);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  xy    array of image coordinates of projected point
             */
			void getImagePoint(const double X, const double Y, const double Z, double* xy);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  pt3D   reference to the 3D point to project
             *  @param  x  x coordinate of projected image point
             *  @param  y  y coordinate of projected image point
             */
			void getImagePoint(const Point3D *pt3D, float* x, float* y);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  @param  pt3D   reference to the 3D point to project
             *  @param  x  x coordinate of projected image point
             *  @param  y  y coordinate of projected image point
             */
			void getImagePoint(const Point3D *pt3D, double* x, double* y);

             /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  pt3D   reference to the 3D point to project
             *  @param  imgPt   reference to the corresponding image point
             */
			void getImagePoint(const Point3D *pt3D, ImagePoint_F *imgPt);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  XYZ   array of coordinates of 3D point to project
             *  @param  imgPt   reference to the corresponding image point
             */
			void getImagePoint(const float* XYZ, ImagePoint_F *imgPt);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  @param  XYZ   array of coordinates of 3D point to project
             *  @param  imgPt   reference to the corresponding image point
             */
			void getImagePoint(const double* XYZ, ImagePoint_F *imgPt);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  imgPt   reference to the corresponding image point
             */
			void getImagePoint(const float X, const float Y, const float Z, 
								ImagePoint_F *imgPt);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  imgPt   reference to the corresponding image point
             */
			void getImagePoint(const double X, const double Y, const double Z, 
								ImagePoint_F *imgPt);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  XYZ   array of coordinates of 3D point to project
             *  @return     corresponding image point
             */
			ImagePoint_F *getImagePoint(const float* XYZ);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  @param  XYZ   array of coordinates of 3D point to project
             *  @return     corresponding image point
             */
			ImagePoint_F *getImagePoint(const double* XYZ);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @return     corresponding image point
             */
			ImagePoint_F *getImagePoint(const float X, const float Y, const float Z);

            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @return     corresponding image point
             */
			ImagePoint_F *getImagePoint(const double X, const double Y, const double Z);

			/**	Returns the transformation from this camera's reference frame to the world
			 *	reference frame, in read-only form.  If needed, recalculates/updates this 
			 *  transformation
             *  @return pointer to the camera's extrinsic parameters           
			 */
			const RigidTransformation3D *getCamToWorldTransf(void);
			
			/**	 Returns the transformation from this camera's reference frame to the world
			 *	reference frame, in writable form.  If needed, recalculates/updates this 
			 *  transformation.  Pretty much only Calibration3D and Registration3D subclasses
             *  have any business calling that one.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return pointer to the camera's extrinsic parameters           
			 */
			RigidTransformation3D *getCamToWorldTransf(ReadWriteStatus rwa);
			
			/**	Sets "to-world" transformation fpor this camera
			 *	@param	transf  new camera to world transformation
			 */
			void setCamToWorldTransf(RigidTransformation3D *transf);

            /** Returns the camera's intrinsic parameters, in read-only form.
             *  This is the version that most applications should use
             *  @return pointer to the camera's intrinsic parameters           
             */
            const IntrinsicParameters *getIntrinsicParameters(void) const;
            

            /** Returns the camera's intrinsic parameters, in writeable form.
             *  Pretty much only Calibration3D and Registration3D subclasses
             *  have any business calling that one.
             *  @param  rwa the sole possible value for this parameter is R_W_ACCESS
             *  @return pointer to the camera's intrinsic parameters           
             */
            IntrinsicParameters *getIntrinsicParameters(ReadWriteStatus rwa) const;
            

//			virtual void setCameraForRendering(void) = 0;
			
        protected:
        
        	/**	Transformation from camera to world reference frame
        	 */
        	RigidTransformation3D	*cam2World_;
        
        	/**	Indicates whether the transformation from camera to world reference frame
        	 *	is up to date
        	 */
        	bool	cam2WorldIsUpToDate_;
        
            /** Intrinsic parameters for this camera
             */
            IntrinsicParameters *intrinsic_;
            
            
 //           /**	Rigid transformation (rotation * translation between
//        	 *	the two cameras)
//        	 */
//        	RigidTransformation3D *transf_;
           
			 /** The new camera stores the pointer to the IntrinsicParameter object passed
			  * as parameter, but it makes a copy of the rigid transformation data.
              *  @param  params     pointer to intrinsic parameter object that this camera will refer to
              *  @param  transf     rigid transformation data that this camera will make a copy of
              */
			Camera(IntrinsicParameters *params, RigidTransformation3D *transf=NULL);


            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>float</float>
             *  numbers as parameters
             *
             *  This function must be implemented by child classes.  All other "float" getImagePoint
             *  methods in this class call this method (and are therefore implemented in this class.
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  x   x coordinate of projected image point
             *  @param  y   y coordinate of projected image point
             */
			virtual void getImagePoint_(const float X, const float Y, const float Z, float* x, float* y) = 0;


            /** Computes the image projection of the 3D point whose coordinates were
             *  received as parameters. This version of the function uses <code>double</float>
             *  numbers as parameters
             *
             *  This function must be implemented by child classes.  All other "double" getImagePoint
             *  methods in this class call this method (and are therefore implemented in this class.
             *
             *  @param  X   X coordinate of 3D point to project
             *  @param  Y   Y coordinate of 3D point to project
             *  @param  Z   Z coordinate of 3D point to project
             *  @param  x  address of x coordinate of projected image point
             *  @param  y  address of y coordinate of projected image point
             */
			virtual void getImagePoint_(const double X, const double Y, const double Z, double* x, double* y) = 0;
            
            
        private:
        
            /** Copy constructor. Disabled.
             *  @param obj  reference to the object to copy
             */
            Camera(const Camera& obj);
            
            /** Copy operator. Disabled.
             *  @param obj  reference to the object to copy
             */
            const Camera& operator = (const Camera& obj);


	};

}

#endif	//	CAMERA_H
