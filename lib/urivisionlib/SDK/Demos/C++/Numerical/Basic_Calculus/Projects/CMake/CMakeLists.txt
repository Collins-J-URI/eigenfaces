SET(demo_BasicCalculus_SRCS 
   ../../BasicCalculus_main.cpp
)

INCLUDE_DIRECTORIES(${uriVisionLib_header_dirs})

LINK_DIRECTORIES(${uriVisionLib_BINARY_DIR}/Development/Source/C++)

ADD_EXECUTABLE(BasicCalculus ${demo_BasicCalculus_SRCS})

TARGET_LINK_LIBRARIES(BasicCalculus glut GLU GL)
TARGET_LINK_LIBRARIES(BasicCalculus uriVision)
TARGET_LINK_LIBRARIES(BasicCalculus uriVision)
