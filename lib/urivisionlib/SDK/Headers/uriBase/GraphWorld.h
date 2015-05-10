/*  NAME:
        GraphWorld.h

    DESCRIPTION:
        GraphWorld public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_GRAPHWORLD_H
#define URIVL_GRAPHWORLD_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "ImageRect.h"

namespace uriVL
{
    /** Enumerates the base image raster types
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    typedef enum BaseRasterType {
            kUndefRasterType = -1,
            kBinaryRaster=1,
            kGrayRaster,        //  8 bits per pixel gray-level
            kLongGrayRaster,    //  12-16 bits per pixel gray-level
            kaRGB16Raster,      //  16 bits per pixel color (1a-5r-5g-5b)
            kRGB24Raster,       /// 24 bits per pixel color (8r-8g-8b)
            kaRGB32Raster,      //  32 bits per pixel color (8a-8r-8g-8b)
            kRGBa32Raster       /// 32 bits per pixel color (8r-8g-8b-8a)
    } BaseRasterType;


    #if URIVL_USE_QUICKTIME

        class _GraphWorld_QT;

    #else

        class _GraphWorld_gen;

        /** Placeholder until we define the Movie type
         */
        typedef char*   Movie;
        
        /** Placeholder until we define the GDHandle type (if ever)
         */
        typedef char*   GDHandle; 
        
        /** Placeholder until we define the GraphicsImportComponent type (if ever)
         */
        typedef char*   GraphicsImportComponent; 
        
        /** Placeholder until we define the GraphicsImportComponent type (if ever)
         */
        typedef char*   SeqGrabComponent;
        
        
        /** Placeholder until we define the GraphicsImportComponent type (if ever)
         */
        typedef char*   SGChannel;
        
        /** Placeholder until we define the GraphicsImportComponent type (if ever)
         */
        typedef char*   ImageSequence;

    #endif

    /** Implements the GraphWorld class.
     *
     *  Graphworld was originally a wrapper class for the GWorld structure of QuickDraw 
     *  and QuickTime (hence some of the remaining QT-isms in the names of its members).
     *  This class is now a pure virtual subclass, with the following subclasses:
     *  <ul>
     *      <li> _GraphWorld_QT: QuickTime implementation
     *      <li> _GraphWorld_gen: "generic" implementation, independent from any library
     *                              other than standard C++
     *      <li> maybe later (not by me) GraphWorld_DX: DirectX implementation
     *
     *
     *  Basically, I had the choice between two implementations:
     *      - public inheritance:  Define GraphWorld as a pure virtual class and provide 
     *          complete implementations in subclasses Graphworld_QT, _GraphWorld_gen, 
     *          GraphWorld_DX, etc.
     *      - composition:  Define variables of type Graphworld_QT or _GraphWorld_gen
     *          inside the Graphworld class.
     *
     *  I opted for the second solution because it allows me to restrict the platform-testing
     *  code to a single class.  Otherwise I would have had to put platform-specific code into
     *  all of the RasterImage classes.
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
    class URIVL_EXPORT GraphWorld
    {
        public:
            /** Constructor.
             *  Creates a new GraphWorld at the dimensions and pixel depth specified.
             *
             *  @param  pixelDepth  number of bits per pixel (8, 16, 24, 32)
             *  @param  boundsRect  bounding rectangle for this raster
             */
            GraphWorld(int pixelDepth, const ImageRect* boundsRect);

            /** Constructor.
             *  Creates a new GraphWorld at the dimensions and pixel depth specified.
             *
             *  @param  pixelDepth  number of bits per pixel (8, 16, 24, 32)
             *  @param  nbRows  number of rows of the raster
             *  @param  nbCols  number of columns of the raster
             */
            GraphWorld(int pixelDepth, int nbRows, int nbCols);

            /** Copy constructor.
             *
             *  @param  theGWorld   the GraphWorld to clone
             */
            GraphWorld(const GraphWorld* theGWorld);

            /** Copy constructor.
             *
             *  @param  theGWorld   the GraphWorld to clone
             */
            GraphWorld(const GraphWorld& theGWorld);

            /**  Destructor
             */
            ~GraphWorld(void);

            /** Copy constructor.
             *
             *  @param  theGWorld   the GraphWorld to clone
             */
            const GraphWorld& operator = (const GraphWorld& theGWorld);

            /**
             *  Returns the number of bits per pixel (pixel depth)
             *
             *  @return     pixel depth
             */
            int getPixelDepth(void) const;

            /** Returns the index of the base raster type for this GWorld.
             *  The base raster type is one of those defined by the BaseRasterType
             *  enum type.
             *
             *  @return     index of the base raster type for this GWorld
             *  @see    BaseRasterType
             */
            int getBaseRasterType(void) const;


            /**
             *  Returns the <i>effective</i> number of columns in the GraphWorld.
             *
             *  Base rasters have <b>padded</b> rows.  This function returns the number of
             *  bytes per row divided by the pixel depth.
             *
             *  @return     <i>effective</i> number of columns in the GraphWorld
             */
            int getEffectiveNbCols(void) const;

            /**
             *  Returns the number of rows in the GraphWorld.
             *
             *  @return     number of rows in the GraphWorld
             */
            int getNbRows(void) const;

            /**
             *  Returns the number of total number of bytes in the GraphWorld.
             *
             *  Beware:     Base rasters have <b>padded</b> rows.  You cannot rely on the
             *              fact that the number of bytes in a row is equal to the number of
             *              columns multiplied by by the number of bytes per pixel.
             *
             *  @return     number of bytes in the GraphWorld
             */
            long getTotalByteCount(void) const;

            /**
             *  Returns the number of bytes on each row of the GraphWorld.
             *
             *  Beware:     Base rasters have <b>padded</b> rows.  You cannot rely on the
             *              fact that the number of bytes in a row is equal to the number of
             *              columns multiplied by by the number of bytes per pixel.
             *
             *  @return     number of bytes on a row of the GraphWorld
             */
            int getBytesPerRow(void) const;

            /*  Does a CopyBits to copy one GraphWorld into another.  Uses the intersection of
             *  the valid data rect as source and writes into the destination provided (fails
             *  if the latter is not contained within the bounding rect of the destination
             *  GraphWorld).  This may result in a rescaling of the raster and a change of pixel
             *  @param  destination     the GraphWorld into which to copy this one
             *  @param  sourceRect      part of this raster to copy
             *  @param  destRect        destination rectangle
             *  @param  inverseGray     true for a gray-color color-gray copy, falose otherwise
             */
            void copyInto(GraphWorld *destination, const ImageRect* sourceRect, 
                                 const ImageRect* destRect, bool inverseGray) const;


            /** Renders the raster, using the OpenGL drawPixel function
             */
            void draw(void);

            /** Returns the base address of the Graphworld's raster (QuickDraw PixMap)
             *  @return     base address of the Graphworld's raster
             */
            unsigned char* getRaster(void);

            /** Sets this GraphWorld object as the destination of a movie.  The movie will play
             *  (render) into this object
             *  @param  theMovie the movie that will play into this object
             */
            void setAsMovieGraphWorld(Movie theMovie);

            /**
             *  Sets this GraphWorld object as destination for a graphic importer.
             *
             *  When data is read from an image file, a GWorld must be set as the
             *  destination for the graphic importer.  This member function allows to 
             *  define this GraphWorld as the destination for the importer,
             *  so as to avoid unnecessary blitting between buffers.
             *
             *  @param  importer the graphic importer that will draw into this object
             *  @see   restoreGraphWorld
             */
            void setAsImporterGraphWorld(GraphicsImportComponent importer);


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
//             *  @return	error code (0 if all OK)
//             *
//             *  @see   restoreGraphWorld
//             */
//            ErrorCode setAsGrabberGraphWorld(FrameGrabber *grabber);


#if URIVL_USE_QUICKTIME
            /** Adds this image to the video digitizer record of a buffer ring
             *  @param  bufferRec   buffer record to add information to
             */
            void addToBufferRing(VdigBufferRec *bufferRec);
#endif

            /** Sets this object as the current GWorld for the QuickDraw engine, preserves
             *  the previous GWorld and graphic device in objects defined by the caller.
             *  Stores copies of these references to restore them later.
             *  @param  theGWorld   reference to GraphWorld where to store previous settings
             */
            void setGraphWorld(const GraphWorld *theGWorld);

            /** Restores previous current GraphWorld and graphic device settings
             */
            void restoreGraphWorld(void);

		
        protected:

            #if URIVL_USE_QUICKTIME
                /** Platform-specific implementation of the graphworld: QuickTime version
                 */
                _GraphWorld_QT   *graphWorld_;
            #else
                /** Platform-specific implementation of the graphworld: "generic version
                 */
                _GraphWorld_gen  *graphWorld_;
            #endif
			
		
		#if URIVL_USE_QUICKTIME
			friend class _FrameGrabber_QT;
		#else
			friend class _FrameGrabber_libvidcap;
		#endif
		
    };
}

#endif
