#include "server.h"

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
}

namespace ExServer
{
    void Server::run()
    {
        grpc::EnableDefaultHealthCheckService(true);

        grpc::ServerBuilder b;
        b.AddListeningPort(SERVER_ADDRESS, grpc::InsecureServerCredentials());
        b.SetDefaultCompressionAlgorithm(GRPC_COMPRESS_GZIP);

        mExService = std::make_unique<ServiceEndPoints>();
        b.RegisterService(mExService.get());

        mGRPCDaemon = b.BuildAndStart();

        std::cout << "Server listening on " << SERVER_ADDRESS << std::endl;
        mGRPCDaemon->Wait();
    }
}
