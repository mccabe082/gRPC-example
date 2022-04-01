#include "clientex.h"

int main()
{
    ClientEx s(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    s.demonstrateRemoteProcedureCall();
    return 0;
}