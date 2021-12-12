aux_source_directory(. SRC_LIST)
message(STATUS "SRC_LIST ${SRC_LIST}")

#set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/output)


add_library(netlib STATIC ${SRC_LIST})
#target_link_libraries(netlib pthread rt)
