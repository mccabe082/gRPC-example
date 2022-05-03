#pragma once
#include <grpcpp/grpcpp.h>
#include "exserviceapi/serviceendpoints.h"

namespace ExServer
{
    class Server
    {
    public:
        Server(const std::string& serverAddress);
        void setup();
        void launch();
    private:
        std::unique_ptr<grpc::Server> mGRPCDaemonPtr;
        std::unique_ptr<ExServiceAPI::Service> mExServicePtr;
        std::unique_ptr<grpc::ServerBuilder> mServiceBuilderPtr;
        const std::string SERVER_ADDRESS;
    };
}
