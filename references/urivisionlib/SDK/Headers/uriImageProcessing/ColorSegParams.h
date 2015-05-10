/*  NAME:
        ColorSegParams.h
 
    DESCRIPTION:
        ColorSegParams public header.
 
    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.
 
        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_COLOR_SEG_PARAMS_H
#define URIVL_COLOR_SEG_PARAMS_H

#include <string>
//
#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif

#include "ColorRegions.h"

namespace uriVL
{
    /** Data structure that stores parameters for a color/gray-based segmentation
     */
    class ColorSegParams
    {
    
        public:
        
            /** Constructor
             */
            ColorSegParams(void);
            
            /** Destructor
             */
            ~ColorSegParams(void);
        
            /** Is this a color region search? (if not, this is a gray-level region search).
             *  This redundant info is needed because we cannot be sure that the unused
             *  region pointer colorReg or grayReg will be properly set to NULL
             */
            bool colorMode;

            /** The number of color or grey-level regions sought
             */
            int	nbModes;

            /** When the ColorSegParams is used by a Thresholder outputing to a LabelRaster,
             *  what value should be assigned to the no-match pixels?
             */
            char noMatchVal;
        
            /** Array of color region definitions.  Should be NULLfor a gray-level search
             */
            ColorRegion* colorReg;

            /** Array of color region definitions.  Should be NULL for a color search
             */
            GrayRegion* grayReg;
            
            /** When the ColorSegParams is used by a Thresholder outputing to a RasterImage,
             *  how should the no-match pixels be found?
             */
            FillingMode fillMode;
            
		private:
		
			/**	Copy constructor.  Declared just so gcc won't complain.
			 *	Not implemented.
			 *	@param	param	the color segmentation param struct to copy
			 */
			ColorSegParams(const ColorSegParams& param);
			
			/**	Copy operator.  Declared just so gcc won't complain.
			 *	Not implemented.
			 *	@param	param	the color segmentation param struct to copy
			 */
			const ColorSegParams& operator = (const ColorSegParams& param);
			
			
    };
}

#endif  //  COLOR_SEG_PARAMS_H
