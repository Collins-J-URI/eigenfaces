SET(demo_Video_Normal_Flow_SRCS 
   ../../videoNormalFlow_main.cpp
)

INCLUDE_DIRECTORIES(${uriVisionLib_header_dirs})

LINK_DIRECTORIES(${uriVisionLib_BINARY_DIR}/Development/Source/C++)

ADD_EXECUTABLE(videoNormalFlow ${demo_Video_Normal_Flow_SRCS})

TARGET_LINK_LIBRARIES(videoNormalFlow glut GLU GL)
TARGET_LINK_LIBRARIES(videoNormalFlow uriVision)
TARGET_LINK_LIBRARIES(videoNormalFlow uriVision)
