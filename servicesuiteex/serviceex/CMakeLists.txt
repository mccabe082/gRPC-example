cmake_minimum_required(VERSION 3.12.1)
project(gRPC-service LANGUAGES CXX)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

########################################################################################################################
# Generate gRPC/Proto files
get_filename_component(SERVICEEX_PROTO_PATH "../../dtos/servicesuiteex/serviceex.proto" ABSOLUTE)
get_filename_component(SERVICEEX_PROTO_DIR "${SERVICEEX_PROTO_PATH}" DIRECTORY)
find_program(PROTOC_EXECUTABLE protoc)
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

add_custom_command(
        OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/serviceex.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/serviceex.pb.h
        ${CMAKE_CURRENT_BINARY_DIR}/serviceex.grpc.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/serviceex.grpc.pb.h
        COMMAND ${PROTOC_EXECUTABLE}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${SERVICEEX_PROTO_DIR}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${SERVICEEX_PROTO_PATH}"
        DEPENDS "${SERVICEEX_PROTO_PATH}"
)
########################################################################################################################


# protoc --grpc_out /home/david/projects/github-mccabe082/gRPC-example/servicesuiteex/serviceex/cmake-build-debug --cpp_out /home/david/projects/github-mccabe082/gRPC-example/servicesuiteex/serviceex/cmake-build-debug -I /home/david/projects/github-mccabe082/gRPC-example/dtos/servicesuiteex --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin /home/david/projects/github-mccabe082/gRPC-example/dtos/servicesuiteex/serviceex.proto

add_executable(serviceex)
target_sources(serviceex
PUBLIC
        main.cpp
        serviceimpl.h
        serviceimpl.cpp
PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/serviceex.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/serviceex.pb.h
        ${CMAKE_CURRENT_BINARY_DIR}/serviceex.grpc.pb.cc
        ${CMAKE_CURRENT_BINARY_DIR}/serviceex.grpc.pb.h
)
target_include_directories(serviceex
PUBLIC
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(serviceex
PRIVATE
        gRPC::grpc++_reflection
        gRPC::grpc++
        protobuf::libprotobuf
)
