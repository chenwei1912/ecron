# cancel -rdynamic link option
#SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
#SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

link_directories(${PROJECT_SOURCE_DIR}/output)
link_libraries(libgflags.a)
link_libraries(libnetlib.a)

add_executable(rpc_server server.cpp RpcServer.cpp RpcChannel.cpp RpcController.cpp rpc.pb.cc my_service.pb.cc)
add_executable(rpc_client client.cpp RpcClient.cpp RpcChannel.cpp RpcController.cpp rpc.pb.cc my_service.pb.cc)

target_link_libraries(rpc_server protobuf pthread)
target_link_libraries(rpc_client protobuf pthread)