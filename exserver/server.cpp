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
    }

    void Server::setup()
    {
        mServiceBuilderPtr->AddListeningPort(SERVER_ADDRESS, grpc::InsecureServerCredentials());
        mServiceBuilderPtr->SetDefaultCompressionAlgorithm(GRPC_COMPRESS_GZIP);

        mExServicePtr = std::make_unique<ServiceEndPoints>();
        mServiceBuilderPtr->RegisterService(mExServicePtr.get());
    }

    void Server::launch()
    {
        mGRPCDaemonPtr = mServiceBuilderPtr->BuildAndStart();

        std::cout << "Server listening on " << SERVER_ADDRESS << std::endl;
        mGRPCDaemonPtr->Wait();
    }
}
