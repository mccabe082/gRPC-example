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

        grpc::Status serverStreamingRPC(grpc::ServerContext* context, const RequestMsgEx* request, grpc::ServerWriter<ResponseMsgEx>* writer) override;

        grpc::Status clientStreamingRPC(grpc::ServerContext* context, grpc::ServerReader<RequestMsgEx>* reader, ResponseMsgEx* response) override;

        grpc::Status bidirectionalStreamingRPC(grpc::ServerContext* context, grpc::ServerReaderWriter<ResponseMsgEx, RequestMsgEx>* stream) override;

        void run();
    };
}
