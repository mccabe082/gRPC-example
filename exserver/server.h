#pragma once
#include <grpcpp/grpcpp.h>
#include "exserviceapi/serviceimpl.h"

namespace ExServer
{
    class Server
    {
    public:
        void run();
    private:
        std::unique_ptr<grpc::Server> mGRPCDaemon;
        std::unique_ptr<ExServiceAPI::Service> mExService;
    };
}
