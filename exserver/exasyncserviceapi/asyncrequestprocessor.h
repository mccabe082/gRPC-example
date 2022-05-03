#pragma once
#include <grpcpp/grpcpp.h>
#include "exserviceapi.grpc.pb.h"

#define UNIQUE_REQUEST_ID this  // quasi-UID

namespace ExServer
{
    class AsyncRequestProcessingUnit
    {
    public:
        AsyncRequestProcessingUnit(ExServiceAPI::AsyncService& service, grpc::ServerCompletionQueue& processedQ);
        ~AsyncRequestProcessingUnit();
        void process(void* tag);

    private:
        ExServiceAPI::AsyncService& mService;
        grpc::ServerCompletionQueue& mCompletionQ;
        grpc::ServerContext mServerContext;
        std::unique_ptr<grpc::ServerAsyncResponseWriter<ResponseMsgEx>> mResponderPtr;
    };

}
