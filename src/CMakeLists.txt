cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_PREFIX_PATH  $ENV{ROOTSYS})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/etc/cmake)

find_package(ROOT REQUIRED COMPONENTS Hist MathCore Tree RIO Net)

include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

add_library(src 
    SHARED
    optimizeCW.cxx
    DrawCW.cxx
)
target_include_directories(src
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    ${ROOT_INCLUDE_DIRS}
)
target_link_libraries(src ${ROOT_LIBRARIES} )
