SET(demo_mainIP_SRCS 
   ../../main_IP.cpp
)

INCLUDE_DIRECTORIES(${uriVisionLib_header_dirs})

LINK_DIRECTORIES(${uriVisionLib_BINARY_DIR}/Development/Source/C++)

ADD_EXECUTABLE(simpleIP ${demo_mainIP_SRCS})

TARGET_LINK_LIBRARIES(simpleIP glut GLU GL)
TARGET_LINK_LIBRARIES(simpleIP uriVision)
TARGET_LINK_LIBRARIES(simpleIP uriVision)
