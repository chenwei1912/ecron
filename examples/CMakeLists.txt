# cancel -rdynamic link option
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

#link_directories( ${PROJECT_SOURCE_DIR}/output)
#link_libraries(libnetlib.a)

add_executable(echo echo.cpp)
add_executable(client client.cpp)

target_link_libraries(echo ecron pthread)
target_link_libraries(client ecron pthread)


add_subdirectory(http)
add_subdirectory(rpc)
