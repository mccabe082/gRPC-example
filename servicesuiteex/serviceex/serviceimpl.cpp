#include "serviceimpl.h"
#include <string>
#include <iostream>
namespace
{
    const std::string SERVER_ADDRESS = "0.0.0.0:50051";

    std::unique_ptr<grpc::ServerContext> createContext()
    {
        // https://grpc.github.io/grpc/cpp/classgrpc_1_1_server_context.html
        // https://grpc.io/docs/what-is-grpc/core-concepts/#metadata

        auto c = std::make_unique<grpc::ServerContext>();
        c->set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
        return c;
    }

    void configureServer(grpc::ServerBuilder& b)
    {
        // https://grpc.github.io/grpc/cpp/classgrpc_1_1_server_builder.html
        b.AddListeningPort(SERVER_ADDRESS, grpc::InsecureServerCredentials());
        b.SetDefaultCompressionAlgorithm(GRPC_COMPRESS_GZIP);
    }
}

namespace ServiceSuiteEx
{
    grpc::Status ServiceImpl::requestEx(grpc::ServerContext* context, const RequestMsgEx* request, ResponseMsgEx* reply)
    {
        const std::string msg = request->foo();
        std::cout << msg << std::endl;
        reply->set_bar(msg);
        return grpc::Status::OK;
    }

    void ServiceImpl::run()
    {
        ServiceImpl serviceInstance;
        grpc::EnableDefaultHealthCheckService(true);
        //grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        grpc::ServerBuilder builder;
        configureServer(builder);
        builder.RegisterService(&serviceInstance);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << SERVER_ADDRESS << std::endl;
        server->Wait();
    }
}