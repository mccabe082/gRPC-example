#include "clientex.h"

namespace
{
    grpc::ChannelArguments channelOptions()
    {
        // https://grpc.github.io/grpc/cpp/classgrpc_1_1_channel_arguments.html
        // https://grpc.io/docs/what-is-grpc/core-concepts/#channels

        grpc::ChannelArguments args;
        args.SetCompressionAlgorithm(GRPC_COMPRESS_GZIP);
        return args;
    }

    std::shared_ptr<grpc::Channel> createChannel()
    {
        // https://grpc.github.io/grpc/cpp/classgrpc_1_1_channel.html#details
        // https://grpc.io/docs/what-is-grpc/core-concepts/#channels

        return grpc::CreateCustomChannel(
                "localhost:50051",
                grpc::InsecureChannelCredentials(),
                channelOptions());
    }
}

ClientEx::ClientEx() : _stub(ServiceSuiteEx::ServiceEx::NewStub(createChannel()))
{}

void ClientEx::callUnaryRPCWithMetaData()
{
    // https://grpc.io/docs/what-is-grpc/core-concepts/#unary-rpc

    ServiceSuiteEx::RequestMsgEx request;
    request.set_foo("echo");
    ServiceSuiteEx::ResponseMsgEx response;

    grpc::ClientContext c;
    c.set_compression_algorithm(GRPC_COMPRESS_DEFLATE);

    // 1. Add Client Metadata
    c.AddMetadata("client-metadata-key1", "client metadata value1");
    c.AddMetadata("client-metadata-key2", "client metadata value2");

    // 2. Remote procedure call on stub
    grpc::Status ier = _stub->unaryRPC(&c, request, &response);

    if (ier.ok())
    {
        // 3. Access Initial Server Metadata
        const auto& initMetadata =  c.GetServerInitialMetadata();
        std::cout << "\nClient received initial metadata from server: " << std::endl;
        std::cout << "\t" << initMetadata.find("server-initial-metadata-key1")->second << std::endl;
        std::cout << "\t" << initMetadata.find("server-initial-metadata-key2")->second << std::endl;
        std::cout << std::endl;

        // 4. Access Trailing Server Metadata
        const auto& trailingMetadata =  c.GetServerTrailingMetadata();
        std::cout << "Client received trailing metadata from server: " << std::endl;
        std::cout << "\t" << trailingMetadata.find("server-trailing-metadata-key1")->second << std::endl;
        std::cout << "\t" << trailingMetadata.find("server-trailing-metadata-key2")->second << std::endl;
        std::cout << std::endl;

        // 5. Access RPC Message
        std::cout << "Client received following response from server: " << std::endl;
        std::cout << "\t[" << response.bar() << "]" << std::endl;
        std::cout << std::endl;

        return;
    }

    std::cout << ier.error_code() << ": " << ier.error_message();
}

void ClientEx::callUnaryRPCWithMetaData_ASyncClient()
{
    ServiceSuiteEx::RequestMsgEx request;
    ServiceSuiteEx::ResponseMsgEx response;
    grpc::ClientContext context;
    grpc::CompletionQueue cq; // The producer-consumer queue we use to communicate asynchronously with the gRPC runtime.
    grpc::Status status;

    using DeferredResponse = std::unique_ptr<grpc::ClientAsyncResponseReader<ServiceSuiteEx::ResponseMsgEx>>;
    DeferredResponse nonBlockingRPC(_stub->PrepareAsyncunaryRPC(&context, request, &cq));

    bool ok = false;
    std::string tag("Super RPC");
    void* RPCOutID = &tag;
    void* RPCInID = nullptr;
    nonBlockingRPC->StartCall();
    nonBlockingRPC->Finish(&response, &status, RPCOutID);

    GPR_ASSERT(cq.Next(&RPCInID, &ok));
    GPR_ASSERT(RPCInID == RPCOutID);
    GPR_ASSERT(ok);

    std::cout << (status.ok() ? response.bar() : "error") << std::endl;

    return;
}

void ClientEx::callServerStreamingRPC()
{
    std::cout << "Client requesting stream from server:\n\t" << std::flush;
    grpc::ClientContext c;
    const ServiceSuiteEx::RequestMsgEx request;
    ServiceSuiteEx::ResponseMsgEx response;

    std::unique_ptr<grpc::ClientReader<ServiceSuiteEx::ResponseMsgEx>> reader(_stub->serverStreamingRPC(&c,request));
    while (reader->Read(&response))
    {
        std::cout << response.bar() << "." << std::flush;
    }
    std::cout << std::endl;
    grpc::Status status = reader->Finish();

    if (status.ok()) {
        std::cout << "\t...streaming complete\n\n" << std::flush;
        return;
    }

    std::cout << status.error_code() << ": " << status.error_message();
}

void ClientEx::callClientStreamingRPC()
{
    std::cout << "Client requesting stream to server:" << std::endl;
    grpc::ClientContext c;
    ServiceSuiteEx::RequestMsgEx request;
    ServiceSuiteEx::ResponseMsgEx response;

    std::unique_ptr<grpc::ClientWriter<ServiceSuiteEx::RequestMsgEx>> writer(_stub->clientStreamingRPC(&c,&response));
    for (int i = 0; i<10; ++i)
    {
        request.set_foo(std::to_string(i));
        writer->Write(request);
    }

    writer->WritesDone();
    grpc::Status status = writer->Finish();

    if (status.ok())
    {
        std::cout << "\t...streaming complete\n\n" << std::flush;
        return;
    }

    std::cout << status.error_code() << ": " << status.error_message();
}


void ClientEx::callBidirectionalStreamingRPC()
{
    std::cout << "Client requesting bi-directional stream with server:\n\t" << std::flush;
    grpc::ClientContext c;
    ServiceSuiteEx::RequestMsgEx request;
    ServiceSuiteEx::ResponseMsgEx response;
    auto&& stream = _stub->bidirectionalStreamingRPC(&c);

    for (int i = 0; i<10; ++i)
    {
        request.set_foo(std::to_string(i));
        stream->Write(request);
        stream->Read(&response);
        std::cout << response.bar() << "." << std::flush;
    }
    std::cout << std::endl;

    stream->WritesDone();
    grpc::Status status = stream->Finish();

    if (status.ok())
    {
        std::cout << "\t...streaming complete\n\n" << std::flush;
        return;
    }

    std::cout << status.error_code() << ": " << status.error_message();
}
