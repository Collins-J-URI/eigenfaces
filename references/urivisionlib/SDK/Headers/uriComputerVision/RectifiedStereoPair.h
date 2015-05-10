/*  NAME:
        RectifiedStereoPair.h
 
    DESCRIPTION:
        RectifiedStereoPair public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef     URIVL_RECTIFIED_STEREO_PAIR_H
#define     URIVL_RECTIFIED_STEREO_PAIR_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include    "RasterImage.h"
#include    "RigidTransformation3D.h"
#include	"StereoPair.h"


namespace uriVL
{
    /**	Future versions of this class will/should include information
     *	about the cameras corresponding to each image (and their
     *	relative transformations and respective parameters)
     */
    class URIVL_EXPORT RectifiedStereoPair : public StereoPair
    {
        public:

			
			/**	For the time being the class does <b>not</b> store private
			 *	copies of the left and right images.
			 *  Constructor.
			 *  Creates a stereo pair with image rectangle, focus of left 
			 *  camera, focus of right camera, baseline of the camera 
			 *	and transformation from world to stereo pair (set to null so far)
			 *  @param  rect       image rectangle
             *  @param  B          baseline of the left and right camera
             *  @param  focX       horizontal focal length of both cameras (in pixels)
             *  @param  focY       vertical focal length of both cameras (in pixels)
             *  @param  w2s   	   transformation from world to stereo pair
			 */
			RectifiedStereoPair(ImageRect* rect, const float B, const double focX, 
						const double focY,  RigidTransformation3D *w2s=NULL);
			
			/**	For the time being the class does <b>not</b> store private
			 *	copies of the left and right images
			 *  Constructor.
			 *  Creates a stereo pair with left image, right image, focus of left 
			 *  camera, focus of right camera, baseline of the camera 
			 *	and transformation from world to stereo pair (set to null so far)
			 *  @param  leftImg    left image of stereo pair
             *  @param  rightImg   right image of stereo pair
             *  @param  B          baseline of the left and right camera
             *  @param  focX       horizontal focal length of both cameras (in pixels)
             *  @param  focY       vertical focal length of both cameras (in pixels)
             *  @param  w2s   	   transformation from world to stereo pair
			 */
			RectifiedStereoPair(RasterImage* leftImg, RasterImage* rightImg, 
						const float B, const double focX, const double focY, 
						RigidTransformation3D *w2s=NULL);
		
			

			/**	For the time being the class does <b>not</b> store private
			 *	copies of the left and right images
			 *  Constructor.
			 *  Creates a stereo pair with image rectangle, baseline of the camera
			 *	and transformation from world to stereo pair (set to null so far)
			 *  @param  rect       image rectangle
             *  @param  B          baseline of the left and right camera
             *  @param  w2s   	   transformation from world to stereo pair
			 */
			RectifiedStereoPair(ImageRect* rect, float B, RigidTransformation3D *w2s=NULL);
			

			/**	For the time being the class does <b>not</b> store private
			 *	copies of the left and right images
			 *  Constructor.
			 *  Creates a stereo pair with left image, right image, baseline
			 *	of the camera and transformation from world to stereo pair (set to null so far)
			 *  @param  leftImg    left image of stereo pair
             *  @param  rightImg   right image of stereo pair
             *  @param  B          baseline of the left and right camera
             *  @param  w2s   	   transformation from world to stereo pair
			 */
			RectifiedStereoPair(RasterImage* leftImg, RasterImage* rightImg, 
						float B, RigidTransformation3D *w2s=NULL);
			
			/**	For the time being the class does <b>not</b> store private
			 *	copies of the left and right images
			 *  Constructor.
			 *  Creates a stereo pair with left and right cameras 
			 *	and transformation from world to stereo pair (set to null so far)
			 *  @param  leftCam    left camera
             *  @param  rightCam   right camera
			 */
			RectifiedStereoPair(Camera *leftCam, Camera *rightCam);
		
			/**
             *  Destructor
             */          
			~RectifiedStereoPair(void);
                       
        private:
        	
    };
}
#endif	//	RECTIFIED_STEREO_PAIR_H
