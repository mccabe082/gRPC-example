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

void ClientEx::callUnaryRPCWithMetaData()
{
    // https://grpc.io/docs/what-is-grpc/core-concepts/#unary-rpc

    ServiceSuiteEx::RequestMsgEx request;
    request.set_foo("echo");
    ServiceSuiteEx::ResponseMsgEx response;

    // 1. Add Client Metadata
    _context->AddMetadata("client-metadata-key1", "client metadata value1");
    _context->AddMetadata("client-metadata-key2", "client metadata value2");

    // 2. Remote procedure call on stub
    grpc::Status ier = _stub->unaryRPC(_context.get(), request, &response);

    if (ier.ok())
    {
//        // 3. Access Initial Server Metadata
//        const auto& initMetadata =  context->GetServerInitialMetadata();
//        std::cout << "Client received initial metadata from server: " << std::endl;
//        std::cout << initMetadata.find("server-initial-metadata-key1")->second << std::endl;
//        std::cout << initMetadata.find("server-initial-metadata-key2")->second << std::endl;

        // 4. Access Trailing Server Metadata
        const auto& trailingMetadata =  _context->GetServerTrailingMetadata();
        std::cout << "Client received trailing metadata from server: " << std::endl;
        std::cout << trailingMetadata.find("server-trailing-metadata-key1")->second << std::endl;
        std::cout << trailingMetadata.find("server-trailing-metadata-key2")->second << std::endl;

        // 5. Access RPC Message
        std::cout<<response.bar()<<std::endl;

        return;
    }

    std::cout << ier.error_code() << ": " << ier.error_message();
}


