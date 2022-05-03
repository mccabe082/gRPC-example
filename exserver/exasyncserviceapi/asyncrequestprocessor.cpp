#include "asyncrequestprocessor.h"

namespace ExServer
{
    AsyncRequestProcessingUnit::AsyncRequestProcessingUnit(ExServiceAPI::AsyncService& service, grpc::ServerCompletionQueue& processedQ) :
            mService(service), mCompletionQ(processedQ)
    {}

    AsyncRequestProcessingUnit::~AsyncRequestProcessingUnit(){mCompletionQ.Shutdown();}


    void AsyncRequestProcessingUnit::process(void* tag)
    {
        RequestMsgEx _request;
        ResponseMsgEx _response;

        static long iTag = 0;
        tag = (void *) iTag++;

        // start listening/tagging
        {
            mService.RequestunaryRPC(
                    &mServerContext,
                    &_request,
                    mResponderPtr.get(),
                    &mCompletionQ,
                    &mCompletionQ,
                    tag);
        }

        // Block waiting to read the next event from the completion queue.
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or cq_ is shutting down.
        {
            bool ok;
            GPR_ASSERT(mCompletionQ.Next(&tag, &ok));  // block
            GPR_ASSERT(ok);
        }

        // respond and stop listening
        {
            _response.set_bar(_request.foo());
            mResponderPtr->Finish(_response, grpc::Status::OK, tag);
        }
    }
}
