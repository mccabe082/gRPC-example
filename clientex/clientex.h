#pragma once
#include <memory>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "serviceex.grpc.pb.h"

class ClientEx
{
public:
    ClientEx(std::shared_ptr<grpc::Channel> channel);
    void demonstrateRemoteProcedureCall();
private:
    std::unique_ptr<ServiceSuiteEx::ServiceEx::Stub> _stub;
};

