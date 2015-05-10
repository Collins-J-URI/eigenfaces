/*  NAME:
        ImageWarper_GL.h
 
    DESCRIPTION:
        ImageWarper_GL public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_WARPER_GL_H
#define URIVL_IMAGE_WARPER_GL_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageWarper.h"


namespace uriVL
{
    /** holds the records of 2D matching result
     */
    class ImageWarper_GL : public ImageWarper
    {
        public:
            
            /** Creates a new OpenGL ImageWarper whose grid points conform to the displacement 
             *  modes specified.  If modes other than INNER_FREE and BORDER_FREE are specified,
             *  then the warper will verify that the point coordinates specified are conform to
             *  its set motion modes prior to applying a warping. 
             *  @param  gridMode    grid warping mode for this warper
             */
            ImageWarper_GL(InnerWarpingMode innerMode = INNER_FREE, 
                           BorderWarpingMode outerMode = BORDER_FREE);

            /** 
             *  Destructor
             */         
            ~ImageWarper_GL(void);

            /** Defines an input image and an output image(for multiple warpings).  
             *  Note that the dimensions of the image <b>must</b> be in agreement with 
             *  values produced by imageEffectiveDimensions.  
             *  The user should therefore
             *  first call imageEffectiveDimensions with desired image dimensions, and then 
             *  create the image at the corresponding "effective" dimensions specified
             *  by that function.
             *  @param  imgIn   input image for (multiple) subsequent warpings
             */
            void setImages(const RasterImage* imgIn, RasterImage* imgOut);
            
            /** Sets the dimensions of the warping grid (should be at least 3x3)
             *  @param  gridRows    number of rows of the grid
             *  @param  gridCols    number of columns of the grid
             */
            void setGrid(int gridRows, int gridCols);
            
            /** Computes the image dimensions that the warper can afford that are closest to
             *  the ones sent by the user.
             *  <p>Some warpers require specific image dimensions/ratios
             *  (e.g. a GL warper only takes 2^n x 2^n images).  Furthermore, QuickTime
             *  GraphWorld rasters are padded on some platforms.  The user should therefore
             *  first call imageEffectiveDimensions with desired image dimensions, and then 
             *  create the image at the corresponding "effective" dimensions specified
             *  by that function.</p>
             *  <p>If you want to warp an image, it is therefore recommended that you check
             *  what are the closest suitable dimensions for the type of warper you are using.</p>
             *
             *  @param  imgWidth        input image width
             *  @param  imgHeight       input image height
             *  @param  lowImgWidth     closest lower suitable width
             *  @param  lowImgHeight    closest lower suitable height
             *  @param  highImgWidth    closest higher suitable width
             *  @param  highImgHeight   closest higher suitable height
             */
             void getImageSuitableDimensions(int imgWidth, int imgHeight,
                                            int* lowImgWidth, int* lowImgHeight,
                                            int* highImgWidth, int* highImgHeight) const;


            /** Returns the "true" number of pixels on a row of the input image (possibly
             *  larger than the nominal width, due to row padding).
             *  @param  nominalWidth    nominal width of the input image
             *  @return     "true" width of the image stored
             */
             int getEffectivePixelsPerRow(const int nominalWidth) const;
             

        protected:
            
            /** Stores the OpenGL id for the texture map used for the warping
             */
            GLuint textureID_;
            
            /** list of uv points
             */
            GLfloat*** texturePoint_;
            
            /** warp the (pre-specified) input image into the (prespecified) output image
             *  using the new grid point coordinates specified
             *  @param  xy      new coordinates of the grid points
             */         
            void warp_(float*** xy);


            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ImageWarper_GL(const ImageWarper_GL& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ImageWarper_GL& operator =(const ImageWarper_GL& obj);

                    
    };
}

#endif  //  IMAGE_WARPER_GL_H
