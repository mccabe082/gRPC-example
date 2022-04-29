#pragma once
#include "exserviceapi.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <string>

namespace ExServer
{
    class ServiceEndPoints final : public ExServiceAPI::Service
    {
    public:
        grpc::Status unaryRPC(grpc::ServerContext* context, const RequestMsgEx* request, ResponseMsgEx* reply) override;

        grpc::Status serverStreamingRPC(grpc::ServerContext* context, const RequestMsgEx* request, grpc::ServerWriter<ResponseMsgEx>* writer) override;

        grpc::Status clientStreamingRPC(grpc::ServerContext* context, grpc::ServerReader<RequestMsgEx>* reader, ResponseMsgEx* response) override;

        grpc::Status bidirectionalStreamingRPC(grpc::ServerContext* context, grpc::ServerReaderWriter<ResponseMsgEx, RequestMsgEx>* stream) override;
    };
}
