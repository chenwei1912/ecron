cmake_minimum_required(VERSION 2.8)

project(netlib)

#set(CMAKE_VERBOSE_MAKEFILEON ON)

#FILE(GLOB SOURCE_1 "${CMAKE_SOURCE_DIR}/code/*.cpp") 
#FILE(GLOB SOURCE_2 "${CMAKE_SOURCE_DIR}/examples/*.cpp")
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
#message(STATUS "cxx flags ${CMAKE_CXX_FLAGS}")

#SET(CMAKE_BUILD_TYPE "Release")
#message(STATUS "cxx flags ${CMAKE_CXX_FLAGS}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)

#set (BOOST_ROOT /mnt/d/linux/boost_1_77_0)
#set (BOOST_INCLUDEDIR ${BOOST_ROOT}/include)
#set (BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib)

include_directories(${PROJECT_SOURCE_DIR}/code)

add_subdirectory(code)
add_subdirectory(examples)
