#pragma once
#include "serviceex.pb.h"
#include <grpcpp/grpcpp.h>
#include <string>

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

namespace ServiceSuiteEx
{
    class ServiceImpl final : public ServiceEx::Service
    {
        grpc::Status requestEx(ServerContext* context, const RequestMsgEx* request, ResponseMsgEx* reply) override
        {
            const std::string msg = request->foo();
            reply->set_bar(msg);
            return grpc::Status::OK;
        }
        void run();
    };
}
