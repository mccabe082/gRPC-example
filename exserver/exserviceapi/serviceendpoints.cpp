#include "serviceendpoints.h"
#include <string>
#include <iostream>

namespace ExServer
{
    grpc::Status ServiceEndPoints::unaryRPC(grpc::ServerContext* context, const RequestMsgEx* request, ResponseMsgEx* reply)
    {
        // 1. Read client metadata
        using ClientMetadata = const std::multimap<grpc::string_ref, grpc::string_ref>;
        ClientMetadata clientDat = context->client_metadata();
        for (auto pair : clientDat)
        {
            std::cout << "Server received Client Metadata:" << std::endl;
            std::cout << "Header key: " << pair.first << ", value: " << pair.second << std::endl;
            std::cout << std::endl;
        }

        // 2. Write server initial metadata
        context->AddInitialMetadata("server-initial-metadata-key1","server initial metadata value1");
        context->AddInitialMetadata("server-initial-metadata-key2","server initial metadata value2");

        // 3. Write server trailing metadata
        context->AddTrailingMetadata("server-trailing-metadata-key1","server trailing metadata value1");
        context->AddTrailingMetadata("server-trailing-metadata-key2","server trailing metadata value2");

        // 4. Read request/write response
        const std::string msg = request->foo();
        std::cout << "Server received request from Client:" << std::endl;
        std::cout << "\t...["<< msg << "]" << std::endl;
        std::cout << std::endl;
        reply->set_bar("pong");

        // Return code
        return grpc::Status::OK;
    }

    grpc::Status ServiceEndPoints::serverStreamingRPC(grpc::ServerContext* context, const RequestMsgEx* request, grpc::ServerWriter<ResponseMsgEx>* writer)
    {
        std::cout << "\nServer streaming to Client:" << std::endl;
        int i = 0;
        while (i<10)
        {
            ResponseMsgEx response;
            response.set_bar(std::to_string(++i));
            writer->Write(response);
        }
        std::cout << "\t...server streaming complete" << std::endl;

        return grpc::Status::OK;
    }

    grpc::Status ServiceEndPoints::clientStreamingRPC(grpc::ServerContext* context, grpc::ServerReader<RequestMsgEx>* reader, ResponseMsgEx* response)
    {
        std::cout << "\nServer receiving stream from Client:\n\t" << std::flush;
        static int i = 0;
        RequestMsgEx request;
        while (reader->Read(&request))
        {
            std::cout << request.foo() << ".";
        }
        response->set_bar(std::to_string(++i));
        std::cout << "\n\t...client streaming complete\n\n" << std::flush;

        return grpc::Status::OK;
    }

    grpc::Status ServiceEndPoints::bidirectionalStreamingRPC(grpc::ServerContext* context, grpc::ServerReaderWriter<ResponseMsgEx, RequestMsgEx>* stream)
    {
        std::cout << "\nServer receiving/sending stream from/to Client:\n\t" << std::flush;
        int i = 0;
        RequestMsgEx request;
        while (stream->Read(&request))
        {
            ResponseMsgEx response;
            response.set_bar(std::to_string(++i));
            stream->Write(response);
        }
        std::cout << "\n\t...streaming complete\n\n" << std::flush;

        return grpc::Status::OK;
    }
}