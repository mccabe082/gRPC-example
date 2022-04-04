#include "clientex.h"

namespace
{
    std::unique_ptr<grpc::ClientContext> createContext()
    {
        // https://grpc.github.io/grpc/cpp/classgrpc_1_1_client_context.html
        // https://grpc.io/docs/what-is-grpc/core-concepts/#metadata

        auto c = std::make_unique<grpc::ClientContext>();
        c->set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
        return c;
    }

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

ClientEx::ClientEx() :
    _stub(ServiceSuiteEx::ServiceEx::NewStub(createChannel())),
    _context(createContext())
{}

void ClientEx::demonstrateRemoteProcedureCall()
{
    ServiceSuiteEx::RequestMsgEx request;
    request.set_foo("echo");
    ServiceSuiteEx::ResponseMsgEx response;
    auto context = createContext();

    grpc::Status ier = _stub->requestEx(_context.get(), request, &response);

    if (ier.ok())
    {
        std::cout<<response.bar()<<std::endl;
        return;
    }

    std::cout << ier.error_code() << ": " << ier.error_message();
}


