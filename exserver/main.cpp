#include "server.h"

const std::string SERVER_ADDRESS = "0.0.0.0:50051";

int main()
{
    ExServer::Server s(SERVER_ADDRESS);
    s.run();

    return 0;
}
