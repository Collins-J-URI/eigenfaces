/*  NAME:
        uriVisionLib.h

    DESCRIPTION:
        uriVisionLib public header.

    COPYRIGHT:
        (c) 2003-2014, 3D Group for Interactive Visualization
                       University of Rhode Island.

        Licensed under the Academic Free License - v. 3.0
        For more information see http://opensource.org/licenses/academic.php
*/
#ifndef URIVL_H
#define URIVL_H

namespace uriVL
{
    //--------------------------------------------------------------------------------
    //  Undefine all URIVL symbol that could have been defined by error or improperly
    //  in a precompiled header file
    //--------------------------------------------------------------------------------

    //  OS
    #ifdef URIVL_OS_WIN32
        #undef URIVL_OS_WIN32
    #endif
    #ifdef URIVL_OS_MACOS
        #undef URIVL_OS_MACOS                         
    #endif
    #ifdef URIVL_OS_UNIX
        #undef URIVL_OS_UNIX                         
    #endif

    //  Compiler/IDE
    #ifdef URIVL_VISUAL
        #undef URIVL_VISUAL
    #endif
    #ifdef URIVL_MINGW32
        #undef URIVL_MINGW32
    #endif
    #ifdef URIVL_CYGWIN
        #undef URIVL_CYGWIN
    #endif
    #ifdef URIVL_XCODE
        #undef URIVL_XCODE
    #endif

    //------------------------------------------------------------------------------
    //  We compile either 
    //      - the library itself --------------------------> URIVL_LIBRARY = 1
    //      - a demo against the source of the library ----> URIVL_SOURCE = 1
    //      - a demo against a binary of the library ------> URIVL_SOURCE = 0
    //------------------------------------------------------------------------------
    #if URIVL_SOURCE
        #if URIVL_LIBRARY
            #error conflicting compilation modes: source or lib?
        #else
            #ifdef URIVL_LIBRARY
                #undef URIVL_LIBRARY
            #endif
            #define URIVL_LIBRARY       0
        #endif
        
        #if URIVL_SOURCE
            #undef URIVL_SOURCE
            #define URIVL_SOURCE        1
        #else
            #undef URIVL_SOURCE
            #define URIVL_SOURCE        0
        #endif
        
    #else
        #if URIVL_LIBRARY
            #undef URIVL_LIBRARY
            #define URIVL_LIBRARY       1
            #define URIVL_SOURCE        0
        #else
            #error compilation mode (source or lib?) not defined
        #endif
    #endif        
    
    //------------------------------------------------------------------------------
    //  The library must have ways to talk to a video source.  We exclude 
    //  concurrent use of some libraries
    //------------------------------------------------------------------------------
    #undef URIVL_CAN_READ_IMAGES
    #undef URIVL_CAN_PLAY_MOVIES
    #undef URIVL_CAN_GRAB_VIDEOS

    #if ((URIVL_USE_QUICKTIME && URIVL_USE_FREEIMAGE) || (URIVL_USE_QUICKTIME && URIVL_USE_HACK_RW) || \
         (URIVL_USE_HACK_RW && URIVL_USE_FREEIMAGE))
        #error  only one image reading library can be activated
    #elif (URIVL_USE_QUICKTIME || URIVL_USE_FREEIMAGE || URIVL_USE_HACK_RW)
        #define URIVL_CAN_READ_IMAGES   1
    #else
        #define URIVL_CAN_READ_IMAGES   0
    #endif
    
    #if (URIVL_USE_QUICKTIME && URIVL_USE_LIBQUICKTIME)
        #error  only one movie playing library can be activated
    #elif (URIVL_USE_QUICKTIME || URIVL_USE_LIBQUICKTIME)
        #define URIVL_CAN_PLAY_MOVIES   1
    #else
        #define URIVL_CAN_PLAY_MOVIES   0
    #endif
    
    #if ((URIVL_USE_QUICKTIME && URIVL_USE_LIBVIDCAP) || (URIVL_USE_QUICKTIME && URIVL_USE_KINECT) || \
	     (URIVL_USE_LIBVIDCAP && URIVL_USE_KINECT))
        #error  only one video grabbing library can be activated
    #elif (URIVL_USE_QUICKTIME || URIVL_USE_LIBVIDCAP || URIVL_USE_KINECT)
        #define URIVL_CAN_GRAB_VIDEOS   1
    #else
        #define URIVL_CAN_GRAB_VIDEOS   0
    #endif
    
    #if !(URIVL_CAN_READ_IMAGES || URIVL_CAN_PLAY_MOVIES || URIVL_CAN_GRAB_VIDEOS)
        #error  URIVL needs to be able to access at least one sort of video source
    #endif
    
    //------------------------------------------------------------------------------
    //  Determine the OS and IDE
    //------------------------------------------------------------------------------

    //  Windows platform
    #if (defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__))

        #define URIVL_OS_WIN32                          1
        #define URIVL_OS_MACOS                          0                        
        #define URIVL_OS_UNIX                           0                        
      
        #if (defined(_MSC_VER))
            #define URIVL_VISUAL                        1
            #define URIVL_MINGW32                       0
            #define URIVL_CYGWIN                        0
            #define URIVL_XCODE                         0
            #define URIVL_GCC_MAKE                      0
        #endif

        #if (defined(__MINGW32__))
            #define URIVL_VISUAL                        0
            #define URIVL_MINGW32                       1
            #define URIVL_CYGWIN                        0
            #define URIVL_XCODE                         0
            #define URIVL_GCC_MAKE                      0
        #endif

        #if (defined(__CYGWIN__))
            #define URIVL_VISUAL                        0
            #define URIVL_MINGW32                       0
            #define URIVL_CYGWIN                        1
            #define URIVL_XCODE                         0
            #define URIVL_GCC_MAKE                      0
        #endif
   
    // Mac OS platform
    #elif (defined(__dest_os) && (__dest_os == __mac_os )) || \
          ((defined(__GNUC__) && (defined(__APPLE_CPP__) || defined(__APPLE_CC__))))
        #define URIVL_OS_MACOS                          1
        #define URIVL_OS_WIN32                          0                        
        #define URIVL_OS_UNIX                           0                        

        #if ((defined(__GNUC__) && (defined(__APPLE_CPP__) || defined(__APPLE_CC__))))
            #define URIVL_VISUAL                        0
            #define URIVL_MINGW32                       0
            #define URIVL_CYGWIN                        0
            #define URIVL_XCODE                         1
            #define URIVL_GCC_MAKE                      0
        #endif

    // Unix
    #elif (defined(__FreeBSD__) || defined(linux) || defined(sgi) || defined(__NetBSD__) \
                              || defined(__OpenBSD) || defined(__QNX__))

        #define URIVL_OS_UNIX                           1
        #define URIVL_OS_WIN32                          0                        
        #define URIVL_OS_MACOS                          0

        #define URIVL_VISUAL                            0
        #define URIVL_MINGW32                           0
        #define URIVL_CYGWIN                            0
        #define URIVL_XCODE                             0
        #define URIVL_GCC_MAKE                          1

    #endif

    //=============================================================================
    //      Platform selection verification
    //-----------------------------------------------------------------------------
    #if ! (URIVL_OS_MACOS || URIVL_OS_WIN32  || URIVL_OS_UNIX)
        #error Target OS not detected!
    #endif

    #if (   (URIVL_OS_MACOS && (URIVL_OS_WIN32 || URIVL_OS_UNIX)) || \
            (URIVL_OS_WIN32 && (URIVL_OS_MACOS || URIVL_OS_UNIX)) || \
            (URIVL_OS_UNIX  && (URIVL_OS_MACOS || URIVL_OS_WIN32)) )
        #error More than one target OS has been selected!
    #endif


    #if !( URIVL_VISUAL || URIVL_MINGW32 || URIVL_CYGWIN || \
           URIVL_XCODE || URIVL_GCC_MAKE )
        #error Compiler not detected!
    #endif

    #if (  (URIVL_VISUAL && \
    			(URIVL_MINGW32 || URIVL_CYGWIN || URIVL_XCODE ||URIVL_GCC_MAKE)) || \
           (URIVL_MINGW32 && \
           		(URIVL_VISUAL || URIVL_CYGWIN || URIVL_XCODE ||URIVL_GCC_MAKE)) || \
           (URIVL_CYGWIN && \
           		(URIVL_MINGW32 || URIVL_VISUAL || URIVL_XCODE ||URIVL_GCC_MAKE)) || \
           (URIVL_XCODE && \
           		(URIVL_MINGW32 || URIVL_CYGWIN || URIVL_VISUAL ||URIVL_GCC_MAKE)) || \
           (URIVL_GCC_MAKE && \
           		(URIVL_MINGW32 || URIVL_CYGWIN || URIVL_XCODE ||URIVL_VISUAL)) )
        #error More than one compiler/IDE has ben selected!
    #endif
}


//=============================================================================
//      Mac OS specific
//-----------------------------------------------------------------------------
//
#if (URIVL_OS_MACOS)

    //  Are we running on PPC or Intel? [should be obsolete by now]
    #ifdef __LITTLE_ENDIAN__
        #define URIVL_SMALL_ENDIAN		1
        #define URIVL_BIG_ENDIAN		0
    #else
        #define URIVL_SMALL_ENDIAN		0
        #define URIVL_BIG_ENDIAN		1
    #endif

    #if URIVL_USE_QUICKTIME
        #undef  URIVL_USE_QUICKTIME
        #define URIVL_USE_QUICKTIME     1
    #else
        #undef  URIVL_USE_QUICKTIME
        #define URIVL_USE_QUICKTIME     0
    #endif
    
    #if URIVL_USE_GLUT
    	#undef	URIVL_USE_GLUT
    	#define	URIVL_USE_GLUT			1
    #else
    	#undef	URIVL_USE_GLUT
    	#define	URIVL_USE_GLUT			0
    #endif

    #pragma options align=power

	//	Xcode does not use export modifiers
	#define URIVL_EXPORT
    
    #ifndef PASCAL_RTN
    	#define PASCAL_RTN			pascal
    #endif
    
#endif // URIVL_OS_MACOS

//=============================================================================
//      Windows specific
//-----------------------------------------------------------------------------
//
#if URIVL_OS_WIN32

    #define URIVL_SMALL_ENDIAN			1
    #define URIVL_BIG_ENDIAN			0
    
    #if URIVL_USE_QUICKTIME
        #undef  URIVL_USE_QUICKTIME
        #define URIVL_USE_QUICKTIME     1
    #else
        #undef  URIVL_USE_QUICKTIME
        #define URIVL_USE_QUICKTIME     0
    #endif
    
    #if URIVL_USE_GLUT
    	#undef	URIVL_USE_GLUT
    	#define	URIVL_USE_GLUT			1
    #else
    	#undef	URIVL_USE_GLUT
    	#define	URIVL_USE_GLUT			0
    #endif

    // Includes for VC++ (Win CW builds should use the Win32Headers prefix)
    #if URIVL_VISUAL
        #include <Windows.h>
		#if URIVL_USE_QUICKTIME
			 #include <ConditionalMacros.h> 
		#endif
		#include <float.h> // to get FLT_MAX amongst many things

		  //
        #pragma warning(disable:4068)       // ignore unknown pragmas
        #pragma warning(disable:4244)       // ignore conversion from "long" to "int", possible loss of data
		  #pragma warning(disable:4267)		// ignore conversion from "size_t" to "int", possible loss of data 
        #pragma warning(disable:4761)       // ignore integral size mismatch in argument: conversion supplied
        #pragma warning(disable:4129)       // ignore 'p': unrecognized character escape sequence
        #pragma warning(disable:4229)       // ignore anachronism used: modifiers on data are ignored
		  #pragma warning(disable:4996)       // ignore the deprecated functions for visual (qualified as unsafe by the compiler)
		  #pragma warning(disable:4800)		// ignore the forced boolean conversion ("true" or "false")
		  #pragma warning(disable:4251)       // ignore dll interface warning
		  #pragma warning(disable:4275)       // ignore dll interface warning

        #if URIVL_LIBRARY
        #   define URIVL_EXPORT   __declspec(dllexport)
        # else
        #   if URIVL_SOURCE
        #       define URIVL_EXPORT
        #   else
        #       define URIVL_EXPORT   __declspec(dllimport)
        #   endif
        #endif

        #ifndef round
            #define round(x) (x<0?ceil((x)-0.5):floor((x)+0.5))
        #endif

		  #ifndef roundf
		     #define roundf(value) round(value)
        #endif

	
	// URIVL_MINGW32 still relies on the Windows SDK so it shares many similarities
	// to the URIVL_VISUAL macro; however, pragma warnings are handled differently. 
	#elif URIVL_MINGW32
		#include <Windows.h>
		#include <ConditionalMacros.h>
        //
        #if URIVL_LIBRARY
        #   define URIVL_EXPORT   __declspec(dllexport)
        # else
        #   if URIVL_SOURCE
        #       define URIVL_EXPORT
        #   else
        #       define URIVL_EXPORT   __declspec(dllimport)
        #   endif
        #endif

    #else   //  Code::Blocks

        #if URIVL_LIBRARY
        #   define URIVL_EXPORT   __declspec(dllexport)
        # else
        #   if URIVL_SOURCE
        #       define URIVL_EXPORT
        #   else
        #       define URIVL_EXPORT   __declspec(dllimport)
        #   endif
        #endif

    #endif
    
    #ifdef PASCAL_RTN
        #undef PASCAL_RTN
    #endif
 
 	#if URIVL_USE_QUICKTIME
        #define PASCAL_RTN  pascal
    #else
        #define PASCAL_RTN
    #endif

    #ifdef MEDIA_TYPE
        #undef  MEDIA_TYPE
    #endif

#endif  //  URIVL_OS_WIN32


//=============================================================================
//      Other Unix specific
//-----------------------------------------------------------------------------
//
#if URIVL_OS_UNIX

    //  For the time being I am going to assume that all Linux boxes have a little-endian CPU
    #define URIVL_SMALL_ENDIAN  1
    #define URIVL_BIG_ENDIAN    0


    #define URIVL_EXPORT

    #if URIVL_USE_QUICKTIME
        #error The target platform does not support QuickTime
    #endif

    #undef  URIVL_USE_QUICKTIME
    #define URIVL_USE_QUICKTIME     0
    
    #if URIVL_USE_GLUT
    	#undef	URIVL_USE_GLUT
    	#define	URIVL_USE_GLUT		1
    #else
    	#undef	URIVL_USE_GLUT
    	#define	URIVL_USE_GLUT		0
    #endif

#endif  //  URIVL_OS_UNIX


//=============================================================================
//      Include files
//-----------------------------------------------------------------------------

#if URIVL_OS_WIN32

    #if URIVL_CYGWIN
        //  force loading math.h first so that QuickTime does not do its own declarations
        //  of float_t and double_t
        #include <cmath>
        //
        #include <gl/gl.h>
        #include <gl/glu.h>
        
        #if URIVL_USE_GLUT
        	#include <glut.h>
        #endif
    #endif

    #if URIVL_VISUAL
        //  force loading math.h first so that QuickTime does not do its own declarations
        //  of float_t and double_t
        #include <cmath>
        //
        #include <gl/gl.h>
        #include <gl/glu.h>
        
        #if URIVL_USE_GLUT
	        #include <glut.h>
	    #endif
    #endif

	#if URIVL_MINGW32
		//  force loading math.h first so that QuickTime does not do its own declarations
        //  of float_t and double_t
        #include <cmath>
		
		//	ConditionalMacros.h for some reason defines TYPE_LONGLONG 1 and so causes the
		//	Math64.h file to use the i64 MSVC-only post-fix on their macros.  This ensures
		//	that it instead uses LL for i64 and ULL for ui64.
		#ifdef TYPE_LONGLONG
			#undef TYPE_LONGLONG
			#define TYPE_LONGLONG	0
		#endif

		//	Windows follows the <gl/gl.h> pattern in both cygwin and mingw
		#include <gl/gl.h>
		#include <gl/glu.h>
		
		#if URIVL_USE_GLUT
			#include <windows.h>
			#include <glut.h>
		#endif
	#endif

    #if URIVL_USE_QUICKTIME
        #include <QTML.h>
        #include <Movies.h>
        #include <FixMath.h>
        #include <QuickTimeComponents.h>
        #ifndef __QUICKDRAW__
            #include <Quickdraw.h>
        #endif
        #ifndef __MOVIES__
            #include <Movies.h>
        #endif
    #endif

#endif  //  URIVL_OS_WIN32


#if URIVL_OS_UNIX
    #include <cmath>
    #include <GL/gl.h>
    #include <GL/glu.h>
    
    #if URIVL_USE_GLUT
	    #include <GL/glut.h>
	#endif
	#if URIVL_USE_LIBQUICKTIME
        #include <lqt/lqt.h>
    #endif
#endif  //  URIVL_OS_UNIX


#if URIVL_OS_MACOS
    #if URIVL_XCODE
		#include <Carbon/Carbon.h>
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
        #if URIVL_USE_GLUT
	        #include <GLUT/glut.h>
		#endif
        #if URIVL_USE_QUICKTIME
			#include <QuickTime/QuickTimeComponents.h>
            #include <QuickTime/ImageCompression.h>
            #include <QuickTime/Movies.h>
        #else
			#if URIVL_USE_LIBQUICKTIME
				#include <lqt/lqt.h>
			#endif
			
			#if URIVL_USE_FREEIMAGE
				#include "Freeimage.h"
			#endif
        #endif
    #endif
#endif  //  URIVL_OS_MACOS




//=============================================================================
//      Common macros, functions, and data types
//  In case none of the header files we loaded defined them...
//=============================================================================
namespace uriVL 
{
    #ifndef MAX
        #define     MAX(xx, yy)     (xx > yy ? xx : yy)
    #endif
    #ifndef MIN
        #define     MIN(xx, yy)     (xx < yy ? xx : yy)
    #endif

    //  Most compilers will have defined these constants by now in one of the headers
    //  we included, but if they were not defined we do it here
    #ifndef M_PI
        #define     M_PI				3.14159265358979324L
    #endif
    #ifndef PI
        #define     PI		M_PI
    #endif
	#ifndef PI_F
		#define		PI_F				3.14159265358979324f
	#endif
	
	#ifndef HUGE_VALF

	   #ifdef FLT_MAX
	      #define HUGE_VALF				FLT_MAX // from <float.h>
      #else 
		   #define HUGE_VALF			1.e38
      #endif
	#endif

	#ifndef HUGE_VALI
		#define HUGE_VALI				2147483647
	#endif

	#ifndef DEGREE_TO_RADIAN
		#define DEGREE_TO_RADIAN		0.01745329f
	#endif

	#ifndef RADIAN_TO_DEGREE
		#define RADIAN_TO_DEGREE		57.29578f
	#endif
	
	//	generally used for normalization tests
	#define	STD_RELATIVE_TOLERANCE		1E-08
	#define	STD_RELATIVE_TOLERANCE_F	1E-06f
	#define	LARGE_RELATIVE_TOLERANCE	1E-06
	#define	LARGE_RELATIVE_TOLERANCE_F	1E-04f
	
	
		
    //  We are going to use this single-value enum type to define different possible
    //  behaviors of our getXXXX functions
    typedef enum ReadWriteStatus 
	{
                                    R_W_ACCESS
    } ReadWriteStatus;
	
	

    /** Enum type for true/false/unknown ternary state variables.  This one is probably going to move
     *  to a more general position in the library.
     */
    typedef enum TriState 
	{
                                 IS_FALSE = 0,
                                 IS_TRUE = 1,	//	within STD_RELATIVE_TOLERANCE
                                 IS_UNKNOWN = -1
    } TriState;


	/**	UriVL class only contains static functions.  It is there to define
	 *	static variables used globally in the library.  
	 */
	class UriVL 
	{
	
		public:
		
			static void setValidDataRectSafeMode(bool mode) 
			{
				validDataRectIsSafe_ = mode;
			};
			
			static bool validDataRectIsSafe(void) 
			{
				return validDataRectIsSafe_;
			};

		private:
		
			static bool validDataRectIsSafe_;
			
			UriVL(void) { };
	};
}


#endif  //  URIVL_H

