#include "clientex.h"
int main()
{
    ClientEx s;

//    s.callUnaryRPCWithMetaData();
    s.callUnaryRPCWithMetaData_ASyncClient();
//    s.callServerStreamingRPC();
//
//    s.callClientStreamingRPC();
//
//    s.callBidirectionalStreamingRPC();

    return 0;
}