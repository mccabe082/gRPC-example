#include "serviceimpl.h"
#include <string>
#include <thread>
#include <future>
#include <iostream>

namespace
{
    const std::string SERVER_ADDRESS = "0.0.0.0:50051";

    std::unique_ptr<grpc::ServerContext> createContext()
    {
        // https://grpc.github.io/grpc/cpp/classgrpc_1_1_server_context.html
        // https://grpc.io/docs/what-is-grpc/core-concepts/#metadata

        auto c = std::make_unique<grpc::ServerContext>();
        c->set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
        return c;
    }

    void configureServer(grpc::ServerBuilder& b)
    {
        // https://grpc.github.io/grpc/cpp/classgrpc_1_1_server_builder.html
        b.AddListeningPort(SERVER_ADDRESS, grpc::InsecureServerCredentials());
        b.SetDefaultCompressionAlgorithm(GRPC_COMPRESS_GZIP);
    }
}

namespace ServiceSuiteEx
{
    grpc::Status ServiceImpl::unaryRPC(grpc::ServerContext* context, const RequestMsgEx* request, ResponseMsgEx* reply)
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

    grpc::Status ServiceImpl::serverStreamingRPC(grpc::ServerContext* context, const RequestMsgEx* request, grpc::ServerWriter<ResponseMsgEx>* writer)
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

    grpc::Status ServiceImpl::clientStreamingRPC(grpc::ServerContext* context, grpc::ServerReader<RequestMsgEx>* reader, ResponseMsgEx* response)
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

    grpc::Status ServiceImpl::bidirectionalStreamingRPC(grpc::ServerContext* context, grpc::ServerReaderWriter<ResponseMsgEx, StreamControl>* stream)
    {
        std::cout << "\nServer receiving/sending stream from/to Client:\n\t" << std::flush;

        int i = 0;
        ResponseMsgEx response;
        StreamControl request;

        std::mutex dumpFrames;  // synchronise client and server

        bool keepStreaming = false;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        auto cameraSim = [&](){
            // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
            auto cameraCallback = [&]() {
                if(dumpFrames.try_lock()) {
                    response.set_bar(std::to_string(++i));
                    stream->Write(response);
                }
                std::cout << "." << keepStreaming<< std::flush;

            };
            // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
            while (keepStreaming)
            {
                std::this_thread::sleep_for(std::chrono::microseconds (1000));
                cameraCallback();
            }
        };
        std::unique_ptr<std::thread> t;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        while (stream->Read(&request))
        {
            switch(request.value())
            {
                case StreamControl_ControlFlag_START_STREAMING:
                {
                    std::cout << "\t - received start streaming command- \n\t" << std::flush;
                    keepStreaming = true;
                    std::unique_lock<std::mutex> lock(dumpFrames);
                    t = std::make_unique<std::thread>(cameraSim);
                    break;
                }
                case StreamControl_ControlFlag_ACKNOWLEDGEMENT:
                {
                    std::cout << "\t - received frame acknowledgement- \n\t" << std::flush;
                    dumpFrames.unlock();
                    break;
                }
                case StreamControl_ControlFlag_STOP_STREAMING:
                {
                    std::cout << "\t - received stop streaming command- \n\t" << std::flush;
                    keepStreaming = false;
                    dumpFrames.unlock();
                    std::unique_lock<std::mutex> lock(dumpFrames);
                    if(t->joinable()) t->join();
                    std::cout << "\n\t...streaming complete\n\n" << std::flush;
                    return grpc::Status::OK;
                }
                default:
                {
                    std::cout << "\t - weird - \n\t" << std::flush;
                    break;
                }
            }
        }

        return grpc::Status(grpc::UNKNOWN,"unspecified error");
    }

    void ServiceImpl::run()
    {
        ServiceImpl serviceInstance;
        grpc::EnableDefaultHealthCheckService(true);
        //grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        grpc::ServerBuilder builder;
        configureServer(builder);
        builder.RegisterService(&serviceInstance);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << SERVER_ADDRESS << std::endl;
        server->Wait();
    }
}