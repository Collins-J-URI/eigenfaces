SET(demo_Optimization_SRCS 
   ../../Sources/optimization_main.cpp
)

INCLUDE_DIRECTORIES(${uriVisionLib_header_dirs})

LINK_DIRECTORIES(${uriVisionLib_BINARY_DIR}/Development/Source/C++)

ADD_EXECUTABLE(optimization ${demo_Optimization_SRCS})

TARGET_LINK_LIBRARIES(optimization glut GLU GL)
TARGET_LINK_LIBRARIES(optimization uriVision)
TARGET_LINK_LIBRARIES(optimization uriVision)
