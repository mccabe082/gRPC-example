#include "server.h"

namespace
{
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
    Server::Server(const std::string& serverAddress) : SERVER_ADDRESS(serverAddress)
    {
        // not entirely sure what this line does atm...
        grpc::EnableDefaultHealthCheckService(true);

        mServiceBuilder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        mServiceBuilder.SetDefaultCompressionAlgorithm(GRPC_COMPRESS_GZIP);

        mExService = std::make_unique<ServiceEndPoints>();
        mServiceBuilder.RegisterService(mExService.get());
    }

    void Server::run()
    {
        mGRPCDaemon = mServiceBuilder.BuildAndStart();

        std::cout << "Server listening on " << SERVER_ADDRESS << std::endl;
        mGRPCDaemon->Wait();
    }
}
