SET(demo_mainDIFF_SRCS 
   ../../videoDifference_main.cpp
)

INCLUDE_DIRECTORIES(${uriVisionLib_header_dirs})

LINK_DIRECTORIES(${uriVisionLib_BINARY_DIR}/Development/Source/C++)

ADD_EXECUTABLE(videoDiff ${demo_mainDIFF_SRCS})

TARGET_LINK_LIBRARIES(videoDiff glut GLU GL)
TARGET_LINK_LIBRARIES(videoDiff uriVision)
TARGET_LINK_LIBRARIES(videoDiff uriVision)
