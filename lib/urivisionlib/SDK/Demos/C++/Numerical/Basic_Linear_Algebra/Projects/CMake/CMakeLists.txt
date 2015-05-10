SET(demo_BasicLinearAlgebra_SRCS 
   ../../BasicLinearAlgebra_main.cpp
)

INCLUDE_DIRECTORIES(${uriVisionLib_header_dirs})

LINK_DIRECTORIES(${uriVisionLib_BINARY_DIR}/Development/Source/C++)

ADD_EXECUTABLE(BasicLinearAlgebra ${demo_BasicLinearAlgebra_SRCS})

TARGET_LINK_LIBRARIES(BasicLinearAlgebra glut GLU GL)
TARGET_LINK_LIBRARIES(BasicLinearAlgebra uriVision)
TARGET_LINK_LIBRARIES(BasicLinearAlgebra uriVision)
