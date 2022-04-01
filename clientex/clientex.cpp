#include "clientex.h"

ClientEx::ClientEx(std::shared_ptr<grpc::Channel> channel) :_stub(ServiceSuiteEx::ServiceEx::NewStub(channel)) {}

void ClientEx::demonstrateRemoteProcedureCall()
{
    ServiceSuiteEx::RequestMsgEx request;
    request.set_foo("echo");
    ServiceSuiteEx::ResponseMsgEx response;
    grpc::ClientContext cookies;

    grpc::Status ier = _stub->requestEx(&cookies, request, &response);

    if (ier.ok())
    {
        std::cout<<response.bar()<<std::endl;
        return;
    }

    std::cout << ier.error_code() << ": " << ier.error_message();
}