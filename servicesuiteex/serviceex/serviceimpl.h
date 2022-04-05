#pragma once
#include "serviceex.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <string>

namespace ServiceSuiteEx
{
    class ServiceImpl final : public ServiceEx::Service
    {
    public:
        grpc::Status unaryRPC(grpc::ServerContext* context, const RequestMsgEx* request, ResponseMsgEx* reply) override;
        void run();
    };
}
