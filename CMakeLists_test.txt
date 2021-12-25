cmake_minimum_required(VERSION 2.8)

project(netlib LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILEON ON)

#add_subdirectory(code)
#add_subdirectory(examples)

aux_source_directory (code SRC_LIST)
#aux_source_directory (examples SRC_LIST)

#FILE(GLOB SOURCE_1 "${CMAKE_SOURCE_DIR}/code/*.cpp") 
#FILE(GLOB SOURCE_2 "${CMAKE_SOURCE_DIR}/examples/*.cpp")
message(STATUS "SRC_LIST ${SRC_LIST}")
 
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall" )
message(STATUS "cxx flags ${CMAKE_CXX_FLAGS}")

#SET(CMAKE_BUILD_TYPE "Release")
#message(STATUS "cxx flags ${CMAKE_CXX_FLAGS}")

# cancel -rdynamic link option
#SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)


INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/code")
#include_directories (code)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)

message(STATUS "CMAKE_THREAD_LIBS_INIT ${CMAKE_THREAD_LIBS_INIT}")
find_package(Threads)
message(STATUS "CMAKE_THREAD_LIBS_INIT ${CMAKE_THREAD_LIBS_INIT}")

# link static lib
#link_directories( ${PROJECT_SOURCE_DIR}/lib/linux)
#link_libraries(libnetlib.a)

# add the executable
add_executable(echo ${SRC_LIST})
#add_library(库名称 STATIC 代码文件名称)
#add_library(库名称 SHARED 代码文件名称) 

target_link_libraries(echo -lpthread) #${CMAKE_THREAD_LIBS_INIT}
