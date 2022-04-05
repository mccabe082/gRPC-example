#pragma once
#include <memory>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "serviceex.grpc.pb.h"

class ClientEx
{
public:
    ClientEx();
    void callUnaryRPCWithMetaData();
private:
    std::unique_ptr<ServiceSuiteEx::ServiceEx::Stub> _stub;
    std::unique_ptr<grpc::ClientContext> _context;
};

