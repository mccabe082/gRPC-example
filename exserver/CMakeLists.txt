cmake_minimum_required(VERSION 3.12.1)
project(example-server LANGUAGES CXX)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

########################################################################################################################
# Generate gRPC/Proto files
get_filename_component(SERVICEEX_PROTO_PATH "../dtos/exserver/exserviceapi.proto" ABSOLUTE)
get_filename_component(SERVICEEX_PROTO_DIR "${SERVICEEX_PROTO_PATH}" DIRECTORY)
find_program(PROTOC_EXECUTABLE protoc)
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

add_custom_command(
        OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/exserviceapi.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/exserviceapi.pb.h
        ${CMAKE_CURRENT_BINARY_DIR}/exserviceapi.grpc.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/exserviceapi.grpc.pb.h
        COMMAND ${PROTOC_EXECUTABLE}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${SERVICEEX_PROTO_DIR}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${SERVICEEX_PROTO_PATH}"
        DEPENDS "${SERVICEEX_PROTO_PATH}"
)
########################################################################################################################

add_executable(exserver)
target_sources(exserver
PRIVATE
        main.cpp
        server.h
        server.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/exserviceapi.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/exserviceapi.pb.h
        ${CMAKE_CURRENT_BINARY_DIR}/exserviceapi.grpc.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/exserviceapi.grpc.pb.h
)
target_include_directories(exserver
PUBLIC
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(exserver
PRIVATE
        gRPC::grpc++_reflection
        gRPC::grpc++
        protobuf::libprotobuf
)
add_subdirectory(exserviceapi)