/*  NAME:
        StereoPair.h
 
    DESCRIPTION:
        StereoPair public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_STEREO_PAIR_H
#define     URIVL_STEREO_PAIR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage.h"
#include    "Camera.h"
#include    "RigidTransformation3D.h"


namespace uriVL
{
	/**	Enumerated type for image identification
	 */
	typedef enum StereoImageID {
		LEFT_IMAGE = 0,
		RIGHT_IMAGE = 1
	} StereoImageID;
	
    /**	Future versions of this class will/should include information
     *	about the cameras corresponding to each image (and their
     *	relative transformations and respective parameters)
     */
    class URIVL_EXPORT StereoPair : public Entity3D
    {
        public:

			/**	For the time being the class does <b>not</b> store private
			 *	copies of the left and right images
			 *  Constructor.
			 *  Creates a stereo pair with left image, right image and transformation
			 *  @param  leftImg    left image of stereo pair
             *  @param  rightImg   right image of stereo pair
             *  @param  transf     Rigid transformation between the left and right cameras
			 *	@param	isRect		true if the pair of images has been rectified
			 */
			StereoPair(RasterImage* leftImg, RasterImage* rightImg, 
						RigidTransformation3D *transf, bool isRect = false);
			
			/** Constructor.
			 *  Creates a stereo pair with left camera, right camera and rectangle
			 *  @param  leftCamera    left camera
             *  @param  rightCamera   right camera
			 *	@param	isRect		true if the pair of images has been rectified
			 */
			StereoPair(Camera *leftCamera, Camera *rightCamera, bool isRect = false);
			
			/** Constructor.
			 *  Creates a stereo pair with left camera, right camera,  
			 *  left image and right image
			 *  @param  leftImg    left image of stereo pair
             *  @param  rightImg   right image of stereo pair
			 *  @param  leftCamera    left camera
             *  @param  rightCamera   right camera
			 *	@param	isRect		true if the pair of images has been rectified
			 */
			StereoPair(RasterImage* leftImg, RasterImage* rightImg,  
						 Camera *leftCamera, Camera *rightCamera, bool isRect = false);
			
			
			/**
             *  Destructor
             */          
			~StereoPair(void);
           
            /** Assigns the stereo pair with left image and right image
             *  @param  leftImg    left image of stereo pair
             *  @param  rightImg   right image of stereo pair
             */
            void setImages(RasterImage* leftImg, RasterImage* rightImg);
            /**
             *  Returns     left image of the stereo pair
             *  @return     left image
             */
            RasterImage* getLeftImage(void);
            /**
             *  Returns     right image of the stereo pair
             *  @return     right image
             */
            RasterImage* getRightImage(void);
            /**
             *  Returns     image
             *  @return     left image if l_or_r is 0, otherwise, return right image 
             */
            RasterImage* getImage(StereoImageID l_or_r);
            /**
             *  Returns     image rectangle
             *  @return     image rectangle acoording to l_or_r
             */
            const ImageRect* getValidRect(StereoImageID l_or_r);
            
        	/**	Indicates whethher this stereo pair is rectified
        	 */
        	 /**
             *  Returns     left Camera of the stereo pair
             *  @return     left Camera
             */
            Camera *getLeftCamera(void);
            /**
             *  Returns     right Camera of the stereo pair
             *  @return     right Camera
             */
            Camera *getRightCamera(void);
            /**
             *  Returns     image
             *  @return     left image if l_or_r is 0, otherwise, return right image 
             */
        	bool isRectified(void); 
        	
	
		protected:
		
            /** Default constructor
			 *	@param	isRect		true if the pair of images has been rectified
             */
            StereoPair(bool isRect = false);
            
        private:

			/**
             *  left camera of stereo pair
             */
        	Camera *leftCamera_;
        	/**
             *  right camera of stereo pair
             */
        	Camera *rightCamera_;

        	/**
             *  left image of stereo pair
             */
        	RasterImage* leftImg_;
        	/**
             *  right image of stereo pair
             */
        	RasterImage* rightImg_;
 
        	/**	Rigid transformation (rotation & translation between
        	 *	the two cameras)
        	 */
        	RigidTransformation3D *transf_;
        	
        	/**	Indicates whethher this stereo pair is rectified
        	 */
        	bool isRectified_; 
        	
           /**
             *  Copy Constructor. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            StereoPair(const StereoPair& theObj);


            /**
             *  Copy operator. Disabled
             *
             *  @param      theObj   reference to the object to copy
             */
            const StereoPair& operator = (const StereoPair& theObj);
        	
    };
}
#endif	//	STEREO_PAIR_H
