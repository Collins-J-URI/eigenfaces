#    File:
#        CMakeList.txt
#
#    Description:
#        CMake Makefile generator for the uriVisionLib
#        project.  
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
#			Based on the original uriVL CMakeLists.txt by Christian Convey
#		 Revised by jyh 2013-09, to add support for FreeImage++

cmake_minimum_required(VERSION 2.4.7 FATAL_ERROR)

#  Name our project uriVisionLib
PROJECT(uriVL)

#  Tells CMake to build the library using the debug configuration
#  ON by default
#  NOTE:  When set to OFF the release version is built 
OPTION(DEBUG "Set project configuration to debug mode" ON)
MESSAGE(STATUS "DEBUG:\t\t" ${DEBUG})

#  Tells CMake to build the library using the release configuration
#  OFF by default
#  NOTE:  This produces a release version of the library with debugging symbols
OPTION(RELEASE "Set project configuration to release mode" OFF)
MESSAGE(STATUS "Release:\t\t" ${RELEASE})

#  Tells CMake to build the library using the release configuration 
#  with no debugging symbols
#  OFF by default
#  NOTE:  Although this is presented as an option, chosing this will
#		  preclude any debugging symbols and is not advisable.
OPTION(RELEASE_ND "Set project configuration to release mode with no debugging symbols" OFF)
MESSAGE(STATUS "RELEASE_ND:\t\t" ${RELEASE_ND})

#  Tells CMake to build the shared version of the library
#  OFF by default
#  NOTE:  This is incompatible with the STATIC and FRAMEWORK options
OPTION(SHARED "Build the shared version of the library" OFF)
MESSAGE(STATUS "SHARED:\t\t" ${SHARED})

#  Tells CMake to build the archive version of the library
#  ON by default
#  NOTE:  This is incompatible with SHARED and FRAMEWORK options
OPTION(STATIC "Build the archived version of the library" ON)
MESSAGE(STATUS "STATIC:\t\t" ${STATIC})

#  Tells CMake to build the library as a Mac OS X framework
#  OFF by default
#  NOTE: This is incompatible with SHARED and STATIC options
OPTION(FRAMEWORK "Build the library as a Mac OS X framework" OFF)
MESSAGE(STATUS "Framework:\t\t" ${FRAMEWORK})

#  Tells CMake to build the library using QuickTime
#  ON by default
#  NOTE: When set to OFF the generic version is built.
#		At present, this means URIVL_USE_LIBQUICKTIME, URIVL_USE_FREEIMAGE, 
#		or URIVL_USE_HACK_RW must be enabled!
OPTION(URIVL_USE_QUICKTIME "Build the library using QuickTime" OFF)
MESSAGE(STATUS "URIVL_USE_QUICKTIME:\t" ${URIVL_USE_QUICKTIME})

#  Tells CMake to build the library using libquicktime
#  OFF by default
#  NOTE: QuickTime and libquicktime are mutually exclusive
OPTION(URIVL_USE_LIBQUICKTIME "Build the library using libquicktime" ON)
MESSAGE(STATUS "URIVL_USE_LIBQUICKTIME:\t" ${URIVL_USE_LIBQUICKTIME})


#  Tells CMake to build the library using FreeImage
#  OFF by default
#  NOTE: FreeImage, hack reader, and QuickTime are mutually exclusive
OPTION(URIVL_USE_FREEIMAGE "Build the library using hack reader" ON)
MESSAGE(STATUS "URIVL_USE_FREEIMAGE:\t" ${URIVL_USE_FREEIMAGE})

#  Tells CMake to build the library using the hack reader
#  OFF by default
#  NOTE: FreeImage, hack reader, and QuickTime are mutually exclusive
OPTION(URIVL_USE_HACK_RW "Build the library using hack reader" OFF)
MESSAGE(STATUS "URIVL_USE_HACK_RW:\t" ${URIVL_USE_HACK_RW})


#  *********************NOT IMPLEMENTED YET***************************
#  Tells CMake to build the library using libvidcap for video capture
#  OFF by default
#  NOTE: QuickTime and libvidcap are mutually exclusive
#  *******************************************************************
OPTION(URIVL_USE_LIBVIDCAP "Build the library using libvidcap" OFF)
MESSAGE(STATUS "URIVL_USE_LIBVIDCAP:\t" ${URIVL_USE_LIBVIDCAP})

#  Now CMake will check to ensure that mutually exclusive options
#  are not enabled.
#  **************************************************************

#  URIVL_USE_QUICKTIME and URIVL_USE_LIBQUICKTIME are also mutually exclusive libraries
IF(URIVL_USE_QUICKTIME AND URIVL_USE_LIBQUICKTIME)
    MESSAGE(FATAL_ERROR "URIVL_USE_LIBQUICKTIME and URIVL_USE_QUICKTIME are both enabled!")
ENDIF(URIVL_USE_QUICKTIME AND URIVL_USE_LIBQUICKTIME)

#  URIVL_USE_QUICKTIME and URIVL_USE_LIBVIDCAP are also mutually exclusive libraries
IF(URIVL_USE_QUICKTIME AND URIVL_USE_LIBVIDCAP)
	MESSAGE(FATAL_ERROR "URIVL_USE_QUICKTIME and URIVL_USE_LIBVIDCAP are both enabled!")
ENDIF(URIVL_USE_QUICKTIME AND URIVL_USE_LIBVIDCAP)

#  URIVL_USE_QUICKTIME and URIVL_USE_FREEIMAGE are also mutually exclusive libraries
IF(URIVL_USE_QUICKTIME AND URIVL_USE_FREEIMAGE)
	MESSAGE(FATAL_ERROR "URIVL_USE_QUICKTIME and URIVL_USE_FREEIMAGE are both enabled!")
ENDIF(URIVL_USE_QUICKTIME AND URIVL_USE_FREEIMAGE)

#  URIVL_USE_QUICKTIME and URIVL_USE_HACK_RW are also mutually exclusive libraries
IF(URIVL_USE_QUICKTIME AND URIVL_USE_HACK_RW)
	MESSAGE(FATAL_ERROR "URIVL_USE_QUICKTIME and URIVL_USE_HACK_RW are both enabled!")
ENDIF(URIVL_USE_QUICKTIME AND URIVL_USE_HACK_RW)

#  URIVL_USE_FREEIMAGE and URIVL_USE_HACK_RW are also mutually exclusive libraries
IF(URIVL_USE_FREEIMAGE AND URIVL_USE_HACK_RW)
	MESSAGE(FATAL_ERROR "URIVL_USE_FREEIMAGE and URIVL_USE_HACK_RW are both enabled!")
ENDIF(URIVL_USE_FREEIMAGE AND URIVL_USE_HACK_RW)


IF(FRAMEWORK AND NOT APPLE)
    MESSAGE(FATAL_ERROR "Frameworks can only built on Mac OS!")
ENDIF(FRAMEWORK AND NOT APPLE)

IF(NOT STATIC AND NOT SHARED AND NOT FRAMEWORK)
    MESSAGE(FATAL_ERROR "At least one type of library must be specified!")
ENDIF(NOT STATIC AND NOT SHARED AND NOT FRAMEWORK)

IF(NOT URIVL_USE_QUICKTIME AND NOT URIVL_USE_LIBQUICKTIME)
    MESSAGE(FATAL_ERROR "Either URIVL_USE_QUICKTIME or URIVL_USE_LIBQUICKTIME must be enabled!")
ENDIF(NOT URIVL_USE_QUICKTIME AND NOT URIVL_USE_LIBQUICKTIME)

#  Projects that use the uriVL use the -DURIVL_SOURCE macro,
#  but since we're building the library we use -DURIVL_LIBRARY
ADD_DEFINITIONS(-DURIVL_LIBRARY)

#  Create a variable pointing to the top of our headers directory
SET(uriVL_headers_top ${CMAKE_SOURCE_DIR}/../../../SDK/Headers)

#  Create a variable pointing to the top of our sources directory
SET(uriVL_sources_top ${CMAKE_SOURCE_DIR}/../../Source/C++)

#  Create a variable pointing to the "private" headers directory
SET(uriVL_private_headers ${uriVL_sources_top}/_uriPrivateClasses/Headers)

#  Create a variable pointing to the "private" sources directory
SET(uriVL_private_sources ${uriVL_sources_top}/_uriPrivateClasses/Source)

#  Create a variable pointing to the top of our libraries directory
SET(uriVL_libraries_top ${CMAKE_SOURCE_DIR}/../../../SDK/Libraries)

#  Set a variable to contain the location of our header files
SET(uriVL_headers 
        ${uriVL_headers_top}/uriBase
        ${uriVL_headers_top}/uriGraphics
        ${uriVL_headers_top}/uriImageProcessing
        ${uriVL_headers_top}/uriMovieEditing
        ${uriVL_headers_top}/uriNumerical
        ${uriVL_headers_top}/uriTracking
        ${uriVL_headers_top}/uriVideoSources
        ${uriVL_headers_top}/uriComputerVision
        ${uriVL_private_headers})

#  Our source files for the uriVL library
set(uriVL_sources 

#------------------BEGIN uriBase-------------------------#
	${uriVL_sources_top}/uriBase/ErrorReport.cpp
	${uriVL_sources_top}/uriBase/DrawableObject2D.cpp
	${uriVL_sources_top}/uriBase/GraphWorld.cpp
	${uriVL_sources_top}/uriBase/ImagePoint.cpp
	${uriVL_sources_top}/uriBase/ImagePoint_F.cpp
	${uriVL_sources_top}/uriBase/ImageRect.cpp
	${uriVL_sources_top}/uriBase/RasterImage.cpp
	${uriVL_sources_top}/uriBase/RasterImage_gray.cpp
	${uriVL_sources_top}/uriBase/RasterImage_gray_F.cpp
	${uriVL_sources_top}/uriBase/RasterImage_binary.cpp
	${uriVL_sources_top}/uriBase/RasterImage_RGB.cpp
	${uriVL_sources_top}/uriBase/RasterImage_RGBa.cpp
	${uriVL_sources_top}/uriBase/RasterImage_RGBa_F.cpp
	${uriVL_sources_top}/uriBase/RasterImage_HSV.cpp
# RasterImage_aRGB16.cpp is a work in progress
#	${uriVL_sources_top}/uriBase/RasterImage_aRGB16.cpp

# RasterImage_HSV24.cpp is a work in progress
#		${uriVL_sources_top}/uriBase/RasterImage_HSV24.cpp

	${uriVL_sources_top}/uriBase/Histogram.cpp
	${uriVL_sources_top}/uriBase/ImageHistogram.cpp
	${uriVL_sources_top}/uriBase/ColorImageHistogram.cpp

# RasterImage_Lab.cpp is a work in progress
#		${uriVL_sources_top}/uriBase/RasterImage_Lab.cpp
	${uriVL_sources_top}/uriBase/VectorField.cpp
	${uriVL_sources_top}/uriBase/VectorField_I.cpp
	${uriVL_sources_top}/uriBase/VectorField_F.cpp
	${uriVL_sources_top}/uriBase/Vector2D.cpp
	${uriVL_sources_top}/uriBase/Vector2D_F.cpp
#------------------END uriBase-------------------------#



#-----------------BEGIN uriComputerVision--------------#

	${uriVL_sources_top}/uriComputerVision/Entity3D.cpp
	${uriVL_sources_top}/uriComputerVision/Point3D.cpp
# Vector3D.cpp is a work in progress
#		${uriVL_sources_top}/uriComputerVision/Vector3D.cpp
	${uriVL_sources_top}/uriComputerVision/Transformation3D.cpp
	${uriVL_sources_top}/uriComputerVision/RigidTransformation3D.cpp
	${uriVL_sources_top}/uriComputerVision/ReferenceFrame3D.cpp
	${uriVL_sources_top}/uriComputerVision/Rotation3D.cpp
	${uriVL_sources_top}/uriComputerVision/Translation3D.cpp
	${uriVL_sources_top}/uriComputerVision/Camera.cpp
	${uriVL_sources_top}/uriComputerVision/WeakPerspectiveCamera.cpp
	${uriVL_sources_top}/uriComputerVision/OrthographicCamera.cpp
	${uriVL_sources_top}/uriComputerVision/PerspectiveCamera.cpp
	${uriVL_sources_top}/uriComputerVision/IntrinsicParameters_CCD.cpp
	${uriVL_sources_top}/uriComputerVision/IntrinsicParameters_pinhole.cpp
	${uriVL_sources_top}/uriComputerVision/IntrinsicParameters.cpp
	${uriVL_sources_top}/uriComputerVision/IntrinsicParameters_TsaiW.cpp
	${uriVL_sources_top}/uriComputerVision/Calibration3D.cpp
#	${uriVL_sources_top}/uriComputerVision/Calibration3D_dyn.cpp
	${uriVL_sources_top}/uriComputerVision/Calibration3D_Kim.cpp
	${uriVL_sources_top}/uriComputerVision/Calibration3D_TsaiW.cpp

	${uriVL_sources_top}/uriComputerVision/BackgroundRemover.cpp
# BackgroundRemover_dynamic.cpp is a work in progress
#		${uriVL_sources_top}/uriComputerVision/BackgroundRemover_dynamic.cpp
# BackgroundRemover_dynamic_MW.cpp is a work in progress
#		${uriVL_sources_top}/uriComputerVision/BackgroundRemover_dynamic_MW.cpp
	${uriVL_sources_top}/uriComputerVision/BackgroundRemover_static.cpp

	${uriVL_sources_top}/uriComputerVision/NormalFlowCalculator.cpp

	${uriVL_sources_top}/uriComputerVision/RangeMap.cpp
	${uriVL_sources_top}/uriComputerVision/DepthMap.cpp
	${uriVL_sources_top}/uriComputerVision/ElevationMap.cpp

	${uriVL_sources_top}/uriComputerVision/Disparity.cpp
	${uriVL_sources_top}/uriComputerVision/StereoDisparity.cpp
#	${uriVL_sources_top}/uriComputerVision/StereoDisparity_F.cpp
#	${uriVL_sources_top}/uriComputerVision/Disparity_F.cpp
	${uriVL_sources_top}/uriComputerVision/StereoPair.cpp
#	${uriVL_sources_top}/uriComputerVision/StereoMatcher.cpp
	${uriVL_sources_top}/uriComputerVision/StereoMatchRecord.cpp
# StereoMatch.cpp is a work in progress
#		${uriVL_sources_top}/uriComputerVision/StereoMatch.cpp
# RectifiedStereoMatch.cpp is a work in progress
#		${uriVL_sources_top}/uriComputerVision/RectifiedStereoMatch.cpp
# RectifiedStereoMatcher_JJL.cpp is a work in progress
#		${uriVL_sources_top}/uriComputerVision/RectifiedStereoMatcher_JJL.cpp
# RectifiedStereoDisparity_F.cpp is a work in progress
#		${uriVL_sources_top}/uriComputerVision/RectifiedStereoDisparity_F.cpp
# RectifiedStereoPair.cpp is a work in progress
#		${uriVL_sources_top}/uriComputerVision/RectifiedStereoPair.cpp
# RectifiedStereoMatcher_corr.cpp just doesn't work yet
#		${uriVL_sources_top}/uriComputerVision/RectifiedStereoMatcher_corr.cpp
# RectifiedStereoMatcher.cpp just doesn't work yet
#		${uriVL_sources_top}/uriComputerVision/RectifiedStereoMatcher.cpp

# VirtualObjectLoader.cpp just doesn't work yet
#	${uriVL_sources_top}/uriComputerVision/VirtualObjectLoader.cpp
	${uriVL_sources_top}/uriComputerVision/VirtualPoint3D.cpp
	${uriVL_sources_top}/uriComputerVision/VirtualFace3D.cpp
	${uriVL_sources_top}/uriComputerVision/VirtualPart3D.cpp
	${uriVL_sources_top}/uriComputerVision/VirtualObject3D.cpp

#------------------END uriComputerVision---------------#



#-----------------BEGIN uriGraphics--------------------#
	${uriVL_sources_top}/uriGraphics/GraphicComponent3D.cpp
	${uriVL_sources_top}/uriGraphics/GlUtils.cpp

# ObjectLoader_3jy.cpp is a work in progress
#		${uriVL_sources_top}/uriGraphics/ObjectLoader_3jy.cpp
	${uriVL_sources_top}/uriGraphics/Box3D.cpp
	${uriVL_sources_top}/uriGraphics/Cone3D.cpp
	${uriVL_sources_top}/uriGraphics/HalfCone3D.cpp
	${uriVL_sources_top}/uriGraphics/Cylinder3D.cpp
	${uriVL_sources_top}/uriGraphics/TubularSection3D.cpp
	${uriVL_sources_top}/uriGraphics/HalfCylinder3D.cpp
	${uriVL_sources_top}/uriGraphics/Prism3D.cpp
	${uriVL_sources_top}/uriGraphics/PolyMesh3D.cpp
#------------------END uriGraphics---------------------#



#-------------BEGIN uriImageProcessing-----------------#
	${uriVL_sources_top}/uriImageProcessing/ImageArithmetics.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageTransformation.cpp


	${uriVL_sources_top}/uriImageProcessing/Transformation2D.cpp
	${uriVL_sources_top}/uriImageProcessing/RigidTransformation2D.cpp
	${uriVL_sources_top}/uriImageProcessing/Rotation2D.cpp
	${uriVL_sources_top}/uriImageProcessing/Translation2D.cpp


	${uriVL_sources_top}/uriImageProcessing/ImageFeature.cpp
	${uriVL_sources_top}/uriImageProcessing/Contour_I.cpp
	${uriVL_sources_top}/uriImageProcessing/Blob.cpp
	${uriVL_sources_top}/uriImageProcessing/EdgeDetector.cpp
	${uriVL_sources_top}/uriImageProcessing/ContourDetector.cpp
	${uriVL_sources_top}/uriImageProcessing/Contour.cpp
	${uriVL_sources_top}/uriImageProcessing/Contour_F.cpp
	${uriVL_sources_top}/uriImageProcessing/ContourForce.cpp
	${uriVL_sources_top}/uriImageProcessing/Pattern2D.cpp
# Segment.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/Segment.cpp
# Corner.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/Corner.cpp
# Cluster.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/Cluster.cpp



	${uriVL_sources_top}/uriImageProcessing/LabelRaster.cpp
	${uriVL_sources_top}/uriImageProcessing/Segmenter.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorThresholdLabeler.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorInverseThresholdLabeler.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorSegParams.cpp
	${uriVL_sources_top}/uriImageProcessing/Labeler.cpp
	${uriVL_sources_top}/uriImageProcessing/GrayThresholdLabeler.cpp
	${uriVL_sources_top}/uriImageProcessing/HysteresisLabeler.cpp


	${uriVL_sources_top}/uriImageProcessing/FeatureDetector.cpp
	${uriVL_sources_top}/uriImageProcessing/BlobFinder.cpp
	${uriVL_sources_top}/uriImageProcessing/ZeroCrossingDetector.cpp
#	${uriVL_sources_top}/uriImageProcessing/ActiveContourDetector.cpp#
#	${uriVL_sources_top}/uriImageProcessing/ActiveContourDetector_GVF.cpp
#	${uriVL_sources_top}/uriImageProcessing/ActiveContourDetector_balloon.cpp
	${uriVL_sources_top}/uriImageProcessing/NonMaximumSuppressor.cpp
# SegmentDetector.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/SegmentDetector.cpp
# EdgeDetector_Canny_I.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/EdgeDetector_Canny_I.cpp
# EdgeDetector_Canny_F.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/EdgeDetector_Canny_F.cpp


	${uriVL_sources_top}/uriImageProcessing/ImageOperator.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageLaplacian.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageLaplacian_Sobel.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageLaplacian_Gaussian_I.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageLaplacian_Gaussian_F.cpp
	${uriVL_sources_top}/uriImageProcessing/GaussianFilter.cpp
	${uriVL_sources_top}/uriImageProcessing/GaussianFilter_I.cpp
	${uriVL_sources_top}/uriImageProcessing/GaussianFilter_F.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorGaussianFilter.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorGaussianFilter_F.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorGaussianFilter_I.cpp
	${uriVL_sources_top}/uriImageProcessing/MedianFilter.cpp
	${uriVL_sources_top}/uriImageProcessing/MedianFilter_F.cpp
	${uriVL_sources_top}/uriImageProcessing/MedianFilter_I.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorMedianFilter.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorMedianFilter_F.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorMedianFilter_I.cpp
# ColorImageLaplacian.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/ColorImageLaplacian.cpp
# ColorImageLaplacian_Gaussian_F.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/ColorImageLaplacian_Gaussian_F.cpp
# ColorImageLaplacian_Gaussian_I.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/ColorImageLaplacian_Gaussian_I.cpp
# ColorImageLaplacian_Sobel.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/ColorImageLaplacian_Sobel.cpp


	${uriVL_sources_top}/uriImageProcessing/VectorOperator.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageGradient.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageGradient_Sobel.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageGradient_Gaussian_I.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageGradient_Gaussian_F.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorImageGradient.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorImageGradient_Gaussian_I.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorImageGradient_Gaussian_F.cpp
	${uriVL_sources_top}/uriImageProcessing/ColorImageGradient_Sobel.cpp

	${uriVL_sources_top}/uriImageProcessing/PatternMatcher2D.cpp
	${uriVL_sources_top}/uriImageProcessing/PatternMatcher2D_scaledDiff.cpp
	${uriVL_sources_top}/uriImageProcessing/Pattern2DMatchRecord.cpp
	${uriVL_sources_top}/uriImageProcessing/PatternMatcher2D_medianDiff.cpp

	${uriVL_sources_top}/uriImageProcessing/PatternMatcher2D_corr.cpp
	${uriVL_sources_top}/uriImageProcessing/PatternMatcher2D_partCorr.cpp
#	${uriVL_sources_top}/uriImageProcessing/WarpMatchScoreFunctionND.cpp
#	${uriVL_sources_top}/uriImageProcessing/PatternMatcher2D_warp.cpp
	${uriVL_sources_top}/uriImageProcessing/Pattern2DMatchRecord_scale.cpp
	${uriVL_sources_top}/uriImageProcessing/Pattern2DMatchRecord_part.cpp
#	${uriVL_sources_top}/uriImageProcessing/Pattern2DMatchRecord_warp.cpp

	
	${uriVL_sources_top}/uriImageProcessing/HistogramCalculator.cpp

	${uriVL_sources_top}/uriImageProcessing/ImageWarper.cpp
	${uriVL_sources_top}/uriImageProcessing/ImageWarper_GL.cpp

# ImageStitcher.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/ImageStitcher.cpp
# ManualStitcher.cpp is a work in progress
#		${uriVL_sources_top}/uriImageProcessing/ManualStitcher.cpp

	${uriVL_sources_top}/uriImageProcessing/Registration2D.cpp
	${uriVL_sources_top}/uriImageProcessing/Registration2D_LPCD.cpp
	${uriVL_sources_top}/uriImageProcessing/Registration2D_LMICP.cpp
	${uriVL_sources_top}/uriImageProcessing/Registration2D_ICP.cpp

#------------------END uriImageProcessing--------------#



#----------------BEGIN uriMovieEditing-----------------#

	${uriVL_sources_top}/uriMovieEditing/ImageWriter.cpp
	${uriVL_sources_top}/uriMovieEditing/VideoDestination.cpp

#------------------END uriMovieEditing-----------------#


#-----------------BEGIN uriNumerical-------------------#

	${uriVL_sources_top}/uriNumerical/Matrix.cpp
	${uriVL_sources_top}/uriNumerical/RowVector.cpp
	${uriVL_sources_top}/uriNumerical/ColumnVector.cpp
	${uriVL_sources_top}/uriNumerical/HomogeneousMatrix.cpp
	${uriVL_sources_top}/uriNumerical/Quaternion.cpp

	${uriVL_sources_top}/uriNumerical/LinearSolverRecord.cpp
	${uriVL_sources_top}/uriNumerical/LinearSolver.cpp
	${uriVL_sources_top}/uriNumerical/LinearSolver_LU.cpp
	${uriVL_sources_top}/uriNumerical/LinearSolver_Cholesky.cpp
	${uriVL_sources_top}/uriNumerical/LinearSolver_SVD.cpp
	${uriVL_sources_top}/uriNumerical/LinearSolver_QR.cpp
	${uriVL_sources_top}/uriNumerical/LinearSolver_Jacobi.cpp
	${uriVL_sources_top}/uriNumerical/LinearSolver_GaussSeidel.cpp
	${uriVL_sources_top}/uriNumerical/LinearSolver_SOR.cpp

	${uriVL_sources_top}/uriNumerical/ConfidenceRecord.cpp

	${uriVL_sources_top}/uriNumerical/Function1D.cpp
	${uriVL_sources_top}/uriNumerical/Function1D_F.cpp
	${uriVL_sources_top}/uriNumerical/FunctionNDSection1D.cpp
	${uriVL_sources_top}/uriNumerical/FunctionNDSection1D_F.cpp
	${uriVL_sources_top}/uriNumerical/FunctionND.cpp
	${uriVL_sources_top}/uriNumerical/FunctionND_F.cpp

	${uriVL_sources_top}/uriNumerical/IterationStatus.cpp

	${uriVL_sources_top}/uriNumerical/OptimizerRecord1D.cpp
	${uriVL_sources_top}/uriNumerical/OptimizerRecordND.cpp
	${uriVL_sources_top}/uriNumerical/NonlinearOptimizer1D_parab.cpp
	${uriVL_sources_top}/uriNumerical/NonlinearOptimizerND.cpp
	${uriVL_sources_top}/uriNumerical/NonlinearOptimizerND_conjDir.cpp
	${uriVL_sources_top}/uriNumerical/NonlinearOptimizer1D_gss.cpp
	${uriVL_sources_top}/uriNumerical/NonlinearOptimizer1D_Brent.cpp

	${uriVL_sources_top}/uriNumerical/RandomGenerator.cpp
	${uriVL_sources_top}/uriNumerical/RandomGenerator_C.cpp
	${uriVL_sources_top}/uriNumerical/RandomGenerator_Knuth.cpp
	${uriVL_sources_top}/uriNumerical/NonlinearOptimizer1D.cpp
	${uriVL_sources_top}/uriNumerical/RandomGenerator_R250_521.cpp
	${uriVL_sources_top}/uriNumerical/RandomGenerator_MT.cpp
	
#------------------END uriNumerical--------------------#



#-----------------BEGIN uriTracking--------------------#		
# SegmentTrackingState.cpp is a work in progress
#	${uriVL_sources_top}/uriTracking/BlobTrackingState.cpp

# SegmentTrackingState.cpp is a work in progress
#		${uriVL_sources_top}/uriTracking/SegmentTrackingState.cpp
#	${uriVL_sources_top}/uriTracking/CornerTrackingState.cpp
# PatternTracker2DController is a work in progress
#	${uriVL_sources_top}/uriTracking/PatternTracker2DController.cpp

# ShapeTrackingState.cpp is a work in progress
#		${uriVL_sources_top}/uriTracking/ShapeTrackingState.cpp

#	${uriVL_sources_top}/uriTracking/Tracker2DController.cpp
#	${uriVL_sources_top}/uriTracking/Tracker2D.cpp
#	${uriVL_sources_top}/uriTracking/TrackingState.cpp
#	${uriVL_sources_top}/uriTracking/PatternTrackingState.cpp

# BlobTracker2DController.cpp is a work in progress
#	${uriVL_sources_top}/uriTracking/BlobTracker2DController.cpp

# Tracker2D_PID.cpp is a work in progress
#		${uriVL_sources_top}/uriTracking/Tracker2D_PID.cpp
#	${uriVL_sources_top}/uriTracking/PatternTracker2D.cpp
#	${uriVL_sources_top}/uriTracking/BlobTracker2D.cpp

#------------------END uriTracking---------------------#		
		


#-----------------BEGIN uriVideoSources----------------#

	${uriVL_sources_top}/uriVideoSources/FrameGrabber.cpp
	${uriVL_sources_top}/uriVideoSources/ImageReader.cpp
	${uriVL_sources_top}/uriVideoSources/MoviePlayer.cpp
	${uriVL_sources_top}/uriVideoSources/VideoSource.cpp
# BufferRing is a work in progress
#		${uriVL_sources_top}/uriVideoSources/BufferRing.cpp

#------------------END uriVideoSources------------------#
)


#------------------BEGIN uriPrivate-----------------------#


#  If URIVL_USE_QUICKTIME is enabled we have a different set of include
#  files than if it is not enabled.
IF(URIVL_USE_QUICKTIME)

    SET(QuickTime_sources 
		${uriVL_private_sources}/_GraphWorld_QT.cpp
		${uriVL_private_sources}/_ImageWriter_QT.cpp        
		${uriVL_private_sources}/_MoviePlayer_QT.cpp
		${uriVL_private_sources}/_ImageReader_QT.cpp
		${uriVL_private_sources}/_FrameGrabber_QT.cpp
		${uriVL_private_sources}/_ImageTransformation_QT.cpp)

ELSE(URIVL_USE_QUICKTIME)

	IF(URIVL_USE_LIBQUICKTIME AND URIVL_USE_HACK_RW)
		SET(Generic_sources
			${uriVL_private_sources}/_GraphWorld_gen.cpp
			${uriVL_private_sources}/_FrameGrabber_gen.cpp
			${uriVL_private_sources}/_ImageWriter_hack.cpp
			${uriVL_private_sources}/_ImageReader_hack.cpp
			${uriVL_private_sources}/_MoviePlayer_libquicktime.cpp)

	ELSEIF(URIVL_USE_LIBQUICKTIME AND URIVL_USE_FREEIMAGE)
		SET(Generic_sources
			${uriVL_private_sources}/_GraphWorld_gen.cpp
			${uriVL_private_sources}/_ImageReader_FreeImage.cpp
			${uriVL_private_sources}/_ImageWriter_FreeImage.cpp
			${uriVL_private_sources}/_MoviePlayer_libquicktime.cpp
			${uriVL_private_sources}/_ImageTransformation_FreeImage.cpp)

	ELSEIF(URIVL_USE_FREEIMAGE)
		SET(Generic_sources
			${uriVL_private_sources}/_GraphWorld_gen.cpp
			${uriVL_private_sources}/_ImageWriter_FreeImage.cpp
			${uriVL_private_sources}/_ImageWriter_FreeImage.cpp
			${uriVL_private_sources}/_ImageTransformation_FreeImage.cpp)


	ENDIF(URIVL_USE_LIBQUICKTIME AND URIVL_USE_HACK_RW)

ENDIF(URIVL_USE_QUICKTIME)

#-------------------END uriPrivate-----------------------#



#  Specify our include directories for this project
INCLUDE_DIRECTORIES(${uriVL_headers})

#  Adjust our compiler flags based upon the type of configuration being built
#  NOTE: -Wno-multichar and -Wno-unknown-pragmas are used for two reasons.
#		1.  -Wno-multichar is used due to the crappy non-standards conformance
#			of the QuickTime SDK.
#		2.  -Wno-unknown-pragmas is used due to the variety of compilers that 
#			this library was built on.  The pragmas can safely be ignored though
#			since they are related to profiling and debugging.
IF(DEBUG)
	IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		SET(CMAKE_BUILD_TYPE Debug)
	    IF(SHARED)
	        SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -g3 -ggdb -gdwarf-2 -Wno-multichar -Wno-unknown-pragmas -shared -c")
	    ELSEIF(STATIC)
   	    	#  Set our compiler flags to give us 'all' warnings
   	    	SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -g3 -ggdb -gdwarf-2 -Wno-multichar -Wno-unknown-pragmas")
    	ELSEIF(FRAMEWORK)
    		# NOTE: Not yet supported
    	    #SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -g3 -ggdb -gdwarf-2 -Wno-multichar -Wno-unknown-pragmas")
    	    MESSAGE(FATAL_ERROR "Build configuration is not yet supported.")
    	ENDIF(SHARED)
    ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

ELSEIF(RELEASE)

	IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
   		SET(CMAKE_BUILD_TYPE Release)
    	IF(SHARED)
    	    SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -Wall -Wno-multichar -Wno-unknown-pragmas -shared -c")
    	ELSEIF(STATIC)
    	    SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -Wall -Wno-multichar -Wno-unknown-pragmas")
    	ELSEIF(FRAMEWORK)
    		# NOTE: Not yet supported
    		#SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -Wall -Wno-multichar -Wno-unknown-pragmas")
    		MESSAGE(FATAL_ERROR "Build configuration is not yet supported.")
    	ENDIF(SHARED)
   	ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)

ENDIF(DEBUG)

#  Next we need to handle the URIVL_USE_QUICKTIME version of the project
IF(URIVL_USE_QUICKTIME)

	#  Add URIVL_USE_QUICKTIME to our list of compiler definitions
	ADD_DEFINITIONS(-DURIVL_USE_QUICKTIME)
    
    IF(APPLE)
    	#  Include the nifty little script for finding frameworks on the MacOS
    	INCLUDE(CMakeFindFrameworks)
    	
    	#  Search for the OpenGL library
    	FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
    	IF(OPENGL_LIBRARY)
    		MESSAGE(STATUS "OpenGL.framework:\tFound")
    		INCLUDE_DIRECTORIES(${OPENGL_LIBRARY}/Headers)
    		LINK_LIBRARIES(${OPENGL_LIBRARY})
    	ELSE(OPENGL_LIBRARY)
    		MESSAGE(FATAL_ERROR "Unable to find OpenGL.")
    	ENDIF(OPENGL_LIBRARY)
    	
    	#  Search for the QuickTime library
    	FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime)
    	IF(QUICKTIME_LIBRARY)
    		MESSAGE(STATUS "QuickTime.framework:\tFound")
    		INCLUDE_DIRECTORIES(${QUICKTIME_LIBRARY}/Headers)
    		LINK_LIBRARIES(${QUICKTIME_LIBRARY})
    	ELSE(QUICKTIME_LIBRARY)
    		MESSAGE(FATAL_ERROR "Unable to locate QuickTime.")
    	ENDIF(QUICKTIME_LIBRARY)
    	
    	#  Search for the Carbon library
    	FIND_LIBRARY(CARBON_LIBRARY Carbon)
    	IF(CARBON_LIBRARY)
    		MESSAGE(STATUS "Carbon.framework:\tFound")
    		INCLUDE_DIRECTORIES(${CARBON_LIBRARY}/Headers)
    		LINK_LIBRARIES(${CARBON_LIBRARY})
    	ELSE(CARBON_LIBRARY)
    		MESSAGE(FATAL_ERROR "Unable to locate the Carbon.framework")
    	ENDIF(CARBON_LIBRARY)
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
    
    IF(UNIX)
    	IF(NOT APPLE)
    		MESSAGE(FATAL_ERROR "QuickTime is not supported on this system!")
    	ENDIF(NOT APPLE)
    ENDIF(UNIX)

#  If URIVL_USE_QUICKTIME is NOT enabled we do the following
#  Currently, the general version of the library makes
#  use of the libquicktime library, which is an open-source
#  cross-platform implementation of the QuickTime library.
ELSE(URIVL_USE_QUICKTIME)

    IF(URIVL_USE_LIBQUICKTIME)
		ADD_DEFINITIONS(-DURIVL_USE_LIBQUICKTIME)

		IF(APPLE)
			#  Include the nifty little script for finding frameworks on the MacOS
				INCLUDE(CMakeFindFrameworks)
			
			FIND_PATH(LQT_HEADERS lqt/lqt.h /opt/local/include)
			
			#  Check to see if the libquicktime headers are there
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
			
			#  Search for the OpenGL library
			FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
			IF(OPENGL_LIBRARY)
				MESSAGE(STATUS "OpenGL.framework:\tFound")
				INCLUDE_DIRECTORIES(${OPENGL_LIBRARY}/Headers)
				LINK_LIBRARIES(${OPENGL_LIBRARY})
			ELSE(OPENGL_LIBRARY)
				MESSAGE(FATAL_ERROR "Unable to find OpenGL.")
			ENDIF(OPENGL_LIBRARY)
			
		ENDIF(APPLE)
		
		IF(WIN32)
			IF(CYGWIN)
				#  Search for the lqt library if not using QuickTime
				FIND_LIBRARY(LQT_LIBRARY quicktime /usr/lib /usr/local/lib)
				IF (LQT_LIBRARY)
					LINK_LIBRARIES(${LQT_LIBRARY})
				ENDIF (LQT_LIBRARY)
			ENDIF(CYGWIN)
			
			IF(MINGW)
			
				#  Search for the freeimage library if not using QuickTime
				FIND_LIBRARY(FREEIMAGE_LIBRARY freeimage ${FREE_IMAGE_SDK})
				IF (FREEIMAGE_LIBRARY)
					LINK_LIBRARIES(${FREEIMAGE_LIBRARY})
				ELSE(FREEIMAGE_LIBRARY)
					MESSAGE(FATAL_ERROR "Unable to find FreeImage.")
				ENDIF (FREEIMAGE_LIBRARY)

			
			ENDIF(MINGW)
		ENDIF(WIN32)

		IF(UNIX)
			#  Search for the OpenGL library
			FIND_LIBRARY(OPENGL_LIBRARY GL)
			IF(OPENGL_LIBRARY)
				MESSAGE(STATUS "OpenGL:\tFound")
				#INCLUDE_DIRECTORIES(${OPENGL_LIBRARY}/Headers)
				LINK_LIBRARIES(${OPENGL_LIBRARY})
			ELSE(OPENGL_LIBRARY)
				MESSAGE(FATAL_ERROR "Unable to find OpenGL  on this system.")
			ENDIF(OPENGL_LIBRARY)

		ENDIF(UNIX)

    ENDIF(URIVL_USE_LIBQUICKTIME)

    IF(URIVL_USE_FREEIMAGE)
		ADD_DEFINITIONS(-DURIVL_USE_FREEIMAGE)
		IF(APPLE)
			#  Include the nifty little script for finding frameworks on the MacOS
				INCLUDE(CMakeFindFrameworks)
			
			FIND_PATH(LFREEIM_HEADERS FreeImage.h /opt/local/include)
			
			#  Check to see if the FreeImage headers are there
			IF(LFREEIM_HEADERS)
				INCLUDE_DIRECTORIES(${LFREEIM_HEADERS})
			ELSE(LFREEIM_HEADERS)
				MESSAGE(FATAL_ERROR "FreeImage not found!")
			ENDIF(LFREEIM_HEADERS)
			
			#  Check for the libquicktime binaries
			FIND_LIBRARY(LFREEIM_LIBRARY freeimage /opt/local/lib)
			IF(LFREEIM_LIBRARY)
				MESSAGE(STATUS "libfreeimage:\tFound")
				LINK_LIBRARIES(${LFREEIM_LIBRARY})
			ELSE(LFREEIM_LIBRARY)
				MESSAGE(FATAL_ERROR "FreeImage not found!")
			ENDIF(LFREEIM_LIBRARY)	 	
			
			#  Search for the OpenGL library
			FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
			IF(OPENGL_LIBRARY)
				MESSAGE(STATUS "OpenGL.framework:\tFound")
				INCLUDE_DIRECTORIES(${OPENGL_LIBRARY}/Headers)
				LINK_LIBRARIES(${OPENGL_LIBRARY})
			ELSE(OPENGL_LIBRARY)
				MESSAGE(FATAL_ERROR "Unable to find OpenGL.")
			ENDIF(OPENGL_LIBRARY)
			
		ENDIF(APPLE)
		
		IF(WIN32)
			IF(CYGWIN)
				#  Search for the lqt library if not using QuickTime
				FIND_LIBRARY(LQT_LIBRARY quicktime /usr/lib /usr/local/lib)
				IF (LQT_LIBRARY)
					LINK_LIBRARIES(${LQT_LIBRARY})
				ENDIF (LQT_LIBRARY)
			ENDIF(CYGWIN)
			
			IF(MINGW)
			ENDIF(MINGW)
		ENDIF(WIN32)

		IF(UNIX)
			#  Search for the OpenGL library
			FIND_LIBRARY(OPENGL_LIBRARY GL)
			IF(OPENGL_LIBRARY)
				MESSAGE(STATUS "OpenGL:\tFound")
				#INCLUDE_DIRECTORIES(${OPENGL_LIBRARY}/Headers)
				LINK_LIBRARIES(${OPENGL_LIBRARY})
			ELSE(OPENGL_LIBRARY)
				MESSAGE(FATAL_ERROR "Unable to find OpenGL  on this system.")
			ENDIF(OPENGL_LIBRARY)

		ENDIF(UNIX)

    ENDIF(URIVL_USE_FREEIMAGE)


ENDIF(URIVL_USE_QUICKTIME)

#  Next we need to define where to output the library to. 
#  Since the uriVL is laid out in such a way that there is
#  a binary folder for each distribution/configuration, depending
#  on what OS and configuration the binary library is outputted
#  appropriately.
IF(WIN32)
    IF(DEBUG)
        SET(LIBRARY_OUTPUT_PATH ${uriVL_libraries_top}/Win32/Debug)
    ELSE(DEBUG)
        SET(LIBRARY_OUTPUT_PATH ${uriVL_libraries_top}/Win32/Release)
    ENDIF(DEBUG)
ENDIF(WIN32)

IF(UNIX)
    IF(APPLE)
        IF(DEBUG)
            SET(LIBRARY_OUTPUT_PATH ${uriVL_libraries_top}/Mac\ OS/Debug)
        ELSE(DEBUG)
            SET(LIBRARY_OUTPUT_PATH ${uriVL_libraries_top}/Mac\ OS/Release)
        ENDIF(DEBUG)
    ELSEIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
        IF(DEBUG)
            SET(LIBRARY_OUTPUT_PATH ${uriVL_libraries_top}/Linux/Debug)
        ELSE(DEBUG)
            SET(LIBRARY_OUTPUT_PATH ${uriVL_libraries_top}/Linux/Release)
        ENDIF(DEBUG)
    ENDIF(APPLE)
ENDIF(UNIX)

#  Determine what form of the library should be built based upon what
#  options are enabled.
IF(SHARED)
    ADD_LIBRARY(uriVL SHARED 
                ${uriVL_sources} ${QuickTime_sources} ${Generic_sources})
ELSEIF(STATIC)
    ADD_LIBRARY(uriVL STATIC 
#                ${uriVL_sources} ${QuickTime_sources} ${Generic_sources})
                ${uriVL_sources} ${QuickTime_sources}  ${Generic_sources})
ELSEIF(FRAMEWORK)
    
ENDIF(SHARED)
