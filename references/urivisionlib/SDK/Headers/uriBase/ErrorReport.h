/*  NAME:
        ErrorReport.h

    DESCRIPTION:
        ErrorReport public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_ERROR_REPORT_H
#define URIVL_ERROR_REPORT_H

#include <string>
#include <cstdlib>
//
#ifdef  URIVL_DEBUG_ASSERT
    #include <cassert>
#endif

#include "uriVisionLib.h"

namespace uriVL
{
    /** List of all the error codes for the library
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    enum ErrorCode
    {
        kNoError = 0,
        //
		kUnknownError = 1,
		kNoCode = 2,                        //  Generic error
		//
        kNullParameterError,
        kInvalidReferenceError,
        kInvalidParameterError,
        kNoninitializedObjectError,
        kFunctionNotImplemented,
        kCaseNotImplemented,
		kMemoryCopyError,
        kMiscError,
        kNullArrayError,
        kInvalidLockAccess,
        kAccessToLockedData,
        //
        //----------------------------
        //
        kUriBaseError = 1000,               //  uriBase-related errors ---------------> 1xxx
        //
        kRasterImageError = 1100,               //  Error in the bowels of RasterImage
        kNullRasterImageError,
        kNullVectorFieldError,
        kRasterAllocationFailure = 1110,
        kRasterInvalidDimensions,
        kRasterDimensionsDontMatch = 1120,
        kNullRasterError = 1130,
        kNullRectangleError = 1140,
        kNullImagePointError,
        kInvalidRectangleError,
        kInvalidRasterType,
        //
        kGWorldRelatedError = 1200,
        kGWorldAllocationError = 1201,
        kGWorldIsNullError = 1202,
        kGWorldNullPixMapError,
        kGWorldInvalidPixelDepth = 1210,        //  the pixel depth is not 8, 16, [24], or 32
        kGWorldIncompatiblePixelDepth,          //  Mac OS & 24 bit GWorlds <--> movies
        kGWorldInvalidCLUT = 1220,
        kGWorldCannotLockPixels = 1230,
        kGWorldCannotUnlockPixels = 1231,
        //
        kQuickTime_relatedError = 1500,         //  QuickTime related error codes --->  14xx
        kQuickTimeNotInstalled,
        kPixMapLockFailed,
        kEnterMoviesError,
        //
        //----------------------------
        //
        kImageProcessingError = 2000,       //  uriImageProcessing error codes -------> 2xxx
        //
        kFilterAllocationError,
        kNullLabelRasterError,
        //
        kNullTransformation2D = 2100,
        kInvalidTransformation2D = 2110,
        //
        //----------------------------
        //
        kComputerVisionError = 3000,        //  uriComputerVision error codes --------> 3xxx
        //
        kNullTransformation3D = 3100,           //  transformation3D-related errors --> 31xx
        kInvalidTransformation3D,
        kNullEntity3D = 3110,
        kInvalidEntity3D,
        //
        kCameraError = 3200,                    //  camera-related errors ------------> 32xx
        kInvalidCameraType = 3210,
        kInvalidIntrisicParameterType = 3220,
        //
        kStereoMatchingError = 3400,            //  stereo-related errors ------------> 34xx
        //
        //----------------------------
        //
        kTrackingError = 4000,              //  uriTracking error codes --------------> 4xxx
        //
        //----------------------------
        //
        kVideoSourceError = 5000,           //  uriVideoSource error codes -----------> 5xxx
        kVideoSourceNotSetError = 5010,           
        kVideoSourceFileNotFoundError = 5020,           
        //
        kVideoInitializingError = 5100,
		kOpenVideoComponentError = 5110,
		kInvalidGrabRectDimensions = 5120,
        //
        kVideoBufferAllocError = 5200,
        //
        kVideoAcquisitionError = 5300,
		kVideoDigitizerComponentError = 5310,
        //
        kUnsupportedImageFormatError = 5400,
        //
        //----------------------------
        //
        kMovieEditingError = 6000,          //  uriMovieEditing error codes ----------> 6xxx
        kInitializeMovieError,
        kMovieToolboxError,
        kMovieStartPointError,
        kFileIOError = 6100,                    //  File IO error codes --------------> 61xx
		kDirectoryNullPathError,
		kFileNullPathError,
		kDirectoryListError,
        kDirectoryNotFoundError = 6120,				//	Not found errors -------------> 612x
        kFileNotFoundError,
		kDirectoryCloseError = 6130,				//	Open-close errors ------------> 613x
        kFileOpenWriteError,
        kFileOpenReadError,
		kFileCloseError,
        kWrongFileTypeError = 6140,					//	File type errors -------------> 614x
        kUnknownFileTypeError,
        kFileReadingError = 6150,					//	Read-write errors ------------> 615x
        kFileWritingError,
        kEndOfFileError,
        //
        //----------------------------
        //
        kNumericalError = 7000,             //  uriNumerical error codes -------------> 7xxx
        //
        kMatrixCalculationError = 7100,          //  Matrix calculation error codes --> 71xx
        kNullMatrixError,
        kMatrixIncorrectDimensionsError,
        kSingularMatrixError,
        kIllConditionedMatrixError,
        //
        kCalculusError = 7200,
        kNullFunctionError,
        kDataRangeError,
        //
        //
        //----------------------------
        //
        kGraphicsError = 8000,              //  uriGraphics error codes --------------> 8xxx
        kOpenGLNumAllocError,
        //
        kOpenGL_relatedError = 8100,            //  OpenGL-related error codes -------> 81xx
        kOpenGLDrawModuleError,
        //
        kGLUT_relatedError = 8200,              //  GLUT-related error codes ---------> 82xx
        //
        //----------------------------
        //
        //
        
        kMaxErrorCodeID
    };


    /** Implements the ErrorReport class.
     *
     *  An ErrorReport is a form of exception.  Instead of defining a class hierarchy for 
	 *	exceptions in the library, we have chosen the path of having a single class with
	 *	an error code and an error string as instance variables.
     *
     *  Version history
     *      - 2004/02/20 [jyh]      first implementation.
	 *		- 2008/x/x	 [jyh]		added appendToMessage
     *
     *  @author jean-yves herve', 3D Group,
     *                              Department of Computer Science and Statistics,
     *                              University of Rhode Island
     */
    class URIVL_EXPORT ErrorReport
    {
        public:
			/**	Creates a new ErrorCode object to throw.
			 *	@param	theCode		code identifying the type of error encountered
			 *	@param	theMessage	string with an error message
			 */
            ErrorReport(ErrorCode theCode, const std::string& theMessage);
			
			/**	Destructor
			 */
            ~ErrorReport(void);
            
			/**	Returns this object's error code
			 *	@return		this object's error code
			 */
            ErrorCode getCode(void ) const;
			
			
			/**	Appends a string to the error message
			 *	@param	addMsg the string to add to the message
			 */
			void appendToMessage(const std::string& addMsg);
			

            /** When you call this, you're responsible for delete[]'ing the 
             * returned pointer.
			 *	@return a const C-style error string with an error message
			 */
            const char*  getMessage(void) const;

        private:

			/**	Code identifying the type of error encountered
			 */
            ErrorCode   code_;
			
			/**	String giving the type of the error and its call sequence 
			 */
			std::string message_;
    };


    #if URIVL_DEBUG_ASSERT
        #define FAIL_CONDITION(a, b, c) assert(!(a))
        #define REPORTS_EXCEPTIONS
    #else
        #define FAIL_CONDITION(a, b, c) if (a) throw ErrorReport(b, c)
        #define REPORTS_EXCEPTIONS throw(ErrorReport)
    #endif


}   //  namespace uriVL

#endif  //  ERROR_REPORT_H
