/*  NAME:
        ImageWarper.h
 
    DESCRIPTION:
        ImageWarper public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_IMAGE_WARPER_H
#define URIVL_IMAGE_WARPER_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"
#include "RasterImage_RGBa.h"
#include "RasterImage_gray.h"
#include "ImagePoint.h"
#include "ImageRect.h"

namespace uriVL
{
    /** Enumarated type listing the types of image warpers supported by the library
     */
    typedef enum ImageWarperType {  GL_WARPER = 0
    } ImageWarperType;
    
    /** Enumerated type for the different modes of warping (as determined by inner grid point
     *  motion) supported.  This is just a first stab at organizing this.  If a mode other than
     *  
     */
    typedef enum InnerWarpingMode { INNER_FREE = 0,  //  unconstrained inner grid point motion
                                    INNER_TOPOLOGY,  //  no grid crossings
                                    INNER_CONVEX,    //  grid quads must remain convex
                                    INNER_BOUNDS     //  rectangular bounds for all grid points      
    } InnerWarpingMode;
    
    /** Enumerated type for the different modes of warping (as determined by outer grid point
     *  motion) supported.  This is just a first stab at organizing this.
     */
    typedef enum BorderWarpingMode {BORDER_FREE = 0,    //   no constraints on motion
                                    BORDER_INWARD,      //  grid border points can move inward
                                    BORDER_SLIDE,       //  grid border points slide along border
                                    BORDER_FIXED        //  grid border points are fixed
    } BorderWarpingMode;
    
    /** Virtual parent class for all image warper object.  Subclasses will implement
     *  a particular kind of warping algorithm (for example, by OpenGL-based texture
     *  mapping.
     *  @author Meng Wan, 3D Group for Interactive Visualization, 
     *                      Dept of Computer Science, University of Rhode Island
     *  @author jean-yves herve', 3D Group for Interactive Visualization, 
     *                      Dept of Computer Science, University of Rhode Island
     */
    class ImageWarper
    {
        public:
            
            /** Creates a new ImageWarper whose grid points conform to the displacement modes
             *  specified.  If modes other than INNER_FREE and BORDER_FREE are specified, then
             *  the warper will verify that the point coordinates specified are conform to its
             *  set motion modes prior to applying a warping. 
			 *	@param	type		type of this warper
             *  @param  innerMode    grid inner warping mode for this warper
             *  @param  outerMode    grid outer warping mode for this warper
             */
            ImageWarper(ImageWarperType type,
						const InnerWarpingMode innerMode = INNER_FREE, 
                        const BorderWarpingMode outerMode = BORDER_FREE);

            /** 
             *  Destructor
             */         
            virtual ~ImageWarper(void);

            /** Indicates the type of warping algorithm used by this warper (e.g. OpenGL)
             */
            ImageWarperType getWarperType(void) const;            

            /** Defines an input image and an output image(for multiple warpings).  
             *  Note that the dimensions of the image <b>must</b> be in agreement with 
             *  values produced by imageEffectiveDimensions: Some warpers require specific image
             *  dimensios/ratios (e.g. a GL warper only takes 2^n x 2^n images).  
             *  Furthermore, QuickTime GraphWorld rasters are padded on some platforms.  
             *  The user should therefore
             *  first call imageEffectiveDimensions with desired image dimensions, and then 
             *  create the image at the corresponding "effective" dimensions specified
             *  by that function.
             *  @param  imgIn   input image for (multiple) subsequent warpings
             */
            virtual void setImages(const RasterImage* imgIn, RasterImage* imgOut);
            
            /** Sets the dimensions of the warping grid (should be at least 3x3)
             *  @param  gridRows    number of rows of the warping grid
             *  @param  gridCols    number of columns of the warping grid
             */
            virtual void setGrid(int gridRows, int gridCols);
            
            /** Returns a new grid of points allocated at the grid dimensions of this 
             *  warper and initialized as a regular rectangular grid.  This function
             *  can only be called after the grid dimensions and input image have been
             *  set.  The user is responsible for deleting this grid when it is not 
             *  needed anymore.
             *  @return a new warping grid to be used with this warper
             */
            float*** const newWarpingGrid(void) const;

            /** Returns the warper's input image
             */
            const RasterImage* getInputImage(void) const;
            
            /** Returns the warper's output image
             */
            RasterImage* getOutputImage(void) const;
             
            /** Indicates whether a point grid is consistent with the displacement modes of
             *  of this warper.
             *  @param  xy      grid to validate
             *  @return true if the point coordinates are appropriate for this warper
             */         
            virtual bool isValidDisplacement(float*** xy) const;

            /** Warps the image.  If this warper's grid motion modes are other than INNER_FREE 
             *  and BORDER_FREE then will only apply a warping that is conform to its motion
             *  modes. Note that the dimensions
             *  of the image <b>must</b> be in agreement with values produced by 
             *  imageEffectiveDimensions: Some warpers require specific image dimensions/ratios
             *  (e.g. a GL warper only takes 2^n x 2^n images).
             *  @param  xy      new coordinates of the grid points
             *  @param  imgIn   input image 
             *  @param  imgOut  output image 
             *  @see setImage
             */         
            void warp(float*** xy, const RasterImage* imgIn, RasterImage* imgOut);

            /** warp the image using the new grid point coordinates specified
             *  @param  xy      new coordinates of the grid points
             */         
            void warp(float*** xy);

            /** Returns the number of rows on this warper's grid
             */
            int getNbGridRows(void) const;

            /** Returns the number of columns on this warper's grid
             */
            int getNbGridCols(void) const;
            
            /** Indicates whether the input image, output image, and grids are all correctly
             *  set up and mutually compatible.
             */
            bool isSetUp(void) const; 
            
            /** Returns the warper's inner  warping mode
             */
            InnerWarpingMode getInnerWarpingMode(void) const;
            
            /** Returns the warper's border  warping mode
             */
            BorderWarpingMode getBorderWarpingMode(void) const;

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
             virtual void getImageSuitableDimensions(int imgWidth, int imgHeight,
                                                    int* lowImgWidth, int* lowImgHeight,
                                                    int* highImgWidth, int* highImgHeight) const = 0;

            /** Returns the "true" number of pixels on a row of the input image (possibly
             *  larger than the nominal width, due to row padding).
             *  @param  nominalWidth    nominal width of the input image
             *  @return     "true" width of the image stored
             */
             virtual int getEffectivePixelsPerRow(const int nominalWidth) const = 0;
             
        protected:
        
            /** Indicates the type of warping algorithm used by this warper (e.g. OpenGL)
             */
            ImageWarperType warperType_;            

            /** Warping mode if the grid's inner points
             */
            InnerWarpingMode innerMode_; 

            /** Warping mode if the grid's border points
             */
            BorderWarpingMode outerMode_;
                        
            /** Indicates whether the input image, output image, and grids are all correctly
             *  set up and mutually compatible.
             */
            bool isSetUp_; 
             
            /** Height of the image to warp
             */
            int imgHeight_;
            
            /** Width of the image to warp
             */
            int imgWidth_;
            
            /** Effective width of the image to warp (taking into account row padding)
             */
            int effImgWidth_;
            
            /** Type of the input image's raster (only RGBa32 and gray supported so far)
             */
            BaseRasterType  rasterType_;
            
            /** number of rows of the warping grid
             */
            int gridRows_;
            
            /** number of cols of the warping grid
             */
            int gridCols_;
            
            /** gridRows_ x gridCols_ array of 2D grid points
             */
            float*** xy_;
            
            /** gridRows_ x gridCols_ array of original 2D grid points
             */
            float*** xy0_;
            
            /** distance between vertical grid points
             */
            float verticalSpacing_;
            
            /** distance between horizontal grid points
             */
            float horizontalSpacing_;
        
            /** texture image
             */
            const RasterImage* imgIn_;
            
            /** output image
             */
            RasterImage* imgOut_;
            
            /** 1D raster of the texture image
             */
            const unsigned char* imgIn1D_;
            
            /** 1D raster of output image
             */
            unsigned char* imgOut1D_;
            
            /** warp the (pre-specified) input image into the (prespecified) output image
             *  using the new grid point coordinates specified
             *  @param  xy      new coordinates of the grid points
             */         
            virtual void warp_(float*** xy) = 0;


            /** Checks whether the grid coordinates specified conform to the grid 
             *  displacement modes set.
             *  @param  grid displacement array to check
             *  @see innerMode_
             *  @see outerMode_
             */
            bool isValidDisplacement_(const float*** const xy) const;
            
            /** Allocates new grids of points at the (pre-approved) dimensions
             */
            void allocateGrids_(void);
            
            /** Initializes the grids once its dimensions and that of the image
             *  are known
             */
            void initializeGrids_(void);
            
            /** Deletes the pre-allocates grids of points (grid resize or destruction)
             */
            void deleteGrids_(void);
            
            /** Copy constructor (disabled)
             *
             *  This constructor is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the prvious one (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            ImageWarper(const ImageWarper& obj);

            /** Copy operator: disabled.
             *
             *  This operator is disabled.  I created it to eliminate compiler warnings, but
			 *	it cannot be implemented for the same reasons as for the copy constructor (constness).
             *
             *  @param  obj   reference of the object to copy
             */
            const ImageWarper& operator =(const ImageWarper& obj);


    };
}

#endif  //  IMAGE_WARPER_H
