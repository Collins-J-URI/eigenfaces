/*  NAME:
        Histogram.h

    DESCRIPTION:
        Histogram public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef URIVL_HISTOGRAM_H
#define URIVL_HISTOGRAM_H

#include <vector>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage_gray.h"
#include "RasterImage_HSV.h"


#define HIST_DEFAULT_RANGE_MIN      0
#define HIST_DEFAULT_RANGE_MAX      255
//
#define HIST_DEFAULT_RANGE_MIN_UC   ((unsigned char)0)
#define HIST_DEFAULT_RANGE_MAX_UC   ((unsigned char)255)
//
#define HIST_DEFAULT_NB_VALS        256

namespace uriVL
{
    /** Enumerated types for all histogram rendering modes supported by this
     *  class
     *      <ul><li>LIN_LIN - x and y both linear (classical plot)
     *          <li>LIN_LOG - x is linear, y is log
     *          <li>LOG_LIN - x kkis log, y is linear
     *          <li>LOG_LOG - x and y are both both log
     *          <li>LIN-POLAR - linear radius polar plot
     *          <li>LOG_POLAR - log-polar plot
     *      </ul>
     */
    typedef enum HistogramRenderMode {
                                        LIN_LIN = 200,    
                                        LIN_LOG,
                                        //LOG_LIN,
                                        //LOG_LOG,
                                        LIN_POLAR,
                                        LOG_POLAR   
    } HistogramRenderMode;
    

    /** Tists the types of data supported by this class
     */
    typedef enum HistogramDataType {
                                        NO_TYPE_HIST = -1,
                                        FLOAT_HIST = 300,
                                        INT_HIST,
                                        UCHAR_HIST
    } HistogramDataType;
    
    
    /** Implements the Histogram class.
     *
     *  This class computes, stores, and returns a 1D array of <code>long</code>
     *  storing the histogram of a given raster image.  It can also render this
     *  histogram in a window.  You should use this class if you want to display
     *  an image's histogram (for verification purposes) or want to do your own
     *  calculations with the histogram array.  Otherwise, the
     *  <code>HistogramCalculator</code> class provides a number of functions.
     *  such as median computation and the generation of gray thresholding regions
     *
     *  Version history
     *      - 2007/03/24 [jyh]      complete rewrite, combined with ImageHistogram subclass
     *      - 2005/04/24 [jyh]      first implementation.
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT Histogram : public DrawableObject2D
    {
        public:

            /** Creates an histogram for a float data set.
             *  @param  x           data we want to compute the histogram of
             *  @param  n           number of data points
             *  @param  nbVals      number of values in the histogram
             *  @param  rangeMin    lower bound of the range of data points
             *  @param  rangeMax    upper bound of the range of data points
             */
            Histogram(const float* x, int n, 
                      int nbVals = HIST_DEFAULT_NB_VALS,
                      float rangeMin = HIST_DEFAULT_RANGE_MIN,
                      float rangeMax = HIST_DEFAULT_RANGE_MAX);

//            /** Creates an histogram for a data set.
//             *  @param  x   data we want to compute the histogram of
//             */
//            Histogram(const vector<float> *x);

            /** Creates an histogram for an int data set.
             *  @param  x           data we want to compute the histogram of
             *  @param  n           number of data points
             *  @param  nbVals      number of values in the histogram
             *  @param  rangeMin    lower bound of the range of data points
             *  @param  rangeMax    upper bound of the range of data points
             */
            Histogram(const int* x, const int n, 
                      int nbVals = HIST_DEFAULT_NB_VALS,
                      int rangeMin = HIST_DEFAULT_RANGE_MIN,
                      int rangeMax = HIST_DEFAULT_RANGE_MAX);


//            /** Creates an histogram for a data set.
//             *  @param  x   data we want to compute the histogram of
//             */
//            Histogram(const vector<int> *x);

            /** Creates an histogram for an unsigned char data set.
             *  @param  x           data we want to compute the histogram of
             *  @param  n           number of data points
             *  @param  nbVals      number of values in the histogram
             *  @param  rangeMin    lower bound of the range of data points
             *  @param  rangeMax    upper bound of the range of data points
             */
            Histogram(const unsigned char* x, const int n, 
                      int nbVals = HIST_DEFAULT_NB_VALS,
                      unsigned char rangeMin = HIST_DEFAULT_RANGE_MIN_UC,
                      unsigned char rangeMax = HIST_DEFAULT_RANGE_MAX_UC);


            /** Creates an histogram for a data set.
             *  @param  x   data we want to compute the histogram of
             *  @param  n   number of data points
             */
            Histogram(const unsigned char* x, int n);

//            /** Creates an histogram for a data set.
//             *  @param  x   data we want to compute the histogram of
//             */
//            Histogram(const vector<unsigned char> *x);


            /**
             *  Destructor
             */
            virtual ~Histogram(void);

            /** returns the histogram values
             *  @return     histogram values
             */
            const long *getHistogram(void) const;

            /**
             *  Informs the vector field object of the dimensions of graphic context
             *  used for its rendering.  These dimensions override the default settings
             *  for the class.
             *  These dimensions are only needed if the user wants to display the
             *  VectorField "shifted" (boolean true passed to render).  If only the
             *  0-based rendering is used (default call to render), then this information
             *  is not needed.
             *  @param  theWidth    width of the window where rendering takes place
             *  @param  theHeight   height of the window where rendering takes place
             *
             *  @see    defaultWindowWidth_
             *  @see    defaultWindowHeight_
             *  @see    setWindowDimensions
             *  @see    hasIndivWindowDimensions_
             *  @see    setDefaultWindowDimensions
             */
            void setWindowDimensions(int theWidth, int theHeight);

            /** Specifies the a color for the background of the histogram, when rendered.
             *  This function overrides (and calls) the virtual function defined in the
             *  parent class DrawableObject2D.
             *
             *  @param  theRed    red value for the background
             *  @param  theGreen  green value for the background
             *  @param  theBlue   blue value for the background
             */
            void setBackgroundColor(float theRed, float theGreen, float theBlue);

            /** Specifies the a color for the histogram's axes, when rendered.
             *
             *  @param  theRed    red value for the axes
             *  @param  theGreen  green value for the axes
             *  @param  theBlue   blue value for the axes
             */
            void setAxesColor(float theRed, float theGreen, float theBlue);

            /** Specifies the a color for the title of the histogram, when rendered.
             *
             *  @param  theRed    red value for the title
             *  @param  theGreen  green value for the title
             *  @param  theBlue   blue value for the title
             */
            void setTitleColor(float theRed, float theGreen, float theBlue);

            /** Defines the rendering mode for the histogram.
             *  A subclass may override this function to limit the choice of rendering modes
             *  availlable (for example, in my opinion a polar rendering makes no sense for 
             *  a gray-level image, so ImageHistogram only accepts LIN_LIN and LIN_LOG modes)
             *  @param  renderMode  rendering mode
             *  @see renderMode_
             */
            virtual void setRenderMode(HistogramRenderMode mode);
            

            /** Called to tell whether or not to draw axes when rendering the histogram
             *  @param  drawAxes    true if the axes should be drawn
             */
            void setDrawAxes(bool drawAxes);

            /** Called to tell whether or not to draw a title when rendering the histogram
             *  @param  drawTitle    true if the label should be drawn
             */
            void setDrawTitle(bool drawTitle);

            /** Applies a Transformation2D to this object
             *  @param  theTransf   the Transformation2D to apply to this object
             */
            void applyTransformation(const Transformation2D* theTransf);

            /**	Rotates the object by the angle specified (in radians)
             *	If rotationCenter is null, then the centroid of the object is used
             *	as center of rotation
             *	@param	angle	the angle of the rotation (in radians)
             *	@param	rotCenter	the center of the rotation
             */
            void rotate(float angle, const ImagePoint* rotCenter = NULL);


		protected:
		
            /** <code>true</code> if histogram labels should be drawn (false by default)
             */
            bool drawAxes_;

            /** <code>true</code> if the histogram's title should be drawn (false by default)
             */
            bool drawTitle_;
            
            /** color for the axes (black by default)
             *  @see setAxesColor
             */
            float* axesColor_;

            /** background color to render the histogram (white by default)
             *  @see setBackgroundColor
             */
            float* backgroundColor_;

            /** color for the title (black by default)
             *  @see setTitleColor
             */
            float* titleColor_;

			/** stores a title for the histogram.
             */
            char   *title_;

            /** Data type of this histogram
             */
            HistogramDataType dataType_;
            
            /** stores the lower bound of an unsigned char histogram's range.  This is not the
             *  the same thing as the minimum value of the image.
             *  So far by default it is 0.
             */
            unsigned char   ucRangeMin_;

            /** stores the upper bound of an unsigned char histogram's range.  This is not the
             *  the same thing as the maximum value of the image
             *  So far by default it is 255.
             */
            unsigned char   ucRangeMax_;

            /** stores the lower bound of an int histogram's range.  This is not the
             *  the same thing as the minimum value of the image.
             *  So far by default it is 0.
             */
            int   iRangeMin_;

            /** stores the upper bound of an int histogram's range.  This is not the
             *  the same thing as the maximum value of the image
             *  So far by default it is 255.
             */
            int   iRangeMax_;

            /** stores the lower bound of a float histogram's range.  This is not the
             *  the same thing as the minimum value of the image.
             *  So far by default it is 0.
             */
            float   flRangeMin_;

            /** stores the upper bound of a float histogram's range.  This is not the
             *  the same thing as the maximum value of the image
             *  So far by default it is 255.
             */
            float   flRangeMax_;

			/** stores the number of discrete values for which the frequency is
             *  calculated.
             *  So far by default it is 256.
             */
            int   histVals_;

            /**
             *  Stores the histogram values.
             */
            long        *hist_;

            /**
             *  number of measurements that make up the histogram.
             */
            long        nbPts_;

            /** Rendering mode for this histogram
             *  @see setRenderMode
             */
            HistogramRenderMode renderMode_;
            
            
            /** true when all the scaling parameters have been set for the rendering of the 
             *  histogram in the selected rendering mode
             */
            mutable bool isRenderReady_;
            
            /** Vertical shift for rendering (from left edge)
             */
            mutable int vertPad_;
            
            /** Horizontal shift for rendering (from left edge)
             */
            mutable int horPad_;
            
			/** Vertical rendering scale
             */
            mutable float vertScale_;
            
            /** Horizontal rendering scale
             */
            mutable float horScale_;
            
            /** Actual width of the rendered histogram (in pixel)
             */
            mutable int renderWidth_;
            
            /** Actual height of the rendered histogram (in pixel)
             */
            mutable int renderHeight_;
            

           /** Default constructor.
             */
            Histogram(void);

            /** Empty implementation mandated by pure virtual parent class DrawableObject2D.
             *
             *
             *  @param  x       X coordinate of the image's origin
             *  @param  y       Y coordinate of the image's origin
             *  @param  theWidth    number of columns occupied by the histogram
             *  @param  theHeight   number of rows occupied by the histogram
             */
            void setBoundRect_(int x, int y, int theWidth, int theHeight);


            /** Empty implementation mandated by pure virtual parent class DrawableObject2D.
             *
             *  @param  x           X coordinate of the valid data upper left corner
             *  @param  y           Y coordinate of the valid data upper left corner
             *  @param  theWidth    number of valid columns
             *  @param  theHeight   number of valid rows
             */
            void setValidRect_(int x, int y, int theWidth, int theHeight);

            /**
             *  Renders the histogram at the depth specified and using the colors
             *  specified.
             *
             *  @param  scaledLeft  x coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledTop	y coordinate of the object's upper-left corner when rendered scaled
			 *	@param	scaledRight x coordinate of the object's lower-right corner when rendered scaled
			 *	@param	scaledBottom y coordinate of the object's lower-right corner when rendered scaled
             *  @param  theZ    Depth at which this object is to be rendered.
             *  @param  shifted     true if the object should be rendered at its true ImageRect
             *                      position, false if its upper left corner should be at 0,0.
             *  @param  origin      location of the object's upper-left corner in the rendering
             */
            virtual void render_(int scaledLeft, int scaledTop, int scaledRight, int scaledBottom,
								 float theZ, bool shifted, const ImagePoint* origin) const;


            /** Computes the translation and scaling parameters for the rendering mode 
             *  set by setRenderMode
             */
            void computeRenderingParameters_(void) const;
            

            /** The function that does all the work
             */
            void computeHistogram_(void);

		private:
		
			/**Copy constructor: not implemented
			 *	@param	hist	the object to copy
			 */
			Histogram(const Histogram& hist);

			/**Copy operator: not implemented
			 *	@param	hist	the object to copy
			 */
			const Histogram& operator = (const Histogram& hist);

            /** Calculates the object's bounding rect
			 *	@param	rect	the bounding box to update
             */
            void computeBoundingRect_(ImageRect& rect) const;


            /** Calculates the object's valid box
			 *	@param	rect	the bounding box to update
             */
            void computeValidRect_(ImageRect& rect) const;


            /** Computes the contour's centroid.
			 *	@param	cent	the centroid point to update
             */
			void computeCentroid_(ImagePoint_F& cent) const;

            /** Computes the object's center of mass by calculating the  
             *  average point
			 *	@param	cent	the center of mass point to update
             */
            void computeCenterOfMass_(ImagePoint_F& cent) const;

    };
}   //  namespace uriVL

#endif  //  HISTOGRAM_H
