/*  NAME:
        _GraphWorld_QT.h

    DESCRIPTION:
        _GraphWorld_QT public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_GRAPHWORLD_QT_H
#define URIVL_GRAPHWORLD_QT_H

// should only be loaded for Quicktime version of the library
#if URIVL_USE_QUICKTIME

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "GraphWorld.h"

namespace uriVL
{

    /** Implements the GraphWorld class using the QuickTime library/framework.
     *
     *  Graphworld_QT is a wrapper for the GWorld structure of QuickDraw and
     *  QuickTime.
     *
     *  The base subclasses of RasterImage correspond to the base raster types.  They are:
     *      - RasterImage_gray for 8-bit gray level images
     *      - RasterImage_rgb16 for 16-bit [1 alpha, 5 red, 5 green, 5 blue] color images
     *      - RasterImage_RGB for 24-bit [8 red, 8 green, 8 blue] color images
     *      - RasterImage_RGBa for 32-bit [8 alpha, 8 red, 8 green, 8 blue] color images
     *  These base subclasses can be instanciated and are the ones most commonly used for
     *  image processing and computer vision purposes.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT _GraphWorld_QT
    {
        private:

            /** Reference to the QuickDraw <code>GWorldPtr</code> structure this object serves
             *  as a wrapper to.
             */
            GWorldPtr       gworld_;

            /** Number of rows of the raster
             */
            int nbRows_;
            
            /** Index for the base raster type.  Any RasterImage object is derived from one
             *  of the five base types; binary, gray, RGB, RGBa, aRGB16.
             */
            BaseRasterType  baseType_;
            
            /** Stores the pixel depth (number of bits per pixel)
             */
            int pixelDepth_;

            /** Reference to the graphic device (QuickDraw structure) on which this object
             *  is defined.  Should normally be NULL.
             */
            GDHandle        device_;

            /** Reference to this object's pixel map (QuickDraw structure).
             */
            PixMapHandle    pixmap_;

            /** Color lookup table of this _GraphWorld_QT.  Used for indexed image types
             */
            //ColorTable      clut_;

            /** Stores a reference to the current GWorldPtr (QuickDraw structure)
             */
            GWorldPtr    currentGWorld_;

            /** Stores a reference to the current graphic device (QuickDraw structure)
             */
            GDHandle     currenGDevice_;

            /** Stores a reference to the previous GWorldPtr (QuickDraw structure)
             */
            GWorldPtr    previousGWorld_;

            /** Stores a reference to the previous graphic device (QuickDraw structure)
             */
            GDHandle     previousGDevice_;


            /** Constructor.
             *  Creates a new _GraphWorld_QT at the dimensions and pixel depth specified.
             *
             *  @param  pixelDepth  number of bits per pixel (8, 16, 24, 32)
             *  @param  boundsRect  bounding rectangle for this raster
             */
            _GraphWorld_QT(int pixelDepth, const ImageRect* boundsRect );

            /** Constructor.
             *  Creates a new _GraphWorld_QT at the dimensions and pixel depth specified.
             *
             *  @param  pixelDepth  number of bits per pixel (8, 16, 24, 32)
             *  @param  nbRows  number of rows of the raster
             *  @param  nbCols  number of columns of the raster
             */
            _GraphWorld_QT(int pixelDepth, int nbRows, int nbCols);

            /** Copy constructor.
             *  Currently not implemented.
             *  @param  theGWorld   the _GraphWorld_QT to clone
             */
            _GraphWorld_QT(const _GraphWorld_QT* theGWorld);

            /** Copy constructor.
             *  Currently not implemented.
             *  @param  theGWorld   the _GraphWorld_QT to clone
             */
            _GraphWorld_QT(const _GraphWorld_QT& theGWorld);

            /**  Destructor
             */
            ~_GraphWorld_QT(void);

            /** = operator.
             *  Currently not implemented.
             *  @param  theGWorld   the _GraphWorld_QT to copy
             */
            const _GraphWorld_QT& operator = (const _GraphWorld_QT& theGWorld);


            /** Returns the index of the base raster type for this GWorld.
             *  The base raster type is one of those defined by the BaseRasterType
             *  enum type.
             *
             *  @return     index of the base raster type for this GWorld
             *  @see    BaseRasterType
             */
            int getBaseRasterType_(void) const;


            /**
             *  Returns the number of bits per pixel (pixel depth)
             *
             *  @return     pixel depth
             *  @see pixelDepth_
             */
            int getPixelDepth_(void) const;


//            /**
//             *  Returns the number of bytes per pixel
//             *
//             *  @return     number of bytes per pixel
//             */
//            int getBytesPerPixel_(void) const;
//
//
            /** Returns the height (number of rows) of the raster
             *  @return height of the raster
             */
            int getNbRows_(void) const;
            
            
            /**
             *  Returns the number of bytes on each row of the _GraphWorld_QT.
             *
             *  Beware:     Base rasters have <b>padded</b> rows.  You cannot rely on the
             *              fact that the number of bytes in a row is equal to the number of
             *              columns multiplied by by the number of bytes per pixel.
             *
             *  @return     number of bytes on a row of the _GraphWorld_QT
             */
            int getBytesPerRow_(void) const;


            /**
             *  Returns the <i>effective</i>number of columns in the GraphWorld.
             *
             *  Base rasters have <b>padded</b> rows.  This function returns the number of
             *  bytes per row divided by the pixel depth.
             *
             *  @return     <i>effective</i>number of columns in the GraphWorld
             */
           int getEffectiveNbCols_(void) const;


            /*  Does a CopyBits to copy one _GraphWorld_QT into another.  Uses the intersection of
             *  the valid data rect as source and writes into the destination provided (fails
             *  if the latter is not contained within the bounding rect of the destination
             *  _GraphWorld_QT).  This may result in a rescaling of the raster and a change of pixel
             *  @param  destination     the _GraphWorld_QT into which to copy this one
             *  @param  sourceRect      part of this raster to copy
             *  @param  destRect        destination rectangle
             *  @param  inverseGray     true for a gray-color color-gray copy, falose otherwise
             */
            void copyInto_(_GraphWorld_QT* destination, const ImageRect* sourceRect, 
                            const ImageRect* destRect, bool inverseGray);


            /** Renders the raster, using the OpenGL drawPixel function
             */
            void draw_(void);

            /** Returns the base address of the Graphworld's raster (QuickDraw PixMap)
             *  @return     base address of the Graphworld's raster
             */
            unsigned char* getRaster_(void) const;

            /** Sets this _GraphWorld_QT object as the destination of a movie.  The movie will play
             *  (render) into this object
             *  @param  theMovie the movie that will play into this object
             */
            void setAsMovieGraphWorld_(Movie theMovie);

            /**
             *  Sets this _GraphWorld_QT object as destination for a graphic importer.
             *
             *  When data is read from an image file, a GWorld must be set as the
             *  destination for the graphic importer.  This member function allows to 
             *  define this GraphWorld as the destination for the importer,
             *  so as to avoid unnecessary blitting between buffers.
             *
             *  @param  importer the graphic importer that will draw into this object
             *  @see   restoreGraphWorld
             */
            void setAsImporterGraphWorld_(GraphicsImportComponent importer);

//            /**
//             *  Sets this GraphWorld as destination for a video capture.
//             *
//             *  When a movie is played or live video is grabbed, a GWorld must be set as the
//             *  destination for the video stream.  This member function allows to define the
//             *  RasterImage's base raster as the destination for the video stream, so as to
//             *  avoid unnecessary blitting between buffers.
//             *
//             *  This function keeps a copy of the previous GraphWorld, so that it can restore
//             *  it later on.
//             *  @param  grabber the FrameGrabber that will play into this object
//             *  @return	MacOS error code (0 if all OK)
//             *  @see   restoreGraphWorld
//             */
////            OSErr setAsGrabberGraphWorld_(FrameGrabber *grabber);
//
//
//            /** Adds this image to the video digitizer record of a buffer ring
//             *  @param  bufferRec   buffer record to add information to
//             */
////            void addToBufferRing_(VdigBufferRec *bufferRec);


            /** Sets this object as the current GWorld for the QuickDraw engine, preserves
             *  the previous GWorld in  a _GraphWorld_QT object defined by the caller.  The Graphic
             *  Device is set to NULL.
             *  @param  theGWorld   reference to _GraphWorld_QT where to store previous settings
             *  Stores copies of these references to restore them later.
             */
            void setGraphWorld_(const _GraphWorld_QT* theGWorld);

            /** Restores previous current _GraphWorld_QT and graphic device settings
             */
            void restoreGraphWorld_(void);

            /** Sets this GWorld as the current graphic port (QuickDraw call)
             *  The current device is set to NULL
             */
            void setGraphWorld_(void);

            /** Locks this GraphWorld's PixMap
             */
            void lock_(void);

            /** Unlocks this GraphWorld's PixMap
             */
            void unlock_(void);


        friend class GraphWorld;
		friend class _FrameGrabber_QT;

    };
}

#endif //	URIVL_USE_QUICKTIME

#endif  //  GRAPHWORLD_QT_H
