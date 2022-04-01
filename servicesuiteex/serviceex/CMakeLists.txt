cmake_minimum_required(VERSION 3.12.1)
project(gRPC-service LANGUAGES CXX)

find_package(Protobuf REQUIRED)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../../dtos/servicesuiteex/serviceex.proto)

add_executable(serviceex)
target_sources(serviceex
PUBLIC
        main.cpp
        serviceimpl.h
        serviceimpl.cpp
        ${PROTO_HDRS}
        ${PROTO_SRCS}
)
target_include_directories(serviceex
PUBLIC
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(serviceex
PUBLIC
        ${Protobuf_LIBRARIES}
)