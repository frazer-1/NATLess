#include "UDPServer.h"
#include <iostream>

void main(int argc, char** argv)
{
    try
    {
        UDPServer udpServer;
        udpServer.Run();
    }
    catch (Poco::Exception& exc)
    {
        std::cout << "ERROR: " << exc.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "ERROR: Unexpected error" << std::endl;
    }
}