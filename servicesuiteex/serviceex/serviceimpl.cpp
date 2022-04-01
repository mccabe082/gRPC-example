#include "serviceimpl.h"
#include <string>

namespace
{
    const std::string SERVER_ADDRESS = "0.0.0.0:50051";
}

namespace ServiceSuiteEx
{
    grpc::Status ServiceImpl::requestEx(grpc::ServerContext* context, const RequestMsgEx* request, ResponseMsgEx* reply)
    {
        const std::string msg = request->foo();
        reply->set_bar(msg);
        return grpc::Status::OK;
    }

    void ServiceImpl::run()
    {
        ServiceImpl serviceInstance;
        grpc::EnableDefaultHealthCheckService(true);
        //grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        grpc::ServerBuilder builder;
        builder.AddListeningPort(SERVER_ADDRESS, grpc::InsecureServerCredentials());
        builder.RegisterService(&serviceInstance);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << SERVER_ADDRESS << std::endl;
        server->Wait();
    }
}