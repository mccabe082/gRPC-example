#pragma once
#include <grpcpp/grpcpp.h>
#include "exserviceapi/serviceendpoints.h"

namespace ExServer
{
    class Server
    {
    public:
        Server(const std::string& serverAddress);
        void run();
    private:
        std::unique_ptr<grpc::Server> mGRPCDaemon;
        std::unique_ptr<ExServiceAPI::Service> mExService;
        grpc::ServerBuilder mServiceBuilder;
        const std::string SERVER_ADDRESS;
    };
}
