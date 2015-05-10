#	File:
#		CMakeLists.txt
#
#    Description:
#		CMake project file for the SimpleIO project.  This project 
#               demonstrates the use of the basic Image and RasterImage classes.
#
#    COPYRIGHT:
#        (c) 2003-2013, 3D Group for Interactive Visualization
#                       University of Rhode Island.
#
#        Licensed under the Academic Free License - v. 3.0
#        For more information see http://opensource.org/licenses/academic.php
#
#    Author:
#        Christopher T.S. Allen
#		 Revised by jyh 2013-09, to add support for FreeImage++

cmake_minimum_required(VERSION 2.4.7 FATAL_ERROR)

#  Name our project main_IO
project(main_IP)

#  Tells CMake to build the project using the debug configuration
#  ON by default
#  NOTE:  When set to OFF the release version is built
option(DEBUG "Set project configuration to debug mode" ON)
message(STATUS "DEBUG:\t\t" ${DEBUG})

#  Tells CMake to build the project using the release configuration
#  OFF by default
#  NOTE:  This produces a release version of the library with debugging symbols
option(RELEASE "Set project configuration to release mode" OFF)
message(STATUS "Release:\t\t" ${RELEASE})

#  Tells CMake to build the project using GLUT
#  ON by default
option(USE_GLUT "Build the project using GLUT for the GUI." ON)
message(STATUS "USE_GLUT:\t\t" ${USE_GLUT})


if(APPLE OR WIN32)


   #  Tells CMake to build the project using QuickTime
    #  OFF by default
    #  NOTE: When set to OFF the generic version is built.
    #        At present, this means USE_LIBQUICKTIME must be enabled!
    option(USE_QUICKTIME "Build the project using QuickTime" OFF)
    message(STATUS "USE_QUICKTIME:\t" ${USE_QUICKTIME})

    #  Tells CMake to build the project using libquicktime
    #  OFF by default
    #  NOTE: QuickTime and libquicktime are mutually exclusive
    option(USE_LIBQUICKTIME "Build the library using libquicktime" ON)
    message(STATUS "USE_LIBQUICKTIME:\t" ${USE_LIBQUICKTIME})

	#  Tells CMake to build the library using FreeImage
	#  ON by default
	#  NOTE: FreeImage, hack reader, and QuickTime are mutually exclusive
	OPTION(USE_FREEIMAGE "Build the library using hack reader" ON)
	MESSAGE(STATUS "USE_FREEIMAGE:\t" ${USE_FREEIMAGE})

	#  Tells CMake to build the library using the hack reader
	#  OFF by default
	#  NOTE: FreeImage, hack reader, and QuickTime are mutually exclusive
	OPTION(USE_HACK_RW "Build the library using hack reader" OFF)
	MESSAGE(STATUS "USE_HACK_RW:\t" ${USE_HACK_RW})

#---> Linux configuration
else(APPLE OR WIN32)


    #  Tells CMake to build the project using QuickTime
    #  OFF by default
    #  NOTE: When set to OFF the generic version is built.
    #        At present, this means USE_LIBQUICKTIME must be enabled!
    option(USE_QUICKTIME "Build the project using QuickTime" OFF)
    message(STATUS "USE_QUICKTIME:\t" ${USE_QUICKTIME})

    #  Tells CMake to build the project *not* using libquicktime
    #  OFF by default (not needed for this project)
    #  NOTE: QuickTime and libquicktime are mutually exclusive
    option(USE_LIBQUICKTIME "Build the library using libquicktime" OFF)
    message(STATUS "USE_LIBQUICKTIME:\t" ${USE_LIBQUICKTIME})
    
	#  Tells CMake to build the library using FreeImage
	#  ON by default
	#  NOTE: FreeImage, hack reader, and QuickTime are mutually exclusive
	OPTION(USE_FREEIMAGE "Build the library using hack reader" ON)
	MESSAGE(STATUS "USE_FREEIMAGE:\t" ${USE_FREEIMAGE})

	#  Tells CMake to build the library using the hack reader
	#  OFF by default
	#  NOTE: FreeImage, hack reader, and QuickTime are mutually exclusive
	OPTION(URIVL_USE_HACK_RW "Build the library using hack reader" OFF)
	MESSAGE(STATUS "USE_HACK_RW:\t" ${USE_HACK_RW})

endif(APPLE OR WIN32)

#  Now CMake will check to ensure that mutually exclusive options
#  are not enabled.

#  USE_QUICKTIME and USE_LIBQUICKTIME are also mutually exclusive libraries
IF(USE_QUICKTIME AND USE_LIBQUICKTIME)
    MESSAGE(FATAL_ERROR "URIVL_USE_LIBQUICKTIME and URIVL_USE_QUICKTIME are both enabled!")
ENDIF(USE_QUICKTIME AND USE_LIBQUICKTIME)

#  USE_QUICKTIME and USE_LIBVIDCAP are also mutually exclusive libraries
IF(USE_QUICKTIME AND USE_LIBVIDCAP)
	MESSAGE(FATAL_ERROR "USE_QUICKTIME and USE_LIBVIDCAP are both enabled!")
ENDIF(USE_QUICKTIME AND USE_LIBVIDCAP)

#  USE_QUICKTIME and USE_FREEIMAGE are also mutually exclusive libraries
IF(USE_QUICKTIME AND USE_FREEIMAGE)
	MESSAGE(FATAL_ERROR "USE_QUICKTIME and USE_FREEIMAGE are both enabled!")
ENDIF(USE_QUICKTIME AND USE_FREEIMAGE)

#  USE_QUICKTIME and USE_HACK_RW are also mutually exclusive libraries
IF(USE_QUICKTIME AND USE_HACK_RW)
	MESSAGE(FATAL_ERROR "USE_QUICKTIME and USE_HACK_RW are both enabled!")
ENDIF(USE_QUICKTIME AND USE_HACK_RW)

#  USE_FREEIMAGE and USE_HACK_RW are also mutually exclusive libraries
IF(USE_FREEIMAGE AND USE_HACK_RW)
	MESSAGE(FATAL_ERROR "USE_FREEIMAGE and USE_HACK_RW are both enabled!")
ENDIF(USE_FREEIMAGE AND USE_HACK_RW)


#  Projects that use the uriVL use the -DURIVL_LIBRARY=1 macro,
#  to build against the binary of the library.
#  NOTE: to build against the source use -DURIVL_SOURCE=1
add_definitions(-DURIVL_LIBRARY=1)

#  Create a variable pointing to the top of our headers directory
set(uriVL_headers_top ${CMAKE_SOURCE_DIR}/../../../../../../Headers)

#  Create a variable pointing to the top of our libraries directory
set(uriVL_libraries_top ${CMAKE_SOURCE_DIR}/../../../../../../Libraries)

set(${PROJECT_NAME}_src_top ${CMAKE_SOURCE_DIR}/../../)

set(${PROJECT_NAME}_headers_top ${CMAKE_SOURCE_DIR}/../../)

set(${PROJECT_NAME}_demoFolder_top ${CMAKE_SOURCE_DIR}/../../../../../../)

#  Set a variable to contain the location of our header files
set(uriVL_headers
        ${uriVL_headers_top}/uriBase
        ${uriVL_headers_top}/uriGraphics
        ${uriVL_headers_top}/uriImageProcessing
        ${uriVL_headers_top}/uriMovieEditing
        ${uriVL_headers_top}/uriNumerical
        ${uriVL_headers_top}/uriTracking
        ${uriVL_headers_top}/uriVideoSources
        ${uriVL_headers_top}/uriComputerVision)

set(${PROJECT_NAME}_headers
		${${PROJECT_NAME}_headers_top}
		${${PROJECT_NAME}_demoFolder_top})

set(${PROJECT_NAME}_src
		${${PROJECT_NAME}_src_top}./${PROJECT_NAME}.cpp)

#  Specify our include directories for this project
include_directories(${uriVL_headers} ${${PROJECT_NAME}_headers})

#  Adjust our compiler flags based upon the type of configuration being built
#  NOTE: -Wno-multichar and -Wno-unknown-pragmas are used for two reasons.
#		1.  -Wno-multichar is used due to the crappy non-standards conformance
#			of the QuickTime SDK.
#		2.  -Wno-unknown-pragmas is used due to the variety of compilers that
#			this library was built on.  The pragmas can safely be ignored though
#			since they are related to profiling and debugging.
if(DEBUG)
	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_BUILD_TYPE Debug)
	    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -g3 -ggdb -gdwarf-2 -Wno-multichar -Wno-unknown-pragmas")
    endif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
elseif(RELEASE)
	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
   		set(CMAKE_BUILD_TYPE Release)
   	    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -Wall -Wno-multichar -Wno-unknown-pragmas")
  	endif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
endif(DEBUG)

if(USE_GLUT)
	add_definitions(-DURIVL_USE_GLUT)
endif(USE_GLUT)

#  Next we need to handle the USE_QUICKTIME version of the project
if(USE_QUICKTIME)

	#  Add URIVL_USE_QUICKTIME to our list of compiler definitions
	add_definitions(-DURIVL_USE_QUICKTIME)

	IF(APPLE)
    	#  Include the nifty little script for finding frameworks on the MacOS
    	include(CMakeFindFrameworks)

    	if(USE_GLUT)
    		find_library(GLUT_LIBRARY GLUT)
    		if(GLUT_LIBRARY)
    			message(STATUS "GLUT.framework:\tFound")
    			include_directories(${GLUT_LIBRARY}/Headers)
    			link_libraries(${GLUT_LIBRARY})
    		else(GLUT_LIBRARY)
    			message(FATAL_ERROR "Unable to find GLUT.")
    		endif(GLUT_LIBRARY)
    	endif(USE_GLUT)

    	#  Search for the OpenGL library
    	find_library(OPENGL_LIBRARY OpenGL)
    	if(OPENGL_LIBRARY)
    		message(STATUS "OpenGL.framework:\tFound")
    		include_directories(${OPENGL_LIBRARY}/Headers)
    		link_libraries(${OPENGL_LIBRARY})
    	else(OPENGL_LIBRARY)
    		message(FATAL_ERROR "Unable to find OpenGL.")
    	endif(OPENGL_LIBRARY)

    	#  Search for the QuickTime library
    	find_library(QUICKTIME_LIBRARY QuickTime)
    	if(QUICKTIME_LIBRARY)
    		message(STATUS "QuickTime.framework:\tFound")
    		include_directories(${QUICKTIME_LIBRARY}/Headers)
    		link_libraries(${QUICKTIME_LIBRARY})
    	else(QUICKTIME_LIBRARY)
    		message(FATAL_ERROR "Unable to locate QuickTime.")
    	endif(QUICKTIME_LIBRARY)

    	#  Search for the Carbon library
    	find_library(CARBON_LIBRARY Carbon)
    	if(CARBON_LIBRARY)
    		message(STATUS "Carbon.framework:\tFound")
    		include_directories(${CARBON_LIBRARY}/Headers)
    		link_libraries(${CARBON_LIBRARY})
    	else(CARBON_LIBRARY)
    		message(FATAL_ERROR "Unable to locate the Carbon.framework")
    	endif(CARBON_LIBRARY)
    ENDIF(APPLE)
    
    IF(WIN32)
        IF(MINGW)
            #  Force CMake to only search for libraries that end in .a or .lib
            SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".lib")

            SET(quicktime_cincludes $ENV{QTSDK}/CIncludes)
            SET(quicktime_component $ENV{QTSDK}/ComponentIncludes)
            SET(quicktime_libraries $ENV{QTSDK}/Libraries)
            INCLUDE_DIRECTORIES(${quicktime_cincludes} ${quicktime_component})
            FIND_LIBRARY(QTMLCLIENT_LIBRARY qtmlclient ${quicktime_libraries})
            IF(QTMLCLIENT_LIBRARY)
                MESSAGE(STATUS "QuickTime libraries found at:\t" ${quicktime_libraries})
                LINK_LIBRARIES(${QTMLCLIENT_LIBRARY})
            ELSE(QTMLCLIENT_LIBRARY)
                MESSAGE(FATAL_ERROR "Unable to find QuickTime libraries!")
            ENDIF(QTMLCLIENT_LIBRARY)

            FIND_LIBRARY(OPENGL_LIBRARY opengl32 /mingw/lib)
            IF(OPENGL_LIBRARY)
                MESSAGE(STATUS "OpenGL library found at:\t\t" ${OPENGL_LIBRARY})
                LINK_LIBRARIES(${OPENGL_LIBRARY})
            ELSE(OPENGL_LIBRARY)
                MESSAGE(FATAL_ERROR "Unable to find libopengl32!")
            ENDIF(OPENGL_LIBRARY)
         ENDIF(MINGW)
    ENDIF(WIN32)
    
    #---> Linux
    IF(UNIX)
   		MESSAGE(FATAL_ERROR "QuickTime is not supported on this system!")
    ENDIF(UNIX)

#  If USE_QUICKTIME is NOT enabled we do the following
#  Currently, the general version of the library makes
#  use of the libquicktime library, which is an open-source
#  cross-platform implementation of the QuickTime library.
ELSE(USE_QUICKTIME)

	IF(APPLE)
	

		#  Include the nifty little script for finding frameworks on the MacOS
		INCLUDE(CMakeFindFrameworks)

		# Search for GLUT libraries
		INCLUDE(FindGLUT)
		IF(GLUT_FOUND)
				MESSAGE(STATUS "GLUT library:\tFound at:\t\t" ${GLUT_LIBRARIES})
				include_directories(${GLUT_INCLUDE_DIR})
				link_libraries(${GLUT_LIBRARIES})
		ELSE(GLUT_FOUND)
				MESSAGE(FATAL_ERROR "GLUT not found!")
		ENDIF(GLUT_FOUND)

		#  Search for the OpenGL library
		FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
		IF(OPENGL_LIBRARY)
				MESSAGE(STATUS "OpenGL.framework:\tFound at:\t\t" ${OPENGL_LIBRARY})
				INCLUDE_DIRECTORIES(${OPENGL_LIBRARY}/Headers)
				LINK_LIBRARIES(${OPENGL_LIBRARY})
		ELSE(OPENGL_LIBRARY)
				MESSAGE(FATAL_ERROR "Unable to find OpenGL.")
		ENDIF(OPENGL_LIBRARY)

	    IF(USE_FREEIMAGE)

			#  Add URIVL_USE_FREEIMAGE to our list of compiler definitions
			ADD_DEFINITIONS(-DURIVL_USE_FREEIMAGE)

			FIND_PATH(LFREEIM_HEADERS FreeImage.h /opt/local/include)			
			#  Check to see if the FreeImage headers are there
			IF(LFREEIM_HEADERS)
				INCLUDE_DIRECTORIES(${LFREEIM_HEADERS})
			ELSE(LFREEIM_HEADERS)
				MESSAGE(FATAL_ERROR "FreeImage not found!")
			ENDIF(LFREEIM_HEADERS)
			
			#  Check for the FreeImage binaries
			FIND_LIBRARY(LFREEIM_LIBRARY freeimage /opt/local/lib)
			IF(LFREEIM_LIBRARY)
				MESSAGE(STATUS "libfreeimage:\tFound")
				LINK_LIBRARIES(${LFREEIM_LIBRARY})
			ELSE(LFREEIM_LIBRARY)
				MESSAGE(FATAL_ERROR "FreeImage not found!")
			ENDIF(LFREEIM_LIBRARY)	 	
	    ENDIF(USE_FREEIMAGE)
	    
	    
    	IF(USE_LIBQUICKTIME)

			#  Add URIVL_USE_LIBQUICKTIME to our list of compiler definitions
			ADD_DEFINITIONS(-DURIVL_USE_LIBQUICKTIME)

			FIND_PATH(LQT_HEADERS lqt/lqt.h /opt/local/include)
			#  Check to see if the libquicktime headers
			IF(LQT_HEADERS)
					INCLUDE_DIRECTORIES(${LQT_HEADERS})
			ELSE(LQT_HEADERS)
					MESSAGE(FATAL_ERROR "libquicktime not found!")
			ENDIF(LQT_HEADERS)

			#  Check for the libquicktime binaries
			FIND_LIBRARY(LQT_LIBRARY quicktime /opt/local/lib)
			IF(LQT_LIBRARY)
					MESSAGE(STATUS "libquicktime:\tFound")
					LINK_LIBRARIES(${LQT_LIBRARY})
			ELSE(LQT_LIBRARY)
					MESSAGE(FATAL_ERROR "libquicktime not found!")
			ENDIF(LQT_LIBRARY)
    	ENDIF(USE_LIBQUICKTIME)
	    
	ENDIF(APPLE)
	
	IF(WIN32)
	
        IF(MINGW)
	
	
			IF(URIVL_USE_FREEIMAGE)
				#  Add URIVL_USE_FREEIMAGE to our list of compiler definitions
				ADD_DEFINITIONS(-DURIVL_USE_FREEIMAGE)

			ENDIF(URIVL_USE_FREEIMAGE)


			IF(URIVL_USE_LIBQUICKTIME)
				#  Add URIVL_USE_LIBQUICKTIME to our list of compiler definitions
				ADD_DEFINITIONS(-DURIVL_USE_LIBQUICKTIME)


			ENDIF(URIVL_USE_LIBQUICKTIME)

            FIND_LIBRARY(OPENGL_LIBRARY opengl32 /mingw/lib)
            IF(OPENGL_LIBRARY)
                MESSAGE(STATUS "OpenGL library found at:\t\t" ${OPENGL_LIBRARY})
                LINK_LIBRARIES(${OPENGL_LIBRARY})
            ELSE(OPENGL_LIBRARY)
                MESSAGE(FATAL_ERROR "Unable to find libopengl32!")
            ENDIF(OPENGL_LIBRARY)

        ENDIF(MINGW)
	
	ENDIF(WIN32)
	
	# --> Linux
	IF(UNIX)
	
		# Search for GLUT libraries
		INCLUDE(FindGLUT)
		IF(GLUT_FOUND)
				MESSAGE(STATUS "GLUT library found at:\t\t" ${GLUT_LIBRARIES})
				include_directories(${GLUT_INCLUDE_DIR})
				link_libraries(${GLUT_LIBRARIES})
		ELSE(GLUT_FOUND)
				MESSAGE(FATAL_ERROR "GLUT not found!")
		ENDIF(GLUT_FOUND)


		IF(USE_LIBQUICKTIME)
			ADD_DEFINITIONS(-DURIVL_USE_LIBQUICKTIME)

			#  Check for the libquicktime binaries
			FIND_LIBRARY(LQT_LIBRARY quicktime /opt/local/lib)
			IF(LQT_LIBRARY)
					MESSAGE(STATUS "libquicktime:\tFound")
					LINK_LIBRARIES(${LQT_LIBRARY})
			ELSE(LQT_LIBRARY)
					MESSAGE(FATAL_ERROR "libquicktime not found!")
			ENDIF(LQT_LIBRARY)
		ENDIF(USE_LIBQUICKTIME)


		IF(USE_FREEIMAGE)
			#  Add URIVL_USE_FREEIMAGE to our list of compiler definitions
			ADD_DEFINITIONS(-DURIVL_USE_FREEIMAGE)

			#  Check for the freeimage binaries
			FIND_LIBRARY(FREEIMAGE_LIBRARY freeimage /usr/lib)
			IF(FREEIMAGE_LIBRARY)
					MESSAGE(STATUS "libfreeimage:\tFound")
					LINK_LIBRARIES(${FREEIMAGE_LIBRARY})
			ELSE(FREEIMAGE_LIBRARY)
					MESSAGE(FATAL_ERROR "libfreeimage not found!")
			ENDIF(FREEIMAGE_LIBRARY)
		ENDIF(USE_FREEIMAGE)
		
		#search for OPENGL Libraries
		INCLUDE(FindOpenGL)
		IF(OPENGL_FOUND)
			IF(OPENGL_GLU_FOUND)
				MESSAGE(STATUS "OpenGL/glu libraries found at:\t\t" ${OPENGL_LIBRARIES})
				include_directories(${OPENGL_INCLUDE_DIR})
				link_libraries(${OPENGL_LIBRARIES})
			ELSE(OPENGL_GLU_FOUND)
					MESSAGE(FATAL_ERROR "OpenGL/glu not found!")
			ENDIF(OPENGL_GLU_FOUND)
		ELSE(OPENGL_FOUND)
				MESSAGE(FATAL_ERROR "OpenGL/gl not found!")
		ENDIF(OPENGL_FOUND)


	ENDIF(UNIX)

ENDIF(USE_QUICKTIME)


if(APPLE)
    if(DEBUG)
    	link_libraries(${uriVL_libraries_top}/Mac\ OS/debug/liburiVL.a)
        set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/debug)
    else(DEBUG)
        link_libraries(${uriVL_libraries_top}/Mac\ OS/release/liburiVL.a)
		set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/release)
    endif(DEBUG)
elseif(WIN32)
    if(DEBUG)
        link_libraries(${uriVL_libraries_top}/Win32/Debug/liburiVL.a)
    else(DEBUG)
        link_libraries(${uriVL_libraries_top}/Win32/Release/liburiVL.a)
    endif(DEBUG)
elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	if(DEBUG)
		link_libraries(${uriVL_libraries_top}/Linux/Debug/liburiVL.a)
	else(DEBUG)
		link_libraries(${uriVL_libaries_top}/Linux/Release/liburiVL.a)
	endif(DEBUG)
endif(APPLE)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_src})
