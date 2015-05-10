/*  NAME:
        VideoDestination.h

    DESCRIPTION:
        VideoDestination public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_VIDEO_DESTINATION_H
#define URIVL_VIDEO_DESTINATION_H

#if !URIVL_XCODE
    #include "ErrorReport.h"
#endif
//
#include "RasterImage.h"


namespace uriVL
{

	//=============================================================================
	//      Some place holder data types needed by non-QT image reader/writer
	//-----------------------------------------------------------------------------

	#if !URIVL_USE_QUICKTIME

		/** Placeholder until we define the FSSpec type (if ever)
		 */
		typedef void*	FSSpec; 

		/** Placeholder until we define the OSErr type (if ever)
		 */
		typedef int		OSErr;
		
	#endif

    /** Parent class for all objects that output raster image objects to files,
     *  videos, VTRs, etc.
     *
     *  Version history
     *      - v. 1.0      05/07/2004  first "working" version
     *      - v. 2.0      05/29/2013  revised to match VideoSource update
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT VideoDestination
    {
        public:
            /**	Default constructor
			 */
            VideoDestination(void);

			/**	Destructor
			 */
            virtual ~VideoDestination(void);

			/**	Use the GUI to assign a new destination
			 */
            virtual void setDestination(void) = 0;

			/**	Outputs the image to the currently assigned destination
			 *	@param the image to output
			 *	@param	writeUpsideDown		true if the image should be outputed upside down
			 *									(temp. setting for some tga files)
			 */
            virtual void outputFrame(const RasterImage* theImagebool, bool writeUpsideDown=false) = 0;


    };
}

#endif
