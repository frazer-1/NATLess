#pragma once
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"

class UDPServer
{
public:
    UDPServer();
    ~UDPServer();

    void Run();

private:
    Poco::Net::SocketAddress serverAddress;
    Poco::Net::DatagramSocket datagramSocket;
};

