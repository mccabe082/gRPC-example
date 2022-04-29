#pragma once
#include <memory>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "exserviceapi.grpc.pb.h"

class ClientEx
{
public:
    ClientEx();
    void callUnaryRPCWithMetaData();
    void callUnaryRPCWithMetaData_ASyncClient();

    void callServerStreamingRPC();
    void callClientStreamingRPC();
    void callBidirectionalStreamingRPC();
private:
    std::unique_ptr<ExServer::ExServiceAPI::Stub> _stub;
};
