# cancel -rdynamic link option
#SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
#SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

link_directories( ${PROJECT_SOURCE_DIR}/output)
link_libraries(libecron.a)
link_libraries(libgflags.a)
link_libraries(libboost_filesystem.a)

add_executable(http HttpMain.cpp HttpServer.cpp HttpTask.cpp HttpRequest.cpp HttpResponse.cpp SqlConnPool.cpp http_parser.c)

target_link_libraries(http pthread mysqlclient)
