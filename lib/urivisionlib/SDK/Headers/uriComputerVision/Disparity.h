/*  NAME:
        Disparity.h
 
    DESCRIPTION:
        Disparity public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/

#ifndef     URIVL_DISPARITY_H
#define     URIVL_DISPARITY_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "Vector2D_F.h"


namespace uriVL
{   
	/**	Generic class for all sort of disparities (stereo, motion in particular).  The
	 *	disparity information proper is held by a Vector2D_F object storing a 2D
	 *	displacement, a boolean that indicates whether or not that displacement has 
	 *	subpixel precision, and a "confidence" in the disparity value.
	 *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
	 */
    class URIVL_EXPORT Disparity 
    {
    	public:
    	
            /**
             *  Default Constructor. Creates a zero-disparity zero-confidence object
			 *	@param	isSubpixelDisparity = true if the disparity has subpixel precision
             */
            Disparity(bool isSubpixelDisparity = false);


    		/**
             *  Copy constructor.
             *  @param  disp        disparity object to copy
             */
    		Disparity(const Disparity* disp);

    		/**
             *  Copy constructor.
             *  @param  disp        disparity object to copy
             */
    		Disparity(const Disparity& disp);

    		/**
             *  Constructor.
             *  Allocates a disparity with a confidence.
             *  @param  disp        disparity
             *  @param  score       confidence [0, 1]
             */
    		Disparity(const Vector2D* disp, float score);

    		/**
             *  Constructor.
             *  Allocates a disparity with a confidence.
             *  @param  dx        horizontal component of disparity
             *  @param  dy        vertical coordinate of disparity
             *  @param  score     confidence [0, 1]
             */
    		Disparity(int dx, int dy, float score);

    		/**
             *  Constructor.
             *  Allocates a disparity with a confidence.
             *  @param  dx        horizontal component of disparity
             *  @param  dy        vertical coordinate of disparity
             *  @param  score     confidence [0, 1]
			 *	@param	isSubpixelDisp		true if this object is trully a float disparity
             */
    		Disparity(float dx, float dy, float score, bool isSubpixelDisp=true);


    		/**
             *  Destructor
             */          
    		~Disparity(void);
    		

			/**
             *  Copy operator.
             *
             *  @param      theObj   reference to the object to copy
             */
            const Disparity& operator = (const Disparity& theObj);


    	    /** Indicates whether the disparity computed uses integers of floating point
    	     *  (subpixel) numbers
    	     *  @return     <code>true</code> if this disparity can return a floating point,
    	     *              subpixel disparity value
    	     */
    	    bool isSubpixelDisparity(void) const;

    		/**
             *  Returns the disparity
             *  @return     disparity
             */
    		const Vector2D_F* getDisparity(void) const;
    		
    		/**
             *  Returns the disparity
             *  @return     disparity
             */
    		Vector2D_F* getDisparity(ReadWriteStatus rw);

    		
    		/**
             *  Returns the confidence
             *  @return     confidence
             */
    		float getScore(void);

    	    /**
             *  Assigns new confidence
             *  @param  score     new confidence
             */
    		virtual void setScore(float score);
    		
            /** Assigns new disparity
             *  @param  disp displacement stored in an ImagePoint object
             */
            void setDisparity(const Vector2D* disp);

            /** Assigns new disparity
             *  @param  disp displacement stored in an ImagePoint object
             */
            void setDisparity(const Vector2D_F* disp);


            /** Assigns new disparity
             *  @param  x   x component of the disparity
             *  @param  y   y component of the disparity
			 *	@param	isSubpixelDisp		true if this object is in fact a float disparity
             */
            void setDisparity(int x, int y);

            /** Assigns new subpixel disparity
             *  @param  x   x component of the disparity
             *  @param  y   y component of the disparity
			 *	@param	isSubpixelDisp		true if this object is in fact a float disparity
             */
            void setDisparity(float x, float y, bool isSubpixelDisp);


		
    	private:
        
        	/**	Stores displacement between the left and right
        	 *	matching points
        	 *	@see	isLeftToRight_
        	 */
    		Vector2D_F* disparity_;
    		
            /** Indicates whether the disparity is calculated with subpixel
             *  accuracy
             */
            bool isSubpixelDisparity_;
            
    		/**	Matching confidence, in [0, 1]
    		 */
    		float	score_;       


    };
}
#endif	//	DISPARITY_H
