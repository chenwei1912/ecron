# cancel -rdynamic link option
#SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
#SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

#link_directories( ${PROJECT_SOURCE_DIR}/output)
#link_libraries(libnetlib.a)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/tests)

add_executable(buffer_test Buffer_test.cpp main.cpp)

target_link_libraries(buffer_test netlib)
